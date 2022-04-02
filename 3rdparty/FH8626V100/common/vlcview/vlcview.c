/*
 * File      : sample_vlcview.c
 * This file is part of SOCs BSP for RT-Thread distribution.
 *
 * Copyright (c) 2017 Shanghai Fullhan Microelectronics Co., Ltd.
 * All rights reserved
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  Visit http://www.fullhan.com to get contact with Fullhan.
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include <sample_common.h>
#include "isp_api.h"
#include "isp/isp_enum.h"
#include "vlcview.h"

static FH_SINT32 g_sample_running = 0;

#ifndef FH_FAST_BOOT
FH_SINT32 g_enc_channel_number = 0; /*for encoder, we start from logic channel 0...*/

FH_SINT32 _vlcview_exit(FH_VOID)
{
    if (!g_sample_running)
    {
        printf("vlcview is not running!\n");
        return 0;
    }

#ifdef FH_APP_OPEN_IVS
    sample_ivs_stop();
#endif

#ifdef FH_APP_OPEN_AF
    sample_af_stop();
#endif

#ifdef FH_APP_OPEN_VENC
    sample_venc_stop();
#endif

#ifdef FH_APP_OPEN_OBJECT_DETECT
    sample_human_detect_stop();
#endif

#ifdef FH_APP_OPEN_NN_OBJECT_DETECT
    sample_nn_human_detect_stop();
#endif

#if defined(FH_APP_OPEN_MOTION_DETECT) || \
    defined(FH_APP_OPEN_RECT_MOTION_DETECT) || \
    defined(FH_APP_OPEN_COVER_DETECT)
    sample_md_cd_stop();
#endif

#ifdef FH_APP_OPEN_NN_DETECT
    sample_fh_nn_obj_detect_stop();
#endif

#ifdef FH_APP_OPEN_OVERLAY
    sample_overlay_stop();
#endif

#ifdef FH_APP_USING_COOLVIEW
    sample_common_stop_coolview();
#endif

#ifdef FH_APP_OPEN_ISP_STRATEGY_DEMO
    sample_isp_stop();
#endif

    sample_common_stop_stream_proc();
    sample_common_bgm_exit();
    sample_dmc_deinit();
    sample_common_stop_isp();
    sample_common_media_sys_exit();

    g_sample_running = 0;

    return 0;
}

FH_SINT32 _vlcview(FH_CHAR *dst_ip, FH_UINT32 port)
{
    FH_SINT32 ret;
    FH_SINT32 chan_vpu;
    FH_SINT32 use_bgm = 0;
    struct channel_info info;
    FH_SINT32 channel_max = sample_common_dsp_get_max_channel_index();

    if (g_sample_running)
    {
        printf("vlcview is running!\n");
        return 0;
    }

    g_sample_running = 1;

    /******************************************
      step  1: do media configuration
     ******************************************/
    sample_common_media_driver_config();

    /******************************************
      step  2: init media platform
     ******************************************/
    ret = sample_common_media_sys_init();
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_media_sys_init!\n", ret, ret);
        goto err_exit;
    }

    /******************************************
      step  3: init vpu vi and open vpu
     ******************************************/
    ret = sample_common_dsp_init();
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_dsp_init!\n", ret, ret);
        goto err_exit;
    }

    /******************************************
      step  4: configure vpss channels
     ******************************************/
    chan_vpu = 0; /*for vpu, we start from physical channel 0...*/
    g_enc_channel_number = 0; /*for encoder, we start from logic channel 0...*/
    for ( ; chan_vpu <= channel_max; chan_vpu++, g_enc_channel_number++)
    {
        ret = sample_common_dsp_get_channel_info (chan_vpu, &info);
        if (ret != 0 || !info.enable)
        {
            continue;
        }

        /******************************************
          step  4: create vpss channel
         ******************************************/
        ret = sample_common_vpu_create_chan(chan_vpu, info.width, info.height, info.yuv_type);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_vpu_create_chan!\n", ret, ret);
            goto err_exit;
        }

        if (!info.enc_type)
        {
            continue;
        }

        /******************************************
          step  6: create venc channel
         ******************************************/
        ret = sample_common_enc_create_chan(g_enc_channel_number, &info);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_enc_create_chan!\n", ret, ret);
            goto err_exit;
        }

        /*****************************************
          step  7: start venc channel
         *****************************************/
        ret = sample_common_enc_set_chan(g_enc_channel_number, &info);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_enc_set_chan!\n", ret, ret);
            goto err_exit;
        }

        /*****************************************
          step  8: bind vpss channel and venc channel
         *****************************************/
        ret = sample_common_vpu_bind_to_enc(chan_vpu, g_enc_channel_number);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_vpu_bind_to_enc!\n", ret, ret);
            goto err_exit;
        }

        if (info.enc_type == FH_SMART_H264 || info.enc_type == FH_SMART_H265)
        {
            use_bgm = 1;
        }
    }

