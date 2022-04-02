#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LUMA_SRC_USING_SWAP_BUF
#define LUMA_DST_USING_SWAP_BUF
#define LUMA_TRANSFORM_LINES (4)

/* #define CHROMA_SAVE_NO_CROSS */

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define SWAP_PTR(m, a, b) do{m=a; a=b; b=m;}while(0)

/*
   static const unsigned char g_MB2LinearMap[64] = {
   0,   1,   16, 17,
   2,   3,   18, 19,
   4,   5,   20, 21,
   6,   7,   22, 23,
   8,   9,   24, 25,
   10,  11,  26, 27,
   12,  13,  28, 29,
   14,  15,  30, 31,

   32,  33,  48, 49,
   34,  35,  50, 51,
   36,  37,  52, 53,
   38,  39,  54, 55,
   40,  41,  56, 57,
   42,  43,  58, 59,
   44,  45,  60, 61,
   46,  47,  62, 63,
   };
   */

static void luma_transform_4lines(
        unsigned char* dst,
        unsigned int   dstStride,
        unsigned char* srcImg,
        unsigned int   srcWidth,
        unsigned int   x,
        unsigned int   y,
        unsigned int   w)
{
    unsigned int mbw = w >> 4;
    unsigned int mby = y >> 4;

    unsigned int *dstCurr;
    unsigned int *dstTemp;
    unsigned int *srcCurr;

    unsigned int i;
    unsigned int k;
    unsigned int t;
    unsigned char map[4] = {0, 8, 32, 40};

    t = map[((y & 15) >> 2)];
    srcCurr = (unsigned int *)(srcImg + mby*((srcWidth>>4) << 8) + ((x>>4) << 8));
    dstCurr = (unsigned int *)dst;

    for (i=mbw; i>0; i--)
    {
        dstTemp = dstCurr;

        for(k=0; k<8; k+=2)
        {
            dstTemp[0] = srcCurr[t+k];
            dstTemp[1] = srcCurr[t+k+1];
            dstTemp[2] = srcCurr[t+k+16];
            dstTemp[3] = srcCurr[t+k+17];
            dstTemp = (unsigned int *)((char *)dstTemp + dstStride);
        }

        dstCurr += 4;  /*16*/
        srcCurr += 64; /*256*/
    }
}

static void luma_transform_8lines(
        unsigned char* dst,
        unsigned int   dstStride,
        unsigned char* srcImg,
        unsigned int   srcWidth,
        unsigned int   x,
        unsigned int   y,
        unsigned int   w)
{
    unsigned int mbw = w >> 4;
    unsigned int mby = y >> 4;

    unsigned int *dstCurr;
    unsigned int *dstTemp;
    unsigned int *srcCurr;

    unsigned int i;
    unsigned int k;
    unsigned int t;
    unsigned char map[2] = {0, 32};

    t = map[((y & 15) >> 3)];
    srcCurr = (unsigned int *)(srcImg + mby*((srcWidth>>4) << 8) + ((x>>4) << 8));
    dstCurr = (unsigned int *)dst;

    for (i=mbw; i>0; i--)
    {
        dstTemp = dstCurr;

        for(k=0; k<16; k+=2)
        {
            dstTemp[0] = srcCurr[t+k];
            dstTemp[1] = srcCurr[t+k+1];
            dstTemp[2] = srcCurr[t+k+16];
            dstTemp[3] = srcCurr[t+k+17];
            dstTemp = (unsigned int *)((char *)dstTemp + dstStride);
        }

        dstCurr += 4;  /*16*/
        srcCurr += 64; /*256*/
    }
}

static int chroma_transform_block_nocross(unsigned char *uDstImg,
                            unsigned char *vDstImg,
                            unsigned int dstStride,
                            unsigned char *srcImg,
                            unsigned int srcWidth,
                            unsigned int x,
                            unsigned int y,
                            unsigned int w)
{
    unsigned int mbw = w >> 4;
    unsigned int mby = y >> 4;

    unsigned int *uDstCurr;
    unsigned int *uDstTemp;
    unsigned int *vDstCurr;
    unsigned int *vDstTemp;
    unsigned int *srcCurr;

    int i;
    int k;

    srcCurr  = (unsigned int *)(srcImg + mby*((srcWidth>>4) << 7) + ((x>>4) << 7));
    uDstCurr = (unsigned int *)uDstImg;
    vDstCurr = (unsigned int *)vDstImg;

    for (i = mbw; i > 0; i--)
    {
        uDstTemp = uDstCurr;
        vDstTemp = vDstCurr;

        for(k=0; k<8; k++)
        {
            uDstTemp[0] = srcCurr[2 * k];
            uDstTemp[1] = srcCurr[2 * k + 1];
            vDstTemp[0] = srcCurr[2 * k + 16];
            vDstTemp[1] = srcCurr[2 * k + 17];
            uDstTemp += dstStride / 8;
            vDstTemp += dstStride / 8;
        }

        uDstCurr += 2;
        vDstCurr += 2;
        srcCurr  += 32;
    }

    return 0;
}

