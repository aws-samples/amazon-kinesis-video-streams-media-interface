#include "dsp/fh_vpu_mpi.h"
#include "isp/isp_common.h"
#include "isp/isp_api.h"
#include "isp/isp_enum.h"
#include "sample_common.h"

#ifndef FH_APP_JPEG_CHANNEL
#define FH_APP_JPEG_CHANNEL 0
#endif

static volatile FH_SINT32 g_stop_venc = 0;
static volatile FH_SINT32 g_venc_runnig = 0;
static FH_SINT32 g_jpeg_channel_inited = 0;

extern FH_SINT32 g_enc_channel_number;

/* 切换主码流分辨率 */
static FH_VOID change_dsp_resolution(FH_VOID)
{
    static FH_SINT32 resolution_changed = 0;
    FH_SINT32 ret;
    FH_UINT32 vpu_channel = 0;
    FH_UINT32 enc_channel = 0;
    struct channel_info target_channel_info;

    /* 获取目标分辨率 */
    ret = sample_common_dsp_get_channel_info(vpu_channel, &target_channel_info);
    if (ret != 0)
    {
        printf("Error: VPU CHANNEL %d is not enabled\n", vpu_channel);
        return;
    }

    if (!resolution_changed)
    {
        target_channel_info.width = ALIGNTO(target_channel_info.width * 4 / 5, 16);
        target_channel_info.height = ALIGNTO(target_channel_info.height * 4 / 5, 16);
    }


    printf("Change resolution to %d x %d\n\n", target_channel_info.width, target_channel_info.height);

    if (sample_common_bgm_get_init_status())
    {
        ret = FH_BGM_Disable();
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): FH_BGM_Disable\n", ret, ret);
            return;
        }
    }

    /* Close GOSD MASK and GOSD GBOX */

    /* 关闭主码流VPU通道 */
    ret = FH_VPSS_CloseChn(vpu_channel);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VPSS_CloseChn\n", ret, ret);
        return;
    }

    if (target_channel_info.enc_type)
    {
        /* 关闭主码流编码通道 */
        ret = FH_VENC_StopRecvPic(enc_channel);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): FH_VENC_StopRecvPic\n", ret, ret);
            return;
        }
    }

    /* 按照目标分辨率重新创建主码流VPU通道 */
    ret = sample_common_vpu_create_chan(vpu_channel, target_channel_info.width, target_channel_info.height, target_channel_info.yuv_type);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_vpu_create_chan\n", ret, ret);
        return;
    }

    /* 按照目标分辨率重新创建主码流编码通道 */
    ret = sample_common_enc_set_chan(enc_channel, &target_channel_info);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): sample_common_enc_set_chan\n", ret, ret);
        return;
    }

    if (target_channel_info.enc_type)
    {
        /* 绑定主码流vpu通道和编码通道 */
        ret = sample_common_vpu_bind_to_enc(vpu_channel, enc_channel);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): sample_common_vpu_bind_to_enc\n", ret, ret);
            return;
        }
    }

    if (sample_common_bgm_get_init_status())
    {
        FH_SIZE bgm_size;

        bgm_size.u32Width =  target_channel_info.width / 8;
        bgm_size.u32Height =  target_channel_info.height / 8;
        ret = FH_BGM_SetConfig(&bgm_size);
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): FH_BGM_SetConfig\n", ret, ret);
            return;
        }

        ret = FH_BGM_Enable();
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): FH_BGM_Enable\n", ret, ret);
            return;
        }

        ret = FH_SYS_BindVpu2Bgm();
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): FH_SYS_BindVpu2Bgm\n", ret, ret);
            return;
        }
    }

    /* Reset GOSD MASK and GOSD GBOX */

    resolution_changed = !resolution_changed;

}

/* 切换ISP帧率 */
static FH_VOID change_isp_fps(FH_VOID)
{
    sample_isp_change_fps();
}

/* 切换ISP帧率 */
static FH_VOID change_isp_resolution(FH_VOID)
{
    sample_isp_change_resolution();
}

/* 旋转编码通道 */
static FH_VOID rotate(FH_VOID)
{
#if defined(CONFIG_ARCH_FH8626V100)
    /* ZT不支持编码旋转 */
    printf("FH8626V100 do not support encode rotation!\n\n");
#else

    FH_SINT32 ret;
    FH_SINT32 channel;

    static FH_UINT32 rotate_value = 0;
    FH_ROTATE_OPS rotate_info;

    channel      = 0;
    rotate_value = (rotate_value + 1) % 4;

    /* 停止编码通道 */
    ret = FH_VENC_StopRecvPic(channel);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VENC_StopRecvPic\n", ret, ret);
        return;
    }

    /* 设置编码旋转角度 */
    rotate_info = rotate_value;

    ret = FH_VPSS_SetVORotate(channel, rotate_info); /* yuv vofmt为tile时，需要该设置 */
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VPSS_SetVORotate\n", ret, ret);
    }

    ret = FH_VENC_SetRotate(channel, rotate_info);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VENC_SetRotate\n", ret, ret);
    }

    /* 开始编码 */
    ret = FH_VENC_StartRecvPic(channel);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VENC_StartRecvPic\n", ret, ret);
    }

    printf("Rotate to %d degree\n\n", rotate_value * 90);
