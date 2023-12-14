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

#include "com/amazonaws/kinesis/video/player/AudioPlayer.h"

#include "C302Common.h"
#include "ipc_audio_api.h"

#define USING_HARD_STREAM_AUDIO

#define C302_HANDLE_GET(x) C302AudioPlayer* audioHandle = (C302AudioPlayer*) ((x))

#define DEFAULT_VOLUME 31

typedef struct {
    AudioPlayerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
    IPC_AUDIO_FRAME aframe;
} C302AudioPlayer;

static int setStatus(AudioPlayerHandle handle, const AudioPlayerStatus newStatus)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    if (newStatus != audioHandle->status) {
        audioHandle->status = newStatus;
        KVS_LOG("AudioPlayer set new status[%d]\n", newStatus);
    }

    return 0;
}

AudioPlayerHandle audioPlayerCreate(void)
{
    C302AudioPlayer* audioHandle = NULL;

    if (!(audioHandle = (C302AudioPlayer*) malloc(sizeof(C302AudioPlayer)))) {
        KVS_LOG("OOM");
        return NULL;
    }

    memset(audioHandle, 0, sizeof(C302AudioPlayer));

    audioHandle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_G711U - 1));
    audioHandle->capability.channels = (1 << (AUD_CHN_MONO - 1)) | (1 << (AUD_CHN_STEREO - 1));
    audioHandle->capability.sampleRates = (1 << (AUD_SAM_8K - 1)) | (1 << (AUD_SAM_16K - 1)) | (1 << (AUD_SAM_32K - 1)) | (1 << (AUD_SAM_48K - 1));
    audioHandle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioPlayerHandle) audioHandle, AUD_PLY_STATUS_STREAM_OFF);

#ifdef USING_HARD_STREAM_AUDIO
    if (IPC_CFG_Init(CFG_AO_FLAG) < 0) {
        KVS_LOG("IPC_CFG_Init AO err\n");
        return NULL;
    }
#endif

    return (AudioPlayerHandle) audioHandle;
}

AudioPlayerStatus audioPlayerGetStatus(const AudioPlayerHandle handle)
{
    if (!handle) {
        return AUD_PLY_STATUS_NOT_READY;
    }

    C302_HANDLE_GET(handle);
    return audioHandle->status;
}

int audioPlayerGetCapability(const AudioPlayerHandle handle, AudioCapability* pCapability)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = audioHandle->capability;
    return 0;
}

int audioPlayerSetFormat(AudioPlayerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                         const AudioBitDepth bitDepth)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    HANDLE_STATUS_CHECK(audioHandle, AUD_PLY_STATUS_STREAM_OFF);

#ifdef USING_HARD_STREAM_AUDIO
    unsigned int enc_type;
    unsigned int channels;
    unsigned int sample_rate;
    unsigned int bit_width;

    switch (format) {
        case AUD_FMT_G711A:
            enc_type = CODEC_G711A;
            break;
        case AUD_FMT_G711U:
            enc_type = CODEC_G711U;
            break;
        default:
            KVS_LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    switch (channel) {
        case AUD_CHN_MONO:
            channels = 1;
            break;
        case AUD_CHN_STEREO:
            channels = 2;
            break;
        default:
            KVS_LOG("Unsupported channel num %d", channel);
            return -EINVAL;
    }

    switch (sampleRate) {
        case AUD_SAM_8K:
            sample_rate = AUDIO_SAMPLERATE_8K;
            break;
        case AUD_SAM_16K:
            sample_rate = AUDIO_SAMPLERATE_16K;
            break;
        case AUD_SAM_32K:
            sample_rate = AUDIO_SAMPLERATE_32K;
            break;
        case AUD_SAM_48K:
            sample_rate = AUDIO_SAMPLERATE_48K;
            break;
        default:
            KVS_LOG("Unsupported sample rate %d", sampleRate);
            return -EINVAL;
    }

    switch (bitDepth) {
        case AUD_BIT_16:
            bit_width = AUDIO_BITWIDTH_16;
            break;
        default:
            KVS_LOG("Unsupported bit depth %d", bitDepth);
            return -EINVAL;
    }

    int ret = IPC_AUDIO_SetConfig(CFG_AO_FLAG, enc_type, channels, sample_rate, bit_width, DEFAULT_VOLUME);
    if (ret < 0) {
        KVS_LOG("IPC_AUDIO_SetConfig AO failed: %d\n", ret);
        IPC_CFG_UnInit(CFG_AO_FLAG);
        return -EAGAIN;
    }
    ret = IPC_AUDIO_Init(CFG_AO_FLAG);
    if (ret < 0) {
        KVS_LOG("IPC_AUDIO_Init AO failed: %d\n", ret);
        IPC_CFG_UnInit(CFG_AO_FLAG);
        return -EAGAIN;
    }
#endif

    audioHandle->format = format;
    audioHandle->channel = channel;
    audioHandle->sampleRate = sampleRate;
    audioHandle->bitDepth = bitDepth;

    return 0;
}

