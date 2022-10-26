/*
 *------------------------------------------------------------------------------
 * @File      :    mi_viss.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __MI_VISS_H__
#define __MI_VISS_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_viss.h"
#include "ei_comm_camera.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_VISS_EnableDev(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_DisableDev(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_SetDevAttr(VISS_DEV VissDev, const VISS_DEV_ATTR_S *pstDevAttr);
EI_S32 EI_MI_VISS_GetDevAttr(VISS_DEV VissDev, VISS_DEV_ATTR_S *pstDevAttr);
EI_S32 EI_MI_VISS_SetChnAttr(VISS_DEV VissDev, VISS_CHN VissChn, const VISS_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_VISS_GetChnAttr(VISS_DEV VissDev, VISS_CHN VissChn, VISS_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_VISS_SetExtChnAttr(VISS_DEV VissDev, VISS_CHN VissChn, const VISS_EXT_CHN_ATTR_S *pstExtChnAttr);
EI_S32 EI_MI_VISS_GetExtChnAttr(VISS_DEV VissDev, VISS_CHN VissChn, VISS_EXT_CHN_ATTR_S *pstExtChnAttr);
EI_S32 EI_MI_VISS_SetVcChnAttr(VISS_DEV VissDev, VISS_CHN VissChn, const VISS_VC_CHN_ATTR_S *pstVcChnAttr);
EI_S32 EI_MI_VISS_GetVcChnAttr(VISS_DEV VissDev, VISS_CHN VissChn, VISS_VC_CHN_ATTR_S *pstVcChnAttr);
EI_S32 EI_MI_VISS_SetChnRotAttr(VISS_DEV VissDev, VISS_CHN VissChn, const VISS_ROTATION_ATTR_S *pstChnRotAttr);
EI_S32 EI_MI_VISS_GetChnRotAttr(VISS_DEV VissDev, VISS_CHN VissChn, VISS_ROTATION_ATTR_S *pstChnRotAttr);
EI_S32 EI_MI_VISS_SetChnCropAttr(VISS_DEV VissDev, VISS_CHN VissChn, const VISS_CROP_INFO_S *pstCropAttr);
EI_S32 EI_MI_VISS_GetChnCropAttr(VISS_DEV VissDev, VISS_CHN VissChn, VISS_CROP_INFO_S *pstCropAttr);
EI_S32 EI_MI_VISS_EnableChn(VISS_DEV VissDev, VISS_CHN VissChn);
EI_S32 EI_MI_VISS_DisableChn(VISS_DEV VissDev, VISS_CHN VissChn);
EI_S32 EI_MI_VISS_EnDevClk(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_ResetDev(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_EnableSnsMclk(VISS_DEV VissDev, EI_U32 SnsMclk);
EI_S32 EI_MI_VISS_InitSns(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_ExitSns(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_GetChnFrame(VISS_DEV VissDev, VISS_CHN VissChn, VIDEO_FRAME_INFO_S *pstVideoFrame, EI_S32 s32MilliSec);
EI_S32 EI_MI_VISS_ReleaseChnFrame(VISS_DEV VissDev, VISS_CHN VissChn, const VIDEO_FRAME_INFO_S *pstVideoFrame);
EI_S32 EI_MI_VISS_TriggerSnapFrame(VISS_DEV dev, VISS_CHN ch, EI_U32 u32FrameCnt);
EI_S32 EI_MI_VISS_ExtDevInit(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_ExtDevExit(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_ExtDevSetAttr(VISS_DEV VissDev,const CTL_ATTR_S *pstCtlAttr);
EI_S32 EI_MI_VISS_ExtDevGetAttr(VISS_DEV VissDev, CTL_ATTR_S *pstCtlAttr);
EI_S32 EI_MI_VISS_SetChnGdcAttr(VISS_DEV VissDev, VISS_CHN VissChn, VISS_GDC_ATTR_S *pstChnGdc);

EI_S32 EI_MI_VISS_GetDevType(VISS_DEV VissDev, VISS_DEV_TYPE_E *penDevType);
VISS_DEV_TYPE_E EI_MI_VISS_SensorGetDevType(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_SensorPowerUp(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_SensorPowerDown(VISS_DEV VissDev);
EI_S32 EI_MI_VISS_SensorDetect(VISS_DEV VissDev);
EI_S32 VISS_SensorEnumSnstype(VISS_DEV VissDev, EI_U32 u32Index, SNS_TYPE_E *peSnstype);
EI_S32 EI_MI_VISS_SensorSetFrameRate(VISS_DEV VissDev, EI_U32 u32FrameRate);
EI_S32 EI_MI_VISS_GetSensorStatus(VISS_DEV VissDev, SNS_STS_S *pstSts);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_VISS_H__ */

