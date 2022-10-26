/*
 *------------------------------------------------------------------------------
 * @File      :    mi_isp_adv.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#ifndef __MI_ISP_ADV_H__
#define __MI_ISP_ADV_H__

#include "ei_comm_isp_adv.h"
#include "ei_comm_3a.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_ISP_ADV_GetMeter(ISP_DEV dev, ISP_PARAM_METER_S *pstMeter);
EI_S32 EI_MI_ISP_ADV_SetMeter(ISP_DEV dev, const ISP_PARAM_METER_S *pstMeter);
EI_S32 EI_MI_ISP_ADV_GetBlc(ISP_DEV dev, ISP_PARAM_BLC_S *pstBlc);
EI_S32 EI_MI_ISP_ADV_SetBlc(ISP_DEV dev, const ISP_PARAM_BLC_S *pstBlc);
EI_S32 EI_MI_ISP_ADV_GetStitch(ISP_DEV dev, ISP_PARAM_STITCH_S *pstStitch);
EI_S32 EI_MI_ISP_ADV_SetStitch(ISP_DEV dev, const ISP_PARAM_STITCH_S *pstStitch);
EI_S32 EI_MI_ISP_ADV_GetPfc(ISP_DEV dev, ISP_PARAM_PFC_S *pstPfc);
EI_S32 EI_MI_ISP_ADV_SetPfc(ISP_DEV dev, const ISP_PARAM_PFC_S *pstPfc);
EI_S32 EI_MI_ISP_ADV_GetCac(ISP_DEV dev, ISP_PARAM_CAC_S *pstCac);
EI_S32 EI_MI_ISP_ADV_SetCac(ISP_DEV dev, const ISP_PARAM_CAC_S *pstCac);
EI_S32 EI_MI_ISP_ADV_GetBayerNr(ISP_DEV dev, ISP_PARAM_BAYER_NR_S *pstBayerNr);
EI_S32 EI_MI_ISP_ADV_SetBayerNr(ISP_DEV dev, const ISP_PARAM_BAYER_NR_S *pstBayerNr);
EI_S32 EI_MI_ISP_ADV_GetTemper(ISP_DEV dev, ISP_PARAM_TEMPER_S *pstTemper);
EI_S32 EI_MI_ISP_ADV_SetTemper(ISP_DEV dev, const ISP_PARAM_TEMPER_S *pstTemper);
EI_S32 EI_MI_ISP_ADV_GetDemosaic(ISP_DEV dev, ISP_PARAM_DEMOSAIC_S *pstDemosaic);
EI_S32 EI_MI_ISP_ADV_SetDemosaic(ISP_DEV dev, const ISP_PARAM_DEMOSAIC_S *pstDemosaic);
EI_S32 EI_MI_ISP_ADV_GetDmscRgbIr(ISP_DEV dev, ISP_PARAM_DMSC_RGBIR_S *pstDmscRgbIr);
EI_S32 EI_MI_ISP_ADV_SetDmscRgbIr(ISP_DEV dev, const ISP_PARAM_DMSC_RGBIR_S *pstDmscRgbIr);
EI_S32 EI_MI_ISP_ADV_GetAntiFC(ISP_DEV dev, ISP_PARAM_ANTI_FC_S *pstAntiFC);
EI_S32 EI_MI_ISP_ADV_SetAntiFC(ISP_DEV dev, const ISP_PARAM_ANTI_FC_S *pstAntiFC);
EI_S32 EI_MI_ISP_ADV_GetDefog(ISP_DEV dev, ISP_PARAM_DEFOG_S *pstDefog);
EI_S32 EI_MI_ISP_ADV_SetDefog(ISP_DEV dev, const ISP_PARAM_DEFOG_S *pstDefog);
EI_S32 EI_MI_ISP_ADV_GetMd(ISP_DEV dev, ISP_PARAM_MD_S *pstMd);
EI_S32 EI_MI_ISP_ADV_SetMd(ISP_DEV dev, const ISP_PARAM_MD_S *pstMd);
EI_S32 EI_MI_ISP_ADV_GetDpc(ISP_DEV dev, ISP_PARAM_DPC_S *pstDpc);
EI_S32 EI_MI_ISP_ADV_SetDpc(ISP_DEV dev, const ISP_PARAM_DPC_S *pstDpc);
EI_S32 EI_MI_ISP_ADV_GetGe(ISP_DEV dev, ISP_PARAM_GE_S *pstGe);
EI_S32 EI_MI_ISP_ADV_SetGe(ISP_DEV dev, const ISP_PARAM_GE_S *pstGe);
EI_S32 EI_MI_ISP_ADV_GetCnr(ISP_DEV dev, ISP_PARAM_CNR_S *pstCnr);
EI_S32 EI_MI_ISP_ADV_SetCnr(ISP_DEV dev, const ISP_PARAM_CNR_S *pstCnr);
EI_S32 EI_MI_ISP_ADV_GetAec(ISP_DEV dev, ISP_PARAM_AEC_S *pstAec);
EI_S32 EI_MI_ISP_ADV_SetAec(ISP_DEV dev, const ISP_PARAM_AEC_S *pstAec);
EI_S32 EI_MI_ISP_ADV_GetAwb(ISP_DEV dev, ISP_PARAM_AWB_S *pstAwb);
EI_S32 EI_MI_ISP_ADV_SetAwb(ISP_DEV dev, const ISP_PARAM_AWB_S *pstAwb);
EI_S32 EI_MI_ISP_ADV_GetAf(ISP_DEV dev, ISP_PARAM_AF_S *pstAf);
EI_S32 EI_MI_ISP_ADV_SetAf(ISP_DEV dev, const ISP_PARAM_AF_S *pstAf);
EI_S32 EI_MI_ISP_ADV_GetIppu(ISP_DEV dev, ISP_PARAM_IPPU_S *pstIppu);
EI_S32 EI_MI_ISP_ADV_SetIppu(ISP_DEV dev, const ISP_PARAM_IPPU_S *pstIppu);
EI_S32 EI_MI_ISP_ADV_GetGamma(ISP_DEV dev, ISP_PARAM_GAMMA_S *pstGamma);
EI_S32 EI_MI_ISP_ADV_SetGamma(ISP_DEV dev, const ISP_PARAM_GAMMA_S *pstGamma);
EI_S32 EI_MI_ISP_ADV_GetSharpRgb(ISP_DEV dev, ISP_PARAM_SHARPEN_RGB_S *pstSharpRgb);
EI_S32 EI_MI_ISP_ADV_SetSharpRgb(ISP_DEV dev, const ISP_PARAM_SHARPEN_RGB_S *pstSharpRgb);
EI_S32 EI_MI_ISP_ADV_GetLtm(ISP_DEV dev, ISP_PARAM_LTM_S *pstLtm);
EI_S32 EI_MI_ISP_ADV_SetLtm(ISP_DEV dev, const ISP_PARAM_LTM_S *pstLtm);
EI_S32 EI_MI_ISP_ADV_GetCsc(ISP_DEV dev, ISP_PARAM_CSC_S *pstCsc);
EI_S32 EI_MI_ISP_ADV_SetCsc(ISP_DEV dev, const ISP_PARAM_CSC_S *pstCsc);
EI_S32 EI_MI_ISP_ADV_GetAlsc(ISP_DEV dev, ISP_PARAM_ALSC_S *pstAlsc);
EI_S32 EI_MI_ISP_ADV_SetAlsc(ISP_DEV dev, const ISP_PARAM_ALSC_S *pstAlsc);
EI_S32 EI_MI_ISP_ADV_GetRc(ISP_DEV dev, ISP_PARAM_RC_S *pstRc);
EI_S32 EI_MI_ISP_ADV_SetRc(ISP_DEV dev, const ISP_PARAM_RC_S *pstRc);
EI_S32 EI_MI_ISP_ADV_GetRcCfg(ISP_DEV dev, ISP_PARAM_RC_CFG_S *pstRcCfg);
EI_S32 EI_MI_ISP_ADV_SetRcCfg(ISP_DEV dev, const ISP_PARAM_RC_CFG_S *pstRcCfg);
EI_S32 EI_MI_ISP_ADV_GetTc(ISP_DEV dev, ISP_PARAM_TC_S *pstTc);
EI_S32 EI_MI_ISP_ADV_SetTc(ISP_DEV dev, const ISP_PARAM_TC_S *pstTc);
EI_S32 EI_MI_ISP_ADV_GetOd(ISP_DEV dev, ISP_PARAM_OD_S *pstOd);
EI_S32 EI_MI_ISP_ADV_SetOd(ISP_DEV dev, const ISP_PARAM_OD_S *pstOd);
EI_S32 EI_MI_ISP_ADV_GetIrd(ISP_DEV dev, ISP_PARAM_IRD_S *pstIrd);
EI_S32 EI_MI_ISP_ADV_SetIrd(ISP_DEV dev, const ISP_PARAM_IRD_S *pstIrd);
EI_S32 EI_MI_ISP_ADV_GetFace(ISP_DEV dev, ISP_PARAM_FACE_S *pstFace);
EI_S32 EI_MI_ISP_ADV_SetFace(ISP_DEV dev, const ISP_PARAM_FACE_S *pstFace);
EI_S32 EI_MI_ISP_ADV_GetCs(ISP_DEV dev, ISP_PARAM_CS_S *pstCs);
EI_S32 EI_MI_ISP_ADV_SetCs(ISP_DEV dev, const ISP_PARAM_CS_S *pstCs);
EI_S32 EI_MI_ISP_ADV_GetDa(ISP_DEV dev, ISP_PARAM_DA_S *pstDa);
EI_S32 EI_MI_ISP_ADV_SetDa(ISP_DEV dev, const ISP_PARAM_DA_S *pstDa);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_ISP_ADV_H__ */
