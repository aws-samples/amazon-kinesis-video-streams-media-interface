/*
 *------------------------------------------------------------------------------
 * @File      :    ei_posix.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_POSIX_H__
#define __EI_POSIX_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>
#include "ei_mem.h"

#ifdef __EOS__
#include <rtthread.h>
#include <dfs_posix.h>
#include <sched.h>
#include <stdio.h>
#include <clock_time.h>
#include <sys/time.h>
#include <prctl.h>
#else
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/select.h>
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_POSIX_H__ */

