/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_vdec.c
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
#include "mi_vdec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

//#define TEST_USER_PIC

typedef enum eiVB_SOURCE_E
{
    VB_SOURCE_COMMON  = 0,
    VB_SOURCE_MODULE  = 1,
    VB_SOURCE_PRIVATE = 2,
    VB_SOURCE_USER    = 3,
    VB_SOURCE_BUTT
} VB_SOURCE_E;

#define SAVE_GET_FRAME

VB_SOURCE_E  g_enVdecVBSource  = VB_SOURCE_USER;
VBUF_POOL Pool;
MDP_CHN_S stSrcChn;
MDP_CHN_S stSinkChn;

EI_U32 SAMPLE_COMM_VDEC_SCALE_Init(SAMPLE_VDEC_CONFIG_S *pstSamplVdec)
{
    EI_U32 src_width, src_height;
    EI_U32 dst_width, dst_height;// , dst_align;
    EI_FLOAT ratio_w, ratio_h, ratio;

    src_width   = pstSamplVdec->u32Width;
    src_height  = pstSamplVdec->u32Height;
    // dst_align   = pstSamplVdec->scaleInfo.u32Align;
    dst_width   = pstSamplVdec->scaleInfo.u32MaxWidth;
    dst_height  = pstSamplVdec->scaleInfo.u32MaxHeight;

    // EI_TRACE(EI_ID_JVC, EI_DBG_ERR, "src_width:%d, src_height:%d\n", src_width, src_height);
    // EI_TRACE(EI_ID_JVC, EI_DBG_ERR, "max_width:%d, max_height:%d\n", dst_width, dst_height);

    if(src_width != dst_width || src_height != dst_height)
    {
        ratio_w = (EI_FLOAT)src_width / (EI_FLOAT)dst_width;
        ratio_h = (EI_FLOAT)src_height / (EI_FLOAT)dst_height;
        EI_TRACE(EI_ID_JVC, EI_DBG_ERR, "ratio_w:%.2f, ratio_h:%.2f\n", ratio_w, ratio_h);
        if(ratio_w > ratio_h)
            ratio = ratio_w;
        else
            ratio = ratio_h;
    } else {
        ratio = 1;
    }
    // EI_TRACE(EI_ID_JVC, EI_DBG_ERR, "ratio:%.2f\n", ratio);

    dst_width   = ceil((src_width / ratio));
    dst_height  = ceil((src_height / ratio));
    pstSamplVdec->scaleInfo.u32MaxWidth   = EI_ALIGN_DOWN(dst_width, 2);
    pstSamplVdec->scaleInfo.u32MaxHeight  = EI_ALIGN_DOWN(dst_height, 2);

    EI_TRACE(EI_ID_JVC, EI_DBG_ERR, "dst_width:%d, dst_height:%d\n", dst_width, dst_height);

    return 0;
}

