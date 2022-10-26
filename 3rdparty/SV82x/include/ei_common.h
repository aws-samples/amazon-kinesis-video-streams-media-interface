/*
 *------------------------------------------------------------------------------
 * @File      :    ei_common.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#ifndef __EI_COMMON_H__
#define __EI_COMMON_H__

#include "ei_type.h"

#include "ei_defines.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define ATTRIBUTE __attribute__((aligned (ALIGN_NUM)))

#define VERSION_NAME_MAXLEN 64
typedef struct eiMDP_VERSION_S {
    EI_CHAR aVersion[VERSION_NAME_MAXLEN];
} MDP_VERSION_S;


/* For VO */
#define VO_MAX_VIRT_DEV_NUM            0
#define VO_MAX_PHY_DEV_NUM             1
#define VO_MAX_DEV_NUM                 1
#define VO_MAX_LAYER_NUM               4
#define VO_MAX_PRIORITY                4
#define VO_MAX_CHN_NUM                 8
#define VO_MAX_LAYER_IN_DEV            4
#define VO_MAX_GRAPHICS_LAYER_NUM      1
#define VC_MAX_CHN_NUM                 16
#define LANE_MAX_NUM                   4

/* For VGSS */
#define VGSS_MAX_DEV_NUM               4
#define VGSS_MAX_CHN_NUM               1

typedef EI_S32 AUDIO_DEV;
typedef EI_S32 AI_CHN;
typedef EI_S32 AO_CHN;
typedef EI_S32 AENC_CHN;
typedef EI_S32 ADEC_CHN;

typedef EI_S32 VO_DEV;
typedef EI_S32 VO_LAYER;
typedef EI_S32 VO_CHN;
typedef EI_S32 VO_WBC;
typedef EI_S32 GRAPHIC_LAYER;
typedef EI_S32 ISP_DEV;
typedef EI_S32 IPPU_DEV;
typedef EI_S32 IPPU_CHN;
typedef EI_S32 SENSOR_ID;
typedef EI_S32 AF_DRV_ID;
typedef EI_S32 VC_CHN;


typedef EI_S32 VISS_DEV;
typedef EI_S32 VISS_PIPE;
typedef EI_S32 VISS_CHN;
typedef EI_S32 VISS_STITCH_GRP;

typedef EI_S32 VGSS_DEV;
typedef EI_S32 VGSS_CHN;
typedef EI_S32 SAM_DEV;
typedef EI_S32 SAM_CHN;

typedef EI_S32 SPLT_DEV;
typedef EI_S32 SPLT_CHN;

#define EI_INVALID_CHN (-1)
#define EI_INVALID_LAYER (-1)
#define EI_INVALID_DEV (-1)
#define EI_INVALID_HANDLE (-1)
#define EI_INVALID_VALUE (-1)
#define EI_INVALID_TYPE (-1)

typedef enum eiMOD_ID_E {
    EI_ID_LOG   = 0,

    EI_ID_MBASE = 1,
    EI_ID_MLINK = 2,
    EI_ID_VBUF  = 3,
    EI_ID_ISP   = 4,
    EI_ID_VISS  = 5,
    EI_ID_DOSS  = 6,
    EI_ID_VPU   = 7,
    EI_ID_JVC   = 8,

    EI_ID_MIPI  = 9,
    EI_ID_GDC   = 10,

    EI_ID_AUDIOIN  = 11,
    EI_ID_AUDIOOUT = 12,
    EI_ID_AENC     = 13,
    EI_ID_ADEC     = 14,
    EI_ID_AUDIOCOMMON = 15,
    EI_ID_VGSS = 16,
    EI_ID_AXNU = 17,
    EI_ID_AXVU = 18,

    EI_ID_IPPU = 19,
    EI_ID_REGION,

    EI_ID_MFAKE,
    EI_ID_MFAKE_SRC,
    EI_ID_MFAKE_SINK,
    EI_ID_SAM,
    EI_ID_SAM_SRC,
    EI_ID_SAM_SINK,

    EI_ID_SPLT,

    EI_ID_MAX,
} MOD_ID_E;

typedef struct eiMDP_CHN_S {
    MOD_ID_E    enModId;
    EI_S32      s32DevId;
    EI_S32      s32ChnId;
} MDP_CHN_S;

#define MDP_MOD_LOG          "logmdp"
#define MDP_MOD_MBASE        "mbase"
#define MDP_MOD_MLINK        "mlink"
#define MDP_MOD_VBUF         "vbuf"
#define MDP_MOD_ISP          "isp"
#define MDP_MOD_IPPU         "ippu"
#define MDP_MOD_VISS         "viss"
#define MDP_MOD_VISS_MIPI    "viss_mipi"
#define MDP_MOD_AUDIOIN      "audioin"
#define MDP_MOD_AUDIOOUT     "audioout"
#define MDP_MOD_AUDIOCOMMON  "audiocommon"

#define MDP_MOD_DOSS         "doss"
#define MDP_MOD_AENC         "aenc"
#define MDP_MOD_ADEC         "adec"
#define MDP_MOD_VPU          "vpu"
#define MDP_MOD_JVC          "jvc"
#define MDP_MOD_GDC          "gdc"
#define MDP_MOD_VGSS         "vgss"
#define MDP_MOD_AXNU         "axnu"
#define MDP_MOD_AXVU         "axvu"
#define MDP_MOD_MFAKE        "mfake"
#define MDP_MOD_REGION       "region"
#define MDP_MOD_SAM          "sam"
#define MDP_MOD_SPLT         "splitter"

#define MDP_DEV_NUM          64
#define MDP_NAME_MAXLEN      32

#define MDP_LOG_MINOR        0
#define MDP_MBASE_MINOR      1
#define MDP_MLINK_MINOR      2
#define MDP_VBUF_MINOR       3
#define MDP_ISP_MINOR        4
#define MDP_VISS_MINOR       5
#define MDP_DOSS_MINOR       6
#define MDP_VPU_MINOR        7
#define MDP_JVC_MINOR        8
#define MDP_VISS_MIPI_MINOR  9

#define MDP_AUDIOIN_MINOR    10
#define MDP_AUDIOOUT_MINOR   11
#define MDP_AENC_MINOR       12
#define MDP_ADEC_MINOR       13
#define MDP_AUDIOCOM_MINOR   14

#define MDP_GDC_MINOR        15
#define MDP_VGSS_MINOR       16
#define MDP_MFAKE_MINOR      17
#define MDP_REGION_MINOR     18
#define MDP_SAM_MINOR        19
#define MDP_SPLT_MINOR       20

#define MDP_AXNU_MINOR       40
#define MDP_AXVU_MINOR       41

#define MDP_DYNAMIC_MINOR    255

#define MDP_VISS_MAX_DEV      3
#define MDP_SAM_MAX_DEV       4
#define MDP_SAM_MAX_CHN       8

#define MDP_SPLT_MAX_DEV      16
#define MDP_SPLT_MAX_CHN      8

typedef enum {
    PT_UNINIT = 0,

    PT_G729          = 18,
    PT_G711A         = 19,
    PT_G711U         = 20,
    PT_G726          = 21,
    PT_LPCM          = 23,
    PT_AAC           = 37,
    PT_MP3           = 43,
    PT_ADPCMA        = 49,

    PT_H264 = 100,
    PT_H265,
    PT_MJPEG,

    PT_JPEG = 200,

    PT_BUTT
} PAYLOAD_TYPE_E;
#define MDP_CHECK_MOD_ID(Id) (((EI_U32)Id) < EI_ID_MAX)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* _EI_COMMON_H_ */

