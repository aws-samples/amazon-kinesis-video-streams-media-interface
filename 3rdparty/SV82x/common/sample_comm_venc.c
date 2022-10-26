/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_venc.c
 * @Date      :    2021-7-7
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Source file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#include "ei_posix.h"

#include "ei_common.h"
#include "sample_comm.h"
#include "mi_venc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#ifdef __EOS__
#define PATH_ROOT "/mnt/userdata"
#else
#define PATH_ROOT "/data"
#endif


#define SAVE_OUTPUT_STREAM 1

static SAMPLE_VENC_PARA_S gs_stPara;
static SAMPLE_VENC_QPMAP_PARA_S gs_stQpMapPara;

static EI_S32 SAMPLE_COMM_VENC_GenerateQpMapData(EI_S32 type,
            EI_S32 width, EI_S32 height, EI_U8 *data)
{
    EI_S32 i  = 0;
    EI_S32 j  = 0;
    EI_S32 qp = 0;
    EI_S32 k  = 0;

    if (type == PT_H264) {
        EI_S32 mb_width = (width + 15) >> 4;
        EI_S32 mb_height = (height + 15) >> 4;

        for (j = 0; j < mb_height; j++) {
            qp = 10 + (j%10);
            for (i = 0; i < mb_width; i++) {
                *(data + k) = qp;
                k++;
            }
        }
    } else if (type == PT_H265) {
        EI_S32 mb_width = (width + 31) >> 5;
        EI_S32 mb_height = (height + 31) >> 5;

        for (j = 0; j < mb_height; j++) {
            qp = 10 + (j%10);
            for (i = 0; i < mb_width; i++) {
                *(data + k) = qp;
                k++;
            }
        }
    }

    return 0;
}



