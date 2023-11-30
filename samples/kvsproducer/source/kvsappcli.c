/*
 * Copyright 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif /* HAVE_SIGNAL_H */

/* Headers for KVS */
#include "kvs/kvsapp.h"
#include "kvs/port.h"

#include "sample_config.h"
#include "option_configuration.h"

#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"
#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#define ERRNO_NONE 0
#define ERRNO_FAIL __LINE__

#define VIDEO_FRAME_BUFFER_SIZE_BYTES (160 * 1024UL)
#if ENABLE_AUDIO_TRACK
#if USE_AUDIO_G711
#define AUDIO_FRAME_BUFFER_SIZE_BYTES (320UL)
#elif USE_AUDIO_AAC
#define AUDIO_FRAME_BUFFER_SIZE_BYTES (1024UL)
#endif
#endif /* ENABLE_AUDIO_TRACK */
#define MICROSECONDS_IN_A_MILLISECOND (1000LL)

#ifdef KVS_USE_POOL_ALLOCATOR
#include "kvs/pool_allocator.h"
static char pMemPool[POOL_ALLOCATOR_SIZE];
#endif

#define SAFE_MEMORYFREE(p)     if (p)   \
{                                       \
    free(p);                            \
    p = NULL;                           \
}                                       \

static VideoCapturerHandle videoCapturerHandle = NULL;
static pthread_t videoThreadTid;

#if ENABLE_AUDIO_TRACK
static AudioCapturerHandle audioCapturerHandle = NULL;
static pthread_t audioThreadTid;
static AudioTrackInfo_t audioTrackInfo = {
    .pTrackName = AUDIO_TRACK_NAME,
    .pCodecName = AUDIO_CODEC_NAME,
    .uFrequency = AUDIO_FREQUENCY,
    .uChannelNumber = AUDIO_CHANNEL_NUMBER,
};
#endif /* ENABLE_AUDIO_TRACK */

/* A global variable to exit program if it's set to true. It can be set to true if signal.h is available and user press Ctrl+c. It can also be set to true via debugger. */
static bool gStopRunning = false;

#ifdef HAVE_SIGNAL_H
static void signalHandler(int signum)
{
    if (!gStopRunning)
    {
        printf("Received interrupt signal\n");
        gStopRunning = true;
    }
    else
    {
        printf("Force leaving\n");
        exit(signum);
    }
}
#endif /* HAVE_SIGNAL_H */

#if DEBUG_STORE_MEDIA_TO_FILE
static FILE *fpDbgMedia = NULL;
static int onMkvSent(uint8_t *pData, size_t uDataLen, void *pAppData)
{
    int res = ERRNO_NONE;
    char pFilename[sizeof(MEDIA_FILENAME_FORMAT) + 21]; /* 20 digits for uint64_t, 1 digit for EOS */

    if (fpDbgMedia == NULL)
    {
        snprintf(pFilename, sizeof(pFilename) - 1, MEDIA_FILENAME_FORMAT, getEpochTimestampInMs());
        fpDbgMedia = fopen(pFilename, "wb");
        if (fpDbgMedia == NULL)
        {
            printf("Failed to open debug file: %s\n", pFilename);
        }
        else
        {
            printf("Opened debug file %s\n", pFilename);
        }
    }

    if (fpDbgMedia != NULL)
    {
        fwrite(pData, 1, uDataLen, fpDbgMedia);
    }

    return res;
}
#endif

