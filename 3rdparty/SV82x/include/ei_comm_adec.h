/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_adec.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef  __EI_COMM_ADEC_H__
#define  __EI_COMM_ADEC_H__


#include "ei_type.h"
#include "ei_common.h"
#include "ei_debug.h"
#include "ei_comm_aio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct eiADEC_ATTR_G711_S {
    EI_U32 resv;
} ADEC_ATTR_G711_S;

typedef struct eiADEC_ATTR_G726_S {
    G726_BPS_E enG726bps;
} ADEC_ATTR_G726_S;

typedef struct eiADEC_ATTR_ADPCM_S {
    ADPCM_TYPE_E enADPCMType;
} ADEC_ATTR_ADPCM_S;

typedef struct eiADEC_ATTR_LPCM_S {
    EI_U32 resv;
} ADEC_ATTR_LPCM_S;

typedef struct eiADEC_ATTR_AAC_S {
    EI_U32 resv;
} ADEC_ATTR_AAC_S;

typedef enum eiADEC_MODE_E {
    ADEC_MODE_PACK = 0,
    ADEC_MODE_STREAM,
    ADEC_MODE_BUTT
} ADEC_MODE_E;

typedef struct eiADEC_CHN_ATTR_S {
    PAYLOAD_TYPE_E enType;
    EI_U32         u32BufSize;
    ADEC_MODE_E    enMode;
    EI_VOID ATTRIBUTE      *pValue;
    EI_S32 s32Ch;
    AUDIO_SAMPLE_RATE_E enSamplerate;
    AUDIO_BIT_WIDTH_E   enBitwidth;
    AUDIO_SOUND_MODE_E  enSoundmode;
    EI_U32              u32PtNumPerFrm;
} ADEC_CHN_ATTR_S;

typedef struct eiADEC_DECODER_S {
    PAYLOAD_TYPE_E  enType;
    EI_CHAR         aszName[17];
    EI_S32(*pfnOpenDecoder)(EI_VOID *pDecoderAttr,
        EI_VOID **ppDecoder);
    EI_S32(*pfnDecodeFrm)(EI_VOID *pDecoder, EI_U8 **pu8Inbuf, EI_S32 *ps32LeftByte,
        EI_U16 *pu16Outbuf, EI_U32 *pu32OutLen, EI_U32 *pu32Chns);
    EI_S32(*pfnGetFrmInfo)(EI_VOID *pDecoder, EI_VOID *pInfo);
    EI_S32(*pfnCloseDecoder)(EI_VOID *pDecoder);
    EI_S32(*pfnResetDecoder)(EI_VOID *pDecoder);
} ADEC_DECODER_S;

typedef enum eiEN_ADEC_ERR_CODE_E {
    ADEC_ERR_DECODER_ERR     = 64,
    ADEC_ERR_BUF_LACK,

} EN_ADEC_ERR_CODE_E;



#define EI_ERR_ADEC_INVALID_DEVID     EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)

#define EI_ERR_ADEC_INVALID_CHNID     EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)

#define EI_ERR_ADEC_ILLEGAL_PARAM     EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)

#define EI_ERR_ADEC_EXIST             EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)

#define EI_ERR_ADEC_UNEXIST           EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)

#define EI_ERR_ADEC_NULL_PTR          EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)

#define EI_ERR_ADEC_NOT_CONFIG        EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)

#define EI_ERR_ADEC_NOT_SUPPORT       EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)

#define EI_ERR_ADEC_NOT_PERM          EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define EI_ERR_ADEC_NOMEM             EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)

#define EI_ERR_ADEC_NOBUF             EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)

#define EI_ERR_ADEC_BUF_EMPTY         EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)

#define EI_ERR_ADEC_BUF_FULL          EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)

#define EI_ERR_ADEC_SYS_NOTREADY      EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define EI_ERR_ADEC_DECODER_ERR       EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_DECODER_ERR)

#define EI_ERR_ADEC_BUF_LACK          EI_DEF_ERR(EI_ID_ADEC, EN_ERR_LEVEL_ERROR, ADEC_ERR_BUF_LACK)

#define EI_TRACE_ADEC(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_ADEC, level, fmt, ##args);\
}while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif/* End of #ifndef __EI_COMM_ADEC_H__*/

