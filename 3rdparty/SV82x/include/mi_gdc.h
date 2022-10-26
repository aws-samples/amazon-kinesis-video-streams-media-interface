/*
 *------------------------------------------------------------------------------
 * @File      :    mi_gdc.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_GDC_H__
#define __MI_GDC_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_gdc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_GDC_Init(EI_VOID);
EI_S32 EI_MI_GDC_Exit(EI_VOID);
EI_S32 EI_MI_GDC_CreateJob(GDC_HANDLE *phHandle, const EI_CHAR *pcName);
EI_S32 EI_MI_GDC_SubmitJob(GDC_HANDLE hHandle);
EI_S32 EI_MI_GDC_CancelJob(GDC_HANDLE hHandle);
EI_S32 EI_MI_GDC_AddCorrectionTask(GDC_HANDLE hHandle, const GDC_TASK_INFO_S *pstTask,
    const FISHEYE_PARAM_S *pstFisheyeAttr);
EI_S32 EI_MI_GDC_AddPMFTask(GDC_HANDLE hHandle, const GDC_TASK_INFO_S *pstTask,
    const GDC_PMF_PARAM_S *pstGdcPmfAttrr);
EI_S32 EI_MI_GDC_AddLDCTask(GDC_HANDLE hHandle, const GDC_TASK_INFO_S *pstTask, const GDC_LDC_PARAM_S *pstLDCAttrr);
EI_S32 EI_MI_GDC_UpdateCoef(const EI_CHAR *pcName, const GDC_COEF_S *pstCoef);
EI_S32 EI_MI_GDC_ClearCoef(const EI_CHAR *pcName);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_GDC_H__ */
