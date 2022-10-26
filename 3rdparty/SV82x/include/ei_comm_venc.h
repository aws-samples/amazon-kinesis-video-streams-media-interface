/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_venc.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_VENC_H__
#define __EI_COMM_VENC_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_debug.h"
#include "ei_comm_rc.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#define MAX_ROI_REGION 8


#ifndef EI_ID_VENC
	#define EI_ID_VENC  EI_ID_VPU
#endif


#define EI_TRACE_VENC(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_VENC, level, fmt, ##args);\
} while(0)

#define EI_TRACE_VPU(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_VPU, level, fmt, ##args);\
} while(0)




#define EI_ERR_VENC_NULL_PTR         EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define EI_ERR_VENC_NOTREADY         EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define EI_ERR_VENC_NOT_PERM         EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define EI_ERR_VENC_NOMEM            EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define EI_ERR_VENC_NOBUF            EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define EI_ERR_VENC_ILLEGAL_PARAM    EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define EI_ERR_VENC_BUSY             EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define EI_ERR_VENC_NOT_SUPPORT      EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define EI_ERR_VENC_SIZE_NOT_ENOUGH  EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)
#define EI_ERR_VENC_SYS_NOTREADY     EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define EI_ERR_VENC_UNEXIST          EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define EI_ERR_VENC_EXIST            EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define EI_ERR_VENC_INVALID_CHNID    EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define EI_ERR_VENC_OTHERS           EI_DEF_ERR(EI_ID_VENC, EN_ERR_LEVEL_ERROR, EN_ERR_OTHERS)



#define VENC_ERRNO_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define VENC_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
        if (cond) { \
                EI_TRACE_VENC(EI_DBG_ERR, "errno[%d]!\n", errno);\
                return (ret);\
        } \
} while(0)

typedef enum eiVENC_GOP_TYPE_E
{
	VENC_GOPTYPE_TYPICAL = 1,
	VENC_GOPTYPE_IPP,
	VENC_GOPTYPE_IPPPP,
} VENC_GOP_TYPE_E;

typedef enum eiVENC_GOP_MODE_E
{
	VENC_GOPMODE_NORMAL_P = 1,
	VENC_GOPMODE_DUAL_P,
	VENC_GOPMODE_DUAL_SVC_P,
	VENC_GOPMODE_ADV_P,
} VENC_GOP_MODE_E;

typedef struct eiVENC_GOPMODE_NORMALP_S
{
	VENC_GOP_TYPE_E       enGopType;
	EI_S32                s32IPQpDelta[8];
	EI_U32                u32IdrPeriod;
} VENC_GOPMODE_NORMALP_S;

typedef struct eiVENC_GOPMODE_DUALP_S
{
	VENC_GOP_TYPE_E       enGopType;
	EI_S32                s32IPQpDelta[8];
	EI_U32                u32IdrPeriod;
} VENC_GOPMODE_DUALP_S;

typedef struct eiVENC_GOPMODE_DUALP_SVC_S
{
	VENC_GOP_TYPE_E       enGopType;
	EI_S32                s32IPQpDelta[8];
	EI_U32                u32IdrPeriod;
	EI_U32                u32Levle;
} VENC_GOPMODE_DUALP_SVC_S;

typedef struct eiVENC_GOPMODE_ADVP_S
{
	VENC_GOP_TYPE_E       enGopType;
	EI_S32                s32IPQpDelta[8];
	EI_U32                u32IdrPeriod;
	EI_U32                u32BgInterval;
} VENC_GOPMODE_ADVP_S;

typedef struct eiVENC_GOP_ATTR_S
{
	VENC_GOP_MODE_E enGopMode;
	union
	{
		VENC_GOPMODE_NORMALP_S     stNormalP;
		VENC_GOPMODE_DUALP_S       stDualP;
		VENC_GOPMODE_DUALP_SVC_S   stDualSvcP;
		VENC_GOPMODE_ADVP_S        stAdvP;
	};
} VENC_GOP_ATTR_S;

typedef enum eiVENC_HEVC_PROFILE_E
{
	HEVCPROFILEMAIN = 1,
	HEVCPROFILESTILLPICTURE = 3,
} VENC_HEVC_PROFILE_E;

typedef enum eiVENC_H264_PROFILE_E
{
	H264PROFILEBP = 1,
	H264PROFILEMP = 2,
	H264PROFILEEXTENDED = 3,
	H264PROFILEHP = 4,
} VENC_H264_PROFILE_E;

