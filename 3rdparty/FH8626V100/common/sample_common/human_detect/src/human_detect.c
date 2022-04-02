/* 适用芯片: FH8626V100、FH885XV200系列 */

#include "sample_common.h"
#ifdef FH_APP_OPEN_OBJECT_DETECT

//#define FH_APP_OBJDETECT_STATISTICS

#if defined(CONFIG_ARCH_FH8626V100) || \
    defined(CONFIG_ARCH_FH885xV200)

#include "human_detect/fh_imgprocess_vbcli.h"
#include "human_config.h"
#include "model.h"

#ifdef FH_APP_OBJDETECT_USE_PINGPONG_BUF
#define DETECT_BUF_NUM (2)
#else
#define DETECT_BUF_NUM (1)
#endif

#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct
{
    FH_UINT32 u32X;
    FH_UINT32 u32Y;
    FH_UINT32 u32Width;
    FH_UINT32 u32Height;
} Detect_Rect;

struct buf_desc
{
    FH_SINT32  valid;         /*buf缓冲区有效标志,1:已有一帧有效数据,可以做人形检测, 0:buf缓冲区无效*/
    MEM_DESC   buf;           /*从VMM分配的缓冲区,用来存储用于人形检测的一帧原始图像的Y数据*/

    Detect_Rect detect_rect;  /*人形检测的区域,可以全屏检测,也可以是基于运动或BGM的一个子区域的检测*/
};

extern int fh_rpc_hd_init(void);

static struct human_detect_config g_human_config;

static volatile FH_SINT32 g_vbus_cli_inited;          /*gvbus功能初始化标志*/
static volatile FH_SINT32 g_human_detect_stop;        /*设置此标志，让人形检测线程退出*/
static volatile FH_SINT32 g_human_detect_pre_running; /*用于人形检测的获取图像的线程是否正在运行*/
static volatile FH_SINT32 g_human_detect_running;     /*人形检测、人形结果处理的线程是否正在运行*/

static struct FH_ODET_Handle *odet = NULL;     /*人形句柄*/
static struct buf_desc        DETECT_BUF[DETECT_BUF_NUM]; /*人形检测数据buffer*/
static MEM_DESC               FACE_DETECT_BUF; /*人脸检测数据buffer*/

static volatile FH_SINT32 g_head_thresh;
static volatile FH_SINT32 g_body_thresh;
static volatile FH_SINT32 g_face_thresh;

/*模型序列号*/
static FH_UINT32 model_serial[] = {MODEL_0_SERIAL_NUM, MODEL_1_SERIAL_NUM, MODEL_2_SERIAL_NUM, MODEL_3_SERIAL_NUM};

/*模型阈值*/
static FH_UINT32 model_thresh[] = {MODEL_0_THRESH,     MODEL_1_THRESH,     MODEL_2_THRESH,     MODEL_3_THRESH};

#ifdef FH_APP_OBJDETECT_STATISTICS
#define STATISTICS_DECLARE(var)        misc_perf_t var
#define STATISTICS_INIT(var,limit,msg) libmisc_perf_init(&var, limit, msg)
#define STATISTICS_ADD(var,msg)        libmisc_perf_add(&var,msg)
#else
#define STATISTICS_DECLARE(var)
#define STATISTICS_INIT(var,limit,msg)
#define STATISTICS_ADD(var,msg)
#endif

