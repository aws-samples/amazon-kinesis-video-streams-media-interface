/******************************************************************************

  版权所有 (C),上海富瀚微电子股份有限公司

 ******************************************************************************
 文 件 名   : human_detect.h
 版 本 号   : 初稿
 作    者   : jinzq272
 生成日期   : 2018年07月23日
 最近修改   :
 功能描述   : 人形检测数据函数定义
 ******************************************************************************/
#ifndef __HUMAN_DETECT_H__
#define __HUMAN_DETECT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "sample_common.h"

#ifdef FH_APP_OPEN_OBJECT_DETECT
#include "human_detect/fh_objdetect_vbcli.h"

struct human_detect_config
{
    FH_UINT32 human_detect_chan;  /*用于人形检测的VPU通道*/
    FH_UINT8  do_yuv_trans;       /*人形检测的VPU通道的数据是否需要转换为光栅扫描顺序*/

    FH_UINT8  do_human_detect;    /*1: 立即进行人形检测,
                                   *0: 不进行人形检测，仅仅初始化人形模块，
                                   *   后续调用human_detect_control来启动人形检测
                                   */

    FH_UINT8  do_face_detect;     /*1: 立即进行人脸检测,
                                   *0: 不进行人脸检测，仅仅初始化人脸模块，
                                   *   后续调用human_detect_control来启动人脸检测
                                   */

    FH_UINT8  init_face_detect;   /*1: 初始化人脸检测模块
                                   *0: 不初始化人脸检测模块,影响如下：
                                   *     1) 为0时，do_face_detect无效；
                                   *     2) 为0时，后续不能调用human_detect_control使能人脸检测;
                                   */
    FH_SINT32 (*pre_filter_func)(
    	FH_UINT32 detect_frame_w,
    	FH_UINT32 detect_frame_h,
    	FH_UINT32 *x,
    	FH_UINT32 *y,
    	FH_UINT32 *w,
    	FH_UINT32 *h);

    FH_SINT32 (*post_filter_func)(
    	FH_UINT32 detect_frame_w,
    	FH_UINT32 detect_frame_h,
    	FH_UINT32 x,
    	FH_UINT32 y,
    	FH_UINT32 w,
    	FH_UINT32 h);

    FH_SINT32 (*process_human_func)(
    	FH_UINT32 detect_frame_w,
    	FH_UINT32 detect_frame_h,
    	FH_UINT32 human_sequence,
        FH_UINT32 model_serial,
        FH_UINT32 human_confidence,
        FH_ODET_objPos_t *p_human_pos,
        FH_UINT8 *src_data);

    FH_SINT32 (*process_face_func)(
    	FH_UINT32 detect_frame_w,
    	FH_UINT32 detect_frame_h,
    	FH_UINT32 face_sequence,
        FH_UINT32 model_serial,
        FH_UINT32 face_confidence,
        FH_ODET_objPos_t *p_face_pos,
        FH_UINT8 *src_data);
};

/*
 * 人形检测模块初始化
 */
FH_SINT32 human_detect_start(struct human_detect_config *pHumanConfig);

/*
 * 人形检测模块退出
 */
FH_SINT32 human_detect_stop(FH_VOID);

/*
 * 暂停/启动人形检测；
 * 暂停/启动人脸检测；
 */
FH_SINT32 human_detect_control(FH_SINT32 do_human_detect, FH_SINT32 do_face_detect);

/*
 * 设置人形/人脸检测的阈值
 */
FH_SINT32 human_detect_set_thresh(FH_SINT32 head_thresh, FH_SINT32 body_thresh, FH_SINT32 face_thresh);
#endif /* FH_APP_OPEN_OBJECT_DETECT */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /*__HUMAN_DETECT_H__*/
