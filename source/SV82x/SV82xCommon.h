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

#include <errno.h>
#include <stdio.h>
#include <pthread.h>

#include "sample_comm.h"

#define LOG(msg, ...) printf(msg "\n", ##__VA_ARGS__)

#define SV82X_HANDLE_NULL_CHECK(x)                                                                                                                   \
    if (!(x)) {                                                                                                                                      \
        return -EINVAL;                                                                                                                              \
    }
#define EI_TRACE_AUDIO(level, fmt, args...) EI_TRACE(EI_ID_AUDIOIN, level, fmt, ##args)
#define SV82X_HANDLE_STATUS_CHECK(Handle, expectedStatus)                                                                                            \
    if ((Handle)->status != (expectedStatus)) {                                                                                                      \
        return -EAGAIN;                                                                                                                              \
    }

typedef enum {
    ELF_AUDIO_FRAME_G711A,
    ELF_AUDIO_FRAME_G711U,
    ELF_AUDIO_FRAME_ADPCMA,
    ELF_AUDIO_FRAME_G726,
    ELF_AUDIO_FRAME_G729,
    ELF_AUDIO_FRAME_LPCM,
    ELF_AUDIO_FRAME_AAC,
    ELF_AUDIO_FRAME_MP3,
    ELF_AUDIO_FRAME_TYPE_MAX,
} sv82x_audio_frame_type_e;

typedef enum { ELF_AUDIO_STREAM_CAPTURE, ELF_AUDIO_STREAM_PLAYBACK, ELF_AUDIO_STREAM_MAX } sv82x_audio_stream_e;

typedef struct {
    PAYLOAD_TYPE_E payload_type; /* payload type */
    AUDIO_SAMPLE_RATE_E samplerate;
    unsigned int num_per_frm; /* sample per frame */
    unsigned int fps;         /* frame rate ;default 25*/
    AUDIO_SOUND_MODE_E sound_mode;
    AUDIO_BIT_WIDTH_E bitwidth;
    int volume;
    unsigned int u32ChnCnt;
} sv82x_audio_param_t;

typedef struct {
    int sample_rate;
    int frame_rate;
    EI_U32 open_mask; /*AEC | AGC | ANR | EQ | HPF */
} sv82x_audio_vqe_t;

typedef struct VIDEO_STREAM_CTX {
    EI_U32 AiDev;
    EI_U32 AencChn;
    EI_U32 AdecChn;
    EI_U32 AoChn;
    EI_U32 AoDev;
    EI_U32 u32ChnCnt;
    sv82x_audio_param_t paramEncode;
    sv82x_audio_param_t paramDecode;
    EI_BOOL audio_adec_ao_flag;
    PAYLOAD_TYPE_E enPayloadType;
    EI_BOOL bAioReSample;
    EI_BOOL enInSampleRate;
    EI_BOOL enOutSampleRate;
    EI_BOOL enVqe;
    EI_BOOL enAudioCreate;
    EI_BOOL enAencCreate;
    EI_BOOL enUserGetMode;
    sv82x_audio_vqe_t audio_vqe;
    EI_CHAR playFileName[512];
} sv82x_audio_stream_info_t;
