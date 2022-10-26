/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_isp.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#ifndef __EI_COMM_ISP_H__
#define __EI_COMM_ISP_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_common.h"
#include "ei_math.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define ISP_HW_NUM                  (1)

#define ISP_MAX_DEV_NUM             (8)
#define ISP_BAYER_CHN_NUM           (4)

#define ISP_VREG_MAX_NUM            (16)

#define ISP_GRID_ZONE_ROW           (32)
#define ISP_GRID_ZONE_COL           (32)
#define ISP_GRID_ZONE_NUM           (ISP_GRID_ZONE_ROW * ISP_GRID_ZONE_COL)

#define AWB_STAT_SAMPLING           (2)
#define AWB_MAX_STAT_CNT            (ISP_GRID_ZONE_NUM/AWB_STAT_SAMPLING/AWB_STAT_SAMPLING)
#define AWB_ZONE_ROW                (ISP_GRID_ZONE_ROW)
#define AWB_ZONE_COL                (ISP_GRID_ZONE_COL)
#define AE_ZONE_ROW                 (ISP_GRID_ZONE_ROW)
#define AE_ZONE_COL                 (ISP_GRID_ZONE_COL)

#define ISP_PARAM_NAME_SIZE         (64)
#define ISP_HEADER_SIZE             (64*1024)

#define MAX_AWB_LIB_NUM                 (4)
#define MAX_AE_LIB_NUM                  (4)
#define MAX_AF_LIB_NUM                  (4)

#define CCM_MATRIX_SIZE                 (9)
#define CCM_MATRIX_NUM                  (7)

#define ISP_AUTO_ISO_STRENGTH_NUM       (16)
#define ISP_SHARPEN_LUMA_NUM            (32)
#define ISP_SHARPEN_GAIN_NUM            (32)

#define WDR_MAX_FRAME_NUM               (4)
#define BAYER_CALIBTAION_MAX_NUM        (50)

#define ISP_RLSC_POINTS                 (129)

#define GAMMA_NODE_NUM                  (1025)
#define PREGAMMA_NODE_NUM               (257)

#define HIST_THRESH_NUM                 (4)
#define EXP_RATIO_NUM                   (3)

#define ISP_EXP_RATIO_STRENGTH_NUM      (16)

#define ISP_CONFIG_PING    (1)
#define ISP_CONFIG_PONG    (0)
#define ISP_CONFIG_PING_SIZE    0x17FC0

#define ISP_ACTIVE_WIDTH    (1920)
#define ISP_ACTIVE_HEIGHT   (1080)

#define ISP_METERING_ZONES_MAX_V    (33)
#define ISP_METERING_ZONES_MAX_H    (33)
#define ISP_METERING_OFFSET_AE    (0)
#define ISP_METERING_OFFSET_AWB    (2192)
#define ISP_METERING_OFFSET_AF    (4384)
#define ISP_METERING_MEM_END    (6575)

#define ISP_WDR_MAX_CHN     (4)
#define ISP_FE_LUT_NUM      (2)
#define ISP_RAW_CHN     (4)
#define ISP_RGBIR_NUM       (4)
#define BLACK_LEVEL_SHIFT   (8)

#define ISP_GAMMA_LUT_SIZE  (65)

#define ISP_MAX_METER_ZONES   (ISP_METERING_ZONES_MAX_V*ISP_METERING_ZONES_MAX_H)
#define ISP_AE_HIST_BINS      (1024)

#define ISP_NP_LUT_SIZE     (128)

#define ISP_GTM_LUT_SIZE    (33)
#define ISP_LTM_LUT_SIZE    (65)

#define ISP_IF_KNEE_POINT   (3)
#define ISP_IF_SLOPE_SEL    (4)

#define ISP_SINTER_TH_NUM   (4)
#define ISP_SINTER_STREN_NUM    (4)
#define ISP_NP_TH_NUM       (3)

#define ISP_HIST_PLANE_NUM  (4)

#define ISP_MAB_CH_NUM      (4)
#define ISP_MAB_PALETTE_NUM (4)
#define ISP_MAB_IM_NUM      (8)

