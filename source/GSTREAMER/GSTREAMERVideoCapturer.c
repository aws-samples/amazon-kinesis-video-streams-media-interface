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
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>

#include "GSTREAMERCommon.h"
#include "GSTREAMERPort.h"
#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#define GST_HANDLE_GET(x) GstVideoCapturer* videoHandle = (GstVideoCapturer*) ((x))

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
} GstVideoCapturer;

static GSTAppSinkSrc gstAppSinkVideHandle;

static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    if (newStatus != videoHandle->status) {
        videoHandle->status = newStatus;
        LOG("VideoCapturer new status[%d]", newStatus);
    }

    return 0;
}

VideoCapturerHandle videoCapturerCreate(void)
{
    GstVideoCapturer* videoHandle = NULL;

    if (!(videoHandle = (GstVideoCapturer*) malloc(sizeof(GstVideoCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(videoHandle, 0, sizeof(GstVideoCapturer));

    // Now we have sample frames for H.264, 1080p
    videoHandle->capability.formats = (1 << (VID_FMT_H264 - 1));
    videoHandle->capability.resolutions = (1 << (VID_RES_1080P - 1));

    memset(&gstAppSinkVideHandle, 0, sizeof(GSTAppSinkSrc));
    gstAppSinkVideHandle.capturerHandle = (void*)videoHandle;
    if (0 != initGstVideoSrc(&gstAppSinkVideHandle)) {
        LOG("initGstVideoSrc failed.");
        return NULL;
    }

    setStatus((VideoCapturerHandle) videoHandle, VID_CAP_STATUS_STREAM_OFF);

    return (VideoCapturerHandle) videoHandle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle handle)
{
    if (!handle) {
        return VID_CAP_STATUS_NOT_READY;
    }

    GST_HANDLE_GET(handle);
    return videoHandle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle handle, VideoCapability* pCapability)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = videoHandle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(videoHandle, VID_CAP_STATUS_STREAM_OFF);

    switch (format) {
        case VID_FMT_H264:
            break;

        default:
            LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    switch (resolution) {
        case VID_RES_1080P:
            break;

        default:
            LOG("Unsupported resolution %d", resolution);
            return -EINVAL;
    }

    videoHandle->format = format;
    videoHandle->resolution = resolution;

    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    *pFormat = videoHandle->format;
    *pResolution = videoHandle->resolution;

    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    startGstVideoSrc();

    return setStatus(handle, VID_CAP_STATUS_STREAM_ON);
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(videoHandle, VID_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    GstSample *sample;
    GstElement *pipeline = NULL, *videoSink = NULL;
    pipeline = getPipeline(VIDEO_CAPTURE_PIPELINE);
    videoSink = gst_bin_get_by_name(GST_BIN(pipeline), GST_VIDEO_SINK_NAME);
    g_signal_emit_by_name(videoSink, "pull-sample", &sample);

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
                    LOG("(Video)FrameDataBufferSize(%ld) < frameSize(%ld), frame dropped", frameDataBufferSize, map.size);
                }
            }
            gst_buffer_unmap(buffer, &map);
        }
        gst_sample_unref(sample);
    }

    return -EINVAL;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{
    GST_HANDLE_NULL_CHECK(handle);
    GST_HANDLE_GET(handle);

    GST_HANDLE_STATUS_CHECK(videoHandle, VID_CAP_STATUS_STREAM_ON);

    stopGstVideoSrc();

    return setStatus(handle, VID_CAP_STATUS_STREAM_OFF);
}

void videoCapturerDestroy(VideoCapturerHandle handle)
{
    if (!handle) {
        return;
    }

    GST_HANDLE_GET(handle);

    if (videoHandle->status == VID_CAP_STATUS_STREAM_ON) {
        videoCapturerReleaseStream(handle);
    }

    deinitGstVideoSrc();

    setStatus(handle, VID_CAP_STATUS_NOT_READY);

    free(handle);
}
