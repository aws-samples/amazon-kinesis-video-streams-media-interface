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

#include "V4L2Common.h"
#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"

typedef struct {
    AudioCapturerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} V4L2AudioCapturer;

AudioCapturerHandle audioCapturerCreate(void)
{
    LOG("V4L2 AudioCapturer is not implemented yet");

    return NULL;
}

AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle const handle)
{
    return AUD_CAP_STATUS_NOT_READY;
}

int audioCapturerGetCapability(const AudioCapturerHandle const handle, AudioCapability* pCapability)
{
    return 0;
}

int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth)
{
    return 0;
}

int audioCapturerGetFormat(const AudioCapturerHandle const handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                           AudioBitDepth* pBitDepth)
{
    return 0;
}

int audioCapturerAcquireStream(AudioCapturerHandle handle)
{
    return 0;
}

int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    sleep(3);
    return -EAGAIN;
}

int audioCapturerReleaseStream(AudioCapturerHandle handle)
{
    return 0;
}

void audioCapturerDestory(AudioCapturerHandle handle)
{
}