EI_S32  SAMPLE_COMM_VENC_Creat(VC_CHN VencChn, PAYLOAD_TYPE_E enType,
    SAMPLE_RC_E RcMode, SAMPLE_VENC_CONFIG_S *pstVencConfig,
    COMPRESS_MODE_E enCompressMode, VENC_GOP_MODE_E GopMode)
{
    VENC_CHN_ATTR_S        stVencChnAttr = {0};
    EI_S32 s32Ret;
    EI_U32 u32width, u32height, u32srcframerate, u32dstframerate, u32bitrate;
    PIXEL_FORMAT_E pixel_format;


    u32width = pstVencConfig->u32width;
    u32height = pstVencConfig->u32height;
    pixel_format = pstVencConfig->enInputFormat;
    u32srcframerate = pstVencConfig->u32srcframerate;
    u32dstframerate = pstVencConfig->u32dstframerate;
    u32bitrate = pstVencConfig->u32bitrate;


    memset(&stVencChnAttr, 0, sizeof(VENC_CHN_ATTR_S));

    stVencChnAttr.stVencAttr.enType = enType;
    stVencChnAttr.stVencAttr.u32PicWidth = u32width;
    stVencChnAttr.stVencAttr.u32PicHeight = u32height;
    stVencChnAttr.stVencAttr.enInputFormat = pixel_format;
    stVencChnAttr.stVencAttr.u32BufSize = u32width * u32height;
    stVencChnAttr.stVencAttr.enCompressMode = enCompressMode;

    if (GopMode == VENC_GOPMODE_NORMAL_P) {
        stVencChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMAL_P;
        stVencChnAttr.stGopAttr.stNormalP.enGopType = VENC_GOPTYPE_TYPICAL;
        if (pstVencConfig->u32IdrPeriod)
            stVencChnAttr.stGopAttr.stNormalP.u32IdrPeriod = pstVencConfig->u32IdrPeriod;
        else
            stVencChnAttr.stGopAttr.stNormalP.u32IdrPeriod = 60;
    } else if (GopMode == VENC_GOPMODE_DUAL_P) {
        stVencChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_DUAL_P;
        stVencChnAttr.stGopAttr.stDualP.enGopType = VENC_GOPTYPE_TYPICAL;
        if (pstVencConfig->u32IdrPeriod)
            stVencChnAttr.stGopAttr.stNormalP.u32IdrPeriod = pstVencConfig->u32IdrPeriod;
        else
            stVencChnAttr.stGopAttr.stNormalP.u32IdrPeriod = 60;
    } else if (GopMode == VENC_GOPMODE_ADV_P) {
        stVencChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_ADV_P;
        stVencChnAttr.stGopAttr.stAdvP.enGopType = VENC_GOPTYPE_TYPICAL;
        if (pstVencConfig->u32IdrPeriod)
            stVencChnAttr.stGopAttr.stNormalP.u32IdrPeriod = pstVencConfig->u32IdrPeriod;
        else
            stVencChnAttr.stGopAttr.stNormalP.u32IdrPeriod = 60;
        stVencChnAttr.stGopAttr.stAdvP.u32BgInterval = 60;
    }

    switch (enType) {
    case PT_H265: {
        VENC_ATTR_H265_S *pstAttrH265e;
        pstAttrH265e = &stVencChnAttr.stVencAttr.stAttrH265e;
        pstAttrH265e->enMirrorDirection = MIRDIRDIR_NONE;
        pstAttrH265e->enRotation = ROTATION_0;
        pstAttrH265e->stCropRect.s32X = 0;
        pstAttrH265e->stCropRect.s32Y = 0;
        pstAttrH265e->stCropRect.u32Width = u32width;
        pstAttrH265e->stCropRect.u32Height = u32height;
        pstAttrH265e->u32ReportEnable = 0;

        if (RcMode == SAMPLE_RC_CBR) {
            VENC_H265_CBR_S *pstH265Cbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
            pstH265Cbr = &stVencChnAttr.stRcAttr.stH265Cbr;

            pstH265Cbr->u32BitRate = u32bitrate;
            pstH265Cbr->u32CuLevelEnable = 1;
            pstH265Cbr->u32SrcFrameRate = u32srcframerate;
            pstH265Cbr->u32DstFrameRate = u32dstframerate;

        } else if (RcMode == SAMPLE_RC_VBR) {
            VENC_H265_VBR_S *pstH265Vbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
            pstH265Vbr = &stVencChnAttr.stRcAttr.stH265Vbr;

            pstH265Vbr->u32BitRate = u32bitrate;
            pstH265Vbr->u32CuLevelEnable = 1;
            pstH265Vbr->u32Delay = 3000;
            pstH265Vbr->u32SrcFrameRate = u32srcframerate;
            pstH265Vbr->u32DstFrameRate = u32dstframerate;
            pstH265Vbr->u32MaxIQp = 51;
            pstH265Vbr->u32MinIQp = 16;
            pstH265Vbr->u32MaxPQp = 51;
            pstH265Vbr->u32MinPQp = 16;

        } else if (RcMode == SAMPLE_RC_AVBR) {
            VENC_H265_AVBR_S *pstH265Avbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265AVBR;
            pstH265Avbr = &stVencChnAttr.stRcAttr.stH265Avbr;

            pstH265Avbr->u32BitRate = u32bitrate;
            pstH265Avbr->u32BitRateRation = 15;
            pstH265Avbr->u32CuLevelEnable = 1;
            pstH265Avbr->u32Delay = 3000;
            pstH265Avbr->u32SrcFrameRate = u32srcframerate;
            pstH265Avbr->u32DstFrameRate = u32dstframerate;
        }

        break;
    }
    case PT_H264: {
        VENC_ATTR_H264_S *pstAttrH264e;
        pstAttrH264e = &stVencChnAttr.stVencAttr.stAttrH264e;

        pstAttrH264e->enMirrorDirection = MIRDIRDIR_NONE;
        pstAttrH264e->enRotation = ROTATION_0;
        pstAttrH264e->stCropRect.s32X = 0;
        pstAttrH264e->stCropRect.s32Y = 0;
        pstAttrH264e->stCropRect.u32Width = u32width;
        pstAttrH264e->stCropRect.u32Height = u32height;
        pstAttrH264e->u32ReportEnable = 0;

        if (RcMode == SAMPLE_RC_CBR) {
            VENC_H264_CBR_S *pstH264Cbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
            pstH264Cbr = &stVencChnAttr.stRcAttr.stH264Cbr;

            pstH264Cbr->u32BitRate = u32bitrate;
            pstH264Cbr->u32CuLevelEnable = 1;
            pstH264Cbr->u32SrcFrameRate = u32srcframerate;
            pstH264Cbr->u32DstFrameRate = u32dstframerate;
        } else if (RcMode == SAMPLE_RC_VBR) {
            VENC_H264_VBR_S *pstH264Vbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
            pstH264Vbr = &stVencChnAttr.stRcAttr.stH264Vbr;

            pstH264Vbr->u32BitRate = u32bitrate;
            pstH264Vbr->u32CuLevelEnable = 1;
            pstH264Vbr->u32Delay = 3000;
            pstH264Vbr->u32SrcFrameRate = u32srcframerate;
            pstH264Vbr->u32DstFrameRate = u32dstframerate;
            pstH264Vbr->u32MaxIQp = 51;
            pstH264Vbr->u32MinIQp = 16;
            pstH264Vbr->u32MaxPQp = 51;
            pstH264Vbr->u32MinPQp = 16;

        } else if (RcMode == SAMPLE_RC_AVBR) {
            VENC_H264_AVBR_S *pstH264Avbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264AVBR;
            pstH264Avbr = &stVencChnAttr.stRcAttr.stH264Avbr;

            pstH264Avbr->u32BitRate = u32bitrate;
            pstH264Avbr->u32BitRateRation = 15;
            pstH264Avbr->u32CuLevelEnable = 1;
            pstH264Avbr->u32Delay = 3000;
            pstH264Avbr->u32SrcFrameRate = u32srcframerate;
            pstH264Avbr->u32DstFrameRate = u32dstframerate;
        }
        break;
    }
    case PT_MJPEG: {
        VENC_ATTR_MJPEG_S *pstAttrMjpeg;
        pstAttrMjpeg = &stVencChnAttr.stVencAttr.stAttrMjpege;

        pstAttrMjpeg->u32PicStride = u32width;


        if (RcMode == SAMPLE_RC_CBR) {
            VENC_MJPEG_CBR_S *pstMJPEGCbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
            pstMJPEGCbr = &stVencChnAttr.stRcAttr.stMjpegCbr;

            pstMJPEGCbr->u32BitRate = u32bitrate;
            pstMJPEGCbr->u32SrcFrameRate = u32srcframerate;
            pstMJPEGCbr->u32DstFrameRate = u32dstframerate;

        } else if (RcMode == SAMPLE_RC_VBR) {
            VENC_MJPEG_VBR_S *pstMJPEGVbr;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGVBR;
            pstMJPEGVbr = &stVencChnAttr.stRcAttr.stMjpegVbr;

            pstMJPEGVbr->u32BitRate = u32bitrate;
            pstMJPEGVbr->u32SrcFrameRate = u32srcframerate;
            pstMJPEGVbr->u32DstFrameRate = u32dstframerate;
            pstMJPEGVbr->u32MinQuallity = 0;
        } else if (RcMode == SAMPLE_RC_FIXQP) {
            VENC_MJPEG_FIXQUALLITY_S *pstMJPEGFixqt;

            stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGFIXQUALLITY;
            pstMJPEGFixqt = &stVencChnAttr.stRcAttr.stMjpegFixQuallity;

            pstMJPEGFixqt->u32SrcFrameRate = u32srcframerate;
            pstMJPEGFixqt->u32DstFrameRate = u32dstframerate;
            pstMJPEGFixqt->u32Quallity = 75;
        }


        break;
    }
    case PT_JPEG: {
        stVencChnAttr.stVencAttr.stAttrJpege.u32Quality = 80;
        stVencChnAttr.stVencAttr.u32BufSize             = 2*1024*1024;
        break;
    }
    default:
        break;
    }

    s32Ret = EI_MI_VENC_CreateChn(VencChn, &stVencChnAttr);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_VENC(EI_DBG_ERR, "creat chn-%d error : %d\n", VencChn, s32Ret);
        return s32Ret;
    }

    memcpy(&gs_stPara.VeChnAttr[VencChn], &stVencChnAttr, sizeof(VENC_CHN_ATTR_S));
    memcpy(&gs_stQpMapPara.VeChnAttr[VencChn], &stVencChnAttr, sizeof(VENC_CHN_ATTR_S));

    return EI_SUCCESS;

}

