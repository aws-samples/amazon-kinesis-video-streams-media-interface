#include <sample_common.h>
#include "media_config_proc.h"
#include "chn_yuv_type.h"
#include "chn_enc_type.h"
#include "chn_rc_type.h"
#include "chn_set_rc.h"

static FH_SINT32 g_get_stream_stop;
static FH_SINT32 g_get_stream_running;

extern FH_VOID *sample_common_get_stream_proc(FH_VOID *arg);

static struct channel_info g_channel_infos[] = {
#ifdef CH0_WIDTH
    {
        .enable      = 1,
        .channel     = 0,
        .width       = CH0_WIDTH,
        .height      = CH0_HEIGHT,
#if defined(CH0_MAX_WIDTH)
        .max_width   = CH0_MAX_WIDTH,
        .max_height  = CH0_MAX_HEIGHT,
#endif
        .yuv_type    = CH0_YUV_TYPE,
        .frame_count = CH0_FRAME_COUNT,
        .frame_time  = CH0_FRAME_TIME,
        .bps         = CH0_BIT_RATE * 1024,
        .enc_type    = CH0_ENC_TYPE,
        .rc_type     = CH0_RC_TYPE,
        .breath_on   = CH0_BREATH_OPEN
    },
#endif /*CH0_WIDTH*/

#ifdef CH1_WIDTH
    {
        .enable      = 1,
        .channel     = 1,
        .width       = CH1_WIDTH,
        .height      = CH1_HEIGHT,
#if defined(CH1_MAX_WIDTH)
        .max_width   = CH1_MAX_WIDTH,
        .max_height  = CH1_MAX_HEIGHT,
#endif
        .yuv_type    = CH1_YUV_TYPE,
        .frame_count = CH1_FRAME_COUNT,
        .frame_time  = CH1_FRAME_TIME,
        .bps         = CH1_BIT_RATE * 1024,
        .enc_type    = CH1_ENC_TYPE,
        .rc_type     = CH1_RC_TYPE,
        .breath_on   = CH1_BREATH_OPEN
    },
#endif /*CH1_WIDTH*/

#ifdef CH2_WIDTH
    {
        .enable      = 1,
        .channel     = 2,
        .width       = CH2_WIDTH,
        .height      = CH2_HEIGHT,
#if defined(CH2_MAX_WIDTH)
        .max_width   = CH2_MAX_WIDTH,
        .max_height  = CH2_MAX_HEIGHT,
#endif
        .yuv_type    = CH2_YUV_TYPE,
        .frame_count = CH2_FRAME_COUNT,
        .frame_time  = CH2_FRAME_TIME,
        .bps         = CH2_BIT_RATE * 1024,
        .enc_type    = CH2_ENC_TYPE,
        .rc_type     = CH2_RC_TYPE,
        .breath_on   = CH2_BREATH_OPEN
    },
#endif /*CH2_WIDTH*/

#ifdef CH3_WIDTH
    {
        .enable      = 1,
        .channel     = 3,
        .width       = CH3_WIDTH,
        .height      = CH3_HEIGHT,
#if defined(CH3_MAX_WIDTH)
        .max_width   = CH3_MAX_WIDTH,
        .max_height  = CH3_MAX_HEIGHT,
#endif
        .yuv_type    = CH3_YUV_TYPE,
        .frame_count = CH3_FRAME_COUNT,
        .frame_time  = CH3_FRAME_TIME,
        .bps         = CH3_BIT_RATE * 1024,
        .enc_type    = CH3_ENC_TYPE,
        .rc_type     = CH3_RC_TYPE,
        .breath_on   = CH3_BREATH_OPEN
    },
#endif /*CH3_WIDTH*/
};

FH_SINT32 sample_common_dsp_get_max_channel_index(FH_VOID)
{
    FH_SINT32 i;
    FH_SINT32 maxidx = -1;
    FH_UINT32 num = sizeof(g_channel_infos)/sizeof(g_channel_infos[0]);

    for (i=0; i < num; i++)
    {
        if (g_channel_infos[i].channel > maxidx)
            maxidx = g_channel_infos[i].channel;
    }

    return maxidx;
}

FH_SINT32 sample_common_dsp_get_channel_info(FH_UINT32 chan_vpu, struct channel_info *info)
{
    struct channel_info *p;
    FH_UINT32 num = sizeof(g_channel_infos)/sizeof(g_channel_infos[0]);

    p = g_channel_infos;
    while (num > 0)
    {
        if (p->channel == chan_vpu)
        {
            /*detect invalid max_width & max_height, and correct it...*/
            if (p->max_width < p->width)
            {
                p->max_width = p->width;
            }
            if (p->max_height < p->height)
            {
                p->max_height = p->height;
            }

            *info = *p;
            return 0;
        }

        p++;
        num--;
    }

    return -1;
}