static FH_SINT32 get_outer_motion_area(struct buf_desc *desc)
{
    FH_UINT32 padLenH;
    FH_UINT32 padLenW;
    FH_UINT32 xEnd;
    FH_UINT32 yEnd;

    if (g_human_config.pre_filter_func &&
            g_human_config.pre_filter_func(
                HUMAN_DETECT_W,
                HUMAN_DETECT_H,
                &desc->detect_rect.u32X,
                &desc->detect_rect.u32Y,
                &desc->detect_rect.u32Width,
                &desc->detect_rect.u32Height) == 0)
    {
        /*printf("pre_filter:%d/%d/%d/%d.\n",
          desc->detect_rect.u32X,
          desc->detect_rect.u32Y,
          desc->detect_rect.u32Width,
          desc->detect_rect.u32Height);*/
        padLenH = MIN(desc->detect_rect.u32Width, desc->detect_rect.u32Height) >> 2;
        padLenW = padLenH << 1;

        /*add right border*/
        xEnd = desc->detect_rect.u32X + desc->detect_rect.u32Width + padLenW;
        if (xEnd > HUMAN_DETECT_W)
        {
            xEnd = HUMAN_DETECT_W;
        }

        /*add left border*/
        if (desc->detect_rect.u32X > padLenW)
        {
            desc->detect_rect.u32X -= padLenW;
        }
        else
        {
            desc->detect_rect.u32X = 0;
        }
        desc->detect_rect.u32Width = xEnd - desc->detect_rect.u32X;

        /*add bottom border*/
        yEnd = desc->detect_rect.u32Y + desc->detect_rect.u32Height + padLenH;
        if (yEnd > HUMAN_DETECT_H)
        {
            yEnd = HUMAN_DETECT_H;
        }

        /*add up border*/
        if (desc->detect_rect.u32Y > padLenH)
        {
            desc->detect_rect.u32Y -= padLenH;
        }
        else
        {
            desc->detect_rect.u32Y = 0;
        }
        desc->detect_rect.u32Height = yEnd - desc->detect_rect.u32Y;

        /*
         * for model 86185770, 42x102 is the minimum detect size
         * for model 86185422, 58x58  is the minimum detect size
         */
        if ( (desc->detect_rect.u32Width  >= 58 &&
                    desc->detect_rect.u32Height >= 58) ||
                (desc->detect_rect.u32Width  >= 42 &&
                 desc->detect_rect.u32Height >= 102) )
        {
            return 1;
        }

        return 0;
    }

    desc->detect_rect.u32X = 0;
    desc->detect_rect.u32Y = 0;
    desc->detect_rect.u32Width  = HUMAN_DETECT_W;
    desc->detect_rect.u32Height = HUMAN_DETECT_H;
    return 1;
}


static int alloc_human_buffer(void)
{
    FH_SINT32 ret;
    FH_SINT32 i;
    struct buf_desc *desc;

    for (i = 0; i < DETECT_BUF_NUM; i++)
    {
        desc = &DETECT_BUF[i];
        ret  = buffer_malloc_withname(&desc->buf, HUMAN_DETECT_W * HUMAN_DETECT_H, DEFALT_ALIGN, "APP_HUMAN_DETECT");
        if (ret)
        {
            goto Exit;
        }

        desc->valid                  = 0;
        desc->detect_rect.u32X       = 0;
        desc->detect_rect.u32Y       = 0;
        desc->detect_rect.u32Width   = HUMAN_DETECT_W;
        desc->detect_rect.u32Height  = HUMAN_DETECT_H;
    }

    if (g_human_config.init_face_detect)
    {
        ret = buffer_malloc_withname(&FACE_DETECT_BUF, ALIGNTO(FACE_DETECT_W, 16) * ALIGNTO(FACE_DETECT_H, 16), DEFALT_ALIGN, "APP_FACE_DETECT");
        if (ret)
        {
            goto Exit;
        }
    }

    return 0;

Exit:
    printf("Error: human, no VMM memory!\n");
    return -1;
}

static int free_human_buffer(void)
{
    FH_SINT32 i;
    struct buf_desc *desc;

    for (i=0; i<DETECT_BUF_NUM; i++)
    {
        /*Note: the VMM memory do not support free, so don't free it...*/
        desc = &DETECT_BUF[i];
        memset(desc, 0, sizeof(*desc));
    }

    return 0;
}

#ifdef FH_APP_OFFLINE_HUMAN_DETECT
static FILE *in_file  = FH_NULL;
static FILE *out_file = FH_NULL;

