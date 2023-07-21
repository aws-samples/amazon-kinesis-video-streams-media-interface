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

#include "SV82xCommon.h"

#define SV82X_HANDLE_GET(x) SV82XAudioPlayer* sv82xHandle = (SV82XAudioPlayer*) ((x))

typedef struct {
    AudioPlayerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} SV82XAudioPlayer;

sv82x_audio_stream_info_t g_audio_player = {
    .AdecChn = 0,
    .AoChn = 0,
    .AoDev = 0,
    .paramDecode.payload_type = PT_MP3,
    .paramDecode.samplerate = AUDIO_SAMPLE_RATE_48000,
    .paramDecode.num_per_frm = 512,
    .paramDecode.fps = 25,
    .paramDecode.sound_mode = AUDIO_SOUND_MODE_MONO,
    .paramDecode.bitwidth = AUDIO_BIT_WIDTH_16,
    .paramDecode.volume = 50,
    .paramDecode.u32ChnCnt = 1,
    .bAioReSample = EI_FALSE,
    .enInSampleRate = AUDIO_SAMPLE_RATE_BUTT,
    .enOutSampleRate = AUDIO_SAMPLE_RATE_BUTT,
    .enVqe = EI_FALSE,
    .enAudioCreate = EI_FALSE,
    .enUserGetMode = EI_FALSE,
};

static int setStatus(AudioPlayerHandle handle, const AudioPlayerStatus newStatus)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    if (newStatus != sv82xHandle->status) {
        sv82xHandle->status = newStatus;
        LOG("AudioPlayer new status[%d]", newStatus);
    }

    return 0;
}

AudioPlayerHandle audioPlayerCreate(void)
{
    SV82XAudioPlayer* sv82xHandle = (SV82XAudioPlayer*) malloc(sizeof(SV82XAudioPlayer));

    if (!sv82xHandle) {
        LOG("%s OOM", __func__);
        return NULL;
    }

    memset(sv82xHandle, 0, sizeof(SV82XAudioPlayer));

    sv82xHandle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_G711U - 1)) | (1 << (AUD_FMT_PCM - 1)) | (1 << (AUD_FMT_AAC - 1));
    sv82xHandle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    sv82xHandle->capability.sampleRates = (1 << (AUD_SAM_8K - 1));
    sv82xHandle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    setStatus((AudioPlayerHandle) sv82xHandle, AUD_PLY_STATUS_STREAM_OFF);

    return (AudioPlayerHandle) sv82xHandle;
}

int audioPlayerGetCapability(const AudioPlayerHandle handle, AudioCapability* pCapability)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = sv82xHandle->capability;

    return 0;
}

int audioPlayerSetFormat(AudioPlayerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                         const AudioBitDepth bitDepth)
{
    EI_S32 s32Ret;
    ADEC_CHN AdChn = 0;
    AIO_ATTR_S stAioAttr;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);
    SV82X_HANDLE_STATUS_CHECK(sv82xHandle, AUD_PLY_STATUS_STREAM_OFF);

    switch (format) {
        case AUD_FMT_AAC:
            g_audio_player.paramDecode.payload_type = PT_AAC;
            break;
        case AUD_FMT_PCM:
            g_audio_player.paramDecode.payload_type = PT_LPCM;
            break;
        case AUD_FMT_G711A:
            g_audio_player.paramDecode.payload_type = PT_G711A;
            break;
        case AUD_FMT_G711U:
            g_audio_player.paramDecode.payload_type = PT_G711U;
            break;
        case AUD_FMT_OPUS:
        default:
            LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    if (channel == AUD_CHN_MONO) {
        g_audio_player.paramDecode.sound_mode = AUDIO_SOUND_MODE_MONO;
        g_audio_player.paramDecode.u32ChnCnt = 1;
    } else {
        g_audio_player.paramDecode.sound_mode = AUDIO_SOUND_MODE_STEREO;
        g_audio_player.paramDecode.u32ChnCnt = 2;
    }

    switch (sampleRate) {
        case AUD_SAM_8K:
            g_audio_player.paramDecode.samplerate = AUDIO_SAMPLE_RATE_8000;
            break;
        case AUD_SAM_16K:
            g_audio_player.paramDecode.samplerate = AUDIO_SAMPLE_RATE_16000;
            break;
        case AUD_SAM_24K:
            g_audio_player.paramDecode.samplerate = AUDIO_SAMPLE_RATE_24000;
            break;
        case AUD_SAM_32K:
            g_audio_player.paramDecode.samplerate = AUDIO_SAMPLE_RATE_32000;
            break;
        case AUD_SAM_44_1K:
            g_audio_player.paramDecode.samplerate = AUDIO_SAMPLE_RATE_44100;
            break;
        case AUD_SAM_48K:
            g_audio_player.paramDecode.samplerate = AUDIO_SAMPLE_RATE_48000;
            break;
        default:
            LOG("Unsupported sampleRate %d", sampleRate);
            return -EINVAL;
    }

    switch (bitDepth) {
        case AUD_BIT_8:
            g_audio_player.paramDecode.bitwidth = AUDIO_BIT_WIDTH_8;
            break;
        case AUD_BIT_16:
            g_audio_player.paramDecode.bitwidth = AUDIO_BIT_WIDTH_16;
            break;
        case AUD_BIT_32:
            g_audio_player.paramDecode.bitwidth = AUDIO_BIT_WIDTH_24;
            break;
        default:
            LOG("Unsupported bitDepth %d", bitDepth);
            return -EINVAL;
    }
    sv82xHandle->format = format;
    sv82xHandle->channel = channel;
    sv82xHandle->sampleRate = sampleRate;
    sv82xHandle->bitDepth = bitDepth;

    return 0;
}

