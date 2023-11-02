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

#include "T31Common.h"
#include <imp/imp_audio.h>
#include <imp/imp_system.h>

#define T31_MIC_DEV_ID       1
#define T31_MIC_CHN_ID       0
#define T31_MIC_DEFAULT_VOL  50
#define T31_MIC_DEFAULT_GAIN 10
#define T31_MIC_ENC_CHN_ID   0

#define T31_HANDLE_GET(x) T31AudioCapturer* t31Handle = (T31AudioCapturer*) ((x))

typedef struct {
    AudioCapturerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} T31AudioCapturer;

static int setStatus(AudioCapturerHandle handle, const AudioCapturerStatus newStatus)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (newStatus != t31Handle->status) {
        t31Handle->status = newStatus;
        LOG("AudioCapturer new status[%d]", newStatus);
    }

    return 0;
}

AudioCapturerHandle audioCapturerCreate(void)
{
    T31AudioCapturer* t31Handle = NULL;

    if (!(t31Handle = (T31AudioCapturer*) malloc(sizeof(T31AudioCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(t31Handle, 0, sizeof(T31AudioCapturer));

    // Now implementation supports raw PCM, G.711 ALAW and ULAW, MONO, 8k/16k, 16 bits
    t31Handle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_G711U - 1)) | (1 << (AUD_FMT_PCM - 1));
    t31Handle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    t31Handle->capability.sampleRates = (1 << (AUD_SAM_8K - 1)) | (1 << (AUD_SAM_16K - 1));
    t31Handle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioCapturerHandle) t31Handle, AUD_CAP_STATUS_STREAM_OFF);

    return (AudioCapturerHandle) t31Handle;
}

AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle handle)
{
    if (!handle) {
        return AUD_CAP_STATUS_NOT_READY;
    }

    T31_HANDLE_GET(handle);
    return t31Handle->status;
}

int audioCapturerGetCapability(const AudioCapturerHandle handle, AudioCapability* pCapability)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = t31Handle->capability;

    return 0;
}

int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    T31_HANDLE_STATUS_CHECK(t31Handle, AUD_CAP_STATUS_STREAM_OFF);

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

    if (IMP_AI_SetPubAttr(T31_MIC_DEV_ID, &ioAttr)) {
        LOG("IMP_AI_SetPubAttr failed");
        return -EAGAIN;
    }

    // Make sure T31_MIC_ENC_CHN_ID is not occupied
    IMP_AENC_DestroyChn(T31_MIC_ENC_CHN_ID);
    if (IMP_AENC_CreateChn(T31_MIC_ENC_CHN_ID, &chnAttr)) {
        LOG("IMP_AENC_CreateChn failed");
        return -EAGAIN;
    }

    t31Handle->format = format;
    t31Handle->channel = channel;
    t31Handle->sampleRate = sampleRate;
    t31Handle->bitDepth = bitDepth;

    return 0;
}

int audioCapturerGetFormat(const AudioCapturerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                           AudioBitDepth* pBitDepth)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    *pFormat = t31Handle->format;
    *pChannel = t31Handle->channel;
    *pSampleRate = t31Handle->sampleRate;
    *pBitDepth = t31Handle->bitDepth;

    return 0;
}

int audioCapturerAcquireStream(AudioCapturerHandle handle)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (IMP_AI_Enable(T31_MIC_DEV_ID)) {
        LOG("IMP_AI_Enable failed");
        return -EAGAIN;
    }

    IMPAudioIChnParam chnParam = {
        .usrFrmDepth = 2,
    };

    if (IMP_AI_SetChnParam(T31_MIC_DEV_ID, T31_MIC_CHN_ID, &chnParam)) {
        LOG("IMP_AI_SetChnParam failed");
        return -EAGAIN;
    }

    if (IMP_AI_EnableChn(T31_MIC_DEV_ID, T31_MIC_CHN_ID)) {
        LOG("IMP_AI_EnableChn failed");
        return -EAGAIN;
    }

    if (IMP_AI_SetVol(T31_MIC_DEV_ID, T31_MIC_CHN_ID, T31_MIC_DEFAULT_VOL)) {
        LOG("IMP_AI_SetVol failed");
        return -EAGAIN;
    }

    if (IMP_AI_SetGain(T31_MIC_DEV_ID, T31_MIC_CHN_ID, T31_MIC_DEFAULT_GAIN)) {
        LOG("IMP_AI_SetGain failed");
        return -EAGAIN;
    }

    return setStatus(handle, AUD_CAP_STATUS_STREAM_ON);
}

