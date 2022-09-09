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

#define FRAME_FILE_POSTFIX_H264     ".h264"
#define FRAME_FILE_START_INDEX_H264 (1)
#define FRAME_FILE_END_INDEX_H264   (240)
//#define FRAME_FILE_PATH_FORMAT_H264 FRAME_FILE_PATH_PREFIX "/h264/frame-%03d" FRAME_FILE_POSTFIX_H264
#define FRAME_FILE_PATH_FORMAT_H264 "/mnt/web/amazon-kinesis-video-streams-media-interface-main/resources/frames/h264/frame-%03d" FRAME_FILE_POSTFIX_H264
#define FRAME_FILE_DURATION_US_H264 (1000 * 1000 / 25UL)

#define FILE_HANDLE_GET(x) FILEVideoCapturer* fileHandle = (FILEVideoCapturer*) ((x))
#define  debug_here printf("111111 %s:%d \n",__FUNCTION__,__LINE__);

typedef struct {
    VideoCapturerStatus status;
    VideoCapability capability;
    VideoFormat format;
    VideoResolution resolution;
    int nVideoHandle;
    char* framePathFormat;
    size_t frameIndex;
    size_t frameIndexStart;
    size_t frameIndexEnd;
    FILE* frameFile;
} FILEVideoCapturer;



static int setStatus(VideoCapturerHandle handle, const VideoCapturerStatus newStatus)
{

    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    if (newStatus != fileHandle->status) {
        fileHandle->status = newStatus;
        LOG("VideoCapturer new status[%d]", newStatus);
    }

    return 0;
}

int ipc_main();
static int g_init = 0;
VideoCapturerHandle videoCapturerCreate(void)
{
    if(g_init == 0)
    {
        ipc_main();
        printf("@@@@@@@@@ %s  @@@@@@@@ %d init ok \n",__FUNCTION__,__LINE__);
        g_init = 1;	
    }
    else
    {
         printf("@@@@@@@@@ %s  @@@@@@@@@ %d repeate ! \n",__FUNCTION__,__LINE__);
    }		
    
    FILEVideoCapturer* fileHandle = NULL;
    if (!(fileHandle = (FILEVideoCapturer*) malloc(sizeof(FILEVideoCapturer)))) {
        LOG("OOM");
        debug_here
        return NULL;
    }
    
    memset(fileHandle, 0, sizeof(FILEVideoCapturer));
    fileHandle->nVideoHandle = -1;
    int iret = AA_LS_RegStreamClient(0,AA_LS_STREAM_VIDEO,&fileHandle->nVideoHandle,NULL,NULL,NULL,NULL);
    printf("1111111 %s 11111111 %d  iret:    %d\n",__FUNCTION__,__LINE__,iret);
    // Now we have sample frames for H.264, 1080p
    fileHandle->capability.formats = (1 << (VID_FMT_H264 - 1));
    fileHandle->capability.resolutions = (1 << (VID_RES_1080P - 1));

    setStatus((VideoCapturerHandle) fileHandle, VID_CAP_STATUS_STREAM_OFF);

    return (VideoCapturerHandle) fileHandle;
}

VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle const handle)
{

    if (!handle) {
    debug_here
        return VID_CAP_STATUS_NOT_READY;
    }

    FILE_HANDLE_GET(handle);
    debug_here
    return fileHandle->status;
}

int videoCapturerGetCapability(const VideoCapturerHandle const handle, VideoCapability* pCapability)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    if (!pCapability) {
    
        return -EAGAIN;
    }

    *pCapability = fileHandle->capability;

    return 0;
}

int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution)
{

    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    FILE_HANDLE_STATUS_CHECK(fileHandle, VID_CAP_STATUS_STREAM_OFF);

    switch (format) {
        case VID_FMT_H264:
            fileHandle->framePathFormat = FRAME_FILE_PATH_FORMAT_H264;
            fileHandle->frameIndexStart = FRAME_FILE_START_INDEX_H264;
            fileHandle->frameIndexEnd = FRAME_FILE_END_INDEX_H264;
            debug_here
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
            debug_here
            return -EINVAL;
    }

    fileHandle->format = format;
    fileHandle->resolution = resolution;

    return 0;
}

int videoCapturerGetFormat(const VideoCapturerHandle const handle, VideoFormat* pFormat, VideoResolution* pResolution)
{
    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    *pFormat = fileHandle->format;
    *pResolution = fileHandle->resolution;

    return 0;
}

int videoCapturerAcquireStream(VideoCapturerHandle handle)
{

    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    fileHandle->frameIndex = fileHandle->frameIndexStart;

    return setStatus(handle, VID_CAP_STATUS_STREAM_ON);
}

int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize)
{

    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    FILE_HANDLE_STATUS_CHECK(fileHandle, VID_CAP_STATUS_STREAM_ON);

    if (!pFrameDataBuffer || !pTimestamp || !pFrameSize) {
    
        return -EINVAL;
    }
    
    int ret = 0;
    aa_frame_info frame;
    static int ic = 0;
    while(1)
    {
   
        int iRet = AA_LS_GetFrame(0,    AA_LS_STREAM_VIDEO, fileHandle->nVideoHandle, &frame);
        //printf("1111111 %s 11111111 %d  iret:    %d\n",__FUNCTION__,__LINE__,iRet);
        if(iRet != 0)
        {
            ak_sleep_ms(5);
        }
        else
        {

                if(ic++%25 == 0)
                	printf("Getframe  : recv a frame((%d, %d, %d) \n",frame.uiDataLen, frame.iWidth, frame.iHeight);
                
                if(frameDataBufferSize > frame.uiDataLen)
                {
                    memcpy(pFrameDataBuffer,frame.pData,frame.uiDataLen);
                    *pFrameSize = frame.uiDataLen;
                    *pTimestamp = frame.u64Time;
                }
                else
                    printf("framesize error!\n");
		    break;
                
        }
        
    }

    //usleep(FRAME_FILE_DURATION_US_H264);

    return ret;
}

int videoCapturerReleaseStream(VideoCapturerHandle handle)
{

    FILE_HANDLE_NULL_CHECK(handle);
    FILE_HANDLE_GET(handle);

    FILE_HANDLE_STATUS_CHECK(fileHandle, VID_CAP_STATUS_STREAM_ON);

    if (fileHandle->frameFile) {

        CLOSE_FILE(fileHandle->frameFile);
    }
    AA_LS_UnRegStreamClient(0, VID_CAP_STATUS_STREAM_OFF, fileHandle->nVideoHandle);
    printf("1111111 %s 11111111 %d  \n",__FUNCTION__,__LINE__);
    return setStatus(handle, VID_CAP_STATUS_STREAM_OFF);
}

void videoCapturerDestory(VideoCapturerHandle handle)
{
    if (!handle) {
    
        return;
    }

    FILE_HANDLE_GET(handle);

    if (fileHandle->status == VID_CAP_STATUS_STREAM_ON) {
    
        videoCapturerReleaseStream(handle);
    }

    setStatus(handle, VID_CAP_STATUS_NOT_READY);

    free(handle);
}
