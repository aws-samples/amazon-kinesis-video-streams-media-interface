/*
 *------------------------------------------------------------------------------
 * @File      :    mi_adec.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __MI_ADEC_H__
#define __MI_ADEC_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_adec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_ADEC_CreateChn(ADEC_CHN AdChn, const ADEC_CHN_ATTR_S *pstAttr);
EI_S32 EI_MI_ADEC_DestroyChn(ADEC_CHN AdChn);

EI_S32 EI_MI_ADEC_SendStream(ADEC_CHN AdChn, const AUDIO_STREAM_S *pstStream, EI_BOOL bBlock);
EI_S32 EI_MI_ADEC_ClearChnBuf(ADEC_CHN AdChn);
EI_S32 EI_MI_ADEC_GetFd(ADEC_CHN AdChn);

EI_S32 EI_MI_ADEC_RegisterDecoder(EI_S32 *ps32Handle, const ADEC_DECODER_S *pstDecoder);
EI_S32 EI_MI_ADEC_UnRegisterDecoder(EI_S32 s32Handle);

EI_S32 EI_MI_ADEC_GetFrame(ADEC_CHN AdChn, AUDIO_FRAME_INFO_S *pstFrmInfo, EI_BOOL bBlock);
EI_S32 EI_MI_ADEC_ReleaseFrame(ADEC_CHN AdChn, const AUDIO_FRAME_INFO_S *pstFrmInfo);
EI_S32 EI_MI_ADEC_SendEndOfStream(ADEC_CHN AdChn, EI_BOOL bInstant);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MI_ADEC_H__ */

