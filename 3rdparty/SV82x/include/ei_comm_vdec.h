/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_vdec.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifndef __EI_COMM_VDEC_H__
#define __EI_COMM_VDEC_H__

#include "ei_type.h"
#include "ei_errno.h"
#include "ei_debug.h"
#include "ei_comm_rc.h"
#include "ei_comm_vbuf.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define EI_IO_BLOCK   EI_TRUE
#define EI_IO_NOBLOCK EI_FALSE

#ifndef EI_ID_VDEC
	#define EI_ID_VDEC  EI_ID_JVC
#endif

#define EI_TRACE_VDEC(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_VDEC, level, fmt, ##args);\
} while(0)


#define EI_ERR_VDEC_INVALID_CHNID     EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define EI_ERR_VDEC_ILLEGAL_PARAM     EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define EI_ERR_VDEC_EXIST             EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
#define EI_ERR_VDEC_NULL_PTR          EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define EI_ERR_VDEC_NOT_CONFIG        EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define EI_ERR_VDEC_NOT_SUPPORT       EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define EI_ERR_VDEC_NOT_PERM          EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define EI_ERR_VDEC_UNEXIST           EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
#define EI_ERR_VDEC_NOMEM             EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define EI_ERR_VDEC_NOBUF             EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
#define EI_ERR_VDEC_BUF_EMPTY         EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define EI_ERR_VDEC_BUF_FULL          EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define EI_ERR_VDEC_SYS_NOTREADY      EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define EI_ERR_VDEC_BUSY              EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define EI_ERR_VDEC_BADADDR           EI_DEF_ERR(EI_ID_VDEC, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR)

#define VDEC_ERRNO_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define VDEC_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
        if (cond) { \
                EI_TRACE_VDEC(EI_DBG_ERR, "errno[%d]!\n", errno);\
                return (ret);\
        } \
} while(0)

typedef enum eiVIDEO_MODE_E
{
    VIDEO_MODE_STREAM = 0,
    VIDEO_MODE_FRAME     ,
}VIDEO_MODE_E;

typedef enum eiVIDEO_POOL_E
{
    VIDEO_POOL_PRIVATE = 0,
    VIDEO_POOL_USER     ,
}VIDEO_POOL_E;

typedef struct eiVDEC_SCALE_S{
    EI_U32              u32Align;
    EI_U32              u32Width;
    EI_U32              u32Height;
    EI_BOOL             bFixedRatio;
    EI_BOOL             bScaleEnable;
}VDEC_SCALE_S;

typedef struct eiVDEC_ATTR_VIDEO_S
{
    VDEC_SCALE_S stVdecScale;
    EI_BOOL      bReorderEn;
}VDEC_ATTR_VIDEO_S;


typedef struct eiVDEC_CHN_ATTR_S
{
    PAYLOAD_TYPE_E      enType;
    VIDEO_MODE_E        enMode;
    EI_U32              u32Width;
    EI_U32              u32Height;
    EI_U32              u32Align;
    PIXEL_FORMAT_E      enPixelFormat;
    EI_U32              u32StreamBufSize;
    EI_U32              u32FrameBufSize;
    EI_U32              u32FrameBufCnt;
    EI_U32              u32UserDisFrameNum;
    VIDEO_POOL_E        enVdecPoolMode;
    VBUF_REMAP_MODE_E   enRemapMode;
    union
    {
        VDEC_ATTR_VIDEO_S stVdecVideoAttr;
    };
}VDEC_CHN_ATTR_S;


typedef struct eiVDEC_STREAM_S
{
    EI_U32  u32Len;
    EI_U64  u64PTS;
    EI_BOOL bEndOfFrame;
    EI_BOOL bEndOfStream;
    EI_BOOL bDisplay;
    EI_BOOL bFlushBuffer;
    EI_U8* ATTRIBUTE pu8Addr;
}VDEC_STREAM_S;

typedef struct eiVDEC_USERDATA_S
{
    EI_U64           u64PhyAddr;
    EI_U32           u32Len;
    EI_BOOL          bValid;
    EI_U8* ATTRIBUTE pu8Addr;
}VDEC_USERDATA_S;

typedef enum eiVIDEO_DISPLAY_MODE_S
{
    VIDEO_DISPLAY_MODE_PLAYBACK      = 0X0,
    VIDEO_DISPLAY_MODE_PREVIEW     = 0X1,
    VIDEO_DISPLAY_MODE_BUTT
}VIDEO_DISPLAY_MODE_S;

typedef struct ei_VDEC_DECODE_ERROR_S
{
    EI_S32 s32PicSizeErrSet;
    EI_S32 s32PicBufSizeErrSet;
    EI_S32 s32PicDecAbortErrSet;
}VDEC_DECODE_ERROR_S;


typedef struct eiVDEC_CHN_STATUS_S
{
    PAYLOAD_TYPE_E enType;
    EI_U32  u32LeftStreamBytes;
    EI_U32  u32LeftStreamFrames;
    EI_U32  u32LeftPics;
    EI_BOOL bStartRecvStream;
    EI_U32  u32RecvStreamFrames;
    EI_U32  u32DecodeStreamFrames;
    VDEC_DECODE_ERROR_S stVdecDecErr;
}VDEC_CHN_STATUS_S;

typedef struct eiVDEC_CHN_PARAM_S
{
    EI_U32  u32DisplayFrameNum;
}VDEC_CHN_PARAM_S;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef  __EI_COMM_VDEC_H__ */