EI_S32 SAMPLE_COMM_VENC_SnapStart(VC_CHN VencChn, PAYLOAD_TYPE_E enType, SAMPLE_VENC_CONFIG_S *pstVencConfig)
{
    EI_S32 s32Ret;
    VENC_RECV_PIC_PARAM_S  stRecvParam;
    VENC_CHN_ATTR_S        stVencChnAttr;
    VENC_JPEG_PARAM_S      stJpegParam;

    memset(&stVencChnAttr, 0, sizeof(VENC_CHN_ATTR_S));

    stVencChnAttr.stVencAttr.enType         = enType;
    stVencChnAttr.stVencAttr.u32PicWidth    = pstVencConfig->u32width;
    stVencChnAttr.stVencAttr.u32PicHeight   = pstVencConfig->u32height;
    stVencChnAttr.stVencAttr.enInputFormat  = pstVencConfig->enInputFormat;
    stVencChnAttr.stVencAttr.u32BufSize     = 2*1024*1024;
    stVencChnAttr.stVencAttr.stAttrJpege.u32Quality = 78;
    s32Ret = EI_MI_VENC_CreateChn(VencChn, &stVencChnAttr);
    if (s32Ret != EI_SUCCESS) {
        EI_PRINT("creat chn-%d error : %d\n", VencChn, s32Ret);
        return s32Ret;
    }

    memcpy(&gs_stPara.VeChnAttr[VencChn], &stVencChnAttr, sizeof(VENC_CHN_ATTR_S));

    s32Ret = EI_MI_VENC_SetJpegEncodeMode(VencChn,JPEG_ENCODE_SNAP);
    if (s32Ret != EI_SUCCESS) {
        EI_PRINT("set jpeg mode chn-%d error : %d\n", VencChn, s32Ret);
        return s32Ret;
    }

    s32Ret = EI_MI_VENC_GetJpegParam(VencChn, &stJpegParam);
    if (s32Ret != EI_SUCCESS) {
        EI_PRINT("get jpeg param chn-%d error : %d\n", VencChn, s32Ret);
        return s32Ret;
    }
    stJpegParam.u32Quality = 78;
    s32Ret = EI_MI_VENC_SetJpegParam(VencChn, &stJpegParam);
    if (s32Ret != EI_SUCCESS) {
        EI_PRINT("set jpeg param chn-%d error : %d\n", VencChn, s32Ret);
        return s32Ret;
    }

    stRecvParam.s32RecvPicNum = -1;
    s32Ret = EI_MI_VENC_StartRecvFrame(VencChn, &stRecvParam);
    if (EI_SUCCESS != s32Ret) {
        EI_PRINT("EI_MI_VENC_StartRecvFrame faild with%#x! \n", s32Ret);
        return EI_FAILURE;
    }
    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VENC_Start(VC_CHN VencChn, PAYLOAD_TYPE_E enType,
    SAMPLE_RC_E RcMode, SAMPLE_VENC_CONFIG_S *pstVencConfig,
    COMPRESS_MODE_E enCompressMode, VENC_GOP_MODE_E GopMode)
{
    EI_S32 s32Ret;
    VENC_RECV_PIC_PARAM_S  stRecvParam;


    s32Ret = SAMPLE_COMM_VENC_Creat(VencChn, enType, RcMode, pstVencConfig, enCompressMode, GopMode);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_VENC(EI_DBG_ERR, "SAMPLE_COMM_VENC_Creat faild with%#x! \n", s32Ret);
        return EI_FAILURE;
    }

    stRecvParam.s32RecvPicNum = -1;
    s32Ret = EI_MI_VENC_StartRecvFrame(VencChn, &stRecvParam);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_VENC(EI_DBG_ERR, "EI_MI_VENC_StartRecvFrame faild with%#x! \n", s32Ret);
        return EI_FAILURE;
    }
    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VENC_SaveStream(VENC_STREAM_S *pstStream, FILE *fp)
{

    if ((pstStream->pstPack.pu8Addr[0] != EI_NULL) && (pstStream->pstPack.u32Len[0] != 0)
        && fp != NULL) {
        fwrite(pstStream->pstPack.pu8Addr[0], 1, pstStream->pstPack.u32Len[0], fp);
        if ((pstStream->pstPack.pu8Addr[1] != EI_NULL) && (pstStream->pstPack.u32Len[1] != 0)) {
            fwrite(pstStream->pstPack.pu8Addr[1], 1, pstStream->pstPack.u32Len[1], fp);
        }
    } else {
        EI_TRACE_VENC(EI_DBG_ERR, "save stream failed!\n");
        return EI_FAILURE;
    }
    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_VENC_GetFilePostfix(PAYLOAD_TYPE_E enPayload, char *szFilePostfix)
{
    if (PT_H264 == enPayload) {
        strcpy(szFilePostfix, ".h264");
    } else if (PT_H265 == enPayload) {
        strcpy(szFilePostfix, ".h265");
    } else if (PT_JPEG == enPayload) {
        strcpy(szFilePostfix, ".jpg");
    } else if (PT_MJPEG == enPayload) {
        strcpy(szFilePostfix, ".mjpeg");
    } else {
        EI_TRACE_VENC(EI_DBG_ERR, "payload type err!\n");
        return EI_FAILURE;
    }
    return EI_SUCCESS;
}


EI_VOID *SAMPLE_COMM_VENC_GetVencStreamProc(EI_VOID *p)
{
    SAMPLE_VENC_PARA_S *pstAttrSampl;
    //VENC_CHN_ATTR_S *pstVencChnAttr;
    VC_CHN VencChn;
    int i;
    VENC_CHN_STATUS_S stStatus;
    VENC_STREAM_S stStream;
    EI_S32 s32Ret, encoded_packet[VC_MAX_CHN_NUM] = {0};
    FILE *flip_out[VC_MAX_CHN_NUM] = {NULL};
    EI_CHAR aszFileName[128];
    EI_CHAR szFilePostfix[10];
    //PAYLOAD_TYPE_E enPayload;
    pstAttrSampl = (SAMPLE_VENC_PARA_S *)p;

    //VencChn = pstAttrSampl->VeChn;
    //pstVencChnAttr = &pstAttrSampl->VeChnAttr;
    //enPayload = pstVencChnAttr->stVencAttr.enType;

#ifdef SAVE_OUTPUT_STREAM
    for (i = 0; i < pstAttrSampl->s32Cnt; i++) {
        VencChn = pstAttrSampl->VeChn[i];

        s32Ret = SAMPLE_COMM_VENC_GetFilePostfix(pstAttrSampl->VeChnAttr[VencChn].stVencAttr.enType, szFilePostfix);
        if (s32Ret != EI_SUCCESS) {
            EI_TRACE_VENC(EI_DBG_ERR, "SAMPLE_COMM_VENC_GetFilePostfix failed with %#x!\n", s32Ret);
            return NULL;
        }

        if (PT_JPEG != pstAttrSampl->VeChnAttr[VencChn].stVencAttr.enType) {
            snprintf(aszFileName, 32, PATH_ROOT"/stream_chn%d%s", VencChn, szFilePostfix);

            flip_out[VencChn] = fopen(aszFileName, "wb");
            if (!flip_out[VencChn]) {
                EI_TRACE_VENC(EI_DBG_ERR, "open file[%s] failed!\n",
                    aszFileName);
                return NULL;
            }else {
                EI_TRACE_VENC(EI_DBG_INFO,"chn %d stream will save at %s\n", VencChn, aszFileName);
            }

        }
        memset(&szFilePostfix[0], 0, strlen(szFilePostfix));
        memset(&aszFileName[0]  , 0, strlen(aszFileName));
    }
#endif

    while (EI_TRUE == pstAttrSampl->bThreadStart) {
        //EI_TRACE_VENC(EI_DBG_INFO, "pstAttrSampl->bThreadStart\n");
        for (i = 0; i < pstAttrSampl->s32Cnt; i++) {

            VencChn = pstAttrSampl->VeChn[i];

            s32Ret = EI_MI_VENC_QueryStatus(VencChn, &stStatus);
            if (s32Ret != EI_SUCCESS) {
                EI_TRACE_VENC(EI_DBG_ERR, "query status chn-%d error : %d\n", VencChn, s32Ret);
                break;
            }

            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32CurPacks : %d\n",          stStatus.u32CurPacks);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftEncPics : %d\n",       stStatus.u32LeftEncPics);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftPics : %d\n",          stStatus.u32LeftPics);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftRecvPics : %d\n",      stStatus.u32LeftRecvPics);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftStreamBytes : %d\n",   stStatus.u32LeftStreamBytes);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftStreamFrames : %d\n",  stStatus.u32LeftStreamFrames);

            s32Ret = EI_MI_VENC_GetStream(VencChn, &stStream, 1000);

            if (s32Ret == EI_ERR_VENC_NOBUF) {
                EI_TRACE_VENC(EI_DBG_INFO, "No buffer\n");
                usleep(5 * 1000);
                continue;
            } else if (s32Ret != EI_SUCCESS) {
                EI_TRACE_VENC(EI_DBG_ERR, "get stream chn-%d error : %d\n", VencChn, s32Ret);
                break;
            }

            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.PackType : %d\n",              stStream.PackType);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.pu8Addr[0] : %x\n",    stStream.pstPack.pu8Addr[0]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.pu8Addr[1] : %x\n",    stStream.pstPack.pu8Addr[1]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u32PhyAddr[0] : %llx\n", stStream.pstPack.u64PhyAddr[0]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u32PhyAddr[1] : %llx\n", stStream.pstPack.u64PhyAddr[1]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u32Len[0] : %d\n",     stStream.pstPack.u32Len[0]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u32Len[1] : %d\n",     stStream.pstPack.u32Len[1]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.enDataType : %d\n",    stStream.pstPack.enDataType);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.bFrameEnd : %d\n",     stStream.pstPack.bFrameEnd);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u64PTS : %lld\n",      stStream.pstPack.u64PTS);