#ifdef CH2_SPLIT_AFTER_GOSD
    ret = FH_VPSS_SetChn2LogoSel(1);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VPSS_SetChn2LogoSel!\n", ret, ret);
        goto err_exit;
    }
#endif

    if (use_bgm)
    {
        /************************************************
          step  9: if use smart encode type, open bgm
         ************************************************/
        ret = sample_common_bgm_init();
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_bgm_init!\n", ret, ret);
            goto err_exit;
        }
    }

    /************************************************
      step  10: start isp
     ************************************************/
    ret = sample_common_start_isp();
    if (ret != 0)
    {
        printf("Error(%d - %x): sample_common_start_isp!\n", ret, ret);
        goto err_exit;
    }

    /************************************************
      step  11: init data-manage-center which is used to dispatch stream...
     ************************************************/
    sample_dmc_init(dst_ip, port, channel_max + 1);

    /******************************************
      step  12: get stream, then send to data manager..
     ******************************************/
    sample_common_start_get_stream();


#ifdef FH_APP_USING_COOLVIEW
    /******************************************
      step  13: open coolview
     ******************************************/
    sample_common_start_coolview(NULL);
#endif

#ifdef FH_APP_OPEN_OVERLAY
    /******************************************
      step  14: add overlayer
     ******************************************/
    sample_overlay_start();
#endif

#if defined(FH_APP_OPEN_MOTION_DETECT) || \
    defined(FH_APP_OPEN_RECT_MOTION_DETECT) || \
    defined(FH_APP_OPEN_COVER_DETECT)
    /******************************************
      step  17: do motion detect and cover detect
     ******************************************/
    sample_md_cd_start();
#endif


#ifdef FH_APP_OPEN_OBJECT_DETECT
    /******************************************
      step  15: do traditional human detect
     ******************************************/
    sample_human_detect_start();
#endif

#ifdef FH_APP_OPEN_NN_OBJECT_DETECT
    /******************************************
      step  16: do nn human detect (Support by YG)
     ******************************************/
    sample_nn_human_detect_start();
#endif

#ifdef FH_APP_OPEN_NN_DETECT
    /******************************************
      step  17: do nn human detect (Support by YG)
     ******************************************/
    sample_fh_nn_obj_detect_start();
#endif


#ifdef FH_APP_OPEN_VENC
    /******************************************
      step  18: open venc functions
     ******************************************/
    sample_venc_start();
#endif

#ifdef FH_APP_OPEN_ISP_STRATEGY_DEMO
    /******************************************
      step  18: open isp strategy functions
     ******************************************/
    sample_isp_start();
#endif

#ifdef FH_APP_OPEN_AF
    sample_af_start();
#endif

#ifdef FH_APP_OPEN_IVS
    sample_ivs_start();
#endif

    return 0;

err_exit:
    _vlcview_exit();
    return -1;
}
#else

FH_SINT32 _vlcview_exit(FH_VOID)
{
    if (!g_sample_running)
    {
        printf("vlcview is not running!\n");
        return 0;
    }

    sample_common_stop_stream_proc();

    sample_dmc_deinit();

    g_sample_running = 0;

    return 0;
}

FH_SINT32 _vlcview(FH_CHAR *dst_ip, FH_UINT32 port)
{
    if (g_sample_running)
    {
        printf("vlcview is running!\n");
        return 0;
    }

    g_sample_running = 1;

    /************************************************
      step  1: init data-manage-center which is used to dispatch stream...
     ************************************************/
    sample_dmc_init(dst_ip, port, 1);

    /******************************************
      step  2: get stream, then send to data manager..
     ******************************************/
    sample_common_start_get_stream();

    return 0;
}
#endif


