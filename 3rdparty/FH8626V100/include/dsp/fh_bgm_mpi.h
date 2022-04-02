#ifndef __FH_BGM_MPI_H__
#define __FH_BGM_MPI_H__
/**BGM**/

#include "types/type_def.h"
#include "fh_common.h"
#include "fh_bgm_mpipara.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/*
*   Name: FH_BGM_InitMem
*            背景建模初始化
*
*   Parameters:
*
*       [in] FH_UINT32 Bgmwidth
*            背景建模支持的最大幅面宽
*
*       [in] FH_UINT32 Bgmheight
*            背景建模支持的最大幅面高
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_InitMem(FH_UINT32 Bgmwidth,FH_UINT32 Bgmheight);


/*
*   Name: FH_BGM_SetConfig
*            设置背景建模处理幅面。
*
*   Parameters:
*
*       [in] const FH_SIZE *pstBgmconfig
*            背景建模处理幅面,幅面大小为主码流幅面16对齐后的1/8下采样的大小，不超过初始化最大幅面。
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_SetConfig(const FH_SIZE *pstBgmconfig);

/*
*   Name: FH_BGM_GetConfig
*            获取背景建模处理幅面。
*
*   Parameters:
*
*       [out] FH_SIZE *pstBgmconfig
*            背景建模处理幅面
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_GetConfig(FH_SIZE *pstBgmconfig);

/*
*   Name: FH_BGM_Enable
*            开启背景建模使能
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_Enable(void);


/*
*   Name: FH_BGM_Disable
*            关闭背景建模使能
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_Disable(void);

/*
*   Name: FH_BGM_SubmitFrame
*            向背景建模模块提交图像数据
*
*   Parameters:
*
*       [in] const FH_BGM_FRAME *pstframe
*            背景建模图像数据描述信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_SubmitFrame(const FH_BGM_FRAME *pstframe);

/*
*   Name: FH_BGM_GetSWStatus
*            获取背景建模软件处理结果
*
*   Parameters:
*
*       [out] FH_BGM_SW_STATUS *pststatus
*            背景建模软件结果
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_GetSWStatus(FH_BGM_SW_STATUS *pststatus);

/*
*   Name: FH_BGM_GetRunTableRect
*            获取连通域处理框信息
*
*   Parameters:
*
*       [out] FH_BGM_RUNTB_RECT *prunrect
*            连通域处理框信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_GetRunTableRect(FH_BGM_RUNTB_RECT *prunrect);

/*
*   Name: FH_BGM_BkgReinit
*            重新初始化背景帧。一般情况下无需用户调用。
*            在一些利用BGM信息做智能分析等应用中，在发生场景切换后需要BGM数据生效时间加快时可以使用。
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_BkgReinit(void);

/*
*   Name: FH_BGM_ADV_SetBkgInitCount
*            设置背景帧初始的帧数。需要BGM驱动启动时开启背景建模时生效。
*            决定初始化或者调用重新初始化时多少帧完成背景帧的建立。
*
*   Parameters:
*
*       [in] FH_UINT32 count
*            背景初始化帧数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_ADV_SetBkgInitCount(FH_UINT32 count);
/*
*   Name: FH_BGM_ADV_SetUpdateSigGauCount
*            设置单高斯模型生效时间。需要驱动关闭多高斯时生效。
*
*   Parameters:
*
*       [in] FH_UINT32 count
*            更新单高斯的更新帧数周期
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_BGM_ADV_SetUpdateSigGauCount(FH_UINT32 count);

/*智能编码GOP设置接口,会在VENC相关接口中被调用,不建议用户直接调用此接口*/
FH_SINT32 fh_bgm_setgopth(const FH_GOP_TH *pgopth);
FH_SINT32 fh_bgm_getgopth(FH_GOP_TH *pgopth);

/*高级参数配置接口,不建议用户自行调用此接口,以免影响背景建模效果*/
FH_SINT32 FH_BGM_ADV_SetBkgInitAttr(const FH_BKG_INIT_CFG *pbkginitcfg);
FH_SINT32 FH_BGM_ADV_GetBkgInitAttr(FH_BKG_INIT_CFG *pbkginitcfg);
FH_SINT32 FH_BGM_ADV_SetSigGauAttr(const FH_SINGLE_GAU_CFG *psiggaucfg);
FH_SINT32 FH_BGM_ADV_GetSigGauAttr(FH_SINGLE_GAU_CFG *psiggaucfg);
FH_SINT32 FH_BGM_ADV_SetMulGauAttr(const FH_MULTI_GAU_CFG *pmulgaucfg);
FH_SINT32 FH_BGM_ADV_GetMulGauAttr(FH_MULTI_GAU_CFG *pmulgaucfg);
FH_SINT32 FH_BGM_ADV_SetDisAttr(const FH_DIS_CFG *pdiscfg);
FH_SINT32 FH_BGM_ADV_GetDisAttr(FH_DIS_CFG *pdiscfg);
FH_SINT32 FH_BGM_ADV_SetConExpaAttr(FH_CON_EXPA_IDX conidx,const FH_CON_EXPA_CFG *pconexpacfg);
FH_SINT32 FH_BGM_ADV_GetConExpaAttr(FH_CON_EXPA_IDX conidx,FH_CON_EXPA_CFG *pconexpacfg);
FH_SINT32 FH_BGM_ADV_SetFrmDiffAttr(const FH_FRM_DIFF_CFG *pfrmdiffcfg);
FH_SINT32 FH_BGM_ADV_GetFrmDiffAttr(FH_FRM_DIFF_CFG *pfrmdiffcfg);
FH_SINT32 FH_BGM_ADV_SetFrmDiffConsistAttr(const FH_FRM_DIFF_CONSIST_CFG *pfrmconsistcfg);
FH_SINT32 FH_BGM_ADV_GetFrmDiffConsistAttr(FH_FRM_DIFF_CONSIST_CFG *pfrmconsistcfg);
FH_SINT32 FH_BGM_ADV_SetGlbBgLumaAttr(const FH_GLOBAL_BG_LUMA_CFG *pbglumacfg);
FH_SINT32 FH_BGM_ADV_GetGlbBgLumaAttr(FH_GLOBAL_BG_LUMA_CFG *pbglumacfg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /*__MPI_VO_H__ */