typedef struct eiVENC_ATTR_H264_S
{
	VENC_H264_PROFILE_E    u32Profile;
	ROTATION_E             enRotation;
	MIRRORDIRECTION_E      enMirrorDirection;
	RECT_S                 stCropRect;
	EI_U32                 u32ReportEnable;
} VENC_ATTR_H264_S;

typedef struct eiVENC_ATTR_H265_S
{
	VENC_HEVC_PROFILE_E    u32Profile;
	ROTATION_E             enRotation;
	MIRRORDIRECTION_E      enMirrorDirection;
	RECT_S                 stCropRect;
	EI_U32                 u32ReportEnable;
} VENC_ATTR_H265_S;

typedef struct eiVENC_ATTR_MJPEG_S
{
    EI_U32          u32PicStride;
	RECT_S          stCropRect;
	EI_U32          u32OutWidth;
	EI_U32          u32OutHeight;
} VENC_ATTR_MJPEG_S;

typedef struct eiVENC_MPF_CFG_S
{
	EI_U8 u8LargeThumbNailNum;
	SIZE_S astLargeThumbNailSize[2];
} VENC_MPF_CFG_S;

typedef struct eiVENC_ATTR_JPEG_S
{
	EI_BOOL bSupportDCF;
	VENC_MPF_CFG_S stMPFCfg;
	EI_U32 u32Quality;
	EI_BOOL bEnableScale;
	EI_U32 u32OutWidth;
	EI_U32 u32OutHeight;
} VENC_ATTR_JPEG_S;

typedef struct eiVENC_ATTR_S
{
	PAYLOAD_TYPE_E           enType;
	PIXEL_FORMAT_E           enInputFormat;
	EI_U32                   u32BufSize;
	EI_U32                   u32PicWidth;
	EI_U32                   u32PicHeight;
	COMPRESS_MODE_E			 enCompressMode;
	union
	{
		VENC_ATTR_H264_S    stAttrH264e;
		VENC_ATTR_H265_S    stAttrH265e;
		VENC_ATTR_MJPEG_S   stAttrMjpege;
		VENC_ATTR_JPEG_S    stAttrJpege;
	};
} VENC_ATTR_S;

typedef struct eiVENC_CHN_ATTR_S
{
	VENC_ATTR_S        stVencAttr;
	VENC_RC_ATTR_S     stRcAttr;
	VENC_GOP_ATTR_S    stGopAttr;
} VENC_CHN_ATTR_S;

typedef struct eiVENC_RECV_PIC_PARAM_S
{
	EI_S32             s32RecvPicNum;
} VENC_RECV_PIC_PARAM_S;

typedef struct eiVENC_CHN_STATUS_S
{
	EI_U32             u32LeftPics;
	EI_U32             u32LeftStreamBytes;
	EI_U32             u32LeftStreamFrames;
	EI_U32             u32CurPacks;
	EI_U32             u32LeftRecvPics;
	EI_U32             u32LeftEncPics;
} VENC_CHN_STATUS_S;

typedef enum eiVENC_DATA_TYPE_E
{
	DATA_TYPE_INVALID = 0,
	DATA_TYPE_INITPACKET = 1,
	DATA_TYPE_I_FRAME,
	DATA_TYPE_P_FRAME,
	DATA_TYPE_B_FRAME,
} VENC_DATA_TYPE_E;

typedef enum eiJPEGE_PACK_TYPE_E
{
	JPEG_PACK_INVALID = 0,
	JPEG_GE_PACK_PIC,
	//reserved
} JPEGE_PACK_TYPE_E;

typedef struct eiVENC_MV_DATA_S
{
	EI_S16 x;
	EI_S16 y;
} VENC_MV_DATA_S;

typedef struct eiVENC_MV_S
{
	VENC_MV_DATA_S *pstMvData;
	EI_U32 u32MvDataLen;
	EI_U32 u32Width;
	EI_U32 u32Height;
} VENC_MV_S;

typedef struct eiVENC_PACK_S
{
	EI_U64             u64PhyAddr[2];
	EI_U8              *pu8Addr[2];
	EI_U32             u32Len[2];

	EI_U64             u64PTS;
	EI_BOOL            bFrameEnd;

	VENC_DATA_TYPE_E   enDataType;

	VENC_MV_S		   stMv;
} VENC_PACK_S;

typedef enum eiVENC_PACK_TYPE_E
{
	PACKET_TYPE_H265 = 1,
	PACKET_TYPE_H264,
	PACKET_TYPE_JPEG,
	PACKET_TYPE_MJPEG,
} VENC_PACK_TYPE_E;

typedef struct eiVENC_STREAM_INFO_H264_S
{
	//reserved
} VENC_STREAM_INFO_H264_S;

