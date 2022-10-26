/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_video.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_VIDEO_H__
#define __EI_COMM_VIDEO_H__

#include "ei_type.h"
#include "ei_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define FRAME_MAX_PLANE 6
#define ABUF_POOL_NUM   24

typedef enum eiROTATION_E {
    ROTATION_0   = 0,
    ROTATION_90  = 1,
    ROTATION_180 = 2,
    ROTATION_270 = 3,
    ROTATION_MAX
} ROTATION_E;


typedef enum eiMIRRORDIRECTION_E {
    MIRDIRDIR_NONE      = 0,
    MIRDIRDIR_VER       = 1,
    MIRDIRDIR_HOR       = 2,
    MIRDIRDIR_HOR_VER   = 3,
    MIRDIRDIR_MAX
} MIRRORDIRECTION_E;

typedef struct eiBORDER_S {
    EI_U32 u32TopWidth;
    EI_U32 u32BottomWidth;
    EI_U32 u32LeftWidth;
    EI_U32 u32RightWidth;
    EI_U32 u32Color;
} BORDER_S;
typedef struct eiPOINT_S {
    EI_S32 s32X;
    EI_S32 s32Y;
} POINT_S;

typedef struct eiSIZE_S {
    EI_U32 u32Width;
    EI_U32 u32Height;
} SIZE_S;

typedef struct eiRECT_S {
    EI_S32 s32X;
    EI_S32 s32Y;
    EI_U32 u32Width;
    EI_U32 u32Height;
} RECT_S;

typedef struct eiAREA_CODE_S {
    RECT_S stRegion;
    EI_U32 u32OutWidth;
    EI_U32 u32OutHeight;
    EI_U32 u32Quality;
    EI_U32 u32AreaCodeInfo;
} AREA_CODE_S;

typedef struct eiCROP_INFO_S {
    EI_BOOL bEnable;
    RECT_S  stRect;
} CROP_INFO_S;

typedef struct eiFRAME_RATE_CTRL_S {
    EI_S32  s32SrcFrameRate;
    EI_S32  s32DstFrameRate;
} FRAME_RATE_CTRL_S;

typedef enum eiASPECT_RATIO_E {
    ASPECT_RATIO_NONE   = 0,
    ASPECT_RATIO_AUTO   = 1,
    ASPECT_RATIO_MANUAL = 2,
    ASPECT_RATIO_BUTT

} ASPECT_RATIO_E;
typedef struct eiASPECT_RATIO_S {
    ASPECT_RATIO_E enMode;
    EI_U32         u32BgColor;
    RECT_S         stVideoRect;

} ASPECT_RATIO_S;

