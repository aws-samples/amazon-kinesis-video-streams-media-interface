#ifndef _FIFO_H_
#define _FIFO_H_

#include "types/type_def.h"

#ifdef OS_LINUX
#include <pthread.h>
#include <string.h>
typedef pthread_mutex_t fh_lock_t;

#define fh_lock_init(x) pthread_mutex_init((fh_lock_t *)&x, NULL)
#define fh_lock(x) pthread_mutex_lock((fh_lock_t *)&x)
#define fh_unlock(x) pthread_mutex_unlock((fh_lock_t *)&x)
#define fh_destoryLock(x) pthread_mutex_destroy((fh_lock_t *)&x)
#else
#include <rtdevice.h>
typedef struct rt_semaphore fh_lock_t;
#define fh_lock_init(x) rt_sem_init((fh_lock_t *)&x, "isp_fifo_lock", 1, RT_IPC_FLAG_FIFO)
#define fh_lock(x) rt_sem_take((fh_lock_t *)&x, RT_WAITING_FOREVER)
#define fh_unlock(x) rt_sem_release((fh_lock_t *)&x)
#define fh_destoryLock(x) rt_sem_detach((fh_lock_t *)&x)
#endif

//fix me! Dynamic FIFO size need Heap support
#define FIFO_DEPTH 1024 // should be power of 2

typedef volatile struct _FifoDesc
{
	FH_UINT8 fifo[FIFO_DEPTH];
	FH_UINT16 rp; // read pointer
	FH_UINT16 wp; // write pointer
	FH_UINT8 empty;
	FH_UINT8 full;
	FH_UINT8 overflow;
	fh_lock_t lock;
} FifoDesc;

int Fifo_Init(FifoDesc *desc);
int Fifo_Clear(FifoDesc *desc);
int Fifo_Read(FifoDesc *desc, FH_UINT8 *buf, int size);
int Fifo_Write(FifoDesc *desc, FH_UINT8 *buf, int size);
int Fifo_DataLeft(FifoDesc *desc);
int Fifo_Inspect(FifoDesc *desc, FH_UINT8 *buf, FH_UINT32 size);
int Fifo_Release(FifoDesc *desc);

#endif
