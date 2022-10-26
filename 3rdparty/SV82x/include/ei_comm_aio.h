/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_aio.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_AIO_H__
#define __EI_COMM_AIO_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX_AUDIO_POINT_BYTES  4
#define MAX_AO_POINT_NUM       4096

#define MAX_AUDIO_FRAME_LEN    (MAX_AUDIO_POINT_BYTES*MAX_AO_POINT_NUM)

#define MAX_AUDIO_STREAM_LEN   MAX_AUDIO_FRAME_LEN

#define MAX_AUDIO_FILE_PATH_LEN 256
#define MAX_AUDIO_FILE_NAME_LEN 256

#define VQE_EQ_BAND_NUM  10
#define VQE_DRC_SECNUM  5

#define AI_VQE_MASK_AEC     0x01
#define AI_VQE_MASK_ANR     0x02
#define AI_VQE_MASK_AGC     0x04
#define AI_VQE_MASK_EQ      0x08
#define AI_VQE_MASK_HPF     0x10
#define AI_VQE_MASK_HDR     0x20
#define AI_VQE_MASK_DRC     0x40
#define AI_VQE_MASK_RNR     0x80
#define AI_VQE_MASK_VAD   0x0100
#define AI_VQE_MASK_RES   0x1000
#define AI_VQE_MASK_AECE  0x2000

#define AO_VQE_MASK_HPF     0x10
#define AO_VQE_MASK_ANR     0x02
#define AO_VQE_MASK_AGC     0x04
#define AO_VQE_MASK_EQ      0x08
#define AO_VQE_MASK_RES   0x1000

#define AUDIOIN_ERRNO_RETURN_ERR_IF(cond, ret) \
    do {if (cond) {return (ret); } else {} } while (0)

#define AUDIOIN_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
        if (cond) { \
            EI_TRACE_AUDIOIN(EI_DBG_ERR, "errno[%d]!\n", errno);\
            return ret; \
        } \
} while (0)

#define AUDIOIN_ERRNO_POINTER_VAL_IF(ptr, ret) \
do { \
    if (EI_NULL == ptr) { \
        EI_TRACE_AUDIOIN(EI_DBG_ERR, "Null Pointer!\n"); \
        return ret; \
    } \
} while (0)


#define AUDIOOUT_ERRNO_RETURN_ERR_IF(cond, ret) \
        do {if (cond) {return (ret); } else {} } while (0)

#define AUDIOOUT_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
    do { \
            if (cond) { \
                EI_TRACE_AUDIOOUT(EI_DBG_ERR, "errno[%d]!\n", errno);\
                return ret; \
            } \
    } while (0)

#define AUDIOOUT_ERRNO_POINTER_VAL_IF(ptr, ret) \
    do { \
        if (EI_NULL == ptr) { \
            EI_TRACE_AUDIOOUT(EI_DBG_ERR, "Null Pointer!\n"); \
            return ret; \
        } \
    } while (0)




#define AUDIOCOMMON_ERRNO_RETURN_ERR_IF(cond, ret) \
    do {if (cond) {return (ret); } else {} } while (0)

#define AUDIOCOMMON_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
    do { \
            if (cond) { \
                EI_TRACE_AUDIOCOMMON(EI_DBG_ERR, "errno[%d]!\n", errno);\
                return ret; \
            } \
    } while (0)

#define AUDIOCOMMON_ERRNO_POINTER_VAL_IF(ptr, ret) \
    do { \
        if (EI_NULL == ptr) { \
            EI_TRACE_AUDIOCOMMON(EI_DBG_ERR, "Null Pointer!\n"); \
            return ret; \
        } \
    } while (0)

typedef enum eiAUDIO_SAMPLE_RATE_E {
    AUDIO_SAMPLE_RATE_8000   = 8000,
    AUDIO_SAMPLE_RATE_12000  = 12000,
    AUDIO_SAMPLE_RATE_11025  = 11025,
    AUDIO_SAMPLE_RATE_16000  = 16000,
    AUDIO_SAMPLE_RATE_22050  = 22050,
    AUDIO_SAMPLE_RATE_24000  = 24000,
    AUDIO_SAMPLE_RATE_32000  = 32000,
    AUDIO_SAMPLE_RATE_44100  = 44100,
    AUDIO_SAMPLE_RATE_48000  = 48000,
    AUDIO_SAMPLE_RATE_64000  = 64000,
    AUDIO_SAMPLE_RATE_96000  = 96000,
    AUDIO_SAMPLE_RATE_BUTT,
} AUDIO_SAMPLE_RATE_E;

