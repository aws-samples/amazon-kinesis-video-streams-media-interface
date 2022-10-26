/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_3a.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#ifndef __EI_COMM_3A_H__
#define __EI_COMM_3A_H__

#include "ei_common.h"
#include "ei_comm_isp.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IS_LINEAR_MODE(mode)      (WDR_MODE_NONE == (mode))
#define IS_WDR_MODE(mode)         ((!IS_LINEAR_MODE(mode)) && ((mode) < WDR_MODE_MAX))
#define IS_BUILDIN_WDR_MODE(mode) (WDR_MODE_BUILD_IN == (mode))
#define IS_FS_WDR_MODE(mode)      ((!IS_LINEAR_MODE(mode)) && (!IS_BUILDIN_WDR_MODE(mode)) && ((mode) < WDR_MODE_MAX))
#define IS_2TO1_WDR_MODE(mode)    (WDR_MODE_2TO1_FRAME == (mode))
#define IS_3TO1_WDR_MODE(mode)    (WDR_MODE_3TO1_FRAME == (mode))

#define ISP_3ALIB_MAX_REGISTER_NUM 2

#define ISP_BASE_GAIN        (1 << 8)

typedef enum eiISP_MODULE_E {
    ISP_MODULE_AE = 0,
    ISP_MODULE_AF,
    ISP_MODULE_AWB,
    ISP_MODULE_BLC,
    ISP_MODULE_DPC,
    ISP_MODULE_DEMOSAIC,
    ISP_MODULE_GAMMA,
    ISP_MODULE_GE,
    ISP_MODULE_SHARPEN,
    ISP_MODULE_WDR,
    ISP_MODULE_CAC,
    ISP_MODULE_CSC,
    ISP_MODULE_MLSC,
    ISP_MODULE_RLSC,
    ISP_MODULE_PFC,
    ISP_MODULE_BAYERNR,
    ISP_MODULE_TEMPER,
    ISP_MODULE_CNR,
    ISP_MODULE_FLICKER,
    ISP_MODULE_CLUT,
    ISP_MODULE_TNR,
    ISP_MODULE_EE,
    ISP_MODULE_LTM,
    ISP_MODULE_STITCH,
    ISP_MODULE_TC,
    ISP_MODULE_MAX,
} ISP_MODULE_E;

typedef enum eiISP_CTRL_CMD_E {
    ISP_AE_FPS_SET = 8000,

    ISP_AE_EXP_BIAS_SET,
    ISP_AE_EXP_BIAS_GET,
    ISP_AE_TARGET_SET,
    ISP_AE_TARGET_GET,
    ISP_AE_PRIORITY_SET,
    ISP_AE_PRIORITY_GET,
    ISP_AE_ROI_WIN_SET,
    ISP_AE_ROI_WIN_GET,

    ISP_AE_LOW_LIGHT_BIAS_SET,
    ISP_AE_LOW_LIGHT_BIAS_GET,
    ISP_AE_HIGH_LIGHT_BIAS_SET,
    ISP_AE_HIGH_LIGHT_BIAS_GET,
    ISP_AE_MD_MAP_GET,
    ISP_AE_OD_FLAG_GET,
    ISP_AE_OD_AVGVAL_GET,
    ISP_AE_MD_MAP_DILATE_GET,
    ISP_AE_QP_MAP_DILATE_GET,
    ISP_AE_WEIGHT_GET,
    ISP_AE_STATS_GET,

    ISP_DEFOG_STRENGTH_SET = 8030,
    ISP_DEFOG_STRENGTH_GET,

    ISP_AWB_ISO_SET = 8040,
    ISP_AWB_INTTIME_SET,
    ISP_AWB_PIRIS_SET,
    ISP_AWB_APEX_SET,
    ISP_AWB_SNAP_MODE_SET,
    ISP_AWB_EXP_CHG_SET,
    ISP_AWB_RGAIN_GET,
    ISP_AWB_BGAIN_GET,

    ISP_AF_MANUAL_SET = 8060,
    ISP_AF_FOCUS_GET,
    ISP_AF_ZOOM_GET,

    ISP_TC_RESET = 8070,
    ISP_TC_SET_BASE_FPS,

    ISP_PARAM_UPDATE = 8080,

    ISP_CTRL_CMD_MAX,
} ISP_CTRL_CMD_E;

typedef struct eiISP_GRID_STAT_S {
    EI_U32 au32SumR[ISP_GRID_ZONE_NUM];
    EI_U32 au32SumGr[ISP_GRID_ZONE_NUM];
    EI_U32 au32SumGb[ISP_GRID_ZONE_NUM];
    EI_U32 au32SumB[ISP_GRID_ZONE_NUM];
    EI_U32 au32Count[ISP_GRID_ZONE_NUM];
} ISP_GRID_STAT_S;

