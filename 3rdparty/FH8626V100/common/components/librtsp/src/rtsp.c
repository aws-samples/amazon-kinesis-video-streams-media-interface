#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <libfhosl/include/list.h>
#include "librtsp/include/librtsp.h"

/* rtp packet macros */
#ifndef TCP_MMSS
#define TCP_MMSS (1460)
#endif
#define RTP_PACKET_MAX_SIZE   (1472)

#define MAX_CLIENT_COUNT      (5)
#define REQUEST_READ_BUF_SIZE (512)
#define MAX_PKTS_PER_CLIENT   (2048)

#define MAX_SUPPORT_SERVER_NUM (4)

#define CLIENT_EXPIRE_TIME (10) /*10 seconds timeout*/

#define CLIENT_STOP_PLAY  (0)
#define CLIENT_START_PLAY (1)

#define FH_RTSP_THREAD_PRORITY 130

struct sdp_video_info
{
    char *sps;
    char *pps;
    char *vps;
    int sps_len;
    int pps_len;
    int vps_len;
};

struct sdp_info
{
    struct sdp_video_info video_config[2];
    unsigned int                video_curr;

    FH_MEDIA_FORMAT_TYPE audio_config;
    unsigned int audio_enable_flag;
    unsigned int video_enable_flag;
};

struct rtp_packet
{
    struct list_head list; /*it must be the first one*/
    char buf[RTP_PACKET_MAX_SIZE];
    int len;
};

struct rtp_info
{
    unsigned long  ssrc;
    unsigned short video_sequence_num;
    unsigned short audio_sequence_num;

    struct sdp_info sdp;
    int rtp_transport;
};

struct client_session
{
    char read_buf[REQUEST_READ_BUF_SIZE];
    char cseq[12];
    unsigned long session_num;
    unsigned int stream_id;
};

struct client_context
{
    struct list_head       client_list; /*所有client的双向链表*/
    struct list_head       cmdresp_list;/*等待发送的命令响应包的双向链表,该链表上的包需要优先发送*/
    struct list_head       data_list;   /*等待发送的数据包的双向链表*/
    int                    data_num;    /*链接在data_list上的数据包的个数*/
    struct rtp_packet     *curr_pkt;    /*当前正在发送的包*/
    int                    curr_pkt_sent_len; /*包curr_pkt已经发送的字节长度*/

    struct server_context *server;
    int                    status;
    int                    client_fd;
    int                    last_rw_time;
    unsigned char   ip[4];
    unsigned short  port;
    unsigned char   blocked;
    unsigned char   reserved;

    struct client_session  session;
};

struct rtsp_hook
{
    void (*hook)(void *);
    void *param;
};

struct server_context
{
    pthread_mutex_t    lock;        /*每个server使用一个锁*/
    int              lock_inited; /*标志lock是否已经初始化*/
    int              alloced;     /*由于该结构体是static的,因此用来标志是否已被占用*/

    struct list_head client_list; /*链表头,用来链接所有connect到该server的client*/
    struct list_head free_ptk_list;/*链表头,所有空闲的rtp_packet都挂在这里*/

    int              stop;         /*stop退出标志*/
    int              server_fd;    /*server fd for accept all the clients*/
    int              now_time;

    struct rtp_info  rtp_info;

    struct rtsp_hook rtsp_play_hook;
};

static struct server_context g_server_ctx_list[MAX_SUPPORT_SERVER_NUM];
static pthread_mutex_t g_server_ctx_lock;
static int g_server_ctx_lock_inited = 0;

static int send_client(struct client_context *client);

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

static int my_strcat(char *dst, char *src)
{
    char chr;
    char *dst_orig = dst;

    do
    {
        chr = *(src++);
        *(dst++) = chr;
    } while(chr);

    return (dst - dst_orig - 1);
}

static unsigned long my_strtol(char *str)
{
    unsigned long v = 0;

    while (*str >= '0' && *str <= 9)
    {
        v = v * 10 + (*str - '0');
        str++;
    }

    return v;
}

static int to_rtp_payload_type(int ac_config)
{
    int ac_type = 97;

    switch (ac_config)
    {
        case FH_SDP_G711A_8K:
            ac_type = 8;
            break;
        case FH_SDP_G711U_8K:
            ac_type = 0;
            break;
        case FH_SDP_G726_32K:
            ac_type = 2;
            break;
        case FH_SDP_G726_16K:
            ac_type = 98;
            break;
        default:
            break;
    }
    return ac_type;
}

static struct rtp_packet *alloc_packet(struct server_context *server)
{
    void *pkt;
    struct list_head *node;

    if (!list_empty(&server->free_ptk_list))
    {
        node = server->free_ptk_list.next;
        list_del(node);
        return (struct rtp_packet *)node;
    }

    pkt = malloc(sizeof(struct rtp_packet));
    if (!pkt)
    {
        printf("RTSP: out of memory, alloc_packet failed!\n");
    }

    return (struct rtp_packet *)pkt;
}

static void free_packet(struct server_context *server, struct rtp_packet *pkt)
{
    list_add_tail(&pkt->list, &server->free_ptk_list);
}

static struct rtp_packet *clone_packet(struct server_context *server, struct rtp_packet *pkt)
{
    struct rtp_packet *pkt_clone;

    pkt_clone = alloc_packet(server);
    if (pkt_clone)
    {
        pkt_clone->len = pkt->len;
        memcpy(pkt_clone->buf, pkt->buf, pkt->len);
    }

    return pkt_clone;
}

