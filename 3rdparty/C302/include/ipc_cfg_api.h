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
    CFG_VI_OPTION_BUTT
} IPC_CFG_VI_OPTIONS;

typedef enum IPC_CFG_STREAM_OPTIONS
{
    CFG_STREAM_OPTION_VIID = 0,
    CFG_STREAM_OPTION_ISPID,
    CFG_STREAM_OPTION_RESOLUTION,
    CFG_STREAM_OPTION_FORMAT,
    CFG_STREAM_OPTION_ENCODEC,
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
    CFG_IMAGE_OPTION_IRCUT,
    CFG_IMAGE_OPTION_D2N,
    CFG_IMAGE_OPTION_N2D,
    CFG_IMAGE_OPTION_BUTT
} IPC_CFG_IMAGE_OPTIONS;

typedef enum IPC_CFG_AI_OPTIONS
{
    CFG_AI_OPTION_ENCODEC = 0,
    CFG_AI_OPTION_CHANNEL,
    CFG_AI_OPTION_SAMPLERATE,
    CFG_AI_OPTION_BITWIDTH,
    CFG_AI_OPTION_VOLUME,
    //CFG_AI_OPTION_VQEENABLE,
    CFG_AI_OPTION_BUTT,
} IPC_CFG_AI_OPTIONS;

typedef enum IPC_CFG_AO_OPTIONS
{
    CFG_AO_OPTION_ENCODEC = 0,
    CFG_AO_OPTION_CHANNEL,
    CFG_AO_OPTION_SAMPLERATE,
    CFG_AO_OPTION_BITWIDTH,
    CFG_AO_OPTION_VOLUME,
    CFG_AO_OPTION_BUTT,
} IPC_CFG_AO_OPTIONS;


typedef enum IPC_PARAM_RESTORE
{
    PARAM_SIMPLE_RECOVERY = 0,
    PARAM_FULL_RECOVERY
} IPC_PARAM_RESTORE;

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
 *Description: Get parameters's information, such as setting range
 *
 *Param      : #[in] s32Chn, Channel number (parameters with channel concept are valid)
 *             #[in] enParamId, Parameter ID (used to distinguish the obtained parameter categories)
 *             #[in] s32SubId, Parameter SubID (some parameters have multiple subtypes)
 *             #[in/out] pOptions, Parameter details obtained
 *
 *Return     : =0, success
 *             =Other, fail
 *********************************************************************************/
int IPC_CFG_GetOptions(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, IPC_CFG_OPTIONS **pOptions);

/*********************************************************************************
 *Function   : IPC_CFG_RestoreParam
 *
 *Description: Restore initial parameters
 *
 *Param      : #[in] enMode, Recovery mode
 *
 *Return     : =0, success
 *             =Other, fail
 *********************************************************************************/
int IPC_CFG_RestoreParam(IPC_PARAM_RESTORE enMode, unsigned int uflag);

/*********************************************************************************
 *Function   : IPC_CFG_GetParam
 *
 *Description: Get parameters
 *
 *Param      : #[in] s32Chn, Channel number (parameters with channel concept are valid)
 *             #[in] enParamId, Parameter ID (used to distinguish the obtained parameter categories)
 *             #[in] s32SubId, Parameter SubID (some parameters have multiple subtypes)
 *             #[in/out] pConfig, Obtained parameter (cannot be NULL)
 *
 *Return     : =0, success
 *             =Other, fail
 *********************************************************************************/
int IPC_CFG_GetParam(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, void *pConfig);

int IPC_CFG_CheckParam(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, void *pConfig);

/*********************************************************************************
 *Function   : IPC_CFG_SetParam
 *
 *Description: Set parameters
 *
 *Param      : #[in] s32Chn, Channel number (parameters with channel concept are valid)
 *             #[in] enParamId, Parameter ID (used to distinguish the obtained parameter categories)
 *             #[in] s32SubId, Parameter SubID (some parameters have multiple subtypes)
 *             #[in] pConfig, Set parameter (cannot be NULL)
 *
 *Return     : =0, success
 *             =Other, fail
 *********************************************************************************/
int IPC_CFG_SetParam(int s32Chn, IPC_PARAM_ID enParamId, int s32SubId, void *pConfig);

int IPC_CFG_Init(unsigned int uflag);
void IPC_CFG_UnInit(unsigned int uflag);


#ifdef __cplusplus
#if __cplusplus
        }
#endif
#endif /* __cplusplus */

#endif /* __IPC_CFG_API_H__ */
