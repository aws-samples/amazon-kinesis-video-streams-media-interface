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

#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"

#include "FH8626V100Common.h"
#include "sample_common.h"

#define USING_HARD_STREAM_AUDIO

#define HANDLE_GET(x) FH8626V100AudioCapturer* audioHandle = (FH8626V100AudioCapturer*) ((x))

#define DEFAULT_PERIOD_SIZE 1024
#define DEFAULT_VOLUME      28
#define DEFAULT_MICIN_VOL   2
#define DEFAULT_AIN_VOL     28

typedef struct {
    AudioCapturerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
    AAC_ENC_HANDLE aacHandle;
} FH8626V100AudioCapturer;

static int setStatus(AudioCapturerHandle handle, const AudioCapturerStatus newStatus)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    if (newStatus != audioHandle->status) {
        audioHandle->status = newStatus;
        KVS_LOG("AudioCapturer set new status[%d]\n", newStatus);
    }

    return 0;
}

AudioCapturerHandle audioCapturerCreate(void)
{
    FH8626V100AudioCapturer* audioHandle = NULL;

    if (!(audioHandle = (FH8626V100AudioCapturer*) malloc(sizeof(FH8626V100AudioCapturer)))) {
        KVS_LOG("OOM");
        return NULL;
    }

    memset(audioHandle, 0, sizeof(FH8626V100AudioCapturer));

    audioHandle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_PCM - 1)) | (1 << (AUD_FMT_AAC - 1));
    audioHandle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    audioHandle->capability.sampleRates = (1 << (AUD_SAM_8K - 1)) | (1 << (AUD_SAM_16K - 1)) | (1 << (AUD_SAM_32K - 1));
    audioHandle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioCapturerHandle) audioHandle, AUD_CAP_STATUS_STREAM_OFF);

#ifdef USING_HARD_STREAM_AUDIO
    if (FH_AC_Init()) {
        KVS_LOG("FH_AC_Init err\n");
        return NULL;
    }
#endif

    return (AudioCapturerHandle) audioHandle;
}

AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle handle)
{
    if (!handle) {
        return AUD_CAP_STATUS_NOT_READY;
    }

    HANDLE_GET(handle);
    return audioHandle->status;
}

int audioCapturerGetCapability(const AudioCapturerHandle handle, AudioCapability* pCapability)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = audioHandle->capability;

    return 0;
}

int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    HANDLE_STATUS_CHECK(audioHandle, AUD_CAP_STATUS_STREAM_OFF);

    FH_AC_CONFIG ac_config;

    switch (format) {
        case AUD_FMT_PCM:
        case AUD_FMT_G711A:
        case AUD_FMT_AAC:
            ac_config.enc_type = FH_PT_LPCM; // only support raw PCM format
            break;

        default:
            KVS_LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    switch (channel) {
        case AUD_CHN_MONO:
            ac_config.channels = 1; // //only support one channel
            break;

        default:
            KVS_LOG("Unsupported channel num %d", channel);
            return -EINVAL;
    }

    switch (sampleRate) {
        case AUD_SAM_8K:
            ac_config.sample_rate = AC_SR_8K;
            break;

        case AUD_SAM_16K:
            ac_config.sample_rate = AC_SR_16K;
            break;

        case AC_SR_32K:
            ac_config.sample_rate = AC_SR_32K;
            break;

        default:
            KVS_LOG("Unsupported sample rate %d", sampleRate);
            return -EINVAL;
    }

    switch (bitDepth) {
        case AUD_BIT_16:
            ac_config.bit_width = AC_BW_16;
            break;

        default:
            KVS_LOG("Unsupported bit depth %d", bitDepth);
            return -EINVAL;
    }

    ac_config.io_type = FH_AC_MIC_IN;
    ac_config.period_size = DEFAULT_PERIOD_SIZE;
    ac_config.volume = DEFAULT_VOLUME;

#ifdef USING_HARD_STREAM_AUDIO
    int ret = 0;
    FH_AC_NR_CONFIG nr_cfg;
    nr_cfg.enable = 1;
    nr_cfg.level = 1;

    if (ret = FH_AC_Set_Config(&ac_config)) {
        KVS_LOG("FH_AC_Set_Config failed: %d\n", ret);
    } else if (ret = FH_AC_AI_MICIN_SetVol(DEFAULT_MICIN_VOL)) { // level-1 gain for mic-in
        KVS_LOG("FH_AC_Set_Config failed: %d\n", ret);
    } else if (ret = FH_AC_AI_SetVol(DEFAULT_AIN_VOL)) { // level-2 gain
        KVS_LOG("FH_AC_AI_SetVol failed: %d\n", ret);
    } else if (ret = FH_AC_NR_SetConfig(&nr_cfg)) {
        KVS_LOG("FH_AC_NR_SetConfig failed: %d\n", ret);
    }

    if (ret) {
#ifdef USING_HARD_STREAM_AUDIO
        FH_AC_AI_Disable();
        if (format == AUD_FMT_AAC) {
            if (audioHandle->aacHandle) {
                fh_aacenc_destroy(audioHandle->aacHandle);
            }
            audioHandle->aacHandle = fh_aacenc_create(1, ac_config.sample_rate, 0);
        }
#endif
        return -EAGAIN;
    }

#endif

    audioHandle->format = format;
    audioHandle->channel = channel;
    audioHandle->sampleRate = sampleRate;
    audioHandle->bitDepth = bitDepth;

    return 0;
}

