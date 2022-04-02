#ifndef __FH_VENC_MPI_H__
#define __FH_VENC_MPI_H__
/**VENC**/
#include "fh_venc_mpipara.h"
#include "types/type_def.h"
#include "fh_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/*系统内存初始化，供内部调用，用户不需要显性调用*/
FH_SINT32 FH_VENC_SysInitMem(void);

/*
*   Name: FH_VENC_CreateChn
*            创建编码通道，指定通道支持的最大编码幅面，和编码格式。
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_VENC_CHN_CAP *stVencChnAttr
*            通道属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          需要按需要处理的最大能力创建，创建之后无法销毁。
*          可以指明一个通道同时支持多种不同的编码类型。
*          支持多种类型，分配内存时会按各类型种最大的一块内存进行内存分配，在设置通道属性时将这块内存给相应驱动使用。
*/
FH_SINT32 FH_VENC_CreateChn(FH_UINT32 chan,const FH_VENC_CHN_CAP *stVencChnAttr);

/*
*   Name: FH_VENC_SetChnAttr
*            设置编码通道属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_VENC_CHN_CONFIG *pstVencChnAttr
*            编码通道属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          在创建通道时可以指定多个通道支持MJPEG/JPEG的编码能力，但设置通道编码类型时，
*          同时只允许一个通道进行JPEG/MJPEG编码（JPEG和MJPEG的能力可以共存，在独立两个通道同时生效）。
*/
FH_SINT32 FH_VENC_SetChnAttr(FH_UINT32 chan,const FH_VENC_CHN_CONFIG *pstVencChnAttr);

/*
*   Name: FH_VENC_GetChnAttr
*            获取编码通道属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  FH_VENC_CHN_CONFIG *pstVencChnAttr
*            编码通道属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VENC_GetChnAttr(FH_UINT32 chan, FH_VENC_CHN_CONFIG *pstVencChnAttr);

/*
*   Name: FH_VENC_StartRecvPic
*            通道开启接收图像数据
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
*          通道为JPEG抓图模式时，不需要调用FH_VENC_StartRecvPic/ FH_VENC_StopRecvPic。
*/
FH_SINT32 FH_VENC_StartRecvPic(FH_UINT32 chan);

/*
*   Name: FH_VENC_StopRecvPic
*            通道停止接收图像数据
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
*          通道为JPEG抓图模式时，不需要调用FH_VENC_StartRecvPic/ FH_VENC_StopRecvPic。
*/
FH_SINT32 FH_VENC_StopRecvPic(FH_UINT32 chan);

/*
*   Name: FH_VENC_Submit_ENC
*            向编码通道提交图像数据进行编码
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_ENC_FRAME *pstVencsubmitframe
*            图像数据信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          调用此接口时通道不能处于绑定状态
*/
FH_SINT32 FH_VENC_Submit_ENC(FH_UINT32 chan,const FH_ENC_FRAME *pstVencsubmitframe);

/*
*   Name: FH_VENC_GetChnStatus
*            查询通道状态信息
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  FH_CHN_STATUS *pstVencStatus
*            通道状态信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          JPEG/MJPEG通道不支持该接口
*/
FH_SINT32 FH_VENC_GetChnStatus(FH_UINT32 chan, FH_CHN_STATUS *pstVencStatus);

/*
*   Name: FH_VENC_GetStream
*            获取编码通道码流。非阻塞。
*
*   Parameters:
*
*       [in] FH_UINT32 request_type
*            需要获取的码流类型(支持的类型见枚举FH_STREAM_TYPE，可以支持查询多种类型的码流)
*
*       [out] FH_VENC_STREAM *pstVencstreamAttr
*            码流信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          可以同时查询两者码流(例如FH_STREAM_JPEG|FH_STREAM_MJPEG)或直接查询所有编码模块(FH_STREAM_ALL)
*          该函数查询不同类型码流可以并发处理，但同一类型不可以并发。
*          例如：
*          正确：线程1：查询FH_STREAM_JPEG|FH_STREAM_MJPEG，线程2：查询FH_STREAM_H264
*          错误：线程1：查询FH_STREAM_JPEG|FH_STREAM_MJPEG，线程2：查询FH_STREAM_MJPEG
*          注意:MJPEG/JPEG在编码码流超过分配的缓冲区大小时会返回大小为0的码流,此时依然需要调用FH_VENC_ReleaseStream释放码流。
*/
FH_SINT32 FH_VENC_GetStream (FH_UINT32 request_type,FH_VENC_STREAM *pstVencstreamAttr);