#define ISP_MEM_CMD_QUEUE_SIZE      (256)
#define ISP_MEM_LUT_3D_SIZE         (1000)
#define ISP_MEM_SCALER_SIZE         (512)
#define ISP_MEM_HSIT_SIZE           (1024)
#define ISP_MEM_METERING_STATS_SIZE (6575)
#define ISP_MEM_DECOMPANDER0_SIZE   (33)
#define ISP_MEM_DECOMPANDER1_SIZE   (257)
#define ISP_MEM_DPC_SIZE            (4096)
#define ISP_MEM_CAC_FILTER_SIZE     (32)
#define ISP_MEM_CAC_MESH_SIZE       (4096)
#define ISP_MEM_MESH_SHADING_SIZE   (4096)

#define ISP_MEM_RADIAL_SHADING_R_OFFS  (0)
#define ISP_MEM_RADIAL_SHADING_G_OFFS  (256)
#define ISP_MEM_RADIAL_SHADING_B_OFFS  (512)
#define ISP_MEM_RADIAL_SHADING_IR_OFFS (768)
#define ISP_MEM_RADIAL_COMP_VALID_SIZE (129)
#define ISP_MEM_RADIAL_SHADING_SIZE    (897)

#define ISP_MEM_IRIDIX_RP_SIZE      (129)
#define ISP_MEM_IRIDIX_FP1_SIZE     (129)
#define ISP_MEM_GAMMA_SIZE          (129)
#define ISP_MEM_LUMVAR_SIZE         (512)

typedef enum eiISP_BAYER_FORMAT_E {
    ISP_BAYER_RGGB = 0,
    ISP_BAYER_GRBG,
    ISP_BAYER_GBRG,
    ISP_BAYER_BGGR,
    ISP_BAYER_MAX
} ISP_BAYER_FORMAT_E;

typedef enum eiISP_IN_BIT_E {
    ISP_IN_BITS_12 = 0,
    ISP_IN_BITS_14,
    ISP_IN_BITS_16,
    ISP_IN_BITS_20,
} ISP_IN_BIT_E;

enum isp_out_bits {
    ISP_OUT_BITS_20 = 0,
    ISP_OUT_BITS_16,
    ISP_OUT_BITS_14,
    ISP_OUT_BITS_12,
};

enum isp_cfa_pattern {
    ISP_CFA_PATTERN_RGGB = 0,
    ISP_CFA_PATTERN_RESERVED,
    ISP_CFA_PATTERN_RIRGB,
    ISP_CFA_PATTERN_RGIRB,
};

enum isp_data_src {
    ISP_DATA_SRC_DIRECTLY_FROM_SENSOR = 0,
    ISP_DATA_SRC_FROM_FRAME_STITCH,
    ISP_DATA_SRC_THROUGH_DECOMPANDER,
};

enum isp_ae_switch {
    AE_SWITCH_AFTER_WB = 0,
    AE_SWITCH_AFTER_WDR_FRAME_STITCH,
    AE_SWITCH_AFTER_VTPG,
};

enum isp_af_switch {
    AF_SWITCH_AFTER_SINTER = 0,
    AF_SWITCH_BEFORE_SINTER,
};

enum isp_awb_switch {
    AWB_SWITCH_AFTER_DEMOSAIC = 0,
    AWB_SWITCH_AFTER_CNR,
};

enum isp_aexp_hist_switch {
    HIST_SWITCH_AFTER_WB = 0,
    HIST_SWITCH_AFTER_WDR_FRAME_STITCH,
    HIST_SWITCH_AFTER_VTPG,
};

enum isp_lumvar_switch {
    LUMAVAR_SWITCH_FULL_RESOLUTION_PIPELINE = 0,
    LUMAVAR_SWITCH_DOWNSCALED_PIPELINE,
};

enum isp_aexp_src {
    AEXP_SRC_AFTER_WB_WHEN_APPLIED_BEFORE_SHADING = 0,
    AEXP_SRC_AFTER_WB_WHEN_APPLIED_AFTER_SHADING,
};

enum isp_input_formatter_mode_in {
    ISP_IF_MODE_IN_LINEAR_DATA = 0,
    ISP_IF_MODE_IN_23_MULTIPLE_EXPOSURE_MULTIPLEXING,
    ISP_IF_MODE_IN_LOGARITHMIC_ENCODING,
    ISP_IF_MODE_IN_COMPANDING_CURVE_WITH_KNEE_POINTS,
    ISP_IF_MODE_IN_16BIT_LINEAR_12BIT_VS,
    ISP_IF_MODE_IN_12BIT_COMPANDED_12BIT_VS,
    ISP_IF_MODE_IN_RESERVED,
    ISP_IF_MODE_IN_PASS_THROUGH_MODE,
};

