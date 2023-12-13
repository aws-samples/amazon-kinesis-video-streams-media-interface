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

#include "Samples.h"

#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"
#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"
#include "com/amazonaws/kinesis/video/player/AudioPlayer.h"

#define VIDEO_FRAME_BUFFER_SIZE_BYTES      (256 * 1024UL)
#define AUDIO_FRAME_BUFFER_SIZE_BYTES      (1024UL)
#define HUNDREDS_OF_NANOS_IN_A_MICROSECOND 10LL

extern PSampleConfiguration gSampleConfiguration;
static AudioCapturerHandle audioCapturerHandle = NULL;
static AudioPlayerHandle audioPlayerHandle = NULL;
static VideoCapturerHandle videoCapturerHandle = NULL;

static void sessionOnShutdown(UINT64 customData, PSampleStreamingSession pSampleStreamingSession)
{
    DLOGD("Shut down session %s\n", pSampleStreamingSession->peerId);
    audioPlayerReleaseStream(audioPlayerHandle);
}

static void remoteAudioFrameHandler(UINT64 customData, PFrame pFrame)
{
    PSampleStreamingSession pSampleStreamingSession = (PSampleStreamingSession) customData;

    DLOGV("Frame received. TrackId: %" PRIu64 ", Size: %u, Flags %u", pFrame->trackId, pFrame->size, pFrame->flags);

    if (pSampleStreamingSession->firstFrame) {
        pSampleStreamingSession->firstFrame = FALSE;
        pSampleStreamingSession->startUpLatency = (GETTIME() - pSampleStreamingSession->offerReceiveTime) / HUNDREDS_OF_NANOS_IN_A_MILLISECOND;
        DLOGI("Start up latency from offer to first frame: %" PRIu64 "ms\n", pSampleStreamingSession->startUpLatency);

        streamingSessionOnShutdown(pSampleStreamingSession, NULL, sessionOnShutdown);

        if (audioPlayerAcquireStream(audioPlayerHandle)) {
            DLOGE("audioPlayerAcquireStream failed");
        }
    }

    audioPlayerWriteFrame(audioPlayerHandle, pFrame->frameData, pFrame->size);
}

static void writeFrameToAllSessions(const UINT64 timestamp, PVOID pData, const SIZE_T size, const PCHAR const trackId)
{
    STATUS status = STATUS_SUCCESS;
    BOOL isVideo = TRUE;
    Frame frame = {
        .presentationTs = timestamp,
        .frameData = pData,
        .size = size,
    };

    if (!STRNCMP(trackId, SAMPLE_VIDEO_TRACK_ID, STRLEN(SAMPLE_VIDEO_TRACK_ID))) {
        isVideo = TRUE;
    } else if (!STRNCMP(trackId, SAMPLE_AUDIO_TRACK_ID, STRLEN(SAMPLE_AUDIO_TRACK_ID))) {
        isVideo = FALSE;
    } else {
        DLOGE("unknown trackId: %s", trackId);
        return;
    }

    MUTEX_LOCK(gSampleConfiguration->streamingSessionListReadLock);
    for (int i = 0; i < gSampleConfiguration->streamingSessionCount; ++i) {
        if (isVideo) {
            status = writeFrame(gSampleConfiguration->sampleStreamingSessionList[i]->pVideoRtcRtpTransceiver, &frame);
        } else {
            status = writeFrame(gSampleConfiguration->sampleStreamingSessionList[i]->pAudioRtcRtpTransceiver, &frame);
        }
        if (status != STATUS_SRTP_NOT_READY_YET) {
            if (status != STATUS_SUCCESS) {
                DLOGV("writeFrame() failed with 0x%08x", status);
            } else if (gSampleConfiguration->sampleStreamingSessionList[i]->firstFrame && status == STATUS_SUCCESS) {
                PROFILE_WITH_START_TIME(gSampleConfiguration->sampleStreamingSessionList[i]->offerReceiveTime, "Time to first frame");
                gSampleConfiguration->sampleStreamingSessionList[i]->firstFrame = FALSE;
            }
        }
    }
    MUTEX_UNLOCK(gSampleConfiguration->streamingSessionListReadLock);
}

