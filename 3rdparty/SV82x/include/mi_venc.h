/*
 *------------------------------------------------------------------------------
 * @File      :    mi_venc.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_VENC_H__
#define __MI_VENC_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_venc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_VENC_CreateChn(VC_CHN VeChn, VENC_CHN_ATTR_S *pstAttr) ;
EI_S32 EI_MI_VENC_DestroyChn(VC_CHN VeChn);
EI_S32 EI_MI_VENC_ResetChn(VC_CHN VeChn);
EI_S32 EI_MI_VENC_StartRecvFrame(VC_CHN VeChn, VENC_RECV_PIC_PARAM_S *pstRecvParam);
EI_S32 EI_MI_VENC_StopRecvFrame(VC_CHN VeChn);
EI_S32 EI_MI_VENC_QueryStatus(VC_CHN VdChn, VENC_CHN_STATUS_S *pstStatus);
EI_S32 EI_MI_VENC_GetStream(VC_CHN VeChn, VENC_STREAM_S *pstStream, EI_S32 s32MilliSec);
EI_S32 EI_MI_VENC_ReleaseStream(VC_CHN VeChn, VENC_STREAM_S *pstStream);
EI_S32 EI_MI_VENC_GetStreamBufInfo(VC_CHN VeChn, VENC_STREAM_BUF_INFO_S *pstStreamBufInfo);
EI_S32 EI_MI_VENC_InsertUserData(VC_CHN VeChn, EI_U8 *pu8Data, EI_U32 u32Len);
EI_S32 EI_MI_VENC_SendFrame(VC_CHN VeChn, VIDEO_FRAME_INFO_S *pstFrame ,EI_S32 s32MilliSec);
EI_S32 EI_MI_VENC_SendFrameEx(VC_CHN VeChn, USER_FRAME_INFO_S *pstFrame, EI_S32 s32MilliSec);
EI_S32 EI_MI_VENC_RequestIDR(VC_CHN VeChn);
EI_S32 EI_MI_VENC_SetRoiAttr(VC_CHN VeChn, VENC_ROI_ATTR_S *pstRoiAttr);
EI_S32 EI_MI_VENC_GetRoiAttr(VC_CHN VeChn, VENC_ROI_ATTR_S *pstRoiAttr);
EI_S32 EI_MI_VENC_SetFrameRate(VC_CHN VeChn, FRAME_RATE_CTRL_S *pFrmRate);
EI_S32 EI_MI_VENC_GetFrameRate(VC_CHN VeChn, FRAME_RATE_CTRL_S *pFrmRate);
EI_S32 EI_MI_VENC_SetTargetBitrate(VC_CHN VeChn, VENC_BITRATE_CTRL_S *pTargetBitrate);
EI_S32 EI_MI_VENC_GetTargetBitrate(VC_CHN VeChn, VENC_BITRATE_CTRL_S *pTargetBitrate);
EI_S32 EI_MI_VENC_SetH264SliceSplit(VC_CHN VeChn, VENC_H264_SLICE_SPLIT_S *pstSliceSplit);
EI_S32 EI_MI_VENC_GetH264SliceSplit(VC_CHN VeChn, VENC_H264_SLICE_SPLIT_S *pstSliceSplit);
EI_S32 EI_MI_VENC_SetH264IntraParam(VC_CHN VeChn, VENC_H264_INTRA_PARAM_S *pstH264IntraParam);
EI_S32 EI_MI_VENC_GetH264IntraParam(VC_CHN VeChn, VENC_H264_INTRA_PARAM_S *pstH264IntraParam);
EI_S32 EI_MI_VENC_SetH264Entropy(VC_CHN VeChn, VENC_H264_ENTROPY_S *pstH264EntropyEnc);
EI_S32 EI_MI_VENC_GetH264Entropy(VC_CHN VeChn, VENC_H264_ENTROPY_S *pstH264EntropyEnc);
EI_S32 EI_MI_VENC_SetH264Dblk(VC_CHN VeChn, VENC_H264_DBLK_S *pstH264Dblk);
EI_S32 EI_MI_VENC_GetH264Dblk(VC_CHN VeChn, VENC_H264_DBLK_S *pstH264Dblk);
EI_S32 EI_MI_VENC_SetH264Vui(VC_CHN VeChn, VENC_H264_VUI_S *pstH264Vui);
EI_S32 EI_MI_VENC_GetH264Vui(VC_CHN VeChn, VENC_H264_VUI_S *pstH264Vui);
EI_S32 EI_MI_VENC_SetH265Vui(VC_CHN VeChn, VENC_H265_VUI_S *pstH265Vui);
EI_S32 EI_MI_VENC_GetH265Vui(VC_CHN VeChn, VENC_H265_VUI_S *pstH265Vui);
EI_S32 EI_MI_VENC_SetJpegEncodeMode(VC_CHN VeChn, VENC_JPEG_ENCODE_MODE_E enJpegEncodeMode);
EI_S32 EI_MI_VENC_GetJpegEncodeMode(VC_CHN VeChn, VENC_JPEG_ENCODE_MODE_E enJpegEncodeMode);
EI_S32 EI_MI_VENC_SetH265SliceSplit(VC_CHN VeChn, VENC_H265_SLICE_SPLIT_S *pstSliceSplit);
EI_S32 EI_MI_VENC_GetH265SliceSplit(VC_CHN VeChn, VENC_H265_SLICE_SPLIT_S *pstSliceSplit);
EI_S32 EI_MI_VENC_SetH265IntraParam(VC_CHN VeChn, VENC_H265_INTRA_PARAM_S *pstH265IntraParam);
EI_S32 EI_MI_VENC_GetH265IntraParam(VC_CHN VeChn, VENC_H265_INTRA_PARAM_S *pstH265IntraParam);
EI_S32 EI_MI_VENC_SetH265Dblk(VC_CHN VeChn, VENC_H265_DBLK_S *pstH265Dblk);
EI_S32 EI_MI_VENC_GetH265Dblk(VC_CHN VeChn, VENC_H265_DBLK_S *pstH265Dblk);
EI_S32 EI_MI_VENC_SetIntraRefresh(VC_CHN VeChn, VENC_INTRA_REFRESH_S *pstIntraRefresh);
EI_S32 EI_MI_VENC_GetIntraRefresh(VC_CHN VeChn, VENC_INTRA_REFRESH_S *pstIntraRefresh);
EI_S32 EI_MI_VENC_SetBgModeingParam(VC_CHN VeChn, VENC_BG_MODEING_S *pstBgModeing);
EI_S32 EI_MI_VENC_GetBgModeingParam(VC_CHN VeChn, VENC_BG_MODEING_S *pstBgModeing);
EI_S32 EI_MI_VENC_SetMDParam(VC_CHN VeChn, VENC_MD_S *pstMD);
EI_S32 EI_MI_VENC_GetMDParam(VC_CHN VeChn, VENC_MD_S *pstMD);
EI_S32 EI_MI_VENC_SetParamRcHvs(VC_CHN VeChn, VENC_PARAMRCHVS_S *pstParamRcHvs);
EI_S32 EI_MI_VENC_GetParamRcHvs(VC_CHN VeChn, VENC_PARAMRCHVS_S *pstParamRcHvs);
EI_S32 EI_MI_VENC_SetParamRcMinMaxQp(VC_CHN VeChn, VENC_RCMINMAXQP_S *pstRcMinMaxQp);
EI_S32 EI_MI_VENC_GetParamRcMinMaxQp(VC_CHN VeChn, VENC_RCMINMAXQP_S *pstRcMinMaxQp);
EI_S32 EI_MI_VENC_SetParamRcInterMinMaxQp(VC_CHN VeChn, VENC_RCINTERMINMAXQP_S *RcInterMinMaxQp);
EI_S32 EI_MI_VENC_GetParamRcInterMinMaxQp(VC_CHN VeChn, VENC_RCINTERMINMAXQP_S *RcInterMinMaxQp);
EI_S32 EI_MI_VENC_SetJpegParam(VC_CHN VeChn, VENC_JPEG_PARAM_S *pstJpegParam);
EI_S32 EI_MI_VENC_GetJpegParam(VC_CHN VeChn, VENC_JPEG_PARAM_S *pstJpegParam);
EI_S32 EI_MI_VENC_SetMjpegParam(VC_CHN VeChn, VENC_MJPEG_PARAM_S *pstMjpegParam);
EI_S32 EI_MI_VENC_GetMjpegParam(VC_CHN VeChn, VENC_MJPEG_PARAM_S *pstMjpegParam);
EI_S32 EI_MI_VENC_GetRcModeType(VC_CHN VeChn, VENC_RC_ATTR_S *pstRcModeType);
EI_S32 EI_MI_VENC_SetRcModeType(VC_CHN VeChn, VENC_RC_ATTR_S *pstRcModeType);
EI_S32 EI_MI_VENC_GetFd(VC_CHN VeChn, EI_S32 *pfd);
EI_S32 EI_MI_VENC_CloseFd(VC_CHN VeChn);
EI_S32 EI_MI_VENC_SetClkRate(VC_CHN VeChn, EI_U32 u32ClkRate);
EI_U32 EI_MI_VENC_GetClkRate(VC_CHN VeChn);
EI_S32 EI_MI_VENC_SetSuperFrameStratege(VC_CHN VeChn, VENC_SUPERFRAME_S *pstCfg);
EI_S32 EI_MI_VENC_GetSuperFrameStratege(VC_CHN VeChn, VENC_SUPERFRAME_S *pstCfg);
EI_S32 EI_MI_VENC_SetHoldDmaBuffers(VC_CHN VeChn, EI_BOOL bEnable);
EI_S32 EI_MI_VENC_ReleaseHoldDmaBuffers(VC_CHN VeChn);
EI_S32 EI_MI_VENC_SetMvReport(VC_CHN VeChn, EI_BOOL bEnable);
EI_S32 EI_MI_VENC_GetMvReportStatus(VC_CHN VeChn, EI_BOOL *pbEnable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_VISS_H__ */