static void *videoThread(void *arg)
{
    int res = ERRNO_NONE;
    void *pFrameBuffer = NULL;
    uint64_t timestamp = 0;
    size_t frameSize = 0;
    KvsAppHandle kvsAppHandle = (KvsAppHandle)(arg);

    if (kvsAppHandle == NULL)
    {
        printf("%s(): Invalid argument: pKvs\n", __FUNCTION__);
        res = ERRNO_FAIL;
    }
    else if (videoCapturerAcquireStream(videoCapturerHandle))
    {
        printf("%s(): Failed to acquire video stream\n", __FUNCTION__);
        res = ERRNO_FAIL;
    }
    else
    {
        while (true)
        {
            if (gStopRunning)
            {
                break;
            }

            pFrameBuffer = malloc(VIDEO_FRAME_BUFFER_SIZE_BYTES);

            if (!pFrameBuffer)
            {
                printf("OOM\n");
                continue;
            }

            if (videoCapturerGetFrame(videoCapturerHandle, pFrameBuffer, VIDEO_FRAME_BUFFER_SIZE_BYTES, &timestamp, &frameSize))
            {
                printf("videoCapturerGetFrame failed\n");
                free(pFrameBuffer);
            }
            else
            {
                // KvsApp will help to free pFrameBuffer
                KvsApp_addFrame(kvsAppHandle, pFrameBuffer, frameSize, VIDEO_FRAME_BUFFER_SIZE_BYTES, timestamp / MICROSECONDS_IN_A_MILLISECOND, TRACK_VIDEO);
            }

            pFrameBuffer = NULL;
        }
    }

    videoCapturerReleaseStream(videoCapturerHandle);
    printf("video thread leaving, err:%d\n", res);

    return NULL;
}

#if ENABLE_AUDIO_TRACK
static void *audioThread(void *arg)
{
    int res = ERRNO_NONE;
    void *pFrameBuffer = NULL;
    uint64_t timestamp = 0;
    size_t frameSize = 0;
    KvsAppHandle kvsAppHandle = (KvsAppHandle)(arg);

    if (!kvsAppHandle)
    {
        printf("%s(): Invalid argument: pKvs\n", __FUNCTION__);
        res = ERRNO_FAIL;
    }
    else if (audioCapturerAcquireStream(audioCapturerHandle))
    {
        printf("%s(): Failed to acquire video stream\n", __FUNCTION__);
        res = ERRNO_FAIL;
    }
    else
    {
        while (true)
        {
            if (gStopRunning)
            {
                break;
            }
            pFrameBuffer = malloc(AUDIO_FRAME_BUFFER_SIZE_BYTES);

            if (!pFrameBuffer)
            {
                printf("OOM\n");
                continue;
            }

            if (audioCapturerGetFrame(audioCapturerHandle, pFrameBuffer, AUDIO_FRAME_BUFFER_SIZE_BYTES, &timestamp, &frameSize))
            {
                printf("audioCapturerGetFrame failed\n");
                free(pFrameBuffer);
            }
            else
            {
                // KvsApp will help to free pFrameBuffer
                KvsApp_addFrame(kvsAppHandle, pFrameBuffer, frameSize, AUDIO_FRAME_BUFFER_SIZE_BYTES, timestamp / MICROSECONDS_IN_A_MILLISECOND, TRACK_AUDIO);
            }

            pFrameBuffer = NULL;
        }
    }

    audioCapturerReleaseStream(videoCapturerHandle);
    printf("audio thread leaving, err:%d\n", res);

    return NULL;
}
#endif /* ENABLE_AUDIO_TRACK */

#if ENABLE_IOT_CREDENTIAL
static int readFile(char* pFileName, char** ppData)
{
    FILE* fp = NULL;
    char* pData = NULL;

    if (ppData == NULL || pFileName == NULL)
    {
        printf("intput parameter is NULL\n");
        return -1;
    }

    fp = fopen(pFileName, "r");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        long lSize = ftell(fp);
        rewind(fp);
        pData = (char *) malloc(lSize);
        if (pData)
        {
            fread(pData, 1, lSize, fp);
        }
        else
        {
            fclose(fp);
            return -1;
        }
        fclose(fp);
    }
    else
    {
        printf("open file:%s failed\n", pFileName);
        return -1;
    }

    *ppData = pData;
    return 0;
}
#endif /* ENABLE_IOT_CREDENTIAL */

