
/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __SAMPLE_COMM_H__
#define __SAMPLE_COMM_H__

#include <pthread.h>
#include <strings.h>

#include "ei_common.h"
#include "ei_comm_mbase.h"
#include "ei_comm_mlink.h"
#include "ei_comm_mfake.h"
#include "ei_comm_vbuf.h"
#include "ei_comm_isp.h"
#include "ei_comm_ippu.h"
#include "ei_comm_adec.h"
#include "ei_comm_aenc.h"
#include "ei_comm_aio.h"
#include "ei_comm_venc.h"
#include "ei_comm_vdec.h"
#include "ei_comm_doss.h"
#include "ei_comm_panel.h"
#include "ei_comm_camera.h"
#include "ei_comm_region.h"

#include "ei_defines.h"

#include "mi_mbase.h"
#include "mi_mlink.h"
#include "mi_mfake.h"
#include "mi_vbuf.h"
#include "mi_isp.h"
#include "mi_isp_adv.h"
#include "mi_ippu.h"
#include "mi_ae.h"
#include "mi_af.h"
#include "mi_af_customer.h"
#include "mi_awb.h"
#include "mi_viss.h"
#include "mi_doss.h"
#include "mi_region.h"
#include "mi_venc.h"
#include "mi_adec.h"
#include "mi_aenc.h"
#include "mi_audioin.h"
#include "mi_audioout.h"
#include "mi_audiopcm.h"
#include "mi_audiomix.h"
#include "ei_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/****************************** BEGIN VISS **********************************/

#define MAX_COMM_POOLS (16)

#define FILE_NAME_LEN               128

#define SAMPLE_AUDIO_EXTERN_AI_DEV 0
#define SAMPLE_AUDIO_EXTERN_AO_DEV 0
#define SAMPLE_AUDIO_INNER_AI_DEV 0
#define SAMPLE_AUDIO_INNER_AO_DEV 0
#define SAMPLE_AUDIO_INNER_HDMI_AO_DEV 1

#define SAMPLE_AUDIO_PTNUMPERFRM   480


/*******************************************************
    enum define
*******************************************************/
typedef enum eiSAMPLE_RC_E {
    SAMPLE_RC_CBR = 0,
    SAMPLE_RC_VBR,
    SAMPLE_RC_AVBR,
    SAMPLE_RC_QPMAP,
    SAMPLE_RC_FIXQP
} SAMPLE_RC_E;

/*******************************************************
    structure define
*******************************************************/

typedef struct eiSAMPLE_VENC_PARA_S {
    EI_BOOL          bThreadStart;
    VC_CHN           VeChn[VC_MAX_CHN_NUM];
    VENC_CHN_ATTR_S  VeChnAttr[VC_MAX_CHN_NUM];
    EI_S32           s32Cnt;
    pthread_t        gs_VencPid;
} SAMPLE_VENC_PARA_S;

typedef struct eiSAMPLE_VENC_QPMAP_PARA_S {
    EI_BOOL          bThreadStart;
    IPPU_DEV         IppuDev[ISP_MAX_DEV_NUM];
    IPPU_CHN         IppuChn[IPPU_PHY_CHN_MAX_NUM];
    VC_CHN           VeChn[VC_MAX_CHN_NUM];
    VENC_CHN_ATTR_S  VeChnAttr[VC_MAX_CHN_NUM];
    EI_S32           s32Cnt;
    pthread_t        gs_VencQpMapPid;
} SAMPLE_VENC_QPMAP_PARA_S;

/****************************** BEGIN VISS **********************************/
typedef struct eiSAMPLE_SENSOR_INFO_S {
    SNS_TYPE_E   enSnsType;

    VISS_DEV_TYPE_E           enDevType;
} SAMPLE_SENSOR_INFO_S;

typedef struct eiSAMPLE_EXTDEV_INFO_S {
    EXT_DEV_TYPE_E   enType;
} SAMPLE_EXTDEV_INFO_S;

