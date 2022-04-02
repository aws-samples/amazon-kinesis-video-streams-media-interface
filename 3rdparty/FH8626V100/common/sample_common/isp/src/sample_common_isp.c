#include "dsp/fh_vpu_mpi.h"
#include "isp/isp_common.h"
#include "isp/isp_api.h"
#include "isp/isp_enum.h"
#include "sample_common.h"
#include "_sensor_.h"
#include "isp_format.h"
#include "sample_smart_ir.h"

static FH_SINT32 g_isp_format      = ISP_FORMAT;
static FH_SINT32 g_isp_init_width  = VI_INPUT_WIDTH;
static FH_SINT32 g_isp_init_height = VI_INPUT_HEIGHT;
#ifdef MAX_ISP_WIDTH
static FH_SINT32 g_isp_max_width   = MAX_ISP_WIDTH > VI_INPUT_WIDTH ? MAX_ISP_WIDTH : VI_INPUT_WIDTH;
static FH_SINT32 g_isp_max_height  = MAX_ISP_HEIGHT > VI_INPUT_HEIGHT ? MAX_ISP_HEIGHT : VI_INPUT_HEIGHT;
#else
static FH_SINT32 g_isp_max_width   = VI_INPUT_WIDTH;
static FH_SINT32 g_isp_max_height  = VI_INPUT_HEIGHT;
#endif

static struct isp_sensor_if *g_sensor;
static pthread_t g_isp_thread;
static volatile int g_isp_stop;
static volatile int g_isp_running;

FH_SINT32 sample_isp_use_large_frame(FH_VOID)
{
    if (g_isp_init_width > 1920 || g_isp_init_height > 1080)
    {
        return 1;
    }

    return 0;
}

FH_UINT32 sample_isp_get_vi_w(FH_VOID)
{
    return g_isp_init_width;
}

FH_UINT32 sample_isp_get_vi_h(FH_VOID)
{
    return g_isp_init_height;
}

FH_UINT32 sample_isp_get_max_w(FH_VOID)
{
    return g_isp_max_width;
}

FH_UINT32 sample_isp_get_max_h(FH_VOID)
{
    return g_isp_max_height;
}

/* 切换ISP幅面制式 */
FH_VOID sample_isp_change_resolution(FH_VOID)
{
    FH_SINT32   ret;
    FH_SINT32   flag     = 0;
    FH_UINT32   target_format;
    FH_UINT32   target_vi_w;
    FH_UINT32   target_vi_h;
    FH_VPU_SIZE vi_pic;
    FH_CHAR    *param;
    FH_SINT32   hex_file_len;

    static FH_SINT32 isp_resolution_changed = 0;

    if (g_isp_max_width < 1920 || g_isp_max_width < 1080)
    {
        printf("Error: origin isp frame should be larger than 1080P\n");
        return;
    }

    if (!isp_resolution_changed)
    {
        target_format = g_isp_format;
        target_vi_w   = sample_isp_get_vi_w();
        target_vi_h   = sample_isp_get_vi_h();
    }
    else
    {
        target_format = FORMAT_1080P25;
        target_vi_w   = 1920;
        target_vi_h   = 1080;
    }

    /* 停止isp */
    ret = API_ISP_Pause();
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): API_ISP_Pause\n", ret, ret);
        return;
    }

    /* 设置isp_format为目标制式 */
    ret = API_ISP_SetSensorFmt(target_format);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): API_ISP_SetSensorFmt\n", ret, ret);
        return;
    }

    if ((target_format >> 16) & 1)
        flag |= SAMPLE_SENSOR_FLAG_WDR;

    /* 获得sensor参数文件位置及大小，并且加载参数 */
    param = get_isp_sensor_param((char *)g_sensor->name, flag, &hex_file_len);
    printf("hex param addr: %p, size: %d\n", param, hex_file_len);
    if (param)
    {
        ret = API_ISP_LoadIspParam(param);
        if (ret)
        {
            printf("Error: API_ISP_LoadIspParam with %d(%x)\n", ret, ret);
        }

        free_isp_sensor_param(param);
    }
    else
    {
        printf("Error: Cann't load sensor hex file!\n");
        return;
    }

    vi_pic.vi_size.u32Width  = target_vi_w;
    vi_pic.vi_size.u32Height = target_vi_h;
    ret = FH_VPSS_SetViAttr(&vi_pic);
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VPSS_SetViAttr\n", ret, ret);
        return;
    }

    /* 恢复isp */
    ret = API_ISP_Resume();
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): API_ISP_Resume!\n", ret, ret);
        return;
    }

    isp_resolution_changed = !isp_resolution_changed;
}

