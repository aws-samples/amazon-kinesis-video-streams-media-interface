#include "sample_common.h"
#include "FHAdv_MD_mpi.h"

#define MDEX_MBNUM_H  (2)
#define MDEX_MBNUM_V  (2)

#define MD_AREA_NUM_H (2) /*MD模块，水平方向上的检测区域数,16*16的宏块为单位*/
#define MD_AREA_NUM_V (2) /*MD模块，垂直方向上的检测区域数,16*16的宏块为单位*/

#define MD_CD_CHECK_INTERVAL (25) /*25毫秒检测一次,FHAdv_MD_CD_Check函数同时检测MD/MD_EX/CD,
                                     如果用户对于MD/MD_EX/CD配置了不同的检测间隔,
                                     advapi内部会取他们的最小检测间隔*/

static volatile FH_SINT32 g_check_md = 0;
static volatile FH_SINT32 g_check_md_ex = 0;
static volatile FH_SINT32 g_check_cd = 0;

static volatile FH_SINT32 g_request_to_stop = 0;
static volatile FH_SINT32 g_md_cd_is_running = 0;

static pthread_mutex_t g_md_cd_lock;
static FH_SINT32 g_md_cd_lock_inited = 0;

static FH_VOID handle_md_result(FH_UINT32 result)
{
#ifdef FH_APP_OPEN_RECT_MOTION_DETECT_DUMP_RESULT
    FH_SINT32 i;

    if (result == 0)
    {
        printf("\r[INFO]: MD result: NO         ");
    }
    else
    {
        printf("\r[INFO]: MD result: YES");
        for(i = 0; i < MD_AREA_NUM_H * MD_AREA_NUM_V; i++)
        {
            if (result & (1<<i))
            {
                printf(",%d", i); /*此区域有运动*/
            }
            else
            {
                printf(", "); /*此区域没有运动*/
            }
        }
    }
#endif
}

static FH_VOID handle_md_ex_result(FHT_MDConfig_Ex_Result_t *result)
{
#ifdef FH_APP_OPEN_MOTION_DETECT_DUMP_RESULT
    FH_SINT32 i;
    FH_SINT32 j;
    FH_UINT8   motion_flag;
    static FH_SINT32 count;

    if (++count >= 10) /*print out every 10 times*/
    {
        count = 0;
        printf("\033[H\033[J");
        printf("======================= Motion Detect Result  ===========================\n");
        for (i = 0; i < result->vertical_count; i++)
        {
            for (j = 0; j < result->horizontal_count; j++)
            {
                motion_flag = *(result->start + i * result->horizontal_count + j);
                if (motion_flag > 0)
                    printf("o "); /*motion*/
                else
                    printf("  "); /*no motion*/
            }
            printf("\n");
        }
    }
#endif
}

static FH_VOID handle_cd_result(FH_UINT32 result)
{
#ifdef FH_APP_OPEN_COVER_DETECT_DUMP_RESULT
    if (result != 0)
    {
        printf("\r[INFO]:  CD result: YES");
    }
    else
    {
        printf("\r[INFO]:  CD result: NO ");
    }
    fflush(stdout);
#endif
}
static FH_VOID sample_md_get_result(FH_VOID)
{
    FH_SINT32 ret;
    FH_UINT32 result;

    ret = FHAdv_MD_GetResult(&result);
    if (ret == RETURN_OK)
    {
        handle_md_result(result);
    }
    else
    {
        printf("\n[ERROR]: FHAdv_MD_GetResult failed, ret=%d\n", ret);
    }
}

static FH_VOID sample_md_ex_get_result(FH_VOID)
{
    FH_SINT32 ret;
    FHT_MDConfig_Ex_Result_t result;

    ret = FHAdv_MD_Ex_GetResult(&result); /* 获取设置全局运动检测结果 */
    if (ret == FH_SUCCESS)
    {
        handle_md_ex_result(&result);
    }
    else
    {
        printf("\nError: FHAdv_MD_Ex_GetResult failed, ret=%d\n", ret);
    }
}

static FH_VOID sample_cd_get_result(FH_VOID)
{
    FH_SINT32 ret;
    FH_UINT32 result;

    ret = FHAdv_CD_GetResult(&result);
    if (ret == RETURN_OK)
    {
        handle_cd_result(result);
    }
    else
    {
        printf("\n[ERROR]: FHAdv_CD_GetResult failed, ret=%d\n", ret);
    }
}

static FH_VOID *md_cd_task(FH_VOID *arg)
{
    while (!g_request_to_stop)
    {
        pthread_mutex_lock(&g_md_cd_lock);
        FHAdv_MD_CD_Check(); /* 做一次运动、遮挡检测 */
        pthread_mutex_unlock(&g_md_cd_lock);

        if (g_check_cd)
            sample_cd_get_result();

        if (g_check_md)
            sample_md_get_result();

        if (g_check_md_ex)
            sample_md_ex_get_result();

        usleep((MD_CD_CHECK_INTERVAL+10)*1000);/*这里增加10ms的目的是,确保每次调用FHAdv_MD_CD_Check都能真正做MD/CD的check*/
    }

    g_md_cd_is_running = 0;
    return NULL;
}

