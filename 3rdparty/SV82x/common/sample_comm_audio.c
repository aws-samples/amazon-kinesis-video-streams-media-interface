/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_audio.c
 * @Date      :    2021-7-7
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Source file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#include "ei_posix.h"

#include "sample_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

//#define COMM_SAVE_PCM_FILE

#define AUDIO_ADPCM_TYPE ADPCM_TYPE_DVI4
#define G726_BPS MEDIA_G726_40K

typedef struct tagSAMPLE_AENC_S {
    EI_BOOL bStart;
    EI_BOOL bStartIn;
    pthread_t stAencPid;
    pthread_t stAencPidIn;
    EI_S32  AeChn;
    EI_S32  AdChn;
    FILE    *pfd;
    FILE *pfdIn;
    EI_BOOL bSendAdChn;
} SAMPLE_AENC_S;

typedef struct tagSAMPLE_AI_S {
    EI_BOOL bStart;
    EI_S32  AiDev;
    EI_S32  AiChn;
    EI_S32  AencChn;
    EI_S32  AoDev;
    EI_S32  AoChn;
    EI_BOOL bSendAenc;
    EI_BOOL bSendAo;
    pthread_t stAiPid;

#ifdef COMM_SAVE_PCM_FILE
    FILE    *pTestFile;
#endif
} SAMPLE_AI_S;

typedef struct tagSAMPLE_ADEC_S {
    EI_BOOL bStart;
    EI_BOOL bStartOut;
    EI_S32 AdChn;
    EI_S32  AoDev;
    EI_S32  AoChn;
    EI_BOOL bSendAo;
    FILE *pfd;
    FILE *pfdOut;
    pthread_t stAdPid;
    pthread_t stAdPidout;
} SAMPLE_ADEC_S;

typedef struct tagSAMPLE_AO_S {
    AUDIO_DEV AoDev;
    EI_BOOL bStart;
    pthread_t stAoPid;
} SAMPLE_AO_S;

static SAMPLE_AI_S   gs_stSampleAi[AI_MAX_DEV_NUM * AI_MAX_CHN_NUM];
//static SAMPLE_AENC_S gs_stSampleAenc[AENC_MAX_CHN_NUM];
static SAMPLE_AENC_S gs_stSampleFileAenc[AENC_MAX_CHN_NUM];
static SAMPLE_ADEC_S gs_stSampleAdec[ADEC_MAX_CHN_NUM];
static SAMPLE_ADEC_S gs_stSampleFileAdec[ADEC_MAX_CHN_NUM];
static SAMPLE_AENC_S gs_stSampleAencAdec[AENC_MAX_CHN_NUM];
static SAMPLE_AO_S   gs_stSampleAo[AO_MAX_DEV_NUM];


