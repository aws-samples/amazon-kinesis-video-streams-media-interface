#ifndef __FH_USING_SRT__


#include <sys/prctl.h>


static void *network_send_proc(void *arg)
{
    int i;
    int ret;
    int count;
    int fd = (int)arg;
    struct ts_units *pkt;

    prctl(PR_SET_NAME, "send_proc");

    _VLC_LOCK();
    while (!g_stop_request)
    {
        count = 0;
        for (i=0; !g_stop_request && i<VLC_CHANNEL_MAX; )
        {
            pkt = ts_get(&g_vlc_channel_info[i]);
            if (pkt)
            {
                _VLC_UNLOCK();
                ret = sendto(fd, pkt->buf,  pkt->len, 0, (struct sockaddr *)&g_vlc_channel_info[i].target_addr, sizeof(struct sockaddr_in));
                if (ret < 0)
                {
                    printf("ERROR: libpes send(%d) failed,%d\n", pkt->len, ret);
                }
                _VLC_LOCK();
                free_ts_pkt(pkt);
                count++;
            }
            else
            {
                i++;
            }
        }

        if (!count)
        {
            _VLC_UNLOCK();
            usleep(10000);
            _VLC_LOCK();
        }
    }

    _VLC_UNLOCK();

    close(fd);
    g_stop_request = 0;
    return NULL;
}

int libpes_init(void)
{
    pthread_attr_t attr;
    pthread_t thread;
    int fd;
    int i;

    _VLC_LOCK();
    if (!g_this_module_inited)
    {
        fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (fd < 0)
        {
            printf("Error: libpes failed with socket!\n");
            _VLC_UNLOCK();
            return -1;
        }
        g_stop_request = 0;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_attr_setstacksize(&attr, 3 * 1024);

#ifdef __RTTHREAD_OS__
        struct sched_param param;
        param.sched_priority = FH_PES_THREAD_PRORITY;
        pthread_attr_setschedparam(&attr, &param);
#endif
        if (pthread_create(&thread, &attr, network_send_proc, (void*)fd) != 0)
        {
            printf("Error: Create libpes thread failed!\n");
            _VLC_UNLOCK();
            return -1;
        }

        INIT_LIST_HEAD(&g_free_ts_list);
        INIT_LIST_HEAD(&g_swap_ts_list);
        for (i=0; i<VLC_CHANNEL_MAX; i++)
        {
            channel_info_reset(&g_vlc_channel_info[i]);
        }
        g_this_module_inited = 1;
    }
    _VLC_UNLOCK();

    return 0;
}
#endif