static FH_SINT32 read_yuv_file(FILE *fp, FH_UINT8 *dst_buf, FH_UINT32 in_frame_w, FH_UINT32 in_frame_h)
{
    FH_SINT32 i;
    FH_SINT32 ret;
    FH_UINT32 dst_stride = ALIGNTO(in_frame_w, 16);
    FH_UINT8 *curr_pos = dst_buf;

    for (i = 0; i < in_frame_h; i++)
    {
        memset(curr_pos, 0, sizeof(dst_stride));
        ret = fread(curr_pos, 1, in_frame_w * sizeof(FH_UINT8), fp);
        if (ret != in_frame_w * sizeof(FH_UINT8))
        {
            return -1;
        }
        curr_pos += dst_stride;
    }

#ifndef FH_APP_OFFLINE_INPUT_GREY_PIC
    fseek(fp, in_frame_w * in_frame_h / 2, SEEK_CUR);
#endif

    return 0;
}
static FH_SINT32 fill_one_frame_for_human(FH_SINT32 channel, FH_SINT32 do_yuv_trans, struct buf_desc *desc)
{
    FH_SINT32 ret;
    FH_UINT8 *y_buf = FH_NULL;

    STATISTICS_DECLARE(luma_trans);

    y_buf = malloc(ALIGNTO(FH_APP_OFFLINE_DETECT_W, 16) * ALIGNTO(FH_APP_OFFLINE_DETECT_H, 16) * sizeof(FH_UINT8));
    if (y_buf == FH_NULL)
    {
        printf("Error: malloc y buf for human fail\n");
        return -1;
    }

    if (read_yuv_file(in_file, y_buf, FH_APP_OFFLINE_DETECT_W, FH_APP_OFFLINE_DETECT_H))
    {
        return -1;
    }

    STATISTICS_INIT(luma_trans, 1, "luma_resize");
    ret = luma_resize(desc->buf.vbase,
            HUMAN_DETECT_W,
            HUMAN_DETECT_H,
            HUMAN_DETECT_W,
            y_buf,
            ALIGNTO(FH_APP_OFFLINE_DETECT_W, 16),
            ALIGNTO(FH_APP_OFFLINE_DETECT_H, 16),
            ALIGNTO(FH_APP_OFFLINE_DETECT_W, 16),
            0,
            0,
            0,
            0);


    STATISTICS_ADD(luma_trans, NULL);

    if (ret)
    {
        printf("Error: fill_one_frame_for_human!\n");
        return -1;
    }

    free(y_buf);

    return 0;
}
#else
static FH_SINT32 fill_one_frame_for_human(FH_SINT32 channel, FH_SINT32 do_yuv_trans, struct buf_desc *desc)
{
    FH_SINT32 ret;
    FH_VPU_STREAM vpuframe;

    STATISTICS_DECLARE(luma_trans);

    ret = FH_VPSS_GetChnFrame(channel, &vpuframe); /*获取一帧vpu数据*/
    if (RETURN_OK != ret)
    {
        printf("Error(%d - %x):FH_VPSS_GetChnFrame!\n", ret, ret);
        return ret;
    }

    if (!do_yuv_trans)
    {
        STATISTICS_INIT(luma_trans, 1, "luma_resize");
        ret = luma_resize(desc->buf.vbase,
                HUMAN_DETECT_W,
                HUMAN_DETECT_H,
                HUMAN_DETECT_W,
                (FH_UINT8 *)vpuframe.yluma.vbase,
                vpuframe.size.u32Width,
                vpuframe.size.u32Height,
                vpuframe.size.u32Width,
                0,
                0,
                0,
                0);
    }
    else
    {
        STATISTICS_INIT(luma_trans, 1, "luma_trans_resize");
        ret = luma_transform_and_resize(desc->buf.vbase,
                HUMAN_DETECT_W,
                HUMAN_DETECT_H,
                HUMAN_DETECT_W,
                (FH_UINT8 *)vpuframe.yluma.vbase,
                vpuframe.size.u32Width,
                vpuframe.size.u32Height,
                vpuframe.size.u32Width,
                0,
                0,
                0,
                0);
    }

    STATISTICS_ADD(luma_trans, NULL);

    if (ret)
    {
        printf("Error: fill_one_frame_for_human!\n");
    }

    return ret;
}
#endif
static FH_VOID change_thresh_hold(FH_VOID)
{
    FH_SINT32 ret0 = 0;
    FH_SINT32 ret1 = 0;
    FH_SINT32 ret2 = 0;

    if (g_head_thresh > 0)
        ret0 = VB_ODET_MODEL_setThresh(odet, MODEL_0_SERIAL_NUM, g_head_thresh);
    if (g_body_thresh > 0)
        ret1 = VB_ODET_MODEL_setThresh(odet, MODEL_1_SERIAL_NUM, g_body_thresh);
    if (g_face_thresh > 0)
        ret2 = VB_ODET_MODEL_setThresh(odet, FACE_MODEL_SERIAL, g_face_thresh);

    g_head_thresh = 0;
    g_body_thresh = 0;
    g_face_thresh = 0;

    if ((ret0 | ret1 | ret2) != 0)
    {
        printf("Error@human, change_thresh_hold,err=%d-%d-%d\n", ret0, ret1, ret2);
    }
}

