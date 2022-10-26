/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_isp_adv.h
 * @Date      :    2021-7-31
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_ISP_PARAM_H__
#define __EI_COMM_ISP_PARAM_H__

#include "ei_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define PARAM_CCM_COEFF_NUM 9
#define PARAM_CCM_OFFS_NUM 3
#define PARAM_CSC_COEFF_NUM 9
#define PARAM_CSC_OFFS_NUM 3
#define PARAM_ISO_NUM 16
#define PARAM_GAMMA_POINT_NUM 129
#define PARAM_3DLUT_POINT_NUM 1000
#define PARAM_EE_TH_NUM 4
#define PARAM_MAX_PLINE_NUM 16
#define PARAM_TNR_PROFILE_NODE 17
#define PARAM_AWB_LIGTH_NAME_SIZE 32
#define PARAM_AWB_BV_NUM 16
#define PARAM_AF_DOF_TBL_SIZE 128
#define PARAM_EE_LUIT_1_SIZE 5
#define PARAM_EE_LUIT_2_SIZE 5
#define PARAM_EE_LUIT_T_SIZE 9
#define PARAM_MD_PROFILE_NODE 17
#define PARAM_TC_NUM 16

#define ISP_MOD_DBG_DECOMPANDER     (1 << 1)
#define ISP_MOD_DBG_SO_GAIN_WDR     (1 << 2)
#define ISP_MOD_DBG_FRAME_STITCH    (1 << 3)
#define ISP_MOD_DBG_DGAIN           (1 << 4)
#define ISP_MOD_DBG_SENSOR_OFFSET   (1 << 5)
#define ISP_MOD_DBG_FE              (1 << 6)
#define ISP_MOD_DBG_SDPC            (1 << 7)
#define ISP_MOD_DBG_BAYER_NR        (1 << 8)
#define ISP_MOD_DBG_TEMPER          (1 << 9)
#define ISP_MOD_DBG_CAC             (1 << 10)
#define ISP_MOD_DBG_SQUARE          (1 << 11)
#define ISP_MOD_DBG_SAHDING         (1 << 12)
#define ISP_MOD_DBG_WB              (1 << 13)
#define ISP_MOD_DBG_LTM             (1 << 14)
#define ISP_MOD_DBG_DEMOSAIC        (1 << 15)
#define ISP_MOD_DBG_PFC             (1 << 16)
#define ISP_MOD_DBG_CCM             (1 << 17)
#define ISP_MOD_DBG_CNR             (1 << 18)
#define ISP_MOD_DBG_CLUT            (1 << 19)
#define ISP_MOD_DBG_GAMMA           (1 << 20)
#define ISP_MOD_DBG_SHARPEN         (1 << 21)
#define ISP_MOD_DBG_CSC             (1 << 22)
#define ISP_MOD_DBG_TNR             (1 << 23)
#define ISP_MOD_DBG_EE              (1 << 24)

typedef enum eiPARAM_AWB_CHN {
    PARAM_AWB_CHN_R = 0,
    PARAM_AWB_CHN_GR,
    PARAM_AWB_CHN_GB,
    PARAM_AWB_CHN_B,

    PARAM_AWB_CHN_MAX
} PARAM_AWB_CHN;

typedef enum eiPARAM_RAW_CHN {
    PARAM_RAW_CHN_R = 0,
    PARAM_RAW_CHN_GR,
    PARAM_RAW_CHN_GB,
    PARAM_RAW_CHN_B,

    PARAM_RAW_CHN_MAX
} PARAM_RAW_CHN;

typedef enum eiPARAM_STITCH_CHN {
    PARAM_STITCH_CHN_LM = 0,
    PARAM_STITCH_CHN_MS,
    PARAM_STITCH_CHN_SVS,
    PARAM_STITCH_CHN_MAX,
} PARAM_STITCH_CHN;

typedef enum eiPARAM_AWB_GAIN {
    PARAM_AWB_GAIN_RG = 0,
    PARAM_AWB_GAIN_BG,

    PARAM_AWB_GAIN_MAX
} PARAM_AWB_GAIN;

typedef enum eiPARAM_THRESHOLD_TYPE {
    PARAM_THRESHOLD_LOW = 0,
    PARAM_THRESHOLD_HIGH,

    PARAM_THRESHOLD_TYPE_MAX
} PARAM_THRESHOLD_TYPE;

typedef enum eiPARAM_AWB_ILLU_TYPE {
    PARAM_AWB_ILLUMINATION_HOR    = 0,
    PARAM_AWB_ILLUMINATION_A      = 1,
    PARAM_AWB_ILLUMINATION_U30    = 2,
    PARAM_AWB_ILLUMINATION_TL84   = 3,
    PARAM_AWB_ILLUMINATION_CWF    = 4,
    PARAM_AWB_ILLUMINATION_D50    = 5,
    PARAM_AWB_ILLUMINATION_D65    = 6,
    PARAM_AWB_ILLUMINATION_D75    = 7,

    PARAM_AWB_ILLUMINATION_USR0   = 8,
    PARAM_AWB_ILLUMINATION_USR1   = 9,
    PARAM_AWB_ILLUMINATION_USR2   = 10,
    PARAM_AWB_ILLUMINATION_USR3   = 11,
    PARAM_AWB_ILLUMINATION_USR4   = 12,
    PARAM_AWB_ILLUMINATION_USR5   = 13,
    PARAM_AWB_ILLUMINATION_USR6   = 14,
    PARAM_AWB_ILLUMINATION_USR7   = 15,

    PARAM_AWB_ILLUMINATION_MAX
} PARAM_AWB_ILLU_TYPE;

typedef enum eiPARAM_AWB_HINT_COLOR_TYPE {
    PARAM_AWB_HINT_COLOR_SKIN_1       = 0,
    PARAM_AWB_HINT_COLOR_SKIN_2       = 1,
    PARAM_AWB_HINT_COLOR_EXD_GREEN    = 2,
    PARAM_AWB_HINT_COLOR_EXD_RED      = 3,
    PARAM_AWB_HINT_COLOR_EXD_BLUE     = 4,
    PARAM_AWB_HINT_COLOR_RES1         = 5,
    PARAM_AWB_HINT_COLOR_RES2         = 6,
    PARAM_AWB_HINT_COLOR_RES3         = 7,

    PARAM_AWB_HINT_COLOR_MAX
} PARAM_AWB_HINT_COLOR_TYPE;

typedef enum eiPARAM_AWB_MCOLOR_TYPE {
    PARAM_AWB_MCOLOR_GREEN            = 0,
    PARAM_AWB_MCOLOR_BLUE             = 1,
    PARAM_AWB_MCOLOR_RED              = 2,

    PARAM_AWB_MCOLOR_MAX
} PARAM_AWB_MCOLOR_TYPE;

typedef enum eiPARAM_AWB_HINT_PROJECT_TYPE {
    PARAM_AWB_HINT_PROJECT_MIN_IDX  = 0,
    PARAM_AWB_HINT_PROJECT_MAX_IDX  = 1,
    PARAM_AWB_HINT_PROJECT_DEF_IDX  = 2,

    PARAM_AWB_HINT_PROJECT_IDX_MAX
} PARAM_AWB_HINT_PROJECT_TYPE;

/* AEC */
typedef enum eiPARAM_AEC_CONVERGENCE {
    PARAM_AEC_SMOOTH_OVER_LOW = 0,
    PARAM_AEC_SMOOTH_OVER_HIGH,
    PARAM_AEC_SMOOTH_UNDER_LOW,
    PARAM_AEC_SMOOTH_UNDER_HIGH,

    PARAM_AEC_SMOOTH_MAX
} PARAM_AEC_CONVERGENCE;