void *SAMPLE_COMM_AUDIO_AiProc(void *parg)
{
    EI_S32 s32Ret;
    SAMPLE_AI_S *pstAiCtl = (SAMPLE_AI_S *)parg;
    AUDIO_FRAME_S stFrame;
    AEC_FRAME_S   stAecFrm;
    EI_S32 AiFd = 0;
    fd_set read_fds;
    struct timeval TimeoutVal;
    AI_CHN_PARAM_S stAiChnPara;

    s32Ret = EI_MI_AI_GetChnParam(pstAiCtl->AiDev, pstAiCtl->AiChn, &stAiChnPara);
    if (EI_SUCCESS != s32Ret) {
        printf("%s: Get ai chn param failed\n", __FUNCTION__);
        return NULL;
    }

    stAiChnPara.u32UsrFrmDepth = 30;

    s32Ret = EI_MI_AI_SetChnParam(pstAiCtl->AiDev, pstAiCtl->AiChn, &stAiChnPara);
    if (EI_SUCCESS != s32Ret) {
        printf("%s: set ai chn param failed\n", __FUNCTION__);
        return NULL;
    }

    FD_ZERO(&read_fds);
    AiFd = EI_MI_AI_GetFd(pstAiCtl->AiDev, pstAiCtl->AiChn);
    FD_SET(AiFd, &read_fds);

#ifdef TEST_SAVE_FILE
    AUDIO_SAVE_FILE_INFO_S stSaveFileInfo = {0};
    AUDIO_FILE_STATUS_S stFileStatus = {0};
    stFileStatus.bSaving = EI_TRUE;
    strcpy(stSaveFileInfo.aFileName, "default");
    strcpy(stSaveFileInfo.aFilePath, "/data");
    stSaveFileInfo.bCfg = EI_TRUE;
    stSaveFileInfo.u32FileSize = 1024;
    printf("File path:%s, file name:%s, file size:%d*1024 bytes\n\n",
        stSaveFileInfo.aFilePath, stSaveFileInfo.aFileName, stSaveFileInfo.u32FileSize);
    s32Ret = EI_MI_AI_SaveFile(pstAiCtl->AiDev, pstAiCtl->AiChn, &stSaveFileInfo);
    if (s32Ret != EI_SUCCESS) {
        printf("EI_MI_AI_SaveFile() error,ret=%x!!!!\n\n", s32Ret);
        return -1;
    }
#endif
    while (pstAiCtl->bStart) {
        TimeoutVal.tv_sec = 1;
        TimeoutVal.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(AiFd, &read_fds);

        s32Ret = select(AiFd + 1, &read_fds, NULL, NULL, &TimeoutVal);
        if (s32Ret < 0) {
            break;
        } else if (0 == s32Ret) {
            printf("%s: get ai frame select time out\n", __FUNCTION__);
            break;
        }

        if (FD_ISSET(AiFd, &read_fds)) {
            memset(&stAecFrm, 0, sizeof(AEC_FRAME_S));
            s32Ret = EI_MI_AI_GetFrame(pstAiCtl->AiDev, pstAiCtl->AiChn, &stFrame, &stAecFrm, 100);
            if (EI_SUCCESS != s32Ret) {
                usleep(5000);
                printf("FUN: %s line:%d len %d,RET:%d!\n", __FUNCTION__, __LINE__, stFrame.u32Len, s32Ret);
                continue;
            }
            //printf("#####file:%s,line:%d:EI_MI_AI_GetFrame len %d!\n", __FILE__, __LINE__, stFrame.u32Len);

            if (EI_TRUE == pstAiCtl->bSendAo) {
                s32Ret = EI_MI_AO_SendFrame(pstAiCtl->AoDev, pstAiCtl->AoChn, &stFrame, 1000);
                if (EI_SUCCESS != s32Ret) {
                    printf("%s: EI_MI_AO_SendFrame(%d, %d), failed with %#x!\n", \
                        __FUNCTION__, pstAiCtl->AoDev, pstAiCtl->AoChn, s32Ret);
                    pstAiCtl->bStart = EI_FALSE;
                    return NULL;
                }
            }

#ifdef TEST_SAVE_FILE
            if (stFileStatus.bSaving) {
                s32Ret = EI_MI_AI_QueryFileStatus(pstAiCtl->AiDev, pstAiCtl->AiChn, &stFileStatus);
                if (EI_SUCCESS != s32Ret) {
                    printf("Error save file\n");
                    break;
                }
                printf("Saving file now, please wait.\n");
            }
#endif

#ifdef COMM_SAVE_PCM_FILE
            if (pstAiCtl->pTestFile &&
                fwrite(stFrame.u64VirAddr, 1, stFrame.u32Len, pstAiCtl->pTestFile) != stFrame.u32Len) {
                printf("Error save sample file\n");
                break;
            }
#endif

            if (EI_TRUE == pstAiCtl->bSendAenc) {
                //printf("file:%s,line:%d:EI_MI_AI_GetFrame len %d!\n", __FILE__,__LINE__,stFrame.u32Len);
                s32Ret = EI_MI_AENC_SendFrame(pstAiCtl->AencChn, &stFrame, &stAecFrm);
                if (EI_SUCCESS != s32Ret) {
                    printf("%s: EI_MI_AENC_SendFrame(%d), failed with %#x!\n", \
                        __FUNCTION__, pstAiCtl->AencChn, s32Ret);
                    pstAiCtl->bStart = EI_FALSE;
                    return NULL;
                }
            }
            //printf("#####file:%s,line:%d:EI_MI_AI_GetFrame len %d!\n", __FILE__,__LINE__,stFrame.u32Len);


            s32Ret = EI_MI_AI_ReleaseFrame(pstAiCtl->AiDev, pstAiCtl->AiChn, &stFrame, &stAecFrm);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AI_ReleaseFrame(%d, %d), failed with %#x!\n", \
                    __FUNCTION__, pstAiCtl->AiDev, pstAiCtl->AiChn, s32Ret);
                pstAiCtl->bStart = EI_FALSE;
                return NULL;
            }
            //printf("#####file:%s,line:%d:EI_MI_AI_GetFrame len %d!\n", __FILE__, __LINE__, stFrame.u32Len);
        }
    }

    pstAiCtl->bStart = EI_FALSE;
    return NULL;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAiAo(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    SAMPLE_AI_S *pstAi = NULL;

    pstAi = &gs_stSampleAi[AiDev * AI_MAX_CHN_NUM + AiChn];
    pstAi->bSendAenc = EI_FALSE;
    pstAi->bSendAo = EI_TRUE;
    pstAi->bStart = EI_TRUE;
    pstAi->AiDev = AiDev;
    pstAi->AiChn = AiChn;
    pstAi->AoDev = AoDev;
    pstAi->AoChn = AoChn;

#ifdef COMM_SAVE_PCM_FILE
    pstAi->pTestFile = fopen("/data/sample_comm_arecord.pcm", "wb");
    if (pstAi->pTestFile == EI_NULL) {
        printf("open test file err!\n");
    }
#endif

    pthread_create(&pstAi->stAiPid, 0, SAMPLE_COMM_AUDIO_AiProc, pstAi);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAiAenc(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn)
{
    SAMPLE_AI_S *pstAi = NULL;

    pstAi = &gs_stSampleAi[AiDev * AI_MAX_CHN_NUM + AiChn];
    pstAi->bSendAenc = EI_TRUE;
    pstAi->bSendAo = EI_FALSE;
    pstAi->bStart = EI_TRUE;
    pstAi->AiDev = AiDev;
    pstAi->AiChn = AiChn;
    pstAi->AencChn = AeChn;
    pthread_create(&pstAi->stAiPid, 0, SAMPLE_COMM_AUDIO_AiProc, pstAi);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAi(AUDIO_DEV AiDev, AI_CHN AiChn)
{
    SAMPLE_AI_S *pstAi = NULL;
    AI_CHN_PARAM_S stAiChnPara;
    EI_S32 s32Ret;
    pstAi = &gs_stSampleAi[AiDev * AI_MAX_CHN_NUM + AiChn];
    if (pstAi->bStart) {
        pstAi->bStart = EI_FALSE;
        //pthread_cancel(pstAi->stAiPid);
        pthread_join(pstAi->stAiPid, 0);

        s32Ret = EI_MI_AI_GetChnParam(pstAi->AiDev, pstAi->AiChn, &stAiChnPara);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: Get ai chn param failed\n", __FUNCTION__);
            return EI_FAILURE;
        }

        stAiChnPara.u32UsrFrmDepth = 0;

        s32Ret = EI_MI_AI_SetChnParam(pstAi->AiDev, pstAi->AiChn, &stAiChnPara);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: set ai chn param failed\n", __FUNCTION__);
            return EI_FAILURE;
        }

#ifdef COMM_SAVE_PCM_FILE
        if (pstAi->pTestFile) {
            fclose(pstAi->pTestFile);
            pstAi->pTestFile = EI_NULL;
        }
#endif
    }

    return EI_SUCCESS;
}


