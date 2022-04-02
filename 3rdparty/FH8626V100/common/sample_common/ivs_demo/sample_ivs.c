#include <stdarg.h>
#include "sample_common.h"
#include "fh_ivs_func.h"

static FH_SINT32 g_ivs_stop = 0;
static FH_SINT32 g_ivs_is_running = 0;
static FH_UINT32 current_max_gbox_id = 0;

extern FH_SINT32 sample_set_gbox(FH_UINT32 chan, FH_UINT32 enable,  FH_UINT32 box_id, FH_UINT32 x, FH_UINT32 y, FH_UINT32 w, FH_UINT32 h, FHT_RgbColor_t color);

static FH_SINT32 clear_screen(int reserve_num)
{
    FH_UINT32 i;
    FH_UINT32 ret;
    FHT_RgbColor_t color = {0, 255, 0, 255};/* 绿 */

    for (i = current_max_gbox_id; i > reserve_num; i --)
    {
        ret = sample_set_gbox(0, 0, i, 0, 0, 0, 0, color);
        if (ret)
        {
            printf("Error: sample_set_gbox %d\n", ret);
            return ret;
        }
    }

    current_max_gbox_id = reserve_num;

    return 0;
}

static FH_SINT32 draw_gbox(FH_UINT32 x, FH_UINT32 y, FH_UINT32 w, FH_UINT32 h, FH_UINT32 colorid)
{
    FHT_RgbColor_t colors[7] = {{255, 0, 0, 255},/* 红 */
                                {0, 255, 0, 255},/* 绿 */
                                {0, 0, 255, 255},/* 蓝 */
                                {255, 255, 0, 255},/* 黄 */
                                {255, 0, 255, 255},/* 粉 */
                                {0, 255, 255, 255},/* 青 */
                                {255, 255, 255, 255}};/* 白 */

    current_max_gbox_id ++;
    return sample_set_gbox(0, 1, current_max_gbox_id, x, y, w, h, colors[colorid]);
}

static FH_VOID debugCallback(FH_SINT32 level, FH_CHAR* format, ...)
{
    printf("[level:%d]", level);

    FH_CHAR param[100] = {'\0'};
    va_list valist;

    va_start(valist, format);
    vsprintf(param, format, valist);
    va_end(valist);

    printf("%s", param);
}

