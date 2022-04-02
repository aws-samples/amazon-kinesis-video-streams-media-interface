#ifdef __FH_USING_SRT__

#include <srt.h>

struct srt_info
{
    int index;
    int port;
};

static int g_fd_list[VLC_CHANNEL_MAX];
static struct srt_info g_srt_info[VLC_CHANNEL_MAX];

static void *create_sock_proc(void *arg)
{
    struct sockaddr_in sa;
    int ss, st;
    int yes = 1;
    struct sockaddr_storage their_addr;
    struct srt_info *info;
    int epoll_id;
    int epoll_mode;
    int timeout_ms;
    int epoll_res;

    info = (struct srt_info *)arg;

    srt_startup();

    ss = srt_create_socket();
    if (ss == SRT_ERROR)
    {
        fprintf(stderr, "srt_socket: %s\n", srt_getlasterror_str());
        return NULL;
    }

    sa.sin_family = AF_INET;
    sa.sin_port = htons(info->port);
    sa.sin_addr.s_addr = INADDR_ANY;

    srt_setsockflag(ss, SRTO_RCVSYN, &yes, sizeof yes);

    epoll_id = srt_epoll_create();
    epoll_mode = SRT_EPOLL_IN | SRT_EPOLL_ERR;

    st = srt_bind(ss, (struct sockaddr*)&sa, sizeof sa);
    if (st == SRT_ERROR)
    {
        fprintf(stderr, "srt_bind: %s\n", srt_getlasterror_str());
        return NULL;
    }

    st = srt_listen(ss, 5);
    if (st == SRT_ERROR)
    {
        fprintf(stderr, "srt_listen: %s\n", srt_getlasterror_str());
        return NULL;
    }

    const int default_len = 3;
    int rlen = default_len;
    SRTSOCKET read[default_len];

    while (!g_stop_request)
    {
        srt_epoll_add_usock(epoll_id, ss, &epoll_mode);
        timeout_ms = 5000;
        epoll_res = srt_epoll_wait(epoll_id, read, &rlen, NULL, NULL, timeout_ms, 0, 0, 0, 0);
        if (epoll_res == SRT_ERROR)
        {
            continue;
        }
        int addr_size = sizeof their_addr;
        int their_fd = srt_accept(ss, (struct sockaddr *)&their_addr, &addr_size);
        if (their_fd == SRT_ERROR)
        {
            fprintf(stderr, "srt_accept: %s\n", srt_getlasterror_str());
            break;
        }

        srt_setsockflag(their_fd, SRTO_SENDER, &yes, sizeof yes);

        if (g_fd_list[info->index] > 0)
        {
            srt_close(g_fd_list[info->index]);
        }

        g_fd_list[info->index] = their_fd;
    }


    srt_epoll_release(epoll_id);
    st = srt_close(ss);
    if (st == SRT_ERROR)
    {
        fprintf(stderr, "srt_close: %s\n", srt_getlasterror_str());
        return NULL;
    }

    printf("srt cleanup\n");
    srt_cleanup();
}


static void *srt_network_send_proc(void *arg)
{
    int i;
    int ret;
    int count;
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
                if (g_fd_list[i] > 0)
                {
                    ret = srt_sendmsg2(g_fd_list[i], pkt->buf, pkt->len, NULL);
                    if (ret == SRT_ERROR)
                    {
                        printf("ERROR: libpes send(%d) failed,%d\n", pkt->len, ret);
                        srt_close(g_fd_list[i]);
                        g_fd_list[i] = 0;
                    }
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

    for (i = 0; i < VLC_CHANNEL_MAX; i++)
    {
        if (g_fd_list[i] > 0)
        {
            srt_close(g_fd_list[i]);
            g_fd_list[i] = 0;
        }
    }
    g_stop_request = 0;
    return NULL;
}

int srt_libpes_init(int port, int max_channel)
{
    pthread_attr_t attr;
    pthread_t send_thread;
    pthread_t sock_thread[max_channel];
    int i;

    _VLC_LOCK();

    if (!g_this_module_inited)
    {
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_attr_setstacksize(&attr, 8 * 1024);

        for (i = 0; i < max_channel; i++)
        {
            g_srt_info[i].index = i;
            g_srt_info[i].port = port + i;
            if (pthread_create(&sock_thread[i], &attr, create_sock_proc, (void*)&g_srt_info[i]) != 0)
            {
                printf("Error: Create libpes thread failed!\n");
                _VLC_UNLOCK();
                return -1;
            }
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

        if (pthread_create(&send_thread, &attr, srt_network_send_proc, NULL) != 0)
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