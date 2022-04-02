#ifndef __FH_VENC_MPIPARA_NEW_H__
#define __FH_VENC_MPIPARA_NEW_H__
/**|VENC|**/
#include "types/type_def.h"
#include "fh_common.h"
#include "fh_bgm_mpi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define MAX_NALU_CNT 20

typedef enum
{
	FH_JPEG        = (1 << 0),// JPEG编码 | [ ]
	FH_MJPEG       = (1 << 1),// MJPEG编码 | [ ]
	FH_NORMAL_H264 = (1 << 2),// H264编码 | [ ]
	FH_SMART_H264  = (1 << 3),// 智能H264编码 | [ ]
	FH_NORMAL_H265 = (1 << 4),// H265编码 | [ ]
	FH_SMART_H265  = (1 << 5),// 智能H265编码 | [ ]
}FH_VENC_TYPE;

typedef enum
{
	FH_RC_MJPEG_FIXQP = 0, // MJPEG固定QP | [ ]
	FH_RC_MJPEG_CBR   = 1, // MJPEG定码率 | [ ]
	FH_RC_MJPEG_VBR   = 2, // MJPEG变码率 | [ ]
	FH_RC_H264_VBR    = 3, // H264变码率 | [ ]
	FH_RC_H264_CBR    = 4, // H264定码率 | [ ]
	FH_RC_H264_FIXQP  = 5, // H264固定QP | [ ]
	FH_RC_H264_AVBR   = 6, // H264自适应变码率 | [ ]
	FH_RC_H265_VBR    = 7, // H265变码率 | [ ]
	FH_RC_H265_CBR    = 8, // H265定码率 | [ ]
	FH_RC_H265_FIXQP  = 9, // H265固定QP | [ ]
	FH_RC_H265_AVBR   = 10,// H265自适应变码率 | [ ]
	FH_RC_H264_CVBR   = 11,// H264 CVBR | [ ]
	FH_RC_H265_CVBR   = 12,// H265 CVBR | [ ]
}FH_VENC_RC_MODE;

typedef enum
{
	FH_STREAM_JPEG  = (1 << 0),// JPEG码流 | [ ]
	FH_STREAM_MJPEG = (1 << 1),// MJPEG码流 | [ ]
	FH_STREAM_H264  = (1 << 2),// H264码流 | [ ]
	FH_STREAM_H265  = (1 << 3),// H265码流 | [ ]

	FH_STREAM_ALL   = 0xffffffff,// 所有码率类型 | [ ]
}FH_STREAM_TYPE;

typedef enum
{
	H264_PROFILE_BASELINE   = 66,// baseline profile | [ ]
	H264_PROFILE_MAIN       = 77,// main profile | [ ]
}FH_H264_PROFILE;

typedef enum
{
	H265_PROFILE_MAIN = 1,// main profile | [ ]
}FH_H265_PROFILE;

/*
* !!注意
* 因为H264和H265 对帧类型的定义不是完全一致
* H264:0-P 1-B 2-I
* H265:0-B 1-P 2-I
* 这里我们对两个标准使用统一的定义
* 同时我们加入我们对刷新帧的私有定义
*/
typedef enum {
	FH_FRAME_P              = 0,// P帧 | [ ]
	FH_FRAME_B              = 1,// B帧 | [ ]
	FH_FRAME_I              = 2,// I帧 | [ ]
	FH_FRAME_FRESH_P        = 20,// 刷新P帧 | [ ]
	FH_FRAME_FRESH_B        = 21,// 刷新B帧 | [ ]
} FH_ENC_FRAME_TYPE;

typedef enum {
	NALU_P_SLICE            = 0,
	NALU_B_SLICE            = 1,
	NALU_I_SLICE            = 2,
	NALU_IDR                = 5,
	NALU_SEI                = 6,
	NALU_SPS                = 7,
	NALU_PPS                = 8,
	NALU_AUD                = 9,
} FH_ENC_NALU_TYPE;

