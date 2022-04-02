#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "types/type_def.h"

#define HTTP_MJPEG_CHANNEL_MAX (1)
#define HTTP_TEMP_BUFF_SIZE (16*1024)
#define MJPEG_BOUNDARY "boundarydonotcross"

struct mjpeg_server_context
{
    int                 lock_inited;
    pthread_mutex_t lock;

    volatile int is_running; /*is running?*/
    volatile int request_to_stop; /*request to stop*/

    FH_UINT16 port;          /*the network service port*/
    FH_UINT16 reserved;
    int server_fd; /*the service fd*/
    int client_fd;  /*the connected client fd*/

    unsigned char    temp_buff[HTTP_TEMP_BUFF_SIZE];
    pthread_t thread_id;
};

static struct mjpeg_server_context g_server_ctx[HTTP_MJPEG_CHANNEL_MAX];

static void _MJPEG_LOCK(struct mjpeg_server_context *server)
{
    if (!server->lock_inited)
    {
        server->lock_inited = 1;
        pthread_mutex_init(&server->lock, NULL);
    }
    pthread_mutex_lock(&server->lock);
}

#define _MJPEG_UNLOCK(server) pthread_mutex_unlock(&server->lock)

static void *copy_noncached_stream(char *dst, const char *src, int count)
{
    char *dst_bak = dst;

    while (((unsigned int)src & 3) && count > 0)
    {
        *(dst++) = *(src++);
        count--;
    }

    while (count >= 4)
    {
        *((int *)dst) =  *((int *)src);
        dst += 4;
        src += 4;
        count -= 4;
    }

    while (count > 0)
    {
        *(dst++) = *(src++);
        count--;
    }

    return dst_bak;
}

static int mysend(struct mjpeg_server_context *server, char *buf, int len)
{
    int ret;
    int sentlen = 0;

    while (!server->request_to_stop && len > 0)
    {
        ret = send(server->client_fd, buf, len, 0);
        if (ret >= 0)
        {
            buf += ret;
            len -= ret;
            sentlen += ret;
        }
        else
        {
            if (errno != EAGAIN)
            {
                printf("Error: http_mjpeg, network send fail, errno=%d, close the connection!\n", errno);
                close(server->client_fd);
                server->client_fd = -1;
                break;
            }
        }
    }

    return sentlen;
}

FH_VOID libmjpeg_send_stream(FH_VOID *data, FH_UINT32 size)
{
    int len;
    struct mjpeg_server_context *server = &g_server_ctx[0];

    if ( !data || (FH_UINT32)size > 8*1024*1024 || !size)
    {
        printf("Error: libmjpeg_send_stream, invalid parameter!\n");
        return;
    }

    _MJPEG_LOCK(server);
    if (server->client_fd < 0)
        goto Exit;

    len = snprintf((char *)server->temp_buff, HTTP_TEMP_BUFF_SIZE,
            "Content-Type: image/jpeg\r\n"
            "Content-Length: %d\r\n"
            "\r\n",
            size);
    if (mysend(server, (char *)server->temp_buff, len) != len || server->request_to_stop)
        goto Exit;

    while (size > 0)
    {
        len = HTTP_TEMP_BUFF_SIZE > size ? size : HTTP_TEMP_BUFF_SIZE;
        copy_noncached_stream((char *)server->temp_buff, data, len);
        if (mysend(server, (char*)server->temp_buff, len) != len || server->request_to_stop)
            goto Exit;

        data += len;
        size -= len;
    }

    len = snprintf((char *)server->temp_buff, HTTP_TEMP_BUFF_SIZE, "\r\n--" MJPEG_BOUNDARY "\r\n");
    mysend(server, (char *)server->temp_buff, len);

Exit:
    _MJPEG_UNLOCK(server);
}

static int send_first_response(struct mjpeg_server_context *server, int fd)
{
    int len;

    len = snprintf((char *)server->temp_buff, HTTP_TEMP_BUFF_SIZE,
            "HTTP/1.0 200 OK\r\n"
            "Connection: close\r\n"
            "Server: MJPG-Streamer/0.2\r\n"
            "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n"
            "Pragma: no-cache\r\n"
            "Expires: Mon, 3 Jan 2000 12:34:56 GMT\r\n"
            "Content-Type: multipart/x-mixed-replace;boundary=" MJPEG_BOUNDARY
            "\r\n"
            "\r\n"
            "--" MJPEG_BOUNDARY "\r\n");
    if (send(fd, server->temp_buff, len, 0) != len)
    {
        printf("Error: http_mjpeg, send_first_response failed!\n");
        return -1;
    }

    return 0;
}

