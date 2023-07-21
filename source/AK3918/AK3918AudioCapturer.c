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

#include "AK3918Common.h"
#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"

#include "fun_stream_mgr.h"
#include "ak_common.h"
#include "ak_log.h"
#include "aas_types.h"

#define AK3918_HANDLE_GET(x) AK3918AudioCapturer* AK3918Handle = (AK3918AudioCapturer*) ((x))

typedef struct {
    AudioCapturerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
    int nAudioHandle;
} AK3918AudioCapturer;

static int setStatus(AudioCapturerHandle handle, const AudioCapturerStatus newStatus)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    if (newStatus != AK3918Handle->status) {
        AK3918Handle->status = newStatus;
        LOG("AudioCapturer new status[%d]", newStatus);
    }

    return 0;
}

extern int ipc_main();
static int g_init = 0;
AudioCapturerHandle audioCapturerCreate(void)
{
    if (g_init == 0) {
        ipc_main();
        LOG("@@@@@@@@@ %s  @@@@@@@@ %d init ok \n", __FUNCTION__, __LINE__);
        g_init = 1;
    } else {
        LOG("@@@@@@@@@ %s  @@@@@@@@@ %d repeate ! \n", __FUNCTION__, __LINE__);
    }

    AK3918AudioCapturer* AK3918Handle = NULL;

    if (!(AK3918Handle = (AK3918AudioCapturer*) malloc(sizeof(AK3918AudioCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(AK3918Handle, 0, sizeof(AK3918AudioCapturer));
    AK3918Handle->nAudioHandle = -1;
    int iRet = AA_LS_RegStreamClient(0, AA_LS_STREAM_AUDIO, &AK3918Handle->nAudioHandle, NULL, NULL, NULL, NULL);

    // Now we have sample frames for G.711 ALAW and AAC, MONO, 8k, 16 bits
    AK3918Handle->capability.formats = (1 << (AUD_FMT_G711A - 1));
    AK3918Handle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    AK3918Handle->capability.sampleRates = (1 << (AUD_SAM_8K - 1));
    AK3918Handle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));
    setStatus((AudioCapturerHandle) AK3918Handle, AUD_CAP_STATUS_STREAM_OFF);

    return (AudioCapturerHandle) AK3918Handle;
}

AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle handle)
{
    if (!handle) {
        return AUD_CAP_STATUS_NOT_READY;
    }

    AK3918_HANDLE_GET(handle);

    return AK3918Handle->status;
}

int audioCapturerGetCapability(const AudioCapturerHandle handle, AudioCapability* pCapability)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = AK3918Handle->capability;

    return 0;
}

int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    AK3918_HANDLE_STATUS_CHECK(AK3918Handle, AUD_CAP_STATUS_STREAM_OFF);
    LOG("format:     %d,AudioChannel:     %d,AudioSampleRate sampleRate:   %d,  AudioBitDepth bitDepth:   %d\n", format, channel, sampleRate,
        bitDepth);
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

    AK3918Handle->format = format;
    AK3918Handle->channel = channel;
    AK3918Handle->sampleRate = sampleRate;
    AK3918Handle->bitDepth = bitDepth;

    return 0;
}

int audioCapturerGetFormat(const AudioCapturerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
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

int audioCapturerAcquireStream(AudioCapturerHandle handle)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    return setStatus(handle, AUD_CAP_STATUS_STREAM_ON);
}

int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    AK3918_HANDLE_STATUS_CHECK(AK3918Handle, AUD_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    int ret = 0;
    aa_frame_info Audioframe;

    while (1) {
        int iRet = AA_LS_GetFrame(0, AA_LS_STREAM_AUDIO, AK3918Handle->nAudioHandle, &Audioframe);
        if (iRet != 0) {
            ak_sleep_ms(5);
        } else {
            if (AK3918Handle->format == AUD_FMT_G711A) {
                if (frameDataBufferSize > Audioframe.uiDataLen) {
                    memcpy(pFrameDataBuffer, Audioframe.pData, Audioframe.uiDataLen);
                    *pFrameSize = Audioframe.uiDataLen;
                    *pTimestamp = Audioframe.u64Time * 1000;
                } else
                    LOG("framesize error!   fun : %s  Line:  %d\n", __FUNCTION__, __LINE__);
            } else {
                LOG("Audio format error:%d\n", AK3918Handle->format);
            }
            break;
        }
    }

    return ret;
}

int audioCapturerReleaseStream(AudioCapturerHandle handle)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    AK3918_HANDLE_STATUS_CHECK(AK3918Handle, AUD_CAP_STATUS_STREAM_ON);

    AA_LS_UnRegStreamClient(0, AUD_CAP_STATUS_STREAM_OFF, AK3918Handle->nAudioHandle);
    return setStatus(handle, AUD_CAP_STATUS_STREAM_OFF);
}

void audioCapturerDestroy(AudioCapturerHandle handle)
{
    if (!handle) {
        return;
    }

    AK3918_HANDLE_GET(handle);

    if (AK3918Handle->status == AUD_CAP_STATUS_STREAM_ON) {
        audioCapturerReleaseStream(handle);
    }

    setStatus(handle, AUD_CAP_STATUS_NOT_READY);

    free(handle);
}
