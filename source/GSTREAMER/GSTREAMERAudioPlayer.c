/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include "GSTREAMERCommon.h"
#include "GSTREAMERPort.h"
#include "com/amazonaws/kinesis/video/player/AudioPlayer.h"

#define GST_HANDLE_GET(x) GstAudioPlayer* audioHandle = (GstAudioPlayer*) ((x))

typedef struct {
    AudioPlayerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} GstAudioPlayer;

static int setStatus(AudioPlayerHandle handle, const AudioPlayerStatus newStatus)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    if (newStatus != audioHandle->status) {
        audioHandle->status = newStatus;
        LOG("AudioPlayer set new status[%d]", newStatus);
    }

    return 0;
}

AudioPlayerHandle audioPlayerCreate(void)
{
    GstAudioPlayer* audioHandle = NULL;

    if (!(audioHandle = (GstAudioPlayer*) malloc(sizeof(GstAudioPlayer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(audioHandle, 0, sizeof(GstAudioPlayer));

    audioHandle->capability.formats = (1 << (AUD_FMT_G711A - 1));
    audioHandle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    audioHandle->capability.sampleRates = (1 << (AUD_SAM_8K - 1));
    audioHandle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    if (0 != initGstAudioPlayer(NULL)) {
        LOG("initGstAudioPlayer failed.");
        return NULL;
    }

    setStatus((AudioPlayerHandle) audioHandle, AUD_PLY_STATUS_STREAM_OFF);

    return (AudioPlayerHandle) audioHandle;
}

AudioPlayerStatus audioPlayerGetStatus(const AudioPlayerHandle handle)
{
    if (!handle) {
        return AUD_PLY_STATUS_NOT_READY;
    }

    GST_HANDLE_GET(handle);
    return audioHandle->status;
}

int audioPlayerGetCapability(const AudioPlayerHandle handle, AudioCapability* pCapability)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = audioHandle->capability;
    return 0;
}

int audioPlayerSetFormat(AudioPlayerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                         const AudioBitDepth bitDepth)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(audioHandle, AUD_PLY_STATUS_STREAM_OFF);

    switch (format) {
        case AUD_FMT_G711A:
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

    audioHandle->format = format;
    audioHandle->channel = channel;
    audioHandle->sampleRate = sampleRate;
    audioHandle->bitDepth = bitDepth;

    return 0;
}

int audioPlayerGetFormat(const AudioPlayerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                         AudioBitDepth* pBitDepth)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    *pFormat = audioHandle->format;
    *pChannel = audioHandle->channel;
    *pSampleRate = audioHandle->sampleRate;
    *pBitDepth = audioHandle->bitDepth;

    return 0;
}

int audioPlayerAcquireStream(AudioPlayerHandle handle)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    startGstAudioPlayer();

    return setStatus(handle, AUD_PLY_STATUS_STREAM_ON);
}

int audioPlayerWriteFrame(AudioPlayerHandle handle, void* pData, const size_t size)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(audioHandle, AUD_PLY_STATUS_STREAM_ON);

    if (!pData || !size) {
        return -EINVAL;
    }

    int ret = writeToGstAudioPlayer(pData, size);

    return ret;
}

int audioPlayerReleaseStream(AudioPlayerHandle handle)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(audioHandle, AUD_PLY_STATUS_STREAM_ON);

    stopGstAudioPlayer();

    return setStatus(handle, AUD_PLY_STATUS_STREAM_OFF);
}

void audioPlayerDestroy(AudioPlayerHandle handle)
{
    if (!handle) {
        return;
    }
    GST_HANDLE_GET(handle);

    deinitGstAudioPlayer();

    setStatus(handle, AUD_PLY_STATUS_NOT_READY);

    free(handle);
}
