/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_region.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_REGION_H__
#define __EI_COMM_REGION_H__

#include "ei_common.h"
#include "ei_comm_video.h"
#include "ei_errno.h"
#include "ei_defines.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define MAX_INVERT_COLOR_NUM 128

typedef EI_U32 RGN_HANDLE;
typedef EI_U32 RGN_HANDLEGROUP;

typedef enum eiRGN_TYPE_E
{
    OVERLAY_RGN = 0,
    COVER_RGN,
    COVEREX_RGN,
    OVERLAYEX_RGN,
    MOSAIC_RGN,
    MOSAICEX_RGN,
    RECTANGLEEX_RGN,
    LINEEX_RGN,
    RGN_MAX
} RGN_TYPE_E;

typedef enum eiINVERT_CORLOR_MODE_E
{
    TWO_CORLOR_INVERT_MODE = 0, /* back and white color */
    THREE_CORLOR_INVERT_MODE, /* back, grey and white color, grey color config by user */
    CORLOR_INVERT_MODE_MAX
} INVERT_CORLOR_MODE_E;

typedef struct eiINVERT_CORLOR_INFO_S
{
    EI_U32 u32FrameCnt; /* read only */
    EI_U32 u32InvertColor; /* read only */
    RECT_S stInvertRect;
}INVERT_CORLOR_INFO_S;

typedef struct eiINVERT_CORLOR_ATTR_S
{
    EI_BOOL bInvertColorEn;
    EI_U32 u32ValidInvertColorNum;
    EI_U32 u32UpdateRate; /* valid range is from 1 to 30 */
    INVERT_CORLOR_MODE_E enInvertMode; /* invert color mode, */
    EI_U32 u32GreyColor; /* only valid when u32InvertMode is THREE_CORLOR_INVERT_MODE */
    INVERT_CORLOR_INFO_S *pstInvertColorInfo; /* kernel driver private */
}INVERT_CORLOR_ATTR_S;

typedef struct eiOVERLAY_ATTR_S
{
    PIXEL_FORMAT_E enPixelFmt;
    EI_U32 u32BgColor;
    SIZE_S stSize;
    EI_U32 u32CanvasNum;
}OVERLAY_ATTR_S;

typedef struct eiOVERLAYEX_COMM_ATTR_S
{
    PIXEL_FORMAT_E enPixelFmt;
    EI_U32 u32BgColor;
    SIZE_S stSize;
    EI_U32 u32CanvasNum;
}OVERLAYEX_ATTR_S;

typedef struct eiOVERLAY_CHN_ATTR_S
{
    POINT_S stPoint;
    EI_U32 u32FgAlpha;
    EI_U32 u32Layer;
    INVERT_CORLOR_ATTR_S stInvertColorAttr;
}OVERLAY_CHN_ATTR_S;

typedef struct eiOVERLAYEX_CHN_ATTR_S
{
    POINT_S stPoint;
    EI_U32 u32FgAlpha;
    EI_U32 u32Layer;
    INVERT_CORLOR_ATTR_S stInvertColorAttr;
}OVERLAYEX_CHN_ATTR_S;

typedef struct eiCOVER_CHN_ATTR_S
{
    RECT_S stRect;
    EI_U32 u32Color;
    EI_U32 u32Layer;
}COVER_CHN_ATTR_S;

typedef struct eiCOVEREX_CHN_ATTR_S
{
    RECT_S stRect;
    EI_U32 u32Color;
    EI_U32 u32Layer;
}COVEREX_CHN_ATTR_S;

typedef enum eiMOSAIC_BLK_SIZE_E
{
    MOSAIC_BLK_SIZE_8 = 0,
    MOSAIC_BLK_SIZE_12,
    MOSAIC_BLK_SIZE_20,
    MOSAIC_BLK_SIZE_40,
    MOSAIC_BLK_SIZE_60,
    MOSAIC_BLK_SIZE_120,
    MOSAIC_BLK_SIZE_BUTT
}MOSAIC_BLK_SIZE_E;

typedef struct eiMOSAIC_CHN_ATTR_S
{
    RECT_S stRect;
    MOSAIC_BLK_SIZE_E enBlkSize;
    EI_U32 u32Layer;
}MOSAIC_CHN_ATTR_S;

typedef struct eiMOSAICEX_CHN_ATTR_S
{
    RECT_S stRect;
    MOSAIC_BLK_SIZE_E enBlkSize;
    EI_U32 u32Layer;
}MOSAICEX_CHN_ATTR_S;

typedef struct eiRECTANGLEEX_CHN_ATTR_S
{
    RECT_S stRect;
    EI_U32 u32Color;
    EI_U32 u32BorderSize;
    EI_U32 u32Layer;
}RECTANGLEEX_CHN_ATTR_S;

typedef struct eiLINEEX_CHN_ATTR_S
{
    POINT_S stPoints[2];
    EI_U32 u32Color;
    EI_U32 u32LineSize;
    EI_U32 u32Layer;
}LINEEX_CHN_ATTR_S;

