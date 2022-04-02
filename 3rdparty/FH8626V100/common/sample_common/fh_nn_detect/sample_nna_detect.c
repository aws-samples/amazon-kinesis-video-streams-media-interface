/* 适用芯片: FH865x系列 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#include "types/bufCtrl.h"
#include "dsp/fh_system_mpi.h"
#include "dsp/fh_nna_mpi.h"
#include "dsp/fh_vpu_mpi.h"
#include "dsp/fh_venc_mpi.h"
#include "types/type_def.h"
#include "sample_common.h"
#include "sample_nna_detect.h"

#define RGB_CHAN (3) /* 只有通道3支持RGB输出格式 */

#define RGB_CHAN_WIDTH (512)
#define RGB_CHAN_HEIGHT (288)

#define NN_RGB_W (512)
#define NN_RGB_H (288)
#define DRAW_BOX_CHAN (0)

#define ALIGNTO(addr, edge) ((addr + edge - 1) & ~(edge - 1))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLIP(x, min, max) MAX((min), MIN((x), (max)))

extern FH_SINT32 sample_set_gbox(FH_UINT32 chan, FH_UINT32 enable, FH_UINT32 box_id, FH_UINT32 x, FH_UINT32 y, FH_UINT32 w, FH_UINT32 h, FHT_RgbColor_t color);

static FH_SINT32 g_stop_nna_detect = 0;
static FH_SINT32 g_nna_running = 0;