FH_SINT32 sample_video_exit(FH_VOID)
{
    if (!g_sample_running)
    {
        printf("vlcview is not running!\n");
        return 0;
    }

#ifdef FH_APP_OPEN_IVS
    sample_ivs_stop();
#endif

#ifdef FH_APP_OPEN_AF
    sample_af_stop();
#endif

#ifdef FH_APP_OPEN_VENC
    sample_venc_stop();
#endif

#ifdef FH_APP_OPEN_OBJECT_DETECT
    sample_human_detect_stop();
#endif

#ifdef FH_APP_OPEN_NN_OBJECT_DETECT
    sample_nn_human_detect_stop();
#endif

#if defined(FH_APP_OPEN_MOTION_DETECT) || \
    defined(FH_APP_OPEN_RECT_MOTION_DETECT) || \
    defined(FH_APP_OPEN_COVER_DETECT)
    sample_md_cd_stop();
#endif

#ifdef FH_APP_OPEN_NN_DETECT
    sample_fh_nn_obj_detect_stop();
#endif

#ifdef FH_APP_OPEN_OVERLAY
    sample_overlay_stop();
#endif

#ifdef FH_APP_USING_COOLVIEW
    sample_common_stop_coolview();
#endif

#ifdef FH_APP_OPEN_ISP_STRATEGY_DEMO
    sample_isp_stop();
#endif

    sample_common_stop_stream_proc();
    sample_common_bgm_exit();
    sample_dmc_deinit();
    sample_common_stop_isp();
    sample_common_media_sys_exit();

    g_sample_running = 0;

    return 0;
}

#define IP_ADDR     "192.168.70.12"

FH_SINT32 sample_video_init(void)
{
    FH_SINT32 ret;
    FH_SINT32 chan_vpu;
    FH_SINT32 use_bgm = 0;
    struct channel_info info;
    FH_SINT32 channel_max = sample_common_dsp_get_max_channel_index();

    int port = 1234;

    if (g_sample_running)
    {
        printf("vlcview is running!\n");
        return 0;
    }

    g_sample_running = 1;

    /******************************************
      step  1: do media configuration
     ******************************************/
    sample_common_media_driver_config();

    /******************************************
      step  2: init media platform
     ******************************************/
    ret = sample_common_media_sys_init();
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_media_sys_init!\n", ret, ret);
        goto err_exit;
    }

    /******************************************
      step  3: init vpu vi and open vpu
     ******************************************/
    ret = sample_common_dsp_init();
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_dsp_init!\n", ret, ret);
        goto err_exit;
    }

    /******************************************
      step  4: configure vpss channels
     ******************************************/
    chan_vpu = 0; /*for vpu, we start from physical channel 0...*/
    g_enc_channel_number = 0; /*for encoder, we start from logic channel 0...*/
    for ( ; chan_vpu <= channel_max; chan_vpu++, g_enc_channel_number++)
    {
        ret = sample_common_dsp_get_channel_info (chan_vpu, &info);
        if (ret != 0 || !info.enable)
        {
            continue;
        }

        /******************************************
          step  4: create vpss channel
         ******************************************/
        ret = sample_common_vpu_create_chan(chan_vpu, info.width, info.height, info.yuv_type);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_vpu_create_chan!\n", ret, ret);
            goto err_exit;
        }

        if (!info.enc_type)
        {
            continue;
        }

        /******************************************
          step  6: create venc channel
         ******************************************/
        ret = sample_common_enc_create_chan(g_enc_channel_number, &info);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_enc_create_chan!\n", ret, ret);
            goto err_exit;
        }

        /*****************************************
          step  7: start venc channel
         *****************************************/
        ret = sample_common_enc_set_chan(g_enc_channel_number, &info);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_enc_set_chan!\n", ret, ret);
            goto err_exit;
        }

        /*****************************************
          step  8: bind vpss channel and venc channel
         *****************************************/
        ret = sample_common_vpu_bind_to_enc(chan_vpu, g_enc_channel_number);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_vpu_bind_to_enc!\n", ret, ret);
            goto err_exit;
        }

        if (info.enc_type == FH_SMART_H264 || info.enc_type == FH_SMART_H265)
        {
            use_bgm = 1;
        }
    }

#ifdef CH2_SPLIT_AFTER_GOSD
    ret = FH_VPSS_SetChn2LogoSel(1);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VPSS_SetChn2LogoSel!\n", ret, ret);
        goto err_exit;
    }
#endif

    if (use_bgm)
    {
        /************************************************
          step  9: if use smart encode type, open bgm
         ************************************************/
        ret = sample_common_bgm_init();
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_bgm_init!\n", ret, ret);
            goto err_exit;
        }
    }

    /************************************************
      step  10: start isp
     ************************************************/
    ret = sample_common_start_isp();
    if (ret != 0)
    {
        printf("Error(%d - %x): sample_common_start_isp!\n", ret, ret);
        goto err_exit;
    }

    /************************************************
      step  11: init data-manage-center which is used to dispatch stream...
     ************************************************/
    sample_dmc_init(IP_ADDR, port, channel_max + 1);

    /******************************************
      step  12: get stream, then send to data manager..
     ******************************************/