typedef enum eiISP_PARAM_AEC_PLINE_TYPE {
    PARAM_PLINE_INT_START    = 0,
    PARAM_PLINE_INT_END      = 1,
    PARAM_PLINE_AGAIN_START  = 2,
    PARAM_PLINE_AGAIN_END    = 3,
    PARAM_PLINE_DGAIN_START  = 4,
    PARAM_PLINE_DGAIN_END    = 5,

    PARAM_PLINE_MAX
} ISP_PARAM_AEC_PLINE_TYPE;

typedef enum eiPARAM_AEC_HINT_PROJECT_TYPE {
    PARAM_AEC_WEIGHT_WIDTH  = 0,
    PARAM_AEC_WEIGHT_HEIGHT = 1,

    PARAM_AEC_WEIGHT_WH_MAX
} PARAM_AEC_WEIGHT_WH;

typedef struct eiISP_PARAM_TOP_S {
    EI_U32 u32Left;
    EI_U32 u32Top;
    EI_U32 u32Width;
    EI_U32 u32Height;
    EI_U32 u32CfaPattern;
    EI_U32 u32BayerOrder;
    EI_U32 u32Hflip;
    EI_U32 u32Vflip;
    EI_U32 u32TestMode;
    EI_U32 u32PrintJson;
    EI_U32 u32ModDbgLvl;
    EI_U32 u32PreAlloc3D;
    EI_U32 au32TemperSwTh[PARAM_THRESHOLD_TYPE_MAX];
} ISP_PARAM_TOP_S;

typedef struct eiISP_PARAM_BYPASS_S {
    EI_U32 u32BypassTestGenerator;
    EI_U32 u32BypassInputFormatter;
    EI_U32 u32BypassDecompander;
    EI_U32 u32BypassSoWdr;
    EI_U32 u32BypassGainWdr;
    EI_U32 u32BypassFrameStitch;
    EI_U32 u32BypassDgain;
    EI_U32 u32BypassSo;
    EI_U32 u32BypassSqrt;
    EI_U32 u32BypassFe;
    EI_U32 u32BypassStaticDpc;
    EI_U32 u32BypassBayerNr;
    EI_U32 u32BypassTemper;
    EI_U32 u32BypassCac;
    EI_U32 u32BypassSquare;
    EI_U32 u32BypassSoShading;
    EI_U32 u32BypassRadialShading;
    EI_U32 u32BypassMeshShading;
    EI_U32 u32BypassWb;
    EI_U32 u32BypassLtmGain;
    EI_U32 u32BypassLtm;
    EI_U32 u32BypassMirror;
    EI_U32 u32BypassDemosaic;
    EI_U32 u32BypassPfc;
    EI_U32 u32BypassCcm;
    EI_U32 u32BypassCnr;
    EI_U32 u32BypassCLut;
    EI_U32 u32BypassGamma;
    EI_U32 u32BypassFrCrop;
    EI_U32 u32BypassFrGamma;
    EI_U32 u32BypassFrSharpen;
    EI_U32 u32BypassFrCsc;
    EI_U32 u32BypassIspRaw;
} ISP_PARAM_BYPASS_S;

typedef struct eiISP_PARAM_ADJUST_S {
    EI_FLOAT fContrast;
    EI_FLOAT fSaturation;
    EI_FLOAT fBrightness;
    EI_FLOAT fHue;
    EI_FLOAT fSharpness;

    EI_FLOAT fIntTime;
    EI_FLOAT fGain;
    EI_FLOAT afWbGain[PARAM_AWB_CHN_MAX];
} ISP_PARAM_ADJUST_S;

typedef struct eiISP_PARAM_SLOPE_OFFS_ARRAY_TH_S {
    EI_FLOAT afSlope[PARAM_ISO_NUM];
    EI_FLOAT afOffset[PARAM_ISO_NUM];
    EI_FLOAT afThresh[PARAM_ISO_NUM];
} ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S;

typedef struct eiISP_PARAM_SLOPE_OFFS_TH_S {
    EI_U32 u32Slope;
    EI_U32 u32Offset;
    EI_U32 u32Thresh;
} ISP_PARAM_SLOPE_OFFS_TH_S;

typedef struct eiISP_PARAM_METER_S {
    EI_U32 u32AeSwitch;
    EI_U32 au32AeHistTh[4];
    EI_U32 au32AeWinNum[2];
    EI_U32 u32AfSwitch;
    EI_U32 au32AfWinNum[2];
    EI_FLOAT afAfKernel[PARAM_ISO_NUM];
    EI_U32 u32AwbSwitch;
    EI_U32 u32AwbStatsMode;
    EI_U32 u32AwbMeterTest;
    EI_U32 au32AwbLimit[2];
    EI_U32 au32AwbCbCrRef0[4];
    EI_U32 au32AwbCbCrRef1[4];
    EI_U32 au32AwbWinNum[2];
} ISP_PARAM_METER_S;

typedef struct eiISP_PARAM_TNR_MOTION_CTRL_S {
    EI_U8 u8Enable;
    EI_FLOAT afTnrMotionOffset[PARAM_ISO_NUM];
    EI_FLOAT afTnrMotionSlop[PARAM_ISO_NUM];
    EI_FLOAT afTnrMotionThresh[PARAM_ISO_NUM];
    EI_FLOAT afTnrMotionCurve[PARAM_ISO_NUM];
} ISP_PARAM_TNR_MOTION_CTRL_S;

typedef struct eiISP_PARAM_TNR_S {
    EI_U32 u32TnrEnable;

    EI_FLOAT afWeightSigma[PARAM_ISO_NUM];
    EI_FLOAT afTnrStrength[PARAM_ISO_NUM];
    EI_FLOAT afSnrStrength[PARAM_ISO_NUM];
    EI_FLOAT afLum2dSigma[PARAM_ISO_NUM];
    EI_FLOAT afLum3dSigma[PARAM_ISO_NUM];
    EI_FLOAT afLumMotionThLow[PARAM_ISO_NUM];
    EI_FLOAT afLumMotionTh[PARAM_ISO_NUM];
    EI_FLOAT afChroma2dSigma[PARAM_ISO_NUM];
    EI_FLOAT afChroma3dSigma[PARAM_ISO_NUM];
    EI_FLOAT afChromaMotionThLow[PARAM_ISO_NUM];
    EI_FLOAT afChromaMotionTh[PARAM_ISO_NUM];

    EI_U8 u8NoiseProfile[PARAM_TNR_PROFILE_NODE];
    EI_U8 u8MotionProfile[PARAM_TNR_PROFILE_NODE];

    EI_FLOAT afAutoMotionAlpha[PARAM_ISO_NUM];
    EI_FLOAT afAutoNoiseAlpha[PARAM_ISO_NUM];
    EI_FLOAT afAutoMotionOffset[PARAM_ISO_NUM];
    EI_FLOAT afAutoMotionSlop[PARAM_ISO_NUM];

    EI_FLOAT afShiftDistTh[PARAM_ISO_NUM];
    EI_FLOAT afShiftOffsYTh[PARAM_ISO_NUM];
    EI_FLOAT afShiftOffsXTh[PARAM_ISO_NUM];

    EI_FLOAT afDistRadius[PARAM_ISO_NUM];
    EI_FLOAT fRadialStrength;

    ISP_PARAM_TNR_MOTION_CTRL_S stTnrMotionCtrl;

} ISP_PARAM_TNR_S;