static int setKvsAppOptions(KvsAppHandle kvsAppHandle)
{
    int res = ERRNO_NONE;

    /* Setup credentials, it should be either using IoT credentials or AWS access key. */
#if ENABLE_IOT_CREDENTIAL
    char *pThingName, *pRootCa, *pCredentialEndPoint, *pCoreCert, *pPrivateKey, *pRoleAlias;
    char *pRootCaContext, *pCoreCertContext, *pPrivateKeyContext;

    if ((pThingName = getenv(IOT_CORE_THING_NAME)) == NULL)
    {
        printf("AWS_IOT_CORE_THING_NAME must be set\n");
    }
    if ((pCoreCert = getenv(IOT_CORE_CERT)) == NULL)
    {
        printf("AWS_IOT_CORE_CERT must be set\n");
    }
    if ((pPrivateKey = getenv(IOT_CORE_PRIVATE_KEY)) == NULL)
    {
        printf("AWS_IOT_CORE_PRIVATE_KEY must be set\n");
    }
    if ((pRoleAlias = getenv(IOT_CORE_ROLE_ALIAS)) == NULL)
    {
        printf("AWS_IOT_CORE_ROLE_ALIAS must be set\n");
    }
    if ((pRootCa = getenv(IOT_CORE_CACERT_PATH)) == NULL)
    {
        printf("AWS_KVS_CACERT_PATH must be set\n");
    }
    if ((pCredentialEndPoint = getenv(IOT_CORE_CREDENTIAL_ENDPOINT)) == NULL)
    {
        printf("AWS_IOT_CORE_CREDENTIAL_ENDPOINT  must be set\n");
    }

    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_CREDENTIAL_HOST, (const char *)pCredentialEndPoint) != 0)
    {
        printf("Failed to set credential host\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_ROLE_ALIAS, (const char *)pRoleAlias) != 0)
    {
        printf("Failed to set role alias\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_THING_NAME, (const char *)pThingName) != 0)
    {
        printf("Failed to set thing name\n");
    }

    res = readFile(pRootCa, &pRootCaContext);
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_X509_ROOTCA, (const char*) pRootCaContext) != 0)
    {
        printf("Failed to set root CA\n");
    }

    res = readFile(pCoreCert, &pCoreCertContext);
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_X509_CERT, (const char *)pCoreCertContext) != 0)
    {
        printf("Failed to set certificate\n");
    }

    res = readFile(pPrivateKey, &pPrivateKeyContext);
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_X509_KEY, (const char *)pPrivateKeyContext) != 0)
    {
        printf("Failed to set private key\n");
    }

    SAFE_MEMORYFREE(pRootCaContext);
    SAFE_MEMORYFREE(pCoreCertContext);
    SAFE_MEMORYFREE(pPrivateKeyContext);