static void release_free_ptk_list(struct server_context *server)
{
    struct list_head *pos;
    struct list_head *next;
    struct rtp_packet *pkt;

    list_for_each_safe(pos, next, &server->free_ptk_list)
    {
        list_del(pos);
        pkt = list_entry(pos, struct rtp_packet, list);
        free(pkt);
    }
}

static void free_sdp_info(struct server_context *server)
{
    int i;
    struct sdp_video_info *video_config;

    for (i=0; i<2; i++)
    {
        video_config = &server->rtp_info.sdp.video_config[i];
        if (video_config->sps)
            free(video_config->sps);
        if (video_config->pps)
            free(video_config->pps);
        if (video_config->vps)
            free(video_config->vps);
    }
}

static int base64_encode(char *dst, char *src, int len)
{
    int count = 0;
    static const char *base64= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int chrbits = 0;
    int okbits = 0;
    int bits;
    unsigned char chr = 0;
    unsigned char ok = 0;

    while (len > 0 || chrbits > 0)
    {
        if (chrbits <= 0)
        {
            chr = *(src++);
            chrbits = 8;
            len--;
        }

        bits = 6 - okbits;
        if (chrbits < bits)
            bits = chrbits;

        ok = (ok << bits) | (chr >> (8-bits));
        chr <<= bits;
        chrbits -= bits;
        okbits += bits;
        if (okbits >= 6)
        {
            dst[count++] = base64[ok];
            ok = 0;
            okbits = 0;
        }
    }

    if (okbits > 0)
    {
        ok = ok << (6 - okbits);
        dst[count++] = base64[ok];
    }

    while (count & 3)
    {
        dst[count++] = '=';
    }

    dst[count] = 0; //NULL terminated

    return count;
}

static int generate_sdp_audio(struct sdp_info *sdp, struct rtp_packet *pkt)
{
    int audio_payload_type;
    char *buf;

    if (!sdp->audio_enable_flag)
        return 0;

    audio_payload_type = to_rtp_payload_type(sdp->audio_config);
    buf = (char *)pkt->buf + pkt->len;

    if(!sdp->video_enable_flag)
    {
        buf += my_strcat(buf, "a=StreamCount:integer;1\r\n");
        buf += my_strcat(buf, "a=control:*\r\n");
        buf += my_strcat(buf, "a=control:streamid=0\r\n");
    }

    switch(sdp->audio_config)
    {
        case FH_SDP_LPCM_L8_R8:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d L8/8000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_LPCM_L16_R8:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d L16/8000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_LPCM_L16_R16:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d L16/16000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_G711A_8K :
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d PCMA/8000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_G711A_16K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d PCMA/16000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_G711U_8K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d PCMU/8000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_G711U_16K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d PCMU/16000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_G726_16K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d G726-16/8000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_G726_32K :
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d G726-32/8000\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_AAC_8K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d MPEG4-GENERIC/8000/1\r\n"
                    "a=mpeg4-esid:1"
                    "a=fmtp:%d profile-level-id=15; streamtype=5; mode=AAC-hbr; config=1588;"
                    "SizeLength=13; IndexLength=3; IndexDeltaLength=3;Profile=1\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_AAC_16K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d MPEG4-GENERIC/16000/1\r\n"
                    "a=mpeg4-esid:1"
                    "a=fmtp:%d profile-level-id=15; streamtype=5; mode=AAC-hbr; config=1408;SizeLength=13; IndexLength=3; IndexDeltaLength=3;Profile=1\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_AAC_32K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d MPEG4-GENERIC/32000/1\r\n"
                    "a=mpeg4-esid:1"
                    "a=fmtp:%d profile-level-id=15; streamtype=5; mode=AAC-hbr; config=1288;SizeLength=13; IndexLength=3; IndexDeltaLength=3;Profile=1\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_AAC_441K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d MPEG4-GENERIC/44100/1\r\n"
                    "a=mpeg4-esid:1"
                    "a=fmtp:%d profile-level-id=15; streamtype=5; mode=AAC-hbr; config=1208;SizeLength=13; IndexLength=3; IndexDeltaLength=3;Profile=1\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type,
                    audio_payload_type);
            break;
        case FH_SDP_AAC_48K:
            buf += sprintf(buf, "m=audio 0 RTP/AVP %d\r\n"
                    "a=rtpmap:%d MPEG4-GENERIC/48000/1\r\n"
                    "a=mpeg4-esid:1"
                    "a=fmtp:%d profile-level-id=15; streamtype=5; mode=AAC-hbr; config=1188;SizeLength=13; IndexLength=3; IndexDeltaLength=3;Profile=1\r\n"
                    "a=control:streamid=1\r\n",
                    audio_payload_type,
                    audio_payload_type,
                    audio_payload_type);
            break;
        default:
            buf += sprintf(buf, " ");
            break;
    }

    pkt->len = buf - (char*)pkt->buf;
    return 0;
}