typedef enum eiPIXEL_FORMAT_E {
    PIX_FMT_RGB_444 = 0x4,
    PIX_FMT_RGB_555,
    PIX_FMT_RGB_565,
    PIX_FMT_RGB_888,

    PIX_FMT_BGR_444,
    PIX_FMT_BGR_555,
    PIX_FMT_BGR_565,
    PIX_FMT_BGR_888,

    PIX_FMT_ARGB_1555,
    PIX_FMT_ARGB_4444,
    PIX_FMT_ARGB_8565,
    PIX_FMT_ARGB_8888,

    PIX_FMT_ABGR_1555,
    PIX_FMT_ABGR_4444,
    PIX_FMT_ABGR_8565,
    PIX_FMT_ABGR_8888,

    PIX_FMT_RGB_BAYER_8BITS = 0x20,
    PIX_FMT_RGB_BAYER_10BITS,
    PIX_FMT_RGB_BAYER_12BITS,
    PIX_FMT_RGB_BAYER_14BITS,
    PIX_FMT_RGB_BAYER_16BITS,
    PIX_FMT_RGB_BAYER_20BITS,

    PIX_FMT_RGB_BAYER_10BITS_CMP,
    PIX_FMT_RGB_BAYER_12BITS_CMP,
    PIX_FMT_RGB_BAYER_14BITS_CMP,

    PIX_FMT_YUV_SEMIPLANAR_422 = 0x30,
    PIX_FMT_YUV_SEMIPLANAR_420,
    PIX_FMT_YUV_SEMIPLANAR_444,

    PIX_FMT_YVU_SEMIPLANAR_422,
    PIX_FMT_YVU_SEMIPLANAR_420,
    PIX_FMT_YVU_SEMIPLANAR_444,

    PIX_FMT_YUV_PLANAR_422,
    PIX_FMT_YUV_PLANAR_420,
    PIX_FMT_YUV_PLANAR_444,

    PIX_FMT_YVU_PLANAR_422,
    PIX_FMT_YVU_PLANAR_420,
    PIX_FMT_YVU_PLANAR_444,

    PIX_FMT_YUYV_PACKAGE_422 = 0x50,
    PIX_FMT_YVYU_PACKAGE_422,
    PIX_FMT_UYVY_PACKAGE_422,
    PIX_FMT_VYUY_PACKAGE_422,
    PIX_FMT_YYUV_PACKAGE_422,
    PIX_FMT_YYVU_PACKAGE_422,
    PIX_FMT_UVYY_PACKAGE_422,
    PIX_FMT_VUYY_PACKAGE_422,

    PIX_FMT_GREY_Y8 = 0x60,
    PIX_FMT_GREY_Y1,
    PIX_FMT_GREY_Y2,
    PIX_FMT_GREY_Y4,
    PIX_FMT_GREY_Y6,
    PIX_FMT_GREY_Y10,
    PIX_FMT_GREY_Y12,
    PIX_FMT_GREY_Y16,
    PIX_FMT_GREY_Y16_BE,

    PIX_FMT_YUYV_I_2_YUV_SP_422 = 0x70,
    PIX_FMT_YVYU_I_2_YUV_SP_422,
    PIX_FMT_UYVY_I_2_YUV_SP_422,
    PIX_FMT_VYUY_I_2_YUV_SP_422,
    PIX_FMT_YYUV_I_2_YUV_SP_422,
    PIX_FMT_YYVU_I_2_YUV_SP_422,
    PIX_FMT_UVYY_I_2_YUV_SP_422,
    PIX_FMT_VUYY_I_2_YUV_SP_422,
    PIX_FMT_YUYV_I_2_YUV_P_422,
    PIX_FMT_YVYU_I_2_YUV_P_422,
    PIX_FMT_UYVY_I_2_YUV_P_422,
    PIX_FMT_VYUY_I_2_YUV_P_422,
    PIX_FMT_YYUV_I_2_YUV_P_422,
    PIX_FMT_YYVU_I_2_YUV_P_422,
    PIX_FMT_UVYY_I_2_YUV_P_422,
    PIX_FMT_VUYY_I_2_YUV_P_422,

    PIX_FMT_MAX
} PIXEL_FORMAT_E;


typedef enum eiVIDEO_FIELD_E {
    VIDEO_FIELD_TOP         = 0x1,
    VIDEO_FIELD_BOTTOM      = 0x2,
    VIDEO_FIELD_INTERLACED  = 0x3,
    VIDEO_FIELD_FRAME       = 0x4,

    VIDEO_FIELD_MAX
} VIDEO_FIELD_E;

typedef enum eiVIDEO_FORMAT_E {
    VIDEO_FORMAT_LINEAR = 0,
    VIDEO_FORMAT_MAX
} VIDEO_FORMAT_E;

typedef enum eiCOMPRESS_MODE_E {
    COMPRESS_MODE_NONE = 0,

    COMPRESS_MODE_LOSSY_75,     /* only support PIX_FMT_YUV_PLANAR_422, PIX_FMT_YUV_PLANAR_420,*/
    COMPRESS_MODE_LOSSY_50,     /* only support PIX_FMT_YUV_PLANAR_422, PIX_FMT_YUV_PLANAR_420,*/
    COMPRESS_MODE_LOSSY_37,     /* only support PIX_FMT_YUV_PLANAR_422, PIX_FMT_YUV_PLANAR_420,*/
    COMPRESS_MODE_LOSSY_25,     /* only support PIX_FMT_YUV_PLANAR_422, PIX_FMT_YUV_PLANAR_420,*/

    COMPRESS_MODE_LOSSLESS,

    COMPRESS_MODE_MAX
} COMPRESS_MODE_E;

