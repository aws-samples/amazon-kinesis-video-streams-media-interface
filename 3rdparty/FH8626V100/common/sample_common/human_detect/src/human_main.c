#include "sample_common.h"
#ifdef FH_APP_OPEN_OBJECT_DETECT

#if defined(CONFIG_ARCH_FH8626V100) || \
    defined(CONFIG_ARCH_FH885xV200)

#define BGM_DAY_CONFIDENCE_THRESH   2  /*bgm白天前景阈值*/
#define BGM_NIGHT_CONFIDENCE_THRESH 1  /*bgm夜晚前景阈值*/


extern FH_SINT32 sample_set_gbox(FH_UINT32 chan, FH_UINT32 enable,  FH_UINT32 box_id, FH_UINT32 x, FH_UINT32 y, FH_UINT32 w, FH_UINT32 h, FHT_RgbColor_t color);
static FH_UINT32 current_max_gbox_id = 0;

FH_SINT32 clear_screen(FH_VOID)
{
    FH_UINT32 ret;
    FH_UINT32 i;
    FHT_RgbColor_t color = {0, 0, 0, 0};

    for (i = current_max_gbox_id; i > 0; i --)
    {
        ret = sample_set_gbox(0, 0, i, 0, 0, 0, 0, color);
        if (ret)
        {
            printf("Error: sample_set_gbox %d\n", ret);
            return ret;
        }
    }

    current_max_gbox_id = 0;

    return 0;
}

FH_SINT32 draw_gbox(FH_UINT32 x, FH_UINT32 y, FH_UINT32 w, FH_UINT32 h, FHT_RgbColor_t color)
{
    current_max_gbox_id ++;
    return sample_set_gbox(0, 1, current_max_gbox_id, x, y, w, h, color);
}

#ifdef FH_APP_OFFLINE_HUMAN_DETECT
static FH_SINT32 draw_offline_box(FH_SINT32 dt_x, FH_SINT32 dt_y, FH_SINT32 dt_w, FH_SINT32 dt_h, FH_UINT32 stride, FH_UINT32 line_width, FH_UINT8 *data_buf)
{
    FH_UINT32 i, j;
    FH_UINT8   *line_start;

    for (i = 0; i < line_width; i++)
    {
        line_start = data_buf + stride * (dt_y + i) + dt_x;
        for (j = 0; j < dt_w; j++)
        {
            *line_start ++ = 255;
        }
    }

    for (i = 0; i < dt_h - 2 *line_width; i++)
    {
        line_start = data_buf + stride * (dt_y + line_width + i) + dt_x;
        for (j = 0; j < line_width; j++)
        {
            *line_start ++ = 255;
        }

        line_start += dt_w - 2 * line_width;
        for (j = 0; j < line_width; j++)
        {
            *line_start ++ = 255;
        }
    }

    for (i = 0; i < line_width; i++)
    {
        line_start = data_buf + stride * (dt_y + dt_h - line_width + i) + dt_x;
        for (j = 0; j < dt_w; j++)
        {
            *line_start ++ = 255;
        }
    }

    return 0;
}

FH_SINT32 human_result_process_func(
        FH_UINT32 detect_frame_w,
        FH_UINT32 detect_frame_h,
        FH_UINT32 human_sequence,
        FH_UINT32 model_serial,
        FH_UINT32 human_confidence,
        FH_ODET_objPos_t *p_human_pos,
        FH_UINT8 *y_data)
{
    if (human_sequence == 0)
    {
        return 0;
    }

    return draw_offline_box(
            p_human_pos->x,
            p_human_pos->y,
            p_human_pos->w,
            p_human_pos->h,
            detect_frame_w,
            2,
            y_data);
}

FH_SINT32 face_result_process_func(
        FH_UINT32 detect_frame_w,
        FH_UINT32 detect_frame_h,
        FH_UINT32 face_sequence,
        FH_UINT32 model_serial,
        FH_UINT32 face_confidence,
        FH_ODET_objPos_t *p_face_pos,
        FH_UINT8 *y_data)
{
    return draw_offline_box(
            p_face_pos->x,
            p_face_pos->y,
            p_face_pos->w,
            p_face_pos->h,
            detect_frame_w,
            2,
            y_data);
}

