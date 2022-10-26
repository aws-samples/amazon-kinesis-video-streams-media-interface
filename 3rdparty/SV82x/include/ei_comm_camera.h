/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_camera.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __EI_COMM_CAMERA_H__
#define __EI_COMM_CAMERA_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_viss.h"
#include "ei_comm_isp.h"
#include "ei_comm_3a.h"
#include "ei_comm_video.h"
#include "ei_comm_extdev.h"

#include "mi_isp.h"
#include "mi_ae.h"
#include "mi_awb.h"
#include "mi_af.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SNS_REG_R8_V8_DESC           ((8UL << 8) | 8UL)
#define SNS_REG_R16_V8_DESC          ((16UL << 8) | 8UL)
#define SNS_REG_R8_V16_DESC          ((8UL << 8) | 16UL)
#define SNS_REG_R16_V16_DESC         ((16UL << 8) | 16UL)
#define SNS_REG_DELAY                (0xFFFFFFFF)

#define MAX_COF 3

#define SNS_GET_CTX(dev, pstCtx)   (pstCtx = &g_sastSns[dev])

#define SNS_GET_ISP_TO_VISS(isp, viss)   (viss = gaVissDev[isp])
#define SNS_SET_ISP_TO_VISS(isp, viss)   (gaVissDev[isp] = viss)

/* 0~1:sensor configuration
 * 2~4:specific sensor
 * 5:  interface type
 *     1:dvp
 *     2:mcsi
 *     3:dvp mcsi
 */