typedef struct
{
	FH_UINT32 support_type;            // 编码类型,可以同时支持多种类型(FH_VENC_TYPE) | [ ]
	FH_SIZE   max_size;                // 最大编码幅面 | [ ]
	FH_UINT32 bframe_num;              // 对B帧的支持,为保留字段,暂不支持 | [ ]
}FH_VENC_CHN_CAP;

/* H264 编码属性*/
typedef struct
{
	FH_H264_PROFILE profile;           // 编码档次 | [ ]
	FH_UINT32       i_frame_intterval; // GOP长度 | [ ]
	FH_SIZE         size;              // 编码图像幅面 | [ ]
}FH_VENC_H264_ATTR;

/*H264 智能编码属性*/
typedef struct
{
	FH_H264_PROFILE profile;                 // 编码档次 | [ ]
	FH_UINT32       refresh_frame_intterval; // 刷新帧间隔 | [ ]
	FH_SIZE         size;                    // 编码图像幅面 | [ ]

	FH_BOOL         smart_en;                // 智能编码使能 | [0-1]
	FH_BOOL         texture_en;              // 纹理映射结果使能 | [0-1]
	FH_BOOL         backgroudmodel_en;       // 背景建模结果使能 | [0-1]
	FH_BOOL         mbconsist_en;            // 宏块一致性结果使能 | [0-1]
	FH_GOP_TH       gop_th;                  // 设置静止帧门限，决定GOP长度 | [ ]
}FH_VENC_S264_ATTR;

/*H265 编码属性*/
typedef struct
{
	FH_H265_PROFILE profile;           // 编码档次 | [ ]
	FH_UINT32       i_frame_intterval; // GOP长度 | [ ]
	FH_SIZE         size;              // 编码图像幅面 | [ ]
}FH_VENC_H265_ATTR;

/*H265 智能编码属性*/
typedef struct
{
	FH_H265_PROFILE profile;                 // 编码档次 | [ ]
	FH_UINT32       refresh_frame_intterval; // 刷新帧间隔 | [ ]
	FH_SIZE         size;                    // 编码图像幅面 | [ ]

	FH_BOOL         smart_en;                // 智能编码使能 | [0-1]
	FH_BOOL         texture_en;              // 纹理映射结果使能 | [0-1]
	FH_BOOL         backgroudmodel_en;       // 背景建模结果使能 | [0-1]
	FH_BOOL         mbconsist_en;            // 宏块一致性结果使能 | [0-1]
	FH_GOP_TH       gop_th;                  // 设置静止帧门限，决定GOP长度 | [ ]
}FH_VENC_S265_ATTR;

/*JPEG 编码属性*/
typedef struct
{
	FH_UINT32 qp;           // 量化参数 | [0-98]
	FH_UINT32 rotate;       // 旋转角度 | [0-3]
	FH_UINT32 encode_speed; // 编码速度，推荐值4.值越小速度越快，对带宽占用越多 | [0-9]
}FH_VENC_JPEG_ATTR;

/*MJPEG 编码属性*/
typedef struct
{
	FH_SIZE   pic_size;     // 量化参数 | [0-98]
	FH_UINT32 rotate;       // 旋转角度 | [0-3]
	FH_UINT32 encode_speed; // 编码速度，推荐值4.值越小速度越快，对带宽占用越多 | [0-9]
}FH_VENC_MJPEG_ATTR;

typedef struct
{
	FH_VENC_TYPE enc_type;  // 编码类型 | [ ]
	union
	{
		FH_VENC_H264_ATTR  h264_attr;  // 普通H264编码属性 | [ ]
		FH_VENC_S264_ATTR  s264_attr;  // 智能H264编码属性 | [ ]
		FH_VENC_H265_ATTR  h265_attr;  // 普通H265编码属性 | [ ]
		FH_VENC_S265_ATTR  s265_attr;  // 智能H265编码属性 | [ ]
		FH_VENC_JPEG_ATTR  jpeg_attr;  // JPEG编码属性 | [ ]
		FH_VENC_MJPEG_ATTR mjpeg_attr; // MJPEG编码属性 | [ ]
	};
}FH_VENC_CHN_ATTR;

