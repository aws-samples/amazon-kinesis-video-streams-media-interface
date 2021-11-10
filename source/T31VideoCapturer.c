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

#include <stdlib.h>
#include <string.h>

#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#include "T31Common.h"
#include <imp/imp_encoder.h>
#include <imp/imp_system.h>
#include "sample-common.h"

#define T31_VIDEO_STREAM_1080P_CHANNEL_NUM CH0_INDEX
#define T31_VIDEO_STREAM_720P_CHANNEL_NUM  CH1_INDEX
#define T31_VIDEO_STREAM_CHANNEL_NUM       2

#define T31_HANDLE_GET(x) T31VideoCapturer* t31Handle = (T31VideoCapturer*) ((x))

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    uint8_t channelNum;
} T31VideoCapturer;

extern struct chn_conf chn[];
static ATOMIC_INT t31VideoSystemUser = 0;

static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (newStatus != t31Handle->status) {
        t31Handle->status = newStatus;
        LOG("VideoCapturer new status[%d]", newStatus);
    }

    return 0;
}

static int getPacket(IMPEncoderStream* pStream, IMPEncoderPack* pPack, uint8_t* pPacketBuf, size_t uPacketSize)
{
    if (pStream == NULL || pPack == NULL || pPacketBuf == NULL || uPacketSize == 0 || pPack->length == 0) {
        return -EINVAL;
    }

    /*  virAddr is a ringbuffer, and the packet may be cut into 2 pieces. */
    uint32_t uRemainingSize = pStream->streamSize - pPack->offset;

    if (uRemainingSize < pPack->length) {
        /* The packet is cut into 2 pieces. */
        memcpy(pPacketBuf, (uint8_t*) (pStream->virAddr + pPack->offset), uRemainingSize);
        memcpy(pPacketBuf + uRemainingSize, (uint8_t*) (pStream->virAddr), pPack->length - uRemainingSize);
    } else {
        /* The packet is a complete one. */
        memcpy(pPacketBuf, (uint8_t*) (pStream->virAddr + pPack->offset), pPack->length);
    }

    return 0;
}

static int startRecvPic(VideoCapturerHandle handle, uint8_t chnNum)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    T31_HANDLE_STATUS_CHECK(t31Handle, VID_CAP_STATUS_STREAM_ON);

    if (IMP_Encoder_StartRecvPic(chnNum)) {
        LOG("IMP_Encoder_StartRecvPic(%d) failed", chnNum);
        return -EAGAIN;
    }

    LOG("IMP_Encoder_StartRecvPic(%d)", chnNum);

    return 0;
}

static int stopRecvPic(VideoCapturerHandle handle, uint8_t chnNum)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (IMP_Encoder_StopRecvPic(chnNum)) {
        LOG("IMP_Encoder_StopRecvPic(%d) failed", chnNum);
        return -EAGAIN;
    }

    LOG("IMP_Encoder_StopRecvPic(%d)", chnNum);

    return 0;
}

