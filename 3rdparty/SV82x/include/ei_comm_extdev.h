/*
 *------------------------------------------------------------------------------
 * @File      :    mi_comm_extdev.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_EXTDEV_H__
#define __EI_COMM_EXTDEV_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_viss.h"
#include "ei_comm_camera.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX_COF 3

#define EXT_DEV_GET_CTX(dev, pstCtx)   (pstCtx = &gastExtDev[dev])


typedef enum eiEXT_DEV_TYPE_E {
    A6208 = 1,
    MS41929,
    AP1511B,
    MS41929_MOTOR = 4,
    EXT_DEV_TYPE_MAX,
} EXT_DEV_TYPE_E;

typedef enum eiCTL_TYPE_E {
    IR_CUT = 1,
    MOTOR,
    EXT_DEV_MAX,
} CTL_TYPE_E;

typedef enum eiIR_STATE_E {
    IR_OPEN,
    IR_CLOSE,
    IR_STATE_MAX,
} IR_STATE_E;

typedef struct eiIR_CUT_ATTR_S
{
    IR_STATE_E eState;
} IR_CUT_ATTR_S;

typedef struct eiMOTOR_CODE_S {
    EI_U32 u32FocusCode;
    EI_U32 u32ZoomCode;
} MOTOR_CODE_S;

typedef struct eiMOTOR_ATTR_S {
    EI_U32 u32Dir;
    EI_U32 u32Unit;
} MOTOR_ATTR_S;

typedef union eiCTL_ATTR_U {
    IR_CUT_ATTR_S      stIrCut;
    MOTOR_CODE_S       stMotorCode;
} CTL_ATTR_U;

typedef struct eiCTL_ATTR_S {
    CTL_TYPE_E        enType;
    CTL_ATTR_U        unAttr;
} CTL_ATTR_S;

typedef struct eiEXT_DEV_OBJ_S {
    EI_S32 (*pfnRegisterVissCallback)(VISS_DEV, EXT_DEV_TYPE_E);
    EI_S32 (*pfnUnRegisterVissCallback)(VISS_DEV);
    EI_S32 (*pfnRegisterIspCallback)(VISS_DEV, ISP_DEV, ISP_3ALIB_S *);
    EI_S32 (*pfnUnRegisterIspCallback)(ISP_DEV, ISP_3ALIB_S *);
} EXT_DEV_OBJ_S;

typedef struct eiEXT_DEV_FUNC_S
{
    EI_S32(*pfnExtDevInit)(VISS_DEV);
    EI_VOID(*pfnExtDevExit)(VISS_DEV);
    EI_S32 (*pfnExtDevSetAttr)(VISS_DEV,const CTL_ATTR_S *);
    EI_S32 (*pfnExtDevGetAttr)(VISS_DEV, CTL_ATTR_S *);

} EXT_DEV_FUNC_S;

typedef struct eiSPI_INFO_S {
    EI_U16         u16SpiBusNum;
    EI_U8          u8RegBits;
    EI_U8          u8ValBits;
    EI_S32         fd;
} SPI_INFO_S;

typedef struct eiCAMERA_POWER_ACT_S CAMERA_POWER_ACT_S;

typedef struct eiEXT_DEV_STATE_S {
    EXT_DEV_TYPE_E  enExtDevType;
    EI_CHAR     *pcName;
    SPI_INFO_S      stSpiInfo;
    CAMERA_POWER_ACT_S *pstSetAct[MAX_COF];
    EI_U8               u8SetActs[MAX_COF];
    EI_VOID             *pvPriv;
    IR_STATE_E eIr;
} EXT_DEV_STATE_S;

typedef struct eiEXT_INFO_S
{
    EXT_DEV_STATE_S *pstExtDev;
    EXT_DEV_FUNC_S stExtDevFun;
} EXT_INFO_S;

extern EXT_DEV_OBJ_S stExtDevA6208Obj;
extern EXT_DEV_OBJ_S stExtDevMs41929Obj;
extern EXT_DEV_OBJ_S stExtDevAP1511BObj;

#define EXT_DEV_CHECK_POINTER(ptr)\
    do {\
        if (EI_NULL == ptr)\
        {\
            EI_TRACE_VISS(EI_DBG_ERR, "EXT Dev Null Pointer!\n");\
            return EI_ERR_VISS_NULL_PTR;\
        }\
    }while(0)

#define EXT_DEV_CHECK_POINTER_VOID(ptr)\
    do {\
        if (EI_NULL == ptr)\
        {\
            EI_TRACE_VISS(EI_DBG_ERR, "EXT Dev Null Pointer!\n");\
            return;\
        }\
    }while(0)

#define EXT_DEV_FREE(ptr)\
    do{\
        if (EI_NULL != ptr)\
        {\
            free(ptr);\
            ptr = EI_NULL;\
        }\
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus */

#endif /*__EI_COMM_EXTDEV_H__ */
