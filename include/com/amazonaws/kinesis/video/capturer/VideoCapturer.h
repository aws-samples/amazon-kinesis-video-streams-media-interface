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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "com/amazonaws/kinesis/video/capability/VideoCapability.h"

typedef enum {
    VID_CAP_STATUS_NOT_READY = 0,
    VID_CAP_STATUS_STREAM_OFF,
    VID_CAP_STATUS_STREAM_ON,
} VideoCapturerStatus;

/**
 * @brief Handle of VideoCapturer.
 *
 */
typedef void* VideoCapturerHandle;

/**
 * @brief Create and init VideoCapturer.
 *
 * @return VideoCapturerHandle NULL or handle of created VideoCapturer.
 */
VideoCapturerHandle videoCapturerCreate(void);

/**
 * @brief Get capturer status.
 *
 * @param[in] handle Handle of VideoCapturer.
 * @return VideoCapturerStatus Status of VideoCapturer.
 */
VideoCapturerStatus videoCapturerGetStatus(const VideoCapturerHandle handle);

/**
 * @brief Get capturer capability.
 *
 * @param[in] handle Handle of VideoCapturer.
 * @param[out] pCapability Capturer capability.
 * @return int 0 or error code.
 */
int videoCapturerGetCapability(const VideoCapturerHandle handle, VideoCapability* pCapability);

/**
 * @brief Set capturer format and resolution.
 *
 * @param[in] handle Handle of VideoCapturer.
 * @param[in] format Frame format.
 * @param[in] resolution Frame resolution.
 * @return int 0 or error code.
 */
int videoCapturerSetFormat(VideoCapturerHandle handle, const VideoFormat format, const VideoResolution resolution);

/**
 * @brief Get capturer format and resolution.
 *
 * @param[in] handle Handle of VideoCapturer.
 * @param[out] pFormat Frame format.
 * @param[out] pResolution Frame resolution.
 * @return int 0 or error code.
 */
int videoCapturerGetFormat(const VideoCapturerHandle handle, VideoFormat* pFormat, VideoResolution* pResolution);

/**
 * @brief Acquire and turn on video stream.
 *
 * @param[in] handle Handle of VideoCapturer.
 * @return int 0 or error code.
 */
int videoCapturerAcquireStream(VideoCapturerHandle handle);

/**
 * @brief Blocking get frame from capturer.
 *
 * @param[in] handle Handle of VideoCapturer.
 * @param[in,out] pFrameDataBuffer Target frame data buffer.
 * @param[in] frameDataBufferSize Frame data buffer size.
 * @param[out] pTimestamp Frame timestamp in microseconds(usec).
 * @param[out] pFrameSize Frame data size in bytes.
 * @return int 0 or error code.
 */
int videoCapturerGetFrame(VideoCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize);

/**
 * @brief Release acquired video stream.
 *
 * @param[in] handle Handle of VideoCapturer.
 * @return int 0 or error code.
 */
int videoCapturerReleaseStream(VideoCapturerHandle handle);

/**
 * @brief Destroy created VideoCapturer.
 *
 * @param[in] handle Handle of VideoCapturer.
 */
void videoCapturerDestroy(VideoCapturerHandle handle);

#ifdef __cplusplus
}
#endif