static FH_SINT32 sample_odet_init(FH_VOID)
{
    FH_SINT32      ret;
    FH_SINT32      i;
    FH_ODET_cfg_t  cfg;

    if (HUMAN_MODEL_NUM + 1/*for face model*/ > MAX_MODEL_NUM)
    {
        printf("Error: too large HUMAN_MODEL_NUM!\n");
        return -1;
    }

    memset(&cfg, 0, sizeof(cfg));

    for (i = 0; i < HUMAN_MODEL_NUM; i++)
    {
        if (i >= sizeof(model_serial)/sizeof(FH_UINT32) || !model_serial[i])
        {
            printf("Error: human,model_serial[%d]=%d!\n", i, model_serial[i]);
            return -1;
        }

        ret = load_hd_model(&cfg.modelFile[i], &cfg.modelFileLen[i], model_serial[i]);
        if (ret != 0)
        {
            printf("Error: load model %d failed\n", model_serial[i]);
            return -1;
        }

        cfg.modelThres[i] =  model_thresh[i];
    }

    if (g_human_config.init_face_detect)
    {
        ret = load_hd_model(&cfg.modelFile[i], &cfg.modelFileLen[i], FACE_MODEL_SERIAL);
        if (ret != 0)
        {
            printf("Error: load model %d failed\n", FACE_MODEL_SERIAL);
            return -1;
        }
        cfg.modelThres[i]  = FACE_MODEL_THRESH;
        i++;
    }

    cfg.maxImageWidth  = HUMAN_DETECT_W;          /*人形检测幅面宽度*/
    cfg.maxImageHeight = HUMAN_DETECT_H;          /*人形检测幅面高度*/
    cfg.modelCnt       = i;                       /*人形检测参考模型数量，最大为5*/
    cfg.rotateAngle    = PIC_ROTATE;              /*人形检测旋转角度*/
    cfg.perScaleRate   = PER_SCALE_RATE;          /*缩放系数*/
    cfg.maxScaleNum    = MAX_SCALE_NUM;           /*最大缩放次数*/
    cfg.slideStep      = SLIDE_STEP;              /*滑窗步长，步长越大，检测时间缩短，检测率降低*/
    cfg.detectMode     = FH_OBJDET_FULL_FRAME;    /*人形检测模式*/
    cfg.model_mode     = USE_MODEL_MODE;          /*算法版本*/

    odet = VB_ODET_create(&cfg);              /*创建人形检测句柄*/
    if (odet == NULL)
    {
        printf("Error: VB_ODET_create failed!\n");
        return -1;
    }

    return 0;
}

/* 人形检测
 *  input:   detect_rect    检测区域
 *           model_num      使用的model数量
 *           out[i].modelId 检测使用的modelId
 *  output： out            人形检测结果
 *  return：  0             成功
 *           -1             失败
 */
static FH_SINT32 do_human_detect(struct buf_desc *desc, Detect_Rect *detect_rect, FH_ODET_objGroup_t *human_out, FH_UINT32 human_model_num)
{
    FH_SINT32     ret;
    FH_subImgY8_t roi;
    FH_imgY8_t   *human_detect_img;

    /********************************************
      1、创建Y8图像结构体变量
     *********************************************/
    human_detect_img = VB_IM_createBufferY8_ext(HUMAN_DETECT_W, HUMAN_DETECT_H);
    if (human_detect_img == NULL)
    {
        printf("Error: VB_IM_createBufferY8_ext\n");
        return -1;
    }

    /********************************************
      2、设置Y8结构体变量图像地址
     *********************************************/
    ret = VB_IM_fillBufferY8_ext(human_detect_img, (FH_UINT8 *)desc->buf.base);
    if (ret != FH_IM_SUCCESS)
    {
        printf("Error(%d): VB_IM_fillBufferY8_ext \n", ret);
        goto human_detect_exit;
    }

    /********************************************
      3、根据设置的检测区域裁剪图像获取roi数据
     *********************************************/
    ret = VB_IM_cropImageY8(human_detect_img, detect_rect->u32X, detect_rect->u32Y, detect_rect->u32Width, detect_rect->u32Height, &roi);
    if (ret != FH_IM_SUCCESS)
    {
        printf("Error(%d): VB_IM_cropImageY8 \n", ret);
        goto human_detect_exit;
    }

    /************************************************
      4、检测人形
     *************************************************/
    ret = VB_ODET_detectObject(odet, &roi, human_out, human_model_num);
    if (ret != FH_ODET_SUCCESS)
    {
        printf("Error(%d): VB_ODET_detectObject \n", ret);
        goto human_detect_exit;
    }

    /************************************************
      5、销毁创建的结构体数据并返回
     *************************************************/
human_detect_exit:
    VB_IM_destroyBufferY8_ext(human_detect_img);

    return ret;
}