typedef struct eiVENC_STREAM_INFO_H265_S
{
	//reserved
} VENC_STREAM_INFO_H265_S;


typedef struct eiVENC_STREAM_INFO_JPEG_S
{
	EI_U32 u32PicBytesNum;
	EI_U32 u32UpdateAttrCnt;
	EI_S32 u32AreaCodeInfo;
} VENC_STREAM_INFO_JPEG_S;

typedef struct eiVENC_STREAM_INFO_MJPEG_S
{
	//reserved
} VENC_STREAM_INFO_MJPEG_S;


typedef struct eiVENC_STREAM_S
{
	VENC_PACK_TYPE_E  PackType;
	VENC_PACK_S       pstPack;
	union
	{
		VENC_STREAM_INFO_H264_S   stH264Info;
		VENC_STREAM_INFO_JPEG_S   stJpegInfo;
		VENC_STREAM_INFO_H265_S   stH265Info;
		VENC_STREAM_INFO_MJPEG_S  stMjpegInfo;
	};
} VENC_STREAM_S;


typedef struct eiVENC_STREAM_BUF_INFO_S
{
	EI_U64        u64PhyAddr;
	EI_VOID       *pUserAddr;
	EI_U32        u32BufSize;
} VENC_STREAM_BUF_INFO_S;


typedef enum eiVENC_MAP_TYPE_E
{
	MAP_TYPE_QP = 0x1,
} VENC_MAP_TYPE_E;
typedef struct eiUSER_RC_INFO_S
{
    VENC_MAP_TYPE_E    enMaptype;
    EI_U64              u64QpMapPhyAddr;
    EI_U8              *pu8QpMapVirAddr;
    EI_U32             u32QpMapSize;
} USER_RC_INFO_S;

typedef struct eiUSER_FRAME_INFO_S
{
	VIDEO_FRAME_INFO_S stUserFrame;
	USER_RC_INFO_S 	   stUserRcInfo;
} USER_FRAME_INFO_S;


typedef struct eiVENC_ROI_ATTR_S
{
	EI_U32  u32RoiNum;
	EI_U32  u32QpOffset[MAX_ROI_REGION];
	RECT_S  stRect[MAX_ROI_REGION];
    EI_U32  u32AvgQp;
} VENC_ROI_ATTR_S;

typedef struct eiVENC_BITRATE_CTRL_S
{
	EI_U32  u32TargetBitrate;
} VENC_BITRATE_CTRL_S;

typedef struct eiVENC_H264_SLICE_SPLIT_S
{
	EI_BOOL bSplitEnable;
	EI_U32  u32MbLineNum;
} VENC_H264_SLICE_SPLIT_S;


typedef struct eiVENC_H264_INTRA_PARAM_S
{
	EI_U32	   intraQP;
	EI_U32     intraPeriod;
	EI_U32     avcIdrPeriod;
	EI_U32	   forcedIdrHeaderEnable;
} VENC_H264_INTRA_PARAM_S;

typedef struct eiVENC_H264_ENTROPY_S
{
	EI_BOOL bCabacEnable;
} VENC_H264_ENTROPY_S;

typedef struct eiVENC_H264_DBLK_S
{
	EI_BOOL    blfCrossSliceBoundaryEnable;
	EI_U32     u32DisableDeblockingFilterIdc;
	EI_S32     s32AlphaOffsetDiv2;
	EI_S32     s32BetaOffsetDiv2;
} VENC_H264_DBLK_S;

typedef struct eiVENC_VUI_ASPECT_RATIO_S
{
	EI_U8      u8AspectRatioInfoPresentFlag;
	EI_U8      u8AspectRatioIdc;
	EI_U8      u8OverscanInfoPresentFlag;
	EI_U8      u8OverscanAppropriateFlag;
	EI_U16     u16SarWidth;
	EI_U16     u16SarHeight;
} VENC_VUI_ASPECT_RATIO_S;


typedef struct hiVENC_VUI_VIDEO_SIGNAL_S
{
	EI_U8      u8VideoSignalTypePresentFlag;
	EI_U8      u8VideoFormat;
	EI_U8      u8VideoFullRangeFlag;
	EI_U8      u8ColourDescriptionPresentFlag;
	EI_U8      u8ColourPrimaries;
	EI_U8      u8TransferCharacteristics;
	EI_U8      u8MatrixCoefficients;
} VENC_VUI_VIDEO_SIGNAL_S;


typedef struct eiVENC_VUI_BITSTREAM_RESTRIC_S
{
	EI_U8      u8BitstreamRestrictionFlag ;
} VENC_VUI_BITSTREAM_RESTRIC_S;


