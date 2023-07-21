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
#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#include "fun_stream_mgr.h"
#include "ak_common.h"
#include "ak_log.h"
#include "aas_types.h"

#define AK3918_HANDLE_GET(x) AK3918VideoCapturer* AK3918Handle = (AK3918VideoCapturer*) ((x))

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    int nVideoHandle;
} AK3918VideoCapturer;

static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    if (newStatus != AK3918Handle->status) {
        AK3918Handle->status = newStatus;
        LOG("VideoCapturer new status[%d]", newStatus);
    }

    return 0;
}

VideoCapturerHandle videoCapturerCreate(void)
{
    AK3918VideoCapturer* AK3918Handle = NULL;
    if (!(AK3918Handle = (AK3918VideoCapturer*) malloc(sizeof(AK3918VideoCapturer)))) {
        LOG("OOM");

        return NULL;
    }

    memset(AK3918Handle, 0, sizeof(AK3918VideoCapturer));
    AK3918Handle->nVideoHandle = -1;
    int iret = AA_LS_RegStreamClient(0, AA_LS_STREAM_VIDEO, &AK3918Handle->nVideoHandle, NULL, NULL, NULL, NULL);

    AK3918Handle->capability.formats = (1 << (VID_FMT_H264 - 1));
    AK3918Handle->capability.resolutions = (1 << (VID_RES_1080P - 1));

    setStatus((VideoCapturerHandle) AK3918Handle, VID_CAP_STATUS_STREAM_OFF);

    return (VideoCapturerHandle) AK3918Handle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle handle)
{
    if (!handle) {
        return VID_CAP_STATUS_NOT_READY;
    }

    AK3918_HANDLE_GET(handle);

    return AK3918Handle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle handle, VideoCapability* pCapability)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = AK3918Handle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    AK3918_HANDLE_STATUS_CHECK(AK3918Handle, VID_CAP_STATUS_STREAM_OFF);

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

    AK3918Handle->format = format;
    AK3918Handle->resolution = resolution;

    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    *pFormat = AK3918Handle->format;
    *pResolution = AK3918Handle->resolution;

    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    return setStatus(handle, VID_CAP_STATUS_STREAM_ON);
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    AK3918_HANDLE_STATUS_CHECK(AK3918Handle, VID_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    int ret = 0;
    aa_frame_info frame;
    static int ic = 0;
    while (1) {
        int iRet = AA_LS_GetFrame(0, AA_LS_STREAM_VIDEO, AK3918Handle->nVideoHandle, &frame);
        if (iRet != 0) {
            ak_sleep_ms(5);
        } else {
            if (ic++ % 25 == 0) {
                LOG("Getframe  : recv a frame((%d, %d, %d) \n", frame.uiDataLen, frame.iWidth, frame.iHeight);
            }
            if (frameDataBufferSize > frame.uiDataLen) {
                memcpy(pFrameDataBuffer, frame.pData, frame.uiDataLen);
                *pFrameSize = frame.uiDataLen;
                *pTimestamp = frame.u64Time;
            }
            break;
        }
    }

    return ret;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{
    AK3918_HANDLE_NULL_CHECK(handle);
    AK3918_HANDLE_GET(handle);

    AK3918_HANDLE_STATUS_CHECK(AK3918Handle, VID_CAP_STATUS_STREAM_ON);

    AA_LS_UnRegStreamClient(0, VID_CAP_STATUS_STREAM_OFF, AK3918Handle->nVideoHandle);

    return setStatus(handle, VID_CAP_STATUS_STREAM_OFF);
}

void videoCapturerDestroy(VideoCapturerHandle handle)
{
    if (!handle) {
        return;
    }

    AK3918_HANDLE_GET(handle);

    if (AK3918Handle->status == VID_CAP_STATUS_STREAM_ON) {
        videoCapturerReleaseStream(handle);
    }

    setStatus(handle, VID_CAP_STATUS_NOT_READY);

    free(handle);
}