static FH_VOID *ivs_task(FH_VOID *arg)
{
    FH_SINT32 i;
    FH_SINT32 ret = -1;

    FH_RECT_S IvsPos;
    FH_IVS_ALL_RESULT_S results;

    FH_TRIP_RULE_PARA_S tripcfg[2];
    FH_PERI_RULE_PARA_S pericfg[2];
    FH_TRIP_ALL_RESULT_S tresult;
    FH_PERI_ALL_RESULT_S presult;
    FH_IVS_TRACK_CFG_t tracfg;

    ret = sample_common_bgm_init();
    if(ret)
    {
        printf("[bgm_Init] failed with:%d\n", ret);
        goto Exit;
    }

    FH_VOID (*callback)(FH_SINT32 level, FH_CHAR* format, ...) = debugCallback;
    FH_IVS_Register(callback);

    tracfg.maxTrackNum = 10;
    tracfg.iouThresh = 15;
    tracfg.minHits = 3;
    tracfg.maxAge = 6;
    ret = FH_IVS_Init(&tracfg);
    if(ret)
    {
        printf("[FH_IVS_Init] failed with:%d\n", ret);
        goto Exit;
    }

    FH_IVS_SetFrameSize(CH0_WIDTH, CH0_HEIGHT);
    FH_IVS_SetDebugLevel(IVS_DEBUG_LEVEL_RET);

#if (FH_APP_TRIP_NUM > 0)
    memset(&tripcfg[0], 0, sizeof(FH_TRIP_RULE_PARA_S));
    tripcfg[0].enable = 1;
    tripcfg[0].startPoint.x = 0;
    tripcfg[0].startPoint.y = 750;
    tripcfg[0].endPoint.x = CH0_WIDTH;
    tripcfg[0].endPoint.y = 750;
    tripcfg[0].iSizeMin = 0;
    tripcfg[0].iSizeMax = 100;
    tripcfg[0].bDobDir = 0;
    tripcfg[0].iForDir = FH_TMODE_LTOR;
    tripcfg[0].minHits = 1;
    ret = FH_IVS_SetTripRule(0, &tripcfg[0]);
    if(ret)
    {
        printf("[FH_IVS_SetTripRule] id:0 failed with:%d\n", ret);
        goto Exit;
    }
    draw_gbox(0, 750, CH0_WIDTH, 2, 1);

#if (FH_APP_TRIP_NUM > 1)
    memset(&tripcfg[1], 0, sizeof(FH_TRIP_RULE_PARA_S));
    tripcfg[1].enable = 1;
    tripcfg[1].startPoint.x = 0;
    tripcfg[1].startPoint.y = 250;
    tripcfg[1].endPoint.x = CH0_WIDTH;
    tripcfg[1].endPoint.y = 250;
    tripcfg[1].iSizeMin = 0;
    tripcfg[1].iSizeMax = 100;
    tripcfg[1].bDobDir = 0;
    tripcfg[1].iForDir = FH_TMODE_LTOR;
    tripcfg[1].minHits = 1;
    ret = FH_IVS_SetTripRule(1, &tripcfg[1]);
    if(ret)
    {
        printf("[FH_IVS_SetTripRule] id:1 failed with:%d\n", ret);
        goto Exit;
    }
    draw_gbox(0, 250, CH0_WIDTH, 2, 1);
#endif
#endif

#if (FH_APP_PERI_NUM > 0)
    memset(&pericfg[0], 0 , sizeof(FH_PERI_RULE_PARA_S));
    pericfg[0].enable = 1;
    pericfg[0].iTimeMin = 7;
    pericfg[0].iMode = FH_PMODE_INTRUSION;
    pericfg[0].iSizeMin = 0;
    pericfg[0].iSizeMax = 100;
    pericfg[0].minHits = 1;

    pericfg[0].stBoundary.iPointNum = 4;
    pericfg[0].stBoundary.astPoint[0].x = 200;
    pericfg[0].stBoundary.astPoint[0].y = 300;

    pericfg[0].stBoundary.astPoint[1].x = 1000;
    pericfg[0].stBoundary.astPoint[1].y = 300;

    pericfg[0].stBoundary.astPoint[2].x = 1000;
    pericfg[0].stBoundary.astPoint[2].y = 1000;

    pericfg[0].stBoundary.astPoint[3].x = 200;
    pericfg[0].stBoundary.astPoint[3].y = 1000;
    ret = FH_IVS_SetPeriRule(0, &pericfg[0]);
    if(ret)
    {
        printf("[FH_IVS_SetPeriRule] id:0 failed with:%d\n", ret);
        goto Exit;
    }
    draw_gbox(200, 300, 800, 700, 1);

#if (FH_APP_PERI_NUM > 1)
    memset(&pericfg[1], 0 , sizeof(FH_PERI_RULE_PARA_S));
    pericfg[1].enable = 1;
    pericfg[1].iTimeMin = 5;
    pericfg[1].iMode = FH_PMODE_EXIT;
    pericfg[1].iSizeMin = 0;
    pericfg[1].iSizeMax = 100;
    pericfg[1].minHits = 1;

    pericfg[1].stBoundary.iPointNum = 4;
    pericfg[1].stBoundary.astPoint[0].x = 1200;
    pericfg[1].stBoundary.astPoint[0].y = 300;

    pericfg[1].stBoundary.astPoint[1].x = CH0_WIDTH;
    pericfg[1].stBoundary.astPoint[1].y = 300;

    pericfg[1].stBoundary.astPoint[2].x = CH0_WIDTH;
    pericfg[1].stBoundary.astPoint[2].y = 1000;

    pericfg[1].stBoundary.astPoint[3].x = 1200;
    pericfg[1].stBoundary.astPoint[3].y = 1000;
    ret = FH_IVS_SetPeriRule(1, &pericfg[1]);
    if(ret)
    {
        printf("[FH_IVS_SetPeriRule] id:1 failed with:%d\n", ret);
        goto Exit;
    }
    draw_gbox(1200, 300, CH0_WIDTH - 1200, 700, 1);
#endif
#endif

    while (!g_ivs_stop)
    {
        clear_screen(FH_APP_TRIP_NUM + FH_APP_PERI_NUM);

        FH_IVS_Process(&results);
        for(i = 0; i < results.num; i++)
        {
            IvsPos.x = results.astresult[i].pos.x;
            IvsPos.y = results.astresult[i].pos.y;
            IvsPos.w = results.astresult[i].pos.w;
            IvsPos.h = results.astresult[i].pos.h;
        }

#if (FH_APP_TRIP_NUM > 0)
        ret = FH_IVS_GetTripResults(0, &tresult);
        if(ret)
        {
            printf("[FH_IVS_GetTripResults] id:0 failed with:%d\n", ret);
        }
        for(i = 0; i < tresult.EventNum; i++)
        {
            IvsPos.x = tresult.astresult[i].stRect.x;
            IvsPos.y = tresult.astresult[i].stRect.y;
            IvsPos.w = tresult.astresult[i].stRect.w;
            IvsPos.h = tresult.astresult[i].stRect.h;
            draw_gbox(IvsPos.x, IvsPos.y, IvsPos.w, IvsPos.h, 0);
        }

#if (FH_APP_TRIP_NUM > 1)
        ret = FH_IVS_GetTripResults(1, &tresult);
        if(ret)
        {
            printf("[FH_IVS_GetTripResults] id:1 failed with:%d\n", ret);
        }
        for(i = 0; i < tresult.EventNum; i++)
        {
            IvsPos.x = tresult.astresult[i].stRect.x;
            IvsPos.y = tresult.astresult[i].stRect.y;
            IvsPos.w = tresult.astresult[i].stRect.w;
            IvsPos.h = tresult.astresult[i].stRect.h;
            draw_gbox(IvsPos.x, IvsPos.y, IvsPos.w, IvsPos.h, 0);
        }
#endif
#endif

#if (FH_APP_PERI_NUM > 0)
        ret = FH_IVS_GetPeriResults(0, &presult);
        if(ret)
        {
            printf("[FH_IVS_GetPeriResults] id:0 failed with:%d\n", ret);
        }
        for(i = 0; i < presult.EventNum; i++)
        {
            IvsPos.x = presult.astresult[i].stRect.x;
            IvsPos.y = presult.astresult[i].stRect.y;
            IvsPos.w = presult.astresult[i].stRect.w;
            IvsPos.h = presult.astresult[i].stRect.h;
            draw_gbox(IvsPos.x, IvsPos.y, IvsPos.w, IvsPos.h, 0);
        }

#if (FH_APP_PERI_NUM > 1)
        ret = FH_IVS_GetPeriResults(1, &presult);
        if(ret)
        {
            printf("[FH_IVS_GetPeriResults] id:1 failed with:%d\n", ret);
        }
        for(i = 0; i < presult.EventNum; i++)
        {
            IvsPos.x = presult.astresult[i].stRect.x;
            IvsPos.y = presult.astresult[i].stRect.y;
            IvsPos.w = presult.astresult[i].stRect.w;
            IvsPos.h = presult.astresult[i].stRect.h;
            draw_gbox(IvsPos.x, IvsPos.y, IvsPos.w, IvsPos.h, 0);
        }
#endif
#endif
        usleep(40*1000);
    }

Exit:
    g_ivs_is_running = 0;
    return NULL;
}

FH_SINT32 sample_ivs_start(FH_VOID)
{
    FH_SINT32 ret;
    pthread_t thread;
    pthread_attr_t attr;

    if (g_ivs_is_running)
    {
        printf("Error: ivs already running!\n");
        return -1;
    }

    g_ivs_stop = 0;
    g_ivs_is_running = 1;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 5 * 1024);
    ret = pthread_create(&thread, &attr, ivs_task, NULL);
    if (ret)
    {
        printf("[ERROR]: create md/cd thread failed, ret=%d\n", ret);
        g_ivs_is_running = 0;
        return ret;
    }

    return 0;
}

FH_SINT32 sample_ivs_stop(FH_VOID)
{
    FH_SINT32 ret;

    while (g_ivs_is_running)
    {
        g_ivs_stop = 1;
        usleep(20*1000);
    }
    clear_screen(0);
    ret = FH_IVS_Release();
    if(ret)
    {
        printf("[FH_IVS_Release] failed with:%d\n", ret);
        return -1;
    }

    return 0;
}