void *SAMPLE_COMM_AUDIO_AencProc(void *parg)
{
    EI_S32 s32Ret;
    EI_S32 AencFd;
    SAMPLE_AENC_S *pstAencCtl = (SAMPLE_AENC_S *)parg;
    AUDIO_STREAM_S stStream;
    fd_set read_fds;
    struct timeval TimeoutVal;

    FD_ZERO(&read_fds);
    AencFd = EI_MI_AENC_GetFd(pstAencCtl->AeChn);
    FD_SET(AencFd, &read_fds);

    while (pstAencCtl->bStart) {
        TimeoutVal.tv_sec = 1;
        TimeoutVal.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(AencFd, &read_fds);

        s32Ret = select(AencFd + 1, &read_fds, NULL, NULL, &TimeoutVal);
        //printf("####func:%s,line:%d,ret:%d\n",__FUNCTION__,__LINE__,s32Ret);
        if (s32Ret < 0) {
            break;
        } else if (0 == s32Ret) {
            printf("%s: get aenc stream select time out\n", __FUNCTION__);
            break;
        }

        if (FD_ISSET(AencFd, &read_fds)) {


            s32Ret = EI_MI_AENC_GetStream(pstAencCtl->AeChn, &stStream, -1);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AENC_GetStream(%d), failed with %#x!\n", \
                    __FUNCTION__, pstAencCtl->AeChn, s32Ret);
                pstAencCtl->bStart = EI_FALSE;
                goto ERR;
            }

            //EI_TRACE_ADEC(EI_DBG_ERR, "EI_MI_AENC_GetStream len %d!\n", stStream.u32Len);

            if (EI_TRUE == pstAencCtl->bSendAdChn) {
                s32Ret = EI_MI_ADEC_SendStream(pstAencCtl->AdChn, &stStream, EI_TRUE);
                if (EI_SUCCESS != s32Ret) {
                    printf("%s: EI_MI_ADEC_SendStream(%d), failed with %#x!\n", \
                        __FUNCTION__, pstAencCtl->AdChn, s32Ret);
                    pstAencCtl->bStart = EI_FALSE;
                    goto ERR;
                }
            }


            if (pstAencCtl->pfd) {
                (EI_VOID)fwrite(stStream.pStream, 1, stStream.u32Len, pstAencCtl->pfd);
            }

            //fflush(pstAencCtl->pfd);
            //EI_TRACE_ADEC(EI_DBG_ERR, "##########fwrite:%d,\n",stStream.u32Len);


            s32Ret = EI_MI_AENC_ReleaseStream(pstAencCtl->AeChn, &stStream);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AENC_ReleaseStream(%d), failed with %#x!\n", \
                    __FUNCTION__, pstAencCtl->AeChn, s32Ret);
                pstAencCtl->bStart = EI_FALSE;
                goto ERR;
            }
        } else {
            printf("####func:%s,line:%d\n", __FUNCTION__, __LINE__);
        }

    }
ERR:

    fclose(pstAencCtl->pfd);
    pstAencCtl->bStart = EI_FALSE;
    return NULL;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAencFile(AENC_CHN AeChn, ADEC_CHN AdChn, FILE *pAecFd)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    if (NULL == pAecFd) {
        return EI_FAILURE;
    }

    pstAenc = &gs_stSampleAencAdec[AeChn];
    pstAenc->AeChn = AeChn;
    pstAenc->AdChn = AdChn;
    pstAenc->bSendAdChn = EI_FALSE;
    pstAenc->pfd = pAecFd;
    pstAenc->bStart = EI_TRUE;
    pthread_create(&pstAenc->stAencPid, 0, SAMPLE_COMM_AUDIO_AencProc, pstAenc);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAencAdec(AENC_CHN AeChn, ADEC_CHN AdChn, FILE *pAecFd)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    if (NULL == pAecFd) {
        return EI_FAILURE;
    }

    pstAenc = &gs_stSampleAencAdec[AeChn];
    pstAenc->AeChn = AeChn;
    pstAenc->AdChn = AdChn;
    pstAenc->bSendAdChn = EI_TRUE;
    pstAenc->pfd = pAecFd;
    pstAenc->bStart = EI_TRUE;
    pthread_create(&pstAenc->stAencPid, 0, SAMPLE_COMM_AUDIO_AencProc, pstAenc);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAencAdec(AENC_CHN AeChn)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    pstAenc = &gs_stSampleAencAdec[AeChn];
    if (pstAenc->bStart) {
        pstAenc->bStart = EI_FALSE;
        pthread_join(pstAenc->stAencPid, 0);
    }

    return EI_SUCCESS;
}


