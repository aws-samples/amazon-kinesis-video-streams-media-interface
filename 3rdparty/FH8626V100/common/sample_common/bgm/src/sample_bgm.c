#include <sample_common.h>

static FH_UINT32     g_bgm_inited = 0;
static pthread_mutex_t g_bgm_lock;
static FH_SINT32     g_bgm_lock_inited = 0;

static FH_UINT8     *g_confidence_buf;      /*存放当前帧bgm前景检测结果*/
static FH_UINT32     g_confidence_bufsz;    /*动态分配缓冲区confidence_level的大小*/
static FH_UINT32     g_confidence_result_w; /*bgm前景结果宽度*/
static FH_UINT32     g_confidence_result_h; /*bgm前景结果高度*/

FH_SINT32 sample_common_bgm_init(FH_VOID)
{
    FH_SINT32 ret = 0;
    FH_UINT32 init_mem_w;
    FH_UINT32 init_mem_h;
    FH_SIZE picsize;
    struct channel_info info;

    if (!g_bgm_lock_inited)
    {
        pthread_mutex_init(&g_bgm_lock, NULL);
        g_bgm_lock_inited = 1;
    }
    pthread_mutex_lock(&g_bgm_lock);

    if (g_bgm_inited)
        goto Exit;

    ret = sample_common_dsp_get_channel_info(0, &info); /*BGM fixed to use channel 0's 1/8 down samples*/
    if (ret != 0)
        goto Exit;

    init_mem_w = ALIGNTO(info.max_width, 16) / 8;
    init_mem_h = ALIGNTO(info.max_height, 16) / 8;
    ret = FH_BGM_InitMem(init_mem_w, init_mem_h);
    if (ret != RETURN_OK)
        goto Exit;

    picsize.u32Width = ALIGNTO(info.width, 16) / 8;
    picsize.u32Height = ALIGNTO(info.height, 16) / 8;
    ret = FH_BGM_SetConfig(&picsize);
    if (ret != RETURN_OK)
        goto Exit;

    ret = FH_BGM_Enable();
    if (ret != RETURN_OK)
        goto Exit;

    ret = FH_SYS_BindVpu2Bgm();
    if (ret != RETURN_OK)
        goto Exit;

    g_bgm_inited = 1;

Exit:
    pthread_mutex_unlock(&g_bgm_lock);
    if (ret != RETURN_OK)
    {
        printf("Error: sample_common_bgm_init failed!\n");
    }
    return ret;
}

FH_SINT32 sample_common_bgm_get_init_status(FH_VOID)
{
    return g_bgm_inited;
}

FH_SINT32 sample_common_bgm_triger_reload(FH_VOID)
{
    int ret;
    FH_BGM_SW_STATUS bgm_status;/*bgm检测结果*/

    if (!g_bgm_inited)
        return -1;

    pthread_mutex_lock(&g_bgm_lock);

    /*invalid previous bgm data...*/
    g_confidence_result_w = 0;
    g_confidence_result_h = 0;

    ret = FH_BGM_GetSWStatus(&bgm_status);
    if (ret != RETURN_OK)/*获取背景建模信息*/
    {
        printf("Error: FH_BGM_GetSWStatus(%d)!\n", ret);
        goto Exit;
    }

    if (bgm_status.confidence_level.size > g_confidence_bufsz)
    {
        if (g_confidence_buf)
        {
            free(g_confidence_buf);
            g_confidence_buf = NULL;
        }
        g_confidence_bufsz = 0;

        g_confidence_buf = malloc(bgm_status.confidence_level.size);
        if (!g_confidence_buf)
        {
            printf("Error: no memory for BGM data!\n");
            ret = -1;
            goto Exit;
        }

        g_confidence_bufsz = bgm_status.confidence_level.size;
    }

    memcpy(g_confidence_buf, bgm_status.confidence_level.addr, bgm_status.confidence_level.size);
    g_confidence_result_w  = bgm_status.size.u32Width  / 16;
    g_confidence_result_h  = bgm_status.size.u32Height / 16;

Exit:
    pthread_mutex_unlock(&g_bgm_lock);

    return ret;
}

FH_SINT32 sample_common_bgm_get_outer_motion_area(
        FH_UINT32 *winX,
        FH_UINT32 *winY,
        FH_UINT32 *winW,
        FH_UINT32 *winH,
        FH_UINT32 *picW,
        FH_UINT32 *picH,
        FH_UINT8   thresh)
{
    if (!g_bgm_inited)
        return -1;

    pthread_mutex_lock(&g_bgm_lock);

    if (g_confidence_result_w <= 0) /*not valid*/
    {
        pthread_mutex_unlock(&g_bgm_lock);
        return -1;
    }

    libmisc_get_outer_box(g_confidence_buf,
            g_confidence_result_w,
            g_confidence_result_h,
            winX, winY, winW, winH, thresh);
    *winX <<= 4;
    *winY <<= 4;
    *winW <<= 4;
    *winH <<= 4;
    *picW = g_confidence_result_w << 4;
    *picH = g_confidence_result_h << 4;

    pthread_mutex_unlock(&g_bgm_lock);

    return 0;
}

FH_SINT32 sample_common_bgm_get_statistics_sum(
        FH_UINT32 detect_frame_w,
        FH_UINT32 detect_frame_h,
        FH_UINT32 x,
        FH_UINT32 y,
        FH_UINT32 w,
        FH_UINT32 h,
        FH_UINT32 *sum,
        FH_UINT32 *blk_num)
{
    FH_SINT32 i;
    FH_SINT32 j;
    FH_UINT32 mbx;
    FH_UINT32 mby;
    FH_UINT32 mbw;
    FH_UINT32 mbh;
    FH_UINT32 confidence_sum = 0;
    FH_UINT8* confidence;

    if (!g_bgm_inited)
        return -1;

    pthread_mutex_lock(&g_bgm_lock);

    if (g_confidence_result_w <= 0)
    {
        pthread_mutex_unlock(&g_bgm_lock);
        return -1;
    }

    mbx = (x * (g_confidence_result_w<<4) / detect_frame_w) >> 4;
    mby = (y * (g_confidence_result_h<<4) / detect_frame_h) >> 4;
    mbw = (w * (g_confidence_result_w<<4) / detect_frame_w + 15) >> 4;
    mbh = (h * (g_confidence_result_h<<4) / detect_frame_h + 15) >> 4;

    confidence = g_confidence_buf + mby * g_confidence_result_w + mbx;

    for (i = 0; i < mbh; i++)
    {
        for (j = 0; j < mbw; j++)
        {
            confidence_sum += confidence[j];
        }
        confidence += g_confidence_result_w;
    }

    pthread_mutex_unlock(&g_bgm_lock);

    *sum = confidence_sum;
    *blk_num = mbw * mbh;

    return 0;
}

FH_SINT32 sample_common_bgm_exit(FH_VOID)
{
    if (g_bgm_inited)
    {
        pthread_mutex_lock(&g_bgm_lock);

        FH_BGM_Disable();

        if (g_confidence_buf)
        {
            free(g_confidence_buf);
            g_confidence_buf = NULL;
        }
        g_confidence_bufsz = 0;
        g_confidence_result_w = 0;
        g_confidence_result_h = 0;

        pthread_mutex_unlock(&g_bgm_lock);
    }

    g_bgm_inited = 0;

    return 0;
}