int audioPlayerGetFormat(const AudioPlayerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
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

int audioPlayerAcquireStream(AudioPlayerHandle handle)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);
    EI_S32 s32Ret;
    AO_CHN AoChn = 0;
    ADEC_CHN AdChn = 0;
    EI_S32 s32AoChnCnt;
    AIO_ATTR_S stAioAttr;
    AUDIO_DEV AoDev = g_audio_player.AoDev;

    stAioAttr.enSamplerate = g_audio_player.paramDecode.samplerate;
    stAioAttr.enBitwidth = g_audio_player.paramDecode.bitwidth;
    stAioAttr.enSoundmode = g_audio_player.paramDecode.sound_mode;
    stAioAttr.u32EXFlag = 0;
    stAioAttr.u32FrmNum = 30;
    stAioAttr.u32PtNumPerFrm = g_audio_player.paramDecode.num_per_frm;
    stAioAttr.u32ChnCnt = g_audio_player.paramDecode.u32ChnCnt;

    s32Ret = SAMPLE_COMM_AUDIO_StartAdec(AdChn, &stAioAttr, g_audio_player.paramDecode.payload_type);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_AUDIO_StartAdec failed, ret=%d\n", s32Ret);
        return -EAGAIN;
    }

    stAioAttr.enSamplerate = g_audio_player.paramDecode.samplerate;
    stAioAttr.enBitwidth = g_audio_player.paramDecode.bitwidth;
    stAioAttr.enSoundmode = g_audio_player.paramDecode.sound_mode;
    stAioAttr.u32EXFlag = 0;
    stAioAttr.u32FrmNum = 30;
    stAioAttr.u32PtNumPerFrm = g_audio_player.paramDecode.num_per_frm;
    stAioAttr.u32ChnCnt = g_audio_player.paramDecode.u32ChnCnt;

    s32AoChnCnt = stAioAttr.u32ChnCnt;
    s32Ret = SAMPLE_COMM_AUDIO_StartAo(AoDev, s32AoChnCnt, &stAioAttr, g_audio_player.enInSampleRate, g_audio_player.bAioReSample);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_AUDIO_StartAo failed, ret=%d\n", s32Ret);
        return -EAGAIN;
    }

    if (g_audio_player.enUserGetMode == EI_TRUE) {
        s32Ret = SAMPLE_COMM_AUDIO_CreatTrdAdeAo(AoDev, AoChn, AdChn, NULL);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_AUDIO_CreatTrdAdeAo failed, ret=%d\n", s32Ret);
            s32AoChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
            s32Ret |= SAMPLE_COMM_AUDIO_StopAo(AoDev, s32AoChnCnt, g_audio_player.bAioReSample);
            if (s32Ret != EI_SUCCESS) {
                LOG("SAMPLE_COMM_AUDIO_StopAo failed, ret=%d\n", s32Ret);
            }
            return -EAGAIN;
        }
    } else {
        s32Ret = SAMPLE_COMM_AUDIO_AoLinkAdec(AoDev, AoChn, AdChn);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_AUDIO_AoLinkAdec failed, ret=%d\n", s32Ret);
            s32AoChnCnt = stAioAttr.u32ChnCnt >> stAioAttr.enSoundmode;
            s32Ret |= SAMPLE_COMM_AUDIO_StopAo(AoDev, s32AoChnCnt, g_audio_player.bAioReSample);
            if (s32Ret != EI_SUCCESS) {
                LOG("SAMPLE_COMM_AUDIO_StopAo failed, ret=%d\n", s32Ret);
            }
            return -EAGAIN;
        }
    }

    setStatus(handle, AUD_PLY_STATUS_STREAM_ON);

    return 0;
}