typedef enum eiSNS_TYPE_E {
    /* dvp sensor */
    AR0230_DVP_1920_1080_30FPS_RAW12 = 100000,

    OV2710_DVP_1280_720_30FPS_RAW10 = 100100,
    OV2710_DVP_1920_1080_15FPS_RAW10,

    OV5640_DVP_640_480_30FPS_YUV = 100200,
    OV5640_DVP_1280_720_30FPS_YUV,

    TP9930_DVP_1920_1080_25FPS_1CH_YUV = 100300,
    TP9930_DVP_1920_1080_25FPS_4CH_YUV,
    TP9930_DVP_1920_1080_30FPS_4CH_YUV,

    CR286_DVP_1280_480_50FPS_YUV = 100400,
    CR286_DVP_640_512_50FPS_YUV,

    TP9950_DVP_1920_1080_30FPS_1CH_YUV = 100500,
    TP9950_DVP_1920_1080_25FPS_1CH_YUV,
    TP9950_DVP_1280_720_25FPS_1CH_YUV,

    TP9950_MIPI_1920_1080_30FPS_1CH_YUV = 100600,
    TP9950_MIPI_1920_1080_25FPS_1CH_YUV,
    TP9950_MIPI_1920_1080_27_5FPS_1CH_YUV,
    TP9950_MIPI_1280_720_30FPS_1CH_YUV,
    TP9950_MIPI_1280_720_25FPS_1CH_YUV,

    TP2850_MIPI_1920_1080_30FPS_1CH_YUV = 100700,
    TP2850_MIPI_1920_1080_25FPS_1CH_YUV,
    TP2850_MIPI_1920_1080_50FPS_1CH_YUV,
    TP2850_MIPI_1920_1080_60FPS_1CH_YUV,
    TP2850_MIPI_1280_720_30FPS_1CH_YUV,
    TP2850_MIPI_1280_720_25FPS_1CH_YUV,
    TP2850_MIPI_1920_1080_27_5FPS_1CH_YUV,

    /* mcsi sensor */
    GC2145_MIPI_800_600_15FPS_YUYV8 = 200000,
    GC2145_MIPI_1600_1200_15FPS_YUYV8,

    OV5640_MIPI_640_480_30FPS_YUV = 200100,
    OV5640_MIPI_640_480_60FPS_YUV,
    OV5640_MIPI_640_480_10FPS_YUV,
    OV5640_MIPI_1280_720_30FPS_YUV,
    OV5640_MIPI_1280_720_60FPS_YUV,
    OV5640_MIPI_1920_1080_15FPS_YUV,
    OV5640_MIPI_1920_1080_30FPS_YUV,
    OV5640_MIPI_640_480_30FPS_YUV_1LANE,

    IMX307_MIPI_1280_720_30FPS_RAW12 = 200200,
    IMX307_MIPI_1280_720_2TO1_15FPS_RAW12,
    IMX307_MIPI_1280_720_3TO1_7P5FPS_RAW10,
    IMX307_MIPI_1920_1080_25FPS_RAW12,
    IMX307_MIPI_1920_1080_30FPS_RAW12,
    IMX307_MIPI_1920_1080_2TO1_25FPS_RAW12_2LANE,
    IMX307_MIPI_1920_1080_2TO1_30FPS_RAW12_2LANE,
    IMX307_MIPI_1920_1080_2TO1_30FPS_RAW12_4LANE,
    IMX307_MIPI_1920_1080_3TO1_15FPS_RAW12,
    IMX307_MIPI_1920_1080_60FPS_RAW12,
    IMX307_MIPI_1280_720_30FPS_RAW12_2LANE,

    IMX327_MIPI_1920_1080_30FPS_RAW12 = 200300,

    SC2335_MIPI_1920_1080_30FPS_RAW10 = 200400,

    OV8858_MIPI_3264_2448_30FPS_RAW10 = 200500,

    SC4210_MIPI_2560_1440_30FPS_RAW12 = 200600,
    SC4210_MIPI_2560_1440_2TO1_30FPS_RAW10,
    SC4210_MIPI_2560_1440_25FPS_RAW12,
    SC4210_MIPI_2560_1440_2TO1_25FPS_RAW10,

    IMX415_MIPI_3864_2192_30FPS_RAW12 = 200700,
    IMX415_MIPI_3864_2192_2TO1_30FPS_RAW10,
    IMX415_MIPI_3864_2192_15FPS_RAW12,
    IMX415_MIPI_3864_2192_2TO1_15FPS_RAW10,
    IMX415_MIPI_3864_2192_20FPS_RAW12,
    IMX415_MIPI_3864_2192_2TO1_20FPS_RAW10,
    IMX415_MIPI_3864_2192_25FPS_RAW12,
    IMX415_MIPI_3864_2192_2TO1_25FPS_RAW10,

    TP2815_MIPI_1920_1080_25FPS_4CH_YUV = 200800,
    TP2815_MIPI_1920_1080_30FPS_4CH_YUV,

    SC200AI_MIPI_1920_1080_30FPS_RAW10 = 200900,
    SC200AI_MIPI_1920_1080_25FPS_RAW10,
    SC200AI_MIPI_1920_1080_30FPS_2TO1_RAW10,
    SC200AI_MIPI_1920_1080_25FPS_2TO1_RAW10,

    GC4663_MIPI_2560_1440_30FPS_RAW10 = 201000,
    GC4663_MIPI_2560_1440_25FPS_RAW10,
    GC4663_MIPI_2560_1440_2TO1_30FPS_RAW10,
    GC4663_MIPI_2560_1440_2TO1_25FPS_RAW10,

    IMX335_MIPI_2560_1440_30FPS_RAW12 = 201100,
    IMX335_MIPI_2560_1440_2TO1_30FPS_RAW10,

    SC4238_MIPI_2688_1520_25FPS_RAW12 = 201200,
    SC4238_MIPI_2688_1520_30FPS_RAW12,
    SC4238_MIPI_2688_1520_2TO1_25FPS_RAW10,
    SC4238_MIPI_2688_1520_2TO1_30FPS_RAW10,

    OV2735_MIPI_1920_1080_30FPS_RAW10 = 201300,
    OV2735_MIPI_1920_1080_25FPS_RAW10,

    OV9732_MIPI_1280_720_25FPS_RAW10 = 201400,
    OV9732_MIPI_1280_720_30FPS_RAW10,

    OS05A20_MIPI_2688_1944_30FPS_RAW10 = 201500,
    OS05A20_MIPI_2688_1944_2TO1_25FPS_RAW10,
    OS05A20_MIPI_2688_1944_2TO1_30FPS_RAW10,

    GC4653_MIPI_2560_1440_30FPS_RAW10 = 201600,
    GC4653_MIPI_2560_1440_15FPS_RAW10,
    GC4653_MIPI_2560_1440_25FPS_RAW10,

    /*other sensor*/
    CLB_1280_720_90FPS_YUV = 300000,
    CLB_1280_720_90FPS_RAW10,

    SAMPLE_SNS_TYPE_MAX,
} SNS_TYPE_E;

typedef struct eiSNS_REG_S {
    EI_U32 u32Reg;
    EI_U32 u32Val;
} SNS_REG_S;

typedef struct eiSNS_STS_S {
    EI_BOOL bChnSts[VISS_DEV_MAX_CHN_NUM];
} SNS_STS_S;

typedef struct eiREG_CFG_S {
    const SNS_REG_S  *pstRegs;
    EI_U32              u32RegsCnt;
} REG_CFG_S;

