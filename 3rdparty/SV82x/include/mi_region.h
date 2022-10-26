/*
 *------------------------------------------------------------------------------
 * @File      :    mi_region.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_REGION_H__
#define __MI_REGION_H__

#include "ei_comm_region.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

EI_S32 EI_MI_RGN_Create(RGN_HANDLE Handle, const RGN_ATTR_S *pstRegion);
EI_S32 EI_MI_RGN_Destroy(RGN_HANDLE Handle);

EI_S32 EI_MI_RGN_QueryAttr(RGN_HANDLE Handle, RGN_ATTR_S *pstRegion);
EI_S32 EI_MI_RGN_SetBitMap(RGN_HANDLE Handle, const BITMAP_S *pstBitmap);

EI_S32 EI_MI_RGN_AddToChn(RGN_HANDLE Handle, const MDP_CHN_S *pstChn, const RGN_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_RGN_DelFromChn(RGN_HANDLE Handle, const MDP_CHN_S *pstChn);

EI_S32 EI_MI_RGN_SetChnAttr(RGN_HANDLE Handle, const MDP_CHN_S *pstChn, const RGN_CHN_ATTR_S *pstChnAttr);
EI_S32 EI_MI_RGN_GetChnAttr(RGN_HANDLE Handle, const MDP_CHN_S *pstChn, RGN_CHN_ATTR_S *pstChnAttr);

EI_S32 EI_MI_RGN_GetCanvInfo(RGN_HANDLE Handle, RGN_CANV_INFO_S *pstCanvInfo);
EI_S32 EI_MI_RGN_UpdateCanv(RGN_HANDLE Handle);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

