/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_ippu.c
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

EI_S32 SAMPLE_COMM_IPPU_Init(SAMPLE_VISS_INFO_S *pstVissInfo)
{
    EI_S32 s32Ret = EI_SUCCESS;
    IPPU_DEV dev = pstVissInfo->stIspInfo.IspDev;
    ISP_PUB_ATTR_S stPubAttr = {0};
    VISS_DEV_ATTR_S stSnsAttr = {0};

    s32Ret = EI_MI_ISP_MemInit(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_IPPU(EI_DBG_ERR, "Init memory failed with %#x!\n", s32Ret);
        SAMPLE_COMM_ISP_Stop(dev);
        return EI_FAILURE;
    }

    SAMPLE_COMM_VISS_GetDevAttrBySns(pstVissInfo->stSnsInfo.enSnsType, &stSnsAttr);
    stPubAttr.s32IspDev     = dev;
    stPubAttr.stSnsSize     = stSnsAttr.stSize;
    stPubAttr.pcSnsName     = stSnsAttr.pcSnsName;
    stPubAttr.u32DataSrc    = pstVissInfo->stDevInfo.VissDev;
    switch (stSnsAttr.enPixelFormat) {
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
        stPubAttr.enInFmt = ISP_IN_FMT_YUV420;
        break;
    }
    stPubAttr.u32IspClk = pstVissInfo->stIspInfo.u32IspClk ? pstVissInfo->stIspInfo.u32IspClk : stSnsAttr.u32IspClk;
    stPubAttr.u32IppuClk = pstVissInfo->stIspInfo.u32IppuClk ? pstVissInfo->stIspInfo.u32IppuClk : stSnsAttr.u32IppuClk;
    stPubAttr.u32SnsFps = stSnsAttr.u32Fps;
    stPubAttr.enRunningMode = pstVissInfo->stIspInfo.enRunningMode;
    stPubAttr.stFrc = pstVissInfo->stIspInfo.stFrc;
    stPubAttr.stWndRect = pstVissInfo->stIspInfo.stCrop;
    stPubAttr.pcParamName   = pstVissInfo->stIspInfo.pcParamName;
    s32Ret = EI_MI_ISP_SetPubAttr(dev, &stPubAttr);
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

    s32Ret = EI_MI_ISP_Run(dev);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_IPPU(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        return EI_FAILURE;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_IPPU_InitNoSensor(IPPU_DEV dev, ISP_PUB_ATTR_S *pstPubAttr)
{
    EI_S32 s32Ret = EI_SUCCESS;

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

    s32Ret = EI_MI_ISP_Run(dev);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_IPPU(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        return EI_FAILURE;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_IPPU_Exit(IPPU_DEV dev)
{
    ISP_CHECK_DEV(dev);

    EI_MI_ISP_Exit(dev);
    EI_MI_ISP_MemExit(dev);

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_IPPU_Start(IPPU_DEV dev, EI_BOOL *pabChnEnable, IPPU_DEV_ATTR_S *pstIppuDevAttr,
            IPPU_CHN_ATTR_S* pastIppuChnAttr)
{
    IPPU_CHN ch;
    EI_S32 s32Ret;
    EI_S32 j;

    s32Ret = EI_MI_IPPU_Create(dev, pstIppuDevAttr);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_IPPU(EI_DBG_ERR, "EI_MI_IPPU_Create(dev:%d) failed with %#x!\n", dev, s32Ret);
        return EI_FAILURE;
    }

    for (j = 0; j < IPPU_PHY_CHN_MAX_NUM; j++) {
        if(EI_TRUE == pabChnEnable[j]) {
            ch = j;
            s32Ret = EI_MI_IPPU_SetChnAttr(dev, ch, &pastIppuChnAttr[ch]);
            if (s32Ret != EI_SUCCESS)
            {
                EI_TRACE_IPPU(EI_DBG_ERR, "EI_MI_IPPU_SetChnAttr failed with %#x\n", s32Ret);
                return EI_FAILURE;
            }

            s32Ret = EI_MI_IPPU_EnableChn(dev, ch);
            if (s32Ret != EI_SUCCESS) {
                EI_TRACE_IPPU(EI_DBG_ERR, "EI_MI_IPPU_EnableChn failed with %#x\n", s32Ret);
                return EI_FAILURE;
            }
        }
    }

    s32Ret = EI_MI_IPPU_Start(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_IPPU(EI_DBG_ERR, "EI_MI_IPPU_Start failed with %#x\n", s32Ret);
        return EI_FAILURE;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_IPPU_Stop(IPPU_DEV dev, EI_BOOL *pabChnEnable)
{
    EI_S32 j;
    EI_S32 s32Ret = EI_SUCCESS;
    IPPU_CHN ch;
    IPPU_CHN_ATTR_S stIppuChnAttr;

    for (j = 0; j < IPPU_PHY_CHN_MAX_NUM; j++) {
        if(EI_TRUE == pabChnEnable[j]) {
            ch = j;
            EI_MI_IPPU_GetChnAttr(dev, ch, &stIppuChnAttr);
            stIppuChnAttr.u32Depth = 0;
            EI_MI_IPPU_SetChnAttr(dev, ch, &stIppuChnAttr);

            s32Ret = EI_MI_IPPU_DisableChn(dev, ch);
            if (s32Ret != EI_SUCCESS) {
                EI_TRACE_IPPU(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
                return EI_FAILURE;
            }
        }
    }

    s32Ret = EI_MI_IPPU_Stop(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_IPPU(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        return EI_FAILURE;
    }

    s32Ret = EI_MI_IPPU_Destroy(dev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_IPPU(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        return EI_FAILURE;
    }

    return EI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