/* 人脸检测
 *  input:   pObjPos        检测区域
 *           model_num      使用的model数量
 *           out[i].modelId 检测使用的modelId
 *           face_box_size  人脸检测幅面大小
 *  output： out            人脸检测结果
 *  return：  0             成功
 *           -1             失败
 */
#ifdef FH_APP_OBJDETECT_DO_FACE_DETECT
static FH_SINT32 do_face_detect(struct buf_desc *desc, FH_ODET_objPos_t *pObjPos, FH_ODET_objGroup_t *out, FH_UINT32 model_num, FH_UINT32 face_box_size)
{
    FH_SINT32 ret;
    FH_imgY8_t *human_box = FH_NULL;
    FH_imgY8_t *face_box  = FH_NULL;
    FH_subImgY8_t roi;

    STATISTICS_DECLARE(face_detect);

    /********************************************
      1、创建Y8图像结构体变量，存储人形检测数据
     *********************************************/
    human_box = VB_IM_createBufferY8_ext(pObjPos->w, pObjPos->h);
    if (human_box == NULL)
    {
        printf("Error: VB_IM_createBufferY8_ext\n");
        return -1;
    }

    /********************************************
      2、创建Y8图像结构体变量，存储人脸检测数据
     *********************************************/
    face_box = VB_IM_createBufferY8_ext(face_box_size, face_box_size);
    if (face_box == NULL)
    {
        printf("Error: VB_IM_createBufferY8_ext\n");
        ret = -1;
        goto face_detect_exit;
    }

    /********************************************
      3、设置人形检测Y8结构体数据的图像地址
     *********************************************/
    ret = VB_IM_fillBufferY8_ext(human_box, (FH_UINT8 *)(desc->buf.base) + pObjPos->y * HUMAN_DETECT_W  + pObjPos->x);
    if (ret != FH_IM_SUCCESS)
    {
        printf("Error(%d): VB_IM_fillBufferY8_ext \n", ret);
        goto face_detect_exit;
    }

    /********************************************
      4、设置人脸检测Y8结构体数据的图像地址
     *********************************************/
    ret = VB_IM_fillBufferY8_ext(face_box, (FH_UINT8 *)FACE_DETECT_BUF.base);
    if (ret != FH_IM_SUCCESS)
    {
        printf("Error(%d): VB_IM_fillBufferY8_ext \n", ret);
        goto face_detect_exit;
    }

    STATISTICS_INIT(face_detect, 1, "face-");

    /***********************************************************************
      5、将包含人形区域的图像数据按照指定的人脸检测大小resize到人脸检测数据buffer中
     ************************************************************************/
    ret = VB_IM_resizeBilinearY8(human_box, HUMAN_DETECT_W, face_box);
    if (ret != FH_IM_SUCCESS)
    {
        printf("Error(%d): VB_IM_resizeBilinearY8 \n", ret);
        goto face_detect_exit;
    }

    STATISTICS_ADD(face_detect, "resize");

    /***********************************************************************
      6、按照指定的人脸检测size裁剪出roi
     ************************************************************************/
    ret = VB_IM_cropImageY8(face_box, 0, 0, face_box_size, face_box_size, &roi);
    if (ret != FH_IM_SUCCESS)
    {
        printf("Error(%d): VB_IM_cropImageY8 \n", ret);
        goto face_detect_exit;
    }

    /***********************************************************************
      8、检测人脸
     ************************************************************************/
    ret = VB_ODET_detectObject(odet, &roi, out, model_num);
    if (ret != FH_ODET_SUCCESS)
    {
        printf("Error(%d): VB_ODET_detectObject \n", ret);
        goto face_detect_exit;
    }

    STATISTICS_ADD(face_detect, "detect");

    /************************************************
      9、销毁创建的结构体数据并推出
     *************************************************/
face_detect_exit:
    if (face_box != FH_NULL)
        VB_IM_destroyBufferY8_ext(face_box);
    if (human_box != FH_NULL)
        VB_IM_destroyBufferY8_ext(human_box);

    return ret;
}
#endif /*FH_APP_OBJDETECT_DO_FACE_DETECT*/

