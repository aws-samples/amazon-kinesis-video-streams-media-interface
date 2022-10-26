/*
 *------------------------------------------------------------------------------
 * @File      :    ei_type.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_TYPE_H__
#define __EI_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



typedef unsigned char           EI_U8;
typedef unsigned short          EI_U16;
typedef unsigned int            EI_U32;

typedef signed char             EI_S8;
typedef short                   EI_S16;
typedef int                     EI_S32;

typedef unsigned long           EI_UL;
typedef signed long             EI_SL;

typedef float                   EI_FLOAT;
typedef double                  EI_DOUBLE;

typedef unsigned long long      EI_U64;
typedef long long               EI_S64;

typedef char                    EI_CHAR;

#define EI_VOID                 void

typedef unsigned int            EI_HANDLE;

typedef unsigned long           EI_SIZE_T;
typedef long                    EI_SSIZE_T;
typedef unsigned long           EI_LENGTH_T;

typedef enum {
    EI_FALSE = 0,
    EI_TRUE  = 1,
} EI_BOOL;


#ifndef NULL
#define NULL        0L
#endif


#define EI_NULL         0L
#define EI_SUCCESS      0
#define EI_FAILURE      (-1)



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_TYPE_H__ */

