#ifndef _ISP_COMMON_H_
#define _ISP_COMMON_H_

#include "types/type_def.h"
#ifdef OS_RTT
#define fh_printf    rt_kprintf
#ifndef fh_memcpy
#define fh_memcpy    rt_memcpy
#endif
#else //OS_LINXU
#define fh_printf    printf
#ifndef fh_memcpy
#define fh_memcpy    memcpy
#endif
#endif //OS_RTT

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define REG_ISP_NXT_PTR0				 (0xEA000050)
#define REG_ISP_DC1_DEBUG3     	 (0xEA0005d4)
#define IPF_PKG_START_REG	REG_ISP_NXT_PTR0
#define IPF_PKG_END_REG		REG_ISP_DC1_DEBUG3

#define REG_ISP_NXT_PTR1 			(0xEA000720)
#define REG_ISP_DC2_DEBUG3 	(0xEA000AAC)

#define IPB_PKG_START_REG	REG_ISP_NXT_PTR1
#define IPB_PKG_END_REG		REG_ISP_DC2_DEBUG3

#define ISPP_PKG_START_REG REG_ISPP_NXT_PTR0
#define ISPP_PKG_END_REG	REG_ISPP_STAT_MD_FLAG11

#define ISPF_PKG_START_REG REG_ISPF_NXT_PTR1
#define ISPF_PKG_END_REG REG_ISPF_RGBX_CCM_IR1

#define MALLOCED_MEM_BASE_ISP			(0)

#define MALLOCED_MEM_ISPPPKG			(0)
#define MALLOCED_MEM_ISPFPKG			(1)

#define MALLOCED_MEM_DRV			(3)

#define GAIN_NODES		12
#define LINEAR_INTERPOLATION(gain0,gain1,y0,y1,gainx)  (((gainx)-(gain0))*(y1)+((gain1)-(gainx))*(y0))/((gain1)-(gain0))

typedef int (*AeOpStatusCallback)(int,int);//参数1 opstatus：1ae op 2 ae stop  参数2 lumadiff

typedef int (*ispInitCfgCallback)(void);
typedef int (*ispIntCallback)(void);
void ispTrace_FunctionName(const char* func);




/**|SYSTEM CONTROL|**/
typedef enum _ISP_BAYER_TYPE_S_ {
    BAYER_RGGB = 0x0,  // 色彩模式RGGB
    BAYER_GRBG = 0x1,  // 色彩模式GRBG
    BAYER_BGGR = 0x2,  // 色彩模式BGGR
    BAYER_GBRG = 0x3,  // 色彩模式GBRG
    BAYER_GBGR_AGAG_GRGB_AGAG = (0x0 << 4),  // 色彩模式GBGR_AGAG_GRGB_AGAG
    BAYER_GRGB_AGAG_GBGR_AGAG = (0x1 << 4),  // 色彩模式GRGB_AGAG_GBGR_AGAG
    BAYER_AGAG_GBGR_AGAG_GRGB = (0x2 << 4),  // 色彩模式AGAG_GBGR_AGAG_GRGB
    BAYER_AGAG_GRGB_AGAG_GBGR = (0x3 << 4),  // 色彩模式AGAG_GRGB_AGAG_GBGR
    BAYER_BGRG_GAGA_RGBG_GAGA = (0x4 << 4),  // 色彩模式BGRG_GAGA_RGBG_GAGA
    BAYER_RGBG_GAGA_BGRG_GAGA = (0x5 << 4),  // 色彩模式RGBG_GAGA_BGRG_GAGA
    BAYER_GAGA_BGRG_GAGA_RGBG = (0x6 << 4),  // 色彩模式GAGA_BGRG_GAGA_RGBG
    BAYER_GAGA_RGBG_GAGA_BGRG = (0x7 << 4),  // 色彩模式GAGA_RGBG_GAGA_BGRG
} ISP_BAYER_TYPE;


typedef struct _ISP_VERSION_S_ {
    FH_UINT32 u32SdkVer;  // sdk版本号 | [0~0xffffffff]
    FH_UINT32 FH_UINT32ChipVer;  // 芯片版本号 | [0~0xffffffff]
    FH_UINT8 u08SdkSubVer;  // sdk sub版本号 | [0~0xff]
    FH_UINT8 u08BuildTime[21];  // sdk构建时间 | [0~0xff]
} ISP_VERSION;

typedef struct _ISP_VI_ATTR_ {
    FH_UINT16 u16WndHeight;  // sensor幅面高度 | [0~0xffff]
    FH_UINT16 u16WndWidth;  // sensor幅面宽度 | [0~0xffff]
    FH_UINT16 u16InputHeight;  // sensor输入图像高度 | [0~0xffff]
    FH_UINT16 u16InputWidth;  // sensor输入图像宽度 | [0~0xffff]
    FH_UINT16 u16OffsetX;  // 裁剪水平偏移 | [0~0xffff]
    FH_UINT16 u16OffsetY;  // 裁剪垂直偏移 | [0~0xffff]
    FH_UINT16 u16PicHeight;  // 处理的图像高度 | [0~0xffff]
    FH_UINT16 u16PicWidth;  // 处理的图像宽度 | [0~0xffff]
    FH_UINT16 u16FrameRate;  // 帧率 | [0~0xffff]
    ISP_BAYER_TYPE  enBayerType;  // bayer数据格式 | [ISP_BAYER_TYPE]
} ISP_VI_ATTR_S;

typedef struct _ISP_VI_STAT_S {
    FH_UINT32 u32IPBIntCnt;  // IPB中断计数 | [0~0xffffffff]
    FH_UINT32 u32IPFIntCnt;  // IPF中断计数 | [0~0xffffffff]
    FH_UINT32 u32FrmRate;  // 当前帧率 | [0~0xffffffff]
    FH_UINT32 u32PicWidth;  // 当前处理图像宽度 | [0~0xffffffff]
    FH_UINT32 u32PicHeight;  // 当前处理图像高度 | [0~0xffffffff]
    FH_UINT32 u32IpfOverFlow;  // ISP Vi溢出中断计数 | [0~0xffffffff]
    FH_UINT32 u32IspErrorSt;  // 出错标志，Vi溢出终端计数大于阈值会被值1 | [0~1]
} ISP_VI_STAT_S;

typedef struct _ISP_DEBUG_INFO_S_ {
    FH_UINT32 envLuma;  // 亮度平均值 | [0~0xffffffff]
    FH_UINT32 sqrtenvLuma;  // 亮度平均值开方 | [0~0xffffffff]
    FH_UINT32 sensor_gain;  // sensor当前增益值 | [0~0xffffffff]
    FH_UINT32 isp_gain;  // ISP当前增益值 | [0~0xffffffff]
} ISP_DEBUG_INFO;

typedef struct _HW_MODULE_CFG_S{
    FH_UINT32 moduleCfg;  // 需要配置的模块 | [ISP_HW_MODULE_LIST]
    FH_UINT32 enable;  // 使能开关 | [0~1]
} HW_MODULE_CFG;

/**|BLC|**/
typedef struct _ISP_BLC_ATTR_S_ {
    FH_BOOL bDeltaModeEn;  // 分通道减DC使能 | [0~1]
    FH_BOOL bMode;  // 增益线性映射使能开关 | [0~1]
    FH_UINT16 u16BlcLevel;  // 手动黑电平值 | [0~0xfff]
    FH_UINT8 u08BlcDeltaR;  // 通道R的delta dc值，在blc_level基础上减delta_r | [0~0xff]
    FH_UINT8 u08BlcDeltaG;  // 通道G的delta dc值，在blc_level基础上减delta_g | [0~0xff]
    FH_UINT8 u08BlcDeltaB;  // 通道B的delta dc值，在blc_level基础上减delta_b | [0~0xff]
    FH_UINT16 u16BlcMapMap[GAIN_NODES];  // 不同增益下的减黑电平值 | [0~0xfff]
    FH_UINT8 u08BlcDeltaRMap[GAIN_NODES];  // 不同增益下的R通道的delta值 | [0~0xff]
    FH_UINT8 u08BlcDeltaGMap[GAIN_NODES];  // 不同增益下的G通道的delta值 | [0~0xff]
    FH_UINT8 u08BlcDeltaBMap[GAIN_NODES];  // 不同增益下的B通道的delta值 | [0~0xff]
} ISP_BLC_ATTR;
/**|GB|**/
typedef struct _ISP_GB_CFG_S_ {
    FH_BOOL bGBEn;  // 绿平衡控制使能 | [0~1]
    FH_UINT16 u16Th;  // 绿平衡上阈值 | [0~0xffff]
    FH_UINT16 u16Tl;  // 绿平衡下阈值 | [0~0xffff]
} ISP_GB_CFG;
/**|DPC|**/
typedef struct _DYNAMIC_DPC_CFG_S_ {
    FH_BOOL  bDpcEn;  // 坏点消除控制使能 | [0~1]
    FH_BOOL  bGainMappingEn;  // 增益线性映射使能开关 | [0~1]
    FH_UINT8 ctrl_mode;  // 去坏点控制模式，值越大越强 | [0~3]
    FH_UINT8 u08Str;  // 手动去坏点强度 | [0~7]
    FH_UINT8 u08DpcMode;  // 去坏点模式，关闭去坏点(0)，打开去白点(1)，打开去黑点(2)，同时打开去白点黑点功能(3) | [0~3]
    FH_UINT8 u08Strenght[GAIN_NODES];  // 包括:mode,w_s,b_s | [0~0xff]
    FH_UINT8 u08WhiteThr[GAIN_NODES];  // 白点门限DC值 | [0~0xff]
    FH_UINT8 u08BlackThr[GAIN_NODES];  // 黑点门限值 | [0~0xff]
    FH_UINT8 u08wdc;  // 手动白点門限值 | [0~0xff]
    FH_UINT8 u08bdc;  // 手动黑点門限值 | [0~0xff]
} DYNAMIC_DPC_CFG;

