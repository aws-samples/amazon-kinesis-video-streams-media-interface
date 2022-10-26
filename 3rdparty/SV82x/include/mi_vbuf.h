/*
 *------------------------------------------------------------------------------
 * @File      :    mi_vbuf.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */



#ifndef __MI_VBUF_H__
#define __MI_VBUF_H__

#include "ei_comm_vbuf.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


VBUF_POOL EI_MI_VBUF_CreatePool(VBUF_POOL_CONFIG_S *pstVbufPoolCfg);
EI_S32 EI_MI_VBUF_DestroyPool(VBUF_POOL Pool);

VBUF_BUFFER EI_MI_VBUF_GetBuffer(VBUF_POOL Pool, EI_U32 u32TimeoutMs);
VBUF_BUFFER EI_MI_VBUF_GetBufferBySize(EI_U32 u32BufSize, VBUF_UID_E enVbufUid,
    EI_U32 u32TimeoutMs);

VBUF_BUFFER EI_MI_VBUF_PhysAddr2Buffer(EI_U64 u64PhyAddr);

EI_S32 EI_MI_VBUF_PutBuffer(VBUF_BUFFER Buffer);
EI_S32 EI_MI_VBUF_RefAdd(VBUF_BUFFER Buffer);
EI_S32 EI_MI_VBUF_RefSub(VBUF_BUFFER Buffer);

VBUF_BUFFER EI_MI_VBUF_ModGetBuffer(VBUF_POOL Pool, MOD_ID_E enModId, EI_U32 u32TimeoutMs);
VBUF_BUFFER EI_MI_VBUF_ModGetBufferBySize(EI_U32 u32BufSize, VBUF_UID_E enVbufUid,
    MOD_ID_E enModId, EI_U32 u32TimeoutMs);
EI_S32 EI_MI_VBUF_ModPutBuffer(VBUF_BUFFER Buffer, MOD_ID_E enModId);
EI_S32 EI_MI_VBUF_ModRefAdd(VBUF_BUFFER Buffer, MOD_ID_E enModId);
EI_S32 EI_MI_VBUF_ModRefSub(VBUF_BUFFER Buffer, MOD_ID_E enModId);

EI_S32 EI_MI_VBUF_SetFrameInfoArray(VBUF_POOL Pool, VIDEO_FRAME_INFO_S *pstFrameInfo, EI_U32 u32Num);
EI_S32 EI_MI_VBUF_SetFrameInfo(VBUF_POOL Pool, VIDEO_FRAME_INFO_S *pstFrameInfo);
EI_S32 EI_MI_VBUF_GetFrameInfo(VBUF_BUFFER Buffer, VIDEO_FRAME_INFO_S *pstFrameInfo);
EI_S32 EI_MI_VBUF_UpdateFrameInfo(VBUF_BUFFER Buffer, VIDEO_FRAME_INFO_S *pstFrameInfo);

EI_S32 EI_MI_VBUF_FrameMmap(VIDEO_FRAME_INFO_S *pstFrameInfo, VBUF_REMAP_MODE_E enRemapMode);
EI_S32 EI_MI_VBUF_FrameMunmap(VIDEO_FRAME_INFO_S *pstFrameInfo, VBUF_REMAP_MODE_E enRemapMode);
EI_S32 EI_MI_VBUF_FrameFlushCache(VIDEO_FRAME_INFO_S *pstFrameInfo);

EI_U32 EI_MI_VBUF_GetPicBufferSize(VIDEO_FRAME_INFO_S *pstFrameInfo);
EI_S32 EI_MI_VBUF_GetUserCnt(VBUF_BUFFER Buffer);
EI_S32 EI_MI_VBUF_GetModRef(VBUF_BUFFER Buffer, MOD_ID_E enModId);

EI_S32 EI_MI_VBUF_InitVbufConfig(VBUF_CONFIG_S *pstVbufCfg);
EI_S32 EI_MI_VBUF_ExitVbufConfig(VBUF_CONFIG_S *pstVbufCfg);

EI_S32 EI_MI_VBUF_Init(EI_VOID);
EI_S32 EI_MI_VBUF_Exit(EI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_VI_H__ */

