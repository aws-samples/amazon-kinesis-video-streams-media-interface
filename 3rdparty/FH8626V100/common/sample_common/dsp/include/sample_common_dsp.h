#ifndef __SAMPLE_COMMON_DSP_H__
#define __SAMPLE_COMMON_DSP_H__

#include "dsp/fh_system_mpi.h"
#include "dsp/fh_vpu_mpi.h"
#include "dsp/fh_venc_mpi.h"

#define ALIGNTO(addr, edge)  ((addr + edge - 1) & ~(edge - 1)) /* 数据结构对齐定义 */

struct channel_info
{
    FH_SINT32 channel;
    FH_SINT32 enable;
    FH_UINT32 width;
    FH_UINT32 height;
    FH_UINT32 max_width;
    FH_UINT32 max_height;
    FH_UINT32 yuv_type;
    FH_UINT8 frame_count;
    FH_UINT8 frame_time;
    FH_UINT32 bps;
    FH_UINT32 enc_type;
    FH_UINT32 rc_type;
    FH_UINT16 breath_on;
};

extern FH_SINT32 sample_common_dsp_get_max_channel_index(FH_VOID);
extern FH_SINT32 sample_common_dsp_get_channel_info(FH_UINT32 chan_vpu, struct channel_info *info);
extern FH_SINT32 sample_common_dsp_h264_opened(FH_VOID);
extern FH_SINT32 sample_common_dsp_h265_opened(FH_VOID);

extern FH_VOID   sample_common_media_driver_config(FH_VOID);
extern FH_SINT32 sample_common_media_sys_init(FH_VOID);
extern FH_SINT32 sample_common_media_sys_exit(FH_VOID);
extern FH_SINT32 sample_common_dsp_init(FH_VOID);
extern FH_SINT32 sample_common_vpu_create_chan(FH_UINT32 chan_vpu, FH_UINT32 width, FH_UINT32 height, FH_UINT32 yuv_type);
extern FH_SINT32 sample_common_vpu_bind_to_enc(FH_UINT32 chan_vpu, FH_UINT32 chan_enc);

extern FH_SINT32 sample_common_enc_create_chan(FH_UINT32 chan_enc, struct channel_info *info);
extern FH_SINT32 sample_common_enc_set_chan(FH_UINT32 chan_enc, struct channel_info *info);

extern FH_SINT32 sample_common_start_get_stream(FH_VOID);
extern FH_SINT32 sample_common_stop_stream_proc(FH_VOID);

extern FH_SINT32 sample_dmc_init(FH_CHAR *dst_ip, FH_UINT32 port ,FH_SINT32 max_channel_no);
extern FH_SINT32 sample_dmc_deinit(FH_VOID);

#endif /*__SAMPLE_COMMON_DSP_H__*/