/**|LSC|**/
typedef struct _ISP_LSC_CFG_S_ {
    FH_BOOL bLscEn;  // 镜头阴影矫正控制使能 | [0~1]
    FH_UINT32 u32Coff[299];  // 补偿系数 | [0~0xffffffff]
} ISP_LSC_CFG;

/**|NR3D|**/
typedef struct _ISP_NR3D_CFG_S_ {
    FH_BOOL bNR3DEn;  // 3d降噪控制使能 | [0~1]
    FH_BOOL bGainMappingEn;  // 增益线性映射使能开关 | [0~1]
    FH_UINT8 u08PresetMap;  // 0: 采用内置coeff映射表（由coeff_map_idx决定）1：采用自定义coeff映射表 | [0~1]
    FH_UINT8 u08CoeffMapIdx;  // Coeff映射线（建议白天配置0或1）：0~3：过渡越来越平滑，但同时强度变弱 | [0~3]
    FH_UINT8 u08Str;  // nr3d去噪强度,u4.4 | [0~0xff]
    FH_UINT16 u16K1;  // 斜率K的斜率,U2.6 | [0~0xff]
    FH_UINT16 u16K2;  // 斜率K的偏移,U8 | [0~0xff]
    FH_UINT16 u16O1;  // 偏移O的斜率,S6.10 | [-32768~32767]
    FH_UINT16 u16O2;  // 偏移O的偏移,S16 | [-32768~32767]
    FH_UINT8 u08CoeffOffset;  // coeff的偏移,U8 | [0~0xff]
    FH_UINT8 u8Nr3dSft;  // nr3d噪声估计整体移位因子,U4 | [0~0xff]
    FH_UINT16 u8MtStr;  // 运动估计等级，U8 | [0~0xff]
    FH_UINT8 u08StrMap[GAIN_NODES];  // NR3D等级，U4.4 | [0~0xff]
    FH_UINT8 u08CoeffOffsetMap[GAIN_NODES];  // coeff的偏移等级，U8| [0~0xff]
    FH_UINT8 u08MtStrMap[GAIN_NODES];  // NR3D运动估计等级 | [0~0xff]

} ISP_NR3D_CFG;

/**|LTM|**/
typedef struct _ISP_LTM_CFG_S_ {
    FH_BOOL bMode;  // 线性增益模式:0 手动,1 gainMapping | [0~1]
    FH_BOOL bLtmHwEn;  // ltm硬件使能开关 | [0~1]
    FH_UINT8 u08CtrlMode;  // 控制模式:0 merge模式，1 负责暗处的算子1, 2 负责亮处的算子2 | [0~3]
    FH_UINT8 u08SatCoeffNum;  // merge之后饱和度矫正系数取值范围 | [0~10]
    FH_UINT8 u08YCoeffNum;  // 算子1亮度矫正系数 | [0~13]
    FH_UINT8 u08LtmWth0;  // 正常亮度权重阈值 | [0~0xff]
    FH_UINT8 u08LtmWth1;  // 高亮度权重阈值 | [0~0xff]
    FH_UINT8 u08K1;  // 正常亮度处的对比系数，值越小，图像对比度越高，U2.6| [0~0xff]
    FH_UINT8 u08K2Offset;  // 图像暗处偏移 | [0x3~0xb]
    FH_UINT8 u08CrtRatio;  // 算子1和算子2 merge之后的锐化系数，图像越锐，光环越明显 | [0~0xff]
    FH_UINT8 u08YMinRatio;  // 统计得到的像素最小值的补偿比例因子，U2.6 | [0~0xff]
    FH_UINT8 u08YMeanRatio;  // 统计得到的像素平均值的补偿比例 | [ ]
    FH_UINT8 u08YMaxRatio;  // 统计得到的像素最大值的补偿比例因子，U2.6 | [0~0xff]
    FH_UINT8 u08CrtRatioMap[GAIN_NODES];  // 不同增益下的锐化比例值 | [0~0xff]
    FH_UINT8 u08Wth0Map[GAIN_NODES];  // 不同增益下的正常亮度权重阈值 | [0~0xff]
    FH_UINT8 u08Wth1Map[GAIN_NODES];  // 不同增益下的高亮度权重阈值 | [0~0xff]
    FH_UINT8 u08YCoeffMap[GAIN_NODES];  // 不同增益下的亮度矫正值 | [0~0xff]
    FH_UINT8 u08SatCoeffMap[GAIN_NODES];  // 不同增益下的饱和度 | [0~0xff]
} ISP_LTM_CFG;
/**|NR2D|**/
typedef struct _ISP_NR2D_CFG_S_ {
    FH_BOOL bNR2DEn;  // 2D降噪控制使能 | [0~1]
    FH_BOOL bGainMappingEn;  // 增益线性映射使能开关 | [0~1]
    FH_UINT8 u8BlendingVal;  // nr2d所占比例 | [0~0xff]
    FH_UINT8 u8Nr2dSft;  // nr2d噪声估计移位因子,U4 | [0~0xf]
    FH_UINT8 u8CoeffOffset;  // nr2d coeff的偏移 | [0~0xf]
    FH_UINT8 u08Str;  // 2d降噪强度，U4.4精度 | [0~0xff]
    FH_UINT16 u16K1;  // 斜率K的斜率,U4.4 | [0~0xff]
    FH_UINT16 u16K2;  // 斜率K的偏移,U8 | [0~0xff]
    FH_UINT16 u16O1;  // 偏移O的斜率,S6.10 | [-32768~32767]
    FH_UINT16 u16O2;  // 偏移O的偏移,S16 | [-32768~32767]
    FH_UINT8 u08StrMap[GAIN_NODES];  // nr2d等级 | [0~0xff]
} ISP_NR2D_CFG;
/**|HLR|**/
typedef struct _ISP_HLR_CFG_S_ {
    FH_BOOL bHlrEn;  // 高光恢复控制使能 | [0~1]
    FH_SINT8 s08Level;  // hlr补偿因子 | [-128~127]
} ISP_HLR_CFG;
/**|IE|**/
typedef struct _ISP_CONTRAST_CFG_S_ {
    FH_BOOL bYcEn;  // 亮度控制使能 | [0~1]
    FH_BOOL bGainMappingEn;  // 对比度控制模式:0 manual;1 gain mapping;2 auto | [0~2]
    FH_UINT8 u08Crt;  // 手动模式下对比度值 U2.6 | [0~0xff]
    FH_UINT8 u08CrtMap[GAIN_NODES];  // 不同噪声强度下对比度值U2.6 | [0~0xff]
} ISP_CONTRAST_CFG;