#ifdef FH_APP_USING_RTSP
    sample_common_start_get_stream();
#endif

#ifdef FH_APP_USING_COOLVIEW
    /******************************************
      step  13: open coolview
     ******************************************/
    sample_common_start_coolview(NULL);
#endif

#ifdef FH_APP_OPEN_OVERLAY
    /******************************************
      step  14: add overlayer
     ******************************************/
    sample_overlay_start();
#endif

#if defined(FH_APP_OPEN_MOTION_DETECT) || \
    defined(FH_APP_OPEN_RECT_MOTION_DETECT) || \
    defined(FH_APP_OPEN_COVER_DETECT)
    /******************************************
      step  17: do motion detect and cover detect
     ******************************************/
    sample_md_cd_start();
#endif

#ifdef FH_APP_OPEN_VENC
    /******************************************
      step  18: open venc functions
     ******************************************/
    sample_venc_start();
#endif

#ifdef FH_APP_OPEN_ISP_STRATEGY_DEMO
    /******************************************
      step  18: open isp strategy functions
     ******************************************/
    sample_isp_start();
#endif

#ifdef FH_APP_OPEN_IVS
    sample_ivs_start();
#endif

#ifdef FH_APP_USING_RTSP
    while (1)
    {
        printf("rtsp loop\n");
        sleep(1);
    }
#endif // FH_APP_USING_RTSP

    return 0;

err_exit:
    _vlcview_exit();
    return -1;
}

static void ClearChnData(FH_UINT32 request_type, int giveupNum)
{
    FH_VENC_STREAM stream;
    int max_time = 1000, waitcnt = 0;
    int getNum = 0;
    int i;
    int  s32Ret;

    if (request_type != FH_STREAM_H264)
        return;

    for (i = 0; i < max_time; i++)
    {
        s32Ret = FH_VENC_GetStream(request_type, &stream);
        if (s32Ret == RETURN_OK)
        {
            FH_VENC_ReleaseStream(stream.mjpeg_stream.chan);

            getNum++;
            waitcnt = 0;
        }
        else
        {
            if (waitcnt++ >= 2 && (getNum >= giveupNum))
                break;
            usleep(1000*5);
        }
    }
}

FH_SINT32 change_sensor(FH_UINT32 format)
{
    int ret;

    API_ISP_Pause();
    FH_VPSS_Disable();

    ret = API_ISP_SetSensorFmt(format); /* can not run in isr */
    if (ret != RETURN_OK)
    {
        printf("Error: API_ISP_SetSensorFmt failed with %d\n", ret);
        return 1;
    }

    FH_VPSS_Enable(VPU_MODE_ISP);
    API_ISP_Resume();

    return 0;
}

FH_SINT32 change_video(FH_UINT32 chan, FH_UINT32 format, FH_UINT32 res)
{
    int ret = 0;

    if (change_sensor(res))
        return 1;

    ret |= FH_VENC_StopRecvPic(chan);
    ret |= FH_SYS_UnBindbyDst(chan);
    ret |= FH_VPSS_CloseChn(chan);
    if (ret != RETURN_OK)
    {
        printf("Error: FH_VPSS_CloseChn failed with %d\n", ret);
        goto test_exit;
    }

    ClearChnData(format, 0);

    struct channel_info info;

    switch (res)
    {
    case FORMAT_1080P15:
        info.width  = 1920;
        info.height = 1080;
        break;
    case FORMAT_720P15:
        info.width  = 1280;
        info.height = 720;
        break;
    default:
        break;
    }

    info.yuv_type = VPU_VOMODE_SCAN;

    ret = sample_common_vpu_create_chan(chan, info.width, info.height, info.yuv_type);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_vpu_create_chan!\n", ret, ret);
        goto test_exit;
    }
    
    info.enc_type = format;

    ret = sample_common_enc_set_chan(chan, &info);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_enc_set_chan!\n", ret, ret);
        goto test_exit;
    }

    ret = sample_common_vpu_bind_to_enc(chan, chan);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_vpu_bind_to_enc!\n", ret, ret);
        goto test_exit;
    }

    return RETURN_OK;

test_exit:
    printf("change_video err!\n");
    return -1;
}
