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

#include "kvs/kvsapp.h"
#include "kvs/port.h"

#include "sample_config.h"
#include "option_configuration.h"

#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"
#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#define ERRNO_NONE 0
#define ERRNO_FAIL __LINE__

#define VIDEO_FRAME_BUFFER_SIZE_BYTES (128 * 1024UL)
#define AUDIO_FRAME_BUFFER_SIZE_BYTES (512UL)
#define MICROSECONDS_IN_A_MILLISECOND (1000LL)

#ifdef KVS_USE_POOL_ALLOCATOR
#include "kvs/pool_allocator.h"
static char pMemPool[POOL_ALLOCATOR_SIZE];
#endif

// static KvsAppHandle kvsAppHandle = NULL;
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

#if DEBUG_STORE_MEDIA_TO_FILE
static FILE* fpDbgMedia = NULL;
static int onMkvSent(uint8_t* pData, size_t uDataLen, void* pAppData)
{
    int res = ERRNO_NONE;
    char pFilename[sizeof(MEDIA_FILENAME_FORMAT) + 21]; /* 20 digits for uint64_t, 1 digit for EOS */

    if (fpDbgMedia == NULL) {
        snprintf(pFilename, sizeof(pFilename) - 1, MEDIA_FILENAME_FORMAT, getEpochTimestampInMs());
        fpDbgMedia = fopen(pFilename, "wb");
        if (fpDbgMedia == NULL) {
            printf("Failed to open debug file: %s\n", pFilename);
        } else {
            printf("Opened debug file %s\n", pFilename);
        }
    }

    if (fpDbgMedia != NULL) {
        fwrite(pData, 1, uDataLen, fpDbgMedia);
    }

    return res;
}
#endif

