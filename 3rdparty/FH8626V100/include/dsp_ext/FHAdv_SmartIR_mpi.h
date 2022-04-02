#ifndef _FHADV_SMART_IR_H_
#define _FHADV_SMART_IR_H_

#include "FH_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum
{
	FHADV_IR_DAY     = 0,
	FHADV_IR_NIGHT   = 1,
}FHADV_DAYNIGHT_STA;

typedef enum
{
	FHADV_IR_BAYER   = 0,    /* for bayer sensor */
	FHADV_IR_RGBIR   = 1,    /* for rgbir sensor */
}FHADV_SMARTIR_ATTR;

typedef enum
{
    SMARTIR_DEBUG_LEVEL_OFF       = 0x0, /* close debug print */
    SMARTIR_DEBUG_LEVEL_IRSTATUS  = 0x1, /* print day to night & night to day debug info */
    SMARTIR_DEBUG_LEVEL_AE        = 0x2, /* print ae & gain debug info */
    SMARTIR_DEBUG_LEVEL_AWB       = 0x4, /* print awb & sat debug info*/
    SMARTIR_DEBUG_LEVEL_ALL       = 0xf, /* print all debug info*/
}FHADV_SMARTIR_DEBUG_LEVEL;

typedef struct
{
    FH_UINT16 gainDay2Night;    /* for bayer sensor */
    FH_UINT16 gainNight2Day;    /* for bayer sensor */
    FH_UINT16 lumaDay2Night;    /* for rgbir sensor */
    FH_UINT16 lumaNight2Day;    /* for rgbir sensor */
}FHADV_SMARTIR_THRESHOLD_t;

typedef struct
{
    FH_UINT16 enable;       /* enable advanced function,default:1 */
    FH_UINT16 rgThre;       /* r/g threshold, default:0x400 */
    FH_UINT16 rgThreNum;    /* block number which is greater than rgThre, range [0, 10], default:5 */
    FH_UINT16 bgThre;       /* b/g threshold, default:0x300 */
    FH_UINT16 bgThreNum;    /* block number which is less than bgThre, range [0, 10], default:3 */
    FH_UINT16 sstableNum;   /* Number of stage stable frames, range [0, 5], default:3 */
    FH_UINT16 lumaplus;     /* sat environment luma offset, default:0xecd */
    FH_UINT16 reserved[3];  /* reserved param, default:set by FHAdv_SmartIR_GetAdvanceMode() automatically*/
}FHADV_SMARTIR_ADVANCE_t;

/*
*@ init
*@return 0: 成功 -1：失败
*/
FH_SINT32 FHAdv_SmartIR_Init(FH_VOID);

/*
*@ Uninit
*@return 0: 成功 -1：失败
*/
FH_VOID FHAdv_SmartIR_UnInit(FH_VOID);

/*
*@ 设置sensor属性
*@param [in] attribute
*@return 0: 成功 非0：失败
*/
FH_SINT32 FHAdv_SmartIR_SetAttr(FHADV_SMARTIR_ATTR attribute);

/*
*@ 设置sensor属性
*@param [in] attribute
*@return 0: 成功 非0：失败
*/
FH_SINT32 FHAdv_SmartIR_GetAttr(FHADV_SMARTIR_ATTR *pattribute);

/*
*@ 设置日夜切换阀值
*@param [in] pstThreshold
*@return 0: 成功 非0：失败
*/
FH_SINT32 FHAdv_SmartIR_Setthreshold(FHADV_SMARTIR_THRESHOLD_t *pstThreshold);

/*
*@ 获取日夜切换增益阀值
*@param [out] pstThreshold
*@return 0: 成功 非0：失败
*/
FH_SINT32 FHAdv_SmartIR_Getthreshold(FHADV_SMARTIR_THRESHOLD_t *pstThreshold);

/*
*@ 设置日夜切换饱和度阈值
*@param [in] thL为环境亮度最低时，饱和度阈值的最大值;thH为环境亮度最高时，饱和度阈值的最低值
*@return 0: 成功 非0：失败
*/
FH_SINT32 FHAdv_SmartIR_SetsaturationTh(FH_SINT32 thL, FH_SINT32 thH);

/*
*@ 获取日夜切换饱和度阈值
*@param [out] thL为环境亮度最低时，饱和度阈值的最大值;thH为环境亮度最高时，饱和度阈值的最低值
*@return 0: 成功 非0：失败
*/
FH_SINT32 FHAdv_SmartIR_GetsaturationTh(FH_SINT32 *thL, FH_SINT32 *thH);

/*
*@ 获取当前白天晚上状态
*@param [in] irCurr  当前ir状态，初始必须为白天
*@return 0:白天 1：晚上
*/
FH_UINT8 FHAdv_SmartIR_GetDayNightStatus(FHADV_DAYNIGHT_STA irCurr);

/*
*@ 配置切换帧延迟，配置的越高，则切换延迟越长。最小为30
*@param [in] num
*@return 0: OK
*/
FH_SINT32 FHAdv_SmartIR_SetFramedelay(FH_SINT32 num);

/********
 * 获取打印库版本信息
 ********/
FH_CHAR* FH_ADV_SMARTIR_Version(FH_UINT32 print_enable);

/*
*@ 配置debug等级
*@param [in] FHADV_SMARTIR_DEBUG_LEVEL
*@return 0: OK
*/
FH_SINT32 FHAdv_SmartIR_SetDebugLevel(FHADV_SMARTIR_DEBUG_LEVEL level);

/*
*@ 配置震荡补偿值和清零时间
*@param [in] 每一次震荡，饱和度阈值增加offset；第一次震荡经过recoverFrm（帧）后，清空补偿值
*@return 0: OK
 */
FH_SINT32 FHAdv_SmartIR_SetCustom_offset(FH_SINT32 offset, FH_SINT32 recoverFrm);

/*
*@ 获取震荡补偿值和清零时间
*@param [in] 补偿值offset，清零时间recoverFrm（帧）
*@return 0: OK
*/
FH_SINT32 FHAdv_SmartIR_GetCustom_offset(FH_SINT32 *poffset, FH_SINT32 *precoverFrm);

/*
*@ 配置高级模式
*@ 注意：需先调用FHAdv_SmartIR_GetAdvanceMode获取当前配置，再调用FHAdv_SmartIR_SetAdvanceMode进行配置
*@param [in] pstThreshold
*@return 0: OK
*/
FH_SINT32 FHAdv_SmartIR_SetAdvanceMode(const FHADV_SMARTIR_ADVANCE_t *padvcfg);

/*
*@ 获取高级模式配置
*@param [in] pstThreshold
*@return 0: OK
*/
FH_SINT32 FHAdv_SmartIR_GetAdvanceMode(FHADV_SMARTIR_ADVANCE_t *padvcfg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* _FHADV_SMART_IR_H_ */


