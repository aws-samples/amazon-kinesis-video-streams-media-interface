/*
 * Copyright (C) 2019-2024 Amlogic, Inc. All rights reserved.
 *
 * All information contained herein is Amlogic confidential.
 *
 * This software is provided to you pursuant to Software License Agreement
 * (SLA) with Amlogic Inc ("Amlogic"). This software may be used
 * only in accordance with the terms of this agreement.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification is strictly prohibited without prior written permission from
 * Amlogic.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __IPC_CFG_API_H__
#define __IPC_CFG_API_H__

#include "ipc_cfg_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define CFG_VIDEO_FLAG      (1 << 0)
#define CFG_AI_FLAG         (1 << 1)
#define CFG_AO_FLAG         (1 << 2)

/*----------------------------------------------------------------------------*/
typedef enum IPC_CFG_VI_OPTIONS
{
    CFG_VI_OPTION_SNS_TYPE = 0,
    CFG_VI_OPTION_FPS_MODE,
    CFG_VI_OPTION_INPUT_LANE,
    CFG_VI_OPTION_BUS_ID,
    CFG_VI_OPTION_BUTT
} IPC_CFG_VI_OPTIONS;

typedef enum IPC_CFG_STREAM_OPTIONS
{
    CFG_STREAM_OPTION_VIID = 0,
    CFG_STREAM_OPTION_PPUID,
    CFG_STREAM_OPTION_RESOLUTION,
    CFG_STREAM_OPTION_FORMAT,
    CFG_STREAM_OPTION_ENCODEC,
    CFG_STREAM_OPTION_DUMP,
    CFG_STREAM_OPTION_FRAMERATE,
    CFG_STREAM_OPTION_PROFILE,
    CFG_STREAM_OPTION_GOP,
    CFG_STREAM_OPTION_GOPMODE,
    CFG_STREAM_OPTION_RCMODE,
    CFG_STREAM_OPTION_BITRATE,
    CFG_STREAM_OPTION_MAX_BIT,
    CFG_STREAM_OPTION_MIN_BIT,
    CFG_STREAM_OPTION_IQP,
    CFG_STREAM_OPTION_PQP,
    CFG_STREAM_OPTION_BQP,
    CFG_STREAM_OPTION_QMAP,
    CFG_STREAM_OPTION_QUALITY,
    CFG_STREAM_OPTION_BUTT
} IPC_CFG_STREAM_OPTIONS;

typedef enum IPC_CFG_IMAGE_OPTIONS
{
    CFG_IMAGE_OPTION_FLIP = 0,
    CFG_IMAGE_OPTION_MIRROR,
    CFG_IMAGE_OPTION_EXP_MODE,
    CFG_IMAGE_OPTION_ME_TIME,
    CFG_IMAGE_OPTION_ME_DGAIN,
    CFG_IMAGE_OPTION_WB_MODE,
    CFG_IMAGE_OPTION_MWB_RGAIN,
    CFG_IMAGE_OPTION_MWB_BGAIN,
    CFG_IMAGE_OPTION_3D_DENOISE,
    CFG_IMAGE_OPTION_BRIGHTNESS,
    CFG_IMAGE_OPTION_SATURATION,
    CFG_IMAGE_OPTION_CONTRAST,
    CFG_IMAGE_OPTION_SHARPEN,
    CFG_IMAGE_OPTION_HUE,
    CFG_IMAGE_OPTION_WDR,
    CFG_IMAGE_OPTION_LDC,
    CFG_IMAGE_OPTION_HLC,
    CFG_IMAGE_OPTION_HLCLEVEL,
    CFG_IMAGE_OPTION_IRCUT,
    CFG_IMAGE_OPTION_D2N,
    CFG_IMAGE_OPTION_N2D,
    CFG_IMAGE_OPTION_FLICKER,
    CFG_IMAGE_OPTION_BUTT
} IPC_CFG_IMAGE_OPTIONS;