enum isp_input_formatter_bit_width {
    ISP_IF_INPUT_BITWIDTH_SELECT_8_BITS = 0,
    ISP_IF_INPUT_BITWIDTH_SELECT_10_BITS,
    ISP_IF_INPUT_BITWIDTH_SELECT_12_BITS,
    ISP_IF_INPUT_BITWIDTH_SELECT_14_BITS,
    ISP_IF_INPUT_BITWIDTH_SELECT_16_BITS,
    ISP_IF_INPUT_BITWIDTH_SELECT_20_BITS,
};

enum isp_input_formatter_slop_select {
    ISP_IF_SLOPE0_SELECT_1X = 0,
    ISP_IF_SLOPE0_SELECT_2X,
    ISP_IF_SLOPE0_SELECT_4X,
    ISP_IF_SLOPE0_SELECT_8X,
    ISP_IF_SLOPE0_SELECT_16X,
    ISP_IF_SLOPE0_SELECT_32X,
    ISP_IF_SLOPE0_SELECT_64X,
    ISP_IF_SLOPE0_SELECT_128X,
    ISP_IF_SLOPE0_SELECT_256X,
    ISP_IF_SLOPE0_SELECT_512X,
    ISP_IF_SLOPE0_SELECT_1024X,
    ISP_IF_SLOPE0_SELECT_2048X,
    ISP_IF_SLOPE0_SELECT_4096X,
    ISP_IF_SLOPE0_SELECT_8192X,
    ISP_IF_SLOPE0_SELECT_16384X,
    ISP_IF_SLOPE0_SELECT_32768X,
};

enum isp_fs_mode_in {
    ISP_FRAME_DEFAULT = 0,
    ISP_FRAME_2TO1,
    ISP_FRAME_3TO1,
    ISP_FRAME_4TO1,
};

enum iproc_ch_num {
    IPROC_CH0_VALID = 0,
    IPROC_CH0_1_VALID,
    IPROC_CH0_2_VALID,
    IPROC_CH0_3_VALID
};

enum iproc_dma_mode {
    IPROC_DATA_FROME_INTERFACE = 0,
    IPROC_DATA_FROME_RDMA,
    IPROC_DATA_FROME_INTERFACE_AND_RDMA,
};

enum iproc_rdma_bits {
    IPROC_RDMA_12BITS = 0,
    IPROC_RDMA_12BITS_COMPACT,
    IPROC_RDMA_16BITS,
    IPROC_RDMA_20BITS,
    IPROC_RDMA_8BITS,
};

typedef enum eiISP_PARAM_TYPE_E {
    PARAM_TYPE_AUTO    = 0,
    PARAM_TYPE_MANUAL  = 1,
    PARAM_TYPE_MAX
} ISP_PARAM_TYPE_E;

typedef enum eiWDR_MODE_E {
    WDR_MODE_NONE = 0,
    WDR_MODE_BUILD_IN,
    WDR_MODE_2TO1_FRAME,
    WDR_MODE_3TO1_FRAME,
    WDR_MODE_MAX,
} WDR_MODE_E;

typedef enum eiEXP_MODE_E {
    EXP_MODE_MANUAL = 0,
    EXP_MODE_AUTO1,
    EXP_MODE_AUTO2,
    EXP_MODE_GAIN_TEST,
    EXP_MODE_TIME_TEST,
    EXP_MODE_GAIN_TIME_TEST,
    EXP_MODE_MAX,
} EXP_MODE_E;

typedef enum eiWB_MODE_E {
    WB_MODE_MANUAL = 0,
    WB_MODE_AUTO,
    WB_MODE_HOR,
    WB_MODE_A,
    WB_MODE_U30,
    WB_MODE_TL84,
    WB_MODE_CWF,
    WB_MODE_D50,
    WB_MODE_D65,
    WB_MODE_D75,
    WB_MODE_MAX,
} WB_MODE_E;

typedef struct eiISP_RESOURCE_S {
    EI_U32 u32IspDev;
    EI_VOID *pIspBase;
    EI_S32 s32IspCoreIrq;
    EI_S32 s32IppuChnIrq;
    EI_S32 s32IspIprocIrq;
    EI_U32 u32IspClk;
    EI_U32 u32IppuClk;
} ISP_RESOURCE_S;

