/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_isp.c
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
#endif /* __cplusplus */

#include "ei_posix.h"

#include "sample_comm.h"

#define USE_CUSTOMER_AF 0

pthread_t g_IspPid[ISP_MAX_DEV_NUM] = {0};
EI_U32 g_au32IspSnsId[ISP_MAX_DEV_NUM] = {0, 1};

static void *SAMPLE_COMM_ISP_Thread(void *param)
{
    EI_S32 s32Ret;
    ISP_DEV dev;
    EI_CHAR acThreadName[16];

    dev = (ISP_DEV)(EI_UL)param;

    sprintf(acThreadName, "ISP%d_Run", dev);
    prctl(PR_SET_NAME, acThreadName);

    s32Ret = EI_MI_ISP_Run(dev);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
    }

    return NULL;
}

EI_S32 SAMPLE_COMM_ISP_Register_AeLib(ISP_DEV dev)
{
    EI_S32 s32Ret;
    ISP_3ALIB_S stAeLib;

    stAeLib.s32Id = dev;
    strncpy(stAeLib.acLibName, EI_AE_LIB_NAME, sizeof(EI_AE_LIB_NAME));
    s32Ret = EI_MI_AE_Register(dev, &stAeLib);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_ISP_UnRegister_AeLib(ISP_DEV dev)
{
    EI_S32 s32Ret;
    ISP_3ALIB_S stAeLib;

    stAeLib.s32Id = dev;
    strncpy(stAeLib.acLibName, EI_AE_LIB_NAME, sizeof(EI_AE_LIB_NAME));
    s32Ret = EI_MI_AE_UnRegister(dev, &stAeLib);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_ISP_Register_AwbLib(ISP_DEV dev)
{
    EI_S32 s32Ret;
    ISP_3ALIB_S stAwbLib;

    stAwbLib.s32Id = dev;
    strncpy(stAwbLib.acLibName, EI_AWB_LIB_NAME, sizeof(EI_AWB_LIB_NAME));
    s32Ret = EI_MI_AWB_Register(dev, &stAwbLib);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
    }
    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_ISP_UnRegister_AwbLib(ISP_DEV dev)
{
    EI_S32 s32Ret;
    ISP_3ALIB_S stAwbLib;

    stAwbLib.s32Id = dev;
    strncpy(stAwbLib.acLibName, EI_AWB_LIB_NAME, sizeof(EI_AWB_LIB_NAME));
    s32Ret = EI_MI_AWB_UnRegister(dev, &stAwbLib);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_ISP_Register_AfLib(ISP_DEV dev)
{
    EI_S32 s32Ret = EI_SUCCESS;
    ISP_3ALIB_S stAfLib;

    stAfLib.s32Id = dev;
#if USE_CUSTOMER_AF
    strncpy(stAfLib.acLibName, EI_AF_CUSTOMER_LIB_NAME, sizeof(EI_AF_CUSTOMER_LIB_NAME));
    s32Ret = EI_MI_AF_CUSTOMER_Register(dev, &stAfLib);
#else
    strncpy(stAfLib.acLibName, EI_AF_LIB_NAME, sizeof(EI_AF_LIB_NAME));
    s32Ret = EI_MI_AF_Register(dev, &stAfLib);
#endif
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
    }
    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_ISP_UnRegister_AfLib(ISP_DEV dev)
{
    EI_S32 s32Ret = EI_SUCCESS;
    ISP_3ALIB_S stAfLib;

    stAfLib.s32Id = dev;
#if USE_CUSTOMER_AF
    strncpy(stAfLib.acLibName, EI_AF_CUSTOMER_LIB_NAME, sizeof(EI_AF_CUSTOMER_LIB_NAME));
    s32Ret = EI_MI_AF_CUSTOMER_UnRegister(dev, &stAfLib);
#else
    strncpy(stAfLib.acLibName, EI_AF_LIB_NAME, sizeof(EI_AF_LIB_NAME));
    s32Ret = EI_MI_AF_UnRegister(dev, &stAfLib);
#endif
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_ISP_Run(ISP_DEV dev)
{
    EI_S32 s32Ret = EI_SUCCESS;
    pthread_attr_t *pstAttr = NULL;

    ISP_CHECK_DEV(dev);

    s32Ret = pthread_create(&g_IspPid[dev], pstAttr, SAMPLE_COMM_ISP_Thread, (EI_VOID *)(EI_UL)dev);
    if (0 != s32Ret) {
        EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        goto out;
    }

out:

    if (NULL != pstAttr) {
        pthread_attr_destroy(pstAttr);
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_ISP_Regiter_Sensor(VISS_DEV Dev, ISP_DEV dev, EI_U32 u32SnsId)
{
    ISP_3ALIB_S stAeLib;
    ISP_3ALIB_S stAwbLib;
    CAMERA_OBJ_S *pstSnsObj;
    EI_S32 s32Ret = EI_FAILURE;

    ISP_CHECK_DEV(dev);

    pstSnsObj = SAMPLE_COMM_VISS_GetSnsObj(u32SnsId);
    if (EI_NULL == pstSnsObj) {
        EI_TRACE_ISP(EI_DBG_ERR, "sensor %d not exist!\n", u32SnsId);
        return EI_FAILURE;
    }

    stAeLib.s32Id = dev;
    stAwbLib.s32Id = dev;
    strncpy(stAeLib.acLibName, EI_AE_LIB_NAME, sizeof(EI_AE_LIB_NAME));
    strncpy(stAwbLib.acLibName, EI_AWB_LIB_NAME, sizeof(EI_AWB_LIB_NAME));

    if (pstSnsObj->pfnRegisterIspCallback != EI_NULL) {
        s32Ret = pstSnsObj->pfnRegisterIspCallback(Dev, dev, &stAeLib, &stAwbLib);
        if (s32Ret != EI_SUCCESS) {
            EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    g_au32IspSnsId[dev] = u32SnsId;

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_ISP_UnRegister_Sensor(ISP_DEV dev)
{
    ISP_3ALIB_S stAeLib;
    ISP_3ALIB_S stAwbLib;
    EI_U32 u32SnsId;
    CAMERA_OBJ_S *pstSnsObj;
    EI_S32 s32Ret = EI_FAILURE;

    ISP_CHECK_DEV(dev);

    u32SnsId = g_au32IspSnsId[dev];
    pstSnsObj = SAMPLE_COMM_VISS_GetSnsObj(u32SnsId);
    if (EI_NULL == pstSnsObj) {
        return EI_FAILURE;
    }

    stAeLib.s32Id = dev;
    stAwbLib.s32Id = dev;
    strncpy(stAeLib.acLibName, EI_AE_LIB_NAME, sizeof(EI_AE_LIB_NAME));
    strncpy(stAwbLib.acLibName, EI_AWB_LIB_NAME, sizeof(EI_AWB_LIB_NAME));

    if (pstSnsObj->pfnUnRegisterIspCallback != EI_NULL) {
        s32Ret = pstSnsObj->pfnUnRegisterIspCallback(dev, &stAeLib, &stAwbLib);
        if (s32Ret != EI_SUCCESS) {
            EI_TRACE_ISP(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_ISP_Start(SAMPLE_VISS_INFO_S *pstVissInfo)
{
    EI_S32 s32Ret = EI_SUCCESS;
    ISP_DEV dev = pstVissInfo->stIspInfo.IspDev;
    ISP_PUB_ATTR_S stPubAttr = {0};
    VISS_DEV_ATTR_S stSnsAttr = {0};

    s32Ret = SAMPLE_COMM_ISP_Regiter_Sensor(pstVissInfo->stDevInfo.VissDev, dev, pstVissInfo->stSnsInfo.enSnsType);
    if (s32Ret != EI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_ISP_Register_AwbLib(dev);
    if (s32Ret != EI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_ISP_Register_AeLib(dev);
    if (s32Ret != EI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_ISP_Register_AfLib(dev);
    if (s32Ret != EI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = EI_MI_ISP_MemInit(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_ISP(EI_DBG_ERR, "Init memory failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    SAMPLE_COMM_VISS_GetDevAttrBySns(pstVissInfo->stSnsInfo.enSnsType, &stSnsAttr);
    stPubAttr.s32IspDev     = dev;
    stPubAttr.stSnsSize     = stSnsAttr.stSize;
    stPubAttr.pcSnsName     = stSnsAttr.pcSnsName;
    stPubAttr.u32DataSrc    = pstVissInfo->stDevInfo.VissDev;
    switch (stSnsAttr.enRgbSeq) {
    case VISS_RGB_SEQ_RGGB:
        stPubAttr.enBayer   = ISP_BAYER_RGGB;
        break;
    case VISS_RGB_SEQ_GRBG:
        stPubAttr.enBayer   = ISP_BAYER_GRBG;
        break;
    case VISS_RGB_SEQ_GBRG:
        stPubAttr.enBayer   = ISP_BAYER_GBRG;
        break;
    case VISS_RGB_SEQ_BGGR:
        stPubAttr.enBayer   = ISP_BAYER_BGGR;
        break;
    default:
        stPubAttr.enBayer   = ISP_BAYER_RGGB;
        break;
    }

    switch (stSnsAttr.enPixelFormat) {
    case PIX_FMT_RGB_BAYER_14BITS:
        stPubAttr.enBitW   = ISP_IN_BITS_14;
        break;
    case PIX_FMT_RGB_BAYER_16BITS:
        stPubAttr.enBitW   = ISP_IN_BITS_16;
        break;
    case PIX_FMT_RGB_BAYER_20BITS:
        stPubAttr.enBitW   = ISP_IN_BITS_20;
        break;
    default:
        stPubAttr.enBitW   = ISP_IN_BITS_12;
        break;
    }

    switch (stSnsAttr.enPixelFormat) {
    case PIX_FMT_RGB_BAYER_8BITS:
    case PIX_FMT_RGB_BAYER_10BITS:
    case PIX_FMT_RGB_BAYER_12BITS:
    case PIX_FMT_RGB_BAYER_14BITS:
    case PIX_FMT_RGB_BAYER_16BITS:
    case PIX_FMT_RGB_BAYER_20BITS:
        stPubAttr.enInFmt = ISP_IN_FMT_RAW;
        break;
    case PIX_FMT_YUV_SEMIPLANAR_420:
    case PIX_FMT_YVU_SEMIPLANAR_420:
    case PIX_FMT_YUV_PLANAR_420:
    case PIX_FMT_YVU_PLANAR_420:
        stPubAttr.enInFmt = ISP_IN_FMT_YUV420;
        break;
    case PIX_FMT_YUV_SEMIPLANAR_422:
    case PIX_FMT_YVU_SEMIPLANAR_422:
    case PIX_FMT_YUV_PLANAR_422:
    case PIX_FMT_YVU_PLANAR_422:
        stPubAttr.enInFmt = ISP_IN_FMT_YUV422;
        break;
    default:
        stPubAttr.enInFmt = ISP_IN_FMT_RAW;
        break;
    }
    stPubAttr.u32IspClk = pstVissInfo->stIspInfo.u32IspClk ? pstVissInfo->stIspInfo.u32IspClk : stSnsAttr.u32IspClk;
    stPubAttr.u32IppuClk = pstVissInfo->stIspInfo.u32IppuClk ? pstVissInfo->stIspInfo.u32IppuClk : stSnsAttr.u32IppuClk;
    stPubAttr.u32SnsFps = stSnsAttr.u32Fps;
    stPubAttr.enWDRMode = stSnsAttr.enIspWdrMode;
    stPubAttr.enRunningMode = pstVissInfo->stIspInfo.enRunningMode;
    stPubAttr.stFrc = pstVissInfo->stIspInfo.stFrc;
    stPubAttr.stWndRect = pstVissInfo->stIspInfo.stCrop;
    stPubAttr.pcParamName   = pstVissInfo->stIspInfo.pcParamName;
    s32Ret = EI_MI_ISP_SetPubAttr(dev, &stPubAttr);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_ISP(EI_DBG_ERR, "SetPubAttr failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    s32Ret = EI_MI_ISP_Init(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_ISP(EI_DBG_ERR, "ISP Init failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_ISP_Run(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_ISP(EI_DBG_ERR, "ISP Run failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_ISP_StartNoSensor(ISP_DEV dev, ISP_PUB_ATTR_S *pstPubAttr)
{
    EI_S32 s32Ret = EI_SUCCESS;

    s32Ret = SAMPLE_COMM_ISP_Register_AwbLib(dev);
    if (s32Ret != EI_SUCCESS) {
        return s32Ret;
    }

    s32Ret = EI_MI_ISP_MemInit(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_IPPU(EI_DBG_ERR, "Init memory failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    s32Ret = EI_MI_ISP_SetPubAttr(dev, pstPubAttr);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_IPPU(EI_DBG_ERR, "SetPubAttr failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    s32Ret = EI_MI_ISP_Init(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_IPPU(EI_DBG_ERR, "ISP Init failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    s32Ret = SAMPLE_COMM_ISP_Run(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_ISP(EI_DBG_ERR, "ISP Run failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_ISP_Stop(ISP_DEV dev)
{
    ISP_CHECK_DEV(dev);

    if (g_IspPid[dev]) {
        EI_MI_ISP_Exit(dev);
        pthread_join(g_IspPid[dev], NULL);
        EI_MI_ISP_MemExit(dev);
        SAMPLE_COMM_ISP_UnRegister_AwbLib(dev);
        SAMPLE_COMM_ISP_UnRegister_AeLib(dev);
        SAMPLE_COMM_ISP_UnRegister_AfLib(dev);
        SAMPLE_COMM_ISP_UnRegister_Sensor(dev);
        g_IspPid[dev] = 0;
    }

    return EI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

