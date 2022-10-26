/*
 *------------------------------------------------------------------------------
 * @File      :    mi_mfake.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_MFAKE_H__
#define __MI_MFAKE_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_mfake.h"
#include "ei_comm_mlink.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_MFAKE_Init(EI_VOID);
EI_S32 EI_MI_MFAKE_Exit(EI_VOID);

EI_S32 EI_MI_MFAKE_SetSrcChnAttr(EI_U32 u32Chn, const MFAKE_SRC_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_MFAKE_GetSrcChnAttr(EI_U32 u32Chn, MFAKE_SRC_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_MFAKE_EnableSrcChn(EI_U32 u32Chn);
EI_S32 EI_MI_MFAKE_DisableSrcChn(EI_U32 u32Chn);
EI_S32 EI_MI_MFAKE_SetSinkChnAttr(EI_U32 u32Chn, const MFAKE_SINK_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_MFAKE_GetSinkChnAttr(EI_U32 u32Chn, MFAKE_SINK_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_MFAKE_EnableSinkChn(EI_U32 u32Chn);
EI_S32 EI_MI_MFAKE_DisableSinkChn(EI_U32 u32Chn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_MFAKE_H__ */