typedef enum eiAUDIO_BIT_WIDTH_E {
    AUDIO_BIT_WIDTH_8   = 0,
    AUDIO_BIT_WIDTH_16  = 1,
    AUDIO_BIT_WIDTH_24  = 2,
    AUDIO_BIT_WIDTH_BUTT,
} AUDIO_BIT_WIDTH_E;

typedef enum eiAIO_SOUND_MODE_E {
    AUDIO_SOUND_MODE_MONO   = 0,
    AUDIO_SOUND_MODE_STEREO = 1,
    AUDIO_SOUND_MODE_BUTT
} AUDIO_SOUND_MODE_E;

typedef enum eiG726_BPS_E {
    G726_16K = 0,
    G726_24K,
    G726_32K,
    G726_40K,
    MEDIA_G726_16K,
    MEDIA_G726_24K,
    MEDIA_G726_32K,
    MEDIA_G726_40K,
    G726_BUTT,
} G726_BPS_E;

typedef enum eiADPCM_TYPE_E {
    ADPCM_TYPE_DVI4 = 0,
    ADPCM_TYPE_IMA,
    ADPCM_TYPE_ORG_DVI4,
    ADPCM_TYPE_BUTT,
} ADPCM_TYPE_E;

typedef struct eiAIO_ATTR_S {
    AUDIO_SAMPLE_RATE_E enSamplerate;
    AUDIO_BIT_WIDTH_E   enBitwidth;
    AUDIO_SOUND_MODE_E  enSoundmode;
    EI_U32              u32EXFlag;
    EI_U32              u32FrmNum;
    EI_U32              u32PtNumPerFrm;
    EI_U32              u32ChnCnt;
} AIO_ATTR_S;

typedef struct eiAI_CHN_PARAM_S {
    EI_U32 u32UsrFrmDepth;
} AI_CHN_PARAM_S;

typedef struct eiAUDIO_FRAME_S {
    AUDIO_BIT_WIDTH_E   enBitwidth;
    AUDIO_SOUND_MODE_E  enSoundmode;
    EI_U8  *u64VirAddr;
    EI_U64  u64PhyAddr;
    EI_U64  u64TimeStamp;
    EI_U32  u32Seq;
    EI_U32  u32Len;
    EI_U32  u32PoolId;
    EI_U32  u32BufferId;
} AUDIO_FRAME_S;

typedef struct eiAEC_FRAME_S {
    AUDIO_FRAME_S   stRefFrame;
    EI_BOOL         bValid;
} AEC_FRAME_S;

typedef struct eiAUDIO_FRAME_INFO_S {
    AUDIO_FRAME_S *pstFrame;
    EI_U32         u32Id;
} AUDIO_FRAME_INFO_S;

typedef struct eiAUDIO_STREAM_S {
    EI_U8 ATTRIBUTE *pStream;
    EI_U64 ATTRIBUTE u64PhyAddr;
    EI_U32 u32Len;
    EI_U64 u64TimeStamp;
    EI_U32 u32Seq;
} AUDIO_STREAM_S;
typedef struct eiAO_CHN_STATE_S {
    EI_U32                  u32ChnTotalNum;
    EI_U32                  u32ChnFreeNum;
    EI_U32                  u32ChnBusyNum;
} AO_CHN_STATE_S;

typedef enum eiAUDIO_TRACK_MODE_E {
    AUDIO_TRACK_NORMAL      = 0,
    AUDIO_TRACK_BOTH_LEFT   = 1,
    AUDIO_TRACK_BOTH_RIGHT  = 2,
    AUDIO_TRACK_EXCHANGE    = 3,
    AUDIO_TRACK_MIX         = 4,
    AUDIO_TRACK_LEFT_MUTE   = 5,
    AUDIO_TRACK_RIGHT_MUTE  = 6,
    AUDIO_TRACK_BOTH_MUTE   = 7,

    AUDIO_TRACK_BUTT
} AUDIO_TRACK_MODE_E;

typedef enum eiAUDIO_FADE_RATE_E {
    AUDIO_FADE_RATE_1   = 0,
    AUDIO_FADE_RATE_2   = 1,
    AUDIO_FADE_RATE_4   = 2,
    AUDIO_FADE_RATE_8   = 3,
    AUDIO_FADE_RATE_16  = 4,
    AUDIO_FADE_RATE_32  = 5,
    AUDIO_FADE_RATE_64  = 6,
    AUDIO_FADE_RATE_128 = 7,

    AUDIO_FADE_RATE_BUTT
} AUDIO_FADE_RATE_E;

typedef struct eiAUDIO_FADE_S {
    EI_BOOL         bFade;
    AUDIO_FADE_RATE_E enFadeInRate;
    AUDIO_FADE_RATE_E enFadeOutRate;
} AUDIO_FADE_S;

