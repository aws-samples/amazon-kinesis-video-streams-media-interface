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

#include "T41Common.h"
#include <imp/imp_encoder.h>
#include <imp/imp_system.h>
#include "sample-common.h"

#define T41_VIDEO_STREAM_1080P_CHANNEL_NUM   CH0_INDEX
#define T41_VIDEO_STREAM_720P_CHANNEL_NUM    CH1_INDEX
#define T41_VIDEO_STREAM_LOW_RES_CHANNEL_NUM CH2_INDEX
#define T41_VIDEO_STREAM_CHANNEL_NUM         3

#define T41_HANDLE_GET(x) T41VideoCapturer* T41Handle = (T41VideoCapturer*) ((x))

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    uint8_t channelNum;
} T41VideoCapturer;

extern struct chn_conf chn[];
static ATOMIC_INT T41VideoSystemUser = 0;

static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (newStatus != T41Handle->status) {
        T41Handle->status = newStatus;
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
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    T41_HANDLE_STATUS_CHECK(T41Handle, VID_CAP_STATUS_STREAM_ON);

    if (T41Handle->format != VID_FMT_RAW) {
        if (IMP_Encoder_StartRecvPic(chnNum)) {
            LOG("IMP_Encoder_StartRecvPic(%d) failed", chnNum);
            return -EAGAIN;
        }

        LOG("IMP_Encoder_StartRecvPic(%d)", chnNum);
    } else {
        if (IMP_FrameSource_SetFrameDepth(T41Handle->channelNum, chn[T41Handle->channelNum].fs_chn_attr.nrVBs * 2)) {
            LOG("IMP_FrameSource_SetFrameDepth(%d,%d) failed", T41Handle->channelNum, chn[T41Handle->channelNum].fs_chn_attr.nrVBs * 2);
            return -EAGAIN;
        }
    }

    return 0;
}

static int stopRecvPic(VideoCapturerHandle handle, uint8_t chnNum)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (T41Handle->format != VID_FMT_RAW) {
        if (IMP_Encoder_StopRecvPic(chnNum)) {
            LOG("IMP_Encoder_StopRecvPic(%d) failed", chnNum);
            return -EAGAIN;
        }

        LOG("IMP_Encoder_StopRecvPic(%d)", chnNum);
    } else {
        IMP_FrameSource_SetFrameDepth(T41Handle->channelNum, 0);
    }

    return 0;
}

