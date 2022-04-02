/* 适用芯片: YG */

#include "sample_common.h"

#ifdef CONFIG_ARCH_YG
#include "human_detect_nn.h"
#include "fh_nn_det.h"

/* *******MATH ALGO******* */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLIP(x, min, max) MAX((min), MIN((x), (max)))
#define ALIGNTO(addr, edge) ((addr + edge - 1) & ~(edge - 1))


/* *******NN DETECT CONFIG******* */
#define RGB_CHAN      2
#define DRAW_BOX_CHAN 0

static FH_SINT32 g_stop_nn_detect  = 1;
static FH_SINT32 g_stop_nn_demo    = 1;
static FH_SINT32 g_nn_demo_running = 0;
static float w_ratio;
static float h_ratio;

extern FH_SINT32 FH_VPSS_GetChnFrame_Ex(FH_UINT32 chan,
                                        FH_VPU_STREAM *pstVpuframeinfo,
                                        FH_UINT32 timeout_ms);
extern FH_SINT32 FH_SYS_MmzFlushCache(FH_UINT32 u32PhyAddr, FH_VOID *pVirtAddr, FH_UINT32 u32Size);

FH_SINT32 compare_rect(FH_RECT_t rect1, FH_RECT_t rect2)
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

FH_UINT32 _ARGB2AYCbCr(FH_UINT32 A, FH_UINT32 R, FH_UINT32 G, FH_UINT32 B)
{
    FH_UINT32 Y = (FH_UINT32)(0.299 * R + 0.587 * G + 0.114 * B);
    FH_UINT32 Cr = (FH_UINT32)((0.500*R - 0.4187*G - 0.0813*B) + 128);
    FH_UINT32 Cb = (FH_UINT32)((-0.1687*R - 0.3313*G + 0.500*B) + 128);

    return (((A & 0xff) << 24) | ((Y & 0xff) << 16) | ((Cb & 0xff) << 8) | ((Cr & 0xff)));
}

