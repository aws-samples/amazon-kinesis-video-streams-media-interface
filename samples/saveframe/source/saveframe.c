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
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>

#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#define VIDEO_BUFFER_SIZE (2 * 1024 * 1024UL)

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: saveframe-static FILE\n");
    }

    int fd = -1;

    fd = open(argv[1], O_RDWR | O_CREAT, 0x644);

    if (fd < 0) {
        printf("Failed to open file %s\n", argv[1]);
        return -EAGAIN;
    }

    VideoCapturerHandle videoCapturerHandle = NULL;
    videoCapturerHandle = videoCapturerCreate();
    if (!videoCapturerHandle) {
        printf("Failed to create video capturer\n");
        close(fd);
        return -EAGAIN;
    }

    if (videoCapturerSetFormat(videoCapturerHandle, VID_FMT_RAW, VID_RES_720P)) {
        printf("Failed to set video capturer format\n");
        close(fd);
        videoCapturerDestory(videoCapturerHandle);
        return -EAGAIN;
    }

    void* videoBuffer = NULL;
    videoBuffer = malloc(VIDEO_BUFFER_SIZE);
    if (!videoBuffer) {
        printf("Failed to allocate video buffer\n");
        close(fd);
        videoCapturerDestory(videoCapturerHandle);
        return -ENOMEM;
    }

    if (videoCapturerAcquireStream(videoCapturerHandle)) {
        printf("Failed to acquire video stream\n");
        close(fd);
        videoCapturerDestory(videoCapturerHandle);
        if (videoBuffer) {
            free(videoBuffer);
        }
        return -EAGAIN;
    }

    uint64_t timestamp = 0;
    size_t frameSize = 0;
    if (videoCapturerGetFrame(videoCapturerHandle, videoBuffer, VIDEO_BUFFER_SIZE, &timestamp, &frameSize)) {
        printf("Failed to get frame\n");
        close(fd);
        videoCapturerDestory(videoCapturerHandle);
        if (videoBuffer) {
            free(videoBuffer);
        }
        return -EAGAIN;
    }

    if (write(fd, videoBuffer, frameSize) != frameSize) {
        printf("Failed to write frame to file\n");
    } else {
        printf("Frame with size %d capturered at %lld, saved as %s\n", frameSize, timestamp, argv[1]);
    }

    close(fd);
    videoCapturerDestory(videoCapturerHandle);
    if (videoBuffer) {
        free(videoBuffer);
    }

    return 0;
}
