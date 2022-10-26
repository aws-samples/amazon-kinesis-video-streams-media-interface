/*
 *------------------------------------------------------------------------------
 * @File      :    ei_ae_comm.h
 * @Date      :    2021-5-13
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_AE_COMM_H__
#define __EI_AE_COMM_H__

#include "ei_type.h"
#include "ei_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define EI_AE_LIB_NAME "ei_ae_lib"

typedef struct eiAE_SENSOR_DEFAULT_S {
    EI_FLOAT f32Fps;
    EI_FLOAT fInitTime;
    EI_FLOAT fInitGain;

    EI_FLOAT fTimeOfLine;

    EI_FLOAT fMaxIntTime;
    EI_FLOAT fMinIntTime;
    EI_FLOAT fMaxGain;
    EI_FLOAT fMinGain;

    EI_BOOL bManualRatio;
    EI_U32 au32Ratio[EXP_RATIO_NUM];
    EI_U32 u32ExpRatioMax;
    EI_U32 u32ExpRatioMin;
} AE_SENSOR_DEFAULT_S;

typedef struct eiAE_FSWDR_ATTR_S {
    WDR_MODE_E enFSWDRMode;
} AE_FSWDR_ATTR_S;

typedef struct eiAE_SENSOR_EXP_FUNC_S {
    EI_S32(*pfnGetAeDefault)(ISP_DEV dev, AE_SENSOR_DEFAULT_S *pstAeSnsDft);

    /* the function of sensor set fps */
    EI_S32(*pfnSetFps)(ISP_DEV dev, EI_FLOAT f32Fps,
        AE_SENSOR_DEFAULT_S *pstAeSnsDft);
    EI_S32(*pfnSlowFps)(ISP_DEV dev, EI_U32 u32FullLines,
        AE_SENSOR_DEFAULT_S *pstAeSnsDft);

    /* while isp notify ae to update sensor regs, ae call these funcs. */
    EI_S32(*pfnSetExpTime)(ISP_DEV dev, EI_FLOAT *afIntTime);
    EI_S32(*pfnSetGain)(ISP_DEV dev, EI_FLOAT fGain);

    EI_S32(*pfnGetExpTime)(ISP_DEV dev, EI_U16 u16ManRatioEnable,
        EI_U32 *au32Ratio, EI_U32 *au32IntTimeMax, EI_U32 *au32IntTimeMin,
        EI_U32 *pu32LFMaxIntTime);

    /* long frame mode set */
    EI_S32(*pfnSetFsWdrAttr)(ISP_DEV dev, AE_FSWDR_ATTR_S *pstAeFSWDRAttr);
} AE_SENSOR_EXP_FUNC_S;

typedef struct eiAE_SENSOR_REGISTER_S {
    AE_SENSOR_EXP_FUNC_S stSnsExp;
    SENSOR_ID SnsId;
} AE_SENSOR_REGISTER_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
