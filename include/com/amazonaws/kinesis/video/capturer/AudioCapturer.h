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

#include "com/amazonaws/kinesis/video/capability/AudioCapability.h"

typedef enum {
    AUD_CAP_STATUS_NOT_READY = 0,
    AUD_CAP_STATUS_STREAM_OFF,
    AUD_CAP_STATUS_STREAM_ON,
} AudioCapturerStatus;

/**
 * @brief Handle of AudioCapturer.
 *
 */
typedef void* AudioCapturerHandle;

/**
 * @brief Create and init AudioCapturer.
 *
 * @return AudioCapturerHandle NULL or handle of created AudioCapturer.
 */
AudioCapturerHandle audioCapturerCreate(void);

/**
 * @brief Get capturer status.
 *
 * @param[in] handle Handle of AudioCapturer.
 * @return AudioCapturerStatus
 */
AudioCapturerStatus audioCapturerGetStatus(const AudioCapturerHandle handle);

/**
 * @brief Get capturer capability.
 *
 * @param[in] handle Handle of AudioCapturer.
 * @param[out] pCapability Capturer capability.
 * @return int 0 or error code.
 */
int audioCapturerGetCapability(const AudioCapturerHandle handle, AudioCapability* pCapability);

/**
 * @brief Set capturer format, channel number, bit depth and sample rate.
 *
 * @param[in] handle Handle of AudioCapturer.
 * @param[in] format Frame format.
 * @param[in] channel Frame channel number.
 * @param[in] sampleRate Frame sample rate.
 * @param[in] bitDepth Frame bit depth.
 * @return int 0 or error code.
 */
int audioCapturerSetFormat(AudioCapturerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                           const AudioBitDepth bitDepth);

/**
 * @brief Get capturer format, channel number, bit depth and sample rate.
 *
 * @param[in] handle Handle of AudioCapturer.
 * @param[in] pFormat Frame format.
 * @param[in] pChannel Frame channel number.
 * @param[in] pSampleRate Frame sample rate.
 * @param[in] pBitDepth Frame bit depth.
 * @return int 0 or error code.
 */
int audioCapturerGetFormat(const AudioCapturerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                           AudioBitDepth* pBitDepth);

/**
 * @brief Acquire and turn on audio stream.
 *
 * @param[in] handle Handle of AudioCapturer.
 * @return int 0 or error code.
 */
int audioCapturerAcquireStream(AudioCapturerHandle handle);

/**
 * @brief Blocking get frame from capturer.
 *
 * @param[in] handle Handle of AudioCapturer.
 * @param[in,out] pFrameDataBuffer Target frame data buffer.
 * @param[in] frameDataBufferSize Frame data buffer size.
 * @param[out] pTimestamp Frame timestamp in microseconds(usec).
 * @param[out] pFrameSize Frame data size in bytes.
 * @return int 0 or error code.
 */
int audioCapturerGetFrame(AudioCapturerHandle handle, void* pFrameDataBuffer, const size_t frameDataBufferSize, uint64_t* pTimestamp,
                          size_t* pFrameSize);

/**
 * @brief Release acquired audio stream.
 *
 * @param[in] handle Handle of AudioCapturer.
 * @return int 0 or error code.
 */
int audioCapturerReleaseStream(AudioCapturerHandle handle);

/**
 * @brief Destroy created AudioCapturer.
 *
 * @param[in] handle Handle of AudioCapturer.
 */
void audioCapturerDestroy(AudioCapturerHandle handle);

#ifdef __cplusplus
}
#endif
