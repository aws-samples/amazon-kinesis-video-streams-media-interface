/*
 *------------------------------------------------------------------------------
 * @File      :    ei_errno.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_ERRNO_H__
#define __EI_ERRNO_H__

#include "ei_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



#define EI_ERR_APPID  (0x80000000L + 0x60000000L)

typedef enum eiERR_LEVEL_E {
    EN_ERR_LEVEL_DEBUG = 0,
    EN_ERR_LEVEL_INFO,
    EN_ERR_LEVEL_NOTICE,
    EN_ERR_LEVEL_WARNING,
    EN_ERR_LEVEL_ERROR,
    EN_ERR_LEVEL_CRIT,
    EN_ERR_LEVEL_ALERT,
    EN_ERR_LEVEL_FATAL,
    EN_ERR_LEVEL_MAX
} ERR_LEVEL_E;




#define EI_DEF_ERR( module, level, errid) \
    ((EI_S32)( (EI_ERR_APPID) | ((module) << 16 ) | ((level)<<12) | (errid) ))


typedef enum eiEN_ERR_CODE_E {
    EN_ERR_INVALID_DEVID        = 0x01,
    EN_ERR_INVALID_CHNID        = 0x02,
    EN_ERR_ILLEGAL_PARAM        = 0x03,
    EN_ERR_EXIST                = 0x04,
    EN_ERR_UNEXIST              = 0x05,
    EN_ERR_NULL_PTR             = 0x06,
    EN_ERR_NOT_CONFIG           = 0x07,
    EN_ERR_NOT_SUPPORT          = 0x08,
    EN_ERR_NOT_PERM             = 0x09,
    EN_ERR_LIST_EMPTY           = 0x0a,
    EN_ERR_NOMEM                = 0x0b,
    EN_ERR_NOBUF                = 0x0c,
    EN_ERR_BUF_EMPTY            = 0x0d,
    EN_ERR_BUF_FULL             = 0x0e,
    EN_ERR_SYS_NOTREADY         = 0x0f,
    EN_ERR_BADADDR              = 0x10,
    EN_ERR_BUSY                 = 0x11,
    EN_ERR_SIZE_NOT_ENOUGH      = 0x12,
    EN_ERR_AGAIN                = 0x13,
    EN_ERR_TIMEOUT              = 0x14,
    EN_ERR_NOTSUPP              = 0x15,
    EN_ERR_NOT_ENABLED          = 0x16,
    EN_ERR_NO_FOUND             = 0x17,

    EN_ERR_OTHERS               = 0x3e,

    EN_ERR_MAX                  = 0x3f,

} EN_ERR_CODE_E;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_ERRNO_H__ */