/*
*   Name: FH_VENC_GetStream_Block
*            获取编码通道码流。阻塞时间1s。
*
*   Parameters:
*
*       [in] FH_UINT32 request_type
*            需要获取的码流类型(支持的类型见枚举FH_STREAM_TYPE，可以支持查询多种类型的码流)
*
*       [out] FH_VENC_STREAM *pstVencstreamAttr
*            码流信息
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          同FH_VENC_GetStream
*/
FH_SINT32 FH_VENC_GetStream_Block(FH_UINT32 request_type,FH_VENC_STREAM *pstVencstreamAttr);

/*
*   Name: FH_VENC_ReleaseStream
*            释放码流
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VENC_ReleaseStream(FH_UINT32 chan);

/*
*   Name: FH_VENC_SetRotate
*            设置编码旋转属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in]  FH_ROTATE_OPS pstVencrotateinfo
*            旋转属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          各平台支持功能有所差异,详见《视频函数开发参考手册》。
*/
FH_SINT32 FH_VENC_SetRotate(FH_UINT32 chan, FH_ROTATE_OPS pstVencrotateinfo);

/*
*   Name: FH_VENC_GetRotate
*            获取编码旋转属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  FH_ROTATE_OPS *pstVencrotateinfo
*            旋转属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VENC_GetRotate(FH_UINT32 chan, FH_ROTATE_OPS *pstVencrotateinfo);

/*
*   Name: FH_VENC_SetRoiCfg
*            设置ROI属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_ROI *pstVencroiinfo
*            ROI配置
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          JPEG、MJPEG通道不支持此接口。
*          ROI通过位图的形式设置，因此不支持获取ROI配置。
*/
FH_SINT32 FH_VENC_SetRoiCfg(FH_UINT32 chan,const FH_ROI *pstVencroiinfo);

/*
*   Name: FH_VENC_ClearRoi
*            清除之前设置的ROI配置
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
*          JPEG、MJPEG通道不支持此接口。
*/
FH_SINT32 FH_VENC_ClearRoi(FH_UINT32 chan);

/*
*   Name: FH_VENC_RequestIDR
*            申请强制I帧
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
*          JPEG、MJPEG通道不支持此接口。
*/
FH_SINT32 FH_VENC_RequestIDR(FH_UINT32 chan);

/*
*   Name: FH_VENC_GetCurPts
*            获取当前时间戳
*
*   Parameters:
*
*       [out] FH_UINT64 *Systemcurpts
*            时间戳
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VENC_GetCurPts(FH_UINT64 *Systemcurpts);


/*
*   Name: FH_VENC_SetRCAttr
*            设置码率控制参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_VENC_RC_ATTR *pstVencrcattr
*            码率控制参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          推荐用户使用此接口来更新码控参数，和FH_VENC_SetChnAttr比可以减少重新绑定，设置通道属性等参数.
*/
FH_SINT32 FH_VENC_SetRCAttr(FH_UINT32 chan,const FH_VENC_RC_ATTR *pstVencrcattr);

/*
*   Name: FH_VENC_GetRCAttr
*            获取码控参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_VENC_RC_ATTR *pstVencrcattr
*            码率控制参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VENC_GetRCAttr(FH_UINT32 chan,FH_VENC_RC_ATTR *pstVencrcattr);

/*
*   Name: FH_VENC_SetRcChangeParam
*            实时调整部分码控参数，通过该接口设置时会保留之前的码率情况，不会重置码控和强制I帧
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_VENC_RC_CHANGEPARAM *rcparam
*            码控参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          该接口可以用作一些对码率比较敏感的应用根据应用的反馈实时进行码率调整。
*          但调整时应注意尽可以的避免码率频繁震荡的来回设置，以免出现码率控制不准的情况。
*          MJPEG/JPEG通道不支持该接口。
*/
FH_SINT32 FH_VENC_SetRcChangeParam(FH_UINT32 chan,const FH_VENC_RC_CHANGEPARAM *rcparam);

/*
*   Name: FH_VENC_SetLostFrameAttr
*            设置丢帧策略参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_DROP_FRAME *pstVencdropattr
*            丢帧策略参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          目前支持以下3种模式的丢帧策略：
*          1. 用户设定的主动丢帧
*               - 作为用户自身丢帧策略的接口
*               - 作为在YUV通道一绑多情况下,不同通道的2次帧率控制
*          2. QP门限触发的丢帧(FixQP下不生效)
*          - 在码控QP调整到某个值(可能未到MAXQP时),优先降低帧率来降低码率,以帧率换取图像质量
*          3. 瞬时码率的丢帧(FixQP下不生效)
*               - 瞬时码率超过预设峰值时进行丢帧
*          通过该接口进行的丢帧,不会调整这一帧的时间戳,丢帧时会出现时间戳的跳跃。
*/
FH_SINT32 FH_VENC_SetLostFrameAttr(FH_UINT32 chan,const FH_DROP_FRAME *pstVencdropattr);