FH_SINT32 sample_common_dsp_h264_opened(FH_VOID)
{
    FH_UINT32 i;
    FH_UINT32 num = sizeof(g_channel_infos)/sizeof(g_channel_infos[0]);

    for (i=0; i<num; i++)
    {
        if (g_channel_infos[i].enc_type == FH_NORMAL_H264 ||
                g_channel_infos[i].enc_type == FH_SMART_H264)
            return 1;
    }

    return 0;
}

FH_SINT32 sample_common_dsp_h265_opened(FH_VOID)
{
    FH_UINT32 i;
    FH_UINT32 num = sizeof(g_channel_infos)/sizeof(g_channel_infos[0]);

    for (i=0; i<num; i++)
    {
        if (g_channel_infos[i].enc_type == FH_NORMAL_H265 ||
                g_channel_infos[i].enc_type == FH_SMART_H265)
            return 1;
    }

    return 0;
}

FH_VOID sample_common_media_driver_config(FH_VOID)
{
    FH_SINT32 ret;
    FH_CHAR   vpu_cmd[48];
    FH_UINT32 vi_w;
    FH_UINT32 vi_h;
    FH_SINT32 channel;
    FH_SINT32 channel_max;
    struct channel_info info;

    vi_w = sample_isp_get_max_w();
    vi_h = sample_isp_get_max_h();
    snprintf(vpu_cmd, sizeof(vpu_cmd), "vi_%d_%d", vi_w, vi_h);
    vpu_write_proc(vpu_cmd);

    channel_max = sample_common_dsp_get_max_channel_index();
    for (channel = 0; channel <= channel_max; channel++)
    {
        ret = sample_common_dsp_get_channel_info(channel, &info);
        if (ret != 0 || !info.enable)
            continue;

        snprintf(vpu_cmd, sizeof(vpu_cmd), "cap_%d_%d_%d", channel, ALIGNTO(info.max_width, 16), ALIGNTO(info.max_height, 16));

        vpu_write_proc(vpu_cmd);

        if (info.breath_on)
        {
            snprintf(vpu_cmd, sizeof(vpu_cmd), "buf_%d_3", channel);
            vpu_write_proc(vpu_cmd);

            if (channel == 0)
            {
                enc_write_proc("depri_1");
            }
        }
    }

#ifdef FH_APP_OPEN_NN_OBJECT_DETECT
    vpu_write_proc("cap_2_1920_1080");
#endif
#ifdef FH_APP_OPEN_NN_DETECT
    vpu_write_proc("cap_3_512_288");
#endif
}

FH_SINT32 sample_common_media_sys_init(FH_VOID)
{
    return FH_SYS_Init();
}

FH_SINT32 sample_common_media_sys_exit(FH_VOID)
{
    return FH_SYS_Exit();
}

FH_SINT32 sample_common_dsp_init(FH_VOID)
{
    FH_SINT32 ret;
    FH_VPU_SIZE vi_pic;

    vi_pic.vi_size.u32Width  = sample_isp_get_vi_w();
    vi_pic.vi_size.u32Height = sample_isp_get_vi_h();
    ret = FH_VPSS_SetViAttr(&vi_pic);
    if (ret != RETURN_OK)
    {
        return ret;
    }

    return FH_VPSS_Enable(VPU_MODE_ISP);
}


FH_SINT32 sample_common_vpu_create_chan(FH_UINT32 chan_vpu, FH_UINT32 width, FH_UINT32 height, FH_UINT32 yuv_type)
{
    FH_SINT32 ret;
    FH_VPU_CHN_CONFIG chn_attr;

    chn_attr.vpu_chn_size.u32Width  = width;
    chn_attr.vpu_chn_size.u32Height = height;
    ret = FH_VPSS_SetChnAttr(chan_vpu, &chn_attr);
    if (ret != RETURN_OK)
    {
        return ret;
    }

    ret = FH_VPSS_SetVOMode(chan_vpu, yuv_type);
    if (ret != RETURN_OK)
    {
        return ret;
    }

    return FH_VPSS_OpenChn(chan_vpu);
}