typedef struct eiISP_INIT_ATTR_S {
    EI_U32 u32ExpTime;
    EI_U32 u32AGain;
    EI_U32 u32DGain;
    EI_U32 u32ISPDGain;
    EI_U32 u32Exposure;
    EI_U32 u32PirisFNO;
    EI_U16 u16WBRgain;
    EI_U16 u16WBGgain;
    EI_U16 u16WBBgain;
} ISP_INIT_ATTR_S ;

typedef enum eiPARAM_INTERP_TYPE {
    PARAM_INTERP_SV = 0,
    PARAM_INTERP_BV,

    PARAM_INTERP_TYPE_MAX
} PARAM_INTERP_TYPE;

typedef struct eiISP_APEX_S {
    EI_FLOAT Aperture;
    EI_FLOAT AV;
    EI_FLOAT SV;
    EI_FLOAT BV;
    EI_FLOAT TV;
    EI_FLOAT EV;
} ISP_APEX_S;

typedef enum eiISP_RUNNING_MODE_E {
    ISP_MODE_RUNNING_OFFLINE = 0,
    ISP_MODE_RUNNING_ONLINE,

    ISP_MODE_RUNNING_MAX,
} ISP_RUNNING_MODE_E;

#define IS_OFFLINE_MODE(mode)    (ISP_MODE_RUNNING_OFFLINE == (mode))
#define IS_ONLINE_MODE(mode)     (ISP_MODE_RUNNING_ONLINE == (mode))

typedef enum eiISP_OFFLINE_TYPE_E {
    ISP_OFFLINE_LINK = 0,
    ISP_OFFLINE_SEND_FRAMEINFO,
    ISP_OFFLINE_SEND_PHYADDR,

    ISP_OFFLINE_MAX,
} ISP_OFFLINE_TYPE_E;

typedef enum eiISP_IN_FMT_E {
    ISP_IN_FMT_RAW = 0,
    ISP_IN_FMT_YUV420,
    ISP_IN_FMT_YUV422,

    ISP_IN_FMT_MAX,
} ISP_IN_FMT_E;

typedef struct eiISP_PUB_ATTR_S {
    EI_S32          s32IspDev;
    EI_U32          u32DataSrc;
    EI_U32          u32IspClk;
    EI_U32          u32IppuClk;
    EI_BOOL         bTnrEn;
    EI_BOOL         bTemperEn;
    EI_BOOL         bTemper2;
    EI_BOOL         bPreAlloc3D;
    EI_BOOL         bUse12Bits;
    RECT_S          stWndRect;
    SIZE_S          stSnsSize;
    EI_CHAR         *pcSnsName;
    EI_CHAR         *pcParamName;
    EI_U32          u32SnsFps;
    WDR_MODE_E      enWDRMode;
    ISP_IN_FMT_E    enInFmt;
    ISP_IN_BIT_E    enBitW;
    ISP_BAYER_FORMAT_E  enBayer;
    ISP_RUNNING_MODE_E  enRunningMode;
    FRAME_RATE_CTRL_S   stFrc;
} ISP_PUB_ATTR_S;

typedef struct eiISP_MODULE_BYPASS_S {
    EI_BOOL bBypassTestGenerator;    /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassInputFormatter;   /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassDecompander;      /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassSoWdr;            /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassGainWdr;          /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassFrameStitch;      /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassDgain;            /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassSo;               /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassSqrt;             /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassFe;               /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassStaticDpc;        /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassSinter;           /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassTemper;           /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassCac;              /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassSquare;           /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassSoShading;        /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassRadialShading;    /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassMeshShading;      /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassWb;               /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassLtmGain;          /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassLtm;              /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassMirror;           /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassDemosaic;         /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassPfc;              /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassCcm;              /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassCnr;              /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassCLut;             /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassGamma;            /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassFrCrop;           /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassFrGamma;          /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassFrSharpen;        /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassFrCsc;            /* range: [0, 1], bypass = 1 means no use this module */
    EI_BOOL bBypassIspRaw;           /* range: [0, 1], bypass = 1 means no use this module */
} ISP_MODULE_BYPASS_S;