typedef struct _ISP_BRIGHTNESS_CFG_S_ {
    FH_BOOL bYcEn;  // 亮度控制使能 | [0~1]
    FH_BOOL bGainMappingEn;  // 亮度控制模式:0 manual;1 gain mapping;2 auto | [0~2]
    FH_UINT8 u08Brt;  // 手动模式下亮度值 S18 | [-128~127]
    FH_UINT8 u08BrtMap[GAIN_NODES];  // 不同噪声强度下亮度值U2.6 | [0~0xff]
} ISP_BRIGHTNESS_CFG;
/**|CE|**/
typedef struct _ISP_SAT_CFG_S_ {
    FH_BOOL bGainMappingEn;  // 饱和度控制模式:0 manual;1 gain mapping | [0~1]
    FH_SINT8 s08RollAngle;  // 色度旋转因子 | [-64,63]
    FH_UINT8 u08Sat;  // 饱和度增益 | [0~0xff]
    FH_UINT8 u08BlueSurp;  // 蓝色分量抑制增益 | [0~0xff]
    FH_UINT8 u08RedSurp;  // 红色分量抑制增益 | [0~0xff]
    FH_UINT8 u08SatMap[GAIN_NODES];  // 不同噪声强度下饱和度值U2.6 | [0~0xff]
} ISP_SAT_CFG;
/**|APC|**/
typedef struct _ISP_APC_CFG_S_ {
    FH_BOOL bApcEn;  // APC控制使能 | [0~1]
    FH_BOOL bGainMappingEn;  // APC 控制模式: 0: manual 1: gain mapping | [0~1]
    FH_BOOL bLutMappingEn;  // LUT 控制模式: 0: manual 1: gain mapping | [0~1]
    FH_BOOL bFilterMappingEn;  //Filter 控制模式: 0: manual 1: gain mapping | [0~1]
    FH_BOOL bMergesel;  // Detail Enhancer　和　Edge Sharpener合并模式选择: 0:　相加　1: 取绝对最大值　| [0~1]
    FH_BOOL bLutMode;  // LUT系数选择模式: 0: 系数表选择模式　1: 手动配置系数 | [0~1]
    FH_BOOL bFilterMode;  // 滤波器选择模式: 0: 系数表选择模式　1: 手动配置系数| [0~1]
    FH_UINT8 u08DetailLutNum;  // 细节增强LUT系数选择 | [0~8]
    FH_UINT8 u08EdgeLutNum;  // 边界锐化LUT系数选择　| [0~8]
    FH_UINT8 u08DetailFNum;  // 细节滤波器选择　| [0~3]
    FH_UINT8 u08EdgeFNum;  // 边界滤波器选择　| [0~3]
    FH_UINT8 u08Pgain;  //  总体APC正向增益　| [0-0xff]
    FH_UINT8 u08Ngain;  // 总体APC负向增益 | [0-0xff]
    FH_UINT8 u08DetailLv;  // 细节锐化等级　|　[0-0xff]
    FH_UINT8 u08EdgeLv;  // 边界锐化等级 | [0-0xff]
    FH_UINT16 u16DetailThl;  // 细节增强下门限值 | [0-0x3ff]
    FH_UINT16 u16DetailThh;  // 细节增强上门限值 | [0-0x3ff]
    FH_UINT16 u16EdgeThl;  // 边界增强下门限值 | [0-0x3ff]
    FH_UINT16 u16EdgeThh;  // 边界增强上门限值 | [0-0x3ff]
    FH_BOOL bShootrcen; // 过冲抑制使能 | [0~1]
    FH_UINT8 u08Shootratio; //　过冲抑制比例，值越大，抑制越强 | [0~3f]
    FH_UINT8 u08Sigmathl; //　方差权重下阈值，方差小于该值，不做过冲抑制 | [0~ff]
    FH_UINT8 u08Sigmathh; //　方差权重上阈值，方差大于该值，都做过冲抑制 | [0~ff]
    FH_UINT8 u08EdgeMap[GAIN_NODES];  // 不同噪声强度下边界锐化强度值U2.6 | [0~0xff]
    FH_UINT8 u08DetailMap[GAIN_NODES];  // 不同噪声强度下细节锐化强度值U2.6 | [0~0xff]
    FH_UINT8 u08PgainMap[GAIN_NODES];  // 不同噪声强度下白边界强度值U2.6 | [0~0xff]
    FH_UINT8 u08NgainMap[GAIN_NODES];  // 不同噪声强度下黑边界强度值U2.6 | [0~0xff]
    FH_UINT8 u08DetailLutMap[GAIN_NODES];  // 不同噪声强度下细节LUT系数值 | [0~0x8]
    FH_UINT8 u08EdgeLutMap[GAIN_NODES];  // 不同噪声强度下边界LUT系数值 | [0~0x8]
    FH_UINT8 u08DetailFilterMap[GAIN_NODES];  // 不同噪声强度下细节滤波系数值 | [0~0x3]
    FH_UINT8 u08EdgeFilterMap[GAIN_NODES];  // 不同噪声强度下边界滤波系数值 | [0~0x3]
    FH_UINT8 u08SigmathlMap[GAIN_NODES];  // 不同噪声强度下方差权重下阈值 | [0~0xff]
    FH_UINT8 u08SigmathhMap[GAIN_NODES];  // 不同噪声强度下方差权重上阈值 | [0~0xff]
} ISP_APC_CFG;

typedef struct _SHARPEN_STAT_CFG_S{
    FH_UINT16 w;  // 配置的统计窗口宽 | [1, PicW]
    FH_UINT16 h;  // 配置的统计窗口的高 | [1, PicH]
    FH_UINT16 x_offset;  // 统计窗口的水平偏移 | [0, PicW - w]
    FH_UINT16 y_offset;  // 统计窗口的垂直偏移 | [0, PicH - h]
} SHARPEN_STAT_CFG;

typedef struct _SHARPEN_STAT_S{
    FH_UINT32 sum;  // 获取到的锐度统计信息 | [0~0xffffffff]
} SHARPEN_STAT;

/**|GAMMA|**/
typedef enum IPB_GAMMA_MODE_S_{
    GAMMA_OFF = 0,  // 关闭GAMMA
    GAMMA_MODE_YC = 1,  // GAMMA采用YC模式
    GAMMA_MODE_RGB = 2,  // GAMMA采用RGB模式
    GAMMA_MODE_RGBY = 3,  // GAMMA采用RGBY模式
} IPB_GAMMA_MODE;

typedef enum  _GAMMA_BUILTIN_IDX_ {
    GAMMA_CURVE_10 = 0,  // GAMMA曲线1.0
    GAMMA_CURVE_12 = 1,  // GAMMA曲线1.2
    GAMMA_CURVE_14 = 2,  // GAMMA曲线1.4
    GAMMA_CURVE_16 = 3,  // GAMMA曲线1.6
    GAMMA_CURVE_18 = 4,  // GAMMA曲线1.8
    GAMMA_CURVE_20 = 5,  // GAMMA曲线2.0
    GAMMA_CURVE_22 = 6,  // GAMMA曲线2.2
    GAMMA_CURVE_24 = 7,  // GAMMA曲线2.4
    GAMMA_CURVE_26 = 8,  // GAMMA曲线2.６
    GAMMA_CURVE_28 = 9,  // GAMMA曲线2.8
} GAMMA_BUILTIN_IDX;

typedef enum _CURVE_TYPE_ {
    CURVE_BUILTIN = 0x1,  // 选用内定曲线
    CURVE_USER_DEFINE = 0x2,  // 选用用户自定义曲线
} CURVE_TYPE;

