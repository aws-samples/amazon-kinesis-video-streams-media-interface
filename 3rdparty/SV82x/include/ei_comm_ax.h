/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_ax.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __EI_COMM_AX_H__
#define __EI_COMM_AX_H__

#include "ei_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#define EI_TRACE_AX(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_AXNU, level, fmt, ##args);\
} while(0)

typedef struct eiAXNU_DBG_PERF_CNT_S {
    EI_U32 mtiks;
    EI_U32 mrbw;
    EI_U32 mwbw;
} AXNU_DBG_PERF_CNT_S;

typedef struct eiAXNU_STAT_INFO_S {
    AXNU_DBG_PERF_CNT_S mstat;
    EI_U32 mboxes;
    EI_S32 status;
} AXNU_STAT_INFO_S;

typedef struct eiAXNU_CTL_S {
    EI_U32 mFstCmdLen;
    EI_U32 mCmdNum;
    EI_U32 mCmdInt[4];
    EI_U32 mCmdPAUS[4];
    EI_U32 mISREn;
    EI_U32 mCmdAddr;
    EI_U32 mInputAddr;
    EI_U32 mInternalAddr;
    EI_U32 mOutputAddr;
    EI_U32 mOcmEn;
    EI_U32 mOcmAddr;
    EI_U32 mWarpInAlign64;
} AXNU_CTL_S;

typedef struct eiAXNU_STATUS_OUT_S {
    EI_S32 err_no;
    EI_U32 mCmdInt[4];
} AXNU_STATUS_OUT_S;

typedef struct eiAXNU_PAUSE_FLAG_S {
    EI_U32 mCmdPAUS[4];
} AXNU_PAUSE_FLAG_S;

typedef struct eiAX_REG_S {
    EI_U32 reg_addr;
    EI_U32 reg_val;
} AX_REG_S;

typedef struct eiAXVU_DBG_PERF_CNT_S {
    EI_U32 mtiks;
    EI_U32 mrbw;
    EI_U32 mwbw;
} AXVU_DBG_PERF_CNT_S;

typedef struct eiAXVU_STAT_INFO_S {
    AXVU_DBG_PERF_CNT_S mstat;
} AXVU_STAT_INFO_S;

typedef struct eiAXVU_RECT_S {
    EI_S32 x;
    EI_S32 y;
    EI_S32 w;
    EI_S32 h;
} AXVU_RECT_S;

typedef struct eiAXVU_RSZ_CFG_S{
	EI_S32 iw;
	EI_S32 ih;

	EI_S32 oft_x;
	EI_S32 oft_y;
	EI_S32 crop_w;
	EI_S32 crop_h;

	EI_S32 yuv_limit_range;//1: limit 601, 0: full 601
	EI_S32 istride;
	EI_S32 fmt_in;
	EI_S32 bw_in; // 16bit-gray, 8bit-rgb
	EI_S32 bw_out;// 16bit-rgb, 8bit-rgb
	EI_U32 phyaddrin;
	EI_U32 phyaddrin_uv;
	EI_U32 phyaddr2rd;
	AXVU_RECT_S hist_rect;
	EI_S32 in_align_byte;
	EI_S32 fmt_out;
	EI_S32 ddr_type;

	EI_S32 ow;
	EI_S32 oh;
	EI_S32 dst_stride;
	EI_U32 phyaddrout;//TBD unsigned long long
	EI_U32 phyaddrout_uv;//TBD unsigned long long
	EI_U32 phyaddrhist;
	EI_S32 merge_mode;
	EI_S32 hist_en;
} AXVU_RSZ_CFG_S;

typedef struct eiAXVU_MBG_CFG_S{
	EI_S32 iw;
	EI_S32 ih;

	EI_S32 cp_en;
	EI_S32 cp_cnt;
	EI_S32 pool_w;
	EI_S32 pool_h;
	EI_S32 thr_en;
	EI_S32 thr;
	EI_S32 alpha;
	EI_S32 lstride;
	EI_S32 reset_flg;//scene change
	EI_S32 msk_thr;
	EI_U32 phyaddrin;
	EI_U32 phyaddrin_bg;
	EI_U32 phyaddrout_bg;
	EI_U32 phyaddrout_msk;
	EI_U32 phyaddr_cp;
} AXVU_MBG_CFG_S;


typedef struct eiAXVU_PYM_CFG_S {
    EI_S32 iw;
    EI_S32 ih;
    EI_S32 istride;
    EI_S32 fmt_in;
    EI_U32 phyaddrin;
    EI_U32 phyaddrin_uv;
    EI_S32 pym_layer;
    EI_S32 fmt_out;
    EI_S32 ow[8];
    EI_S32 oh[8];
    EI_U32 phyaddrout;
} AXVU_PYM_CFG_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
