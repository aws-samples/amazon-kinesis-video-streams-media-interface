#include <stdio.h>
#include <string.h>
#include "libdmc/include/libdmc.h"
#include "libdmc/include/libdmc_pes.h"
#include "libpes/include/libpes.h"

#define MAX_PES_CHANNEL_COUNT 8

struct dmc_pes_info
{
    char tar_ip[16];
    unsigned int port[MAX_PES_CHANNEL_COUNT];
    unsigned int printed[MAX_PES_CHANNEL_COUNT];
};

static int g_frame_length;
static int g_nalu_count;
static struct vlcview_enc_stream_element g_stream_element;
static struct dmc_pes_info g_print_info;

static int _pes_input_fn(int media_chn, int media_type, int media_subtype, \
        unsigned long long frame_pts, unsigned char *frame_data, \
        int frame_len, int frame_end_flag )
{
    int ret;

    if (frame_len > 0)
    {
        if (g_nalu_count >= MAX_NALU_COUNT)
        {
            printf("Error: libpes, max supported NALU is %d!\n", MAX_NALU_COUNT);
            return 0;
        }

        if (media_type == DMC_MEDIA_TYPE_H264)
            g_stream_element.enc_type = VLCVIEW_ENC_H264;
        else /*if (media_type == DMC_MEDIA_TYPE_H265)*/
            g_stream_element.enc_type = VLCVIEW_ENC_H265;

        if (media_subtype == DMC_MEDIA_SUBTYPE_IFRAME)
            g_stream_element.frame_type = VLCVIEW_ENC_I_FRAME;
        else /*if (media_subtype == DMC_MEDIA_SUBTYPE_PFRAME)*/
            g_stream_element.frame_type = VLCVIEW_ENC_P_FRAME;

        g_stream_element.time_stamp = frame_pts;
        g_stream_element.nalu[g_nalu_count].start = frame_data;
        g_stream_element.nalu[g_nalu_count].len = frame_len;

        g_frame_length += frame_len;
        g_nalu_count++;
    }

    if (frame_end_flag)
    {
        g_stream_element.nalu_count = g_nalu_count;
        g_stream_element.frame_len  = g_frame_length;
        g_frame_length = 0;
        g_nalu_count = 0;

        ret = libpes_stream_pack(media_chn, g_stream_element);
        if(ret)
        {
            printf("Error: libpes_stream_pack failed with %d\n", ret);
            return -1;
        }
    }

    if (!g_print_info.printed[media_chn])
    {
        printf("PES: send stream to %s:%d through UDP\n", g_print_info.tar_ip, g_print_info.port[media_chn]);
        g_print_info.printed[media_chn] = 1;
    }

    return 0;
}

int dmc_pes_subscribe(int max_channel, char* ip, int port)
{
    int i;
    int ret;

    if (ip == NULL)
    {
        printf("Error: NULL ip address, please run \"vlcview -h\"\n");
        return -1;
    }

    if (max_channel > MAX_PES_CHANNEL_COUNT)
    {
        printf("Error: channel num is larger than %d\n", MAX_PES_CHANNEL_COUNT);
        return -1;
    }

    g_frame_length = 0;
    g_nalu_count   = 0;

#ifdef __FH_USING_SRT__
    ret = srt_libpes_init(port, max_channel);
#else
    ret = libpes_init();
#endif
    if (ret != 0)
    {
        return -1;
    }

    strncpy(g_print_info.tar_ip, ip, sizeof(g_print_info.tar_ip));

    for(i = 0; i < max_channel; i++)
    {
        libpes_send_to_vlc(i, ip, port + i);
        g_print_info.port[i] = port + i;
        g_print_info.printed[i] = 0;
    }

    dmc_subscribe("PES", DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265, _pes_input_fn);
    return 0;
}

int dmc_pes_unsubscribe(void)
{
    dmc_unsubscribe("PES", DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265);
    libpes_uninit();
    return 0;
}