typedef struct _ISP_GAMMA_CFG_S_ {
    FH_BOOL bGammaEn;  // gamma控制使能 | [0~1]
    IPB_GAMMA_MODE u8GammaMode;  // gamma当前工作模式:0 gamma off;1 YC mode;2 RGB mode;3 RGBY mode | [0~3]
    CURVE_TYPE eCCurveType;  // c gamma控制模式:0 内置gamma表;1 用户自定义gamma表 | [0~1]
    GAMMA_BUILTIN_IDX eCGammaBuiltInIdx;  // c gamma控制模式等于0时有效，内置cgamma表索引 | [0~0xf]
    FH_UINT16 u16CGamma[160];  // 用户自定义cgamma表 | [0~0xffffffff]
    CURVE_TYPE eYCurveType;  // y gamma控制模式:0 内置gamma表;1 用户自定义gamma表 | [0~1]
    GAMMA_BUILTIN_IDX eYGammaBuiltInIdx;  // y gamma控制模式等于0时有效，内置ygamma表索引 | [0~0xf]
    FH_UINT16 u16YGamma[160];  // 用户自定义ygamma表 | [0~0xffffffff]
} ISP_GAMMA_CFG;
/**|HIST|**/
typedef struct _ISP_HIST_STAT_S_ {
    FH_UINT32 u32histBin[33][2];  // HIST统计值，该统计值包含33个bin。每个bin包含sum和cnt值 | [0xffffffff]
} ISP_HIST_STAT;
/**|YCNR|**/
typedef struct _ISP_YNR_CFG_S_ {
    FH_BOOL bYnrEn;  // ynr控制使能 | [0~1]
    FH_BOOL bGainMappingEn;  // ynr gainMapping control mode: 0:gain mapping OFF 1:gainMapping ON | [0~1]
    FH_BOOL bCtrlMode;  // ynr模式: 0:弱模式　1:强模式| [0~1]
    FH_BOOL bAcgCtrlMode;  // ynr acgain mapping control mode:  0:gain mapping OFF 1:gainMapping ON | [0~1]
    FH_UINT16 u16Acgain0;  // 高频AC gain 斜率　| [0~0xfff]
    FH_UINT16 u16Acgain1;  // 中频AC gain 斜率　| [0~0xfff]
    FH_UINT16 u16Acgain2;  // 低频AC gain 斜率　| [0~0xfff]
    FH_UINT8 u08AcgainStr;  //AC gain强度 | [0~0xff]
    FH_UINT8 u08MergeWeight;  //融合权重 （去噪结果与原始输入融合，weight权重越大，原始输入权重越大，去噪越弱）| [0~0xff]
    FH_UINT8 u08MtCoeff0;  //YNR与NR3D联动去噪第一个coeff分界点，小运动 | [0~0xff]
    FH_UINT8 u08MtCoeff1;  //YNR与NR3D联动去噪第二个coeff分界点，大运动 | [0~0xff]
    FH_UINT8 u08YnrMtStrM0;  //YNR与NR3D联动去噪第一级强度，小运动 | [0~0xff]
    FH_UINT8 u08YnrMtStrM1;  //YNR与NR3D联动去噪第二级强度，大运动 | [0~0xff]
    FH_UINT16 u16YnrThStr0;  //YNR高频降噪去噪等级　| [0~0xfff]
    FH_UINT16 u16YnrThStr1;  //YNR中频降噪去噪等级　| [0~0xfff]
    FH_UINT16 u16YnrThStr2;  //YNR低频降噪去噪等级　| [0~0xfff]
    FH_UINT16 u16Thl0[16];  //YNR亮度阈值（高频分量） | [0~0x3ff]
    FH_UINT16 u16Thl1[16];  //YNR亮度阈值（中频分量） | [0~0x3ff]
    FH_UINT16 u16Thl2[16];  //YNR亮度阈值（低频分量） | [0~0x3ff]
    FH_UINT8 u08AcgainStrMap[GAIN_NODES];  //不同噪声强度下ACgain强度值　| [0~0xff]
    FH_UINT8 u08YnrMergeWeightMap[GAIN_NODES];  //不同噪声强度下融合权重值　| [0~0xff]
    FH_UINT8 u08YnrMtStr0[GAIN_NODES];  //运动去噪等级（小运动） | [0~0xff]
    FH_UINT8 u08YnrMtStr1[GAIN_NODES];  //运动去噪等级（大运动） | [0~0xff]
    FH_UINT16 u16YnrTh0[GAIN_NODES];  //不同噪声强度下亮度去噪强度（高频）　| [0~0x3ff]
    FH_UINT16 u16YnrTh1[GAIN_NODES];  //不同噪声强度下亮度去噪强度（中频）　| [0~0x3ff]
    FH_UINT16 u16YnrTh2[GAIN_NODES];  //不同噪声强度下亮度去噪强度（低频）　| [0~0x3ff]
} ISP_YNR_CFG;

typedef struct _ISP_CNR_CFG_S_ {
    FH_BOOL bGainMappingEn;  // 线性映射使能:0 手动，1 自动 | [0~1]
    FH_UINT8 u08CnrStr;       // 色度去噪强度 | [0~0xff]
    FH_UINT8 u08CnrThh;     // 边界阈值上限 | [0~0xff]
    FH_UINT8 u08CnrThl;     // 边界阈值下限 | [0~0xff]
    FH_UINT8 u08CnrStrMap[GAIN_NODES];  // 不同增益下的色度噪声等级,gainMapping模式用 | [0~0xff]
    FH_UINT8 u08CnrThhMap[GAIN_NODES];  // 不同增益下的边界阈值上限,gainMapping模式用 | [0~0xff]
    FH_UINT8 u08CnrThlMap[GAIN_NODES];  // 不同增益下的边界阈值下限,gainMapping模式用 | [0~0xff]
} ISP_CNR_CFG;
/**|PURPLE|**/
typedef struct _ISP_PURPLEFRI_CFG_S_ {
    FH_BOOL bPurpleEn;  // 去紫边控制使能 | [0~1]
    FH_UINT8 u08Shiftbits;  // 矫正力度调整单元，通过移位增大或减小饱和度(当为正数时表示左移，当为负数时表示右移) | [-8~3]
    FH_BOOL bSatcorrenable;  // 饱和度矫正使能 | [0~1]
    FH_UINT16 u16Rollangle;  // 旋转因子，单位为度 | [0~0x1ff]
    FH_UINT16 u16Adjangle;  // 调整区域因子，单位为度 | [0~0x1ff]
    FH_UINT16 u16Ygapth;  // 7x7窗亮度分量最大值和最小值之差的最小阈值，Ymax-YMin>=YGapTh | [0~0x3ff]
    FH_UINT16 u16Yth;  // 7x7窗亮度分量最大值的最小阈值，YMax>=Yth | [0~0x3ff]
    FH_UINT16 u16Expoth;  // 亮度分量的最大阈值，Y<ExpoTh | [0~0x3ff]
    FH_UINT16 u16Ulth;  // 旋转后色度分量U' 最小阈值，U' > ULth | [0~0x3ff]
    FH_UINT16 u16Uhth;  // 旋转后色度分量U' 最小阈值，U' < UHth | [0~0x3ff]
    FH_UINT16 u16Satthh;  // 饱和度最大阈值，Sat > SatThL | [0~0x3ff]
    FH_UINT16 u16Satthl;  // 饱和度最小阈值 | [0~0x3ff]
} ISP_PURPLEFRI_CFG;
/**|LC|**/
typedef struct _ISP_LC_CFG_S_ {
    FH_BOOL bLc0En;  // 第一个色调调整使能 | [0~1]
    FH_UINT8 u08Gain0;  // 中心增益,U2.6| [0x27~0xff]
    FH_UINT8 u08Alpha0;  // 调整范围角度alpha | [0xf~0x59]
    FH_UINT16 u16Theta0;  // 中心线角度theta | [0~0x167]
    FH_BOOL bLc1En;  // 第二个色调调整使能 | [0~1]
    FH_UINT8 u08Gain1;  // 中心增益,U2.6| [0x27~0xff]
    FH_UINT8 u08Alpha1;  // 调整范围角度alpha | [0xf~0x59]
    FH_UINT16 u16Theta1;  // 中心线角度theta | [0~0x167]
    FH_BOOL bLc2En;  // 第三个色调调整使能 | [0~1]
    FH_UINT8 u08Gain2;  // 中心增益,U2.6| [0x27~0xff]
    FH_UINT8 u08Alpha2;  // 调整范围角度alpha | [0xf~0x59]
    FH_UINT16 u16Theta2;  // 中心线角度theta | [0~0x167]
} ISP_LC_CFG;

/**|AE|**/

typedef enum _AE_CMD_KEY_S_ {
    SET_AE_DEFAULT = 0x0,  // 配置AE DRV参数
    SET_ASC_EN = 0x1,  // 配置自动快門开关
    SET_AGC_EN = 0x2,  // 配置自动增益控制
    SET_APERTURE_EN = 0x3,  // 配置自动光圈控制
    SET_LUMA_REF = 0x4,  // 配置参考亮度
    SET_STAB_ZONE = 0x5,  // 配置上下死区
    SET_STAB_CNT = 0x6,  // 配置稳定时间
    SET_DGAIN_MAX = 0x7,  // 配置ISP GAIN最大值
    SET_SENSOR_GAIN_MAX = 0x8,  // 配置模拟增益最大值
    SET_INTT_MAX = 0x9,  // 配置sensor最大曝光行数
    SET_INTT_FINE_TIMING = 0xa,  // sensor gain延时生效时机点
    SET_GAIN_PREC = 0xb,  // sensor gain精度
    SET_INTT_MIN = 0xc,  // sensor最小曝光行数
    SET_AE_SPPED = 0xd,  // 曝光速度
    SET_METERING_MODE = 0xe,  // 测光模式
    SET_METERING_PARAM = 0xf,  // 背光/前光补偿强度
    SET_METERING_WEIGHT = 0x10,  // 曝光统计块权重
    SET_SENSOR_UP_MAX = 0x11,  // 慢快門倍数最大值
    SET_SENSOR_UPEN_GAIN = 0x12,  // sensor增益开始門限值
    SET_SENSOR_UPDIS_GAIN = 0x13,  // 无效
    SET_ADAPTIVE_EN = 0x14,  // 自动曝光自适应模式
    SET_LUMA_REF_LOW = 0x15,  // 参考亮度最低阈值
    SET_LUMA_REF_HIGH = 0x16,  // 参考亮度最高阈值
    UPDATE_TAR_LUMA_STATUS = 0x17,  // 更新曝光状态中的目标亮度
    GET_ENV_LUMA = 0x18,  // 获取当前亮度值
} AE_CMD_KEY;

typedef struct _ISP_AE_STAT1_S_ {
    FH_UINT32 u32SumLuma[9];  // ae统计sum值 | [0~0xffffffff]
    FH_UINT32 u32Cnt[9];  // ae统计cnt值 | [0~0xffffffff]
} ISP_AE_STAT1;

