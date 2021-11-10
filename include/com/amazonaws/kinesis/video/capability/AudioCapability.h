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

#include <stddef.h>
#include <stdint.h>

typedef enum {
    AUD_FMT_INVALID = 0,
    AUD_FMT_OPUS,
    AUD_FMT_G711A,
    AUD_FMT_G711U,
    AUD_FMT_PCM,
    AUD_FMT_AAC,
} AudioFormat;

typedef enum {
    AUD_CHN_INVALID = 0,
    AUD_CHN_MONO,
    AUD_CHN_STEREO,
} AudioChannel;

typedef enum {
    AUD_SAM_INVALID = 0,
    AUD_SAM_8K,
    AUD_SAM_16K,
    AUD_SAM_24K,
    AUD_SAM_32K,
    AUD_SAM_44_1K,
    AUD_SAM_48K,
} AudioSampleRate;

typedef enum {
    AUD_BIT_INVALID = 0,
    AUD_BIT_8,
    AUD_BIT_16,
    AUD_BIT_32,
} AudioBitDepth;

typedef struct {
    /*
     * ┌──────────────┐┌──────────────┐┌────────────────┐┌────────────────┐┌───────────────┐
     * │reserved[15-4]││AUD_FMT_AAC[3]││AUD_FMT_G711U[2]││AUD_FMT_G711A[1]││AUD_FMT_OPUS[0]│
     * └──────────────┘└──────────────┘└────────────────┘└────────────────┘└───────────────┘
     */
    uint16_t formats;
    /*
     * ┌──────────────┐┌─────────────────┐┌───────────────┐
     * │reserved[15-2]││AUD_CHN_STEREO[1]││AUD_CHN_MONO[0]│
     * └──────────────┘└─────────────────┘└───────────────┘
     */
    uint16_t channels;
    /*
     * ┌──────────────┐┌──────────────┐┌────────────────┐┌──────────────┐┌──────────────┐┌──────────────┐┌─────────────┐
     * │reserved[15-6]││AUD_SAM_48K[5]││AUD_SAM_44_1K[4]││AUD_SAM_32K[3]││AUD_SAM_24K[2]││AUD_SAM_16K[1]││AUD_SAM_8K[0]│
     * └──────────────┘└──────────────┘└────────────────┘└──────────────┘└──────────────┘└──────────────┘└─────────────┘
     */
    uint16_t sampleRates;
    /*
     * ┌──────────────┐┌─────────────┐┌─────────────┐┌────────────┐
     * │reserved[15-3]││AUD_BIT_32[2]││AUD_BIT_16[1]││AUD_BIT_8[0]│
     * └──────────────┘└─────────────┘└─────────────┘└────────────┘
     */
    uint16_t bitDepths;
} AudioCapability;

#ifdef __cplusplus
}
#endif