typedef enum eiISP_AE_EXP_CFG_TYPE_E {
    ISP_AE_EXP_CFG_INT_TIME = 0,
    ISP_AE_EXP_CFG_EXP_RATIO_LM,
    ISP_AE_EXP_CFG_EXP_RATIO_MS,
    ISP_AE_EXP_CFG_EXP_RATIO_SVS,
    ISP_AE_EXP_CFG_MAX
} ISP_AE_EXP_CFG_TYPE_E;

typedef struct eiISP_AE_PARAM_S {
    SENSOR_ID SnsId;
    EI_U8  u8WDRMode;
    EI_U16 u16BlackLevel;
    EI_FLOAT f32Fps;
    ISP_BAYER_FORMAT_E enBayer;
    EI_VOID *pstRegCfg;
} ISP_AE_PARAM_S;

typedef struct eiISP_AE_STAT_GLOBAL_S {
    EI_U32 u32PixelCount;
    EI_U32 *pu32Histogram;
    EI_U32 u32LtmPixelCount;
    EI_U32 *pu32LtmHistogram;
} ISP_AE_STAT_GLOBAL_S;

typedef struct eiISP_AE_STAT_ZONE_S {
    EI_U16 au16Hist0[ISP_MAX_METER_ZONES];
    EI_U16 au16Hist1[ISP_MAX_METER_ZONES];
    EI_U16 au16Hist2[ISP_MAX_METER_ZONES];
    EI_U16 au16Hist3[ISP_MAX_METER_ZONES];
    EI_U16 au16Hist4[ISP_MAX_METER_ZONES];
} ISP_AE_STAT_ZONE_S;

typedef struct eiISP_AE_INFO_S {
    EI_U32 u32FrameCnt;
    ISP_AE_STAT_GLOBAL_S *pstAeStatGlb;
    ISP_AE_STAT_ZONE_S *pstAeStatZone;
    ISP_GRID_STAT_S *pstAeStatGrid;
    EI_VOID *pstRegCfg;
} ISP_AE_INFO_S;

typedef struct eiISP_AE_RESULT_S {
    EI_U32 u32Iso;
    EI_U32 u32IspDgain;
    EI_FLOAT fSnsGain;
    EI_FLOAT afIntTime[ISP_AE_EXP_CFG_MAX];
    EI_FLOAT afWdrGain[ISP_LTM_LUT_SIZE];
    EI_U32 au32LtmCurve[ISP_LTM_LUT_SIZE];
    EI_U8 u8ExpChange;

    EI_U8 u8ObjLum;
    EI_U8 u8AvgLum;
    EI_U8 u8OrgTarget;
    EI_U8 u8FinalTarget;

    ISP_APEX_S stApex;

    /*for defog*/
    EI_U32 u32GammaGain;
    EI_U32 u32GammaOffset;

    /*for ltm*/
    EI_U16 u16DarkEnh;
    EI_U16 u16LtmGain;
    EI_U32 u32LtmOffset;
    EI_U32 u32LtmContrast;
    EI_U32 u32LtmStrengthRatio;

    /* for motion detect */
    EI_FLOAT fMotionLevel;
    EI_U32 u32MotionFlag;
    EI_U32 u32MeanVar;
    EI_U32 u32RcVal;
    EI_FLOAT fRcMotionRatio;
    EI_FLOAT fIPQpDelta;
} ISP_AE_RESULT_S;

typedef struct eiISP_AE_EXP_FUNC_S {
    EI_S32(*pfn_ae_init)(EI_S32 s32Id, const ISP_AE_PARAM_S *pstAeParam);
    EI_S32(*pfn_ae_exec)(EI_S32 s32Id, const ISP_AE_INFO_S *pstAeInfo, ISP_AE_RESULT_S *pstAeResult);
    EI_S32(*pfn_ae_ctrl)(EI_S32 s32Id, EI_U32 u32Cmd, EI_VOID *pValue);
    EI_S32(*pfn_ae_exit)(EI_S32 s32Id);
} ISP_AE_FUNC_S;

typedef struct eiISP_AE_REGISTER_S {
    ISP_AE_FUNC_S stAeFunc;
} ISP_AE_REGISTER_S;

typedef struct eiISP_AWB_PARAM_S {
    SENSOR_ID SnsId;
    EI_U8 u8WDRMode;

    EI_VOID *pstRegCfg;
} ISP_AWB_PARAM_S;


typedef struct eiISP_AWB_STAT_GLOBAL_S {
    EI_U16 u16MeteringRgain;
    EI_U16 u16MeteringBgain;
    EI_U32 u32MeteringCount;
} ISP_AWB_STAT_GLOBAL_S;