typedef struct eiSNS_TYPE_ATTR_S {
    SNS_TYPE_E          enSnsType;
    PIXEL_FORMAT_E      enPixelFormat;      /* RW;Pixel format */
    PIXEL_FORMAT_E      enRealPFormat;      /* compatible encoding does not support Y8 config */
    SIZE_S              stSize;                         /* RW;Input size */
    VISS_WORK_MODE_E    enWorkMode;
    EI_S32              u32Mclock;
    EI_U32              u32FrameRate;
    REG_CFG_S           astRegCfgs[MAX_COF];
    EI_U32              u32ModuleClk;
    VISS_WDR_MODE_E     enWdrMode;

    VISS_DEV_CFG_S          stDevCfg;
    union {
        VISS_YUV_SEQ_E      enYuvSeq;
        VISS_RGB_SEQ_E      enRgbSeq;
    };

    EI_U32                  u32IspClk;
    EI_U32                  u32IppuClk;
    WDR_MODE_E          enIspWdrMode;

    EI_U32              u32Pclk;
    EI_U32              u32Left;
    EI_U32              u32Top;
    EI_U32              u32Vts;
    EI_U32              u32Hts;
    EI_U32              u32MaxShortExp;
} SNS_TYPE_ATTR_S;

typedef struct eiCAMERA_OBJ_S {
    EI_S32 (*pfnRegisterVissCallback)(VISS_DEV, SNS_TYPE_E);
    EI_S32 (*pfnUnRegisterVissCallback)(VISS_DEV);
    EI_S32 (*pfnRegisterIspCallback)(VISS_DEV, ISP_DEV, ISP_3ALIB_S *, ISP_3ALIB_S *);
    EI_S32 (*pfnUnRegisterIspCallback)(ISP_DEV, ISP_3ALIB_S *, ISP_3ALIB_S *);
    EI_S32 (*pfnGetVissDevAttrBySns)(SNS_TYPE_E, VISS_DEV_ATTR_S *);
    EI_S32 (*pfnGetVissChnAttrBySns)(SNS_TYPE_E, VISS_CHN_ATTR_S *);
    EI_S32 (*pfnGetIspAttrBySns)(EI_VOID);
} CAMERA_OBJ_S;

typedef struct eiSENSOR_EXP_VISS_FUNC_S
{
    EI_S32 (*pfnSensorPowerUp)(VISS_DEV);
    EI_S32 (*pfnSensorPowerDown)(VISS_DEV);
    EI_S32 (*pfnSensorDetect)(VISS_DEV);
    EI_S32 (*pfnSensorInit)(VISS_DEV);
    EI_VOID (*pfnSensorExit)(VISS_DEV);
    EI_S32 (*pfnSensorStream)(VISS_DEV, EI_BOOL);
    EI_S32 (*pfnSensorSetImageMode)(VISS_DEV, SNS_TYPE_E);
    EI_S32 (*pfnSensorEnumSnstype)(VISS_DEV, EI_U32, SNS_TYPE_E *);
    EI_S32 (*pfnSensorSetFrameRate)(VISS_DEV, EI_U32);
    EI_S32 (*pfnSensorGetStatus)(VISS_DEV, SNS_STS_S*);
} SENSOR_EXP_FUNC_S;

typedef struct eiI2C_INFO_S I2C_INFO_S;
typedef struct eiSNS_STATE_S SNS_STATE_S;

typedef struct eiSNS_INFO_S
{
    SENSOR_ID            eSensorId;
    VISS_DEV_TYPE_E      enDevType;
    SNS_STATE_S          *pstSns;
    SENSOR_EXP_FUNC_S    stSnsExp;
} SNS_INFO_S;

typedef struct eiSENSOR_S {
    SNS_INFO_S     stSnsAttrInfo;
    EXT_INFO_S      stExtInfo;
} SENSOR_S;

typedef struct eiI2C_INFO_S {
    EI_U8          u8RegBits;
    EI_U8          u8ValBits;
    EI_U16         u16I2cBusNum;
    EI_U16         u16I2cDevAddr;
    EI_U16         u16I2cExtAddr;
    EI_S32         fd;
#if defined(__EOS__)
    struct 	   rt_i2c_bus_device *i2c_bus;
#else

#endif
    EI_S32         (*pfnWriteReg)(SNS_STATE_S *, EI_U32, EI_U32);
    EI_S32         (*pfnReadReg)(SNS_STATE_S *, EI_U32, EI_VOID *);
    EI_S32         (*pfnwriteRegList)(SNS_STATE_S *, const SNS_REG_S *, EI_U32);
} I2C_INFO_S;

typedef struct eiCAMERA_POWER_ACT_S {
    EI_U8  u8Act;
    EI_U8  u8Ud;
} CAMERA_POWER_ACT_S;