typedef struct eiAUDIO_ANR_CONFIG_S {
    EI_BOOL bUsrMode;

    EI_S32 s32Reserved;
} AUDIO_ANR_CONFIG_S;

typedef struct eiAUDIO_AGC_CONFIG_S {
    EI_BOOL bUsrMode;          /* mode 0: auto, mode 1: manual.*/

    EI_S16 s16TargetLevel;       /*Target level in -dBfs of envelope (default -3).range: [0, 31]dB */
    EI_S16 s16MaxGain;           /* (default 20).  max gain, range: [0, 30]dB */

    EI_S32 s32Reserved;
} AUDIO_AGC_CONFIG_S;

typedef struct eiAUDIO_VAD_CONFIG_S {
    EI_BOOL bUsrMode;          /* mode 0: auto, mode 1: manual.*/

    EI_S32 s32NoiseFloor;       /*noise floor  (default 8000).range: [0, 2^18] */
    EI_S32 s32DelayMs;           /* delay time(ms)(default 1000ms).  max gain, range: [0, 5000]ms */

    EI_S32 s32Reserved;
} AUDIO_VAD_CONFIG_S;

typedef enum eiAUDIO_HPF_FREQ_E {
    AUDIO_HPF_FREQ_80   = 80,
    AUDIO_HPF_FREQ_100  = 100,
    AUDIO_HPF_FREQ_120  = 120,
    AUDIO_HPF_FREQ_150  = 150,
    AUDIO_HPF_FREQ_BUTT,
} AUDIO_HPF_FREQ_E;

typedef struct eiAUDIO_HPF_CONFIG_S {
    EI_BOOL bUsrMode;
    AUDIO_HPF_FREQ_E enHpfFreq;
} AUDIO_HPF_CONFIG_S;

typedef struct eiAI_RNR_CONFIG_S {
    EI_BOOL bUsrMode;

    EI_S32  s32NrMode;

    EI_S32 s32MaxNrLevel;

    EI_S32  s32NoiseThresh;
} AI_RNR_CONFIG_S;

typedef struct eiAUDIO_EQ_CONFIG_S {
    EI_S8  s8GaindB[VQE_EQ_BAND_NUM];
    EI_S32 s32Reserved;
} AUDIO_EQ_CONFIG_S;


typedef enum eiVQE_WORKSTATE_E {
    VQE_WORKSTATE_COMMON  = 0,
    VQE_WORKSTATE_MUSIC   = 1,
    VQE_WORKSTATE_NOISY   = 2,
} VQE_WORKSTATE_E;


typedef enum eiVQE_RECORD_TYPE {
    VQE_RECORD_NORMAL        = 0,
    VQE_RECORD_BUTT,
} VQE_RECORD_TYPE;


typedef EI_S32(*pFuncGainCallBack)(EI_S32 s32SetGain);

typedef struct eiAI_HDR_CONFIG_S {
    EI_BOOL bUsrMode;

    EI_S32 s32MinGaindB;
    EI_S32 s32MaxGaindB;

    EI_S32 s32MicGaindB;
    EI_S32 s32MicGainStepdB;
    pFuncGainCallBack pcallback;
} AI_HDR_CONFIG_S;

typedef struct eiAI_DRC_CONFIG_S {
    EI_BOOL bUsrMode;

    EI_S16  s16AttackTime;
    EI_S16  s16ReleaseTime;

    EI_S16 s16LevDb[VQE_DRC_SECNUM];
} AI_DRC_CONFIG_S;


typedef struct eiAI_VQE_CONFIG_S {
    EI_U32              u32OpenMask;
    EI_S32              s32Channels;
    EI_S32              s32WorkSampleRate;
    EI_S32              s32FrameSample;
    VQE_WORKSTATE_E     enWorkstate;

    AUDIO_HPF_CONFIG_S  stHpfCfg;
    AUDIO_ANR_CONFIG_S  stAnrCfg;
    AUDIO_AGC_CONFIG_S  stAgcCfg;
    AUDIO_EQ_CONFIG_S   stEqCfg;
    AUDIO_VAD_CONFIG_S  stVadCfg;
    AI_DRC_CONFIG_S     stDrcCfg;
} AI_VQE_CONFIG_S;

typedef struct eiAO_VQE_CONFIG_S {
    EI_U32              u32OpenMask;

    EI_S32              s32WorkSampleRate;
    EI_S32              s32FrameSample;
    VQE_WORKSTATE_E     enWorkstate;

    AUDIO_HPF_CONFIG_S stHpfCfg;
    AUDIO_ANR_CONFIG_S stAnrCfg;
    AUDIO_AGC_CONFIG_S stAgcCfg;
    AUDIO_EQ_CONFIG_S  stEqCfg;
} AO_VQE_CONFIG_S;


