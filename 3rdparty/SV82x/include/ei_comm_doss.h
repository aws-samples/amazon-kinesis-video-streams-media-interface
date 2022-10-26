/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_doss.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_DOSS_H__
#define __EI_COMM_DOSS_H__

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

#define EI_TRACE_DOSS(level, fmt, args...)\
do{ \
    EI_TRACE(EI_ID_DOSS, level, fmt, ##args);\
} while(0)

typedef enum eiEN_VOU_ERR_CODE_E {
    EN_ERR_VO_DEV_NOT_CONFIG = 0x40,
    EN_ERR_VO_DEV_NOT_ENABLE = 0x41,
    EN_ERR_VO_DEV_HAS_ENABLED = 0x42,
    EN_ERR_VO_DEV_HAS_BINDED = 0x43,
    EN_ERR_VO_DEV_NOT_BINDED = 0x44,

    ERR_VO_NOT_ENABLE = 0x45,
    ERR_VO_NOT_DISABLE = 0x46,
    ERR_VO_NOT_CONFIG = 0x47,

    ERR_VO_CHN_NOT_DISABLE = 0x48,
    ERR_VO_CHN_NOT_ENABLE = 0x49,
    ERR_VO_CHN_NOT_CONFIG = 0x4a,
    ERR_VO_CHN_NOT_ALLOC = 0x4b,

    ERR_VO_CCD_INVALID_PAT = 0x4c,
    ERR_VO_CCD_INVALID_POS = 0x4d,

    ERR_VO_WAIT_TIMEOUT = 0x4e,
    ERR_VO_INVALID_VFRAME = 0x4f,
    ERR_VO_INVALID_RECT_PARA = 0x50,
    ERR_VO_SETBEGIN_ALREADY = 0x51,
    ERR_VO_SETBEGIN_NOTYET = 0x52,
    ERR_VO_SETEND_ALREADY = 0x53,
    ERR_VO_SETEND_NOTYET = 0x54,

    ERR_VO_GRP_INVALID_ID = 0x55,
    ERR_VO_GRP_NOT_CREATE = 0x56,
    ERR_VO_GRP_HAS_CREATED = 0x57,
    ERR_VO_GRP_NOT_DESTROY = 0x58,
    ERR_VO_GRP_CHN_FULL = 0x59,
    ERR_VO_GRP_CHN_EMPTY = 0x5a,
    ERR_VO_GRP_CHN_NOT_EMPTY = 0x5b,
    ERR_VO_GRP_INVALID_SYN_MODE = 0x5c,
    ERR_VO_GRP_INVALID_BASE_PTS = 0x5d,
    ERR_VO_GRP_NOT_START = 0x5e,
    ERR_VO_GRP_NOT_STOP = 0x5f,
    ERR_VO_GRP_INVALID_FRMRATE = 0x60,
    ERR_VO_GRP_CHN_HAS_REG = 0x61,
    ERR_VO_GRP_CHN_NOT_REG = 0x62,
    ERR_VO_GRP_CHN_NOT_UNREG = 0x63,
    ERR_VO_GRP_BASE_NOT_CFG = 0x64,

    ERR_GFX_NOT_DISABLE = 0x65,
    ERR_GFX_NOT_BIND = 0x66,
    ERR_GFX_NOT_UNBIND = 0x67,
    ERR_GFX_INVALID_ID = 0x68,

    ERR_VO_WBC_NOT_DISABLE = 0x69,
    ERR_VO_WBC_NOT_CONFIG = 0x6a,

    ERR_VO_CHN_AREA_OVERLAP = 0x6b,

    EN_ERR_INVALID_WBCID = 0x6c,
    EN_ERR_INVALID_LAYERID = 0x6d,
    EN_ERR_VO_VIDEO_HAS_BINDED = 0x6e,
    EN_ERR_VO_VIDEO_NOT_BINDED = 0x6f,
    ERR_VO_WBC_HAS_BIND = 0x70,
    ERR_VO_WBC_HAS_CONFIG = 0x71,
    ERR_VO_WBC_NOT_BIND = 0x72,

    /* new added */
    ERR_VO_BUTT
} EN_VOU_ERR_CODE_E;

/* System define error code */
#define EI_ERR_VO_BUSY                 EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define EI_ERR_VO_NO_MEM               EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define EI_ERR_VO_NULL_PTR             EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define EI_ERR_VO_SYS_NOTREADY         EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define EI_ERR_VO_INVALID_DEVID        EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define EI_ERR_VO_INVALID_CHNID        EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define EI_ERR_VO_ILLEGAL_PARAM        EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
#define EI_ERR_VO_NOT_SUPPORT          EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define EI_ERR_VO_NOT_PERMIT           EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
#define EI_ERR_VO_INVALID_WBCID        EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_WBCID)
#define EI_ERR_VO_INVALID_LAYERID      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_LAYERID)
#define EI_ERR_VO_SIZE_NOT_ENOUGH      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_SIZE_NOT_ENOUGH)

/* Device relative error code */
#define EI_ERR_VO_DEV_NOT_CONFIG       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_NOT_CONFIG)
#define EI_ERR_VO_DEV_NOT_ENABLE       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_NOT_ENABLE)
#define EI_ERR_VO_DEV_HAS_ENABLED      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_HAS_ENABLED)
#define EI_ERR_VO_DEV_HAS_BINDED       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_HAS_BINDED)
#define EI_ERR_VO_DEV_NOT_BINDED       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_VO_DEV_NOT_BINDED)

/* Video layer relative error code */
#define EI_ERR_VO_VIDEO_NOT_ENABLE     EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_NOT_ENABLE)
#define EI_ERR_VO_VIDEO_NOT_DISABLE    EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_NOT_DISABLE)
#define EI_ERR_VO_VIDEO_NOT_CONFIG     EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_NOT_CONFIG)
#define EI_ERR_VO_VIDEO_HAS_BINDED     EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_VO_VIDEO_HAS_BINDED)
#define EI_ERR_VO_VIDEO_NOT_BINDED     EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, EN_ERR_VO_VIDEO_NOT_BINDED)

/* WBC Relative error code */
#define EI_ERR_VO_WBC_NOT_DISABLE      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_WBC_NOT_DISABLE)
#define EI_ERR_VO_WBC_NOT_CONFIG       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_WBC_NOT_CONFIG)
#define EI_ERR_VO_WBC_HAS_CONFIG       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_WBC_HAS_CONFIG)
#define EI_ERR_VO_WBC_NOT_BIND         EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_WBC_NOT_BIND)
#define EI_ERR_VO_WBC_HAS_BIND         EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_WBC_HAS_BIND)

/* Channel Relative error code */
#define EI_ERR_VO_CHN_NOT_DISABLE      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_CHN_NOT_DISABLE)
#define EI_ERR_VO_CHN_NOT_ENABLE       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_CHN_NOT_ENABLE)
#define EI_ERR_VO_CHN_NOT_CONFIG       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_CHN_NOT_CONFIG)
#define EI_ERR_VO_CHN_NOT_ALLOC        EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_CHN_NOT_ALLOC)
#define EI_ERR_VO_CHN_AREA_OVERLAP     EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_CHN_AREA_OVERLAP)

/* Cascade Relatvie error code */
#define EI_ERR_VO_INVALID_PATTERN      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_CCD_INVALID_PAT)
#define EI_ERR_VO_INVALID_POSITION     EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_CCD_INVALID_POS)

/* MISCellaneous error code */
#define EI_ERR_VO_WAIT_TIMEOUT         EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_WAIT_TIMEOUT)
#define EI_ERR_VO_INVALID_VFRAME       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_INVALID_VFRAME)
#define EI_ERR_VO_INVALID_RECT_PARA    EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_INVALID_RECT_PARA)
#define EI_ERR_VO_SETBEGIN_ALREADY     EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_SETBEGIN_ALREADY)
#define EI_ERR_VO_SETBEGIN_NOTYET      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_SETBEGIN_NOTYET)
#define EI_ERR_VO_SETEND_ALREADY       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_SETEND_ALREADY)
#define EI_ERR_VO_SETEND_NOTYET        EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_SETEND_NOTYET)

/* Synchronization group relative error code */
#define EI_ERR_VO_GRP_INVALID_ID       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_INVALID_ID)
#define EI_ERR_VO_GRP_NOT_CREATE       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_NOT_CREATE)
#define EI_ERR_VO_GRP_HAS_CREATED      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_HAS_CREATED)
#define EI_ERR_VO_GRP_NOT_DESTROY      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_NOT_DESTROY)
#define EI_ERR_VO_GRP_CHN_FULL         EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_CHN_FULL)
#define EI_ERR_VO_GRP_CHN_EMPTY        EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_CHN_EMPTY)
#define EI_ERR_VO_GRP_CHN_NOT_EMPTY    EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_CHN_NOT_EMPTY)
#define EI_ERR_VO_GRP_INVALID_SYN_MODE EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_INVALID_SYN_MODE)
#define EI_ERR_VO_GRP_INVALID_BASE_PTS EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_INVALID_BASE_PTS)
#define EI_ERR_VO_GRP_NOT_START        EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_NOT_START)
#define EI_ERR_VO_GRP_NOT_STOP         EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_NOT_STOP)
#define EI_ERR_VO_GRP_INVALID_FRMRATE  EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_INVALID_FRMRATE)
#define EI_ERR_VO_GRP_CHN_HAS_REG      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_CHN_HAS_REG)
#define EI_ERR_VO_GRP_CHN_NOT_REG      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_CHN_NOT_REG)
#define EI_ERR_VO_GRP_CHN_NOT_UNREG    EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_CHN_NOT_UNREG)
#define EI_ERR_VO_GRP_BASE_NOT_CFG     EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_VO_GRP_BASE_NOT_CFG)

/* Graphics layer relative error code */
#define EI_ERR_VO_GFX_NOT_DISABLE      EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_GFX_NOT_DISABLE)
#define EI_ERR_VO_GFX_NOT_BIND         EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_GFX_NOT_BIND)
#define EI_ERR_VO_GFX_NOT_UNBIND       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_GFX_NOT_UNBIND)
#define EI_ERR_VO_GFX_INVALID_ID       EI_DEF_ERR(EI_ID_DOSS, EN_ERR_LEVEL_ERROR, ERR_GFX_INVALID_ID)

#define DOSS_ERRNO_RETURN_ERR_IF(cond, ret)  do {if (cond) {return (ret);} else {}} while(0)

#define DOSS_ERRNO_RETURN_VAL_IF(cond, errno, ret) \
do { \
    if (cond) { \
        EI_TRACE_DOSS(EI_DBG_ERR, "errno[%d]!\n", errno);\
        return (ret);\
    } \
} while(0)

#define VO_INTF_CVBS                   (0x01L << 0)
#define VO_INTF_YPBPR                  (0x01L << 1)
#define VO_INTF_VGA                    (0x01L << 2)
#define VO_INTF_BT656                  (0x01L << 3)
#define VO_INTF_BT1120                 (0x01L << 4)
#define VO_INTF_HDMI                   (0x01L << 5)
#define VO_INTF_LCD                    (0x01L << 6)
#define VO_INTF_BT656_H                (0x01L << 7)
#define VO_INTF_BT656_L                (0x01L << 8)
#define VO_INTF_LCD_6BIT               (0x01L << 9)
#define VO_INTF_LCD_8BIT               (0x01L << 10)
#define VO_INTF_LCD_16BIT              (0x01L << 11)
#define VO_INTF_LCD_18BIT              (0x01L << 12)
#define VO_INTF_LCD_24BIT              (0x01L << 13)
#define VO_INTF_MIPI                   (0x01L << 14)
#define VO_INTF_MIPI_SLAVE             (0x01L << 15)
#define VO_INTF_I8080_18BIT_RGB666     (0x01L << 16)
#define VO_INTF_I8080_16BIT_RGB888     (0x01L << 17)
#define VO_INTF_I8080_16BIT_RGB666     (0x01L << 18)
#define VO_INTF_I8080_16BIT_RGB565     (0x01L << 19)
#define VO_INTF_I8080_9BIT_RGB666      (0x01L << 20)
#define VO_INTF_I8080_8BIT_RGB666      (0x01L << 21)
#define VO_INTF_I8080_8BIT_RGB565      (0x01L << 22)
#define VO_INTF_I8080_8BIT_RGB888      (0x01L << 23)
#define VO_INTF_LVDS_JEIDA_24BIT       (0x01L << 24)
#define VO_INTF_LVDS_JEIDA_18BIT       (0x01L << 25)
#define VO_INTF_LVDS_VESA_24BIT        (0x01L << 26)

typedef EI_U32 VO_INTF_TYPE_E;

typedef enum eiVO_INTF_SYNC_E {
    VO_OUTPUT_USER,

    VO_OUTPUT_BUTT
} VO_INTF_SYNC_E;

typedef enum eiVO_ZOOM_IN_E {
    VO_ZOOM_IN_RECT = 0,
    VO_ZOOM_IN_RATIO,
    VO_ZOOM_IN_BUTT
} VO_ZOOM_IN_E;

typedef enum eiVO_CSC_MATRIX_E {
    VO_CSC_MATRIX_IDENTITY = 0,

    VO_CSC_MATRIX_BT601_TO_BT709,
    VO_CSC_MATRIX_BT709_TO_BT601,

    VO_CSC_MATRIX_BT601_TO_RGB_PC,
    VO_CSC_MATRIX_BT709_TO_RGB_PC,

    VO_CSC_MATRIX_RGB_TO_BT601_PC,
    VO_CSC_MATRIX_RGB_TO_BT709_PC,

    VO_CSC_MATRIX_RGB_TO_BT2020_PC,
    VO_CSC_MATRIX_BT2020_TO_RGB_PC,

    VO_CSC_MATRIX_RGB_TO_BT601_TV,
    VO_CSC_MATRIX_RGB_TO_BT709_TV,

    VO_CSC_MATRIX_BUTT
} VO_CSC_MATRIX_E;

typedef struct eiVO_CHN_ATTR_S {
    EI_U32 u32Priority;
    RECT_S stRect;
} VO_CHN_ATTR_S;

typedef struct eiVO_QUERY_STATUS_S {
    EI_U32 u32ChnBufUsed;
} VO_QUERY_STATUS_S;

typedef enum {
    OUTPUT_MODE_CSI            = 0x0,
    OUTPUT_MODE_DSI_VIDEO      = 0x1,
    OUTPUT_MODE_DSI_CMD        = 0x2,

    OUTPUT_MODE_BUTT
} output_mode_t;

typedef enum {
    BURST_MODE                      = 0x0,
    NON_BURST_MODE_SYNC_PULSES      = 0x1,
    NON_BURST_MODE_SYNC_EVENTS      = 0x2,
} video_mode_t;

typedef enum {
    OUT_FORMAT_RGB_16_BIT          = 0x0,
    OUT_FORMAT_RGB_18_BIT          = 0x1,
    OUT_FORMAT_RGB_24_BIT          = 0x2,
    OUT_FORMAT_YUV420_8_BIT_NORMAL = 0x3,
    OUT_FORMAT_YUV420_8_BIT_LEGACY = 0x4,
    OUT_FORMAT_YUV422_8_BIT        = 0x5,

    OUT_FORMAT_BUTT
} output_format_t;

typedef struct eiVO_SYNC_INFO_S {
    EI_BOOL bIop;
    EI_BOOL bDual;

    EI_U16 u16Vact;
    EI_U16 u16Vbb;
    EI_U16 u16Vfb;

    EI_U16 u16Hact;
    EI_U16 u16Hbb;
    EI_U16 u16Hfb;
    EI_U16 u16Hmid;

    EI_U16 u16Bvact;
    EI_U16 u16Bvbb;
    EI_U16 u16Bvfb;

    EI_U16 u16Hpw;
    EI_U16 u16Vpw;

    EI_U32 u32Pclk;

    EI_BOOL bIdv;
    EI_BOOL bIhs;
    EI_BOOL bIvs;
    EI_BOOL bICv;

    EI_BOOL bHasHsyncCmd;
    EI_BOOL bHasVsyncCmd;
    EI_BOOL bHasTE;
    EI_BOOL bTEPol;
    EI_U32 u32CmdAlign;

    EI_BOOL bDataMirror;

    short               lane_id[LANE_MAX_NUM];
    output_mode_t       output_mode;
    video_mode_t        video_mode;
    output_format_t     output_format;
} VO_SYNC_INFO_S;

typedef struct {
    unsigned short      cmd_size;
    void       *cmd;
} cmd_info_t;

typedef struct eiVO_PUB_ATTR_S {
    VO_INTF_TYPE_E enIntfType;
    VO_INTF_SYNC_E enIntfSync;
    VO_SYNC_INFO_S stSyncInfo;
} VO_PUB_ATTR_S;

typedef struct eiVO_WBC_ATTR_S {
    SIZE_S stTargetSize;
    PIXEL_FORMAT_E enPixelFormat;
    EI_U32 u32FrameRate;
    DYNAMIC_RANGE_E enDynamicRange;
    COMPRESS_MODE_E enCompressMode;
} VO_WBC_ATTR_S;

typedef enum eiVO_WBC_MODE_E {
    VO_WBC_MODE_NORMAL = 0,
    VO_WBC_MODE_DROP_REPEAT,
    VO_WBC_MODE_PROG_TO_INTL,
    VO_WBC_MODE_BUTT,
} VO_WBC_MODE_E;

typedef enum eiVO_WBC_SOURCE_TYPE_E {
    VO_WBC_SOURCE_DEV = 0x0,
    VO_WBC_SOURCE_VIDEO = 0x1,
    VO_WBC_SOURCE_GRAPHIC = 0x2,
    VO_WBC_SOURCE_BUTT
} VO_WBC_SOURCE_TYPE_E;

typedef struct eiVO_WBC_SOURCE_S {
    VO_WBC_SOURCE_TYPE_E enSourceType;
    EI_U32 u32SourceId;
} VO_WBC_SOURCE_S;

typedef enum eiVO_PART_MODE_E {
    VO_PART_MODE_SINGLE = 0,
    VO_PART_MODE_MULTI = 1,
    VO_PART_MODE_BUTT
} VO_PART_MODE_E;

typedef struct eiVO_VIDEO_LAYER_ATTR_S {
    RECT_S stDispRect;
    SIZE_S stImageSize;
    EI_U32 u32DispFrmRt;
    EI_U32 u32Align;
    PIXEL_FORMAT_E enPixFormat;
} VO_VIDEO_LAYER_ATTR_S;

typedef struct eiVO_ZOOM_RATIO_S {
    EI_U32 u32XRatio;
    EI_U32 u32YRatio;
    EI_U32 u32WRatio;
    EI_U32 u32HRatio;
} VO_ZOOM_RATIO_S;

typedef struct eiVO_ZOOM_ATTR_S {
    VO_ZOOM_IN_E enZoomType;
    union {
        RECT_S stZoomRect;
        VO_ZOOM_RATIO_S stZoomRatio;
    };
} VO_ZOOM_ATTR_S;

typedef struct eiVO_CSC_S {
    EI_U32 u32Luma;
    EI_U32 u32Contrast;
    EI_U32 u32Satuature;

    EI_U32 u32SatuatureRed;
    EI_U32 u32SatuatureOrange;
    EI_U32 u32SatuatureYellow;
    EI_U32 u32SatuatureGreen;
    EI_U32 u32SatuatureCyan;
    EI_U32 u32SatuatureBlue;
    EI_U32 u32SatuaturePurple;
    EI_U32 u32SatuatureMagenta;
} VO_CSC_S;

struct eiVO_WBC_FRAME_INFO_S {
    VIDEO_FRAME_INFO_S stVFrame;
    EI_S32 s32MilliSec;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* __EI_COMM_MLINK_H__ */