#else
    if (KvsApp_setoption(kvsAppHandle, OPTION_AWS_ACCESS_KEY_ID, OptCfg_getAwsAccessKey()) != 0)
    {
        printf("Failed to set AWS_ACCESS_KEY\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_AWS_SECRET_ACCESS_KEY, OptCfg_getAwsSecretAccessKey()) != 0)
    {
        printf("Failed to set AWS_SECRET_KEY\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_AWS_SESSION_TOKEN, OptCfg_getAwsSessionToken()) != 0)
    {
        printf("Failed to set AWS_SESSION_TOKEN\n");
    }
#endif /* ENABLE_IOT_CREDENTIAL */

#if ENABLE_AUDIO_TRACK
    if (audioCapturerHandle)
    {
        if (KvsApp_setoption(kvsAppHandle, OPTION_KVS_AUDIO_TRACK_INFO, (const char *)(&audioTrackInfo)) != 0)
        {
            printf("Failed to set audio track info\n");
        }
    }
#endif /* ENABLE_AUDIO_TRACK */

#if ENABLE_RING_BUFFER_MEM_LIMIT
    KvsApp_streamPolicy_t xPolicy = STREAM_POLICY_RING_BUFFER;
    if (KvsApp_setoption(kvsAppHandle, OPTION_STREAM_POLICY, (const char *)&xPolicy) != 0)
    {
        printf("Failed to set stream policy\n");
    }
    size_t uRingBufferMemLimit = RING_BUFFER_MEM_LIMIT;
    if (KvsApp_setoption(kvsAppHandle, OPTION_STREAM_POLICY_RING_BUFFER_MEM_LIMIT, (const char *)&uRingBufferMemLimit) != 0)
    {
        printf("Failed to set ring buffer memory limit\n");
    }
#endif /* ENABLE_RING_BUFFER_MEM_LIMIT */

#if DEBUG_STORE_MEDIA_TO_FILE
    if (KvsApp_setOnMkvSentCallback(kvsAppHandle, onMkvSent, NULL) != 0)
    {
        printf("Failed to set onMkvSentCallback\n");
    }
#endif /* DEBUG_STORE_MEDIA_TO_FILE */

    return res;
}

int main(int argc, char *argv[])
{
    int res = 0;
    KvsAppHandle kvsAppHandle;
    uint64_t uLastPrintMemStatTimestamp = 0;
    ePutMediaFragmentAckEventType eAckEventType = eUnknown;
    uint64_t uFragmentTimecode = 0;
    unsigned int uErrorId = 0;
    const char *pKvsStreamName = NULL;
    DoWorkExParamter_t xDoWorkExParamter = {0};

#ifdef KVS_USE_POOL_ALLOCATOR
    poolAllocatorInit((void *)pMemPool, sizeof(pMemPool));
#endif

#ifdef HAVE_SIGNAL_H
    /* Register interrupt signal handler so user can press Ctrl+C to exit this program gracefully. */
    signal(SIGINT, signalHandler);
#endif /* HAVE_SIGNAL_H */

    /* Resolve KVS stream name */
    pKvsStreamName = (argc >= 2) ? argv[1] : KVS_STREAM_NAME;

    if ((kvsAppHandle = KvsApp_create(OptCfg_getHostKinesisVideo(), OptCfg_getRegion(), OptCfg_getServiceKinesisVideo(), pKvsStreamName)) == NULL)
    {
        printf("Failed to initialize KVS\n");
        return ERRNO_FAIL;
    }

#if ENABLE_AUDIO_TRACK
#if USE_AUDIO_G711
    AudioFormat audioFormat = AUD_FMT_G711A;
#elif USE_AUDIO_AAC
    AudioFormat audioFormat = AUD_FMT_AAC;
#endif
    if ((audioCapturerHandle = audioCapturerCreate()) == NULL)
    {
        printf("Failed to create audio capturer\n");
    }
    else if (audioCapturerSetFormat(audioCapturerHandle, audioFormat, AUD_CHN_MONO, AUD_SAM_8K, AUD_BIT_16))
    {
        printf("Failed to set audio format\n");
        audioCapturerDestroy(audioCapturerHandle);
        audioCapturerHandle = NULL;
    }
    else if (pthread_create(&audioThreadTid, NULL, audioThread, kvsAppHandle))
    {
        printf("Failed to create audio thread\n");
        audioCapturerDestroy(audioCapturerHandle);
        audioCapturerHandle = NULL;
    }
    else
    {
#if USE_AUDIO_G711
        Mkv_generatePcmCodecPrivateData(AUDIO_CODEC_OBJECT_TYPE, audioTrackInfo.uFrequency, audioTrackInfo.uChannelNumber, &audioTrackInfo.pCodecPrivate, &audioTrackInfo.uCodecPrivateLen);
#elif USE_AUDIO_AAC
        Mkv_generateAacCodecPrivateData(AUDIO_CODEC_OBJECT_TYPE, audioTrackInfo.uFrequency, audioTrackInfo.uChannelNumber, &audioTrackInfo.pCodecPrivate, &audioTrackInfo.uCodecPrivateLen);
#endif
    }
#endif /* ENABLE_AUDIO_TRACK */

    if ((videoCapturerHandle = videoCapturerCreate()) == NULL)
    {
        printf("Failed to create video capturer\n");
    }
    else if (videoCapturerSetFormat(videoCapturerHandle, VID_FMT_H264, VID_RES_1080P))
    {
        printf("Failed to set video format\n");
    }
    else if (pthread_create(&videoThreadTid, NULL, videoThread, kvsAppHandle))
    {
        printf("Failed to create video thread\n");
    }
    else if (setKvsAppOptions(kvsAppHandle) != ERRNO_NONE)
    {
        printf("Failed to set options\n");
    }
    else
    {
        while (true)
        {
            /* FIXME: Check if network is reachable before running KVS. */
            if (gStopRunning)
            {
                break;
            }

            if ((res = KvsApp_open(kvsAppHandle)) != 0)
            {
                printf("Failed to open KVS app, err:-%X\n", -res);
                break;
            }

            while (true)
            {
                if (gStopRunning)
                {
                    break;
                }
                
                if ((res = KvsApp_doWork(kvsAppHandle)) != 0)
                {
                    printf("do work err:-%X\n", -res);
                    break;
                }

                while (KvsApp_readFragmentAck(kvsAppHandle, &eAckEventType, &uFragmentTimecode, &uErrorId) == 0)
                {
                    if (eAckEventType == ePersisted)
                    {
                        // printf("key-frame with timecode %" PRIu64 " is persisted\n", uFragmentTimecode);
                    }
                }

                if (getEpochTimestampInMs() > uLastPrintMemStatTimestamp + 1000)
                {
                    printf("Buffer memory used: %zu\n", KvsApp_getStreamMemStatTotal(kvsAppHandle));
                    uLastPrintMemStatTimestamp = getEpochTimestampInMs();
#ifdef KVS_USE_POOL_ALLOCATOR
                    PoolStats_t stats = {0};
                    poolAllocatorGetStats(&stats);
                    printf("Sum of used/free memory:%zu/%zu, size of largest used/free block:%zu/%zu, number of used/free blocks:%zu/%zu\n", stats.uSumOfUsedMemory, stats.uSumOfFreeMemory, stats.uSizeOfLargestUsedBlock, stats.uSizeOfLargestFreeBlock, stats.uNumberOfUsedBlocks, stats.uNumberOfFreeBlocks);
#endif
                }
            }

            xDoWorkExParamter.eType = DO_WORK_SEND_END_OF_FRAMES;
            KvsApp_doWorkEx(kvsAppHandle, &xDoWorkExParamter);

            while (KvsApp_readFragmentAck(kvsAppHandle, &eAckEventType, &uFragmentTimecode, &uErrorId) == 0)
            {
                if (eAckEventType == eError)
                {
                    /* Please refer to the following link to get more information on the error ID.
                     *      https://docs.aws.amazon.com/kinesisvideostreams/latest/dg/API_dataplane_PutMedia.html
                     */
                }
            }

            if (KvsApp_close(kvsAppHandle) != 0)
            {
                printf("Failed to close KVS app\n");
                break;
            }
            else
            {
                printf("KvsApp closed\n");
#if DEBUG_STORE_MEDIA_TO_FILE
                if (fpDbgMedia != NULL)
                {
                    fclose(fpDbgMedia);
                    fpDbgMedia = NULL;
                    printf("Closed debug file\n");
                }
#endif
            }
        }
    }

    KvsApp_close(kvsAppHandle);
    gStopRunning = true;

    pthread_join(videoThreadTid, NULL);
#if ENABLE_AUDIO_TRACK
    pthread_join(audioThreadTid, NULL);
#endif /* ENABLE_AUDIO_TRACK */

    videoCapturerDestroy(videoCapturerHandle);
    videoCapturerHandle = NULL;
#if ENABLE_AUDIO_TRACK
    audioCapturerDestroy(audioCapturerHandle);
    audioCapturerHandle = NULL;
#endif

    KvsApp_terminate(kvsAppHandle);

#ifdef KVS_USE_POOL_ALLOCATOR
    poolAllocatorDeinit();
#endif

    return 0;
}
