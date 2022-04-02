#ifndef __CONFIG_H__
#define __CONFIG_H__

/*Automatically generated make config: don't edit */
/*Linux Kernel Configuration */
/*Thu Jan  6 15:00:17 2022 */

#define FH_APP_OPEN_AUDIO
/*FH_APP_OPEN_OVERLAY is not set */
/*FH_APP_OPEN_VENC is not set */
/*FH_APP_OPEN_ISP_STRATEGY_DEMO is not set */
/*FH_APP_OPEN_MOTION_DETECT is not set */
/*FH_APP_OPEN_RECT_MOTION_DETECT is not set */
/*FH_APP_OPEN_COVER_DETECT is not set */
/*FH_APP_OPEN_OBJECT_DETECT is not set */
/*FH_APP_OPEN_IVS is not set */

/*ISP Config */

/*FH_APP_USING_IRCUT is not set */
#define MAX_ISP_WIDTH 0
#define MAX_ISP_HEIGHT 0
/*FH_USING_JXF23_MIPI is not set */
/*FH_USING_JXF23_DVP is not set */
/*FH_USING_JXF23_IR is not set */
#define FH_USING_JXF37_MIPI
/*FH_USING_JXH62_MIPI is not set */
/*FH_USING_SC2231_DVP is not set */
/*FH_APP_USING_FORMAT_1080P30 is not set */
/*FH_APP_USING_FORMAT_1080P25 is not set */
/*FH_APP_USING_FORMAT_1080P20 is not set */
#define FH_APP_USING_FORMAT_1080P15

/*DSP Config */

#define CH0_ENABLE
#define CH0_MAX_WIDTH 0
#define CH0_MAX_HEIGHT 0
#define CH0_WIDTH 1920
#define CH0_HEIGHT 1080
#define CH0_BIT_RATE 2048
#define CH0_FRAME_COUNT 15
#define CH0_FRAME_TIME 1
#define FH_CH0_USING_SAMPLE_H264
/*FH_CH0_USING_SAMPLE_S264 is not set */
/*FH_CH0_USING_SAMPLE_MJPEG is not set */
/*FH_CH0_USING_SAMPLE_NONE is not set */
#define FH_CH0_USING_SAMPLE_H264_VBR
/*FH_CH0_USING_SAMPLE_H264_FIXQP is not set */
/*FH_CH0_USING_SAMPLE_H264_CBR is not set */
/*FH_CH0_USING_SAMPLE_H264_AVBR is not set */
/*FH_CH0_USING_SAMPLE_H264_CVBR is not set */
/*CH1_ENABLE is not set */
/*FH_CH1_USING_SAMPLE_H264 is not set */
/*FH_CH1_USING_SAMPLE_S264 is not set */
/*FH_CH1_USING_SAMPLE_MJPEG is not set */
/*FH_CH1_USING_SAMPLE_NONE is not set */
/*FH_CH1_USING_SAMPLE_H264_VBR is not set */
/*FH_CH1_USING_SAMPLE_H264_FIXQP is not set */
/*FH_CH1_USING_SAMPLE_H264_CBR is not set */
/*FH_CH1_USING_SAMPLE_H264_AVBR is not set */
/*FH_CH1_USING_SAMPLE_H264_CVBR is not set */
/*CH2_ENABLE is not set */
/*FH_CH2_USING_SAMPLE_H264 is not set */
/*FH_CH2_USING_SAMPLE_S264 is not set */
/*FH_CH2_USING_SAMPLE_MJPEG is not set */
/*FH_CH2_USING_SAMPLE_NONE is not set */
/*FH_CH2_USING_SAMPLE_H264_VBR is not set */
/*FH_CH2_USING_SAMPLE_H264_FIXQP is not set */
/*FH_CH2_USING_SAMPLE_H264_CBR is not set */
/*FH_CH2_USING_SAMPLE_H264_AVBR is not set */
/*FH_CH2_USING_SAMPLE_H264_CVBR is not set */

/*Network Protocol for sending stream */

// #define FH_APP_USING_PES
// #define FH_APP_USING_UDP
/*FH_APP_USING_SRT is not set */
// #define FH_APP_USING_RTSP        // rtsp test only
/*FH_APP_RECORD_RAW_STREAM is not set */
#define FH_APP_USING_COOLVIEW

#endif
