/*
 *------------------------------------------------------------------------------
 * @File      :    mi_ae.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_AE_H__
#define __MI_AE_H__

#include "ei_comm_isp.h"
#include "ei_comm_3a.h"
#include "ei_ae_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* The interface of ae lib register to isp. */
EI_S32 EI_MI_AE_Register(ISP_DEV dev, ISP_3ALIB_S *pstAeLib);
EI_S32 EI_MI_AE_UnRegister(ISP_DEV dev, ISP_3ALIB_S *pstAeLib);


/* The callback function of sensor register to ae lib. */
EI_S32 EI_MI_AE_SensorRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAeLib, AE_SENSOR_REGISTER_S *pstRegister);
EI_S32 EI_MI_AE_SensorUnRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAeLib, SENSOR_ID SensorId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
