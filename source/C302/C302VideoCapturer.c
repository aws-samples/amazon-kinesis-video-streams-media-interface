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

#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#include "C302Common.h"
#include "ipc_video_api.h"

#define USING_HARD_STREAM_VIDEO

#define DEFAULT_STREAM_CHN 0
#define DEFAULT_STREAM_FPS 30
#define DEFAULT_STREAM_PRO 1

#define C302_HANDLE_GET(x) C302VideoCapturer* videoHandle = (C302VideoCapturer*) ((x))

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    IPC_VIDEO_FRAME vframe;
} C302VideoCapturer;

static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    if (newStatus != videoHandle->status) {
        videoHandle->status = newStatus;
        KVS_LOG("VideoCapturer set new status[%d]\n", newStatus);
    }

    return 0;
}

static int startRecvPic(VideoCapturerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);
    HANDLE_STATUS_CHECK(videoHandle, VID_CAP_STATUS_STREAM_ON);

#ifdef USING_HARD_STREAM_VIDEO
    if (videoHandle->format == VID_FMT_H264) {
        if (IPC_VIDEO_Enable() < 0) {
            KVS_LOG("IPC_VIDEO_Enable() failed\n");
            return -EAGAIN;
        }
        IPC_VFRAME_Init(&videoHandle->vframe);
    } else {
        KVS_LOG("err todo\n");
        return -EAGAIN;
    }
#endif
    return 0;
}

static int stopRecvPic(VideoCapturerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_VIDEO
    if (videoHandle->format == VID_FMT_H264) {
        IPC_VFRAME_Relase(&videoHandle->vframe);
        IPC_VIDEO_Disable();
    } else {
        KVS_LOG("err todo\n");
        return -EAGAIN;
    }
#endif
    return 0;
}

VideoCapturerHandle videoCapturerCreate(void)
{
    C302VideoCapturer* videoHandle = NULL;

    if (!(videoHandle = (C302VideoCapturer*) malloc(sizeof(C302VideoCapturer)))) {
        KVS_LOG("videoHandle malloc failed\n");
        return NULL;
    }

    memset(videoHandle, 0, sizeof(C302VideoCapturer));

#ifdef USING_HARD_STREAM_VIDEO
    if (IPC_CFG_Init(CFG_VIDEO_FLAG) < 0) {
        KVS_LOG("IPC_CFG_Init video failed\n");
        free(videoHandle);
        return NULL;
    }
#endif

    videoHandle->capability.formats = (1 << (VID_FMT_H264 - 1));
    videoHandle->capability.resolutions = (1 << (VID_RES_1080P - 1)) | (1 << (VID_RES_2K - 1));

    setStatus((VideoCapturerHandle) videoHandle, VID_CAP_STATUS_STREAM_OFF);

    return (VideoCapturerHandle) videoHandle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle handle)
{
    if (!handle) {
        return VID_CAP_STATUS_NOT_READY;
    }

    C302_HANDLE_GET(handle);

    return videoHandle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle handle, VideoCapability* pCapability)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = videoHandle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_VIDEO
    IPC_RESOLUTION_SIZE vresolution;
    IPC_VCODEC_TYPE vcodec;

    switch (format) {
        case VID_FMT_H264:
            vcodec = CODEC_H264;
            break;
        case VID_FMT_H265:
            vcodec = CODEC_H265;
            break;
        default:
            KVS_LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    switch (resolution) {
        case VID_RES_1080P:
            vresolution.u32Width = 1920;
            vresolution.u32Height = 1080;
            break;
        case VID_RES_720P:
            vresolution.u32Width = 1280;
            vresolution.u32Height = 720;
            break;
        default:
            KVS_LOG("Unsupported resolution %d", resolution);
            return -EINVAL;
    }

    int ret = IPC_VIDEO_SetStreamConfig(DEFAULT_STREAM_CHN, FORMAT_NONE, &vresolution, vcodec, DEFAULT_STREAM_FPS, DEFAULT_STREAM_PRO);
    if (ret < 0) {
        IPC_CFG_UnInit(CFG_VIDEO_FLAG);
        return -EAGAIN;
    }
    ret = IPC_VIDEO_Init();
    if (ret < 0) {
        IPC_CFG_UnInit(CFG_VIDEO_FLAG);
        return -EAGAIN;
    }
#endif

    videoHandle->format = format;
    videoHandle->resolution = resolution;
    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    *pFormat = videoHandle->format;
    *pResolution = videoHandle->resolution;

    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    setStatus(videoHandle, VID_CAP_STATUS_STREAM_ON);

    return startRecvPic(videoHandle);
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    int ret;

    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);
    HANDLE_STATUS_CHECK(videoHandle, VID_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        KVS_LOG("param err\n");
        return -EINVAL;
    }

#ifdef USING_HARD_STREAM_VIDEO
    if (videoHandle->format == VID_FMT_H264) {
        ret = IPC_VIDEO_GetFrame(&videoHandle->vframe, DEFAULT_STREAM_CHN, 0);
        if (ret > 0) {
            // KVS_LOG("IPC_VIDEO_GetFrame:%u\n", videoHandle->vframe.u32len);
            if (videoHandle->vframe.u32len > frameDataBufferSize) {
                KVS_LOG("Video buffer is too small\n");
                return -EAGAIN;
            }
            memcpy(pFrameDataBuffer, videoHandle->vframe.u8data, videoHandle->vframe.u32len);
            *pFrameSize = videoHandle->vframe.u32len;
            *pTimestamp = videoHandle->vframe.u64pts;
        } else {
            return -EAGAIN;
        }
    } else {
        KVS_LOG("format not support");
        return -EINVAL;
    }
#endif

    return 0;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    C302_HANDLE_GET(handle);

    stopRecvPic(videoHandle);

    return setStatus(videoHandle, VID_CAP_STATUS_STREAM_OFF);
}

void videoCapturerDestroy(VideoCapturerHandle handle)
{
    if (!handle) {
        return;
    }
    C302_HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_VIDEO
    if (videoHandle->status == VID_CAP_STATUS_STREAM_ON) {
        videoCapturerReleaseStream(videoHandle);
    }
    IPC_VIDEO_UnInit();
    IPC_CFG_UnInit(CFG_VIDEO_FLAG);
#endif

    setStatus(videoHandle, VID_CAP_STATUS_NOT_READY);
    free(videoHandle);
}