typedef struct eiISP_EFFECT_ATTR_S {
    EI_U8 u8Contrast;           /* range: [0, 255], default 128 */
    EI_U8 u8Saturation;         /* range: [0, 255], default 128 */
    EI_S8 s8Brightness;         /* range: [-128, 127], default 0 */
    EI_S8 s8Hue;                /* range: [-90, 90], default 0 */
    EI_S16 s16Sharpness;        /* range: [-1024, 1024], default 0 */
} ISP_EFFECT_ATTR_S;

typedef struct eiISP_AE_SPOT_ATTR_S {
    EI_U32 u32Mode;             /* range: 0 ~1, 0: average, 1: spot */
    EI_U32 u32WinNum;           /* ae spot win num, max is 16 */
    RECT_S stRoiWin[16];        /* ae roi win */
    EI_S32 s32Chn;              /* for isp get the src w/h, and rot */
} ISP_AE_SPOT_ATTR_S;

typedef struct eiISP_AE_ATTR_S {
    EI_U32 u32AeMode;           /* range: [0, 5], 0: manual, 1, 2: auto, 3,4,5: test */
    EI_U32 u32AePriority;       /* 0: exposure by ae pline in json, 1: shutter priority, 2: gain priority, 3 aperture priority*/
    EI_U32 u32FlickerMode;      /* 0: off, 1: 50HZ, 2: 60HZ*/
    EI_FLOAT fAeSpeed;          /* UnderExp range: [0.0, 1.0]*/
    EI_FLOAT fIntTime;          /* sensor exposure time, unit is s */
    EI_FLOAT fGain;             /* sensor exposure gain, unit is 1.0 */
    EI_FLOAT fHighlightWeight;  /* range: [0.1, 10], only active in linear mode, default 1.0 */
    EI_FLOAT fLowlightWeight;   /* range: [0.1, 10], only active in linear mode, default 1.0 */
    EI_FLOAT fFps;              /* range: [0.1, MAX SENOR FPS], when  u32Fps < 0.1 will not set fps to driver */

    EI_S32 s32ExpBias;          /* range: -3 ~3 */
    EI_U32 u32AeTarget;         /* range: 0 ~200, when u32AeTarget is zero, ae will use target in json */

    ISP_AE_SPOT_ATTR_S stAeSpot;/*  */
} ISP_AE_ATTR_S;

typedef struct eiISP_AF_ATTR_S {
    EI_U32 u32AfMode;           /* range: [MANUAL = 0, CAF = 1, SAF = 2, SPOT = 3, FULLRANGE = 4]*/
    EI_U32 u32ManualZoom;       /* range: [0, 1023] */
    EI_U32 u32ManualFocus;      /* range: [0, 1023] */
} ISP_AF_ATTR_S;

typedef struct eiISP_AWB_ATTR_S {
    EI_U32 u32AwbMode;      /* range: [0, 9], 0: manual, 1: auto, 2: HOR, 3: A, 4: U30, 5: TL84, 6: CWF, 7: D50, 8: D65, 9: D75 */
    EI_FLOAT afWbGain[4];   /* range: [1.0, 8.0] */
} ISP_AWB_ATTR_S;

typedef struct eiISP_NR_ATTR_S {
    EI_BOOL bTnrEnable;     /* range: [0, 1] */
    EI_U32 u32TnrStrength;  /* range: [0, 255], default 0 */
} ISP_NR_ATTR_S;

typedef struct eiISP_DEFOG_ATTR_S {
    EI_BOOL bDefogEnable;       /* range: [0, 1] */
    EI_U32 u32DefogStrength;    /* range: [0, 255], default 0 */
} ISP_DEFOG_ATTR_S;

typedef struct eiISP_WB_META_DATA_S {
    EI_FLOAT afWbGainsOrg[ISP_RAW_CHN];
    EI_FLOAT afWbGainsPref[ISP_RAW_CHN];
} ISP_WB_META_DATA_S;

typedef struct eiISP_META_DATA_S {
    ISP_WB_META_DATA_S stWbData;
    EI_U8  au8MotionMap[ISP_GRID_ZONE_NUM];
    EI_U8  au8MotionQpMap[ISP_GRID_ZONE_NUM];
    EI_U8  au8MotionVal[ISP_GRID_ZONE_NUM];
    EI_U8  au8QpMap[ISP_GRID_ZONE_NUM];
    EI_S32 s32TempIdx;
    EI_CHAR acParamName[ISP_PARAM_NAME_SIZE];
    EI_CHAR acHeader[ISP_HEADER_SIZE];
    EI_CHAR acPath[16];
    EI_BOOL bReloadJson;
    EI_BOOL bSaveJson;
    EI_BOOL bSaveHeader;
} ISP_META_DATA_S ;