#ifdef SAVE_OUTPUT_STREAM
            if (PT_JPEG == pstAttrSampl->VeChnAttr[VencChn].stVencAttr.enType) {
                FILE *jpegout_file = NULL;
                EI_CHAR jpegFileName[128];
                snprintf(jpegFileName, 128, PATH_ROOT"/stream_chn%d_frame%d.jpeg", VencChn, encoded_packet[VencChn]);
                EI_TRACE_VENC(EI_DBG_ERR, "jpeg file : %s\n", jpegFileName);
                jpegout_file = fopen(jpegFileName, "wb");
                if (jpegout_file == NULL) {
                    EI_TRACE_VENC(EI_DBG_ERR, "open file[%s] failed!\n", jpegFileName);
                    return NULL;
                } else {
                    s32Ret = SAMPLE_COMM_VENC_SaveStream(&stStream, jpegout_file);
                    if (s32Ret != EI_SUCCESS) {
                        EI_MI_VENC_ReleaseStream(VencChn, &stStream);
                        EI_TRACE_VENC(EI_DBG_ERR, "save stream chn-%d error : %d\n", VencChn, s32Ret);
                        break;
                    }
                    fclose(jpegout_file);
                    jpegout_file = NULL;
                }
            } else {
                s32Ret = SAMPLE_COMM_VENC_SaveStream(&stStream, flip_out[VencChn]);
                if (s32Ret != EI_SUCCESS) {
                    EI_MI_VENC_ReleaseStream(VencChn, &stStream);
                    EI_TRACE_VENC(EI_DBG_ERR, "save stream chn-%d error : %d\n", VencChn, s32Ret);
                    break;
                }
            }