/*RC相关配置*/
typedef struct
{
	FH_FRAMERATE src_frmrate;            // 输入帧率，作为参考帧率 | [0-1]

	FH_UINT32    usrdrop_enable;         // 是否进行用户自定义丢帧 | [0-1]
	FH_FRAMERATE dst_frmrate;            // 输出帧率，输出帧率大于等于设定输入帧率，输出帧率为输入帧率 | [ ]

	FH_UINT32    qpdrop_enable;          // 是否允许QP门限触发丢帧 | [0-1]
	FH_UINT32    qpdrop_qpth;            // QP门限触发丢帧,QP的门限值 | [H264/H265 : 0-51,MJPEG : 0-98]
	FH_FRAMERATE qpdrop_minfrmrate;      // QP门限触发丢帧,丢帧后不低于该帧率 | [ ]

	FH_UINT32    InstantRate_enable;     // 是否允许瞬时码率触发丢帧 | [0-1]
	FH_UINT32    InstantRate_percentth;  // 触发瞬时码率丢帧的码率门限,为目标码率的百分比 | [110-1000]
	FH_FRAMERATE InstantRate_minfrmrate; // 瞬时码率的丢帧间隔，丢帧后不低于该帧率 | [ ]
}FH_DROP_FRAME;

typedef struct
{
	FH_UINT32    iqp;                 // I帧QP | [0-51]
	FH_UINT32    pqp;                 // P帧QP | [0-51]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。 | [ ]
}FH_H264_FIXQP_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             // 初始化QP | [0-51]
	FH_UINT32    bitrate;             // 目标码率，单位bit | [ ]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
	FH_UINT32    maxrate_percent;     // 最大码率，为目标码率的百分比。推荐值200。| [120-800]
	FH_UINT32    IFrmMaxBits;         // 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。| [ ]
	FH_SINT32    IP_QPDelta;          // P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.| [ ]
	FH_SINT32    I_BitProp;           // I帧P帧的码率分配比例，推荐值5。| [ ]
	FH_SINT32    P_BitProp;           // I帧P帧的码率分配比例，推荐值1。| [ ]
	FH_SINT32    fluctuate_level;     // 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。| [0-6]
}FH_H264_CBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             // 初始化QP | [0-51]
	FH_UINT32    bitrate;             // 目标码率，单位bit | [ ]
	FH_UINT32    IminQP;              // I帧最小QP | [0-51]
	FH_UINT32    ImaxQP;              // I帧最大QP | [0-51]
	FH_UINT32    PminQP;              // P帧最小QP | [0-51]
	FH_UINT32    PmaxQP;              // P帧最大QP | [0-51]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
	FH_UINT32    maxrate_percent;     // 最大码率，为目标码率的百分比。推荐值200。| [120-800]
	FH_UINT32    IFrmMaxBits;         // 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。| [ ]
	FH_SINT32    IP_QPDelta;          // P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.| [ ]
	FH_SINT32    I_BitProp;           // I帧P帧的码率分配比例，推荐值5。| [ ]
	FH_SINT32    P_BitProp;           // I帧P帧的码率分配比例，推荐值1。| [ ]
	FH_SINT32    fluctuate_level;     // 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。| [0-6]
}FH_H264_VBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             // 初始化QP | [0-51]
	FH_UINT32    bitrate;             // 目标码率，单位bit | [ ]
	FH_UINT32    IminQP;              // I帧最小QP | [0-51]
	FH_UINT32    ImaxQP;              // I帧最大QP | [0-51]
	FH_UINT32    PminQP;              // P帧最小QP | [0-51]
	FH_UINT32    PmaxQP;              // P帧最大QP | [0-51]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
	FH_UINT32    maxrate_percent;     // 最大码率，为目标码率的百分比。推荐值200。| [120-800]
	FH_UINT32    IFrmMaxBits;         // 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。| [ ]
	FH_SINT32    IP_QPDelta;          // P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.| [ ]
	FH_SINT32    I_BitProp;           // I帧P帧的码率分配比例，推荐值5。| [ ]
	FH_SINT32    P_BitProp;           // I帧P帧的码率分配比例，推荐值1。| [ ]
	FH_SINT32    fluctuate_level;     // 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。| [0-6]
	FH_UINT32    stillrate_percent;   // 静止下最小码率，单位%。推荐值30。| [25-100]
	FH_UINT32    maxstillqp;          // 静止最大QP，作为编码质量的限制。推荐值3４。| [0-51]
}FH_H264_AVBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;         // 初始化QP | [0-51]
	FH_UINT32    stbitrate;       // 短期目标码率，单位bit | [ ]
	FH_UINT32    ltbitrate;       // 长期目标码率，单位bit | [ ]
	FH_UINT32    maxrate_percent; // 最大码率，为短期目标码率的百分比。推荐值200。| [120-800]
	FH_UINT32    IminQP;          // I帧最小QP | [0-51]
	FH_UINT32    ImaxQP;          // I帧最大QP | [0-51]
	FH_UINT32    PminQP;          // P帧最小QP | [0-51]
	FH_UINT32    PmaxQP;          // P帧最大QP | [0-51]
	FH_FRAMERATE FrameRate;       // 参考帧率，没有帧率控制的作用。| [ ]
	FH_UINT32    IFrmMaxBits;     // 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。| [ ]
	FH_SINT32    IP_QPDelta;      // P帧最小QP与当前I帧QP 的限制，pqp > = iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.| [ ]
	FH_SINT32    I_BitProp;       // I帧P帧的码率分配比例，推荐值5。| [ ]
	FH_SINT32    P_BitProp;       // I帧P帧的码率分配比例，推荐值1。| [ ]
	FH_SINT32    fluctuate_level; // 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。| [0-6]
	FH_SINT32    acceptable_qp;   // cvbr模式时可接受图像质量阈值,QP小于等于此值时不会透支码率,推荐值:32 | [0-51]
}FH_H264_CVBR_ATTR;

