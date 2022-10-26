/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_viss.c
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Source file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "ei_posix.h"

#include "sample_comm.h"
#include "ei_comm_extdev.h"

static SNS_TYPE_E g_aenVissSnsType[VISS_MAX_DEV_NUM] = {0, 1};

EI_S32 SAMPLE_COMM_VISS_GetDevAttrBySns(SNS_TYPE_E enSnsType,
    VISS_DEV_ATTR_S *pstDevAttr)
{
    const CAMERA_OBJ_S* pstSnsObj;
    EI_S32    s32Ret = EI_FAILURE;

    pstSnsObj = SAMPLE_COMM_VISS_GetSnsObj(enSnsType);

    pstDevAttr->enWorkMode = 1;

    if (pstSnsObj->pfnGetVissDevAttrBySns != EI_NULL)
    {
        s32Ret = pstSnsObj->pfnGetVissDevAttrBySns(enSnsType, pstDevAttr);
        if (s32Ret != EI_SUCCESS)
        {
            PRT_VISS_INFO("SAMPLE_COMM_VISS_GetDevAttrBySns failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        PRT_VISS_INFO("SAMPLE_COMM_VISS_GetDevAttrBySns failed with EI_NULL!\n");
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_VISS_GetChnAttrBySns(SNS_TYPE_E enSnsType,
    VISS_CHN_ATTR_S *pstChnAttr)
{
    const CAMERA_OBJ_S* pstSnsObj;
    EI_S32    s32Ret = EI_FAILURE;

    pstSnsObj = SAMPLE_COMM_VISS_GetSnsObj(enSnsType);

    if (pstSnsObj->pfnGetVissChnAttrBySns != EI_NULL)
    {
        s32Ret = pstSnsObj->pfnGetVissChnAttrBySns(enSnsType, pstChnAttr);
        if (s32Ret != EI_SUCCESS)
        {
            PRT_VISS_INFO("SAMPLE_COMM_VISS_GetChnAttrBySns failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        PRT_VISS_INFO("SAMPLE_COMM_VISS_GetChnAttrBySns failed with EI_NULL!\n");
    }

    return s32Ret;
}

CAMERA_OBJ_S* SAMPLE_COMM_VISS_GetSnsObj(SNS_TYPE_E enSnsType)
{
    switch (enSnsType)
    {
        case GC2145_MIPI_800_600_15FPS_YUYV8:
        case GC2145_MIPI_1600_1200_15FPS_YUYV8:
            return &stCameraGc2145Obj;
        case AR0230_DVP_1920_1080_30FPS_RAW12:
            return &stCameraAr0230Obj;
        case OV2710_DVP_1280_720_30FPS_RAW10:
        case OV2710_DVP_1920_1080_15FPS_RAW10:
            return &stCameraOv2710Obj;
        case OV5640_MIPI_640_480_30FPS_YUV:
        case OV5640_MIPI_640_480_60FPS_YUV:
        case OV5640_MIPI_640_480_10FPS_YUV:
        case OV5640_MIPI_1280_720_30FPS_YUV:
        case OV5640_MIPI_1280_720_60FPS_YUV:
        case OV5640_MIPI_1920_1080_15FPS_YUV:
        case OV5640_MIPI_1920_1080_30FPS_YUV:
        case OV5640_MIPI_640_480_30FPS_YUV_1LANE:
            return &stCameraOv5640mipiObj;
        case OV5640_DVP_1280_720_30FPS_YUV:
        case OV5640_DVP_640_480_30FPS_YUV:
            return &stCameraOv5640dvpObj;
        case IMX307_MIPI_1280_720_30FPS_RAW12:
        case IMX307_MIPI_1280_720_2TO1_15FPS_RAW12:
        case IMX307_MIPI_1280_720_3TO1_7P5FPS_RAW10:
        case IMX307_MIPI_1920_1080_25FPS_RAW12:
        case IMX307_MIPI_1920_1080_30FPS_RAW12:
        case IMX307_MIPI_1920_1080_2TO1_25FPS_RAW12_2LANE:
        case IMX307_MIPI_1920_1080_2TO1_30FPS_RAW12_2LANE:
        case IMX307_MIPI_1920_1080_2TO1_30FPS_RAW12_4LANE:
        case IMX307_MIPI_1920_1080_3TO1_15FPS_RAW12:
        case IMX307_MIPI_1920_1080_60FPS_RAW12:
        case IMX307_MIPI_1280_720_30FPS_RAW12_2LANE:
            return &stCameraImx307mipiObj;
        case IMX327_MIPI_1920_1080_30FPS_RAW12:
            return &stCameraImx327mipiObj;
        case SC2335_MIPI_1920_1080_30FPS_RAW10:
            return &stCameraSc2335mipiObj;
        case CLB_1280_720_90FPS_YUV:
        case CLB_1280_720_90FPS_RAW10:
            return &stCameraClbObj;
        case OV8858_MIPI_3264_2448_30FPS_RAW10:
            return &stCameraOv8858mipiObj;
        case SC4210_MIPI_2560_1440_30FPS_RAW12:
        case SC4210_MIPI_2560_1440_2TO1_30FPS_RAW10:
        case SC4210_MIPI_2560_1440_25FPS_RAW12:
        case SC4210_MIPI_2560_1440_2TO1_25FPS_RAW10:
            return &stCameraSc4210mipiObj;
        case IMX415_MIPI_3864_2192_30FPS_RAW12:
        case IMX415_MIPI_3864_2192_2TO1_30FPS_RAW10:
        case IMX415_MIPI_3864_2192_15FPS_RAW12:
        case IMX415_MIPI_3864_2192_2TO1_15FPS_RAW10:
        case IMX415_MIPI_3864_2192_20FPS_RAW12:
        case IMX415_MIPI_3864_2192_2TO1_20FPS_RAW10:
            return &stCameraImx415mipiObj;
        case TP2815_MIPI_1920_1080_25FPS_4CH_YUV:
        case TP2815_MIPI_1920_1080_30FPS_4CH_YUV:
            return &stCameraTp2815mipiObj;
        case TP9930_DVP_1920_1080_25FPS_1CH_YUV:
        case TP9930_DVP_1920_1080_25FPS_4CH_YUV:
        case TP9930_DVP_1920_1080_30FPS_4CH_YUV:
            return &stCameraTp9930dvpObj;
        case SC200AI_MIPI_1920_1080_30FPS_RAW10:
        case SC200AI_MIPI_1920_1080_25FPS_RAW10:
        case SC200AI_MIPI_1920_1080_30FPS_2TO1_RAW10:
        case SC200AI_MIPI_1920_1080_25FPS_2TO1_RAW10:
            return &stCameraSc200aimipiObj;
        case GC4663_MIPI_2560_1440_30FPS_RAW10:
        case GC4663_MIPI_2560_1440_25FPS_RAW10:
        case GC4663_MIPI_2560_1440_2TO1_30FPS_RAW10:
        case GC4663_MIPI_2560_1440_2TO1_25FPS_RAW10:
            return &stCameraGc4663mipiObj;
        case IMX335_MIPI_2560_1440_30FPS_RAW12:
        case IMX335_MIPI_2560_1440_2TO1_30FPS_RAW10:
            return &stCameraImx335mipiObj;
        case SC4238_MIPI_2688_1520_25FPS_RAW12:
        case SC4238_MIPI_2688_1520_30FPS_RAW12:
        case SC4238_MIPI_2688_1520_2TO1_25FPS_RAW10:
        case SC4238_MIPI_2688_1520_2TO1_30FPS_RAW10:
            return &stCameraSc4238mipiObj;
        case OV2735_MIPI_1920_1080_30FPS_RAW10:
        case OV2735_MIPI_1920_1080_25FPS_RAW10:
            return &stCameraOv2735mipiObj;
        case CR286_DVP_1280_480_50FPS_YUV:
        case CR286_DVP_640_512_50FPS_YUV:
            return &stCameraCr286Obj;
        case TP9950_DVP_1920_1080_30FPS_1CH_YUV:
        case TP9950_DVP_1920_1080_25FPS_1CH_YUV:
        case TP9950_DVP_1280_720_25FPS_1CH_YUV:
            return &stCameraTp9950dvpObj;
        case TP9950_MIPI_1920_1080_30FPS_1CH_YUV:
        case TP9950_MIPI_1920_1080_25FPS_1CH_YUV:
        case TP9950_MIPI_1920_1080_27_5FPS_1CH_YUV:
        case TP9950_MIPI_1280_720_30FPS_1CH_YUV:
        case TP9950_MIPI_1280_720_25FPS_1CH_YUV:
            return &stCameraTp9950mipiObj;
        case TP2850_MIPI_1920_1080_30FPS_1CH_YUV:
        case TP2850_MIPI_1920_1080_25FPS_1CH_YUV:
        case TP2850_MIPI_1920_1080_50FPS_1CH_YUV:
        case TP2850_MIPI_1920_1080_60FPS_1CH_YUV:
        case TP2850_MIPI_1280_720_30FPS_1CH_YUV:
        case TP2850_MIPI_1280_720_25FPS_1CH_YUV:
        case TP2850_MIPI_1920_1080_27_5FPS_1CH_YUV:
            return &stCameraTp2850mipiObj;
        case OV9732_MIPI_1280_720_25FPS_RAW10:
        case OV9732_MIPI_1280_720_30FPS_RAW10:
            return &stCameraOv9732mipiObj;
        case OS05A20_MIPI_2688_1944_30FPS_RAW10:
        case OS05A20_MIPI_2688_1944_2TO1_25FPS_RAW10:
        case OS05A20_MIPI_2688_1944_2TO1_30FPS_RAW10:
            return &stCameraOs05a20mipiObj;
        case GC4653_MIPI_2560_1440_30FPS_RAW10:
        case GC4653_MIPI_2560_1440_15FPS_RAW10:
        case GC4653_MIPI_2560_1440_25FPS_RAW10:
            return &stCameraGc4653mipiObj;
        default:
            return EI_NULL;
    }
}

EXT_DEV_OBJ_S* SAMPLE_COMM_VISS_GetExtDevObj(EXT_DEV_TYPE_E enType)
{
    switch (enType)
    {
        case A6208:
            return &stExtDevA6208Obj;
        case MS41929:
            return &stExtDevMs41929Obj;
        default:
            return EI_NULL;
    }
}
EI_S32 SAMPLE_COMM_VISS_ExtDevRegiterCallback(VISS_DEV VissDev, EXT_DEV_TYPE_E enType)
{
    EI_S32 s32Ret = EI_SUCCESS;
    EXT_DEV_OBJ_S *pstExtDev;

    pstExtDev = SAMPLE_COMM_VISS_GetExtDevObj(enType);

    if (pstExtDev->pfnRegisterVissCallback != EI_NULL)
    {
        s32Ret = pstExtDev->pfnRegisterVissCallback(VissDev, enType);
        if (s32Ret != EI_SUCCESS)
        {
            PRT_VISS_INFO("ExtDevRegiterCallback failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_ExtDevUnRegiterCallback(VISS_DEV VissDev, EXT_DEV_TYPE_E enType)
{
    EI_S32 s32Ret = EI_SUCCESS;
    EXT_DEV_OBJ_S *pstExtDev;

    pstExtDev = SAMPLE_COMM_VISS_GetExtDevObj(enType);

    if (pstExtDev->pfnUnRegisterVissCallback != EI_NULL)
    {
        s32Ret = pstExtDev->pfnUnRegisterVissCallback(VissDev);
        if (s32Ret != EI_SUCCESS)
        {
            PRT_VISS_INFO("ExtDevUnRegiterCallback failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_SensorRegiterCallback(VISS_DEV VisspDev, SNS_TYPE_E enSnsType)
{
    const CAMERA_OBJ_S* pstSnsObj;
    EI_S32    s32Ret = -1;

    pstSnsObj = SAMPLE_COMM_VISS_GetSnsObj(enSnsType);

    if (EI_NULL == pstSnsObj)
    {
        PRT_VISS_INFO("sensor %d not exist!\n", enSnsType);
        return EI_FAILURE;
    }

    if (pstSnsObj->pfnRegisterVissCallback != EI_NULL)
    {
        s32Ret = pstSnsObj->pfnRegisterVissCallback(VisspDev, enSnsType);

        if (s32Ret != EI_SUCCESS)
        {
            PRT_VISS_INFO("sensor_register_callback failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        PRT_VISS_INFO("sensor_register_callback failed with EI_NULL!\n");
    }

    g_aenVissSnsType[VisspDev] = enSnsType;

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_VISS_SensorUnRegiterCallback(VISS_DEV VisspDev)
{

    SNS_TYPE_E enSnsType;
    const CAMERA_OBJ_S* pstSnsObj;
    EI_S32    s32Ret = -1;

    enSnsType = g_aenVissSnsType[VisspDev];
    if (SAMPLE_SNS_TYPE_MAX <= enSnsType)
    {
        PRT_VISS_INFO("invalid sensor id: %d\n", enSnsType);
        return EI_FAILURE;
    }

    pstSnsObj = SAMPLE_COMM_VISS_GetSnsObj(enSnsType);
    if (EI_NULL == pstSnsObj)
    {
        return EI_FAILURE;
    }

    if (pstSnsObj->pfnUnRegisterVissCallback != EI_NULL)
    {
        s32Ret = pstSnsObj->pfnUnRegisterVissCallback(VisspDev);
        if (s32Ret != EI_SUCCESS)
        {
            PRT_VISS_INFO("sensor_unregister_callback failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }
    else {
        PRT_VISS_INFO("sensor_unregister_callback failed with EI_NULL!\n");
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_StartDev(SAMPLE_VISS_INFO_S *pstVissInfo)
{
    EI_S32              s32Ret;
    VISS_DEV            VissDev = pstVissInfo->stDevInfo.VissDev;
    SNS_TYPE_E          enSnsType = pstVissInfo->stSnsInfo.enSnsType;
    VISS_DEV_ATTR_S     stVissDevAttr;

    memset(&stVissDevAttr, 0, sizeof(VISS_DEV_ATTR_S));

    SAMPLE_COMM_VISS_GetDevAttrBySns(enSnsType, &stVissDevAttr);

    stVissDevAttr.enOutPath = pstVissInfo->stDevInfo.enOutPath;

    memcpy(stVissDevAttr.aBindPhyChn, pstVissInfo->stDevInfo.aBindPhyChn,
        sizeof(stVissDevAttr.aBindPhyChn));

    s32Ret = SAMPLE_COMM_VISS_SensorRegiterCallback(VissDev, enSnsType);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_INFO("SAMPLE_COMM_VISS_SensorRegiterCallback failed with %#x!\n", s32Ret);
        goto ERR4;
    }

    if (pstVissInfo->stExtDevInfo.enType) {
        s32Ret = SAMPLE_COMM_VISS_ExtDevRegiterCallback(VissDev, pstVissInfo->stExtDevInfo.enType);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_INFO("SAMPLE_COMM_VISS_ExtDevRegiterCallback failed with %#x!\n", s32Ret);
            goto ERR3;
        }

        s32Ret = EI_MI_VISS_ExtDevInit(VissDev);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_INFO("EI_MI_VISS_ExtDevInit failed with %#x!\n", s32Ret);
            goto ERR2;
        }
    }

    s32Ret = EI_MI_VISS_EnDevClk(VissDev);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_INFO("EI_MI_VISS_EnDevClk failed with %#x!\n", s32Ret);
            goto ERR2;
    }

    s32Ret = EI_MI_VISS_ResetDev(VissDev);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_ERR("EI_MI_VISS_ResetDev failed with %#x!\n", s32Ret);
            goto ERR2;
    }

    s32Ret = EI_MI_VISS_SetDevAttr(VissDev, &stVissDevAttr);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_ERR("EI_MI_VISS_SetDevAttr failed with %#x!\n", s32Ret);
            goto ERR2;
    }

    s32Ret = EI_MI_VISS_EnableSnsMclk(VissDev, stVissDevAttr.mClock);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_ERR("EI_MI_VISS_EnableSnsMclk failed with %#x!\n", s32Ret);
            goto ERR2;
    }

#if 0
    s32Ret = EI_MI_VISS_InitSns(VissDev);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_INFO("EI_MI_VISS_InitSns failed with %#x!\n", s32Ret);
        return EI_FAILURE;
    }
#endif

    s32Ret = EI_MI_VISS_EnableDev(VissDev);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_ERR("EI_MI_VISS_EnableDev failed with %#x!\n", s32Ret);
        goto ERR1;
    }

    s32Ret = EI_MI_VISS_InitSns(VissDev);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_ERR("EI_MI_VISS_InitSns failed with %#x!\n", s32Ret);
        goto ERR0;
    }

    return EI_SUCCESS;
ERR0:
    EI_MI_VISS_ExitSns(VissDev);

ERR1:
    EI_MI_VISS_DisableDev(VissDev);

ERR2:
    if (pstVissInfo->stExtDevInfo.enType) {
        s32Ret = EI_MI_VISS_ExtDevExit(VissDev);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_ERR("EI_MI_VISS_ExtDevInit failed with %#x!\n", s32Ret);
        }
    }

ERR3:
   if (pstVissInfo->stExtDevInfo.enType) {
        s32Ret = SAMPLE_COMM_VISS_ExtDevUnRegiterCallback(VissDev, pstVissInfo->stExtDevInfo.enType);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_ERR("SAMPLE_COMM_VISS_ExtDevRegiterCallback failed with %#x!\n", s32Ret);
        }
    }

ERR4:
    SAMPLE_COMM_VISS_SensorUnRegiterCallback(VissDev);

    return EI_FAILURE;
}

EI_S32 SAMPLE_COMM_VISS_StopDev(SAMPLE_VISS_INFO_S *pstVissInfo)
{
    EI_S32 s32Ret;
    VISS_DEV VissDev = pstVissInfo->stDevInfo.VissDev;

    s32Ret  = EI_MI_VISS_DisableDev(pstVissInfo->stDevInfo.VissDev);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_ERR("EI_MI_VISS_DisableDev failed with %#x!\n", s32Ret);
        return EI_FAILURE;
    }

    s32Ret = EI_MI_VISS_ExitSns(VissDev);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_ERR("EI_MI_VISS_ExitSns %#x!\n", s32Ret);
        return EI_FAILURE;
    }

    if (pstVissInfo->stExtDevInfo.enType) {
        s32Ret = EI_MI_VISS_ExtDevExit(VissDev);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_INFO("EI_MI_VISS_ExtDevExit failed with %#x!\n", s32Ret);
            return EI_FAILURE;
        }

        s32Ret = SAMPLE_COMM_VISS_ExtDevUnRegiterCallback(VissDev, pstVissInfo->stExtDevInfo.enType);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_ERR("SAMPLE_COMM_VISS_ExtDevUnRegiterCallback failed with %#x!\n", s32Ret);
            return EI_FAILURE;
        }
    }

    s32Ret = SAMPLE_COMM_VISS_SensorUnRegiterCallback(VissDev);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_ERR("SAMPLE_COMM_VISS_SensorRegiterCallback failed with %#x!\n", s32Ret);
        return EI_FAILURE;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_StartVissChn(SAMPLE_VISS_INFO_S *pstVissInfo)
{
    EI_S32              s32Ret = EI_SUCCESS;
    VISS_CHN            VissChn;
    VISS_DEV            VissDev = pstVissInfo->stDevInfo.VissDev;
    VISS_CHN_ATTR_S       stChnAttr = {0};
    VISS_EXT_CHN_ATTR_S       *pstExtChnAttr = &pstVissInfo->stChnInfo.stExtChnAttr;
    VISS_VC_CHN_ATTR_S       *pstVcChnAttr = &pstVissInfo->stChnInfo.stVcChnAttr;
    EI_U32              i;

    SAMPLE_COMM_VISS_GetChnAttrBySns(pstVissInfo->stSnsInfo.enSnsType, &stChnAttr);

    for (i = 0; i < pstVissInfo->stChnInfo.enWorkMode; i++) {
        if (pstVissInfo->stChnInfo.aVissChn[i] >= 0 &&
            pstVissInfo->stChnInfo.aVissChn[i] < VISS_MAX_CHN_NUM) {
            VissChn = pstVissInfo->stChnInfo.aVissChn[i];

            if (VissChn >= VISS_MAX_PHY_CHN_NUM + VISS_MAX_EXT_CHN_NUM) {
                pstVcChnAttr->enPixFormat = stChnAttr.enPixelFormat;
                pstVcChnAttr->stFrameRate.s32SrcFrameRate = stChnAttr.stFrameRate.s32SrcFrameRate;
                s32Ret = EI_MI_VISS_SetVcChnAttr(VissDev, VissChn, pstVcChnAttr);
                if (s32Ret != EI_SUCCESS) {
                    PRT_VISS_ERR("EI_MI_VISS_SetVcChnAttr failed with %#x!\n", s32Ret);
                    return EI_FAILURE;
                }
                s32Ret = EI_MI_VISS_EnableChn(VissDev, VissChn);
                if (s32Ret != EI_SUCCESS) {
                    PRT_VISS_INFO("EI_MI_VISS_EnableChn failed with %#x!\n", s32Ret);
                    return EI_FAILURE;
                }
                continue;
            }

            if (VissChn >= VISS_MAX_PHY_CHN_NUM) {
                pstExtChnAttr->enPixFormat = stChnAttr.enPixelFormat;
                pstExtChnAttr->stFrameRate.s32SrcFrameRate = stChnAttr.stFrameRate.s32SrcFrameRate;
                s32Ret = EI_MI_VISS_SetExtChnAttr(VissDev, VissChn, pstExtChnAttr);
                if (s32Ret != EI_SUCCESS) {
                    PRT_VISS_ERR("EI_MI_VISS_SetExtChnAttr failed with %#x!\n", s32Ret);
                    return EI_FAILURE;
                }

                s32Ret = EI_MI_VISS_EnableChn(VissDev, VissChn);
                if (s32Ret != EI_SUCCESS) {
                    PRT_VISS_INFO("EI_MI_VISS_EnableChn failed with %#x!\n", s32Ret);
                    return EI_FAILURE;
                }
                continue;
            }

            stChnAttr.enMirrorFlip = pstVissInfo->stChnInfo.stChnAttr.enMirrorFlip;
            stChnAttr.stFrameRate.s32DstFrameRate = pstVissInfo->stChnInfo.stChnAttr.stFrameRate.s32DstFrameRate;
            stChnAttr.u32Align = pstVissInfo->stChnInfo.stChnAttr.u32Align;
            stChnAttr.u32Depth = pstVissInfo->stChnInfo.stChnAttr.u32Depth;
            s32Ret = EI_MI_VISS_SetChnAttr(VissDev, VissChn, &stChnAttr);
            if (s32Ret != EI_SUCCESS) {
                PRT_VISS_ERR("EI_MI_VISS_SetChnAttr failed with %#x!\n", s32Ret);
                return EI_FAILURE;
            }

            s32Ret = EI_MI_VISS_EnableChn(VissDev, VissChn);
            if (s32Ret != EI_SUCCESS) {
                PRT_VISS_INFO("EI_MI_VISS_EnableChn failed with %#x!\n", s32Ret);
                return EI_FAILURE;
            }
        }
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_VISS_StopVissChn(SAMPLE_VISS_INFO_S *pstVissInfo)
{
    EI_BOOL             bNeedChn;
    EI_S32              s32Ret = EI_SUCCESS;
    VISS_CHN            VissChn;
    VISS_DEV            VissDev = pstVissInfo->stDevInfo.VissDev;
    VISS_CHN_ATTR_S     stChnAttr;
    EI_U32              i;

    SAMPLE_COMM_VISS_GetChnAttrBySns(pstVissInfo->stSnsInfo.enSnsType, &stChnAttr);

    bNeedChn = EI_TRUE;

    if (bNeedChn) {
        for (i = 0; i < pstVissInfo->stChnInfo.enWorkMode; i++) {
            if (pstVissInfo->stChnInfo.aVissChn[i] >= 0 &&
                pstVissInfo->stChnInfo.aVissChn[i] < VISS_MAX_CHN_NUM) {

                VissChn = pstVissInfo->stChnInfo.aVissChn[i];

                s32Ret = EI_MI_VISS_DisableChn(VissDev, VissChn);
                if (s32Ret != EI_SUCCESS) {
                    PRT_VISS_INFO("EI_MI_VISS_DisableChn failed with %#x!\n", s32Ret);
                    return EI_FAILURE;
                }
            }
        }

    }
    return s32Ret;
}

static EI_S32 SAMPLE_COMM_VISS_CreateSingleViss(SAMPLE_VISS_INFO_S *pstVissInfo)
{
    EI_S32 s32Ret = EI_SUCCESS;

    s32Ret = SAMPLE_COMM_VISS_StartDev(pstVissInfo);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_INFO("SAMPLE_COMM_VISS_StartDev failed !\n");
        return EI_FAILURE;
    }

    if (pstVissInfo->stDevInfo.enOutPath != VISS_OUT_PATH_PIXEL) {
        s32Ret = SAMPLE_COMM_VISS_StartVissChn(pstVissInfo);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_INFO("SAMPLE_COMM_VISS_StartViChn failed !\n");
            goto EXIT1;
        }
    }

    return EI_SUCCESS;

EXIT1:
    SAMPLE_COMM_VISS_StopDev(pstVissInfo);

    return s32Ret;
}

static EI_S32 SAMPLE_COMM_VISS_DestroySingleViss(SAMPLE_VISS_INFO_S *pstVissInfo)
{
    EI_S32 s32Ret = EI_SUCCESS;

    if (pstVissInfo->stDevInfo.enOutPath != VISS_OUT_PATH_PIXEL) {
        s32Ret = SAMPLE_COMM_VISS_StopVissChn(pstVissInfo);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_INFO("SAMPLE_COMM_VISS_StopVissChn failed !\n");
        }
    }

    SAMPLE_COMM_VISS_StopDev(pstVissInfo);

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_CreateViss(SAMPLE_VISS_CONFIG_S *pstVissConfig)
{
    EI_S32              i;
    EI_S32              s32Ret = EI_SUCCESS;

    if (!pstVissConfig) {
        PRT_VISS_INFO("null ptr\n");
        return EI_FAILURE;
    }

    if (pstVissConfig->s32WorkingVissNum < 1) {
        PRT_VISS_ERR("s32WorkingVissNum %d failed !\n", pstVissConfig->s32WorkingVissNum);
        return EI_FAILURE;
    }

    for (i = 0; i < pstVissConfig->s32WorkingVissNum; i++) {
        s32Ret = SAMPLE_COMM_VISS_CreateSingleViss(&pstVissConfig->astVissInfo[i]);
        if (s32Ret != EI_SUCCESS) {
            PRT_VISS_INFO("SAMPLE_COMM_VISS_CreateSingleVi failed !\n");
            goto EXIT;
        }
    }

    return EI_SUCCESS;
EXIT:

    for (i = 0; i < pstVissConfig->s32WorkingVissNum; i++) {
        SAMPLE_COMM_VISS_DestroySingleViss(&pstVissConfig->astVissInfo[i]);
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_VISS_DestroyViss(SAMPLE_VISS_CONFIG_S *pstVissConfig)
{
    EI_S32            i;

    if (!pstVissConfig) {
        PRT_VISS_INFO("null ptr\n");
        return EI_FAILURE;
    }

    for (i = 0; i < pstVissConfig->s32WorkingVissNum; i++) {
        SAMPLE_COMM_VISS_DestroySingleViss(&pstVissConfig->astVissInfo[i]);
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_StartViss(SAMPLE_VISS_CONFIG_S *pstVissConfig)
{
    EI_S32 s32Ret = EI_SUCCESS;

    if (!pstVissConfig) {
        PRT_VISS_INFO("null ptr\n");
        return EI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_VISS_CreateViss(pstVissConfig);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_INFO("SAMPLE_COMM_VISS_CreateViss failed!\n");
        return EI_FAILURE;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_VISS_StopViss(SAMPLE_VISS_CONFIG_S *pstVissConfig)
{
    EI_S32 s32Ret = EI_SUCCESS;


    s32Ret = SAMPLE_COMM_VISS_DestroyViss(pstVissConfig);
    if (s32Ret != EI_SUCCESS) {
        PRT_VISS_INFO("SAMPLE_COMM_VISS_DestroyViss failed !\n");
        return EI_FAILURE;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_VISS_GetPicTypeBySensor(SNS_TYPE_E enSnsType, VISS_PIC_TYPE_E *pstType)
{
    EI_S32 s32Ret = EI_SUCCESS;
    const CAMERA_OBJ_S* pstSnsObj;
    VISS_CHN_ATTR_S stChnAttr;

    pstSnsObj = SAMPLE_COMM_VISS_GetSnsObj(enSnsType);
    if (pstType == EI_NULL) {
        return EI_FAILURE;
    }

    if (pstSnsObj->pfnGetVissChnAttrBySns != EI_NULL)
    {
        s32Ret = pstSnsObj->pfnGetVissChnAttrBySns(enSnsType, &stChnAttr);
        if (s32Ret != EI_SUCCESS)
        {
            PRT_VISS_INFO("VissChnAttrBySns failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        PRT_VISS_INFO("VissChnAttrBySns failed with EI_NULL!\n");
    }

    pstType->stSize = stChnAttr.stSize;
    pstType->enPixelFormat = stChnAttr.enPixelFormat;

    return s32Ret;
}

EI_VOID SAMPLE_COMM_VISS_GetSensorInfo(SAMPLE_VISS_CONFIG_S *pstVissConfig)
{
    EI_S32 i;

    for (i = 0; i < pstVissConfig->s32WorkingVissNum; i++) {
        if (pstVissConfig->astVissInfo[i].stDevInfo.VissDev == 1) {
            pstVissConfig->astVissInfo[i].stSnsInfo.enSnsType = IMX307_MIPI_1920_1080_25FPS_RAW12;
            //pstVissConfig->astVissInfo[0].stSnsInfo.enSnsType = OV5640_MIPI_640_480_30FPS_YUV;
        } else {
            pstVissConfig->astVissInfo[i].stSnsInfo.enSnsType = OV2710_DVP_1920_1080_15FPS_RAW10;
            //pstVissConfig->astVissInfo[0].stSnsInfo.enSnsType = AR0230_DVP_1920_1080_30FPS_RAW12;
        }
    }
}

EI_S32 SAMPLE_COMM_VISS_DetectSnstype(VISS_DEV VissDev, SNS_TYPE_E enSnsType)
{
    VISS_DEV_TYPE_E enDevType;
    VISS_DEV_TYPE_E enSnsDevType;
    const CAMERA_OBJ_S* pstSnsObj;
    EI_S32    s32Ret = EI_SUCCESS;
    VISS_DEV_ATTR_S     stVissDevAttr;
    EI_BOOL bDetect = EI_SUCCESS;
    EI_U32 i = 0;
    SNS_TYPE_E eSnstype;

    s32Ret = EI_MI_VISS_GetDevType(VissDev, &enDevType);
    if (s32Ret != EI_SUCCESS)
    {
        EI_TRACE_VISS(EI_DBG_ERR, "set error!\n");
        return s32Ret;
    }

    pstSnsObj = SAMPLE_COMM_VISS_GetSnsObj(enSnsType);
    if (pstSnsObj == EI_NULL)
    {
        EI_TRACE_VISS(EI_DBG_ERR, "null pointer!\n");
        return EI_ERR_VISS_NULL_PTR;
    }

    if ((pstSnsObj->pfnRegisterVissCallback == EI_NULL) ||
        (pstSnsObj->pfnGetVissDevAttrBySns == EI_NULL) ||
        (pstSnsObj->pfnUnRegisterVissCallback == EI_NULL))
    {
        EI_TRACE_VISS(EI_DBG_ERR, "null pointer!\n");
        return EI_ERR_VISS_NULL_PTR;
    }

    s32Ret = pstSnsObj->pfnRegisterVissCallback(VissDev, enSnsType);
    if (s32Ret != EI_SUCCESS)
    {
        EI_TRACE_VISS(EI_DBG_ERR, "set error!\n");
        return s32Ret;
    }

    enSnsDevType = EI_MI_VISS_SensorGetDevType(VissDev);

    if ((enSnsDevType != enDevType) && (enSnsDevType != DVP_AND_MCSI)) {
        PRT_VISS_ERR("dev type %d, sns type %d", enDevType, enSnsDevType);
        goto OUT;
    }

    s32Ret = pstSnsObj->pfnGetVissDevAttrBySns(enSnsType, &stVissDevAttr);
    if (s32Ret != EI_SUCCESS)
    {
        PRT_VISS_INFO("pfnGetVissDevAttrBySns failed with %#x!\n", s32Ret);
        goto OUT;
    }

    EI_MI_VISS_ResetDev(VissDev);

    s32Ret = EI_MI_VISS_EnableSnsMclk(VissDev, stVissDevAttr.mClock);
    if (s32Ret != EI_SUCCESS)
    {
        PRT_VISS_INFO("EI_MI_VISS_EnableSnsMclk failed with %#x!\n", s32Ret);
        goto OUT;
    }


    s32Ret = EI_MI_VISS_SensorPowerUp(VissDev);
    if (s32Ret != EI_SUCCESS)
    {
        PRT_VISS_INFO("VISS_SensorPowerUp failed with %#x!\n", s32Ret);
        goto OUT;
    }

    bDetect = EI_MI_VISS_SensorDetect(VissDev);

    while(bDetect == EI_SUCCESS) {
        s32Ret = VISS_SensorEnumSnstype(VissDev, i, &eSnstype);
        i++;

        if (s32Ret != EI_SUCCESS)
            break;

        printf("type:%d  %d\n", i, eSnstype);
    }

    s32Ret = EI_MI_VISS_SensorPowerDown(VissDev);
    if (s32Ret != EI_SUCCESS)
    {
        PRT_VISS_INFO("VISS_SensorPowerDown failed with %#x!\n", s32Ret);
        goto OUT;
    }

OUT:
    s32Ret = pstSnsObj->pfnUnRegisterVissCallback(VissDev);

    if (bDetect || s32Ret)
        return EI_ERR_VISS_NOT_SUPPORT;

    return EI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
