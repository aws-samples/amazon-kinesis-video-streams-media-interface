/*
 *------------------------------------------------------------------------------
 * @File      :    ei_g2d_api.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef _EI_API_G2D_H_
#define _EI_API_G2D_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif  /* __cplusplus */

#include "ei_type.h"
#include "ei_g2d_type.h"

G2D_HANDLE  EI_G2D_BeginJob(EI_VOID);

EI_S32 EI_G2D_EndJob(G2D_HANDLE hHandle, EI_BOOL bSync, EI_BOOL bBlock, EI_U32 u32TimeOut);

EI_S32 EI_G2D_CancelJob(G2D_HANDLE hHandle);

EI_S32 EI_G2D_QuickCopy(G2D_HANDLE hHandle, const G2D_SURFACE_S *pstSrc, const G2D_RECT_S  *pstSrcRect,
    const G2D_SURFACE_S *pstDst, const G2D_RECT_S *pstDstRect);

EI_S32 EI_G2D_QuickFill(G2D_HANDLE hHandle, const G2D_SURFACE_S *pstDst, const G2D_RECT_S *pstDstRect,
    EI_U32 u32FillData);

EI_S32 EI_G2D_QuickResize(G2D_HANDLE hHandle, const G2D_SURFACE_S *pstSrc, const G2D_RECT_S  *pstSrcRect,
    const G2D_SURFACE_S *pstDst, const G2D_RECT_S  *pstDstRect);

EI_S32 EI_G2D_Bitblit(G2D_HANDLE hHandle,
    const G2D_SURFACE_S *pstBackGround, const G2D_RECT_S  *pstBackGroundRect,
    const G2D_SURFACE_S *pstForeGround, const G2D_RECT_S  *pstForeGroundRect,
    const G2D_RECT_S  *pstForeOnBackCrtcRect, const G2D_SURFACE_S *pstDst,
    const G2D_RECT_S  *pstDstRect, const G2D_OPT_S *pstOpt);

EI_S32 EI_G2D_SolidDraw(G2D_HANDLE hHandle,
    const G2D_SURFACE_S *pstForeGround, const G2D_RECT_S *pstForeGroundRect,
    const G2D_SURFACE_S *pstDst, const G2D_RECT_S *pstDstRect,
    G2D_FILLCOLOR_S fillColor, const G2D_OPT_S *pstOpt);

EI_S32 EI_G2D_BitmapMaskBlend(G2D_HANDLE hHandle,
    const G2D_SURFACE_S *pstBackGround, const G2D_RECT_S  *pstBackGroundRect,
    const G2D_SURFACE_S *pstForeGround, const G2D_RECT_S  *pstForeGroundRect,
    const G2D_RECT_S  *pstForeOnBackCrtcRect,
    const G2D_SURFACE_S *pstMask, const G2D_RECT_S  *pstMaskRect,
    const G2D_SURFACE_S *pstDst, const G2D_RECT_S  *pstDstRect,
    const G2D_OPT_S *pstOpt);

EI_S32 EI_G2D_Rotate(G2D_HANDLE hHandle, const G2D_SURFACE_S *pstSrc, const G2D_RECT_S *pstSrcRect,
    const G2D_SURFACE_S *pstDst, const G2D_RECT_S *pstDstRect, G2D_ROTATE_S *pstRotate);

#if 0
EI_S32 EI_G2D_MultiBlending(G2D_HANDLE hHandle, const G2D_SURFACE_LIST_S *pstSurfaceList);
#endif
/********** === G2G API declaration end === **********/
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif  /* __cplusplus */

#endif  /* _EI_API_G2D_H_ */
