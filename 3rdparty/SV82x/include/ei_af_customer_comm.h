/*
 *------------------------------------------------------------------------------
 * @File      :    ei_af_customer_comm.h
 * @Date      :    2021-5-13
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_AF_CUSTOMER_COMM_H__
#define __EI_AF_CUSTOMER_COMM_H__

#include "ei_type.h"
#include "ei_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define EI_AF_CUSTOMER_LIB_NAME "ei_af_customer_lib"

typedef struct eiAF_CUSTOMER_DRIVER_FUNC_S {
    EI_S32(*pfnGetZoomCode)(ISP_DEV dev, EI_U32 *pu32ZoomCode);
    EI_S32(*pfnSetZoomCode)(ISP_DEV dev, EI_U32 u32ZoomCode);
    EI_S32(*pfnGetFocusCode)(ISP_DEV dev, EI_U32 *pu32FocusCode);
    EI_S32(*pfnSetFocusCode)(ISP_DEV dev, EI_U32 u32FocusCode);
    EI_S32(*pfnGetMotorCode)(ISP_DEV dev, EI_U32 *pu32ZoomCode, EI_U32 *pu32FocusCode);
    EI_S32(*pfnSetMotorCode)(ISP_DEV dev, EI_U32 u32ZoomCode, EI_U32 u32FocusCode);
    EI_S32(*pfnMotorReset)(ISP_DEV dev, EI_BOOL bReset);
    EI_BOOL(*pfnMotorBusyReq)(ISP_DEV dev);
} AF_CUSTOMER_DRIVER_FUNC_S;

typedef struct eiAF_CUSTOMER_DRIVER_REGISTER_S {
    AF_CUSTOMER_DRIVER_FUNC_S stAfDrv;
    EI_S32 AfDrvId;
} AF_CUSTOMER_DRIVER_REGISTER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