typedef struct eiISP_PARAM_EE_S {
    EI_U32 u32EEEnable;
    EI_U32 u8EEPosition;
    EI_U32 u8EENormStEn;

    EI_FLOAT afEEStrength1[PARAM_ISO_NUM];
    EI_FLOAT afEES1Sigma[PARAM_ISO_NUM];
    EI_FLOAT afEES1NrLvl[PARAM_ISO_NUM];
    EI_FLOAT afEES1OUShoot[PARAM_ISO_NUM];
    EI_FLOAT afEES1HoldLen[PARAM_ISO_NUM];
    EI_FLOAT afEES1HighSupp[PARAM_ISO_NUM];

    EI_FLOAT afEEStrength2[PARAM_ISO_NUM];
    EI_FLOAT afEES2Sigma[PARAM_ISO_NUM];
    EI_FLOAT afEES2NrLvl[PARAM_ISO_NUM];
    EI_FLOAT afEES2OUShoot[PARAM_ISO_NUM];
    EI_FLOAT afEES2HoldLen[PARAM_ISO_NUM];
    EI_FLOAT afEES2HighSupp[PARAM_ISO_NUM];

    EI_FLOAT afEEStrengthT[PARAM_ISO_NUM];
    EI_FLOAT afEEStSigma[PARAM_ISO_NUM];
    EI_FLOAT afEEStNrLvl[PARAM_ISO_NUM];
    EI_FLOAT afEEStOUShoot[PARAM_ISO_NUM];
    EI_FLOAT afEEStHoldLen[PARAM_ISO_NUM];
    EI_FLOAT afEEStHighSupp[PARAM_ISO_NUM];
    EI_FLOAT afEEStrengthTRadius[PARAM_ISO_NUM];

    EI_FLOAT afEEStrengthLP[PARAM_ISO_NUM];
    EI_FLOAT afEEHvSlop[PARAM_ISO_NUM];

    ISP_PARAM_TYPE_E enAmplManual;
    EI_FLOAT afEEVer[PARAM_EE_TH_NUM];
    EI_FLOAT afEEHor[PARAM_EE_TH_NUM];
    EI_FLOAT afEETexture[PARAM_EE_TH_NUM];

    EI_U8 u8ManualLut;
    EI_S8 as8EELut1[PARAM_EE_LUIT_1_SIZE][PARAM_EE_LUIT_1_SIZE];
    EI_S8 as8EELut2[PARAM_EE_LUIT_2_SIZE][PARAM_EE_LUIT_2_SIZE];
    EI_S8 as8EELutT[PARAM_EE_LUIT_T_SIZE][PARAM_EE_LUIT_T_SIZE];

    EI_U8 u8PreSetHvMode;
    EI_FLOAT afEEHvMode[PARAM_ISO_NUM];
    EI_FLOAT afEEHvIdx[PARAM_ISO_NUM];

} ISP_PARAM_EE_S;

typedef struct eiISP_PARAM_IPPU_S {
    EI_U32 u32IppuEn;

    ISP_PARAM_TNR_S stTnr;
    ISP_PARAM_EE_S stEE;
} ISP_PARAM_IPPU_S;

/* Q12 */
typedef struct eiISP_PARAM_BLC_S {
    EI_U32 au32BlackLevelSt0[PARAM_RAW_CHN_MAX];
    EI_U32 au32BlackLevelSt1[PARAM_RAW_CHN_MAX];
    EI_FLOAT afCurve[PARAM_ISO_NUM];
} ISP_PARAM_BLC_S;

typedef struct eiISP_PARAM_PFC_S {
    EI_U8  u8UseColorCorrectedRgb;

    EI_FLOAT afHueStrenght[PARAM_ISO_NUM];
    EI_FLOAT afSatStrength[PARAM_ISO_NUM];
    EI_FLOAT afLumaStrength[PARAM_ISO_NUM];
    EI_FLOAT afPurpleStrength[PARAM_ISO_NUM];
    EI_FLOAT afSaturationStrength[PARAM_ISO_NUM];
    EI_U16 au16CcmCoeff[PARAM_CCM_COEFF_NUM];

    ISP_PARAM_SLOPE_OFFS_TH_S stSad;
    ISP_PARAM_SLOPE_OFFS_TH_S stHue0;
    ISP_PARAM_SLOPE_OFFS_TH_S stHue1;
    ISP_PARAM_SLOPE_OFFS_TH_S stSat0;
    ISP_PARAM_SLOPE_OFFS_TH_S stSat1;
    ISP_PARAM_SLOPE_OFFS_TH_S stLum00;
    ISP_PARAM_SLOPE_OFFS_TH_S stLum01;
    ISP_PARAM_SLOPE_OFFS_TH_S stLum10;
    ISP_PARAM_SLOPE_OFFS_TH_S stLum11;
    ISP_PARAM_SLOPE_OFFS_TH_S stHsl;

    ISP_PARAM_TYPE_E enParamManual;

    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoSad;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoHue0;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoHue1;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoSat0;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoSat1;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoLum00;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoLum01;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoLum10;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoLum11;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stAutoHsl;

    EI_U32 u32DebugSel;
} ISP_PARAM_PFC_S;

typedef struct eiISP_PARAM_STITCH_S {
    EI_U32 u32Mode;
    EI_U32 au32ExposureRatio[PARAM_STITCH_CHN_MAX];
    EI_FLOAT afExposureRatio[PARAM_STITCH_CHN_MAX];
    EI_U32 au32BlackLevel[PARAM_RAW_CHN_MAX];
    EI_U32 u32BlackLevelOut;

    EI_FLOAT afHighThreshLM[PARAM_ISO_NUM];
    EI_FLOAT afHighThreshMS[PARAM_ISO_NUM];
    EI_FLOAT afHighThreshSVS[PARAM_ISO_NUM];

    EI_FLOAT afLowThreshLM[PARAM_ISO_NUM];
    EI_FLOAT afLowThreshMS[PARAM_ISO_NUM];
    EI_FLOAT afLowThreshSVS[PARAM_ISO_NUM];

    EI_FLOAT afNpMultLM[PARAM_ISO_NUM];
    EI_FLOAT afNpMultMS[PARAM_ISO_NUM];
    EI_FLOAT afNpMultSVS[PARAM_ISO_NUM];

    EI_FLOAT afMotionSlopLM[PARAM_ISO_NUM];
    EI_FLOAT afMotionSlopMS[PARAM_ISO_NUM];
    EI_FLOAT afMotionSlopSVS[PARAM_ISO_NUM];

    EI_U8 u8OutputSelect;
    EI_U16 u16GainR;
    EI_U16 u16GainB;
    EI_U8 u8BwbSelect;
    EI_U8 u8LmAlgSelect;
    EI_U8 u8Use3x3Max;
    EI_U8 u8McoffNcEnable;
    EI_U8 u8McoffModeEnable;

    EI_FLOAT afConsistencyThreshMov[PARAM_ISO_NUM];
    EI_FLOAT afConsistencyThreshLvl[PARAM_ISO_NUM];
    EI_FLOAT afLmNoiseThresh[PARAM_ISO_NUM];
    EI_FLOAT afLmPosWeight[PARAM_ISO_NUM];
    EI_FLOAT afLmNegWeight[PARAM_ISO_NUM];
    EI_FLOAT afLmMedNoiseAThresh[PARAM_ISO_NUM];
    EI_FLOAT afLmMedNoiseIntThresh[PARAM_ISO_NUM];
    EI_FLOAT afLmMcMagLblendThresh[PARAM_ISO_NUM];
    EI_FLOAT afMcoffWbOffset[PARAM_ISO_NUM];
    EI_FLOAT afExposureMaskThresh[PARAM_ISO_NUM];
    EI_FLOAT afMcoffNcThresh_low[PARAM_ISO_NUM];
    EI_FLOAT afMcoffNcThresh_high[PARAM_ISO_NUM];
    EI_FLOAT afMcoffNcScale[PARAM_ISO_NUM];

    EI_U16 au16McoffMax[ISP_WDR_MAX_CHN];
    EI_U16 au16McoffScaler[ISP_WDR_MAX_CHN - 1];

    ISP_PARAM_SLOPE_OFFS_TH_S stLmMcBlend;
    ISP_PARAM_SLOPE_OFFS_TH_S stLmMcThresh;
    ISP_PARAM_SLOPE_OFFS_TH_S stLmMcMagThresh;

    EI_FLOAT afNP0MultCurve[PARAM_ISO_NUM];
    EI_FLOAT afNP1MultCurve[PARAM_ISO_NUM];
    EI_FLOAT afNP2MultCurve[PARAM_ISO_NUM];
    EI_FLOAT afNP3MultCurve[PARAM_ISO_NUM];

    EI_U32 au32NPLut0[ISP_NP_LUT_SIZE];
    EI_U32 au32NPLut1[ISP_NP_LUT_SIZE];
    EI_U32 au32NPLut2[ISP_NP_LUT_SIZE];
    EI_U32 au32NPLut3[ISP_NP_LUT_SIZE];
} ISP_PARAM_STITCH_S;

