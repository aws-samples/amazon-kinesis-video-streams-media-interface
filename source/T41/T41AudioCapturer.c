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

#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"

#include "T41Common.h"
#include <imp/imp_audio.h>
#include <imp/imp_system.h>

#define T41_MIC_DEV_ID       1
#define T41_MIC_CHN_ID       0
#define T41_MIC_DEFAULT_VOL  50
#define T41_MIC_DEFAULT_GAIN 10
#define T41_MIC_ENC_CHN_ID   0

#define T41_HANDLE_GET(x) T41AudioCapturer* T41Handle = (T41AudioCapturer*) ((x))

typedef struct {
    AudioCapturerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} T41AudioCapturer;

static int setStatus(AudioCapturerHandle handle, const AudioCapturerStatus newStatus)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (newStatus != T41Handle->status) {
        T41Handle->status = newStatus;
        LOG("AudioCapturer new status[%d]", newStatus);
    }

    return 0;
}

AudioCapturerHandle audioCapturerCreate(void)
{
    T41AudioCapturer* T41Handle = NULL;

    if (!(T41Handle = (T41AudioCapturer*) malloc(sizeof(T41AudioCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(T41Handle, 0, sizeof(T41AudioCapturer));

    // Now implementation supports raw PCM, G.711 ALAW and ULAW, MONO, 8k/16k, 16 bits
    T41Handle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_G711U - 1)) | (1 << (AUD_FMT_PCM - 1));
    T41Handle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    T41Handle->capability.sampleRates = (1 << (AUD_SAM_8K - 1)) | (1 << (AUD_SAM_16K - 1));
    T41Handle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioCapturerHandle) T41Handle, AUD_CAP_STATUS_STREAM_OFF);

    return (AudioCapturerHandle) T41Handle;
}

AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle handle)
{
    if (!handle) {
        return AUD_CAP_STATUS_NOT_READY;
    }

    T41_HANDLE_GET(handle);
    return T41Handle->status;
}

int audioCapturerGetCapability(const AudioCapturerHandle handle, AudioCapability* pCapability)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = T41Handle->capability;

    return 0;
}

int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    T41_HANDLE_STATUS_CHECK(T41Handle, AUD_CAP_STATUS_STREAM_OFF);

    IMPAudioEncChnAttr chnAttr = {
        .bufSize = 2,
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

    if (IMP_AI_SetPubAttr(T41_MIC_DEV_ID, &ioAttr)) {
        LOG("IMP_AI_SetPubAttr failed");
        return -EAGAIN;
    }

    // Make sure T41_MIC_ENC_CHN_ID is not occupied
    IMP_AENC_DestroyChn(T41_MIC_ENC_CHN_ID);
    if (IMP_AENC_CreateChn(T41_MIC_ENC_CHN_ID, &chnAttr)) {
        LOG("IMP_AENC_CreateChn failed");
        return -EAGAIN;
    }

    T41Handle->format = format;
    T41Handle->channel = channel;
    T41Handle->sampleRate = sampleRate;
    T41Handle->bitDepth = bitDepth;

    return 0;
}

int audioCapturerGetFormat(const AudioCapturerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
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

int audioCapturerAcquireStream(AudioCapturerHandle handle)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (IMP_AI_Enable(T41_MIC_DEV_ID)) {
        LOG("IMP_AI_Enable failed");
        return -EAGAIN;
    }

    IMPAudioIChnParam chnParam = {
        .usrFrmDepth = 2,
    };

    if (IMP_AI_SetChnParam(T41_MIC_DEV_ID, T41_MIC_CHN_ID, &chnParam)) {
        LOG("IMP_AI_SetChnParam failed");
        return -EAGAIN;
    }

    if (IMP_AI_EnableChn(T41_MIC_DEV_ID, T41_MIC_CHN_ID)) {
        LOG("IMP_AI_EnableChn failed");
        return -EAGAIN;
    }

    if (IMP_AI_SetVol(T41_MIC_DEV_ID, T41_MIC_CHN_ID, T41_MIC_DEFAULT_VOL)) {
        LOG("IMP_AI_SetVol failed");
        return -EAGAIN;
    }

    if (IMP_AI_SetGain(T41_MIC_DEV_ID, T41_MIC_CHN_ID, T41_MIC_DEFAULT_GAIN)) {
        LOG("IMP_AI_SetGain failed");
        return -EAGAIN;
    }

    return setStatus(handle, AUD_CAP_STATUS_STREAM_ON);
}