typedef struct eiVENC_H264_VUI_TIME_INFO_S
{
	EI_U32     u32TimingInfoPresentFlag;
	EI_U32     u32FixedFrameRateFlag;
	EI_U32     u32NumUnitsInTick;
	EI_U32     u32TimeScale;
} VENC_VUI_H264_TIME_INFO_S;


typedef struct eiVENC_VUI_H265_TIME_INFO_S
{
	EI_U32     u32TimingInfoPresentFlag;
	EI_U32     u32NumUnitsInTick;
	EI_U32     u32TimeScale;
	EI_U32     u32NumTicksPocDiffOneMinus1;
} VENC_VUI_H265_TIME_INFO_S;


typedef struct eiVENC_H264_VUI_S
{
	VENC_VUI_ASPECT_RATIO_S             stVuiAspectRatio;
	VENC_VUI_H264_TIME_INFO_S           stVuiTimeInfo;
	VENC_VUI_VIDEO_SIGNAL_S             stVuiVideoSignal;
} VENC_H264_VUI_S;


typedef struct eiVENC_H265_VUI_S
{
	VENC_VUI_ASPECT_RATIO_S       stVuiAspectRatio;
	VENC_VUI_H265_TIME_INFO_S     stVuiTimeInfo;
	VENC_VUI_VIDEO_SIGNAL_S       stVuiVideoSignal;
} VENC_H265_VUI_S;

typedef struct eiVENC_JPEG_PARAM_S
{
	EI_U32 u32Quality;
	EI_U8 u8LumQt[64];
	EI_U8 u8ChromQt[64];
	EI_U8 u8EnQtTable;
	EI_U8 bEnableAreaCode;
} VENC_JPEG_PARAM_S;

typedef struct eiVENC_MJPEG_PARAM_S
{
	EI_U32 u32Quality;
	EI_U8 u8LumQt[64];
	EI_U8 u8ChromQt[64];
} VENC_MJPEG_PARAM_S;


typedef enum eiVENC_JPEG_ENCODE_MODE_E
{
	JPEG_ENCODE_ALL   = 0,
	JPEG_ENCODE_SNAP  = 1,
	JPEG_ENCODE_BUTT,
} VENC_JPEG_ENCODE_MODE_E;

typedef enum eiVENC_H265_SLICE_MODE_E
{
	H265_SINGLE_SLICE_MODE        = 0,
	H265_DEPENDING_SLICE_MODE     = 0x1,
	H265_INDEPENDING_SLICE_MODE   = 0x2,
} VENC_H265_SLICE_MODE_E;

typedef enum eiVENC_DEPENDING_SLICE_MODE_E
{
	H265_DEPENDING_SLICE_SINGLE_MODE = 0x0,
	H265_DEPENDING_SLICE_CTUS_MODE,
	H265_DEPENDING_SLICE_BYTES_MODE,
} VENC_DEPENDING_SLICE_MODE_E;

typedef struct eiVENC_DEPENDING_SLICE_MODE_S
{
    VENC_DEPENDING_SLICE_MODE_E    enMode;
	union
	{
		EI_U32 u32CtusLineNum;
		EI_U32 u32BytesLineNum;
	};
} VENC_DEPENDING_SLICE_MODE_S;


typedef enum eiVENC_INDEPENDING_SLICE_MODE_E
{
	H265_INDEPENDING_SLICE_SINGLE_MODE = 0x0,
	H265_INDEPENDING_SLICE_CTUS_MODE,
} VENC_INDEPENDING_SLICE_MODE_E;

typedef struct eiVENC_INDEPENDING_SLICE_MODE_S
{
	VENC_INDEPENDING_SLICE_MODE_E    enMode;
	EI_U32 u32CtusLineNum;
} VENC_INDEPENDING_SLICE_MODE_S;


typedef struct eiVENC_H265_SLICE_SPLIT_S
{
    VENC_H265_SLICE_MODE_E           enSliceMode;
    VENC_DEPENDING_SLICE_MODE_S      DepenSlicemode;
    VENC_INDEPENDING_SLICE_MODE_S    IndependingSlicemode;
} VENC_H265_SLICE_SPLIT_S;


typedef struct eiVENC_H265_INTRA_PRED_S
{
	EI_U32	 u32ConstrainedIntraPredFlag;
} VENC_H265_INTRA_PRED_S;

typedef struct eiVENC_H265_INTRA_PARAM_S
{
	EI_U32	 intraQP;
	EI_U32	 forcedIdrHeaderEnable;
	EI_U32   intraPeriod;
} VENC_H265_INTRA_PARAM_S;

