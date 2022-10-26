/*
 *------------------------------------------------------------------------------
 * @File      :    ei_g2d_type.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __G2D_TYPE_H__
#define __G2D_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef EI_S32 G2D_HANDLE;

typedef enum eiG2D_COLOR_FMT_E {
    G2D_COLOR_FMT_RGB444 = 0,
    G2D_COLOR_FMT_BGR444,
    G2D_COLOR_FMT_RGB555,
    G2D_COLOR_FMT_BGR555,
    G2D_COLOR_FMT_RGB565,
    G2D_COLOR_FMT_BGR565,
    G2D_COLOR_FMT_RGB888,
    G2D_COLOR_FMT_BGR888,
    G2D_COLOR_FMT_ARGB4444,
    G2D_COLOR_FMT_ABGR4444,
    G2D_COLOR_FMT_RGBA4444,
    G2D_COLOR_FMT_BGRA4444,
    G2D_COLOR_FMT_ARGB1555,
    G2D_COLOR_FMT_ABGR1555,
    G2D_COLOR_FMT_RGBA1555,
    G2D_COLOR_FMT_BGRA1555,
    G2D_COLOR_FMT_ARGB8565,
    G2D_COLOR_FMT_ABGR8565,
    G2D_COLOR_FMT_RGBA8565,
    G2D_COLOR_FMT_BGRA8565,
    G2D_COLOR_FMT_ARGB8888,
    G2D_COLOR_FMT_ABGR8888,
    G2D_COLOR_FMT_RGBA8888,
    G2D_COLOR_FMT_BGRA8888,
    G2D_COLOR_FMT_RABG8888,
    G2D_COLOR_FMT_A1,
    G2D_COLOR_FMT_A8,
    G2D_COLOR_FMT_YCbCr888,
    G2D_COLOR_FMT_AYCbCr8888,
    G2D_COLOR_FMT_YCbCr420,
    G2D_COLOR_FMT_YCrCb420,
    G2D_COLOR_FMT_YCbCr422,
    G2D_COLOR_FMT_YCrCb422,
    G2D_COLOR_FMT_PKGVYUY,
    G2D_COLOR_FMT_BUTT
} G2D_COLOR_FMT_E;

#ifdef ALIGN_NUM
#undef ALIGN_NUM
#define ALIGN_NUM 8
#endif
#define ATTRIBUTE __attribute__((aligned (ALIGN_NUM)))

typedef struct eiG2D_SURFACE_S {
    EI_U64 PhyAddr;

    G2D_COLOR_FMT_E enColorFmt;

    EI_U32 u32Width;

    EI_U32 u32Height;

    EI_U32 u32Stride;

    EI_BOOL bAlphaMax255;

    EI_U64 CbCrPhyAddr;
    EI_U32 u32CbCrStride;
} G2D_SURFACE_S;

typedef struct eiG2D_RECT_S {
    EI_S32 s32Xpos;
    EI_S32 s32Ypos;
    EI_U32 u32Width;
    EI_U32 u32Height;
} G2D_RECT_S;

typedef EI_U32 G2D_FILLCOLOR_S;

typedef enum eiG2D_COLORKEY_MODE_E {
    G2D_COLORKEY_MODE_NONE = 0,
    G2D_COLORKEY_MODE_FOREGROUND,
    G2D_COLORKEY_MODE_BACKGROUND,
    G2D_COLORKEY_MODE_BUTT
} G2D_COLORKEY_MODE_E;

typedef enum eiG2D_COLORKEY_RULE_E {
    G2D_COLORKEY_RULE_ALWAYS_MATCHING = 0,
    G2D_COLORKEY_RULE_BETWEEN_MIN_AND_MAX,
    G2D_COLORKEY_RULE_BOTH_SIDES_OF_MIN_AND_MAX,
    G2D_COLORKEY_RULE_BUTT,
} G2D_COLORKEY_RULE_E;

typedef struct eiG2D_COLORKEY_COMP_S {
    G2D_COLORKEY_RULE_E eRule;
    EI_S32 s32CompMin;
    EI_S32 s32CompMax;
} G2D_COLORKEY_COMP_S;

typedef struct eiG2D_COLORKEY_VALUE_S {
    G2D_COLORKEY_COMP_S stRed;
    G2D_COLORKEY_COMP_S stGreen;
    G2D_COLORKEY_COMP_S stBlue;
} G2D_COLORKEY_VALUE_S;

typedef enum eiG2D_ALPHA_MODE_E {
    G2D_ALPHA_MODE_PIXEL_ALPHA = 0,
    G2D_ALPHA_MODE_PLANE_ALPHA,
    G2D_ALPHA_MODE_PIXEL_ALPHA_X_PLANE_ALPHA,
    G2D_ALPHA_MODE_BUTT,
} G2D_ALPHA_MODE_E;

typedef enum eiG2D_BLENDCMD_E {
    G2D_BLENDCMD_NONE = 0x0,
    G2D_BLENDCMD_CLEAR,
    G2D_BLENDCMD_SRC,
    G2D_BLENDCMD_DST,
    G2D_BLENDCMD_SRCOVER,
    G2D_BLENDCMD_DSTOVER,
    G2D_BLENDCMD_SRCIN,
    G2D_BLENDCMD_DSTIN,
    G2D_BLENDCMD_SRCOUT,
    G2D_BLENDCMD_DSTOUT,
    G2D_BLENDCMD_SRCATOP,
    G2D_BLENDCMD_DSTATOP,
    G2D_BLENDCMD_XOR,
    G2D_BLENDCMD_BUTT
} G2D_BLENDCMD_E;

typedef enum eiG2D_COLOR_SPACE_MODE_E {
    G2D_COLOR_SPACE_MODE_BT601L = 0,
    G2D_COLOR_SPACE_MODE_BT601F,
    G2D_COLOR_SPACE_MODE_BT709L,
    G2D_COLOR_SPACE_MODE_BT709F,
    G2D_COLOR_SPACE_MODE_2_1_0,
    G2D_COLOR_SPACE_MODE_BUTT
} G2D_COLOR_SPACE_MODE_E;

typedef struct eiG2D_OPT_S {
    G2D_COLORKEY_MODE_E enColorKeyMode;

    G2D_COLORKEY_VALUE_S stColorKeyValue;

    EI_BOOL bPremultiplied;

    G2D_ALPHA_MODE_E enAlphaMode;

    EI_U8 u8AlphaValue;

    G2D_BLENDCMD_E enBlendCmd;

    G2D_COLOR_SPACE_MODE_E enColorSpaceMode;
} G2D_OPT_S;

/**Definition of rotation directions*/
typedef enum eiG2D_ROTATE_ANGLE_E {
    G2D_ROTATE_CLOCKWISE_NONE = 0,
    G2D_ROTATE_CLOCKWISE_90,
    G2D_ROTATE_CLOCKWISE_180,
    G2D_ROTATE_CLOCKWISE_270,
    G2D_ROTATE_BUTT
} G2D_ROTATE_ANGLE_E;

typedef enum eiG2D_MIRROR_DIRECTION_E {
    G2D_MIRROR_DIRECTION_NONE = 0,
    G2D_MIRROR_DIRECTION_VER,
    G2D_MIRROR_DIRECTION_HOR,
    G2D_MIRROR_DIRECTION_HOR_VER,
    G2D_MIRROR_DIRECTION_BUTT
} G2D_MIRROR_DIRECTION_E;

typedef struct eiG2D_ROTATE_S {
    G2D_ROTATE_ANGLE_E enRotateAngle;
    G2D_MIRROR_DIRECTION_E enMirrorDirection;
} G2D_ROTATE_S;

typedef struct eiG2D_COMPOSOR_S {
    G2D_SURFACE_S stSrcSurface;
    G2D_RECT_S stInRect;
    G2D_RECT_S stOutRect;
    G2D_OPT_S stOpt;
} G2D_COMPOSOR_S;

typedef struct eiG2D_SURFACE_LIST_S {
    EI_U32 u32SurfaceNum;
    G2D_SURFACE_S *pDstSurface;
    G2D_COMPOSOR_S *pstComposor;
} G2D_SURFACE_LIST_S;
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __G2D_TYPE_H__ */


