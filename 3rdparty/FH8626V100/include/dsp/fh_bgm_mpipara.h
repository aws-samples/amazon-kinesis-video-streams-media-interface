#ifndef __FH_BGM_MPIPARA_H__
#define __FH_BGM_MPIPARA_H__
/**|BGM|**/

#include "types/type_def.h"
#include "fh_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef struct
{
	FH_SIZE         size;       // 图像幅面  | [ ]
	FH_ADDR         YADDR;      // 亮度物理地址 | [ ]
	FH_ADDR         CADDR;      // 色度物理地址,参数无效 | [ ]
	FH_UINT64       time_stamp; // 时间戳  | [ ]
}FH_BGM_FRAME;

typedef struct
{
	FH_SIZE         size;                   //幅面大小 | [ ]
	FH_ADDR_INFO    confidence_level;       //置信度信息 | [ ]
	FH_ADDR_INFO    mb_consistency;         //一致性信息 | [ ]
	FH_UINT32       background_change_flag; //场景切换标识 | [ ]
	FH_UINT32       static_frm_flag;        //背景更新标识 | [ ]
	FH_UINT64       time_stamp;             //时间戳 | [ ]
}FH_BGM_SW_STATUS;

#define MAX_RUNTABLE_RECT 50
typedef struct
{
	FH_UINT32 rect_num;              //框数量 | [ ]
	FH_UINT32 base_w;                //框信息的基准幅面 | [ ]
	FH_UINT32 base_h;                //框信息的基准幅面 | [ ]
	FH_AREA rect[MAX_RUNTABLE_RECT]; //框信息 | [ ]
	FH_UINT64 timestamp;             //时间戳 | [ ]
}FH_BGM_RUNTB_RECT;

#define MAX_TH_NUM 5
typedef struct
{
	FH_UINT32 GOP_TH_NUM;            // GOP门限数量 | [0-5]
	FH_UINT32 TH_VAL[MAX_TH_NUM];    // 门限值,单位百分比,从小到大,代表从安静到运动 | [0-100]
	FH_UINT32 MIN_GOP[MAX_TH_NUM+1]; // 小于对应门限时最小的GOP长度  | [ ]
}FH_GOP_TH;

/*高级参数配置,不建议用户自行配置此参数*/

typedef struct
{
	FH_UINT32       foreground_point_num;
	FH_UINT32       sta_diff_point_num;
	FH_UINT32       frm_diff_point_num;
	FH_UINT32       fg_consist_mb_sum;
	FH_UINT32       bg_consist_mb_sum;
	FH_UINT32       fg_mb_diff_sum;
	FH_UINT32       bg_mb_diff_sum;
	FH_UINT32       valid_mb_num;
	FH_UINT32       bg_point_num[2];
	FH_UINT32       darken_point_num[2];
	FH_UINT32       brighten_point_num[2];
	FH_UINT32       cur_luma_sum[2];
	FH_UINT32       bg_luma_sum[2];
	FH_ADDR_INFO    gau_dis_addr;
	FH_ADDR_INFO    gau_bg_re_pic_addr;
	FH_ADDR_INFO    run_num_addr;
	FH_ADDR_INFO    run0_pos_addr;
	FH_ADDR_INFO    run1_pos_addr;
	FH_ADDR_INFO    diff_addr;
	FH_UINT64       time_stamp;
}FH_BGM_HW_STATUS;

typedef struct
{
	FH_UINT8 refresh_rate;
	FH_UINT8 bkg_th0;
	FH_UINT8 bkg_th1;
}FH_BKG_INIT_CFG;

typedef struct
{
	FH_UINT16 refresh_rate;
	FH_UINT8  precision;
	FH_UINT8  variance_gain;
	FH_UINT8  variance_min;
	FH_UINT8  variance_max;
}FH_SINGLE_GAU_CFG;

typedef struct
{
	FH_UINT16 refresh_rate;
	FH_UINT16 refresh_weight_rate;
	FH_UINT8  precision;
	FH_UINT8  variance_gain;
	FH_UINT8  variance_init;
	FH_UINT8  variance_min;
	FH_UINT8  variance_max;
	FH_UINT8  bg_weight_th;
	FH_UINT8  bg_time_th;
	FH_UINT8  sel_mode;
	FH_UINT8  time_update_en;
}FH_MULTI_GAU_CFG;

typedef struct
{
	FH_UINT16 refresh_rate;
	FH_UINT8  precision;
	FH_UINT8  variance_th;
	FH_UINT8  weight_th;
	FH_UINT8  time_th;
	FH_UINT8  freq_th;
	FH_UINT8  freq_time_th;
	FH_UINT8  freq_weight_th;
	FH_UINT8  freq_update_en;
}FH_DIS_CFG;

typedef enum
{
	FH_MULTI_GAU_FG_A  = 0,
	FH_MULTI_GAU_DIS   = 1,
	FH_MULTI_GAU_FG_B  = 2,
	FH_LONG_TERM_FG_A  = 3,
	FH_LONG_TERM_FG_B  = 4,
	FH_LONG_TERM_FG_A1 = 5,
	FH_LONG_TERM_FG_B1 = 6,
}FH_CON_EXPA_IDX;

typedef struct
{
	FH_UINT32 win_mask;
	FH_UINT8  expa_th;
	FH_UINT8  center_en;
}FH_CON_EXPA_CFG;

typedef struct
{
	FH_UINT8 diff_th0;
	FH_UINT8 diff_th1;
	FH_UINT8 longTermFgTh0;
	FH_UINT8 longTermFgTh1;
	FH_UINT8 sta_regre_val;
	FH_UINT8 sta_diff_num_th;
}FH_FRM_DIFF_CFG;

typedef struct
{
	FH_UINT8 bg_diff_th;
}FH_FRM_DIFF_CONSIST_CFG;

typedef struct
{
	FH_UINT8 bright_th0;
	FH_UINT8 bright_th1;
}FH_GLOBAL_BG_LUMA_CFG;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

