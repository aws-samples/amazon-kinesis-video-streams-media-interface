#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "dbi_over_tcp.h"
#ifdef __LINUX_OS__
#include "isp_ext/debug_interface.h"
#else
#include "di/debug_interface.h"
#endif

struct dbi_over_tcp
{
    struct Debug_Interface *di;

    unsigned char buf[16]; /*16 is from function DI_handle, workaround DI_handle problem...*/
    int                  buf_valid_len;
    int error;
    int client_fd;
    int server_fd;
};

static struct dbi_over_tcp g_dbi_over_tcp;

static int tcp_send(void *obj, unsigned char *buf, int size)
{
    int ret;
    int len;
    int sentlen = 0;
    struct dbi_over_tcp *pdbi = (struct dbi_over_tcp *)obj;

    if (pdbi->error)
    {
        printf("Error: dbi, tcp_send with previous error!\n");
        return 0;
    }

    while (size > 0)
    {
        len = size > 1460 ? 1460 : size;
        ret = send(pdbi->client_fd, buf, len, 0);
        if (ret < 0)
        {
            printf("Error:dbi, send failed!\n");
            pdbi->error = 1;
            break;
        }
        buf += ret;
        size -= ret;
        sentlen += ret;
    }

    return sentlen;
}

static int tcp_recv(void *obj, unsigned char *buf, int size)
{
    int ret;
    struct dbi_over_tcp *pdbi = (struct dbi_over_tcp *)obj;

    if (pdbi->error)
    {
        printf("Error: dbi, tcp_recv with previous error!\n");
        return -1;
    }

    if (pdbi->buf_valid_len > 0)
    {
        if (size < pdbi->buf_valid_len)
        {
            printf("Error: dbi, DI_handle with small buf!\n");
            pdbi->error = 1;
            return -1;
        }

        ret = pdbi->buf_valid_len;
        pdbi->buf_valid_len = 0;
        memcpy(buf, pdbi->buf, ret);
        return ret;
    }

    ret = recv(pdbi->client_fd, buf, size, 0);
    if (ret == 0)
    {
        printf("Msg: dbi, peer close connection @tcp_recv!\n");
        pdbi->error = 1;
    }
    else if (ret < 0)
    {
        printf("Msg: dbi, recv failed!\n");
        pdbi->error = 1;
    }

    return ret;
}

static int tcp_dbi_init(struct dbi_over_tcp *pdbi, int port)
{
    int on = 1;
    int fd;
    struct sockaddr_in local_addr;
    struct timeval timeout;
    struct DI_config di_cfg;
    struct Debug_Interface *di;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Error: libdbi_over_tcp, create socket failed!\n");
        goto Exit;
    }

    timeout.tv_sec   = 0;
    timeout.tv_usec = 100*1000; /*100ms timeout*/
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    local_addr.sin_family      = AF_INET;
    local_addr.sin_port        = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) != 0)
    {
        printf("Error: libdbi_over_tcp, bind failed!\n");
        goto Exit;
    }

    if (listen(fd, 5) != 0)
    {
        printf("Error: libdbi_over_tcp, listen failed!\n");
        goto Exit;
    }

    di_cfg.obj  = pdbi;
    di_cfg.send = tcp_send;
    di_cfg.recv = tcp_recv;
    di = DI_create(&di_cfg);
    if (!di)
    {
        printf("Error: DI_create failed!\n");
        goto Exit;
    }

    pdbi->di = di;
    pdbi->client_fd = -1;
    pdbi->server_fd = fd;
    pdbi->buf_valid_len = 0;
    pdbi->error = 0;
    return 0;

Exit:
    if (fd >= 0)
    {
        close(fd);
    }
    return -1;
}

int *libtcp_dbi_thread(struct dbi_tcp_config *conf)
{
    int ret;
    struct timeval tv;
    int fd;
    int maxfd;
    fd_set rfds;
    struct sockaddr_in addr;
    socklen_t addr_len;
    struct dbi_over_tcp *pdbi = &g_dbi_over_tcp;

    if (!conf || !conf->cancel)
    {
        printf("Error: libdbi_over_tcp,invalid parameter!\n");
        return NULL;
    }

    if (pdbi->di)
    {
        printf("Error: libdbi_over_tcp already running!\n");
        return NULL;
    }

    ret = tcp_dbi_init(pdbi, conf->port);
    if (ret)
    {
        /*if failed to create socket, we should wait for stop command*/
        while (!(*(conf->cancel)))
        {
            usleep(100*1000);
        }

        *(conf->cancel) = 0;
        return NULL;
    }

    while (!(*(conf->cancel)))
    {
        if (pdbi->error)
        {
            if (pdbi->client_fd >= 0)
            {
                close(pdbi->client_fd);
                pdbi->client_fd = -1;
            }
            pdbi->error = 0;
        }

        FD_ZERO(&rfds);
        FD_SET(pdbi->server_fd, &rfds);
        if (pdbi->client_fd >= 0)
        {
            FD_SET(pdbi->client_fd, &rfds);
        }

        tv.tv_sec = 0;
        tv.tv_usec = 100*1000;
        maxfd = pdbi->server_fd > pdbi->client_fd ? pdbi->server_fd : pdbi->client_fd;
        ret = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        if (ret <= 0)
            continue;

        if (FD_ISSET(pdbi->server_fd, &rfds))
        {
            addr_len = sizeof(addr);
            fd = accept(pdbi->server_fd, (struct sockaddr *)&addr, &addr_len);
            if (fd >= 0)
            {
                tv.tv_sec  = 0;
                tv.tv_usec = 500*1000; /*500ms timeout*/
                setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
                setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
                if (pdbi->client_fd >= 0)
                {
                    close(pdbi->client_fd);
                    pdbi->client_fd = -1;
                    printf("Msg: dbi, new client connected, and close the old connection!\n");
                }
                else
                {
                    printf("Msg: dbi, new client connected!\n");
                }
                pdbi->client_fd = fd;
                continue;
            }
        }

        if (pdbi->client_fd >= 0 && FD_ISSET(pdbi->client_fd, &rfds))
        {
            ret = recv(pdbi->client_fd, pdbi->buf, sizeof(pdbi->buf), 0);
            if (ret <= 0) /*error or peer close the socket*/
            {
                printf("Msg: dbi, peer close connection @recv.\n");
                pdbi->error = 1;
                continue;
            }

            pdbi->buf_valid_len = ret;
            ret = DI_handle(pdbi->di);
            pdbi->buf_valid_len = 0;
            if (ret < 0)
            {
                printf("Msg: dbi, DI_handle failed,connection is closed!\n");
                pdbi->error = 1;
            }
        }
    }

    if (pdbi->server_fd >= 0)
    {
        close(pdbi->server_fd);
        pdbi->server_fd = -1;
    }

    if (pdbi->client_fd >= 0)
    {
        close(pdbi->client_fd);
        pdbi->client_fd = -1;
    }

    DI_destroy(pdbi->di);
    pdbi->di = NULL;

    *(conf->cancel) = 0;

    return NULL;
}
