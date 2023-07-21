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
#include <pthread.h>
#include <sys/prctl.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"
#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"
#include "SV82xCommon.h"
#include "sample_comm.h"

#define SV82X_VIDEO_STREAM_1080P_CHN   (0)
#define SV82X_VIDEO_STREAM_720P_CHN    (1)
#define SV82X_VIDEO_STREAM_LOW_RES_CHN (2)
#define SV82X_VIDEO_STREAM_CHN_NUM     (3)

#define SV82X_VIDEO_STREAM_FPS     (30)
#define SV82X_VIDEO_STREAM_BITRATE (2 * 1024 * 1024)
#define SV82X_VIDEO_STREAM_GOP     (30)
#define SV82X_VIDEO_RC_MODE        (SAMPLE_RC_VBR)

#define SV82X_IPPU_DEV (0)
#define SV82X_ISP_DEV  (0)

#define SV82X_HANDLE_NULL_CHECK_RET_VOID(x)                                                                                                          \
    do {                                                                                                                                             \
        if (x == EI_NULL) {                                                                                                                          \
            LOG("Null Pointer!\n");                                                                                                                  \
            return;                                                                                                                                  \
        }                                                                                                                                            \
    } while (0)

#define SV82X_HANDLE_GET(x) Sv82xVideoCapturer_t* Sv82xHandle = (Sv82xVideoCapturer_t*) ((x))

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    uint8_t channel;
    VBUF_POOL Pool;
    uint8_t VideoCreatFlag;
    uint8_t enable;
} Sv82xVideoCapturer_t;

typedef struct {
    SAMPLE_VISS_CONFIG_S stVissConfig;
    int VideoCreatHandleCnt;
    int IppuEnableCnt;
} Sv82xVideoInfo_t;
static Sv82xVideoInfo_t VideoAttrs = {0};
static int SystemInitFlag = 0;

static int SystemInit(void)
{
    EI_S32 s32Ret = EI_FAILURE;

    SNS_TYPE_E enSnsType;
    VISS_PIC_TYPE_E stPicType = {0};
    IPPU_DEV_ATTR_S stDevAttr = {0};
    SAMPLE_VISS_CONFIG_S stVissConfig;

    EI_MI_MBASE_Init();
    EI_MI_VBUF_Init();
    EI_MI_MLINK_Init();

    enSnsType = GC4663_MIPI_2560_1440_30FPS_RAW10;
    memset(&VideoAttrs.stVissConfig, 0, sizeof(SAMPLE_VISS_CONFIG_S));
    stVissConfig = VideoAttrs.stVissConfig;
    stVissConfig.astVissInfo[0].stDevInfo.VissDev = (enSnsType < 200000) ? 0 : 1;
    stVissConfig.astVissInfo[0].stDevInfo.aBindPhyChn[0] = 0;
    stVissConfig.astVissInfo[0].stDevInfo.enOutPath = VISS_OUT_PATH_PIXEL;
    stVissConfig.astVissInfo[0].stChnInfo.aVissChn[0] = 0;
    stVissConfig.astVissInfo[0].stChnInfo.enWorkMode = VISS_WORK_MODE_1Chn;
    stVissConfig.astVissInfo[0].stIspInfo.IspDev = SV82X_ISP_DEV;
    stVissConfig.astVissInfo[0].stIspInfo.enRunningMode = ISP_MODE_RUNNING_ONLINE;
    stVissConfig.astVissInfo[0].stSnsInfo.enSnsType = enSnsType;
    stVissConfig.s32WorkingVissNum = 1;

    s32Ret = SAMPLE_COMM_ISP_Start(&stVissConfig.astVissInfo[0]);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_ISP_Start failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    s32Ret = SAMPLE_COMM_VISS_StartViss(&stVissConfig);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_VISS_StartViss failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    s32Ret = SAMPLE_COMM_VISS_GetPicTypeBySensor(enSnsType, &stPicType);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_VISS_GetPicTypeBySensor failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    stDevAttr.s32IppuDev = SV82X_IPPU_DEV;
    stDevAttr.u32DataSrc = (enSnsType < 200000) ? 0 : 1;
    stDevAttr.u32InputWidth = stPicType.stSize.u32Width;
    stDevAttr.u32InputHeight = stPicType.stSize.u32Height;
    stDevAttr.enRunningMode = IPPU_MODE_RUNNING_ONLINE;
    s32Ret = EI_MI_IPPU_Create(SV82X_IPPU_DEV, &stDevAttr);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_IPPU_Create failed with %#x\n", s32Ret);
        return -EAGAIN;
    }
    s32Ret = EI_MI_IPPU_Start(SV82X_IPPU_DEV);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_IPPU_Start failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    return 0;
}