typedef struct _ISP_AE_STAT2_S_ {
    FH_UINT64 u64SumLuma[33];  // hist统计sum值 | [0~0xffffffffffffffff]
    FH_UINT32 u32Cnt[33];  // hist统计cnt值 | [0~0xffffffff]
} ISP_AE_STAT2;

typedef struct _ISP_AE_STAT3_S_ {
    FH_UINT16 u32GlobalAvgLuma[1024];  // 全局统计平均亮度值 | [0~0xffffffff]
} ISP_AE_STAT3;

typedef struct _ISP_AE_STAT_S_ {
    ISP_AE_STAT1 stAeStat1;  // ae统计成员 | [ISP_AE_STAT1]
    ISP_AE_STAT2 stAeStat2;  // hist统计值成员 | [ISP_AE_STAT2]
} ISP_AE_STAT;

typedef struct _AE_STAT_CFG_S_ {
    FH_BOOL bChange;  // 阈值是否需要改变 | [0~1]
    FH_UINT16 FH_UINT16HistStatThr[32];  // 直方图每个bin亮度阈值 | [0~0xfff]
} AE_STAT_CFG;

typedef struct _ISP_AE_INFO_S_ {
    FH_UINT32 u32Intt;  // sensor曝光行数 | [0~0xffffffff]
    FH_UINT32 u32IspGain;  // isp增益值 | [0~0xffffffff]
    FH_UINT32 u32IspGainShift;  // isp增益值移位 | [0~0xffffffff]
    FH_UINT32 u32SensorGain;  // sensor增益值 |[0~0xffffffff]
    FH_UINT32 u32TotalGain;  // 总增益 | [0~0xffffffff]
    AE_STAT_CFG stAeStatCfg;  // ae统计配置结构体 | [AE_STAT_CFG]
} ISP_AE_INFO;

typedef struct _ISP_AE_STAT_WIN_S_ {
    FH_UINT16 winHOffset;  // 统计窗口水平偏移 | [0~PicW]
    FH_UINT16 winVOffset;  // 统计窗口垂直偏移 | [0~PicH]
    FH_UINT16 winHSize;  // 统计窗口宽度 | [0~(PicW-winHOffset)]
    FH_UINT16 winVSize;  // 统计窗口高度 | [0~(PicH-winVOffset)]
} ISP_AE_STAT_WIN;

typedef struct _AE_WEIGHT_CFG_S_ {
    FH_UINT8 u08Weight[9];  // ae统计块权重值 | [0~0xff]
} AE_WEIGHT_CFG;

typedef struct _AE_DEFAULT_CFG_S_ {
    FH_BOOL bAscEn;  // 自动快门控制 | [0~1]
    FH_BOOL bAgcEn;  // 自动增益控制 | [0~1]
    FH_BOOL bApertureEn;  // 自动光圈控制 | [0~1]
    FH_UINT8 u08LumaRef;  // 自动曝光参考亮度值 | [0~0xff]
    FH_UINT8 u08StabZone0;  // 稳定区域0,目标亮度偏差小于此阈值，曝光行为停止 | [0~0xff]
    FH_UINT8 u08StabZone1;  // 稳定区域1,目标亮度偏差大于此阈值，曝光调整启动 | [0~0xff]
    FH_UINT8 u08StabCnt;  // 稳定时间，按帧数计，超过稳定时间后，曝光才　进入下一种状态 | [0~0xff]
    FH_UINT16 u16DgainMax;  // ISP最大数字增益值 | [0~0xffff]
    FH_UINT16 u16SensorGainMax;  // sensor最大模拟增益值 | [0~0xffff]
    FH_UINT16 u16InttMax;  // 最大曝光行数 | [0~0xffff]
    FH_UINT8 u08InttFineTiming;  // 模拟增益延时生效时间:0 不延时;1 延时1帧 | [0~1]
    FH_UINT8 u08InttMin;  // sensor最小曝光行数 | [0~0xf]
    FH_UINT8 u08AeSpeed;  // 曝光速度 | [0~0xff]
    FH_UINT8 u08MeteringMode;  // 测光模式:0 全局曝光;1 中心测光;2 中心权重测光;3 背光补偿;4 前光补偿;5 用户自定义 | [0~5]
    FH_UINT8 u08MeteringParam;  // 背光or前光补偿模式的强度 | [0~8]
    AE_WEIGHT_CFG weightParam;  // ae每块统计的权重值 | [0~0xff]
    FH_UINT8 u08UpMax;  // 慢快門倍数的最大值 | [0~0xff]
    FH_UINT16 u16UpEnGain;  // sensor增益开始門限值 | [0~0xff]
    FH_UINT16 u16UpDisGAIN;  // 无用 | [0~0xff]
    FH_BOOL bAdaptiveEn;  // 自动曝光亮度自适应使能 | [0~1]
    FH_UINT8 u08LumaRefLow;  // 低亮度时的参考亮度 | [0~0xff]
    FH_UINT8 u08LumaRefHigh;  // 高亮度时的参考亮度值 | [0~0xff]
    FH_UINT8 u08GainPrec;  // sensor增益精度 | [0~7]
} AE_DEFAULT_CFG;

/**|AWB|**/

typedef enum _AWB_CMD_KEY_S_ {
    SET_AWB_DEFAULT = 0x0,  // 配置AWB DRV参数
    CTRL_AWB_ENABLE = 0x1,  // AWB策略控制使能
    SET_AWB_SPEED = 0x2,  // AWB增益调节速度
    SET_AWB_SPEED_BYPASS = 0x3,  // AWB增益调节速度屏蔽开关
    SET_AWB_WHITEPOINT = 0x4,  // 配置AWB白点
    SET_STAT_CTRL = 0x5,  // AWB统计門限值
    SET_CCM_ARRAY = 0x6,  // CCM矩阵配置
    SET_AWB_GAIN = 0x7,  // 配置AWB增益
    GET_AWB_GAIN = 0x8,  // 获取当前AWB增益值
    SET_AWB_SENSOR_GAIN_ENABLE = 0x9,  // sensor WB增益使能开关
} AWB_CMD_KEY;

typedef struct _ISP_AWB_STAT1_S_ {
    FH_UINT32 u32AwbBlockCnt[9];  // AWB 统计值cnt值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockB[9];  // AWB B分量sum统计值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockG[9];  // AWB G分量sum统计值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockR[9];  // AWB R分量sum统计值 | [0~0xffffffff]
} ISP_AWB_STAT1;

typedef struct _ISP_AWB_STAT2_S_ {
    FH_UINT16 u32AwbBlockCnt[1024];  // AWB 统计值cnt值 | [0~0xffffffff]
    FH_UINT16 u32AwbBlockB[1024];  // AWB B分量sum统计值 | [0~0xffffffff]
    FH_UINT16 u32AwbBlockG[1024];  // AWB G分量sum统计值 | [0~0xffffffff]
    FH_UINT16 u32AwbBlockR[1024];  // AWB R分量sum统计值 | [0~0xffffffff]
} ISP_AWB_STAT2;

typedef struct _ISP_AWB_FRONT_STAT_S_{
    FH_UINT32 u32AwbBlockCnt[64];  // AWB 统计值cnt值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockB[64];  // AWB B分量sum统计值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockG[64];  // AWB G分量sum统计值 | [0~0xffffffff]
    FH_UINT32 u32AwbBlockR[64];  // AWB R分量sum统计值 | [0~0xffffffff]
} ISP_AWB_FRONT_STAT;

typedef struct _ISP_AWB_INFO_S_ {
    ISP_AWB_STAT1 stAwbStat1;  // awb统计值 | [ISP_AWB_STAT1]
} ISP_AWB_INFO;

typedef struct _STAT_WHITE_POINT_S_ {
    FH_UINT16 u16Coordinate_w;  // awb白框横坐标 | [0~0xffff]
    FH_UINT16 u16Coordinate_h;  // awb白框纵坐标 | [0~0xffff]
} STAT_WHITE_POINT;

typedef struct _AWB_STAT_CFG_S_ {
    FH_BOOL bChange;  // 是否更新该结构体包含的配置到硬件寄存器 | [0~1]
    FH_UINT16 u16YHighThreshold;  // awb统计上門限值 | [0~0xffff]
    FH_UINT16 u16YLowThreshold;  // awb统计低門限值 | [0~0xffff]
    STAT_WHITE_POINT stPoint[7];  // awb白框点 | [STAT_WHITE_POINT]
} AWB_STAT_CFG;

typedef struct _ISP_AWB_CFG_S_ {
    FH_UINT32 u32AwbGain[3];  // rgain,ggain,bgain | [0~0xffffffff]
    FH_UINT16 u16CcmCfg[12];  // ccm增益值 | [0~0xffff]
    AWB_STAT_CFG stAwbStatCtrl;  // awb统计配置结构体 | [AWB_STAT_CFG]
    AWB_STAT_CFG stAwbLongStatCtrl;  // awb短帧统计配置结构体 | [AWB_STAT_CFG]
    AWB_STAT_CFG stAwbShortStatCtrl;  // awb长帧统计配置结构体 | [AWB_STAT_CFG]
} ISP_AWB_CFG;

