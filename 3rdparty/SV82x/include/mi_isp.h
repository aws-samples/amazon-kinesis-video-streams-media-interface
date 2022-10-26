/*
 *------------------------------------------------------------------------------
 * @File      :    mi_isp.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */


#ifndef __MI_ISP_H__
#define __MI_ISP_H__

#include "ei_comm_isp.h"
#include "ei_comm_3a.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

EI_S32 EI_MI_ISP_MemInit(ISP_DEV dev);
EI_S32 EI_MI_ISP_MemExit(ISP_DEV dev);
EI_S32 EI_MI_ISP_Init(ISP_DEV dev);
EI_S32 EI_MI_ISP_Exit(ISP_DEV dev);
EI_S32 EI_MI_ISP_Run(ISP_DEV dev);
EI_S32 EI_MI_ISP_RegRestore(ISP_DEV dev);
EI_S32 EI_MI_ISP_GetFrameDone(ISP_DEV dev, EI_U32 u32Timeout);

EI_S32 EI_MI_ISP_AELibRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAeLib, ISP_AE_REGISTER_S *pstRegister);
EI_S32 EI_MI_ISP_AWBLibRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAwbLib, ISP_AWB_REGISTER_S *pstRegister);
EI_S32 EI_MI_ISP_AFLibRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAfLib, ISP_AF_REGISTER_S *pstRegister);

EI_S32 EI_MI_ISP_AELibUnRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAeLib);
EI_S32 EI_MI_ISP_AWBLibUnRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAwbLib);
EI_S32 EI_MI_ISP_AFLibUnRegCallBack(ISP_DEV dev, ISP_3ALIB_S *pstAfLib);


EI_S32 EI_MI_ISP_SetClkFreq(ISP_DEV dev, const EI_U32 u32IspClk, const EI_U32 u32IppuClk);
EI_S32 EI_MI_ISP_GetClkFreq(ISP_DEV dev, EI_U32 *pu32IspClk, EI_U32 *pu32IppuClk);

EI_S32 EI_MI_ISP_SetPubAttr(ISP_DEV dev, const ISP_PUB_ATTR_S *pstPubAttr);
EI_S32 EI_MI_ISP_GetPubAttr(ISP_DEV dev, ISP_PUB_ATTR_S *pstPubAttr);

EI_S32 EI_MI_ISP_SetFrameInfo(ISP_DEV dev, const ISP_FRAME_INFO_S *pstIspFrame);
EI_S32 EI_MI_ISP_GetFrameInfo(ISP_DEV dev, ISP_FRAME_INFO_S *pstIspFrame);

EI_S32 EI_MI_ISP_GetResetStatus(ISP_DEV dev, EI_U32 *pu32Status);
EI_S32 EI_MI_ISP_SetResetExit(ISP_DEV dev, EI_U32 u32Status);

EI_S32 EI_MI_ISP_SendFrame(ISP_DEV dev, VIDEO_FRAME_INFO_S *pstFrame);

EI_S32 EI_MI_ISP_SetModuleBypass(ISP_DEV dev, const ISP_MODULE_BYPASS_S *pstModule);
EI_S32 EI_MI_ISP_GetModuleBypass(ISP_DEV dev, ISP_MODULE_BYPASS_S *pstModule);

EI_S32 EI_MI_ISP_SetEffectAttr(ISP_DEV dev, const ISP_EFFECT_ATTR_S *pstEffect);
EI_S32 EI_MI_ISP_GetEffectAttr(ISP_DEV dev, ISP_EFFECT_ATTR_S *pstEffect);

EI_S32 EI_MI_ISP_SetAeAttr(ISP_DEV dev, const ISP_AE_ATTR_S *pstAe);
EI_S32 EI_MI_ISP_GetAeAttr(ISP_DEV dev, ISP_AE_ATTR_S *pstAe);

EI_S32 EI_MI_ISP_SetAwbAttr(ISP_DEV dev, const ISP_AWB_ATTR_S *pstAwb);
EI_S32 EI_MI_ISP_GetAwbAttr(ISP_DEV dev, ISP_AWB_ATTR_S *pstAwb);

EI_S32 EI_MI_ISP_SetAfAttr(ISP_DEV dev, const ISP_AF_ATTR_S *pstAf);
EI_S32 EI_MI_ISP_GetAfAttr(ISP_DEV dev, ISP_AF_ATTR_S *pstAf);

EI_S32 EI_MI_ISP_SetNrAttr(ISP_DEV dev, const ISP_NR_ATTR_S *pstNr);
EI_S32 EI_MI_ISP_GetNrAttr(ISP_DEV dev, ISP_NR_ATTR_S *pstNr);

EI_S32 EI_MI_ISP_SetDefogAttr(ISP_DEV dev, const ISP_DEFOG_ATTR_S *pstDefog);
EI_S32 EI_MI_ISP_GetDefogAttr(ISP_DEV dev, ISP_DEFOG_ATTR_S *pstDefog);

EI_S32 EI_MI_ISP_BWEnable(ISP_DEV dev, EI_BOOL bEn);

EI_S32 EI_MI_ISP_GetMdMap(ISP_DEV dev, EI_U8 au8MotionMap[ISP_GRID_ZONE_NUM]);

EI_S32 EI_MI_ISP_SetOdAttr(ISP_DEV dev, const ISP_OD_ATTR_S *pstOd);
EI_S32 EI_MI_ISP_GetOdAttr(ISP_DEV dev, ISP_OD_ATTR_S *pstOd);

EI_S32 EI_MI_ISP_SetIrdAttr(ISP_DEV dev, const ISP_IRD_ATTR_S *pstIrd);
EI_S32 EI_MI_ISP_GetIrdAttr(ISP_DEV dev, ISP_IRD_ATTR_S *pstIrd);

EI_S32 EI_MI_ISP_SetGammaAttr(ISP_DEV dev, const ISP_GAMMA_ATTR_S *pstGamma);
EI_S32 EI_MI_ISP_GetGammaAttr(ISP_DEV dev, ISP_GAMMA_ATTR_S *pstGamma);

EI_S32 EI_MI_ISP_SetCscAttr(ISP_DEV dev, const ISP_CSC_ATTR_S *pstCsc);
EI_S32 EI_MI_ISP_GetCscAttr(ISP_DEV dev, ISP_CSC_ATTR_S *pstCsc);

EI_S32 EI_MI_ISP_SetDwdrAttr(ISP_DEV dev, const ISP_DWDR_ATTR_S *pstDwdr);
EI_S32 EI_MI_ISP_GetDwdrAttr(ISP_DEV dev, ISP_DWDR_ATTR_S *pstDwdr);

EI_S32 EI_MI_ISP_GetAEStatistics(ISP_DEV dev, ISP_AE_STATISTICS_S *pstAeStat);
EI_S32 EI_MI_ISP_GetAWBStatistics(ISP_DEV dev, ISP_AWB_STATISTICS_S *pstAwbStat);
EI_S32 EI_MI_ISP_GetAFStatistics(ISP_DEV dev, ISP_AF_STATISTICS_S *pstAfStat);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MI_ISP_H__ */