static int SystemDeinit(void)
{
    EI_S32 s32Ret = EI_FAILURE;
    SAMPLE_VISS_CONFIG_S stVissConfig;

    s32Ret = EI_MI_IPPU_Stop(SV82X_IPPU_DEV);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_IPPU_Stop failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    s32Ret = EI_MI_IPPU_Destroy(SV82X_IPPU_DEV);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_IPPU_Destroy failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    stVissConfig = VideoAttrs.stVissConfig;
    s32Ret = SAMPLE_COMM_VISS_StopViss(&stVissConfig);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_VISS_StopViss failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    s32Ret = SAMPLE_COMM_ISP_Stop(SV82X_IPPU_DEV);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_ISP_Stop failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    EI_MI_MLINK_Exit();
    EI_MI_VBUF_Exit();
    EI_MI_MBASE_Exit();

    return 0;
}

static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    if (newStatus != Sv82xHandle->status) {
        Sv82xHandle->status = newStatus;
        LOG("VideoCapturer new status[%d]", newStatus);
    }

    return 0;
}

static int _Sv82xVideoBufPoolCreat(VideoCapturerHandle handle, VIDEO_FRAME_INFO_S* pstVideoFrameInfo)
{
    EI_S32 s32Ret = EI_FAILURE;
    VBUF_POOL_CONFIG_S stPoolCfg = {0};
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    stPoolCfg.u32BufSize = EI_MI_VBUF_GetPicBufferSize(pstVideoFrameInfo);
    stPoolCfg.u32BufCnt = 4;
    stPoolCfg.enRemapMode = VBUF_REMAP_MODE_NOCACHE;
    stPoolCfg.enVbufUid = VBUF_UID_COMMON;
    /* create buffer pool */
    Sv82xHandle->Pool = EI_MI_VBUF_CreatePool(&stPoolCfg);
    /* set buffer pool info */
    s32Ret = EI_MI_VBUF_SetFrameInfo(Sv82xHandle->Pool, pstVideoFrameInfo);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_VBUF_SetFrameInfo failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    return 0;
}

static int _Sv82xVideoBufPoolDestroy(VideoCapturerHandle handle)
{
    EI_S32 s32Ret = EI_FAILURE;
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    s32Ret = EI_MI_VBUF_DestroyPool(Sv82xHandle->Pool);
    if (s32Ret) {
        LOG("EI_MI_VBUF_DestroyPool error s32Ret：%d\n", s32Ret);
        return -EAGAIN;
    }
    return 0;
}

static int _Sv82xIppuStart(VideoCapturerHandle handle, const IPPU_CHN_ATTR_S* pstChnAttr)
{
    EI_S32 s32Ret = EI_FAILURE;
    IPPU_DEV dev = 0;
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    s32Ret = EI_MI_IPPU_SetChnAttr(dev, Sv82xHandle->channel, pstChnAttr);
    if (s32Ret != EI_SUCCESS) {
        LOG("%s:%d EI_MI_IPPU_SetChnAttr failed\n", __func__, __LINE__);
        EI_MI_IPPU_Destroy(dev);
        return -EAGAIN;
    }
    s32Ret = EI_MI_IPPU_EnableChn(dev, Sv82xHandle->channel);
    if (s32Ret != EI_SUCCESS) {
        LOG("%s:%d EI_MI_IPPU_EnableChn failed\n", __func__, __LINE__);
        EI_MI_IPPU_Destroy(dev);
        return -EAGAIN;
    }
    Sv82xHandle->enable = 1;
    VideoAttrs.IppuEnableCnt++;

    return 0;
}

