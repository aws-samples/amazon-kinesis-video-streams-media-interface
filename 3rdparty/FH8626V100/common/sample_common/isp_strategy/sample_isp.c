#include "dsp/fh_vpu_mpi.h"
#include "isp/isp_common.h"
#include "isp/isp_api.h"
#include "isp/isp_enum.h"
#include "sample_common.h"

#if defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)
#include "FHAdv_Isp_mpi_v2.h"
#else
#include "FHAdv_Isp_mpi.h"
#endif

static volatile FH_SINT32 g_stop_isp = 0;
static volatile FH_SINT32 g_isp_runnig = 0;

extern FH_SINT32 g_enc_channel_number;

static FH_VOID SetMirrorAndFlip(FH_VOID)
{
    int time_wait;

    time_wait = 5;

    printf("[ISP_Strategy] set Mirror.\n");
    FHAdv_Isp_SetMirrorAndflip(1, 0);
    sleep(time_wait);
    printf("[ISP_Strategy] set Flip.\n");
    FHAdv_Isp_SetMirrorAndflip(0, 1);
    sleep(time_wait);
    printf("[ISP_Strategy] set Mirror & Flip.\n");
    FHAdv_Isp_SetMirrorAndflip(1, 1);
    sleep(time_wait);
    printf("[ISP_Strategy] reset.\n");
    FHAdv_Isp_SetMirrorAndflip(0, 0);

}

static FH_VOID SetAEMode(FH_VOID)
{
    int time_wait;
    int mode;
    int ae_time_level;
    int gain_level;

    time_wait = 5;

    sleep(time_wait);
    mode          = 1;
    ae_time_level = 8;
    gain_level    = 0;
    FHAdv_Isp_SetAEMode(mode, ae_time_level, gain_level);

    sleep(time_wait);
    mode          = 2;
    ae_time_level = 0;
    gain_level    = 50;
    FHAdv_Isp_SetAEMode(mode, ae_time_level, gain_level);

    sleep(time_wait);
    mode          = 3;
    ae_time_level = 8;
    gain_level    = 50;
    FHAdv_Isp_SetAEMode(mode, ae_time_level, gain_level);

    sleep(time_wait);
    mode          = 0;
    ae_time_level = 0;
    gain_level    = 0;
    FHAdv_Isp_SetAEMode(mode, ae_time_level, gain_level);
}

static FH_VOID SetSharpness(FH_VOID)
{
    int time_wait;
    int mode;
    int value;

    time_wait = 5;

    mode  = 1;
    value = 50;
    FHAdv_Isp_SetSharpeness(mode, value);

    sleep(time_wait);
    mode  = 1;
    value = -50;
    FHAdv_Isp_SetSharpeness(mode, value);

    sleep(time_wait);
    mode  = 0;
    value = 0;
    FHAdv_Isp_SetSharpeness(mode, value);
}

static FH_VOID SetSaturation(FH_VOID)
{
    int i;
    int time_wait;
    int mode;
    int value;

    time_wait = 5;

    for(i = 0; i < 4; i++)
    {
        sleep(time_wait);
        mode  = 1;
        value = 50 * i;
        FHAdv_Isp_SetSaturation(mode, value);
    }

    sleep(time_wait);
    mode  = 0;
    value = 0;
    FHAdv_Isp_SetSaturation(mode, value);
}

static FH_VOID SetChroma(FH_VOID)
{
    return;
}


FH_VOID* sample_isp_thread(FH_VOID *args)
{
    int cmd = 0;
    int k;

    while (!g_stop_isp)
    {
#ifdef FH_APP_ISP_MIRROR_FLIP
        if (cmd == 0)
        {
            SetMirrorAndFlip();
        }
#endif

#ifdef FH_APP_CHANGE_AE_MODE
        if (cmd == 1)
        {
            SetAEMode();
        }
#endif

#ifdef FH_APP_CHANGE_NPMODE
        if (cmd == 2)
        {
            SetSharpness();
        }
#endif

#ifdef FH_APP_CHANGE_SATURATION
        if (cmd == 3)
        {
            SetSaturation();
        }
#endif

#ifdef FH_APP_CHANGE_CHROMA
        if (cmd == 4)
        {
            SetChroma();
        }
#endif

        if (++cmd > 4)
            cmd = 0;

        for (k=0; k<20 && !g_stop_isp; k++)
        {
            usleep(100*1000);
        }
    }

    g_isp_runnig = 0;

    return FH_NULL;
}

FH_SINT32 sample_isp_start(FH_VOID)
{
    FH_SINT32 ret;

    pthread_t isp_thread;
    pthread_attr_t attr;

    if (g_isp_runnig)
    {
        printf("Isp strategy is already running\n");
        return 0;
    }

    ret = FHAdv_Isp_Init();
    if (ret)
    {
        printf("Error: isp_strategy init failed!\n");
        return ret;
    }

    g_isp_runnig = 1;
    g_stop_isp   = 0;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 10 * 1024);

    ret = pthread_create(&isp_thread, &attr, sample_isp_thread, FH_NULL);
    if (ret)
    {
        printf("Error: Create isp_strategy thread failed!\n");
        g_isp_runnig = 0;
    }

    return ret;
}

FH_VOID sample_isp_stop(FH_VOID)
{
    g_stop_isp   = 1;
    while (g_isp_runnig)
    {
        usleep(40 * 1000);
    }
}
