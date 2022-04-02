#ifndef __FH_VPU_MPI_H__
#define __FH_VPU_MPI_H__
/**VPU**/
#include "types/type_def.h"
#include "fh_vpu_mpipara.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/*
*   Name: FH_VPSS_SetViAttr
*            设置视频输入属性
*
*   Parameters:
*
*       [in] FH_VPU_SIZE *pstViconfig
*            视频输入幅面
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          输入幅面必须和ISP输出幅面完全一致,否则会导致图像错误或者无法完成。
*/
FH_SINT32 FH_VPSS_SetViAttr(FH_VPU_SIZE *pstViconfig);

/*
*   Name: FH_VPSS_GetViAttr
*            获取视频输入属性
*
*   Parameters:
*
*       [out] FH_VPU_SIZE *pstViconfig
*            视频输出幅面
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetViAttr(FH_VPU_SIZE *pstViconfig);

/*初始化视频处理的系统内存，供内部调用，用户不需要显性调用*/
FH_SINT32 FH_VPSS_SysInitMem(void);

/*
*   Name: FH_VPSS_GetBGMData
*            获取视频处理模块中供BGM使用的图像数据
*
*   Parameters:
*
*       [out] FH_PIC_DATA *pstBGMData
*            供BGM使用图像数据信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          阻塞接口,超时时间为1s。
*/
FH_SINT32 FH_VPSS_GetBGMData(FH_PIC_DATA *pstBGMData);

/*
*   Name: FH_VPSS_GetCPYData
*            获取视频处理模块中纹理复杂度数据
*
*   Parameters:
*
*       [out] FH_PIC_DATA *pstCPYData
*            纹理复杂度数据信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          阻塞接口,超时时间为1s。
*/
FH_SINT32 FH_VPSS_GetCPYData(FH_PIC_DATA *pstCPYData);

/*
*   Name: FH_VPSS_Enable
*            使能视频处理模块
*
*   Parameters:
*
*       [in] FH_VPU_VI_MODE mode
*            视频处理模块工作模式
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          更换工作模式时，需先调用FH_VPSS_Disable,然后再调用此函数。
*/
FH_SINT32 FH_VPSS_Enable(FH_VPU_VI_MODE mode);

/*
*   Name: FH_VPSS_Disable
*            关闭视频处理模块
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_Disable(void);

/*
*   Name: FH_VPSS_FreezeVideo
*            视频冻结,视频处理模块将会向后面数据处理模块,不断发送最后一帧的数据.直到调用FH_VPSS_UnfreezeVideo。
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          当通道的输出缓存数量为1帧或者特殊工作模式时,此接口无效。
*/
FH_SINT32 FH_VPSS_FreezeVideo(void);

/*
*   Name: FH_VPSS_UnfreezeVideo
*            视频解冻
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_UnfreezeVideo(void);

/*
*   Name: FH_VPSS_GetChnFrame
*            获取视频处理模块通道输出的图像数据
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_VPU_STREAM *pstVpuframeinfo
*            图像数据信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          阻塞接口,超时时间为1s。
*/
FH_SINT32 FH_VPSS_GetChnFrame(FH_UINT32 chan,FH_VPU_STREAM *pstVpuframeinfo);

/*
*   Name: FH_VPSS_SendUserPic
*            提交用户图像给视频处理模块进行处理,仅在处于非直通模式下有效。
*
*   Parameters:
*
*       [in] const FH_VPU_USER_PIC *pstUserPic
*            用户图像信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SendUserPic(const FH_VPU_USER_PIC *pstUserPic);

/*API弃用,不再支持*/
FH_SINT32 FH_VPSS_GetUserPicAddr(FH_VPU_USER_PIC *pstUserPic);

/*
*   Name: FH_VPSS_SetChnAttr
*            设置视频处理模块通道属性。修改通道属性时不需要停止视频处理模块。但建议先停止该通道的使能。
*
*   Parameters:
*
*       [in] FH_UINT32  chan
*            通道号
*
*       [in] const FH_VPU_CHN_CONFIG *pstChnconfig
*            通道属性信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          在设置通道属性后,包含帧率控制,视频冻结,绑定关系等通道配置会失效,需要重新设置。
*/
FH_SINT32 FH_VPSS_SetChnAttr(FH_UINT32  chan,const FH_VPU_CHN_CONFIG *pstChnconfig);