static FH_SINT32 sample_cd_init(FH_VOID)
{
    FH_SINT32 ret;
    FHT_CDConfig_t cd_config;

    ret = FHAdv_CD_Init(); /* 初始化遮挡检测 */
    if (ret != 0)
    {
        printf("[ERROR]: FHAdv_CD_Init failed, ret=%d\n", ret);
        return ret;
    }

    /* 设置遮挡检测参数 */
    cd_config.framedelay = MD_CD_CHECK_INTERVAL; /* 设置检测帧间隔 | 时间间隔，默认使用帧间隔，可以调用FHAdv_MD_Set_Check_Mode设置使用时间间隔 */
    cd_config.cd_level = CD_LEVEL_MID; /* 设置遮挡检测灵敏度 */
    cd_config.enable = 1;

    ret = FHAdv_CD_SetConfig(&cd_config);
    if (ret != 0)
    {
        printf("[ERROR]: FHAdv_CD_SetConfig failed, ret=%d\n", ret);
    }

    return ret;
}

static FH_SINT32 sample_md_init(FH_VOID)
{
    FH_SINT32 ret;
    FH_SINT32 i;
    FH_SINT32 j;
    FH_SINT32 vi_w;
    FH_SINT32 vi_h;
    FH_SINT32 block_w;
    FH_SINT32 block_h;

    FHT_MDConfig_t md_config;

    vi_w = sample_isp_get_vi_w();
    vi_h = sample_isp_get_vi_h();
    block_w = vi_w / MD_AREA_NUM_H;
    block_h = vi_h / MD_AREA_NUM_V;

    ret = FHAdv_MD_Init(); /* 初始化区域运动检测功能 */
    if (ret != 0)
    {
        printf("[ERROR]: FHAdv_MD_Init failed, ret=%d\n", ret);
        return ret;
    }

    memset(&md_config, 0, sizeof(md_config));
    md_config.enable = 1;      /* 使能控制 */
    md_config.threshold = 80; /* 运动检测阈值，该值越大，检测灵敏度越高 */
    md_config.framedelay = MD_CD_CHECK_INTERVAL;  /* 设置检测帧间隔或者时间间隔，默认使用帧间隔，可以调用FHAdv_MD_Set_Check_Mode设置使用时间间隔 */

    /*
     * MD模块支持32个检测区域，
     * 我们的sample程序设置了(MD_AREA_NUM_H*MD_AREA_NUM_V),即4个检测区域,
     * 其他28个区域会被忽略，因为其fWidth,fHeigh为0
     */
    for (i = 0; i < MD_AREA_NUM_V; i++)
    {
        for (j = 0; j < MD_AREA_NUM_H; j++)
        {
            md_config.md_area[MD_AREA_NUM_H * i + j].fTopLeftX = j * block_w;
            md_config.md_area[MD_AREA_NUM_H * i + j].fTopLeftY = i * block_h;
            md_config.md_area[MD_AREA_NUM_H * i + j].fWidth = block_w;
            md_config.md_area[MD_AREA_NUM_H * i + j].fHeigh = block_h;
        }
    }

    /* 设置区域检测配置 */
    ret = FHAdv_MD_SetConfig(&md_config);
    if (ret != 0)
    {
        printf("[ERROR]: FHAdv_MD_SetConfig failed, ret=%d\n", ret);
        return ret;
    }

    return 0;
}

static FH_SINT32 sample_md_ex_init(FH_VOID)
{
    FH_SINT32 ret;

    FHT_MDConfig_Ex_t md_config;

    ret = FHAdv_MD_Ex_Init();
    if (ret != 0)
    {
        printf("[ERROR]: FHAdv_MD_Ex_Init failed, ret=%d\n", ret);
        return ret;
    }

    md_config.threshold = 80; /* 运动检测阈值，该值越大，检测灵敏度越高 */
    md_config.framedelay = MD_CD_CHECK_INTERVAL; /* 设置检测帧间隔 | 时间间隔，默认使用帧间隔，可以调用FHAdv_MD_Set_Check_Mode设置使用时间间隔 */
    md_config.enable = 1;

    /* 设置区域检测配置 */
    ret = FHAdv_MD_Ex_SetConfig(&md_config);
    if (ret != 0)
    {
        printf("[ERROR]: FHAdv_MD_Ex_SetConfig failed, ret=%d\n", ret);
        return ret;
    }

    return 0;
}

