/*
 * fh_audio_mpi.h
 *
 *  Created on: 2015/2/15
 *      Author: fanggm
 */

#ifndef FH_AC_MPI_H_
#define FH_AC_MPI_H_

#include "types/type_def.h"
#include "fh_audio_mpipara.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/**
* FH_AC_Init
*@brief 音频系统初始化，完成打开驱动设备
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_Init(void);

/**
 * * FH_AC_Init_WithExternalCodec
 * *@brief 外部codec音频系统初始化，完成打开驱动设备
 * *@param [in] 无
 * *@param [out] 无
 * *@return 是否成功
 * * - RETURN_OK(0):  成功
 * * - 其他：失败,错误详见错误号
 */
FH_SINT32 FH_AC_Init_WithExternalCodec(void);

/**
* FH_AC_DeInit
*@brief 音频系统释放资源，关闭音频设备
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_DeInit(void);

/**
* FH_AC_Set_Config
*@brief 设置AI、AO设备参数
*@param [in] pstConfig 设备参数信息结构体指针
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_Set_Config(FH_AC_CONFIG *pstConfig);

/**
* FH_AC_Set_Config_Ext
*@brief 设置AI、AO设备参数
*@param [in] pstConfig 设备参数信息结构体指针
*@param [in] frame_num, how many frames it will buffer for audio data
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_Set_Config_Ext(FH_AC_CONFIG *pstConfig, FH_UINT32 frame_num);

/**
* FH_AC_AI_Enable
*@brief 使能AI设备
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AI_Enable(void);

/**
* FH_AC_AI_Disable
*@brief 禁用AI设备
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AI_Disable(void);

/**
* FH_AC_AO_Enable
*@brief 使能AO设备
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AO_Enable(void);

/**
 * Set AO's workmode, only support for FH8856
 */
FH_SINT32 FH_AC_AO_Set_Mode(FH_AC_AO_MODE_E mode);

/**
* FH_AC_AO_Disable
*@brief 禁用AO设备
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AO_Disable(void);

/**
* FH_AC_AI_Pause
*@brief 暂停AI设备运行
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AI_Pause(void);

/**
* FH_AC_AI_Resume
*@brief 恢复AI设备运行
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AI_Resume(void);

/**
* FH_AC_AO_Pause
*@brief 暂停AO设备运行
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AO_Pause(void);

/**
* FH_AC_AO_Resume
*@brief 恢复AO设备运行
*@param [in] 无
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AO_Resume(void);

/**
* FH_AC_AI_SetVol
*@brief 设置AI设备音量大小
*@param [in] volume 音量大小,0 ~ 100
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AI_SetVol(FH_SINT32 volume);

FH_SINT32 FH_AC_AI_MICIN_SetVol(FH_SINT32 volume);

FH_SINT32 FH_AC_AO_SetVol(FH_SINT32 volume);

/**
* FH_AC_AI_GetFrame
*@brief 获取一帧音频数据
*@param [in] 无
*@param [out] pstFrame 音频数据结构体指针
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AI_GetFrame(FH_AC_FRAME_S *pstFrame);

/*
 * this function support PTS, it increase 1000000 per second...
 * don't use FH_AC_AI_GetFrameWithPts and FH_AC_AI_GetFrame the same time.
 * Only use one of them is OK!!!!
 */
FH_SINT32 FH_AC_AI_GetFrameWithPts(FH_AC_FRAME_S *pstFrame, FH_UINT64 *pPts);

/**
* FH_AC_AO_SendFrame
*@brief 发送一帧音频数据
*@param [in] pstFrame 音频数据结构体指针
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
FH_SINT32 FH_AC_AO_SendFrame(FH_AC_FRAME_S *pstFrame);

/*
 *@brief config Audio echo canceller's parameter...
 *@return 是否成功
 * - RETURN_OK(0):  成功
 * - 其他：失败,错误详见错误号
 */
FH_SINT32 FH_AC_AEC_SetConfig(FH_AC_AEC_CONFIG *pAecPara);

/*
 *@brief config Audio record noise-reduce parameter...
 *@return 是否成功
 * - RETURN_OK(0):  成功
 * - 其他：失败,错误详见错误号
 */
FH_SINT32 FH_AC_NR_SetConfig(FH_AC_NR_CONFIG *pNrPara);

/*
 *@brief config Audio play noise-reduce parameter...
 *@return 是否成功
 * - RETURN_OK(0):  成功
 * - 其他：失败,错误详见错误号
 */
FH_SINT32 FH_AC_PlayNR_SetConfig(FH_AC_NR_CONFIG *pNrPara);

/*
 *@brief config Audio record gain...
 *@return 是否成功
 * - RETURN_OK(0):  成功
 * - 其他：失败,错误详见错误号
 */
FH_SINT32 FH_AC_Agc_SetConfig(FH_AC_AGC_CONFIG *pPara);

/*
 *@brief set audio module's work mode, you should not use it. If you really want to use, please consult our FAE.
 *@return 是否成功
 * - RETURN_OK(0):  成功
 * - 其他：失败,错误详见错误号
 */
FH_SINT32 FH_AC_Set_WorkMode(FH_AC_WORKMODE_E mode);

/*
 * Get file descriptor, don't care it, just reserved for special use...
 */
//int FH_AC_GetFD(void);

/*
 * extended ioctl handle interface, for special use, don't care it...
 */
FH_SINT32 FH_AC_Ext_Ioctl(FH_AC_EXT_IOCTL_CMD *pIoctl);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* FH_AC_MPI_H_ */