typedef struct eiSAMPLE_DEV_INFO_S {
    VISS_DEV    VissDev;
    WDR_MODE_E  enWDRMode;
    VISS_CHN                aBindPhyChn[VISS_DEV_MAX_CHN_NUM];
    VISS_OUT_PATH_E         enOutPath;    /* 0: DMA 1:ISP */
} SAMPLE_DEV_INFO_S;

typedef struct eiSAMPLE_ISP_INFO_S {
    ISP_DEV             IspDev;
    ISP_RUNNING_MODE_E  enRunningMode;
    FRAME_RATE_CTRL_S   stFrc;
    RECT_S              stCrop;
    EI_CHAR             *pcParamName;
    EI_U32              u32IspClk;
    EI_U32              u32IppuClk;
} SAMPLE_ISP_INFO_S;

typedef struct eiSAMPLE_CHN_INFO_S {
    VISS_CHN            aVissChn[VISS_MAX_CHN_NUM];
    EI_U32    enWorkMode;
    PIXEL_FORMAT_E      enPixFormat;
    VIDEO_FORMAT_E      enVideoFormat;
    COMPRESS_MODE_E     enCompressMode;
    VISS_CHN_ATTR_S       stChnAttr;
    VISS_EXT_CHN_ATTR_S       stExtChnAttr;
    VISS_VC_CHN_ATTR_S       stVcChnAttr;
} SAMPLE_CHN_INFO_S;

typedef struct eiSAMPLE_VISS_INFO_S {
    SAMPLE_SENSOR_INFO_S    stSnsInfo;
    SAMPLE_DEV_INFO_S       stDevInfo;
    SAMPLE_ISP_INFO_S       stIspInfo;
    SAMPLE_CHN_INFO_S       stChnInfo;
    SAMPLE_EXTDEV_INFO_S    stExtDevInfo;
} SAMPLE_VISS_INFO_S;

typedef struct eiSAMPLE_VISS_CONFIG_S {
    SAMPLE_VISS_INFO_S  astVissInfo[VISS_MAX_DEV_NUM];
    EI_S32              as32WorkingVissId[VISS_MAX_DEV_NUM];
    EI_S32              s32WorkingVissNum;
} SAMPLE_VISS_CONFIG_S;
/****************************** END VISS **********************************/

typedef struct eiSAMPLE_AB_POOL_CONFIG_S {
    EI_S32  s32Channels;
    EI_S32  s32SampleRate;
    EI_S32  s32BitsPerSample;
    VBUF_POOL Pool;
    VBUF_POOL_CONFIG_S stPoolCfg;
} SAMPLE_AB_POOL_CONFIG_S;

typedef struct eiSAMPLE_AB_CONFIG_S {
    EI_U32 u32PoolCnt;
    SAMPLE_AB_POOL_CONFIG_S astCommPool[MAX_COMM_POOLS];
} SAMPLE_AB_CONFIG_S;

typedef struct eiSAMPLE_VENC_CONFIG_S {
    EI_U32 u32width;
    EI_U32 u32height;
    EI_U32 u32widthOut;
    EI_U32 u32heightOut;
    PIXEL_FORMAT_E enInputFormat;
    EI_U32 u32bitrate;
    EI_U32 u32IdrPeriod;
    EI_U32 u32srcframerate;
    EI_U32 u32dstframerate;
    EI_BOOL bTimeLapse;
} SAMPLE_VENC_CONFIG_S;

typedef struct eiSAMPLE_VDEC_SCALE_INFO_S {
    EI_U32              u32Align;
    EI_U32              u32MaxWidth;
    EI_U32              u32MaxHeight;
    EI_BOOL             bFixedRatio;
    EI_BOOL             bScaleEnable;
} SAMPLE_VDEC_SCALE_INFO;

typedef enum eiTHREAD_CONTRL_E {
    THREAD_CTRL_START,
    THREAD_CTRL_PAUSE,
    THREAD_CTRL_STOP,
} THREAD_CONTRL_E;

