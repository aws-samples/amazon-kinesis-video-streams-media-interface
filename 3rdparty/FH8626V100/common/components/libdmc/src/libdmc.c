#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/libdmc.h"

#define _DMC_SUBSCRIBER_MAX (8)
#define _DMC_NAME_LEN_MAX   (64)

struct _dmc_subscribe_t {
    int occupied;
    char module_name[_DMC_NAME_LEN_MAX];

    int media_type;
    dmc_media_input_fn proc;
} g_dmc_subscribe_list[_DMC_SUBSCRIBER_MAX];


int dmc_init(void)
{
    memset(g_dmc_subscribe_list, 0, sizeof(g_dmc_subscribe_list));
    return 0;
}

int dmc_input(int media_chn,
        int media_type,
        int media_subtype,
        unsigned long long frame_pts,
        unsigned char *frame_data,
        int frame_len,
        int frame_end_flag)
{
    int i;
    struct _dmc_subscribe_t *subscribe;

    for(i=0; i<_DMC_SUBSCRIBER_MAX; i++)
    {
        subscribe = &g_dmc_subscribe_list[i];
        if (!subscribe->occupied)
        {
            continue;
        }

        if (subscribe->media_type & media_type)
        {
            subscribe->proc(media_chn, media_type, media_subtype, frame_pts, frame_data, frame_len, frame_end_flag);
        }
    }

    return 0;
}

int dmc_subscribe(char *module_name, int media_type, dmc_media_input_fn proc)
{
    int i;
    int len = 0;
    struct _dmc_subscribe_t *subscribe;
    struct _dmc_subscribe_t *sel = NULL;

    if (!module_name)
        return -1;

    len = strlen(module_name);
    if (len <= 0 || len >= _DMC_NAME_LEN_MAX || !media_type || !proc)
        return -1;

    for(i=0; i<_DMC_SUBSCRIBER_MAX; i++)
    {
        subscribe = &g_dmc_subscribe_list[i];
        if (!subscribe->occupied)
        {
            if (!sel)
                sel = subscribe;
        }
        else if (!strcmp(module_name, subscribe->module_name))
        {
            printf("DMC[Warning]: %s, re-subscribed!\n", module_name);
            sel = subscribe;
            sel->occupied = 0;
            break;
        }
    }

    if (!sel)
    {
        return -1;
    }

    strncpy(sel->module_name, module_name, _DMC_NAME_LEN_MAX);
    sel->media_type = media_type;
    sel->proc = proc;
    sel->occupied = 1;
    return 0;
}

int dmc_unsubscribe(char *module_name, int media_type)
{
    int i;
    struct _dmc_subscribe_t *subscribe;

    if (!module_name || !media_type)
        return 0;

    for(i=0; i<_DMC_SUBSCRIBER_MAX; i++)
    {
        subscribe = &g_dmc_subscribe_list[i];
        if (subscribe->occupied)
        {
            if (!strcmp(module_name, subscribe->module_name))
            {
                media_type = (subscribe->media_type & (~media_type));
                subscribe->media_type = media_type;
                if (!media_type)
                {
                    subscribe->occupied = 0;
                }
                else
                {
                    printf("DMC[Warning]: %s, partial un-subscribe!\n", module_name);
                }

                break;
            }
        }
    }

    return 0;
}

int dmc_deinit(void)
{
    memset(g_dmc_subscribe_list, 0, sizeof(g_dmc_subscribe_list));
    return 0;
}
