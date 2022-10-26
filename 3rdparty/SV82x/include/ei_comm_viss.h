/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_viss.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __EI_COMM_VISS_H__
#define __EI_COMM_VISS_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_debug.h"
#include "ei_comm_vbuf.h"
#include "ei_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define EI_TRACE_VISS(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_VISS, level, fmt, ##args);\
} while(0)

#ifndef ARRARSIZE
#define ARRARSIZE(x) (sizeof(x) / sizeof(*x))
#endif /* ARRARSIZE */

#define PRT_VISS_ERR(fmt, args...)  EI_TRACE_VISS(EI_DBG_ERR, fmt, ##args)
#define PRT_VISS_WARN(fmt, args...) EI_TRACE_VISS(EI_DBG_WARN, fmt, ##args)
#define PRT_VISS_INFO(fmt, args...) EI_TRACE_VISS(EI_DBG_INFO, fmt, ##args)
#define PRT_VISS_DBG(fmt, args...)  EI_TRACE_VISS(EI_DBG_DEBUG, fmt, ##args)

#define EI_ERR_VISS_NULL_PTR         EI_DEF_ERR(EI_ID_VISS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define EI_ERR_VISS_NOTREADY         EI_DEF_ERR(EI_ID_VISS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define EI_ERR_VISS_NOT_PERM         EI_DEF_ERR(EI_ID_VISS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define EI_ERR_VISS_NOMEM            EI_DEF_ERR(EI_ID_VISS, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define EI_ERR_VISS_ILLEGAL_PARAM    EI_DEF_ERR(EI_ID_VISS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define EI_ERR_VISS_BUSY             EI_DEF_ERR(EI_ID_VISS, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define EI_ERR_VISS_NOT_SUPPORT      EI_DEF_ERR(EI_ID_VISS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define EI_ERR_VISS_SIZE_NOT_ENOUGH  EI_DEF_ERR(EI_ID_VISS, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)

#define VISS_ERRNO_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define VISS_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
        if (cond) { \
                EI_TRACE_VISS(EI_DBG_ERR, "errno[%d]!\n", errno);\
                return (ret);\
        } \
} while(0)

/* add info ------viss begin------*/

#define VISS_DEV_MAX_CHN_NUM 4

typedef enum eiVISS_DVP_TYPE_E {
    VISS_DVP_DC,
    VISS_DVP_JPEG,
    VISS_DVP_BT601_M0,  /* mode 0: 8-bit HSYNC VSYNC */
    VISS_DVP_BT601_M1,  /* mode 1: 8-bit HSYNC FIELD */
    VISS_DVP_BT601_M2,  /* mode 2: 8-bit HSYNC VSYNC FIELD */
    VISS_DVP_BT601_M3,  /* mode 3: 16-bit HSYNC VSYNC */
    VISS_DVP_BT656,     /* mode 0: 8-bit */
    VISS_DVP_BT1120_M0, /* mode 0: 16-bit */
    VISS_DVP_BT1120_M1, /* mode 1: 8-bit */
    VISS_DVP_TYPE_MAX,
} VISS_DVP_TYPE_E;

typedef enum eiVISS_V_MODE_E {
    VISS_PROGRESSIVE,
    VISS_INTERLACED,
    VISS_V_MODE_MAX,
} VISS_V_MODE_E;

typedef enum eiVISS_YUV_SEQ_E {
    VISS_YUV_SEQ_C0C1C2C3,/* UYVY */
    VISS_YUV_SEQ_C2C1C0C3,/* VYUY */
    VISS_YUV_SEQ_C1C0C3C2,/* YUYV */
    VISS_YUV_SEQ_C3C0C1C2,/* YVYU */
    VISS_YUV_SEQ_MAX,
} VISS_YUV_SEQ_E;

typedef enum eiVISS_RGB_SEQ_E {
    VISS_RGB_SEQ_RGGB,
    VISS_RGB_SEQ_BGGR,
    VISS_RGB_SEQ_GRBG,
    VISS_RGB_SEQ_GBRG,
    VISS_RGB_SEQ_MAX,
} VISS_RGB_SEQ_E;

typedef enum eiVISS_WDR_MODE_E {
    VISS_WDR_MODE_VC,
    VISS_WDR_MODE_DT,
    VISS_WDR_MODE_DOL,
    VISS_WDR_MODE_LC,
    VISS_WDR_MODE_MAX,
} VISS_WDR_MODE_E;

typedef enum eiVISS_MIR_FLIP_E {
    VISS_MIR_FLIP_NO,
    VISS_MIR_FLIP_MIRROR,
    VISS_MIR_FLIP_FLIP,
    VISS_MIR_FLIP_M_AND_F,
    VISS_MIR_FLIP_MAX,
} VISS_MIR_FLIP_E;

typedef enum eiVISS_DEV_TYPE_E {
    DVP,
    MCSI,
    DVP_AND_MCSI,
    VISS_DEV_TYPE_MAX
} VISS_DEV_TYPE_E;