int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    T31_HANDLE_STATUS_CHECK(t31Handle, AUD_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    int ret = 0;
    IMPAudioFrame rawFrame = {0};

    if (IMP_AI_PollingFrame(T31_MIC_DEV_ID, T31_MIC_CHN_ID, T31_POLLING_STREAM_TIMEOUT_MS)) {
        LOG("IMP_AI_PollingFrame failed");
        return -EAGAIN;
    }

    // Get raw PCM
    if (IMP_AI_GetFrame(T31_MIC_DEV_ID, T31_MIC_CHN_ID, &rawFrame, BLOCK)) {
        LOG("IMP_AI_GetFrame failed");
        return -EAGAIN;
    }

    // Encode frame
    if (t31Handle->format != AUD_FMT_PCM) {
        // Need to encode
        IMPAudioStream encodeStream = {0};

        if (IMP_AENC_SendFrame(T31_MIC_ENC_CHN_ID, &rawFrame)) {
            LOG("IMP_AENC_SendFrame failed");
            ret = -EAGAIN;
        } else if (IMP_AENC_PollingStream(T31_MIC_ENC_CHN_ID, T31_POLLING_STREAM_TIMEOUT_MS)) {
            LOG("IMP_AENC_PollingStream failed");
            ret = -EAGAIN;
        } else if (IMP_AENC_GetStream(T31_MIC_ENC_CHN_ID, &encodeStream, BLOCK)) {
            LOG("IMP_AENC_GetStream failed");
            ret = - EAGAIN;
        } else if (frameDataBufferSize < encodeStream.len) {
            LOG("FrameDataBufferSize(%d) < frameSize(%d), frame dropped", frameDataBufferSize, encodeStream.len);
            *pFrameSize = 0;
            ret = -ENOMEM;
        } else {
            memcpy(pFrameDataBuffer, (void*) encodeStream.stream, encodeStream.len);
            *pFrameSize = encodeStream.len;
            *pTimestamp = IMP_System_GetTimeStamp();
        }

        IMP_AENC_ReleaseStream(T31_MIC_ENC_CHN_ID, &encodeStream);
    } else {
        memcpy(pFrameDataBuffer, (void*) rawFrame.virAddr, rawFrame.len);
        *pFrameSize = rawFrame.len;
        *pTimestamp = IMP_System_GetTimeStamp();
    }

    IMP_AI_ReleaseFrame(T31_MIC_DEV_ID, T31_MIC_CHN_ID, &rawFrame);

    return ret;
}

int audioCapturerReleaseStream(AudioCapturerHandle handle)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    T31_HANDLE_STATUS_CHECK(t31Handle, AUD_CAP_STATUS_STREAM_ON);

    if (IMP_AI_DisableChn(T31_MIC_DEV_ID, T31_MIC_CHN_ID)) {
        LOG("Audio channel disable failed");
        return -EAGAIN;
    }

    if (IMP_AI_Disable(T31_MIC_DEV_ID)) {
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
    T31_HANDLE_GET(handle);

    if (t31Handle->status == AUD_CAP_STATUS_STREAM_ON) {
        audioCapturerReleaseStream(handle);
    }

    if (IMP_AENC_DestroyChn(T31_MIC_ENC_CHN_ID)) {
        LOG("Encode channel disable failed");
    }

    setStatus(handle, AUD_CAP_STATUS_NOT_READY);

    free(handle);
}