typedef struct eiISP_PARAM_CAC_S {
    EI_U32 u32Mesh[2];
    EI_U32 u32Polynomial[4][10];
} ISP_PARAM_CAC_S;

typedef struct eiISP_PARAM_BAYER_NR_S {
    EI_U8 u8Enable;
    EI_U8 u8NlmEnable;
    EI_U8 u8RmEnable;
    EI_U8 u8ViewFilter;
    EI_U8 u8ScaleMode;
    EI_U8 u8NonlinearWkgen;
    EI_U8 u8FilterSelect;
    EI_U8 u8IntSelect;

    EI_FLOAT afHighFreqStren[PARAM_ISO_NUM];
    EI_FLOAT afHighFreqTh[PARAM_ISO_NUM];
    EI_FLOAT afLowFreqStren[PARAM_ISO_NUM];
    EI_FLOAT afLowFreqTh[PARAM_ISO_NUM];
    EI_FLOAT afIntConfig[PARAM_ISO_NUM];
    EI_FLOAT afSadTh[PARAM_ISO_NUM];

    EI_U32 u32UseLut;
    EI_U32 u32UseExpMask;
    EI_U32 u32BlackReflect;
    EI_U32 u32BlackLevel;
    EI_U32 au32Thresh[ISP_NP_TH_NUM];
    EI_U32 au32NoiseLevel[ISP_WDR_MAX_CHN];
    EI_FLOAT afGlobalOffset[PARAM_ISO_NUM];
    EI_U32 au32NPLut[ISP_NP_LUT_SIZE];
} ISP_PARAM_BAYER_NR_S;

typedef struct eiISP_PARAM_DMSC_LUM_CFG_S {
    EI_U16 u16ThreshLow;
    EI_U8  u8OffsetLow;
    EI_U32 u32SlopeLow;
    EI_U16 u16ThreshHigh;
    EI_U32 u32SlopeHigh;
} ISP_PARAM_DMSC_SHARP_CFG_S;

typedef struct eiISP_PARAM_DEMOSAIC_S {
    EI_U16 u16LumThresh;
    EI_U8 u8DmscConfig;
    EI_U8 u8NpOff;
    EI_U8 u8NpOffReflect;
    EI_U32 au32NPLut[ISP_NP_LUT_SIZE];
    EI_FLOAT afNpOffset[PARAM_ISO_NUM];
    EI_FLOAT afDemosaicUuSlope[PARAM_ISO_NUM];
    EI_FLOAT afSadAmp[PARAM_ISO_NUM];

    EI_FLOAT afMinDStrength[PARAM_ISO_NUM];
    EI_FLOAT afMinUdStrength[PARAM_ISO_NUM];
    EI_FLOAT afMaxDStrength[PARAM_ISO_NUM];
    EI_FLOAT afMaxUdStrength[PARAM_ISO_NUM];

    EI_FLOAT afLgDetThresh[PARAM_ISO_NUM];
    EI_FLOAT afGrayDetThresh[PARAM_ISO_NUM];

    EI_U16 u16LgDetSlope;
    EI_U16 u16GrayDetSlope;
    ISP_PARAM_DMSC_SHARP_CFG_S stLumaD;
    ISP_PARAM_DMSC_SHARP_CFG_S stLumaUD;

    ISP_PARAM_SLOPE_OFFS_TH_S stVH;
    ISP_PARAM_SLOPE_OFFS_TH_S stAA;
    ISP_PARAM_SLOPE_OFFS_TH_S stVA;
    ISP_PARAM_SLOPE_OFFS_TH_S stUU;
    ISP_PARAM_SLOPE_OFFS_TH_S stSA;
    ISP_PARAM_SLOPE_OFFS_TH_S stAC;
    ISP_PARAM_SLOPE_OFFS_TH_S stUUSh;

    EI_U8 u8SharpenAlgSelect;
    EI_FLOAT afSharpAltD[PARAM_ISO_NUM];
    EI_FLOAT afSharpAltUd[PARAM_ISO_NUM];
    EI_FLOAT afSharpAltLd[PARAM_ISO_NUM];
    EI_FLOAT afSharpAltLdu[PARAM_ISO_NUM];
    EI_FLOAT afSharpAltLu[PARAM_ISO_NUM];
} ISP_PARAM_DEMOSAIC_S;

typedef struct eiISP_PARAM_DMSC_RGBIR_S {

    EI_U8 u8RgbirConfig;

    EI_U16 u16ClipLevel;
    EI_U16 u16ClipDebloom;

    EI_U8 u8IrOnBlueRow;
    EI_U8 u8DeclipMode;

    EI_U16 u16GainR;
    EI_U16 u16GainB;

    EI_U16 u16StaticGainR;
    EI_U16 u16StaticGainB;
    EI_U16 u16StaticGainI;

    EI_U16 u16InterpolationDir;
    EI_U16 u16SharpLimit;
    EI_U16 u16SharpHigh;
    EI_U16 u16SharpLow;

    EI_U16 u16FcLow;
    EI_U16 u16FcGrad;

    EI_U16 au16IrCorrectMat[16];

} ISP_PARAM_DMSC_RGBIR_S;

typedef struct eiISP_PARAM_SHARPEN_RGB_S {
    EI_U8 u8Enable;
    EI_U8 u8Debug;
    EI_FLOAT afStrength[PARAM_ISO_NUM];
    EI_FLOAT afCnotrolR[PARAM_ISO_NUM];
    EI_FLOAT afCnotrolB[PARAM_ISO_NUM];
    EI_FLOAT afAlphaUnderShoot[PARAM_ISO_NUM];
    EI_FLOAT afClipStrMax[PARAM_ISO_NUM];
    EI_FLOAT afClipStrMin[PARAM_ISO_NUM];
    ISP_PARAM_SLOPE_OFFS_TH_S stLum0;
    ISP_PARAM_SLOPE_OFFS_TH_S stLum1;
} ISP_PARAM_SHARPEN_RGB_S ;

typedef struct eiISP_PARAM_ANTI_FC_S {
    EI_FLOAT afFcSlope[PARAM_ISO_NUM];
    EI_FLOAT afFcAliasSlope[PARAM_ISO_NUM];
    EI_FLOAT afFcAliasThresh[PARAM_ISO_NUM];
} ISP_PARAM_ANTI_FC_S;

typedef struct eiISP_PARAM_DPC_S {
    EI_U8 u8Enable;
    EI_U8 u8ShowDp;
    EI_U8 u8DarkDisable;
    EI_U8 u8BrightDisable;
    EI_U32 u32DebugSel;

    EI_FLOAT afDpcSlope[PARAM_ISO_NUM];
    EI_FLOAT afDpcTh[PARAM_ISO_NUM];
    EI_FLOAT afDpcEdge[PARAM_ISO_NUM];
    EI_FLOAT afDpcBlend[PARAM_ISO_NUM];
} ISP_PARAM_DPC_S;