#endif

            EI_TRACE_VENC(EI_DBG_INFO, "chn %d encoded_packet : %d\n", VencChn, encoded_packet[VencChn]);

            encoded_packet[VencChn]++;
            s32Ret = EI_MI_VENC_ReleaseStream(VencChn, &stStream);
            if (s32Ret != EI_SUCCESS) {
                EI_TRACE_VENC(EI_DBG_ERR, "release stream chn-%d error : %d\n", VencChn, s32Ret);
                break;
            }
        }
    }

#ifdef SAVE_OUTPUT_STREAM
    for (i = 0; i < pstAttrSampl->s32Cnt; i++) {
        VencChn = pstAttrSampl->VeChn[i];
        if (flip_out[VencChn] != NULL) {
            fclose(flip_out[VencChn]);
        }
    }
#endif
    //EI_TRACE_VENC(EI_DBG_INFO, "pstAttrSampl->bThreadStart exit\n");
    return NULL;
}


EI_S32 SAMPLE_COMM_VENC_StartGetStream(VC_CHN VeChn[], EI_S32 s32Cn)
{
    int i = 0;

    if (s32Cn < 0 || s32Cn > VC_MAX_CHN_NUM) {
        return EI_FAILURE;
    }

    gs_stPara.bThreadStart = EI_TRUE;

    for (i = 0; i < s32Cn; i++) {
        gs_stPara.VeChn[i] = VeChn[i];
    }

    gs_stPara.s32Cnt = s32Cn;
    return pthread_create(&gs_stPara.gs_VencPid, 0, SAMPLE_COMM_VENC_GetVencStreamProc,
            (EI_VOID *)&gs_stPara);
}

