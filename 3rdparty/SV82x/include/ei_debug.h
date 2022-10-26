/*
 *------------------------------------------------------------------------------
 * @File      :    ei_debug.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_DEBUG_H__
#define __EI_DEBUG_H__

#ifndef __KERNEL__
#include <stdio.h>
#include <stdarg.h>
#endif

#include "ei_type.h"
#include "ei_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define EI_DBG_EMERG      0
#define EI_DBG_ALERT      1
#define EI_DBG_CRIT       2
#define EI_DBG_ERR        3
#define EI_DBG_WARN       4
#define EI_DBG_NOTICE     5
#define EI_DBG_INFO       6
#define EI_DBG_DEBUG      7

#ifdef __EOS__
#include <debug.h>

#define EI_PRINT printf
#define EI_MONITOR  EI_PRINT("Monitor Func: %s, Line: %d\n", __FUNCTION__, __LINE__);

#define EI_ASSERT(expr)               \
    do{                                   \
        if (!(expr)) {                    \
            printf("\nASSERT at:\n" \
                "----->Func[%s], Line[%d], Condition: %s\n", \
                __FUNCTION__, __LINE__, #expr); \
            exit(-1);\
        } \
    }while(0)

int EI_MBASE_LOG(int mod_id, int level, const char *fmt, ...);
#define EI_TRACE(enModId, level, fmt, args...)\
        do{\
            EI_MBASE_LOG(enModId, level, "[k] %-32s L[%04d] "fmt, __func__, __LINE__, ##args); \
        } while(0)

#else /* __EOS__ */

#ifndef __KERNEL__

#define EI_PRINT printf
#define EI_MONITOR  EI_PRINT("Monitor Func: %s, Line: %d\n", __FUNCTION__, __LINE__);

#define EI_ASSERT(expr)               \
    do{                                   \
        if (!(expr)) {                    \
            printf("\nASSERT at:\n" \
                "----->Func[%s], Line[%d], Condition: %s\n", \
                __FUNCTION__, __LINE__, #expr); \
            exit(-1);\
        } \
    }while(0)

#ifndef LOG_TAG
#define LOG_TAG NULL
#endif
EI_VOID EI_MI_MBASE_LOG(int level, int mod_id, const char *fmt, ...);
#define EI_TRACE(enModId, level, fmt, args...)\
    do{\
        EI_MI_MBASE_LOG(level, enModId, "[u][Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##args); \
    } while(0)

#else /* __KERNEL__ */

#include "ei_os.h"
int EI_MBASE_LOG(int mod_id, int level, const char *fmt, ...);
#define EI_PRINT os_printk
#define EI_MONITOR  EI_PRINT("Monitor Func[%s], Line[%d]\n", __FUNCTION__, __LINE__);

#define EI_ASSERT(expr)               \
do{                                   \
    if (!(expr)) {                    \
        os_panic("\nASSERT at:\n" \
              "----->Func[%s], Line[%d], Condition: %s\n",    \
              __FUNCTION__, __LINE__, #expr);\
    } \
} while(0)

#define EI_TRACE(mod_id, level, fmt, args...) \
    do { \
        EI_MBASE_LOG(mod_id, level, "[k] %-32s L[%04d] "fmt, __func__, __LINE__, ##args); \
    } while (0)

#endif /* __KERNEL__ */

#endif /* __EOS__ */

#define EI_DBG(mod_id, fmt, args...)    EI_TRACE(mod_id, EI_DBG_DEBUG, fmt, ##args)
#define EI_INF(mod_id, fmt, args...)    EI_TRACE(mod_id, EI_DBG_INFO, fmt, ##args)
#define EI_NOTICE(mod_id, fmt, args...) EI_TRACE(mod_id, EI_DBG_NOTICE, fmt, ##args)
#define EI_WRN(mod_id, fmt, args...)    EI_TRACE(mod_id, EI_DBG_WARN, fmt, ##args)
#define EI_ERR(mod_id, fmt, args...)    EI_TRACE(mod_id, EI_DBG_ERR, fmt, ##args)
#define EI_CRIT(mod_id, fmt, args...)   EI_TRACE(mod_id, EI_DBG_CRIT, fmt, ##args)
#define EI_ALERT(mod_id, fmt, args...)  EI_TRACE(mod_id, EI_DBG_ALERT, fmt, ##args)
#define EI_EMERG(mod_id, fmt, args...)  EI_TRACE(mod_id, EI_DBG_EMERG, fmt, ##args)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_DEBUG_H__ */
