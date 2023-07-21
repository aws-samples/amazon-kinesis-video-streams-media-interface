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
#include <stdlib.h>
#include <string.h>

#include "V4L2Common.h"
#include "com/amazonaws/kinesis/video/player/AudioPlayer.h"

typedef struct {
    AudioPlayerStatus status;
    AudioCapability capability;
    AudioFormat format;
    AudioChannel channel;
    AudioBitDepth bitDepth;
    AudioSampleRate sampleRate;
} V4L2AudioPlayer;

AudioPlayerHandle audioPlayerCreate(void)
{
    LOG("V4L2 AudioPlayer is not implemented yet");

    return NULL;
}

AudioPlayerStatus audioPlayerGetStatus(const AudioPlayerHandle handle)
{
    return AUD_PLY_STATUS_NOT_READY;
}

int audioPlayerGetCapability(const AudioPlayerHandle handle, AudioCapability* pCapability)
{
    return -EAGAIN;
}

int audioPlayerSetFormat(AudioPlayerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                         const AudioBitDepth bitDepth)
{
    return -EAGAIN;
}

int audioPlayerGetFormat(const AudioPlayerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                         AudioBitDepth* pBitDepth)
{
    return -EAGAIN;
}

int audioPlayerAcquireStream(AudioPlayerHandle handle)
{
    return -EAGAIN;
}

int audioPlayerWriteFrame(AudioPlayerHandle handle, void* pData, const size_t size)
{
    return -EAGAIN;
}

int audioPlayerReleaseStream(AudioPlayerHandle handle)
{
    return -EAGAIN;
}

void audioPlayerDestroy(AudioPlayerHandle handle)
{
}
