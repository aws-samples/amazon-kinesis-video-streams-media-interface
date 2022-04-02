#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/prctl.h>
#include <libfhosl/include/list.h>
#include <libpes/include/libpes.h>

#define MIN_PES_HEAD_STUFF_LEN    2
#define MAX_PES_PACKET_LEN      (65496 & (~3)) /*ensure 4 bytes aligned*/
#define MAX_BYTE_RATE       (256*1024)
#define FH_PES_THREAD_PRORITY (130)

typedef struct _PES_PACKET_INFO
{
    unsigned int    stream_id;
    unsigned int    add_pts;
    unsigned int    total_data_len;
    unsigned int    rest_data_len;
    unsigned int    payload_len;
} PES_PACK_INFO;


/* 数据块处理参数 */
typedef struct _PSMUX_UNIT_
{
    unsigned int    error_code;
    unsigned int    total_len;

    unsigned int    enc_type;
    unsigned int    frame_type;         /* 输入帧类型                           */
    unsigned int    is_first_unit;      /* 是否是一帧的第一个unit。标准H.264每帧会分成多个unit*/
    unsigned int    is_last_unit;       /* 是否是一帧的最后一个unit       */
    unsigned int    is_key_frame;       /* 是否关键数据(I帧)                    */
    unsigned int    is_unit_start;      /* 若是一个nalu或一帧的第一段数据，则置1，若送进的是完整的一帧或一个nalu也置1*/
    unsigned int    is_unit_end;        /* 若是一个nalu或一帧的最后一段数据，则置1，若送进的是完整的一帧或一个nalu也置1*/
    unsigned long long    sys_clk_ref;  /* 系统参考时钟，以 1/45000 秒为单位    */
    unsigned long long    ptime_stamp;  /* 该帧在接收端的显示时标，单位同上     */
    unsigned int    frame_num;          /* 当前帧号       */
    unsigned char *unit_in_buf;        /* 输入 unit 指针 */
    unsigned int    unit_in_len;        /* 输出 unit 长度 */

    unsigned int    encrypt;            /* 是否加密                             */

} PSMUX_PROCESS_PARAM;


/* video stream type */
#define STREAM_TYPE_VIDEO_H264      0x1b    /* 标准H264       */
#define STREAM_TYPE_VIDEO_H265      0x24

/* 输入帧类型，对应frame_type */
#define FRAME_TYPE_VIDEO_IFRAME         3       /* 视频数据 I 帧 */
#define FRAME_TYPE_VIDEO_PFRAME         1       /* 视频数据 P 帧 */
#define FRAME_TYPE_VIDEO_BFRAME         0       /* 视频数据 B 帧 */

#define STUFF_BYTE_0XFF         0xff
#define PROGRAM_STREAM_PSH_TAG  0xba
#define PROGRAM_STREAM_PSM_TAG  0xbc
#define PES_VIDEO_BASE          0xe0

/*
 * In vlc-3.0.3 modules/access/udp.c Open() fuction line 171:
 *
 *     sys->mtu = 7 * 188;
 *
 * 7 * 188 = 1316, the maximum Transport Stream units within MTU(1500),
 * 188 is the packet size of Transport Stream unit.
 *
 * vlc uses the same udp module to process Program Stream and Transport Stream.
 *
 * If the send size is greater than 1316, vlc will truncate the data and then
 * it cannot decode the picture.
 */
#define TS_UNITS_MAX_SIZE (1316)

/*the max channel supported*/
#define VLC_CHANNEL_MAX (8)

/*the max packets num on g_free_ts_list*/
#define FREE_CACHED_TS_NUM_MAX (800)

struct ts_units
{
    struct list_head list; /*it must be the first one*/

    char buf[TS_UNITS_MAX_SIZE];
    int len; /*the real data length*/
};

struct vlcview_info
{
    struct list_head ts_list;
    unsigned int       ts_list_pkt_num;


    unsigned int configured;
    unsigned int frame_num;
    unsigned int memory_limit;
    struct sockaddr_in target_addr;
};

static volatile int  g_vlc_lock_inited;
static pthread_mutex_t g_vlc_lock;

static volatile int  g_stop_request;
static volatile int  g_this_module_inited;

static struct list_head g_swap_ts_list;
static struct ts_units *g_swap_ts_pkt;

static int              g_free_ts_pkt_num;
static struct list_head g_free_ts_list; /*the free ts list*/

static struct vlcview_info g_vlc_channel_info[VLC_CHANNEL_MAX];

static void *copy_noncached_stream(char *dst, const char *src, int count)
{
    char *dst_bak = dst;

    while (((unsigned int)src & 3) && count > 0)
    {
        *(dst++) = *(src++);
        count--;
    }

    while (count >= 4)
    {
        *((int *)dst) =  *((int *)src);
        dst += 4;
        src += 4;
        count -= 4;
    }

    while (count > 0)
    {
        *(dst++) = *(src++);
        count--;
    }

    return dst_bak;
}

