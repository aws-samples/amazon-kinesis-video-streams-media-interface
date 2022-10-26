/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_gdc.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_GDC_H__
#define __EI_COMM_GDC_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_debug.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define EI_TRACE_GDC(level, fmt, args...)\
do { \
    EI_TRACE(EI_ID_GDC, level, fmt, ##args);\
} while(0)

#define EI_GDC_TRACE_ERR(fmt, args...) \
do { \
    EI_TRACE_GDC(EI_DBG_ERR, fmt, ##args);\
} while(0)

#define FISHEYE_MAX_AREA_NUM 4
#define GDC_PMF_COEF_NUM 9

typedef EI_S32      GDC_HANDLE;

typedef struct eiGDC_TASK_INFO_S {
    VIDEO_FRAME_INFO_S  stImgIn;
    VIDEO_FRAME_INFO_S  stImgOut;
} GDC_TASK_INFO_S;

typedef enum eiGDC_TASK_TYPE_E {
    GDC_TASK_TYPE_FISHEYE  = 0,
    GDC_TASK_TYPE_LDC      = 1,
    GDC_TASK_TYPE_PMF      = 2,
    GDC_TASK_TYPE_MAX
} GDC_TASK_TYPE_E;

typedef enum eiFISHEYE_MOUNT_MODE_E {
    FISHEYE_DESKTOP_MOUNT  = 0,
    FISHEYE_CEILING_MOUNT  = 1,
    FISHEYE_WALL_MOUNT     = 2,
    FISHEYE_MOUNT_MODE_MAX
} FISHEYE_MOUNT_MODE_E;

typedef enum eiFISHEYE_VIEW_MODE_E {
    FISHEYE_VIEW_360_PANORAMA = 0,
    FISHEYE_VIEW_180_PANORAMA = 1,
    FISHEYE_VIEW_NORMAL       = 2,
    FISHEYE_VIEW_DISABLED     = 3,
    FISHEYE_VIEW_MODE_MAX
} FISHEYE_VIEW_MODE_E;

typedef struct eiFISHEYE_REGION_PARAM_S {
    FISHEYE_VIEW_MODE_E enViewMode;
    RECT_S              stOutRect;
} FISHEYE_AREA_PARAM_S;

typedef struct eiFISHEYE_PARAM_S {
    EI_BOOL                bEnable;
    FISHEYE_MOUNT_MODE_E   enMountMode;

    EI_BOOL                bBgColor;
    EI_U32                 u32BgColor;

    EI_U32                 u32AreaNum; /* must be 1 or 4 */
    FISHEYE_AREA_PARAM_S astFishEyeAreaParam[FISHEYE_MAX_AREA_NUM];
} FISHEYE_PARAM_S;

typedef struct eiGDC_PMF_PARAM_S {
    EI_BOOL                bNoneLut;
    EI_BOOL                bBgColor;
    EI_U32                 u32BgColor;

    EI_S64 as64PMFCoef[GDC_PMF_COEF_NUM];
} GDC_PMF_PARAM_S;

typedef struct eiGDC_TABLE_S {
    EI_BOOL enable;
    EI_U32  u32Width;
    EI_U32  u32Height;
    EI_U32  u32Size;
    EI_U32  *pu32Table;

    EI_U32  u32ZeroSize;
    EI_U32  u32ZeroWidth;
    EI_U32  *pu32ZeroTable;
} GDC_TABLE_S;

typedef struct eiGDC_COEF_S {
    EI_U32 u32DownScale;
    EI_U32 u32InWidth;
    EI_U32 u32InHeight;
    EI_U32 u32OutWidth;
    EI_U32 u32OutHeight;
    EI_U32 u32OutLineStride;

    EI_S64 as64PMFCoef[GDC_PMF_COEF_NUM];

    GDC_TABLE_S stLdcTable;
    GDC_TABLE_S stPmfTable;
    GDC_TABLE_S st180WallTable;
    GDC_TABLE_S st360DesktopTable;
    GDC_TABLE_S st360CeilTable;
    GDC_TABLE_S stNormalWallTable;
    GDC_TABLE_S stNormalCeilTable;
    GDC_TABLE_S stNormalDesktopTable;
    GDC_TABLE_S stNormalSplitTable[FISHEYE_MAX_AREA_NUM];
} GDC_COEF_S;

typedef struct eiGDC_LDC_PARAM_S {
    EI_BOOL bBgColor;
    EI_U32  u32BgColor;

    EI_BOOL bAspect;
    EI_S32  s32DistortionRatio;
} GDC_LDC_PARAM_S;

typedef struct eiGDC_LUT_PARAM_S {
    EI_U32  u32TableWidth;
    EI_U32  u32TableHeight;
    EI_U32  u32ZeroTableWidth;
    EI_U32  u32TableSize;
    EI_U32  u32ZeroTableSize;

    EI_VOID *pvTableBufferVAddr;
    EI_VOID *pvZeroTableBufferVAddr;
    EI_U64  u64TableBufferPAddr;
    EI_U64  u64ZeroTableBufferPAddr;
} GDC_LUT_PARAM_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_GDC_H__ */