/*
*   Name: FH_VPSS_GetChnAttr
*            获取视频处理模块通道属性。
*
*   Parameters:
*
*       [in] FH_UINT32  chan
*            通道号
*
*       [out]  FH_VPU_CHN_CONFIG *pstChnconfig
*            通道属性信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetChnAttr(FH_UINT32  chan, FH_VPU_CHN_CONFIG *pstChnconfig);

/*
*   Name: FH_VPSS_SetVOMode
*            设置视频处理模块通道输出组织格式
*
*   Parameters:
*
*       [in] FH_UINT32  chan
*            通道号
*
*       [in]  FH_VPU_VO_MODE mode
*            图像数据输出组织格式
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          各模块支持的组织格式在各平台有所不同,详见《视频函数开发参考手册》。
*/
FH_SINT32 FH_VPSS_SetVOMode(FH_UINT32  chan, FH_VPU_VO_MODE mode);

/*
*   Name: FH_VPSS_OpenChn
*            开启通道使能
*
*   Parameters:
*
*       [in] FH_UINT32  chan
*            通道号
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_OpenChn(FH_UINT32  chan);

/*
*   Name: FH_VPSS_CloseChn
*            关闭通道使能
*
*   Parameters:
*
*       [in] FH_UINT32  chan
*            通道号
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_CloseChn(FH_UINT32  chan);

/*
*   Name: FH_VPSS_SetMask
*            设置视频处理模块隐私屏蔽属性
*
*   Parameters:
*
*       [in] const FH_VPU_MASK *pstVpumaskinfo
*            隐私屏蔽属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          各平台支持功能有所差异,详见《视频函数开发参考手册》。
*/
FH_SINT32 FH_VPSS_SetMask(const FH_VPU_MASK *pstVpumaskinfo);

/*
*   Name: FH_VPSS_GetMask
*            获取视频处理模块隐私屏蔽属性
*
*   Parameters:
*
*       [out] FH_VPU_MASK *pstVpumaskinfo
*            隐私屏蔽属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetMask(FH_VPU_MASK *pstVpumaskinfo);

/*
*   Name: FH_VPSS_ClearMask
*            关闭对应隐私屏蔽区域
*
*   Parameters:
*
*       [in] FH_UINT32 clearmaskarea
*            隐私屏蔽区域索引,范围0-7
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_ClearMask(FH_UINT32 clearmaskarea);

/*
*   Name: FH_VPSS_SetGraph
*            设置图形叠加信息。
*
*   Parameters:
*
*       [in] const FH_VPU_LOGO  *pstVpugraphinfo
*            图形叠加信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          各平台支持功能有所差异,详见《视频函数开发参考手册》。
*/
FH_SINT32 FH_VPSS_SetGraph(const FH_VPU_LOGO  *pstVpugraphinfo);


/*
*   Name: FH_VPSS_GetGraph
*            获取图形叠加信息。
*
*   Parameters:
*
*       [out] FH_VPU_LOGO  *pstVpugraphinfo
*            图形叠加信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetGraph(FH_VPU_LOGO  *pstVpugraphinfo);

/*
*   Name: FH_VPSS_SetOsd
*            设置字符叠加信息。
*
*   Parameters:
*
*       [in] const FH_VPU_OSD  *pstVpuosdinfo
*            字符叠加信息。
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          各平台支持功能有所差异,详见《视频函数开发参考手册》。
*/
FH_SINT32 FH_VPSS_SetOsd(const FH_VPU_OSD  *pstVpuosdinfo);

