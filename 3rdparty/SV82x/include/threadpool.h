/*
 *------------------------------------------------------------------------------
 * @File      :    threadpool.h
 * @Date      :    2021-7-5
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Source file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX_THREADS 64
#define MAX_QUEUE 65536

typedef struct threadpool_t threadpool_t;

typedef enum {
    threadpool_invalid        = -1,
    threadpool_lock_failure   = -2,
    threadpool_queue_full     = -3,
    threadpool_shutdown       = -4,
    threadpool_thread_failure = -5
} threadpool_error_t;

typedef enum {
    threadpool_graceful       = 1
} threadpool_destroy_flags_t;

threadpool_t *threadpool_create(int thread_count, int queue_size, int flags);

int threadpool_add(threadpool_t *pool, void (*routine)(void *),
                   void *arg, int flags);

int threadpool_destroy(threadpool_t *pool, int flags);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus */


#endif /* _THREADPOOL_H_ */