/*
*   Name: FH_VENC_GetLostFrameAttr
*            获取丢帧策略参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_DROP_FRAME *pstVencdropattr
*            丢帧策略参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VENC_GetLostFrameAttr(FH_UINT32 chan,FH_DROP_FRAME *pstVencdropattr);

/*
*   Name: FH_VENC_SetBkgQP
*            设置背景QP配置
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] FH_UINT32 bkg_delta_qp
*            背景QP与前景QP之间的差值
*
*       [in] FH_UINT32 bkg_max_qp
*            背景QP的最大值
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          MJPEG/JPEG通道不支持该接口。
*          在智能模式下生效.背景QP与前景QP之间的差值和背景QP的最大值,
*          当码控QP大于背景QP时，在QP分配上将退化为和普通编码一致;
*/
FH_SINT32 FH_VENC_SetBkgQP(FH_UINT32 chan,FH_UINT32 bkg_delta_qp,FH_UINT32 bkg_max_qp);

/*
*   Name: FH_VENC_GetBkgQP
*            获取背景QP配置
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_UINT32 *bkg_delta_qp
*            背景QP与前景QP之间的差值
*
*       [out] FH_UINT32 *bkg_max_qp
*            背景QP的最大值
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VENC_GetBkgQP(FH_UINT32 chan,FH_UINT32 *bkg_delta_qp,FH_UINT32 *bkg_max_qp);

/*
*   Name: FH_VENC_SetEncParam
*            配置一些编码参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] FH_ENCPARAM_CMD cmd
*            命令号
*
*       [in] void *param
*            命令号对应结构体
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          Cmd命令不适用于所有类型编码。
*          此函数主要定义一些不常用的参数设置。以减少SDK的API数量和变动。
*          正常流程中不需要使用此接口,主要用户效果调试或特殊需求等。
*/
FH_SINT32 FH_VENC_SetEncParam(FH_UINT32 chan,FH_ENCPARAM_CMD cmd,void *param);

/*
*   Name: FH_VENC_GetEncParam
*            获取一些编码参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] FH_ENCPARAM_CMD cmd
*            命令号
*
*       [out] void *param
*            命令号对应结构体
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*/
FH_SINT32 FH_VENC_GetEncParam(FH_UINT32 chan,FH_ENCPARAM_CMD cmd,void *param);

/*
*   Name: FH_VENC_SetEncryptSeed
*            设置加密密钥种子
*
*   Parameters:
*
*       [in] FH_UINT32 hash_seed0
*            加密密钥种子(和支持人员确定)
*
*       [in] FH_UINT32 hash_seed1
*            加密密钥种子(和支持人员确定)
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          MJPEG/JPEG通道不支持该接口。
*          一般用户无需设置该接口，除非富瀚支持人员要求。
*/
FH_SINT32 FH_VENC_SetEncryptSeed(FH_UINT32 hash_seed0,FH_UINT32 hash_seed1);

/*
*   Name: FH_VENC_SetEncRefMode
*            设置编码跳帧参考
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_VENC_REF_MODE *pVencrefmode
*            跳帧属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          MJPEG/JPEG通道不支持该接口。
*/
FH_SINT32 FH_VENC_SetEncRefMode(FH_UINT32 chan,const FH_VENC_REF_MODE *pVencrefmode);

/*
*   Name: FH_VENC_GetEncRefMode
*            获取编码跳帧参考
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  FH_VENC_REF_MODE *pVencrefmode
*            跳帧属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          MJPEG/JPEG通道不支持该接口。
*/
FH_SINT32 FH_VENC_GetEncRefMode(FH_UINT32 chan, FH_VENC_REF_MODE *pVencrefmode);