EI_S32 SAMPLE_COMM_VDEC_Start(VC_CHN s32ChnNum, SAMPLE_VDEC_CONFIG_S *pstSamplVdec)
{
    EI_S32  s32Ret;
    VDEC_CHN_ATTR_S stChnAttr;

    stChnAttr.enType           = pstSamplVdec->enType;
    stChnAttr.enMode           = pstSamplVdec->enMode;
    stChnAttr.enPixelFormat    = pstSamplVdec->enPixelFormat;
    stChnAttr.u32Align         = pstSamplVdec->u32Align;
    stChnAttr.u32Width         = pstSamplVdec->u32Width;
    stChnAttr.u32Height        = pstSamplVdec->u32Height;
    stChnAttr.u32StreamBufSize = 1024 * 1024;
    stChnAttr.u32FrameBufCnt   = pstSamplVdec->u32FrameBufCnt;
    stChnAttr.enRemapMode      = VBUF_REMAP_MODE_CACHED;

    stChnAttr.stVdecVideoAttr.stVdecScale.u32Align      = pstSamplVdec->scaleInfo.u32Align;
    stChnAttr.stVdecVideoAttr.stVdecScale.u32Width      = pstSamplVdec->scaleInfo.u32MaxWidth;
    stChnAttr.stVdecVideoAttr.stVdecScale.u32Height     = pstSamplVdec->scaleInfo.u32MaxHeight;
    stChnAttr.stVdecVideoAttr.stVdecScale.bFixedRatio   = pstSamplVdec->scaleInfo.bFixedRatio;
    stChnAttr.stVdecVideoAttr.stVdecScale.bScaleEnable  = pstSamplVdec->scaleInfo.bScaleEnable;

    if (PT_H264 == pstSamplVdec->enType || PT_H265 == pstSamplVdec->enType)
    {
        ;
    }
    else if (PT_JPEG == pstSamplVdec->enType || PT_MJPEG == pstSamplVdec->enType)
    {
        ;
    }

    s32Ret = EI_MI_VDEC_CreateChn(s32ChnNum, &stChnAttr);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_VENC(EI_DBG_ERR, "EI_MI_VDEC_CreateChn faild with%#x! \n", s32Ret);
        return EI_FAILURE;
    }

    s32Ret = EI_MI_VDEC_StartRecvStream(s32ChnNum);
    if (EI_SUCCESS != s32Ret) {
        EI_TRACE_VENC(EI_DBG_ERR, "EI_MI_VDEC_StartRecvStream faild with%#x! \n", s32Ret);
        return EI_FAILURE;
    }

    return EI_SUCCESS;
}

