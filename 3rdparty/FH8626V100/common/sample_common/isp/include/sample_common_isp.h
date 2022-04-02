#ifndef __SAMPLE_COMMON_ISP_H__
#define __SAMPLE_COMMON_ISP_H__

#define SAMPLE_SENSOR_FLAG_NORMAL    (0x00)
#define SAMPLE_SENSOR_FLAG_NIGHT     (0x02)
#define SAMPLE_SENSOR_FLAG_WDR       (0x01)
#define SAMPLE_SENSOR_FLAG_WDR_NIGHT (SAMPLE_SENSOR_FLAG_NIGHT | SAMPLE_SENSOR_FLAG_WDR)

extern FH_SINT32 sample_isp_use_large_frame(FH_VOID);
extern FH_UINT32 sample_isp_get_vi_w(FH_VOID);
extern FH_UINT32 sample_isp_get_vi_h(FH_VOID);
extern FH_UINT32 sample_isp_get_max_w(FH_VOID);
extern FH_UINT32 sample_isp_get_max_h(FH_VOID);
extern FH_VOID sample_isp_change_resolution(FH_VOID);
extern FH_VOID   sample_isp_change_fps(FH_VOID);
extern FH_SINT32 sample_common_start_isp(FH_VOID);
extern FH_SINT32 sample_common_stop_isp(FH_VOID);

extern FH_SINT32 sample_common_start_coolview(FH_VOID *arg);
extern FH_SINT32 sample_common_stop_coolview(FH_VOID);

#endif /*__SAMPLE_COMMON_ISP_H__*/