FH_SINT32 yuv_draw_rect_with_block_format(FH_UINT8 *src_y, FH_UINT8 *src_c, FH_UINT32 w, FH_UINT32 h,
    FH_UINT32 rx, FH_UINT32 ry, FH_UINT32 rw, FH_UINT32 rh, FH_UINT32 ycbcr)
{
    FH_UINT32 offset[256] = {
        0, 1, 2, 3, 4, 5, 6, 7, 64, 65, 66, 67, 68, 69, 70, 71,
        8, 9, 10, 11, 12, 13, 14, 15, 72, 73, 74, 75, 76, 77, 78, 79,
        16, 17, 18, 19, 20, 21, 22, 23, 80, 81, 82, 83, 84, 85, 86, 87,
        24, 25, 26, 27, 28, 29, 30, 31, 88, 89, 90, 91, 92, 93, 94, 95,
        32, 33, 34, 35, 36, 37, 38, 39, 96, 97, 98, 99, 100, 101, 102, 103,
        40, 41, 42, 43, 44, 45, 46, 47, 104, 105, 106, 107, 108, 109, 110, 111,
        48, 49, 50, 51, 52, 53, 54, 55, 112, 113, 114, 115, 116, 117, 118, 119,
        56, 57, 58, 59, 60, 61, 62, 63, 120, 121, 122, 123, 124, 125, 126, 127,
        128, 129, 130, 131, 132, 133, 134, 135, 192, 193, 194, 195, 196, 197, 198, 199,
        136, 137, 138, 139, 140, 141, 142, 143, 200, 201, 202, 203, 204, 205, 206, 207,
        144, 145, 146, 147, 148, 149, 150, 151, 208, 209, 210, 211, 212, 213, 214, 215,
        152, 153, 154, 155, 156, 157, 158, 159, 216, 217, 218, 219, 220, 221, 222, 223,
        160, 161, 162, 163, 164, 165, 166, 167, 224, 225, 226, 227, 228, 229, 230, 231,
        168, 169, 170, 171, 172, 173, 174, 175, 232, 233, 234, 235, 236, 237, 238, 239,
        176, 177, 178, 179, 180, 181, 182, 183, 240, 241, 242, 243, 244, 245, 246, 247,
        184, 185, 186, 187, 188, 189, 190, 191, 248, 249, 250, 251, 252, 253, 254, 255
    };

    FH_UINT32 coffset[64] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 62, 63,
    };

    FH_SINT32 w_align, h_align, w_mbs;
    FH_SINT32 i, m[4], n,cm[4],cn;
    FH_UINT8 *data[4];
    FH_UINT8 *cdata[4];
    FH_UINT32 offset_num, coffset_num;
    FH_UINT32 y, cb, cr;

    w_align = (w + 15)&(~15);
    h_align = (h + 15)&(~15);
    w_mbs = w_align / 16;

    //×ø±ê2¶ÔÆë£¬420
    rx &= (~1);
    ry &= (~1);
    rw &= (~1);
    rh &= (~1);
    if (rx > w_align || ry > h_align || rw > w_align || ry > h_align ||
        (rx + rw) > w_align || (ry + rh) > h_align)
    {
        if (rx > w_align || ry > h_align)
            return -1;
        if (rw > w_align)
            rw = w_align;
        if (ry > h_align)
            rh = h_align;
        if ((rx + rw) > w_align)
        {
            rw = w_align - rx;
        }

        if ((ry + rh) > h_align)
        {
            rh = h_align - ry;
        }
    }

    y = ((ycbcr >> 16) & 0xff);
    cb = ((ycbcr >> 8) & 0xff);
    cr = ((ycbcr) & 0xff);

    data[0] = src_y + (((ry >> 4) * w_mbs) << 8);
    data[1] = src_y + ((((ry + 1) >> 4) * w_mbs) << 8);
    data[2] = src_y + ((((ry + rh - 1) >> 4) * w_mbs) << 8);
    data[3] = src_y + ((((ry + rh - 2) >> 4) * w_mbs) << 8);
    m[0] = ((ry & 0xf) << 4);
    m[1] = (((ry + 1) & 0xf) << 4);
    m[2] = (((ry + rh - 1) & 0xf) << 4);
    m[3] = (((ry + rh - 2) & 0xf) << 4);

    cdata[0] = src_c + (((ry >> 4) * w_mbs) << 7);
    cdata[1] = src_c + ((((ry + 1) >> 4) * w_mbs) << 7);
    cdata[2] = src_c + ((((ry + rh - 1) >> 4) * w_mbs) << 7);
    cdata[3] = src_c + ((((ry + rh - 2) >> 4) * w_mbs) << 7);
    cm[0] = (((ry & 0xf) >> 1) << 3);
    cm[1] = ((((ry + 1) & 0xf) >> 1) << 3);
    cm[2] = ((((ry + rh - 1) & 0xf) >> 1) << 3);
    cm[3] = ((((ry + rh - 2) & 0xf) >> 1) << 3);
    for (i = rx; i < rx + rw; i++)
    {
        offset_num = ((i >> 4) << 8);
        coffset_num = ((i >> 4) << 7);
        n = (i & 0xf);
        cn = (n >> 1);

        *(data[0] + offset_num + offset[m[0] + n]) = y;
        *(data[1] + offset_num + offset[m[1] + n]) = y;
        *(data[2] + offset_num + offset[m[2] + n]) = y;
        *(data[3] + offset_num + offset[m[3] + n]) = y;

        *(cdata[0] + coffset_num + coffset[cm[0] + cn]) = cb;
        *(cdata[2] + coffset_num + coffset[cm[2] + cn]) = cb;

        *(cdata[0] + coffset_num + coffset[cm[0] + cn] + 64) = cr;
        *(cdata[2] + coffset_num + coffset[cm[2] + cn] + 64) = cr;
    }

    data[0] = src_y + ((rx >> 4) << 8);
    data[1] = src_y + (((rx+1) >> 4) << 8);
    data[2] = src_y + (((rx+rw-1) >> 4) << 8);
    data[3] = src_y + (((rx+rw-2) >> 4) << 8);
    m[0] = ((rx & 0xf));
    m[1] = (((rx + 1) & 0xf));
    m[2] = (((rx + rw - 1) & 0xf));
    m[3] = (((rx + rw - 2) & 0xf));

    cdata[0] = src_c + ((rx >> 4) << 7);
    cdata[1] = src_c + (((rx + 1) >> 4) << 7);
    cdata[2] = src_c + (((rx + rw - 1) >> 4) << 7);
    cdata[3] = src_c + (((rx + rw - 2) >> 4) << 7);
    cm[0] = ((rx & 0xf) >> 1);
    cm[1] = (((rx + 1) & 0xf) >> 1);
    cm[2] = (((rx + rw - 1) & 0xf) >> 1);
    cm[3] = (((rx + rw - 2) & 0xf) >> 1);
    for (i = ry; i < ry + rh; i++)
    {
        offset_num = ((i >> 4) * w_mbs << 8);
        coffset_num = ((i >> 4) * w_mbs << 7);
        n = ((i & 0xf) << 4);
        cn = (((i & 0xf) >> 1) << 3);

        *(data[0] + offset_num + offset[m[0] + n]) = y;
        *(data[1] + offset_num + offset[m[1] + n]) = y;
        *(data[2] + offset_num + offset[m[2] + n]) = y;
        *(data[3] + offset_num + offset[m[3] + n]) = y;

        *(cdata[0] + coffset_num + coffset[cm[0] + cn]) = cb;
        *(cdata[2] + coffset_num + coffset[cm[2] + cn]) = cb;

        *(cdata[0] + coffset_num + coffset[cm[0] + cn] + 64) = cr;
        *(cdata[2] + coffset_num + coffset[cm[2] + cn] + 64) = cr;
    }

    return 0;
}

