/*
 *------------------------------------------------------------------------------
 * @File      :    mi_mlink.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_MLINK_H__
#define __MI_MLINK_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_mlink.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_MLINK_Init(EI_VOID);
EI_S32 EI_MI_MLINK_Exit(EI_VOID);

EI_S32  EI_MI_MLINK_Link(const MDP_CHN_S *pstSrcChn, const MDP_CHN_S *pstSinkChn);
EI_S32  EI_MI_MLINK_UnLink(const MDP_CHN_S *pstSrcChn, const MDP_CHN_S *pstSinkChn);
EI_S32  EI_MI_MLINK_GetSrcbySink(const MDP_CHN_S *pstSinkChn, MDP_CHN_S *pstSrcChn);
EI_S32  EI_MI_MLINK_GetSinksbySrc(const MDP_CHN_S *pstSrcChn, MDP_LINK_SINK_S *pstSinks);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_MLINK_H__ */