static int generate_sdp_h265(struct sdp_info *sdp, struct rtp_packet *pkt)
{
    char *buf;
    struct sdp_video_info *video_config = &sdp->video_config[1 - sdp->video_curr];

    if (!video_config->sps || !video_config->pps || !video_config->vps)
        return -1;

    buf = (char*)pkt->buf + pkt->len;
    buf += my_strcat(buf, "v=0\r\n");
    if(sdp->audio_enable_flag)
    {
        buf += my_strcat(buf, "a=StreamCount:integer;2\r\n");
        buf += my_strcat(buf, "a=control:*\r\n");
    }
    buf += my_strcat(buf, "m=video 0 RTP/AVP 96\r\n");
    if(sdp->audio_enable_flag)
    {
        buf += my_strcat(buf, "a=control:streamid=0\r\n");
    }
    buf += my_strcat(buf,
            "a=rtpmap:96 H265/90000\r\n"
            "a=fmtp:96 \0");

    buf += my_strcat(buf,"sprop-vps=");
    buf += base64_encode(buf, video_config->vps, video_config->vps_len);
    buf += my_strcat(buf, "; ");
    buf += my_strcat(buf,"sprop-sps=");
    buf += base64_encode(buf, video_config->sps, video_config->sps_len);
    buf += my_strcat(buf, "; ");
    buf += my_strcat(buf,"sprop-pps=");
    buf += base64_encode(buf, video_config->pps, video_config->pps_len);
    buf += my_strcat(buf, "\r\n");
    pkt->len = buf - pkt->buf;

    return 0;
}

static int generate_sdp_h264(struct sdp_info *sdp, struct rtp_packet *pkt)
{
    char *buf;
    struct sdp_video_info *video_config = &sdp->video_config[1 - sdp->video_curr];

    if (!video_config->sps || !video_config->pps)
        return -1;

    buf = pkt->buf + pkt->len;
    buf += my_strcat(buf, "v=0\r\n");
    if(sdp->audio_enable_flag)
    {
        buf += my_strcat(buf, "a=StreamCount:integer;2\r\n");
        buf += my_strcat(buf, "a=control:*\r\n");
    }

    buf += my_strcat(buf, "m=video 0 RTP/AVP 96\r\n");
    if(sdp->audio_enable_flag)
    {
        buf += my_strcat(buf, "a=control:streamid=0\r\n");
    }

    buf += my_strcat(buf,
            "a=rtpmap:96 H264/90000\r\n"
            "a=fmtp:96 profile-level-id=4DE028;packetization-mode=1;"
            "sprop-parameter-sets=");

    buf += base64_encode(buf, video_config->sps, video_config->sps_len);
    *(buf++) = ',';
    buf += base64_encode(buf, video_config->pps, video_config->pps_len);
    buf += my_strcat(buf, "\r\n");
    pkt->len = buf - pkt->buf;

    return 0;
}

static int generate_sdp(struct sdp_info *sdp, struct rtp_packet *pkt)
{
    pkt->buf[0] = 0;
    pkt->len = 0;
    if (generate_sdp_h265(sdp, pkt) < 0)
    {
        if (generate_sdp_h264(sdp, pkt) < 0)
        {
            printf("Error: rtsp_handle_describe, SDP not generated yet!\n");
            return -1;
        }
    }

    generate_sdp_audio(sdp, pkt);
    if (pkt->len >= sizeof(pkt->buf))
    {
        printf("Error: rtsp_generate_sdp, too large sdp,length=%d!\n", pkt->len);
        *((int *)0) = 0; /*don't let it run...*/
    }

    return 0;
}

static void generate_rtp_header(unsigned char *dst, int payload_len, int is_last, unsigned short seq_num, unsigned long pts,
        unsigned long ssrc, int rtp_transport, unsigned int channel_id, unsigned int media_type)
{
    if (rtp_transport == RTP_TRANSPORT_TCP)
    {
        /* RTSP interleaved frame header */
        dst[0] = 0x24;
        dst[1] = channel_id;
        dst[2] = ((payload_len + 12) >> 8) & 0xff;
        dst[3] = (payload_len + 12) & 0xff;
        dst += 4;
    }

    /* RTP header */
    dst[0]  = 0x80;
    dst[1]  = (is_last ? 0x80 : 0x00) | media_type;
    dst[2]  = seq_num >> 8;
    dst[3]  = seq_num & 0xff;
    dst[4]  = ((unsigned char *)(&pts))[3]; /*note: fixed to little endian now...*/
    dst[5]  = ((unsigned char *)(&pts))[2];
    dst[6]  = ((unsigned char *)(&pts))[1];
    dst[7]  = ((unsigned char *)(&pts))[0];
    dst[8]  = ((unsigned char *)(&ssrc))[3];
    dst[9]  = ((unsigned char *)(&ssrc))[2];
    dst[10] = ((unsigned char *)(&ssrc))[1];
    dst[11] = ((unsigned char *)(&ssrc))[0];
}

static void pre_queue_packet_on_client(struct server_context *server, int whole_pkt_num)
{
    struct list_head *pos;
    struct client_context *client;

    list_for_each(pos, &server->client_list)
    {
        client = list_entry(pos, struct client_context, client_list);
        client->blocked = 0;
        if (client->data_num >= MAX_PKTS_PER_CLIENT)
        {
            client->blocked = 1;
            printf("RTSP: error, drop packet, client %p, pkt_num=%d,pkts_of_this_frame=%d,max_pkt_num=%d.\n",
                    client, client->data_num, whole_pkt_num, MAX_PKTS_PER_CLIENT);
        }
    }
}

static int queue_packet_on_client(struct server_context *server, struct rtp_packet *pkt, int is_last_pkt)
{
    struct list_head *pos;
    struct client_context *client;
    struct rtp_packet *orig_pkt = pkt;

    list_for_each(pos, &server->client_list)
    {
        client = list_entry(pos, struct client_context, client_list);
        if (!client->blocked && client->status == CLIENT_START_PLAY)
        {
            if (!pkt)
            {
                pkt = clone_packet(server, orig_pkt);
                if (!pkt)
                {
                    break;
                }
            }

            list_add_tail(&pkt->list, &client->data_list);
            client->data_num++;

            pkt = NULL;
        }
    }

    if (pkt)
        free_packet(server, pkt);

    if (is_last_pkt)
    {
        list_for_each(pos, &server->client_list)
        {
            client = list_entry(pos, struct client_context, client_list);
            if (!client->blocked && client->status == CLIENT_START_PLAY)
            {
                send_client(client);
            }
        }
    }

    return 0;
}