int audioPlayerWriteFrame(AudioPlayerHandle handle, void* pData, const size_t size)
{
    EI_S32 s32Ret = EI_SUCCESS;
    AUDIO_STREAM_S stStream;
    int AdChn = g_audio_player.AdecChn;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    SV82X_HANDLE_STATUS_CHECK(sv82xHandle, AUD_PLY_STATUS_STREAM_ON);

    if (!pData)
        return -EINVAL;

    if (size <= 0) {
        s32Ret = EI_MI_ADEC_SendEndOfStream(AdChn, EI_FALSE);
        if (s32Ret != EI_SUCCESS) {
            LOG("%s: EI_MI_ADEC_SendEndOfStream failed:%d!\n", __func__, size);
            return -EAGAIN;
        }
        return 0;
    }

    memset(&stStream, 0, sizeof(AUDIO_STREAM_S));
    stStream.pStream = (EI_U8 ATTRIBUTE*) pData;
    stStream.u32Len = size;
    s32Ret = EI_MI_ADEC_SendStream(AdChn, &stStream, EI_TRUE);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_ADEC_SendStream(%d) failed with %#x!\n", AdChn, s32Ret);
        return -EAGAIN;
    }

    return 0;
}

int audioPlayerReleaseStream(AudioPlayerHandle handle)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    int s32Ret = EI_SUCCESS;
    AO_CHN AoChn = g_audio_player.AoChn;
    ADEC_CHN AdChn = g_audio_player.AdecChn;
    AUDIO_DEV AoDev = g_audio_player.AoDev;
    EI_S32 s32AoChnCnt;
    EI_BOOL bResampleEn = EI_FALSE;

    if (g_audio_player.enUserGetMode == EI_TRUE) {
        s32Ret = SAMPLE_COMM_AUDIO_DestroyTrdAdec(AdChn);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_AUDIO_DestroyTrdAdec failed %x\n", s32Ret);
            return -EAGAIN;
        }
    } else {
        s32Ret = SAMPLE_COMM_AUDIO_AoUnbindAdec(AoDev, AoChn, AdChn);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_AUDIO_AoUnbindAdec failed %x\n", s32Ret);
            return -EAGAIN;
        }
    }

    s32AoChnCnt = g_audio_player.paramDecode.u32ChnCnt >> g_audio_player.paramDecode.sound_mode;
    s32Ret = SAMPLE_COMM_AUDIO_StopAo(AoDev, s32AoChnCnt, bResampleEn);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_AUDIO_StopAo failed %x\n", s32Ret);
        return -EAGAIN;
    }
    s32Ret = SAMPLE_COMM_AUDIO_StopAdec(AdChn);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_AUDIO_StopAdec failed %x\n", s32Ret);
        return -EAGAIN;
    }
    return setStatus(handle, AUD_PLY_STATUS_STREAM_OFF);
}

void audioPlayerDestroy(AudioPlayerHandle handle)
{
    if (!handle)
        return;

    SV82X_HANDLE_GET(handle);

    if (sv82xHandle->status == AUD_PLY_STATUS_STREAM_ON)
        audioPlayerReleaseStream(handle);

    setStatus(handle, AUD_PLY_STATUS_NOT_READY);

    free(handle);
}
