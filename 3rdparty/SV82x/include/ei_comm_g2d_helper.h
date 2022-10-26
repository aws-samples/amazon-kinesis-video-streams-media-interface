/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_g2d_helper.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_G2D_HELPER_H__
#define __EI_COMM_G2D_HELPER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "ei_g2d_type.h"

typedef struct eiG2D_QUICKCOPY_HELPER_S {
    G2D_HANDLE                hHandle;
    G2D_SURFACE_S             stSrc;
    G2D_RECT_S                stSrcRect;
    G2D_SURFACE_S             stDst;
    G2D_RECT_S                stDstRect;
} G2D_QUICKCOPY_HELPER_S;

typedef struct eiG2D_QUICKFILL_HELPER_S {
    G2D_HANDLE                hHandle;
    G2D_SURFACE_S             stDst;
    G2D_RECT_S                stDstRect;
    EI_U32                    u32FillData;
} G2D_QUICKFILL_HELPER_S;

typedef struct eiG2D_QUICKRESIZE_HELPER_S {
    G2D_HANDLE                hHandle;
    G2D_SURFACE_S             stSrc;
    G2D_RECT_S                stSrcRect;
    G2D_SURFACE_S             stDst;
    G2D_RECT_S                stDstRect;
} G2D_QUICKRESIZE_HELPER_S;

typedef struct eiG2D_BITBLIT_HELPER_S {
    G2D_HANDLE                hHandle;
    G2D_SURFACE_S             stBackGround;
    G2D_RECT_S                stBackGroundRect;
    G2D_SURFACE_S             stForeGround;
    G2D_RECT_S                stForeGroundRect;
    G2D_RECT_S                stForeOnBackCrtcRect;
    G2D_SURFACE_S             stDst;
    G2D_RECT_S                stDstRect;
    G2D_OPT_S                 stOpt;
} G2D_BITBLIT_HELPER_S;

typedef struct eiG2D_SOLIDDRAW_HELPER_S {
    G2D_HANDLE                hHandle;
    G2D_SURFACE_S             stForeGround;
    G2D_RECT_S                stForeGroundRect;
    G2D_SURFACE_S             stDst;
    G2D_RECT_S                stDstRect;
    G2D_FILLCOLOR_S           fillColor;
    G2D_OPT_S                 stOpt;
} G2D_SOLIDDRAW_HELPER_S;

typedef struct eiG2D_BITMAPMASKBLEND_HELPER_S {
    G2D_HANDLE                hHandle;
    G2D_SURFACE_S             stBackGround;
    G2D_RECT_S                stBackGroundRect;
    G2D_SURFACE_S             stForeGround;
    G2D_RECT_S                stForeGroundRect;
    G2D_RECT_S                stForeOnBackCrtcRect;
    G2D_SURFACE_S             stMask;
    G2D_RECT_S                stMaskRect;
    G2D_SURFACE_S             stDst;
    G2D_RECT_S                stDstRect;
    G2D_OPT_S                 stOpt;
} G2D_BITMAPMASKBLEND_HELPER_S;

typedef struct eiG2D_ROTATE_HELPER_S {
    G2D_HANDLE                hHandle;
    G2D_SURFACE_S             stSrc;
    G2D_RECT_S                stSrcRect;
    G2D_SURFACE_S             stDst;
    G2D_RECT_S                stDstRect;
    G2D_ROTATE_S              stRotate;
} G2D_ROTATE_HELPER_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_COMM_G2D_HELPER_H__ */