static int _Sv82xIppuStop(VideoCapturerHandle handle)
{
    EI_S32 s32Ret = EI_SUCCESS;
    IPPU_DEV dev = 0;
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    LOG("%s chn is :%d\n", __func__, Sv82xHandle->channel);
    if (!Sv82xHandle->enable) {
        LOG("ippu channel:%d is disable\n", Sv82xHandle->channel);
        return -EINVAL;
    }

    if (Sv82xHandle->channel >= IPPU_PHY_CHN_MAX_NUM || Sv82xHandle->channel < 0) {
        LOG("channel:%d is illegal\n", Sv82xHandle->channel);
        return -EINVAL;
    }

    s32Ret = EI_MI_IPPU_DisableChn(dev, Sv82xHandle->channel);
    if (s32Ret != EI_SUCCESS) {
        LOG("IPPU_DisableChn failed with %d!\n", s32Ret);
        return -EAGAIN;
    }
    Sv82xHandle->enable = 0;
    VideoAttrs.IppuEnableCnt--;

    return 0;
}

static int _Sv82xVideoCreat(VideoCapturerHandle handle, const VideoFormat format, SAMPLE_VENC_CONFIG_S* pstSamplVenc)
{
    EI_S32 s32Ret = 0, RcMode = 0, GopMode = 0, PayloadType = 0;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);
    RcMode = SV82X_VIDEO_RC_MODE;
    GopMode = VENC_GOPMODE_NORMAL_P;
    switch (format) {
        case VID_FMT_H264:
            PayloadType = PT_H264;
            break;
        case VID_FMT_H265:
            PayloadType = PT_H265;
            break;
        default:
            LOG("Unsupported format %d", format);
            return -EINVAL;
    }

    if (PayloadType == PT_H264 || PayloadType == PT_H265) {
        s32Ret = SAMPLE_COMM_VENC_Creat(Sv82xHandle->channel, PayloadType, RcMode, pstSamplVenc, COMPRESS_MODE_NONE, GopMode);
        if (EI_SUCCESS != s32Ret) {
            LOG("SAMPLE_COMM_VENC_Creat faild with%#x! \n", s32Ret);
            return -EAGAIN;
        }

        s32Ret = SAMPLE_COMM_IPPU_Link_VPU(0, Sv82xHandle->channel, Sv82xHandle->channel);
        if (s32Ret != EI_SUCCESS) {
            LOG("SAMPLE_COMM_IPPU_Link_VPU failed with %#x\n", s32Ret);
            EI_MI_VENC_DestroyChn(Sv82xHandle->channel);
            return -EAGAIN;
        }
    }
    Sv82xHandle->VideoCreatFlag = 1;
    return 0;
}

static int _Sv82xVideoDestroy(VideoCapturerHandle handle)
{
    EI_S32 s32Ret = 0;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    s32Ret = SAMPLE_COMM_IPPU_UnLink_VPU(0, Sv82xHandle->channel, Sv82xHandle->channel);
    if (s32Ret != EI_SUCCESS) {
        LOG("SAMPLE_COMM_IPPU_UnLink_VPU failed with %#x\n", s32Ret);
        return -EAGAIN;
    }
    s32Ret = EI_MI_VENC_DestroyChn(Sv82xHandle->channel);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_VENC_DestroyChn failed with %#x\n", s32Ret);
        return -EAGAIN;
    }
    Sv82xHandle->VideoCreatFlag = 0;

    return 0;
}

