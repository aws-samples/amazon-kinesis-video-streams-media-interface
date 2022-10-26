/*
 *------------------------------------------------------------------------------
 * @File      :    mi_ippu.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#ifndef __MI_IPPU_H__
#define __MI_IPPU_H__

#include "ei_comm_vbuf.h"
#include "ei_comm_ippu.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


EI_S32 EI_MI_IPPU_Start(IPPU_DEV dev);
EI_S32 EI_MI_IPPU_Stop(IPPU_DEV dev);
EI_S32 EI_MI_IPPU_Create(IPPU_DEV dev, const IPPU_DEV_ATTR_S *pstDevAttr);
EI_S32 EI_MI_IPPU_Destroy(IPPU_DEV dev);

EI_S32 EI_MI_IPPU_SetChnAttr(IPPU_DEV dev, IPPU_CHN ch, const IPPU_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_IPPU_GetChnAttr(IPPU_DEV dev, IPPU_CHN ch, IPPU_CHN_ATTR_S *pstChnAttr);

/* ExtChn Settings */
EI_S32 EI_MI_IPPU_SetExtChnAttr(IPPU_DEV dev, IPPU_CHN ch, const IPPU_EXT_CHN_ATTR_S *pstExtChnAttr);
EI_S32 EI_MI_IPPU_GetExtChnAttr(IPPU_DEV dev, IPPU_CHN ch, IPPU_EXT_CHN_ATTR_S *pstExtChnAttr);

EI_S32 EI_MI_IPPU_SetChnRotation(IPPU_DEV dev, IPPU_CHN ch, const IPPU_ROTATION_ATTR_S *pstChnRot);
EI_S32 EI_MI_IPPU_GetChnRotation(IPPU_DEV dev, IPPU_CHN ch, IPPU_ROTATION_ATTR_S *pstChnRot);

EI_S32 EI_MI_IPPU_SetChnCrop(IPPU_DEV dev, IPPU_CHN ch, const IPPU_CROP_ATTR_S *pstChnCrop);
EI_S32 EI_MI_IPPU_GetChnCrop(IPPU_DEV dev, IPPU_CHN ch, IPPU_CROP_ATTR_S *pstChnCrop);

EI_S32 EI_MI_IPPU_SetChnGdc(IPPU_DEV dev, IPPU_CHN ch, IPPU_GDC_ATTR_S *pstChnGdc);

EI_S32 EI_MI_IPPU_EnableChn(IPPU_DEV dev, IPPU_CHN ch);
EI_S32 EI_MI_IPPU_DisableChn(IPPU_DEV dev, IPPU_CHN ch);

EI_S32 EI_MI_IPPU_SetDevAttr(IPPU_DEV dev, const IPPU_DEV_ATTR_S *pstDevAttr);
EI_S32 EI_MI_IPPU_GetDevAttr(IPPU_DEV dev, IPPU_DEV_ATTR_S *pstDevAttr);

EI_S32 EI_MI_IPPU_GetChnFrame(IPPU_DEV dev, IPPU_CHN ch, VIDEO_FRAME_INFO_S *pstVideoFrame, EI_S32 s32MilliSec);
EI_S32 EI_MI_IPPU_ReleaseChnFrame(IPPU_DEV dev, IPPU_CHN ch, const VIDEO_FRAME_INFO_S *pstVideoFrame);

EI_S32 EI_MI_IPPU_TriggerSnapFrame(IPPU_DEV dev, IPPU_CHN ch, EI_U32 u32FrameCnt);

EI_S32 EI_MI_IPPU_VissInitLdc(EI_UL *pOutLdc, VISS_GDC_ATTR_S *pstChnGdc);
EI_S32 EI_MI_IPPU_VissReleaseLdc(EI_UL pOutLdc);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_IPPU_H__ */