INT32 main(INT32 argc, CHAR* argv[])
{
    STATUS retStatus = STATUS_SUCCESS;
    UINT32 frameSize;
    PSampleConfiguration pSampleConfiguration = NULL;
    PCHAR pChannelName;
    SignalingClientMetrics signalingClientMetrics;
    signalingClientMetrics.version = SIGNALING_CLIENT_METRICS_CURRENT_VERSION;

    SET_INSTRUMENTED_ALLOCATORS();
    UINT32 logLevel = setLogLevel();

#ifndef _WIN32
    signal(SIGINT, sigintHandler);
#endif

    /* Media Interface Construct */
    AudioCapability audioCapability = {0};
    audioCapturerHandle = audioCapturerCreate();
    if (!audioCapturerHandle) {
    } else {
        audioCapturerGetCapability(audioCapturerHandle, &audioCapability);
        DLOGD("AudioCapturer Capability: formats[%x], channels[%x], sampleRates[%x], bitDepths[%x]", audioCapability.formats,
              audioCapability.channels, audioCapability.sampleRates, audioCapability.bitDepths);
        if (audioCapturerSetFormat(audioCapturerHandle, AUD_FMT_G711A, AUD_CHN_MONO, AUD_SAM_8K, AUD_BIT_16)) {
            DLOGE("Unable to set AudioCapturer format");
            audioCapturerDestroy(audioCapturerHandle);
            audioCapturerHandle = NULL;
        }
    }
    DLOGI("Board AudioCapturer initialized");

    audioPlayerHandle = audioPlayerCreate();
    if (!audioPlayerHandle) {
        DLOGE("AudioPlayer init failed");
    } else {
        audioPlayerGetCapability(audioPlayerHandle, &audioCapability);
        DLOGD("AudioPlayer Capability: formats[%x], channels[%x], sampleRates[%x], bitDepths[%x]", audioCapability.formats, audioCapability.channels,
              audioCapability.sampleRates, audioCapability.bitDepths);

        if (audioPlayerSetFormat(audioPlayerHandle, AUD_FMT_G711A, AUD_CHN_MONO, AUD_SAM_8K, AUD_BIT_16)) {
            DLOGE("Unable to set AudioPlayer format");
            audioPlayerDestroy(audioPlayerHandle);
            audioPlayerHandle = NULL;
        }
    }
    DLOGI("Board AudioPlayer initialized\n");

    videoCapturerHandle = videoCapturerCreate();
    CHK_ERR(videoCapturerHandle, STATUS_INVALID_OPERATION, "VideoCapturer init failed");

    VideoCapability videoCapability = {0};
    videoCapturerGetCapability(videoCapturerHandle, &videoCapability);
    DLOGD("VudioCapturerCapability: formats[%x], resolutions[%x]", videoCapability.formats, videoCapability.resolutions);

    CHK_STATUS_ERR(videoCapturerSetFormat(videoCapturerHandle, VID_FMT_H264, VID_RES_1080P), STATUS_INVALID_OPERATION, "Unable to set video format");
    DLOGI("Board VideoCapturer initialized");
    /* Media Interface Construct */

    // do trickleIce by default
    printf("[KVS Master] Using trickleICE by default\n");

#ifdef IOT_CORE_ENABLE_CREDENTIALS
    CHK_ERR((pChannelName = getenv(IOT_CORE_THING_NAME)) != NULL, STATUS_INVALID_OPERATION, "AWS_IOT_CORE_THING_NAME must be set");
#else
    pChannelName = argc > 1 ? argv[1] : SAMPLE_CHANNEL_NAME;
#endif

    CHK_STATUS(createSampleConfiguration(pChannelName, SIGNALING_CHANNEL_ROLE_TYPE_MASTER, TRUE, TRUE, logLevel, &pSampleConfiguration));

    // Set the audio and video handlers
    if (videoCapturerHandle) {
        pSampleConfiguration->videoSource = sendVideoPackets;
    }
    if (audioCapturerHandle) {
        pSampleConfiguration->audioSource = sendAudioPackets;
        pSampleConfiguration->mediaType = SAMPLE_STREAMING_AUDIO_VIDEO;
    } else {
        pSampleConfiguration->mediaType = SAMPLE_STREAMING_VIDEO_ONLY;
    }
    if (audioPlayerHandle) {
        pSampleConfiguration->receiveAudioVideoSource = sampleReceiveAudioVideoFrame;
    }
    pSampleConfiguration->onDataChannel = onDataChannel;
    DLOGI("[KVS Master] Finished setting handlers");

    // Initialize KVS WebRTC. This must be done before anything else, and must only be done once.
    CHK_STATUS(initKvsWebRtc());
    DLOGI("[KVS Master] KVS WebRTC initialization completed successfully");

    CHK_STATUS(initSignaling(pSampleConfiguration, SAMPLE_MASTER_CLIENT_ID));
    DLOGI("[KVS Master] Channel %s set up done ", pChannelName);

    // Checking for termination
    CHK_STATUS(sessionCleanupWait(pSampleConfiguration));
    DLOGI("[KVS Master] Streaming session terminated");

CleanUp:

    if (retStatus != STATUS_SUCCESS) {
        DLOGE("[KVS Master] Terminated with status code 0x%08x", retStatus);
    }

    DLOGI("[KVS Master] Cleaning up....");
    if (pSampleConfiguration != NULL) {
        // Kick of the termination sequence
        ATOMIC_STORE_BOOL(&pSampleConfiguration->appTerminateFlag, TRUE);

        if (pSampleConfiguration->mediaSenderTid != INVALID_TID_VALUE) {
            THREAD_JOIN(pSampleConfiguration->mediaSenderTid, NULL);
        }

        retStatus = signalingClientGetMetrics(pSampleConfiguration->signalingClientHandle, &signalingClientMetrics);
        if (retStatus == STATUS_SUCCESS) {
            logSignalingClientStats(&signalingClientMetrics);
        } else {
            DLOGE("[KVS Master] signalingClientGetMetrics() operation returned status code: 0x%08x", retStatus);
        }
        retStatus = freeSignalingClient(&pSampleConfiguration->signalingClientHandle);
        if (retStatus != STATUS_SUCCESS) {
            DLOGE("[KVS Master] freeSignalingClient(): operation returned status code: 0x%08x", retStatus);
        }

        retStatus = freeSampleConfiguration(&pSampleConfiguration);
        if (retStatus != STATUS_SUCCESS) {
            DLOGE("[KVS Master] freeSampleConfiguration(): operation returned status code: 0x%08x", retStatus);
        }
    }
    DLOGI("[KVS Master] Cleanup done");
    CHK_LOG_ERR(retStatus);

    /* Media Interface Destruct */
    if (audioCapturerHandle) {
        audioCapturerDestroy(audioCapturerHandle);
        audioCapturerHandle = NULL;
    }

    if (audioPlayerHandle) {
        audioPlayerDestroy(audioPlayerHandle);
        audioPlayerHandle = NULL;
    }

    if (videoCapturerHandle) {
        videoCapturerDestroy(videoCapturerHandle);
        videoCapturerHandle = NULL;
    }

    printf("Board SDK exited\n");
    /* Media Interface Destruct */

    RESET_INSTRUMENTED_ALLOCATORS();

    // https://www.gnu.org/software/libc/manual/html_node/Exit-Status.html
    // We can only return with 0 - 127. Some platforms treat exit code >= 128
    // to be a success code, which might give an unintended behaviour.
    // Some platforms also treat 1 or 0 differently, so it's better to use
    // EXIT_FAILURE and EXIT_SUCCESS macros for portability.
    return STATUS_FAILED(retStatus) ? EXIT_FAILURE : EXIT_SUCCESS;
}

