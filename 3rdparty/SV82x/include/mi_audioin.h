/*
 *------------------------------------------------------------------------------
 * @File      :    mi_audioin.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_AUDIOIN_H__
#define __MI_AUDIOIN_H__

#include "ei_common.h"
#include "ei_comm_mlink.h"
#include "ei_comm_aio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_AUDIOIN_Init(AUDIO_DEV AiDevId);
EI_S32 EI_MI_AUDIOIN_Exit(AUDIO_DEV AiDevId);

EI_S32 EI_MI_AI_SetPubAttr(AUDIO_DEV AiDevId, const AIO_ATTR_S *pstAttr);
EI_S32 EI_MI_AI_GetPubAttr(AUDIO_DEV AiDevId, AIO_ATTR_S *pstAttr);

EI_S32 EI_MI_AI_Enable(AUDIO_DEV AiDevId);
EI_S32 EI_MI_AI_Disable(AUDIO_DEV AiDevId);

EI_S32 EI_MI_AI_EnableChn(AUDIO_DEV AiDevId, AI_CHN AiChn);
EI_S32 EI_MI_AI_DisableChn(AUDIO_DEV AiDevId, AI_CHN AiChn);

EI_S32 EI_MI_AI_GetFrame(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_FRAME_S *pstFrm,
    AEC_FRAME_S *pstAecFrm, EI_S32 s32MilliSec);
EI_S32 EI_MI_AI_ReleaseFrame(AUDIO_DEV AiDevId, AI_CHN AiChn,
    const AUDIO_FRAME_S *pstFrm, const AEC_FRAME_S *pstAecFrm);

EI_S32 EI_MI_AI_SetChnParam(AUDIO_DEV AiDevId, AI_CHN AiChn, const AI_CHN_PARAM_S *pstChnParam);
EI_S32 EI_MI_AI_GetChnParam(AUDIO_DEV AiDevId, AI_CHN AiChn, AI_CHN_PARAM_S *pstChnParam);

EI_S32 EI_MI_AI_GetFd(AUDIO_DEV AiDevId, AI_CHN AiChn);

EI_S32 EI_MI_AI_ClrPubAttr(AUDIO_DEV AiDevId);
EI_S32 EI_MI_AI_SaveFile(AUDIO_DEV AiDevId, AI_CHN AiChn, const AUDIO_SAVE_FILE_INFO_S *pstSaveFileInfo);

EI_S32 EI_MI_AI_QueryFileStatus(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_FILE_STATUS_S *pstFileStatus);

EI_S32 EI_MI_AI_SetVolume(AUDIO_DEV AiDevId, EI_S32 s32Length, EI_S32 as32VolumeDb[]);
EI_S32 EI_MI_AI_GetVolume(AUDIO_DEV AiDevId, EI_S32 s32Length, EI_S32 *as32VolumeDb);
EI_S32 EI_MI_AI_SetMute(AUDIO_DEV AiDevId, EI_S32 s32Length, EI_BOOL abMuteEnable[]);
EI_S32 EI_MI_AI_GetMute(AUDIO_DEV AiDevId, EI_S32 s32Length, EI_BOOL *pabMuteEnable);

EI_S32 EI_MI_AI_EnableReSmp(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_SAMPLE_RATE_E enOutSampleRate);
EI_S32 EI_MI_AI_DisableReSmp(AUDIO_DEV AiDevId, AI_CHN AiChn);

EI_S32 EI_MI_AI_EnableVqe(AUDIO_DEV AiDevId, AI_CHN AiChn);
EI_S32 EI_MI_AI_DisableVqe(AUDIO_DEV AiDevId, AI_CHN AiChn);

EI_S32 EI_MI_AI_SetTrackMode(AUDIO_DEV AiDevId, AUDIO_TRACK_MODE_E enTrackMode);
EI_S32 EI_MI_AI_GetTrackMode(AUDIO_DEV AiDevId, AUDIO_TRACK_MODE_E *penTrackMode);

EI_S32 EI_MI_AI_EnableAecRefFrame(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_DEV AoDevId, AO_CHN AoChn);
EI_S32 EI_MI_AI_DisableAecRefFrame(AUDIO_DEV AiDevId, AI_CHN AiChn);

EI_S32 EI_MI_AI_SetVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, AUDIO_DEV AoDevId, AO_CHN AoChn, const
    AI_VQE_CONFIG_S *pstVqeConfig);
EI_S32 EI_MI_AI_GetVqeAttr(AUDIO_DEV AiDevId, AI_CHN AiChn, AI_VQE_CONFIG_S *pstVqeConfig);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_AUDIOIN_H__ */