typedef enum eiCOLOR_SPACE_E {
    COLOR_SPACE_BT601 = 0,
    COLOR_SPACE_BT601_LIMIT,
    COLOR_SPACE_BT709,
    COLOR_SPACE_BT709_LIMIT,
    COLOR_SPACE_USER,
    COLOR_SPACE_MAX
} COLOR_SPACE_E;

typedef enum eiDYNAMIC_RANGE_E {
    DYNAMIC_RANGE_SDR8 = 0,
    DYNAMIC_RANGE_HDR10,
    DYNAMIC_RANGE_MAX
} DYNAMIC_RANGE_E;

typedef struct eiVIDEO_FRAME_S {
    EI_U32              u32TotalSize;

    EI_U32              u32PlaneNum;
    EI_U32              u32PlaneStride[FRAME_MAX_PLANE];
    EI_U32              u32PlaneSize[FRAME_MAX_PLANE];
    EI_U32              u32PlaneSizeValid[FRAME_MAX_PLANE];

    EI_U64              u64PlanePhyAddr[FRAME_MAX_PLANE];
    EI_UL               ulPlaneVirAddr[FRAME_MAX_PLANE];

    EI_U32              u32TimeRef;
    EI_U64              u64PTS;

    EI_U32              u32PrivateData;
    EI_U32              u32FrameFlag;
    ROTATION_E          enRotation;
} VIDEO_FRAME_S;

typedef enum eiMDP_FRAME_TYPE_E {
    MDP_FRAME_TYPE_COMMON,
    MDP_FRAME_TYPE_VISS,
    MDP_FRAME_TYPE_ISP,
    MDP_FRAME_TYPE_IPPU,
    MDP_FRAME_TYPE_DOSS,
    MDP_FRAME_TYPE_VENC,
    MDP_FRAME_TYPE_AUDIO,
    MDP_FRAME_TYPE_VDEC,

    MDP_FRAME_TYPE_USER_DEF,

    MDP_FRAME_TYPE_MAX
} MDP_FRAME_TYPE_E;

typedef struct eiCOMMON_FRAME_INFO_S {
    EI_U32              u32Width;
    EI_U32              u32Height;
    EI_U32              u32Align;
    VIDEO_FIELD_E       enField;
    PIXEL_FORMAT_E      enPixelFormat;
    VIDEO_FORMAT_E      enVideoFormat;
    COMPRESS_MODE_E     enCompressMode;
    COLOR_SPACE_E       enColorSpace;
} COMMON_FRAME_INFO_S;

typedef struct eiVISS_GDC_ATTR_S {
    EI_BOOL         bEnable;
    EI_CHAR         acName[16];
    VISS_CHN        s32Chn;
    EI_U32          u32DownScale;
    EI_S64          as64PMFCoef[9];
    EI_U32          u32TblWidth;
    EI_U32          u32TblHeight;
    EI_U32          u32TblSize;
    EI_U32          *pu32TblAddr;
    SIZE_S          stChnSize;
} VISS_GDC_ATTR_S;

typedef struct eiVISS_FRAME_INFO_S {
    COMMON_FRAME_INFO_S  stCommFrameInfo;
} VISS_FRAME_INFO_S;