int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    T41_HANDLE_STATUS_CHECK(T41Handle, AUD_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    int ret = 0;
    IMPAudioFrame rawFrame = {0};

    if (IMP_AI_PollingFrame(T41_MIC_DEV_ID, T41_MIC_CHN_ID, T41_POLLING_STREAM_TIMEOUT_MS)) {
        LOG("IMP_AI_PollingFrame failed");
        return -EAGAIN;
    }

    // Get raw PCM
    if (IMP_AI_GetFrame(T41_MIC_DEV_ID, T41_MIC_CHN_ID, &rawFrame, BLOCK)) {
        LOG("IMP_AI_GetFrame failed");
        return -EAGAIN;
    }

    // Encode frame
    if (T41Handle->format != AUD_FMT_PCM) {
        // Need to encode
        IMPAudioStream encodeStream = {0};

        if (IMP_AENC_SendFrame(T41_MIC_ENC_CHN_ID, &rawFrame)) {
            LOG("IMP_AENC_SendFrame failed");
            ret = -EAGAIN;
        } else if (IMP_AENC_PollingStream(T41_MIC_ENC_CHN_ID, T41_POLLING_STREAM_TIMEOUT_MS)) {
            LOG("IMP_AENC_PollingStream failed");
            ret = -EAGAIN;
        } else if (IMP_AENC_GetStream(T41_MIC_ENC_CHN_ID, &encodeStream, BLOCK)) {
            LOG("IMP_AENC_GetStream failed");
            ret - EAGAIN;
        } else if (frameDataBufferSize < encodeStream.len) {
            LOG("FrameDataBufferSize(%d) < frameSize(%d), frame dropped", frameDataBufferSize, encodeStream.len);
            *pFrameSize = 0;
            ret = -ENOMEM;
        } else {
            memcpy(pFrameDataBuffer, (void*) encodeStream.stream, encodeStream.len);
            *pFrameSize = encodeStream.len;
            *pTimestamp = IMP_System_GetTimeStamp();
        }

        IMP_AENC_ReleaseStream(T41_MIC_ENC_CHN_ID, &encodeStream);
    } else {
        memcpy(pFrameDataBuffer, (void*) rawFrame.virAddr, rawFrame.len);
        *pFrameSize = rawFrame.len;
        *pTimestamp = IMP_System_GetTimeStamp();
    }

    IMP_AI_ReleaseFrame(T41_MIC_DEV_ID, T41_MIC_CHN_ID, &rawFrame);

    return ret;
}

int audioCapturerReleaseStream(AudioCapturerHandle handle)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    T41_HANDLE_STATUS_CHECK(T41Handle, AUD_CAP_STATUS_STREAM_ON);

    if (IMP_AI_DisableChn(T41_MIC_DEV_ID, T41_MIC_CHN_ID)) {
        LOG("Audio channel disable failed");
        return -EAGAIN;
    }

    if (IMP_AI_Disable(T41_MIC_DEV_ID)) {
        LOG("Audio device disable failed");
        return -EAGAIN;
    }

    return setStatus(handle, AUD_CAP_STATUS_STREAM_OFF);
}

void audioCapturerDestroy(AudioCapturerHandle handle)
{
    if (!handle) {
        return;
    }
    T41_HANDLE_GET(handle);

    if (T41Handle->status == AUD_CAP_STATUS_STREAM_ON) {
        audioCapturerReleaseStream(handle);
    }

    if (IMP_AENC_DestroyChn(T41_MIC_ENC_CHN_ID)) {
        LOG("Encode channel disable failed");
    }

    setStatus(handle, AUD_CAP_STATUS_NOT_READY);

    free(handle);
}
