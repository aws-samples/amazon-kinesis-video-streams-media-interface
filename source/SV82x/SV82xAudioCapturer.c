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

#include "SV82xCommon.h"

#define SV82X_HANDLE_GET(x) SV82XAudioCapturer* sv82xHandle = (SV82XAudioCapturer*) ((x))
#define AAC_HEADER_SIZE     (7)

typedef struct {
    AudioCapturerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} SV82XAudioCapturer;

sv82x_audio_stream_info_t g_audio_capture = {
    .AiDev = 0,
    .AencChn = 0,
    .paramEncode.payload_type = PT_LPCM,
    .paramEncode.samplerate = AUDIO_SAMPLE_RATE_48000,
    .paramEncode.num_per_frm = 160,
    .paramEncode.fps = 25,
    .paramEncode.sound_mode = AUDIO_SOUND_MODE_MONO,
    .paramEncode.bitwidth = AUDIO_BIT_WIDTH_8,
    .paramEncode.volume = 50,
    .paramEncode.u32ChnCnt = 1,
    .bAioReSample = EI_FALSE,
    .enInSampleRate = AUDIO_SAMPLE_RATE_BUTT,
    .enOutSampleRate = AUDIO_SAMPLE_RATE_BUTT,
    .enVqe = EI_TRUE,
    .audio_vqe.open_mask = AI_VQE_MASK_AEC,
    .enAudioCreate = EI_FALSE,
    .enAencCreate = EI_FALSE,
};

static int setStatus(AudioCapturerHandle handle, const AudioCapturerStatus newStatus)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    if (newStatus != sv82xHandle->status) {
        sv82xHandle->status = newStatus;
        LOG("AudioCapturer new status[%d]", newStatus);
    }

    return 0;
}

AudioCapturerHandle audioCapturerCreate(void)
{
    SV82XAudioCapturer* sv82xHandle = (SV82XAudioCapturer*) malloc(sizeof(SV82XAudioCapturer));

    if (!sv82xHandle) {
        LOG("%s OOM", __func__);
        return NULL;
    }

    memset(sv82xHandle, 0, sizeof(SV82XAudioCapturer));

    // Now implementation supports raw PCM, G.711 ALAW and ULAW, MONO, 8k/16k, 16 bits
    sv82xHandle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_G711U - 1)) | (1 << (AUD_FMT_PCM - 1)) | (1 << (AUD_FMT_AAC - 1));
    sv82xHandle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    sv82xHandle->capability.sampleRates = (1 << (AUD_SAM_8K - 1)) | (1 << (AUD_SAM_16K - 1));
    sv82xHandle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioCapturerHandle) sv82xHandle, AUD_CAP_STATUS_STREAM_OFF);
    return (AudioCapturerHandle) sv82xHandle;
}

AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle handle)
{
    if (!handle) {
        return AUD_CAP_STATUS_NOT_READY;
    }

    SV82X_HANDLE_GET(handle);
    return sv82xHandle->status;
}

int audioCapturerGetCapability(const AudioCapturerHandle handle, AudioCapability* pCapability)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = sv82xHandle->capability;

    return 0;
}

