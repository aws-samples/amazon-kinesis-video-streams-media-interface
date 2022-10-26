/*
 *------------------------------------------------------------------------------
 * @File      :    mi_af.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_AF_H__
#define __MI_AF_H__

#include "ei_comm_isp.h"
#include "ei_comm_3a.h"
#include "ei_af_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* The interface of af lib register to isp. */
EI_S32 EI_MI_AF_Register(ISP_DEV dev, ISP_3ALIB_S *pstAfLib);
EI_S32 EI_MI_AF_UnRegister(ISP_DEV dev, ISP_3ALIB_S *pstAfLib);


/* The callback function of sensor register to af lib. */
EI_S32 EI_MI_AF_DriverRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAfLib, AF_DRIVER_REGISTER_S *pstRegister);
EI_S32 EI_MI_AF_DriverUnRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAfLib, SENSOR_ID SensorId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