EI_VOID *SAMPLE_COMM_VENC_PollVencStreamProc(EI_VOID *p)
{
    SAMPLE_VENC_PARA_S *pstAttrSampl;
    //VENC_CHN_ATTR_S *pstVencChnAttr;
    VC_CHN VencChn;
    EI_S32 i;
    VENC_CHN_STATUS_S stStatus;
    VENC_STREAM_S stStream;
    EI_S32 s32Ret, encoded_packet[VC_MAX_CHN_NUM] = {0};
    FILE *flip_out[VC_MAX_CHN_NUM] = {NULL};
    EI_CHAR aszFileName[128];
    EI_CHAR szFilePostfix[10];
    //PAYLOAD_TYPE_E enPayload;
    pstAttrSampl = (SAMPLE_VENC_PARA_S *)p;
    EI_S32 chn_fds[VC_MAX_CHN_NUM] = {-1};
    EI_S32 fd = 0;
    EI_S32 fd_tmp;
    EI_S32 s32MaxFd = 0;
    fd_set read_fds;
    struct timeval TimeoutVal;

    //VencChn = pstAttrSampl->VeChn;
    //pstVencChnAttr = &pstAttrSampl->VeChnAttr;
    //enPayload = pstVencChnAttr->stVencAttr.enType;

    for (i = 0; i < pstAttrSampl->s32Cnt; i++) {
        VencChn = pstAttrSampl->VeChn[i];
        s32Ret = EI_MI_VENC_GetFd(VencChn, &fd);
        if (s32Ret != 0) {
            EI_TRACE_VENC(EI_DBG_ERR, " -- chn %d get fd error %d\n", VencChn, s32Ret);
        }
        if (s32MaxFd < fd)
            s32MaxFd = fd;
        chn_fds[VencChn] = fd;
        EI_TRACE_VENC(EI_DBG_ERR, " -- chn %d get fd %d\n", VencChn, fd);
    }

#ifdef SAVE_OUTPUT_STREAM
    for (i = 0; i < pstAttrSampl->s32Cnt; i++) {
        VencChn = pstAttrSampl->VeChn[i];

        s32Ret = SAMPLE_COMM_VENC_GetFilePostfix(pstAttrSampl->VeChnAttr[VencChn].stVencAttr.enType, szFilePostfix);
        if (s32Ret != EI_SUCCESS) {
            EI_TRACE_VENC(EI_DBG_ERR, "SAMPLE_COMM_VENC_GetFilePostfix failed with %#x!\n", s32Ret);
            return NULL;
        }

        if (PT_JPEG != pstAttrSampl->VeChnAttr[VencChn].stVencAttr.enType) {
            snprintf(aszFileName, 32, PATH_ROOT"/stream_chn%d%s", VencChn, szFilePostfix);

            flip_out[VencChn] = fopen(aszFileName, "wb");
            if (!flip_out[VencChn]) {
                EI_TRACE_VENC(EI_DBG_ERR, "open file[%s] failed!\n",
                    aszFileName);
                return NULL;
            }else {
                EI_TRACE_VENC(EI_DBG_INFO,"chn %d stream will save at %s\n", VencChn, aszFileName);
            }

        }
        memset(&szFilePostfix[0], 0, strlen(szFilePostfix));
        memset(&aszFileName[0]  , 0, strlen(aszFileName));
    }
#endif

    while (EI_TRUE == pstAttrSampl->bThreadStart) {

        FD_ZERO(&read_fds);
        for (i = 0; i < pstAttrSampl->s32Cnt; i++) {
            VencChn = pstAttrSampl->VeChn[i];
            FD_SET(chn_fds[VencChn], &read_fds);
        }

        TimeoutVal.tv_sec = 0;
        TimeoutVal.tv_usec = 10*1000;
        s32Ret = select(s32MaxFd + 1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0) {
            EI_TRACE_VENC(EI_DBG_ERR, "select error\n");
            break;
        } else if (s32Ret == 0) {
            ;//EI_TRACE_VENC(EI_DBG_ERR, "time out\n");
            continue;
        } else
            ;//EI_TRACE_VENC(EI_DBG_ERR, "read_fds : %x\n", read_fds);;


        for (i = 0; i < pstAttrSampl->s32Cnt; i++) {

            VencChn = pstAttrSampl->VeChn[i];
            fd_tmp = chn_fds[VencChn];

            if ((FD_ISSET(fd_tmp, &read_fds)) == 0)
                continue;

            EI_TRACE_VENC(EI_DBG_ERR, "read_fds2 : %x\n", read_fds);

            s32Ret = EI_MI_VENC_QueryStatus(VencChn, &stStatus);
            if (s32Ret != EI_SUCCESS) {
                EI_TRACE_VENC(EI_DBG_ERR, "query status chn-%d error : %d\n", VencChn, s32Ret);
                break;
            }

            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32CurPacks : %d\n",          stStatus.u32CurPacks);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftEncPics : %d\n",       stStatus.u32LeftEncPics);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftPics : %d\n",          stStatus.u32LeftPics);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftRecvPics : %d\n",      stStatus.u32LeftRecvPics);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftStreamBytes : %d\n",   stStatus.u32LeftStreamBytes);
            //EI_TRACE_VENC(EI_DBG_ERR, "stStatus.u32LeftStreamFrames : %d\n",  stStatus.u32LeftStreamFrames);

            s32Ret = EI_MI_VENC_GetStream(VencChn, &stStream, 0);

            if (s32Ret == EI_ERR_VENC_NOBUF) {
                EI_TRACE_VENC(EI_DBG_INFO, "No buffer\n");
                usleep(5 * 1000);
                continue;
            } else if (s32Ret != EI_SUCCESS) {
                EI_TRACE_VENC(EI_DBG_ERR, "get stream chn-%d error : %d\n", VencChn, s32Ret);
                break;
            }

            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.PackType : %d\n",              stStream.PackType);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.pu8Addr[0] : %x\n",    stStream.pstPack.pu8Addr[0]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.pu8Addr[1] : %x\n",    stStream.pstPack.pu8Addr[1]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u32PhyAddr[0] : %llx\n", stStream.pstPack.u64PhyAddr[0]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u32PhyAddr[1] : %llx\n", stStream.pstPack.u64PhyAddr[1]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u32Len[0] : %d\n",     stStream.pstPack.u32Len[0]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u32Len[1] : %d\n",     stStream.pstPack.u32Len[1]);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.enDataType : %d\n",    stStream.pstPack.enDataType);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.bFrameEnd : %d\n",     stStream.pstPack.bFrameEnd);
            // EI_TRACE_VENC(EI_DBG_ERR, "stStream.pstPack.u64PTS : %lld\n",      stStream.pstPack.u64PTS);

#ifdef SAVE_OUTPUT_STREAM
            if (PT_JPEG == pstAttrSampl->VeChnAttr[VencChn].stVencAttr.enType) {
                FILE *jpegout_file = NULL;
                EI_CHAR jpegFileName[128];
                snprintf(jpegFileName, 128, PATH_ROOT"/stream_chn%d_frame%d.jpeg", VencChn, encoded_packet[VencChn]);
                EI_TRACE_VENC(EI_DBG_ERR, "jpeg file : %s\n", jpegFileName);
                jpegout_file = fopen(jpegFileName, "wb");
                if (jpegout_file == NULL) {
                    EI_TRACE_VENC(EI_DBG_ERR, "open file[%s] failed!\n", jpegFileName);
                    return NULL;
                } else {
                    s32Ret = SAMPLE_COMM_VENC_SaveStream(&stStream, jpegout_file);
                    if (s32Ret != EI_SUCCESS) {
                        EI_MI_VENC_ReleaseStream(VencChn, &stStream);
                        EI_TRACE_VENC(EI_DBG_ERR, "save stream chn-%d error : %d\n", VencChn, s32Ret);
                        break;
                    }
                    fclose(jpegout_file);
                    jpegout_file = NULL;
                }
            } else {
                s32Ret = SAMPLE_COMM_VENC_SaveStream(&stStream, flip_out[VencChn]);
                if (s32Ret != EI_SUCCESS) {
                    EI_MI_VENC_ReleaseStream(VencChn, &stStream);
                    EI_TRACE_VENC(EI_DBG_ERR, "save stream chn-%d error : %d\n", VencChn, s32Ret);
                    break;
                }
            }