int audioPlayerGetFormat(const AudioPlayerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                         AudioBitDepth* pBitDepth)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    *pFormat = audioHandle->format;
    *pChannel = audioHandle->channel;
    *pSampleRate = audioHandle->sampleRate;
    *pBitDepth = audioHandle->bitDepth;

    return 0;
}

int audioPlayerAcquireStream(AudioPlayerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_AUDIO
    int ret = IPC_AUDIO_Enable(CFG_AO_FLAG);
    if (ret < 0) {
        KVS_LOG("AO device enable failed");
        IPC_AUDIO_UnInit(CFG_AO_FLAG);
        IPC_CFG_UnInit(CFG_AO_FLAG);
        return -EAGAIN;
    }
    IPC_AFRAME_Init(&audioHandle->aframe);
    audioHandle->aframe.u32len = IPC_AUDIO_GetFrmSize();
    ret = IPC_AFRAME_Alloc(&audioHandle->aframe, audioHandle->aframe.u32len);
    if (ret < 0) {
        KVS_LOG("AO device alloc failed");
        IPC_AUDIO_Disable(CFG_AO_FLAG);
        IPC_AUDIO_UnInit(CFG_AO_FLAG);
        IPC_CFG_UnInit(CFG_AO_FLAG);
        return -EAGAIN;
    }
#endif

    return setStatus(audioHandle, AUD_PLY_STATUS_STREAM_ON);
}

int audioPlayerWriteFrame(AudioPlayerHandle handle, void* pData, const size_t size)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    HANDLE_STATUS_CHECK(audioHandle, AUD_PLY_STATUS_STREAM_ON);

    if (!pData || !size) {
        return -EINVAL;
    }

#ifdef USING_HARD_STREAM_AUDIO
    int ret = 0;

    if (size > audioHandle->aframe.u32len) {
        KVS_LOG("AO device buffer too small");
        return -EAGAIN;
    }

    memcpy(audioHandle->aframe.u8data, pData, size);
    ret = IPC_AUDIO_SendFrame(&audioHandle->aframe);
    if (ret < 0) {
        KVS_LOG("IPC_AUDIO_SendFrame failed");
    }
#endif
    return 0;
}

int audioPlayerReleaseStream(AudioPlayerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_AUDIO
    IPC_AFRAME_Release(&audioHandle->aframe);
    IPC_AUDIO_Disable(CFG_AO_FLAG);
#endif

    return setStatus(audioHandle, AUD_PLY_STATUS_STREAM_OFF);
}

void audioPlayerDestroy(AudioPlayerHandle handle)
{
    if (!handle) {
        return;
    }
    C302_HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_AUDIO
    IPC_AFRAME_Release(&audioHandle->aframe);
    if (audioHandle->status == AUD_PLY_STATUS_STREAM_ON) {
        IPC_AUDIO_Disable(CFG_AO_FLAG);
    }
    IPC_AUDIO_UnInit(CFG_AO_FLAG);
    IPC_CFG_UnInit(CFG_AO_FLAG);
#endif

    setStatus(audioHandle, AUD_PLY_STATUS_NOT_READY);

    free(audioHandle);
}
