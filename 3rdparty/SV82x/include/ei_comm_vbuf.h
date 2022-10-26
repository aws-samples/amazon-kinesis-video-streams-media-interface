/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_vbuf.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_VBUF_H__
#define __EI_COMM_VBUF_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_debug.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define VBUF_INVALID_POOL (-1U)
#define VBUF_INVALID_HANDLE (-1U)
#define VBUF_INVALID_BUFFER (-1U)

#define VBUF_MAX_POOLS 256
#define VBUF_MAX_BUFFERS 128
#define VBUF_MAX_COMM_POOLS 16
#define VBUF_MAX_FRM_INFO_ARRAY 8

#define POOL_OWNER_COMMON    -1
#define POOL_OWNER_PRIVATE    -2

typedef EI_UL VBUF_POOL;
typedef EI_UL VBUF_BUFFER;

#define RESERVE_MEM_NAME "media"

typedef enum eiVBUF_UID_E {
    VBUF_UID_COMMON       = 0,
    VBUF_UID_ISP,
    VBUF_UID_VISS,
    VBUF_UID_DOSS,
    VBUF_UID_ANONYMOUS,
    VBUF_UID_AUDIOIN,
    VBUF_UID_AUDIODEC,
    VBUF_UID_VGSS,
    VBUF_UID_GDC,
    VBUF_UID_VENC,
    VBUF_UID_VDEC,
    VBUF_UID_MAX,

} VBUF_UID_E;

#define VBUF_MAX_USER   VBUF_UID_MAX

typedef struct eiVBUF_POOL_LINK_S {
    EI_S32      s32DevId;
    EI_S32      s32ChnId;
    VBUF_POOL   PoolId;
} VBUF_POOL_LINK_S;

typedef enum eiVBUF_REMAP_MODE_E {
    VBUF_REMAP_MODE_CACHED    = 0,
    VBUF_REMAP_MODE_NOCACHE   = 1,
    VBUF_REMAP_MODE_NONE      = 2,
    VBUF_REMAP_MODE_MAX
} VBUF_REMAP_MODE_E;

typedef enum eiVBUF_FRAME_INFO_TYPE_E {
    VBUF_FRAME_INFO_TYPE_SINGLE    = 0,
    VBUF_FRAME_INFO_TYPE_ARRAY     = 1,
    VBUF_FRAME_INFO_TYPE_MAX
} VBUF_FRAME_INFO_TYPE_E;

typedef struct eiVBUF_POOL_CONFIG_S {
    EI_U32 u32BufSize;
    EI_U32 u32BufCnt;
    VBUF_UID_E enVbufUid;
    VBUF_REMAP_MODE_E enRemapMode;
    EI_CHAR acPoolName[MAX_POOL_NAME_LEN];
    EI_BOOL bForceDestroyEn;
} VBUF_POOL_CONFIG_S;

typedef struct eiVBUF_CONFIG_S {
    EI_U32 u32PoolCnt;
    VBUF_FRAME_INFO_TYPE_E enFrmInfoType;
    EI_U32 au32FrmInfoCnt[VBUF_MAX_COMM_POOLS];
    VBUF_POOL_CONFIG_S astCommPool[VBUF_MAX_COMM_POOLS];
    union {
        VIDEO_FRAME_INFO_S astFrameInfo[VBUF_MAX_COMM_POOLS];
        VIDEO_FRAME_INFO_S astFrameInfoArray[VBUF_MAX_COMM_POOLS][VBUF_MAX_FRM_INFO_ARRAY];
    };
} VBUF_CONFIG_S;

#define EI_ERR_VBUF_INVALID_DEVID        EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID      )
#define EI_ERR_VBUF_INVALID_CHNID        EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID      )
#define EI_ERR_VBUF_ILLEGAL_PARAM        EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM      )
#define EI_ERR_VBUF_EXIST                EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST              )
#define EI_ERR_VBUF_UNEXIST              EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST            )
#define EI_ERR_VBUF_NULL_PTR             EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR           )
#define EI_ERR_VBUF_NOT_CONFIG           EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG         )
#define EI_ERR_VBUF_NOT_SUPPORT          EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT        )
#define EI_ERR_VBUF_NOT_PERM             EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM           )
#define EI_ERR_VBUF_LIST_EMPTY           EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_LIST_EMPTY         )
#define EI_ERR_VBUF_NOMEM                EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM              )
#define EI_ERR_VBUF_NOBUF                EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF              )
#define EI_ERR_VBUF_BUF_EMPTY            EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY          )
#define EI_ERR_VBUF_BUF_FULL             EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL           )
#define EI_ERR_VBUF_BADADDR              EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR            )
#define EI_ERR_VBUF_BUSY                 EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY               )
#define EI_ERR_VBUF_SIZE_NOT_ENOUGH      EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH    )
#define EI_ERR_VBUF_AGAIN                EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_AGAIN              )
#define EI_ERR_VBUF_TIMEOUT              EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_TIMEOUT            )
#define EI_ERR_VBUF_NOTSUPP              EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NOTSUPP            )
#define EI_ERR_VBUF_NOT_ENABLED          EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_ENABLED        )
#define EI_ERR_VBUF_NO_FOUND             EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_NO_FOUND           )
#define EI_ERR_VBUF_OTHERS               EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_OTHERS             )
#define EI_ERR_VBUF_NOTREADY             EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY       )

#define EI_ERR_VBUF_2MPOOLS              EI_DEF_ERR(EI_ID_VBUF, EN_ERR_LEVEL_ERROR, EN_ERR_MAX + 1            )

#define EI_VBUF_CHECK_POOL(Pool) (Pool != EI_NULL && Pool != VBUF_INVALID_POOL)
#define EI_VBUF_CHECK_BUFFER(Buffer) (Buffer != EI_NULL && Buffer != VBUF_INVALID_BUFFER)

#define EI_TRACE_VBUF(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_VBUF, level, fmt, ##args);\
}while(0)

#define VBUF_ERRNO_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define VBUF_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
        if (cond) { \
                EI_TRACE_VBUF(EI_DBG_ERR, "errno[%d]!\n", errno);\
                return (ret);\
        } \
} while(0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_VBUF_H_ */

