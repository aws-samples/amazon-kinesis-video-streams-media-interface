#ifndef __FH_SYSTEM_MPI_H__
#define __FH_SYSTEM_MPI_H__
/**System Control**/

#include "types/type_def.h"
#include "fh_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/*
*   Name: FH_SYS_Init
*            初始化MPP系统。打开各个设备驱动。
*            完成对VPU通道和系统/VENC模块系统内存的分配。
*
*   Parameters:
*
*            None
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*   Note:
*            对于各模块相关内存和功能裁剪需要在此函数之前调用。
*/
FH_SINT32  FH_SYS_Init(void);

/*
*   Name: FH_SYS_Init_Pre
*            初始化MPP系统。打开部分设备驱动。
*            完成对VPU通道和系统内存的分配。
*            适用于快速启动相关应用场景
*
*   Parameters:
*
*            None
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*   Note:
*            对于在此接口中打开的模块相关内存和功能裁剪需要在此函数之前调用。
*/
FH_SINT32 FH_SYS_Init_Pre(void);
/*
*   Name: FH_SYS_Init_Post
*            初始化MPP系统。打开FH_SYS_Init_Pre未被初始化的设备驱动。
*            完成对VENC模块系统内存的分配。
*            适用于快速启动相关应用场景
*
*   Parameters:
*
*            None
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*   Note:
*            对于在此接口中打开的模块相关内存和功能裁剪需要在此函数之前调用。
*/
FH_SINT32 FH_SYS_Init_Post(void);

/*
*   Name: FH_SYS_Exit
*            退出MPP系统。关闭各个设备驱动。释放内存资源。
*
*   Parameters:
*
*            None
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*/
FH_SINT32  FH_SYS_Exit(void);


/*
*   Name: FH_SYS_BindVpu2Enc
*            数据源到编码通道的绑定接口
*
*   Parameters:
*
*       [in] FH_UINT32 srcchn
*            VPU通道号
*
*       [in] FH_UINT32 dstschn
*            VENC通道号
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*   Note:
*            1. 一个VPU通道可以绑定到多个VENC通道。
*            2. 一个VENC通道只能接收一个VPU通道的数据。
*            3. 除JPEG通道外,其他通道需要满足数据源和接收者的幅面一致。
*            4. 重新设置VPU/VENC通道属性会导致其相关绑定关系失效,需要重新绑定。
*            5. 对JPEG通道的绑定,在完成一次抓图后会自动失效,需要再次绑定触发下一次抓图。
*/
FH_SINT32  FH_SYS_BindVpu2Enc(FH_UINT32 srcchn,FH_UINT32 dstschn);


/*
*   Name: FH_SYS_BindVpu2Bgm
*            VPU-BGM数据到BGM的绑定接口
*
*   Parameters:
*
*            None
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*   Note:
*            1. 重新设置VPU通道0或者重新设置BGM属性会导致其相关绑定关系失效,需要重新绑定。
*/
FH_SINT32 FH_SYS_BindVpu2Bgm(void);

/*
*   Name: FH_SYS_GetBindbyDest
*            获取编码通道的数据源的通道号
*
*   Parameters:
*
*       [out] FH_UINT32 *srcchn
*            绑定到VENC通道的VPU通道号
*
*       [in] FH_UINT32 dstschn
*            VENC通道号
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*/
FH_SINT32 FH_SYS_GetBindbyDest(FH_UINT32 *srcchn,FH_UINT32 dstschn);

/*
*   Name: FH_SYS_UnBindbySrc
*            解除VPU通道的所有绑定关系(存在多个接收者时也会全部解除)
*
*   Parameters:
*
*       [in] FH_UINT32 srcchn
*            VPU通道号
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*/
FH_SINT32  FH_SYS_UnBindbySrc(FH_UINT32 srcchn);

/*
*   Name: FH_SYS_UnBindbyDst
*            解除VENC通道的所有绑定关系
*
*   Parameters:
*
*       [in] FH_UINT32 dstchn
*            VENC通道号
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*/
FH_SINT32 FH_SYS_UnBindbyDst(FH_UINT32 dstchn);

/*
*   Name: FH_SYS_GetVersion
*            获取系统版本信息
*
*   Parameters:
*
*       [out] FH_VERSION_INFO *pstSystemversion
*            版本号等信息
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*/
FH_SINT32  FH_SYS_GetVersion(FH_VERSION_INFO *pstSystemversion);

/*
*   Name: *FH_DSP_Version
*            获取DSP版本信息字符串
*
*   Parameters:
*
*       [in] FH_UINT32 print_enable
*            是否自动打印相关版本信息
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*/
FH_CHAR *FH_DSP_Version(FH_UINT32 print_enable);

/*
*   Name: FH_SYS_GetChipID
*            获取芯片标识,芯片标识定义见fh_common.h
*
*   Parameters:
*
*            None
*
*   Return:
*            芯片标识
*
*/
FH_UINT32 FH_SYS_GetChipID(FH_VOID);

/*
*   Name: FH_SYS_SetReg
*            设置寄存器/内存地址
*
*   Parameters:
*
*       [in] FH_UINT32 addr
*            物理地址
*
*       [out]  FH_UINT32 value
*            设置值
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*/
FH_SINT32 FH_SYS_SetReg(FH_UINT32 addr, FH_UINT32 value);

/*
*   Name: FH_SYS_GetReg
*            获取寄存器/内存地址
*
*   Parameters:
*
*       [in] FH_UINT32 u32Addr
*            物理地址
*
*       [out]  FH_UINT32 *pu32Value
*            实际值
*
*   Return:
*              0(成功)
*            非0(失败,详见错误码)
*/
FH_SINT32 FH_SYS_GetReg(FH_UINT32 u32Addr, FH_UINT32 *pu32Value);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /*__MPI_VO_H__ */