typedef struct eiVENC_H265_DBLK_S
{
	EI_BOOL    blfCrossSliceBoundaryEnable;
	EI_S32     s32SliceDeblockingFilterDisabledFlag;
	EI_S32     s32SliceBetaOffsetDiv2;
	EI_S32     s32SliceTcOffsetDiv2;
} VENC_H265_DBLK_S;


typedef enum eiVENC_INTRA_REFRESH_MODE_E
{
	INTRA_REFRESH_MODE_ROW = 1,
	INTRA_REFRESH_MODE_COLUMN,
	INTRA_REFRESH_MODE_STEP_SIZE,
	INTRA_REFRESH_MODE_AIR,
} VENC_INTRA_REFRESH_MODE_E;


typedef struct eiVENC_INTRA_REFRESH_S
{
	EI_BOOL                    bRefreshEnable;
	VENC_INTRA_REFRESH_MODE_E  RefreshMode;
	EI_U32                     u32RefreshArg;
} VENC_INTRA_REFRESH_S;


typedef struct eiVENC_BG_MODEING_S
{
	EI_U32                  u32BgThrDiff;
	EI_U32                  u32BgThrMeanDiff;
	EI_U32                  u32BgLambdaQp;
	EI_U32                  u32BgDeltaQp;
	EI_U32	                u32S2fmeDisable;
} VENC_BG_MODEING_S;


typedef struct eiVENC_MD_S
{
	EI_U32                  u32Pu04DeltaRate;
	EI_U32                  u32Pu04IntraPlanarDeltaRate;
	EI_U32                  u32Pu04IntraDcDeltaRate;
	EI_U32                  u32Pu04IntraAngleDeltaRate;

	EI_U32                  u32Pu08DeltaRate;
	EI_U32                  u32Pu08IntraPlanarDeltaRate;
	EI_U32                  u32Pu08IntraDcDeltaRate;
	EI_U32                  u32Pu08IntraAngleDeltaRate;

	EI_U32                  u32Pu16DeltaRate;
	EI_U32                  u32Pu16IntraPlanarDeltaRate;
	EI_U32                  u32Pu16IntraDcDeltaRate;
	EI_U32                  u32Pu16IntraAngleDeltaRate;

	EI_U32                  u32Pu32DeltaRate;
	EI_U32                  u32Pu32IntraPlanarDeltaRate;
	EI_U32                  u32Pu32IntraDcDeltaRate;
	EI_U32                  u32Pu32IntraAngleDeltaRate;

	EI_U32                  u32Cu08IntraDeltaRate;
	EI_U32                  u32Cu08InterDeltaRate;
	EI_U32                  u32Cu08MergeDeltaRate;

	EI_U32                  u32Cu16IntraDeltaRate;
	EI_U32                  u32Cu16InterDeltaRate;
	EI_U32                  u32Cu16MergeDeltaRate;

	EI_U32                  u32Cu32IntraDeltaRate;
	EI_U32                  u32Cu32InterDeltaRate;
	EI_U32                  u32Cu32MergeDeltaRate;
} VENC_MD_S;


typedef struct eiVENC_CHORMAQPOFFSET_S
{
	EI_U32                  u32CbQpOffset;
	EI_U32                  u32CrQpOffset;
} VENC_CHORMAQPOFFSET_S;


typedef struct eiVENC_PARAMRCHVS_S
{
	EI_U32                  u32HvsQPEnable;
	EI_U32                  u32HvsQpScale;
	EI_U32                  u32VbvBufferSize;
} VENC_PARAMRCHVS_S;


typedef struct eiVENC_RCMINMAXQP_S
{
	EI_U32                  u32MinQpI;
	EI_U32                  u32MaxQpI;
	EI_U32                  u32HvsMaxDeltaQp;
} VENC_RCMINMAXQP_S;


typedef struct eiVENC_RCINTERMINMAXQP_S
{
	EI_U32                  u32MaxQpP;
	EI_U32                  u32MinQpP;
	EI_U32                  u32MaxQpB;
	EI_U32                  u32MinQpB;
} VENC_RCINTERMINMAXQP_S;


typedef enum eiVENC_SUPERFRAME_MODE_E
{
	SUPERFRM_NONE=0,
	SUPERFRM_DISCARD,
	SUPERFRM_REENCODE
}VENC_SUPERFRAME_MODE_E;

typedef struct eiVENC_SUPERFRAME_S
{
	VENC_SUPERFRAME_MODE_E enMode;
	EI_U32 u32IFrmBitsThr;
	EI_U32 u32PFrmBitsThr;
} VENC_SUPERFRAME_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_VENC_H__ */