static struct ts_units *alloc_ts_pkt(void)
{
    struct ts_units *pkt;
    struct list_head *node;

    if (!list_empty(&g_free_ts_list))
    {
        node = g_free_ts_list.next;
        list_del(node);
        g_free_ts_pkt_num--;
        ((struct ts_units *)node)->len = 0;
        return (struct ts_units *)node;
    }

    pkt = (struct ts_units *)malloc(sizeof(struct ts_units));
    if (pkt)
    {
        pkt->len = 0;
    }
    return pkt;
}

static void free_ts_pkt(struct ts_units *pkt)
{
    if (g_free_ts_pkt_num < FREE_CACHED_TS_NUM_MAX)
    {
        list_add_tail(&pkt->list, &g_free_ts_list);
        g_free_ts_pkt_num++;
    }
    else
    {
        free(pkt);
    }
}

static int ts_add_data(char *data, int len)
{
    int nbytes;

    while (len > 0)
    {
        if (!g_swap_ts_pkt)
        {
            g_swap_ts_pkt = alloc_ts_pkt();
            if (!g_swap_ts_pkt)
            {
                printf("Error: no memory for libpes!\n");
                return -1;
            }
        }

        nbytes = TS_UNITS_MAX_SIZE - g_swap_ts_pkt->len;
        if (nbytes > len)
            nbytes = len;

        /* copy_noncached_stream(g_swap_ts_pkt->buf + g_swap_ts_pkt->len, data, nbytes); */
        memcpy(g_swap_ts_pkt->buf + g_swap_ts_pkt->len, data, nbytes);
        g_swap_ts_pkt->len += nbytes;
        if (g_swap_ts_pkt->len >= TS_UNITS_MAX_SIZE)
        {
            list_add_tail(&g_swap_ts_pkt->list, &g_swap_ts_list);
            g_swap_ts_pkt = NULL;
        }

        data += nbytes;
        len -= nbytes;
    }

    return 0; /*success*/
}

static void ts_add_data_end(struct vlcview_info *info)
{
    struct list_head *pos;
    struct list_head *n;
    struct list_head *head = &g_swap_ts_list;

    if (g_swap_ts_pkt)
    {
        if (g_swap_ts_pkt->len > 0)
            list_add_tail(&g_swap_ts_pkt->list, head);
        else
            free_ts_pkt(g_swap_ts_pkt);
        g_swap_ts_pkt = NULL;
    }

    list_for_each_safe(pos, n, head)
    {
        list_add_tail(pos, &info->ts_list);
        info->ts_list_pkt_num++;
    }

    INIT_LIST_HEAD(&g_swap_ts_list);
}

static void ts_add_data_abandom(struct vlcview_info *info)
{
    struct list_head *head = &g_swap_ts_list;
    struct list_head *pos;
    struct list_head *n;

    if (g_swap_ts_pkt)
    {
        free_ts_pkt(g_swap_ts_pkt);
        g_swap_ts_pkt = NULL;
    }

    list_for_each_safe(pos, n, head)
    {
        list_del(pos);
        free_ts_pkt((struct ts_units *)pos);
    }
}

static struct ts_units *ts_get(struct vlcview_info *info)
{
    struct list_head *node;

    if (info->configured && !list_empty(&info->ts_list))
    {
        node = info->ts_list.next;
        list_del(node);
        info->ts_list_pkt_num--;
        return (struct ts_units *)node;
    }

    return NULL;
}

static void channel_info_reset(struct vlcview_info *info)
{
    struct list_head *head;
    struct list_head *pos;
    struct list_head *n;

    if (!info->ts_list.next)
    {
        INIT_LIST_HEAD(&info->ts_list);
    }
    else
    {
        head = &info->ts_list;
        list_for_each_safe(pos, n, head)
        {
            list_del(pos);
            free(pos);
        }
    }

    info->ts_list_pkt_num = 0;
    info->frame_num = 0;
    info->memory_limit = 0;
    info->configured = 0;
}

