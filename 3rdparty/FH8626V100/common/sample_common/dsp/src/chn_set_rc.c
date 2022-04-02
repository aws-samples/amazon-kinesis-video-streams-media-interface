#include <sample_common.h>
#include "chn_set_rc.h"

#define H264_BREATH_IP_QPDELTA 5
#define H264_BREATH_I_BITPROP  15
#define H264_BREATH_P_BITPROP  1

#define H265_BREATH_IP_QPDELTA 3
#define H265_BREATH_I_BITPROP  10
#define H265_BREATH_P_BITPROP  1

FH_VOID sample_common_set_h264_rc_fixqp(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H264_FIXQP;
    cfg->rc_attr.h264_fixqp.iqp                   = 28;
    cfg->rc_attr.h264_fixqp.pqp                   = 28;
    cfg->rc_attr.h264_fixqp.FrameRate.frame_count = info->frame_count;
    cfg->rc_attr.h264_fixqp.FrameRate.frame_time  = info->frame_count;
}

FH_VOID sample_common_set_h264_rc_cbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H264_CBR;
    cfg->rc_attr.h264_cbr.bitrate                 = info->bps;
    cfg->rc_attr.h264_cbr.init_qp                 = 35;
    cfg->rc_attr.h264_cbr.FrameRate.frame_count   = info->frame_count;
    cfg->rc_attr.h264_cbr.FrameRate.frame_time    = info->frame_time;
    cfg->rc_attr.h264_cbr.maxrate_percent         = 200;
    cfg->rc_attr.h264_cbr.IFrmMaxBits             = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h264_cbr.IP_QPDelta          = H264_BREATH_IP_QPDELTA;
        cfg->rc_attr.h264_cbr.I_BitProp           = H264_BREATH_I_BITPROP;
        cfg->rc_attr.h264_cbr.P_BitProp           = H264_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h264_cbr.IP_QPDelta          = 3;
        cfg->rc_attr.h264_cbr.I_BitProp           = 5;
        cfg->rc_attr.h264_cbr.P_BitProp           = 1;
    }

    cfg->rc_attr.h264_cbr.fluctuate_level         = 0;
}

FH_VOID sample_common_set_h264_rc_vbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H264_VBR;
    cfg->rc_attr.h264_vbr.bitrate                 = info->bps;
    cfg->rc_attr.h264_vbr.init_qp                 = 35;
    cfg->rc_attr.h264_vbr.ImaxQP                  = 50;
    cfg->rc_attr.h264_vbr.IminQP                  = 28;
    cfg->rc_attr.h264_vbr.PmaxQP                  = 50;
    cfg->rc_attr.h264_vbr.PminQP                  = 28;
    cfg->rc_attr.h264_vbr.FrameRate.frame_count   = info->frame_count;
    cfg->rc_attr.h264_vbr.FrameRate.frame_time    = info->frame_time;
    cfg->rc_attr.h264_vbr.maxrate_percent         = 200;
    cfg->rc_attr.h264_vbr.IFrmMaxBits             = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h264_vbr.IP_QPDelta          = H264_BREATH_IP_QPDELTA;
        cfg->rc_attr.h264_vbr.I_BitProp           = H264_BREATH_I_BITPROP;
        cfg->rc_attr.h264_vbr.P_BitProp           = H264_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h264_vbr.IP_QPDelta          = 3;
        cfg->rc_attr.h264_vbr.I_BitProp           = 5;
        cfg->rc_attr.h264_vbr.P_BitProp           = 1;
    }
    cfg->rc_attr.h264_vbr.fluctuate_level         = 0;
}

FH_VOID sample_common_set_h264_rc_avbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H264_AVBR;
    cfg->rc_attr.h264_avbr.bitrate                = info->bps;
    cfg->rc_attr.h264_avbr.init_qp                = 35;
    cfg->rc_attr.h264_avbr.ImaxQP                 = 50;
    cfg->rc_attr.h264_avbr.IminQP                 = 28;
    cfg->rc_attr.h264_avbr.PmaxQP                 = 50;
    cfg->rc_attr.h264_avbr.PminQP                 = 28;
    cfg->rc_attr.h264_avbr.FrameRate.frame_count  = info->frame_count;
    cfg->rc_attr.h264_avbr.FrameRate.frame_time   = info->frame_time;
    cfg->rc_attr.h264_avbr.maxrate_percent        = 200;
    cfg->rc_attr.h264_avbr.IFrmMaxBits            = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h264_avbr.IP_QPDelta         = H264_BREATH_IP_QPDELTA;
        cfg->rc_attr.h264_avbr.I_BitProp          = H264_BREATH_I_BITPROP;
        cfg->rc_attr.h264_avbr.P_BitProp          = H264_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h264_avbr.IP_QPDelta         = 3;
        cfg->rc_attr.h264_avbr.I_BitProp          = 5;
        cfg->rc_attr.h264_avbr.P_BitProp          = 1;
    }
    cfg->rc_attr.h264_avbr.fluctuate_level        = 0;
    cfg->rc_attr.h264_avbr.stillrate_percent      = 30;
    cfg->rc_attr.h264_avbr.maxstillqp             = 34;
}