typedef union eiRGN_ATTR_U
{
    OVERLAY_ATTR_S      stOverlay;
    OVERLAYEX_ATTR_S    stOverlayEx;
} RGN_ATTR_U;

typedef union eiRGN_CHN_ATTR_U
{
    OVERLAY_CHN_ATTR_S      stOverlayChn;
    COVER_CHN_ATTR_S        stCoverChn;
    COVEREX_CHN_ATTR_S      stCoverExChn;
    OVERLAYEX_CHN_ATTR_S    stOverlayExChn;
    MOSAIC_CHN_ATTR_S       stMosaicChn;
    MOSAICEX_CHN_ATTR_S     stMosaicExChn;
    RECTANGLEEX_CHN_ATTR_S  stRectangleExChn;
    LINEEX_CHN_ATTR_S       stLineExChn;
} RGN_CHN_ATTR_U;


typedef struct eiRGN_ATTR_S
{
    RGN_TYPE_E enType;
    RGN_ATTR_U unAttr;
} RGN_ATTR_S;


typedef struct eiRGN_CHN_ATTR_S
{
    EI_BOOL           bShow;
    RGN_TYPE_E        enType;
    RGN_CHN_ATTR_U    unChnAttr;
} RGN_CHN_ATTR_S;

#define RGN_MAX_BMP_UPDATE_NUM 16

#define RGN_BATCHHANDLE_MAX 24

typedef struct eiBITMAP_S
{
    PIXEL_FORMAT_E enPixelFormat;
    EI_U32 u32Width;
    EI_U32 u32Height;
    EI_VOID* ATTRIBUTE pData;
} BITMAP_S;

typedef struct eiRGN_BMP_UPDATE_S
{
    POINT_S             stPoint;
    BITMAP_S            stBmp;
    EI_U32              u32Stride;
} RGN_BMP_UPDATE_S;

typedef struct eiRGN_BMP_UPDATE_CFG_S
{
    EI_U32              u32BmpCnt;
    RGN_BMP_UPDATE_S    astBmpUpdate[RGN_MAX_BMP_UPDATE_NUM];
} RGN_BMP_UPDATE_CFG_S;

typedef struct eiRGN_CANVAS_INFO_S
{
    EI_U64         u64PhyAddr;
    EI_VOID        *pVirtAddr;
    SIZE_S         stSize;
    EI_U32         u32Stride;
    PIXEL_FORMAT_E enPixelFmt;
} RGN_CANV_INFO_S;


#define EI_NOTICE_RGN_BUFFER_CHANGE        EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_NOTICE, EI_SUCCESS)
#define EI_ERR_REGION_INVALID_DEVID        EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID      )
#define EI_ERR_REGION_INVALID_CHNID        EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID      )
#define EI_ERR_REGION_ILLEGAL_PARAM        EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM      )
#define EI_ERR_REGION_EXIST                EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST              )
#define EI_ERR_REGION_UNEXIST              EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST            )
#define EI_ERR_REGION_NULL_PTR             EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR           )
#define EI_ERR_REGION_NOT_CONFIG           EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG         )
#define EI_ERR_REGION_NOT_SUPPORT          EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT        )
#define EI_ERR_REGION_NOT_PERM             EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM           )
#define EI_ERR_REGION_LIST_EMPTY           EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_LIST_EMPTY         )
#define EI_ERR_REGION_NOMEM                EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM              )
#define EI_ERR_REGION_NOBUF                EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF              )
#define EI_ERR_REGION_BUF_EMPTY            EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY          )
#define EI_ERR_REGION_BUF_FULL             EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL           )
#define EI_ERR_REGION_SYS_NOTREADY         EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY       )
#define EI_ERR_REGION_BADADDR              EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR            )
#define EI_ERR_REGION_BUSY                 EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY               )
#define EI_ERR_REGION_SIZE_NOT_ENOUGH      EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH    )
#define EI_ERR_REGION_AGAIN                EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_AGAIN              )
#define EI_ERR_REGION_TIMEOUT              EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_TIMEOUT            )
#define EI_ERR_REGION_NOTSUPP              EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NOTSUPP            )
#define EI_ERR_REGION_NOT_ENABLED          EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_ENABLED        )
#define EI_ERR_REGION_NO_FOUND             EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_NO_FOUND           )
#define EI_ERR_REGION_OTHERS               EI_DEF_ERR(EI_ID_REGION, EN_ERR_LEVEL_ERROR, EN_ERR_OTHERS             )

#define EI_TRACE_REGION(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_REGION, level, fmt, ##args);\
} while(0)

#define REGION_ERRNO_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define REGION_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
        if (cond) { \
                EI_TRACE_REGION(EI_DBG_ERR, "errno[%d]!\n", errno);\
                return (ret);\
        } \
} while(0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif



