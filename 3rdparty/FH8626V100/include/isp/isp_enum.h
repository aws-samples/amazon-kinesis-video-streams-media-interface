#ifndef ISP_ENUM_H_
#define ISP_ENUM_H_

#ifdef OS_RTT
#define ISP_WAIT_VD				0
#define ISP_WAIT_FRAME_EDGE		1
#define ISP_PROC_RUN			2
#define ISP_GET_VERSION			3
#define ISP_GET_AE_STAT		    4
#define ISP_GET_AWB_STAT        5
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

enum  IPB_LTM_CURVE_TYPE{
	LTM_CURVE_LINEAR = 0x0,
	LTM_CURVE_60DB = 0x1,
	LTM_CURVE_80DB = 0x2,
	LTM_CURVE_100DB = 0x3,
	LTM_CURVE_120DB = 0x4,
	LTM_CURVE_USER01 = 0x5,
	LTM_CURVE_USER02 = 0x6,
	LTM_CURVE_USER03 = 0x7,
	LTM_MANUAL_CURVE = 0xF,
};

enum  MISC_DATA_COMP{
	MISC_COMP_NONE = 0x0,
	MISC_COMP_LINEAR_12BIT = 0x1,
	MISC_COMP_LINEAR_10BIT = 0x2,
	MISC_COMP_NORMAL = 0x3,
	MISC_COMP_USER = 0x4,
};

/*-------format setting------------*/
/**
 * @brief bit[0]~bit[7]　传入帧率设置([0~199]12.5fps,15fps,20fps,25fps,30fps,40fps,50fps,60fps,120fps,240fps,220fps,110fps...;[200~255]user defined)
 *        bit[8]~bit[15]　传入幅面设置([0~199]720P,960P,1080P,1600*1200,1536*1536,1536P,400w,500w,800w,270P,540P,192*192,384*384;[200~255]:user defined)
 *        bit[16]~bit[23]　传入sensor属性(目前bit16配０代表非宽动态，１代表宽动态)
 */

enum FORMAT
{
    FORMAT_720P15       = 0x02,
    FORMAT_720P25       = 0x03,
    FORMAT_720P30       = 0x04,
    FORMAT_960P25       = 0x0103,
    FORMAT_960P30       = 0x0104,
    FORMAT_1080P15      = 0x0201,
    FORMAT_1080P20      = 0x0202,  // not support
    FORMAT_1080P25      = 0x0203,  // not support
    FORMAT_1080P30      = 0x0204,  // not support
    FORMAT_1200P30      = 0x0304,  // not support
    FORMAT_1536X1536P15 = 0x0401,
    FORMAT_1536X1536P25 = 0x0403,  // not support
    FORMAT_1536X1536P30 = 0x0404,  // not support
    FORMAT_1536P15      = 0x0501,  // not support
    FORMAT_1536P25      = 0x0503,  // not support
    FORMAT_1536P30      = 0x0504,  // not support
    FORMAT_1536P40      = 0x0505,  // not support
    FORMAT_400WP15      = 0x0601,  // not support
    FORMAT_400WP20      = 0x0602,  // not support
    FORMAT_400WP25      = 0x0603,  // not support
    FORMAT_400WP30      = 0x0604,  // not support
    FORMAT_500W12P5     = 0x0700,  // not support
    FORMAT_500WP15      = 0x0701,  // not support
    FORMAT_500WP20      = 0x0702,  // not support
    FORMAT_800WP15      = 0x0801,  // not support
    FORMAT_270P240      = 0x0909,
    FORMAT_540P120      = 0x0a08,  // not support
    FORMAT_192X192P220  = 0x0c0a,
    FORMAT_384X384P110  = 0x0d0b,
    FORMAT_WDR_1080P25  = 0x10203,  // not support
    FORMAT_WDR_1080P30  = 0x10204,  // not support
    FORMAT_WDR_1536P15  = 0x10501,  // not support
    FORMAT_WDR_1536P25  = 0x10503,  // not support
    FORMAT_WDR_1536P30  = 0x10504,  // not support
    FORMAT_WDR_1536P40  = 0x10505,  // not support
    FORMAT_WDR_400WP20  = 0x10602,  // not support
    FORMAT_WDR_400WP25  = 0x10603,  // not support
    FORMAT_WDR_400WP30  = 0x10604,  // not support
    FORMAT_WDR_500W12P5 = 0x10700,  // not support
    FORMAT_WDR_500WP15  = 0x10701,  // not support
    FORMAT_WDR_500WP20  = 0x10702,  // not support
    FORMAT_WDR_800WP15  = 0x10801,  // not support
    SENSOR_FORMAT_MAX   = 0xfffff,
};

enum MOD
{
	TWO_BLOCK_MODE = 0,
	DIRECT_MODE = 1,
	LEFT_DIRECT_RIGHT_DRR = 2,
	FOUR_BLOCK_MODE = 4,
};

enum ISPP_MODE
{
	ISPP_CIRCULAR = 0,
	ISPP_NON_CIRCULAR = 1,
};

enum CHIP_TYPE
{
    FHZY  = 0,
    FHZY2 = 1,
};

enum AWB_MODULE_POS
{
	POST_WB = 0,
	FRONT_SHORT_WB = 1,
	FRONT_LONG_WB = 2,
};

enum SNS_IF
{
    DEFAULT_CASE = 0,
};

enum ISP_MODULE_ENABLE_CFG0
{
    ISP_CLP             =   (1 << 5),
    ISP_RGBAIR          =   (1 << 8),
    ISP_STATIR          =   (1 << 9),
    ISP_LUT12           =   (1 << 10),
    ISP_RGBLUT          =   (1 << 11),
    ISP_GAIN            =   (1 << 12),
    ISP_STAT_GLB        =   (1 << 13),
    ISP_STAT_WB0        =   (1 << 14),
    ISP_STAT_WB1        =   (1 << 15),
    ISP_STAT_MD         =   (1 << 16),
    ISP_WB              =   (1 << 17),
    ISP_LTM             =   (1 << 18),
    ISP_DPC             =   (1 << 19),
    ISP_GB              =   (1 << 20),
    ISP_RGBX            =   (1 << 21),
    ISP_HMIR            =   (1 << 22),
    ISP_NR3D            =   (1 << 23),
    ISP_POST_STAT_GL    =   (1 << 24),
    ISP_POST_WB         =   (1 << 25),
    ISP_MV_COEF         =   (1 << 26),
};

enum ISP_MODULE_ENABLE_CFG1
{
    ISP_CFA             =   (1 << 0),
    ISP_FCCFA           =   (1 << 1),
    ISP_HLR             =   (1 << 2),
    ISP_RGBB            =   (1 << 3),
    ISP_CGAMMA          =   (1 << 4),
    ISP_YNR             =   (1 << 5),
    ISP_CNR             =   (1 << 6),
    ISP_APC             =   (1 << 7),
    ISP_PURPLE          =   (1 << 8),
    ISP_STAT_SHARPEN    =   (1 << 9),
    ISP_YGAMMA          =   (1 << 10),
    ISP_CHROMA          =   (1 << 11),
    ISP_LOCALCHROMA     =   (1 << 12),
};

enum ISP_BYPASS_OR_ENABLE
{
    ISP_BYPASS = 0,
    ISP_ENABLE = 1,
};
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* ISP_ENUM_IF_H_ */
