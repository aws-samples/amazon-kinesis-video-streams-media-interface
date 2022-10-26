/*
 *------------------------------------------------------------------------------
 * @File      :    mi_aenc.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __MI_AENC_H__
#define __MI_AENC_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_aenc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/* AENC function api. */
EI_S32 EI_MI_AENC_CreateChn(AENC_CHN AeChn, const AENC_CHN_ATTR_S *pstAttr);
EI_S32 EI_MI_AENC_DestroyChn(AENC_CHN AeChn);

EI_S32 EI_MI_AENC_SendFrame(AENC_CHN AeChn, const AUDIO_FRAME_S *pstFrm, const AEC_FRAME_S *pstAecFrm);

EI_S32 EI_MI_AENC_GetStream(AENC_CHN AeChn, AUDIO_STREAM_S *pstStream, EI_S32 s32MilliSec);
EI_S32 EI_MI_AENC_ReleaseStream(AENC_CHN AeChn, const AUDIO_STREAM_S *pstStream);

EI_S32 EI_MI_AENC_GetFd(AENC_CHN AeChn);

EI_S32 EI_MI_AENC_RegisterEncoder(EI_S32 *ps32Handle, const AENC_ENCODER_S *pstEncoder);
EI_S32 EI_MI_AENC_UnRegisterEncoder(EI_S32 s32Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MI_AENC_H__ */