// START AENC
int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth)
{
    EI_S32 s32Ret = EI_SUCCESS;
    EI_S32 s32AencChnCnt;
    AIO_ATTR_S stAioAttr;
    PAYLOAD_TYPE_E payType;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);
    SV82X_HANDLE_STATUS_CHECK(sv82xHandle, AUD_CAP_STATUS_STREAM_OFF);

    switch (format) {
        case AUD_FMT_AAC:
            payType = PT_AAC;
            break;
        case AUD_FMT_PCM:
            payType = PT_LPCM;
            break;
        case AUD_FMT_G711A:
            payType = PT_G711A;
            break;
        case AUD_FMT_G711U:
            payType = PT_G711U;
            break;
        case AUD_FMT_OPUS:
        default:
            LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    if (channel == AUD_CHN_MONO) {
        g_audio_capture.paramEncode.sound_mode == AUDIO_SOUND_MODE_MONO;
        g_audio_capture.paramEncode.u32ChnCnt = 1;
    } else {
        g_audio_capture.paramEncode.sound_mode == AUDIO_SOUND_MODE_STEREO;
        g_audio_capture.paramEncode.u32ChnCnt = 2;
    }

    switch (sampleRate) {
        case AUD_SAM_8K:
            g_audio_capture.paramEncode.samplerate = AUDIO_SAMPLE_RATE_8000;
            break;
        case AUD_SAM_16K:
            g_audio_capture.paramEncode.samplerate = AUDIO_SAMPLE_RATE_16000;
            break;
        case AUD_SAM_24K:
            g_audio_capture.paramEncode.samplerate = AUDIO_SAMPLE_RATE_24000;
            break;
        case AUD_SAM_32K:
            g_audio_capture.paramEncode.samplerate = AUDIO_SAMPLE_RATE_32000;
            break;
        case AUD_SAM_44_1K:
            g_audio_capture.paramEncode.samplerate = AUDIO_SAMPLE_RATE_44100;
            break;
        case AUD_SAM_48K:
            g_audio_capture.paramEncode.samplerate = AUDIO_SAMPLE_RATE_48000;
            break;
        default:
            LOG("Unsupported sampleRate %d", sampleRate);
            return -EINVAL;
    }

    switch (bitDepth) {
        case AUD_BIT_8:
            g_audio_capture.paramEncode.bitwidth = AUDIO_BIT_WIDTH_8;
            break;
        case AUD_BIT_16:
            g_audio_capture.paramEncode.bitwidth = AUDIO_BIT_WIDTH_16;
            break;
        case AUD_BIT_32:
            g_audio_capture.paramEncode.bitwidth = AUDIO_BIT_WIDTH_24;
            break;
        default:
            LOG("Unsupported bitDepth %d", bitDepth);
            return -EINVAL;
    }

    g_audio_capture.paramEncode.payload_type = payType;
    stAioAttr.enSamplerate = g_audio_capture.paramEncode.samplerate;
    stAioAttr.enBitwidth = g_audio_capture.paramEncode.bitwidth;
    stAioAttr.enSoundmode = g_audio_capture.paramEncode.sound_mode;
    stAioAttr.u32EXFlag = 0;
    stAioAttr.u32FrmNum = 30;
    stAioAttr.u32PtNumPerFrm = g_audio_capture.paramEncode.num_per_frm;
    stAioAttr.u32ChnCnt = g_audio_capture.paramEncode.u32ChnCnt;

    s32AencChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
    s32Ret = SAMPLE_COMM_AUDIO_StartAenc(s32AencChnCnt, &stAioAttr, g_audio_capture.paramEncode.payload_type);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_AUDIO_StartAenc failed, s32Ret=%d\n", s32Ret);
        s32Ret = SAMPLE_COMM_AUDIO_StopAenc(s32AencChnCnt);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_AUDIO_StopAenc failed, s32Ret=%d\n", s32Ret);
        }
        return -EAGAIN;
    }

    sv82xHandle->format = format;
    sv82xHandle->channel = channel;
    sv82xHandle->sampleRate = sampleRate;
    sv82xHandle->bitDepth = bitDepth;

    return 0;
}

int audioCapturerGetFormat(const AudioCapturerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                           AudioBitDepth* pBitDepth)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    *pFormat = sv82xHandle->format;
    *pChannel = sv82xHandle->channel;
    *pSampleRate = sv82xHandle->sampleRate;
    *pBitDepth = sv82xHandle->bitDepth;

    return 0;
}

// START AI
int audioCapturerAcquireStream(AudioCapturerHandle handle)
{
    EI_S32 s32Ret = EI_SUCCESS;
    AI_CHN AiChn = 0;
    EI_S32 s32AiChnCnt;
    AENC_CHN AeChn = 0;
    AIO_ATTR_S stAioAttr;
    AI_VQE_CONFIG_S stAiVqeAttr;
    EI_VOID* pAiVqeAttr = NULL;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    AUDIO_DEV AiDev = g_audio_capture.AiDev;

    stAioAttr.enSamplerate = g_audio_capture.paramEncode.samplerate;
    stAioAttr.enBitwidth = g_audio_capture.paramEncode.bitwidth;
    stAioAttr.enSoundmode = g_audio_capture.paramEncode.sound_mode;
    stAioAttr.u32EXFlag = 0;
    stAioAttr.u32FrmNum = 30;
    stAioAttr.u32PtNumPerFrm = g_audio_capture.paramEncode.num_per_frm;
    stAioAttr.u32ChnCnt = g_audio_capture.paramEncode.u32ChnCnt;

    s32AiChnCnt = stAioAttr.u32ChnCnt;
    if (g_audio_capture.enVqe == EI_TRUE) {
        memset(&stAiVqeAttr, 0, sizeof(AI_VQE_CONFIG_S));
        g_audio_capture.audio_vqe.sample_rate = g_audio_capture.paramEncode.samplerate;
        stAiVqeAttr.s32WorkSampleRate = g_audio_capture.audio_vqe.sample_rate;
        stAiVqeAttr.s32FrameSample = g_audio_capture.paramEncode.num_per_frm;
        stAiVqeAttr.enWorkstate = VQE_WORKSTATE_COMMON;
        stAiVqeAttr.u32OpenMask = g_audio_capture.audio_vqe.open_mask;
        pAiVqeAttr = (EI_VOID*) &stAiVqeAttr;
    }

    s32Ret = SAMPLE_COMM_AUDIO_StartAi(AiDev, s32AiChnCnt, &stAioAttr, g_audio_capture.enOutSampleRate, g_audio_capture.bAioReSample, pAiVqeAttr);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_AUDIO_StartAi failed, s32Ret=%d\n", s32Ret);
        return -EAGAIN;
    }

    s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAiAenc(AiDev, AiChn, AeChn);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_AUDIO_OpenSaveFile failed, s32Ret=%d\n", s32Ret);
        s32AiChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
        s32Ret |= SAMPLE_COMM_AUDIO_StopAi(AiDev, s32AiChnCnt, g_audio_capture.bAioReSample, g_audio_capture.enVqe);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_AUDIO_StopAi failed, s32Ret=%d\n", s32Ret);
        }
        return -EAGAIN;
    }

    LOG("SAMPLE_COMM_AUDIO_StartAi OK,AI(%d,%d),AencChn:%d\n", AiDev, AiChn, AeChn);

    return setStatus(handle, AUD_CAP_STATUS_STREAM_ON);
}