PVOID sendVideoPackets(PVOID args)
{
    STATUS retStatus = STATUS_SUCCESS;
    PSampleConfiguration pSampleConfiguration = (PSampleConfiguration) args;
    void* pFrameBuffer = NULL;
    UINT64 timestamp = 0;
    SIZE_T frameSize = 0;

    CHK_ERR(pSampleConfiguration != NULL, STATUS_NULL_ARG, "[KVS Master] Streaming session is NULL");

    pFrameBuffer = MEMALLOC(VIDEO_FRAME_BUFFER_SIZE_BYTES);

    CHK_ERR(pFrameBuffer != NULL, STATUS_NOT_ENOUGH_MEMORY, "[KVS Master] OOM when allocating buffer");
    CHK_ERR(videoCapturerHandle != NULL, STATUS_NULL_ARG, "[KVS Master] VideoCapturerHandle is NULL");
    CHK_ERR(videoCapturerAcquireStream(videoCapturerHandle) == 0, STATUS_INVALID_OPERATION, "[KVS Master] Acquire video stream failed");

    int getFrameStatus = 0;
    while (!ATOMIC_LOAD_BOOL(&pSampleConfiguration->appTerminateFlag)) {
        getFrameStatus = videoCapturerGetFrame(videoCapturerHandle, pFrameBuffer, VIDEO_FRAME_BUFFER_SIZE_BYTES, &timestamp, &frameSize);
        switch (getFrameStatus) {
            case 0:
                // successfully get a frame
                writeFrameToAllSessions(timestamp * HUNDREDS_OF_NANOS_IN_A_MICROSECOND, pFrameBuffer, frameSize, SAMPLE_VIDEO_TRACK_ID);
                break;
            case -EAGAIN:
                // frame is not ready yet
                usleep(1000);
                break;
            default:
                DLOGE("videoCapturerGetFrame failed");
        }
    }

CleanUp:
    DLOGI("[KVS Master] Closing video thread");

    videoCapturerReleaseStream(videoCapturerHandle);

    CHK_LOG_ERR(retStatus);

    MEMFREE(pFrameBuffer);
    pFrameBuffer = NULL;

    return (PVOID) (ULONG_PTR) retStatus;
}

