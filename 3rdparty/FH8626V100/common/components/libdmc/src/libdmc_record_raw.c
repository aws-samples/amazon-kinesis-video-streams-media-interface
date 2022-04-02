#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libdmc/include/libdmc.h"
#include "libdmc/include/libdmc_record_raw.h"

#define MAX_FILE_NAME  50

struct _record_file_info
{
    unsigned int printed;
    FILE *fp;
    char file_name[MAX_FILE_NAME];
};

static struct _record_file_info *g_record_files = NULL;
static unsigned int g_max_num = 0;

static int _record_input_fn(int media_chn,
        int media_type,
        int media_subtype,
        unsigned long long frame_pts,
        unsigned char *frame_data,
        int frame_len,
        int frame_end_flag)
{
    unsigned int ret;

    if (media_chn > g_max_num)
    {
        printf("Error: libpes, max channel num is %d!\n", media_chn);
        return 0;
    }

    if (g_record_files[media_chn].fp == NULL)
    {
        if (media_type == DMC_MEDIA_TYPE_H264)
        {
            snprintf(g_record_files[media_chn].file_name, MAX_FILE_NAME, "chan_%d.h264", media_chn);
        }
        else if (media_type == DMC_MEDIA_TYPE_H265)
        {
            snprintf(g_record_files[media_chn].file_name, MAX_FILE_NAME, "chan_%d.h265", media_chn);
        }
        else if (media_type == DMC_MEDIA_TYPE_AUDIO)
        {
            snprintf(g_record_files[media_chn].file_name, MAX_FILE_NAME, "chan_%d.data", media_chn);
        }
        else if (media_type == DMC_MEDIA_TYPE_MJPEG)
        {
            snprintf(g_record_files[media_chn].file_name, MAX_FILE_NAME, "chan_%d.mjpeg", media_chn);
        }

        g_record_files[media_chn].fp = fopen(g_record_files[media_chn].file_name, "wb+");
        if (!g_record_files[media_chn].fp)
        {
            printf("Error: open file %s failed\n", g_record_files[media_chn].file_name);
            return -1;
        }
    }

    ret = fwrite(frame_data, 1, frame_len, g_record_files[media_chn].fp);
    if (ret < frame_len)
    {
        printf("Error: write file %s fail, <%d> <%d>\n", g_record_files[media_chn].file_name, ret, frame_len);
        return -1;
    }

    if (!g_record_files[media_chn].printed)
    {
        printf("Save stream in %s\n", g_record_files[media_chn].file_name);
        g_record_files[media_chn].printed = 1;
    }

    return 0;
}

int dmc_record_subscribe(int max_channel)
{
    if (g_record_files)
    {
        dmc_record_unsubscribe();
    }

    g_record_files = malloc(sizeof(struct _record_file_info) * max_channel);
    if (!g_record_files)
    {
        printf("Error: insufficient space for stream record\n");
        return -1;
    }

    memset(g_record_files, 0, sizeof(struct _record_file_info) * max_channel);

    g_max_num = max_channel;

    dmc_subscribe("RECORD", DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO | DMC_MEDIA_TYPE_MJPEG, _record_input_fn);

    return 0;
}

int dmc_record_unsubscribe(void)
{
    int i;

    dmc_unsubscribe("RECORD",  DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO | DMC_MEDIA_TYPE_MJPEG);

    for (i = 0; i < g_max_num; i++)
    {
        if (g_record_files[i].fp)
        {
            fclose(g_record_files[i].fp);
            g_record_files[i].fp = NULL;
        }
    }

    if (g_record_files)
    {
        free(g_record_files);
        g_record_files = NULL;
    }

    return 0;
}