static int generate_rtp_video_packets_and_queue(struct server_context *server, unsigned char *nal, int nal_len,
        unsigned long pts, FH_MEDIA_FORMAT_TYPE venc_format, unsigned int channel_id)
{
    char   fu_buf0;
    char   fu_buf1;
    char   fu_buf2;
    struct rtp_packet *pkt;
    int header_offset;
    int max_len;
    int video_type = 96;
    int mark_bit;
    int fragment_len;
    int pkt_num;
    int fu_len;
    char *fu_buf;

    if (server->rtp_info.rtp_transport == RTP_TRANSPORT_TCP)
    {
        header_offset = 16;
        max_len       = TCP_MMSS;
    }
    else
    {
        header_offset = 12;
        max_len       = RTP_PACKET_MAX_SIZE;
    }

    nal     += 4;
    nal_len -= 4;

    if (nal_len > max_len - header_offset)
    {
        if(FH_VENC_H264 == venc_format)
        {
            fragment_len = max_len - header_offset - 2;
            fu_len = fragment_len;

            mark_bit = 0;
            fu_buf0 = (nal[0] & 0x60) | 28;
            fu_buf1 = (nal[0] & 0x1f) | 0x80; /*0x80 means it's the first packet*/
            nal++;
            nal_len--;

            pkt_num = (nal_len + fragment_len - 1)/fragment_len;
            pre_queue_packet_on_client(server, pkt_num);
            while (nal_len > 0)
            {
                pkt = alloc_packet(server);
                if (!pkt)
                    break;

                if (nal_len <= fragment_len) /*it's the last packet*/
                {
                    fu_len = nal_len;
                    fu_buf1 |= 0x40;
                    mark_bit = 1;
                }
                server->rtp_info.video_sequence_num++;
                generate_rtp_header((unsigned char*)pkt->buf, fu_len + 2, mark_bit, server->rtp_info.video_sequence_num,
                        pts, server->rtp_info.ssrc, server->rtp_info.rtp_transport, 0, video_type);
                fu_buf    = pkt->buf + header_offset;
                fu_buf[0] = fu_buf0;
                fu_buf[1] = fu_buf1;
                fu_buf1 &= 0x7f; /*clear first packet flag*/
                copy_noncached_stream(fu_buf + 2, (char *)nal, fu_len);
                pkt->len = header_offset + 2 + fu_len;
                queue_packet_on_client(server, pkt, mark_bit);

                nal += fu_len;
                nal_len -= fu_len;
            }
        }
        else if(FH_VENC_H265 == venc_format)
        {
            fragment_len = max_len - header_offset - 3;
            fu_len = fragment_len;

            mark_bit = 0;
            fu_buf0 = (nal[0] & 0x81)|(49 << 1);
            fu_buf1 = nal[1];
            fu_buf2 =  (nal[0] >> 1) & 0x3f; /*get nal_type*/
            fu_buf2 |= 0x80; /*0x80 means it's the first packet*/

            nal += 2;
            nal_len -= 2;

            pkt_num = (nal_len + fragment_len - 1)/fragment_len;
            pre_queue_packet_on_client(server, pkt_num);
            while (nal_len > 0)
            {
                pkt = alloc_packet(server);
                if (!pkt)
                    break;

                if (nal_len <= fragment_len) /*it's the last packet*/
                {
                    fu_len = nal_len;
                    fu_buf2 |= 0x40; /*0x40 means it's the last packet*/
                    mark_bit = 1;
                }

                server->rtp_info.video_sequence_num++;
                generate_rtp_header((unsigned char*)pkt->buf, fu_len + 3, mark_bit, server->rtp_info.video_sequence_num,
                        pts, server->rtp_info.ssrc, server->rtp_info.rtp_transport, 0, video_type);
                fu_buf    = pkt->buf + header_offset;
                fu_buf[0] = fu_buf0;
                fu_buf[1] = fu_buf1;
                fu_buf[2] = fu_buf2;
                fu_buf2 &= 0x7f; /*clear first packet flag*/

                copy_noncached_stream(fu_buf + 3, (char *)nal, fu_len);
                pkt->len = header_offset + 3 + fu_len;
                queue_packet_on_client(server, pkt, mark_bit);

                nal += fu_len;
                nal_len -= fu_len;
            }
        }
    }
    else
    {
        pkt = alloc_packet(server);
        if (!pkt)
            return 0;

        mark_bit = 1;
        /*if use M = 1 HaiKang NVR no play hevc*/
        if (venc_format == FH_VENC_H265)
        {
            if (((nal[0] >> 1) & 0x3f) == 32) /*if nal_type is 32, it means it is vps data*/
                mark_bit = 0;
        }

        server->rtp_info.video_sequence_num++;
        generate_rtp_header((unsigned char*)pkt->buf, nal_len, mark_bit, server->rtp_info.video_sequence_num, pts,
                server->rtp_info.ssrc, server->rtp_info.rtp_transport, 0, video_type);
        copy_noncached_stream(pkt->buf + header_offset, (char *)nal, nal_len);
        pkt->len = header_offset + nal_len;
        pre_queue_packet_on_client(server, 1);
        queue_packet_on_client(server, pkt, 1);
    }

    return 0;
}


