/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_rc.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __EI_COMM_RC_H__
#define __EI_COMM_RC_H__

typedef enum eiVENC_RC_MODE_E
{
	VENC_RC_MODE_H264CBR = 1,
	VENC_RC_MODE_H264VBR,
	VENC_RC_MODE_H264AVBR,
	VENC_RC_MODE_H264FIXQP,

	VENC_RC_MODE_MJPEGCBR = 10,
	VENC_RC_MODE_MJPEGVBR,
	VENC_RC_MODE_MJPEGFIXQUALLITY,

	VENC_RC_MODE_H265CBR = 20,
	VENC_RC_MODE_H265VBR,
	VENC_RC_MODE_H265AVBR,
	VENC_RC_MODE_H265FIXQP,

	VENC_RC_MODE_BUTT,

} VENC_RC_MODE_E;


typedef struct eiVENC_H264_CBR_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32BitRate;
	EI_U32               u32CuLevelEnable;
} VENC_H264_CBR_S;

typedef struct eiVENC_H264_VBR_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32BitRate;
	EI_U32               u32Delay;
	EI_U32               u32CuLevelEnable;
	EI_U32               u32MaxIQp;
	EI_U32               u32MinIQp;
	EI_U32               u32MaxPQp;
	EI_U32               u32MinPQp;
} VENC_H264_VBR_S;

typedef struct eiVENC_H264_AVBR_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32BitRate;
	EI_U32               u32BitRateRation;
	EI_U32               u32Delay;
	EI_U32               u32CuLevelEnable;
} VENC_H264_AVBR_S;


typedef struct eiVENC_H264_FIXQP_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32FixIQp;
	EI_U32               u32FixPQp;
} VENC_H264_FIXQP_S;

typedef struct eiVENC_MJPEG_CBR_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32BitRate;
} VENC_MJPEG_CBR_S;


typedef struct eiVENC_MJPEG_VBR_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32BitRate;
	EI_U32               u32MinQuallity;
} VENC_MJPEG_VBR_S;


typedef struct VENC_MJPEG_FIXQUALLITY_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32Quallity;
} VENC_MJPEG_FIXQUALLITY_S;


typedef struct eiVENC_H265_CBR_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32BitRate;
	EI_U32               u32CuLevelEnable;
} VENC_H265_CBR_S;


typedef struct eiVENC_H265_VBR_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32BitRate;
	EI_U32               u32Delay;
	EI_U32               u32CuLevelEnable;
	EI_U32               u32MaxIQp;
	EI_U32               u32MinIQp;
	EI_U32               u32MaxPQp;
	EI_U32               u32MinPQp;
} VENC_H265_VBR_S;


typedef struct eiVENC_H265_AVBR_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32BitRate;
	EI_U32               u32BitRateRation;
	EI_U32               u32Delay;
	EI_U32               u32CuLevelEnable;
} VENC_H265_AVBR_S;


typedef struct eiVENC_H265_FIXQP_S
{
	EI_U32               u32SrcFrameRate;
	EI_U32               u32DstFrameRate;
	EI_U32               u32FixIQp;
	EI_U32               u32FixPQp;
} VENC_H265_FIXQP_S;

typedef struct eiVENC_RC_ATTR_S
{
	VENC_RC_MODE_E	enRcMode;
	EI_BOOL			bTimeLapse;
	union
	{
		VENC_H264_CBR_S    stH264Cbr;
		VENC_H264_VBR_S    stH264Vbr;
		VENC_H264_AVBR_S   stH264Avbr;
		VENC_H264_FIXQP_S  stH264FixQp;

		VENC_MJPEG_CBR_S         stMjpegCbr;
		VENC_MJPEG_VBR_S         stMjpegVbr;
		VENC_MJPEG_FIXQUALLITY_S stMjpegFixQuallity;

		VENC_H265_CBR_S    stH265Cbr;
		VENC_H265_VBR_S    stH265Vbr;
		VENC_H265_AVBR_S   stH265Avbr;
		VENC_H265_FIXQP_S  stH265FixQp;
	};
} VENC_RC_ATTR_S;


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_MLINK_H__ */