void *SAMPLE_COMM_AUDIO_AdecProc(void *parg)
{
    EI_S32 s32Ret;
    AUDIO_STREAM_S stAudioStream = {0};
    EI_U32 u32Len = 1024 * 4;
    EI_U32 u32ReadLen;
    EI_S32 s32AdecChn;
    EI_U8 *pu8AudioStream = NULL;
    SAMPLE_ADEC_S *pstAdecCtl = (SAMPLE_ADEC_S *)parg;
    FILE *pfd = pstAdecCtl->pfd;
    s32AdecChn = pstAdecCtl->AdChn;

    pu8AudioStream = (EI_U8 *)malloc(sizeof(EI_U8) * MAX_AUDIO_STREAM_LEN);
    if (NULL == pu8AudioStream) {
        printf("%s: malloc failed!\n", __FUNCTION__);
        return NULL;
    }

    while (EI_TRUE == pstAdecCtl->bStart) {


        stAudioStream.pStream = pu8AudioStream;
        u32ReadLen = fread(stAudioStream.pStream, 1, u32Len, pfd);
        //printf("###########fread:%d\n",u32ReadLen);
        if (u32ReadLen <= 0) {
            s32Ret = EI_MI_ADEC_SendEndOfStream(s32AdecChn, EI_FALSE);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_ADEC_SendEndOfStream failed:%d!\n", __FUNCTION__, u32ReadLen);
            }
            EI_TRACE_ADEC(EI_DBG_ERR, "read end len:%d,ret:%d\n", u32ReadLen, s32Ret);
            break;
        }


        stAudioStream.u32Len = u32ReadLen;
        s32Ret = EI_MI_ADEC_SendStream(s32AdecChn, &stAudioStream, EI_TRUE);
        //printf("send return u32len:%d\n",stAudioStream.u32Len);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_ADEC_SendStream(%d) failed with %#x!\n", \
                __FUNCTION__, s32AdecChn, s32Ret);
            break;
        }
    }

    free(pu8AudioStream);
    pu8AudioStream = NULL;
    //fclose(pfd);
    pstAdecCtl->bStart = EI_FALSE;
    return NULL;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdFileAdec(ADEC_CHN AdChn, FILE *pAdcFd)
{
    SAMPLE_ADEC_S *pstAdec = NULL;

    if (NULL == pAdcFd) {
        return EI_FAILURE;
    }

    pstAdec = &gs_stSampleFileAdec[AdChn];
    pstAdec->AdChn = AdChn;
    pstAdec->pfd = pAdcFd;
    pstAdec->bStart = EI_TRUE;
    pthread_create(&pstAdec->stAdPid, 0, SAMPLE_COMM_AUDIO_AdecProc, pstAdec);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdFileAdec(ADEC_CHN AdChn)
{
    SAMPLE_ADEC_S *pstAdec = NULL;

    pstAdec = &gs_stSampleFileAdec[AdChn];
    if (pstAdec->bStart) {
        pstAdec->bStart = EI_FALSE;
        pthread_join(pstAdec->stAdPid, 0);
    }

    return EI_SUCCESS;
}


void *SAMPLE_COMM_AUDIO_AoVolProc(void *parg)
{
    EI_S32 s32Ret;
    EI_S32 s32Volume;
    AUDIO_DEV AoDev;
    AUDIO_FADE_S stFade;
    SAMPLE_AO_S *pstAoCtl = (SAMPLE_AO_S *)parg;
    AoDev = pstAoCtl->AoDev;

    while (pstAoCtl->bStart) {
        for (s32Volume = -14; s32Volume <= 0; s32Volume++) {
            s32Ret = EI_MI_AO_SetVolume(AoDev, s32Volume);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AO_SetVolume(%d), failed with %#x!\n", \
                    __FUNCTION__, AoDev, s32Ret);
            }
            printf("\rset volume %d          ", s32Volume);
            fflush(stdout);
            sleep(2);
        }

        for (s32Volume = -8; s32Volume >= -15; s32Volume--) {
            s32Ret = EI_MI_AO_SetVolume(AoDev, s32Volume);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AO_SetVolume(%d), failed with %#x!\n", \
                    __FUNCTION__, AoDev, s32Ret);
            }
            printf("\rset volume %d          ", s32Volume);
            fflush(stdout);
            sleep(2);
        }

        for (s32Volume = -14; s32Volume <= -7; s32Volume++) {
            s32Ret = EI_MI_AO_SetVolume(AoDev, s32Volume);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AO_SetVolume(%d), failed with %#x!\n", \
                    __FUNCTION__, AoDev, s32Ret);
            }
            printf("\rset volume %d          ", s32Volume);
            fflush(stdout);
            sleep(2);
        }

        stFade.bFade         = EI_TRUE;
        stFade.enFadeInRate  = AUDIO_FADE_RATE_128;
        stFade.enFadeOutRate = AUDIO_FADE_RATE_128;

        s32Ret = EI_MI_AO_SetMute(AoDev, EI_TRUE, &stFade);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_AO_SetVolume(%d), failed with %#x!\n", \
                __FUNCTION__, AoDev, s32Ret);
        }
        printf("\rset Ao mute            ");
        fflush(stdout);
        sleep(2);

        s32Ret = EI_MI_AO_SetMute(AoDev, EI_FALSE, NULL);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_AO_SetVolume(%d), failed with %#x!\n", \
                __FUNCTION__, AoDev, s32Ret);
        }
        printf("\rset Ao unmute          ");
        fflush(stdout);
        sleep(2);
    }
    return NULL;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAoVolCtrl(AUDIO_DEV AoDev)
{
    SAMPLE_AO_S *pstAoCtl = NULL;

    pstAoCtl =  &gs_stSampleAo[AoDev];
    pstAoCtl->AoDev =  AoDev;
    pstAoCtl->bStart = EI_TRUE;
    pthread_create(&pstAoCtl->stAoPid, 0, SAMPLE_COMM_AUDIO_AoVolProc, pstAoCtl);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAoVolCtrl(AUDIO_DEV AoDev)
{
    SAMPLE_AO_S *pstAoCtl = NULL;

    pstAoCtl =  &gs_stSampleAo[AoDev];
    if (pstAoCtl->bStart) {
        pstAoCtl->bStart = EI_FALSE;
        // pthread_cancel(pstAoCtl->stAoPid);
        pthread_join(pstAoCtl->stAoPid, 0);
    }


    return EI_SUCCESS;
}


void *SAMPLE_COMM_AUDIO_FileAencProc(void *parg)
{
    EI_S32 s32Ret;
    //AUDIO_STREAM_S stAudioStream;
    AUDIO_FRAME_S stFrm;
    EI_U32 u32Len = 2048;
    EI_U32 u32ReadLen = 0;
    EI_S32 s32AencChn;
    EI_S16 *ps16AudioStream = NULL;
    SAMPLE_AENC_S *pstAencCtl = (SAMPLE_AENC_S *)parg;
    FILE *pfd = pstAencCtl->pfdIn;
    s32AencChn = pstAencCtl->AdChn;
    //printf("file:%s,func:%s,line:%d\n",__FILE__,__FUNCTION__,__LINE__);

    ps16AudioStream = (EI_S16 *)malloc(sizeof(EI_S16) * MAX_AUDIO_FRAME_LEN);
    if (NULL == ps16AudioStream) {
        printf("%s: malloc failed!\n", __FUNCTION__);
        return NULL;
    }
    stFrm.u32Len = 0;
    //printf("file:%s,func:%s,line:%d,%d\n",__FILE__,__FUNCTION__,__LINE__,pstAencCtl->bStart);
    while (EI_TRUE == pstAencCtl->bStartIn) {

        stFrm.u64VirAddr = (EI_U8 *)ps16AudioStream;
        u32ReadLen = fread(stFrm.u64VirAddr, 1, u32Len, pfd);
        //EI_TRACE_ADEC(EI_DBG_ERR, "###########fread:%d\n",u32ReadLen);
        if (u32ReadLen <= 0) {
            //printf("%s: EI_MI_AENC_SendEndOfStream failed:%d!\n", __FUNCTION__ ,u32ReadLen);
        }


        stFrm.u32Len = u32ReadLen;
        //printf("file:%s,func:%s,line:%d\n",__FILE__,__FUNCTION__,__LINE__);
        s32Ret = EI_MI_AENC_SendFrame(s32AencChn, &stFrm, NULL);
        //printf("send return u32len:%d\n",stFrm.u32Len);
        if (s32Ret != EI_SUCCESS) {
            printf("%s: EI_MI_AENC_SendStream(%d) failed with %#x!\n", \
                __FUNCTION__, s32AencChn, s32Ret);
            break;
        }
    }

    free(ps16AudioStream);
    EI_TRACE_ADEC(EI_DBG_ERR, "end!!!!\n");
    ps16AudioStream = NULL;
    //fclose(pfd);
    pstAencCtl->bStartIn = EI_FALSE;
    return NULL;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdFileAenc(AENC_CHN AdChn, FILE *pAdcFd)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    if (NULL == pAdcFd) {
        return EI_FAILURE;
    }

    pstAenc = &gs_stSampleFileAenc[AdChn];
    pstAenc->AdChn = AdChn;
    pstAenc->pfdIn = pAdcFd;
    pstAenc->bStartIn = EI_TRUE;
    pthread_create(&pstAenc->stAencPidIn, 0, SAMPLE_COMM_AUDIO_FileAencProc, pstAenc);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdFileAenc(AENC_CHN AdChn)
{
    SAMPLE_AENC_S *pstAenc = NULL;

    pstAenc = &gs_stSampleFileAenc[AdChn];
    if (pstAenc->bStartIn) {
        pstAenc->bStartIn = EI_FALSE;
        pthread_join(pstAenc->stAencPidIn, 0);
    }


    return EI_SUCCESS;
}


void *SAMPLE_COMM_AUDIO_AdecToFileProc(void *parg)
{
    EI_S32 s32Ret;
    AUDIO_FRAME_INFO_S stFrmInfo = {0};
    AUDIO_FRAME_S  stFrame = {0};
    EI_U32 u32ReadLen;
    EI_S32 s32AdecChn;
    //EI_U8* pu8AudioStream = NULL;
    SAMPLE_ADEC_S *pstAdecCtl = (SAMPLE_ADEC_S *)parg;
    FILE *pfd = pstAdecCtl->pfdOut;
    s32AdecChn = pstAdecCtl->AdChn;

    stFrmInfo.pstFrame = &stFrame;

    while (EI_TRUE == pstAdecCtl->bStartOut) {

        s32Ret = EI_MI_ADEC_GetFrame(s32AdecChn, &stFrmInfo, EI_FALSE);

        if ((s32Ret == EI_ERR_ADEC_NOBUF) || (s32Ret == EI_ERR_ADEC_SYS_NOTREADY)) {
            EI_TRACE_ADEC(EI_DBG_ERR, "ret:%d\n", s32Ret);
            usleep(20000);
            continue;
        } else if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_ADEC_GetFrame(%d) failed with %#x!\n", \
                __FUNCTION__, s32AdecChn, s32Ret);
            break;
        }


        if (EI_TRUE == pstAdecCtl->bSendAo) {
            s32Ret = EI_MI_AO_SendFrame(pstAdecCtl->AoDev, pstAdecCtl->AoChn, &stFrame, 1000);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AO_SendFrame(%d, %d), failed with %#x!\n", \
                    __FUNCTION__, pstAdecCtl->AoDev, pstAdecCtl->AoChn, s32Ret);
                pstAdecCtl->bStartOut = EI_FALSE;
                return NULL;
            }
        }

        if ((stFrmInfo.pstFrame->u32Len > 0) && (pfd)) {
            u32ReadLen = fwrite(stFrmInfo.pstFrame->u64VirAddr, 1, stFrmInfo.pstFrame->u32Len, pfd);
            //printf("##########fwrite:%d,ret:%d\n",stFrmInfo.pstFrame->u32Len,u32ReadLen);
            if (u32ReadLen <= 0) {
                printf("%s: fwrite ret:%d,len:%d failed!\n", __FUNCTION__, u32ReadLen, stFrmInfo.pstFrame->u32Len);
            }
        }

        s32Ret = EI_MI_ADEC_ReleaseFrame(s32AdecChn, &stFrmInfo);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_ADEC_ReleaseFrame(%d) failed with %#x!\n", \
                __FUNCTION__, s32AdecChn, s32Ret);
            break;
        }
    }

    //fclose(pfd);
    pstAdecCtl->bStart = EI_FALSE;
    return NULL;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAdeAo(ADEC_CHN AdChn, AUDIO_DEV AoDev, AO_CHN AoChn, FILE *pAdcFd)
{
    SAMPLE_ADEC_S *pstAdec = NULL;

    pstAdec = &gs_stSampleAdec[AdChn];
    pstAdec->AdChn = AdChn;
    pstAdec->bSendAo = EI_TRUE;
    pstAdec->AoDev = AoDev;
    pstAdec->AoChn = AoChn;
    pstAdec->pfdOut = pAdcFd;
    pstAdec->bStartOut = EI_TRUE;
    pthread_create(&pstAdec->stAdPidout, 0, SAMPLE_COMM_AUDIO_AdecToFileProc, pstAdec);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAdecFile(ADEC_CHN AdChn, FILE *pAdcFd)
{
    SAMPLE_ADEC_S *pstAdec = NULL;

    if (NULL == pAdcFd) {
        return EI_FAILURE;
    }

    pstAdec = &gs_stSampleAdec[AdChn];
    pstAdec->AdChn = AdChn;
    pstAdec->bSendAo = EI_FALSE;
    pstAdec->pfdOut = pAdcFd;
    pstAdec->bStartOut = EI_TRUE;
    pthread_create(&pstAdec->stAdPidout, 0, SAMPLE_COMM_AUDIO_AdecToFileProc, pstAdec);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAdec(ADEC_CHN AdChn)
{
    SAMPLE_ADEC_S *pstAdec = NULL;

    pstAdec = &gs_stSampleAdec[AdChn];
    if (pstAdec->bStartOut) {
        pstAdec->bStartOut = EI_FALSE;
        //pthread_cancel(pstAdec->stAdPid);
        pthread_join(pstAdec->stAdPidout, 0);
    }
    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_AoLinkAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn)
{
    MDP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = EI_ID_ADEC;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = AdChn;
    stDestChn.enModId = EI_ID_AUDIOOUT;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

    return EI_MI_MLINK_Link(&stSrcChn, &stDestChn);
}


EI_S32 SAMPLE_COMM_AUDIO_AoUnbindAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn)
{
    MDP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = EI_ID_ADEC;
    stSrcChn.s32ChnId = AdChn;
    stSrcChn.s32DevId = 0;
    stDestChn.enModId = EI_ID_AUDIOOUT;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

    return EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);
}