typedef struct eiISP_PARAM_GE_S {
    EI_U8 u8Enable;
    EI_FLOAT afGeStrength[PARAM_ISO_NUM];
    EI_FLOAT afGeThreshold[PARAM_ISO_NUM];
    EI_FLOAT afGeSlope[PARAM_ISO_NUM];
    EI_FLOAT afGeEdge[PARAM_ISO_NUM];
    EI_FLOAT afLineThresh[PARAM_ISO_NUM];
    EI_FLOAT afSigmaIn[PARAM_ISO_NUM];
    EI_FLOAT afThreshShort[PARAM_ISO_NUM];
    EI_FLOAT afThreshLong[PARAM_ISO_NUM];
    EI_FLOAT afExpThresh[PARAM_ISO_NUM];
    EI_FLOAT afShortRatio[PARAM_ISO_NUM];
    EI_FLOAT afLongRatio[PARAM_ISO_NUM];
    EI_FLOAT afNpOff[PARAM_ISO_NUM];
    EI_FLOAT afNpOffReflect[PARAM_ISO_NUM];
    EI_U32 au32NPLut[ISP_NP_LUT_SIZE];
} ISP_PARAM_GE_S;

typedef struct eiISP_PARAM_CNR_S {

    EI_U8  u8Enable;
    EI_U8  u8SquareRootEnable;
    EI_U8  u8Debug;
    EI_U8  u8Mode;
    EI_U16 u16Ucenter;
    EI_U16 u16Vcenter;

    EI_U8 u8UvVar1Scale;
    EI_U8 u8UvVar2Scale;

    EI_FLOAT afDeltaFactor[PARAM_ISO_NUM];
    EI_FLOAT afEffectiveKernel[PARAM_ISO_NUM];
    EI_FLOAT afUVSlope[PARAM_ISO_NUM];
    EI_FLOAT afGlobalOffset[PARAM_ISO_NUM];
    EI_FLOAT afGlobalSlope[PARAM_ISO_NUM];

    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stUmean0;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stUmean1;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stVmean0;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stVmean1;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stUvVar0;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stUvVar1;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stUvDelta0;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stUvDelta1;
    ISP_PARAM_SLOPE_OFFS_ARRAY_TH_S stUvSeg1;

} ISP_PARAM_CNR_S;

typedef struct eiISP_PARAM_AEC_SPOT_S {
    EI_FLOAT fSpotTarget;

    EI_FLOAT fAeSpotRadius;
    EI_FLOAT fAeSpotTolerance;

    EI_FLOAT afAeSpotLumRange[2];
    EI_FLOAT fAeSpotDamp;
    EI_U32 u32AeSpotKeeping;
} ISP_PARAM_AEC_SPOT_S;

typedef struct eiISP_PARAM_LTM_CURVE_S {
    EI_U32 u32CurveMode;
    EI_FLOAT afCurveLow[PARAM_ISO_NUM];
    EI_FLOAT afCurveHigh[PARAM_ISO_NUM];
    EI_FLOAT afCurveAlpha[PARAM_ISO_NUM];
    EI_FLOAT afSmoothStrength[PARAM_ISO_NUM];
} ISP_PARAM_LTM_CURVE_S;

typedef struct eiISP_PARAM_DEFOG_S {
    EI_U32 u32Enable;
    EI_FLOAT afAvgCoeff[PARAM_ISO_NUM];
    EI_FLOAT afBlackPercentage[PARAM_ISO_NUM];
    EI_FLOAT afWhitePercentage[PARAM_ISO_NUM];
    EI_FLOAT afAutoBlackMin[PARAM_ISO_NUM];
    EI_FLOAT afAutoBlackMax[PARAM_ISO_NUM];
    EI_FLOAT afAutoWhitePrcTarget[PARAM_ISO_NUM];
} ISP_PARAM_DEFOG_S;

typedef struct eiISP_PARAM_LTM_AUTO_GAIN_S {
    EI_U32 u32Enable;

    EI_FLOAT afLtmOffsetMin[PARAM_ISO_NUM];
    EI_FLOAT afLtmOffsetMax[PARAM_ISO_NUM];
    EI_FLOAT afLtmGainMin[PARAM_ISO_NUM];
    EI_FLOAT afLtmGainMax[PARAM_ISO_NUM];
    EI_FLOAT afLtmSmooth[PARAM_ISO_NUM];
    EI_FLOAT afLtmTarget[PARAM_ISO_NUM];
    EI_FLOAT afLtmHighProp[PARAM_ISO_NUM];
    EI_FLOAT afLtmLowProp[PARAM_ISO_NUM];
} ISP_PARAM_LTM_AUTO_GAIN_S;

typedef struct eiISP_PARAM_MD_S {
    EI_U32 u32Enable;
    EI_FLOAT afMdTh[PARAM_ISO_NUM];
    EI_FLOAT afMdBlkCntTh[PARAM_ISO_NUM];

    EI_FLOAT afMdRoiThLow[PARAM_ISO_NUM];
    EI_FLOAT afMdRoiThHigh[PARAM_ISO_NUM];

    EI_FLOAT afMdRoiTextureLow[PARAM_ISO_NUM];
    EI_FLOAT afMdRoiTextureHigh[PARAM_ISO_NUM];

    EI_FLOAT afMdRoiFrameCnt[PARAM_ISO_NUM];
    EI_FLOAT afMdRoiDilateR[PARAM_ISO_NUM];
    EI_U8 au8MdRoiMProf[PARAM_MD_PROFILE_NODE];
} ISP_PARAM_MD_S;

typedef struct eiISP_PARAM_AEC_S {
    EI_FLOAT afTarget[PARAM_ISO_NUM];
    EI_FLOAT afTolerance[PARAM_ISO_NUM];
    EI_FLOAT afBrightWeight[PARAM_ISO_NUM];
    EI_FLOAT afDrakWeight[PARAM_ISO_NUM];

    EI_U8 u8FastConvMode;
    EI_U8 u8MinLum;
    EI_FLOAT fExpTh;
    EI_FLOAT fExpFix;
    EI_FLOAT fExpRatio;

    EI_U32 u32InterpolateMode;
    EI_U32 u32FlickerMode;
    EI_U32 u32FlickerOffMs;
    EI_FLOAT fTimeOfLine;
    EI_U32 u32AeMode;
    EI_U32 u32AeDbgLvl;
    EI_U32 u32WeightDir;
    EI_U32 au32WeightWH[PARAM_AEC_WEIGHT_WH_MAX];
    EI_FLOAT afWinWeight[ISP_GRID_ZONE_NUM];
    EI_U32 u32GainDly;
    EI_U32 u32ExpDly;
    EI_U32 u32ExpDlyTime;
    EI_FLOAT fBvBias;
    EI_FLOAT fSvBias;

    EI_FLOAT afMeterTh[PARAM_THRESHOLD_TYPE_MAX];
    EI_FLOAT fAMeterSmooth;
    EI_FLOAT fExposureStart;

    EI_FLOAT afConvergence[PARAM_AEC_SMOOTH_MAX];

    EI_U8 u8ExpSetMode;
    EI_U8 u8SlowFpsEn;
    EI_U32 u32PlineLen;
    EI_FLOAT afPline[PARAM_MAX_PLINE_NUM][PARAM_PLINE_MAX];
    ISP_PARAM_AEC_SPOT_S stSpot;
} ISP_PARAM_AEC_S;

typedef struct eiISP_PARAM_AF_S {
    EI_U32 u32AfMode;
    EI_U32 u32AfDbgLvl;
    EI_U32 u32InitZoomCode;
    EI_U32 u32InitFocusCode;
    EI_U32 u32MinZoomCode;
    EI_U32 u32MaxZoomCode;
    EI_U32 u32MinFocusCode;
    EI_U32 u32MaxFocusCode;
    EI_U32 u32ManualZoom;
    EI_U32 u32ManualFocus;
    EI_U32 u32SkipFrames;
    EI_U16 u16TblLength;
    EI_U16 u16FocusTblTolerance;
    EI_U16 au16ZoomCodeTbl[PARAM_AF_DOF_TBL_SIZE];
    EI_U16 au16InfCodeTbl[PARAM_AF_DOF_TBL_SIZE];
    EI_U16 au16MicroCodeTbl[PARAM_AF_DOF_TBL_SIZE];
} ISP_PARAM_AF_S;