typedef struct
{
	FH_UINT32    iqp;                 // I帧QP | [0-51]
	FH_UINT32    qp;                  // P/B帧QP | [0-51]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。 | [ ]
}FH_H265_FIXQP_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             // 初始化QP | [0-51]
	FH_UINT32    bitrate;             // 目标码率，单位bit | [ ]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
	FH_UINT32    maxrate_percent;     // 最大码率，为目标码率的百分比。推荐值200。| [120-800]
	FH_UINT32    IFrmMaxBits;         // 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。| [ ]
	FH_SINT32    IP_QPDelta;          // P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.| [ ]
	FH_SINT32    I_BitProp;           // I帧P帧的码率分配比例，推荐值5。| [ ]
	FH_SINT32    P_BitProp;           // I帧P帧的码率分配比例，推荐值1。| [ ]
	FH_SINT32    fluctuate_level;     // 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。| [0-6]
}FH_H265_CBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             // 初始化QP | [0-51]
	FH_UINT32    bitrate;             // 目标码率，单位bit | [ ]
	FH_UINT32    IminQP;              // I帧最小QP | [0-51]
	FH_UINT32    ImaxQP;              // I帧最大QP | [0-51]
	FH_UINT32    PminQP;              // P帧最小QP | [0-51]
	FH_UINT32    PmaxQP;              // P帧最大QP | [0-51]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
	FH_UINT32    maxrate_percent;     // 最大码率，为目标码率的百分比。推荐值200。| [120-800]
	FH_UINT32    IFrmMaxBits;         // 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。| [ ]
	FH_SINT32    IP_QPDelta;          // P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.| [ ]
	FH_SINT32    I_BitProp;           // I帧P帧的码率分配比例，推荐值5。| [ ]
	FH_SINT32    P_BitProp;           // I帧P帧的码率分配比例，推荐值1。| [ ]
	FH_SINT32    fluctuate_level;     // 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。| [0-6]
}FH_H265_VBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;             // 初始化QP | [0-51]
	FH_UINT32    bitrate;             // 目标码率，单位bit | [ ]
	FH_UINT32    IminQP;              // I帧最小QP | [0-51]
	FH_UINT32    ImaxQP;              // I帧最大QP | [0-51]
	FH_UINT32    PminQP;              // P帧最小QP | [0-51]
	FH_UINT32    PmaxQP;              // P帧最大QP | [0-51]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
	FH_UINT32    maxrate_percent;     // 最大码率，为目标码率的百分比。推荐值200。| [120-800]
	FH_UINT32    IFrmMaxBits;         // 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。| [ ]
	FH_SINT32    IP_QPDelta;          // P帧最小QP与当前I帧QP 的限制，pqp >= iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.| [ ]
	FH_SINT32    I_BitProp;           // I帧P帧的码率分配比例，推荐值5。| [ ]
	FH_SINT32    P_BitProp;           // I帧P帧的码率分配比例，推荐值1。| [ ]
	FH_SINT32    fluctuate_level;     // 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。| [0-6]
	FH_UINT32    stillrate_percent;   // 静止下最小码率，单位%。推荐值30。| [25-100]
	FH_UINT32    maxstillqp;          // 静止最大QP，作为编码质量的限制。推荐值3４。| [0-51]
}FH_H265_AVBR_ATTR;