/* 切换ISP帧率 */
FH_VOID sample_isp_change_fps(FH_VOID)
{
    FH_SINT32 ret;
    FH_UINT32 fps = 0;

    switch (g_isp_format)
    {
        case FORMAT_720P25:
            g_isp_format = FORMAT_720P30;
            fps          = 30;
            break;
        case FORMAT_720P30:
            g_isp_format = FORMAT_720P25;
            fps          = 25;
            break;
        case FORMAT_960P25:
            g_isp_format = FORMAT_960P30;
            fps          = 30;
            break;
        case FORMAT_960P30:
            g_isp_format = FORMAT_960P25;
            fps          = 25;
            break;
        case FORMAT_1080P25:
            g_isp_format = FORMAT_1080P30;
            fps          = 30;
            break;
        case FORMAT_1080P30:
            g_isp_format = FORMAT_1080P25;
            fps          = 25;
            break;
        default:
            break;
    }

    if (!fps)
    {
        printf("Keep current fps\n");
        return;
    }

    /* 停止isp */
    API_ISP_Pause();

    /* 去使能vpu */
    ret = FH_VPSS_Disable();
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VPSS_Disable\n", ret, ret);
        return;
    }

    API_ISP_SetSensorFmt(g_isp_format);

    /* 使能vpu */
    ret = FH_VPSS_Enable(VPU_MODE_ISP); /*ISP直通模式,可以节省内存开销*/
    if (ret != RETURN_OK)
    {
        printf("Error(%d - %x): FH_VPSS_Enable\n", ret, ret);
        return;
    }

    /* 恢复isp */
    API_ISP_Resume();

    printf("Change current fps to %d\n\n", fps);
}


FH_VOID isp_vpu_reconfig(FH_VOID)
{
    FH_VPU_SIZE vpu_size;
    ISP_VI_ATTR_S isp_vi;

    FH_VPSS_Reset();
    API_ISP_Pause();
    API_ISP_Resume();

    FH_VPSS_GetViAttr(&vpu_size);
    API_ISP_GetViAttr(&isp_vi);
    if (vpu_size.vi_size.u32Width != isp_vi.u16PicWidth ||
            vpu_size.vi_size.u32Height != isp_vi.u16PicHeight)
    {
        vpu_size.vi_size.u32Width  = isp_vi.u16PicWidth;
        vpu_size.vi_size.u32Height = isp_vi.u16PicHeight;
        FH_VPSS_SetViAttr(&vpu_size);
    }

    API_ISP_SensorKick();
}

FH_SINT32 sample_isp_init(FH_VOID)
{
    FH_SINT32 flag = SAMPLE_SENSOR_FLAG_NORMAL;
    FH_SINT32 ret;
    FH_SINT32 hex_file_len;
    FH_CHAR   *param;
    FH_UINT32 param_addr, param_size;

    ret = API_ISP_MemInit(g_isp_max_width, g_isp_max_height);
    if (ret)
    {
        printf("Error:API_ISP_MemInit failed with %d(%x)\n", ret, ret);
        return ret;
    }

    ret = API_ISP_GetBinAddr(&param_addr, &param_size);
    if (ret)
    {
        printf("Error: API_ISP_GetBinAddr with %d(%x)\n", ret, ret);
        return ret;
    }

    g_sensor = start_sensor();
    if (!g_sensor)
    {
        printf("Error: start_sensor failed\n");
        return -1;
    }

    ret = API_ISP_SensorRegCb(0, g_sensor);
    if (ret)
    {
        printf("Error: API_ISP_SensorRegCb with %d(%x)\n", ret, ret);
        return ret;
    }

    ret = API_ISP_SensorInit();
    if (ret)
    {
        printf("Error: API_ISP_SensorInit with %d(%x)\n", ret, ret);
        return ret;
    }

    ret = API_ISP_SetSensorFmt(g_isp_format);
    if (ret)
    {
        printf("API_ISP_SetSensorFmt failed with %d(%x)\n", ret, ret);
        return ret;
    }

#ifdef FH_USING_EMU_MODE
    ISP_VI_ATTR_S gst_sensor_vi_attr;
    gst_sensor_vi_attr.u16InputHeight = VI_INPUT_HEIGHT;
    gst_sensor_vi_attr.u16InputWidth  = VI_INPUT_WIDTH;
    gst_sensor_vi_attr.u16PicHeight   = VI_INPUT_HEIGHT;
    gst_sensor_vi_attr.u16PicWidth    = VI_INPUT_WIDTH;
    gst_sensor_vi_attr.u16OffsetX     = 0;
    gst_sensor_vi_attr.u16OffsetY     = 0;
    gst_sensor_vi_attr.enBayerType    = BAYER_GRBG;
    API_ISP_SetViAttr(&gst_sensor_vi_attr);
#endif

    ret = API_ISP_Init();
    if (ret)
    {
        printf("API_ISP_Init failed with %d(%x)\n", ret, ret);
        return ret;
    }

    if (strcmp((char *)g_sensor->name, "imx291") == 0)
    {
        ret = API_ISP_SetCisClk((unsigned int)36000000);
        if (ret)
        {
            printf("API_ISP_SetCisClk(CIS_CLK_36M) failed with %d(%x)\n", ret, ret);
            return ret;
        }
    }

    if ((g_isp_format >> 16) & 1)
        flag |= SAMPLE_SENSOR_FLAG_WDR;

    param = get_isp_sensor_param((char *)g_sensor->name, flag, &hex_file_len);
    if (param)
    {
        if (1 /* hex_file_len == param_size */)
        {
            ret = API_ISP_LoadIspParam(param);
            if (ret)
            {
                printf("Error: API_ISP_LoadIspParam failed with %d(%x)\n", ret, ret);
            }
        }
        else
        {
            printf("Error: Invalid sensor hex file length!\n");
        }
        free_isp_sensor_param(param);
    }
    else
    {
        printf("Error: Cann't load sensor hex file!\n");
    }

#ifdef FH_APP_USING_IRCUT
    sample_SmartIR_init(g_sensor->name);
#endif

    API_ISP_SensorKick();

    return 0;
}