int audioCapturerGetFormat(const AudioCapturerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                           AudioBitDepth* pBitDepth)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    *pFormat = audioHandle->format;
    *pChannel = audioHandle->channel;
    *pSampleRate = audioHandle->sampleRate;
    *pBitDepth = audioHandle->bitDepth;

    return 0;
}

int audioCapturerAcquireStream(AudioCapturerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_AUDIO
    if (FH_AC_AI_Enable()) {
        KVS_LOG("Audio device disable failed");
        return -EAGAIN;
    }
#endif

    return setStatus(handle, AUD_CAP_STATUS_STREAM_ON);
}

int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    HANDLE_STATUS_CHECK(audioHandle, AUD_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

#ifdef USING_HARD_STREAM_AUDIO
    int ret = 0;
    short frame_buf[1024];
    uint64_t audio_pts;
    FH_AC_FRAME_S audio_frame;

    audio_frame.data = (FH_UINT8*) frame_buf;
    ret = FH_AC_AI_GetFrameWithPts(&audio_frame, &audio_pts);
    if (ret || !audio_frame.len) {
        KVS_LOG("FH_AC_AI_GetFrame failed");
        return -EAGAIN;
    }

    // Encode frame
    if (audioHandle->format != AUD_FMT_PCM) {
        int convert_num;
        unsigned char* convert_raw_addr = NULL;
        unsigned char* convert_buf = malloc(audio_frame.len >> 1);
        if (!convert_buf) {
            KVS_LOG("oom\n");
            return -EAGAIN;
        }

        switch (audioHandle->format) {
            case AUD_FMT_G711A:
                convert_num = fh_pcm_2_g711A((unsigned char*) audio_frame.data, audio_frame.len, (unsigned char*) convert_buf);
                convert_raw_addr = convert_buf;
                break;

            case AUD_FMT_AAC:
                convert_num = fh_aacenc_encode(audioHandle->aacHandle, (unsigned char*) audio_frame.data, audio_frame.len,
                                               (unsigned char*) convert_buf, (audio_frame.len >> 2));
                convert_num -= 7; // offset aac header, kvs not support
                convert_raw_addr = convert_buf + 7;
                break;

            default:
                KVS_LOG("Unsupported format %d", audioHandle->format);
                break;
        }

        if (frameDataBufferSize >= convert_num) {
            memcpy(pFrameDataBuffer, (void*) convert_raw_addr, convert_num);
            *pFrameSize = convert_num;
            *pTimestamp = audio_pts;
            free(convert_buf);
        } else {
            KVS_LOG("FrameDataBufferSize(%d) < frameSize(%d), frame dropped", frameDataBufferSize, convert_num);
            *pFrameSize = 0;
            ret = -ENOMEM;
            free(convert_buf);
        }
    } else {
        memcpy(pFrameDataBuffer, (void*) audio_frame.data, audio_frame.len);
        *pFrameSize = audio_frame.len;
        *pTimestamp = audio_pts;
    }

    return ret;
#else
    return 0;
#endif
}

int audioCapturerReleaseStream(AudioCapturerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_AUDIO
    if (FH_AC_AI_Disable()) {
        KVS_LOG("Audio device disable failed");
        return -EAGAIN;
    }
#endif

    return setStatus(handle, AUD_CAP_STATUS_STREAM_OFF);
}

void audioCapturerDestroy(AudioCapturerHandle handle)
{
    if (!handle) {
        return;
    }
    HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_AUDIO
    if (audioHandle->status == AUD_CAP_STATUS_STREAM_ON) {
        FH_AC_AI_Disable();
    }

    FH_AC_DeInit();

    if (audioHandle->aacHandle)
        fh_aacenc_destroy(audioHandle->aacHandle);
#endif

    setStatus(handle, AUD_CAP_STATUS_NOT_READY);

    free(handle);
}