typedef struct eiSNS_STATE_S {
    EI_BOOL     bSyncInit;
    SNS_TYPE_E  enSnsType;
    EI_CHAR            *pcName;
    EI_BOOL     bPower;
    SNS_TYPE_ATTR_S *pstSnsAttr;

    I2C_INFO_S      stI2cInfo;
    CAMERA_POWER_ACT_S *pstPowerUpAct;
    CAMERA_POWER_ACT_S *pstPowerDownAct;
    EI_U8               u8PwrUpActs;
    EI_U8               u8PwrDnActs;
    VISS_DEV            Dev;
} SNS_STATE_S;

typedef enum eiSNS_PWR_ACT_E {
    SNS_RESET,
    SNS_PWDN,
    SNS_IR_RESET,
    SNS_IR_PWDN,
    MS_RESET,
    MS_VD_FZ,
    CAM_EN,
    VCC_EN0,
    VCC_EN1,
    VCC_EN2,
    SNS_MCLK,
    SNS_DELAY,
    SNS_PWR_ACT_MAX,
} SNS_PWR_ACT_E;

typedef enum eiSNS_PIN_STATUS_E {
    SNS_DOWN,
    SNS_UP,
    SNS_PIN_STATUS_MAX,
} SNS_PIN_STATUS_E;

extern CAMERA_OBJ_S stCameraGc2145Obj;
extern CAMERA_OBJ_S stCameraAr0230Obj;
extern CAMERA_OBJ_S stCameraOv2710Obj;
extern CAMERA_OBJ_S stCameraOv5640mipiObj;
extern CAMERA_OBJ_S stCameraOv5640dvpObj;
extern CAMERA_OBJ_S stCameraImx307mipiObj;
extern CAMERA_OBJ_S stCameraImx335mipiObj;
extern CAMERA_OBJ_S stCameraImx327mipiObj;
extern CAMERA_OBJ_S stCameraSc2335mipiObj;
extern CAMERA_OBJ_S stCameraClbObj;
extern CAMERA_OBJ_S stCameraOv8858mipiObj;
extern CAMERA_OBJ_S stCameraSc4210mipiObj;
extern CAMERA_OBJ_S stCameraImx415mipiObj;
extern CAMERA_OBJ_S stCameraTp2815mipiObj;
extern CAMERA_OBJ_S stCameraTp9930dvpObj;
extern CAMERA_OBJ_S stCameraSc200aimipiObj;
extern CAMERA_OBJ_S stCameraGc4663mipiObj;
extern CAMERA_OBJ_S stCameraSc4238mipiObj;
extern CAMERA_OBJ_S stCameraOv2735mipiObj;
extern CAMERA_OBJ_S stCameraCr286Obj;
extern CAMERA_OBJ_S stCameraTp9950dvpObj;
extern CAMERA_OBJ_S stCameraTp9950mipiObj;
extern CAMERA_OBJ_S stCameraTp2850mipiObj;
extern CAMERA_OBJ_S stCameraOv9732mipiObj;
extern CAMERA_OBJ_S stCameraOs05a20mipiObj;
extern CAMERA_OBJ_S stCameraGc4653mipiObj;

#define CAMERA_CHECK_POINTER(ptr)\
    do {\
        if (EI_NULL == ptr)\
        {\
            EI_TRACE_ISP(EI_DBG_ERR, "Null Pointer!\n");\
            return EI_ERR_ISP_NULL_PTR;\
        }\
    }while(0)

#define CAMERA_CHECK_POINTER_VOID(ptr)\
    do {\
        if (EI_NULL == ptr)\
        {\
            EI_TRACE_ISP(EI_DBG_ERR, "Null Pointer!\n");\
            return;\
        }\
    }while(0)

#define CAMERA_FREE(ptr)\
    do{\
        if (EI_NULL != ptr)\
        {\
            free(ptr);\
            ptr = EI_NULL;\
        }\
    } while (0)

#define CAMERA_CHECK_SUCCESS(val)\
    do {\
        if (EI_SUCCESS != val)\
        {\
            EI_TRACE_VISS(EI_DBG_ERR, "Set error!\n");\
            return val;\
        }\
    }while(0)

#define CAMERA_CHECK_POWERON(ptr)\
    do {\
        if (ptr->bPower == EI_FALSE)\
        {\
            EI_TRACE_VISS(EI_DBG_ERR, "%s sensor not power on!\n", ptr->pcName);\
            return EI_FAILURE;\
        }\
    } while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus */

#endif /*__EI_COMM_CAMERA_H__ */