static FH_SINT32 handle_human_result(
        struct buf_desc *desc,
        FH_ODET_objGroup_t *human_out,
        FH_UINT32 human_model_num)
{
    FH_SINT32 i;
    FH_SINT32 j;
    FH_SINT32 ret;
    FH_ODET_objGroup_t face_out;
    FH_ODET_objPos_t   *pObjPos;
    FH_ODET_objPos_t   *pFacePos;
    FH_SINT32 obj_count = 0;
    FH_SINT32 face_count = 0;
    FH_SINT32 do_face_count = 0;

    if (g_human_config.process_human_func)
    {
        g_human_config.process_human_func(HUMAN_DETECT_W, HUMAN_DETECT_H,
                obj_count++, 0, 0, NULL, NULL);
    }

    /*处理人形检测结果*/
    for (i = 0; i < human_model_num; i++)
    {
        for (j = 0; j < human_out[i].num; j++)
        {
            pObjPos = &human_out[i].objlist[j].pos;

            if (g_human_config.post_filter_func &&
                    g_human_config.post_filter_func(
                        HUMAN_DETECT_W,
                        HUMAN_DETECT_H,
                        pObjPos->x,
                        pObjPos->y,
                        pObjPos->w,
                        pObjPos->h) != 0)
                continue;

            if (g_human_config.process_human_func)
            {
                ret = g_human_config.process_human_func(
                        HUMAN_DETECT_W,
                        HUMAN_DETECT_H,
                        obj_count++,
                        human_out[i].modelId,
                        human_out[i].objlist[j].confidence,
                        pObjPos,
                        desc->buf.vbase);
                if (ret == 1) /*indicate to end*/
                {
                    goto Exit;
                }
            }

#ifdef FH_APP_OBJDETECT_DO_FACE_DETECT
            if (!g_human_config.do_face_detect || pObjPos->w > pObjPos->h)
            {
                continue; /*don't do face*/
            }

            if (do_face_count++ == 0)
            {
                /***********************************************************************
                  7、设置人脸检测搜索参数
                 ************************************************************************/
                ret = VB_ODET_setSampleWinPara(odet, 200, 40, 1);
                if (ret != FH_IM_SUCCESS)
                {
                    printf("Error(%d): VB_ODET_setSampleWinPara \n", ret);
                }
            }

            /*检测人脸*/
            pObjPos->h = pObjPos->w;
            face_out.modelId = FACE_MODEL_SERIAL;
            ret = do_face_detect(desc, pObjPos, &face_out, 1, FACE_DETECT_W);
            if (ret != 0 || face_out.num <= 0)
            {
                continue;
            }

            if (g_human_config.process_face_func)
            {
                pFacePos = &face_out.objlist[0].pos;
                pFacePos->x = (FH_UINT32)pFacePos->x * (FH_UINT32)pObjPos->w / FACE_DETECT_W + pObjPos->x;
                pFacePos->y = (FH_UINT32)pFacePos->y * (FH_UINT32)pObjPos->h / FACE_DETECT_H + pObjPos->y;
                pFacePos->w = (FH_UINT32)pFacePos->w * (FH_UINT32)pObjPos->w / FACE_DETECT_W;
                pFacePos->h = (FH_UINT32)pFacePos->h * (FH_UINT32)pObjPos->h / FACE_DETECT_H;

                ret = g_human_config.process_face_func(
                        HUMAN_DETECT_W,
                        HUMAN_DETECT_H,
                        ++face_count,
                        face_out.modelId,
                        face_out.objlist[0].confidence,
                        pFacePos,
                        desc->buf.vbase);
                if (ret == 1) /*indicate to end*/
                {
                    goto Exit;
                }
            }
#endif /*FH_APP_OBJDETECT_DO_FACE_DETECT*/
        }
    }

#ifdef FH_APP_OFFLINE_HUMAN_DETECT
    fwrite(desc->buf.vbase, 1, HUMAN_DETECT_W * HUMAN_DETECT_H, out_file);
#endif

    /************************************************
      5、设置人形检测算法搜索参数, 人脸检测会修改这些参数
     *************************************************/
Exit:
    if (do_face_count > 0)
    {
        ret = VB_ODET_setSampleWinPara(odet, PER_SCALE_RATE, MAX_SCALE_NUM, SLIDE_STEP);
        if (ret != FH_ODET_SUCCESS)
        {
            printf("Error(%d): VB_ODET_setSampleWinPara \n", ret);
        }
    }

    return 0;
}

