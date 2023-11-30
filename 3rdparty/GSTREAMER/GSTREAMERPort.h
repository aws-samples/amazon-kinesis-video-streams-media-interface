/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include <stdint.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>

#define GST_VIDEO_SINK_NAME    "h264sink"
#define GST_AUDIO_SINK_NAME    "alawsink"
#define GST_AUDIO_SRC_NAME     "alawsrc"

typedef struct {
    void *capturerHandle;
} GSTAppSinkSrc;

typedef enum {
    VIDEO_CAPTURE_PIPELINE,
    AUDIO_CAPTURE_PIPELINE,
    AUDIO_PLAYER_PIPELINE
} GstPipelineType;

int initGstVideoSrc(GSTAppSinkSrc *handle);
int startGstVideoSrc(void);
int stopGstVideoSrc(void);
int deinitGstVideoSrc(void);

int initGstAudioSrc(GSTAppSinkSrc *handle);
int startGstAudioSrc(void);
int stopGstAudioSrc(void);
int deinitGstAudioSrc(void);

int initGstAudioPlayer(GSTAppSinkSrc *handle);
int startGstAudioPlayer(void);
int stopGstAudioPlayer(void);
int deinitGstAudioPlayer(void);
int writeToGstAudioPlayer(uint8_t *pData, size_t size);

uint64_t getEpochTimestampInUs(void);
GstElement * getPipeline(int type);