FH_VOID sample_common_set_h264_rc_cvbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H264_CVBR;
    cfg->rc_attr.h264_cvbr.ltbitrate              = info->bps;
    cfg->rc_attr.h264_cvbr.init_qp                = 35;
    cfg->rc_attr.h264_cvbr.ImaxQP                 = 50;
    cfg->rc_attr.h264_cvbr.IminQP                 = 28;
    cfg->rc_attr.h264_cvbr.PmaxQP                 = 50;
    cfg->rc_attr.h264_cvbr.PminQP                 = 28;
    cfg->rc_attr.h264_cvbr.FrameRate.frame_count  = info->frame_count;
    cfg->rc_attr.h264_cvbr.FrameRate.frame_time   = info->frame_time;
    cfg->rc_attr.h264_cvbr.maxrate_percent        = 200;
    cfg->rc_attr.h264_cvbr.IFrmMaxBits            = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h264_cvbr.IP_QPDelta         = H264_BREATH_IP_QPDELTA;
        cfg->rc_attr.h264_cvbr.I_BitProp          = H264_BREATH_I_BITPROP;
        cfg->rc_attr.h264_cvbr.P_BitProp          = H264_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h264_cvbr.IP_QPDelta         = 3;
        cfg->rc_attr.h264_cvbr.I_BitProp          = 5;
        cfg->rc_attr.h264_cvbr.P_BitProp          = 1;
    }
    cfg->rc_attr.h264_cvbr.fluctuate_level        = 0;
    cfg->rc_attr.h264_cvbr.stbitrate              = info->bps * 2;
    cfg->rc_attr.h264_cvbr.acceptable_qp          = 32;
}

#if defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
FH_VOID sample_common_set_h264_rc_qvbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H264_QVBR;
    cfg->rc_attr.h264_qvbr.bitrate                = info->bps;
    cfg->rc_attr.h264_qvbr.init_qp                = 35;
    cfg->rc_attr.h264_qvbr.ImaxQP                 = 50;
    cfg->rc_attr.h264_qvbr.IminQP                 = 28;
    cfg->rc_attr.h264_qvbr.PmaxQP                 = 50;
    cfg->rc_attr.h264_qvbr.PminQP                 = 28;
    cfg->rc_attr.h264_qvbr.FrameRate.frame_count  = info->frame_count;
    cfg->rc_attr.h264_qvbr.FrameRate.frame_time   = info->frame_time;
    cfg->rc_attr.h264_qvbr.maxrate_percent        = 200;
    cfg->rc_attr.h264_qvbr.IFrmMaxBits            = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h264_qvbr.IP_QPDelta         = H264_BREATH_IP_QPDELTA;
        cfg->rc_attr.h264_qvbr.I_BitProp          = H264_BREATH_I_BITPROP;
        cfg->rc_attr.h264_qvbr.P_BitProp          = H264_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h264_qvbr.IP_QPDelta         = 3;
        cfg->rc_attr.h264_qvbr.I_BitProp          = 5;
        cfg->rc_attr.h264_qvbr.P_BitProp          = 1;
    }
    cfg->rc_attr.h264_qvbr.fluctuate_level        = 0;
    cfg->rc_attr.h264_qvbr.minrate_percent        = 30;
    cfg->rc_attr.h264_qvbr.psnr_ul                = 3200;
    cfg->rc_attr.h264_qvbr.psnr_ll                = 2900;
}
#endif

#if defined(CONFIG_ARCH_YG) || defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
FH_VOID sample_common_set_h265_rc_fixqp(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H265_FIXQP;
    cfg->rc_attr.h265_fixqp.iqp                   = 28;
    cfg->rc_attr.h265_fixqp.qp                    = 28;
    cfg->rc_attr.h265_fixqp.FrameRate.frame_count = info->frame_count;
    cfg->rc_attr.h265_fixqp.FrameRate.frame_time  = info->frame_time;
}

FH_VOID sample_common_set_h265_rc_cbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H265_CBR;
    cfg->rc_attr.h265_cbr.bitrate                 = info->bps;
    cfg->rc_attr.h265_cbr.init_qp                 = 35;
    cfg->rc_attr.h265_cbr.FrameRate.frame_count   = info->frame_count;
    cfg->rc_attr.h265_cbr.FrameRate.frame_time    = info->frame_time;
    cfg->rc_attr.h265_cbr.maxrate_percent         = 200;
    cfg->rc_attr.h265_cbr.IFrmMaxBits             = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h265_cbr.IP_QPDelta          = H265_BREATH_IP_QPDELTA;
        cfg->rc_attr.h265_cbr.I_BitProp           = H265_BREATH_I_BITPROP;
        cfg->rc_attr.h265_cbr.P_BitProp           = H265_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h265_cbr.IP_QPDelta          = 3;
        cfg->rc_attr.h265_cbr.I_BitProp           = 5;
        cfg->rc_attr.h265_cbr.P_BitProp           = 1;
    }
    cfg->rc_attr.h265_cbr.fluctuate_level         = 0;
}

FH_VOID sample_common_set_h265_rc_vbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H265_VBR;
    cfg->rc_attr.h265_vbr.bitrate                 = info->bps;
    cfg->rc_attr.h265_vbr.init_qp                 = 35;
    cfg->rc_attr.h265_vbr.ImaxQP                  = 50;
    cfg->rc_attr.h265_vbr.IminQP                  = 28;
    cfg->rc_attr.h265_vbr.PmaxQP                  = 50;
    cfg->rc_attr.h265_vbr.PminQP                  = 28;
    cfg->rc_attr.h265_vbr.FrameRate.frame_count   = info->frame_count;
    cfg->rc_attr.h265_vbr.FrameRate.frame_time    = info->frame_time;
    cfg->rc_attr.h265_vbr.maxrate_percent         = 200;
    cfg->rc_attr.h265_vbr.IFrmMaxBits             = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h265_vbr.IP_QPDelta          = H265_BREATH_IP_QPDELTA;
        cfg->rc_attr.h265_vbr.I_BitProp           = H265_BREATH_I_BITPROP;
        cfg->rc_attr.h265_vbr.P_BitProp           = H265_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h265_vbr.IP_QPDelta          = 3;
        cfg->rc_attr.h265_vbr.I_BitProp           = 5;
        cfg->rc_attr.h265_vbr.P_BitProp           = 1;
    }
    cfg->rc_attr.h265_vbr.fluctuate_level         = 0;
}

FH_VOID sample_common_set_h265_rc_avbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H265_AVBR;
    cfg->rc_attr.h265_avbr.bitrate                = info->bps;
    cfg->rc_attr.h265_avbr.init_qp                = 35;
    cfg->rc_attr.h265_avbr.ImaxQP                 = 50;
    cfg->rc_attr.h265_avbr.IminQP                 = 28;
    cfg->rc_attr.h265_avbr.PmaxQP                 = 50;
    cfg->rc_attr.h265_avbr.PminQP                 = 28;
    cfg->rc_attr.h265_avbr.FrameRate.frame_count  = info->frame_count;
    cfg->rc_attr.h265_avbr.FrameRate.frame_time   = info->frame_time;
    cfg->rc_attr.h265_avbr.maxrate_percent        = 200;
    cfg->rc_attr.h265_avbr.IFrmMaxBits            = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h265_avbr.IP_QPDelta         = H265_BREATH_IP_QPDELTA;
        cfg->rc_attr.h265_avbr.I_BitProp          = H265_BREATH_I_BITPROP;
        cfg->rc_attr.h265_avbr.P_BitProp          = H265_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h265_avbr.IP_QPDelta         = 3;
        cfg->rc_attr.h265_avbr.I_BitProp          = 5;
        cfg->rc_attr.h265_avbr.P_BitProp          = 1;
    }
    cfg->rc_attr.h265_avbr.fluctuate_level        = 0;
    cfg->rc_attr.h265_avbr.stillrate_percent      = 30;
    cfg->rc_attr.h265_avbr.maxstillqp             = 34;
}

