/*
 *------------------------------------------------------------------------------
 * @File      :    mi_vgss.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_VGSS_H__
#define __MI_VGSS_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_vgss.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_VGSS_Init(EI_VOID);
EI_S32 EI_MI_VGSS_Exit(EI_VOID);

EI_S32 EI_MI_VGSS_SetAttr(VGSS_DEV VgssDev, const VGSS_DEV_ATTR_S *pstDevAttr);
EI_S32 EI_MI_VGSS_GetAttr(VGSS_DEV VgssDev, VGSS_DEV_ATTR_S *pstDevAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_VGSS_H__ */