FH_SINT32 sample_common_vpu_bind_to_enc(FH_UINT32 chan_vpu, FH_UINT32 chan_enc)
{
    FH_SINT32 ret;

    ret = FH_VENC_StartRecvPic(chan_enc);
    if (ret != RETURN_OK)
    {
        return ret;
    }

    return FH_SYS_BindVpu2Enc(chan_vpu, chan_enc);
}

/* 设置码控类型和参数 */

static FH_SINT32 sample_common_set_rc_type(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    FH_SINT32 ret = 0;
    FH_UINT32 rc_type = info->rc_type;

    switch (rc_type)
    {
        case FH_RC_H264_FIXQP:
        {
            sample_common_set_h264_rc_fixqp(info, cfg);
            break;
        }
        case FH_RC_H264_VBR:
        {
            sample_common_set_h264_rc_vbr(info, cfg);
            break;
        }
        case FH_RC_H264_CBR:
        {
            sample_common_set_h264_rc_cbr(info, cfg);
            break;
        }
        case FH_RC_H264_AVBR:
        {
            sample_common_set_h264_rc_avbr(info, cfg);
            break;
        }
        case FH_RC_H264_CVBR:
        {
            sample_common_set_h264_rc_cvbr(info, cfg);
            break;
        }
#if defined(CONFIG_ARCH_YG) || defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
        case FH_RC_H265_FIXQP:
        {
            sample_common_set_h265_rc_fixqp(info, cfg);
            break;
        }
        case FH_RC_H265_VBR:
        {
            sample_common_set_h265_rc_vbr(info, cfg);
            break;
        }
        case FH_RC_H265_CBR:
        {
            sample_common_set_h265_rc_cbr(info, cfg);
            break;
        }
        case FH_RC_H265_CVBR:
        {
            sample_common_set_h265_rc_cvbr(info, cfg);
            break;
        }
        case FH_RC_H265_AVBR:
        {
            sample_common_set_h265_rc_avbr(info, cfg);
            break;
        }
#endif
#if defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
        case FH_RC_H264_QVBR:
        {
            sample_common_set_h264_rc_qvbr(info, cfg);
            break;
        }
        case FH_RC_H265_QVBR:
        {
            sample_common_set_h265_rc_qvbr(info, cfg);
            break;
        }
#endif
        case FH_RC_MJPEG_FIXQP:
        {
            sample_common_set_mjpeg_rc_fixqp(info, cfg);
            break;
        }
        case FH_RC_MJPEG_VBR:
        {
            sample_common_set_mjpeg_rc_vbr(info, cfg);
            break;
        }
        default:
        {
            ret = -1;
            break;
        }
    }

    return ret;
}

/* 设置H264编码通道 */
static FH_SINT32 sample_common_set_h264_chan(FH_UINT32 chan, struct channel_info *info)
{
    FH_VENC_CHN_CONFIG cfg_param;

    cfg_param.chn_attr.enc_type                      = FH_NORMAL_H264;
    cfg_param.chn_attr.h264_attr.profile             = H264_PROFILE_MAIN;
    cfg_param.chn_attr.h264_attr.i_frame_intterval   = 50;
    cfg_param.chn_attr.h264_attr.size.u32Width       = info->width;
    cfg_param.chn_attr.h264_attr.size.u32Height      = info->height;

    if (sample_common_set_rc_type(info, &cfg_param))
    {
        sample_common_set_h264_rc_vbr(info, &cfg_param);
    }

    return  FH_VENC_SetChnAttr(chan, &cfg_param);
}

/* 设置H264智能编码通道 */
static FH_SINT32 sample_common_set_s264_chan(FH_UINT32 chan, struct channel_info *info)
{
    FH_VENC_CHN_CONFIG cfg_param;

    cfg_param.chn_attr.enc_type                          = FH_SMART_H264;
    cfg_param.chn_attr.h264_attr.profile                 = H264_PROFILE_MAIN;
    cfg_param.chn_attr.s264_attr.refresh_frame_intterval = 50;
    cfg_param.chn_attr.s264_attr.size.u32Width           = info->width;
    cfg_param.chn_attr.s264_attr.size.u32Height          = info->height;
    cfg_param.chn_attr.s264_attr.smart_en                = FH_TRUE;
    cfg_param.chn_attr.s264_attr.texture_en              = FH_TRUE;
    cfg_param.chn_attr.s264_attr.backgroudmodel_en       = FH_TRUE;
    cfg_param.chn_attr.s264_attr.mbconsist_en            = FH_FALSE;

    cfg_param.chn_attr.s264_attr.gop_th.GOP_TH_NUM       = 4;
    cfg_param.chn_attr.s264_attr.gop_th.TH_VAL[0]        = 8;
    cfg_param.chn_attr.s264_attr.gop_th.TH_VAL[1]        = 15;
    cfg_param.chn_attr.s264_attr.gop_th.TH_VAL[2]        = 25;
    cfg_param.chn_attr.s264_attr.gop_th.TH_VAL[3]        = 35;
    cfg_param.chn_attr.s264_attr.gop_th.MIN_GOP[0]       = 380;
    cfg_param.chn_attr.s264_attr.gop_th.MIN_GOP[1]       = 330;
    cfg_param.chn_attr.s264_attr.gop_th.MIN_GOP[2]       = 270;
    cfg_param.chn_attr.s264_attr.gop_th.MIN_GOP[3]       = 220;
    cfg_param.chn_attr.s264_attr.gop_th.MIN_GOP[4]       = 160;

    if (sample_common_set_rc_type(info, &cfg_param))
    {
        sample_common_set_h264_rc_vbr(info, &cfg_param);
    }

    return FH_VENC_SetChnAttr(chan, &cfg_param);
}