typedef struct
{
	FH_UINT32    init_qp;         // 初始化QP | [0-51]
	FH_UINT32    stbitrate;       // 短期目标码率，单位bit | [ ]
	FH_UINT32    ltbitrate;       // 长期目标码率，单位bit | [ ]
	FH_UINT32    maxrate_percent; // 最大码率，为短期目标码率的百分比。推荐值200。| [120-800]
	FH_UINT32    IminQP;          // I帧最小QP | [0-51]
	FH_UINT32    ImaxQP;          // I帧最大QP | [0-51]
	FH_UINT32    PminQP;          // P帧最小QP | [0-51]
	FH_UINT32    PmaxQP;          // P帧最大QP | [0-51]
	FH_FRAMERATE FrameRate;       // 参考帧率，没有帧率控制的作用。| [ ]
	FH_UINT32    IFrmMaxBits;     // 允许的最大I帧大小，单位为bit.设为0为不限制I帧大小。| [ ]
	FH_SINT32    IP_QPDelta;      // P帧最小QP与当前I帧QP 的限制，pqp > = iqp + IP_QPDelta.可以用来抑制呼吸效应等现象。推荐值3.| [ ]
	FH_SINT32    I_BitProp;       // I帧P帧的码率分配比例，推荐值5。| [ ]
	FH_SINT32    P_BitProp;       // I帧P帧的码率分配比例，推荐值1。| [ ]
	FH_SINT32    fluctuate_level; // 码率波动级别，从小到大，为从质量平稳优先到码率平稳优先。推荐值0。| [0-6]
	FH_SINT32    acceptable_qp;   // cvbr模式时可接受图像质量阈值,QP小于等于此值时不会透支码率,推荐值:32 | [0-51]
}FH_H265_CVBR_ATTR;

typedef struct
{
	FH_UINT32    qp;                  // 量化参数 | [0-98]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
}FH_MJPEG_FIXQP_ATTR;

typedef struct
{
	FH_UINT32    initqp;              // 量化参数 | [0-98]
	FH_UINT32    bitrate;             // 目标码率，单位bit | [ ]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
}FH_MJPEG_CBR_ATTR;

typedef struct
{
	FH_UINT32    initqp;              // 量化参数 | [0-98]
	FH_UINT32    bitrate;             // 目标码率，单位bit | [ ]
	FH_UINT32    minqp;               // 码控最小QP | [0-98]
	FH_UINT32    maxqp;               // 码控最大QP | [0-98]
	FH_FRAMERATE FrameRate;           // 参考帧率，没有帧率控制的作用。| [ ]
}FH_MJPEG_VBR_ATTR;