#endif

            EI_TRACE_VENC(EI_DBG_INFO, "chn %d encoded_packet : %d\n", VencChn, encoded_packet[VencChn]);

            encoded_packet[VencChn]++;
            s32Ret = EI_MI_VENC_ReleaseStream(VencChn, &stStream);
            if (s32Ret != EI_SUCCESS) {
                EI_TRACE_VENC(EI_DBG_ERR, "release stream chn-%d error : %d\n", VencChn, s32Ret);
                break;
            }
        }
    }

#ifdef SAVE_OUTPUT_STREAM
    for (i = 0; i < pstAttrSampl->s32Cnt; i++) {
        VencChn = pstAttrSampl->VeChn[i];
        if (flip_out[VencChn] != NULL) {
            fclose(flip_out[VencChn]);
        }
    }
#endif
    //EI_TRACE_VENC(EI_DBG_INFO, "pstAttrSampl->bThreadStart exit\n");
    return NULL;
}



EI_S32 SAMPLE_COMM_VENC_StartPollStream(VC_CHN VeChn[], EI_S32 s32Cn)
{
    int i = 0;

    if (s32Cn < 0 || s32Cn > VC_MAX_CHN_NUM) {
        return EI_FAILURE;
    }

    gs_stPara.bThreadStart = EI_TRUE;

    for (i = 0; i < s32Cn; i++) {
        gs_stPara.VeChn[i] = VeChn[i];
    }

    gs_stPara.s32Cnt = s32Cn;
    return pthread_create(&gs_stPara.gs_VencPid, 0, SAMPLE_COMM_VENC_PollVencStreamProc,
            (EI_VOID *)&gs_stPara);
}




EI_S32 SAMPLE_COMM_VENC_StopGetStream(void)
{

    if (EI_TRUE == gs_stPara.bThreadStart) {
        gs_stPara.bThreadStart = EI_FALSE;
        pthread_join(gs_stPara.gs_VencPid, 0);
    }
    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VENC_Stop(VC_CHN VencChn)
{
    EI_S32 s32Ret;

    s32Ret = EI_MI_VENC_StopRecvFrame(VencChn);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_VENC(EI_DBG_ERR, "EI_MI_VENC_StopRecvPic vechn[%d] failed with %#x!\n", \
            VencChn, s32Ret);
        return EI_FAILURE;
    }

    s32Ret = EI_MI_VENC_DestroyChn(VencChn);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_VENC(EI_DBG_ERR, "EI_MI_VENC_DestroyChn vechn[%d] failed with %#x!\n", \
            VencChn, s32Ret);
        return EI_FAILURE;
    }

    return EI_SUCCESS;
}


EI_VOID *SAMPLE_COMM_QpmapSendFrameProc(EI_VOID *p)
{
    SAMPLE_VENC_QPMAP_PARA_S *pstQpMapAttrSampl;
    VENC_CHN_ATTR_S *pstVencChnAttr;
    VC_CHN VencChn;
    IPPU_DEV IppuDev;
    IPPU_CHN IppuChn;
    EI_S32 i;
    EI_S32 s32Ret;
    //PAYLOAD_TYPE_E enPayload;

    pstQpMapAttrSampl = (SAMPLE_VENC_QPMAP_PARA_S *)p;

    while (EI_TRUE == pstQpMapAttrSampl->bThreadStart) {

        for (i = 0; i < pstQpMapAttrSampl->s32Cnt; i++) {
            VIDEO_FRAME_INFO_S stVFrameInfo = {0};
            EI_S32 width;
            EI_S32 height;
            EI_S32 pt_type;
            EI_S32 qp_map_size;
            USER_FRAME_INFO_S use_info;

            IppuDev = pstQpMapAttrSampl->IppuDev[i];
            IppuChn = pstQpMapAttrSampl->IppuChn[i];
            VencChn = pstQpMapAttrSampl->VeChn[i];
            pstVencChnAttr = &pstQpMapAttrSampl->VeChnAttr[i];

            //EI_TRACE_VENC(EI_DBG_ERR, "IppuDev:%d - IppuChn:%d - VencChn: %d\n", IppuDev, IppuChn, VencChn);

            if ((s32Ret = EI_MI_IPPU_GetChnFrame(IppuDev, IppuChn, &stVFrameInfo, 2000)) != EI_SUCCESS) {
                EI_TRACE_VENC(EI_DBG_ERR, "Get frame from ISP%d ch%d fail(0x%x)!\n", IppuDev, IppuChn, s32Ret);
                continue;
            } else {
                //EI_TRACE_VENC(EI_DBG_ERR, "Get frame from ISP%d ch%d sucess!\n", IppuDev, IppuChn);

                width = pstVencChnAttr->stVencAttr.u32PicWidth;
                height = pstVencChnAttr->stVencAttr.u32PicHeight;
                pt_type = pstVencChnAttr->stVencAttr.enType;

                if (pt_type == PT_H264)
                    qp_map_size = ((width + 15)>>4) * ((height + 15)>>4);
                else
                    qp_map_size = ((width + 63)>>6) * ((height + 63)>>6) * 4;

                use_info.stUserRcInfo.enMaptype = MAP_TYPE_QP;
                use_info.stUserFrame.u32BufferId = stVFrameInfo.u32BufferId;
                use_info.stUserRcInfo.u32QpMapSize = qp_map_size;

                s32Ret = EI_MI_MBASE_MemAlloc(&use_info.stUserRcInfo.u64QpMapPhyAddr,
                            (void **)&use_info.stUserRcInfo.pu8QpMapVirAddr, "sample vc qp map", NULL, qp_map_size);

                if ((s32Ret != 0) || (use_info.stUserRcInfo.pu8QpMapVirAddr == NULL)) {
                    EI_TRACE_VENC(EI_DBG_ERR, "malloc venc qp map buffer\n");
                } else {
                    SAMPLE_COMM_VENC_GenerateQpMapData(pt_type, width, height, use_info.stUserRcInfo.pu8QpMapVirAddr);
                    s32Ret = EI_MI_VENC_SendFrameEx(VencChn, &use_info, 100);
                    if (s32Ret != 0) {
                        EI_TRACE_VENC(EI_DBG_ERR, "send use info error error\n");
                        goto exit;
                    }
                    s32Ret = EI_MI_MBASE_MemFree(use_info.stUserRcInfo.u64QpMapPhyAddr, (void *)use_info.stUserRcInfo.pu8QpMapVirAddr);
                    if (s32Ret != 0) {
                        EI_TRACE_VENC(EI_DBG_ERR, "free qp buffer error");
                    }
                }
                EI_MI_IPPU_ReleaseChnFrame(IppuDev, IppuChn, &stVFrameInfo);
            }
        }
    }
exit:
    return NULL;
}