#if defined(CONFIG_ARCH_YG) || defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
/* 设置H265编码通道 */
static FH_SINT32 sample_common_set_h265_chan(FH_UINT32 chan, struct channel_info *info)
{
    FH_VENC_CHN_CONFIG cfg_param;

    cfg_param.chn_attr.enc_type                      = FH_NORMAL_H265;
    cfg_param.chn_attr.h265_attr.profile             = H265_PROFILE_MAIN;
    cfg_param.chn_attr.h265_attr.i_frame_intterval   = 50;
    cfg_param.chn_attr.h265_attr.size.u32Width       = info->width;
    cfg_param.chn_attr.h265_attr.size.u32Height      = info->height;

    if (sample_common_set_rc_type(info, &cfg_param))
    {
        sample_common_set_h265_rc_vbr(info, &cfg_param);
    }

    return FH_VENC_SetChnAttr(chan, &cfg_param);
}

/* 设置H265智能编码通道 */
static FH_SINT32 sample_common_set_s265_chan(FH_UINT32 chan, struct channel_info *info)
{
    FH_VENC_CHN_CONFIG cfg_param;

    cfg_param.chn_attr.enc_type                          = FH_SMART_H265;
    cfg_param.chn_attr.s265_attr.profile                 = H265_PROFILE_MAIN;
    cfg_param.chn_attr.s265_attr.refresh_frame_intterval = 50;
    cfg_param.chn_attr.s265_attr.size.u32Width           = info->width;
    cfg_param.chn_attr.s265_attr.size.u32Height          = info->height;
    cfg_param.chn_attr.s265_attr.smart_en                = FH_TRUE;
    cfg_param.chn_attr.s265_attr.texture_en              = FH_TRUE;
    cfg_param.chn_attr.s265_attr.backgroudmodel_en       = FH_TRUE;
    cfg_param.chn_attr.s265_attr.mbconsist_en            = FH_FALSE;

    cfg_param.chn_attr.s265_attr.gop_th.GOP_TH_NUM       = 4;
    cfg_param.chn_attr.s265_attr.gop_th.TH_VAL[0]        = 8;
    cfg_param.chn_attr.s265_attr.gop_th.TH_VAL[1]        = 15;
    cfg_param.chn_attr.s265_attr.gop_th.TH_VAL[2]        = 25;
    cfg_param.chn_attr.s265_attr.gop_th.TH_VAL[3]        = 35;
    cfg_param.chn_attr.s265_attr.gop_th.MIN_GOP[0]       = 380;
    cfg_param.chn_attr.s265_attr.gop_th.MIN_GOP[1]       = 330;
    cfg_param.chn_attr.s265_attr.gop_th.MIN_GOP[2]       = 270;
    cfg_param.chn_attr.s265_attr.gop_th.MIN_GOP[3]       = 220;
    cfg_param.chn_attr.s265_attr.gop_th.MIN_GOP[4]       = 160;

    if (sample_common_set_rc_type(info, &cfg_param))
    {
        sample_common_set_h265_rc_vbr(info, &cfg_param);
    }

    return FH_VENC_SetChnAttr(chan, &cfg_param);
}
#endif