typedef struct eiISP_AWB_STAT_ZONE_S {
    EI_U16 au16MeteringRgain[ISP_MAX_METER_ZONES];
    EI_U16 au16MeteringBgain[ISP_MAX_METER_ZONES];
    EI_U32 au32MeteringCount[ISP_MAX_METER_ZONES];
} ISP_AWB_STAT_ZONE_S;

typedef struct eiISP_AWB_INFO_S {
    EI_U32  u32FrameCnt;

    ISP_AWB_STAT_GLOBAL_S *pstAwbStatGlb;
    ISP_AWB_STAT_ZONE_S *pstAwbStatZone;
    ISP_GRID_STAT_S *pstAwbStatGrid;
    EI_VOID *pstRegCfg;
} ISP_AWB_INFO_S;

typedef struct eiISP_AWB_RESULT_S {
    EI_U32 au32WBGain[ISP_BAYER_CHN_NUM];
    EI_S16 as16CcmCoeff[CCM_MATRIX_SIZE];
    EI_S16 as16CcmOffset[3];
    EI_U32 u32ColorTempId;
    EI_U8  u8Saturation;
    EI_U32 u8StatsNum;
    EI_U16 au16StatsArray[6][3];

    EI_U8 au8LightWeight[32];
} ISP_AWB_RESULT_S;

typedef struct eiISP_AWB_EXP_FUNC_S {
    EI_S32(*pfn_awb_init)(EI_S32 s32Id, const ISP_AWB_PARAM_S *pstAwbParam);
    EI_S32(*pfn_awb_exec)(EI_S32 s32Id, const ISP_AWB_INFO_S *pstAwbInfo, ISP_AWB_RESULT_S *pstAwbResult);
    EI_S32(*pfn_awb_ctrl)(EI_S32 s32Id, EI_U32 u32Cmd, EI_VOID *pValue);
    EI_S32(*pfn_awb_exit)(EI_S32 s32Id);
} ISP_AWB_FUNC_S;

typedef struct eiISP_AWB_REGISTER_S {
    ISP_AWB_FUNC_S stAwbFunc;
} ISP_AWB_REGISTER_S;

typedef enum eiAF_MODE_E {
    AF_MODE_MANUAL = 0,
    AF_MODE_CAF,
    AF_MODE_SAF,
    AF_MODE_SPOT,
    AF_MODE_FULLRANGE,
    AF_MODE_CALIBRATION
} AF_MODE_E;

typedef enum eiAF_STATE_E {
    AF_STATE_INACTIVE,
    AF_STATE_SCAN,
    AF_STATE_FOCUSED,
    AF_STATE_REFOCUS,
    AF_STATE_CALIBRATION,
} AF_STATE_E;

typedef struct eiISP_AF_STAT_S {
    EI_U64 u64Sharp[ISP_MAX_METER_ZONES];
    EI_U32 u32SharpReliablility[ISP_MAX_METER_ZONES];
} ISP_AF_STAT_S;

typedef struct eiISP_AF_PARAM_S {
    SENSOR_ID SnsId;
    EI_FLOAT f32Fps;
    EI_VOID *pstRegCfg;
} ISP_AF_PARAM_S;

typedef struct eiISP_AF_INFO_S {
    EI_U32  u32FrameCnt;
    EI_U32  u32ZoomCode;
    EI_U32  u32FocusCode;
    ISP_AF_STAT_S *pstAfStat;
    EI_VOID *pstRegCfg;
} ISP_AF_INFO_S;

#define AF_INT_TIME_NUM     (4)
typedef struct eiISP_AF_RESULT_S {
    EI_U16 u16FocusCode;
    EI_U16 u16ZoomCode;
    EI_U32 u32AfSharpVal;
    AF_STATE_E enState;
} ISP_AF_RESULT_S;

typedef struct eiISP_AF_EXP_FUNC_S {
    EI_S32(*pfn_af_init)(EI_S32 s32Id, const ISP_AF_PARAM_S *pstAfParam);
    EI_S32(*pfn_af_exec)(EI_S32 s32Id, const ISP_AF_INFO_S *pstAfInfo, ISP_AF_RESULT_S *pstAfResult);
    EI_S32(*pfn_af_ctrl)(EI_S32 s32Id, EI_U32 u32Cmd, EI_VOID *pValue);
    EI_S32(*pfn_af_exit)(EI_S32 s32Id);
} ISP_AF_FUNC_S;

typedef struct eiISP_AF_REGISTER_S {
    ISP_AF_FUNC_S stAfFunc;
} ISP_AF_REGISTER_S;

#define ISP_3ALIB_NAME_SIZE   (20)
typedef struct eiISP_3ALIB_S {
    EI_S32  s32Id;
    EI_CHAR acLibName[ISP_3ALIB_NAME_SIZE];
} ISP_3ALIB_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