/* 符合mpeg2 part1定义的crc32码表 */
static unsigned int PSMUX_CTX[257] = {

    0x00000000,0xb71dc104,0x6e3b8209,0xd926430d,0xdc760413,0x6b6bc517,0xb24d861a,0x0550471e,
    0xb8ed0826,0x0ff0c922,0xd6d68a2f,0x61cb4b2b,0x649b0c35,0xd386cd31,0x0aa08e3c,0xbdbd4f38,
    0x70db114c,0xc7c6d048,0x1ee09345,0xa9fd5241,0xacad155f,0x1bb0d45b,0xc2969756,0x758b5652,
    0xc836196a,0x7f2bd86e,0xa60d9b63,0x11105a67,0x14401d79,0xa35ddc7d,0x7a7b9f70,0xcd665e74,
    0xe0b62398,0x57abe29c,0x8e8da191,0x39906095,0x3cc0278b,0x8bdde68f,0x52fba582,0xe5e66486,
    0x585b2bbe,0xef46eaba,0x3660a9b7,0x817d68b3,0x842d2fad,0x3330eea9,0xea16ada4,0x5d0b6ca0,
    0x906d32d4,0x2770f3d0,0xfe56b0dd,0x494b71d9,0x4c1b36c7,0xfb06f7c3,0x2220b4ce,0x953d75ca,
    0x28803af2,0x9f9dfbf6,0x46bbb8fb,0xf1a679ff,0xf4f63ee1,0x43ebffe5,0x9acdbce8,0x2dd07dec,
    0x77708634,0xc06d4730,0x194b043d,0xae56c539,0xab068227,0x1c1b4323,0xc53d002e,0x7220c12a,
    0xcf9d8e12,0x78804f16,0xa1a60c1b,0x16bbcd1f,0x13eb8a01,0xa4f64b05,0x7dd00808,0xcacdc90c,
    0x07ab9778,0xb0b6567c,0x69901571,0xde8dd475,0xdbdd936b,0x6cc0526f,0xb5e61162,0x02fbd066,
    0xbf469f5e,0x085b5e5a,0xd17d1d57,0x6660dc53,0x63309b4d,0xd42d5a49,0x0d0b1944,0xba16d840,
    0x97c6a5ac,0x20db64a8,0xf9fd27a5,0x4ee0e6a1,0x4bb0a1bf,0xfcad60bb,0x258b23b6,0x9296e2b2,
    0x2f2bad8a,0x98366c8e,0x41102f83,0xf60dee87,0xf35da999,0x4440689d,0x9d662b90,0x2a7bea94,
    0xe71db4e0,0x500075e4,0x892636e9,0x3e3bf7ed,0x3b6bb0f3,0x8c7671f7,0x555032fa,0xe24df3fe,
    0x5ff0bcc6,0xe8ed7dc2,0x31cb3ecf,0x86d6ffcb,0x8386b8d5,0x349b79d1,0xedbd3adc,0x5aa0fbd8,
    0xeee00c69,0x59fdcd6d,0x80db8e60,0x37c64f64,0x3296087a,0x858bc97e,0x5cad8a73,0xebb04b77,
    0x560d044f,0xe110c54b,0x38368646,0x8f2b4742,0x8a7b005c,0x3d66c158,0xe4408255,0x535d4351,
    0x9e3b1d25,0x2926dc21,0xf0009f2c,0x471d5e28,0x424d1936,0xf550d832,0x2c769b3f,0x9b6b5a3b,
    0x26d61503,0x91cbd407,0x48ed970a,0xfff0560e,0xfaa01110,0x4dbdd014,0x949b9319,0x2386521d,
    0x0e562ff1,0xb94beef5,0x606dadf8,0xd7706cfc,0xd2202be2,0x653deae6,0xbc1ba9eb,0x0b0668ef,
    0xb6bb27d7,0x01a6e6d3,0xd880a5de,0x6f9d64da,0x6acd23c4,0xddd0e2c0,0x04f6a1cd,0xb3eb60c9,
    0x7e8d3ebd,0xc990ffb9,0x10b6bcb4,0xa7ab7db0,0xa2fb3aae,0x15e6fbaa,0xccc0b8a7,0x7bdd79a3,
    0xc660369b,0x717df79f,0xa85bb492,0x1f467596,0x1a163288,0xad0bf38c,0x742db081,0xc3307185,
    0x99908a5d,0x2e8d4b59,0xf7ab0854,0x40b6c950,0x45e68e4e,0xf2fb4f4a,0x2bdd0c47,0x9cc0cd43,
    0x217d827b,0x9660437f,0x4f460072,0xf85bc176,0xfd0b8668,0x4a16476c,0x93300461,0x242dc565,
    0xe94b9b11,0x5e565a15,0x87701918,0x306dd81c,0x353d9f02,0x82205e06,0x5b061d0b,0xec1bdc0f,
    0x51a69337,0xe6bb5233,0x3f9d113e,0x8880d03a,0x8dd09724,0x3acd5620,0xe3eb152d,0x54f6d429,
    0x7926a9c5,0xce3b68c1,0x171d2bcc,0xa000eac8,0xa550add6,0x124d6cd2,0xcb6b2fdf,0x7c76eedb,
    0xc1cba1e3,0x76d660e7,0xaff023ea,0x18ede2ee,0x1dbda5f0,0xaaa064f4,0x738627f9,0xc49be6fd,
    0x09fdb889,0xbee0798d,0x67c63a80,0xd0dbfb84,0xd58bbc9a,0x62967d9e,0xbbb03e93,0x0cadff97,
    0xb110b0af,0x060d71ab,0xdf2b32a6,0x6836f3a2,0x6d66b4bc,0xda7b75b8,0x035d36b5,0xb440f7b1,
    0x00000001
};