typedef struct eiVDEC_THREAD_PARAM_S {
    EI_S32 s32ChnId;
    PAYLOAD_TYPE_E enType;
    EI_CHAR cFilePath[128];
    EI_CHAR cFileName[128];
    EI_CHAR cUserFilePath[128];
    EI_CHAR cUserFileName[128];
    EI_S32 s32StreamMode;
    EI_S32 s32MilliSec;
    EI_S32 s32MinBufSize;
    THREAD_CONTRL_E eThreadCtrl;
    EI_U32 frame_cnt;
    EI_S32 s32IntervalTime;
    EI_U64  u64PtsInit;
    EI_U64  u64PtsIncrease;
    EI_BOOL bCircleSend;
} SAMPLE_VDEC_THREAD_PARAM_S;

typedef struct eiSAMPLE_VDEC_CONFIG_S {
    PAYLOAD_TYPE_E  enType;
    MOD_ID_E        enModId;
    VIDEO_MODE_E    enMode;
    EI_U32          u32Align;
    EI_U32          u32Width;
    EI_U32          u32Height;
    PIXEL_FORMAT_E  enPixelFormat;
    EI_U32          u32FrameBufCnt;
    VBUF_POOL       Pool;
    SAMPLE_VDEC_SCALE_INFO scaleInfo;
    // union
    // {
    //     SAMPLE_VDEC_VIDEO_ATTR stSapmleVdecVideo;      /* structure with video ( h265/h264) */
    //     SAMPLE_VDEC_PICTURE_ATTR stSapmleVdecPicture; /* structure with picture (jpeg/mjpeg )*/
    // };
} SAMPLE_VDEC_CONFIG_S;

typedef struct eiSAMPLE_VO_WBC_CONFIG_S {
    VO_WBC_SOURCE_TYPE_E    enSourceType;
    DYNAMIC_RANGE_E         enDynamicRange;
    COMPRESS_MODE_E         enCompressMode;
    EI_S32                  s32Depth;

    EI_S32                  VoWbc;
    VO_WBC_ATTR_S           stWbcAttr;
    VO_WBC_SOURCE_S         stWbcSource;
    VO_WBC_MODE_E           enWbcMode;
} SAMPLE_VO_WBC_CONFIG_S;

typedef enum eiSAMPLE_VO_MODE_E {
    VO_MODE_1MUX,
    VO_MODE_2MUX,
    VO_MODE_4MUX,
    VO_MODE_8MUX,
    VO_MODE_9MUX,
    VO_MODE_16MUX,
    VO_MODE_25MUX,
    VO_MODE_36MUX,
    VO_MODE_49MUX,
    VO_MODE_64MUX,
    VO_MODE_2X4,
    VO_MODE_BUTT
} SAMPLE_VO_MODE_E;

typedef enum eiPIC_SIZE_E {
    PIC_CIF,
    PIC_360P,      /* 640 * 360 */
    PIC_D1_PAL,    /* 720 * 576 */
    PIC_D1_NTSC,   /* 720 * 480 */
    PIC_720P,      /* 1280 * 720  */
    PIC_1080P,     /* 1920 * 1080 */
    PIC_2592x1520,
    PIC_2592x1536,
    PIC_2592x1944,
    PIC_2688x1536,
    PIC_2716x1524,
    PIC_3840x2160,
    PIC_4096x2160,
    PIC_3000x3000,
    PIC_4000x3000,
    PIC_7680x4320,
    PIC_3840x8640,
    PIC_BUTT
} PIC_SIZE_E;

typedef struct eiSAMPLE_COMM_VO_LAYER_CONFIG_S {
    PANEL_TYPE_E            enPanelType;

    /* for layer */
    VO_LAYER                VoLayer;
    RECT_S                  stDispRect;
    SIZE_S                  stImageSize;
    PIXEL_FORMAT_E          enPixFormat;
    EI_U32                  u32Align;
    EI_U32                  u32DisBufLen;
    VO_PART_MODE_E          enVoPartMode;

    /* for chn */
    SAMPLE_VO_MODE_E        enVoMode;
} SAMPLE_COMM_VO_LAYER_CONFIG_S;