EI_S32 SAMPLE_COMM_AUDIO_AoLinkAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    MDP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = EI_ID_AUDIOIN;
    stSrcChn.s32ChnId = AiChn;
    stSrcChn.s32DevId = AiDev;
    stDestChn.enModId = EI_ID_AUDIOOUT;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

    return EI_MI_MLINK_Link(&stSrcChn, &stDestChn);
}


EI_S32 SAMPLE_COMM_AUDIO_AoUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn)
{
    MDP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = EI_ID_AUDIOIN;
    stSrcChn.s32ChnId = AiChn;
    stSrcChn.s32DevId = AiDev;
    stDestChn.enModId = EI_ID_AUDIOOUT;
    stDestChn.s32DevId = AoDev;
    stDestChn.s32ChnId = AoChn;

    return EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);
}


EI_S32 SAMPLE_COMM_AUDIO_AencLinkAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn)
{
    MDP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = EI_ID_AUDIOIN;
    stSrcChn.s32DevId = AiDev;
    stSrcChn.s32ChnId = AiChn;
    stDestChn.enModId = EI_ID_AENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = AeChn;

    return EI_MI_MLINK_Link(&stSrcChn, &stDestChn);
}

EI_S32 SAMPLE_COMM_AUDIO_AencUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn)
{
    MDP_CHN_S stSrcChn, stDestChn;

    stSrcChn.enModId = EI_ID_AUDIOIN;
    stSrcChn.s32DevId = AiDev;
    stSrcChn.s32ChnId = AiChn;
    stDestChn.enModId = EI_ID_AENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = AeChn;

    return EI_MI_MLINK_UnLink(&stSrcChn, &stDestChn);
}