/*
*   Name: FH_VENC_SetH264Entropy
*            设置熵编码类型
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_H264_ENTROPY *pstVencentropy
*            熵编码属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H264编码支持该接口。
*/
FH_SINT32 FH_VENC_SetH264Entropy(FH_UINT32 chan,const FH_H264_ENTROPY *pstVencentropy);
/*
*   Name: FH_VENC_GetH264Entropy
*            获取熵编码类型
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_H264_ENTROPY *pstVencentropy
*            熵编码属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H264编码支持该接口。
*/
FH_SINT32 FH_VENC_GetH264Entropy(FH_UINT32 chan,FH_H264_ENTROPY *pstVencentropy);
/*
*   Name: FH_VENC_SetH264Dblk
*            设置Deblocking参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_H264_DBLK *pstVencdblk
*            Deblocking参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H264编码支持该接口。
*          含义参考H264协议
*/
FH_SINT32 FH_VENC_SetH264Dblk(FH_UINT32 chan,const FH_H264_DBLK *pstVencdblk);
/*
*   Name: FH_VENC_GetH264Dblk
*            获取Deblocking参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_H264_DBLK *pstVencdblk
*            Deblocking参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H264编码支持该接口。
*/
FH_SINT32 FH_VENC_GetH264Dblk(FH_UINT32 chan,FH_H264_DBLK *pstVencdblk);
/*
*   Name: FH_VENC_SetH264SliceSplit
*            设置多slice属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_H264_SLICE_SPLIT *pstVencslicesplit
*            多slice属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H264编码支持该接口。
*          slice数量不能超过16。
*/
FH_SINT32 FH_VENC_SetH264SliceSplit(FH_UINT32 chan,const FH_H264_SLICE_SPLIT *pstVencslicesplit);
/*
*   Name: FH_VENC_GetH264SliceSplit
*            获取多slice属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_H264_SLICE_SPLIT *pstVencslicesplit
*            多slice属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H264编码支持该接口。
*/
FH_SINT32 FH_VENC_GetH264SliceSplit(FH_UINT32 chan,FH_H264_SLICE_SPLIT *pstVencslicesplit);
/*
*   Name: FH_VENC_SetH264IntraFresh
*            设置帧内刷新属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_H264_INTRA_FRESH *pstVencintrafresh
*            帧内刷新属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H264编码支持该接口。
*/
FH_SINT32 FH_VENC_SetH264IntraFresh(FH_UINT32 chan,const FH_H264_INTRA_FRESH *pstVencintrafresh);
/*
*   Name: FH_VENC_GetH264IntraFresh
*            获取帧内刷新属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  FH_H264_INTRA_FRESH *pstVencintrafresh
*            帧内刷新属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H264编码支持该接口。
*/
FH_SINT32 FH_VENC_GetH264IntraFresh(FH_UINT32 chan, FH_H264_INTRA_FRESH *pstVencintrafresh);

/*
*   Name: FH_VENC_SetH265Dblk
*            设置Deblocking参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_H265_DBLK *pstVencdblk
*            Deblocking参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H265编码支持该接口。
*/
FH_SINT32 FH_VENC_SetH265Dblk(FH_UINT32 chan,const FH_H265_DBLK *pstVencdblk);
/*
*   Name: FH_VENC_GetH265Dblk
*            获取Deblocking参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_H265_DBLK *pstVencdblk
*            Deblocking参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H265编码支持该接口。
*/
FH_SINT32 FH_VENC_GetH265Dblk(FH_UINT32 chan,FH_H265_DBLK *pstVencdblk);
/*
*   Name: FH_VENC_SetH265SliceSplit
*            设置多slice属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_H265_SLICE_SPLIT *pstVencslicesplit
*            多slice属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H265编码支持该接口。
*/
FH_SINT32 FH_VENC_SetH265SliceSplit(FH_UINT32 chan,const FH_H265_SLICE_SPLIT *pstVencslicesplit);
/*
*   Name: FH_VENC_GetH265SliceSplit
*            获取多slice属性
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out] FH_H265_SLICE_SPLIT *pstVencslicesplit
*            多slice属性
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H265编码支持该接口。
*/
FH_SINT32 FH_VENC_GetH265SliceSplit(FH_UINT32 chan,FH_H265_SLICE_SPLIT *pstVencslicesplit);
/*
*   Name: FH_VENC_SetH265IntraFresh
*            设置帧内刷新参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [in] const FH_H265_INTRA_FRESH *pstVencintrafresh
*            帧内刷新参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H265编码支持该接口。
*/
FH_SINT32 FH_VENC_SetH265IntraFresh(FH_UINT32 chan,const FH_H265_INTRA_FRESH *pstVencintrafresh);
/*
*   Name: FH_VENC_GetH265IntraFresh
*            获取帧内刷新参数
*
*   Parameters:
*
*       [in] FH_UINT32 chan
*            通道号
*
*       [out]  FH_H265_INTRA_FRESH *pstVencintrafresh
*            帧内刷新参数
*
*   Return:
*           0(成功)
*          非0(失败，详见错误码)
*   Note:
*          仅H265编码支持该接口。
*/
FH_SINT32 FH_VENC_GetH265IntraFresh(FH_UINT32 chan, FH_H265_INTRA_FRESH *pstVencintrafresh);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /*__MPI_VO_H__ */



