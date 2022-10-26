/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_mbase.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_MBASE_H__
#define __EI_COMM_MBASE_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct eiMBASE_MOD_INFO_S {
    MOD_ID_E    enModId;
    EI_CHAR     cModName[32];
    EI_S32      s32LogLvl;
} MBASE_MOD_INFO_S;

typedef struct eiMBASE_VIRMEM_INFO_S
{
    EI_U64  u64PhyAddr;
    EI_BOOL bCached;
} MBASE_VIRMEM_INFO_S;

#define EI_ERR_MBASE_INVALID_DEVID        EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID      )
#define EI_ERR_MBASE_INVALID_CHNID        EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID      )
#define EI_ERR_MBASE_ILLEGAL_PARAM        EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM      )
#define EI_ERR_MBASE_EXIST                EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST              )
#define EI_ERR_MBASE_UNEXIST              EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST            )
#define EI_ERR_MBASE_NULL_PTR             EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR           )
#define EI_ERR_MBASE_NOT_CONFIG           EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG         )
#define EI_ERR_MBASE_NOT_SUPPORT          EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT        )
#define EI_ERR_MBASE_NOT_PERM             EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM           )
#define EI_ERR_MBASE_LIST_EMPTY           EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_LIST_EMPTY         )
#define EI_ERR_MBASE_NOMEM                EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM              )
#define EI_ERR_MBASE_NOBUF                EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF              )
#define EI_ERR_MBASE_BUF_EMPTY            EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY          )
#define EI_ERR_MBASE_BUF_FULL             EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL           )
#define EI_ERR_MBASE_SYS_NOTREADY         EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY       )
#define EI_ERR_MBASE_BADADDR              EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR            )
#define EI_ERR_MBASE_BUSY                 EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY               )
#define EI_ERR_MBASE_SIZE_NOT_ENOUGH      EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH    )
#define EI_ERR_MBASE_AGAIN                EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_AGAIN              )
#define EI_ERR_MBASE_TIMEOUT              EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_TIMEOUT            )
#define EI_ERR_MBASE_NOTSUPP              EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NOTSUPP            )
#define EI_ERR_MBASE_NOT_ENABLED          EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_ENABLED        )
#define EI_ERR_MBASE_NO_FOUND             EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_NO_FOUND           )
#define EI_ERR_MBASE_OTHERS               EI_DEF_ERR(EI_ID_MBASE, EN_ERR_LEVEL_ERROR, EN_ERR_OTHERS             )

#define EI_TRACE_MBASE(level, fmt, args...)\
    do{ \
        EI_TRACE(EI_ID_MBASE, level, fmt, ##args);\
    } while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_MBASE_H__ */

