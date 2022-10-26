/*
 *------------------------------------------------------------------------------
 * @File      :    ei_math.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_MATH_H__
#define __EI_MATH_H__

#ifndef __KERNEL__
#include <unistd.h>
#endif
#include "ei_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#define EI_ABS(x)          ( (x) >= 0 ? (x) : (-(x)) )
#define EI_SIGN(x)         ( (x) >= 0 ? 1 : -1 )
#define EI_CMP(x,y)        (((x) == (y)) ? 0 : (((x) > (y)) ? 1 : -1))

#define EI_BIT_SHIFT(v, s) ((s > 0) ? (v << s) : (v >> (-s)))

#define EI_MAX2(x,y)       ( (x)>(y) ? (x):(y) )
#define EI_MIN2(x,y)       ( (x)<(y) ? (x):(y) )
#define EI_MAX3(x,y,z)     ( (x)>(y) ? EI_MAX2(x,z) : EI_MAX2(y,z) )
#define EI_MIN3(x,y,z)     ( (x)<(y) ? EI_MIN2(x,z) : EI_MIN2(y,z) )
#define EI_MEDIAN(x,y,z)   (((x)+(y)+(z) - EI_MAX3(x,y,z)) - EI_MIN3(x,y,z) )
#define EI_MEAN2(x,y)      (((x)+(y)) >> 1 )
#define EI_CONSTRAIN(x, lo, hi) (EI_MIN2(EI_MAX2((x), (lo)), (hi)))

#define EI_CLIP_MIN(x,min)          (((x) >= min) ? (x) : min)
#define EI_CLIP3(x,min,max)         ( (x)< (min) ? (min) : ((x)>(max)?(max):(x)) )
#define EI_CLIP_MAX(x,max)          ((x)>(max)?(max):(x))
#define EI_WRAP_MAX(x,max,min)      ( (x)>=(max) ? (min) : (x) )
#define EI_WRAP_MIN(x,min,max)      ( (x)<=(min) ? (max) : (x) )
#define EI_VALUE_BETWEEN(x,min,max) (((x)>=(min)) && ((x) <= (max)))

#define EI_MULTI_OF_2_POWER(x,a)    (!((x) & ((a) - 1) ) )
#define EI_EICEILING(x, a)           (((x)+(a)-1)/(a))

#define EI_ALIGN_UP(x, a)           ( ( ((x) + ((a) - 1) ) / a ) * a )
#define EI_ALIGN_DOWN(x, a)         ( ( (x) / (a)) * (a) )

#define EI_DIV_UP(x, a)             ( ((x) + ((a) - 1) ) / a )
#define EI_DIV_ROUND(x, a)          (((x) < 0) ^ ((a) < 0)) ? (((x) - (a)/2)/(a)) : (((x) + (a)/2)/(a))

#define EI_SPAN(type, begin, end)\
({                            \
   type b = (begin);          \
   type e = (end);            \
   (type)((b >= e) ? (b - e) : (b + ((~((type)0))-e))); \
})

#define EI_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define ei_usleep(usec) \
do { \
    usleep(usec); \
} while (0)

#define ei_msleep(msec) \
do { \
    ei_usleep(msec * 1000); \
} while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __EI_MATH_H__ */

