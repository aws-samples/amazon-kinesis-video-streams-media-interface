/*
 *------------------------------------------------------------------------------
 * @File      :    ei_mem.h
 * @Date      :    2021-5-25
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>

#ifndef EI_MEM_H
#define EI_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

void *ei_malloc(size_t size);

void *ei_calloc(int n, size_t size);

void *ei_realloc(void *p, int n);

void ei_free(void *mem);

#ifdef __cplusplus
}
#endif

#endif