typedef struct eiISP_OD_ATTR_S {
    EI_U8  u8Enable;
    EI_U32 u32OdTh;             /* range: [0, 200000] */
    EI_U32 u32OdFrmCnt;         /* range: [0, 512] */

    EI_U32 u32OdFlag;           /* output value, range: [0, 1] 0: no occlusion detected,  1: occlusion detected*/
    EI_U32 u32AvgVal;           /* output value, range: [0, 0xffffffff] */
} ISP_OD_ATTR_S;

typedef struct eiISP_IRD_ATTR_S {
    EI_U8  u8Enable;
    EI_U8  u8IrDetEn;           /*RGB: set 0, BW: set 1*/
    EI_U8  u8IrLightIdx;        /*range: [5, 7], default 7*/
    EI_U8  u8BvThRatio;         /*range: [0, 100], default 50*/
    EI_U8  u8IrdThRatio;        /*range: [0, 100], default 50*/
    EI_U32 u8NoWaitExp;         /*range: [0, 1], default 1*/
    EI_S32 s32BvLowTh;          /*range: [-65536, 65535], default -200*/
    EI_S32 s32BvHighTh;         /*range: [-65536, 65535], default 200*/
    EI_U32 s32BvAbsTh;          /*range: [-65536, 65535], default 1800, over this value would close ir immediately*/
    EI_U32 u32IrdFrmCnt;        /*range: [0, 512], default 4*/

    EI_S8  s8IrOpenFlag;        /*range: [-1, 1], -1: invalid value, 0: set Rgb mode, 1: set BW mode*/
} ISP_IRD_ATTR_S;

typedef struct eiISP_GAMMA_ATTR_S {
    EI_FLOAT fGamma;            /*range: [1.0, 3.0], */
} ISP_GAMMA_ATTR_S;

typedef struct eiISP_CSC_ATTR_S {
    EI_U8 u8Brightness; /*range: [0, 255], */
    EI_U8 u8Contrast;   /*range: [0, 255], */
    EI_U8 u8Hue;        /*range: [0, 255], */
    EI_U8 u8Saturation; /*range: [0, 255], */
} ISP_CSC_ATTR_S;

typedef struct eiISP_DWDR_ATTR_S {
    EI_U8 u8Enable;     /*range: [0, 1], */
    EI_U8 u8Strength;   /*range: [0, 255], */
} ISP_DWDR_ATTR_S;

typedef struct eiISP_AE_STATISTICS_S {
    EI_U32 au32Histogram[ISP_AE_HIST_BINS];     /*range: [0x0, 0xFFFFFFFF]; 1024 bins histogram*/
    EI_U32 u32PixelCount;
    EI_U8 au8MeanR[ISP_GRID_ZONE_NUM];          /*range: [0, 255]; Format:8.0, 32*32 grid mean r*/
    EI_U8 au8MeanGr[ISP_GRID_ZONE_NUM];         /*range: [0, 255]; Format:8.0, 32*32 grid mean gr*/
    EI_U8 au8MeanGb[ISP_GRID_ZONE_NUM];         /*range: [0, 255]; Format:8.0, 32*32 grid mean gb*/
    EI_U8 au8MeanB[ISP_GRID_ZONE_NUM];          /*range: [0, 255]; Format:8.0, 32*32 grid mean b*/
} ISP_AE_STATISTICS_S;

typedef struct eiISP_AWB_STATISTICS_S {
    EI_U32 au32Rg[ISP_GRID_ZONE_NUM];           /*range: [0x0, 0xFFFFFFFF]; Format:24.8, 32*32   grid R/G*/
    EI_U32 au32Bg[ISP_GRID_ZONE_NUM];           /*range: [0x0, 0xFFFFFFFF]; Format:24.8, 32*32 grid  B/G*/
} ISP_AWB_STATISTICS_S;

