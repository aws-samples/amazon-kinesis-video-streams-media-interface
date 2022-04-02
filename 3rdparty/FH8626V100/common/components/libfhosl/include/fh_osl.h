#if 0
#ifndef __fh_osl_h__
#define __fh_osl_h__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#ifdef __RTTHREAD_OS__
#include <rtthread.h>
#include "type_def.h"
#include "bufCtrl.h"

#ifdef RT_USING_SAL
#include <sys/socket.h>
#include <sys/select.h>
#else
#include <lwip/sockets.h>
#include <lwip/netif.h>

// #if !LWIP_POSIX_SOCKETS_IO_NAMES
// #define close lwip_close
// extern int fcntl(int s, int cmd, int val);
// #endif
#endif

#ifndef FH_APP_THREAD_PRIORITY
#define FH_APP_THREAD_PRIORITY (90)
#endif

#define fh_osl_print    rt_kprintf
#define fh_osl_sprintf  rt_sprintf
#define fh_osl_snprintf rt_snprintf
#define fh_osl_strcmp   rt_strcmp
#define fh_osl_strcpy   rt_strcpy
#define fh_osl_strncpy   rt_strncpy
#define fh_osl_strcat   rt_strcat
#define fh_osl_strlen   rt_strlen

typedef rt_thread_t     fh_osl_thread_t;
typedef struct rt_mutex fh_osl_lock_t;

extern void do_gettimeofday(struct timeval *tv);

#elif __LINUX_OS__
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>
#include "types/bufCtrl.h"
#include "types/type_def.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

#define fh_osl_print    printf
#define fh_osl_sprintf  sprintf
#define fh_osl_snprintf snprintf
#define fh_osl_strcmp   strcmp
#define fh_osl_strcpy   strcpy
#define fh_osl_strncpy   strncpy
#define fh_osl_strcat   strcat
#define fh_osl_strlen   strlen

#define FH_APP_THREAD_PRIORITY (90)
typedef pthread_t       fh_osl_thread_t;
typedef pthread_mutex_t fh_osl_lock_t;

#endif

#define ALIGNTO(addr, edge)  ((addr + edge - 1) & ~(edge - 1)) /* 数据结构对齐定义 */

typedef struct
{
    unsigned int  stack_size;
    unsigned char priority;
} fh_osl_thread_attr_t;

typedef void *(*thread_proc_t)(void *);


void *fh_osl_malloc(unsigned int size);
void fh_osl_free(void *ptr);

void *fh_osl_memset(void *s, int c, unsigned int n);
void *fh_osl_memcpy(void *dst, void *src, unsigned int n);

unsigned int fh_osl_sleep(unsigned int seconds);
int fh_osl_usleep(unsigned int micro_seconds);

int fh_osl_create_thread(fh_osl_thread_t *thread, fh_osl_thread_attr_t *attr, thread_proc_t proc, void *arg, char* name);
int fh_osl_destroy_thread(fh_osl_thread_t thread);

void fh_osl_gettimeofday(struct timeval *tv);

void fh_osl_init_lock(fh_osl_lock_t *lock);
void fh_osl_take_lock(fh_osl_lock_t *lock);
void fh_osl_release_lock(fh_osl_lock_t *lock);
void fh_osl_destroy_lock(fh_osl_lock_t *lock);

int vpu_write_proc(char *s);
int enc_write_proc(char *s);
int jpeg_write_proc(char *s);
int isp_write_proc(char *s);


#endif /*__fh_osl_h__*/
#endif