EI_VOID *SAMPLE_COMM_VDEC_SendStream(EI_VOID *pArgs)
{
    SAMPLE_VDEC_THREAD_PARAM_S *pstVdecThreadParam =(SAMPLE_VDEC_THREAD_PARAM_S *)pArgs;
    EI_BOOL bEndOfStream = EI_FALSE;
    EI_S32 s32UsedBytes = 0, s32ReadLen = 0;
    FILE *fpStrm=NULL;
    // FILE *fpUserStrm=NULL;
    EI_U8 *pu8Buf = NULL;
    // EI_U8 *pu8UserBuf = NULL;
    VDEC_STREAM_S stStream;
    EI_BOOL bFindStart;
    // EI_BOOL bFindEnd;
    EI_U64 u64PTS = 0;
    EI_U32 u32Len, u32Start;
    EI_S32 s32Ret, i;
    EI_CHAR cStreamFile[256];
    // EI_CHAR cUserStreamFile[256];
    // VBUF_BUFFER buffer_id;
    // VIDEO_FRAME_INFO_S stUserPic;
    // VDEC_USERDATA_S stUserData;
    pstVdecThreadParam->frame_cnt = 0;

    sprintf(cStreamFile, "%s/%s", pstVdecThreadParam->cFilePath,pstVdecThreadParam->cFileName);
    if(cStreamFile != 0)
    {
        fpStrm = fopen(cStreamFile, "rb");
        if(fpStrm == NULL)
        {
            EI_TRACE_VDEC(EI_DBG_DEBUG, "chn %d can't open file %s in send stream thread!\n", pstVdecThreadParam->s32ChnId, cStreamFile);
            return NULL;
        }
    }
    EI_TRACE_VDEC(EI_DBG_DEBUG, "\n \033[0;36m chn %d, stream file:%s, userbufsize: %d \033[0;39m\n", pstVdecThreadParam->s32ChnId,
        pstVdecThreadParam->cFileName, pstVdecThreadParam->s32MinBufSize);

    pu8Buf = malloc(pstVdecThreadParam->s32MinBufSize);
    if(pu8Buf == NULL)
    {
        EI_TRACE_VDEC(EI_DBG_DEBUG, "chn %d can't alloc %d in send stream thread!\n", pstVdecThreadParam->s32ChnId, pstVdecThreadParam->s32MinBufSize);
        fclose(fpStrm);
        return NULL;
    }

    fflush(stdout);

    u64PTS = pstVdecThreadParam->u64PtsInit;
    pstVdecThreadParam->bCircleSend = EI_FALSE;
    while (1)
    {
        bEndOfStream = EI_FALSE;
        bFindStart   = EI_FALSE;
        // bFindEnd     = EI_FALSE;
        u32Start     = 0;
#ifdef TEST_USER_PIC
        if(pstVdecThreadParam->frame_cnt == 10) {
            sprintf(cUserStreamFile, "%s/%s", pstVdecThreadParam->cUserFilePath,pstVdecThreadParam->cUserFileName);
            if(cUserStreamFile != 0)
            {
                fpUserStrm = fopen(cUserStreamFile, "rb");
                if(fpUserStrm == NULL)
                {
                    EI_TRACE_VDEC(EI_DBG_DEBUG, "chn %d can't open file %s in send stream thread!\n", pstVdecThreadParam->s32ChnId, cUserStreamFile);
                    return NULL;
                }
            }
            EI_TRACE_VDEC(EI_DBG_DEBUG, "\n \033[0;36m chn %d, stream file:%s, userbufsize: %d \033[0;39m\n", pstVdecThreadParam->s32ChnId,
                pstVdecThreadParam->cUserFileName, pstVdecThreadParam->s32MinBufSize);

            pu8UserBuf = malloc(pstVdecThreadParam->s32MinBufSize);
            if(pu8UserBuf == NULL)
            {
                EI_TRACE_VDEC(EI_DBG_DEBUG, "chn %d can't alloc %d in send stream thread!\n", pstVdecThreadParam->s32ChnId, pstVdecThreadParam->s32MinBufSize);
                fclose(fpUserStrm);
                return NULL;
            }
            s32ReadLen = fread(pu8UserBuf, 1, pstVdecThreadParam->s32MinBufSize, fpUserStrm);
            EI_TRACE_VDEC(EI_DBG_DEBUG, "test s32ReadLen = %d **************\n", s32ReadLen);
            if (s32ReadLen == 0)
            {
                EI_TRACE_VDEC(EI_DBG_DEBUG, "Read File Len = %d **************\n", s32ReadLen);
            }
            stUserData.pu8Addr = pu8UserBuf;
            stUserData.u32Len  = s32ReadLen;
            s32Ret = EI_MI_VDEC_GetUserData(pstVdecThreadParam->s32ChnId, &stUserData, 1000);
            if(s32Ret != EI_SUCCESS)
            {
                EI_TRACE_VDEC(EI_DBG_DEBUG, "GetUserData VDEC fail for %#d!\n", s32Ret);
                return NULL;
            }
            s32Ret = EI_MI_VDEC_EnableUserPic(pstVdecThreadParam->s32ChnId, 0);/* 0:delay insert user pic*/
            if(s32Ret != EI_SUCCESS)
            {
                EI_TRACE_VDEC(EI_DBG_DEBUG, "EnableUserPic VDEC fail for %#d!\n", s32Ret);
                return NULL;
            }
            pstVdecThreadParam->frame_cnt++;
            //EI_MI_VDEC_DisableUserPic(pstVdecThreadParam->s32ChnId);// interface line 374
            /*EI_MI_VDEC_SetUserPic(0, &stUserPic); */
            s32Ret = EI_MI_VDEC_ReleaseUserData(pstVdecThreadParam->s32ChnId, &stUserData);
            if(s32Ret != EI_SUCCESS)
            {
                EI_TRACE_VDEC(EI_DBG_DEBUG, "ReleaseUserData VDEC fail for %#d!", s32Ret);
                return NULL;
            }
            if (pu8UserBuf != NULL)
            {
                free(pu8UserBuf);
                EI_TRACE_VDEC(EI_DBG_DEBUG, "free pu8UserBuf ok.\n");
            }
            if(fpUserStrm != NULL)
            {
                fclose(fpUserStrm);
            }
            EI_TRACE_VDEC(EI_DBG_DEBUG, "----------- TEST_USER_PIC end ----------------\n");
        }
#endif
        fseek(fpStrm, s32UsedBytes, SEEK_SET);
        s32ReadLen = fread(pu8Buf, 1, pstVdecThreadParam->s32MinBufSize, fpStrm);
        EI_TRACE_VDEC(EI_DBG_DEBUG, "test s32ReadLen = %d **************\n", s32ReadLen);
        if (s32ReadLen == 0)
        {
            pstVdecThreadParam->bCircleSend = EI_FALSE;
            EI_TRACE_VDEC(EI_DBG_DEBUG, "read len == 0 **************\n");
            if (pstVdecThreadParam->bCircleSend == EI_TRUE)
            {
                memset(&stStream, 0, sizeof(VDEC_STREAM_S) );
                stStream.bEndOfStream = EI_TRUE;
                EI_MI_VDEC_SendStream(pstVdecThreadParam->s32ChnId, &stStream, -1);

                s32UsedBytes = 0;
                fseek(fpStrm, 0, SEEK_SET);
                s32ReadLen = fread(pu8Buf, 1, pstVdecThreadParam->s32MinBufSize, fpStrm);
            } else {
                EI_TRACE_VDEC(EI_DBG_DEBUG, "break **************\n");
                break;
            }
        }
        if (pstVdecThreadParam->s32StreamMode==VIDEO_MODE_FRAME && pstVdecThreadParam->enType == PT_H264) {

        }
        else if (pstVdecThreadParam->s32StreamMode==VIDEO_MODE_FRAME
            && pstVdecThreadParam->enType == PT_H265) {

        }
        else if (pstVdecThreadParam->enType == PT_MJPEG || pstVdecThreadParam->enType == PT_JPEG) {
            for (i=0; i<s32ReadLen-1; i++)
            {
                if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD8)
                {
                    u32Start = i;
                    bFindStart = EI_TRUE;
                    i = i + 2;
                    break;
                }
            }

            for (; i<s32ReadLen-3; i++)
            {
                if ((pu8Buf[i] == 0xFF) && (pu8Buf[i+1]& 0xF0) == 0xE0)
                {
                     u32Len = (pu8Buf[i+2]<<8) + pu8Buf[i+3];
                     i += 1 + u32Len;
                }
                else
                {
                    break;
                }
            }

            for (; i<s32ReadLen-1; i++)
            {
                if (pu8Buf[i] == 0xFF && pu8Buf[i+1] == 0xD9)
                {
                    // bFindEnd = EI_TRUE;
                    break;
                }
            }
            s32ReadLen = i+2;

            if (bFindStart == EI_FALSE)
            {
                EI_TRACE_VDEC(EI_DBG_DEBUG, "chn %d can not find JPEG start code!s32ReadLen %d, s32UsedBytes %d.!\n",
                    pstVdecThreadParam->s32ChnId, s32ReadLen, s32UsedBytes);
            }
        }
        else
        {
            if((s32ReadLen != 0) && (s32ReadLen < pstVdecThreadParam->s32MinBufSize))
            {
                bEndOfStream = EI_TRUE;
            }
        }
        stStream.u64PTS       = u64PTS;
        stStream.pu8Addr      = pu8Buf + u32Start;
        stStream.u32Len       = s32ReadLen;
        stStream.bEndOfFrame  = (pstVdecThreadParam->s32StreamMode==VIDEO_MODE_FRAME)? EI_TRUE: EI_FALSE;
        stStream.bEndOfStream = bEndOfStream;
        stStream.bDisplay     = 1;
        stStream.bFlushBuffer = EI_TRUE;
SendAgain:
        s32Ret = EI_MI_VDEC_SendStream(pstVdecThreadParam->s32ChnId, &stStream, pstVdecThreadParam->s32MilliSec);
        if( (EI_FAILURE == s32Ret) && (THREAD_CTRL_START == pstVdecThreadParam->eThreadCtrl) )
        {
            usleep(pstVdecThreadParam->s32IntervalTime);
            goto SendAgain;
        }
        else
        {
            if(s32Ret == EI_SUCCESS)
                pstVdecThreadParam->frame_cnt++;
            bEndOfStream = EI_FALSE;
            s32UsedBytes = s32UsedBytes +s32ReadLen + u32Start;
            u64PTS += pstVdecThreadParam->u64PtsIncrease;
            EI_TRACE_VDEC(EI_DBG_DEBUG, "test frame_cnt = %d u64PTS %lld**************\n", pstVdecThreadParam->frame_cnt, u64PTS);
        }
        usleep(pstVdecThreadParam->s32IntervalTime);
    }
    /* send the flag of stream end */
    memset(&stStream, 0, sizeof(VDEC_STREAM_S) );
    stStream.bEndOfStream = EI_TRUE;
    pstVdecThreadParam->eThreadCtrl = THREAD_CTRL_STOP;
    //EI_MI_VDEC_SendStream(pstVdecThreadParam->s32ChnId, &stStream, -1);

    EI_TRACE_VDEC(EI_DBG_DEBUG, "\033[0;35m chn %d send steam thread return ...  \033[0;39m\n", pstVdecThreadParam->s32ChnId);
    fflush(stdout);
    if (pu8Buf != EI_NULL)
    {
        free(pu8Buf);
    }
    fclose(fpStrm);
    return (EI_VOID *)EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_VDEC_StartSendStream(EI_S32 s32ChnNum, SAMPLE_VDEC_THREAD_PARAM_S *pstVdecSend, pthread_t *pVdecThread)
{
    pVdecThread[s32ChnNum] = 0;
    pthread_create(&pVdecThread[s32ChnNum], 0, SAMPLE_COMM_VDEC_SendStream, (EI_VOID *)&pstVdecSend[s32ChnNum]);
    return 0;
}