EI_S32 SAMPLE_COMM_VENC_QpmapSendFrame(IPPU_DEV IppuDev[], IPPU_CHN IppuChn[], VC_CHN VencChn[], EI_S32 s32Cn)
{
    EI_S32 i;

    if (s32Cn < 0 || s32Cn > VC_MAX_CHN_NUM) {
        return EI_FAILURE;
    }

    gs_stQpMapPara.bThreadStart   = EI_TRUE;
    gs_stQpMapPara.s32Cnt         = s32Cn;

    for (i = 0; i < s32Cn; i++) {
        gs_stQpMapPara.IppuDev[i] = IppuDev[i];
        gs_stQpMapPara.IppuChn[i] = IppuChn[i];
        gs_stQpMapPara.VeChn[i]   = VencChn[i];
    }

     return pthread_create(&gs_stQpMapPara.gs_VencQpMapPid, 0, SAMPLE_COMM_QpmapSendFrameProc,
                (EI_VOID *)&gs_stQpMapPara);
}



EI_VOID SAMPLE_COMM_VENC_StopSendQpmapFrame()
{
    if (EI_TRUE == gs_stQpMapPara.bThreadStart) {
        gs_stQpMapPara.bThreadStart = EI_FALSE;
        pthread_join(gs_stQpMapPara.gs_VencQpMapPid, 0);
    }
}


EI_S32 SAMPLE_COMM_VENC_ReadOneFrame(PIXEL_FORMAT_E input_mode, VIDEO_FRAME_INFO_S *frame_info, unsigned char *y_data,
                                    unsigned char *u_data, unsigned char *v_data, FILE *input_flip)
{
    int i = 0;
    int read_len = 0;
    unsigned char *y_tmp = y_data;
    unsigned char *u_tmp = u_data;
    unsigned char *v_tmp = v_data;
    unsigned int stride_y, stride_u, stride_v, width, height;

    stride_y =  frame_info->stVFrame.u32PlaneStride[0];
    stride_u = frame_info->stVFrame.u32PlaneStride[1];
    stride_v = frame_info->stVFrame.u32PlaneStride[2];
    width = frame_info->stCommFrameInfo.u32Width;
    height = frame_info->stCommFrameInfo.u32Height;

    if (stride_y != width) {
        for (i = 0; i < height; i++) {
            read_len = fread(y_tmp, 1, width, input_flip);
            if (read_len != width) {
                EI_TRACE_VENC(EI_DBG_ERR, "file end 1\n");
                return -1;
            }
            y_tmp += stride_y;
        }
    } else {
        read_len = fread(y_tmp, 1, width * height, input_flip);
        if (read_len != width * height) {
            EI_TRACE_VENC(EI_DBG_ERR, "file end 1\n");
            return -1;
        }
    }


    if ((input_mode == PIX_FMT_YUV_PLANAR_420) || (input_mode == PIX_FMT_YVU_PLANAR_420)) {
        if (stride_y != width) {
            for (i = 0; i < height / 2; i++) {
                read_len = fread(u_tmp, 1, width / 2, input_flip);
                if (read_len != width / 2) {
                    EI_TRACE_VENC(EI_DBG_ERR, "file end 2\n");
                    return -1;
                }
                u_tmp += stride_u;
            }
            for (i = 0; i < height / 2; i++) {
                read_len = fread(v_tmp, 1, width / 2, input_flip);
                if (read_len != width / 2) {
                    EI_TRACE_VENC(EI_DBG_ERR, "file end 3\n");
                    return -1;
                }
                v_tmp += stride_v;
            }
        } else {
            read_len = fread(u_tmp, 1, height * width / 4, input_flip);
            if (read_len != height * width / 4) {
                EI_TRACE_VENC(EI_DBG_ERR, "file end 2\n");
                return -1;
            }

            read_len = fread(v_tmp, 1, height * width / 4, input_flip);
            if (read_len != height * width / 4) {
                EI_TRACE_VENC(EI_DBG_ERR, "file end 2\n");
                return -1;
            }
        }

    } else if ((input_mode == PIX_FMT_YUV_SEMIPLANAR_420) || (input_mode == PIX_FMT_YVU_SEMIPLANAR_420)) {
        if (stride_y != width) {
            for (i = 0; i < height / 2; i++) {
                read_len = fread(u_tmp, 1, width, input_flip);
                if (read_len != width) {
                    EI_TRACE_VENC(EI_DBG_ERR, "file end 2\n");
                    return -1;
                }
                u_tmp += (stride_u);
            }
        } else {
            read_len = fread(u_tmp, 1, width * height / 2, input_flip);
            if (read_len != width * height / 2) {
                EI_TRACE_VENC(EI_DBG_ERR, "file end 2\n");
                return -1;
            }
        }
    } else {
        EI_TRACE_VENC(EI_DBG_ERR, "not support inputformat\n");
        return -1;
    }
    return 0;
}





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