typedef struct eiVQE_MODULE_CONFIG_S {
    EI_VOID *pHandle;
} VQE_MODULE_CONFIG_S;

typedef struct eiAUDIO_SAVE_FILE_INFO_S {
    EI_BOOL     bCfg;
    EI_CHAR     aFilePath[MAX_AUDIO_FILE_PATH_LEN];
    EI_CHAR     aFileName[MAX_AUDIO_FILE_NAME_LEN];
    EI_U32      u32FileSize;
} AUDIO_SAVE_FILE_INFO_S;

typedef struct eiAUDIO_FILE_STATUS_S {
    EI_BOOL     bSaving;
} AUDIO_FILE_STATUS_S;


/* audioin */
#define EI_ERR_AUDIOIN_NULL_PTR \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)

#define EI_ERR_AUDIOIN_NOTREADY \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define EI_ERR_AUDIOIN_NOT_PERM \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define EI_ERR_AUDIOIN_NOT_ENABLED \
        EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_ENABLED)

#define EI_ERR_AUDIOIN_NOMEM \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)

#define EI_ERR_AUDIOIN_ILLEGAL_PARAM \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)

#define EI_ERR_AUDIOIN_BUSY \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

#define EI_ERR_AUDIOIN_NOT_SUPPORT \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)

#define EI_ERR_AUDIOIN_SIZE_NOT_ENOUGH \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)

#define EI_ERR_AUDIOIN_INVALID_DEVID \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)

#define EI_ERR_AUDIOIN_INVALID_CHNID \
        EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)

#define EI_ERR_AUDIOIN_NOT_CONFIG \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)

#define EI_ERR_AUDIOIN_NOBUF \
    EI_DEF_ERR(EI_ID_AUDIOIN, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)

#define EI_TRACE_AUDIOIN(level, fmt, args...) \
       EI_TRACE(EI_ID_AUDIOIN, level, fmt, ##args);

/* audioout */

#define EI_ERR_AUDIOOUT_NULL_PTR \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)

#define EI_ERR_AUDIOOUT_NOTREADY \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define EI_ERR_AUDIOOUT_NOT_PERM \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define EI_ERR_AUDIOOUT_NOT_ENABLED \
                   EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_ENABLED)

#define EI_ERR_AUDIOOUT_NOMEM \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)

#define EI_ERR_AUDIOOUT_ILLEGAL_PARAM \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)

#define EI_ERR_AUDIOOUT_BUSY \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

#define EI_ERR_AUDIOOUT_NOT_SUPPORT \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)

#define EI_ERR_AUDIOOUT_SIZE_NOT_ENOUGH \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)

#define EI_ERR_AUDIOOUT_INVALID_DEVID \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)

#define EI_ERR_AUDIOOUT_INVALID_CHNID \
                   EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)

#define EI_ERR_AUDIOOUT_NOT_CONFIG \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)

#define EI_ERR_AUDIOOUT_NOBUF \
               EI_DEF_ERR(EI_ID_AUDIOOUT, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)

#define EI_TRACE_AUDIOOUT(level, fmt, args...) \
                  EI_TRACE(EI_ID_AUDIOOUT, level, fmt, ##args);

/* audiocommon */
#define EI_ERR_AUDIOCOMMON_NULL_PTR \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)

#define EI_ERR_AUDIOCOMMON_NOTREADY \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define EI_ERR_AUDIOCOMMON_NOT_PERM \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define EI_ERR_AUDIOCOMMON_NOT_ENABLED \
                          EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_ENABLED)

#define EI_ERR_AUDIOCOMMON_NOMEM \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)

#define EI_ERR_AUDIOCOMMON_ILLEGAL_PARAM \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)

#define EI_ERR_AUDIOCOMMON_BUSY \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

#define EI_ERR_AUDIOCOMMON_NOT_SUPPORT \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)

#define EI_ERR_AUDIOCOMMON_SIZE_NOT_ENOUGH \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)

#define EI_ERR_AUDIOCOMMON_INVALID_DEVID \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)

#define EI_ERR_AUDIOCOMMON_INVALID_CHNID \
                          EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)

#define EI_ERR_AUDIOCOMMON_NOT_CONFIG \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)

#define EI_ERR_AUDIOCOMMON_NOBUF \
                      EI_DEF_ERR(EI_ID_AUDIOCOMMON, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)

#define EI_TRACE_AUDIOCOMMON(level, fmt, args...) \
                         EI_TRACE(EI_ID_AUDIOCOMMON, level, fmt, ##args);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_AIO_H__ */