/******************************************************************************
 * 功  能：生成crc32校验码，参考ffmpeg
 * 参  数：  buffer  - 生成crc码的缓冲区
 *           length  - 生成crc码的缓冲区长度
 * 返回值：crc校验码
 ******************************************************************************/
static unsigned int PSMUX_mpeg2_crc(const unsigned char *buffer, unsigned int length)
{
    const unsigned char *end= buffer + length;
    unsigned int crc = 0xffffffff;

    while (buffer < end)
    {
        crc = PSMUX_CTX[((unsigned char)crc) ^ *buffer++] ^ (crc >> 8);
    }
    return crc;
}

/******************************************************************************
 * 功  能：根据 PES_PACK_INFO *pes 内的信息在 buffer 里生成 PES 头数据
 * 参  数：  buffer      - PES 头缓冲区
 *           pes         - 当前处理的pes
 *           prc         - 当前处理的单元
 *           pes         - 输入输出参数结构体
 * 返回值：PES 头数据的长度
 ******************************************************************************/
static void PSMUX_create_pes_header(PSMUX_PROCESS_PARAM *param, PES_PACK_INFO *pes)
{
    int             i, pes_header_stuff_len = 0;
    unsigned int    tmp;
    unsigned int    pes_header_basic_len    = 9;
    unsigned int    pes_header_ext_len      = 0;
    unsigned int    pos, stuff_flag = 0xff;
    unsigned int    pes_header_len_syt;
    unsigned int    pes_heder_fix_len;
    unsigned int    pes_priority = (param->frame_type != FRAME_TYPE_VIDEO_BFRAME);
    unsigned long long pts;
    unsigned int max_pes_len = MAX_PES_PACKET_LEN;
    unsigned char buffer[32]; /*it should be enough*/

    if ((pes->rest_data_len == pes->total_data_len) && param->is_unit_start)
    {
        stuff_flag  &= 0xfd;
    }

    /* 计算 pes 头字节和填充字节的长度 */
    if (pes->add_pts)
    {
        pes_header_ext_len += 5;
    }
    pes_heder_fix_len = pes_header_basic_len + pes_header_ext_len + MIN_PES_HEAD_STUFF_LEN;
    tmp = pes_heder_fix_len + pes->rest_data_len;

    if (tmp > max_pes_len)
    {
        pes_header_stuff_len = 2;
        pes_header_ext_len   +=  pes_header_stuff_len;
        pes->payload_len     =   max_pes_len - pes_header_basic_len - pes_header_ext_len;
    }
    else
    {
        /* pes_header_stuff_len 至少为2，且要保证 pes 包 4 byte 对齐 */
        if ((tmp == max_pes_len) || ((tmp & 3) == 0))
        {
            pes_header_stuff_len = 2;
        }
        else
        {
            pes_header_stuff_len = 2 + (4 - (tmp & 3));
        }
        pes_header_ext_len   +=  pes_header_stuff_len;
        pes->payload_len     =   pes->rest_data_len;

        if (param->is_unit_end)
        {
            stuff_flag &= 0xfe;
        }

        if (param->is_last_unit && param->is_unit_end)
        {
            stuff_flag &= 0xfb;
        }
    }

    pes_header_len_syt = pes_header_basic_len + pes_header_ext_len + pes->payload_len - 6;

    /* 生成 pes 头数据 */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x01;
    buffer[3] = (unsigned char)(pes->stream_id);

    buffer[4] = (unsigned char)((pes_header_len_syt >> 8) & 0xff);
    buffer[5] = (unsigned char)(pes_header_len_syt & 0xff);

    buffer[6] =   (1<<7)/* 2 - check bits '10' */
        | ((param->encrypt & 0x03) << 4)/* 2 - PES_scrambling_control(0) */
        | ((pes_priority & 1) << 3)/* 1 - PES_priority(0) */
        | (((~stuff_flag) << 1) & 0x04)     /* 1 - data_alignment_indicator(0) */
        | 0                                 /* 1 - copyright(0) */
        | 0;                                /* 1 - original_or_copy(0) */

    buffer[7] = (((unsigned char)pes->add_pts) << 7);
    /* 2 - PTS_DTS_flags() */
    /* 1 - ESCR_flag(0) */
    /* 1 - ES_rate_flag(0) */
    /* 1 - DSM_trick_mode_flag(0) */
    /* 1 - additional_copy_info_flag(0) */
    /* 1 - PES_CRC_flag(0) */
    /* 1 - PES_extension_flag() */

    buffer[8] = (unsigned char)pes_header_ext_len;    /* 8 - PES_header_data_length */

    pos = 9;
    if (pes->add_pts)
    {
        pts = param->ptime_stamp;
        buffer[pos++] = (pts >> 29 & 0x0e) | 0x21;        /* 4 - '0010' */
        /* 3 - PTS [32..30] */
        /* 1 - marker_bit */
        buffer[pos++] = (pts >> 22);                    /* 8 - PTS [29..22] */
        buffer[pos++] = (pts >> 14 & 0xfe) | 0x01;        /* 7 - PTS [21..15] */
        /* 1 - marker_bit */
        buffer[pos++] = (pts >>  7);                    /* 8 - PTS [14..7] */
        buffer[pos++] = (pts << 1  & 0xfe) | 0x01;        /* 7 - PTS [6..0] */
        /* 1 - marker_bit */
    }

    if (1/*prg->set_frame_end_flg*/)
    {
        for (i = 0; i < pes_header_stuff_len - 1; i++)
        {
            buffer[pos++] = STUFF_BYTE_0XFF;
        }
        buffer[pos++] = (unsigned char)stuff_flag;
    }
    else
    {
        for (i = 0; i < pes_header_stuff_len; i++)
        {
            buffer[pos++] = STUFF_BYTE_0XFF;
        }
    }

    param->error_code |= ts_add_data((char *)buffer, pos);
    param->total_len += pos;
}