static int generate_rtp_audio_packets_and_queue(struct server_context *server, void *data, int size,
        unsigned long pts, FH_MEDIA_FORMAT_TYPE audio_type, unsigned int channel_id)
{
    struct rtp_packet *pkt;
    int header_offset;
    int max_len;
    int ac_type;

    if (server->rtp_info.rtp_transport == RTP_TRANSPORT_TCP)
    {
        header_offset = 16;
        max_len       = TCP_MMSS;
    }
    else
    {
        header_offset = 12;
        max_len       = RTP_PACKET_MAX_SIZE;
    }

    if (size > max_len - header_offset)
    {
        printf("RTSP: error, supported audio packet max length is %d,real length=%d!\n",
                max_len - header_offset, size);
        return 0;
    }

    pkt = alloc_packet(server);
    if (!pkt)
        return 0;

    ac_type = to_rtp_payload_type(server->rtp_info.sdp.audio_config);
    server->rtp_info.audio_sequence_num++;

    generate_rtp_header((unsigned char*)pkt->buf, size, 1, server->rtp_info.audio_sequence_num, pts,
            server->rtp_info.ssrc, server->rtp_info.rtp_transport, 2, ac_type);
    copy_noncached_stream(pkt->buf + header_offset, data, size);
    pkt->len = header_offset + size;
    pre_queue_packet_on_client(server, 1);
    queue_packet_on_client(server, pkt, 1);

    return 0;
}

static void new_client(struct server_context *server, int client_fd, struct sockaddr_in *addr)
{
    int num = 0;
    int flags;
    struct list_head *pos;
    struct client_context *client;

    list_for_each(pos, &server->client_list)
    {
        num++;
    }

    if (num >= MAX_CLIENT_COUNT)
    {
        goto Error;
    }

    flags = fcntl(client_fd, F_GETFL, 0);
    if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        printf("RTSP: error with fcntl, ignore the new connection!\n");
        goto Error;
    }

    client = malloc(sizeof(struct client_context));
    if (!client)
    {
        printf("RTSP: out of memory, new_client failed!\n");
        goto Error;
    }

    memset(client, 0, sizeof(*client));
    INIT_LIST_HEAD(&client->cmdresp_list);
    INIT_LIST_HEAD(&client->data_list);
    client->server       = server;
    client->status = CLIENT_STOP_PLAY;
    client->client_fd           = client_fd;
    client->last_rw_time = server->now_time;
    *((int *)(client->ip)) = addr->sin_addr.s_addr;
    client->port = ntohs(addr->sin_port);

    list_add_tail(&client->client_list, &server->client_list);
    printf("RTSP: client %p %d.%d.%d.%d/%d established!\n", client, client->ip[0],
            client->ip[1], client->ip[2], client->ip[3], client->port);
    return;

Error:
    close(client_fd);
}

static void delete_client(struct client_context *client, char *reason)
{
    struct list_head *head;
    struct list_head *pos;
    struct list_head *next;
    struct rtp_packet *pkt;

    printf("RTSP: client %p %d.%d.%d.%d/%d deleted(%s)!\n", client, client->ip[0],
            client->ip[1], client->ip[2], client->ip[3], client->port, reason);

    list_del(&client->client_list);

    head = &client->data_list;
Retry:
    list_for_each_safe(pos, next, head)
    {
        pkt = list_entry(pos, struct rtp_packet, list);
        list_del(pos);
        free(pkt);
    }

    if (head == &client->data_list)
    {
        head = &client->cmdresp_list;
        goto Retry;
    }

    if (client->curr_pkt)
        free(client->curr_pkt);

    if (client->client_fd >= 0)
        close(client->client_fd);

    free(client);
}

static int send_client(struct client_context *client)
{
    int ret;
    struct list_head *pos;

Retry:
    if (!client->curr_pkt)
    {
        pos = client->cmdresp_list.next;
        if (pos == &client->cmdresp_list) /*cmdresp_list is empty*/
        {
            pos = client->data_list.next;
            if (pos == &client->data_list)
            {
                return 0;
            }
            client->data_num--;
        }

        list_del(pos);
        client->curr_pkt = list_entry(pos, struct rtp_packet, list);
        client->curr_pkt_sent_len = 0;
    }

    ret = send(client->client_fd, client->curr_pkt->buf + client->curr_pkt_sent_len, client->curr_pkt->len - client->curr_pkt_sent_len, 0);
    if (ret == 0)
    {
        return 0;
    }
    else if (ret < 0)
    {
        if (errno == EAGAIN)
        {
            return 0;
        }
        return -1;
    }

    client->last_rw_time = client->server->now_time;

    client->curr_pkt_sent_len += ret;
    if (client->curr_pkt_sent_len >= client->curr_pkt->len)
    {
        free_packet(client->server, client->curr_pkt);
        client->curr_pkt = NULL;
    }

    goto Retry;

    return ret;
}

static unsigned long long audio_conver_pts(unsigned long long pts, FH_MEDIA_FORMAT_TYPE audio_type)
{
    int ac_timef = 0;
    unsigned long long time_stamp;

    switch (audio_type)
    {
        case FH_SDP_AAC_48K:
            ac_timef = 48000;
            break;
        case FH_SDP_AAC_441K:
            ac_timef = 44100;
            break;
        case FH_SDP_AAC_32K:
            ac_timef = 32000;
            break;
        case FH_SDP_AAC_16K:
        case FH_SDP_LPCM_L16_R16:
        case FH_SDP_G711A_16K:
        case FH_SDP_G711U_16K:
            ac_timef = 16000;
            break;
        default:
            ac_timef = 8000;
            break;
    }

    time_stamp = pts * ac_timef / 1000000;
    return time_stamp;
}

