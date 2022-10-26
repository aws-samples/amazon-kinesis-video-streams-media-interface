/*
 *------------------------------------------------------------------------------
 * @File      :    mi_audioout.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_AUDIOOUT_H__
#define __MI_AUDIOOUT_H__

#include "ei_common.h"
#include "ei_comm_mlink.h"
#include "ei_comm_aio.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_AUDIOOUT_Init(AUDIO_DEV AoDevId);
EI_S32 EI_MI_AUDIOOUT_Exit(AUDIO_DEV AoDevId);

EI_S32 EI_MI_AO_SetPubAttr(AUDIO_DEV AoDevId, const AIO_ATTR_S *pstAttr);
EI_S32 EI_MI_AO_GetPubAttr(AUDIO_DEV AoDevId, AIO_ATTR_S *pstAttr);
EI_S32 EI_MI_AO_Enable(AUDIO_DEV AoDevId);
EI_S32 EI_MI_AO_Disable(AUDIO_DEV AoDevId);
EI_S32 EI_MI_AO_EnableChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
EI_S32 EI_MI_AO_DisableChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
EI_S32 EI_MI_AO_SendFrame(AUDIO_DEV AoDevId, AO_CHN AoChn,
    const AUDIO_FRAME_S *pstData, EI_S32 s32MilliSec);

EI_S32 EI_MI_AO_ClearChnBuf(AUDIO_DEV AoDevId, AO_CHN AoChn);
EI_S32 EI_MI_AO_QueryChnStat(AUDIO_DEV AoDevId, AO_CHN AoChn, AO_CHN_STATE_S *pstStatus);

EI_S32 EI_MI_AO_PauseChn(AUDIO_DEV AoDevId, AO_CHN AoChn);
EI_S32 EI_MI_AO_ResumeChn(AUDIO_DEV AoDevId, AO_CHN AoChn);

EI_S32 EI_MI_AO_SetVolume(AUDIO_DEV AoDevId, EI_S32 s32VolumeDb);
EI_S32 EI_MI_AO_GetVolume(AUDIO_DEV AoDevId, EI_S32 *ps32VolumeDb);

EI_S32 EI_MI_AO_SetMute(AUDIO_DEV AoDevId, EI_BOOL bEnable, const AUDIO_FADE_S *pstFade);
EI_S32 EI_MI_AO_GetMute(AUDIO_DEV AoDevId, EI_BOOL *pbEnable, AUDIO_FADE_S *pstFade);

EI_S32 EI_MI_AO_GetFd(AUDIO_DEV AoDevId, AO_CHN AoChn);
EI_S32 EI_MI_AO_ClrPubAttr(AUDIO_DEV AoDevId);
EI_S32 EI_MI_AO_EnableReSmp(AUDIO_DEV AoDevId, AO_CHN AoChn, AUDIO_SAMPLE_RATE_E enInSampleRate);
EI_S32 EI_MI_AO_DisableReSmp(AUDIO_DEV AoDevId, AO_CHN AoChn);
EI_S32 EI_MI_AO_SetTrackMode(AUDIO_DEV AoDevId, AUDIO_TRACK_MODE_E enTrackMode);
EI_S32 EI_MI_AO_GetTrackMode(AUDIO_DEV AoDevId, AUDIO_TRACK_MODE_E *penTrackMode);
EI_S32 EI_MI_AO_SetVqeAttr(AUDIO_DEV AoDevId, AO_CHN AoChn, const AO_VQE_CONFIG_S *pstVqeConfig);
EI_S32 EI_MI_AO_GetVqeAttr(AUDIO_DEV AoDevId, AO_CHN AoChn, AO_VQE_CONFIG_S *pstVqeConfig);
EI_S32 EI_MI_AO_EnableVqe(AUDIO_DEV AoDevId, AO_CHN AoChn);
EI_S32 EI_MI_AO_DisableVqe(AUDIO_DEV AoDevId, AO_CHN AoChn);





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_AUDIOOUT_H__ */

