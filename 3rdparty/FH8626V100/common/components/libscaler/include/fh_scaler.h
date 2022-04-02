#ifndef __fh_scaler_h__
#define __fh_scaler_h__

extern int chroma_transform(
        unsigned char* dstImg,    /*uv dst start address*/
        unsigned char* srcImg,    /*uv src start address*/
        unsigned int   width,     /*16对齐*/
        unsigned int   height,    /*16对齐*/
        unsigned int   dstStride);/*4对齐，effctive only uv data distribute alternately in memory，
                                    otherwise, dst data is saved by stride width/2 */

extern int luma_transform(
        unsigned char* dstImg,
        unsigned char* srcImg,
        unsigned int   width,     /*16对齐*/
        unsigned int   height,    /*16对齐*/
        unsigned int   dstStride);/*4对齐*/

extern int yuv_transform(unsigned char *dst,
                  unsigned char *srcy,
                  unsigned char *srcc,
                  unsigned int width,
                  unsigned int height,
                  unsigned int dstStride);

extern int chroma_resize(
        unsigned char *dstImg,   /*uv dst start address*/
        unsigned int   dstWidth,
        unsigned int   dstHeight,
        unsigned int   dstStride, /*dstStride must be greater than or equal to dstWidth,
                                    4对齐，effctive only uv data distribute alternately in memory，
                                    otherwise, dst data is saved by stride width/2 */
        unsigned char *srcImg,    /*uv src start address*/
        unsigned int   srcWidth,
        unsigned int   srcHeight,
        unsigned int   srcStride, /*srcStride must be greater than or equal to srcWidth*/
        unsigned int   srcCropX,  /*the cropped area's x position in srcImg*/
        unsigned int   srcCropY,  /*the cropped area's y position in srcImg*/
        unsigned int   srcCropW,  /*the cropped area's width in srcImg*/
        unsigned int   srcCropH);  /*the cropped area's height in srcImg*/

extern int luma_resize(
        unsigned char* dstImg,
        unsigned int   dstWidth,  /*4对齐*/
        unsigned int   dstHeight, /*4对齐*/
        unsigned int   dstStride, /*4对齐*//*dstStride must be greater than or equal to dstWidth*/
        unsigned char* srcImg,
        unsigned int   srcWidth,  /*4对齐*/
        unsigned int   srcHeight, /*4对齐*/
        unsigned int   srcStride, /*4对齐*//*srcStride must be same with srcWidth*/
        unsigned int   srcCropX,  /*4对齐*//*the cropped area's x position in srcImg*/
        unsigned int   srcCropY,  /*4对齐*//*the cropped area's y position in srcImg*/
        unsigned int   srcCropW,  /*4对齐*//*the cropped area's width in srcImg*/
        unsigned int   srcCropH); /*4对齐*//*the cropped area's height in srcImg*/

extern int yuv_resize(
               unsigned char *dstImg,
               unsigned int   dstWidth,
               unsigned int   dstHeight,
               unsigned int   dstStride, /*dstStride must be greater than or equal to dstWidth*/
               unsigned char *srcyImg,   /*y data src start address*/
               unsigned char *srccImg,   /*uv data src start address*/
               unsigned int   srcWidth,
               unsigned int   srcHeight,
               unsigned int   srcStride, /*srcStride must be greater than or equal to srcWidth*/
               unsigned int   srcCropX,  /*the cropped area's x position in srcImg*/
               unsigned int   srcCropY,  /*the cropped area's y position in srcImg*/
               unsigned int   srcCropW,  /*the cropped area's width in srcImg*/
               unsigned int   srcCropH);  /*the cropped area's height in srcImg*/

extern int chroma_transform_and_resize(
        unsigned char *dstImg,    /*uv dst start address*/
        unsigned int   dstWidth,
        unsigned int   dstHeight,
        unsigned int   dstStride, /*dstStride must be greater than or equal to dstWidth,
                                    4对齐，effctive only uv data distribute alternately in memory，
                                    otherwise, dst data is saved by stride width/2 */
        unsigned char *srcImg,    /*uv data src start address*/
        unsigned int   srcWidth,
        unsigned int   srcHeight,
        unsigned int   srcStride, /*srcStride must be same with srcWidth*/
        unsigned int   srcCropX,  /*the cropped area's x position in srcImg*/
        unsigned int   srcCropY,  /*the cropped area's y position in srcImg*/
        unsigned int   srcCropW,  /*the cropped area's width in srcImg*/
        unsigned int   srcCropH);  /*the cropped area's height in srcImg*/

extern int luma_transform_and_resize(
        unsigned char* dstImg,
        unsigned int   dstWidth,  /*4对齐*/
        unsigned int   dstHeight, /*4对齐*/
        unsigned int   dstStride, /*4对齐*//*dstStride must be greater than or equal to dstWidth*/
        unsigned char* srcImg,
        unsigned int   srcWidth,  /*16对齐*/
        unsigned int   srcHeight, /*16对齐*/
        unsigned int   srcStride, /*16对齐*//*srcStride必须等于srcWidth,0表示等于srcWidth*/
        unsigned int   srcCropX,  /*16对齐*//*the cropped area's x position in srcImg*/
        unsigned int   srcCropY,  /*16对齐*//*the cropped area's y position in srcImg*/
        unsigned int   srcCropW,  /*16对齐*//*the cropped area's width in srcImg*/
        unsigned int   srcCropH); /*16对齐*//*the cropped area's height in srcImg*/

extern int yuv_transform_and_resize(
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
        unsigned int   srcCropH);
#endif /*__fh_scaler_h__*/