static int create_server_socket(FH_UINT16 port)
{
    int on = 1;
    int fd;
    struct sockaddr_in addr;
    struct timeval tv;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        printf("Error: http_mjpeg, create socket failed!\n");
        return fd;
    }

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    tv.tv_sec  = 0;
    tv.tv_usec = 50*1000;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(port);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        printf("Error: http_mjpeg, bind failed!\n");
        goto Exit;
    }

    if (listen(fd, 5) != 0)
    {
        printf("Error: http_mjpeg, listen failed!\n");
        goto Exit;
    }

    return fd;

Exit:
    close(fd);
    return -1;
}

static FH_VOID *mjpeg_server_thread(FH_VOID *arg)
{
    struct mjpeg_server_context *server = (struct mjpeg_server_context *)arg;
    struct sockaddr_in addr;
    socklen_t sock_len;
    int ret;
    int fd;
    int maxfd;
    struct timeval tv;
    fd_set rfds;

    fd = create_server_socket(server->port);
    if (fd < 0)
    {
        server->is_running = 0;
        return NULL;
    }

    _MJPEG_LOCK(server);
    server->server_fd = fd;
    while (!server->request_to_stop)
    {
        tv.tv_sec  = 0;
        tv.tv_usec = 50*1000;
        FD_ZERO(&rfds);
        FD_SET(server->server_fd, &rfds);
        if (server->client_fd >= 0)
        {
            FD_SET(server->client_fd, &rfds);
        }
        maxfd = server->server_fd > server->client_fd ? server->server_fd : server->client_fd;

        _MJPEG_UNLOCK(server);
        ret = select(maxfd + 1, &rfds, NULL, NULL, &tv);
        _MJPEG_LOCK(server);
        if (ret <= 0)
            continue;

        if (FD_ISSET(server->server_fd, &rfds))
        {
            sock_len = sizeof(addr);
            fd = accept(server->server_fd, (struct sockaddr *)&addr, &sock_len);
            if (fd >= 0)
            {
                tv.tv_sec  = 0;
                tv.tv_usec = 50*1000;
                setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
                setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

                if (send_first_response(server, fd) < 0)
                {
                    close(fd);
                    fd = -1;
                }
                else
                {
                    if (server->client_fd >= 0)
                    {
                        close(server->client_fd);
                        server->client_fd = -1;
                        printf("Msg: http_mjpeg, new client connected, and close the old connection!\n");
                    }
                    else
                    {
                        printf("Msg: http_mjpeg, new client connected!\n");
                    }
                    server->client_fd = fd;
                    continue;
                }
            }
        }

        if (server->client_fd >= 0 && FD_ISSET(server->client_fd, &rfds))
        {
            ret = read(server->client_fd, server->temp_buff, sizeof(server->temp_buff));
            if (ret <= 0) /*error or peer close the socket*/
            {
                printf("Msg: http_mjpeg, peer close connection.\n");
                close(server->client_fd);
                server->client_fd = -1;
            }
        }
    }

    if (server->client_fd >= 0)
    {
        close(server->client_fd);
        server->client_fd = -1;
    }

    if (server->server_fd >= 0)
    {
        close(server->server_fd);
        server->server_fd = -1;
    }

    server->is_running = 0;
    _MJPEG_UNLOCK(server);

    return NULL;
}

FH_SINT32 libmjpeg_start_server(FH_UINT16 port)
{
    int ret;
    pthread_attr_t pth_attr;
    struct mjpeg_server_context *server = &g_server_ctx[0];

    _MJPEG_LOCK(server);

    if (server->is_running)
    {
        printf("Error: http_mjpeg, server is already running!\n");
        goto Error;
    }

    server->is_running = 1;
    server->request_to_stop = 0;
    server->port = port;
    server->server_fd = -1;
    server->client_fd = -1;

    pthread_attr_init(&pth_attr);
    pthread_attr_setdetachstate(&pth_attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&server->thread_id, &pth_attr, mjpeg_server_thread, server);
    pthread_attr_destroy(&pth_attr);
    if (ret)
    {
        printf("Error: http_mjpeg, create thread failed!\n");
        server->is_running = 0;
        goto Error;
    }

    _MJPEG_UNLOCK(server);
    return 0;

Error:
    _MJPEG_UNLOCK(server);
    return -1;
}

FH_VOID libmjpeg_stop_server(FH_VOID)
{
    struct mjpeg_server_context *server = &g_server_ctx[0];

    _MJPEG_LOCK(server);
    while (server->is_running)
    {
        server->request_to_stop = 1; /*let the server thread to exit*/
        _MJPEG_UNLOCK(server);
        usleep(20000);
        _MJPEG_LOCK(server);
    }
    _MJPEG_UNLOCK(server);
}