typedef struct _AWB_WHITE_POINT_S_ {
    FH_UINT16 u16BOverG[4];  // 四种色温下标定白点的B/G分量 | [0~0xffff]
    FH_UINT16 u16ROverG[4];  // 四种色温下标定白点的R/G分量 | [0~0xffff]
} AWB_WHITE_POINT;

typedef struct _AWB_WHITE_EREA_S_ {
    FH_UINT32 u32Area_P[5];  // 白框的横纵坐标配置值 | [0~0xfffffff]
} AWB_WHITE_EREA;

typedef struct _ISP_AWB_STAT_WIN_S_ {
    FH_UINT16 winHOffset;  // 统计窗水平方向偏移 | [0, PicW - winHSize]
    FH_UINT16 winVOffset;  // 统计窗垂直方向偏移 | [0, PicH - winVSize]
    /*FH8856's WinHsize and winVsize need to be 16 pixels alignment in wdr mode*/  
    FH_UINT16 winHSize;  // 统计窗宽度 | [0, PicW]
    FH_UINT16 winVSize;  // 统计窗高度 | [0, PicH]
} ISP_AWB_STAT_WIN;

typedef struct _CCM_TABLE_S_ {
    FH_UINT16 u16CcmTable[4][12];  // ccm矩阵表 | [0~0xffff]
} CCM_TABLE;

typedef struct _STAT_CTRL_S_ {
    FH_UINT16 u16AwbThresholdL;  // awb统计上門限值 | [0~0xffff]
    FH_UINT16 u16AwbThresholdH;  // awb统计下門限值 | [0~0xffff]
} STAT_CTRL;

typedef struct _AWB_DEFAULT_CFG_S_ {
    FH_BOOL bAwbEn;  // 白平衡控制使能 | [0~1]
    FH_BOOL bCcmEn;  // 色彩矫正控制使能 | [0~1]
    FH_UINT8 awbMode;  // 自动白平衡工作模式:0 grey world;1 awb pro | [0~2]
    FH_UINT16 u16AwbSpeed;  // 自动白平衡调整速度 | [0~0xf]
    AWB_WHITE_POINT stWhitePoint;  // 标定白点的结构体 | [AWB_WHITE_POINT]
    AWB_WHITE_EREA  stWhiteArea;  // 统计区域左边配置结构体 | [AWB_WHITE_EREA]
    STAT_CTRL stStatCtrl;  // 统计門限值配置结构体 | [STAT_CTRL]
    CCM_TABLE stCcmTable;  // 色彩矩阵表配置结构体 | [CCM_TABLE]
} AWB_DEFAULT_CFG;

/**|GLOBE|**/
typedef struct _GLOBE_STAT_S {
    struct _Block_gstat {
        FH_UINT32 sum;  // 全局统计sum值 | [0~0xffffffff]
        FH_UINT32 cnt;  // 全局统计cnt值 | [0~0xffffffff]
        FH_UINT32 max;  // 统计块内出现的亮度最大值 | [0~0xffffffff]
        FH_UINT32 min;  // 统计块内出现的亮度最小值 | [0~0xffffffff]
    } r, gr, gb, b;
} GLOBE_STAT;

typedef struct _GLOBE_STAT_CFG_S {
    FH_UINT16 width;  // 全局统计窗口宽度 | [0~PicW]
    FH_UINT16 height;  // 全局统计窗口高度 | [0~PicH]
    FH_UINT16 x_offset;  // 全局统计窗口水平方向偏移 | [0~PicW-width]
    FH_UINT16 y_offset;  // 全局统计窗口垂直方向偏移 | [0~PicH-height]
    FH_UINT8 cnt_x;  // 全局统计水平方向窗口个数 | [0~32]
    FH_UINT8 cnt_y;  // 全局统计垂直方向窗口个数 | [0~32]
} GLOBE_STAT_CFG;

/**|AF|**/
//----------------AF-------------------
typedef struct _ISP_AF_FILTER_S {
    FH_SINT8  afPreFilterCoe[7];  //  | [ ]
    FH_UINT16 afPreFilterMul;  //  | [ ]
    FH_UINT16 afPreFilterShift;  //  | [ ]
    FH_SINT8  afVfilter0[25];  //  | [ ]
    FH_UINT16 afVFilter0Shift;  //  | [ ]
    FH_UINT16 afVFilter0T;  //  | [ ]
//*no use below (FH8856)*//
    FH_SINT8  afVfilter1[25];  //  | [ ]
    FH_UINT16 afVFilter1Shift;  //  | [ ]
    FH_UINT16 afVFilter1T;  //  | [ ]
    FH_BOOL   afHfilter0En[3];  //  | [ ]
    FH_UINT8  afHfilter0Gain;  //  | [ ]
    FH_UINT16 afHFilter0T;  //  | [ ]
    FH_SINT16 afHFilter0Offset;  //  | [ ]
    FH_UINT8  afHFilter0Shift[7];  //  | [ ]
    FH_UINT16 afHFilter0C[6];  //  | [ ]
    FH_UINT16 afHFilter0D[6];  //  | [ ]
    FH_BOOL   afHfilter1En[3];  //  | [ ]
    FH_UINT8  afHfilter1Gain;  //  | [ ]
    FH_UINT16 afHFilter1T;  //  | [ ]
    FH_SINT16 afHFilter1Offset;  //  | [ ]
    FH_UINT8  afHFilter1Shift[7];  //  | [ ]
    FH_UINT16 afHFilter1C[6];  //  | [ ]
    FH_UINT16 afHFilter1D[6];  //  | [ ]

} ISP_AF_FILTER;

typedef struct _ISP_AF_WIN_INFO_S {
    FH_UINT16 winHStart0;  // the start pixel in horizontal direction, more than 2 | [ ]
    FH_UINT16 winVStart0;  // the start pixel in vertical direction, more than 2 | [ ]

    FH_UINT8 winWidth0;  // window width | [ ]
    FH_UINT8 winHeight0;  // window height | [ ]
    FH_UINT16 winHStart1;  // the start pixel in horizontal direction, more than 2 | [ ]
    FH_UINT16 winVStart1;  // the start pixel in vertical direction, more than 2 | [ ]
    FH_UINT8 winHCnt1;  // the number of window in horizontal direction | [ ]
    FH_UINT8 winVCnt1;  // the number of window in vertical direction | [ ]
    FH_UINT8 winWidth1;  // window width | [ ]
    FH_UINT8 winHeight1;  // window height | [ ]
} ISP_AF_WIN_INFO;

typedef struct _ISP_AF_STAT_ADDR_S {
    FH_UINT32 startAddr;  //  | [ ]
} ISP_AF_STAT_ADDR;

typedef struct _ISP_AF_STAT_PIXEL_S {
    FH_UINT32 line_e;  //  | [ ]
    FH_UINT32 line_o;  //  | [ ]
    FH_UINT32 column_e;  //  | [ ]
    FH_UINT32 column_o;  //  | [ ]
} ISP_AF_STAT_DATA;

typedef struct _ISP_AF_STAT_S {
    ISP_AF_STAT_DATA stat_win1[16];  //  | [ ]
    ISP_AF_STAT_DATA stat_win0;  //  | [ ]
} ISP_AF_STAT;

/**|GME|**/
typedef struct _ISP_GME_PARAM_S_ {
    FH_SINT32 s32x; //x vector | [ ]
    FH_SINT32 s32y; //y vector | [ ]
} ISP_GME_PARAM;

/**|RUBBISH|**/
typedef enum _TAB_COLOR_CODE{
    CC_R = 0,  // 颜色R
    CC_GR = 1,  // 颜色GR
    CC_B = 2,  // 颜色B
    CC_GB = 3,  // 颜色GB
}TAB_COLOR_CODE;

typedef enum _SNS_CLK_S_ {
    SNS_CLK_24_POINT_0	= 0x1,
    SNS_CLK_27_POINT_0	= 0x2,
    SNS_CLK_37_POINT_125	= 0x3,
} SNS_CLK;

typedef enum _SNS_DATA_BIT_S_ {
    LINER_DATA_8_BITS = 0x1,
    LINER_DATA_12_BITS = 0x2,
    LINER_DATA_14_BITS = 0x3,
    WDR_DATA_16_BITS = 0x4,
} SNS_DATA_BITS;

typedef enum _SIGNAL_POLARITY_S_ {
    ACTIVE_HIGH	= 0x0,
    ACTIVE_LOW	= 0x1
} SIGNAL_POLARITY;

