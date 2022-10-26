/*
 *------------------------------------------------------------------------------
 * @File      :    mi_sam.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_SAM_H__
#define __MI_SAM_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_sam.h"
#include "ei_comm_mlink.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_SAM_Init(void);
EI_S32 EI_MI_SAM_Exit(void);
EI_S32 EI_MI_SAM_SetSrcChnAttr(SAM_DEV Dev, EI_U32 u32Chn, const SAM_SRC_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_SAM_GetSrcChnAttr(SAM_DEV Dev, EI_U32 u32Chn, SAM_SRC_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_SAM_EnableSrcChn(SAM_DEV Dev, EI_U32 u32Chn);
EI_S32 EI_MI_SAM_DisableSrcChn(SAM_DEV Dev, EI_U32 u32Chn);
EI_S32 EI_MI_SAM_SetSinkChnAttr(SAM_DEV Dev, EI_U32 u32Chn, const SAM_SINK_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_SAM_GetSinkChnAttr(SAM_DEV Dev, EI_U32 u32Chn, SAM_SINK_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_SAM_EnableSinkChn(SAM_DEV Dev, EI_U32 u32Chn);
EI_S32 EI_MI_SAM_DisableSinkChn(SAM_DEV Dev, EI_U32 u32Chn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_SAM_H__ */