typedef struct
{
	FH_VENC_RC_MODE rc_type;          // 码控模式 | [ ]
	union
	{
		FH_H264_FIXQP_ATTR  h264_fixqp;  // H264 定QP | [ ]
		FH_H264_CBR_ATTR    h264_cbr;    // H264 定码率 | [ ]
		FH_H264_VBR_ATTR    h264_vbr;    // H264 变码率 | [ ]
		FH_H264_AVBR_ATTR   h264_avbr;   // H264 自适应变码率 | [ ]
		FH_H264_CVBR_ATTR   h264_cvbr;   // H264 CVBR | [ ]

		FH_H265_FIXQP_ATTR  h265_fixqp;  // H265 定QP | [ ]
		FH_H265_CBR_ATTR    h265_cbr;    // H265 定码率 | [ ]
		FH_H265_VBR_ATTR    h265_vbr;    // H265 变码率 | [ ]
		FH_H265_AVBR_ATTR   h265_avbr;   // H265 自适应变码率 | [ ]
		FH_H265_CVBR_ATTR   h265_cvbr;   // H265 CVBR | [ ]

		FH_MJPEG_FIXQP_ATTR mjpeg_fixqp; // MJPEG 定QP | [ ]
		FH_MJPEG_CBR_ATTR   mjpeg_cbr;   // MJPEG 定码率 | [ ]
		FH_MJPEG_VBR_ATTR   mjpeg_vbr;   // MJPEG 变码率 | [ ]
	};
}FH_VENC_RC_ATTR;

typedef struct
{
	FH_VENC_CHN_ATTR chn_attr;                  // 通道属性 | [ ]
	FH_VENC_RC_ATTR  rc_attr;                   // 码率属性 | [ ]
}FH_VENC_CHN_CONFIG;

typedef struct
{
	FH_ADDR                 lumma_addr;         // 亮度地址 | [ ]
	FH_ADDR                 chroma_addr;        // 色度地址 | [ ]
	FH_UINT64               time_stamp;         // 时间戳 | [ ]
	FH_SIZE                 size;               // 幅面 | [ ]
}FH_ENC_FRAME;

typedef struct
{
	FH_ENC_NALU_TYPE        type;               // NALU类型 | [ ]
	FH_UINT32               length;             // 码流长度 | [ ]
	FH_ADDR                 start;              // 码流地址 | [ ]
}FH_ENC_STREAM_NALU;

typedef struct
{
	FH_UINT32               chan;               // 通道号 | [ ]
	FH_ADDR                 start;              // 码流地址 | [ ]
	FH_ENC_FRAME_TYPE       frame_type;         // 帧类型 | [ ]
	FH_UINT32               length;             // 码流长度 | [ ]
	FH_UINT64               time_stamp;         // 时间戳 | [ ]
	FH_UINT32               nalu_cnt;           // NALU数量 | [ ]
	FH_ENC_STREAM_NALU      nalu[MAX_NALU_CNT]; // NALU属性 | [ ]
}FH_H264_STREAM;

typedef struct
{
	FH_UINT32               chan;               // 通道号 | [ ]
	FH_ADDR                 start;              // 码流地址 | [ ]
	FH_ENC_FRAME_TYPE       frame_type;         // 帧类型 | [ ]
	FH_UINT32               length;             // 码流长度 | [ ]
	FH_UINT64               time_stamp;         // 时间戳 | [ ]
	FH_UINT32               nalu_cnt;           // NALU数量 | [ ]
	FH_ENC_STREAM_NALU      nalu[MAX_NALU_CNT]; // NALU属性 | [ ]
}FH_H265_STREAM;

typedef struct
{
	FH_UINT32               chan;               // 通道号 | [ ]
	FH_ADDR                 start;              // 码流地址 | [ ]
	FH_UINT32               length;             // 码流长度 | [ ]
	FH_UINT64               time_stamp;         // 时间戳 | [ ]
	FH_UINT32               qp;                 // 当前帧使用的量化值 | [ ]
}FH_JPEG_STREAM;

typedef struct
{
	FH_STREAM_TYPE stmtype;                     // 码流类型 | [ ]
	union
	{
		FH_H264_STREAM h264_stream;                // H264码流 | [ ]
		FH_H265_STREAM h265_stream;                // H265码流 | [ ]
		FH_JPEG_STREAM jpeg_stream;                // JPEG码流 | [ ]
		FH_JPEG_STREAM mjpeg_stream;               // MJPEG 码流 | [ ]
	};
}FH_VENC_STREAM;

