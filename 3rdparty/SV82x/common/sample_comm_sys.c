/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_sys.c
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

EI_U32 SAMPLE_COMM_SYS_Init(VBUF_CONFIG_S *pstVbConfig)
{
    EI_S32 s32Ret = EI_SUCCESS;
    EI_S32 i = 0;

    EI_MI_MBASE_Init();
    EI_MI_VBUF_Init();
    EI_MI_MLINK_Init();

    s32Ret = EI_MI_VBUF_InitVbufConfig(pstVbConfig);
    if (s32Ret) {
        EI_TRACE_VBUF(EI_DBG_ERR, "create pool size %d failed!\n", pstVbConfig->astCommPool[i].u32BufSize);
        return s32Ret;
    }

    return s32Ret;
}

EI_U32 SAMPLE_COMM_SYS_Exit(VBUF_CONFIG_S *pstVbConfig)
{
    EI_S32 s32Ret = EI_SUCCESS;
    EI_S32 i = 0;

    s32Ret = EI_MI_VBUF_ExitVbufConfig(pstVbConfig);
    if (s32Ret) {
        EI_TRACE_VBUF(EI_DBG_ERR, "create pool size %d failed!\n", pstVbConfig->astCommPool[i].u32BufSize);
        return s32Ret;
    }
    EI_MI_MLINK_Exit();
    EI_MI_VBUF_Exit();
    EI_MI_MBASE_Exit();

    return s32Ret;
}


