/*
 * Copyright 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#pragma once

#include <errno.h>
#include <stdio.h>
#include <pthread.h>

#define ATOMIC_INT        volatile size_t
#define ATOMIC_INT_ADD(x) __sync_add_and_fetch((x), 1)
#define ATOMIC_INT_SUB(x) __sync_sub_and_fetch((x), 1)

#define ATOMIC_BOOL          volatile bool
#define ATOMIC_BOOL_TRUE(x)  __sync_or_and_fetch((x), TRUE)
#define ATOMIC_BOOL_FALSE(x) __sync_and_and_fetch((x), FALSE)

#define MUTEX            pthread_mutex_t
#define MUTEX_CREATE(x)  pthread_mutex_init((x), NULL)
#define MUTEX_LOCK(x)    pthread_mutex_lock((x))
#define MUTEX_UNLOCK(x)  pthread_mutex_unlock((x))
#define MUTEX_DESTORY(x) pthread_mutex_destroy((x))

#define THREAD                                  pthread_t
#define THREAD_CREATE(thread, threadFunc, args) pthread_create((thread), NULL, (threadFunc), (args))
#define THREAD_JOIN(thread, threadReturn)       pthread_join((thread), (threadReturn))

#define LOG(msg, ...) printf(msg "\n", ##__VA_ARGS__)

#define T41_HANDLE_NULL_CHECK(x)                                                                                                                     \
    if (!(x)) {                                                                                                                                      \
        return -EINVAL;                                                                                                                              \
    }
#define T41_HANDLE_STATUS_CHECK(T41Handle, expectedStatus)                                                                                           \
    if ((T41Handle)->status != (expectedStatus)) {                                                                                                   \
        return -EAGAIN;                                                                                                                              \
    }

#define T41_POLLING_STREAM_TIMEOUT_MS 1000