EI_S32 SAMPLE_COMM_AUDIO_StartAi(AUDIO_DEV AiDevId, EI_S32 s32AiChnCnt,
    AIO_ATTR_S *pstAioAttr, AUDIO_SAMPLE_RATE_E enOutSampleRate, EI_BOOL bResampleEn,
    EI_VOID *pstAiVqeAttr)
{
    EI_S32 i;
    EI_S32 s32Ret;

    EI_S32 as32VolumeDb[2] = {5, 0};
    EI_S32 s32Length = (sizeof(as32VolumeDb) / sizeof(as32VolumeDb[0]));

    s32Ret = EI_MI_AUDIOIN_Init(AiDevId);
    if (EI_SUCCESS != s32Ret) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return s32Ret;
    }

    s32Ret = EI_MI_AI_SetPubAttr(AiDevId, pstAioAttr);
    if (s32Ret) {
        printf("%s: EI_MI_AI_SetPubAttr(%d) failed with %#x\n", __FUNCTION__, AiDevId, s32Ret);
        return s32Ret;
    }

    s32Ret = EI_MI_AI_Enable(AiDevId);
    if (s32Ret) {
        printf("%s: EI_MI_AI_Enable(%d) failed with %#x\n", __FUNCTION__, AiDevId, s32Ret);
        return s32Ret;
    }

    s32Ret = EI_MI_AI_SetVolume(AiDevId, s32Length, as32VolumeDb);
    if (s32Ret) {
        printf("%s: EI_MI_AI_SetVolume(%d) failed with %#x\n", __FUNCTION__, AiDevId, s32Ret);
        return s32Ret;
    }

    for (i = 0; i < s32AiChnCnt >> pstAioAttr->enSoundmode; i++) {
        s32Ret = EI_MI_AI_EnableChn(AiDevId, i);
        if (s32Ret) {
            printf("%s: EI_MI_AI_EnableChn(%d,%d) failed with %#x\n", __FUNCTION__, AiDevId, i, s32Ret);
            return s32Ret;
        }

        if (EI_TRUE == bResampleEn) {
            s32Ret = EI_MI_AI_EnableReSmp(AiDevId, i, enOutSampleRate);
            if (s32Ret) {
                printf("%s: EI_MI_AI_EnableReSmp(%d,%d) failed with %#x\n", __FUNCTION__, AiDevId, i, s32Ret);
                return s32Ret;
            }
        }

        if (NULL != pstAiVqeAttr) {
            s32Ret = EI_MI_AI_SetVqeAttr(AiDevId, i, 0, 0, pstAiVqeAttr);
            if (s32Ret) {
                printf("%s: SetAiVqe(%d,%d) failed with %#x\n", __FUNCTION__, AiDevId, i, s32Ret);
                return s32Ret;
            }
            s32Ret = EI_MI_AI_EnableVqe(AiDevId, i);
            if (s32Ret) {
                printf("%s: EI_MI_AI_EnableVqe(%d,%d) failed with %#x\n", __FUNCTION__, AiDevId, i, s32Ret);
                return s32Ret;
            }
        }
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_StopAi(AUDIO_DEV AiDevId, EI_S32 s32AiChnCnt,
    EI_BOOL bResampleEn, EI_BOOL bVqeEn)
{
    EI_S32 i;
    EI_S32 s32Ret;

    for (i = 0; i < s32AiChnCnt; i++) {
        if (EI_TRUE == bResampleEn) {
            s32Ret = EI_MI_AI_DisableReSmp(AiDevId, i);
            if (EI_SUCCESS != s32Ret) {
                printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
                return s32Ret;
            }
        }

        if (EI_TRUE == bVqeEn) {
            s32Ret = EI_MI_AI_DisableVqe(AiDevId, i);
            if (EI_SUCCESS != s32Ret) {
                printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
                return s32Ret;
            }
        }

        s32Ret = EI_MI_AI_DisableChn(AiDevId, i);
        if (EI_SUCCESS != s32Ret) {
            printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
            return s32Ret;
        }
    }

    s32Ret = EI_MI_AI_Disable(AiDevId);
    if (EI_SUCCESS != s32Ret) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return s32Ret;
    }

    s32Ret = EI_MI_AUDIOIN_Exit(AiDevId);
    if (EI_SUCCESS != s32Ret) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return s32Ret;
    }
    printf("EI_MI_AUDIOIN_Exit %d ok\n", __LINE__);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_StartAo(AUDIO_DEV AoDevId, EI_S32 s32AoChnCnt,
    AIO_ATTR_S *pstAioAttr, AUDIO_SAMPLE_RATE_E enInSampleRate, EI_BOOL bResampleEn)
{
    EI_S32 i;
    EI_S32 s32Ret;

    s32Ret = EI_MI_AUDIOOUT_Init(AoDevId);
    if (EI_SUCCESS != s32Ret) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return s32Ret;
    }

    s32Ret = EI_MI_AO_SetPubAttr(AoDevId, pstAioAttr);
    if (EI_SUCCESS != s32Ret) {
        printf("%s: EI_MI_AO_SetPubAttr(%d) failed with %#x!\n", __FUNCTION__, \
            AoDevId, s32Ret);
        return EI_FAILURE;
    }
    printf("%s: EI_MI_AO_SetPubAttr ok \n", __FUNCTION__);

    s32Ret = EI_MI_AO_Enable(AoDevId);
    if (EI_SUCCESS != s32Ret) {
        printf("%s: EI_MI_AO_Enable(%d) failed with %#x!\n", __FUNCTION__, AoDevId, s32Ret);
        return EI_FAILURE;
    }
    printf("%s: EI_MI_AO_Enable ok \n", __FUNCTION__);

    for (i = 0; i < s32AoChnCnt >> pstAioAttr->enSoundmode; i++) {
        s32Ret = EI_MI_AO_EnableChn(AoDevId, i);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_AO_EnableChn(%d) failed with %#x!\n", __FUNCTION__, i, s32Ret);
            return EI_FAILURE;
        }

        if (EI_TRUE == bResampleEn) {
            s32Ret = EI_MI_AO_DisableReSmp(AoDevId, i);
            s32Ret |= EI_MI_AO_EnableReSmp(AoDevId, i, enInSampleRate);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AO_EnableReSmp(%d,%d) failed with %#x!\n", __FUNCTION__, AoDevId, i, s32Ret);
                return EI_FAILURE;
            }
        }

    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_StopAo(AUDIO_DEV AoDevId, EI_S32 s32AoChnCnt, EI_BOOL bResampleEn)
{
    EI_S32 i;
    EI_S32 s32Ret;

    for (i = 0; i < s32AoChnCnt; i++) {
        if (EI_TRUE == bResampleEn) {
            s32Ret = EI_MI_AO_DisableReSmp(AoDevId, i);
            if (EI_SUCCESS != s32Ret) {
                printf("%s: EI_MI_AO_DisableReSmp failed with %#x!\n", __FUNCTION__, s32Ret);
                return s32Ret;
            }
        }

        s32Ret = EI_MI_AO_DisableChn(AoDevId, i);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_AO_DisableChn failed with %#x!\n", __FUNCTION__, s32Ret);
            return s32Ret;
        }
    }

    s32Ret = EI_MI_AO_Disable(AoDevId);
    if (EI_SUCCESS != s32Ret) {
        printf("%s: EI_MI_AO_Disable failed with %#x!\n", __FUNCTION__, s32Ret);
        return s32Ret;
    }

    s32Ret = EI_MI_AUDIOOUT_Exit(AoDevId);
    if (EI_SUCCESS != s32Ret) {
        printf("[Func]:%s [Line]:%d [Info]:%s\n", __FUNCTION__, __LINE__, "failed");
        return s32Ret;
    }
    printf("EI_MI_AUDIOOUT_Exit %d ok\n", __LINE__);

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_StartAdec(ADEC_CHN AdChn,  AIO_ATTR_S *pstAioAttr, PAYLOAD_TYPE_E enType)
{
    EI_S32 s32Ret;
    ADEC_CHN_ATTR_S stAdecAttr;
    ADEC_ATTR_ADPCM_S stAdpcm;
    ADEC_ATTR_G711_S stAdecG711;
    ADEC_ATTR_G726_S stAdecG726;
    ADEC_ATTR_LPCM_S stAdecLpcm;
    ADEC_ATTR_AAC_S stAdecAac;
    ADEC_ATTR_G711_S stMp3Attr;

    stAdecAttr.enType = enType;
    stAdecAttr.u32BufSize = 20;
    stAdecAttr.enMode = ADEC_MODE_STREAM;
    stAdecAttr.s32Ch = pstAioAttr->enSoundmode + 1;
    stAdecAttr.enSamplerate = pstAioAttr->enSamplerate;
    stAdecAttr.enBitwidth = pstAioAttr->enBitwidth;
    stAdecAttr.enSoundmode = pstAioAttr->enSoundmode;
    stAdecAttr.u32PtNumPerFrm = pstAioAttr->u32PtNumPerFrm;
    if (PT_ADPCMA == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdpcm;
        stAdpcm.enADPCMType = AUDIO_ADPCM_TYPE ;
    } else if (PT_G711A == stAdecAttr.enType || PT_G711U == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdecG711;
    } else if (PT_G726 == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdecG726;
        stAdecG726.enG726bps = G726_BPS ;
    } else if (PT_G729 == stAdecAttr.enType) {
    } else if (PT_LPCM == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdecLpcm;
        stAdecAttr.enMode = ADEC_MODE_PACK;
    } else if (PT_AAC == stAdecAttr.enType) {
        stAdecAttr.pValue = &stAdecAac;
        stAdecAttr.enMode = ADEC_MODE_STREAM;
    } else if (PT_MP3 == stAdecAttr.enType) {
        stAdecAttr.pValue = &stMp3Attr;
    } else {
        printf("%s: invalid aenc payload type:%d\n", __FUNCTION__, stAdecAttr.enType);
        return EI_FAILURE;
    }


    s32Ret = EI_MI_ADEC_CreateChn(AdChn, &stAdecAttr);
    if (EI_SUCCESS != s32Ret) {
        printf("%s: EI_MI_ADEC_CreateChn(%d) failed with %#x!\n", __FUNCTION__, \
            AdChn, s32Ret);
        return s32Ret;
    }
    return 0;
}