static int _Sv82xVideoStart(VideoCapturerHandle handle)
{
    EI_S32 s32Ret = 0;
    VENC_RECV_PIC_PARAM_S stRecvParam = {0};

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    stRecvParam.s32RecvPicNum = -1;
    s32Ret = EI_MI_VENC_StartRecvFrame(Sv82xHandle->channel, &stRecvParam);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_VENC_StartRecvFrame failed with %#x\n", s32Ret);
        return -EAGAIN;
    }
    setStatus(handle, VID_CAP_STATUS_STREAM_ON);

    return 0;
}

static int _Sv82xVideoStop(VideoCapturerHandle handle)
{
    EI_S32 s32Ret = 0;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    s32Ret = EI_MI_VENC_StopRecvFrame(Sv82xHandle->channel);
    if (s32Ret != EI_SUCCESS) {
        LOG("EI_MI_VENC_StopRecvFrame failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    setStatus(handle, VID_CAP_STATUS_STREAM_OFF);

    return 0;
}

VideoCapturerHandle videoCapturerCreate(void)
{
    EI_S32 s32Ret = EI_SUCCESS;
    Sv82xVideoCapturer_t* Handle = NULL;
    if (VideoAttrs.VideoCreatHandleCnt >= SV82X_VIDEO_STREAM_CHN_NUM)
        return NULL;
    if (!(Handle = (Sv82xVideoCapturer_t*) malloc(sizeof(Sv82xVideoCapturer_t)))) {
        LOG("%s:%d oom\n", __func__, __LINE__);
        return NULL;
    }
    memset(Handle, 0, sizeof(Sv82xVideoCapturer_t));

    if (!SystemInitFlag) {
        s32Ret = SystemInit();
        if (s32Ret) {
            LOG("%s:%d SystemInit\n", __func__, __LINE__);
            free(Handle);
            return NULL;
        }
    }
    SystemInitFlag = 1;
    Handle->capability.formats = (1 << (VID_FMT_H264 - 1)) | (1 << (VID_FMT_H265 - 1)) | (1 << (VID_FMT_RAW - 1));
    Handle->capability.resolutions =
        (1 << (VID_RES_1080P - 1)) | (1 << (VID_RES_720P - 1)) | (1 << (VID_RES_480P - 1)) | (1 << (VID_RES_360P - 1)) | (1 << (VID_RES_320P - 1));

    setStatus((VideoCapturerHandle) Handle, VID_CAP_STATUS_STREAM_OFF);

    VideoAttrs.VideoCreatHandleCnt++;

    return (VideoCapturerHandle) Handle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle handle)
{
    if (!handle) {
        return VID_CAP_STATUS_NOT_READY;
    }

    SV82X_HANDLE_GET(handle);
    return Sv82xHandle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle handle, VideoCapability* pCapability)
{
    EI_S32 s32Ret = EI_SUCCESS;
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    if (!pCapability)
        return -EINVAL;

    *pCapability = Sv82xHandle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle const handle, const VideoFormat format, const VideoResolution resolution)
{
    EI_S32 s32Ret = EI_FAILURE;
    EI_S32 width = 0, height = 0;
    VIDEO_FRAME_INFO_S stVideoFrameInfo = {0};
    IPPU_CHN_ATTR_S ippu_chn_attr = {0};
    SAMPLE_VENC_CONFIG_S stSamplVenc = {0};

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);
    SV82X_HANDLE_STATUS_CHECK(Sv82xHandle, VID_CAP_STATUS_STREAM_OFF);

    switch (resolution) {
        case VID_RES_1080P:
            Sv82xHandle->channel = SV82X_VIDEO_STREAM_1080P_CHN;
            width = 1920;
            height = 1080;
            break;
        case VID_RES_720P:
            Sv82xHandle->channel = SV82X_VIDEO_STREAM_720P_CHN;
            width = 1280;
            height = 720;
            break;
        case VID_RES_480P:
            Sv82xHandle->channel = SV82X_VIDEO_STREAM_LOW_RES_CHN;
            width = 640;
            height = 480;
            break;
        case VID_RES_360P:
            Sv82xHandle->channel = SV82X_VIDEO_STREAM_LOW_RES_CHN;
            width = 480;
            height = 360;
            break;
        case VID_RES_320P:
            Sv82xHandle->channel = SV82X_VIDEO_STREAM_LOW_RES_CHN;
            width = 416;
            height = 320;
            break;
        default:
            LOG("Unsupported resolution %d", resolution);
            return -EINVAL;
    }
    // creat buf pool
    stVideoFrameInfo.enFrameType = MDP_FRAME_TYPE_COMMON;
    stVideoFrameInfo.stCommFrameInfo.u32Align = 32;
    stVideoFrameInfo.stCommFrameInfo.u32Width = width;
    stVideoFrameInfo.stCommFrameInfo.u32Height = height;
    stVideoFrameInfo.stCommFrameInfo.enPixelFormat = PIX_FMT_YUV_SEMIPLANAR_420;
    s32Ret = _Sv82xVideoBufPoolCreat(handle, &stVideoFrameInfo);
    if (s32Ret != EI_SUCCESS) {
        LOG("%s:%d _sv82x_video_init failed\n", __func__, __LINE__);
        return -EAGAIN;
    }

    // set ippu chn attrs and enable ippu chn
    ippu_chn_attr.s32Chn = Sv82xHandle->channel;
    ippu_chn_attr.u32Width = width;
    ippu_chn_attr.u32Height = height;
    ippu_chn_attr.enPixelFormat = PIX_FMT_YUV_SEMIPLANAR_420;
    ippu_chn_attr.u32Align = 32;
    ippu_chn_attr.stFrameRate.s32DstFrameRate = SV82X_VIDEO_STREAM_FPS;
    ippu_chn_attr.stFrameRate.s32SrcFrameRate = SV82X_VIDEO_STREAM_FPS;
    if (format == VID_FMT_RAW)
        ippu_chn_attr.u32Depth = 1;
    else
        ippu_chn_attr.u32Depth = 0;
    ippu_chn_attr.enCompressMode = COMPRESS_MODE_NONE;

    s32Ret = _Sv82xIppuStart(handle, &ippu_chn_attr);
    if (s32Ret != EI_SUCCESS) {
        LOG("%s:%d _Sv82xIppuStart failed\n", __func__, __LINE__);
        return -EAGAIN;
    }
    if (format != VID_FMT_RAW) {
        // vpu chn creat
        stSamplVenc.enInputFormat = PIX_FMT_YUV_SEMIPLANAR_420;
        stSamplVenc.u32width = width;
        stSamplVenc.u32height = height;
        stSamplVenc.u32bitrate = SV82X_VIDEO_STREAM_BITRATE;
        stSamplVenc.u32srcframerate = SV82X_VIDEO_STREAM_FPS;
        stSamplVenc.u32dstframerate = SV82X_VIDEO_STREAM_FPS;
        s32Ret = _Sv82xVideoCreat(handle, format, &stSamplVenc);
        if (s32Ret != EI_SUCCESS) {
            LOG("%s:%d _Sv82xVideoCreat failed\n", __func__, __LINE__);
            return -EAGAIN;
        }
    }

    Sv82xHandle->format = format;
    Sv82xHandle->resolution = resolution;

    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);

    *pFormat = Sv82xHandle->format;
    *pResolution = Sv82xHandle->resolution;
    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{
    EI_S32 s32Ret = EI_FAILURE;

    s32Ret = _Sv82xVideoStart(handle);
    if (s32Ret != EI_SUCCESS) {
        LOG("_Sv82xVideoStart failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    return 0;
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{
    EI_S32 s32Ret = -1;
    VENC_CHN_STATUS_S stStatus = {0};
    VENC_STREAM_S stStream = {0};
    VENC_STREAM_S VencStream = {0};
    VIDEO_FRAME_INFO_S stVFrameInfo = {0};
    VC_CHN channel;
    EI_U32 data_len = 0;

    SV82X_HANDLE_NULL_CHECK(handle);
    SV82X_HANDLE_GET(handle);
    channel = Sv82xHandle->channel;
    if (Sv82xHandle->format == VID_FMT_RAW) {
        s32Ret = EI_MI_IPPU_GetChnFrame(0, channel, &stVFrameInfo, 2000);
        if (s32Ret != EI_SUCCESS) {
            LOG("%s:%d EI_MI_IPPU_GetChnFrame chn-%d error : %d\n", __func__, __LINE__, channel, s32Ret);
            return -EAGAIN;
        }

        EI_MI_VBUF_FrameMmap(&stVFrameInfo, VBUF_REMAP_MODE_CACHED);
        if (((char*) stVFrameInfo.stVFrame.ulPlaneVirAddr[0] != NULL) && (stVFrameInfo.stVFrame.u32PlaneSize[0] != 0) &&
            ((char*) stVFrameInfo.stVFrame.ulPlaneVirAddr[1] != NULL) && (stVFrameInfo.stVFrame.u32PlaneSize[1] != 0)) {
            *pFrameSize = stVFrameInfo.stCommFrameInfo.u32Width * stVFrameInfo.stCommFrameInfo.u32Height * 1.5;
        }
        memset(pFrameDataBuffer, 0, frameDataBufferSize);
        if (*pFrameSize <= frameDataBufferSize) {
            if (((char*) stVFrameInfo.stVFrame.ulPlaneVirAddr[0] != NULL) && (stVFrameInfo.stVFrame.u32PlaneSize[0] != 0)) {
                memcpy(pFrameDataBuffer, (char*) stVFrameInfo.stVFrame.ulPlaneVirAddr[0],
                       stVFrameInfo.stCommFrameInfo.u32Width * stVFrameInfo.stCommFrameInfo.u32Height);
                if (((char*) stVFrameInfo.stVFrame.ulPlaneVirAddr[1] != NULL) && (stVFrameInfo.stVFrame.u32PlaneSize[1] != 0)) {
                    memcpy(pFrameDataBuffer + stVFrameInfo.stCommFrameInfo.u32Width * stVFrameInfo.stCommFrameInfo.u32Height,
                           (char*) stVFrameInfo.stVFrame.ulPlaneVirAddr[1],
                           stVFrameInfo.stCommFrameInfo.u32Width * stVFrameInfo.stCommFrameInfo.u32Height * 0.5);
                }
            }
        } else
            LOG("ch:%d data len is to big datalen:%d buflen:%d\n", channel, *pFrameSize, frameDataBufferSize);
        *pTimestamp = stVFrameInfo.stVFrame.u64PTS;
        EI_MI_IPPU_ReleaseChnFrame(0, channel, &stVFrameInfo);
    } else if (Sv82xHandle->format == VID_FMT_H264 || Sv82xHandle->format == VID_FMT_H265) {
        s32Ret = EI_MI_VENC_QueryStatus(channel, &stStatus);
        if (s32Ret != EI_SUCCESS) {
            LOG("%s:%d query status chn-%d error : %d\n", __func__, __LINE__, channel, s32Ret);
            return -EAGAIN;
        }

        s32Ret = EI_MI_VENC_GetStream(channel, &stStream, 1000);
        if (s32Ret == EI_ERR_VENC_NOBUF) {
            LOG("No buffer\n");
            return -EAGAIN;
        } else if (s32Ret != EI_SUCCESS) {
            LOG("get stream chn-%d error : %d\n", channel, s32Ret);
            return -EAGAIN;
        }

        memset(&VencStream, 0, sizeof(VENC_STREAM_S));
        memcpy(&VencStream, &stStream, sizeof(VENC_STREAM_S));

        if ((VencStream.pstPack.pu8Addr[0] != EI_NULL) && (VencStream.pstPack.u32Len[0] != 0)) {
            data_len = VencStream.pstPack.u32Len[0];
            if ((VencStream.pstPack.pu8Addr[1] != EI_NULL) && (VencStream.pstPack.u32Len[1] != 0))
                data_len += VencStream.pstPack.u32Len[1];
        }

        if (pFrameDataBuffer == NULL) {
            LOG("pFrameDataBuffer is null\n");
            EI_MI_VENC_ReleaseStream(channel, &stStream);
            return -EAGAIN;
        }

        *pFrameSize = data_len;
        memset(pFrameDataBuffer, 0, frameDataBufferSize);
        if (frameDataBufferSize < *pFrameSize) {
            LOG("ch:%d data len is to big datalen:%d buflen:%d\n", channel, *pFrameSize, frameDataBufferSize);
            EI_MI_VENC_ReleaseStream(channel, &stStream);
            return -EAGAIN;
        }

        if ((VencStream.pstPack.pu8Addr[0] != EI_NULL) && (VencStream.pstPack.u32Len[0] != 0)) {
            memcpy(pFrameDataBuffer, VencStream.pstPack.pu8Addr[0], VencStream.pstPack.u32Len[0]);
            if ((VencStream.pstPack.pu8Addr[1] != EI_NULL) && (VencStream.pstPack.u32Len[1] != 0))
                memcpy(pFrameDataBuffer + VencStream.pstPack.u32Len[0], VencStream.pstPack.pu8Addr[1], VencStream.pstPack.u32Len[1]);
        }
        *pTimestamp = stStream.pstPack.u64PTS;
        s32Ret = EI_MI_VENC_ReleaseStream(channel, &stStream);
        if (s32Ret != EI_SUCCESS) {
            LOG("release stream chn-%d error s32Ret: %d\n", channel, s32Ret);
            return -EAGAIN;
        }
    }

    return 0;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{
    EI_S32 s32Ret = EI_FAILURE;

    s32Ret = _Sv82xVideoStop(handle);
    if (s32Ret != EI_SUCCESS) {
        LOG("_Sv82xVideoStop failed with %#x\n", s32Ret);
        return -EAGAIN;
    }

    return 0;
}

void videoCapturerDestroy(VideoCapturerHandle handle)
{
    EI_S32 s32Ret = EI_FAILURE;

    SV82X_HANDLE_NULL_CHECK_RET_VOID(handle);
    SV82X_HANDLE_GET(handle);

    if (Sv82xHandle->status == VID_CAP_STATUS_STREAM_ON) {
        s32Ret = videoCapturerReleaseStream(handle);
        if (s32Ret != EI_SUCCESS) {
            LOG("videoCapturerReleaseStream failed with %#x\n", s32Ret);
            return;
        }
    }

    if (Sv82xHandle->VideoCreatFlag) {
        s32Ret = _Sv82xVideoDestroy(handle);
        if (s32Ret != EI_SUCCESS) {
            LOG("_Sv82xVideoDestroy failed with %#x\n", s32Ret);
            return;
        }
    }
    if (Sv82xHandle->enable) {
        s32Ret = _Sv82xIppuStop(handle);
        if (s32Ret != EI_SUCCESS) {
            LOG("_Sv82xIppuStop failed with %#x\n", s32Ret);
            return;
        }
    }

    s32Ret = _Sv82xVideoBufPoolDestroy(handle);
    if (s32Ret != EI_SUCCESS) {
        LOG("_Sv82xVideoBufPoolDestroy failed with %#x\n", s32Ret);
        return;
    }
    VideoAttrs.VideoCreatHandleCnt--;
    if (VideoAttrs.IppuEnableCnt == 0) {
        s32Ret = SystemDeinit();
        if (s32Ret != EI_SUCCESS) {
            LOG("SystemDeinit failed with %#x\n", s32Ret);
            return;
        }
    }

    SystemInitFlag = 0;

    return;
}