typedef struct eiVISS_RESOURCE_S {
    EI_VOID               *pvTopBase;
    EI_VOID               *pvWbDmaBase;
    EI_VOID               *apvDevBases[VISS_MAX_DEV_NUM];
    EI_S32                as32MdId[VISS_MAX_DEV_NUM];
    EI_S32                as32I2cBus[VISS_MAX_DEV_NUM];
    EI_S32                as32I2cAddr[VISS_MAX_DEV_NUM];
    EI_U32                au32Freq[VISS_MAX_DEV_NUM];
    VISS_DEV_TYPE_E       aenDevType[VISS_MAX_DEV_NUM];
    EI_U32                u32DevCount;
    EI_U32                au32Mclk[VISS_MAX_DEV_NUM];
    EI_U32                u32DevId[VISS_MAX_DEV_NUM];/* only use for eos */
} VISS_RESOURCE_S;

typedef struct eiVISS_PIC_TYPE_S {
    SIZE_S              stSize;
    PIXEL_FORMAT_E      enPixelFormat;
} VISS_PIC_TYPE_E;

typedef enum eiVISS_WORK_MODE_E {
    VISS_WORK_MODE_1Chn = 1,
    VISS_WORK_MODE_2Chn,
    VISS_WORK_MODE_3Chn,
    VISS_WORK_MODE_4Chn,
    VISS_WORK_MODE_MAX
} VISS_WORK_MODE_E;

typedef enum eiVISS_LANE_NUM_E {
    VISS_LANE_NUM_1Lane = 1,
    VISS_LANE_NUM_2Lane,
    VISS_LANE_NUM_3Lane,
    VISS_LANE_NUM_4Lane,
    VISS_LANE_NUM_MAX
} VISS_LANE_NUM_E;

typedef enum eiVISS_DEV_CHN_E {
    DEV_CHN0 = 1,
    DEV_CHN1,
    DEV_CHN2,
    DEV_CHN3,
    DEV_CHN_MAX,
} VISS_DEV_CHN_E;

typedef enum eiVISS_OUT_PATH_E {
    VISS_OUT_PATH_DMA = 0,
    VISS_OUT_PATH_PIXEL,
    VISS_OUT_PATH_DMA_AND_PIXEL,
    VISS_OUT_PATH_OFFLINE_ISP,
    VISS_OUT_PATH_MAX
} VISS_OUT_PATH_E;

typedef enum eiVISS_SORT_TYPE_E {
    VISS_HORIZONTAL = 0,
    VISS_VERTICAL,
    VISS_FIELD,
    VISS_SORT_TYPE_MAX
} VISS_SORT_TYPE_E;

typedef enum eiVISS_VIC_IO_INV_E {
    SLM_VS_INV  = 0x00020000,/* low */
    SLM_HS_INV  = 0x00010000,/* low */
    VS_INV      = 0x00000008,/* high */
    HS_INV      = 0x00000004,/* low */
    FIELD_INV   = 0x00000002,/* low */
    PCLK_INV    = 0x00000001,/* falling */
} VISS_VIC_IO_INV_E;

typedef enum eiVISS_PCLK_SM_E {
    VISS_SDR = 0,
    VISS_DDR,
    VISS_SM_MAX
} VISS_PCLK_SM_E;

typedef enum eiVISS_YC_SYNC_E {
    VISS_L8 = 0,
    VISS_H8,
    VISS_YC_SYNC_MAX
} VISS_YC_SYNC_E;

typedef enum eiVISS_CMD_TYPE_E {
    SET_IR = 1,
    SET_I2C,
    GET_I2C,
    SET_MOTOR,
    SHOW,
    ISNS,
    ESNS,
    EXIT_PROCESS,
    VISS_CMD_TYPE_MAX,
} VISS_CMD_TYPE_E;


typedef struct eiVISS_VIC_CFG_S {
    VISS_V_MODE_E       enVideoMode;
    EI_U32              u32IoInv;
    EI_U32              u32PclkPhase;/* unit 0.2us */
    VISS_PCLK_SM_E      enPclkSm;
    VISS_YC_SYNC_E      eYcSync;
    VISS_DVP_TYPE_E     eDvpType;
} VISS_VIC_CFG_S;

typedef struct eiVISS_MCSI_CFG_S {
    VISS_LANE_NUM_E enDlanes;
    EI_BOOL         bParseLPacket;
    EI_U32          u32PhyFreq;
    EI_U32          u32Settle;/* MCSI user */
} VISS_MCSI_CFG_S;

typedef struct eiVISS_OTHER_CFG_S {
    VISS_VIC_CFG_S    stVicCfg;
    VISS_MCSI_CFG_S   stMcsiCfg;
} VISS_OTHER_CFG_S;

typedef union eiVISS_DEV_CFG_S {
    VISS_VIC_CFG_S    stVicCfg;
    VISS_MCSI_CFG_S   stMcsiCfg;
    VISS_OTHER_CFG_S  stOtherCfg;
} VISS_DEV_CFG_S;