/******************************************************************************
 * 功  能：根据 byte_rate 和 scr 信息在 buffer 里生成 PS 头数据
 * 参  数：  buffer      - PS 头缓冲区
 *           byte_rate   - PS流码率
 *           scr         - 系统参考时钟
 *            frame_num    - 帧号
 * 返回值：PS 头数据的长度
 ******************************************************************************/
static void PSMUX_fill_PSH(PSMUX_PROCESS_PARAM *param)
{
    int pos = 0;
    unsigned int mux_rate;
    unsigned int stream_num   = 4;
    unsigned char  buffer[64]; /*it should be enough*/

    mux_rate = MAX_BYTE_RATE / 50 + 1;

    buffer[pos++]    = 0x00;
    buffer[pos++]    = 0x00;
    buffer[pos++]    = 0x01;
    buffer[pos++]    = PROGRAM_STREAM_PSH_TAG;            /* 32bits    0x 00 00 01 BA */

    buffer[pos++]    = 0x40                                /* 2bits    MPEG2标志 "01" */
        | ((param->sys_clk_ref>>27) & 0x38)     /* 3bits    system_clock_reference_base [32..30] */
        | 0x04                                /* 1bits    marker_bit */
        | ((param->sys_clk_ref>>28) & 0x03);    /* 2bits    system_clock_reference_base [29..28] */
    buffer[pos++]    = (param->sys_clk_ref>>20) & 0xff;    /* 8bits    system_clock_reference_base [27..20] */
    buffer[pos++]    = ((param->sys_clk_ref>>12) & 0xf8)    /* 5bits    system_clock_reference_base [19..15] */
        | 0x04                                /* 1bits    marker_bit */
        | ((param->sys_clk_ref>>13) & 0x03);    /* 2bits    system_clock_reference_base [14..13] */
    buffer[pos++]    = (param->sys_clk_ref>>5) & 0xff;        /* 8bits    system_clock_reference_base [12..5] */
    buffer[pos++]    = ((param->sys_clk_ref<<3) & 0xf0)    /* 5bits    system_clock_reference_base [4..0] */
        | 0x04                    /* 1bits    marker_bit */
        | 0x00;                    /* 2bits    system_clock_reference_ext [8..7] */
    buffer[pos++]    = 0x01;                    /* 7bits    system_clock_reference_ext [6..0] */
    /* 1bits    marker_bit */
    buffer[pos++]    = (mux_rate>>14) & 0xff;/* 8bits    mux_rate [21..14] */
    buffer[pos++]    = (mux_rate>>6) & 0xff;    /* 8bits    mux_rate [13..6] */
    buffer[pos++]    = (mux_rate<<2)            /* 6bits    mux_rate [5..0] */
        | 0x03;                    /* 2bits    marker_bit */

    buffer[pos++]    = 0xFE;                    /* 5bits    reserved, 3bits stuffing_len = 6; */
    buffer[pos++]    = STUFF_BYTE_0XFF;        /* stuffing_byte */
    buffer[pos++]    = STUFF_BYTE_0XFF;        /* stuffing_byte */

    if (1/*prg->set_frame_end_flg*/)
    {
        buffer[pos++]    = (param->frame_num >> 24) & 0xff;
        buffer[pos++]    = (param->frame_num >> 16) & 0xff;
        buffer[pos++]    = (param->frame_num >> 8) & 0xff;
        buffer[pos++]    = param->frame_num & 0xff;
    }
    else
    {
        buffer[pos++]    = STUFF_BYTE_0XFF;
        buffer[pos++]    = STUFF_BYTE_0XFF;
        buffer[pos++]    = STUFF_BYTE_0XFF;
        buffer[pos++]    = STUFF_BYTE_0XFF;
    }

    if (param->is_key_frame)
    {
        /* system header */
        buffer[pos++] = 0x00;
        buffer[pos++] = 0x00;
        buffer[pos++] = 0x01;
        buffer[pos++] = 0xbb;                                /* 32bits 0x000001BB */

        buffer[pos++] = (6 + 3 * stream_num) >> 8;            /* 8bits    header_length_high_8_bits */
        buffer[pos++] = (6 + 3 * stream_num) & 0xff;        /* 8bits    header_length_low_8_bits */

        buffer[pos++] = 0x80                                /* 1bits marker_bit */
            |((mux_rate >> 15) & 0x7f);           /* 7bits    rate_bound [21..15] */
        buffer[pos++] = (mux_rate >> 7) & 0xff;                /* 8bits    rate_bound [14..7] */
        buffer[pos++] = ((mux_rate << 1) & 0xfe)            /* 7bits    rate_bound [6..0] */
            |0x01;                                /* 1bits marker_bit */

        buffer[pos++] = 0x04                                /* 6bits    audio_bound */
            |0x00                                    /* 1bits    fixed_flag */
            |0x00;                                /* 1bits    csps_flag */
        buffer[pos++] = 0x80                                /* 1bits    sys_audio_lock_flag */
            |0x40                                    /* 1bits    sys_video_lock_flag */
            |0x20                                    /* 1bits    marker_bit */
            |0x01;                                /* 5bits    video_bound */
        buffer[pos++] = 0x00                                /* 1bits packet_rate_restriction_flag */
            |0x7f;                                /* 7bits    reserved */

        buffer[pos++] = 0xe0;                               /* stream_id */
        buffer[pos++] = (0xc0                               /* 2 bits "11" */
                | 0x20);                              /* 1 bit P-STD_buffer_bound_scale, video must '1' */
        buffer[pos++] = 0x80;                               /* 13 bits P-STD_buffer_size_bound, if bound_scale is '0', unit is 128 bytes, or unit is 1024 bytes */

        buffer[pos++] = 0xc0;                               /* stream_id */
        buffer[pos++] = (0xc0                               /* 2 bits "11" */
                | 0x00);                              /* 1 bit P-STD_buffer_bound_scale, audio must '0' */
        buffer[pos++] = 0x08;                               /* 13 bits P-STD_buffer_size_bound, if bound_scale is '0', unit is 128 bytes, or unit is 1024 bytes */

        buffer[pos++] = 0xbd;                               /* stream_id */
        buffer[pos++] = (0xc0                               /* 2 bits "11" */
                | 0x20);                              /* 1 bit P-STD_buffer_bound_scale, video must '1' */
        buffer[pos++] = 0x80;                               /* 13 bits P-STD_buffer_size_bound, if bound_scale is '0', unit is 128 bytes, or unit is 1024 bytes */

        buffer[pos++] = 0xbf;                               /* stream_id */
        buffer[pos++] = (0xc0                               /* 2 bits "11" */
                | 0x20);                              /* 1 bit P-STD_buffer_bound_scale, video must '1' */
        buffer[pos++] = 0x80;                               /* 13 bits P-STD_buffer_size_bound, if bound_scale is '0', unit is 128 bytes, or unit is 1024 bytes */
    }

    param->error_code |= ts_add_data((char*)buffer, pos);
    param->total_len += pos;
}

