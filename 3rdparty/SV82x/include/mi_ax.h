/*
 *------------------------------------------------------------------------------
 * @File      :    mi_ax.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __MI_AX_H__
#define __MI_AX_H__

#include "ei_type.h"
#include "ei_comm_ax.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_AXNU_Init(EI_VOID);
EI_S32 EI_MI_AXNU_Exit(EI_S32 s32AxnuFd);
EI_S32 EI_MI_AXNU_GetVer(EI_S32 s32AxnuFd, EI_U32 * u32Ver);
EI_S32 EI_MI_AXNU_SetFreq(EI_S32 s32AxnuFd, EI_U32 u32Freq);
EI_S32 EI_MI_AXNU_SetNormalFreq(EI_S32 s32AxnuFd, EI_U32 u32Freq);
EI_S32 EI_MI_AXNU_GetFreq(EI_S32 s32AxnuFd, EI_U32 *u32Freq);
EI_S32 EI_MI_AXNU_GetNormalFreq(EI_S32 s32AxnuFd, EI_U32 *u32Freq);
EI_S32 EI_MI_AXNU_SetTimeOut(EI_S32 s32AxnuFd, EI_U32 u32TimeOut);
EI_S32 EI_MI_AXNU_GetTimeOut(EI_S32 s32AxnuFd, EI_U32 *u32TimeOut);
EI_S32 EI_MI_AXNU_Run(EI_S32 s32AxnuFd, AXNU_CTL_S *pstAxnuCtrl);
EI_S32 EI_MI_AXNU_Query(EI_S32 s32AxnuFd);

EI_S32 EI_MI_AXVU_Init(EI_VOID);
EI_S32 EI_MI_AXVU_Exit(EI_S32 s32AxvuFd);
EI_S32 EI_MI_AXVU_SetFreq(EI_S32 s32AxvuFd, EI_U32 s32Freq);
EI_S32 EI_MI_AXVU_GetFreq(EI_S32 s32AxvuFd, EI_U32 *s32Freq);
EI_S32 EI_MI_AXVU_SetTimeOut(EI_S32 s32AxvuFd, EI_U32 u32TimeOut);
EI_S32 EI_MI_AXVU_GetTimeOut(EI_S32 s32AxvuFd, EI_U32 *u32TimeOut);
EI_S32 EI_MI_AXVU_MBG_RUN(EI_S32 s32AxvuFd, AXVU_MBG_CFG_S *pstAxvuMbgCfgs);
EI_S32 EI_MI_AXVU_RSZ_RUN(EI_S32 s32AxvuFd, AXVU_RSZ_CFG_S *pstAxvuRszCfgs);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif