#ifndef __lib_http_mjpeg_h__
#define __lib_http_mjpeg_h__

#ifdef __cplusplus
extern "C" {
#endif


extern FH_SINT32 libmjpeg_start_server(FH_UINT16 port);
extern FH_VOID libmjpeg_stop_server(FH_VOID);
extern FH_VOID libmjpeg_send_stream(FH_VOID *data, FH_UINT32 size);


#ifdef __cplusplus
}
#endif

#endif /*__lib_http_mjpeg_h__*/