/*
*   Name: FH_VPSS_GetOsd
*            获取字符叠加信息。
*
*   Parameters:
*
*       [out] FH_VPU_OSD  *pstVpuosdinfo
*            字符叠加信息。
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetOsd(FH_VPU_OSD  *pstVpuosdinfo);

/*
*   Name: FH_VPSS_SetOsdAddr
*            设置字符叠加地址,主要供用户使用双缓冲时切换数据地址。
*
*   Parameters:
*
*       [in] const FH_VPU_OSD_ADDR *pstVpuosdinfo
*            字库和索引地址
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SetOsdAddr(const FH_VPU_OSD_ADDR *pstVpuosdinfo);

/*
*   Name: FH_VPSS_SetRotate
*            设置字符旋转属性
*
*   Parameters:
*
*       [in] const FH_ROTATE_OPS  Rotate
*            字符旋转属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SetRotate(const FH_ROTATE_OPS  Rotate);

/*
*   Name: FH_VPSS_SetOsdInvert
*            设置字符反色属性
*
*   Parameters:
*
*       [in] const FH_INVERT_CTRL  *pstOsdinvertctl
*            字符反色属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SetOsdInvert(const FH_INVERT_CTRL  *pstOsdinvertctl);

/*
*   Name: FH_VPSS_GetOsdInvert
*            获取字符反色属性
*
*   Parameters:
*
*       [out] FH_INVERT_CTRL  *pstOsdinvertctl
*            字符反色属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetOsdInvert(FH_INVERT_CTRL  *pstOsdinvertctl);

/*
*   Name: FH_VPSS_SetOsdHighlight
*            设置字符高亮属性
*
*   Parameters:
*
*       [in] const FH_HIGHLIGHT_CTRL  *pstOsdhlctl
*            字符高亮属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SetOsdHighlight(const FH_HIGHLIGHT_CTRL  *pstOsdhlctl);

/*
*   Name: FH_VPSS_GetOsdHighlight
*            获取字符高亮属性
*
*   Parameters:
*
*       [out] FH_HIGHLIGHT_CTRL  *pstOsdhlctl
*            字符高亮属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetOsdHighlight(FH_HIGHLIGHT_CTRL  *pstOsdhlctl);


/*
*   Name: FH_VPSS_EnableYCmean
*           开启均值统计
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_EnableYCmean(void);

/*
*   Name: FH_VPSS_DisableYCmean
*            关闭均值统计
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_DisableYCmean(void);

/*
*   Name: FH_VPSS_SetYCmeanMode
*            配置均值统计下采样模式
*
*   Parameters:
*
*       [in] FH_UINT32 mode
*            下采样模式(4/8/16)
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SetYCmeanMode(FH_UINT32 mode);

/*
*   Name: FH_VPSS_GetYCmean
*            获取均值统计结果
*
*   Parameters:
*
*       [out] FH_VPU_YCMEAN *pstVpuycmeaninfo
*            均值统计结果
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetYCmean(FH_VPU_YCMEAN *pstVpuycmeaninfo);

/*
*   Name: FH_VPSS_SetFramectrl
*            设置帧率控制参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_FRAMERATE *pstVpuframectrl
*            帧率控制参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          1. 通道处于特殊工作模式时不支持帧率控制。
*          2. 帧率控制时,图像时间戳将被修改为目标帧间隔。
*          3. 只有在通道缓存大于等于3个时,才支持增帧模式。
*/
FH_SINT32 FH_VPSS_SetFramectrl(FH_UINT32 chan,const FH_FRAMERATE *pstVpuframectrl);

/*
*   Name: FH_VPSS_GetFramectrl
*            获取帧率控制参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  FH_FRAMERATE *pstVpuframectrl
*            帧率控制参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetFramectrl(FH_UINT32 chan, FH_FRAMERATE *pstVpuframectrl);

/*
*   Name: FH_VPSS_GetFrameRate
*            获取当前通道帧率统计结果
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_FRAMERATE *fps
*            帧率统计结果
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetFrameRate(FH_UINT32 chan,FH_FRAMERATE *fps);


/*
*   Name: FH_VPSS_LOW_LATENCY_Enable
*            开启低延时模式，同时只能有一个通道使能低延时模式
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          1. 低延时模式只能在一个通道上生效，且与智能编码，帧率控制，编码旋转不兼容。
*          2. 主要作用是降低某个通道从sensor采集到编码完成的时间，采用低延时时可以考虑减少该通道的缓冲内存数量。
*/
FH_SINT32 FH_VPSS_LOW_LATENCY_Enable(FH_UINT32 chan);

/*
*   Name: FH_VPSS_LOW_LATENCY_Disable
*            禁用低延时模式
*
*   Parameters:
*
*            None
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_LOW_LATENCY_Disable(void);

/*
*   Name: FH_VPSS_SetCrop
*            设置通道裁剪属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_VPU_CROP *pstVpucropinfo
*            裁剪属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          各平台支持功能有所差异,详见《视频函数开发参考手册》。
*/
FH_SINT32 FH_VPSS_SetCrop(FH_UINT32 chan,const FH_VPU_CROP *pstVpucropinfo);

/*
*   Name: FH_VPSS_GetCrop
*            获取通道裁剪属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_VPU_CROP *pstVpucropinfo
*            裁剪属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetCrop(FH_UINT32 chan,FH_VPU_CROP *pstVpucropinfo);


/*API弃用,不再支持*/
FH_SINT32  FH_VPSS_Reset(void);