typedef struct _ISP_STATUS_S_ {
    FH_UINT32 u32day;  //  | [ ]
} ISP_STATUS;

typedef struct _STATIC_DPC_CFG_S_ {
    FH_BOOL bStaticDccEn;  //  | [ ]
    FH_BOOL bStaticDpcEn;  //  | [ ]
    FH_UINT32 u32DpcTable[1024];  //  | [ ]
    FH_UINT16 u16DpcCol[32];  //  | [ ]
} STATIC_DPC_CFG;

typedef struct _ISP_ALGORITHM_S_ {
    FH_UINT8	u08Name[16];  //  | [ ]
    FH_UINT8 u08AlgorithmId;  //  | [ ]
    FH_VOID (*run)(FH_VOID);  //  | [ ]
} ISP_ALGORITHM;

typedef struct _ISP_VI_HW_ATTR_S_ {
    SNS_CLK eSnsClock;  // 配置的cis时钟 | [SNS_CLK]
    SNS_DATA_BITS eSnsDataBits;  // 配置的sensor数据位宽 | [SNS_DATA_BITS]
    SIGNAL_POLARITY eHorizontalSignal;  // 时钟水平极性 | [0~1]
    SIGNAL_POLARITY eVerticalSignal;  // 时钟垂直极性 | [0~1]
    FH_BOOL u08Mode16;  //  | [ ]
    FH_UINT32 u32DataMaskHigh;  //  | [ ]
    FH_UINT32 u32DataMaskLow;  //  | [ ]
} ISP_VI_HW_ATTR;

typedef struct _ISP_DEFAULT_PARAM_ {
    ISP_BLC_ATTR stBlcCfg;  //　 | [ ]
    ISP_LTM_CFG stLtmCfg;  //  | [ ]
    ISP_GAMMA_CFG stGamma;  //  | [ ]
    ISP_SAT_CFG stSaturation;  //  | [ ]
    ISP_APC_CFG stApc;  //  | [ ]
    ISP_CONTRAST_CFG stContrast;  //  | [ ]
    ISP_BRIGHTNESS_CFG stBrt;  //  | [ ]
    ISP_NR3D_CFG stNr3d;  //  | [ ]
    ISP_NR2D_CFG stNr2d;  //  | [ ]
    ISP_YNR_CFG stYnr;  //  | [ ]
    ISP_CNR_CFG stCnr;  //  | [ ]
    DYNAMIC_DPC_CFG  stDpc;  //  | [ ]
    ISP_LSC_CFG stLscCfg;  //  | [ ]
} ISP_DEFAULT_PARAM;

/**|WDR|**/
typedef struct _ISP_WDR_MERGE_CFG_ {
    FH_UINT16 u16MergeX0;  // Merge曲线的第一个折点横坐标。 | [0~0xffff]
    FH_UINT16 u16MergeX1;  // Merge曲线的第二个折点横坐标。 | [0~0xffff]
    FH_UINT16 u16MergeX2;  // Merge曲线的第三个折点横坐标。 | [0~0xffff]
    FH_UINT16 u16MergeX3;  // Merge曲线的第四个折点横坐标。 | [0~0xffff]
    FH_UINT16 u16MergeY0;  // Merge曲线的第一个折点纵坐标。 | [0~0xffff]
    FH_UINT16 u16MergeY1;  // Merge曲线的第二个折点纵坐标。 | [0~0xffff]
    FH_UINT16 u16MergeY2;  // Merge曲线的第三个折点纵坐标。 | [0~0xffff]
    FH_UINT16 u16MergeY3;  // Merge曲线的第四个折点纵坐标。 | [0~0xffff]
}ISP_WDR_MERGE_CFG;

typedef struct _ISP_WDR_CFG_S_ {
    FH_BOOL bWdrEn;  //  | [ ]
    FH_BOOL bShowHist;  //  | [ ]
    FH_BOOL bMode;  //  | [ ]
    FH_BOOL bAutoAdjustEn;  //  | [ ]
    FH_UINT8 u08SframeCorrMode;  //  | [ ]
    FH_UINT16 u16SensorOutputMax;  //  | [ ]
    FH_UINT16 u16SframeCorrTiming;  //  | [ ]
    FH_UINT8 u08ExposureRatio;  //  | [ ]
    FH_UINT8 u08ExposureRatioMin;  //  | [ ]
    FH_UINT8 u08ExposureRatioMax;  //  | [ ]
    FH_UINT16 u16WdrDc;  //  | [ ]
    FH_UINT8 u08WdrMergeCfg;  //  | [ ]
    FH_UINT8 u08PreCurveIdx;  //  | [ ]
    FH_UINT16 u16LfOverexpRatioThh;  //  | [ ]
    FH_UINT16 u16LfOverexpRatioThl;  //  | [ ]
    FH_UINT16 u16SfOverexpRatioTh;  //  | [ ]
    FH_UINT16 u16GainTh;  //  | [ ]
} ISP_WDR_CFG;

/**|POST GAIN|**/
typedef struct _ISP_POST_GAIN_CFG_S {
    FH_UINT16 rGain;  //  | [ ]
    FH_UINT16 gGain;  //  | [ ]
    FH_UINT16 bGain;  //  | [ ]
    FH_UINT16 rOffset;  //  | [ ]
    FH_UINT16 gOffset;  //  | [ ]
    FH_UINT16 bOffset;  //  | [ ]
} ISP_POST_GAIN_CFG;

/**|RGBA|**/
typedef enum _RGBA_CMD_KEY_S_ {
    SET_RGBA_DEFAULT = 0x0,  // 配置RGBA　DRV寄存器
    SET_RGBA_EN = 0x1,  // RGBA策略使能开关
    SET_RGBA_FIRFRM = 0x2,  // 配置RGBA处理第一帧标志
    SET_RGBA_IRMODE = 0x3,  // Abal使能
    SET_ABAL_PARA = 0x4,  // 配置Abal相关DRV参数
    SET_WEIGHT_PARA = 0x5,  // 配置W2相关DRV参数
    SET_RGBA_CCM_TABLE = 0x6,  // 配置RGBA的CCM矩阵
    SET_CONVERT_TH = 0x7,  // 格式转换LUT上下阈值配置
    SET_ENHANCE_TH = 0x8,  //  亮度/锐度上下阈值设置
    SET_ETH = 0x9,  // 边界强度上下阈值设置
    SET_EGAIN = 0xa,  // RGB通道亮度/锐度增强模块增益设置
} RGBA_CMD_KEY;

typedef struct _RGBA_ABAL_CFG_S_
{
    FH_UINT8 abalSpeed;  // Abal的调整速度 | [0~0xf]
    FH_UINT8 abalThl;  // Abal不调整的阈值 | [0~0xf]
    FH_UINT8 abalThh;  // Abal开始调整的阈值 | [0~0xf]
    FH_UINT8 abalStabTime;  // Abal的稳定时间 | [0~0xff]
    FH_UINT8 abalStep;  // ABal调整的步长 | [0~0xf]
    FH_UINT8 abalTargetStabTime;  // AbalTarget的稳定时间 | [0~0xff]
    FH_UINT8 abalTargetThr;  // AbalTarget的调整阈值 | [0~0xf]
} RGBA_ABAL_CFG;

typedef struct _RGBA_WEIGHT_CFG_S_
{
    FH_UINT8  W2Speed;  // W2的调整速度 | [0~0xf]
    FH_UINT8  W2Thl;  // W2的不调整阈值 | [0~0xf]
    FH_UINT8  W2Thh;  // W2开始调整的阈值 | [0~0xf]
    FH_UINT8  W2StabTime;  // W2的稳定时间 | [0~0xf]
    FH_UINT8  W2Step;  // W2的调整步长 | [0~0xf]
    FH_UINT8  W2AThl;  // W2计算时增益的上阈值 | [0~0x7f]
    FH_UINT8  W2AThh;  // W2计算时增益的下阈值 | [0~0x7f]
    FH_UINT16 W2GThl;  // W2计算时红外的下阈值 | [0~0x7f]
    FH_UINT16 W2GThh;  // W2计算时红外的上阈值 | [0~0x7f]
} RGBA_WEIGHT_CFG;

typedef struct _RGBA_CCM_TABLE_S_
{
    FH_UINT16 RgbaCcmTable[2][16];  // rgba ccm矩阵系数 | [-4096~4095]
} RGBA_CCM_TABLE;

typedef struct _RGBA_CONVERT_TH_S_
{
    FH_UINT16 convThl;  // 格式转换模块LUT上阈值 | [0~0xffff]
    FH_UINT16 convThh;  // 格式转换模块LUT下阈值 | [0~0xffff]
} RGBA_CONVERT_TH;