/*****************************************************************************
  函 数 名:   sample_hd_task
  功能描述  : 人形人脸检测线程函数
  输入参数  : void *args
  输出参数  : 无
  返 回 值  : void*
 *****************************************************************************/
static FH_VOID *human_check_thread_proc(FH_VOID *args)
{
    FH_SINT32 ret;
    FH_UINT32 i;
    FH_UINT32 model_num;
    FH_UINT32 waitcnt = 0;
    FH_SINT32 detect_buf_index = 0;

    FH_ODET_objGroup_t *human_out = NULL;
    struct buf_desc    *desc;

    STATISTICS_DECLARE(human_detect);

    ret = sample_odet_init(); /*初始化人形功能*/
    if (ret)
        goto Exit;

    model_num = HUMAN_MODEL_NUM;
    if ((human_out = malloc(model_num * sizeof(FH_ODET_objGroup_t))) == FH_NULL)
    {
        goto Exit;
    }

    for (i = 0; i < model_num; i++)
    {
        human_out[i].modelId = model_serial[i];
    }

    STATISTICS_INIT(human_detect, 10, "human-detect");

    while (!g_human_detect_stop)
    {
        if ((g_head_thresh | g_body_thresh | g_face_thresh) != 0)
        {
            change_thresh_hold();
        }

#ifdef FH_APP_OBJDETECT_USE_PINGPONG_BUF
        desc = &DETECT_BUF[detect_buf_index];
        if (!desc->valid)
        {
            usleep(g_human_config.do_human_detect ? 20*1000 : 200*1000);
            waitcnt++;
            continue;
        }
        else
        {
            if (waitcnt > 0)
            {
                printf("Human: wait pic to check(%d).\n", waitcnt);
                waitcnt = 0;
            }
        }
#else
        if (!g_human_config.do_human_detect)
        {
            usleep(200*1000);
            continue;
        }

        desc = &DETECT_BUF[0];

        if (!get_outer_motion_area(desc))
        {
            usleep(40*1000); /*wait for the next frame and check it*/
            continue;
        }

        ret = fill_one_frame_for_human(g_human_config.human_detect_chan, g_human_config.do_yuv_trans, desc);
        if (ret)
        {
            usleep(1000*1000);
            continue;
        }
        desc->valid = 1;
#endif

        ret = do_human_detect(desc, &desc->detect_rect, human_out, model_num); /*做一帧人形检测*/
        if (ret == 0)
        {
            handle_human_result(desc, human_out, model_num);
        }

        STATISTICS_ADD(human_detect, NULL);

        desc->valid = 0;
        detect_buf_index = 1 - detect_buf_index; /*pingpong buffer switch*/
    }

Exit:
    if (human_out)
    {
        free(human_out);
    }
    g_human_detect_running = 0;
    return NULL;
}

/*****************************************************************************
  函 数 名:   sample_hd_data_process_thread
  功能描述  : 获取vpu yuv数据并处理用于人形检测
  输入参数  : *args
  输出参数  : 无
  返 回 值  : void*
 *****************************************************************************/
#ifdef FH_APP_OBJDETECT_USE_PINGPONG_BUF
static FH_VOID *human_pre_thread_proc(FH_VOID *args)
{
    FH_SINT32 ret;
    FH_SINT32 detect_buf_index = 0;
    FH_UINT32 do_yuv_trans;
    struct buf_desc *desc;

    while (!g_human_detect_stop)
    {
        if (!g_human_config.do_human_detect)
        {
            usleep(200*1000);
            continue;
        }

        desc = &DETECT_BUF[detect_buf_index];
        if (desc->valid)
        {
            usleep(30*1000);
            continue;
        }

        if (!get_outer_motion_area(desc))
        {
            usleep(40*1000); /*wait for the next frame and check it*/
            continue;
        }

        ret = fill_one_frame_for_human(g_human_config.human_detect_chan, g_human_config.do_yuv_trans, desc);
        if (ret)
        {
            usleep(1000*1000);
            continue;
        }

        desc->valid = 1;
        detect_buf_index = 1 - detect_buf_index;
    }

    g_human_detect_pre_running = 0;
    return NULL;
}
#endif