typedef struct
{
	FH_UINT32               lastqp;             // 上帧QP | [ ]
	FH_UINT32               lastiqp;            // 上一I帧QP | [ ]
	FH_UINT32               bps;                // 实际码率 c| [ ]
	FH_UINT32               FrameToEnc;         // 待编码帧数 | [ ]
	FH_UINT32               framecnt;           // 已编码帧数 | [ ]
	FH_UINT32               streamcnt;          // 输出队列中的帧数 | [ ]
	FH_UINT32               lostcnt;            // 通道的累计丢帧数 | [ ]
	FH_UINT32               stat[5];            // 保留变量 | [ ]
}FH_CHN_STATUS;

typedef struct
{
 	FH_UINT32               enable;             // ROI使能 | [0-1]
	FH_SINT32               qp;                 // 对应level使用的QP | [绝对QP:0-51，相对QP:-51-51]
	FH_AREA                 area;               // ROI区域 | [ ]
	FH_UINT32               level;              // ROI level,0代表不使用ROI | [0-3]
	FH_BOOL                 isdeltaqp;          // 0:绝对QP,使用用户QP 1:相对QP,在RC QP基础上加减Delta值 | [0-1]
}FH_ROI;

typedef struct
{
	FH_UINT32               bitrate;            // 目标码率，单位bit | [ ]
	FH_UINT32               IminQP;             // I帧最小QP | [0-51]
	FH_UINT32               ImaxQP;             // I帧最大QP | [0-51]
	FH_UINT32               PminQP;             // P帧最小QP | [0-51]
	FH_UINT32               PmaxQP;             // P帧最大QP | [0-51]
	FH_FRAMERATE            FrameRate;          // 参考帧率，没有帧率控制的作用。| [ ]
}FH_VENC_RC_CHANGEPARAM;

typedef struct
{
	FH_UINT32             mode;                 // 0:不支持抽帧 1:单层跳帧 2:双层跳帧 | [0-2]
	FH_SINT32             svc_m;                // 第一层跳帧的间隔，mode = 1 or 2有效 | [H264:0-15,H265:0-7]
	FH_SINT32             svc_n;                // 第二层跳帧的间隔，mode = 2 有效 | [H264:0-15,H265:0-7]
}FH_VENC_REF_MODE;

typedef enum
{
	ENCPARAM_CMD_NONE                = 0,
	ENCPARAM_CMD_H264_LINE_RC        = 1,// 行级码控,适用于H264 | [ ]
	ENCPARAM_CMD_LINE_RC             = 1,// 行级码控,适用于H264&H265 | [ ]
	ENCPARAM_CMD_H264_SEARCH_RANGE   = 2,// 搜索范围,适用于H264 | [ ]
	ENCPARAM_CMD_H264_CHROMAQP_DELTA = 3,// 色度QP偏移,适用于H264 | [ ]
	ENCPARAM_CMD_H264_ENCODE_STYLE   = 4,// 编码风格,适用于H264 | [ ]
	ENCPARAM_CMD_TEXTUREQP_RANGE     = 5,// 纹理QP范围,适用于H264&H265 | [ ]
	ENCPARAM_CMD_RC_ADV_PARAM        = 6,// 高级码控参数,适用于H264&H265 | [ ]
}FH_ENCPARAM_CMD;

typedef struct
{
	FH_UINT32 linerc_en;        // 行级码控使能 | [0-1]
	FH_UINT32 linerc_iadd;      // 行级码控I帧QP增加范围 | [0-51]
	FH_UINT32 linerc_iminus;    // 行级码控I帧QP减少范围 | [0-51]
	FH_UINT32 linerc_padd;      // 行级码控P帧QP增加范围 | [0-51]
	FH_UINT32 linerc_pminus;    // 行级码控P帧QP减少范围 | [0-51]
}ENCPARAM_H264_LINE_RC;

typedef ENCPARAM_H264_LINE_RC ENCPARAM_LINE_RC;

