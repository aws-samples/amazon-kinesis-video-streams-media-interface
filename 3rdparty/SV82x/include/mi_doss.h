/*
 *------------------------------------------------------------------------------
 * @File      :    mi_doss.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_DOSS_H__
#define __MI_DOSS_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_region.h"
#include "ei_comm_doss.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_DOSS_Init(EI_VOID);
EI_S32 EI_MI_DOSS_Exit(EI_VOID);

EI_S32 EI_MI_VO_SET_PANEL_INIT_CODE(cmd_info_t *pstCmd);
EI_S32 EI_MI_VO_SetPubAttr(VO_DEV VoDev, const VO_PUB_ATTR_S *pstPubAttr);
EI_S32 EI_MI_VO_GetPubAttr(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr);

EI_S32 EI_MI_VO_Enable(VO_DEV VoDev);
EI_S32 EI_MI_VO_Disable(VO_DEV VoDev);
EI_S32 EI_MI_VO_CloseFd(EI_VOID);

EI_S32 EI_MI_VO_SetVideoLayerAttr(VO_LAYER VoLayer, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);
EI_S32 EI_MI_VO_GetVideoLayerAttr(VO_LAYER VoLayer, VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);

EI_S32 EI_MI_VO_EnableVideoLayer(VO_LAYER VoLayer);
EI_S32 EI_MI_VO_DisableVideoLayer(VO_LAYER VoLayer);

EI_S32 EI_MI_VO_SetVideoLayerPriority(VO_LAYER VoLayer, EI_U32 u32Priority);
EI_S32 EI_MI_VO_GetVideoLayerPriority(VO_LAYER VoLayer, EI_U32 *pu32Priority);

EI_S32 EI_MI_VO_SetVideoLayerCSC(VO_LAYER VoLayer, const VO_CSC_S *pstVideoCSC);
EI_S32 EI_MI_VO_GetVideoLayerCSC(VO_LAYER VoLayer, VO_CSC_S *pstVideoCSC);

EI_S32 EI_MI_VO_SetVideoLayerPartitionMode(VO_LAYER VoLayer, VO_PART_MODE_E enPartMode);
EI_S32 EI_MI_VO_GetVideoLayerPartitionMode(VO_LAYER VoLayer, VO_PART_MODE_E *penPartMode);

EI_S32 EI_MI_VO_SetVideoLayerCrop(VO_LAYER VoLayer, const CROP_INFO_S *pstCropInfo);
EI_S32 EI_MI_VO_GetVideoLayerCrop(VO_LAYER VoLayer, CROP_INFO_S *pstCropInfo);

EI_S32 EI_MI_VO_SetDisplayBufLen(VO_LAYER VoLayer, EI_U32 u32BufLen);
EI_S32 EI_MI_VO_GetDisplayBufLen(VO_LAYER VoLayer, EI_U32 *pu32BufLen);

EI_S32 EI_MI_VO_SetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn, const VO_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_VO_GetChnAttr(VO_LAYER VoLayer, VO_CHN VoChn, VO_CHN_ATTR_S *pstChnAttr);

EI_S32 EI_MI_VO_EnableChn(VO_LAYER VoLayer, VO_CHN VoChn);
EI_S32 EI_MI_VO_DisableChn(VO_LAYER VoLayer, VO_CHN VoChn);

EI_S32 EI_MI_VO_SetChnDisplayPosition(VO_LAYER VoLayer, VO_CHN VoChn, const POINT_S *pstDispPos);
EI_S32 EI_MI_VO_GetChnDisplayPosition(VO_LAYER VoLayer, VO_CHN VoChn, POINT_S *pstDispPos);

EI_S32 EI_MI_VO_GetChnFrame(VO_LAYER VoLayer, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstFrame, EI_S32 s32MilliSec);
EI_S32 EI_MI_VO_ReleaseChnFrame(VO_LAYER VoLayer, VO_CHN VoChn, const VIDEO_FRAME_INFO_S *pstFrame);

EI_S32 EI_MI_VO_PauseChn(VO_LAYER VoLayer, VO_CHN VoChn);
EI_S32 EI_MI_VO_ResumeChn(VO_LAYER VoLayer, VO_CHN VoChn);
EI_S32 EI_MI_VO_ShowChn(VO_LAYER VoLayer, VO_CHN VoChn);
EI_S32 EI_MI_VO_HideChn(VO_LAYER VoLayer, VO_CHN VoChn);

EI_S32 EI_MI_VO_SetZoomInWindow(VO_LAYER VoLayer, VO_CHN VoChn, const VO_ZOOM_ATTR_S *pstZoomAttr);
EI_S32 EI_MI_VO_GetZoomInWindow(VO_LAYER VoLayer, VO_CHN VoChn, VO_ZOOM_ATTR_S *pstZoomAttr);

EI_S32 EI_MI_VO_GetChnPTS(VO_LAYER VoLayer, VO_CHN VoChn, EI_U64 *pu64ChnPTS);
EI_S32 EI_MI_VO_QueryChnStatus(VO_LAYER VoLayer, VO_CHN VoChn, VO_QUERY_STATUS_S *pstStatus);

EI_S32 EI_MI_VO_SendFrame(VO_LAYER VoLayer, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstVFrame, EI_S32 s32MilliSec);

EI_S32 EI_MI_VO_ClearChnBuf(VO_LAYER VoLayer, VO_CHN VoChn, EI_BOOL bClrAll);

EI_S32 EI_MI_VO_SetChnRecvThreshold(VO_LAYER VoLayer, VO_CHN VoChn, EI_U32 u32Threshold);
EI_S32 EI_MI_VO_GetChnRecvThreshold(VO_LAYER VoLayer, VO_CHN VoChn, EI_U32 *pu32Threshold);

EI_S32 EI_MI_VO_SetChnRotation(VO_LAYER VoLayer, VO_CHN VoChn, ROTATION_E enRotation);
EI_S32 EI_MI_VO_GetChnRotation(VO_LAYER VoLayer, VO_CHN VoChn, ROTATION_E *penRotation);

EI_S32 EI_MI_VO_SetChnFrameRate(VO_LAYER VoLayer, VO_CHN VoChn, EI_S32 s32ChnFrmRate);
EI_S32 EI_MI_VO_GetChnFrameRate(VO_LAYER VoLayer, VO_CHN VoChn, EI_S32 *ps32ChnFrmRate);

EI_S32 EI_MI_VO_SetWBCSource(VO_WBC VoWBC, const VO_WBC_SOURCE_S *pstWBCSource);
EI_S32 EI_MI_VO_GetWBCSource(VO_WBC VoWBC, VO_WBC_SOURCE_S *pstWBCSources);

EI_S32 EI_MI_VO_SetWBCAttr(VO_WBC VoWBC, const VO_WBC_ATTR_S *pstWBCAttr);
EI_S32 EI_MI_VO_GetWBCAttr(VO_WBC VoWBC, VO_WBC_ATTR_S *pstWBCAttr);

EI_S32 EI_MI_VO_EnableWBC(VO_WBC VoWBC);
EI_S32 EI_MI_VO_DisableWBC(VO_WBC VoWBC);

EI_S32 EI_MI_VO_SetWBCMode(VO_WBC VoWBC, VO_WBC_MODE_E enWBCMode);
EI_S32 EI_MI_VO_GetWBCMode(VO_WBC VoWBC, VO_WBC_MODE_E *penWBCMode);

EI_S32 EI_MI_VO_SetWBCDepth(VO_WBC VoWBC, EI_U32 u32Depth);
EI_S32 EI_MI_VO_GetWBCDepth(VO_WBC VoWBC, EI_U32 *pu32Depth);

EI_S32 EI_MI_VO_GetWBCFrame(VO_WBC VoWBC, VIDEO_FRAME_INFO_S *pstVFrame, EI_S32 s32MilliSec);
EI_S32 EI_MI_VO_ReleaseWBCFrame(VO_WBC VoWBC, const VIDEO_FRAME_INFO_S *pstVFrame);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_DOSS_H__ */