#endif /* CONFIG_ARCH_FH8626V100 LINE204*/
}

/* jpeg抓图 */
static FH_VOID capture_jpeg(FH_VOID)
{
    int ret;
    FH_VENC_STREAM stream;
    struct channel_info info;
    FH_UINT32 chan;
    FH_UINT32 len;
    FH_SINT32 vpu_channel = FH_APP_JPEG_CHANNEL;
    FH_SINT32 jpeg_channel = g_enc_channel_number;

    if (!g_jpeg_channel_inited)
    {
        /* 第一次调用需要创建jpeg编码通道 */
        ret = sample_common_dsp_get_channel_info(vpu_channel, &info);
        if (!(ret == 0 && info.enable))
        {
            printf("Error: we use channel 0 for JPEG, Please enable channel 0 firstly!\n");
            return;
        }

        info.enc_type = FH_JPEG;
        ret = sample_common_enc_create_chan(jpeg_channel, &info);
        if (ret)
        {
            printf("Error: create jpeg channel failed,ret=%d!\n", ret);
            return;
        }

        ret = sample_common_enc_set_chan(jpeg_channel, &info);
        if (ret)
        {
            printf("Error: set jpeg channel failed,ret=%d!\n", ret);
            return;
        }

        g_jpeg_channel_inited = 1;
    }

    /* jpeg编码通道需要每次都绑定 */
    ret = FH_SYS_BindVpu2Enc(vpu_channel, jpeg_channel);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_SYS_BindVpu2Enc\n", ret, ret);
        return;
    }

    /* 获取jpeg数据 */
    ret = FH_VENC_GetStream_Block(FH_STREAM_JPEG, &stream);
    if (ret == RETURN_OK)
    {
        chan = stream.jpeg_stream.chan;
        /*start = stream.jpeg_stream.start;*/
        len = stream.jpeg_stream.length;
        FH_VENC_ReleaseStream(chan);
        printf("Capture JPEG OK, size=%d\n\n", len);
    }
    else
    {
        printf("Error(%d - %x): GetSteam failed for JPEG!\n", ret, ret);
    }
}

/* 切换冻屏状态 */
static FH_VOID toggle_freeze(FH_VOID)
{
    static FH_SINT32 on = 0;
    FH_SINT32 ret;

    on = !on;
    if (on)
    {
        printf("Freeze Video\n\n");
        ret = FH_VPSS_FreezeVideo();
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): FH_VPSS_FreezeVideo\n", ret, ret);
        }
    }
    else
    {
        printf("Unfreeze Video\n\n");
        ret = FH_VPSS_UnfreezeVideo();
        if (ret != RETURN_OK)
        {
            printf("Error(%d - %x): FH_VPSS_UnfreezeVideo\n", ret, ret);
        }
    }
}

