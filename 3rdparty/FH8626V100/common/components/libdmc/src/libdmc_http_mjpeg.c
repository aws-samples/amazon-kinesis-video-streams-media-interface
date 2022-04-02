#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types/type_def.h"
#include "libdmc/include/libdmc.h"
#include "libdmc/include/libdmc_http_mjpeg.h"
#include <libmjpeg_over_http/include/libhttp_mjpeg.h>

static unsigned int g_mjpeg_port;
static unsigned int g_printed = 0;

static int _http_mjpeg_input_fn(int media_chn,
        int media_type,
        int media_subtype,
        unsigned long long frame_pts,
        unsigned char *frame_data,
        int frame_len,
        int frame_end_flag)
{
    libmjpeg_send_stream(frame_data, frame_len);
    if (!g_printed)
    {
        printf("http_mjpeg: listen on port %d ... \n", g_mjpeg_port);
        g_printed = 1;
    }

    return 0;
}

FH_SINT32 dmc_http_mjpeg_subscribe(FH_UINT16 port)
{
    int ret;

    ret = libmjpeg_start_server(port);
    if (ret == 0)
    {
        dmc_subscribe("HTTP_MJPEG", DMC_MEDIA_TYPE_MJPEG, _http_mjpeg_input_fn);
    }

    g_mjpeg_port = port;
    g_printed    = 0;

    return ret;
}

FH_SINT32 dmc_http_mjpeg_unsubscribe(FH_VOID)
{
    dmc_unsubscribe("HTTP_MJPEG", DMC_MEDIA_TYPE_MJPEG);
    libmjpeg_stop_server();
    return 0;
}
