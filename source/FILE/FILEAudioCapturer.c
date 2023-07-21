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
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "FILECommon.h"
#include "FILEPort.h"
#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"

#define FRAME_FILE_POSTFIX_AAC       ".aac"
#define FRAME_FILE_POSTFIX_G711A     ".alaw"
#define FRAME_FILE_START_INDEX_AAC   (1)
#define FRAME_FILE_START_INDEX_G711A (1)
#define FRAME_FILE_END_INDEX_AAC     (760)
#define FRAME_FILE_END_INDEX_G711A   (999)
#define FRAME_FILE_PATH_FORMAT_AAC   FRAME_FILE_PATH_PREFIX "aac/frame-%03d" FRAME_FILE_POSTFIX_AAC
#define FRAME_FILE_PATH_FORMAT_G711A FRAME_FILE_PATH_PREFIX "g711a/frame-%03d" FRAME_FILE_POSTFIX_G711A
#define FRAME_FILE_DURATION_US_AAC   (1000 * 1000 / 48UL)
#define FRAME_FILE_DURATION_US_G711A (1000 * 1000 / 25UL)

#define FILE_HANDLE_GET(x) FILEAudioCapturer* fileHandle = (FILEAudioCapturer*) ((x))

typedef struct {
    AudioCapturerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
    char* framePathFormat;
    size_t frameIndex;
    size_t frameIndexStart;
    size_t frameIndexEnd;
    size_t frameDurationUs;
    FILE* frameFile;
} FILEAudioCapturer;

static int setStatus(AudioCapturerHandle handle, const AudioCapturerStatus newStatus)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    if (newStatus != fileHandle->status) {
        fileHandle->status = newStatus;
        LOG("AudioCapturer new status[%d]", newStatus);
    }

    return 0;
}

AudioCapturerHandle audioCapturerCreate(void)
{
    FILEAudioCapturer* fileHandle = NULL;

    if (!(fileHandle = (FILEAudioCapturer*) malloc(sizeof(FILEAudioCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(fileHandle, 0, sizeof(FILEAudioCapturer));

    // Now we have sample frames for G.711 ALAW and AAC, MONO, 8k, 16 bits
    fileHandle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_AAC - 1));
    fileHandle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    fileHandle->capability.sampleRates = (1 << (AUD_SAM_8K - 1));
    fileHandle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioCapturerHandle) fileHandle, AUD_CAP_STATUS_STREAM_OFF);

    return (AudioCapturerHandle) fileHandle;
}

AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle handle)
{
    if (!handle) {
        return AUD_CAP_STATUS_NOT_READY;
    }

    FILE_HANDLE_GET(handle);
    return fileHandle->status;
}

int audioCapturerGetCapability(const AudioCapturerHandle handle, AudioCapability* pCapability)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = fileHandle->capability;

    return 0;
}

int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    FILE_HANDLE_STATUS_CHECK(fileHandle, AUD_CAP_STATUS_STREAM_OFF);

    switch (format) {
        case AUD_FMT_G711A:
            fileHandle->framePathFormat = FRAME_FILE_PATH_FORMAT_G711A;
            fileHandle->frameIndexStart = FRAME_FILE_START_INDEX_G711A;
            fileHandle->frameIndexEnd = FRAME_FILE_END_INDEX_G711A;
            fileHandle->frameDurationUs = FRAME_FILE_DURATION_US_G711A;
            break;
        case AUD_FMT_AAC:
            fileHandle->framePathFormat = FRAME_FILE_PATH_FORMAT_AAC;
            fileHandle->frameIndexStart = FRAME_FILE_START_INDEX_AAC;
            fileHandle->frameIndexEnd = FRAME_FILE_END_INDEX_AAC;
            fileHandle->frameDurationUs = FRAME_FILE_DURATION_US_AAC;
            break;

        default:
            LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    switch (channel) {
        case AUD_CHN_MONO:
            break;

        default:
            LOG("Unsupported channel num %d", channel);
            return -EINVAL;
    }

    switch (sampleRate) {
        case AUD_SAM_8K:
            break;

        default:
            LOG("Unsupported sample rate %d", sampleRate);
            return -EINVAL;
    }

    switch (bitDepth) {
        case AUD_BIT_16:
            break;

        default:
            LOG("Unsupported bit depth %d", bitDepth);
            return -EINVAL;
    }

    fileHandle->format = format;
    fileHandle->channel = channel;
    fileHandle->sampleRate = sampleRate;
    fileHandle->bitDepth = bitDepth;

    return 0;
}

int audioCapturerGetFormat(const AudioCapturerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                           AudioBitDepth* pBitDepth)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    *pFormat = fileHandle->format;
    *pChannel = fileHandle->channel;
    *pSampleRate = fileHandle->sampleRate;
    *pBitDepth = fileHandle->bitDepth;

    return 0;
}

int audioCapturerAcquireStream(AudioCapturerHandle handle)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    fileHandle->frameIndex = fileHandle->frameIndexStart;

    return setStatus(handle, AUD_CAP_STATUS_STREAM_ON);
}

int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    FILE_HANDLE_STATUS_CHECK(fileHandle, AUD_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    int ret = 0;

    if (fileHandle->frameFile) {
        CLOSE_FILE(fileHandle->frameFile);
    }

    char filePath[FRAME_FILE_PATH_MAX_LENGTH] = {0};
    snprintf(filePath, FRAME_FILE_PATH_MAX_LENGTH, fileHandle->framePathFormat, fileHandle->frameIndex);
    if (fileHandle->frameIndex < fileHandle->frameIndexEnd) {
        fileHandle->frameIndex++;
    } else {
        fileHandle->frameIndex = fileHandle->frameIndexStart;
    }

    size_t frameSize = 0;
    fileHandle->frameFile = fopen(filePath, "r");
    if (fileHandle->frameFile) {
        GET_FILE_SIZE(fileHandle->frameFile, frameSize);
        if (frameSize >= 0) {
            if (frameDataBufferSize >= frameSize) {
                *pFrameSize = fread(pFrameDataBuffer, 1, frameSize, fileHandle->frameFile);
                *pTimestamp = getEpochTimestampInUs();
            } else {
                LOG("FrameDataBufferSize(%ld) < frameSize(%ld), frame dropped", frameDataBufferSize, frameSize);
                *pFrameSize = 0;
                ret = -ENOMEM;
            }
        } else {
            LOG("Failed to get size of file %s", filePath);
            ret = -EAGAIN;
        }
        CLOSE_FILE(fileHandle->frameFile);
    } else {
        LOG("Failed to open file %s", filePath);
        ret = -EAGAIN;
    }

    usleep(fileHandle->frameDurationUs);

    return ret;
}

int audioCapturerReleaseStream(AudioCapturerHandle handle)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    FILE_HANDLE_STATUS_CHECK(fileHandle, AUD_CAP_STATUS_STREAM_ON);

    if (fileHandle->frameFile) {
        CLOSE_FILE(fileHandle->frameFile);
    }

    return setStatus(handle, AUD_CAP_STATUS_STREAM_OFF);
}

void audioCapturerDestroy(AudioCapturerHandle handle)
{
    if (!handle) {
        return;
    }

    FILE_HANDLE_GET(handle);

    if (fileHandle->status == AUD_CAP_STATUS_STREAM_ON) {
        audioCapturerReleaseStream(handle);
    }

    setStatus(handle, AUD_CAP_STATUS_NOT_READY);

    free(handle);
}