FH_VOID *sample_isp_proc(FH_VOID *arg)
{
    FH_SINT32 ret;
    FH_SINT32 err_cnt = 0;

    while (!g_isp_stop)
    {
        ret = API_ISP_Run();
        if (ret)
        {
            err_cnt++;
            ret = API_ISP_DetectPicSize();
            if (ret && err_cnt > 5)
            {
                isp_vpu_reconfig();
                err_cnt = 0;
            }
        }

#ifdef FH_APP_USING_IRCUT
        sample_SmartIR_Ctrl();
#endif

#ifdef FH_APP_OPEN_AF
        sample_af_run();
#endif
    }

    g_isp_running = 0;
    return NULL;
}

FH_SINT32 sample_common_start_isp(FH_VOID)
{
    FH_UINT32 ret;
    pthread_attr_t attr;
    struct sched_param param;

    if (g_isp_running)
    {
        printf("Error: ISP already running!\n");
        return -1;
    }

    if (sample_isp_init() != 0)
    {
        return -1;
    }

    g_isp_stop = 0;
    g_isp_running = 1;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 10 * 1024);
#ifdef __RTTHREAD_OS__
    param.sched_priority = 130;
#endif
    pthread_attr_setschedparam(&attr, &param);

    ret = pthread_create(&g_isp_thread, &attr, sample_isp_proc, NULL);
    if (ret != 0)
    {
        printf("Error: create ISP thread failed!\n");
        g_isp_running = 0;
    }

    return ret;
}

FH_SINT32 sample_common_stop_isp(FH_VOID)
{
    if (g_isp_running)
    {
        g_isp_stop = 1;
        while (g_isp_running)
        {
            usleep(20*1000);
        }

#ifdef FH_APP_USING_IRCUT
        sample_SmartIR_deinit();
#endif
        API_ISP_Exit();
    }

    return 0;
}

static struct dbi_tcp_config g_dbi_tcp_conf;
static volatile int g_coolview_is_running;
static volatile int g_stop_coolview;

FH_SINT32 sample_common_start_coolview(FH_VOID *arg)
{
    FH_SINT32 ret;
    pthread_t thread_dbg;
    pthread_attr_t attr;
    struct sched_param param;

    if (g_coolview_is_running)
    {
        printf("Error: coolview is already running!\n");
        return -1;
    }

    g_stop_coolview = 0;
    g_dbi_tcp_conf.cancel = &g_stop_coolview;
    g_dbi_tcp_conf.port = 8888;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 3 * 1024);
#ifdef __RTTHREAD_OS__
    param.sched_priority = 130;
#endif
    pthread_attr_setschedparam(&attr, &param);
    ret = pthread_create(&thread_dbg, &attr, (void *(*)(void *))libtcp_dbi_thread, &g_dbi_tcp_conf);
    if (!ret)
    {
        g_coolview_is_running = 1;
    }
    else
    {
        printf("Error: create coolview thread failed!\n");
    }

    return ret;
}

FH_SINT32 sample_common_stop_coolview(FH_VOID)
{
    if (g_coolview_is_running)
    {
        /*wait for coolview thread to exit*/
        g_stop_coolview = 1;
        while (g_stop_coolview)
        {
            usleep(20*1000);
        }
        g_coolview_is_running = 0;
    }

    return 0;
}
