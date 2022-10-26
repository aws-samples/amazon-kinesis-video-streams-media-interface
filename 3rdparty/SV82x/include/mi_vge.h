/*
 *------------------------------------------------------------------------------
 * @File      :    mi_vge.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_VGE_H__
#define __MI_VGE_H__

#include "ei_common.h"
#include "ei_comm_video.h"
#include "ei_comm_vge.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*__cplusplus*/

EI_S32 EI_MI_VGE_BeginJob(VGE_HANDLE *phHandle);

EI_S32 EI_MI_VGE_EndJob(VGE_HANDLE hHandle);

EI_S32 EI_MI_VGE_CancelJob(VGE_HANDLE hHandle);

EI_S32 EI_MI_VGE_AddScaleTask(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask);

EI_S32 EI_MI_VGE_AddCoverTask(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, const VGE_ADD_COVER_S *pstVgeAddCover);

EI_S32 EI_MI_VGE_AddOsdTask(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, const VGE_ADD_OSD_S *pstVgeAddOsd);

EI_S32 EI_MI_VGE_AddCoverTaskArray(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, const VGE_ADD_COVER_S astVgeAddCover[], EI_U32 u32ArraySize);

EI_S32 EI_MI_VGE_AddOsdTaskArray(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, const VGE_ADD_OSD_S astVgeAddOsd[], EI_U32 u32ArraySize);

EI_S32 EI_MI_VGE_AddRotationTask(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, ROTATION_E enRotationAngle);

EI_S32 EI_MI_VGE_AddRotationCropTask(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, const VGE_ADD_ROT_CROP_S *pstRotCrop);

EI_S32 EI_MI_VGE_AddMosaicTask(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, const VGE_ADD_MOSAIC_S *pstVgeAddMosaic);

EI_S32 EI_MI_VGE_AddRectangleTask(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, const VGE_ADD_RECTANGLE_S *pstVgeAddRectangle);

EI_S32 EI_MI_VGE_AddStitchTask(VGE_HANDLE hHandle, EI_U32 u32ValidStitchNum, const VGE_ADD_STITCH_S **pstStitches, const VIDEO_FRAME_INFO_S *pstImgOut);

EI_S32 EI_MI_VGE_AddLineTask(VGE_HANDLE hHandle, const VGE_TASK_ATTR_S *pstTask, const VGE_ADD_LINE_S *pstVgeAddLine);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus*/

#endif /*end of __MI_VGE_H__*/