PVOID sendAudioPackets(PVOID args)
{
    STATUS retStatus = STATUS_SUCCESS;
    PSampleConfiguration pSampleConfiguration = (PSampleConfiguration) args;
    void* pFrameBuffer = NULL;
    UINT64 timestamp = 0;
    SIZE_T frameSize = 0;

    CHK_ERR(pSampleConfiguration != NULL, STATUS_NULL_ARG, "[KVS Master] Streaming session is NULL");

    pFrameBuffer = MEMALLOC(AUDIO_FRAME_BUFFER_SIZE_BYTES);

    CHK_ERR(pFrameBuffer != NULL, STATUS_NOT_ENOUGH_MEMORY, "[KVS Master] OOM when allocating buffer");
    CHK_ERR(audioCapturerHandle != NULL, STATUS_NULL_ARG, "[KVS Master] AudioCapturerHandle is NULL");
    CHK_ERR(audioCapturerAcquireStream(audioCapturerHandle) == 0, STATUS_INVALID_OPERATION, "[KVS Master] Acquire audio stream failed");

    int getFrameStatus = 0;
    while (!ATOMIC_LOAD_BOOL(&pSampleConfiguration->appTerminateFlag)) {
        getFrameStatus = audioCapturerGetFrame(audioCapturerHandle, pFrameBuffer, AUDIO_FRAME_BUFFER_SIZE_BYTES, &timestamp, &frameSize);
        switch (getFrameStatus) {
            case 0:
                // successfully get a frame
                writeFrameToAllSessions(timestamp * HUNDREDS_OF_NANOS_IN_A_MICROSECOND, pFrameBuffer, frameSize, SAMPLE_AUDIO_TRACK_ID);
                break;
            case -EAGAIN:
                // frame is not ready yet
                usleep(1000);
                break;
            default:
                DLOGE("audioCapturerGetFrame failed");
        }
    }

CleanUp:
    DLOGI("[KVS Master] closing audio thread");

    audioCapturerReleaseStream(audioCapturerHandle);

    CHK_LOG_ERR(retStatus);

    MEMFREE(pFrameBuffer);
    pFrameBuffer = NULL;

    return (PVOID) (ULONG_PTR) retStatus;
}

PVOID sampleReceiveAudioVideoFrame(PVOID args)
{
    STATUS retStatus = STATUS_SUCCESS;
    PSampleStreamingSession pSampleStreamingSession = (PSampleStreamingSession) args;
    CHK_ERR(pSampleStreamingSession != NULL, STATUS_NULL_ARG, "[KVS Master] Streaming session is NULL");
    CHK_STATUS(transceiverOnFrame(pSampleStreamingSession->pVideoRtcRtpTransceiver, (UINT64) pSampleStreamingSession, sampleVideoFrameHandler));
    CHK_STATUS(transceiverOnFrame(pSampleStreamingSession->pAudioRtcRtpTransceiver, (UINT64) pSampleStreamingSession, remoteAudioFrameHandler));

CleanUp:

    return (PVOID) (ULONG_PTR) retStatus;
}