EI_S32 SAMPLE_COMM_VDEC_StartGetFrame(EI_S32 s32ChnNum,SAMPLE_VDEC_THREAD_PARAM_S *pstVdecSend, pthread_t *pVdecThread)
{
    EI_S32 s32Ret, buffer_cnt;
    VDEC_CHN_STATUS_S stStatus;
    VIDEO_FRAME_INFO_S frame_info;
#ifdef SAVE_GET_FRAME
    char *y_data, *u_data, *v_data;
#endif

    buffer_cnt = 0;

#ifdef SAVE_GET_FRAME
    char file_name[100];
    sprintf((char *)file_name, "/data/mdp_jpeg_dec.yuv");
    FILE *flip_tmp = NULL;

    EI_TRACE_VDEC(EI_DBG_ERR, "file_name : %s\n", file_name);
    flip_tmp = fopen(file_name, "wb");
    if (flip_tmp == NULL) {
        EI_TRACE_VDEC(EI_DBG_ERR, "open file %s error\n", file_name);
        return -1;
    }
#endif

    while(1)
    {
        EI_MI_VDEC_QueryStatus(pstVdecSend[s32ChnNum].s32ChnId, &stStatus);
        if (stStatus.u32LeftPics == 0) {
            usleep(5 * 1000);
            continue;
        }

        s32Ret = EI_MI_VDEC_GetFrame(pstVdecSend[s32ChnNum].s32ChnId, &frame_info, 1000);
        if(s32Ret != EI_SUCCESS){
            EI_TRACE_VDEC(EI_DBG_INFO, "get output buffer error\n");
            continue;
        }

#ifdef SAVE_GET_FRAME
        EI_TRACE_VENC(EI_DBG_ERR, "u32TotalSize : %d\n", frame_info.stVFrame.u32TotalSize);
        EI_TRACE_VENC(EI_DBG_ERR, "u32PlaneSize : %d\n", frame_info.stVFrame.u32PlaneSize);
        EI_TRACE_VENC(EI_DBG_ERR, "u32PlaneStride : %d\n", frame_info.stVFrame.u32PlaneStride);
        EI_TRACE_VENC(EI_DBG_ERR, "u32PlaneSizeValid : %d\n", frame_info.stVFrame.u32PlaneSizeValid);

        y_data =
            (char *)EI_MI_MBASE_MmapCache(frame_info.stVFrame.u64PlanePhyAddr[0], frame_info.stVFrame.u32PlaneStride[0]);
        u_data =
            (char *)EI_MI_MBASE_MmapCache(frame_info.stVFrame.u64PlanePhyAddr[1], frame_info.stVFrame.u32PlaneStride[1]);
        v_data =
            (char *)EI_MI_MBASE_MmapCache(frame_info.stVFrame.u64PlanePhyAddr[2], frame_info.stVFrame.u32PlaneStride[2]);

        fwrite(y_data, 1, frame_info.stVFrame.u32PlaneSizeValid[0], flip_tmp);
        fwrite(u_data, 1, frame_info.stVFrame.u32PlaneSizeValid[1], flip_tmp);
        fwrite(v_data, 1, frame_info.stVFrame.u32PlaneSizeValid[2], flip_tmp);
#endif
        buffer_cnt++;
        s32Ret = EI_MI_VDEC_ReleaseFrame(pstVdecSend[s32ChnNum].s32ChnId, &frame_info);
        if (s32Ret != 0){
            EI_TRACE_VDEC(EI_DBG_DEBUG, "release error\n");
            continue;
        }

#ifdef SAVE_GET_FRAME
        EI_MI_MBASE_Munmap((void *)y_data, frame_info.stVFrame.u32TotalSize);
        EI_MI_MBASE_Munmap((void *)u_data, frame_info.stVFrame.u32TotalSize);
        EI_MI_MBASE_Munmap((void *)v_data, frame_info.stVFrame.u32TotalSize);
        EI_TRACE_VDEC(EI_DBG_ERR, "decode cnt:%d  rece cnt: %d,send cnt:%d\n",
            stStatus.u32DecodeStreamFrames, buffer_cnt, pstVdecSend[s32ChnNum].frame_cnt);
#endif
        if(pstVdecSend[s32ChnNum].eThreadCtrl == THREAD_CTRL_STOP &&
           buffer_cnt >= pstVdecSend[s32ChnNum].frame_cnt)
        {
            EI_TRACE_VDEC(EI_DBG_ERR, "*getframe cnt:%d ,send packet:%d THREAD_CTRL_STOP EXIT!\n",
                                                buffer_cnt, pstVdecSend[s32ChnNum].frame_cnt-1);
            break;
        }

    }
    // fclose(flip_tmp);
    return -1;
}

