/*
 *------------------------------------------------------------------------------
 * @File      :    ei_awb_comm.h
 * @Date      :    2021-5-13
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_AWB_COMM_H__
#define __EI_AWB_COMM_H__

#include "ei_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define EI_AWB_LIB_NAME "ei_awb_lib"

typedef struct eiAWB_SENSOR_DEFAULT_S {
    EI_U16 u16GoldenRgain;         /* Rgain for the golden sample */
    EI_U16 u16GoldenBgain;         /* Bgain for the golden sample */

    EI_U16 u16InitRgain;           /*Init WB gain*/
    EI_U16 u16InitGgain;
    EI_U16 u16InitBgain;
} AWB_SENSOR_DEFAULT_S;

typedef struct eiAWB_SENSOR_EXP_FUNC_S {
    EI_S32(*pfnGetAwbDefault)(ISP_DEV dev, AWB_SENSOR_DEFAULT_S *pstAwbSnsDft);
} AWB_SENSOR_EXP_FUNC_S;

typedef struct eiAWB_SENSOR_REGISTER_S {
    AWB_SENSOR_EXP_FUNC_S stSnsExp;
    SENSOR_ID SnsId;
} AWB_SENSOR_REGISTER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
