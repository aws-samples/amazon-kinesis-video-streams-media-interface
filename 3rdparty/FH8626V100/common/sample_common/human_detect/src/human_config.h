/******************************************************************************

                  版权所有 (C),上海富瀚微电子股份有限公司

 ******************************************************************************
  文 件 名   : human_detect.h
  版 本 号   : v2
  作    者   : jinzq272
  生成日期   : 2019年12月09日
  最近修改   :
  功能描述   : 人形检测数据函数定义
******************************************************************************/
#ifndef __HUMAN_CONFIG_H__
#define __HUMAN_CONFIG_H__

#include "human_detect/fh_objdetect_vbcli.h"

#define HUMAN_DETECT_W    ALIGNTO(FH_APP_HUMAN_DETECT_W, 16)  /*人形检测幅面宽度*/
#define HUMAN_DETECT_H    ALIGNTO(FH_APP_HUMAN_DETECT_H, 16)  /*人形检测幅面高度*/

#define PIC_ROTATE     0                    /*人形检测旋转角度*/
#define MAX_SCALE_NUM  40                   /*人形检测最大缩放次数*/
#define SLIDE_STEP     2                    /*人形检测滑窗步长，步长越大，检测速度越快，但检测率会有所降低*/
#define PER_SCALE_RATE 128                  /*人形检测缩放系数，缩放系数越小，检测速度越快，检测率会有所降低，128是快速人形模式
                                              检测速度会提升很多，但会牺牲部分检测率*/

#define USE_MODEL_MODE FH_MODE_PERFORMACE_INHANCE   /*人形检测算法选择，FH_MODE_PERFORMACE_INHANCE为富瀚0.6版本人形检测算法
                                                      FH_MODE_YS_MODELS为萤石算法*/

#define HUMAN_MODEL_NUM         2  /* 加载的人形检测模型数量 */
#define MODEL_0_SERIAL_NUM      FH_ODET_HEAD_2_MODEL      /* 头肩模型 */
#define MODEL_0_THRESH          30                        /* 头肩模型阈值*/
#define MODEL_1_SERIAL_NUM      FH_ODET_BODY_2_MODEL      /* 整人模型 */
#define MODEL_1_THRESH          50                        /* 整人模型阈值*/
#define MODEL_2_SERIAL_NUM      0
#define MODEL_2_THRESH          0
#define MODEL_3_SERIAL_NUM      0
#define MODEL_3_THRESH          0

#define FACE_DETECT_W           146    /* 人脸检测幅面宽度 */
#define FACE_DETECT_H           146    /* 人脸检测幅面高度 */

#define FACE_MODEL_SERIAL       FH_ODET_FACE_MODEL /* 人脸检测模型 */
#define FACE_MODEL_THRESH       10                 /* 人脸模型阈值*/


#endif /*__HUMAN_CONFIG_H__*/