static FH_VOID draw_box(FH_DETECTION_t out, FH_UINT8 *src_y, FH_UINT8 *src_c, FH_UINT32 w, FH_UINT32 h, FH_UINT32 ycbcr)
{
    static FH_DETECTION_t out_last = {0};
    FH_SINT32 i;
    FH_SINT32 dt_x;
    FH_SINT32 dt_y;
    FH_SINT32 dt_w;
    FH_SINT32 dt_h;

    for (i = 0; i < out.bboxNum; i++) {
        /*draw gosd rect*/
        if (!compare_rect(out_last.detBBox[i].bbox, out.detBBox[i].bbox))
        {
           out.detBBox[i].bbox.x = out_last.detBBox[i].bbox.x;
           out.detBBox[i].bbox.y = out_last.detBBox[i].bbox.y;
           out.detBBox[i].bbox.w = out_last.detBBox[i].bbox.w;
           out.detBBox[i].bbox.h = out_last.detBBox[i].bbox.h;
        }
        else
        {
            out_last.detBBox[i].bbox = out.detBBox[i].bbox;
        }

        dt_x = (float)out.detBBox[i].bbox.x / w_ratio;
        dt_y = (float)out.detBBox[i].bbox.y / h_ratio;
        dt_w = (float)out.detBBox[i].bbox.w / w_ratio;
        dt_h = (float)out.detBBox[i].bbox.h / h_ratio;

        yuv_draw_rect_with_block_format(src_y, src_c, w, h, dt_x, dt_y, dt_w, dt_h, ycbcr);
    }

    out_last.bboxNum = out.bboxNum;
}


static FH_SINT32            g_frame_ready = 0;

typedef struct nn_detect_input
{
    FH_VOID           *handle;
    FH_IMAGE_t     *src_img;
    FH_DETECTION_t *out;
}NN_DETECT_INPUT_T;

NN_DETECT_INPUT_T  nn_detect_input;

FH_VOID * thread_nn_detect(FH_VOID *args)
{
    FH_SINT32 ret;

    NN_DETECT_INPUT_T *nn_input = args;

    while(!g_stop_nn_detect)
    {
        while(!g_frame_ready)
        {
            usleep(2000);
        }

        ret = FH_NN_DET_Process(nn_input->handle, nn_input->src_img, nn_input->out);
        if (ret != FH_SUCCESS)
        {
            printf("Error(%d): FH_NN_DET_Process\n", ret);
            g_frame_ready = 0;
            continue;
        }

        g_frame_ready = 0;

    }

    g_stop_nn_demo = 1;

    return NULL;
}

static FH_VOID sample_nn_detect_init(FH_VOID *args)
{
    pthread_t nn_detect_thread;

    g_stop_nn_detect = 0;
    pthead_create(&nn_detect_thread, NULL, thread_nn_detect, args, "nn_obj_do_detect");
}

