#ifndef _ISP_API_H_
#define _ISP_API_H_


#include "isp_common.h"
#include "isp_sensor_if.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define lift_shift_bit_num(bit_num)			(1<<bit_num)
// #define DEBUG_API 1

#define CHECK_VALIDATION(x,a,b)	\
do {							\
	int r;						\
	if( (x<a) || (x>b))			\
	{							\
		if (x<a)				\
			r=a;				\
		else					\
			r=b;				\
		fh_printf("[WARNING]parameter out of range @%s %s= %d | range=[%d,%d] | auto clip to %d\n",__func__,#x,x,a,b,r);\
		x=r;					\
	}							\
}while(0)

#define TRACE_API ispTrace_FunctionName(__func__)

#define FUNC_DEP __attribute__((deprecated))

enum ISP_HW_MODULE_LIST
{
    HW_MODUL_CLOSE_ALL          = 0,
    HW_MODUL_RGBA_IR            = lift_shift_bit_num(0),
    HW_MODUL_STATIR             = lift_shift_bit_num(1),
    HW_MODUL_GAIN               = lift_shift_bit_num(2),
    HW_MODUL_GLOBE_STAT         = lift_shift_bit_num(3),
    HW_MODUL_STAT_WB0           = lift_shift_bit_num(4),
    HW_MODUL_STAT_WB1           = lift_shift_bit_num(5),
    HW_MODUL_STAT_MD            = lift_shift_bit_num(6),
    HW_MODUL_WB                 = lift_shift_bit_num(7),
    HW_MODUL_LTM                = lift_shift_bit_num(8),
    HW_MODUL_DPC                = lift_shift_bit_num(9),
    HW_MODUL_GB                 = lift_shift_bit_num(10),
    HW_MODUL_RGBX               = lift_shift_bit_num(11),
    HW_MODUL_HMIRROR            = lift_shift_bit_num(12),
    HW_MODUL_NR3D               = lift_shift_bit_num(13),
    HW_MODUL_POST_GLOBE_STAT    = lift_shift_bit_num(14),
    HW_MODUL_POST_WB            = lift_shift_bit_num(15),
    HW_MODUL_MV_COEF            = lift_shift_bit_num(16),

    HW_MODUL_CFA                = lift_shift_bit_num(17),
    HW_MODUL_FCCFA              = lift_shift_bit_num(18),
    HW_MODUL_HLR                = lift_shift_bit_num(19),
    HW_MODUL_RGBB               = lift_shift_bit_num(20),
    HW_MODUL_CGAMMA             = lift_shift_bit_num(21),
    HW_MODUL_YNR                = lift_shift_bit_num(22),
    HW_MODUL_CNR                = lift_shift_bit_num(23),
    HW_MODUL_APC                = lift_shift_bit_num(24),
    HW_MODUL_PURPLE             = lift_shift_bit_num(25),
    HW_MODUL_STAT_SHARPEN       = lift_shift_bit_num(26),
    HW_MODUL_YGAMMA             = lift_shift_bit_num(27),
    HW_MODUL_CHROMA             = lift_shift_bit_num(28),
    HW_MODUL_LOCAL_CHROMA       = lift_shift_bit_num(29),
    HW_MODUL_RESERVED           = lift_shift_bit_num(30),
    // These modules are not supported. They are reserved for compatibility.
    HW_MODUL_WBS                = HW_MODUL_RESERVED + 1,
    HW_MODUL_WBL                = HW_MODUL_RESERVED + 2,
    HW_MODUL_WDR                = HW_MODUL_RESERVED + 3,
    HW_MODUL_LSC                = HW_MODUL_RESERVED + 4,
    HW_MODUL_NR2D               = HW_MODUL_RESERVED + 5,
    HW_MODUL_POST_GAIN          = HW_MODUL_RESERVED + 6,
    HW_MODUL_STAT_HIST          = HW_MODUL_RESERVED + 7,
};
/**SYSTEM CONTROL**/
FH_SINT32 FUNC_DEP API_ISP_GetBuffSize(unsigned int width, unsigned int height);
/*
*   Name: API_ISP_MemInit
*            ISP内部使用的memory分配与初始化
*
*   Parameters:
*
*       [IN]  FH_UINT32 width
*            ISP处理的幅面宽度
*
*       [IN]  FH_UINT32 height
*            ISP处理的幅面高度
*
*   Return:
*            0(正确)
*           -1(ISP设备驱动打开失败)
*/
FH_SINT32 API_ISP_MemInit( FH_UINT32 width, FH_UINT32 height );
/*
*   Name: API_ISP_GetBinAddr
*            获取ISP的param参数的地址和大小
*
*   Parameters:
*
*       [OUT] FH_UINT32 *u32BinAddr
*            param的地址
*
*       [OUT] FH_UINT32 *u32BinSize
*            param的size
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetBinAddr(FH_UINT32 *u32BinAddr,FH_UINT32 *u32BinSize);
FH_SINT32 FUNC_DEP API_ISP_SetViHwAttr(const ISP_VI_HW_ATTR *pstViHwAttr);
/*
*   Name: API_ISP_SetCisClk
*            配置供给sensor的时钟
*
*   Parameters:
*
*       [IN] FH_UINT32 cisClk
*            需要配置时钟频率值
*
*   Return:
*            正确则返回0,错误则返回ERROR_ISP_SET_CIS_CLK
*/
FH_SINT32 API_ISP_SetCisClk(FH_UINT32 cisClk);
/*
*   Name: API_ISP_CisClkEn
*            供给sensor的时钟使能配置
*
*   Parameters:
*
*       [IN] FH_BOOL bEn
*            1表示使能时钟，0表示关闭时钟。
*
*   Return:
*            0(正确)，错误(ERROR_ISP_SET_CIS_CLK)
*/
FH_SINT32 API_ISP_CisClkEn(FH_BOOL bEn);
/*
*   Name: API_ISP_SetViAttr
*            配置vi相关的一些幅面信息
*
*   Parameters:
*
*       [IN] const ISP_VI_ATTR_S *pstViAttr
*            结构体ISP_VI_ATTR_S的指针
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetViAttr(const ISP_VI_ATTR_S *pstViAttr);
/*
*   Name: API_ISP_GetViAttr
*            获取当前幅面相关信息
*
*   Parameters:
*
*       [OUT] ISP_VI_ATTR_S *pstViAttr
*            结构体ISP_VI_ATTR_S的指针
*
*   Return:
*            0(正确)，
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetViAttr(ISP_VI_ATTR_S *pstViAttr);
FH_SINT32 FUNC_DEP API_ISP_GetIspMode(FH_UINT32 *mode);
/*
*   Name: API_ISP_Init
*            初始化ISP硬件寄存器，并启动ISP
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*        -1003(ISP初始化异常)
*/
FH_SINT32 API_ISP_Init(FH_VOID);
FH_SINT32 FUNC_DEP API_ISP_SetIspParam(ISP_DEFAULT_PARAM *pstDefaultParam);
/*
*   Name: API_ISP_LoadIspParam
*            加载指定参数到DRV寄存器
*
*   Parameters:
*
*       [IN] char *isp_param_buff
*            指定参数的指针
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_LoadIspParam(char *isp_param_buff);
FH_SINT32 FUNC_DEP API_ISP_CLEAN_CIRCLUAR_BUFF(FH_VOID);
/*
*   Name: API_ISP_Pause
*            暂停ISP对图像的处理与输出
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_Pause(FH_VOID);
FH_SINT32 FUNC_DEP API_ISP_SoftReset(FH_VOID);
FH_SINT32 FUNC_DEP API_ISP_KickStart(FH_VOID);
/*
*   Name: API_ISP_Resume
*            恢复ISP对图像处理与输出
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_Resume(FH_VOID);
/*
*   Name: API_ISP_DetectPicSize
*            检测幅面信息是否符合预期
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)，错误(ERROR_ISP_DETECT_NO_INPUT, ERROR_ISP_UNEXPECTED_PIC_SIZE)
*/
FH_SINT32 API_ISP_DetectPicSize(FH_VOID);
/*
*   Name: API_ISP_Run
*            ISP策略处理
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)，
*           -1(图像丢失)
*/
FH_SINT32 API_ISP_Run(FH_VOID);
/*
*   Name: API_ISP_AE_AWB_Run
*            ISP AE&AWB策略处理
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*   0xa0074100(图像丢失)
*/
FH_SINT32 API_ISP_AE_AWB_Run(FH_VOID);
/*
*   Name: API_ISP_Exit
*            ISP线程退出，清理状态
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_Exit(FH_VOID);
/*
*   Name: API_ISP_Set_HWmodule_cfg
*            配置ISP模块硬件使能
*
*   Parameters:
*
*       [IN] FH_UINT32 u32modulecfg
*            每1bit对应一个硬件使能位0表示关闭，1表示打开，详见枚举ISP_HW_MODULE_LIST。
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_Set_HWmodule_cfg(FH_UINT32 u32modulecfg);
/*
*   Name: API_ISP_Get_HWmodule_cfg
*            获取当前ISP模块硬件状态
*
*   Parameters:
*
*       [IN] FH_UINT32 *u32modulecfg
*            每1bit对应一个硬件使能位0表示关闭，1表示打开，详见枚举ISP_HW_MODULE_LIST。
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_Get_HWmodule_cfg(FH_UINT32 *u32modulecfg);
/*
*   Name: API_ISP_GetAlgCtrl
*            获取当前DRV策略控制器配置值。
*
*   Parameters:
*
*       [OUT] FH_UINT32 *u32AlgCtrl
*            存放当前DRV策略控制的值。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAlgCtrl(FH_UINT32 *u32AlgCtrl);
/*
*   Name: API_ISP_SetAlgCtrl
*            配置当前DRV策略控制器配置值。
*
*   Parameters:
*
*       [IN] FH_UINT32 u32AlgCtrl
*            目标设置的DRV　FUNC_CTRL寄存器值。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetAlgCtrl(FH_UINT32 u32AlgCtrl);

/*
*   Name: API_ISP_Get_Determined_HWmodule
*            获取指定模块开关状态
*
*   Parameters:
*
*       [OUT] HW_MODULE_CFG *pstModuleCfg
*            类型为HW_MODULE_CFG的结构体指针，详细成员变量请查看HW_MODULE_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3004(传入空指针或者传入指定位不在枚举列表中)
*/
FH_SINT32 API_ISP_Get_Determined_HWmodule(HW_MODULE_CFG *pstModuleCfg);
/*
*   Name: API_ISP_Set_Determined_HWmodule
*            配置指定位的ISP硬件使能开关
*
*   Parameters:
*
*       [IN] HW_MODULE_CFG *pstModuleCfg
*            类型为HW_MODULE_CFG的结构体指针，详细成员变量请查看HW_MODULE_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3004(传入空指针或者传入指定位不在枚举列表中)
*/
FH_SINT32 API_ISP_Set_Determined_HWmodule(HW_MODULE_CFG *pstModuleCfg);
FH_SINT32 FUNC_DEP API_ISP_WaitVD(FH_VOID);
FH_SINT32 FUNC_DEP API_ISP_GetVersion(ISP_VERSION *pstIspVersion);
FH_SINT32 FUNC_DEP API_ISP_SetIspReg(FH_UINT32 u32RegAddr,FH_UINT32 u32RegData);
FH_SINT32 FUNC_DEP API_ISP_GetIspReg(FH_UINT32 u32RegAddr,FH_UINT32 *u32RegData);
/*
*   Name: API_ISP_GetRaw
*            获取ISP　NR3D处的raw数据
*
*   Parameters:
*
*       [IN] FH_BOOL strategy_en
*            关闭硬件影响图像效果模块(0)，打开硬件影响图像效果模块(1)
*
*       [OUT]  FH_VOID* pRawBuff
*            存放raw数据的地址
*
*       [IN]  FH_UINT32 u32Size
*            存放raw数据buffer大小
*
*       [IN]  FH_UINT32 u32FrameCnt
*            需要连续导出的帧数
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*        -1009(buffer大小小于预期)
*/
FH_SINT32 API_ISP_GetRaw(FH_BOOL strategy_en, FH_VOID* pRawBuff, FH_UINT32 u32Size, FH_UINT32 u32FrameCnt);
/*
*   Name: API_ISP_CheckStatReady
*            查询硬件统计状态信息是否就绪
*
*   Parameters:
*
*       [OUT] FH_BOOL *isReady
*            统计就绪(0)，统计未就绪(1)
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_CheckStatReady(FH_BOOL *isReady);
/*
*   Name: API_ISP_GetVIState
*            获取当前ISP的一些运行状态信息
*
*   Parameters:
*
*       [OUT] ISP_VI_STAT_S *pstStat
*            类型为ISP_VI_STAT_S的结构体指针，详细成员变量请查看ISP_VI_STAT_S结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetVIState(ISP_VI_STAT_S *pstStat);
/*
*   Name: API_ISP_Dump_Param
*            拷贝所有DRV寄存器值到指定地址
*
*   Parameters:
*
*       [OUT] FH_UINT32 *addr
*            存放DRV参数的地址
*
*       [IN] FH_UINT32 *size
*            拷贝数据的大小
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_Dump_Param(FH_UINT32 *addr,FH_UINT32 *size);
FH_SINT32 FUNC_DEP API_ISP_GetDebugInfo(ISP_DEBUG_INFO *pstIspDebug);
/*
*   Name: *FH_ISP_Version
*            获取、打印ISP版本号。
*
*   Parameters:
*
*       [IN] FH_UINT32 print_enable
*            打印ISP库版本号(1)，不打印ISP库版本号(0)
*
*   Return:
*            ISP库版本号的字符串。
*/
FH_CHAR *FH_ISP_Version(FH_UINT32 print_enable);
/*
*   Name: *FH_ISPCORE_Version
*            获取、打印ISP版本号。
*
*   Parameters:
*
*       [IN] FH_UINT32 print_enable
*            打印ISP_CORE库的版本号(1)，不打印ISP_CORE库的版本号(0)
*
*   Return:
*            ISP_CORE库的版本号的字符串。
*/
FH_CHAR *FH_ISPCORE_Version(FH_UINT32 print_enable);
/**SENSOR CONTROL**/
/*
*   Name: API_ISP_SensorInit
*            sensor预初始化，并未配置sensor寄存器
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确)
*           -1(Sensor的回调函数未被初始化)
*/
FH_SINT32 API_ISP_SensorInit(FH_VOID);
/*
*   Name: API_ISP_SetSensorFmt
*            初始化sensor
*
*   Parameters:
*
*       [IN] FH_UINT32 format
*            传入的时枚举FORMAT中的枚举值，选择幅面
*
*   Return:
*            0(正确)
*        -3005(输入的制式不支持)
*     a0074006(Sensor的回调函数未被初始化)
*     a007400e(分配的内存不够)
*/
FH_SINT32 API_ISP_SetSensorFmt(FH_UINT32 format);

/*
*   Name: API_ISP_SensorKick
*            启动sensor输出，有的sensor需要用到
*
*   Parameters:
*
*            None
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*/
FH_SINT32 API_ISP_SensorKick(FH_VOID);
/*
*   Name: API_ISP_SetSensorIntt
*            配置sensor的曝光值
*
*   Parameters:
*
*       [IN] FH_UINT32 intt
*            传入的曝光值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)。
*/
FH_SINT32 API_ISP_SetSensorIntt(FH_UINT32 intt);
/*
*   Name: API_ISP_SetSensorGain
*            配置sensor增益值
*
*   Parameters:
*
*       [IN] FH_UINT32 gain
*            增益值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*/
FH_SINT32 API_ISP_SetSensorGain(FH_UINT32 gain);

/*
*   Name: API_ISP_SetSensorReg
*            配置sensor寄存器
*
*   Parameters:
*
*       [IN] FH_UINT16 addr
*            sensor寄存器地址
*
*       [IN] FH_UINT16 data
*            配置的值
*
*   Return:
*            0(正确返回)
*           -1(sensor相关的回调函数未被注册)
*/
FH_SINT32 API_ISP_SetSensorReg(FH_UINT16 addr,FH_UINT16 data);
/*
*   Name: API_ISP_SensorRegCb
*            拷贝sensor的回调函数到目标地址。
*
*   Parameters:
*
*       [IN] FH_UINT32 u32SensorId
*            无用
*
*       [IN]  struct isp_sensor_if *pstSensorFunc
*            类型为isp_sensor_if的结构体指针，详细成员变量请查看isp_sensor_if结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SensorRegCb(FH_UINT32 u32SensorId, struct isp_sensor_if *pstSensorFunc);
/*
*   Name: API_ISP_SensorUnRegCb
*            注销ISP中注册的sensor的回调函数。
*
*   Parameters:
*
*       [IN] FH_UINT32 u32SensorId
*            无用
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_SensorUnRegCb(FH_UINT32 u32SensorId);
/*
*   Name: API_ISP_GetSensorReg
*            读取指定的SENSOR寄存器
*
*   Parameters:
*
*       [IN] FH_UINT16 addr
*            目标读取地址
*
*       [OUT]  FH_UINT16 *data
*            存放读取到寄存器值的地址
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_GetSensorReg(FH_UINT16 addr, FH_UINT16 *data);
/*
*   Name: API_ISP_SetSensorFrameRate
*            配置SENSOR的垂直消隐至指定倍率
*
*   Parameters:
*
*       [IN] int m
*            垂直消隐的倍率
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_SetSensorFrameRate(int m);
/**  AE */
/*
*   Name: API_ISP_AEAlgEn
*            AE策略使能
*
*   Parameters:
*
*       [IN] FH_BOOL bEn
*            打开AE策略(0)，关闭AE策略(1)
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_AEAlgEn(FH_BOOL bEn);
/*
*   Name: API_ISP_AESendCmd
*            配置AE的DRV寄存器
*
*   Parameters:
*
*       [IN] FH_UINT32 u32Cmd
*            配置的寄存器类型，详见枚举变量AE_CMD_KEY。
*
*       [IN] FH_VOID *param
*            配置的参数指针，传入的大小和类型根据u32Cmd改变。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_AESendCmd(FH_UINT32 u32Cmd,FH_VOID *param);
/*
*   Name: API_ISP_SetAeStatWin
*            配置AE统计窗口
*
*   Parameters:
*
*       [IN] ISP_AE_STAT_WIN *pstAeStatWin
*            类型为ISP_AE_STAT_WIN的结构体指针，详细成员变量请查看ISP_AE_STAT_WIN结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetAeStatWin(ISP_AE_STAT_WIN *pstAeStatWin);
/*
*   Name: API_ISP_GetAeStatWin
*            获取当前AE统计窗口配置
*
*   Parameters:
*
*       [IN] ISP_AE_STAT_WIN *pstAeStatWin
*            类型为ISP_AE_STAT_WIN的结构体指针，详细成员变量请查看ISP_AE_STAT_WIN结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAeStatWin(ISP_AE_STAT_WIN *pstAeStatWin);
/*
*   Name: API_ISP_GetAeStat
*            获取AE统计值
*
*   Parameters:
*
*       [OUT] ISP_AE_STAT *pstAeStat
*            类型为ISP_AE_STAT的结构体指针，详细成员变量请查看ISP_AE_STAT结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAeStat(ISP_AE_STAT *pstAeStat);
/*
*   Name: API_ISP_SetAeInfo
*            配置ISP_AE_INFO的内容到对应的寄存器去。
*
*   Parameters:
*
*       [IN] const ISP_AE_INFO *pstAeInfo
*            类型为ISP_AE_INFO的结构体指针，详细成员变量请查看ISP_AE_INFO结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetAeInfo(const ISP_AE_INFO *pstAeInfo);
/*
*   Name: API_ISP_GetAeInfo
*            从寄存器获取当前AE曝光信息。
*
*   Parameters:
*
*       [OUT] ISP_AE_INFO *pstAeInfo
*            类型为ISP_AE_INFO的结构体指针，详细成员变量请查看ISP_AE_INFO结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAeInfo(ISP_AE_INFO *pstAeInfo);
/*
*   Name: API_ISP_GetAeCfg
*            获取当前DRV寄存器中AE部分的配置参数。
*
*   Parameters:
*
*       [OUT] AE_DEFAULT_CFG *pstAeDefaultCfg
*            类型为AE_DEFAULT_CFG的结构体指针，详细成员变量请查看AE_DEFAULT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAeCfg(AE_DEFAULT_CFG *pstAeDefaultCfg);
/*
*   Name: API_ISP_RegisterAeOpStatusCallback
*            注册AE中的回调函数
*
*   Parameters:
*
*       [IN] AeOpStatusCallback cb
*            类型为AeOpStatusCallback的结构体指针，详细成员变量请查看AeOpStatusCallback结构体定义。
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_RegisterAeOpStatusCallback(AeOpStatusCallback cb);
/*
*   Name: API_ISP_SetAwbStatWin
*            配置AWB统计窗口
*
*   Parameters:
*
*       [IN] ISP_AWB_STAT_WIN *pstAwbStatWin
*            类型为ISP_AWB_STAT_WIN的结构体指针，详细成员变量请查看ISP_AWB_STAT_WIN结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
/** AWB **/
FH_SINT32 API_ISP_SetAwbStatWin(ISP_AWB_STAT_WIN *pstAwbStatWin);
/*
*   Name: API_ISP_GetAwbStatWin
*            获取当前AWB统计窗口信息
*
*   Parameters:
*
*       [OUT] ISP_AWB_STAT_WIN *pstAwbStatWin
*            类型为ISP_AWB_STAT_WIN的结构体指针，详细成员变量请查看ISP_AWB_STAT_WIN结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAwbStatWin(ISP_AWB_STAT_WIN *pstAwbStatWin);
/*
*   Name: API_ISP_GetAwbStat
*            获取当前帧AWB0统计值。
*
*   Parameters:
*
*       [OUT] const ISP_AWB_INFO *pstAwbInfo
*            类型为ISP_AWB_INFO的结构体指针，详细成员变量请查看ISP_AWB_INFO结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAwbStat(const ISP_AWB_INFO *pstAwbInfo);
/*
*   Name: API_ISP_GetAwb1Stat
*            获取当前帧AWB1统计值。
*
*   Parameters:
*
*       [OUT] const ISP_AWB_INFO *pstAwbInfo
*            类型为ISP_AWB_INFO的结构体指针，详细成员变量请查看ISP_AWB_INFO结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAwb1Stat(const ISP_AWB_INFO  *pstAwbInfo);
FH_SINT32 FUNC_DEP API_ISP_GetLongAwbStat(const ISP_AWB_FRONT_STAT * pstAwbFrontStat);
FH_SINT32 FUNC_DEP API_ISP_GetShortAwbStat(const ISP_AWB_FRONT_STAT * pstAwbFrontStat);
/*
*   Name: API_ISP_SetAwbCfg
*            更新结构体ISP_AWB_CFG中的内容到AWB相关硬件寄存器。
*
*   Parameters:
*
*       [IN] ISP_AWB_CFG *pstAwbCfg
*            类型为ISP_AWB_CFG的结构体指针，详细成员变量请查看ISP_AWB_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetAwbCfg(ISP_AWB_CFG *pstAwbCfg);
/*
*   Name: API_ISP_AWBAlgEn
*            打开AWB的策略使能开关
*
*   Parameters:
*
*       [IN] FH_BOOL bEn
*            使能(bEn=1)，关闭(bEn=0)
*
*   Return:
*            正确
*/
FH_SINT32 API_ISP_AWBAlgEn(FH_BOOL bEn);
/*
*   Name: API_ISP_AWBSendCmd
*            根据cmd执行不同的配置AWB的SWITCH CASE
*
*   Parameters:
*
*       [IN] FH_UINT32 u32Cmd
*            取值范围为AWB_CMD_KEY，每个值对应不同的sitch case.
*
*       [IN/OUT] FH_VOID *param
*            传入参数的地址或者时获取状态值的地址，输入输出类型根据cmd判断。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_AWBSendCmd(FH_UINT32 u32Cmd,FH_VOID *param);
/*
*   Name: API_ISP_GetAwbCfg
*            获取当前AWB的DRV配置
*
*   Parameters:
*
*       [OUT] AWB_DEFAULT_CFG *pstAwbDefaultCfg
*            类型为AWB_DEFAULT_CFG的结构体指针，详细成员变量请查看AWB_DEFAULT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAwbCfg(AWB_DEFAULT_CFG *pstAwbDefaultCfg);
/** GLOBE **/
/*
*   Name: API_ISP_GetGlobeStat
*            获取当前GLOBE统计
*
*   Parameters:
*
*       [OUT] GLOBE_STAT *pstGlobeStat
*            类型为GLOBE_STAT的结构体指针，详细成员变量请查看GLOBE_STAT结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetGlobeStat(GLOBE_STAT *pstGlobeStat);
FH_SINT32 FUNC_DEP API_ISP_SetGlobeStat(GLOBE_STAT_CFG *pstGlobeStat);
/*
*   Name: API_ISP_SetGlobeStatCfg
*            配置GLOBE统计窗
*
*   Parameters:
*
*       [IN] GLOBE_STAT_CFG *pstGlobeStat
*            类型为GLOBE_STAT_CFG的结构体指针，详细成员变量请查看GLOBE_STAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetGlobeStatCfg(GLOBE_STAT_CFG *pstGlobeStat);
/*
*   Name: API_ISP_GetGlobeStatCfg
*            获取GLOBE统计窗
*
*   Parameters:
*
*       [OUT] GLOBE_STAT_CFG *pstGlobeStat
*            类型为GLOBE_STAT_CFG的结构体指针，详细成员变量请查看GLOBE_STAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetGlobeStatCfg(GLOBE_STAT_CFG *pstGlobeStat);
/**  AF*/
FH_SINT32 FUNC_DEP API_ISP_AFlibRegCb(FH_VOID);
FH_SINT32 FUNC_DEP API_ISP_AFlibUnRegCb(FH_VOID);
FH_SINT32 FUNC_DEP API_ISP_AFAlgEn(FH_BOOL bEn);
FH_SINT32 FUNC_DEP API_ISP_AFSendCmd(FH_UINT32 u32Dev,FH_UINT32 u32Cmd,FH_VOID *param);
FH_SINT32 FUNC_DEP API_ISP_SetAFFilter(const ISP_AF_FILTER *pstAfFilter);
FH_SINT32 FUNC_DEP API_ISP_GetAFFilter(ISP_AF_FILTER *pstAfFilter);
FH_SINT32 FUNC_DEP API_ISP_SetAFWinInfo(const ISP_AF_WIN_INFO *pstAfWinInfo);
FH_SINT32 FUNC_DEP API_ISP_GetAFStat(ISP_AF_STAT *pstAfStat);
FH_SINT32 FUNC_DEP API_ISP_SetAFStatAddr(const ISP_AF_STAT_ADDR *pstAfStatAddr);
FH_SINT32 FUNC_DEP API_ISP_GetAFStatAddr(ISP_AF_STAT_ADDR *pstAfStatAddr);
/**LTM*/
/*
*   Name: API_ISP_SetLtmCfg
*            配置LTM的DRV寄存器值
*
*   Parameters:
*
*       [IN] ISP_LTM_CFG *pstLtmCfg
*            类型为ISP_LTM_CFG的结构体指针，详细成员变量请查看ISP_LTM_CFG结构体定义。
*
*   Return:
*        -3002(空指针异常)
*            0(正确)
*/
FH_SINT32 API_ISP_SetLtmCfg(ISP_LTM_CFG *pstLtmCfg);
/*
*   Name: API_ISP_GetLtmCfg
*            获取当前DRV中的LTM配置
*
*   Parameters:
*
*       [OUT] ISP_LTM_CFG *pstLtmCfg
*            类型为ISP_LTM_CFG的结构体指针，详细成员变量请查看ISP_LTM_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetLtmCfg(ISP_LTM_CFG *pstLtmCfg);
/**BLC**/
/*
*   Name: API_ISP_SetBlcAttr
*            配置BLC的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_BLC_ATTR *pstBlcAttr
*            类型为ISP_BLC_ATTR的结构体指针，详细成员变量请查看ISP_BLC_ATTR结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetBlcAttr(ISP_BLC_ATTR *pstBlcAttr);
/*
*   Name: API_ISP_GetBlcAttr
*            获取当前配置的BLC的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_BLC_ATTR *pstBlcAttr
*            类型为ISP_BLC_ATTR的结构体指针，详细成员变量请查看ISP_BLC_ATTR结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetBlcAttr(ISP_BLC_ATTR *pstBlcAttr);

/** GB*/
/*
*   Name: API_ISP_SetGbCfg
*            配置GB的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_GB_CFG *pstGbCfg
*            类型为ISP_GB_CFG的结构体指针，详细成员变量请查看ISP_GB_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetGbCfg(ISP_GB_CFG *pstGbCfg);
/*
*   Name: API_ISP_GetGbCfg
*            获取当前配置的GB的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_GB_CFG *pstGbCfg
*            类型为ISP_GB_CFG的结构体指针，详细成员变量请查看ISP_GB_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetGbCfg(ISP_GB_CFG *pstGbCfg);
/** DPC*/

FH_SINT32 FUNC_DEP API_ISP_SetStaticDpc(STATIC_DPC_CFG *pstStaticDpc);
FH_SINT32 FUNC_DEP API_ISP_GetStaticDpc(STATIC_DPC_CFG *pstStaticDpc);
/*
*   Name: API_ISP_SetDynamicDpc
*            配置动态DPC的DRV寄存器
*
*   Parameters:
*
*       [IN] DYNAMIC_DPC_CFG *pstDynamicDpc
*            类型为DYNAMIC_DPC_CFG的结构体指针，详细成员变量请查看DYNAMIC_DPC_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetDynamicDpc(DYNAMIC_DPC_CFG *pstDynamicDpc);
/*
*   Name: API_ISP_GetDynamicDpc
*            获取当前配置的动态DPC的DRV寄存器值
*
*   Parameters:
*
*       [OUT] DYNAMIC_DPC_CFG *pstDynamicDpc
*            类型为DYNAMIC_DPC_CFG的结构体指针，详细成员变量请查看DYNAMIC_DPC_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetDynamicDpc(DYNAMIC_DPC_CFG *pstDynamicDpc);
/** LSC*/

FH_SINT32 FUNC_DEP API_ISP_SetLscCfg(ISP_LSC_CFG *pstLscCfg);
FH_SINT32 FUNC_DEP API_ISP_GetLscCfg(ISP_LSC_CFG *pstLscCfg);
/** NR3D*/
/*
*   Name: API_ISP_SetNR3D
*            配置NR3D的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_NR3D_CFG *pstNr3dCfg
*            类型为ISP_NR3D_CFG的结构体指针，详细成员变量请查看ISP_NR3D_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetNR3D(ISP_NR3D_CFG *pstNr3dCfg);
/*
*   Name: API_ISP_GetNR3D
*            获取当前配置的NR3D的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_NR3D_CFG *pstNr3dCfg
*            类型为ISP_NR3D_CFG的结构体指针，详细成员变量请查看ISP_NR3D_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetNR3D(ISP_NR3D_CFG *pstNr3dCfg);
/** NR2D*/
/*
*   Name: API_ISP_SetNR2DCfg
*            配置NR2D的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_NR2D_CFG *pstNr2dCfg
*            类型为ISP_NR2D_CFG的结构体指针，详细成员变量请查看ISP_NR2D_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetNR2DCfg(ISP_NR2D_CFG *pstNr2dCfg);
/*
*   Name: API_ISP_GetNR2DCfg
*            获取当前配置的NR2D的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_NR2D_CFG *pstNr2dCfg
*            类型为ISP_NR2D_CFG的结构体指针，详细成员变量请查看ISP_NR2D_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetNR2DCfg(ISP_NR2D_CFG *pstNr2dCfg);

/**HLR*/
/*
*   Name: API_ISP_SetHlrCfg
*            配置HLR的DRV寄存器
*
*   Parameters:
*
*       [] ISP_HLR_CFG *pstHlrCfg
*            类型为ISP_HLR_CFG的结构体指针，详细成员变量请查看ISP_HLR_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetHlrCfg(ISP_HLR_CFG *pstHlrCfg);
/*
*   Name: API_ISP_GetHlrCfg
*            获取当前配置的HLR的DRV寄存器值
*
*   Parameters:
*
*       [] ISP_HLR_CFG *pstHlrCfg
*            类型为ISP_HLR_CFG的结构体指针，详细成员变量请查看ISP_HLR_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetHlrCfg(ISP_HLR_CFG *pstHlrCfg);
/**IE*/
/*
*   Name: API_ISP_SetContrastCfg
*            配置对比度相关的DRV寄存器
*
*   Parameters:
*
*       [] ISP_CONTRAST_CFG *pstContrastCfg
*            类型为ISP_CONTRAST_CFG的结构体指针，详细成员变量请查看ISP_CONTRAST_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetContrastCfg(ISP_CONTRAST_CFG *pstContrastCfg);
/*
*   Name: API_ISP_GetContrastCfg
*            获取当前配置的对比度相关的DRV寄存器值
*
*   Parameters:
*
*       [] ISP_CONTRAST_CFG *pstContrastCfg
*            类型为ISP_CONTRAST_CFG的结构体指针，详细成员变量请查看ISP_CONTRAST_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetContrastCfg(ISP_CONTRAST_CFG *pstContrastCfg);
/*
*   Name: API_ISP_SetBrightnessCfg
*            配置亮度相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_BRIGHTNESS_CFG *pstBrightnessCfg
*            类型为ISP_BRIGHTNESS_CFG的结构体指针，详细成员变量请查看ISP_BRIGHTNESS_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetBrightnessCfg(ISP_BRIGHTNESS_CFG *pstBrightnessCfg);
/*
*   Name: API_ISP_GetBrightnessCfg
*            获取当前配置的亮度相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_BRIGHTNESS_CFG *pstBrightnessCfg
*            类型为ISP_BRIGHTNESS_CFG的结构体指针，详细成员变量请查看ISP_BRIGHTNESS_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetBrightnessCfg(ISP_BRIGHTNESS_CFG *pstBrightnessCfg);
/**CE*/
/*
*   Name: API_ISP_SetSaturation
*            配置对比度相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_SAT_CFG *pstCeCfg
*            类型为ISP_SAT_CFG的结构体指针，详细成员变量请查看ISP_SAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetSaturation(ISP_SAT_CFG *pstCeCfg);
/*
*   Name: API_ISP_GetSaturation
*            获取当前配置的对比度相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_SAT_CFG *pstCeCfg
*            类型为ISP_SAT_CFG的结构体指针，详细成员变量请查看ISP_SAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetSaturation(ISP_SAT_CFG *pstCeCfg);
/**APC*/
/*
*   Name: API_ISP_SetApcCfg
*            配置锐度相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_APC_CFG *pstApcCfg
*            类型为ISP_APC_CFG的结构体指针，详细成员变量请查看ISP_APC_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetApcCfg(ISP_APC_CFG *pstApcCfg);
/*
*   Name: API_ISP_GetApcCfg
*            获取当前配置的锐度相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_APC_CFG *pstApcCfg
*            类型为ISP_APC_CFG的结构体指针，详细成员变量请查看ISP_APC_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetApcCfg(ISP_APC_CFG *pstApcCfg);
/*
*   Name: API_ISP_SetSharpenStatCfg
*            配置SHARPEN统计窗
*
*   Parameters:
*
*       [IN] SHARPEN_STAT_CFG *pstSharpenStatCfg
*            类型为SHARPEN_STAT_CFG的结构体指针，详细成员变量请查看SHARPEN_STAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            空指针异常(ERROR_ISP_NULL_POINTER)，传入参数超过限制范围(ERROR_ISP_PARA_OUTOF_RANGE)，0(正确)
*/
FH_SINT32 API_ISP_SetSharpenStatCfg(SHARPEN_STAT_CFG *pstSharpenStatCfg);
/*
*   Name: API_ISP_GetSharpenStatCfg
*            获取当前SHARPEN配置的统计窗口信息
*
*   Parameters:
*
*       [OUT] SHARPEN_STAT_CFG *pstSharpenStatCfg
*            类型为SHARPEN_STAT_CFG的结构体指针，详细成员变量请查看SHARPEN_STAT_CFG结构体定义。
*
*   Return:
*            0(正确)
*            空指针异常(ERROR_ISP_NULL_POINTER)
*/
FH_SINT32 API_ISP_GetSharpenStatCfg(SHARPEN_STAT_CFG *pstSharpenStatCfg);
/*
*   Name: API_ISP_GetSharpenStat
*            获取SHARPEN统计信息即锐度统计信息
*
*   Parameters:
*
*       [OUT] SHARPEN_STAT *pstSharpenStat
*            类型为SHARPEN_STAT的结构体指针，详细成员变量请查看SHARPEN_STAT结构体定义。
*
*   Return:
*            0(正确)
*            空指针异常(ERROR_ISP_NULL_POINTER)，
*/
FH_SINT32 API_ISP_GetSharpenStat(SHARPEN_STAT *pstSharpenStat);
/**GAMMA*/
/*
*   Name: API_ISP_SetGammaCfg
*            配置gamma相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_GAMMA_CFG *pstGammaCfg
*            类型为ISP_GAMMA_CFG的结构体指针，详细成员变量请查看ISP_GAMMA_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetGammaCfg(ISP_GAMMA_CFG *pstGammaCfg);
/*
*   Name: API_ISP_GetGammaCfg
*            获取当前配置的gamma相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_GAMMA_CFG *pstGammaCfg
*            类型为ISP_GAMMA_CFG的结构体指针，详细成员变量请查看ISP_GAMMA_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetGammaCfg(ISP_GAMMA_CFG *pstGammaCfg);
/**HIST*/
FH_SINT32 FUNC_DEP API_ISP_GetHist(ISP_HIST_STAT *psthistStat);
/**YCNR*/
/*
*   Name: API_ISP_SetYnrCfg
*            配置YNR相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_YNR_CFG *pstYnrCfg
*            类型为ISP_YNR_CFG的结构体指针，详细成员变量请查看ISP_YNR_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetYnrCfg(ISP_YNR_CFG *pstYnrCfg);
/*
*   Name: API_ISP_GetYnrCfg
*            获取当前配置的YNR相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_YNR_CFG *pstYnrCfg
*            类型为ISP_YNR_CFG的结构体指针，详细成员变量请查看ISP_YNR_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetYnrCfg(ISP_YNR_CFG *pstYnrCfg);
/*
*   Name: API_ISP_SetCnrCfg
*            配置CNR相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_CNR_CFG *pstCnrCfg
*            类型为ISP_CNR_CFG的结构体指针，详细成员变量请查看ISP_CNR_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetCnrCfg(ISP_CNR_CFG *pstCnrCfg);
/*
*   Name: API_ISP_GetCnrCfg
*            获取当前配置的CNR相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_CNR_CFG *pstCnrCfg
*            类型为ISP_CNR_CFG的结构体指针，详细成员变量请查看ISP_CNR_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetCnrCfg(ISP_CNR_CFG *pstCnrCfg);

/**PURPLE*/
/*
*   Name: API_ISP_SetAntiPurpleBoundary
*            配置去紫边相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_PURPLEFRI_CFG *pstPurplefriCfg
*            类型为ISP_PURPLEFRI_CFG的结构体指针，详细成员变量请查看ISP_PURPLEFRI_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetAntiPurpleBoundary(ISP_PURPLEFRI_CFG *pstPurplefriCfg);
/*
*   Name: API_ISP_GetAntiPurpleBoundary
*            获取当前配置的去紫边的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_PURPLEFRI_CFG *pstPurplefriCfg
*            类型为ISP_PURPLEFRI_CFG的结构体指针，详细成员变量请查看ISP_PURPLEFRI_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetAntiPurpleBoundary(ISP_PURPLEFRI_CFG *pstPurplefriCfg);

/**LC*/
/*
*   Name: API_ISP_SetLcCfg
*            配置Local Chroma相关的DRV寄存器
*
*   Parameters:
*
*       [IN] ISP_LC_CFG *pstLCCfg
*            类型为ISP_LC_CFG的结构体指针，详细成员变量请查看ISP_LC_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetLcCfg(ISP_LC_CFG *pstLCCfg);
/*
*   Name: API_ISP_GetLcCfg
*            获取当前配置的Local Chroma相关的DRV寄存器值
*
*   Parameters:
*
*       [OUT] ISP_LC_CFG *pstLCCfg
*            类型为ISP_LC_CFG的结构体指针，详细成员变量请查看ISP_LC_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetLcCfg(ISP_LC_CFG *pstLCCfg);

/**Debug Interface**/
/*
*   Name: API_ISP_ReadMallocedMem
*            读取指定偏移的VMM分配的内存的值
*
*   Parameters:
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [OUT]  FH_UINT32 *pstData
*            存放读取到数据的地址。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_ReadMallocedMem(FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstData);
/*
*   Name: API_ISP_WriteMallocedMem
*            写指定偏移的VMM分配的内存的值
*
*   Parameters:
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [IN]  FH_UINT32 *pstData
*            目标值，将该值写入目标地址。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_WriteMallocedMem(FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstData);
/*
*   Name: API_ISP_ImportMallocedMem
*            导入指定大小的数据到指定偏移的VMM分配的内存
*
*   Parameters:
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [IN]  FH_UINT32 *pstSrc
*            导入数据的起始地址
*
*       [IN]  FH_UINT32 size
*            需要导入的数据大小
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_ImportMallocedMem(FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstSrc, FH_UINT32 size);
/*
*   Name: API_ISP_ExportMallocedMem
*            从指定偏移的VMM分配的内存导出指定大小的数据
*
*   Parameters:
*
*       [IN] FH_SINT32 intMemSlot
*            分配的内存的类型，决定基地址。
*
*       [IN]  FH_UINT32 offset
*            偏移地址，选定的内存会给定其基地址。
*
*       [OUT]  FH_UINT32 *pstDst
*            存放导出数据的起始地址
*
*       [IN]  FH_UINT32 size
*            需要导出的数据大小
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_ExportMallocedMem(FH_SINT32 intMemSlot, FH_UINT32 offset, FH_UINT32 *pstDst, FH_UINT32 size);
/**MIRROR**/
/*
*   Name: API_ISP_MirrorEnable
*            ISP的MIRROR模块，可以实现镜像。
*
*   Parameters:
*
*       [IN] MIRROR_CFG_S *pMirror
*            类型为MIRROR_CFG_S的结构体指针，详细成员变量请查看MIRROR_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_MirrorEnable(MIRROR_CFG_S *pMirror);
/*
*   Name: API_ISP_SetMirrorAndflip
*            配置SENSOR的镜像和水平翻转。
*
*   Parameters:
*
*       [IN] FH_BOOL mirror
*            镜像(mirror=1)，正常(mirror=0)
*
*       [IN]  FH_BOOL flip
*            水平翻转(flip=1)，正常(flip=0)
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_SetMirrorAndflip(FH_BOOL mirror, FH_BOOL flip);
/*
*   Name: API_ISP_SetMirrorAndflipEx
*            配置SENSOR的镜像和水平翻转，同时会更改ISP中相关bayer配置。
*
*   Parameters:
*
*       [IN] FH_BOOL mirror
*            镜像(mirror=1)，正常(mirror=0)
*
*       [IN]  FH_BOOL flip
*            水平翻转(flip=1)，正常(flip=0)
*
*       [IN] FH_UINT32 bayer
*            镜像和水平翻转影响之后的BAYER格式。
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_SetMirrorAndflipEx(FH_BOOL mirror, FH_BOOL flip,FH_UINT32 bayer);
/*
*   Name: API_ISP_GetMirrorAndflip
*            获取当前SENSOR镜像和水平翻转的状态
*
*   Parameters:
*
*       [OUT] FH_BOOL *mirror
*            镜像(mirror=1)，正常(mirror=0)
*
*       [OUT]  FH_BOOL *flip
*            水平翻转(flip=1)，正常(flip=0)
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetMirrorAndflip(FH_BOOL *mirror, FH_BOOL *flip);


/**post gain*/
FH_SINT32 FUNC_DEP API_ISP_SetPostGain(const ISP_POST_GAIN_CFG *pstPostGainCfg);
FH_SINT32 FUNC_DEP API_ISP_GetPostGain(ISP_POST_GAIN_CFG *pstPostGainCfg);

/**RGBA*/
/*
*   Name: API_ISP_RgbaSendCmd
*            根据cmd执行不同的配置RGBA的SWITCH CASE
*
*   Parameters:
*
*       [IN] FH_UINT32 u32Cmd
*            取值范围为RGBA_CMD_KEY，每个值对应不同的sitch case.
*
*       [IN/OUT]  FH_VOID *param
*            传入参数的地址或者时获取状态值的地址，输入输出类型根据cmd判断。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_RgbaSendCmd(FH_UINT32 u32Cmd, FH_VOID *param);
/*
*   Name: API_ISP_GetRgbaCfg
*            获取当前DRV RGBA配置
*
*   Parameters:
*
*       [OUT] RGBA_DEFAULT_CFG *pstRgbaDefaultCfg
*            类型为RGBA_DEFAULT_CFG的结构体指针，详细成员变量请查看RGBA_DEFAULT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetRgbaCfg(RGBA_DEFAULT_CFG *pstRgbaDefaultCfg);
/*
*   Name: API_ISP_GetRgbaInfo
*            获取当前RGBA的一些信息，详见RGBA_CURR_INFO
*
*   Parameters:
*
*       [OUT] RGBA_CURR_INFO *pstRgbaInfo
*            类型为RGBA_CURR_INFO的结构体指针，详细成员变量请查看RGBA_CURR_INFO结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetRgbaInfo(RGBA_CURR_INFO *pstRgbaInfo);
/*
*   Name: API_ISP_GetRgbaStat
*            获取RGBA统计
*
*   Parameters:
*
*       [OUT] RGBA_STAT_INFO *pstRgbaStat
*            类型为RGBA_STAT_INFO的结构体指针，详细成员变量请查看RGBA_STAT_INFO结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetRgbaStat(RGBA_STAT_INFO *pstRgbaStat);
/*
*   Name: API_ISP_SetRgbaDpc
*            配置RGBA DPC配置
*
*   Parameters:
*
*       [IN] RGBA_DPC_CFG *pstRgbaDpc
*            类型为RGBA_DPC_CFG的结构体指针，详细成员变量请查看MIRROR_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetRgbaDpc(RGBA_DPC_CFG *pstRgbaDpc);
/*
*   Name: API_ISP_GetRgbaDpc
*            获取当前RGBA DPC配置
*
*   Parameters:
*
*       [OUT] RGBA_DPC_CFG *pstRgbaDpc
*            类型为RGBA_DPC_CFG的结构体指针，详细成员变量请查看MIRROR_CFG_S结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetRgbaDpc(RGBA_DPC_CFG *pstRgbaDpc);

/**SMART IR**/
/*
*   Name: API_ISP_SetSmartIrCfg
*            配置智能IR的增益門限值
*
*   Parameters:
*
*       [IN] SMART_IR_CFG *pstSmartIrCfg
*            类型为SMART_IR_CFG的结构体指针，详细成员变量请查看SMART_IR_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetSmartIrCfg(SMART_IR_CFG *pstSmartIrCfg);
/*
*   Name: API_ISP_GetSmartIrCfg
*            获取当前配置的智能IR的增益門限值
*
*   Parameters:
*
*       [OUT] SMART_IR_CFG *pstSmartIrCfg
*            类型为SMART_IR_CFG的结构体指针，详细成员变量请查看SMART_IR_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_GetSmartIrCfg(SMART_IR_CFG *pstSmartIrCfg);
/**WDR**/
FH_SINT32 FUNC_DEP API_ISP_SetWdrSframeCorrCfg(unsigned int *sframeGain);
FH_SINT32 FUNC_DEP API_ISP_GetWdrSframeCorrCfg(unsigned int *sframeGain);
FH_SINT32 FUNC_DEP API_ISP_SetWdrShortFrameMergeCfg(ISP_WDR_MERGE_CFG *pstWdrMergeCfg);
FH_SINT32 FUNC_DEP API_ISP_SetWdrLongFrameMergeCfg(ISP_WDR_MERGE_CFG *pstWdrMergeCfg);
FH_SINT32 FUNC_DEP API_ISP_GetWdrCfg(ISP_WDR_CFG *pstWdrCfg);
FH_SINT32 FUNC_DEP API_ISP_SetWdrCfg(ISP_WDR_CFG *pstWdrCfg);
/**FAST BOOT**/
/*
*   Name: API_ISP_SetCropInfo
*            配置输入幅面的水平和垂直的处理偏移，其执行位置在ISP初始化阶段，此时ISP未启动，流程中仅执行一次。
*
*   Parameters:
*
*       [IN] int offset_x
*            水平方向的偏移
*
*       [IN] int offset_y
*            垂直方向的偏移。
*
*   Return:
*            0(正确)
*        -3004(偏移值超过输入幅面或者为奇数)
*/
FH_SINT32 API_ISP_SetCropInfo(int offset_x,int offset_y);
/*
*   Name: API_ISP_RegisterPicStartCallback
*            在ISP PicStartB的时候的回调函数，仅用在RTT下统计时间。
*
*   Parameters:
*
*       [IN] ispIntCallback cb
*            void型的函数指针，指向回调函数的位置。
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_RegisterPicStartCallback(ispIntCallback cb);
/*
*   Name: API_ISP_RegisterPicEndCallback
*            在ISP PicEndP的时候的回调函数，仅用在RTT下统计时间。
*
*   Parameters:
*
*       [IN] ispIntCallback cb
*            void型的函数指针，指向回调函数的位置。
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_RegisterPicEndCallback(ispIntCallback cb);
/*
*   Name: API_ISP_RegisterIspInitCfgCallback
*            ISP硬件初始化回调函数，其执行位置在ISP初始化阶段，此时ISP未启动，流程中仅执行一次。
*
*   Parameters:
*
*       [IN] ispInitCfgCallback cb
*            void型的函数指针，指向回调函数的位置。
*
*   Return:
*            0(正确)
*/
FH_SINT32 API_ISP_RegisterIspInitCfgCallback(ispInitCfgCallback cb);
/*
*   Name: API_ISP_SetBlcInitCfg
*            初始化BLC所有通道的减DC值，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] BLC_INIT_CFG *pstBlcInitCfg
*            类型为BLC_INIT_CFG的结构体指针，详细成员变量请查看BLC_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetBlcInitCfg(BLC_INIT_CFG *pstBlcInitCfg);
/*
*   Name: API_ISP_SetWbInitCfg
*            初始化WB三个通道增益值，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] WB_INIT_CFG *pstWbInitCfg
*            类型为WB_INIT_CFG的结构体指针，详细成员变量请查看WB_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetWbInitCfg(WB_INIT_CFG *pstWbInitCfg);
/*
*   Name: API_ISP_SetCcmInitCfg
*            初始化CCM矩阵硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] CCM_INIT_CFG *pstCcmInitCfg
*            类型为DPC_INIT_CFG的结构体指针，详细成员变量请查看DPC_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetCcmInitCfg(CCM_INIT_CFG *pstCcmInitCfg);
/*
*   Name: API_ISP_SetDpcInitCfg
*            初始化APC硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] DPC_INIT_CFG *pstDpcInitCfg
*            类型为DPC_INIT_CFG的结构体指针，详细成员变量请查看DPC_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetDpcInitCfg(DPC_INIT_CFG *pstDpcInitCfg);
/*
*   Name: API_ISP_SetApcInitCfg
*            初始化APC硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] APC_INIT_CFG *pstApcInitCfg
*            类型为APC_INIT_CFG的结构体指针，详细成员变量请查看APC_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetApcInitCfg(APC_INIT_CFG *pstApcInitCfg);
/*
*   Name: API_ISP_SetYnrInitCfg
*            初始化YNR硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] YNR_INIT_CFG *pstYnrInitCfg
*            类型为YNR_INIT_CFG的结构体指针，详细成员变量请查看YNR_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetYnrInitCfg(YNR_INIT_CFG *pstYnrInitCfg);
/*
*   Name: API_ISP_SetCtrInitCfg
*            配置对比度值到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] CTR_INIT_CFG *pstCtrInitCfg
*            类型为CTR_INIT_CFG的结构体指针，详细成员变量请查看CTR_INIT_CFG结构体定义。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetCtrInitCfg(CTR_INIT_CFG *pstCtrInitCfg);
/*
*   Name: API_ISP_SetSatInitCfg
*            配置饱和度值到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] SAT_INIT_CFG *pstSatInitCfg
*            SAT_INIT_CFG的结构体指针，详细参数查看结构体SAT_INIT_CFG注释。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetSatInitCfg(SAT_INIT_CFG *pstSatInitCfg);
/*
*   Name: API_ISP_SetGammaModeInitCfg
*            根据传入的gamma mode值，配置相应硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] GAMMA_MODE_INIT_CFG *pstGammaModeInitCfg
*            目标配置的gamma mode。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetGammaModeInitCfg(GAMMA_MODE_INIT_CFG *pstGammaModeInitCfg);
/*
*   Name: API_ISP_SetCGammaInitCfg
*            配置cgamma table到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] GAMMA_INIT_CFG *pstCGammaInitCfg
*            结构体GAMMA_INIT_CFG的指针，目标配置cgamma表。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetCGammaInitCfg(GAMMA_INIT_CFG *pstCGammaInitCfg);
/*
*   Name: API_ISP_SetYGammaInitCfg
*            配置ygamma table到硬件寄存器，调用阶段是在API_ISP_RegisterIspInitCfgCallback中。
*
*   Parameters:
*
*       [IN] GAMMA_INIT_CFG *pstYGammaInitCfg
*            结构体GAMMA_INIT_CFG的指针，目标配置的ygamma表。
*
*   Return:
*            0(正确)
*        -3002(空指针异常)
*/
FH_SINT32 API_ISP_SetYGammaInitCfg(GAMMA_INIT_CFG *pstYGammaInitCfg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*_ISP_API_H_*/
