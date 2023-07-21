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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "AK3918Common.h"
#include "com/amazonaws/kinesis/video/player/AudioPlayer.h"

#include "fun_stream_mgr.h"

#define AK3918_HANDLE_GET(x) AK3918AudioPlayer* AK3918Handle = (AK3918AudioPlayer*) ((x))

typedef struct {
    AudioPlayerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} AK3918AudioPlayer;

static int setStatus(AudioPlayerHandle handle, const AudioPlayerStatus newStatus)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    if (newStatus != AK3918Handle->status) {
        AK3918Handle->status = newStatus;
        LOG("AudioPlayer new status[%d]", newStatus);
    }

    return 0;
}

AudioPlayerHandle audioPlayerCreate(void)
{
    AK3918AudioPlayer* AK3918Handle = NULL;
    if (!(AK3918Handle = (AK3918AudioPlayer*) malloc(sizeof(AK3918AudioPlayer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(AK3918Handle, 0, sizeof(AK3918AudioPlayer));

    // Now implementation supports raw PCM, G.711 ALAW and ULAW, MONO, 8k, 16 bits
    AK3918Handle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_G711U - 1)) | (1 << (AUD_FMT_PCM - 1));
    AK3918Handle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    AK3918Handle->capability.sampleRates = (1 << (AUD_SAM_8K - 1));
    AK3918Handle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioPlayerHandle) AK3918Handle, AUD_PLY_STATUS_STREAM_OFF);

    return (AudioPlayerHandle) AK3918Handle;
}

AudioPlayerStatus audioPlayerGetStatus(const AudioPlayerHandle handle)
{
    if (!handle) {
        return AUD_PLY_STATUS_NOT_READY;
    }

    AK3918_HANDLE_GET(handle);

    return AK3918Handle->status;
}

int audioPlayerGetCapability(const AudioPlayerHandle handle, AudioCapability* pCapability)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = AK3918Handle->capability;

    return 0;
}

int audioPlayerSetFormat(AudioPlayerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                         const AudioBitDepth bitDepth)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    AK3918_HANDLE_STATUS_CHECK(AK3918Handle, AUD_PLY_STATUS_STREAM_OFF);

    switch (format) {
        case AUD_FMT_PCM:

            break;

        case AUD_FMT_G711A:

            break;

        case AUD_FMT_G711U:

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

    AK3918Handle->format = format;
    AK3918Handle->channel = channel;
    AK3918Handle->sampleRate = sampleRate;
    AK3918Handle->bitDepth = bitDepth;

    return 0;
}

int audioPlayerGetFormat(const AudioPlayerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                         AudioBitDepth* pBitDepth)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    *pFormat = AK3918Handle->format;
    *pChannel = AK3918Handle->channel;
    *pSampleRate = AK3918Handle->sampleRate;
    *pBitDepth = AK3918Handle->bitDepth;

    return 0;
}

int audioPlayerAcquireStream(AudioPlayerHandle handle)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    return setStatus(handle, AUD_PLY_STATUS_STREAM_ON);
}

int audioPlayerWriteFrame(AudioPlayerHandle handle, void* pData, const size_t size)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    AK3918_HANDLE_STATUS_CHECK(AK3918Handle, AUD_PLY_STATUS_STREAM_ON);
    static FILE* ad = NULL;
    if (!pData) {
        return -EINVAL;
    }
    if (AK3918Handle->format == AUD_FMT_G711A) {
        int iRet = AA_LS_SendVoice(0, pData, size);
        if (iRet) {
            LOG("SendVoice failed!\n");
        }
    }

    return 0;
}

int audioPlayerReleaseStream(AudioPlayerHandle handle)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    return setStatus(handle, AUD_PLY_STATUS_STREAM_OFF);
}

void audioPlayerDestroy(AudioPlayerHandle handle)
{
    if (!handle) {
        return;
    }

    AK3918_HANDLE_GET(handle);

    if (AK3918Handle->status == AUD_PLY_STATUS_STREAM_ON) {
        audioPlayerReleaseStream(handle);
    }

    setStatus(handle, AUD_PLY_STATUS_NOT_READY);

    free(handle);
}