typedef struct eiVISS_DEV_ATTR_S {
    VISS_WORK_MODE_E        enWorkMode;
    VISS_CHN                aBindPhyChn[VISS_DEV_MAX_CHN_NUM];
    EI_S32                  mClock;
    SIZE_S                  stSize;                         /* RW;Input size */
    PIXEL_FORMAT_E          enPixelFormat;      /* RW;Pixel format */
    EI_BOOL                 enClb;
    EI_U32                  u32ModuleClk;
    VISS_WDR_MODE_E         enWdrMode;
    EI_CHAR                 *pcSnsName;

    VISS_DEV_CFG_S          stDevCfg;
    union {
        VISS_YUV_SEQ_E      enYuvSeq;
        VISS_RGB_SEQ_E      enRgbSeq;
    };

    VISS_OUT_PATH_E         enOutPath;    /* 0: DMA 1:ISP 2:DMA ISP 3: OFFLINE ISP*/
    EI_U32                  u32Fps;
    EI_U32                  u32IspClk;
    EI_U32                  u32IppuClk;
    WDR_MODE_E              enIspWdrMode;
} VISS_DEV_ATTR_S;

typedef struct eiVISS_CHN_ATTR_S {
    VISS_CHN            VissChn;
    SIZE_S              stSize;             /* RW;Channel out put size */
    PIXEL_FORMAT_E      enPixelFormat;      /* RW;Pixel format */
    PIXEL_FORMAT_E      enRealPFormat;      /* compatible encoding does not support Y8 config */
    VIDEO_FORMAT_E      enVideoFormat;      /* RW;Video format */
    EI_U32              u32Depth;           /* RW;Range [0,8];Depth */
    FRAME_RATE_CTRL_S   stFrameRate;        /* RW;Frame rate */
    VISS_MIR_FLIP_E     enMirrorFlip;  /* user cfg */
    EI_U32              u32Align;
    VISS_DEV_CHN_E      enDevSrcChn;
} VISS_CHN_ATTR_S;

typedef struct eiVISS_EXT_CHN_ATTR_S {
    VISS_CHN            s32ExtChn;
    VISS_CHN            s32BindChn;/* user cfg */     /* RW;Range [VISS_CHN0, VISS_MAX_PHY_CHN_NUM);
                                                        The channel num which extend channel will bind to*/
    SIZE_S              stSize;/* user cfg */         /* RW;Channel out put size */
    PIXEL_FORMAT_E      enPixFormat;    /* RW;Pixel format */
    EI_U32              u32Depth;/* user cfg */       /* RW;Range [0,8];Depth */
    FRAME_RATE_CTRL_S   stFrameRate;/*dst framerate user cfg */    /* RW;Frame rate */
    EI_U32              u32Align;
} VISS_EXT_CHN_ATTR_S;

typedef struct eiVISS_VC_CHN_ATTR_S {
    VISS_CHN            s32VcChn;
    VISS_CHN            s32BindPhyChn[VISS_MAX_VC_BIND_PHY_NUM];/* user cfg */     /* RW;Range [VISS_CHN0, VISS_MAX_PHY_CHN_NUM);
                                                                The channel num which extend channel will bind to*/
    EI_U32              u32PhyCnt;/* user cfg */
    VISS_SORT_TYPE_E    enSortType;/* user cfg */
    SIZE_S              stSize;         /* RW;Channel out put size */
    PIXEL_FORMAT_E      enPixFormat;    /* RW;Pixel format */
    EI_U32              u32Depth;/* user cfg */
    FRAME_RATE_CTRL_S   stFrameRate;/* user cfg */    /* RW;Frame rate */

    VISS_MIR_FLIP_E     enMirrorFlip;/* user cfg */
    EI_U32              u32Align;
} VISS_VC_CHN_ATTR_S;

typedef struct eiVISS_ROTATION_ATTR_S {
    EI_BOOL         bEnable;                 /* Whether ROTATE is enbale */
    ROTATION_E      enRotation;
    VISS_CHN            s32Chn;
    RECT_S          stRect;
} VISS_ROTATION_ATTR_S;


/* Information of chn crop */
typedef struct eiVISS_CROP_INFO_S {
    EI_BOOL                 bEnable;            /* RW;CROP enable*/
    RECT_S                  stCropRect;         /* RW;CROP rectangular*/
    VISS_CHN            VissChn;
} VISS_CROP_INFO_S;

typedef struct eiVISS_STATUS_S {
    EI_BOOL              bStatus;
    VISS_CMD_TYPE_E      enCmdType;
    EI_U32               u32Zoom;
    EI_U32               u32Focus;
    EI_U32               u32I2cReg;
    EI_U32               u32I2cVal;
    EI_BOOL              bIrStatus;
} VISS_STATUS_S;

/* add info ------viss end------*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_MLINK_H__ */