static FH_SINT32 sample_common_set_mjpeg_chan(FH_UINT32 chan, struct channel_info *info)
{
    int ret;
    FH_VENC_CHN_CONFIG cfg_param;

    cfg_param.chn_attr.enc_type = FH_MJPEG;
    cfg_param.chn_attr.mjpeg_attr.pic_size.u32Width     = info->width;
    cfg_param.chn_attr.mjpeg_attr.pic_size.u32Height    = info->height;
    cfg_param.chn_attr.mjpeg_attr.rotate                = 0;
    cfg_param.chn_attr.mjpeg_attr.encode_speed          = 4;/* 0-9 */

    if (sample_common_set_rc_type(info, &cfg_param))
    {
        sample_common_set_mjpeg_rc_fixqp(info, &cfg_param);
    }

    ret = FH_VENC_SetChnAttr(chan, &cfg_param);
    if (ret != RETURN_OK)
    {
        printf("Error: FH_VENC_SetChnAttr failed with %d\n", ret);
        return -1;
    }
    return 0;
}

static FH_SINT32 sample_common_set_jpeg_chan(FH_UINT32 chan, struct channel_info *info)
{
    FH_VENC_CHN_CONFIG enc_cfg_param;

    enc_cfg_param.chn_attr.enc_type               = FH_JPEG;
    enc_cfg_param.chn_attr.jpeg_attr.qp           = 50;
    enc_cfg_param.chn_attr.jpeg_attr.rotate       = 0;
    enc_cfg_param.chn_attr.jpeg_attr.encode_speed = 4; /* 大幅面下编码器性能不够时，设置为0 */

    return FH_VENC_SetChnAttr(chan, &enc_cfg_param);
}

FH_SINT32 sample_common_enc_create_chan(FH_UINT32 chan_enc, struct channel_info *info)
{
    FH_VENC_CHN_CAP cfg_vencmem;

    cfg_vencmem.support_type       = info->enc_type;
    cfg_vencmem.max_size.u32Width  = info->max_width;
    cfg_vencmem.max_size.u32Height = info->max_height;

    return FH_VENC_CreateChn(chan_enc, &cfg_vencmem);
}

static FH_SINT32 sample_common_enc_set_breath(FH_UINT32 chan, FH_UINT16 breath_en)
{
#ifdef CONFIG_ARCH_FH8626V100
    return 0;
#else
    FH_DEBREATH breath;

    breath.debreath_en = breath_en;
    breath.debreath_ratio = 16;

    return FH_VENC_SetDeBreathEffect(chan, &breath);
#endif
}

FH_SINT32 sample_common_enc_set_chan(FH_UINT32 chan_enc, struct channel_info *info)
{
    FH_SINT32 ret;

    switch (info->enc_type)
    {
        case FH_NORMAL_H264:
            ret = sample_common_set_h264_chan(chan_enc, info);
            break;

        case FH_SMART_H264:
            ret = sample_common_set_s264_chan(chan_enc, info);
            break;

#if defined(CONFIG_ARCH_YG) || defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
        case FH_NORMAL_H265:
            ret = sample_common_set_h265_chan(chan_enc, info);
            break;

        case FH_SMART_H265:
            ret = sample_common_set_s265_chan(chan_enc, info);
            break;
#endif

        case FH_MJPEG:
            ret = sample_common_set_mjpeg_chan(chan_enc, info);
            break;

        case FH_JPEG:
            ret = sample_common_set_jpeg_chan(chan_enc, info);
            break;

        default:
            ret = 0;
            break;
    }

    if (ret)
    {
        printf("Error: set encode chan failed %d(%x)\n", ret, ret);
        return ret;
    }

    if (info->enc_type != FH_JPEG && info->enc_type != FH_MJPEG)
    {
        ret = sample_common_enc_set_breath(chan_enc, info->breath_on);
    }

    return ret;
}

FH_SINT32 sample_common_start_get_stream(FH_VOID)
{
    pthread_attr_t attr;
    pthread_t thread_stream;
    struct sched_param param;

    if (!g_get_stream_running)
    {
        g_get_stream_running = 1;
        g_get_stream_stop = 0;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_attr_setstacksize(&attr, 3 * 1024);
#ifdef __RTTHREAD_OS__
        param.sched_priority = 130;
#endif
        pthread_attr_setschedparam(&attr, &param);
        pthread_create(&thread_stream, &attr, sample_common_get_stream_proc, &g_get_stream_stop);
    }

    return 0;
}

FH_SINT32  sample_common_stop_stream_proc(FH_VOID)
{
    if (g_get_stream_running)
    {
        /*让获取码流的线程退出,置退出标记*/
        g_get_stream_stop = 1;

        /*等待获取码流的线程退出*/
        while (g_get_stream_stop != 0)
            usleep(10*1000);

        g_get_stream_running = 0;
    }

    return 0;
}
