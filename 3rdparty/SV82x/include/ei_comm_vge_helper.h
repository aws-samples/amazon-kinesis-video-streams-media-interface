/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_vge_helper.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_VGE_HELPER_H__
#define __EI_COMM_VGE_HELPER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "ei_comm_vge.h"

typedef EI_S32 VGE_HANDLE;

typedef struct eiVGE_SCALE_HELPER_S
{
    VGE_HANDLE                hHandle;
    VGE_TASK_ATTR_S           stTask;
} VGE_SCALE_HELPER_S;

typedef struct eiVGE_COVER_HELPER_S
{
    VGE_HANDLE                hHandle;
    VGE_TASK_ATTR_S           stTask;
    VGE_ADD_COVER_S           stCover;
} VGE_COVER_HELPER_S;

typedef struct eiVGE_OSD_HELPER_S
{
    VGE_HANDLE                hHandle;
    VGE_TASK_ATTR_S           stTask;
    VGE_ADD_OSD_S             stOsd;
} VGE_OSD_HELPER_S;

typedef struct eiVGE_ROT_HELPER_S
{
    VGE_HANDLE                hHandle;
    VGE_TASK_ATTR_S           stTask;
    ROTATION_E                enRotationAngle;
} VGE_ROT_HELPER_S;

typedef struct eiVGE_ROT_CROP_HELPER_S
{
    VGE_HANDLE                hHandle;
    VGE_TASK_ATTR_S           stTask;
    VGE_ADD_ROT_CROP_S        stRotCrop;
} VGE_ROT_CROP_HELPER_S;

typedef struct eiVGE_MOSAIC_HELPER_S
{
    VGE_HANDLE                hHandle;
    VGE_TASK_ATTR_S           stTask;
    VGE_ADD_MOSAIC_S          stMosaic;
} VGE_MOSAIC_HELPER_S;

typedef struct eiVGE_RECTANGLE_HELPER_S
{
    VGE_HANDLE                hHandle;
    VGE_TASK_ATTR_S           stTask;
    VGE_ADD_RECTANGLE_S       stRectangle;
} VGE_RECTANGLE_HELPER_S;

typedef struct eiVGE_STITCH_HELPER_S
{
    VGE_HANDLE                hHandle;
    EI_U32                    u32ValidStitchNum;
    VGE_ADD_STITCH_S          stStitches[MAX_STITCH_NUM];
    VIDEO_FRAME_INFO_S        stImgOut;
} VGE_STITCH_HELPER_S;

typedef struct eiVGE_LINE_HELPER_S
{
    VGE_HANDLE                hHandle;
    VGE_TASK_ATTR_S           stTask;
    VGE_ADD_LINE_S            stLine;
} VGE_LINE_HELPER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_COMM_VGE_HELPER_H__ */