EI_S32 SAMPLE_COMM_VDEC_StopSendStream(EI_S32 s32ChnNum, SAMPLE_VDEC_THREAD_PARAM_S *pstVdecSend, pthread_t *pVdecThread)
{
    EI_S32 s32Ret;
    s32Ret = EI_MI_VDEC_StopRecvStream(s32ChnNum);
    if(s32Ret != EI_SUCCESS){
        EI_TRACE_VDEC(EI_DBG_DEBUG, "EI_MI_VDEC_DestroyChn failed\n");
    }
    if(0 != pVdecThread[s32ChnNum])
    {
        pthread_join(pVdecThread[s32ChnNum], EI_NULL);
        pVdecThread[s32ChnNum] = 0;
    }
    else {
        EI_TRACE_VDEC(EI_DBG_DEBUG, "SAMPLE_COMM_VDEC_StopSendStream FAILURE\n");
        return EI_FAILURE;
    }
    s32Ret = EI_MI_VDEC_DestroyChn(s32ChnNum);
    if(s32Ret != EI_SUCCESS){
        EI_TRACE_VDEC(EI_DBG_DEBUG, "EI_MI_VDEC_DestroyChn failed\n");
    }
    EI_TRACE_VDEC(EI_DBG_DEBUG, "SAMPLE_COMM_VDEC_StopSendStream SUCCESS\n");
    return EI_SUCCESS;
}