typedef struct eiSAMPLE_VO_CONFIG_S {
    PANEL_TYPE_E            enPanelType;

    /* for device */
    VO_DEV                  VoDev;

    /* for layer */
    VO_LAYER                VoLayer;
    PIXEL_FORMAT_E          enPixFormat;
    EI_U32                  u32Align;
    RECT_S                  stDispRect;
    SIZE_S                  stImageSize;
    VO_PART_MODE_E          enVoPartMode;
    EI_U32                  u32DisBufLen;

    /* for chnnel */
    SAMPLE_VO_MODE_E        enVoMode;
} SAMPLE_VO_CONFIG_S;

EI_U32 SAMPLE_COMM_SYS_Init(VBUF_CONFIG_S *pstVbufConfig);
EI_U32 SAMPLE_COMM_SYS_Exit(VBUF_CONFIG_S *pstVbufConfig);
EI_S32 SAMPLE_COMM_IPPU_Link_VPU(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VC_CHN VcChn);
EI_S32 SAMPLE_COMM_IPPU_Link_JVC(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VC_CHN VcChn);
EI_S32 SAMPLE_COMM_IPPU_UnLink_VPU(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VC_CHN VcChn);
EI_S32 SAMPLE_COMM_IPPU_UnLink_JVC(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VC_CHN VcChn);
EI_S32 SAMPLE_COMM_VISS_Link_VPU(VISS_DEV VissDev, VISS_CHN VissChn, VC_CHN VcChn);
EI_S32 SAMPLE_COMM_VISS_UnLink_VPU(VISS_DEV VissDev, VISS_CHN VissChn, VC_CHN VcChn);
EI_S32 SAMPLE_COMM_VISS_Link_JVC(VISS_DEV VissDev, VISS_CHN VissChn, VC_CHN VcChn);
EI_S32 SAMPLE_COMM_VISS_UnLink_JVC(VISS_DEV VissDev, VISS_CHN VissChn, VC_CHN VcChn);
EI_S32 SAMPLE_COMM_VISS_Link_DOSS(VISS_DEV VissDev, VISS_CHN VissChn, VO_LAYER VoLayer, VO_CHN VoChn);
EI_S32 SAMPLE_COMM_VISS_UnLink_DOSS(VISS_DEV VissDev, VISS_CHN VissChn, VO_LAYER VoLayer, VO_CHN VoChn);
EI_S32 SAMPLE_COMM_IPPU_Link_DOSS(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VO_LAYER VoLayer, VO_CHN VoChn);
EI_S32 SAMPLE_COMM_IPPU_UnLink_DOSS(IPPU_DEV IppuDev, IPPU_CHN IppuChn, VO_LAYER VoLayer, VO_CHN VoChn);
EI_S32 SAMPLE_COMM_DOSS_Link_DOSS(VO_LAYER SrcVoLayer, VO_CHN SrcVoChn, VO_LAYER DstVoLayer, VO_CHN DstVoChn);
EI_S32 SAMPLE_COMM_DOSS_UnLink_DOSS(VO_LAYER DstVoLayer, VO_CHN DstVoChn);

EI_S32 SAMPLE_COMM_ISP_Start(SAMPLE_VISS_INFO_S *pstVissInfo);
EI_S32 SAMPLE_COMM_ISP_StartNoSensor(ISP_DEV dev, ISP_PUB_ATTR_S *pstPubAttr);
EI_S32 SAMPLE_COMM_ISP_Stop(ISP_DEV IspDev);
EI_S32 SAMPLE_COMM_ISP_Run(ISP_DEV IspDev);

EI_S32 SAMPLE_COMM_IPPU_Init(SAMPLE_VISS_INFO_S *pstVissInfo);
EI_S32 SAMPLE_COMM_IPPU_InitNoSensor(IPPU_DEV dev, ISP_PUB_ATTR_S *pstPubAttr);
EI_S32 SAMPLE_COMM_IPPU_Exit(IPPU_DEV dev);