typedef struct eiISP_AF_STATISTICS_S {
    EI_U64 u64Sharp[ISP_MAX_METER_ZONES];       /*range: [0x0, 0xFFFFFFFFFFFFFFFF]; Format:64.0,  32*32 grid*/
    EI_U32 u32SharpReliablility[ISP_MAX_METER_ZONES];   /*range: [0x0, 0xFFFFFFFF]; Format:32.0,  32*32 grid*/
} ISP_AF_STATISTICS_S;

typedef enum eiISP_ERR_CODE_E {
    ERR_ISP_NOT_INIT                = 0x40,
    ERR_ISP_MEM_NOT_INIT            = 0x41,
    ERR_ISP_ATTR_NOT_CFG            = 0x42,
    ERR_ISP_SNS_UNREGISTER          = 0x43,
    ERR_ISP_INVALID_ADDR            = 0x44,
    ERR_ISP_NO_INT                  = 0x46,
    ERR_ISP_OUT_OF_RANGE            = 0x47,
} ISP_ERR_CODE_E;

#define EI_ERR_ISP_NOT_INIT         EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_NOT_INIT          )
#define EI_ERR_ISP_MEM_NOT_INIT     EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_MEM_NOT_INIT      )
#define EI_ERR_ISP_ATTR_NOT_CFG     EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_ATTR_NOT_CFG      )
#define EI_ERR_ISP_SNS_UNREGISTER   EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_SNS_UNREGISTER    )
#define EI_ERR_ISP_INVALID_ADDR     EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_INVALID_ADDR      )
#define EI_ERR_ISP_NO_INT           EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_NO_INT            )
#define EI_ERR_ISP_OUT_OF_RANGE     EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_OUT_OF_RANGE      )

#define EI_ERR_ISP_INVALID_DEVID    EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID      )
#define EI_ERR_ISP_INVALID_CHNID    EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID      )
#define EI_ERR_ISP_ILLEGAL_PARAM    EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM      )
#define EI_ERR_ISP_EXIST            EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST              )
#define EI_ERR_ISP_UNEXIST          EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST            )
#define EI_ERR_ISP_NULL_PTR         EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR           )
#define EI_ERR_ISP_NOT_CONFIG       EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG         )
#define EI_ERR_ISP_NOT_SUPPORT      EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT        )
#define EI_ERR_ISP_NOT_PERM         EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM           )
#define EI_ERR_ISP_LIST_EMPTY       EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_LIST_EMPTY         )
#define EI_ERR_ISP_NOMEM            EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM              )
#define EI_ERR_ISP_NOBUF            EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF              )
#define EI_ERR_ISP_BUF_EMPTY        EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY          )
#define EI_ERR_ISP_BUF_FULL         EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL           )
#define EI_ERR_ISP_SYS_NOTREADY     EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY       )
#define EI_ERR_ISP_BADADDR          EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR            )
#define EI_ERR_ISP_BUSY             EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY               )
#define EI_ERR_ISP_SIZE_NOT_ENOUGH  EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH    )
#define EI_ERR_ISP_AGAIN            EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_AGAIN              )
#define EI_ERR_ISP_TIMEOUT          EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_TIMEOUT            )
#define EI_ERR_ISP_NOTSUPP          EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NOTSUPP            )
#define EI_ERR_ISP_NOT_ENABLED      EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_ENABLED        )
#define EI_ERR_ISP_NO_FOUND         EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NO_FOUND           )
#define EI_ERR_ISP_OTHERS           EI_DEF_ERR(EI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_OTHERS             )

#define ISP_RETURN_IF(cond)  do {if (cond) {return;} else {}} while(0)
#define ISP_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define EI_TRACE_ISP(level, fmt, args...)\
    do{ \
        EI_TRACE(EI_ID_ISP, level, fmt, ##args);\
    } while(0)

#define ISP_CHECK_RETURN(ret)  do {if (ret) {return (ret);} else {}} while(0)

#define ISP_CHECK_DEV(dev) \
    do {\
        if (((dev) < 0) || ((dev) >= ISP_MAX_DEV_NUM))\
        {\
            EI_TRACE_ISP(EI_DBG_ERR, "Err isp dev %d!\n", dev);\
            return EI_ERR_ISP_NOT_SUPPORT;\
        }\
    }while(0)

#define DIV_0_TO_1(a)   ( (0 == (a)) ? 1 : (a) )
#define DIV_0_TO_1_FLOAT(a) ((((a) < 1E-10) && ((a) > -1E-10)) ? 1 : (a))

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_COMM_ISP_H__ */
