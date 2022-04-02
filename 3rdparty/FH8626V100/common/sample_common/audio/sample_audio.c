/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2019-04-29     jianges       first version
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "dsp/fh_audio_mpi.h"
#include "sample_audio.h"
#include "sample_audio_config.h"
#include "audio_data.h"

#define ONE_FRAME_SIZE 1024       /* how many audio samples in one frame...*/

static int g_stop_record_audio = 0;
static int g_stop_play_audio   = 0;
static int g_audio_running     = 0;

static unsigned int record_frame_period;
static unsigned int play_frame_period;

static int samples_to_bytes(int n, int sample_bits)
{
    if (sample_bits == 24)
    {
        return (n * 4);
    }

    return (n * (sample_bits >> 3));
}

int audio_deinit(void)
{
    FH_AC_AI_Disable();
    FH_AC_AO_Disable();
    FH_AC_DeInit();

    return 0;
}

void *audio_record_proc(void *args)
{
    int ret;
    FH_AC_CONFIG *cfg;
    FH_AC_FRAME_S frame_info;

    cfg             = (FH_AC_CONFIG *)args;
    frame_info.data = malloc(record_frame_period);
    if (!frame_info.data)
    {
        printf("Error: audiop record malloc audio data failed!\n");
        return NULL;
    }

    while (!g_stop_record_audio)
    {
        frame_info.len = record_frame_period;
        ret = FH_AC_AI_GetFrame(&frame_info);
        if (ret != 0)
        {
            //printf("Error: FH_AC_AI_GetFrame failed, ret=%d!\n", ret);
        }
    }

    if (frame_info.data)
        free(frame_info.data);

    g_stop_record_audio = 0;

    return NULL;
}

void *audio_play_proc(void *args)
{
    int ret;
    int total_len;

    FH_AC_CONFIG *cfg;
    FH_AC_FRAME_S frame_info;

    cfg             = (FH_AC_CONFIG *)args;
    frame_info.data = (FH_UINT8 *)audio_data;
    frame_info.len  = play_frame_period;
    total_len       = sizeof(audio_data);

    while(!g_stop_play_audio)
    {
        frame_info.len   = play_frame_period;
        frame_info.data += frame_info.len;
        total_len       -= frame_info.len;
        FH_AC_AO_SendFrame(&frame_info);
        if (total_len < play_frame_period)
        {
            frame_info.data = (FH_UINT8 *)audio_data;
            total_len       = sizeof(audio_data);
        }
        usleep(5000);
    }

    g_stop_play_audio = 0;

    return NULL;
}

void random_delay(char *msg)
{
    unsigned int t = random();

    t %= 5;

    printf("%s: t=%d.\n", msg, t);
    sleep(t);
}

void *ai_restart_proc(void *args)
{
    int ret;
    int delay;

    while (1)
    {
        random_delay("AI-Delay_stop");
        ret = FH_AC_AI_Disable();

        random_delay("AI-Delay_start");
        ret = FH_AC_AI_Enable();
    }

    return NULL;
}

void *ao_restart_proc(void *args)
{
    int ret;
    int delay;

    while (1)
    {
        random_delay("AO-Delay_stop");
        ret = FH_AC_AO_Disable();

        random_delay("AO-Delay_start");
        ret = FH_AC_AO_Enable();
    }

    return NULL;
}

void audio_restart_test(void)
{
    pthread_attr_t attr;
    pthread_t thread_ai_restart;
    pthread_t thread_ao_restart;

    srandom(time(0));

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 3 * 1024);

    pthread_create(&thread_ai_restart, &attr, ai_restart_proc, NULL);
    pthread_create(&thread_ao_restart, &attr, ao_restart_proc, NULL);
}

