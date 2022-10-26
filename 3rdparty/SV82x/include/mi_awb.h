/*
 *------------------------------------------------------------------------------
 * @File      :    mi_awb.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_AWB_H__
#define __MI_AWB_H__

#include "ei_comm_isp.h"
#include "ei_comm_3a.h"
#include "ei_awb_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* The interface of awb lib register to isp. */
EI_S32 EI_MI_AWB_Register(ISP_DEV dev, ISP_3ALIB_S *pstAwbLib);
EI_S32 EI_MI_AWB_UnRegister(ISP_DEV dev, ISP_3ALIB_S *pstAwbLib);

/* The callback function of sensor register to awb lib. */
EI_S32 EI_MI_AWB_SensorRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAwbLib, AWB_SENSOR_REGISTER_S *pstRegister);
EI_S32 EI_MI_AWB_SensorUnRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAwbLib,
    SENSOR_ID SensorId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
