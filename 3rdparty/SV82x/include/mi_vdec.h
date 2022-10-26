/*
 *------------------------------------------------------------------------------
 * @File      :    mi_vdec.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __MI_DEC_H_
#define __MI_DEC_H_

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_vdec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
EI_S32 EI_MI_VDEC_CreateChn(VC_CHN VdChn, VDEC_CHN_ATTR_S *pstAttr);
EI_S32 EI_MI_VDEC_DestroyChn(VC_CHN VdChn);
EI_S32 EI_MI_VDEC_GetChnAttr(VC_CHN VdChn, VDEC_CHN_ATTR_S *pstAttr);
EI_S32 EI_MI_VDEC_SetChnParam(VC_CHN VdChn, VDEC_CHN_PARAM_S *pstVdecParam);
EI_S32 EI_MI_VDEC_StartRecvStream(VC_CHN VdChn);
EI_S32 EI_MI_VDEC_StopRecvStream(VC_CHN VdChn);
EI_S32 EI_MI_VDEC_QueryStatus(VC_CHN VdChn, VDEC_CHN_STATUS_S *pstStatus);
EI_S32 EI_MI_VDEC_ResetChn(VC_CHN VdChn);
EI_S32 EI_MI_VDEC_SendStream(VC_CHN VdChn, VDEC_STREAM_S *pstStream, EI_S32 s32MilliSec);
EI_S32 EI_MI_VDEC_GetFrame(VC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo, EI_S32 s32MilliSec);
EI_S32 EI_MI_VDEC_ReleaseFrame(VC_CHN VdChn, VIDEO_FRAME_INFO_S *pstFrameInfo);
EI_S32 EI_MI_VDEC_SetDisplayMode(VC_CHN VdChn, VIDEO_DISPLAY_MODE_S *pstDisplayMode);
EI_S32 EI_MI_VDEC_GetDisplayMode(VC_CHN VdChn, VIDEO_DISPLAY_MODE_S *pstDisplayMode);
EI_S32 EI_MI_VDEC_GetUserData(VC_CHN VdChn, VDEC_USERDATA_S *pstUserData, EI_S32 s32MilliSec);
EI_S32 EI_MI_VDEC_ReleaseUserData(VC_CHN VdChn, const VDEC_USERDATA_S *pstUserData);
EI_S32 EI_MI_VDEC_SetUserPic(VC_CHN VdChn, VIDEO_FRAME_INFO_S *pstUserPic);
EI_S32 EI_MI_VDEC_EnableUserPic(VC_CHN VdChn, EI_BOOL bInstant);
EI_S32 EI_MI_VDEC_DisableUserPic(VC_CHN VdChn);
EI_S32 EI_MI_VDEC_GetFd(VC_CHN VdChn, EI_S32 *pfd);
EI_S32 EI_MI_VDEC_CloseFd(VC_CHN VdChn);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