EI_S32 SAMPLE_COMM_IPPU_Start(IPPU_DEV dev, EI_BOOL *pabChnEnable,
    IPPU_DEV_ATTR_S *pstIppuDevAttr, IPPU_CHN_ATTR_S *pastIppuChnAttr);
EI_S32 SAMPLE_COMM_IPPU_Stop(IPPU_DEV dev, EI_BOOL *pabChnEnable);

EI_S32 SAMPLE_COMM_VISS_GetDevAttrBySns(SNS_TYPE_E enSnsType, VISS_DEV_ATTR_S *pstDevAttr);
EI_S32 SAMPLE_COMM_VISS_GetChnAttrBySns(SNS_TYPE_E enSnsType, VISS_CHN_ATTR_S *pstChnAttr);
EI_S32 SAMPLE_COMM_VISS_GetPicTypeBySensor(SNS_TYPE_E enSnsType, VISS_PIC_TYPE_E *pstType);
EI_S32 SAMPLE_COMM_VISS_SensorRegiterCallback(VISS_DEV VisspDev, SNS_TYPE_E enSnsType);
EI_S32 SAMPLE_COMM_VISS_SensorUnRegiterCallback(VISS_DEV VisspDev);
EI_S32 SAMPLE_COMM_VISS_StartDev(SAMPLE_VISS_INFO_S *pstVissInfo);
EI_S32 SAMPLE_COMM_VISS_StopDev(SAMPLE_VISS_INFO_S *pstVissInfo);
EI_S32 SAMPLE_COMM_VISS_StartVissChn(SAMPLE_VISS_INFO_S *pstVissInfo);
EI_S32 SAMPLE_COMM_VISS_StopVissChn(SAMPLE_VISS_INFO_S *pstVissInfo);
EI_S32 SAMPLE_COMM_VISS_CreateViss(SAMPLE_VISS_CONFIG_S *pstVissConfig);
EI_S32 SAMPLE_COMM_VISS_DestroyViss(SAMPLE_VISS_CONFIG_S *pstVissConfig);
EI_S32 SAMPLE_COMM_VISS_StartViss(SAMPLE_VISS_CONFIG_S *pstVissConfig);
EI_S32 SAMPLE_COMM_VISS_StopViss(SAMPLE_VISS_CONFIG_S *pstVissConfig);
EI_VOID SAMPLE_COMM_VISS_GetSensorInfo(SAMPLE_VISS_CONFIG_S *pstVissConfig);
CAMERA_OBJ_S *SAMPLE_COMM_VISS_GetSnsObj(SNS_TYPE_E enSnsType);
EXT_DEV_OBJ_S* SAMPLE_COMM_VISS_GetExtDevObj(EXT_DEV_TYPE_E enType);
EI_S32 SAMPLE_COMM_VISS_DetectSnstype(VISS_DEV VissDev, SNS_TYPE_E enSnsType);

EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAencFile(AENC_CHN AeChn, ADEC_CHN AdChn, FILE *pAecFd);
EI_S32 SAMPLE_COMM_AUDIO_CreatTrdFileAenc(AENC_CHN AdChn, FILE *pAdcFd);

EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdFileAenc(AENC_CHN AdChn);
EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAencFile(AENC_CHN AdChn);

EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAiAenc(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn);
EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAdeAo(ADEC_CHN AdChn, AUDIO_DEV AoDev, AO_CHN AoChn, FILE *pAdcFd);
EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAdecFile(ADEC_CHN AdChn, FILE *pAdcFd);
EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAencAdec(AENC_CHN AeChn, ADEC_CHN AdChn, FILE *pAecFd);
EI_S32 SAMPLE_COMM_AUDIO_CreatTrdFileAdec(ADEC_CHN AdChn, FILE *pAdcFd);
EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAoVolCtrl(AUDIO_DEV AoDev);

EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAi(AUDIO_DEV AiDev, AI_CHN AiChn);
EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdFileAdec(ADEC_CHN AdChn);
EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAdec(ADEC_CHN AdChn);
EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAoVolCtrl(AUDIO_DEV AoDev);

EI_S32 SAMPLE_COMM_AUDIO_AoLinkAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn);
EI_S32 SAMPLE_COMM_AUDIO_AoUnbindAdec(AUDIO_DEV AoDev, AO_CHN AoChn, ADEC_CHN AdChn);
EI_S32 SAMPLE_COMM_AUDIO_AencLinkAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn);
EI_S32 SAMPLE_COMM_AUDIO_AencUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AENC_CHN AeChn);
EI_S32 SAMPLE_COMM_AUDIO_StartAi(AUDIO_DEV AiDevId, EI_S32 s32AiChnCnt,
    AIO_ATTR_S *pstAioAttr, AUDIO_SAMPLE_RATE_E enOutSampleRate, EI_BOOL bResampleEn,
    EI_VOID *pstAiVqeAttr);
EI_S32 SAMPLE_COMM_AUDIO_CreatTrdAiAo(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn);
EI_S32 SAMPLE_COMM_AUDIO_AoLinkAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn);
EI_S32 SAMPLE_COMM_AUDIO_AoUnbindAi(AUDIO_DEV AiDev, AI_CHN AiChn, AUDIO_DEV AoDev, AO_CHN AoChn);
EI_S32 SAMPLE_COMM_AUDIO_StopAi(AUDIO_DEV AiDevId, EI_S32 s32AiChnCnt, EI_BOOL bResampleEn, EI_BOOL bVqeEn);
EI_S32 SAMPLE_COMM_AUDIO_StartAenc(EI_S32 s32AencChnCnt, AIO_ATTR_S *pstAioAttr, PAYLOAD_TYPE_E enType);
EI_S32 SAMPLE_COMM_AUDIO_StopAenc(EI_S32 s32AencChnCnt);
EI_S32 SAMPLE_COMM_AUDIO_StartAdec(ADEC_CHN AdChn, AIO_ATTR_S *pstAioAttr, PAYLOAD_TYPE_E enType);
EI_S32 SAMPLE_COMM_AUDIO_StopAdec(ADEC_CHN AdChn);
EI_S32 SAMPLE_COMM_AUDIO_DestroyTrdAencAdec(AENC_CHN AeChn);

EI_S32 SAMPLE_COMM_AUDIO_DestroyAllTrd(void);

EI_S32 SAMPLE_COMM_AUDIO_StartAo(AUDIO_DEV AoDevId, EI_S32 s32AoChnCnt,
    AIO_ATTR_S *pstAioAttr, AUDIO_SAMPLE_RATE_E enInSampleRate, EI_BOOL bResampleEn);
EI_S32 SAMPLE_COMM_AUDIO_StopAo(AUDIO_DEV AoDevId, EI_S32 s32AoChnCnt, EI_BOOL bResampleEn);

EI_S32 SAMPLE_COMM_VENC_SnapStart(VC_CHN VencChn, PAYLOAD_TYPE_E enType, SAMPLE_VENC_CONFIG_S *pstVencConfig);
EI_S32 SAMPLE_COMM_VENC_Start(VC_CHN VencChn, PAYLOAD_TYPE_E enType, SAMPLE_RC_E RcMode,
    SAMPLE_VENC_CONFIG_S *pstVencConfig, COMPRESS_MODE_E enCompressMode, VENC_GOP_MODE_E GopMode);
EI_S32  SAMPLE_COMM_VENC_Creat(VC_CHN VencChn, PAYLOAD_TYPE_E enType,
    SAMPLE_RC_E RcMode, SAMPLE_VENC_CONFIG_S *pstVencConfig,
    COMPRESS_MODE_E enCompressMode, VENC_GOP_MODE_E GopMode);