static void set_sdp_element(struct sdp_info *sdp, int is_h265, unsigned char *data, int len)
{
    unsigned char chr;
    char **element;
    int    *element_len;
    struct sdp_video_info *vid = &sdp->video_config[sdp->video_curr];

    if (len < 5)
        return;

    if (!is_h265)
    {
        chr = data[4] & 0x1f;
        if (7 == chr)
        {
            element = &vid->sps;
            element_len = &vid->sps_len;
        }
        else if (8 == chr)
        {
            element = &vid->pps;
            element_len = &vid->pps_len;
        }
        else
        {
            return;
        }
    }
    else
    {
        chr = (data[4] >> 1) & 0x3f;
        if (32 == chr)
        {
            element = &vid->vps;
            element_len = &vid->vps_len;
        }
        else if (33 == chr)
        {
            element = &vid->sps;
            element_len = &vid->sps_len;
        }
        else if (34 == chr)
        {
            element = &vid->pps;
            element_len = &vid->pps_len;
        }
        else
        {
            return;
        }
    }

    if (*element)
    {
        free(*element);
    }

    *element = malloc(len);
    if (*element)
    {
        copy_noncached_stream(*element, (char *)data, len);
        *element_len = len;
        if (vid->pps && vid->sps && (!is_h265 || vid->vps ))
        {
            sdp->video_curr = 1 - sdp->video_curr;
            vid = &sdp->video_config[sdp->video_curr];
            if (vid->sps)
            {
                free(vid->sps);
                vid->sps = NULL;
            }
            if (vid->pps)
            {
                free(vid->pps);
                vid->pps = NULL;
            }
            if (vid->vps)
            {
                free(vid->vps);
                vid->vps = NULL;
            }
        }
    }
    else
    {
        printf("RTSP: out of memory, set_sdp_element failed!\n");
        *element_len = 0;
    }
}

static void generate_response_header(struct client_session *session, struct rtp_packet *pkt)
{
    pkt->len = sprintf(pkt->buf, "RTSP/1.0 200 OK\r\nCSeq: %s\r\n", session->cseq);
}

static unsigned long generate_session_number(void)
{
    unsigned long msec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    msec = tv.tv_sec * 1000L + tv.tv_usec / 1000L;
    return msec;
}

static int rtsp_handle_options(struct client_context *client, struct client_session *session)
{
    struct rtp_packet *pkt = alloc_packet(client->server);

    if (!pkt)
        return -1;

    generate_response_header(session, pkt);
    pkt->len += my_strcat(pkt->buf + pkt->len, "Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE\r\n\r\n");
    list_add_tail(&pkt->list, &client->cmdresp_list);
    return 0;
}

static int rtsp_handle_describe(struct client_context *client, struct client_session *session)
{
    struct server_context *server;
    struct rtp_packet *sdp_pkt;
    struct rtp_packet *pkt;

    server = client->server;
    pkt = alloc_packet(server);
    sdp_pkt = alloc_packet(server);

    if (!pkt || !sdp_pkt || generate_sdp(&server->rtp_info.sdp, sdp_pkt) < 0)
        goto Error;

    generate_response_header(session, pkt);
    pkt->len += my_strcat(pkt->buf + pkt->len, "Content-Type: application/sdp\r\n");
    pkt->len += sprintf(pkt->buf + pkt->len, "Content-Length: %d\r\n\r\n", sdp_pkt->len);
    pkt->len += my_strcat(pkt->buf + pkt->len, sdp_pkt->buf);
    if (pkt->len >= sizeof(pkt->buf))
    {
        printf("Error: rtsp_handle_describe, too large sdp,length=%d!\n", pkt->len);
        *((int *)0) = 0; /*don't let it run...*/
    }
    list_add_tail(&pkt->list, &client->cmdresp_list);
    free_packet(server, sdp_pkt);
    return 0;

Error:
    if (pkt)
        free_packet(server, pkt);
    if (sdp_pkt)
        free_packet(server, sdp_pkt);
    return -1;
}

static int rtsp_handle_setup(struct client_context *client, struct client_session *session)
{
    struct rtp_packet *pkt = alloc_packet(client->server);

    if (!pkt)
        return -1;

    if (!session->session_num)
    {
        session->session_num = generate_session_number();
    }

    generate_response_header(session, pkt);
    pkt->len += sprintf(pkt->buf + pkt->len, "Session: %lu\r\n", session->session_num);

    //now fixed to use TCP
    pkt->len += sprintf(pkt->buf + pkt->len, "Transport: RTP/AVP/TCP;unicast;interleaved=%d-%d\r\n\r\n",
            session->stream_id * 2, session->stream_id * 2 + 1);

    list_add_tail(&pkt->list, &client->cmdresp_list);

    return 0;
}

static int rtsp_handle_play(struct client_context *client, struct client_session *session)
{
    struct rtp_packet *pkt = alloc_packet(client->server);

    if (!pkt)
        return -1;

    generate_response_header(session, pkt);
    pkt->len += my_strcat(pkt->buf + pkt->len, "\r\n");
    list_add_tail(&pkt->list, &client->cmdresp_list);
    if (client->status != CLIENT_START_PLAY)
    {
        if (client->server->rtsp_play_hook.hook)
        {
            client->server->rtsp_play_hook.hook(client->server->rtsp_play_hook.param);
            printf("RTSP: play hook is called.\n");
        }
    }
    client->status = CLIENT_START_PLAY;
    return 0;
}

