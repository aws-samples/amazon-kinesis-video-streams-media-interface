/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_vge.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_VGE_H__
#define __EI_COMM_VGE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_video.h"
#include "ei_comm_region.h"
#include "ei_errno.h"
#include "ei_debug.h"

#define MAX_STITCH_NUM 8

/* failure caused by malloc buffer */
#define EI_ERR_VGE_NOBUF           EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define EI_ERR_VGE_BUF_EMPTY       EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define EI_ERR_VGE_NULL_PTR        EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define EI_ERR_VGE_ILLEGAL_PARAM   EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define EI_ERR_VGE_BUF_FULL        EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define EI_ERR_VGE_SYS_NOTREADY    EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define EI_ERR_VGE_NOT_SUPPORT     EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define EI_ERR_VGE_NOT_PERMITTED   EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

typedef EI_S32 VGE_HANDLE;

typedef struct eiVGE_TASK_ATTR_S
{
    VIDEO_FRAME_INFO_S       stImgIn;
    VIDEO_FRAME_INFO_S       stImgOut;
} VGE_TASK_ATTR_S;

typedef struct eiVGE_ADD_COVER_S
{
    RECT_S                   stDstRect;
    EI_U32                   u32Color;
} VGE_ADD_COVER_S;

typedef struct eiVGE_ADD_OSD_S
{
    RECT_S                    stRect;
    EI_U32                    u32BgColor;
    PIXEL_FORMAT_E            enPixelFmt;
    EI_U64                    u64PhyAddr;
    EI_U32                    u32Stride;
    EI_U32                    u32FgAlpha;
    INVERT_CORLOR_ATTR_S      *pstInvertColorAttr;
} VGE_ADD_OSD_S;

typedef struct eiVGE_ADD_ROT_CROP_S
{
    ROTATION_E               enRotationAngle;
    MIRRORDIRECTION_E        enMirrorDirection;
    RECT_S                   stSrcRect;
    RECT_S                   stDstRect;
} VGE_ADD_ROT_CROP_S;

typedef struct eiVGE_ADD_MOSAIC_S
{
    RECT_S                   stDstRect;
    MOSAIC_BLK_SIZE_E        enBlkSize;
} VGE_ADD_MOSAIC_S;

typedef struct eiVGE_ADD_RECTANGLE_S
{
    RECT_S                   stDstRect;
    EI_U32                   u32Color;
    EI_U32                   u32BorderSize;
} VGE_ADD_RECTANGLE_S;

typedef struct eiVGE_ADD_STITCH_S
{
    PIXEL_FORMAT_E           enPixelFormat;
    EI_U32                   u32Width;
    EI_U32                   u32Height;
    EI_U64                   u64PlanePhyAddr[FRAME_MAX_PLANE];
    EI_U32                   u32PlaneStride[FRAME_MAX_PLANE];
    RECT_S                   stSrcRect;
    RECT_S                   stDstRect;
} VGE_ADD_STITCH_S;

typedef struct eiVGE_ADD_LINE_S
{
    POINT_S                  stPoints[2];
    EI_U32                   u32Color;
    EI_U32                   u32LineSize;
} VGE_ADD_LINE_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_COMM_VGE_H__ */