static int setKvsAppOptions(KvsAppHandle kvsAppHandle)
{
    int res = ERRNO_NONE;

    /* Setup credentials, it should be either using IoT credentials or AWS access key. */
#if ENABLE_IOT_CREDENTIAL
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_CREDENTIAL_HOST, (const char*) CREDENTIALS_HOST) != 0) {
        printf("Failed to set credential host\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_ROLE_ALIAS, (const char*) ROLE_ALIAS) != 0) {
        printf("Failed to set role alias\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_THING_NAME, (const char*) THING_NAME) != 0) {
        printf("Failed to set thing name\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_X509_ROOTCA, (const char*) ROOT_CA) != 0) {
        printf("Failed to set root CA\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_X509_CERT, (const char*) CERTIFICATE) != 0) {
        printf("Failed to set certificate\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_IOT_X509_KEY, (const char*) PRIVATE_KEY) != 0) {
        printf("Failed to set private key\n");
    }
#else
    if (KvsApp_setoption(kvsAppHandle, OPTION_AWS_ACCESS_KEY_ID, OptCfg_getAwsAccessKey()) != 0) {
        printf("Failed to set AWS_ACCESS_KEY\n");
    }
    if (KvsApp_setoption(kvsAppHandle, OPTION_AWS_SECRET_ACCESS_KEY, OptCfg_getAwsSecretAccessKey()) != 0) {
        printf("Failed to set AWS_SECRET_KEY\n");
    }
#endif /* ENABLE_IOT_CREDENTIAL */

#if ENABLE_AUDIO_TRACK
    if (KvsApp_setoption(kvsAppHandle, OPTION_KVS_AUDIO_TRACK_INFO, (const char*) (&audioTrackInfo)) != 0) {
        printf("Failed to set audio track info\n");
    }
#endif /* ENABLE_AUDIO_TRACK */

#if ENABLE_RING_BUFFER_MEM_LIMIT
    KvsApp_streamPolicy_t xPolicy = STREAM_POLICY_RING_BUFFER;
    if (KvsApp_setoption(kvsAppHandle, OPTION_STREAM_POLICY, (const char*) &xPolicy) != 0) {
        printf("Failed to set stream policy\n");
    }
    size_t uRingBufferMemLimit = RING_BUFFER_MEM_LIMIT;
    if (KvsApp_setoption(kvsAppHandle, OPTION_STREAM_POLICY_RING_BUFFER_MEM_LIMIT, (const char*) &uRingBufferMemLimit) != 0) {
        printf("Failed to set ring buffer memory limit\n");
    }
#endif /* ENABLE_RING_BUFFER_MEM_LIMIT */

#if DEBUG_STORE_MEDIA_TO_FILE
    if (KvsApp_setOnMkvSentCallback(kvsAppHandle, onMkvSent, NULL) != 0) {
        printf("Failed to set onMkvSentCallback\n");
    }
#endif /* DEBUG_STORE_MEDIA_TO_FILE */

    return res;
}

static void* videoThread(void* args)
{
    int res = ERRNO_NONE;
    void* pFrameBuffer = NULL;
    uint64_t timestamp = 0;
    size_t frameSize = 0;
    KvsAppHandle kvsAppHandle = (KvsAppHandle) (args);

    if (!kvsAppHandle) {
        printf("Invalid kvsAppHandle\n");
        res = ERRNO_FAIL;
    } else if (videoCapturerAcquireStream(videoCapturerHandle)) {
        printf("Failed to acquire video stream\n");
        res = ERRNO_FAIL;
    } else {
        while (true) {
            pFrameBuffer = malloc(VIDEO_FRAME_BUFFER_SIZE_BYTES);

            if (!pFrameBuffer) {
                printf("OOM\n");
                continue;
            }

            if (videoCapturerGetFrame(videoCapturerHandle, pFrameBuffer, VIDEO_FRAME_BUFFER_SIZE_BYTES, &timestamp, &frameSize)) {
                printf("videoCapturerGetFrame failed\n");
                free(pFrameBuffer);
            } else {
                // KvsApp will help to free pFrameBuffer
                KvsApp_addFrame(kvsAppHandle, pFrameBuffer, frameSize, VIDEO_FRAME_BUFFER_SIZE_BYTES, timestamp / MICROSECONDS_IN_A_MILLISECOND,
                                TRACK_VIDEO);
            }

            pFrameBuffer = NULL;
        }
    }

    videoCapturerReleaseStream(videoCapturerHandle);

    return (void*) res;
}

static void* audioThread(void* args)
{
    int res = ERRNO_NONE;
    void* pFrameBuffer = NULL;
    uint64_t timestamp = 0;
    size_t frameSize = 0;
    KvsAppHandle kvsAppHandle = (KvsAppHandle) (args);

    if (!kvsAppHandle) {
        printf("Invalid kvsAppHandle\n");
        res = ERRNO_FAIL;
    } else if (audioCapturerAcquireStream(audioCapturerHandle)) {
        printf("Failed to acquire video stream\n");
        res = ERRNO_FAIL;
    } else {
        while (true) {
            pFrameBuffer = malloc(AUDIO_FRAME_BUFFER_SIZE_BYTES);

            if (!pFrameBuffer) {
                printf("OOM\n");
                continue;
            }

            if (audioCapturerGetFrame(audioCapturerHandle, pFrameBuffer, AUDIO_FRAME_BUFFER_SIZE_BYTES, &timestamp, &frameSize)) {
                printf("audioCapturerGetFrame failed\n");
                free(pFrameBuffer);

            } else {
                // KvsApp will help to free pFrameBuffer
                KvsApp_addFrame(kvsAppHandle, pFrameBuffer, frameSize, AUDIO_FRAME_BUFFER_SIZE_BYTES, timestamp / MICROSECONDS_IN_A_MILLISECOND,
                                TRACK_AUDIO);
            }

            pFrameBuffer = NULL;
        }
    }

    videoCapturerReleaseStream(videoCapturerHandle);

    return (void*) res;
}

int main(int argc, char* argv[])
{
    KvsAppHandle kvsAppHandle;
    uint64_t uLastPrintMemStatTimestamp = 0;
    const char* pKvsStreamName = NULL;

#if ENABLE_AUDIO_TRACK
#if USE_AUDIO_G711
    Mkv_generatePcmCodecPrivateData(AUDIO_PCM_OBJECT_TYPE, audioTrackInfo.uFrequency, audioTrackInfo.uChannelNumber, &audioTrackInfo.pCodecPrivate,
                                    &audioTrackInfo.uCodecPrivateLen);
#elif USE_AUDIO_AAC
    Mkv_generateAacCodecPrivateData(AUDIO_MPEG_OBJECT_TYPE, audioTrackInfo.uFrequency, audioTrackInfo.uChannelNumber, &audioTrackInfo.pCodecPrivate,
                                    &audioTrackInfo.uCodecPrivateLen);
#endif
#endif

#ifdef KVS_USE_POOL_ALLOCATOR
    poolAllocatorInit((void*) pMemPool, sizeof(pMemPool));
#endif

    /* Resolve KVS stream name */
    pKvsStreamName = (argc >= 2) ? argv[1] : KVS_STREAM_NAME;

    if ((kvsAppHandle = KvsApp_create(OptCfg_getHostKinesisVideo(), OptCfg_getRegion(), OptCfg_getServiceKinesisVideo(), pKvsStreamName)) == NULL) {
        printf("Failed to initialize KVS\n");
    } else if ((videoCapturerHandle = videoCapturerCreate()) == NULL) {
        printf("Failed to create video capturer\n");
    } else if (videoCapturerSetFormat(videoCapturerHandle, VID_FMT_H264, VID_RES_720P)) {
        printf("Failed to set video format\n");
    } else if (pthread_create(&videoThreadTid, NULL, videoThread, kvsAppHandle)) {
        printf("Failed to create video thread\n");
    }
#if ENABLE_AUDIO_TRACK
    else if ((audioCapturerHandle = audioCapturerCreate()) == NULL) {
        printf("Failed to create audio capturer\n");
    } else if (audioCapturerSetFormat(audioCapturerHandle, AUD_FMT_G711A, AUD_CHN_MONO, AUD_SAM_8K, AUD_BIT_16)) {
        printf("Failed to set audio format\n");
    } else if (pthread_create(&audioThreadTid, NULL, audioThread, kvsAppHandle)) {
        printf("Failed to create audio thread\n");
    }
#endif /* ENABLE_AUDIO_TRACK */
    else if (setKvsAppOptions(kvsAppHandle) != ERRNO_NONE) {
        printf("Failed to set options\n");
    } else {
        while (1) {
            /* FIXME: Check if network is reachable before running KVS. */

            if (KvsApp_open(kvsAppHandle) != 0) {
                printf("Failed to open KVS app\n");
                break;
            } else {
                printf("KvsApp opened\n");
            }

            while (1) {
                if (KvsApp_doWork(kvsAppHandle) != 0) {
                    break;
                }

                if (getEpochTimestampInMs() > uLastPrintMemStatTimestamp + 1000) {
                    printf("Buffer memory used: %zu\n", KvsApp_getStreamMemStatTotal(kvsAppHandle));
                    uLastPrintMemStatTimestamp = getEpochTimestampInMs();

#ifdef KVS_USE_POOL_ALLOCATOR
                    PoolStats_t stats = {0};
                    poolAllocatorGetStats(&stats);
                    printf("Sum of used/free memory:%zu/%zu, size of largest used/free block:%zu/%zu, number of used/free blocks:%zu/%zu\n",
                           stats.uSumOfUsedMemory, stats.uSumOfFreeMemory, stats.uSizeOfLargestUsedBlock, stats.uSizeOfLargestFreeBlock,
                           stats.uNumberOfUsedBlocks, stats.uNumberOfFreeBlocks);
#endif
                }
            }

            if (KvsApp_close(kvsAppHandle) != 0) {
                printf("Failed to close KVS app\n");
                break;
            } else {
                printf("KvsApp closed\n");
#if DEBUG_STORE_MEDIA_TO_FILE
                if (fpDbgMedia != NULL) {
                    fclose(fpDbgMedia);
                    fpDbgMedia = NULL;
                    printf("Closed debug file\n");
                }
#endif
            }
        }
    }

    KvsApp_close(kvsAppHandle);

    videoCapturerDestory(videoCapturerHandle);
#if ENABLE_AUDIO_TRACK
    audioCapturerDestory(audioCapturerHandle);
#endif

    KvsApp_terminate(kvsAppHandle);

#ifdef KVS_USE_POOL_ALLOCATOR
    poolAllocatorDeinit();
#endif

    return 0;
}