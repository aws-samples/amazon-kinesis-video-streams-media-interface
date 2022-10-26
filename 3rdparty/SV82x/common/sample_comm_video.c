/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_video.c
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Source file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "ei_posix.h"

#include "ei_common.h"
#include "sample_comm.h"

static VIDEO_FRAME_FMT_S g_stVideoFrameFmt[] = {
    {
        .enPixelFormat  = PIX_FMT_RGB_444,
        .cName          = "RGB444",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 12 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_555,
        .cName          = "RGB555",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_565,
        .cName          = "RGB565",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_888,
        .cName          = "RGB888",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 32 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_BGR_444,
        .cName          = "BGR444",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 12 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_BGR_555,
        .cName          = "BGR555",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_BGR_565,
        .cName          = "BGR565",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_BGR_888,
        .cName          = "BGR888",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 32 },
        .u8HeightDepth  = { 8 },
    },

    {
        .enPixelFormat  = PIX_FMT_ARGB_1555,
        .cName         = "ARGB1555",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },

    },
    {
        .enPixelFormat  = PIX_FMT_ARGB_4444,
        .cName         = "ARGB4444",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_ARGB_8565,
        .cName         = "ARGB8565",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 32 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_ARGB_8888,
        .cName         = "ARGB8888",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 32 },
        .u8HeightDepth  = { 8 },
    },

    {
        .enPixelFormat  = PIX_FMT_ABGR_1555,
        .cName         = "ABGR1555",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },

    },
    {
        .enPixelFormat  = PIX_FMT_ABGR_4444,
        .cName         = "ABGR4444",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_ABGR_8565,
        .cName         = "ABGR8565",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 32 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_ABGR_8888,
        .cName         = "ABGR8888",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 32 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_8BITS,
        .cName         = "RAW8",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 8 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_10BITS,
        .cName         = "RAW10",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_12BITS,
        .cName         = "RAW12",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_14BITS,
        .cName         = "RAW14",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_16BITS,
        .cName         = "RAW16",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_20BITS,
        .cName         = "RAW20",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 32 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_10BITS_CMP,
        .cName         = "RAW10C",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 10 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_12BITS_CMP,
        .cName         = "RAW12C",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 12 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_RGB_BAYER_14BITS_CMP,
        .cName         = "RAW14C",
        .u8DataPlanes   = 1,
        .u8WidthDepth   = { 14 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YUV_SEMIPLANAR_422,
        .cName         = "YUVSP422",
        .u8DataPlanes   = 2,
        .u8WidthDepth   = { 8, 8 },
        .u8HeightDepth  = { 8, 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YUV_SEMIPLANAR_420,
        .cName         = "YUVSP420",
        .u8DataPlanes   = 2,
        .u8WidthDepth   = { 8, 8 },
        .u8HeightDepth  = { 8, 4 },
    },
    {
        .enPixelFormat  = PIX_FMT_YUV_SEMIPLANAR_444,
        .cName         = "YUVSP444",
        .u8DataPlanes   = 2,
        .u8WidthDepth   = { 8, 16 },
        .u8HeightDepth  = { 8, 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YVU_SEMIPLANAR_422,
        .cName         = "YVUSP422",
        .u8DataPlanes   = 2,
        .u8WidthDepth    = { 8, 8 },
        .u8HeightDepth  = { 8, 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YVU_SEMIPLANAR_420,
        .cName         = "YVUSP420",
        .u8DataPlanes   = 2,
        .u8WidthDepth    = { 8, 8 },
        .u8HeightDepth  = { 8, 4 },
    },
    {
        .enPixelFormat  = PIX_FMT_YVU_SEMIPLANAR_444,
        .cName         = "YVUSP444",
        .u8DataPlanes   = 2,
        .u8WidthDepth    = { 8, 16 },
        .u8HeightDepth  = { 8, 8 },
    },

    {
        .enPixelFormat  = PIX_FMT_YUV_PLANAR_422,
        .cName         = "YUVP422",
        .u8DataPlanes   = 3,
        .u8WidthDepth    = { 8, 4, 4 },
        .u8HeightDepth  = { 8, 8, 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YUV_PLANAR_420,
        .cName         = "YUVP420",
        .u8DataPlanes   = 3,
        .u8WidthDepth    = { 8, 4, 4 },
        .u8HeightDepth  = { 8, 4, 4 },
    },
    {
        .enPixelFormat  = PIX_FMT_YUV_PLANAR_444,
        .cName         = "YUVP444",
        .u8DataPlanes   = 3,
        .u8WidthDepth    = { 8, 8, 8 },
        .u8HeightDepth  = { 8, 8, 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YVU_PLANAR_422,
        .cName         = "YVUP422",
        .u8DataPlanes   = 3,
        .u8WidthDepth    = { 8, 4, 4 },
        .u8HeightDepth  = { 8, 8, 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YVU_PLANAR_420,
        .cName         = "YVUP420",
        .u8DataPlanes   = 3,
        .u8WidthDepth    = { 8, 4, 4 },
        .u8HeightDepth  = { 8, 4, 4 },
    },
    {
        .enPixelFormat  = PIX_FMT_YVU_PLANAR_444,
        .cName         = "YVUP444",
        .u8DataPlanes   = 3,
        .u8WidthDepth    = { 8, 8, 8 },
        .u8HeightDepth  = { 8, 8, 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YUYV_PACKAGE_422,
        .cName         = "YUYV_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YUYV_PACKAGE_422,
        .cName         = "YUYV_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YVYU_PACKAGE_422,
        .cName         = "YVYU_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_UYVY_PACKAGE_422,
        .cName         = "UYVY_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_VYUY_PACKAGE_422,
        .cName         = "VYUY_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YYUV_PACKAGE_422,
        .cName         = "YYUV_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_YYVU_PACKAGE_422,
        .cName         = "YYVU_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },

    {
        .enPixelFormat  = PIX_FMT_UVYY_PACKAGE_422,
        .cName         = "UVYY_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },
    {
        .enPixelFormat  = PIX_FMT_VUYY_PACKAGE_422,
        .cName         = "VUYY_422",
        .u8DataPlanes   = 1,
        .u8WidthDepth    = { 16 },
        .u8HeightDepth  = { 8 },
    },

};

VIDEO_FRAME_FMT_S *VIDEO_GetPicFmt(PIXEL_FORMAT_E enPixelFormat)
{
    VIDEO_FRAME_FMT_S *pstVideoFmt = EI_NULL;
    unsigned int i;

    for (i = 0; i < EI_ARRAY_SIZE(g_stVideoFrameFmt); ++i) {
        pstVideoFmt = &g_stVideoFrameFmt[i];
        if (pstVideoFmt->enPixelFormat == enPixelFormat) {
            return pstVideoFmt;
        }
    }
    return EI_NULL;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