/******************************************************************************
 * 功  能：根据 PROGRAM_INFO *prg 信息在 buffer 里生成 PSM 数据
 * 参  数：  buffer      - PSM 缓冲区
 *           prg         - 节目流参考信息
 *           prc         - 处理参数，主要用到全局时间
 * 返回值：PSM 数据的长度
 ******************************************************************************/
static void PSMUX_fill_PSM(PSMUX_PROCESS_PARAM *param)
{
    int pos;
    int tmp_pos;
    int tmp_len = 0;
    unsigned int crc;
    int len_pos;
    unsigned char buffer[64]; /*it should be enough*/

    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x01;
    buffer[3] = PROGRAM_STREAM_PSM_TAG;

    /* 暂时填充pes包头长度，后面修改1 */
    buffer[4] = STUFF_BYTE_0XFF;
    buffer[5] = STUFF_BYTE_0XFF;

    buffer[6] = 0xe0;
    buffer[7] = STUFF_BYTE_0XFF;

    buffer[8]   = (unsigned char)(tmp_len >> 8);
    buffer[9]   = (unsigned char)(tmp_len);
    pos         =   10;

    /* 暂时填充pes包头长度，后面修改2 */
    tmp_pos     = pos;
    buffer[pos++] = STUFF_BYTE_0XFF;
    buffer[pos++] = STUFF_BYTE_0XFF;

    buffer[pos++] = param->enc_type;// STREAM_TYPE_VIDEO_H264 //FIXME
    buffer[pos++] = PES_VIDEO_BASE; //FIXME
    len_pos = pos;
    tmp_len = 0;
    pos += 2;
    buffer[len_pos++] = tmp_len >> 8;
    buffer[len_pos]   = tmp_len;

    /* 暂时填充修改2 */
    buffer[tmp_pos]     = (unsigned char)((pos - tmp_pos - 2) >> 8);
    buffer[tmp_pos+1]   = (unsigned char)(pos - tmp_pos - 2);

    /* 暂时填充修改1 */
    buffer[4] = (pos - 2) >> 8;
    buffer[5] = pos - 2;

    crc = PSMUX_mpeg2_crc(buffer, pos);

    buffer[pos++] = (unsigned char)(crc >> 24);
    buffer[pos++] = (unsigned char)(crc >> 16);
    buffer[pos++] = (unsigned char)(crc >> 8);
    buffer[pos++] = (unsigned char)(crc);

    param->error_code |= ts_add_data((char *)buffer, pos);
    param->total_len += pos;
}