static int rtsp_handle_teardown(struct client_context *client, struct client_session *session)
{
    client->status = CLIENT_STOP_PLAY;
    printf("RTSP: client %p %d.%d.%d.%d/%d teardown!\n", client, client->ip[0],
            client->ip[1], client->ip[2], client->ip[3], client->port);
    return -1;
}

static int rtsp_handle_request(struct client_context *client)
{
    int ret;
    int len;
    char *sub_str;
    char *pos;
    char *start;
    struct client_session *session = &client->session;

    len = strlen(session->read_buf);
    ret = recv(client->client_fd, session->read_buf + len, sizeof(session->read_buf) - len, 0);
    if (ret <= 0)
        return -1; /*error*/

    client->last_rw_time = client->server->now_time;

    len += ret;
    if (len >= sizeof(session->read_buf))
    {
        printf("Error: rtsp_handle_request with large request packet %d!\n", len);
        return -1;
    }
    session->read_buf[len] = 0; /*ensure NULL terminated*/

    pos = strstr(session->read_buf, "\r\n\r\n");
    if (!pos) /*the whole packet not received yet...*/
    {
        session->read_buf[0] = 0; /*prepare to receive new request*/
        return 0;
    }

    pos = strstr(session->read_buf, "\r\n");
    if (!pos)
        return 0; /*it will not happen*/

    start = pos + 2;
    if ((sub_str = strstr(start, "CSeq: ")) != NULL)
    {
        pos = strstr(sub_str, "\r\n");
        if (pos)
        {
            len = pos - sub_str - 6;
            if (len >= sizeof(session->cseq))
            {
                printf("Error: %s, CSeq length=%d!\n", __func__, len);
                len = sizeof(session->cseq) - 1;
            }
            memcpy(session->cseq, sub_str + 6, len);
            session->cseq[len] = 0;
        }
    }

    /*
       if ((sub_str = strstr(start, "client_port=")) != NULL)
       {
       session->ctx->udp_send_port = my_strtol(sub_str + 12);
       }
       */

    if ((sub_str = strstr(start, "interleaved=")) != NULL)
    {
        pos = strstr(sub_str, "-");
        if (pos)
        {
            session->stream_id = my_strtol(sub_str + 12) / 2;
        }
    }

    if ((sub_str = strstr(start, "Session: ")) != NULL)
    {
        pos = strstr(sub_str, "\r\n");
        if (pos)
        {
            session->session_num = my_strtol(sub_str + 9);
        }
    }

    ret = 0;
    if (strstr(session->read_buf, "OPTIONS"))
    {
        ret = rtsp_handle_options(client, session);
    }
    else if (strstr(session->read_buf, "DESCRIBE"))
    {
        ret = rtsp_handle_describe(client, session);
    }
    else if (strstr(session->read_buf, "SETUP"))
    {
        ret = rtsp_handle_setup(client, session);
    }
    else if (strstr(session->read_buf, "PLAY"))
    {
        ret = rtsp_handle_play(client, session);
    }
    else if (strstr(session->read_buf, "TEARDOWN"))
    {
        ret = rtsp_handle_teardown(client, session);
    }

    session->read_buf[0] = 0; /*prepare to receive new request*/
    return ret;
}

static int create_tcp_server_socket(unsigned short port)
{
    int server_fd;
    struct sockaddr_in addr;
    int on = 1;
    struct timeval timeout;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        printf("Error: socket failed with RTSP!\n");
        return server_fd;
    }

    timeout.tv_sec = 0;
    timeout.tv_usec = 10*1000;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        printf("Error: bind failed with RTSP!\n");
        goto Exit;
    }

    if (listen(server_fd, 5) != 0)
    {
        printf("Error: listen failed with RTSP!\n");
        goto Exit;
    }

    return server_fd;

Exit:
    close(server_fd);
    return -1;
}