typedef enum IPC_CFG_AI_OPTIONS
{
    CFG_AI_OPTION_ENABLED = 0,
    CFG_AI_OPTION_ENCODEC,
    CFG_AI_OPTION_CHANNEL,
    CFG_AI_OPTION_SAMPLERATE,
    CFG_AI_OPTION_BITWIDTH,
    CFG_AI_OPTION_VOLUME,
    CFG_AI_OPTION_VQEENABLE,
    CFG_AI_OPTION_BUTT,
} IPC_CFG_AI_OPTIONS;

typedef enum IPC_CFG_AO_OPTIONS
{
    CFG_AO_OPTION_ENABLED = 0,
    CFG_AO_OPTION_ENCODEC,
    CFG_AO_OPTION_CHANNEL,
    CFG_AO_OPTION_SAMPLERATE,
    CFG_AO_OPTION_BITWIDTH,
    CFG_AO_OPTION_VOLUME,
    CFG_AO_OPTION_BUTT,
} IPC_CFG_AO_OPTIONS;

typedef enum IPC_PARAM_ID
{
    PARAM_VIDEO_ID = 0,
    PARAM_AI_ID,
    PARAM_AO_ID,
    PARAM_ID_BUTT
} IPC_PARAM_ID;

typedef enum IPC_PARAM_VIDEO_ID
{
    PARAM_VIDEO_ID_ALL = 0,
    PARAM_VIDEO_ID_VI,
    PARAM_VIDEO_ID_STREAM,
    PARAM_VIDEO_ID_IMAGE,
    PARAM_VIDEO_ID_BUTT
} IPC_PARAM_VIDEO_ID;

/*********************************************************************************
 *Function   : IPC_CFG_GetOptions
 *
 *Description: Obtain detailed information of parameters such as settable range
 *
 *Param      : #[in] s32Chn Channel number (parameters with channel concept are valid)
 *             #[in] enParamId Parameter ID (used to distinguish the obtained parameter categories)
 *             #[in] s32SubId Parameter ID (some parameters have multiple sub types) Example:IPC_PARAM_VIDEO_ID
 *             #[in/out] pOptions Parameter details obtained
 *
 *Return     : =0 Success
 *             =Other Failed
 *********************************************************************************/
int IPC_CFG_GetOptions(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, IPC_CFG_OPTIONS **pOptions);

/*********************************************************************************
 *Function   : IPC_CFG_GetParam
 *
 *Description: Get parameters
 *
 *Param      : #[in] s32Chn Channel number (parameters with channel concept are valid)
 *             #[in] enParamId Parameter ID (used to distinguish the obtained parameter categories)
 *             #[in] s32SubId Parameter ID (some parameters have multiple sub types) Example:IPC_PARAM_VIDEO_ID
 *             #[in/out] pConfig Obtained parameter (cannot be NULL)
 *
 *Return     : =0 Success
 *             =Other Failed
 *********************************************************************************/
int IPC_CFG_GetParam(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, void *pConfig);

int IPC_CFG_CheckParam(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, void *pConfig);

/*********************************************************************************
 *Function   : IPC_CFG_SetParam
 *
 *Description: Set parameters
 *
 *Param      : #[in] s32Chn Channel number (parameters with channel concept are valid)
 *             #[in] enParamId Parameter ID (used to distinguish the obtained parameter categories)
 *             #[in] s32SubId Parameter ID (some parameters have multiple sub types) Example:IPC_PARAM_VIDEO_ID
 *             #[in] pConfig Set parameter (cannot be NULL)
 *
 *Return     : =0 Success
 *             =Other Failed
 *********************************************************************************/
int IPC_CFG_SetParam(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, void *pConfig);

int IPC_CFG_GetParamItemInfo(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, int s32Option,
                                        unsigned int *psMin, unsigned int *psMax, unsigned int *psCur, unsigned int *psDef);
int IPC_CFG_SetParamItemInfo(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, int s32Option, int s32Value);

int IPC_CFG_Init(unsigned int uflag);
void IPC_CFG_UnInit(unsigned int uflag);


#ifdef __cplusplus
#if __cplusplus
        }
#endif
#endif /* __cplusplus */

#endif /* __IPC_CFG_API_H__ */