typedef struct _RGBA_ENHANCE_TH_S_
{
    FH_UINT16 enhThl;  // 亮度/锐度增强模块亮度下阈值 | [0~0xffff]
    FH_UINT16 enhThh;  // 亮度/锐度增强模块亮度上阈值 | [0~0xffff]
} RGBA_ENHANCE_TH;

typedef struct _RGBA_ETH_S_
{
    FH_UINT16 eThl;  // 边界强度下限 | [0~0xffff]
    FH_UINT16 eThh;  // 边界强度上限 | [0~0xffff]
} RGBA_ETH;

typedef struct _RGBA_EGAIN_CFG_S_
{
    FH_UINT8 egainR;  // 亮度/锐度增强模块R通道增强增益 | [0~0xff]
    FH_UINT8 egainG;  // 亮度/锐度增强模块G通道增强增益 | [0~0xff]
    FH_UINT8 egainB;  // 亮度/锐度增强模块B通道增强增益 | [0~0xff]
} RGBA_EGAIN_CFG;

typedef struct _RGBA_DEFAULT_CFG_S_
{
    FH_BOOL         bRgbaEn;  // RGBA控制使能 | [0~1]
    FH_BOOL         bIrmode;  // Abal是否使能 | [0~1]
    FH_BOOL         bFirFrm;  // 是否是第一帧标志 | [0~1]
    RGBA_ABAL_CFG   abalParam;  // Abal参数结构体 | [RGBA_ABAL_CFG]
    RGBA_WEIGHT_CFG weightParam;  // W2参数结构体 | [RGBA_WEIGHT_CFG]
    RGBA_CCM_TABLE  ccmTab;  // ccm矩阵表 | [RGBA_CCM_TABLE]
    RGBA_CONVERT_TH convTh;  // 格式转换LUT阈值参数结构体 | [RGBA_CONVERT_TH]
    RGBA_ENHANCE_TH enhTh;  // 亮度/锐度阈值参数结构体 | [RGBA_ENHANCE_TH]
    RGBA_ETH        eTh;  // 边界强度参数结构体 | [RGBA_ETH]
    RGBA_EGAIN_CFG  egain;  // 分通道亮度/锐度增强模块增益参数结构体 | [RGBA_EGAIN_CFG]
} RGBA_DEFAULT_CFG;

typedef struct _RGBA_CURR_INFO_S_
{
    FH_UINT16 u08RBalGainPre;  // 上一帧RBalGain | [0~0xffff]
    FH_UINT8  u08RBalCnt;  // 当前RBal稳定时间计数 | [0~0xff]
    FH_UINT8  u08RBalTargetCnt;  // 当前RBal目标值的稳定时间计数 | [0~0xff]
    FH_UINT16 u16W2Pre;  // 上一帧CCM2权重 | [0~0x7fff]
    FH_UINT16 u16W2Cnt;  // W2的稳定时间计数 | [0~0xff]
} RGBA_CURR_INFO;

typedef struct _RGBA_STAT_INFO_S_
{
    struct rgba_blk_info
    {
        FH_UINT32 R_sum;  // RGBA统计值R sum | [0~0xffffffff]
        FH_UINT32 G_sum;  // RGBA统计值G sum | [0~0xffffffff]
        FH_UINT32 B_sum;  // RGBA统计值B sum | [0~0xffffffff]
        FH_UINT32 A_sum;  // RGBA统计值A sum | [0~0xffffffff]
        FH_UINT32 IR_sum;  // RGBA统计值IR sum | [0~0xffffffff]
        FH_UINT32 cnt;  // RGBA统计值CNT | [0~0xffffffff]
    } blk_rgba[32];
} RGBA_STAT_INFO;

typedef struct _RGBA_DPC_CFG_S
{
    FH_BOOL   bRGBADpcEn;  // RGBA模式下的dpc控制使能 | [0~1]
    FH_BOOL   bGainMappingEn;  // 线性映射使能:0 手动，1 自动 | [0~1]
    FH_UINT8  u08DpcMode;  // 0:all close;1:white pixel correct open;2.black pixel correct open;3.all open. | [0~3]
    FH_UINT8  u08ModeIdx;  // 去坏点强度 | [0~2]
    FH_UINT8  u08Str;  // 手动dpc强度 | [0~7]
    FH_UINT8  u08wdc;  // 手动白点門限值 | [0~0xff]
    FH_UINT8  u08bdc;  // 手动黑点門限值 | [0~0xff]
    FH_UINT8  u08ThhIR;  // IR通道比值上阈值 | [0~0x3f]
    FH_UINT8  u08ThlIR;  // IR通道比值下阈值 | [0~0xf]
    FH_UINT8  u08ThhG;  // G通道比值上阈值 | [0~0x3f]
    FH_UINT8  u08ThlG;  // G通道比值下阈值 | [0~0xf]
    FH_UINT16 u16DiffMin;  // 变化程度的最小值 | [0~0x3ff]
    FH_UINT8  u08DiffShift;  // 变化程度的移位值 | [0~0xf]
    FH_UINT8  u08Strenght[GAIN_NODES];  // 包括:mode,w_s,b_s | [0~0xff]
    FH_UINT8  u08WhiteThr[GAIN_NODES];  // 白点门限DC值 | [0~0xff]
    FH_UINT8  u08BlackThr[GAIN_NODES];  // 黑点门限值 | [0~0xff]
} RGBA_DPC_CFG;

/**|FAST BOOT|**/
typedef struct _SMART_IR_CFG_S {
    FH_UINT16 u16gainDay2Night;  // 白天切换到夜晚的增益門限，bayer sensor | [0~0xffff]
    FH_UINT16 u16gainNight2Day;  // 夜晚切换到白天的增益門限，bayer sensor | [0~0xffff]
    FH_UINT16 u16lumaDay2Night;  // 白天切换到夜晚的增益門限，rgbir sensor | [0~0xffff]
    FH_UINT16 u16lumaNight2Day;  // 夜晚切换到白天的增益門限，rgbir sensor | [0~0xffff]
} SMART_IR_CFG;

typedef struct _BLC_INIT_CFG_S {
    FH_UINT16 blc_level;  // 减黑电平值 | [0~0xfff]
} BLC_INIT_CFG;

typedef struct _WB_INIT_CFG_S {
    FH_UINT16 wbGain[3];  // 白平衡三通道增益值rgain, ggain, bgain; | [0~0x1fff]
} WB_INIT_CFG;

typedef struct _CCM_INIT_CFG_S {
    FH_UINT16 ccmCfg[9];  // 颜色矫正矩阵 | [-4096~4095]
} CCM_INIT_CFG;

typedef struct _DPC_INIT_CFG_S {
    FH_UINT8 ctrlMode;  // 去坏点模式 | [0~3]
    FH_UINT8 str;  // 去坏点比例門限值 | [0~7]
} DPC_INIT_CFG;

typedef struct _APC_INIT_CFG_S {
    FH_UINT8 edgeStr;  // 边界锐化增益, U4.4 | [0~0xff]
    FH_UINT8 detailStr;  // 细节增强增益, U4.4 | [0~0xff]
    FH_UINT8 positiveStr;  // 总体正向增益, U4.4 | [0~0xff]
    FH_UINT8 negativeStr;  // 总体负向增益, U4.4 | [0~0xff]
} APC_INIT_CFG;

typedef struct _YNR_INIT_CFG_S {
    FH_UINT8 th_str[3];  // th_str[0] 高频亮度阈值, th_str[1] 中频亮度阈值, th_str[2] 低频亮度阈值 | [0~0x3ff]
} YNR_INIT_CFG;

typedef struct _CTR_INIT_CFG_S {
    FH_UINT16 ctr;  // 对比度增强因子,U2.6 | [0~255]
} CTR_INIT_CFG;

typedef struct _SAT_INIT_CFG_S {
    FH_UINT16 sat;  // 饱和度调整因子,U3.5 | [0~0xff]
    FH_UINT16 blue_surp;  // 饱和度蓝色分量,U2.6 | [0~200]
    FH_UINT16 red_surp;  // 饱和度红色分量,U2.6 | [0~200]
} SAT_INIT_CFG;

typedef struct _GAMMA_MODE_INIT_CFG_S {
    FH_UINT16 gamma_mode;  // 当前gamma运行模式 | [0~3]
} GAMMA_MODE_INIT_CFG;

typedef struct _GAMMA_INIT_CFG_S {
    FH_UINT32 gamma[80];  // gamma表对应的值 | [0~0xffffffff]
} GAMMA_INIT_CFG;

/**|MIRROR|**/
typedef struct mirror_cfg_s
{
    FH_BOOL             bEN;  // 镜像使能开关 | [0~1]
    ISP_BAYER_TYPE      normal_bayer;  // 无效成员变量 | [0~3]
    ISP_BAYER_TYPE      mirror_bayer;  //　无效成员变量 | [0~3]
}MIRROR_CFG_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*_ISP_COMMON_H_*/