int sample_audio(void)
{
    pthread_attr_t attr;
    pthread_t thread_audio_record;
    pthread_t thread_audio_play;
    struct sched_param param;
    int ret;

    if (g_audio_running == 1)
    {
        printf("Audio is running\n");
        return 0;
    }

    ret = FH_AC_Init();
    //ret = FH_AC_Init_WithExternalCodec();
    if (ret != 0)
    {
        printf("Error: FH_AC_Init failed, ret=%d!\n", ret);
        return -1;
    }

    FH_AC_CONFIG cfg;
    cfg.io_type     = FH_AC_MIC_IN;
    cfg.sample_rate = AC_SR_16K;
    cfg.bit_width   = AC_BW_16;
    cfg.enc_type    = FH_PT_LPCM;
    cfg.channels    = 1;
    cfg.period_size = ONE_FRAME_SIZE;
    cfg.volume      = 31;
    ret = FH_AC_Set_Config(&cfg);
    if (ret != 0)
    {
        printf("Error: FH_AC_Set_Config failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    record_frame_period = samples_to_bytes(cfg.period_size, cfg.bit_width);

    ret = FH_AC_AI_MICIN_SetVol(3);
    if (ret != 0)
    {
        printf("Error: FH_AC_AI_MICIN_SetVol failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    FH_AC_AEC_CONFIG aec_config   = {1, 55};
    FH_AC_NR_CONFIG  ac_nr_config = {1, 3};
    FH_AC_AGC_CONFIG agc_config   = {1, 3};

    ret = FH_AC_AEC_SetConfig(&aec_config);
    if (ret != 0)
    {
        printf("Error: FH_AC_AEC_SetConfig failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    ret = FH_AC_NR_SetConfig(&ac_nr_config);
    if (ret != 0)
    {
        printf("Error: FH_AC_NR_SetConfig failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    ret = FH_AC_Agc_SetConfig(&agc_config);
    if (ret != 0)
    {
        printf("Error: FH_AC_Agc_SetConfig failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    ret = FH_AC_AI_Enable();
    if (ret != 0)
    {
        printf("Error: FH_AC_AI_Enable failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    cfg.io_type     = FH_AC_LINE_OUT;
    cfg.sample_rate = AC_SR_16K;
    cfg.bit_width   = AC_BW_16;
    cfg.enc_type    = FH_PT_LPCM;
    cfg.channels    = 1;
    cfg.period_size = ONE_FRAME_SIZE;
    cfg.volume      = 31;
    ret = FH_AC_Set_Config(&cfg);
    if (ret != 0)
    {
        printf("Error: FH_AC_Set_Config failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    play_frame_period = samples_to_bytes(cfg.period_size, cfg.bit_width);

    ret = FH_AC_AO_SetVol(85);
    if (ret != RETURN_OK)
    {
        printf("Error: FH_AC_AI_SetVol failed with %d\n", ret);
        goto Err_Exit;
    }

    FH_AC_NR_CONFIG  play_nr_config = {1, 3};

    ret = FH_AC_PlayNR_SetConfig(&play_nr_config);
    if (ret != 0)
    {
        printf("Error: FH_AC_PlayNR_SetConfig failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    ret = FH_AC_AO_Enable();
    if (ret != 0)
    {
        printf("Error: FH_AC_AO_Enable failed, ret=%d!\n", ret);
        goto Err_Exit;
    }

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 3 * 1024);
    pthread_attr_setschedparam(&attr, &param);

    pthread_create(&thread_audio_record, &attr, audio_record_proc, NULL);
    pthread_create(&thread_audio_play, &attr, audio_play_proc, NULL);

    //audio_restart_test();

    g_audio_running = 1;

    return 0;

Err_Exit:
    audio_deinit();
}

int audio_exit(void)
{
    if (g_audio_running)
    {
        g_stop_record_audio = 1;
        g_stop_play_audio   = 1;

        while (g_stop_record_audio == 1)
        {
            usleep(10 * 1000);
        }

        while (g_stop_play_audio == 1)
        {
            usleep(10 * 1000);
        }

        audio_deinit();
        g_audio_running = 0;
    }

    return 0;
}
