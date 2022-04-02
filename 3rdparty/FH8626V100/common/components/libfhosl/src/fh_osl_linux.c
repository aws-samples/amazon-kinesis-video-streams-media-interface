#if 0
#ifdef __LINUX_OS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/fh_osl.h"

void *fh_osl_malloc(unsigned int size)
{
    return malloc(size);
}

void *fh_osl_memset(void *s, int c, unsigned int n)
{
    return memset(s, c, n);
}

void *fh_osl_memcpy(void *dst, void *src, unsigned int n)
{
    return memcpy(dst, src, n);
}

void fh_osl_free(void *ptr)
{
    free(ptr);
}

unsigned int fh_osl_sleep(unsigned int seconds)
{
    return sleep(seconds);
}

int fh_osl_usleep(unsigned int micro_seconds)
{
    return usleep(micro_seconds);
}

int fh_osl_create_thread(fh_osl_thread_t *thread, fh_osl_thread_attr_t *attr, thread_proc_t proc, void *arg, char* name)
{
	int ret;
    pthread_attr_t pth_attr;

    if (!thread || !proc)
    	return -1;

    pthread_attr_init(&pth_attr);
    pthread_attr_setdetachstate(&pth_attr, PTHREAD_CREATE_DETACHED);
    if (attr != NULL)
    {
        pthread_attr_setstacksize(&pth_attr, attr->stack_size);
    }
    ret = pthread_create(thread, &pth_attr, proc, arg);
    pthread_attr_destroy(&pth_attr);
    if (ret != 0)
    	ret = -1;

	return ret;
}

int fh_osl_destroy_thread(fh_osl_thread_t thread)
{
    return pthread_cancel(thread);
}

void fh_osl_gettimeofday(struct timeval *tv)
{
    gettimeofday(tv, NULL);
}

void fh_osl_init_lock(fh_osl_lock_t *lock)
{
    pthread_mutex_init(lock, NULL);
}

void fh_osl_take_lock(fh_osl_lock_t *lock)
{
    pthread_mutex_lock(lock);
}

void fh_osl_release_lock(fh_osl_lock_t *lock)
{
    pthread_mutex_unlock(lock);
}

void fh_osl_destroy_lock(fh_osl_lock_t *lock)
{
    pthread_mutex_destroy(lock);
}

static int write_proc(char *path, char *s)
{
    int fd;

    fd = open(path, O_RDWR);
    if(fd < 0)
    {
        printf("Error: open %s!\n", path);
        return -1;
    }

    write(fd, s, strlen(s));
    close(fd);
    return 0;
}

int vpu_write_proc(char *s)
{
    return write_proc("/proc/driver/vpu", s);
}

int enc_write_proc(char *s)
{
    return write_proc("proc/driver/enc", s);
}

int jpeg_write_proc(char *s)
{
    return write_proc("/proc/driver/jpeg", s);
}

int isp_write_proc(char *s)
{
    return write_proc("/proc/driver/isp", s);
}

#endif /*__LINUX_OS__*/
#endif