EI_S32 SAMPLE_COMM_IPPU_Link_VPU(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VC_CHN VcChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_IPPU;
    stSrcChn.s32DevId  = IppuDev;
    stSrcChn.s32ChnId  = IppuChn;

    stDestChn.enModId  = EI_ID_VPU;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VcChn;

    s32Ret = EI_MI_MLINK_Link(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VBUF(EI_DBG_ERR, "link ippu src_dev:%d_chn:%d to vpu dst_chn:%d error\n", IppuDev, IppuChn, VcChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_IPPU_Link_JVC(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VC_CHN VcChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_IPPU;
    stSrcChn.s32DevId  = IppuDev;
    stSrcChn.s32ChnId  = IppuChn;

    stDestChn.enModId  = EI_ID_JVC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VcChn;

    s32Ret = EI_MI_MLINK_Link(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VBUF(EI_DBG_ERR, "link ippu src_dev:%d_chn:%d to jvc dst_chn:%d error\n", IppuDev, IppuChn, VcChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}



EI_S32 SAMPLE_COMM_IPPU_UnLink_VPU(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VC_CHN VcChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_IPPU;
    stSrcChn.s32DevId  = IppuDev;
    stSrcChn.s32ChnId  = IppuChn;

    stDestChn.enModId  = EI_ID_VPU;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VcChn;

    s32Ret = EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VBUF(EI_DBG_ERR, "unlink ippu src_dev:%d_chn:%d to vpu dst_chn:%d error\n", IppuDev, IppuChn, VcChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_IPPU_UnLink_JVC(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VC_CHN VcChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_IPPU;
    stSrcChn.s32DevId  = IppuDev;
    stSrcChn.s32ChnId  = IppuChn;

    stDestChn.enModId  = EI_ID_JVC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VcChn;

    s32Ret = EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VBUF(EI_DBG_ERR, "unlink ippu src_dev:%d_chn:%d to jvc dst_chn:%d error\n", IppuDev, IppuChn, VcChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_Link_VPU(VISS_DEV VissDev, VISS_CHN VissChn, VC_CHN VcChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_VISS;
    stSrcChn.s32DevId  = VissDev;
    stSrcChn.s32ChnId  = VissChn;

    stDestChn.enModId  = EI_ID_VPU;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VcChn;

    s32Ret = EI_MI_MLINK_Link(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VBUF(EI_DBG_ERR, "link viss src_dev:%d_chn:%d to vpu dst_chn:%d error\n", VissDev, VissChn, VcChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_UnLink_VPU(VISS_DEV VissDev, VISS_CHN VissChn, VC_CHN VcChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_VISS;
    stSrcChn.s32DevId  = VissDev;
    stSrcChn.s32ChnId  = VissChn;

    stDestChn.enModId  = EI_ID_VPU;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VcChn;

    s32Ret = EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VBUF(EI_DBG_ERR, "unlink viss src_dev:%d_chn:%d to vpu dst_chn:%d error\n", VissDev, VissChn, VcChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_Link_JVC(VISS_DEV VissDev, VISS_CHN VissChn, VC_CHN VcChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_VISS;
    stSrcChn.s32DevId  = VissDev;
    stSrcChn.s32ChnId  = VissChn;

    stDestChn.enModId  = EI_ID_JVC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VcChn;

    s32Ret = EI_MI_MLINK_Link(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VBUF(EI_DBG_ERR, "link viss src_dev:%d_chn:%d to jvc dst_chn:%d error\n", VissDev, VissChn, VcChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_UnLink_JVC(VISS_DEV VissDev, VISS_CHN VissChn, VC_CHN VcChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_VISS;
    stSrcChn.s32DevId  = VissDev;
    stSrcChn.s32ChnId  = VissChn;

    stDestChn.enModId  = EI_ID_JVC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = VcChn;

    s32Ret = EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VBUF(EI_DBG_ERR, "unlink viss src_dev:%d_chn:%d to jvc dst_chn:%d error\n", VissDev, VissChn, VcChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VISS_Link_DOSS(VISS_DEV VissDev, VISS_CHN VissChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_VISS;
    stSrcChn.s32DevId  = VissDev;
    stSrcChn.s32ChnId  = VissChn;

    stDestChn.enModId  = EI_ID_DOSS;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = EI_MI_MLINK_Link(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "link viss src_dev:%d_chn:%d to doss layer:%d_chn:%d error\n",
            VissDev, VissChn, VoLayer, VoChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_VISS_UnLink_DOSS(VISS_DEV VissDev, VISS_CHN VissChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_VISS;
    stSrcChn.s32DevId  = VissDev;
    stSrcChn.s32ChnId  = VissChn;

    stDestChn.enModId  = EI_ID_DOSS;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "unlink viss src_dev:%d_chn:%d to doss layer:%d_chn:%d error\n",
            VissDev, VissChn, VoLayer, VoChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_IPPU_Link_DOSS(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_IPPU;
    stSrcChn.s32DevId  = IppuDev;
    stSrcChn.s32ChnId  = IppuChn;

    stDestChn.enModId  = EI_ID_DOSS;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = EI_MI_MLINK_Link(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "link ippu src_dev:%d_chn:%d to doss layer:%d_chn:%d error\n",
            IppuDev, IppuChn, VoLayer, VoChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_IPPU_UnLink_DOSS(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VO_LAYER VoLayer, VO_CHN VoChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_IPPU;
    stSrcChn.s32DevId  = IppuDev;
    stSrcChn.s32ChnId  = IppuChn;

    stDestChn.enModId  = EI_ID_DOSS;
    stDestChn.s32DevId = VoLayer;
    stDestChn.s32ChnId = VoChn;

    s32Ret = EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "unlink ippu src_dev:%d_chn:%d to doss layer:%d_chn:%d error\n",
            IppuDev, IppuChn, VoLayer, VoChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_DOSS_Link_DOSS(VO_LAYER SrcVoLayer, VO_CHN SrcVoChn, VO_LAYER DstVoLayer, VO_CHN DstVoChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stSrcChn.enModId   = EI_ID_DOSS;
    stSrcChn.s32DevId  = SrcVoLayer;
    stSrcChn.s32ChnId  = SrcVoChn;

    stDestChn.enModId  = EI_ID_DOSS;
    stDestChn.s32DevId = DstVoLayer;
    stDestChn.s32ChnId = DstVoChn;

    s32Ret = EI_MI_MLINK_Link(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "link doss src_dev:%d_chn:%d to doss layer:%d_chn:%d error\n",
            SrcVoLayer, SrcVoChn, DstVoLayer, DstVoChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_DOSS_UnLink_DOSS(VO_LAYER DstVoLayer, VO_CHN DstVoChn)
{
    EI_S32 s32Ret = EI_SUCCESS;
    MDP_CHN_S stSrcChn;
    MDP_CHN_S stDestChn;

    stDestChn.enModId  = EI_ID_DOSS;
    stDestChn.s32DevId = DstVoLayer;
    stDestChn.s32ChnId = DstVoChn;

    s32Ret = EI_MI_MLINK_GetSrcbySink(&stDestChn, &stSrcChn);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "doss sink layer:%d_chn:%d get src info failed\n");
        return s32Ret;
    }

    s32Ret = EI_MI_MLINK_Link(&stSrcChn, &stDestChn);

    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "unlink doss src_dev:%d_chn:%d to doss layer:%d_chn:%d error\n",
            stSrcChn.s32DevId, stSrcChn.s32ChnId, DstVoLayer, DstVoChn);
        return s32Ret;
    }

    return EI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