static int chroma_transform_block(unsigned char *dstImg,
                            unsigned int dstStride,
                            unsigned char *srcImg,
                            unsigned int srcWidth,
                            unsigned int x,
                            unsigned int y,
                            unsigned int w)
{
    unsigned int mbw = w >> 4;
    unsigned int mby = y >> 4;

    unsigned char *dstCurr;
    unsigned char *dstTemp;
    unsigned char *srcCurr;

    int i;
    int k;
    int t;

    int onelineMap[16] = {0, 64, 1, 65, 2, 66, 3, 67, 4, 68, 5, 69, 6, 70, 7, 71};

    srcCurr = srcImg + mby*((srcWidth>>4) << 7) + ((x>>4) << 7);
    dstCurr = dstImg;

    for (i = mbw; i > 0; i--)
    {
        dstTemp = dstCurr;

        for(k=0; k<8; k++)
        {
            for (t = 0; t < 16; t++)
            {
                dstTemp[t] = srcCurr[onelineMap[t]];
            }

            dstTemp += dstStride;
        }

        dstCurr += 16;
        srcCurr += 128;
    }

    return 0;
}

int chroma_transform(
        unsigned char* dstImg,
        unsigned char* srcImg,
        unsigned int   width,
        unsigned int   height,
        unsigned int   dstStride)
{
    if (!dstStride)
        dstStride = width;

    /*w and h must be 16 bytes aligned!*/
    /*src and dst must be 4 bytes aligned!*/
    if (!(((width | height) & 15) == 0 &&
                (dstStride & 3) == 0 &&
                dstStride >= width &&
                width > 0 && height > 0 &&
                (((unsigned long)srcImg | (unsigned long)dstImg) & 3) == 0))
    {
        return -1;
    }

#ifdef CHROMA_SAVE_NO_CROSS
    unsigned char *uDst = dstImg;
    unsigned char *vDst = dstImg + height * width / 4;
    unsigned int dstJump = width / 16 * 64;
    unsigned int y;

    for (y = 0; y < height; y += 16)
    {
        chroma_transform_block_nocross(uDst, vDst, width, srcImg, width, 0, y, width);
        uDst += dstJump;
        vDst += dstJump;
    }
#else
    unsigned char *uDst = dstImg;
    unsigned int dstJump = dstStride / 16 * 128;
    unsigned int y;

    for (y = 0; y < height; y += 16)
    {
        chroma_transform_block(uDst, dstStride, srcImg, width, 0, y, width);
        uDst += dstJump;
    }
#endif
    return 0;
}

/*transform from fullhan's MB format to standard NV12 format*/
int luma_transform(
        unsigned char* dstImg,
        unsigned char* srcImg,
        unsigned int   width,
        unsigned int   height,
        unsigned int   dstStride)
{
    unsigned int y;

    if (!dstStride)
        dstStride = width;

    /*w and h must be 16 bytes aligned!*/
    /*src and dst must be 4 bytes aligned!*/
    if (!(((width | height) & 15) == 0 &&
                (dstStride & 3) == 0 &&
                dstStride >= width &&
                width > 0 && height > 0 &&
                (((unsigned long)srcImg | (unsigned long)dstImg) & 3) == 0))
    {
        return -1;
    }

    for (y = 0; y < height; y += 8)
    {
        luma_transform_8lines(dstImg + y * dstStride, dstStride, srcImg, width, 0, y, width);
    }

    return 0;
}

int yuv_transform(unsigned char *dst,
                  unsigned char *srcy,
                  unsigned char *srcc,
                  unsigned int width,
                  unsigned int height,
                  unsigned int dstStride)
{
    int ret = -1;
    ret = luma_transform(dst, srcy, width, height, dstStride);

    if (ret)
    {
        return ret;
    }

    if (!dstStride)
        dstStride = width;

    return chroma_transform(dst + dstStride * height, srcc, width, height, dstStride);
}


