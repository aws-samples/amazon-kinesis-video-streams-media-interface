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
#include <stdlib.h>
#include <string.h>

#include "com/amazonaws/kinesis/video/player/AudioPlayer.h"

#include "T41Common.h"
#include <imp/imp_audio.h>
#include <imp/imp_system.h>

#define T41_SPK_DEV_ID      0
#define T41_SPK_DEV_CHN_ID  0
#define T41_SPK_DEC_CHN_ID  0
#define T41_SPK_DEFAULT_VOL 60

#define T41_HANDLE_GET(x) T41AudioPlayer* T41Handle = (T41AudioPlayer*) ((x))

typedef struct {
    AudioPlayerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} T41AudioPlayer;

static int setStatus(AudioPlayerHandle handle, const AudioPlayerStatus newStatus)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (newStatus != T41Handle->status) {
        T41Handle->status = newStatus;

        LOG("AudioPlayer new status[%d]", newStatus);
    }

    return 0;
}

static int playFrame(AudioPlayerHandle handle, IMPAudioFrame* frame)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);
    T41_HANDLE_STATUS_CHECK(T41Handle, AUD_PLY_STATUS_STREAM_ON);

    if (!frame) {
        return -EINVAL;
    }

    if (IMP_AO_SendFrame(T41_SPK_DEV_ID, T41_SPK_DEV_CHN_ID, frame, BLOCK)) {
        LOG("IMP_AO_SendFrame failed");
        return -EINVAL;
    }

    return 0;
}

AudioPlayerHandle audioPlayerCreate(void)
{
    T41AudioPlayer* T41Handle = NULL;

    if (!(T41Handle = (T41AudioPlayer*) malloc(sizeof(T41AudioPlayer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(T41Handle, 0, sizeof(T41AudioPlayer));

    // Now implementation supports raw PCM, G.711 ALAW and ULAW, MONO, 8k, 16 bits
    T41Handle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_G711U - 1)) | (1 << (AUD_FMT_PCM - 1));
    T41Handle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    T41Handle->capability.sampleRates = (1 << (AUD_SAM_8K - 1));
    T41Handle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioPlayerHandle) T41Handle, AUD_PLY_STATUS_STREAM_OFF);

    return (AudioPlayerHandle) T41Handle;
}

int audioPlayerGetCapability(const AudioPlayerHandle handle, AudioCapability* pCapability)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = T41Handle->capability;

    return 0;
}

int audioPlayerSetFormat(AudioPlayerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                         const AudioBitDepth bitDepth)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    T41_HANDLE_STATUS_CHECK(T41Handle, AUD_PLY_STATUS_STREAM_OFF);

    IMPAudioDecChnAttr chnAttr = {
        .bufSize = 2,
        .mode = ADEC_MODE_PACK,
    };

    IMPAudioIOAttr ioAttr = {
        .frmNum = 2,
        .numPerFrm = 400,
        .chnCnt = 1,
    };

    switch (format) {
        case AUD_FMT_PCM:
            chnAttr.type = PT_PCM;
            break;

        case AUD_FMT_G711A:
            chnAttr.type = PT_G711A;
            break;

        case AUD_FMT_G711U:
            chnAttr.type = PT_G711U;
            break;

        default:
            LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    switch (channel) {
        case AUD_CHN_MONO:
            ioAttr.soundmode = AUDIO_SOUND_MODE_MONO;
            break;

        default:
            LOG("Unsupported channel num %d", channel);
            return -EINVAL;
    }

    switch (sampleRate) {
        case AUD_SAM_8K:
            ioAttr.samplerate = AUDIO_SAMPLE_RATE_8000;
            break;

        case AUD_SAM_16K:
            ioAttr.samplerate = AUDIO_SAMPLE_RATE_16000;
            break;

        default:
            LOG("Unsupported sample rate %d", sampleRate);
            return -EINVAL;
    }

    switch (bitDepth) {
        case AUD_BIT_16:
            ioAttr.bitwidth = AUDIO_BIT_WIDTH_16;
            break;

        default:
            LOG("Unsupported bit depth %d", bitDepth);
            return -EINVAL;
    }

    if (IMP_AO_SetPubAttr(T41_SPK_DEV_ID, &ioAttr)) {
        LOG("IMP_AO_SetPubAttr failed");
        return -EAGAIN;
    }

    // Make sure T41_SPK_DEC_CHN_ID is not occupied
    IMP_ADEC_DestroyChn(T41_SPK_DEC_CHN_ID);
    if (IMP_ADEC_CreateChn(T41_SPK_DEC_CHN_ID, &chnAttr)) {
        LOG("IMP_ADEC_CreateChn failed");
        return -EAGAIN;
    }

    if (IMP_ADEC_ClearChnBuf(T41_SPK_DEC_CHN_ID)) {
        LOG("IMP_ADEC_ClearChnBuf failed");
        return -EAGAIN;
    }

    T41Handle->format = format;
    T41Handle->channel = channel;
    T41Handle->sampleRate = sampleRate;
    T41Handle->bitDepth = bitDepth;

    return 0;
}