static FH_UINT8 *load_nn_models(FH_CHAR *modelPath)
{
    FH_SINT32 readLen;
    FH_SINT32 fileLen;
    FILE *fp;

    if (!(fp = fopen(modelPath, "rb")))
    {
        printf("Error[FH_NN]: open %s failed\n", modelPath);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    fileLen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    FH_UINT8 *buf;

    buf = malloc(fileLen);
    if(buf == NULL)
    {
        printf("Error[FH_NN]: malloc model buf failed, size = %d\n", fileLen);
        fclose(fp);
        return NULL;
    }

    readLen = fread(buf, 1, fileLen, fp);
    if (readLen < fileLen)
    {
        printf("Error[FH_NN]: read model file failed, fileLen = %d, readLen = %d\n", fileLen, readLen);
        free(buf);
        buf = NULL;
    }

    fclose(fp);
    return buf;
}

static void free_nn_models(FH_UINT8 *buf)
{
    free(buf);
}

static FH_SINT32 compare_rect(FH_RECT_T rect1, FH_RECT_T rect2)
{
    FH_SINT32 d1;
    FH_SINT32 d2;
    FH_SINT32 d3;
    FH_SINT32 d4;
    FH_SINT32 thresh = 0.1;

    d1 = (FH_SINT32)(rect1.x - rect2.x) > 0 ? (rect1.x - rect2.x) : (rect2.x - rect1.x);
    if (((float)d1 / (float)rect1.w)  > thresh)
    {
        return 1;
    }

    d2 = (FH_SINT32)(rect1.y - rect2.y) > 0 ? (rect1.y - rect2.y) : (rect2.y - rect1.y);
    if (((float)d2 / (float)rect1.h)  > thresh)
    {
        return 1;
    }

    d3 = (FH_SINT32)(rect1.w - rect2.w) > 0 ? (rect1.w - rect2.w) : (rect2.w - rect1.w);
    if (((float)d3 / (float)rect1.w)  > thresh)
    {
        return 1;
    }

    d4 = (FH_SINT32)(rect1.h - rect2.h) > 0 ? (rect1.h - rect2.h) : (rect2.h -  rect1.h);
    if (((float)d4 / (float)rect1.h)  > thresh)
    {
        return 1;
    }

    return 0;

}

static FH_VOID draw_box(FH_DETECTION_T *out, FH_UINT32 box_chnw, FH_UINT32 box_chnh)
{
    static FH_DETECTION_T out_last = {0};
    FH_SINT32 i;
    FH_SINT32 dt_x;
    FH_SINT32 dt_y;
    FH_SINT32 dt_w;
    FH_SINT32 dt_h;

    FHT_RgbColor_t box_color = {255, 0, 0, 255};

    for (i = 0; i < out->boxNum; i++) {
        /*draw gosd rect*/
        out->detBBox[i].bbox.x *= box_chnw;
        out->detBBox[i].bbox.y *= box_chnh;
        out->detBBox[i].bbox.w *= box_chnw;
        out->detBBox[i].bbox.h *= box_chnh;

        if (!compare_rect(out_last.detBBox[i].bbox, out->detBBox[i].bbox))
        {
           continue;
        }
        else
        {
            out_last.detBBox[i].bbox = out->detBBox[i].bbox;
        }

        dt_x = out->detBBox[i].bbox.x;
        dt_y = out->detBBox[i].bbox.y;
        dt_w = out->detBBox[i].bbox.w;
        dt_h = out->detBBox[i].bbox.h;

        sample_set_gbox(0, 1, i, dt_x, dt_y, dt_w, dt_h, box_color);
    }

    for (; i < out_last.boxNum; i++)
    {
        sample_set_gbox(0, 0, i, 0, 0, 0, 0, box_color);
    }

    out_last.boxNum = out->boxNum;
}

static FH_SINT32 sample_nna_deinit(FH_VOID *detHandle)
{
    int ret;

    if(detHandle)
    {
        ret = FH_NNA_DET_Exit(detHandle);
        if(ret != RETURN_OK)
        {
            printf("Error[FH_NN]: FH_NNA_DET_Exit failed, ret = %x\n", ret);
        }
    }

    return ret;
}

static FH_VOID *sample_init_fh_nn(FH_VOID)
{
    FH_UINT32 ret;
    FH_VOID *detHandle = NULL;
    FH_CHAR modelPath[256];
    FH_NN_INIT_PARAM_T nna_param;

    nna_param.src_w_in = NN_RGB_W;
    nna_param.src_h_in = NN_RGB_H;
    nna_param.src_c_in = 4;
    nna_param.type     = FN_DET_TYPE_PERSON;
    nna_param.conf_thr = 0.5;
    nna_param.rotate   = FN_ROT_0;

    if (nna_param.type == FN_DET_TYPE_PERSON){
        strcpy(modelPath, "./pedestrian/persondet.nbg");
    }
    else{
        printf("Invalid detect type %d\n",nna_param.type);
        return NULL;
    }

    nna_param.nbg_data = load_nn_models(modelPath);
    if (!nna_param.nbg_data)
    {
        return NULL;
    }

    ret = FH_NNA_DET_Init(&detHandle, 0,  &nna_param);
    if(ret)
    {
        printf("FH_NNA_DET_Init failed with %x\n", ret);
        detHandle = NULL;
    }

    free_nn_models(nna_param.nbg_data);

    printf("FH_NNA_DET_Init ok !!! %p\n", detHandle);

    return detHandle;
}

static FH_SINT32 sample_create_rgb_chan(FH_VOID)
{
    FH_SINT32 ret;
    FH_VPU_CHN_CONFIG rgb_chan_attr;

    rgb_chan_attr.vpu_chn_size.u32Width = ALIGNTO(RGB_CHAN_WIDTH, 16);
    rgb_chan_attr.vpu_chn_size.u32Height = ALIGNTO(RGB_CHAN_HEIGHT, 16);
    ret = FH_VPSS_SetChnAttr(RGB_CHAN, &rgb_chan_attr);
    if (ret != 0)
    {
         printf("Error[FH_NN]: FH_VPSS_SetChnAttr fail, ret = %x\n", ret);
         return ret;
    }

    ret = FH_VPSS_SetVOMode(RGB_CHAN, VPU_VOMODE_RGB888);
    if (ret != 0)
    {
         printf("Error[FH_NN]: FH_VPSS_SetVOMode fail, ret = %x\n", ret);
         return ret;
    }

    ret = FH_VPSS_OpenChn(RGB_CHAN);
    if (ret != 0)
    {
         printf("Error[FH_NN]: FH_VPSS_OpenChn fail, ret = %x\n", ret);
         return ret;
    }

    ret = FH_SYS_BindVpu2Nn(0);
    if (ret != 0)
    {
         printf("Error[FH_NN]: FH_SYS_BindVpu2Nn fail, ret = %x\n", ret);
    }

    return ret;
}

/* 512 * 288 */
FH_VOID *thread_nn_detect(FH_VOID *args)
{
    FH_SINT32 ret;
    FH_VOID *detHandle = NULL;
    FH_DETECTION_T nn_out;

    detHandle = sample_init_fh_nn();
    if (detHandle == NULL)
    {
        printf("Error[FH_NN]: sample_init_fh_nn failed\n");
        goto Exit;
    }

    ret = sample_create_rgb_chan();
    if (ret)
    {
        goto Exit;
    }

    FH_UINT32 box_chn_w;
    FH_UINT32 box_chn_h;
    FH_VPU_CHN_CONFIG box_chan_cfg;
    ret = FH_VPSS_GetChnAttr(DRAW_BOX_CHAN, &box_chan_cfg);
    if(ret)
    {
        printf("Error[FH_NN]: FH_VPSS_GetChnAttr failed, ret = %x\n", ret);
        goto Exit;
    }

    box_chn_w = box_chan_cfg.vpu_chn_size.u32Width;
    box_chn_h = box_chan_cfg.vpu_chn_size.u32Height;

    while (!g_stop_nna_detect)
    {
        ret = FH_NNA_DET_Forward(detHandle, &nn_out);
        if(ret)
        {
            printf("Error[FH_NN]: FH_NNA_DET_Process failed, ret = %x\n", ret);
            goto Exit;
        }

        draw_box(&nn_out, box_chn_w, box_chn_h);
    }

Exit:
    g_nna_running = 0;
    sample_nna_deinit(detHandle);
    FH_SYS_UnBindbySrc(DRAW_BOX_CHAN);
    FH_VPSS_CloseChn(DRAW_BOX_CHAN);

    return NULL;
}

FH_SINT32 sample_fh_nn_obj_detect_start(FH_VOID)
{
    pthread_t nn_thread;
    pthread_attr_t nn_attr;
    if (g_nna_running)
    {
        printf("[FH_NN] nna demo is running\n");
        return 0;
    }

    g_stop_nna_detect = 0;

    pthread_attr_init(&nn_attr);

    pthread_attr_setdetachstate(&nn_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&nn_attr, 15 * 1024);
    if (pthread_create(&nn_thread, &nn_attr, thread_nn_detect, NULL))
    {
        printf("Error[FH_NN]: pthread_create failed\n");
        goto Exit;
    }

    g_nna_running = 1;

    return 0;

Exit:
    return -1;
}

FH_SINT32 sample_fh_nn_obj_detect_stop(FH_VOID)
{
    g_stop_nna_detect  = 1;

    while (g_nna_running)
    {
        usleep(10);
    }

    return 0;
}
