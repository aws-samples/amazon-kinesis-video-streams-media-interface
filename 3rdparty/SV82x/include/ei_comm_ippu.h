/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_ippu.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#ifndef __EI_COMM_IPPU_H__
#define __EI_COMM_IPPU_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_common.h"
#include "ei_comm_isp.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define IPPU_PHY_CHN_MAX_NUM         (3)
#define IPPU_EXT_CHN_MAX_NUM         (3)
#define IPPU_CHN_MAX_NUM             (IPPU_PHY_CHN_MAX_NUM+IPPU_EXT_CHN_MAX_NUM)

typedef enum eiIPPU_RUNNING_MODE_E {
    IPPU_MODE_RUNNING_OFFLINE = 0,
    IPPU_MODE_RUNNING_ONLINE,

    IPPU_MODE_RUNNING_MAX,
} IPPU_RUNNING_MODE_E;

typedef struct eiIPPU_DEV_ATTR_S {
    EI_S32                  s32IppuDev;
    EI_U32                  u32InputWidth;
    EI_U32                  u32InputHeight;
    EI_U32                  u32DataSrc;
    IPPU_RUNNING_MODE_E     enRunningMode;
    FRAME_RATE_CTRL_S       stFrameRate;
} IPPU_DEV_ATTR_S;

typedef struct eiIPPU_CHN_ATTR_S {
    EI_S32              s32Chn;
    EI_U32              u32Width;
    EI_U32              u32Height;
    EI_U32              u32Align;
    VIDEO_FORMAT_E      enVideoFormat;
    PIXEL_FORMAT_E      enPixelFormat;
    DYNAMIC_RANGE_E     enDynamicRange;
    COMPRESS_MODE_E     enCompressMode;
    FRAME_RATE_CTRL_S   stFrameRate;
    EI_BOOL             bMirror;
    EI_BOOL             bFlip;
    EI_BOOL             bNoFifo;        /* 0: FIFO mode, driver and user would occupy more than u32Depth buffers,
                                         * 1: NO FIFO, driver and user can occupy at most u32Depth buffers
                                         */
    EI_BOOL             bSnapFrame;     /* support one buffer for hardware and user use at snap frame mode */
    EI_U32              u32Depth;
    EI_U32              u32DropCnt;     /* drop frame count when start phy ch */
} IPPU_CHN_ATTR_S;

typedef struct eiIPPU_EXT_CHN_ATTR_S {
    EI_S32              s32Chn;
    EI_S32              s32BindChn;     /* source phy chn */
    EI_S32              s32Position;    /* 0: after region and rot; 1: before region and rot */
    EI_U32              u32Width;
    EI_U32              u32Height;
    EI_U32              u32Align;
    VIDEO_FORMAT_E      enVideoFormat;
    PIXEL_FORMAT_E      enPixelFormat;
    DYNAMIC_RANGE_E     enDynamicRange;
    COMPRESS_MODE_E     enCompressMode;
    FRAME_RATE_CTRL_S   stFrameRate;
    EI_BOOL             bNoFifo;
    EI_U32              u32Depth;
} IPPU_EXT_CHN_ATTR_S;

typedef struct eiIPPU_ROTATION_ATTR_S {
    EI_BOOL         bEnable;
    ROTATION_E      eRotation;
    MIRRORDIRECTION_E eMirror;
    RECT_S          stRect;
    IPPU_CHN        s32Chn;
} IPPU_ROTATION_ATTR_S;

typedef struct eiIPPU_GDC_ATTR_S {
    EI_BOOL         bEnable;
    EI_CHAR         acName[16];
    IPPU_CHN        s32Chn;
    /* usr input attr */
    EI_U32          u32Rot;
    EI_U32          u32Mode;
    EI_FLOAT        fAngle;
    EI_U32          u32DownScale;
    RECT_S          stRect;

    /* inner output attr */
    EI_S64          as64PMFCoef[9];
    EI_U32          u32TblWidth;
    EI_U32          u32TblHeight;
    EI_U32          u32TblSize;
    EI_U32          *pu32TblAddr;
} IPPU_GDC_ATTR_S;