/*****************************************************************************
  函 数 名:   sample_start_human_detect
  功能描述  : 启动人形检测功能
  输入参数  : 无
  输出参数  : 无
  返 回 值  : void*
 *****************************************************************************/
FH_SINT32 human_detect_start(struct human_detect_config *pHumanConfig)
{
    FH_UINT32 ret;
    pthread_t human_th;
    pthread_attr_t attr;

    if (!pHumanConfig)
        goto Exit;

    if (g_human_detect_pre_running || g_human_detect_running)
    {
        printf("human detect is already running!\n");
        goto Exit;
    }

    g_human_detect_stop = 0;
    g_human_config = *pHumanConfig;
    if (g_human_config.do_face_detect)
        g_human_config.init_face_detect = 1; /*force to init*/

    ret = alloc_human_buffer();
    if (ret)
    {
        goto Exit;
    }

#ifdef FH_APP_OFFLINE_HUMAN_DETECT
    in_file = fopen(FH_APP_OFFLINE_INPUT_FILE_NAME, "rb");
    if (in_file == FH_NULL)
    {
        printf("Error: open file %s for offline human detect fail\n", FH_APP_OFFLINE_INPUT_FILE_NAME);
        goto Exit;
    }

    out_file = fopen(FH_APP_OFFLINE_OUTPUT_FILE_NAME, "wb");
    if (out_file == FH_NULL)
    {
        printf("Error: open file %s for offline human detect fail\n", FH_APP_OFFLINE_OUTPUT_FILE_NAME);
        goto Exit;
    }
#endif

#ifdef FH_APP_OBJDETECT_USE_PINGPONG_BUF
    pthread_t human_pre_th;

    g_human_detect_pre_running = 1;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&human_pre_th, &attr, human_pre_thread_proc, NULL);
    if (ret)
    {
        g_human_detect_pre_running = 0;
        printf("Error: create human-pre thread failed!\n");
        goto Exit;
    }
#endif

    /*创建人形人脸检测线程*/
    g_human_detect_running = 1;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 0x1000);
    ret = pthread_create(&human_th, &attr, human_check_thread_proc, NULL);
    if (ret)
    {
        g_human_detect_running = 0;
        printf("Error: create human-check thread failed!\n");
        goto Exit;
    }

    return 0;
Exit:
    return -1;
}

/*****************************************************************************
  函 数 名:   sample_stop_human_detect
  功能描述  : 退出人形检测功能
  输入参数  : 无
  输出参数  : 无
  返 回 值  : void*
 *****************************************************************************/
FH_SINT32 human_detect_stop(FH_VOID)
{
    FH_SINT32 ret;

    g_human_detect_stop = 1;

    while (g_human_detect_pre_running + g_human_detect_running != 0)
    {
        usleep(20*1000);
    }

    if (odet != NULL)
    {
        ret = VB_ODET_destroy(odet); /*线程结束，销毁人形检测句柄*/
        if (ret != FH_ODET_SUCCESS)
            printf("Error(%d): VB_ODET_destroy!\n", ret);
        odet = NULL;
    }

#ifdef FH_APP_OFFLINE_HUMAN_DETECT
    if (in_file != FH_NULL)
    {
        fclose(in_file);
        in_file = FH_NULL;
    }

    if (out_file != FH_NULL)
    {
        fclose(out_file);
        out_file = FH_NULL;
    }
#endif

    free_human_buffer();

    return 0;
}

FH_SINT32 human_detect_control(FH_SINT32 do_human_detect, FH_SINT32 do_face_detect)
{
    if (!g_human_config.init_face_detect)
    {
        do_face_detect = 0; /*if face is not inited, force to disable*/
    }

    g_human_config.do_human_detect = do_human_detect;
    g_human_config.do_face_detect = do_face_detect;
    return 0;
}

FH_SINT32 human_detect_set_thresh(FH_SINT32 head_thresh, FH_SINT32 body_thresh, FH_SINT32 face_thresh)
{
    while ((g_head_thresh | g_body_thresh | g_face_thresh) != 0)
    {
        usleep(20*1000);
    }

    g_head_thresh = head_thresh;
    g_body_thresh = body_thresh;
    g_face_thresh = face_thresh;

    return 0;
}

#endif /*defined(CONFIG_ARCH_FH8626V100) ...*/

#endif /*FH_APP_OPEN_OBJECT_DETECT*/