/*
*   Name: FH_VPSS_SetScalerCoeff
*            设置图像缩放的滤波器系数,可以将图像效果从平滑到锐利。
*            建议输入输出缩放比差不多或放大时，设的较强。反之设弱些。
*            另外在编码压力较大时可以使用此接口对图像进行一定程度滤波降低编码压力。
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in]  FH_UINT32 level
*            滤波等级，0-15,越大越清晰
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SetScalerCoeff(FH_UINT32 chan, FH_UINT32 level);


/*
*   Name: FH_VPSS_GetFrameBufferSize
*            获取对应通道设的内存块数量需要的内存大小。供快速启动应用增加临时输出缓存数量。
*
*   Parameters:
*
*       [in] unsigned int chan
*            通道号
*
*       [in] unsigned int bufnum
*            增加buf数量
*
*       [out] unsigned int *needsize
*            需要分配的内存数量
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetFrameBufferSize(unsigned int chan,unsigned int bufnum,unsigned int *needsize);

/*
*   Name: FH_VPSS_FrameBufferRegister
*            给通道输出注册临时内存
*
*   Parameters:
*
*       [in] unsigned int chan
*            通道号
*
*       [in] unsigned int bufnum
*            增加buf数量
*
*       [in] FH_MEM_INFO buf
*            临时内存的信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_FrameBufferRegister(unsigned int chan,unsigned int bufnum,FH_MEM_INFO buf);
/*
*   Name: FH_VPSS_FrameBufferUnRegister
*            将通道输出临时内存注销
*
*   Parameters:
*
*       [in] unsigned int chan
*            通道号
*
*       [out] FH_MEM_INFO *buf
*            临时内存的信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          此函数需要等待内存释放,带有阻塞等待,请注意调用时机。
*/
FH_SINT32 FH_VPSS_FrameBufferUnRegister(unsigned int chan,FH_MEM_INFO *buf);

/*
*   Name: FH_VPSS_SetDefaultScalerSize
*            修改各个通道默认的缩放大小，必须在配置vi属性和通道属性之前，用于一些小幅面高帧率的场景。
*            在设置通道属性时对应通道的默认大小会被修改。
*
*   Parameters:
*
*       [in] FH_SIZE *picsize
*            默认输出幅面
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SetDefaultScalerSize(FH_SIZE *picsize);

/*内部接口,不建议用户调用此接口*/
FH_SINT32 FH_VPU_GetPkginfo(FH_PKG_INFO *pstVpupkginfo);

/*
*   Name: FH_VPSS_SetGlbGraphV2
*            设置全局图形叠加信息。
*
*   Parameters:
*
*       [in] const FH_VPU_LOGOV2 *pstVpugraphinfo
*            图形叠加信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          各平台支持功能有所差异,详见《视频函数开发参考手册》。
*/
FH_SINT32 FH_VPSS_SetGlbGraphV2(const FH_VPU_LOGOV2 *pstVpugraphinfo);

/*
*   Name: FH_VPSS_GetGlbGraphV2
*            获取全局图形叠加信息。
*
*   Parameters:
*
*       [out] FH_VPU_LOGOV2 * pstVpugraphinfo
*            图形叠加信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetGlbGraphV2(FH_VPU_LOGOV2 * pstVpugraphinfo);

/*
*   Name: FH_VPSS_SetChnGraphV2
*            设置通道图形叠加信息。
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  const FH_VPU_LOGOV2 *pstVpugraphinfo
*            图形叠加信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_SetChnGraphV2(FH_UINT32 chan, const FH_VPU_LOGOV2 *pstVpugraphinfo);

/*
*   Name: FH_VPSS_GetChnGraphV2
*            获取通道图形叠加信息。
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  FH_VPU_LOGOV2 * pstVpugraphinfo
*            图形叠加信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VPSS_GetChnGraphV2(FH_UINT32 chan, FH_VPU_LOGOV2 * pstVpugraphinfo);\

/*
*   Name: FH_VPSS_SetChn2LogoSel
*            设置通道2缩放图像源位置
*
*   Parameters:
*
*       [in] FH_UINT32 mode
*            0:缩放位置在通道图形叠加之前
*            1:缩放位置在通道图形叠加之前
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          此接口仅适用于FH8626系列产品。
*/
FH_SINT32 FH_VPSS_SetChn2LogoSel(FH_UINT32 mode);

/*内部调试函数，不建议用户调用*/
FH_SINT32 FH_VPSS_ReadMallocedMem(int intMemSlot, FH_UINT32 offset, FH_UINT32 *pstData);
FH_SINT32 FH_VPSS_WriteMallocedMem(int intMemSlot, FH_UINT32 offset, FH_UINT32 *pstData);
FH_SINT32 FH_VPSS_ImportMallocedMem(int intMemSlot, FH_UINT32 offset, FH_UINT32 *pstSrc, FH_UINT32 size);
FH_SINT32 FH_VPSS_ExportMallocedMem(int intMemSlot, FH_UINT32 offset, FH_UINT32 *pstDst, FH_UINT32 size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /*__MPI_VO_H__ */
