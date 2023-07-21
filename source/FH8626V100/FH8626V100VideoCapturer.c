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

#include "FH8626V100Common.h"
#include "sample_common.h"

#define USING_HARD_STREAM_VIDEO

#define HANDLE_GET(x) FH8626V100VideoCapturer* videoHandle = (FH8626V100VideoCapturer*) ((x))

#define MAIN_CHN 0

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    uint8_t channelNum;
} FH8626V100VideoCapturer;

static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    if (newStatus != videoHandle->status) {
        videoHandle->status = newStatus;
        KVS_LOG("VideoCapturer set new status[%d]\n", newStatus);
    }

    return 0;
}

static int startRecvPic(VideoCapturerHandle handle, uint8_t chnNum)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);
    HANDLE_STATUS_CHECK(videoHandle, VID_CAP_STATUS_STREAM_ON);

#ifdef USING_HARD_STREAM_VIDEO
    if (videoHandle->format == VID_FMT_H264) {
        if (FH_VENC_StartRecvPic(chnNum)) {
            KVS_LOG("FH_VENC_StartRecvPic(%d) failed\n", chnNum);
            return -EAGAIN;
        }

    } else {
        KVS_LOG("err todo\n");
        return -EAGAIN;
    }
#endif

    return 0;
}

static int stopRecvPic(VideoCapturerHandle handle, uint8_t chnNum)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_VIDEO
    if (videoHandle->format == VID_FMT_H264) {
        if (FH_VENC_StopRecvPic(chnNum)) {
            KVS_LOG("FH_VENC_StopRecvPic(%d) failed\n", chnNum);
            return -EAGAIN;
        }
    } else {
        KVS_LOG("err todo\n");
        return -EAGAIN;
    }
#endif
    return 0;
}

VideoCapturerHandle videoCapturerCreate(void)
{
    FH8626V100VideoCapturer* videoHandle = NULL;

    if (!(videoHandle = (FH8626V100VideoCapturer*) malloc(sizeof(FH8626V100VideoCapturer)))) {
        KVS_LOG("videoHandle malloc failed\n");
        return NULL;
    }

    memset(videoHandle, 0, sizeof(FH8626V100VideoCapturer));

#ifdef USING_HARD_STREAM_VIDEO
    if (sample_video_init()) {
        KVS_LOG("video init failed\n");
        free(videoHandle);
        return NULL;
    }

    FH_VENC_StopRecvPic(videoHandle->channelNum);
#endif

    // Now implementation supports H.264, RAW(NV12), 1080p, 720p, 480p, 360p and 320p
    videoHandle->capability.formats = (1 << (VID_FMT_H264 - 1));
    videoHandle->capability.resolutions = (1 << (VID_RES_1080P - 1));

    setStatus((VideoCapturerHandle) videoHandle, VID_CAP_STATUS_STREAM_OFF);

    return (VideoCapturerHandle) videoHandle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle handle)
{
    if (!handle) {
        return VID_CAP_STATUS_NOT_READY;
    }

    HANDLE_GET(handle);

    return videoHandle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle handle, VideoCapability* pCapability)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = videoHandle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{
    int ret;
    int fmt, chn, res;

    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    switch (resolution) {
        case VID_RES_1080P:
            chn = MAIN_CHN;
            videoHandle->channelNum = chn;
            res = FORMAT_1080P15;
            break;
        default:
            KVS_LOG("Unsupported resolution %d", resolution);
            return -EINVAL;
    }

    switch (format) {
        case VID_FMT_H264:
            fmt = FH_NORMAL_H264;
            break;
        default:
            KVS_LOG("Unsupported format %d", format);
            return -EINVAL;
    }

#ifdef USING_HARD_STREAM_VIDEO
    if (change_video(chn, fmt, res)) {
        KVS_LOG("change video failed\n");
        return -EINVAL;
    }
#endif

    videoHandle->format = format;
    videoHandle->resolution = resolution;

    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    *pFormat = videoHandle->format;
    *pResolution = videoHandle->resolution;

    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);

    setStatus(handle, VID_CAP_STATUS_STREAM_ON);

    return startRecvPic(handle, videoHandle->channelNum);
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    int i, ret;
    int frmlen, offset;
    FH_VENC_STREAM stream;

    HANDLE_NULL_CHECK(handle);
    HANDLE_GET(handle);
    HANDLE_STATUS_CHECK(videoHandle, VID_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        KVS_LOG("param err\n");
        return -EINVAL;
    }

#ifdef USING_HARD_STREAM_VIDEO
    if (videoHandle->format == VID_FMT_RAW) {
        KVS_LOG("TODO VID_FMT_RAW");
        return -EINVAL;
    } else if (videoHandle->format == VID_FMT_H264) {
        ret = FH_VENC_GetStream_Block(FH_STREAM_H264, &stream);
        if (ret == RETURN_OK) {
            frmlen = 0;
            for (i = 0; i < stream.h264_stream.nalu_cnt; i++) {
                frmlen += stream.h264_stream.nalu[i].length;
            }

            if (frameDataBufferSize >= frmlen) {
                *pFrameSize = frmlen;
                *pTimestamp = stream.h264_stream.time_stamp;

                offset = 0;
                for (i = 0; i < stream.h264_stream.nalu_cnt; i++) {
                    memcpy(pFrameDataBuffer + offset, stream.h264_stream.nalu[i].start, stream.h264_stream.nalu[i].length);
                    offset += stream.h264_stream.nalu[i].length;
                }
            } else {
                KVS_LOG("FrameDataBufferSize(%d) < frameSize(%d), frame dropped", frameDataBufferSize, frmlen);
                *pFrameSize = 0;
                ret = -ENOMEM;
            }

            FH_VENC_ReleaseStream(videoHandle->channelNum);
        } else {
            KVS_LOG("FH_VENC_GetStream_Block failed, %x\n", ret);
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
    HANDLE_GET(handle);

    stopRecvPic(handle, videoHandle->channelNum);

    return setStatus(handle, VID_CAP_STATUS_STREAM_OFF);
}

void videoCapturerDestroy(VideoCapturerHandle handle)
{
    if (!handle) {
        return;
    }
    HANDLE_GET(handle);

#ifdef USING_HARD_STREAM_VIDEO
    if (videoHandle->status == VID_CAP_STATUS_STREAM_ON) {
        videoCapturerReleaseStream(handle);
    }

    sample_video_exit();
#endif

    setStatus(handle, VID_CAP_STATUS_NOT_READY);
    free(handle);
}