/* *******NN DETECT THREAD******* */
FH_VOID *thread_nn_demo(FH_VOID *arg)
{
    FH_SINT32 ret;
    FH_SINT32 box_chnw;
    FH_SINT32 box_chnh;

    FH_SINT32 is_init = 0;

    FH_VPU_CHN_CONFIG Chn_info;
    FH_VPU_STREAM rgb_frm;
    FH_VPU_STREAM vpu_frm[2];
    FH_ENC_FRAME  frame;

    FH_UINT32 channel    = 3;
    FH_VOID           *detHandle = NULL;
    FH_IMAGE_t     srcImage;
    FH_DETECTION_t out_curr;
    FH_DETECTION_t out;

    FH_NN_INIT_PARAM_t param;

    FH_SINT32 rgb_w;
    FH_SINT32 rgb_h;

    switch (OBJECT_DETECT_TYPE)
    {
        case FH_DET_TYPE_DENSE512:
        {
            rgb_w = 512;
            rgb_h = 512;
            param.conf_thr = 0.8;
            strncpy(param.model, "nn/dense512.nb", sizeof(param.model));
            break;
        }
        case FH_DET_TYPE_DENSE512x288:
        {
            rgb_w = 512;
            rgb_h = 288;
            param.conf_thr = 0.6;
            strncpy(param.model, "nn/dense512x288.nb", sizeof(param.model));
            break;
        }
        case FH_DET_TYPE_FACE1024x576:
        {
            rgb_w = 1024;
            rgb_h = 576;
            param.conf_thr = 0.8;
            strncpy(param.model, "nn/face1024x576.nb", sizeof(param.model));
            break;
        }
        default:
        {
            rgb_w = 640;
            rgb_h = 360;
            param.conf_thr = 0.87;
            strncpy(param.model, "nn/face640x360.nb", sizeof(param.model));
            break;
        }
    }

    ret = FH_VPSS_GetChnAttr(DRAW_BOX_CHAN, &Chn_info);
    if (ret != RETURN_OK)
    {
        printf("Error(%x - %d):FH_VPSS_GetChnAttr\n", ret, ret);
        return NULL;
    }

    box_chnw = Chn_info.vpu_chn_size.u32Width;
    box_chnh = Chn_info.vpu_chn_size.u32Height;

    w_ratio = (float)rgb_w / (float)box_chnw;
    h_ratio = (float)rgb_h / (float)box_chnh;

    Chn_info.vpu_chn_size.u32Width = ALIGNTO(rgb_w, 16);
    Chn_info.vpu_chn_size.u32Height = ALIGNTO(rgb_h, 16);


    ret = FH_VPSS_SetChnAttr(RGB_CHAN, &Chn_info);
    if (ret != RETURN_OK)
    {
        printf("Error(%x - %d):FH_VPSS_SetChnAttr\n", ret, ret);
        return NULL;
    }

    ret = FH_VPSS_SetVOMode(RGB_CHAN, VPU_PIXFMT_RGB888);
    if (ret != RETURN_OK)
    {
        printf("Error(%x - %d):FH_VPSS_SetVOMode\n", ret, ret);
        return NULL;
    }

    ret = FH_VPSS_OpenChn(RGB_CHAN);
    if (ret != RETURN_OK)
    {
        printf("Error(%x - %d):FH_VPSS_OpenChn\n", ret, ret);
        return NULL;
    }

    param.type     = OBJECT_DETECT_TYPE; //?FH_DET_TYPE_DENSE512:FH_DET_TYPE_FACE1024x576;
    param.width    = rgb_w;
    param.height   = rgb_h;
    param.channel  = 3;

    printf("Detect file %s\n", param.model);

    ret = FH_NN_DET_Init(&detHandle, &param);
    if (FH_SUCCESS != ret)
    {
        printf("Error(%d): FH_NN_DET_Init\n", ret);
        goto MALLOC_FAIL;
    }

    out_curr.bboxNum = 0;

    nn_detect_input.handle  = detHandle;
    nn_detect_input.src_img = &srcImage;
    nn_detect_input.out     = &out_curr;

    sample_nn_detect_init(&nn_detect_input);

    ret = FH_SYS_UnBindbyDst(DRAW_BOX_CHAN);
    if (ret != 0)
    {
        printf("Error(%x - %d):FH_SYS_UnBindbyDst\n", ret, ret);
        goto MALLOC_FAIL;
    }

    FH_SINT32 vpu_frm_index = 0;

    FH_SINT32 box_color = _ARGB2AYCbCr(255, 0, 255, 0);

    g_nn_demo_running = 1;

    while (!g_stop_nn_demo)
    {
        draw_box(out, vpu_frm[1 - vpu_frm_index].yluma.vbase, vpu_frm[1 - vpu_frm_index].chroma.vbase, box_chnw, box_chnh, box_color);

        if (is_init == 2)
        {
            FH_SYS_MmzFlushCache(vpu_frm[1 - vpu_frm_index].yluma.base, vpu_frm[1 - vpu_frm_index].yluma.vbase, vpu_frm[1 - vpu_frm_index].yluma.size);
            FH_SYS_MmzFlushCache(vpu_frm[1 - vpu_frm_index].chroma.base, vpu_frm[1 - vpu_frm_index].chroma.vbase, vpu_frm[1 - vpu_frm_index].chroma.size);
            frame.lumma_addr     = (FH_UINT8 *)vpu_frm[1 - vpu_frm_index].yluma.base;
            frame.chroma_addr    = (FH_UINT8 *)vpu_frm[1 - vpu_frm_index].chroma.base;
            frame.size.u32Width  = box_chnw;
            frame.size.u32Height = box_chnh;
            frame.time_stamp = vpu_frm[1 - vpu_frm_index].time_stamp;
            ret = FH_VENC_Submit_ENC_Ex(0, &frame, VPU_VOMODE_BLK);
            if (ret != 0)
            {
                printf("Error(%x - %d):FH_VENC_Submit_ENC_Ex\n", ret, ret);
            }
        }

        if (is_init == 1)
        {
            is_init++;
        }

        ret = FH_VPSS_GetChnFrame(DRAW_BOX_CHAN, &vpu_frm[vpu_frm_index]);
        if (ret != 0)
        {
          printf("Error(%x - %d):FH_VPSS_GetChnFrame\n", ret, ret);
          continue;
        }

        vpu_frm_index = 1 - vpu_frm_index;

        ret = FH_VPSS_GetChnFrame_Ex(RGB_CHAN, &rgb_frm, 0);
        if (ret != 0)
        {
            printf("Error(%x - %d):FH_VPSS_GetChnFrame\n", ret, ret);
            continue;
        }

        while (g_frame_ready || !is_init)
        {
            if (is_init == 0)
            {
                is_init++;
                break;
            }

            usleep(1000);
        }

        out = out_curr;

        SET_FH_IMAGE(srcImage, rgb_h, rgb_w,
                     rgb_w * channel, FH_IMAGE_FORMAT_U8C3, rgb_frm.b.vbase);

        g_frame_ready = 1;
    }

    MALLOC_FAIL:
      if (detHandle)
        FH_NN_DET_Exit(detHandle);

    g_nn_demo_running = 0;

    return NULL;
}

FH_VOID set_nn_thresh(float thresh)
{
    FH_SINT32 ret;
    FH_DET_SETPARAM_t nn_thresh;

    nn_thresh.conf_thr = thresh;

    ret = FH_NN_DET_SetParam(nn_detect_input.handle, &nn_thresh);
    if (ret != FH_SUCCESS)
    {
        printf("Error(%d): FH_NN_DET_Setparam\n", ret);
    }

    printf("Set nn detect thresh: %f\n", thresh);

    return;

}

static pthread_t g_nn_det_thread;

FH_SINT32 sample_nn_human_detect_start(FH_VOID)
{
    if (g_nn_demo_running == 1)
    {
        printf("nn det is running\n");
        return 0;
    }

    g_stop_nn_demo   = 0;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&g_nn_det_thread, &attr, thread_nn_demo, NULL, "nn_obj_data_process");

    return 0;
}

FH_SINT32 sample_nn_human_detect_stop(FH_VOID)
{
    g_stop_nn_detect = 1;
    while(g_nn_demo_running)
        usleep(2000);
    return 0;
}

#endif /* CONFIG_ARCH_YG */
