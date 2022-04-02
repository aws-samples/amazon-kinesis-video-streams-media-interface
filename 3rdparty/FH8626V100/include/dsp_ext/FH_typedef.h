/**
 * @file     FH_typedef.h
 * @brief    FHAdv video capture module interface
 * @version  V1.0.0
 * @date     12-May-2016
 * @author   Software Team
 *
 * @note
 * Copyright (C) 2016 Shanghai Fullhan Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * @par
 * Fullhan is supplying this software which provides customers with programming
 * information regarding the products. Fullhan has no responsibility or
 * liability for the use of the software. Fullhan not guarantee the correctness
 * of this software. Fullhan reserves the right to make changes in the software
 * without notification.
 *
 */

#ifndef __FH_TYPEDEF_H__
#define __FH_TYPEDEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef __TYPE_DEF_H__
typedef unsigned long long FH_UINT64;
typedef long long FH_SINT64;
typedef unsigned int FH_UINT32;
typedef int FH_SINT32;
typedef unsigned short FH_UINT16;
typedef short FH_SINT16;
typedef unsigned char FH_UINT8;
typedef char FH_SINT8;
typedef FH_UINT8* FH_ADDR;

typedef char FH_CHAR;
typedef void FH_VOID;

typedef enum {
    FH_FALSE = 0,
    FH_TRUE  = 1,
    DUMMY    = 0xffffffff,
} FH_BOOL;

#define FH_NULL 0L
#define FH_SUCCESS 0
#define FH_FAILURE (-1)

#define INT_MINI (-2147483647 - 1) /*!< minimum (signed) int value */
#define INT_MAXI 2147483647        /*!< maximum (signed) int value */
#endif

/**
*  @brief 矩形区域信息结构体
*/
typedef struct
{
    FH_UINT32 fTopLeftX; /*!< 矩形区域左上点水平方向上的坐标 */
    FH_UINT32 fTopLeftY; /*!< 矩形区域左上点垂直方向上的坐标 */
    FH_UINT32 fWidth;    /*!< 矩形区域的宽度 */
    FH_UINT32 fHeigh;    /*!< 矩形区域的高度 */
} FHT_Rect_t;

typedef struct
{
    FH_UINT32 pos_x;
    FH_UINT32 pos_y;
} FHT_LeftTopNode_t;

typedef struct
{
    FH_UINT8 fRed;
    FH_UINT8 fGreen;
    FH_UINT8 fBlue;
    FH_UINT8 fAlpha;     /* 透明度 0全透明，FF完全不透明 */
} FHT_RgbColor_t;

typedef enum
{
    FHEN_FONT_TYPE_ASC,       /* 英文字符字库 */
    FHEN_FONT_TYPE_CHINESE,   /* 中文字符字库 */
} FHT_OSD_FontType_e;

typedef struct
{
    FHT_RgbColor_t norcolor;  /* 正常颜色 */
    FHT_RgbColor_t invcolor;  /* 反色 */
} FHT_Color_t;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TYPE_DEF_H__ */