EI_S32 SAMPLE_COMM_AUDIO_StopAdec(ADEC_CHN AdChn)
{
    EI_S32 s32Ret;

    s32Ret = EI_MI_ADEC_DestroyChn(AdChn);
    if (EI_SUCCESS != s32Ret) {
        printf("%s: EI_MI_ADEC_DestroyChn(%d) failed with %#x!\n", __FUNCTION__,
            AdChn, s32Ret);
        return s32Ret;
    }
    printf("SAMPLE_COMM_AUDIO_StopAdec %d ok\n", __LINE__);
    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_StartAenc(EI_S32 s32AencChnCnt, AIO_ATTR_S *pstAioAttr, PAYLOAD_TYPE_E enType)
{
    AENC_CHN AeChn;
    EI_S32 s32Ret, i;
    AENC_CHN_ATTR_S stAencAttr;
    AENC_ATTR_ADPCM_S stAdpcmAenc;
    AENC_ATTR_G711_S stAencG711;
    AENC_ATTR_G726_S stAencG726;
    AENC_ATTR_LPCM_S stAencLpcm;
    AENC_ATTR_AAC_S  stAencAac;



    stAencAttr.enType = enType;
    stAencAttr.u32BufSize = 30;
    stAencAttr.u32PtNumPerFrm = pstAioAttr->u32PtNumPerFrm;
    stAencAttr.enSoundmode = pstAioAttr->enSoundmode;
    stAencAttr.enSamplerate = pstAioAttr->enSamplerate;

    if (PT_ADPCMA == stAencAttr.enType) {
        stAencAttr.pValue       = &stAdpcmAenc;
        stAdpcmAenc.enADPCMType = AUDIO_ADPCM_TYPE;
    } else if (PT_G711A == stAencAttr.enType || PT_G711U == stAencAttr.enType) {
        stAencAttr.pValue       = &stAencG711;
    } else if (PT_G726 == stAencAttr.enType) {
        stAencAttr.pValue       = &stAencG726;
        stAencG726.enG726bps    = G726_BPS;
    } else if (PT_G729 == stAencAttr.enType) {
    } else if (PT_LPCM == stAencAttr.enType) {
        stAencAttr.pValue = &stAencLpcm;
    } else if (PT_AAC == stAencAttr.enType) {

        stAencAttr.pValue = &stAencAac;
        stAencAac.enBitRate = pstAioAttr->enSamplerate * (pstAioAttr->enSoundmode + 1) * 16 / 10;//128000;
		stAencAac.enBitRate = 128000;
		stAencAac.enBitWidth = AUDIO_BIT_WIDTH_16;
        stAencAac.enSmpRate = pstAioAttr->enSamplerate;
        stAencAac.enSoundMode = pstAioAttr->enSoundmode;
        stAencAac.s16BandWidth = 0;
    } else {
        printf("%s: invalid aenc payload type:%d\n", __FUNCTION__, stAencAttr.enType);
        return EI_FAILURE;
    }

    for (i = 0; i < s32AencChnCnt; i++) {
        AeChn = i;


        s32Ret = EI_MI_AENC_CreateChn(AeChn, &stAencAttr);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_AENC_CreateChn(%d) failed with %#x!\n", __FUNCTION__,
                AeChn, s32Ret);
            return s32Ret;
        }
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_StopAenc(EI_S32 s32AencChnCnt)
{
    EI_S32 i;
    EI_S32 s32Ret;

    for (i = 0; i < s32AencChnCnt; i++) {
        s32Ret = EI_MI_AENC_DestroyChn(i);
        if (EI_SUCCESS != s32Ret) {
            printf("%s: EI_MI_AENC_DestroyChn(%d) failed with %#x!\n", __FUNCTION__,
                i, s32Ret);
            return s32Ret;
        }

    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_AUDIO_DestroyAllTrd(void)
{
    EI_U32 u32DevId, u32ChnId;

    for (u32DevId = 0; u32DevId < AI_MAX_DEV_NUM; u32DevId ++) {
        for (u32ChnId = 0; u32ChnId < AI_MAX_CHN_NUM; u32ChnId ++) {
            if (EI_SUCCESS != SAMPLE_COMM_AUDIO_DestroyTrdAi(u32DevId, u32ChnId)) {
                printf("%s: SAMPLE_COMM_AUDIO_DestroyTrdAi(%d,%d) failed!\n", __FUNCTION__,
                    u32DevId, u32ChnId);
                return EI_FAILURE;
            }
        }
    }

    for (u32ChnId = 0; u32ChnId < AI_MAX_DEV_NUM; u32ChnId ++) {
        if (EI_SUCCESS != SAMPLE_COMM_AUDIO_DestroyTrdAencAdec(u32ChnId)) {
            printf("%s: SAMPLE_COMM_AUDIO_DestroyTrdAencAdec(%d) failed!\n", __FUNCTION__,
                u32ChnId);
            return EI_FAILURE;
        }
    }

    for (u32ChnId = 0; u32ChnId < ADEC_MAX_CHN_NUM; u32ChnId ++) {
        if (EI_SUCCESS != SAMPLE_COMM_AUDIO_DestroyTrdFileAdec(u32ChnId)) {
            printf("%s: SAMPLE_COMM_AUDIO_DestroyTrdFileAdec(%d) failed!\n", __FUNCTION__,
                u32ChnId);
            return EI_FAILURE;
        }
    }

    for (u32ChnId = 0; u32ChnId < AO_MAX_DEV_NUM; u32ChnId ++) {
        if (EI_SUCCESS != SAMPLE_COMM_AUDIO_DestroyTrdAoVolCtrl(u32ChnId)) {
            printf("%s: SAMPLE_COMM_AUDIO_DestroyTrdAoVolCtrl(%d) failed!\n", __FUNCTION__,
                u32ChnId);
            return EI_FAILURE;
        }
    }

    return EI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