int chroma_resize(
        unsigned char *dstImg,
        unsigned int   dstWidth,
        unsigned int   dstHeight,
        unsigned int   dstStride, /*dstStride must be greater than or equal to dstWidth*/
        unsigned char *srcImg,
        unsigned int   srcWidth,
        unsigned int   srcHeight,
        unsigned int   srcStride, /*srcStride must be greater than or equal to srcWidth*/
        unsigned int   srcCropX,  /*the cropped area's x position in srcImg*/
        unsigned int   srcCropY,  /*the cropped area's y position in srcImg*/
        unsigned int   srcCropW,  /*the cropped area's width in srcImg*/
        unsigned int   srcCropH)  /*the cropped area's height in srcImg*/
{
    unsigned int i;
    unsigned int j;
    unsigned char *srcTemp;
    unsigned char *dstTemp;
    unsigned char *srcTempL0;
    unsigned char *srcTempL1;

    if (!dstStride)
        dstStride = dstWidth;
    if (!srcStride)
        srcStride = srcWidth;

#if 1 /*you can comment it to skip checking...*/
    i  = dstWidth | dstHeight | dstStride | srcWidth | srcHeight | srcStride;
    i |= srcCropX | srcCropY | srcCropW | srcCropH;
    if (((i >> 16) | (i & 3)) != 0)
        return -1;

    if (!( dstWidth > 0 &&
                dstHeight > 0 &&
                dstStride >= dstWidth &&
                srcWidth > 0 &&
                srcHeight > 0 &&
                srcStride >= srcWidth &&
                srcCropX + srcCropW <= srcWidth &&
                srcCropY + srcCropH <= srcHeight &&
                srcCropX < srcWidth &&
                srcCropY < srcHeight))
        return -1;
#endif

    if (!srcCropW)
        srcCropW = srcWidth - srcCropX;
    if (!srcCropH)
        srcCropH = srcHeight - srcCropY;

    srcImg = srcImg + srcCropY / 2 * srcStride + srcCropX;

    srcWidth  = srcCropW;
    srcHeight = srcCropH;

#ifdef CHROMA_SAVE_NO_CROSS
    unsigned char *vDst = dstImg + dstHeight * dstWidth / 4;
    unsigned char *vDstTemp;
    if (dstWidth == srcWidth && dstHeight == srcHeight)
    {
        dstTemp  = dstImg;
        vDstTemp = vDst;
        srcTemp = srcImg;
        for (i = srcHeight * srcWidth / 4; i > 0; i--)
        {
            *dstTemp = srcTemp[0];
            *vDstTemp = srcTemp[1];
            srcTemp += 2;
            dstTemp++;
            vDstTemp++;
        }
        return 0;
    }
#else
    if (dstWidth == srcWidth && dstHeight == srcHeight)
    {
        dstTemp = dstImg;
        srcTemp = srcImg;
        for (i = srcHeight / 2; i > 0; i--)
        {
            memcpy(dstTemp, srcTemp, srcWidth);
            srcTemp += srcStride;
            dstTemp += dstStride;
        }
        return 0;
    }
#endif

    unsigned int sft = 19;
    unsigned int scaleX = (srcWidth << sft) / dstWidth;
    unsigned int scaleY = (srcHeight << sft) / dstHeight;
    unsigned int posX, posY;
    unsigned int fx, fy, sx, sy;
    unsigned int Sy0, Sy1;
    unsigned int y0, y1;

    unsigned int coeffSft      = 6;
    unsigned int sftMCoeffSft  = sft - coeffSft;
    unsigned int coeffSftX2    = coeffSft << 1;
    unsigned int coeffFix       = (1 << coeffSft);
    unsigned int coeffFixM1    = coeffFix - 1;
    unsigned int coeffFix2Half  = (1 << coeffSftX2) >> 1;

    unsigned short *x0;
    unsigned short *x1;
    unsigned char  *Sx1;

#ifdef LUMA_SRC_USING_SWAP_BUF
    int  nline0 = -1;
    int  nline1 = -1;
    unsigned char  *bufSwap;
    unsigned char  *tmpSwap;
#endif

    unsigned short x0temp, x1temp;
    unsigned char  Sx0temp, Sx1temp;

#ifdef LUMA_DST_USING_SWAP_BUF
    unsigned char  *oneline;
    x0  = malloc(dstWidth * 6);
#else
    x0  = malloc(dstWidth * 5);
#endif
    if (!x0)
        return -1;
    x1  = x0 + dstWidth;
    Sx1 = (unsigned char *)(x1 + dstWidth);
#ifdef LUMA_DST_USING_SWAP_BUF
    oneline = Sx1 + dstWidth;
#endif

#ifdef LUMA_SRC_USING_SWAP_BUF
    bufSwap = malloc(2 * srcWidth);
    if (!bufSwap)
    {
        free(x0);
        return -1;
    }
    srcTempL0 = bufSwap;
    srcTempL1 = bufSwap + srcWidth;
#endif

    for (i = 0; i < dstWidth / 2; i++)
    {
        posX   = i * scaleX;
        sx     = posX >> sft;
        fx     = (posX >> sftMCoeffSft) & coeffFixM1;
        Sx1[i] = fx;
        x0[i]  = sx;
        x1[i]  = MIN(sx + 1, srcWidth / 2 - 1);
    }

    for (j = 0; j < dstHeight / 2; j++)
    {
        posY = j * scaleY;
        sy   = posY >> sft;
        fy   = (posY >> sftMCoeffSft) & coeffFixM1;
        Sy0  = coeffFix - fy;
        Sy1  = fy;
        y0   = sy;
        y1   = MIN(sy + 1, srcHeight / 2 - 1);

#ifdef LUMA_SRC_USING_SWAP_BUF
        if(y0 == nline1)
        {
            SWAP_PTR(tmpSwap, srcTempL0, srcTempL1);
            memcpy(srcTempL1, srcImg + y1 * srcStride, srcWidth);
            nline0 = y0;
            nline1 = y1;
        }
        else if(y0 != nline0)
        {
            memcpy(srcTempL0, srcImg + y0 * srcStride, srcWidth);
            memcpy(srcTempL1, srcImg + y1 * srcStride, srcWidth);
            nline0 = y0;
            nline1 = y1;
        }
#else
        srcTempL0 = srcImg + y0 * srcStride;
        srcTempL1 = srcImg + y1 * srcStride;
#endif

#ifdef CHROMA_SAVE_NO_CROSS

#ifdef LUMA_DST_USING_SWAP_BUF
        dstTemp   = oneline;
        vDstTemp  = oneline + dstWidth / 2;
#else
        dstTemp   = dstImg + j * dstWidth / 2;
        vDstTemp  = vDst +  j * dstWidth / 2;
#endif
#else
#ifdef LUMA_DST_USING_SWAP_BUF
        dstTemp   = oneline;
#else
        dstTemp   = dstImg + j * dstStride / 2;
#endif

#endif /*CHROMA_SAVE_NO_CROSS*/

        for (i = 0; i < dstWidth / 2; i++)
        {
            x0temp  = x0[i] * 2;
            x1temp  = x1[i] * 2;
            Sx1temp = Sx1[i];
            Sx0temp = coeffFix - Sx1temp;
#ifdef CHROMA_SAVE_NO_CROSS
            dstTemp[i]  = (srcTempL0[x0temp] * Sy0 * Sx0temp +
                    srcTempL1[x0temp] * Sy1 * Sx0temp +
                    srcTempL0[x1temp] * Sy0 * Sx1temp +
                    srcTempL1[x1temp] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;
            vDstTemp[i]  = (srcTempL0[x0temp + 1] * Sy0 * Sx0temp +
                    srcTempL1[x0temp + 1] * Sy1 * Sx0temp +
                    srcTempL0[x1temp + 1] * Sy0 * Sx1temp +
                    srcTempL1[x1temp + 1] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;
#else
            dstTemp[2 * i]  = (srcTempL0[x0temp] * Sy0 * Sx0temp +
                    srcTempL1[x0temp] * Sy1 * Sx0temp +
                    srcTempL0[x1temp] * Sy0 * Sx1temp +
                    srcTempL1[x1temp] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;
            dstTemp[2 * i + 1]  = (srcTempL0[x0temp + 1] * Sy0 * Sx0temp +
                    srcTempL1[x0temp + 1] * Sy1 * Sx0temp +
                    srcTempL0[x1temp + 1] * Sy0 * Sx1temp +
                    srcTempL1[x1temp + 1] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;
#endif
        }
#ifdef LUMA_DST_USING_SWAP_BUF
#ifdef CHROMA_SAVE_NO_CROSS
        memcpy(dstImg + j * dstWidth / 2, oneline, dstWidth / 2);
        memcpy(vDst + j * dstWidth / 2, oneline + dstWidth / 2, dstWidth / 2);
#else
        memcpy(dstImg + j * dstStride, oneline, dstWidth);
#endif
#endif
    }

    free(x0);
#ifdef LUMA_SRC_USING_SWAP_BUF
    free(bufSwap);
#endif

    return 0;
}

int luma_resize(
        unsigned char *dstImg,
        unsigned int   dstWidth,
        unsigned int   dstHeight,
        unsigned int   dstStride, /*dstStride must be greater than or equal to dstWidth*/
        unsigned char *srcImg,
        unsigned int   srcWidth,
        unsigned int   srcHeight,
        unsigned int   srcStride, /*srcStride must be greater than or equal to srcWidth*/
        unsigned int   srcCropX,  /*the cropped area's x position in srcImg*/
        unsigned int   srcCropY,  /*the cropped area's y position in srcImg*/
        unsigned int   srcCropW,  /*the cropped area's width in srcImg*/
        unsigned int   srcCropH)  /*the cropped area's height in srcImg*/
{
    unsigned int i;
    unsigned int j;
    unsigned char *srcTemp;
    unsigned char *dstTemp;
    unsigned char *srcTempL0;
    unsigned char *srcTempL1;

    if (!dstStride)
        dstStride = dstWidth;
    if (!srcStride)
        srcStride = srcWidth;

#if 1 /*you can comment it to skip checking...*/
    i  = dstWidth | dstHeight | dstStride | srcWidth | srcHeight | srcStride;
    i |= srcCropX | srcCropY | srcCropW | srcCropH;
    if (((i >> 16) | (i & 3)) != 0)
        return -1;

    if (!( dstWidth > 0 &&
                dstHeight > 0 &&
                dstStride >= dstWidth &&
                srcWidth > 0 &&
                srcHeight > 0 &&
                srcStride >= srcWidth &&
                srcCropX + srcCropW <= srcWidth &&
                srcCropY + srcCropH <= srcHeight &&
                srcCropX < srcWidth &&
                srcCropY < srcHeight))
        return -1;
#endif

    if (!srcCropW)
        srcCropW = srcWidth - srcCropX;
    if (!srcCropH)
        srcCropH = srcHeight - srcCropY;
    srcImg = srcImg + srcCropY * srcStride + srcCropX;
    srcWidth  = srcCropW;
    srcHeight = srcCropH;

    if (dstWidth == srcWidth && dstHeight == srcHeight)
    {
        dstTemp = dstImg;
        srcTemp = srcImg;
        for (i = srcHeight; i > 0; i--)
        {
            memcpy(dstTemp, srcTemp, srcWidth);
            srcTemp += srcStride;
            dstTemp += dstStride;
        }
        return 0;
    }

    unsigned int sft = 19;
    unsigned int scaleX = (srcWidth << sft) / dstWidth;
    unsigned int scaleY = (srcHeight << sft) / dstHeight;
    unsigned int posX, posY;
    unsigned int fx, fy, sx, sy;
    unsigned int Sy0, Sy1;
    unsigned int y0, y1;

    unsigned int coeffSft      = 6;
    unsigned int sftMCoeffSft  = sft - coeffSft;
    unsigned int coeffSftX2    = coeffSft << 1;
    unsigned int coeffFix       = (1 << coeffSft);
    unsigned int coeffFixM1    = coeffFix - 1;
    unsigned int coeffFix2Half  = (1 << coeffSftX2) >> 1;

    unsigned short *x0;
    unsigned short *x1;
    unsigned char  *Sx1;

#ifdef LUMA_SRC_USING_SWAP_BUF
    int  nline0 = -1;
    int  nline1 = -1;
    unsigned char  *bufSwap;
    unsigned char  *tmpSwap;
#endif

    unsigned short x0temp, x1temp;
    unsigned char  Sx0temp, Sx1temp;

#ifdef LUMA_DST_USING_SWAP_BUF
    unsigned char  *oneline;
    x0  = malloc(dstWidth * 6);
#else
    x0  = malloc(dstWidth * 5);
#endif
    if (!x0)
        return -1;
    x1  = x0 + dstWidth;
    Sx1 = (unsigned char *)(x1 + dstWidth);
#ifdef LUMA_DST_USING_SWAP_BUF
    oneline = Sx1 + dstWidth;
#endif

#ifdef LUMA_SRC_USING_SWAP_BUF
    bufSwap = malloc(2 * srcWidth);
    if (!bufSwap)
    {
        free(x0);
        return -1;
    }
    srcTempL0 = bufSwap;
    srcTempL1 = bufSwap + srcWidth;
#endif

    for (i = 0; i < dstWidth; i++)
    {
        posX   = i * scaleX;
        sx     = posX >> sft;
        fx     = (posX >> sftMCoeffSft) & coeffFixM1;
        Sx1[i] = fx;
        x0[i]  = sx;
        x1[i]  = MIN(sx + 1, srcWidth - 1);
    }

    for (j = 0; j < dstHeight; j++)
    {
        posY = j * scaleY;
        sy   = posY >> sft;
        fy   = (posY >> sftMCoeffSft) & coeffFixM1;
        Sy0  = coeffFix - fy;
        Sy1  = fy;
        y0   = sy;
        y1   = MIN(sy + 1, srcHeight - 1);

#ifdef LUMA_SRC_USING_SWAP_BUF
        if(y0 == nline1)
        {
            SWAP_PTR(tmpSwap, srcTempL0, srcTempL1);
            memcpy(srcTempL1, srcImg + y1 * srcStride, srcWidth);
            nline0 = y0;
            nline1 = y1;
        }
        else if(y0 != nline0)
        {
            memcpy(srcTempL0, srcImg + y0 * srcStride, srcWidth);
            memcpy(srcTempL1, srcImg + y1 * srcStride, srcWidth);
            nline0 = y0;
            nline1 = y1;
        }
#else
        srcTempL0 = srcImg + y0 * srcStride;
        srcTempL1 = srcImg + y1 * srcStride;
#endif

#ifdef LUMA_DST_USING_SWAP_BUF
        dstTemp   = oneline;
#else
        dstTemp   = dstImg + j * dstStride;
#endif
        for (i = 0; i < dstWidth; i++)
        {
            x0temp  = x0[i];
            x1temp  = x1[i];
            Sx1temp = Sx1[i];
            Sx0temp = coeffFix - Sx1temp;
            dstTemp[i]  = (srcTempL0[x0temp] * Sy0 * Sx0temp +
                    srcTempL1[x0temp] * Sy1 * Sx0temp +
                    srcTempL0[x1temp] * Sy0 * Sx1temp +
                    srcTempL1[x1temp] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;
        }
#ifdef LUMA_DST_USING_SWAP_BUF
        memcpy(dstImg + j * dstStride, oneline, dstWidth);
#endif
    }

    free(x0);
#ifdef LUMA_SRC_USING_SWAP_BUF
    free(bufSwap);
#endif

    return 0;
}

int yuv_resize(
               unsigned char *dstImg,
               unsigned int   dstWidth,
               unsigned int   dstHeight,
               unsigned int   dstStride, /*dstStride must be greater than or equal to dstWidth*/
               unsigned char *srcyImg,
               unsigned char *srccImg,
               unsigned int   srcWidth,
               unsigned int   srcHeight,
               unsigned int   srcStride, /*srcStride must be greater than or equal to srcWidth*/
               unsigned int   srcCropX,  /*the cropped area's x position in srcImg*/
               unsigned int   srcCropY,  /*the cropped area's y position in srcImg*/
               unsigned int   srcCropW,  /*the cropped area's width in srcImg*/
               unsigned int   srcCropH)  /*the cropped area's height in srcImg*/
{
     int ret;

     ret = luma_resize(dstImg, dstWidth, dstHeight, dstStride, srcyImg, srcWidth, srcHeight, srcStride,
                        srcCropX, srcCropY, srcCropW, srcCropH);
     if (ret)
     {
        return -1;
     }

     if (!dstStride)
        dstStride = dstWidth;

     return chroma_resize(dstImg + dstStride * dstHeight, dstWidth, dstHeight, dstStride,
                          srccImg, srcWidth, srcHeight, srcStride,
                          srcCropX, srcCropY, srcCropW, srcCropH);

}

#if LUMA_TRANSFORM_LINES == 4
#define _luma_transform_lines luma_transform_4lines
#endif
#if LUMA_TRANSFORM_LINES == 8
#define _luma_transform_lines luma_transform_8lines
#endif

int luma_transform_and_resize(
        unsigned char *dstImg,
        unsigned int   dstWidth,
        unsigned int   dstHeight,
        unsigned int   dstStride, /*dstStride must be greater than or equal to dstWidth*/
        unsigned char *srcImg,
        unsigned int   srcWidth,
        unsigned int   srcHeight,
        unsigned int   srcStride, /*srcStride must be same with srcWidth*/
        unsigned int   srcCropX,  /*the cropped area's x position in srcImg*/
        unsigned int   srcCropY,  /*the cropped area's y position in srcImg*/
        unsigned int   srcCropW,  /*the cropped area's width in srcImg*/
        unsigned int   srcCropH)  /*the cropped area's height in srcImg*/
{
    int ret = -1;
    unsigned int i;
    unsigned int j;
    unsigned char *srcTempL0;
    unsigned char *srcTempL1;
    unsigned char *dstTemp;

    srcHeight &= (~15); /*ensure 16 aligned, for example, 1920*1080, force 1080 to 1072*/

    if (!dstStride)
        dstStride = dstWidth;
    if (!srcStride)
        srcStride = srcWidth;

#if 1 /*you can comment it to skip checking...*/
    i  = srcWidth | srcHeight | srcCropX | srcCropY | srcCropW | srcCropH;
    j  = dstWidth | dstHeight | dstStride;
    if ((((i|j) >> 16) | (i & 15) | (j & 3)) != 0)
    {
        return ret;
    }

    if (!( dstWidth > 0 &&
                dstHeight > 0 &&
                dstStride >= dstWidth &&
                srcWidth > 0 &&
                srcHeight > 0 &&
                srcStride == srcWidth &&
                srcCropX < srcWidth &&
                srcCropY < srcHeight &&
                srcCropX + srcCropW <= srcWidth &&
                srcCropY + srcCropH <= srcHeight))
    {
        return ret;
    }

#endif

    if (srcCropW <= 0)
        srcCropW = srcWidth - srcCropX;
    if (srcCropH <= 0)
        srcCropH = srcHeight - srcCropY;

    /*don't need to resize, just transform from MB format to NV12*/
    if (dstWidth == srcCropW && dstHeight == srcCropH)
    {
        for (i = 0; i < srcCropH; i += LUMA_TRANSFORM_LINES)
        {
            _luma_transform_lines(dstImg + i * dstStride, dstStride, \
                    srcImg, srcWidth, srcCropX, srcCropY + i, srcCropW);
        }
        return 0;
    }

    unsigned int sft = 19;
    unsigned int scaleX = (srcCropW << sft) / dstWidth;
    unsigned int scaleY = (srcCropH << sft) / dstHeight;
    unsigned int posX, posY;
    unsigned int fx, fy, sx, sy;
    unsigned int Sy0, Sy1;
    unsigned int y0, y1;
    unsigned int y0_align, y1_align;

    unsigned int coeffSft      = 6;
    unsigned int sftMCoeffSft  = sft - coeffSft;
    unsigned int coeffSftX2    = coeffSft << 1;
    unsigned int coeffFix       = (1 << coeffSft);
    unsigned int coeffFixM1    = coeffFix - 1;
    unsigned int coeffFix2Half  = (1 << coeffSftX2) >> 1;

    unsigned short *x0;
    unsigned short *x1;
    unsigned char  *Sx1;

    int  nline0 = -1;
    int  nline1 = -1;
    unsigned char  *pline0;
    unsigned char  *pline1;
    unsigned char  *swap;

    unsigned short x0temp, x1temp;
    unsigned char  Sx0temp, Sx1temp;

#ifdef LUMA_DST_USING_SWAP_BUF
    unsigned char *oneline;
    x0     = malloc(dstWidth * 6);
#else
    x0     = malloc(dstWidth * 5);
#endif
    x1     = x0 + dstWidth;
    Sx1    = (unsigned char *)(x1 + dstWidth);
#ifdef LUMA_DST_USING_SWAP_BUF
    oneline = Sx1 + dstWidth;
#endif

    pline0 = malloc(LUMA_TRANSFORM_LINES * srcCropW);
    pline1 = malloc(LUMA_TRANSFORM_LINES * srcCropW);
    if (!x0 || !pline0 || !pline1)
    {
        printf("no mem\n");
        goto Exit;
    }

    for (i = 0; i < dstWidth; i++)
    {
        posX   = i * scaleX;
        sx     = posX >> sft;
        fx     = (posX >> sftMCoeffSft) & coeffFixM1;
        Sx1[i] = fx;
        x0[i]  = sx;
        x1[i]  = MIN(sx + 1, srcCropW - 1);
    }

    for (j = 0; j < dstHeight; j++)
    {
        posY = j * scaleY;
        sy   = posY >> sft;
        fy   = (posY >> sftMCoeffSft) & coeffFixM1;
        Sy0  = coeffFix - fy;
        Sy1  = fy;
        y0   = sy;
        y1   = MIN(sy + 1, srcCropH - 1);

        /*begin find in the cache*/
        y0_align = y0 & ~(LUMA_TRANSFORM_LINES - 1);
        y1_align = y1 & ~(LUMA_TRANSFORM_LINES - 1);
        if(y0_align == nline0)
        {
        }
        else if(y0_align == nline1)
        {
            SWAP_PTR(swap, pline0, pline1);
            nline0 = nline1;
            nline1 = -1;
        }
        else
        {
            _luma_transform_lines(pline0, srcCropW, srcImg, srcWidth, srcCropX, srcCropY + y0_align, srcCropW);
            nline0 = y0_align;
        }
        srcTempL0 = pline0 + (y0 & (LUMA_TRANSFORM_LINES - 1)) * srcCropW;

        if(y1_align == nline0)
        {
            srcTempL1 = pline0 + (y1 & (LUMA_TRANSFORM_LINES - 1)) * srcCropW;
        }
        else
        {
            if(y1_align != nline1)
            {
                _luma_transform_lines(pline1, srcCropW, srcImg, srcWidth, srcCropX, srcCropY + y1_align, srcCropW);
                nline1 = y1_align;
            }
            srcTempL1 = pline1 + (y1 & (LUMA_TRANSFORM_LINES - 1)) * srcCropW;
        }
        /*end find in the cache*/

#ifdef LUMA_DST_USING_SWAP_BUF
        dstTemp   = oneline;
#else
        dstTemp   = dstImg + j * dstStride;
#endif
        for (i = 0; i < dstWidth; i++)
        {
            x0temp  = x0[i];
            x1temp  = x1[i];
            Sx1temp = Sx1[i];
            Sx0temp = coeffFix - Sx1temp;
            dstTemp[i] = (srcTempL0[x0temp] * Sy0 * Sx0temp +
                    srcTempL1[x0temp] * Sy1 * Sx0temp +
                    srcTempL0[x1temp] * Sy0 * Sx1temp +
                    srcTempL1[x1temp] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;
        }
#ifdef LUMA_DST_USING_SWAP_BUF
        memcpy(dstImg + j * dstStride, oneline, dstWidth);
#endif
    }

    ret = 0;
Exit:
    if (x0)
        free(x0);
    if (pline0)
        free(pline0);
    if (pline1)
        free(pline1);

    return ret;
}

int chroma_transform_and_resize(
        unsigned char *dstImg,
        unsigned int   dstWidth,
        unsigned int   dstHeight,
        unsigned int   dstStride, /*dstStride must be greater than or equal to dstWidth*/
        unsigned char *srcImg,
        unsigned int   srcWidth,
        unsigned int   srcHeight,
        unsigned int   srcStride, /*srcStride must be same with srcWidth*/
        unsigned int   srcCropX,  /*the cropped area's x position in srcImg*/
        unsigned int   srcCropY,  /*the cropped area's y position in srcImg*/
        unsigned int   srcCropW,  /*the cropped area's width in srcImg*/
        unsigned int   srcCropH)  /*the cropped area's height in srcImg*/
{
    int ret = -1;
    unsigned int i;
    unsigned int j;
    unsigned char *srcTempL0;
    unsigned char *srcTempL1;
    unsigned char *dstTemp;

    srcHeight &= (~15); /*ensure 16 aligned, for example, 1920*1080, force 1080 to 1072*/

    if (!dstStride)
        dstStride = dstWidth;
    if (!srcStride)
        srcStride = srcWidth;

#if 1 /*you can comment it to skip checking...*/
    i  = srcWidth | srcHeight | srcCropX | srcCropY | srcCropW | srcCropH;
    j  = dstWidth | dstHeight | dstStride;
    if ((((i|j) >> 16) | (i & 15) | (j & 3)) != 0)
    {
        return ret;
    }

    if (!( dstWidth > 0 &&
                dstHeight > 0 &&
                dstStride >= dstWidth &&
                srcWidth > 0 &&
                srcHeight > 0 &&
                srcStride == srcWidth &&
                srcCropX < srcWidth &&
                srcCropY < srcHeight &&
                srcCropX + srcCropW <= srcWidth &&
                srcCropY + srcCropH <= srcHeight))
    {
        return ret;
    }

#endif

    if (srcCropW <= 0)
        srcCropW = srcWidth - srcCropX;
    if (srcCropH <= 0)
        srcCropH = srcHeight - srcCropY;

#ifdef CHROMA_SAVE_NO_CROSS
 unsigned char *vDst = dstImg + dstHeight * dstWidth / 4;
#endif

    /*don't need to resize, just transform from MB format to NV12*/
    if (dstWidth == srcCropW && dstHeight == srcCropH)
    {
#ifdef CHROMA_SAVE_NO_CROSS
        unsigned int  dstJump = dstWidth / 16 * 64;
        unsigned char *uDst = dstImg;

        for (i = 0; i < srcCropH; i += 16)
        {
            chroma_transform_block_nocross(uDst, vDst, dstWidth, srcImg, srcWidth, srcCropX, srcCropY + i, srcCropW);
            uDst += dstJump;
            vDst += dstJump;
        }
#else
        unsigned int  dstJump = dstStride / 16 * 128;
        unsigned char *dst = dstImg;
        for (i = 0; i < srcCropH; i += 16)
        {
            chroma_transform_block(dst, dstStride, srcImg, srcWidth, srcCropX, srcCropY + i, srcCropW);
            dst += dstJump;
        }
#endif

        return 0;
    }

    unsigned int sft = 19;
    unsigned int scaleX = (srcCropW << sft) / dstWidth;
    unsigned int scaleY = (srcCropH << sft) / dstHeight;
    unsigned int posX, posY;
    unsigned int fx, fy, sx, sy;
    unsigned int Sy0, Sy1;
    unsigned int y0, y1;
    unsigned int y0_align, y1_align;

    unsigned int coeffSft      = 6;
    unsigned int sftMCoeffSft  = sft - coeffSft;
    unsigned int coeffSftX2    = coeffSft << 1;
    unsigned int coeffFix       = (1 << coeffSft);
    unsigned int coeffFixM1    = coeffFix - 1;
    unsigned int coeffFix2Half  = (1 << coeffSftX2) >> 1;

    unsigned short *x0;
    unsigned short *x1;
    unsigned char  *Sx1;

    int  nline0 = -1;
    int  nline1 = -1;
    unsigned char  *pline0;
    unsigned char  *pline1;
    unsigned char  *swap;

    unsigned short x0temp, x1temp;
    unsigned char  Sx0temp, Sx1temp;

#ifdef LUMA_DST_USING_SWAP_BUF
    unsigned char *oneline;
    x0     = malloc(dstWidth * 6);
#else
    x0     = malloc(dstWidth * 5);
#endif
    x1     = x0 + dstWidth;
    Sx1    = (unsigned char *)(x1 + dstWidth);
#ifdef LUMA_DST_USING_SWAP_BUF
    oneline = Sx1 + dstWidth;
#endif

    pline0 = malloc(16 / 2 * srcCropW);
    pline1 = malloc(16 / 2 * srcCropW);
    if (!x0 || !pline0 || !pline1)
    {
        goto Exit;
    }

    for (i = 0; i < dstWidth / 2; i++)
    {
        posX   = i * scaleX;
        sx     = posX >> sft;
        fx     = (posX >> sftMCoeffSft) & coeffFixM1;
        Sx1[i] = fx;
        x0[i]  = sx;
        x1[i]  = MIN(sx + 1, srcCropW / 2 - 1);
    }

    for (j = 0; j < dstHeight / 2; j++)
    {
        posY = j * scaleY;
        sy   = posY >> sft;
        fy   = (posY >> sftMCoeffSft) & coeffFixM1;
        Sy0  = coeffFix - fy;
        Sy1  = fy;
        y0   = sy;
        y1   = MIN(sy + 1, srcCropH / 2 - 1);

        /*begin find in the cache*/
        y0_align = (y0) & ~(16 / 2 - 1);
        y1_align = (y1) & ~(16 / 2 - 1);

        y0_align *= 2;
        y1_align *= 2;

        if(y0_align == nline0)
        {
        }
        else if(y0_align == nline1)
        {
            SWAP_PTR(swap, pline0, pline1);
            nline0 = nline1;
            nline1 = -1;
        }
        else
        {
            chroma_transform_block(pline0, srcCropW, srcImg, srcWidth, srcCropX, srcCropY + y0_align, srcCropW);
            nline0 = y0_align;
        }

        srcTempL0 = pline0 + (y0 & (16 / 2 - 1)) * srcCropW;

        if(y1_align == nline0)
        {
            srcTempL1 = pline0 + (y1 & (16 / 2 - 1)) * srcCropW;
        }
        else
        {
            if(y1_align != nline1)
            {
                chroma_transform_block(pline1, srcCropW, srcImg, srcWidth, srcCropX, srcCropY + y1_align, srcCropW);
                nline1 = y1_align;
            }
            srcTempL1 = pline1 + (y1 & (16 / 2 - 1)) * srcCropW;
        }
        /*end find in the cache*/

#ifdef CHROMA_SAVE_NO_CROSS
#ifdef LUMA_DST_USING_SWAP_BUF
        dstTemp   = oneline;
        unsigned char *vDstTemp = oneline + dstWidth / 2;
#else
        dstTemp   = dstImg + j * dstWidth / 2;
        unsigned char *vDstTemp = vDst + j * dstWidth / 2;
#endif
        for (i = 0; i < dstWidth / 2; i++)
        {
            x0temp  = x0[i] * 2;
            x1temp  = x1[i] * 2;
            Sx1temp = Sx1[i];
            Sx0temp = coeffFix - Sx1temp;

            dstTemp[i] = (srcTempL0[x0temp] * Sy0 * Sx0temp +
                    srcTempL1[x0temp] * Sy1 * Sx0temp +
                    srcTempL0[x1temp] * Sy0 * Sx1temp +
                    srcTempL1[x1temp] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;

            vDstTemp[i] = (srcTempL0[x0temp + 1] * Sy0 * Sx0temp +
                    srcTempL1[x0temp + 1] * Sy1 * Sx0temp +
                    srcTempL0[x1temp + 1] * Sy0 * Sx1temp +
                    srcTempL1[x1temp + 1] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;

        }
#ifdef LUMA_DST_USING_SWAP_BUF
        memcpy(dstImg + j * dstWidth / 2, oneline, dstWidth / 2);
        memcpy(vDst + j * dstWidth / 2, oneline + dstWidth / 2, dstWidth / 2);
#endif

#else

#ifdef LUMA_DST_USING_SWAP_BUF
        dstTemp   = oneline;
#else
        dstTemp   = dstImg + j * dstStride;
#endif
        for (i = 0; i < dstWidth / 2; i++)
        {
            x0temp  = x0[i] * 2;
            x1temp  = x1[i] * 2;
            Sx1temp = Sx1[i];
            Sx0temp = coeffFix - Sx1temp;

            dstTemp[2 * i] = (srcTempL0[x0temp] * Sy0 * Sx0temp +
                    srcTempL1[x0temp] * Sy1 * Sx0temp +
                    srcTempL0[x1temp] * Sy0 * Sx1temp +
                    srcTempL1[x1temp] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;

            dstTemp[2 * i + 1] = (srcTempL0[x0temp + 1] * Sy0 * Sx0temp +
                    srcTempL1[x0temp + 1] * Sy1 * Sx0temp +
                    srcTempL0[x1temp + 1] * Sy0 * Sx1temp +
                    srcTempL1[x1temp + 1] * Sy1 * Sx1temp +
                    coeffFix2Half) >> coeffSftX2;
        }
#ifdef LUMA_DST_USING_SWAP_BUF
        memcpy(dstImg + j * dstStride, oneline, dstWidth);
#endif

#endif /* CHROMA_SAVE_NO_CROSS */

    }

    ret = 0;

Exit:
    if (x0)
        free(x0);
    if (pline0)
        free(pline0);
    if (pline1)
        free(pline1);

    return ret;
}

int yuv_transform_and_resize(
        unsigned char *dstImg,
        unsigned int   dstWidth,
        unsigned int   dstHeight,
        unsigned int   dstStride,
        unsigned char *srcyImg,
        unsigned char *srccImg,
        unsigned int   srcWidth,
        unsigned int   srcHeight,
        unsigned int   srcStride,
        unsigned int   srcCropX,
        unsigned int   srcCropY,
        unsigned int   srcCropW,
        unsigned int   srcCropH)
{
    int ret;
    ret = luma_transform_and_resize(dstImg, dstWidth, dstHeight, dstStride,
                                    srcyImg, srcWidth, srcHeight, srcStride,
                                    srcCropX, srcCropY, srcCropW, srcCropH);
    if (ret)
    {
        return ret;
    }

    if (!dstStride)
        dstStride = dstWidth;

    return chroma_transform_and_resize(dstImg + dstStride * dstHeight, dstWidth, dstHeight, dstStride,
                                    srccImg, srcWidth, srcHeight, srcStride,
                                    srcCropX, srcCropY, srcCropW, srcCropH);
}