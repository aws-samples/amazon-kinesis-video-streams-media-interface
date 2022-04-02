#ifndef __lib_dmc_h__
#define __lib_dmc_h__

#define DMC_MEDIA_TYPE_H264   	(1<<8)  /*H264*/
#define DMC_MEDIA_TYPE_H265    	(1<<9)  /*H265*/
#define DMC_MEDIA_TYPE_JPEG    	(1<<10) /*JPEG*/
#define DMC_MEDIA_TYPE_AUDIO   	(1<<11) /*Audio*/
#define DMC_MEDIA_TYPE_MJPEG    (1<<12) /*motion JPEG*/

/*DMC_MEDIA_TYPE_AUDIO: 媒体子类型*/
#define DMC_MEDIA_SUBTYPE_PCM  (1<<0)
#define DMC_MEDIA_SUBTYPE_ALAW (1<<1)
#define DMC_MEDIA_SUBTYPE_ULAW (1<<2)
#define DMC_MEDIA_SUBTYPE_AAC  (1<<3)

/*DMC_MEDIA_TYPE_H264/DMC_MEDIA_TYPE_H265: 媒体子类型*/
#define DMC_MEDIA_SUBTYPE_IFRAME  (1<<0)
#define DMC_MEDIA_SUBTYPE_PFRAME  (1<<1)
#define DMC_MEDIA_SUBTYPE_BFRAME  (1<<2)


typedef int (*dmc_media_input_fn)(int media_chn,
		                  int media_type,
		                  int media_subtype,
		                  unsigned long long frame_pts,
		                  unsigned char *frame_data,
		                  int frame_len,
		                  int frame_end_flag);

/*
 * 功能: DMC模块初始化
 */
extern int dmc_init(void);

/*
 *功能: 获取码流的线程通过调用此函数把码流输入DMC模块,
 *      DMC模块会同步把码流推送到subscribe的相关模块.
 *参数: media_chn,  媒体通道,0/1/2/3...
 *      media_type, 媒体类型,DMC_MEDIA_TYPE_xxx
 *      media_subtype, 媒体子类型,DMC_MEDIA_SUBTYPE_xxx
 *      frame_pts, 数据帧对应的pts,单位为微秒
 *      frame_data, 数据帧的地址
 *      frame_len, 数据帧的长度
 *      frame_end_flag, 
 *          数据帧结束标志,
 *          对于H264/H265来说,一帧包含好几个NAL单元,非0表示最后一个NAL单元.
 *          对于音频帧,JPEG帧,该标志始终为1
 */
extern int dmc_input(int media_chn,
		      int media_type,
		      int media_subtype,
		      unsigned long long frame_pts,
		      unsigned char *frame_data,
		      int frame_len,
		      int frame_end_flag);


/*
 *功能: 用于其他模块向DMC模块预订相关媒体,当DMC模块有数据输入的时候,
 *      就会调用对应的callback函数.
 *参数: module_name, 模块名字,比如"RTSP",系统内必须唯一
 *      media_type, 预订的媒体类型
 *      proc, 对应的callback函数
 */
extern int dmc_subscribe(char *module_name, int media_type, dmc_media_input_fn proc);


/*
 *功能: 取消预订
 *参数: module_name, 模块名字,比如"RTSP",系统内必须唯一
 *      media_type, 预订的媒体类型
 */
extern int dmc_unsubscribe(char *module_name, int media_type);

/*
 * 功能: DMC模块退出
 */
extern int dmc_deinit(void);

#endif //__lib_dmc_h__
