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
    AUD_PLY_STATUS_NOT_READY = 0,
    AUD_PLY_STATUS_STREAM_OFF,
    AUD_PLY_STATUS_STREAM_ON,
} AudioPlayerStatus;

/**
 * @brief Handle of AudioPlayer.
 *
 */
typedef void* AudioPlayerHandle;

/**
 * @brief Create and init AudioPlayer.
 *
 * @return AudioPlayerHandle NULL or handle of created AudioPlayer.
 */
AudioPlayerHandle audioPlayerCreate(void);

/**
 * @brief Get player status.
 *
 * @param[in] handle Handle of AudioPlayer.
 * @return AudioPlayerStatus Status of AudioPlayer.
 */
AudioPlayerStatus audioPlayerGetStatus(const AudioPlayerHandle handle);

/**
 * @brief Get player capability.
 *
 * @param[in] handle Handle of AudioPlayer.
 * @param[out] pCapability Player capability.
 * @return int 0 or error code.
 */
int audioPlayerGetCapability(const AudioPlayerHandle handle, AudioCapability* pCapability);

/**
 * @brief Set player format, channel number, bit depth and sample rate.
 *
 * @param[in] handle Handle of AudioPlayer.
 * @param[in] format Frame format.
 * @param[in] channel Frame channel number.
 * @param[in] sampleRate Frame sample rate.
 * @param[in] bitDepth Frame bit depth.
 * @return int 0 or error code.
 */
int audioPlayerSetFormat(AudioPlayerHandle handle, const AudioFormat format, const AudioChannel channel, const AudioSampleRate sampleRate,
                         const AudioBitDepth bitDepth);

/**
 * @brief Get player format, channel number, bit depth and sample rate.
 *
 * @param[in] handle Handle of AudioPlayer.
 * @param[in] pFormat Frame format.
 * @param[in] pChannel Frame channel number.
 * @param[in] pSampleRate Frame sample rate.
 * @param[in] pBitDepth Frame bit depth.
 * @return int 0 or error code.
 */
int audioPlayerGetFormat(const AudioPlayerHandle handle, AudioFormat* pFormat, AudioChannel* pChannel, AudioSampleRate* pSampleRate,
                         AudioBitDepth* pBitDepth);

/**
 * @brief Acquire and turn on audio stream.
 *
 * @param[in] handle Handle of AudioPlayer.
 * @return int 0 or error code.
 */
int audioPlayerAcquireStream(AudioPlayerHandle handle);

/**
 * @brief Write a frame to audio player.
 *
 * @param[in] handle Handle of AudioPlayer.
 * @param[in] pData Frame data.
 * @param[in] size Size of frame data.
 * @return int 0 or error code.
 */
int audioPlayerWriteFrame(AudioPlayerHandle handle, void* pData, const size_t size);

/**
 * @brief Release acquired audio stream. If stream user count reached 0, stream will be turned off.
 *
 * @param[in] handle Handle of AudioPlayer.
 * @return int 0 or error code.
 */
int audioPlayerReleaseStream(AudioPlayerHandle handle);

/**
 * @brief Destroy created AudioPlayer.
 *
 * @param[in] handle Handle of AudioPlayer.
 */
void audioPlayerDestroy(AudioPlayerHandle handle);

#ifdef __cplusplus
}
#endif