typedef struct eiISP_FRAME_INFO_S {
    COMMON_FRAME_INFO_S  stCommFrameInfo;
    EI_U32      u32AwbMode;
    EI_U32      u32ColorTempId;
    EI_U32      u32AeMode;
    EI_U32      u32FlickerMode;
    EI_U32      u32ISO;
    EI_U32      u32ExposureTime;
    EI_U32      u32IspDgain;
    EI_U32      u32Again;
    EI_U32      u32Dgain;
    EI_U32      au32Ratio[3];
    EI_S32      s32BV;
    EI_U32      u32FNumber;

    EI_U8       u32ObjLum;
    EI_U8       u32AvgLum;
    EI_U8       u32OrgTarget;
    EI_U8       u32FinalTarget;

    EI_U8       u8MotionLevel;
    EI_U8       u8MotionFlag;
    EI_U32      u32RcVal;
    EI_S32      s32IQpDelta;
    EI_S32      s32Temperature;
    EI_U16      u16IsoIdx;
    EI_U16      u16BvIdx;
    EI_VOID     *pData;
} ISP_FRAME_INFO_S;

typedef struct eiIPPU_FRAME_INFO_S {
    COMMON_FRAME_INFO_S  stCommFrameInfo;
} IPPU_FRAME_INFO_S;

typedef struct eiDOSS_FRAME_INFO_S {
    COMMON_FRAME_INFO_S  stCommFrameInfo;
} DOSS_FRAME_INFO_S;

typedef struct eiAREA_CODE_INFO_S {
    EI_U32           u32AreaNum;
    AREA_CODE_S      *pstAreaCode;
} AREA_CODE_INFO_S;

typedef struct eiVENC_FRAME_INFO_S {
    COMMON_FRAME_INFO_S  stCommFrameInfo;
    AREA_CODE_INFO_S     stAreaCodeInfo;
    EI_BOOL              bEnableScale;
} VENC_FRAME_INFO_S;

typedef struct eiVDEC_SCALE_INFO_S {
    EI_U32 u32PicWidth;
    EI_U32 u32PicHeight;
    EI_U32 u32outWidth;
    EI_U32 u32outHeight;
} VDEC_SCALE_INFO_S;

typedef struct eiVDEC_FRAME_INFO_S {
    COMMON_FRAME_INFO_S  stCommFrameInfo;
    VDEC_SCALE_INFO_S    stScaleInfo;
    EI_BOOL bEndOfStream;
} VDEC_FRAME_INFO_S;

typedef struct eiABUF_FRAME_INFO_S {
    EI_U64  u64PlanePhyAddr;
    EI_UL   ulPlaneVirAddr;
    EI_S32  s32Channels;
    EI_S32  s32SampleRate;
    EI_S32  s32BitsPeSample;
    EI_U32  u32TotalSize;
} ABUF_FRAME_INFO_S;

typedef struct eiVIDEO_FRAME_INFO_S {
    VIDEO_FRAME_S        stVFrame;
    MDP_FRAME_TYPE_E     enFrameType;
    EI_U32               u32Idx;
    MOD_ID_E             enModId;
    EI_UL                u32PoolId;
    EI_UL                u32BufferId;
    union {
        COMMON_FRAME_INFO_S  stCommFrameInfo;
        VISS_FRAME_INFO_S    stVissFrameInfo;
        ISP_FRAME_INFO_S     stIspFrameInfo;
        IPPU_FRAME_INFO_S    stIppuFrameInfo;
        DOSS_FRAME_INFO_S    stDossFrameInfo;
        VENC_FRAME_INFO_S    stVencFrameInfo;
        VDEC_FRAME_INFO_S    stVdecFrameInfo;
        ABUF_FRAME_INFO_S    stAudioFrameInfo;
        EI_U8                u8UserDef[128];
    };
} VIDEO_FRAME_INFO_S;

typedef struct eiVIDEO_FRAME_FMT_S {
    PIXEL_FORMAT_E      enPixelFormat;
    EI_CHAR             *cName;
    EI_U8                u8DataPlanes;
    EI_U8                u8WidthDepth[FRAME_MAX_PLANE];
    EI_U8                u8HeightDepth[FRAME_MAX_PLANE];
} VIDEO_FRAME_FMT_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