int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    EI_S32 s32Ret = EI_SUCCESS;
    AUDIO_STREAM_S stStream;
    int AeChn = g_audio_capture.AencChn;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);
    SV82X_HANDLE_STATUS_CHECK(sv82xHandle, AUD_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize)
        return -EINVAL;

    while (handle) {
        s32Ret = EI_MI_AENC_GetStream(AeChn, &stStream, -1);
        if (s32Ret != EI_SUCCESS) {
            LOG("EI_MI_AENC_GetStream failed. s32Ret=%x\n", s32Ret);
            continue;
        }
        if (PT_AAC == g_audio_capture.paramEncode.payload_type) {
            if (stStream.u32Len > 0 && stStream.u32Len <= frameDataBufferSize) {
                memcpy(pFrameDataBuffer, stStream.pStream + AAC_HEADER_SIZE, stStream.u32Len - AAC_HEADER_SIZE);
                *pFrameSize = stStream.u32Len - AAC_HEADER_SIZE;
                *pTimestamp = stStream.u64TimeStamp;
                EI_MI_AENC_ReleaseStream(AeChn, &stStream);
                return 0;
            }
        } else {
            if (stStream.u32Len > 0 && stStream.u32Len <= frameDataBufferSize) {
                memcpy(pFrameDataBuffer, stStream.pStream, stStream.u32Len);
                *pFrameSize = stStream.u32Len;
                *pTimestamp = stStream.u64TimeStamp;
                EI_MI_AENC_ReleaseStream(AeChn, &stStream);
                return 0;
            }
        }

        s32Ret = EI_MI_AENC_ReleaseStream(AeChn, &stStream);
        if (s32Ret != EI_SUCCESS) {
            LOG("EI_MI_AENC_ReleaseStream(%d), failed with %#x!\n", AeChn, s32Ret);
        }
        return -EAGAIN;
    }

    return -EAGAIN;
}

int audioCapturerReleaseStream(AudioCapturerHandle handle)
{
    int s32Ret = EI_SUCCESS;
    int bAioReSample = 0;
    EI_BOOL bVqeEn = g_audio_capture.enVqe;
    int s32AiChncnt = 1;
    AUDIO_DEV AiDev = g_audio_capture.AiDev;
    AUDIO_DEV AdChn = g_audio_capture.AdecChn;
    AI_CHN AiChn;
    int i = 0;
    int s32AencChnCnt;
    int u32ChnCnt = 2;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    SV82X_HANDLE_STATUS_CHECK(sv82xHandle, AUD_CAP_STATUS_STREAM_ON);

    if (g_audio_capture.enAencCreate == EI_FALSE)
        return -EINVAL;

    g_audio_capture.enAencCreate = EI_FALSE;

    s32AencChnCnt = u32ChnCnt >> g_audio_capture.paramEncode.sound_mode;

    for (i = 0; i < s32AencChnCnt; i++) {
        AiChn = i;
        s32Ret = SAMPLE_COMM_AUDIO_DestroyTrdAi(AiDev, AiChn);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_AUDIO_DestroyTrdAi failed, s32Ret=%d\n", s32Ret);
        }

        s32Ret = SAMPLE_COMM_AUDIO_DestroyTrdAencAdec(AdChn);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_AUDIO_DestroyTrdAencAdec failed, s32Ret=%d\n", s32Ret);
        }
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAenc(s32AencChnCnt);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_AUDIO_StopAenc failed %x\n", s32Ret);
        return -EAGAIN;
    }

    s32Ret = SAMPLE_COMM_AUDIO_StopAi(AiDev, s32AiChncnt, bAioReSample, bVqeEn);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_AUDIO_StopAi failed %x\n", s32Ret);
        return -EAGAIN;
    }

    return setStatus(handle, AUD_CAP_STATUS_STREAM_OFF);
}

void audioCapturerDestroy(AudioCapturerHandle handle)
{
    if (!handle)
        return;

    SV82X_HANDLE_GET(handle);

    if (sv82xHandle->status == AUD_CAP_STATUS_STREAM_ON) {
        audioCapturerReleaseStream(handle);
    }

    setStatus(handle, AUD_CAP_STATUS_NOT_READY);

    free(handle);
}
