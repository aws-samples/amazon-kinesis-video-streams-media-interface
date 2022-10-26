/*
 *------------------------------------------------------------------------------
 * @File      :    mi_mbase.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_MBASE_H__
#define __MI_MBASE_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_mbase.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_MBASE_Init(EI_VOID);
EI_S32 EI_MI_MBASE_Exit(EI_VOID);

EI_S32 EI_MI_MBASE_GetVersion(MDP_VERSION_S *pstVersion);

EI_S32 EI_MI_MBASE_MemAlloc(EI_U64 *pu64PhyAddr, EI_VOID **ppVirAddr,
    const EI_CHAR *strMb, const EI_CHAR *strZone, EI_U32 u32Len);
EI_S32 EI_MI_MBASE_MemAllocCached(EI_U64 *pu64PhyAddr, EI_VOID **ppVirAddr,
    const EI_CHAR *strMb, const EI_CHAR *strZone, EI_U32 u32Len);
EI_S32 EI_MI_MBASE_MemFree(EI_U64 u64PhyAddr, EI_VOID *pVirAddr);
EI_VOID *EI_MI_MBASE_Mmap(EI_U64 u64PhyAddr, EI_U32 u32Size);
EI_VOID *EI_MI_MBASE_MmapCache(EI_U64 u64PhyAddr, EI_U32 u32Size);
EI_S32 EI_MI_MBASE_Munmap(EI_VOID *pVirAddr, EI_U32 u32Size);
EI_S32 EI_MI_MBASE_MflushCache(EI_U64 u64PhyAddr, EI_VOID *pVirAddr, EI_U32 u32Size);
EI_S32 EI_MI_MBASE_MflushCacheAll(EI_VOID);
EI_S32 EI_MI_MBASE_GetVmemInfo(EI_VOID *pVirAddr, MBASE_VIRMEM_INFO_S *pstMemInfo);

EI_VOID *EI_MI_MBASE_IOMmap(EI_U64 u64PhyAddr, EI_U32 u32Size);
EI_S32 EI_MI_MBASE_IOMunmap(EI_VOID *pVirStart, EI_U32 u32Size);

EI_S32 EI_MI_MBASE_GetCurPTS(EI_U64 *pu64CurPTS);
EI_S32 EI_MI_MBASE_InitPTSBase(EI_U64 u64PTSBase);
EI_S32 EI_MI_MBASE_SyncPTS(EI_U64 u64PTSBase);
EI_S32 EI_MI_MBASE_GetModInfo(MOD_ID_E enModId, MBASE_MOD_INFO_S *pstModInfo);
EI_S32 EI_MI_MBASE_GetCpuTemp(EI_VOID);

EI_VOID EI_MI_MBASE_LOG(int level, int mod_id, const char *fmt, ...);

EI_S32 EI_MI_MBASE_SetUsrLogLevel(MOD_ID_E enModId, EI_S32 s32Level);
EI_S32 EI_MI_MBASE_GetUsrLogLevel(MOD_ID_E enModId);
EI_S32 EI_MI_MBASE_SetKerLogLevel(MOD_ID_E enModId, EI_S32 s32Level);
EI_S32 EI_MI_MBASE_GetKerLogLevel(MOD_ID_E enModId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_MBASE_H__ */

