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

#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#include "V4l2Capturer.h"

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    uint8_t channelNum;
    V4l2CapturerHandle privHandle;
} V4L2VideoCapturer;

VideoCapturerHandle videoCapturerCreate(void)
{
    V4L2VideoCapturer v4l2Handle;
    v4l2Handle.privHandle = v4l2CapturerOpen("/dev/video0");
    return NULL;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle const handle)
{
    return VID_CAP_STATUS_NOT_READY;
}

int videoCapturerGetCapability(const VideoCapturerHandle const handle, VideoCapability* pCapability)
{
    return -EAGAIN;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{
    return -EAGAIN;
}

int videoCapturerGetFormat(const VideoCapturerHandle const handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    return -EAGAIN;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    return -EAGAIN;
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    return -EAGAIN;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{
    return -EAGAIN;
}

void videoCapturerDestory(VideoCapturerHandle handle)
{
}