typedef struct eiIPPU_CROP_ATTR_S {
    EI_BOOL         bEnable;
    IPPU_CHN        s32Chn;
    union {
        RECT_S      stRect;
        RECT_S      astStich[6];
    };
    EI_U32          u32CropNum;
    EI_U32          u32OutWidth;
    EI_U32          u32OutHeight;
} IPPU_CROP_ATTR_S;

typedef enum eiIPPU_ERR_CODE_E {
    ERR_IPPU_NOT_INIT                = 0x40,
    ERR_IPPU_MEM_NOT_INIT            = 0x41,
    ERR_IPPU_ATTR_NOT_CFG            = 0x42,
    ERR_IPPU_SNS_UNREGISTER          = 0x43,
    ERR_IPPU_INVALID_ADDR            = 0x44,
    ERR_IPPU_NO_INT                  = 0x46,
} IPPU_ERR_CODE_E;

#define EI_ERR_IPPU_NOT_INIT         EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, ERR_IPPU_NOT_INIT)
#define EI_ERR_IPPU_MEM_NOT_INIT     EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, ERR_IPPU_MEM_NOT_INIT)
#define EI_ERR_IPPU_ATTR_NOT_CFG     EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, ERR_IPPU_ATTR_NOT_CFG)
#define EI_ERR_IPPU_SNS_UNREGISTER   EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, ERR_IPPU_SNS_UNREGISTER)
#define EI_ERR_IPPU_INVALID_ADDR     EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, ERR_IPPU_INVALID_ADDR)
#define EI_ERR_IPPU_NO_INT           EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, ERR_IPPU_NO_INT)

#define EI_ERR_IPPU_NULL_PTR         EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define EI_ERR_IPPU_NOTREADY         EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define EI_ERR_IPPU_NOT_PERM         EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define EI_ERR_IPPU_NOMEM            EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define EI_ERR_IPPU_ILLEGAL_PARAM    EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define EI_ERR_IPPU_BUSY             EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define EI_ERR_IPPU_NOT_SUPPORT      EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define EI_ERR_IPPU_SIZE_NOT_ENOUGH  EI_DEF_ERR(EI_ID_IPPU, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)

#define EI_TRACE_IPPU(level, fmt, args...)\
    do{ \
        EI_TRACE(EI_ID_IPPU, level, fmt, ##args);\
    } while(0)

#define IPPU_CHECK_RETURN(ret)  do {if (ret) {return (ret);} else {}} while(0)

#define IPPU_CHECK_RETURN_MSG(ret, msg) \
    do { \
        if (ret) { \
            EI_TRACE_IPPU(EI_DBG_ERR, "%s!\n", msg);\
            return (ret);\
        } \
    } while(0)

#define IPPU_CHECK_PRT_MSG(ret, msg) \
    do { \
        if (ret) { \
            EI_TRACE_IPPU(EI_DBG_ERR, "%s!\n", msg);\
        } \
    } while(0)

#define IPPU_CHECK_CHN(ch) \
    do {\
        if (((ch) < 0) || ((ch) >= IPPU_CHN_MAX_NUM))\
        {\
            EI_TRACE_ISP(EI_DBG_ERR, "Err isp ch %d!\n", ch);\
            return EI_ERR_ISP_NOT_SUPPORT;\
        }\
    }while(0)

#define IPPU_CHECK_PHY_CHN(ch) \
    do {\
        if (((ch) < 0) || ((ch) >= IPPU_PHY_CHN_MAX_NUM))\
        {\
            EI_TRACE_ISP(EI_DBG_ERR, "Err isp phy ch %d!\n", ch);\
            return EI_ERR_ISP_NOT_SUPPORT;\
        }\
    }while(0)

#define IPPU_CHECK_EXT_CHN(ch) \
    do {\
        if (((ch) < IPPU_PHY_CHN_MAX_NUM) || ((ch) >= IPPU_CHN_MAX_NUM))\
        {\
            EI_TRACE_ISP(EI_DBG_ERR, "Err isp phy ch %d!\n", ch);\
            return EI_ERR_ISP_NOT_SUPPORT;\
        }\
    }while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
