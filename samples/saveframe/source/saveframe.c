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
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"

#define VIDEO_BUFFER_SIZE               (1 * 1024 * 1024UL)
#define SENSOR_ADAPTION_DELAY_MICRO_SEC (500 * 1000UL)

#define CONVERT_NV12_TO_RGB24

#ifdef CONVERT_NV12_TO_RGB24

#define RGB24_FRAME_WIDTH  (416)
#define RGB24_FRAME_HEIGHT (320)
#define RGB_FRAME_SIZE     (RGB24_FRAME_WIDTH * RGB24_FRAME_HEIGHT * 3UL)

int NV12_TO_RGB24(uint8_t* yuyv, uint8_t* rgbBuffer, size_t width, size_t height)
{
    const int nv_start = width * height;
    uint32_t i, j, index = 0, rgb_index = 0;
    uint8_t y, u, v;
    int r, g, b, nv_index = 0;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            nv_index = i / 2 * width + j - j % 2;

            y = yuyv[rgb_index];
            u = yuyv[nv_start + nv_index];
            v = yuyv[nv_start + nv_index + 1];

            r = y + (140 * (v - 128)) / 100;                         // r
            g = y - (34 * (u - 128)) / 100 - (71 * (v - 128)) / 100; // g
            b = y + (177 * (u - 128)) / 100;                         // b

            if (r > 255)
                r = 255;
            if (g > 255)
                g = 255;
            if (b > 255)
                b = 255;
            if (r < 0)
                r = 0;
            if (g < 0)
                g = 0;
            if (b < 0)
                b = 0;

            index = i * width * 3 + 3 * j;

            rgbBuffer[index] = r;
            rgbBuffer[index + 1] = g;
            rgbBuffer[index + 2] = b;

            rgb_index++;
        }
    }
    return 0;
}
#endif

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("Usage: saveframe-static FILE NUM\n");
        return -EINVAL;
    }

    int ret = 0;
    int fd = -1;
    VideoCapturerHandle videoCapturerHandle = NULL;
    uint8_t videoBuffer[VIDEO_BUFFER_SIZE];
    uint64_t timestamp = 0;
    size_t frameSize = 0;
    char outPutNameBuffer[128];

    videoCapturerHandle = videoCapturerCreate();

    for (int i = 0; i < atoi(argv[2]); ++i) {
        sprintf(outPutNameBuffer, "%s-%d", argv[1], i);
        fd = open(outPutNameBuffer, O_RDWR | O_CREAT, 0x644);

        if (fd < 0) {
            printf("Failed to open file %s\n", outPutNameBuffer);
            ret = -EAGAIN;
        } else if (!videoCapturerHandle) {
            printf("Failed to create video capturer\n");
            ret = -EAGAIN;
        } else if (videoCapturerSetFormat(videoCapturerHandle, VID_FMT_H264, VID_RES_1080P)) {
            printf("Failed to set video capturer format\n");
            ret = -EAGAIN;
        } else if (videoCapturerAcquireStream(videoCapturerHandle)) {
            printf("Failed to acquire video stream\n");
            ret = -EAGAIN;
        }

        // Now we try to capture a raw frame
        if (!ret) {
            // Camera sensor might need some time to adaptive lights
            usleep(SENSOR_ADAPTION_DELAY_MICRO_SEC);

            if (videoCapturerGetFrame(videoCapturerHandle, videoBuffer, VIDEO_BUFFER_SIZE, &timestamp, &frameSize)) {
                printf("Failed to get frame\n");
                ret = -EAGAIN;
            } else {
                videoCapturerReleaseStream(videoCapturerHandle);
            }

#ifdef CONVERT_NV12_TO_RGB24
            uint8_t rgbBuffer[RGB_FRAME_SIZE] = {0};
            NV12_TO_RGB24(videoBuffer, rgbBuffer, RGB24_FRAME_WIDTH, RGB24_FRAME_HEIGHT);

            if (write(fd, rgbBuffer, RGB_FRAME_SIZE) != RGB_FRAME_SIZE) {
                printf("Failed to write frame to file\n");
                ret = -EAGAIN;
            } else {
                printf("Frame with size %ld capturered at %ld, saved as %s\n", RGB_FRAME_SIZE, timestamp, outPutNameBuffer);
            }
#else
            if (write(fd, videoBuffer, frameSize) != frameSize) {
                printf("Failed to write frame to file\n");
                ret = -EAGAIN;
            } else {
                printf("Frame with size %ld capturered at %ld, saved as %s\n", frameSize, timestamp, outPutNameBuffer);
            }
#endif
        }

        if (fd >= 0) {
            close(fd);
        }
    }

    // Clean up
    if (videoCapturerHandle) {
        videoCapturerDestroy(videoCapturerHandle);
    }

    return ret;
}