typedef struct eiISP_PARAM_LIGHTS_S {

    /* AWB param for lights */
    EI_CHAR  acLightName[PARAM_AWB_LIGTH_NAME_SIZE];
    EI_U32   u32LightTemp;
    EI_FLOAT fWeight;
    EI_FLOAT fJudgWeight;
    EI_FLOAT fTolerance;
    EI_FLOAT afPreference[PARAM_AWB_GAIN_MAX];
    EI_FLOAT afRedBlueGain[PARAM_AWB_GAIN_MAX];
    EI_FLOAT afPrincipalTh[PARAM_THRESHOLD_TYPE_MAX];

    /* CCM for lights */
    EI_FLOAT afCcmCoeff[PARAM_CCM_COEFF_NUM];
    EI_FLOAT afCcmOffset[PARAM_CCM_OFFS_NUM];
    EI_FLOAT afSaturationCb[PARAM_ISO_NUM];
    EI_FLOAT afSaturationCr[PARAM_ISO_NUM];

    /* LSC for lights */
    EI_FLOAT afLscStrength[PARAM_ISO_NUM];
} ISP_PARAM_LIGHTS_S;

typedef struct eiISP_PARAM_HCOLOR_S {
    EI_CHAR  acLightName[PARAM_AWB_LIGTH_NAME_SIZE];
    EI_U32   u32LightTemp;
    EI_FLOAT fJudgWeight;
    EI_S32   as32ProjLightIdx[PARAM_AWB_HINT_PROJECT_IDX_MAX];
    EI_FLOAT afRedBlueGain[PARAM_AWB_GAIN_MAX];
    EI_FLOAT fProbTth;
    EI_FLOAT fTolerance;
} ISP_PARAM_HCOLOR_S;

typedef struct eiISP_PARAM_MCOLOR_S {
    EI_BOOL  bMColorEn;
    EI_FLOAT afMColorOffset[PARAM_AWB_GAIN_MAX];
    EI_FLOAT afMColorPreference[PARAM_AWB_GAIN_MAX];
    EI_FLOAT afMColorProbTh[PARAM_THRESHOLD_TYPE_MAX];
    EI_U8 au8MColorDir[PARAM_AWB_GAIN_MAX];
    EI_U8 au8MColorLightRange[PARAM_AWB_HINT_PROJECT_IDX_MAX];
} ISP_PARAM_MCOLOR_S;

typedef struct eiISP_PARAM_AWB_S {
    /* Global AWB Param */
    EI_U32   u32AwbDbgLvl;
    EI_U32   u32AwbMode;
    EI_U32   u32SkipFrame;
    EI_U32   u32InterpolateMode;

    EI_FLOAT afBandWidth[PARAM_ISO_NUM];
    EI_FLOAT afMinProb[PARAM_ISO_NUM];
    EI_FLOAT afToleranceRatio[PARAM_ISO_NUM];

    /* Smooth Param */
    EI_BOOL  bSmoothEn;
    EI_FLOAT afSmoothTh[PARAM_THRESHOLD_TYPE_MAX];
    EI_FLOAT afSmoothStepAdd[PARAM_ISO_NUM];
    EI_FLOAT afSmoothStepSub[PARAM_ISO_NUM];
    EI_FLOAT afSmoothStableTh[PARAM_ISO_NUM];
    EI_U32   u32SmoothFrame;

    /* mix light Param */
    EI_BOOL  bMixLightEn;
    EI_FLOAT afMixLightMinSat;
    EI_FLOAT afMixLightOffset;
    EI_FLOAT afMixLightSlop;

    /* Lights Param */
    EI_U32   u32LightNum;
    EI_U32   u32HintNum;
    EI_U32   u32MColorNum;
    EI_U32   u32StartIdx;
    EI_U32   u32BvIlluNum;

    /* Green Enhance Param */
    EI_FLOAT   fGreenCoeff;
    EI_FLOAT   fGreenOffs;

    ISP_PARAM_LIGHTS_S astLights[PARAM_AWB_ILLUMINATION_MAX];
    ISP_PARAM_HCOLOR_S astHintColor[PARAM_AWB_HINT_COLOR_MAX];
    ISP_PARAM_MCOLOR_S astMColor[PARAM_AWB_MCOLOR_MAX];
    EI_FLOAT afBvWeight[PARAM_AWB_ILLUMINATION_MAX][PARAM_AWB_BV_NUM];
} ISP_PARAM_AWB_S;

typedef struct eiISP_PARAM_GAMMA_S {
    EI_U32 u32InterpolateMode;
    EI_FLOAT afPreGammaAlpha[PARAM_ISO_NUM];
    EI_U16 au16PreGamma[ISP_GAMMA_LUT_SIZE];
    EI_U16 au16PreGammaDark[ISP_GAMMA_LUT_SIZE];
    EI_FLOAT afGammaAlpha[PARAM_ISO_NUM];
    EI_U32 au32Curve[PARAM_GAMMA_POINT_NUM];
    EI_U32 au32CurveDark[PARAM_GAMMA_POINT_NUM];
    EI_FLOAT fGamma;
} ISP_PARAM_GAMMA_S;

typedef struct eiISP_PARAM_3DLUT_S {
    EI_U8 au8Lut[PARAM_3DLUT_POINT_NUM * 3];
} ISP_PARAM_3DLUT_S;

typedef struct eiISP_PARAM_LTM_S {
    EI_U32 u32LtmOn;
    EI_U32 u32Gain;
    EI_U32 u32Offset;
    EI_U32 u32SlopeMax;
    EI_U32 u32SlopeMin;
    EI_U32 u32BlackLevel;
    EI_U32 u32WhiteLevel;
    EI_U32 u32CollectionCorrection;
    EI_U32 u32FwdPerceptControl;
    EI_U32 u32RevPerceptControl;
    EI_U32 u32FilterMux;
    EI_U32 u32WbOffset;
    EI_U32 au32WbGain[ISP_RAW_CHN];
    EI_U32 u32GtmSelect;
    ISP_PARAM_LTM_CURVE_S stLtmCurveCfg;
    EI_U32 au32AsymmetryLut[ISP_LTM_LUT_SIZE];
    EI_U32 au32GlobaltmXLut[ISP_GTM_LUT_SIZE];
    EI_U32 au32GlobaltmYLut[ISP_GTM_LUT_SIZE];
    EI_U32 u32InterpolateMode;
    EI_U32 au32VarianceSpace[PARAM_ISO_NUM];
    EI_U32 au32VarianceIntensity[PARAM_ISO_NUM];
    EI_U32 au32Svariance[PARAM_ISO_NUM];
    EI_U32 au32BrightPr[PARAM_ISO_NUM];
    EI_U32 au32Contrast[PARAM_ISO_NUM];
    EI_U32 au32DarkEnh[PARAM_ISO_NUM];
    EI_U32 au32FwdAlpha[PARAM_ISO_NUM];
    EI_U32 au32RevAlpha[PARAM_ISO_NUM];
    EI_U32 au32StrengthInroi[PARAM_ISO_NUM];
    EI_U32 au32StrengthOutroi[PARAM_ISO_NUM];
    ISP_PARAM_LTM_AUTO_GAIN_S stLtmAutoGain;

    EI_FLOAT afAutoExpRatioLM[PARAM_ISO_NUM];
    EI_FLOAT afAutoExpRatioMS[PARAM_ISO_NUM];
    EI_FLOAT afAutoExpRatioSVS[PARAM_ISO_NUM];
} ISP_PARAM_LTM_S;

