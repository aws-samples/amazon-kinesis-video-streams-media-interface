/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_sam.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_SAM_H__
#define __EI_COMM_SAM_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_debug.h"
#include "ei_common.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef struct eiSAM_SRC_CHN_ATTR_S {
    EI_S32              s32Dev;
    EI_S32              s32Chn;
    EI_U32              u32Width;
    EI_U32              u32Height;
    EI_U32              u32Align;
    VIDEO_FORMAT_E      enVideoFormat;
    PIXEL_FORMAT_E      enPixelFormat;
    DYNAMIC_RANGE_E     enDynamicRange;
    COMPRESS_MODE_E     enCompressMode;
} SAM_SRC_CHN_ATTR_S;

typedef struct eiSAM_SINK_CHN_ATTR_S {
    EI_S32              s32Chn;
    EI_U32              u32DumpFrame;
    EI_U32              u32DumpNum;
    EI_CHAR             acDataName[64];

} SAM_SINK_CHN_ATTR_S;

#define EI_ERR_SAM_INVALID_DEVID        EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID      )
#define EI_ERR_SAM_INVALID_CHNID        EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID      )
#define EI_ERR_SAM_ILLEGAL_PARAM        EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM      )
#define EI_ERR_SAM_EXIST                EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST              )
#define EI_ERR_SAM_UNEXIST              EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST            )
#define EI_ERR_SAM_NULL_PTR             EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR           )
#define EI_ERR_SAM_NOT_CONFIG           EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG         )
#define EI_ERR_SAM_NOT_SUPPORT          EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT        )
#define EI_ERR_SAM_NOT_PERM             EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM           )
#define EI_ERR_SAM_LIST_EMPTY           EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_LIST_EMPTY         )
#define EI_ERR_SAM_NOMEM                EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM              )
#define EI_ERR_SAM_NOBUF                EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF              )
#define EI_ERR_SAM_BUF_EMPTY            EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY          )
#define EI_ERR_SAM_BUF_FULL             EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL           )
#define EI_ERR_SAM_SYS_NOTREADY         EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY       )
#define EI_ERR_SAM_BADADDR              EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR            )
#define EI_ERR_SAM_BUSY                 EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY               )
#define EI_ERR_SAM_SIZE_NOT_ENOUGH      EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH    )
#define EI_ERR_SAM_AGAIN                EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_AGAIN              )
#define EI_ERR_SAM_TIMEOUT              EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_TIMEOUT            )
#define EI_ERR_SAM_NOTSUPP              EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NOTSUPP            )
#define EI_ERR_SAM_NOT_ENABLED          EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_ENABLED        )
#define EI_ERR_SAM_NO_FOUND             EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_NO_FOUND           )
#define EI_ERR_SAM_OTHERS               EI_DEF_ERR(EI_ID_SAM, EN_ERR_LEVEL_ERROR, EN_ERR_OTHERS             )

#define EI_TRACE_SAM(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_SAM, level, fmt, ##args);\
} while(0)

#define SAM_ERRNO_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define SAM_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
        if (cond) { \
                EI_TRACE_SAM(EI_DBG_ERR, "errno[%d]!\n", errno);\
                return (ret);\
        } \
} while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_SAM_H__ */

