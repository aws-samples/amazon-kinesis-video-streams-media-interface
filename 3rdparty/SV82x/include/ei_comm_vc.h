/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_vc.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_VC_H__
#define __EI_COMM_VC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "ei_comm_venc.h"

typedef enum VC_CHNMODE_E
{
	INVALID,
	DECODE,
	ENCODE,
} VC_CHNMODE_E;

typedef struct VC_CHNPARAM_S
{
	EI_U32            u32UseFlag;
	EI_VOID           *pvoidHandle;
	PAYLOAD_TYPE_E    enType;
	VC_CHNMODE_E      enChnMode;
} VC_CHNPARAM_S;

typedef struct eiJVC_RESOURCE_S {
    EI_VOID *pDev;
    EI_VOID *pBase;
    EI_U32 u32ClkRate;
    EI_U32 u32Irq;
} JVC_RESOURCE_S;

EI_S32 EI_MI_VC_Init(EI_VOID);
EI_S32 EI_MI_VC_Exit(EI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_VISS_H__ */