FH_VOID sample_common_set_h265_rc_cvbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H265_CVBR;
    cfg->rc_attr.h265_cvbr.ltbitrate              = info->bps;
    cfg->rc_attr.h265_cvbr.init_qp                = 35;
    cfg->rc_attr.h265_cvbr.ImaxQP                 = 50;
    cfg->rc_attr.h265_cvbr.IminQP                 = 28;
    cfg->rc_attr.h265_cvbr.PmaxQP                 = 50;
    cfg->rc_attr.h265_cvbr.PminQP                 = 28;
    cfg->rc_attr.h265_cvbr.FrameRate.frame_count  = info->frame_count;
    cfg->rc_attr.h265_cvbr.FrameRate.frame_time   = info->frame_time;
    cfg->rc_attr.h265_cvbr.maxrate_percent        = 200;
    cfg->rc_attr.h265_cvbr.IFrmMaxBits            = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h265_cvbr.IP_QPDelta         = H265_BREATH_IP_QPDELTA;
        cfg->rc_attr.h265_cvbr.I_BitProp          = H265_BREATH_I_BITPROP;
        cfg->rc_attr.h265_cvbr.P_BitProp          = H265_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h265_cvbr.IP_QPDelta         = 3;
        cfg->rc_attr.h265_cvbr.I_BitProp          = 5;
        cfg->rc_attr.h265_cvbr.P_BitProp          = 1;
    }
    cfg->rc_attr.h265_cvbr.fluctuate_level        = 0;
    cfg->rc_attr.h265_cvbr.stbitrate              = info->bps * 2;
    cfg->rc_attr.h265_cvbr.acceptable_qp          = 32;
}
#if defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
FH_VOID sample_common_set_h265_rc_qvbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                          = FH_RC_H265_QVBR;
    cfg->rc_attr.h265_qvbr.bitrate                = info->bps;
    cfg->rc_attr.h265_qvbr.init_qp                = 35;
    cfg->rc_attr.h265_qvbr.ImaxQP                 = 50;
    cfg->rc_attr.h265_qvbr.IminQP                 = 28;
    cfg->rc_attr.h265_qvbr.PmaxQP                 = 50;
    cfg->rc_attr.h265_qvbr.PminQP                 = 28;
    cfg->rc_attr.h265_qvbr.FrameRate.frame_count  = info->frame_count;
    cfg->rc_attr.h265_qvbr.FrameRate.frame_time   = info->frame_time;
    cfg->rc_attr.h265_qvbr.maxrate_percent        = 200;
    cfg->rc_attr.h265_qvbr.IFrmMaxBits            = 0;
    if (info->breath_on)
    {
        cfg->rc_attr.h265_qvbr.IP_QPDelta         = H265_BREATH_IP_QPDELTA;
        cfg->rc_attr.h265_qvbr.I_BitProp          = H265_BREATH_I_BITPROP;
        cfg->rc_attr.h265_qvbr.P_BitProp          = H265_BREATH_P_BITPROP;
    }
    else
    {
        cfg->rc_attr.h265_qvbr.IP_QPDelta         = 3;
        cfg->rc_attr.h265_qvbr.I_BitProp          = 5;
        cfg->rc_attr.h265_qvbr.P_BitProp          = 1;
    }
    cfg->rc_attr.h265_qvbr.fluctuate_level        = 0;
    cfg->rc_attr.h265_qvbr.minrate_percent        = 30;
    cfg->rc_attr.h265_qvbr.psnr_ul                = 3200;
    cfg->rc_attr.h265_qvbr.psnr_ll                = 2900;
}
#endif
#endif

FH_VOID sample_common_set_mjpeg_rc_fixqp(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                           = FH_RC_MJPEG_FIXQP;
    cfg->rc_attr.mjpeg_fixqp.qp                    = 50;
    cfg->rc_attr.mjpeg_fixqp.FrameRate.frame_count = info->frame_count;
    cfg->rc_attr.mjpeg_fixqp.FrameRate.frame_time  = info->frame_time;
}

FH_VOID sample_common_set_mjpeg_rc_vbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg)
{
    cfg->rc_attr.rc_type                         = FH_RC_MJPEG_VBR;
    cfg->rc_attr.mjpeg_vbr.initqp                = 50;
    cfg->rc_attr.mjpeg_vbr.bitrate               = info->bps;
    cfg->rc_attr.mjpeg_vbr.minqp                 = 30;
    cfg->rc_attr.mjpeg_vbr.maxqp                 = 80;
    cfg->rc_attr.mjpeg_vbr.FrameRate.frame_count = info->frame_count;
    cfg->rc_attr.mjpeg_vbr.FrameRate.frame_time  = info->frame_time;
}