#else
FH_SINT32 human_result_process_func(
        FH_UINT32 detect_frame_w,
        FH_UINT32 detect_frame_h,
        FH_UINT32 human_sequence,
        FH_UINT32 model_serial,
        FH_UINT32 human_confidence,
        FH_ODET_objPos_t *p_human_pos,
        FH_UINT8 *y_data)
{
    FH_SINT32 ret;
    FHT_RgbColor_t color = {255, 0, 0, 255};
    struct channel_info info;

    ret = sample_common_dsp_get_channel_info (0, &info);
    if (ret)
    {
        return -1;
    }

    if (human_sequence == 0)
    {
        return clear_screen();
    }

    return draw_gbox(
            p_human_pos->x * info.width  / detect_frame_w,
            p_human_pos->y * info.height / detect_frame_h,
            p_human_pos->w * info.width  / detect_frame_w,
            p_human_pos->h * info.height / detect_frame_h,
            color);
}

FH_SINT32 face_result_process_func(
        FH_UINT32 detect_frame_w,
        FH_UINT32 detect_frame_h,
        FH_UINT32 face_sequence,
        FH_UINT32 model_serial,
        FH_UINT32 face_confidence,
        FH_ODET_objPos_t *p_face_pos,
        FH_UINT8 *y_data)
{
    FH_SINT32 ret;
    FHT_RgbColor_t color = {0, 255, 0, 255};
    struct channel_info info;

    ret = sample_common_dsp_get_channel_info (0, &info);
    if (ret)
    {
        return -1;
    }

    return draw_gbox(
            p_face_pos->x * info.width  / detect_frame_w,
            p_face_pos->y * info.height / detect_frame_h,
            p_face_pos->w * info.width  / detect_frame_w,
            p_face_pos->h * info.height / detect_frame_h,
            color);
}
#endif
static FH_SINT32 pre_filter_callback(
        FH_UINT32 detect_frame_w,
        FH_UINT32 detect_frame_h,
        FH_UINT32 *px,
        FH_UINT32 *py,
        FH_UINT32 *pw,
        FH_UINT32 *ph)
{
    FH_SINT32 ret = -1;
    FH_SINT32 picW,picH;

#if defined(FH_APP_OBJDETECT_USE_BGM_PRE_FILTER) || defined(FH_APP_OBJDETECT_USE_BGM_AFTER_FILTER)
    sample_common_bgm_triger_reload(); /*here we should get latest BGM data*/
#endif

#if defined(FH_APP_OBJDETECT_USE_MD_PRE_FILTER)
    ret = sample_md_get_outer_motion_area(px, py, pw, ph, &picW, &picH);
#elif defined(FH_APP_OBJDETECT_USE_BGM_PRE_FILTER)
    FH_UINT8 confidence_thresh = BGM_DAY_CONFIDENCE_THRESH;
    ret = sample_common_bgm_get_outer_motion_area(px, py, pw, ph, &picW, &picH, confidence_thresh);
#endif
    if (ret == 0)
    {
        *px = detect_frame_w * (*px) / picW;
        *py = detect_frame_h * (*py) / picH;
        *pw = detect_frame_w * (*pw) / picW;
        *ph = detect_frame_h * (*ph) / picH;
    }

    return ret;
}

static FH_SINT32 post_filter_callback(FH_UINT32 detect_frame_w, FH_UINT32 detect_frame_h, FH_UINT32 x, FH_UINT32 y, FH_UINT32 w, FH_UINT32 h)
{
#if defined(FH_APP_OBJDETECT_USE_BGM_AFTER_FILTER)
    if (1)
    {
        FH_SINT32 ret;
        FH_UINT32 sum;
        FH_UINT32 blk_num;
        FH_UINT8 confidence_thresh = BGM_DAY_CONFIDENCE_THRESH; /*夜晚白天前景阈值*/

        ret = sample_common_bgm_get_statistics_sum(detect_frame_w, detect_frame_h, x, y, w, h, &sum, &blk_num);
        if (ret == 0)
        {
            if (blk_num == 0)
                blk_num = 1; /*ensure don't division by 0*/

            if (sum / blk_num < confidence_thresh)
                return 1; /*filter out it*/
        }
    }
#elif defined(FH_APP_OBJDETECT_USE_MD_AFTER_FILTER)
    if (1)
    {
        FH_SINT32 ret;
        FH_UINT32 sum;
        FH_UINT32 blk_num;
        ret = sample_md_get_statistics_sum(detect_frame_w, detect_frame_h, x, y, w, h, &sum, &blk_num);
        if (ret == 0)
        {
            if (sum < (blk_num / 50 + 1))  /*1/2 blocks has motion, it's OK!*/
                return 1; /*filter out it*/
        }
    }
#endif

    return 0; /*don't filter out, accept it...*/
}

