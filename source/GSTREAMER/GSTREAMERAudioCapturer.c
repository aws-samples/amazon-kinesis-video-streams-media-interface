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
#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"

#define AUDIO_AAC_ADTS_HEADER_LENGTH 7
#define AUDIO_AAC_ADTS_HEADER_CRC_LENGTH 2

#define GST_HANDLE_GET(x) GstAudioCapturer* audioHandle = (GstAudioCapturer*) ((x))

typedef struct {
    AudioCapturerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} GstAudioCapturer;

static GSTAppSinkSrc gstAppSinkAudioHandle;

static int setStatus(AudioCapturerHandle handle, const AudioCapturerStatus newStatus)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    if (newStatus != audioHandle->status) {
        audioHandle->status = newStatus;
        LOG("AudioCapturer new status[%d]", newStatus);
    }

    return 0;
}

AudioCapturerHandle audioCapturerCreate(void)
{
    GstAudioCapturer* audioHandle = NULL;

    if (!(audioHandle = (GstAudioCapturer*) malloc(sizeof(GstAudioCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(audioHandle, 0, sizeof(GstAudioCapturer));

    // Now we have sample frames for G.711 ALAW and AAC, MONO, 8k, 16 bits
    audioHandle->capability.formats = (1 << (AUD_FMT_G711A - 1)) | (1 << (AUD_FMT_AAC - 1));
    audioHandle->capability.channels = (1 << (AUD_CHN_MONO - 1));
    audioHandle->capability.sampleRates = (1 << (AUD_SAM_8K - 1));
    audioHandle->capability.bitDepths = (1 << (AUD_BIT_16 - 1));

    memset(&gstAppSinkAudioHandle, 0, sizeof(GSTAppSinkSrc));
    gstAppSinkAudioHandle.capturerHandle = (void*)audioHandle;
    if (0 != initGstAudioSrc(&gstAppSinkAudioHandle)) {
        LOG("initGstAudioSrc failed.");
        return NULL;
    }

    setStatus((AudioCapturerHandle) audioHandle, AUD_CAP_STATUS_STREAM_OFF);

    return (AudioCapturerHandle) audioHandle;
}

AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle handle)
{
    if (!handle) {
        return AUD_CAP_STATUS_NOT_READY;
    }

    GST_HANDLE_GET(handle);
    return audioHandle->status;
}

int audioCapturerGetCapability(const AudioCapturerHandle handle, AudioCapability* pCapability)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = audioHandle->capability;

    return 0;
}

int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(audioHandle, AUD_CAP_STATUS_STREAM_OFF);

    switch (format) {
        case AUD_FMT_G711A:
            break;

        case AUD_FMT_AAC:
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

int audioCapturerGetFormat(const AudioCapturerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
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

int audioCapturerAcquireStream(AudioCapturerHandle handle)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    startGstAudioSrc();

    return setStatus(handle, AUD_CAP_STATUS_STREAM_ON);
}

int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(audioHandle, AUD_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    GstSample *sample;
    GstElement *pipeline = NULL, *audioSink = NULL;
    pipeline = getPipeline(AUDIO_CAPTURE_PIPELINE);
    audioSink = gst_bin_get_by_name(GST_BIN(pipeline), GST_AUDIO_SINK_NAME);
    g_signal_emit_by_name(audioSink, "pull-sample", &sample);

    if (sample) {
        GstBuffer *buffer = gst_sample_get_buffer(sample);
        if (buffer) {
            GstMapInfo map;
            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                if (map.size < frameDataBufferSize) {
                    memcpy(pFrameDataBuffer, map.data, map.size);
                    *pFrameSize = map.size;
                    *pTimestamp = getEpochTimestampInUs();
                    gst_buffer_unmap(buffer, &map);
                    gst_sample_unref(sample);
                    return 0;
                } else {
                    LOG("(Audio)FrameDataBufferSize(%ld) < frameSize(%ld), frame dropped", frameDataBufferSize, map.size);
                }
            }
            gst_buffer_unmap(buffer, &map);
        }
        gst_sample_unref(sample);
    }

    return -EINVAL;
}

int audioCapturerReleaseStream(AudioCapturerHandle handle)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(audioHandle, AUD_CAP_STATUS_STREAM_ON);

    stopGstAudioSrc();

    return setStatus(handle, AUD_CAP_STATUS_STREAM_OFF);
}

void audioCapturerDestroy(AudioCapturerHandle handle)
{
    if (!handle) {
        return;
    }

    GST_HANDLE_GET(handle);

    if (audioHandle->status == AUD_CAP_STATUS_STREAM_ON) {
        audioCapturerReleaseStream(handle);
    }

    deinitGstAudioSrc();

    setStatus(handle, AUD_CAP_STATUS_NOT_READY);

    free(handle);
}