static void *rtsp_tcp_server_thread(void *arg)
{
    struct server_context *server = (struct server_context *)arg;
    struct sockaddr_in addr;
    int ret;
    socklen_t addr_len;
    int fd;
    int maxfd;
    struct timeval tv;
    fd_set rfds;
    fd_set wfds;
    struct list_head *pos;
    struct list_head *next;
    struct client_context *client;

    pthread_mutex_lock(&server->lock);

    while (!server->stop)
    {
        gettimeofday(&tv, NULL);
        server->now_time = tv.tv_sec;

        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_SET(server->server_fd, &rfds);

        maxfd = server->server_fd;

        list_for_each_safe(pos, next, &server->client_list)
        {
            client = list_entry(pos, struct client_context, client_list);
            if (server->now_time - client->last_rw_time >= CLIENT_EXPIRE_TIME)
            {
                delete_client(client, "client-expire");
                continue;
            }

            FD_SET(client->client_fd, &rfds);
            if (client->curr_pkt || !list_empty(&client->data_list) || !list_empty(&client->cmdresp_list))
            {
                FD_SET(client->client_fd, &wfds);
            }
            if (client->client_fd > maxfd)
                maxfd = client->client_fd;
        }

        pthread_mutex_unlock(&server->lock);
        tv.tv_sec = 0;
        tv.tv_usec = 20*1000;
        ret = select(maxfd + 1, &rfds, &wfds, NULL, &tv);
        pthread_mutex_lock(&server->lock);
        if (ret <= 0)
            continue;

        if (FD_ISSET(server->server_fd, &rfds))
        {
            addr_len = sizeof(addr);
            fd = accept(server->server_fd, (struct sockaddr *)&addr, &addr_len);
            if (fd >= 0)
            {
                new_client(server, fd, &addr);
            }
        }

        list_for_each_safe(pos, next, &server->client_list)
        {
            client = list_entry(pos, struct client_context, client_list);
            if (FD_ISSET(client->client_fd, &rfds))
            {
                ret = rtsp_handle_request(client);
                if (ret < 0)
                {
                    delete_client(client, "handle-request");
                    continue;
                }
            }

            if (FD_ISSET(client->client_fd, &wfds))
            {
                ret = send_client(client);
                if (ret < 0)
                {
                    delete_client(client, "send-packet");
                }
            }
        }
    }

    list_for_each_safe(pos, next, &server->client_list)
    {
        client = list_entry(pos, struct client_context, client_list);
        delete_client(client, "app-exit");
    }

    release_free_ptk_list(server);

    free_sdp_info(server);

    if (server->server_fd >= 0)
        close(server->server_fd);

    server->stop = 0;
    server->alloced = 0;

    pthread_mutex_unlock(&server->lock);

    return NULL;
}

RTSP_HANDLE librtsp_start_server(enum rtp_transport transport, int port)
{
    int i;
    int server_fd;
    struct server_context *server = NULL;
    pthread_attr_t attr;
    pthread_t thread;

    if (port <= 0)
        return NULL;

    transport = RTP_TRANSPORT_TCP; /*FIXME, fixed to tcp now*/

    if (!g_server_ctx_lock_inited)
    {
        g_server_ctx_lock_inited = 1;
        pthread_mutex_init(&g_server_ctx_lock, NULL);
    }

    pthread_mutex_lock(&g_server_ctx_lock);

    for (i=0; i<MAX_SUPPORT_SERVER_NUM; i++)
    {
        if (!g_server_ctx_list[i].alloced)
            break;
    }

    if (i < MAX_SUPPORT_SERVER_NUM)
    {
        if ((server_fd = create_tcp_server_socket(port)) >= 0)
        {
            server = &g_server_ctx_list[i];
            if (!server->lock_inited)
            {
                server->lock_inited = 1;
                pthread_mutex_init(&server->lock, NULL);
            }
            server->alloced = 1;
            INIT_LIST_HEAD(&server->client_list);
            INIT_LIST_HEAD(&server->free_ptk_list);
            server->stop = 0;
            server->server_fd = server_fd;
            memset(&server->rtp_info, 0, sizeof(server->rtp_info));
            server->rtp_info.rtp_transport = transport;
            server->rtsp_play_hook.hook = NULL;

            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            pthread_attr_setstacksize(&attr, 3 * 1024);
#ifdef __RTTHREAD_OS__
            struct sched_param param;
            param.sched_priority = FH_RTSP_THREAD_PRORITY;
            pthread_attr_setschedparam(&attr, &param);
#endif
            if (pthread_create(&thread, &attr, rtsp_tcp_server_thread, server) != 0)
            {
                printf("Error: create rtsp-server failed!\n");
                close(server_fd);
                server->server_fd = -1;
                server->alloced = 0;
                server = NULL;
            }
        }
    }
    else
    {
        printf("RTSP: error, now only support %d server!\n", MAX_SUPPORT_SERVER_NUM);
    }

    pthread_mutex_unlock(&g_server_ctx_lock);

    return (RTSP_HANDLE)server;
}

void librtsp_play_sethook(RTSP_HANDLE h, void (*hook)(void *), void *param)
{
    struct server_context *server = (struct server_context *)h;

    if (server)
    {
        server->rtsp_play_hook.hook = hook;
        server->rtsp_play_hook.param = param;
    }
}

void librtsp_push_data(RTSP_HANDLE h, void *data, int size, unsigned long long pts,
        unsigned int channel_id, FH_MEDIA_FORMAT_TYPE media_format)
{
    struct server_context *server = (struct server_context *)h;

    if (!server || !data || size <= 0)
        return;

    pthread_mutex_lock(&server->lock);
    if (server->alloced)
    {
        server->rtp_info.sdp.audio_enable_flag = 0;
        server->rtp_info.sdp.video_enable_flag = 1;

        if(media_format > FH_SDP_AC_MAX) /*video*/
        {
            set_sdp_element(&server->rtp_info.sdp, FH_VENC_H265 == media_format ? 1 : 0, data, size);
            generate_rtp_video_packets_and_queue(server, data, size, pts * 9 / 100,
                    media_format, channel_id);
        }
        else /*audio*/
        {
            server->rtp_info.sdp.audio_config = media_format;
            pts = audio_conver_pts(pts, media_format);
            generate_rtp_audio_packets_and_queue(server, data, size, pts,
                    media_format, channel_id);
        }
    }
    pthread_mutex_unlock(&server->lock);
}

void librtsp_stop_server(RTSP_HANDLE h)
{
    int run;
    struct server_context *server = (struct server_context *)h;

    if (server)
    {
        pthread_mutex_lock(&g_server_ctx_lock);
        run = server->alloced;
        pthread_mutex_unlock(&g_server_ctx_lock);
        if (run)
        {
            server->stop = 1;
            while (server->stop)
            {
                usleep(10*1000);
            }
        }
    }
}
