#ifndef __CHN_SET_RC_H__
#define __CHN_SET_RC_H__

FH_VOID sample_common_set_h264_rc_fixqp(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h264_rc_cbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h264_rc_vbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h264_rc_avbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h264_rc_cvbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);

#if defined(CONFIG_ARCH_YG) || defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
FH_VOID sample_common_set_h265_rc_fixqp(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h265_rc_cbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h265_rc_vbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h265_rc_avbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h265_rc_cvbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
#endif

#if defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
FH_VOID sample_common_set_h264_rc_qvbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_h265_rc_qvbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
#endif

FH_VOID sample_common_set_mjpeg_rc_fixqp(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);
FH_VOID sample_common_set_mjpeg_rc_vbr(struct channel_info *info, FH_VENC_CHN_CONFIG *cfg);

#endif /* __CHN_SET_RC_H__ */