static void do_pack_nalu(PSMUX_PROCESS_PARAM *param)
{
    PES_PACK_INFO    pes;
    unsigned int    packed_len = 0;

    if (param->is_first_unit && param->is_unit_start)
    {
        /* 当输入为视频P帧或者关键帧时，添加PSH */
        PSMUX_fill_PSH(param);
        /* 当输入为关键帧时，添加PSM */
        if (param->is_key_frame)
        {
            PSMUX_fill_PSM(param);
        }
    }

    pes.stream_id = PES_VIDEO_BASE; //FIXME
    pes.add_pts         = (param->is_first_unit && param->is_unit_start);
    pes.total_data_len  = param->unit_in_len;
    pes.rest_data_len   = param->unit_in_len;
    pes.payload_len     = 0;

    /* 将输入数据打包 */
    while (packed_len < pes.total_data_len)
    {
        PSMUX_create_pes_header(param, &pes);
        param->error_code |= ts_add_data((char *)&param->unit_in_buf[packed_len], pes.payload_len);
        param->total_len += pes.payload_len;
        packed_len  += pes.payload_len;
        pes.add_pts         = 0;/* 除了第一个pes包外，其他包不添加pts和userdata */
        pes.rest_data_len    = pes.total_data_len - packed_len;
    }
}

static void _VLC_LOCK(void)
{
    if (!g_vlc_lock_inited)
    {
        g_vlc_lock_inited = 1;
        pthread_mutex_init(&g_vlc_lock, NULL);
    }
    pthread_mutex_lock(&g_vlc_lock);
}

#define _VLC_UNLOCK() pthread_mutex_unlock(&g_vlc_lock)

int libpes_uninit(void)
{
    int i;
    struct list_head *pos;
    struct list_head *n;

    static volatile int is_uniniting = 0;

Retry:
    _VLC_LOCK();
    if (is_uniniting)
    {
        _VLC_UNLOCK();
        usleep(20000);
        goto Retry;
    }

    is_uniniting = 1;

    if (g_this_module_inited)
    {
        g_stop_request = 1;
        while (g_stop_request)
        {
            _VLC_UNLOCK();
            usleep(20000);
            _VLC_LOCK();
        }

        for (i=0; i<VLC_CHANNEL_MAX; i++)
        {
            channel_info_reset(&g_vlc_channel_info[i]);
        }

        list_for_each_safe(pos, n, &g_free_ts_list)
        {
            list_del(pos);
            free(pos);
        }
        g_free_ts_pkt_num = 0;

        g_this_module_inited = 0;
    }

    is_uniniting = 0;
    _VLC_UNLOCK();

    return 0;
}

int libpes_send_to_vlc(int channel, char *ip, int port)
{
    struct vlcview_info *info;

    if ((unsigned int)channel >= VLC_CHANNEL_MAX || !ip || port <= 0 || (port>>16) != 0)
    {
        return -1;
    }

    info = &g_vlc_channel_info[channel];

    _VLC_LOCK();
    if (!g_this_module_inited || info->configured/*reconfigure is not allowed...*/)
    {
        _VLC_UNLOCK();
        return -1;
    }

    channel_info_reset(info);
    info->memory_limit = channel == 0 ? 0x400000/TS_UNITS_MAX_SIZE : 0x200000/TS_UNITS_MAX_SIZE;
    info->target_addr.sin_family = AF_INET;
    info->target_addr.sin_addr.s_addr = inet_addr(ip);
    info->target_addr.sin_port = htons(port);
    info->configured = 1;
    _VLC_UNLOCK();

    return 0;
}

