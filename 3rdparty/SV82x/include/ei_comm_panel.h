/*
 *------------------------------------------------------------------------------
 * @File      :    ei_comm_panel.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_COMM_PANEL_H__
#define __EI_COMM_PANEL_H__

#include "ei_type.h"
#include "ei_common.h"
#include "ei_comm_doss.h"
#include "ei_comm_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef unsigned char *PANEL_INIT_CODE_TYPE;
//#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef enum eiPANEL_TYPE_E {
    OTM1289A_MIPI_720_1280_60FPS,              /* 0 */
    WKS70002_LVDS_1024_600_60FPS,              /* 1 */
    WKS70002_LVDS_1024_600_60FPS_DUAL_LINK,    /* 2 */
    LT8912B_MIPI_720_480_60FPS,                /* 3 */
    LT8912B_MIPI_1280_720_50FPS,               /* 4 */
    LT8912B_MIPI_1920_1080_60FPS,              /* 5 */
    WKS43178_CPU_800_480_60FPS,                /* 6 */
    SPCS92048_PRGB_800_480_60FPS,              /* 7 */
    ILI9488_MIPI_320_480_60FPS,                /* 8 */

    EOS_PANEL_TYPE_DEFAULT = 250, /* use a rather large value */

    SAMPLE_PANEL_TYPE_MAX,
} PANEL_TYPE_E;

typedef struct eiPANEL_TYPE_ATTR_S {
    VO_PUB_ATTR_S        stVoPubAttr;
    PANEL_INIT_CODE_TYPE init;
    EI_U32               u32InitCodeLen;
} PANEL_TYPE_ATTR_S;

typedef struct eiPANEL_OBJ_S {
    EI_S32(*pfnGetPanelAttrByPanel)(PANEL_TYPE_E enPanelType, PANEL_TYPE_ATTR_S *pstPanelAttr);
    EI_S32(*pfnGetPanelInitCodeByPanel)(PANEL_TYPE_E enPanelType, PANEL_INIT_CODE_TYPE *init, EI_U32 *pLen);
} PANEL_OBJ_S;

extern PANEL_OBJ_S stPanelOtm1289aObj;
extern PANEL_OBJ_S stPanelWks70002Obj;
extern PANEL_OBJ_S stPanelLt8912bObj;
extern PANEL_OBJ_S stPanelWks43178Obj;
extern PANEL_OBJ_S stPanelSpcs92048Obj;
extern PANEL_OBJ_S stPanelIli9488Obj;
extern PANEL_OBJ_S stPanelEosDefaultObj;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus */

#endif /*__EI_COMM_PANEL_H__ */
