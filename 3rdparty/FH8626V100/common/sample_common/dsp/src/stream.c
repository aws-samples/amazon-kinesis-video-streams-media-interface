#include <sample_common.h>

#define HTTP_MJPEG_PORT      (1111)

FH_SINT32 sample_dmc_init(FH_CHAR *dst_ip, FH_UINT32 port ,FH_SINT32 max_channel_no)
{
    dmc_init();

#ifdef FH_APP_USING_PES
    if (dst_ip != NULL && *dst_ip != 0)
    {
        dmc_pes_subscribe(max_channel_no, dst_ip, port);
    }
#endif

#ifdef FH_APP_USING_RTSP
    dmc_rtsp_subscribe(max_channel_no, port);
#endif

#ifdef FH_APP_RECORD_RAW_STREAM
    dmc_record_subscribe(max_channel_no);
#endif

    dmc_http_mjpeg_subscribe(HTTP_MJPEG_PORT);

    return 0;
}

FH_SINT32 sample_dmc_deinit(FH_VOID)
{
    dmc_http_mjpeg_unsubscribe();

    #ifdef FH_APP_USING_RTSP
    dmc_rtsp_unsubscribe();
    #endif

    #ifdef FH_APP_USING_PES
    dmc_pes_unsubscribe();
    #endif

    #ifdef FH_APP_RECORD_RAW_STREAM
    dmc_record_unsubscribe();
    #endif

    dmc_deinit();

    return 0;
}

FH_VOID *sample_common_get_stream_proc(FH_VOID *arg)
{
    FH_SINT32 ret, i;
    FH_SINT32 end_flag;
    FH_SINT32 subtype;
    FH_VENC_STREAM stream;
    FH_UINT32 chan = 0;
    FH_SINT32 *stop = (FH_SINT32 *)arg;

    while (*stop == 0)
    {
    	/*阻塞模式下,获取一帧H264或者H265数据*/
        ret = FH_VENC_GetStream_Block(FH_STREAM_ALL & (~(FH_JPEG)), &stream);
        if (ret != RETURN_OK)
        	continue;

		/*获取到一帧H264数据,按照下面的方式处理*/
        if (stream.stmtype == FH_STREAM_H264)
        {
            subtype = stream.h264_stream.frame_type == FH_FRAME_I ? DMC_MEDIA_SUBTYPE_IFRAME : DMC_MEDIA_SUBTYPE_PFRAME;
            for (i = 0; i < stream.h264_stream.nalu_cnt; i++)
            {
            	end_flag = (i == (stream.h264_stream.nalu_cnt - 1)) ? 1 : 0;
            	chan = stream.h264_stream.chan;
                dmc_input(chan,
			    		  DMC_MEDIA_TYPE_H264,
			    		  subtype,
			    		  stream.h264_stream.time_stamp,
		    		      stream.h264_stream.nalu[i].start,
		    		      stream.h264_stream.nalu[i].length,
		    		      end_flag);
            }
        }
        #if defined(CONFIG_ARCH_YG) || defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
		/*获取到一帧H265数据,按照下面的方式处理*/
        else if (stream.stmtype == FH_STREAM_H265)
        {
            subtype = stream.h265_stream.frame_type == FH_FRAME_I ? DMC_MEDIA_SUBTYPE_IFRAME : DMC_MEDIA_SUBTYPE_PFRAME;
            for (i = 0; i < stream.h265_stream.nalu_cnt; i++)
			{
            	end_flag = (i == (stream.h265_stream.nalu_cnt - 1)) ? 1 : 0;
            	chan = stream.h265_stream.chan;
                dmc_input(chan,
			    		  DMC_MEDIA_TYPE_H265,
			    		  subtype,
			    		  stream.h265_stream.time_stamp,
		    		      stream.h265_stream.nalu[i].start,
		    		      stream.h265_stream.nalu[i].length,
		    		      end_flag);
			}
        }
        #endif

        /*获取到一帧MJPEG数据,按照下面的方式处理*/
        else if (stream.stmtype == FH_STREAM_MJPEG)
        {
            chan  = stream.mjpeg_stream.chan;
            dmc_input(chan,
                      DMC_MEDIA_TYPE_MJPEG,
                      0,
                      0,
                      stream.mjpeg_stream.start,
                      stream.mjpeg_stream.length,
                      1);
        }

		/*必须和FH_VENC_GetStream配套调用,以释放码流资源*/
        FH_VENC_ReleaseStream(chan);
    }

    *stop = 0;
    return NULL;
}