VideoCapturerHandle videoCapturerCreate(void)
{
    T31VideoCapturer* t31Handle = NULL;

    if (!(t31Handle = (T31VideoCapturer*) malloc(sizeof(T31VideoCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(t31Handle, 0, sizeof(T31VideoCapturer));

    if (!t31VideoSystemUser) {
        /* Step.1 System init */
        if (sample_system_init()) {
            LOG("IMP_System_Init() failed");
            free(t31Handle);
            return NULL;
        }

        chn[T31_VIDEO_STREAM_1080P_CHANNEL_NUM].payloadType = IMP_ENC_PROFILE_AVC_MAIN;
        chn[T31_VIDEO_STREAM_1080P_CHANNEL_NUM].enable = true;
        chn[T31_VIDEO_STREAM_720P_CHANNEL_NUM].payloadType = IMP_ENC_PROFILE_AVC_MAIN;
        chn[T31_VIDEO_STREAM_720P_CHANNEL_NUM].enable = true;

        /* Step.2 FrameSource init */
        if (sample_framesource_init()) {
            LOG("FrameSource init failed");
            free(t31Handle);
            return NULL;
        }

        /* Step.3 Encoder init */
        for (int i = 0; i < T31_VIDEO_STREAM_CHANNEL_NUM; i++) {
            if (chn[i].enable) {
                if (IMP_Encoder_CreateGroup(chn[i].index)) {
                    LOG("IMP_Encoder_CreateGroup(%d) error !", chn[i].index);
                    free(t31Handle);
                    return NULL;
                }
            }
        }

        if (sample_encoder_init()) {
            LOG("Encoder init failed");
            free(t31Handle);
            return NULL;
        }

        /* Step.4 Bind */
        for (int i = 0; i < T31_VIDEO_STREAM_CHANNEL_NUM; i++) {
            if (chn[i].enable) {
                if (IMP_System_Bind(&chn[i].framesource_chn, &chn[i].imp_encoder)) {
                    LOG("Bind FrameSource channel%d and Encoder failed", i);
                    free(t31Handle);
                    return NULL;
                }
            }
        }
    }

    // Now implementation supports H.264, 1080p and 720p
    t31Handle->capability.formats = (1 << (VID_FMT_H264 - 1));
    t31Handle->capability.resolutions = (1 << (VID_RES_1080P - 1)) | (1 << (VID_RES_720P - 1));

    setStatus((VideoCapturerHandle) t31Handle, VID_CAP_STATUS_STREAM_OFF);

    ATOMIC_INT_ADD(&t31VideoSystemUser);

    return (VideoCapturerHandle) t31Handle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle const handle)
{
    if (!handle) {
        return VID_CAP_STATUS_NOT_READY;
    }

    T31_HANDLE_GET(handle);
    return t31Handle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle const handle, VideoCapability* pCapability)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = t31Handle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (format != VID_FMT_H264) {
        LOG("Unsupported format %d", format);
        return -EINVAL;
    }

    if (resolution == VID_RES_1080P) {
        t31Handle->channelNum = T31_VIDEO_STREAM_1080P_CHANNEL_NUM;
    } else if (resolution == VID_RES_720P) {
        t31Handle->channelNum = T31_VIDEO_STREAM_720P_CHANNEL_NUM;
    } else {
        LOG("Unsupported resolution %d", resolution);
        return -EINVAL;
    }

    t31Handle->format = format;
    t31Handle->resolution = resolution;

    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle const handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    *pFormat = t31Handle->format;
    *pResolution = t31Handle->resolution;

    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (sample_framesource_streamon()) {
        LOG("ImpStreamOn failed");
        return -EAGAIN;
    }

    setStatus(handle, VID_CAP_STATUS_STREAM_ON);
    return startRecvPic(handle, t31Handle->channelNum);
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    T31_HANDLE_STATUS_CHECK(t31Handle, VID_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    int ret = 0;
    size_t uPacketLen = 0;
    IMPEncoderStream encodeStream = {0};

    if (IMP_Encoder_PollingStream(t31Handle->channelNum, T31_POLLING_STREAM_TIMEOUT_MS)) {
        LOG("IMP_Encoder_PollingStream(%d) timeout", t31Handle->channelNum);
        return -EAGAIN;
    }

    if (IMP_Encoder_GetStream(t31Handle->channelNum, &encodeStream, 1)) {
        LOG("IMP_Encoder_GetStream(%d) failed", t31Handle->channelNum);
        return -EAGAIN;
    }

    for (int i = 0; i < encodeStream.packCount; i++) {
        uPacketLen += encodeStream.pack[i].length;
    }

    if (frameDataBufferSize < uPacketLen) {
        LOG("FrameDataBufferSize(%d) < frameSize(%d), frame dropped", frameDataBufferSize, uPacketLen);
        *pFrameSize = 0;
        ret = -ENOMEM;
    } else {
        size_t offset = 0;

        for (int i = 0; i < encodeStream.packCount; i++) {
            getPacket(&encodeStream, &encodeStream.pack[i], pFrameDataBuffer + offset, uPacketLen - offset);
            offset += encodeStream.pack[i].length;
        }
        *pFrameSize = uPacketLen;
        *pTimestamp = IMP_System_GetTimeStamp() * HUNDREDS_OF_NANOS_IN_A_MICROSECOND;
    }

    IMP_Encoder_ReleaseStream(t31Handle->channelNum, &encodeStream);

    return ret;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{
    T31_HANDLE_NULL_CHECK(handle);
    T31_HANDLE_GET(handle);

    if (stopRecvPic(handle, t31Handle->channelNum)) {
        return -EAGAIN;
    }

    if (sample_framesource_streamoff()) {
        LOG("FrameSource StreamOff failed");
        return -EAGAIN;
    }

    return setStatus(handle, VID_CAP_STATUS_STREAM_OFF);
}

void videoCapturerDestory(VideoCapturerHandle handle)
{
    if (!handle) {
        return;
    }
    T31_HANDLE_GET(handle);

    if (t31Handle->status == VID_CAP_STATUS_STREAM_ON) {
        videoCapturerReleaseStream(handle);
    }

    setStatus(handle, VID_CAP_STATUS_NOT_READY);

    free(handle);

    ATOMIC_INT_SUB(&t31VideoSystemUser);

    if (!t31VideoSystemUser) {
        for (int i = 0; i < T31_VIDEO_STREAM_CHANNEL_NUM; i++) {
            if (chn[i].enable) {
                if (IMP_System_UnBind(&chn[i].framesource_chn, &chn[i].imp_encoder)) {
                    LOG("UnBind FrameSource channel%d and Encoder failed", i);
                }
            }
        }

        sample_encoder_exit();
        sample_framesource_exit();
        sample_system_exit();
    }
}
