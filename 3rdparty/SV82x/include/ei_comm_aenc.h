/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_aenc.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#ifndef  __EI_COMM_AENC_H__
#define  __EI_COMM_AENC_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_debug.h"
#include "ei_comm_aio.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct eiAENC_ATTR_G711_S {
    EI_U32 resv;
} AENC_ATTR_G711_S;

typedef struct eiAENC_ATTR_G726_S {
    G726_BPS_E enG726bps;
} AENC_ATTR_G726_S;

typedef struct eiAENC_ATTR_ADPCM_S {
    ADPCM_TYPE_E enADPCMType;
} AENC_ATTR_ADPCM_S;

typedef struct eiAENC_ATTR_LPCM_S {
    EI_U32 resv;
} AENC_ATTR_LPCM_S;

typedef struct eiAENC_ATTR_AAC_S {
    EI_S32           enBitRate;
    AUDIO_SAMPLE_RATE_E enSmpRate;
    AUDIO_BIT_WIDTH_E   enBitWidth;
    AUDIO_SOUND_MODE_E  enSoundMode;

    EI_S16              s16BandWidth;
} AENC_ATTR_AAC_S;

typedef struct eiAENC_ENCODER_S {
    PAYLOAD_TYPE_E  enType;
    EI_U32          u32MaxFrmLen;
    EI_CHAR         aszName[17];
    EI_S32(*pfnOpenEncoder)(EI_VOID *pEncoderAttr,
        EI_VOID **ppEncoder);
    EI_S32(*pfnEncodeFrm)(EI_VOID *pEncoder, const AUDIO_FRAME_S *pstData,
        EI_U8 *pu8Outbuf, EI_U32 *pu32OutLen);
    EI_S32(*pfnCloseEncoder)(EI_VOID *pEncoder);
} AENC_ENCODER_S;

typedef struct eiAENC_CHN_ATTR_S {
    PAYLOAD_TYPE_E      enType;
    EI_U32              u32PtNumPerFrm;
    EI_U32              u32BufSize;
    EI_VOID ATTRIBUTE   *pValue;
    AUDIO_SAMPLE_RATE_E enSamplerate;
    AUDIO_SOUND_MODE_E  enSoundmode;
} AENC_CHN_ATTR_S;

typedef enum eiEN_AENC_ERR_CODE_E {
    AENC_ERR_ENCODER_ERR     = 64,
    AENC_ERR_VQE_ERR        = 65,

} EN_AENC_ERR_CODE_E;



#define EI_ERR_AENC_INVALID_DEVID     EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)

#define EI_ERR_AENC_INVALID_CHNID     EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)

#define EI_ERR_AENC_ILLEGAL_PARAM     EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)

#define EI_ERR_AENC_EXIST             EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)

#define EI_ERR_AENC_UNEXIST           EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)

#define EI_ERR_AENC_NULL_PTR          EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)

#define EI_ERR_AENC_NOT_CONFIG        EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)

#define EI_ERR_AENC_NOT_SUPPORT       EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)

#define EI_ERR_AENC_NOT_PERM          EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define EI_ERR_AENC_NOMEM             EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)

#define EI_ERR_AENC_NOBUF             EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)

#define EI_ERR_AENC_BUF_EMPTY         EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)

#define EI_ERR_AENC_BUF_FULL          EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)

#define EI_ERR_AENC_SYS_NOTREADY      EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)

#define EI_ERR_AENC_ENCODER_ERR       EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, AENC_ERR_ENCODER_ERR)

#define EI_ERR_AENC_VQE_ERR       EI_DEF_ERR(EI_ID_AENC, EN_ERR_LEVEL_ERROR, AENC_ERR_VQE_ERR)

#define EI_TRACE_AENC(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_AENC, level, fmt, ##args);\
}while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif/* End of #ifndef __EI_COMM_AENC_H__*/

