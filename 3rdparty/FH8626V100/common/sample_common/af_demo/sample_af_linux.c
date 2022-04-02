#ifdef __LINUX_OS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "fh_af_ctrl.h"

void *af_run(void *args)
{
    int cnt;
    FH_AF_CTRL_e cmd;
    FH_AF_CFG_t cfg;

    cnt = 5;
    cmd = AF_MOVE_ZOOM;
    cfg.id = 1;
    cfg.dir = 1;
    cfg.step = 100;

    sleep(10);
    while(cnt --)
    {
        printf("[af] zoom forward 100 step\n");
        FH_AF_Ctrl(cmd, &cfg);
        sleep(5);
    }

    return NULL;
}

int sample_af_start()
{
    int i;
    int ret = -1;
    pthread_t af_thread;
    pthread_attr_t af_attr;
    FH_AF_MOTOR_CFG_t af_cfg;

    af_cfg.zoom_range = 2342;
    af_cfg.focus_range = 3011;
    af_cfg.focus_reserve_range = 0;
    af_cfg.zoom_reserve_range = 0;
    af_cfg.inv_error = 6;
    af_cfg.freq_low = 100;
    af_cfg.freq_mid = 100;
    af_cfg.freq_high = 150;

    for(i = 0; i < 4; i++)
        af_cfg.zoom_pwm[i] = i + 6;
    for(i = 0; i < 4; i++)
        af_cfg.focus_pwm[i] = i + 2;


    ret = FH_AF_Init(&af_cfg);
    if(ret)
    {
        printf("[FH_AF_Init] failed with:%d\n", ret);
        goto Exit;
    }
    FH_AF_SetDebugLevel(AF_DEBUG_OFF);

    pthread_attr_init(&af_attr);
    pthread_attr_setdetachstate(&af_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&af_attr, 10 * 1024);
    ret = pthread_create(&af_thread, &af_attr, af_run, NULL);
    if(ret)
    {
        printf("[pthread_create]: failed with %d\n", ret);
        goto Exit;
    }

Exit:
    return ret;

}

void sample_af_stop(void)
{
    FH_AF_Exit();
}

void sample_af_run(void)
{
    FH_AF_FrameRun();
}

#endif