VideoCapturerHandle videoCapturerCreate(void)
{
    T41VideoCapturer* T41Handle = NULL;

    if (!(T41Handle = (T41VideoCapturer*) malloc(sizeof(T41VideoCapturer)))) {
        LOG("OOM");
        return NULL;
    }

    memset(T41Handle, 0, sizeof(T41VideoCapturer));

    if (!T41VideoSystemUser) {
        for (int i = 0; i < T41_VIDEO_STREAM_CHANNEL_NUM; i++) {
            chn[i].enable = false;
        }
        /* Step.1 System init */
        if (sample_system_init()) {
            LOG("IMP_System_Init() failed");
            free(T41Handle);
            return NULL;
        }
    }

    // Now implementation supports H.264, RAW(NV12), 1080p, 720p, 480p, 360p and 320p
    T41Handle->capability.formats = (1 << (VID_FMT_H264 - 1)) | (1 << (VID_FMT_RAW - 1));
    T41Handle->capability.resolutions =
        (1 << (VID_RES_1080P - 1)) | (1 << (VID_RES_720P - 1)) | (1 << (VID_RES_480P - 1)) | (1 << (VID_RES_360P - 1)) | (1 << (VID_RES_320P - 1));

    setStatus((VideoCapturerHandle) T41Handle, VID_CAP_STATUS_STREAM_OFF);

    ATOMIC_INT_ADD(&T41VideoSystemUser);

    return (VideoCapturerHandle) T41Handle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle handle)
{
    if (!handle) {
        return VID_CAP_STATUS_NOT_READY;
    }

    T41_HANDLE_GET(handle);
    return T41Handle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle handle, VideoCapability* pCapability)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (!pCapability) {
        return -EAGAIN;
    }

    *pCapability = T41Handle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    T41_HANDLE_STATUS_CHECK(T41Handle, VID_CAP_STATUS_STREAM_OFF);

    switch (resolution) {
        case VID_RES_1080P:
            T41Handle->channelNum = T41_VIDEO_STREAM_1080P_CHANNEL_NUM;
            break;
        case VID_RES_720P:
            T41Handle->channelNum = T41_VIDEO_STREAM_720P_CHANNEL_NUM;
            break;

        case VID_RES_480P:
            T41Handle->channelNum = T41_VIDEO_STREAM_LOW_RES_CHANNEL_NUM;
            chn[T41Handle->channelNum].fs_chn_attr.picWidth = 640;
            chn[T41Handle->channelNum].fs_chn_attr.picHeight = 480;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.enable = 1;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.outwidth = chn[T41Handle->channelNum].fs_chn_attr.picWidth;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.outheight = chn[T41Handle->channelNum].fs_chn_attr.picHeight;
            break;

        case VID_RES_360P:
            T41Handle->channelNum = T41_VIDEO_STREAM_LOW_RES_CHANNEL_NUM;
            chn[T41Handle->channelNum].fs_chn_attr.picWidth = 480;
            chn[T41Handle->channelNum].fs_chn_attr.picHeight = 360;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.enable = 1;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.outwidth = chn[T41Handle->channelNum].fs_chn_attr.picWidth;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.outheight = chn[T41Handle->channelNum].fs_chn_attr.picHeight;
            break;
        case VID_RES_320P:
            T41Handle->channelNum = T41_VIDEO_STREAM_LOW_RES_CHANNEL_NUM;
            chn[T41Handle->channelNum].fs_chn_attr.picWidth = 416;
            chn[T41Handle->channelNum].fs_chn_attr.picHeight = 320;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.enable = 1;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.outwidth = chn[T41Handle->channelNum].fs_chn_attr.picWidth;
            chn[T41Handle->channelNum].fs_chn_attr.scaler.outheight = chn[T41Handle->channelNum].fs_chn_attr.picHeight;
            break;

        default:
            LOG("Unsupported resolution %d", resolution);
            return -EINVAL;
    }

    switch (format) {
        case VID_FMT_H264:
            chn[T41Handle->channelNum].payloadType = IMP_ENC_PROFILE_AVC_MAIN;
            break;
        case VID_FMT_RAW:
            break;

        default:
            LOG("Unsupported format %d", format);
            return -EINVAL;
    }
    chn[T41Handle->channelNum].enable = true;

    /* Step.2 FrameSource init */

    if (IMP_FrameSource_CreateChn(chn[T41Handle->channelNum].index, &chn[T41Handle->channelNum].fs_chn_attr)) {
        LOG("IMP_FrameSource_CreateChn(chn%d) error !", chn[T41Handle->channelNum].index);
        return -EAGAIN;
    }

    if (IMP_FrameSource_SetChnAttr(chn[T41Handle->channelNum].index, &chn[T41Handle->channelNum].fs_chn_attr)) {
        LOG("IMP_FrameSource_SetChnAttr(chn%d) error !", chn[T41Handle->channelNum].index);
        return -EAGAIN;
    }

    if (format != VID_FMT_RAW) {
        /* Step.3 Encoder init */

        if (IMP_Encoder_CreateGroup(chn[T41Handle->channelNum].index)) {
            LOG("IMP_Encoder_CreateGroup(%d) error !", chn[T41Handle->channelNum].index);
            return -EAGAIN;
        }

        if (sample_encoder_init()) {
            LOG("Encoder init failed");
            return -EAGAIN;
        }

        /* Step.4 Bind */
        if (IMP_System_Bind(&chn[T41Handle->channelNum].framesource_chn, &chn[T41Handle->channelNum].imp_encoder)) {
            LOG("Bind FrameSource channel%d and Encoder failed", T41Handle->channelNum);
            return -EAGAIN;
        }
    }

    T41Handle->format = format;
    T41Handle->resolution = resolution;

    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    *pFormat = T41Handle->format;
    *pResolution = T41Handle->resolution;

    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (IMP_FrameSource_EnableChn(chn[T41Handle->channelNum].index)) {
        LOG("IMP_FrameSource_EnableChn(%d) error", chn[T41Handle->channelNum].index);
        return -EAGAIN;
    }

    setStatus(handle, VID_CAP_STATUS_STREAM_ON);
    return startRecvPic(handle, T41Handle->channelNum);
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    T41_HANDLE_STATUS_CHECK(T41Handle, VID_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
        return -EINVAL;
    }

    int ret = 0;

    if (T41Handle->format == VID_FMT_RAW) {
        IMPFrameInfo* rawFrame = NULL;
        if (IMP_FrameSource_GetFrame(T41Handle->channelNum, &rawFrame)) {
            LOG("IMP_FrameSource_GetFrame(%d) failed", T41Handle->channelNum);
            return -EAGAIN;
        }
        if (frameDataBufferSize < rawFrame->size) {
            LOG("FrameDataBufferSize(%d) < frameSize(%d), frame dropped", frameDataBufferSize, rawFrame->size);
            ret = -ENOMEM;
        } else {
            memcpy(pFrameDataBuffer, (void*) rawFrame->virAddr, rawFrame->size);
            *pFrameSize = rawFrame->size;
            *pTimestamp = IMP_System_GetTimeStamp();
        }

        IMP_FrameSource_ReleaseFrame(T41Handle->channelNum, rawFrame);
    } else {
        size_t uPacketLen = 0;
        IMPEncoderStream encodeStream = {0};

        if (IMP_Encoder_PollingStream(T41Handle->channelNum, T41_POLLING_STREAM_TIMEOUT_MS)) {
            LOG("IMP_Encoder_PollingStream(%d) timeout", T41Handle->channelNum);
            return -EAGAIN;
        }

        if (IMP_Encoder_GetStream(T41Handle->channelNum, &encodeStream, 1)) {
            LOG("IMP_Encoder_GetStream(%d) failed", T41Handle->channelNum);
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
            *pTimestamp = IMP_System_GetTimeStamp();
        }

        IMP_Encoder_ReleaseStream(T41Handle->channelNum, &encodeStream);
    }

    return ret;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{
    T41_HANDLE_NULL_CHECK(handle);
    T41_HANDLE_GET(handle);

    if (stopRecvPic(handle, T41Handle->channelNum)) {
        return -EAGAIN;
    }

    if (IMP_FrameSource_DisableChn(chn[T41Handle->channelNum].index)) {
        LOG("FrameSource StreamOff failed");
        return -EAGAIN;
    }

    return setStatus(handle, VID_CAP_STATUS_STREAM_OFF);
}

void videoCapturerDestroy(VideoCapturerHandle handle)
{
    if (!handle) {
        return;
    }
    T41_HANDLE_GET(handle);

    if (T41Handle->status == VID_CAP_STATUS_STREAM_ON) {
        videoCapturerReleaseStream(handle);
    }

    setStatus(handle, VID_CAP_STATUS_NOT_READY);

    if (chn[T41Handle->channelNum].enable) {
        if (T41Handle->format != VID_FMT_RAW) {
            if (IMP_System_UnBind(&chn[T41Handle->channelNum].framesource_chn, &chn[T41Handle->channelNum].imp_encoder)) {
                LOG("UnBind FrameSource channel%d and Encoder failed");
            }
            sample_encoder_exit();
        }

        /*Destroy channel */
        if (IMP_FrameSource_DestroyChn(chn[T41Handle->channelNum].index)) {
            LOG("IMP_FrameSource_DestroyChn(%d) error", chn[T41Handle->channelNum].index);
        }
        chn[T41Handle->channelNum].enable = false;
    }

    ATOMIC_INT_SUB(&T41VideoSystemUser);

    if (!T41VideoSystemUser) {
        sample_system_exit();
    }

    free(handle);
}