int audioPlayerGetFormat(const AudioPlayerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                         AudioBitDepth* pBitDepth)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    *pFormat = T41Handle->format;
    *pChannel = T41Handle->channel;
    *pSampleRate = T41Handle->sampleRate;
    *pBitDepth = T41Handle->bitDepth;

    return 0;
}

int audioPlayerAcquireStream(AudioPlayerHandle handle)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (IMP_AO_Enable(T41_SPK_DEV_ID)) {
        LOG("IMP_AO_Enable failed");
        return -EAGAIN;
    }

    if (IMP_AO_EnableChn(T41_SPK_DEV_ID, T41_SPK_DEV_CHN_ID)) {
        LOG("IMP_AO_EnableChn failed");
        return -EAGAIN;
    }

    if (IMP_AO_SetVol(T41_SPK_DEV_ID, T41_SPK_DEV_CHN_ID, T41_SPK_DEFAULT_VOL)) {
        LOG("IMP_AO_SetVol failed");
        return -EAGAIN;
    }

    return setStatus(handle, AUD_PLY_STATUS_STREAM_ON);
}

int audioPlayerWriteFrame(AudioPlayerHandle handle, void* pData, const size_t size)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    T41_HANDLE_STATUS_CHECK(T41Handle, AUD_PLY_STATUS_STREAM_ON);

    if (!pData) {
        return -EINVAL;
    }

    IMPAudioFrame frame = {0};

    if (T41Handle->format != AUD_FMT_PCM) {
        // Need to decode
        IMPAudioStream streamIn;
        streamIn.stream = (uint8_t*) pData;
        streamIn.len = size;

        if (IMP_ADEC_SendStream(T41_SPK_DEC_CHN_ID, &streamIn, BLOCK)) {
            LOG("IMP_ADEC_SendStream failed");
            return -EAGAIN;
        }

        IMPAudioStream streamOut;
        if (IMP_ADEC_PollingStream(T41_SPK_DEC_CHN_ID, 1000)) {
            LOG("IMP_ADEC_PollingStream failed");
            return -EAGAIN;
        }

        if (IMP_ADEC_GetStream(T41_SPK_DEC_CHN_ID, &streamOut, BLOCK)) {
            LOG("IMP_ADEC_GetStream failed");
            return -EAGAIN;
        }

        frame.virAddr = (uint32_t*) streamOut.stream;
        frame.len = streamOut.len;
        playFrame(handle, &frame);

        if (IMP_ADEC_ReleaseStream(T41_SPK_DEC_CHN_ID, &streamOut)) {
            LOG("IMP_ADEC_ReleaseStream failed");
            return -EAGAIN;
        }
    } else {
        // No need to decode
        frame.virAddr = (uint32_t*) pData;
        frame.len = size;
        playFrame(handle, &frame);
    }

    return 0;
}

int audioPlayerReleaseStream(AudioPlayerHandle handle)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (IMP_AO_DisableChn(T41_SPK_DEV_ID, T41_SPK_DEV_CHN_ID)) {
        LOG("IMP_AO_DisableChn failed");
        return -EAGAIN;
    }

    if (IMP_AO_Disable(T41_SPK_DEV_ID)) {
        LOG("IMP_AO_Disable failed");
        return -EAGAIN;
    }

    return setStatus(handle, AUD_PLY_STATUS_STREAM_OFF);
}

void audioPlayerDestroy(AudioPlayerHandle handle)
{
    if (!handle) {
        return;
    }
    T41_HANDLE_GET(handle);

    if (T41Handle->status == AUD_PLY_STATUS_STREAM_ON) {
        audioPlayerReleaseStream(handle);
    }

    if (IMP_ADEC_DestroyChn(T41_SPK_DEC_CHN_ID)) {
        LOG("IMP_ADEC_DestroyChn failed");
    }

    setStatus(handle, AUD_PLY_STATUS_NOT_READY);

    free(handle);
}