typedef struct
{
	FH_UINT32 search_x;         // 编码搜索范围x,,范围为(-(i+1)*16,(i+1)*16-1).增大搜索范围会导致编码性能下降，默认0 | [0~3]
	FH_UINT32 search_y;         // 编码搜索范围y,,范围为(-(i+1)*16,(i+1)*16-1).增大搜索范围会导致编码性能下降，默认0 | [0~3]
}ENCPARAM_H264_SEARCH_RANGE;

typedef struct
{
	FH_SINT32 ChromaQPdelta;    // 色度QP相对与亮度QP的delta | [-12~12]
}ENCPARAM_H264_CHROMAQP_DELTA_ST;

typedef enum
{
	ENCODE_STYLE_NORMAL = 0,
	ENCODE_STYLE_NIGHT = 1,
}ENCPARAM_H264_ENCODE_STYLE;

typedef struct
{
	FH_UINT32 text_minqp_minus; // 纹理映射的最小QP在原先RC的最小QP上减少的值 | [0-51]
	FH_UINT32 text_maxqp_add;   // 纹理映射的最大QP在原先RC的最大QP上的增加的值 | [0-51]
}ENCPARAM_TEXTUREQP_RANGE_ST;

typedef struct
{
	FH_SINT32 iqp_weight_tbit; // I帧QP分配,目标码率的影响权重，推荐值:1。 | [ ]
	FH_SINT32 iqp_weight_pqp;  // I帧QP分配,之前GOP PQP的影响权重，推荐值:2。 | [ ]
	FH_SINT32 pqp_limit;       // gop pqp在I帧QP上允许下降的幅度，推荐值:-3。 | [ ]
	FH_UINT32 vbuf_depth;      // 码控虚拟缓存深度,含义为平均每帧码率的倍数，推荐值:40。 | [ ]
}ENCPARAM_RC_ADV_PARAM;

typedef struct
{
	FH_UINT32             entropy_coding_mode; // 详细含义参见H264协议 0 : cavlc 1 : cabac baseline不支持cabac | [0-1]
	FH_UINT32             cabac_init_idc;      // 详细含义参见H264协议 | [0-2]
}FH_H264_ENTROPY;

typedef struct
{
	FH_UINT32             deblocking_filter;   // 详细含义参见H264协议 | [0-1]
	FH_UINT32             disable_deblocking;  // 详细含义参见H264协议 | [0-2]
	FH_SINT32             slice_alpha;         // 详细含义参见H264协议 | [-6~+6]
	FH_SINT32             slice_beta;          // 详细含义参见H264协议 | [-6~+6]
}FH_H264_DBLK;

typedef struct
{
 	FH_UINT32             enable;             // 使能 | [0-1]
 	FH_UINT32             slicesplit;         // 多少个宏块行作为一个slice。最多不能多于16个slice。 | [ ]
}FH_H264_SLICE_SPLIT;

typedef struct
{
	FH_UINT32             enable;              // 使能 | [0-1]
	FH_UINT32             fresh_line;          // 刷新的Islice的行数 | [ ]
	FH_UINT32             fresh_step;          // 每帧刷新Islice的步进行数 | [ ]
}FH_H264_INTRA_FRESH;

typedef struct
{
	FH_UINT32             disable_flag;     // 详细含义参见H265协议 | [0-1]
	FH_SINT32             beta_offset_div2; // 详细含义参见H265协议 | [-6~+6]
	FH_SINT32             tc_offset_div2;   // 详细含义参见H265协议 | [-6~+6]
}FH_H265_DBLK;

typedef struct
{
 	FH_UINT32             mode;            // 0 : 单slice 1 : 按行划分slice | [0-1]
 	FH_UINT32             slice_arg;       // Slice行数(64-aligned)，slice不能超过15个 | [ ]
}FH_H265_SLICE_SPLIT;

typedef struct
{
	FH_UINT32             mode;             // 0: 不使用帧内刷新 1:按行刷新 2:按列刷新 3:按CTU进行刷新 | [0-3]
	FH_UINT32             fresh_line;       // 刷新帧内编码的行数 | [ ]
}FH_H265_INTRA_FRESH;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