EI_S32 SAMPLE_COMM_VENC_StartGetStream(VC_CHN VeChn[], EI_S32 s32Cn);
EI_S32 SAMPLE_COMM_VENC_StartPollStream(VC_CHN VeChn[], EI_S32 s32Cn);
EI_S32 SAMPLE_COMM_VENC_StopGetStream(void);
EI_S32 SAMPLE_COMM_VENC_Stop(VC_CHN VencChn);
EI_S32 SAMPLE_COMM_VENC_QpmapSendFrame(IPPU_DEV IppuDev[], IPPU_CHN IppuChn[], VC_CHN VencChn[], EI_S32 s32Cn);
EI_VOID SAMPLE_COMM_VENC_StopSendQpmapFrame();

EI_U32 SAMPLE_COMM_VDEC_SCALE_Init(SAMPLE_VDEC_CONFIG_S *pstSamplVdec);
EI_S32 SAMPLE_COMM_VDEC_Start(VC_CHN s32ChnNum, SAMPLE_VDEC_CONFIG_S *pstSamplVdec);
EI_S32 SAMPLE_COMM_VDEC_StartSendStream(EI_S32 s32ChnNum, SAMPLE_VDEC_THREAD_PARAM_S *pstVdecSend,
    pthread_t *pVdecThread);
EI_S32 SAMPLE_COMM_VDEC_StartGetFrame(EI_S32 s32ChnNum, SAMPLE_VDEC_THREAD_PARAM_S *pstVdecSend,
    pthread_t *pVdecThread);
EI_S32 SAMPLE_COMM_VDEC_StopSendStream(EI_S32 s32ChnNum, SAMPLE_VDEC_THREAD_PARAM_S *pstVdecSend,
    pthread_t *pVdecThread);

PANEL_OBJ_S *SAMPLE_COMM_DOSS_GetPanelObj(PANEL_TYPE_E enPanelType);
EI_S32 SAMPLE_COMM_DOSS_GetPanelAttr(PANEL_TYPE_E enPanelType, PANEL_TYPE_ATTR_S *pstPanelAttr);
EI_S32 SAMPLE_COMM_DOSS_GetWH(PANEL_TYPE_E enPanelType, VO_INTF_SYNC_E enIntfSync,
    EI_U32 *pu32W, EI_U32 *pu32H, EI_U32 *pu32Frm);
EI_S32 SAMPLE_COMM_DOSS_StartDev(VO_DEV VoDev, PANEL_TYPE_ATTR_S *pstPanelAttr);
EI_S32 SAMPLE_COMM_DOSS_StopDev(VO_DEV VoDev);
EI_S32 SAMPLE_COMM_DOSS_StartLayer(VO_LAYER VoLayer, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);
EI_S32 SAMPLE_COMM_DOSS_StopLayer(VO_LAYER VoLayer);
EI_S32 SAMPLE_COMM_DOSS_StartChn(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode);
EI_S32 SAMPLE_COMM_DOSS_StopChn(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode);
EI_S32 SAMPLE_COMM_DOSS_GetDefConfig(SAMPLE_VO_CONFIG_S *pstVoConfig);
EI_S32 SAMPLE_COMM_DOSS_StopVO(SAMPLE_VO_CONFIG_S *pstVoConfig);
EI_S32 SAMPLE_COMM_DOSS_StartVO(SAMPLE_VO_CONFIG_S *pstVoConfig);
EI_S32 SAMPLE_COMM_DOSS_GetDefLayerConfig(SAMPLE_COMM_VO_LAYER_CONFIG_S *pstVoLayerConfig);
EI_S32 SAMPLE_COMM_DOSS_StartLayerChn(SAMPLE_COMM_VO_LAYER_CONFIG_S *pstVoLayerConfig);
EI_S32 SAMPLE_COMM_DOSS_StopLayerChn(SAMPLE_COMM_VO_LAYER_CONFIG_S *pstVoLayerConfig);
EI_VOID SAMPLE_COMM_DOSS_Exit(EI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SAMPLE_COMMON_H__ */
