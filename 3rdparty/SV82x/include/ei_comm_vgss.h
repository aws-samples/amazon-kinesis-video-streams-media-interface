/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_vgss.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_VGSS_H__
#define __EI_COMM_VGSS_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_video.h"
#include "ei_errno.h"
#include "ei_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define EI_TRACE_VGSS(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_VGSS, level, fmt, ##args);\
} while(0)

#define EI_ERR_VGSS_NULL_PTR         EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define EI_ERR_VGSS_NOTREADY         EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define EI_ERR_VGSS_NOT_PERM         EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define EI_ERR_VGSS_NOMEM            EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define EI_ERR_VGSS_ILLEGAL_PARAM    EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define EI_ERR_VGSS_BUSY             EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define EI_ERR_VGSS_NOT_SUPPORT      EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define EI_ERR_VGSS_SIZE_NOT_ENOUGH  EI_DEF_ERR(EI_ID_VGSS, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)

#define VGSS_ERRNO_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define VGSS_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
    if (cond) { \
        EI_TRACE_VGSS(EI_DBG_ERR, "errno[%d]!\n", errno);\
        return (ret);\
    } \
} while(0)

typedef struct eiVGSS_ROT_ATTR_S {
    EI_BOOL             bEnable;
    ROTATION_E          enRotation;
    MIRRORDIRECTION_E   enMirrorDirection;
} VGSS_ROT_ATTR_S;

typedef struct eiVGSS_DEV_ATTR_S {
    EI_S32              s32Dev;
    EI_U32              u32Depth;
    EI_U32              u32Width;
    EI_U32              u32Height;
    PIXEL_FORMAT_E      enInputFormat;
    PIXEL_FORMAT_E      enOutputFormat;
    CROP_INFO_S         stSrcCropInfo;
    CROP_INFO_S         stDstCropInfo;
    VGSS_ROT_ATTR_S     stRotAttr;
} VGSS_DEV_ATTR_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_VGSS_H__ */