int  libpes_stream_pack(int channel, struct vlcview_enc_stream_element stream_element)
{
    int i;
    int len;
    unsigned long long clock_90K;
    struct vlcview_info *info;
    char _zero[4] = {0, 0, 0, 0};
    PSMUX_PROCESS_PARAM psmux_param;

    if ((unsigned int)channel >= VLC_CHANNEL_MAX)
        return -1;

    info = &g_vlc_channel_info[channel];

    _VLC_LOCK();
    if (!info->configured)
        goto Error;

    info->frame_num++;
    clock_90K = stream_element.time_stamp * 9 /100;

    psmux_param.error_code = 0;
    psmux_param.total_len = 0;
    psmux_param.is_unit_start = 1;
    psmux_param.is_unit_end = 1;
    psmux_param.sys_clk_ref = clock_90K;
    psmux_param.ptime_stamp = clock_90K;
    psmux_param.encrypt = 0;
    psmux_param.frame_num = info->frame_num;
    if (stream_element.enc_type == VLCVIEW_ENC_H264)
    {
        psmux_param.enc_type = STREAM_TYPE_VIDEO_H264;
    }
    else if (stream_element.enc_type == VLCVIEW_ENC_H265)
    {
        psmux_param.enc_type = STREAM_TYPE_VIDEO_H265;
    }
    else
    {
        goto Error;
    }
    if (stream_element.frame_type == VLCVIEW_ENC_I_FRAME)
    {
        psmux_param.frame_type = FRAME_TYPE_VIDEO_IFRAME;
        psmux_param.is_key_frame = 1;
    }
    else if (stream_element.frame_type == VLCVIEW_ENC_P_FRAME)
    {
        psmux_param.frame_type = FRAME_TYPE_VIDEO_PFRAME;
        psmux_param.is_key_frame = 0;
    }
    else
    {
        goto Error;
    }

    for (len = 0, i = 0; i < stream_element.nalu_count; i++)
    {
        len += stream_element.nalu[i].len;
    }
    if (len/TS_UNITS_MAX_SIZE + info->ts_list_pkt_num > info->memory_limit)
    {
        printf("Warning: libpes send slow, drop one frame!\n");
        goto Error;
    }

    for (i = 0; i < stream_element.nalu_count; i++)
    {
        psmux_param.is_first_unit = (i == 0);
        psmux_param.is_last_unit = (i == (stream_element.nalu_count - 1));
        psmux_param.unit_in_buf = stream_element.nalu[i].start;
        psmux_param.unit_in_len = stream_element.nalu[i].len;
        do_pack_nalu(&psmux_param);
    }

    if (psmux_param.error_code == 0) /*no error*/
    {
        if (psmux_param.total_len & 3)
        {
            psmux_param.error_code |= ts_add_data(_zero, 4 - (psmux_param.total_len & 3));
            psmux_param.total_len += (4 - (psmux_param.total_len & 3));
        }
    }

    if (psmux_param.error_code == 0) /*no error*/
    {
        ts_add_data_end(info);
    }
    else
    {
        ts_add_data_abandom(info);
    }

    _VLC_UNLOCK();
    return 0;

Error:
    _VLC_UNLOCK();
    return -1;
}

// #ifdef __FH_USING_SRT__
// #include "libpes_srt.c"
// #else
// #include "libpes_udp.c"
// #endif

#ifndef __FH_USING_SRT__

static void *network_send_proc(void *arg)
{
    int i;
    int ret;
    int count;
    int fd = (int)arg;
    struct ts_units *pkt;

    prctl(PR_SET_NAME, "send_proc");

    _VLC_LOCK();
    while (!g_stop_request)
    {
        count = 0;
        for (i=0; !g_stop_request && i<VLC_CHANNEL_MAX; )
        {
            pkt = ts_get(&g_vlc_channel_info[i]);
            if (pkt)
            {
                _VLC_UNLOCK();
                ret = sendto(fd, pkt->buf,  pkt->len, 0, (struct sockaddr *)&g_vlc_channel_info[i].target_addr, sizeof(struct sockaddr_in));
                if (ret < 0)
                {
                    printf("ERROR: libpes send(%d) failed,%d\n", pkt->len, ret);
                }
                _VLC_LOCK();
                free_ts_pkt(pkt);
                count++;
            }
            else
            {
                i++;
            }
        }

        if (!count)
        {
            _VLC_UNLOCK();
            usleep(10000);
            _VLC_LOCK();
        }
    }

    _VLC_UNLOCK();

    close(fd);
    g_stop_request = 0;
    return NULL;
}

int libpes_init(void)
{
    pthread_attr_t attr;
    pthread_t thread;
    int fd;
    int i;

    _VLC_LOCK();
    if (!g_this_module_inited)
    {
        fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (fd < 0)
        {
            printf("Error: libpes failed with socket!\n");
            _VLC_UNLOCK();
            return -1;
        }
        g_stop_request = 0;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_attr_setstacksize(&attr, 3 * 1024);

#ifdef __RTTHREAD_OS__
        struct sched_param param;
        param.sched_priority = FH_PES_THREAD_PRORITY;
        pthread_attr_setschedparam(&attr, &param);
#endif
        if (pthread_create(&thread, &attr, network_send_proc, (void*)fd) != 0)
        {
            printf("Error: Create libpes thread failed!\n");
            _VLC_UNLOCK();
            return -1;
        }

        INIT_LIST_HEAD(&g_free_ts_list);
        INIT_LIST_HEAD(&g_swap_ts_list);
        for (i=0; i<VLC_CHANNEL_MAX; i++)
        {
            channel_info_reset(&g_vlc_channel_info[i]);
        }
        g_this_module_inited = 1;
    }
    _VLC_UNLOCK();

    return 0;
}
#endif
