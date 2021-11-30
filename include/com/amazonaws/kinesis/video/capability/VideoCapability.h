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
    VID_FMT_INVALID = 0,
    VID_FMT_H264,
    VID_FMT_H265,
    VID_FMT_VP8,
    VID_FMT_RAW,
} VideoFormat;

typedef enum {
    VID_RES_INVALID = 0,
    VID_RES_320P,
    VID_RES_360P,
    VID_RES_480P,
    VID_RES_720P,
    VID_RES_1080P,
    VID_RES_2K,
    VID_RES_4K,
} VideoResolution;

typedef struct {
    /*
     * ┌──────────────┐┌──────────────┐┌──────────────┐┌───────────────┐┌───────────────┐
     * │reserved[15-4]││VID_FMT_RAW[3]││VID_FMT_VP8[2]││VID_FMT_H265[1]││VID_FMT_H264[0]│
     * └──────────────┘└──────────────┘└──────────────┘└───────────────┘└───────────────┘
     */
    uint16_t formats;
    /*
     * ┌──────────────┐┌─────────────┐┌─────────────┐┌────────────────┐┌───────────────┐┌───────────────┐┌───────────────┐┌───────────────┐
     * │reserved[15-7]││VID_RES_4K[6]││VID_RES_2K[5]││VID_RES_1080P[4]││VID_RES_720P[3]││VID_RES_480P[2]││VID_RES_360P[1]││VID_RES_320P[0]│
     * └──────────────┘└─────────────┘└─────────────┘└────────────────┘└───────────────┘└───────────────┘└───────────────┘└───────────────┘
     */
    uint16_t resolutions;
} VideoCapability;

#ifdef __cplusplus
}
#endif