typedef struct eiISP_PARAM_ALSC_S {
    EI_FLOAT afMeshStren[PARAM_ISO_NUM];
    EI_FLOAT afLumLsc[PARAM_AWB_GAIN_MAX];
    EI_U32   u32CcMode;
    EI_FLOAT fCcStrength;
    EI_FLOAT fCcAccuracy;
    EI_U32   u32SkipFrame;
} ISP_PARAM_ALSC_S;

typedef struct eiISP_PARAM_H265_CBR_S {
    EI_U8   u8ParamEnable;
    EI_U8   u8NrEnable;
    EI_U8   u8SkipEnable;
    EI_U8   u8DeBlkEnable;
    EI_U8   u8MinQp;
    EI_U8   u8MaxQp;
    EI_U8   u8MinIQp;
    EI_U8   u8MaxIQp;
    EI_U8   u8IDRTargetRation;
    EI_U8   u8QpMapEn;
    EI_U8   u8QpOffset;
    EI_U8   u8QpSlop;
    EI_U8   u8QpThresh;
} ISP_PARAM_H265_CBR_S;

typedef struct eiISP_PARAM_H265_VBR_S {
    EI_U8   u8ParamEnable;
    EI_U8   u8NrEnable;
    EI_U8   u8SkipEnable;
    EI_U8   u8DeBlkEnable;
    EI_U8   u8MinQp;
    EI_U8   u8MaxQp;
    EI_U8   u8MinIQp;
    EI_U8   u8MaxIQp;
    EI_U8   u8QpMapEn;
    EI_U8   u8QpOffset;
    EI_U8   u8QpSlop;
    EI_U8   u8QpThresh;
} ISP_PARAM_H265_VBR_S;

typedef struct eiISP_PARAM_H265_AVBR_S {
    EI_U8   u8ParamEnable;
    EI_U8   u8NrEnable;
    EI_U8   u8SkipEnable;
    EI_U8   u8DeBlkEnable;
    EI_U8   u8MinQp;
    EI_U8   u8MaxQp;
    EI_U8   u8MinIQp;
    EI_U8   u8MaxIQp;
    EI_U8   u8QpMapEn;
    EI_U8   u8QpOffset;
    EI_U8   u8QpSlop;
    EI_U8   u8QpThresh;
} ISP_PARAM_H265_AVBR_S;

typedef struct eiISP_PARAM_H264_CBR_S {
    EI_U8   u8ParamEnable;
    EI_U8   u8NrEnable;
    EI_U8   u8SkipEnable;
    EI_U8   u8DeBlkEnable;
    EI_U8   u8MinQp;
    EI_U8   u8MaxQp;
    EI_U8   u8MinIQp;
    EI_U8   u8MaxIQp;
    EI_U8   u8IDRTargetRation;
    EI_U8   u8QpMapEn;
    EI_U8   u8QpOffset;
    EI_U8   u8QpSlop;
    EI_U8   u8QpThresh;
} ISP_PARAM_H264_CBR_S;

typedef struct eiISP_PARAM_H264_VBR_S {
    EI_U8   u8ParamEnable;
    EI_U8   u8NrEnable;
    EI_U8   u8SkipEnable;
    EI_U8   u8DeBlkEnable;
    EI_U8   u8MinQp;
    EI_U8   u8MaxQp;
    EI_U8   u8MinIQp;
    EI_U8   u8MaxIQp;
    EI_U8   u8QpMapEn;
    EI_U8   u8QpOffset;
    EI_U8   u8QpSlop;
    EI_U8   u8QpThresh;
} ISP_PARAM_H264_VBR_S;

typedef struct eiISP_PARAM_H264_AVBR_S {
    EI_U8   u8ParamEnable;
    EI_U8   u8NrEnable;
    EI_U8   u8SkipEnable;
    EI_U8   u8DeBlkEnable;
    EI_U8   u8MinQp;
    EI_U8   u8MaxQp;
    EI_U8   u8MinIQp;
    EI_U8   u8MaxIQp;
    EI_U8   u8QpMapEn;
    EI_U8   u8QpOffset;
    EI_U8   u8QpSlop;
    EI_U8   u8QpThresh;
} ISP_PARAM_H264_AVBR_S;

typedef struct eiISP_PARAM_VC_3DNR_S {
    EI_U8 u8Enable;
    EI_FLOAT afLumaCoeff[PARAM_ISO_NUM];
    EI_FLOAT afChromaCoeff[PARAM_ISO_NUM];
    EI_FLOAT afLumaMotionTH[PARAM_ISO_NUM];
    EI_FLOAT afChromaSADTH[PARAM_ISO_NUM];

    EI_U8 u8LumaCoeff;
    EI_U8 u8ChromaCoeff;
    EI_U8 u8LumaMotionTH;
    EI_U8 u8ChromaSADTH;
    EI_U8 au8MotionProfile[PARAM_TNR_PROFILE_NODE];
} ISP_PARAM_VC_3DNR_S;

typedef struct eiISP_PARAM_BG_Filter_S {
    EI_U8 u8Enable;
    EI_U8 u8FilterInBgModeEn;
    EI_U8 u8QPAdjustInBgModeEn;

    EI_FLOAT afMotionObjectTH[PARAM_ISO_NUM];
    EI_FLOAT afMotionObjectMVTH[PARAM_ISO_NUM];
    EI_FLOAT afWeakFilterPara[PARAM_ISO_NUM];
    EI_FLOAT afMiddleFilterPara[PARAM_ISO_NUM];
    EI_FLOAT afStrongFilterPara[PARAM_ISO_NUM];

    EI_U8 u8MotionObjectTH;
    EI_U8 u8MotionObjectMVTH;
    EI_U8 u8WeakFilterPara;
    EI_U8 u8MiddleFilterPara;
    EI_U8 u8StrongFilterPara;
} ISP_PARAM_BG_Filter_S;

typedef struct eiISP_PARAM_INTRA_CTRL_S {
    EI_U8 u8Enable;
    EI_FLOAT afICMotionOffset[PARAM_ISO_NUM];
    EI_FLOAT afICMotionSlop[PARAM_ISO_NUM];
    EI_FLOAT afICMotionThresh[PARAM_ISO_NUM];
} ISP_PARAM_INTRA_CTRL_S;

typedef struct eiISP_PARAM_RC_CFG_S {
    EI_U8 u8Enable;

    union {
        ISP_PARAM_H264_CBR_S stH264Cbr;
        EI_U8 au8Data0[32];
    };
    union {
        ISP_PARAM_H264_VBR_S stH264Vbr;
        EI_U8 au8Data1[32];
    };
    union {
        ISP_PARAM_H264_AVBR_S stH264Avbr;
        EI_U8 au8Data2[32];
    };
    union {
        ISP_PARAM_H265_CBR_S stH265Cbr;
        EI_U8 au8Data3[32];
    };
    union {
        ISP_PARAM_H265_VBR_S stH265Vbr;
        EI_U8 au8Data4[32];
    };
    union {
        ISP_PARAM_H265_AVBR_S stH265Avbr;
        EI_U8 au8Data5[32];
    };
    EI_U32 au32RcDbgLvl;

    ISP_PARAM_VC_3DNR_S stVc3Dnr;
    ISP_PARAM_BG_Filter_S stBgFilter;
    ISP_PARAM_INTRA_CTRL_S stIntraCtrl;
} ISP_PARAM_RC_CFG_S;