FH_SINT32 sample_md_get_outer_motion_area(
	FH_UINT32 *winX,
	FH_UINT32 *winY,
	FH_UINT32 *winW,
	FH_UINT32 *winH,
	FH_UINT32 *picW,
	FH_UINT32 *picH)
{
	FH_SINT32 ret;
    FHT_MDConfig_Ex_Result_t result;

	if (!g_md_cd_lock_inited || !g_check_md_ex)
	{
		return -1;
	}

	pthread_mutex_lock(&g_md_cd_lock);

    ret = FHAdv_MD_Ex_GetResult(&result); /* 获取设置全局运动检测结果 */
    if (ret != FH_SUCCESS)
    {
    	pthread_mutex_unlock(&g_md_cd_lock);
    	printf("Error: FHAdv_MD_Ex_GetResult failed, ret=%d\n", ret);
    	return -1;
    }

    libmisc_get_outer_box(result.start,
			      result.horizontal_count,
			      result.vertical_count,
			      winX, winY, winW, winH, 1);
    *winX *= (MDEX_MBNUM_H*16);
	*winY *= (MDEX_MBNUM_V*16);
	*winW *= (MDEX_MBNUM_H*16);
	*winH *= (MDEX_MBNUM_V*16);
	*picW = result.horizontal_count * (MDEX_MBNUM_H*16);
	*picH = result.vertical_count * (MDEX_MBNUM_V*16);

	pthread_mutex_unlock(&g_md_cd_lock);
	return 0;
}

FH_SINT32 sample_md_get_statistics_sum(FH_UINT32 detect_frame_w,
	                                    FH_UINT32 detect_frame_h,
	                                    FH_UINT32 x,
	                                    FH_UINT32 y,
	                                    FH_UINT32 w,
	                                    FH_UINT32 h,
	                                    FH_UINT32 *sum,
	                                    FH_UINT32 *blk_num)
{
	FH_SINT32 ret;
    FH_SINT32 i;
    FH_SINT32 j;
    FH_UINT32 mbx;
    FH_UINT32 mby;
    FH_UINT32 mbw;
    FH_UINT32 mbh;
    FH_UINT32 md_sum = 0;
    FH_UINT8* confidence;
    FHT_MDConfig_Ex_Result_t result;

	if (!g_md_cd_lock_inited)
    	return -1;

	pthread_mutex_lock(&g_md_cd_lock);
	ret = FHAdv_MD_Ex_GetResult(&result); /* 获取设置全局运动检测结果 */
    if (ret != FH_SUCCESS)
    {
    	pthread_mutex_unlock(&g_md_cd_lock);
    	printf("Error: FHAdv_MD_Ex_GetResult failed, ret=%d\n", ret);
    	return -1;
    }

	mbx = (x * (result.horizontal_count * (MDEX_MBNUM_H*16)) / detect_frame_w) / (MDEX_MBNUM_H*16);
	mby = (y * (result.vertical_count   * (MDEX_MBNUM_V*16)) / detect_frame_h) / (MDEX_MBNUM_V*16);
	mbw = (w * (result.horizontal_count * (MDEX_MBNUM_H*16)) / detect_frame_w + (MDEX_MBNUM_H*16-1)) / (MDEX_MBNUM_H*16);
	mbh = (h * (result.vertical_count   * (MDEX_MBNUM_V*16)) / detect_frame_h + (MDEX_MBNUM_V*16-1)) / (MDEX_MBNUM_V*16);

    confidence = result.start + mby * result.horizontal_count + mbx;

    for (i = 0; i < mbh; i++)
    {
        for (j = 0; j < mbw; j++)
        {
            md_sum += confidence[j];
        }
        confidence += result.horizontal_count;
    }

	pthread_mutex_unlock(&g_md_cd_lock);

	*sum = md_sum;
	*blk_num = mbw * mbh;

	return 0;
}

FH_SINT32 sample_md_cd_start(FH_VOID)
{
    FH_SINT32 ret;
    pthread_t thread;
    pthread_attr_t attr;

    if (g_md_cd_is_running)
    {
        printf("Error: MD/CD already running!\n");
        return -1;
    }

    g_check_md = 0;
    g_check_md_ex = 0;
    g_check_cd = 0;

    if (!g_md_cd_lock_inited)
    {
        pthread_mutex_init(&g_md_cd_lock, NULL);
        g_md_cd_lock_inited = 1;
    }

#ifdef FH_APP_OPEN_RECT_MOTION_DETECT
    ret = sample_md_init();
    if (ret == 0)
        g_check_md = 1;
#endif

#ifdef FH_APP_OPEN_MOTION_DETECT
    ret = sample_md_ex_init();
    if (ret == 0)
        g_check_md_ex = 1;
#endif

#ifdef FH_APP_OPEN_COVER_DETECT
    ret = sample_cd_init();
    if (ret == 0)
        g_check_cd = 1;
#endif

    if (g_check_md || g_check_md_ex || g_check_cd)
    {
        g_request_to_stop = 0;
        g_md_cd_is_running = 1;
        FHAdv_MD_Set_Check_Mode(FH_MD_TIME_CHEAK); /*通过调用此函数配置为使用时间间隔,如果不调用,就是缺省的帧间隔*/
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        ret = pthread_create(&thread, &attr, md_cd_task, NULL);
        if (ret)
        {
            printf("[ERROR]: create md/cd thread failed, ret=%d\n", ret);
            g_md_cd_is_running = 0;
            return ret;
        }
    }

    return 0;
}

FH_SINT32 sample_md_cd_stop(FH_VOID)
{
    while (g_md_cd_is_running)
    {
        g_request_to_stop = 1;
        usleep(20*1000);
    }

    g_check_md = 0;
    g_check_md_ex = 0;
    g_check_cd = 0;

    return 0;
}