static FH_SINT32 config_human_channel(FH_SINT32 *pchannel, FH_UINT8 *yuv_trans)
{
    FH_SINT32 channel;
    FH_UINT8  trans = 0;

    channel = sample_common_dsp_get_max_channel_index();
    if (channel < 0)
    {
        printf("Error@Human, sample_common_dsp_get_max_channel_index!\n");
        return -1;
    }

#ifndef CONFIG_ARCH_FH8626V100
    FH_SINT32 ret;
    struct channel_info info;

    ret = sample_common_dsp_get_channel_info(channel, &info);
    if (ret != 0 || !info.enable)
    {
        printf("Error@Human, sample_common_dsp_get_channel_info,channel=%d,enable=%d!\n", channel, info.enable);
        return -1;
    }

    if (info.enc_type == 0)
    {
        if (!FH_VPSS_SetVOMode(channel, VPU_VOMODE_SCAN))
        {
            printf("HUMAN: config to scan mode.\n");
            info.yuv_type = VPU_VOMODE_SCAN;
        }
    }

    if (info.yuv_type != VPU_VOMODE_SCAN && info.yuv_type != VPU_VOMODE_BLK)
    {
        printf("Error@Human, wrong yuv format for human detect as input data\n");
        return -1;
    }

    if (info.yuv_type == VPU_VOMODE_BLK)
        trans = 1;
#endif

    *yuv_trans = trans;
    *pchannel  = channel;
    return 0;
}

FH_SINT32 sample_human_detect_start(FH_VOID)
{
    FH_SINT32 ret;
    struct human_detect_config detcfg;

    memset(&detcfg, 0, sizeof(detcfg));

    ret = config_human_channel((FH_SINT32 *)(&detcfg.human_detect_chan), &detcfg.do_yuv_trans);
    if (ret != 0)
        return ret;

#if defined(FH_APP_OBJDETECT_USE_BGM_PRE_FILTER) || \
    defined(FH_APP_OBJDETECT_USE_BGM_AFTER_FILTER)
    sample_common_bgm_init();
#endif

    detcfg.do_human_detect = 1;

#if	defined(FH_APP_OBJDETECT_DO_FACE_DETECT)
    detcfg.do_face_detect = 1;
    detcfg.init_face_detect = 1;
#endif

#if defined(FH_APP_OBJDETECT_USE_BGM_PRE_FILTER) || \
    defined(FH_APP_OBJDETECT_USE_BGM_AFTER_FILTER) || \
    defined(FH_APP_OBJDETECT_USE_MD_PRE_FILTER)
    detcfg.pre_filter_func = pre_filter_callback;
#endif

#if defined(FH_APP_OBJDETECT_USE_BGM_AFTER_FILTER) || \
    defined(FH_APP_OBJDETECT_USE_MD_AFTER_FILTER)
    detcfg.post_filter_func = post_filter_callback;
#endif

#ifdef FH_APP_OBJDETECT_DO_POST_PROCESS
    detcfg.process_human_func = human_result_process_func;
    detcfg.process_face_func  = face_result_process_func;
#endif

    return human_detect_start(&detcfg);
}

FH_SINT32 sample_human_detect_stop(FH_VOID)
{
    return human_detect_stop();
}

#endif /*defined(CONFIG_ARCH_FH8626V100) ...*/

#endif /*FH_APP_OPEN_OBJECT_DETECT*/