typedef struct eiISP_PARAM_RC_S {
    EI_FLOAT afRcMotion[PARAM_ISO_NUM];
    EI_FLOAT afRcOffset[PARAM_ISO_NUM];
    EI_FLOAT afRcSlop[PARAM_ISO_NUM];
    EI_FLOAT afRcThresh[PARAM_ISO_NUM];
    EI_FLOAT afIPQpDelta[PARAM_ISO_NUM];
    EI_FLOAT afMotionOffset[PARAM_ISO_NUM];
    EI_FLOAT afMotionSlop[PARAM_ISO_NUM];
    EI_FLOAT afMotionThresh[PARAM_ISO_NUM];
    EI_FLOAT afTextOffset[PARAM_ISO_NUM];
    EI_FLOAT afTextSlop[PARAM_ISO_NUM];
    EI_FLOAT afTextThresh[PARAM_ISO_NUM];
} ISP_PARAM_RC_S;

typedef struct eiISP_PARAM_CSC_S {
    EI_U32 u32ColorSpaceMode;
    EI_U32 s16UsrCscCoeff[PARAM_CCM_COEFF_NUM];
    EI_U32 s16UsrCscOffset[PARAM_CCM_OFFS_NUM];
} ISP_PARAM_CSC_S;

typedef struct eiISP_PARAM_TEMPER_S {
    EI_U8 u8Enable;
    EI_U8 u8Temper2Mode;
    EI_U8 u8LogEnable;
    EI_U8 u8Use12Bits;
    EI_U32 u32FrameDelay;
    EI_U8 u8ShowAlpha;
    EI_U8 u8ShowAlphaAb;
    EI_U8 u8MixerSelect;
    EI_U8 u8RecursionLimit;
    EI_FLOAT afDelta[PARAM_ISO_NUM];

    EI_U32 u32UseLut;
    EI_U32 u32UseExpMask;
    EI_U32 u32BlackReflect;
    EI_U32 u32BlackLevel;
    EI_U32 au32Thresh[ISP_NP_TH_NUM];
    EI_U32 au32NoiseLevel[ISP_WDR_MAX_CHN];
    EI_FLOAT afGlobalOffset[PARAM_ISO_NUM];
    EI_U32 au32NPLut[ISP_NP_LUT_SIZE];
} ISP_PARAM_TEMPER_S;

typedef struct eiISP_PARAM_TC_S {
    EI_U8 u8Enable;
    EI_S32 u32TempTriggerLow;
    EI_S32 u32TempTriggerHigh;
    EI_U32 u32BayerNrSkipEn;
    EI_FLOAT fMinFps;
    EI_FLOAT fFpsRatio;
    EI_FLOAT fAwbSkipRatio;
    EI_FLOAT afTempNode[PARAM_TC_NUM];
    EI_FLOAT afBnrCurve[PARAM_TC_NUM];
    EI_FLOAT afSharpCurve[PARAM_TC_NUM];
    EI_FLOAT afTnrCurve[PARAM_TC_NUM];
    EI_FLOAT afSnrCurve[PARAM_TC_NUM];
    EI_FLOAT afEeCurve[PARAM_TC_NUM];
    EI_FLOAT afAeTargetCurve[PARAM_TC_NUM];
    EI_FLOAT afDpcSlopeCurve[PARAM_TC_NUM];
    EI_FLOAT afDpcThCurve[PARAM_TC_NUM];
} ISP_PARAM_TC_S;

typedef struct eiISP_PARAM_OD_S {
    EI_U8  u8Enable;
    EI_U32 u32OdTh;             /* range: [0, 200000] */
    EI_U32 u32OdFrmCnt;         /* range: [0, 512] */
} ISP_PARAM_OD_S;

typedef struct eiISP_PARAM_IRD_S {
    EI_U8  u8Enable;
    EI_U8  u8IrDetEn;
    EI_U8  u8IrLightIdx;
    EI_U8  u8BvThRatio;
    EI_U8  u8IrdThRatio;
    EI_U8  u8NoWaitExp;
    EI_S32 s32BvLowTh;
    EI_S32 s32BvHighTh;
    EI_S32 s32BvAbsTh;
    EI_U32 u32IrdFrmCnt;

    EI_S8  s8IrOpenFlag;
} ISP_PARAM_IRD_S;

typedef struct eiISP_PARAM_FACE_S {
    EI_U8 u8Enable;
    EI_U8 u8SmoothCnt;
    EI_FLOAT fSmoothCoeff;

    EI_FLOAT afHighFreqStrenCurve[PARAM_ISO_NUM];
    EI_FLOAT afLowFreqStrenCurve[PARAM_ISO_NUM];
    EI_FLOAT afBnrHighFreqThCurve[PARAM_ISO_NUM];
    EI_FLOAT afBnrLowFreqThCurve[PARAM_ISO_NUM];
    EI_FLOAT afBnrGlobalOffsetCurve[PARAM_ISO_NUM];

    EI_FLOAT afRecursionLimitCurve[PARAM_ISO_NUM];
    EI_FLOAT afTemperGlobalOffsetCurve[PARAM_ISO_NUM];
    EI_FLOAT afSharpAltLdCurve[PARAM_ISO_NUM];
    EI_FLOAT afSharpAltLduCurve[PARAM_ISO_NUM];
    EI_FLOAT afSharpAltLuCurve[PARAM_ISO_NUM];
    EI_FLOAT afSharpStrengthCurve[PARAM_ISO_NUM];
    EI_FLOAT afClipStrMaxCurve[PARAM_ISO_NUM];
    EI_FLOAT afClipStrMinCurve[PARAM_ISO_NUM];
    EI_FLOAT afDpcSlopeCurve[PARAM_ISO_NUM];
    EI_FLOAT afDpcThCurve[PARAM_ISO_NUM];
    EI_FLOAT afDpcEdgeCurve[PARAM_ISO_NUM];

    EI_FLOAT afLumMotionThCurve[PARAM_ISO_NUM];
    EI_FLOAT afChromaMotionThCurve[PARAM_ISO_NUM];
    EI_FLOAT afShiftDistThCurve[PARAM_ISO_NUM];
    EI_FLOAT afLum2dSigmaCurve[PARAM_ISO_NUM];
    EI_FLOAT afChroma2dSigmaCurve[PARAM_ISO_NUM];
    EI_FLOAT afEEHvSlopCurve[PARAM_ISO_NUM];
    EI_FLOAT afEEStrength1Curve[PARAM_ISO_NUM];
    EI_FLOAT afEEStrength2Curve[PARAM_ISO_NUM];
    EI_FLOAT afEEStrengthTCurve[PARAM_ISO_NUM];

    EI_FLOAT afCnrUVDeltaSlopeCurve[PARAM_ISO_NUM];
    EI_FLOAT afCnrUVDeltaOffsetCurve[PARAM_ISO_NUM];
    EI_FLOAT afCnrUVDeltaThreshCurve[PARAM_ISO_NUM];
} ISP_PARAM_FACE_S;

typedef struct eiISP_PARAM_CS_S {
    EI_U8 u8Enable;

    EI_FLOAT afCsTh0[PARAM_ISO_NUM];
    EI_FLOAT afCsTh1[PARAM_ISO_NUM];
    EI_FLOAT afCsSlop0[PARAM_ISO_NUM];
    EI_FLOAT afCsSlop1[PARAM_ISO_NUM];
} ISP_PARAM_CS_S;

typedef struct eiISP_PARAM_DA_S {
    EI_U8 u8Enable;

    EI_FLOAT afLumMode[PARAM_ISO_NUM];
    EI_FLOAT afLumStrength[PARAM_ISO_NUM];
    EI_FLOAT afChromaMode[PARAM_ISO_NUM];
    EI_FLOAT afChromaStrength[PARAM_ISO_NUM];
} ISP_PARAM_DA_S;

typedef struct eiISP_FRAME_PRI_DATA_S {
    ISP_PARAM_RC_CFG_S *pstRcCfg;
    EI_U8 *pu8MotionMap;                    /* Size: 32*32 Bytes*/
    EI_U8 *pu8MotionQpMap;                  /* Size: 32*32 Bytes*/
} ISP_FRAME_PRI_DATA_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_COMM_ISP_PARAM_H__ */