/* 获取VPU YUV数据 */
static FH_VOID capture_yuv(FH_VOID)
{
    FH_VPU_STREAM yuv_data;
    FH_UINT8 *dst, *in_buf;
    FH_SINT32 ret;
    FH_SINT32 chan;
    struct channel_info info;
    FH_UINT32 w, h;
    FH_SINT32 channel_max = sample_common_dsp_get_max_channel_index();

    /* 获取VPU通道参数 */
    for(chan = 0; chan <= channel_max; chan++)
    {
        ret = sample_common_dsp_get_channel_info(chan, &info);
        if (ret == 0 && info.enable)
            break;
    }

    if (chan > channel_max)
    {
        printf("Error: no vpu channel is enabled\n");
        return;
    }

    w = (info.width + 15)&(~15);
    h = (info.height + 15)&(~15);

    in_buf = FH_NULL;
    dst    = FH_NULL;

    /* 分配yuv格式转换源数据空间 */
    in_buf = malloc(w * h * 3 / 2);
    if (in_buf == FH_NULL)
    {
        printf("Error: failed to malloc yuv transform input buffer\n");
        return;
    }

    /* 分配yuv格式转换目标数据空间 */
    dst = malloc(w * h * 3 / 2);
    if (dst == FH_NULL)
    {
        printf("Error: failed to allocate yuv transform buffer\n");
        free(in_buf);
        return;
    }

    /* 获取一帧YUV数据 */
    ret = FH_VPSS_GetChnFrame(chan, &yuv_data);
    if (ret == RETURN_OK)
    {
        if (info.enc_type == FH_NORMAL_H265 || info.enc_type == FH_SMART_H265)
        {
            memcpy(in_buf, yuv_data.yluma.vbase, w * h);
            memcpy(in_buf + w * h, yuv_data.chroma.vbase, w * h / 2);
            yuv_transform(dst, in_buf, in_buf + w * h, w, h, w);
        }
        else
        {
#if defined(CONFIG_ARCH_FH8626V100)  /* to do: apply HL */
            /* FH8626V100 VPU只支持光栅格式输出 */
            memcpy(dst, yuv_data.yluma.vbase, w * h);
            memcpy(dst + w * h, yuv_data.chroma.vbase, w * h / 2);
#else
            /* YUV数据格式为宏块，将YUV格式转换为光栅格式 */
            memcpy(in_buf, yuv_data.yluma.vbase, w * h);
            memcpy(in_buf + w * h, yuv_data.chroma.vbase, w * h / 2);
            yuv_transform(dst, in_buf, in_buf + w * h, w, h, w);
#endif
        }

        printf("GET CHN %d YUV DATA w:%d h:%d OK\n\n", chan, w, h);
    }
    else
    {
        printf("Error(%d - %x): FH_VPSS_GetChnFrame %d\n\n", ret, ret, chan);
    }

    free(dst);
    free(in_buf);
}

FH_VOID* sample_venc_thread(FH_VOID *args)
{
    int cmd = 0;
    int k;

    while (!g_stop_venc)
    {
#ifdef FH_APP_CHANGE_DSP_RESOLUTION
        if (cmd == 0)
        {
#if defined(FH_APP_OPEN_OBJECT_DETECT) || defined(FH_APP_OPEN_NN_OBJECT_DETECT)
            printf("Warning: Change resolution may cause problems when object detect is open\n");
#else
            change_dsp_resolution();
#endif
        }
#endif

#ifdef FH_APP_CHANGE_FPS
        if (cmd == 1)
        {
            change_isp_fps();
        }
#endif

#ifdef FH_APP_CHANGE_ISP_RESOLUTION
        if (cmd == 2)
        {
#if defined(FH_APP_OPEN_OBJECT_DETECT) || defined(FH_APP_OPEN_NN_OBJECT_DETECT)
            printf("Warning: Change resolution may cause problems when object detect is open\n");
#else
            change_isp_resolution();
#endif
        }
#endif

#ifdef FH_APP_ROTATE
        if (cmd == 3)
        {
#if defined(FH_APP_OPEN_OBJECT_DETECT) || defined(FH_APP_OPEN_NN_OBJECT_DETECT)
            printf("Warning: Rotating screen may cause problems when object detect is open\n");
#else
            rotate();
#endif
        }
#endif

#ifdef FH_APP_CAPTURE_JPEG
        if (cmd == 4)
        {
            capture_jpeg();
        }
#endif

#ifdef FH_APP_TOOGLE_FREEZE
        if (cmd == 5)
        {
            toggle_freeze();
        }
#endif

#ifdef FH_APP_CAPTURE_YUV
        if (cmd == 6)
        {
            capture_yuv();
        }
#endif

        if (++cmd > 6)
            cmd = 0;

        for (k=0; k<20 && !g_stop_venc; k++)
        {
            usleep(100*1000);
        }
    }

    g_venc_runnig = 0;

    return FH_NULL;
}

FH_SINT32 sample_venc_start(FH_VOID)
{
    FH_SINT32 ret;

    pthread_t venc_thread;
    pthread_attr_t attr;

    if (g_venc_runnig)
    {
        printf("Venc is already running\n");
        return 0;
    }

    g_venc_runnig = 1;
    g_stop_venc   = 0;
    g_jpeg_channel_inited = 0;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 10 * 1024);
#ifdef __RTTHREAD_OS__
    struct sched_param param;
    param.sched_priority = 130;
    pthread_attr_setschedparam(&attr, &param);
#endif
    ret = pthread_create(&venc_thread, &attr, sample_venc_thread, FH_NULL);
    if (ret)
    {
        printf("Error: Create Venc thread failed!\n");
        g_venc_runnig = 0;
    }

    return ret;
}

FH_VOID sample_venc_stop(FH_VOID)
{
    g_stop_venc   = 1;
    while (g_venc_runnig)
    {
        usleep(40 * 1000);
    }
}
