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
#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#include "V4L2Port.h"
#include "V4l2Capturer.h"

#define V4L2_TARGET_BITRATE             (5 * 1024 * 1024LL)
#define V4L2_SYNC_GET_FRAME_TIMEOUT_SEC (1)

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    V4l2CapturerHandle privHandle;
} V4L2VideoCapturer;

#define V4L2_HANDLE_GET(x) V4L2VideoCapturer* v4l2Handle = (V4L2VideoCapturer*) ((x))

static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{
    V4L2_HANDLE_NULL_CHECK(handle);
    V4L2_HANDLE_GET(handle);

    if (newStatus != v4l2Handle->status) {
        v4l2Handle->status = newStatus;
        LOG("VideoCapturer new status[%d]", newStatus);
    }

    return 0;
}

VideoCapturerHandle videoCapturerCreate(void)
{
    V4L2VideoCapturer* v4l2Handle = NULL;

    if (!(v4l2Handle = (V4L2VideoCapturer*) malloc(sizeof(V4L2VideoCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(v4l2Handle, 0, sizeof(V4L2VideoCapturer));

    v4l2Handle->privHandle = v4l2CapturerOpen("/dev/video0");

    if (!v4l2Handle->privHandle) {
        LOG("Failed to open /dev/video0");
        videoCapturerDestroy((VideoCapturerHandle) v4l2Handle);
        return NULL;
    }

    // Now implementation supports H.264, 1080p, 720p, 480p, 360p and 320p
    v4l2Handle->capability.formats = (1 << (VID_FMT_H264 - 1));
    v4l2Handle->capability.resolutions =
        (1 << (VID_RES_1080P - 1)) | (1 << (VID_RES_720P - 1)) | (1 << (VID_RES_480P - 1)) | (1 << (VID_RES_360P - 1)) | (1 << (VID_RES_320P - 1));

    setStatus((VideoCapturerHandle) v4l2Handle, VID_CAP_STATUS_STREAM_OFF);

    return (VideoCapturerHandle) v4l2Handle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle handle)
{
    if (!handle) {
        return VID_CAP_STATUS_NOT_READY;
    }

    V4L2_HANDLE_GET(handle);
    return v4l2Handle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle handle, VideoCapability* pCapability)
{
    V4L2_HANDLE_NULL_CHECK(handle);
    V4L2_HANDLE_GET(handle);

    if (!pCapability) {
        return -EINVAL;
    }

    *pCapability = v4l2Handle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{
    V4L2_HANDLE_NULL_CHECK(handle);
    V4L2_HANDLE_GET(handle);

    V4L2_HANDLE_STATUS_CHECK(v4l2Handle, VID_CAP_STATUS_STREAM_OFF);

    uint32_t width, height = 0;
    switch (resolution) {
        case VID_RES_1080P:
            width = 1920;
            height = 1080;
            break;
        case VID_RES_720P:
            width = 1080;
            height = 720;
            break;
        case VID_RES_480P:
            width = 720;
            height = 480;
            break;
        case VID_RES_360P:
            width = 480;
            height = 360;
            break;
        case VID_RES_320P:
            width = 416;
            height = 320;
            break;

        default:
            LOG("Unsupported resolution %d", resolution);
            return -EINVAL;
    }

    V4l2CapturerFormat privFormat = V4L2_CAP_FMT_H264;
    switch (format) {
        case VID_FMT_H264:
            privFormat = V4L2_CAP_FMT_H264;
            break;

        default:
            LOG("Unsupported format %d", format);
            return -EINVAL;
            break;
    }

    if (v4l2CapturerConfig(v4l2Handle->privHandle, width, height, privFormat, V4L2_TARGET_BITRATE)) {
        LOG("Failed to config V4L2 Capturer");
        return -EAGAIN;
    }

    v4l2Handle->format = format;
    v4l2Handle->resolution = resolution;

    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    V4L2_HANDLE_NULL_CHECK(handle);
    V4L2_HANDLE_GET(handle);

    *pFormat = v4l2Handle->format;
    *pResolution = v4l2Handle->resolution;

    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    V4L2_HANDLE_NULL_CHECK(handle);
    V4L2_HANDLE_GET(handle);

    if (v4l2CapturerStartStreaming(v4l2Handle->privHandle)) {
        LOG("Failed to acquire stream");
        return -EAGAIN;
    }

    return setStatus(handle, VID_CAP_STATUS_STREAM_ON);
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    V4L2_HANDLE_NULL_CHECK(handle);
    V4L2_HANDLE_GET(handle);

    V4L2_HANDLE_STATUS_CHECK(v4l2Handle, VID_CAP_STATUS_STREAM_ON);

    int ret = v4l2CapturerSyncGetFrame(v4l2Handle->privHandle, V4L2_SYNC_GET_FRAME_TIMEOUT_SEC, pFrameDataBuffer, frameDataBufferSize, pFrameSize);
    if (!ret) {
        *pTimestamp = getEpochTimestampInUs();
    }

    return ret;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{
    V4L2_HANDLE_NULL_CHECK(handle);
    V4L2_HANDLE_GET(handle);

    if (v4l2CapturerStopStreaming(v4l2Handle->privHandle)) {
        LOG("Failed to release stream");
        return -EAGAIN;
    }

    return setStatus(handle, VID_CAP_STATUS_STREAM_OFF);
}

void videoCapturerDestroy(VideoCapturerHandle handle)
{
    if (!handle) {
        return;
    }
    V4L2_HANDLE_GET(handle);

    if (v4l2Handle->status == VID_CAP_STATUS_STREAM_ON) {
        videoCapturerReleaseStream(v4l2Handle);
    }

    setStatus(handle, VID_CAP_STATUS_NOT_READY);

    v4l2CapturerClose(v4l2Handle->privHandle);

    free(handle);
}
