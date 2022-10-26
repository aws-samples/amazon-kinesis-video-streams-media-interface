/*
 *------------------------------------------------------------------------------
 * @File      :    sample_comm_doss.c
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Source file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "ei_posix.h"

#include "sample_comm.h"

PANEL_OBJ_S *SAMPLE_COMM_DOSS_GetPanelObj(PANEL_TYPE_E enPanelType)
{
    PANEL_OBJ_S *pstObj;

    switch (enPanelType) {
    case OTM1289A_MIPI_720_1280_60FPS:
        pstObj = &stPanelOtm1289aObj;
        break;
    case WKS70002_LVDS_1024_600_60FPS:
    case WKS70002_LVDS_1024_600_60FPS_DUAL_LINK:
        pstObj = &stPanelWks70002Obj;
        break;
    case LT8912B_MIPI_720_480_60FPS:
    case LT8912B_MIPI_1280_720_50FPS:
    case LT8912B_MIPI_1920_1080_60FPS:
        pstObj = &stPanelLt8912bObj;
        break;
    case WKS43178_CPU_800_480_60FPS:
        pstObj = &stPanelWks43178Obj;
        break;
    case SPCS92048_PRGB_800_480_60FPS:
        pstObj = &stPanelSpcs92048Obj;
        break;
    case ILI9488_MIPI_320_480_60FPS:
        pstObj = &stPanelIli9488Obj;
        break;
    case EOS_PANEL_TYPE_DEFAULT:
        pstObj = &stPanelEosDefaultObj;
        break;
    default:
        pstObj = EI_NULL;
        break;
    }

    return pstObj;
}

EI_S32 SAMPLE_COMM_DOSS_GetPanelAttr(PANEL_TYPE_E enPanelType, PANEL_TYPE_ATTR_S *pstPanelAttr)
{
    EI_S32 s32Ret = EI_SUCCESS;
    PANEL_OBJ_S *pstPanelObj = SAMPLE_COMM_DOSS_GetPanelObj(enPanelType);

    if (!pstPanelObj) {
        EI_TRACE_DOSS(EI_DBG_ERR, "panel %d not exist!\n", enPanelType);
        return EI_FAILURE;
    }

    if (!pstPanelAttr) {
        EI_TRACE_DOSS(EI_DBG_ERR, "argument NULL\n");
        return EI_ERR_VO_NULL_PTR;
    }

    if (pstPanelObj->pfnGetPanelAttrByPanel) {
        s32Ret = pstPanelObj->pfnGetPanelAttrByPanel(enPanelType, pstPanelAttr);
        if (s32Ret) {
            EI_TRACE_DOSS(EI_DBG_ERR, "get panel attr by PANEL_TYPE_E fail\n");
            return s32Ret;
        }
    }

    if (pstPanelObj->pfnGetPanelInitCodeByPanel) {
        s32Ret = pstPanelObj->pfnGetPanelInitCodeByPanel(enPanelType,
                &pstPanelAttr->init, &pstPanelAttr->u32InitCodeLen);
        if (s32Ret) {
            EI_TRACE_DOSS(EI_DBG_ERR, "get panel init code by PANEL_TYPE_E fail\n");
            return s32Ret;
        }
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_DOSS_GetWH(PANEL_TYPE_E enPanelType, VO_INTF_SYNC_E enIntfSync,
    EI_U32 *pu32W, EI_U32 *pu32H, EI_U32 *pu32Frm)
{
    PANEL_OBJ_S *pstPanelObj;
    PANEL_TYPE_ATTR_S stPanelAttr = {0};
    VO_SYNC_INFO_S *pstSync = &stPanelAttr.stVoPubAttr.stSyncInfo;
    EI_S32 s32Ret;

    switch (enIntfSync) {
    case VO_OUTPUT_USER:
        pstPanelObj = SAMPLE_COMM_DOSS_GetPanelObj(enPanelType);

        if (!pstPanelObj) {
            EI_TRACE_DOSS(EI_DBG_ERR, "panel %d not exist!\n", enPanelType);
            return EI_FAILURE;
        }

        if (pstPanelObj->pfnGetPanelAttrByPanel) {
            s32Ret = pstPanelObj->pfnGetPanelAttrByPanel(enPanelType, &stPanelAttr);
            if (s32Ret) {
                EI_TRACE_DOSS(EI_DBG_ERR, "get panel attr by PANEL_TYPE_E fail\n");
                return s32Ret;
            }
        }

        *pu32W = pstSync->u16Hact;
        *pu32H = pstSync->u16Vact;
        *pu32Frm = pstSync->u32Pclk * 1000 /
            ((pstSync->u16Hact + pstSync->u16Hfb + pstSync->u16Hbb + pstSync->u16Hpw) *
                (pstSync->u16Vact + pstSync->u16Vfb + pstSync->u16Vbb + pstSync->u16Vpw));

        break;
    default:
        EI_TRACE_DOSS(EI_DBG_ERR, "vo enIntfSync %d not support!\n", enIntfSync);
        return EI_FAILURE;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_DOSS_StartDev(VO_DEV VoDev, PANEL_TYPE_ATTR_S *pstPanelAttr)
{
    EI_S32 s32Cnt = 0, s32Ret = EI_SUCCESS;
    VO_PUB_ATTR_S *pstPubAttr;

    if (!pstPanelAttr) {
        EI_TRACE_DOSS(EI_DBG_ERR, "argument NULL\n");
        return EI_ERR_VO_NULL_PTR;
    }
    pstPubAttr = &pstPanelAttr->stVoPubAttr;

reconfig:
    s32Ret = EI_MI_VO_SetPubAttr(VoDev, pstPubAttr);
    if (s32Ret != EI_SUCCESS) {
        if (s32Ret == EI_ERR_VO_DEV_HAS_ENABLED) {
            s32Cnt++;
            if (s32Cnt > 1) {
                EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_SetPubAttr() reconfig failed\n");
                return EI_FAILURE;
            }
            s32Ret = EI_MI_VO_Disable(VoDev);
            if (s32Ret != EI_SUCCESS) {
                EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_Disable() failed with %#x!\n", s32Ret);
                return s32Ret;
            }
            EI_MI_VO_CloseFd();
            goto reconfig;
        } else {
            EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_SetPubAttr() failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    if ((pstPubAttr->enIntfType >= VO_INTF_MIPI) &&
        (pstPubAttr->enIntfType <= VO_INTF_I8080_8BIT_RGB888)) {
        cmd_info_t Cmd = { 0 };

        if (pstPubAttr->enIntfType == VO_INTF_MIPI) {
            Cmd.cmd_size = pstPanelAttr->u32InitCodeLen * sizeof(unsigned char);
        } else {
            Cmd.cmd_size = pstPanelAttr->u32InitCodeLen * sizeof(unsigned int);
        }
        Cmd.cmd = (void *)pstPanelAttr->init;
        EI_TRACE_DOSS(EI_DBG_ERR, "cmd @0x%x len=%d\n", Cmd.cmd, Cmd.cmd_size);
        s32Ret = EI_MI_VO_SET_PANEL_INIT_CODE(&Cmd);
        if (s32Ret != EI_SUCCESS) {
            EI_TRACE_DOSS(EI_DBG_ERR, "send panel init code failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = EI_MI_VO_Enable(VoDev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_Enable() failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_DOSS_StopDev(VO_DEV VoDev)
{
    EI_S32 s32Ret = EI_SUCCESS;

    s32Ret = EI_MI_VO_Disable(VoDev);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_Disable() failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_DOSS_StartLayer(VO_LAYER VoLayer, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr)
{
    EI_S32 s32Ret = EI_SUCCESS;

    s32Ret = EI_MI_VO_SetVideoLayerAttr(VoLayer, pstLayerAttr);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = EI_MI_VO_EnableVideoLayer(VoLayer);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

EI_S32 SAMPLE_COMM_DOSS_StopLayer(VO_LAYER VoLayer)
{
    EI_S32 s32Ret = EI_SUCCESS;

    s32Ret = EI_MI_VO_DisableVideoLayer(VoLayer);
    if (s32Ret != EI_SUCCESS) {
        EI_TRACE_DOSS(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}


EI_S32 SAMPLE_COMM_DOSS_StartChn(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode)
{
    EI_S32 s32Ret = EI_SUCCESS;
    EI_U32 i, u32WndNum = 0, u32Square = 0, u32Width  = 0, u32Height = 0;
    VO_CHN_ATTR_S         stChnAttr;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    switch (enMode) {
    case VO_MODE_1MUX:
        u32WndNum = 1;
        u32Square = 1;
        break;
    case VO_MODE_2MUX:
        u32WndNum = 2;
        u32Square = 2;
        break;
    case VO_MODE_4MUX:
        u32WndNum = 4;
        u32Square = 2;
        break;
    default:
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_VO_MODE_E %d not support!\n", enMode);
        return EI_ERR_VO_NOT_SUPPORT;
    }

    s32Ret = EI_MI_VO_GetVideoLayerAttr(VoLayer, &stLayerAttr);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "failed with %#x!\n", s32Ret);
        return s32Ret;
    }
    u32Width  = stLayerAttr.stImageSize.u32Width;
    u32Height = stLayerAttr.stImageSize.u32Height;
    EI_TRACE_DOSS(EI_DBG_INFO, "u32Width:%d, u32Height:%d\n", u32Width, u32Height);

    for (i = 0; i < u32WndNum; i++) {
        if (u32Width > u32Height) {
            stChnAttr.stRect.s32X       = EI_ALIGN_DOWN((u32Width / u32Square) * (i % u32Square), 2);
            stChnAttr.stRect.s32Y       = EI_ALIGN_DOWN((u32Height / u32Square) * (i / u32Square), 2);
            stChnAttr.stRect.u32Width   = EI_ALIGN_DOWN(u32Width / u32Square, 2);
            stChnAttr.stRect.u32Height  = EI_ALIGN_DOWN(u32Height / u32Square, 2);
        } else {
            stChnAttr.stRect.s32X       = EI_ALIGN_DOWN((u32Width / u32Square) * (i / u32Square), 2);
            stChnAttr.stRect.s32Y       = EI_ALIGN_DOWN((u32Height / u32Square) * (i % u32Square), 2);
            stChnAttr.stRect.u32Width   = EI_ALIGN_DOWN(u32Width / u32Square, 2);
            stChnAttr.stRect.u32Height  = EI_ALIGN_DOWN(u32Height / u32Square, 2);
        }
        EI_TRACE_DOSS(EI_DBG_INFO, "chn-%d disp(x,y,w,h)[%d %d %d %d]\n", i,
            stChnAttr.stRect.s32X, stChnAttr.stRect.s32Y, stChnAttr.stRect.u32Width, stChnAttr.stRect.u32Height);

        stChnAttr.u32Priority = 0;

        s32Ret = EI_MI_VO_SetChnAttr(VoLayer, i, &stChnAttr);
        if (s32Ret != EI_SUCCESS) {
            EI_TRACE_DOSS(EI_DBG_ERR, "set Volayer-%d Chn-%d Attr failed with %#x!\n", VoLayer, i, s32Ret);
            return s32Ret;
        }

        s32Ret = EI_MI_VO_EnableChn(VoLayer, i);
        if (s32Ret != EI_SUCCESS) {
            EI_TRACE_DOSS(EI_DBG_ERR, "Enable Chn-%d failed with %#x!\n", i, s32Ret);
            return s32Ret;
        }
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_DOSS_StopChn(VO_LAYER VoLayer, SAMPLE_VO_MODE_E enMode)
{
    EI_S32 s32Ret = EI_SUCCESS;
    EI_U32 i, u32WndNum = 0;

    switch (enMode) {
    case VO_MODE_1MUX:
        u32WndNum = 1;
        break;
    case VO_MODE_2MUX:
      u32WndNum = 2;
      break;
    case VO_MODE_4MUX:
      u32WndNum = 4;
      break;
    default:
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_VO_MODE_E %d not support!\n", enMode);
        return EI_ERR_VO_NOT_SUPPORT;
    }

    for (i = 0; i < u32WndNum; i++) {
        s32Ret = EI_MI_VO_DisableChn(VoLayer, i);
        if (s32Ret != EI_SUCCESS) {
            EI_TRACE_DOSS(EI_DBG_ERR, "Disable Chn-%d failed with %#x!\n", i, s32Ret);
            return s32Ret;
        }
    }

    return s32Ret;
}

/* just get config of frontier DSI panel config for now */
EI_S32 SAMPLE_COMM_DOSS_GetDefConfig(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    RECT_S  stDefDispRect  = {0, 0, 720, 1280};
    SIZE_S  stDefImageSize = {720, 1280};
    PANEL_TYPE_ATTR_S stPanelAttr = { 0 };

    if (!pstVoConfig) {
        EI_TRACE_DOSS(EI_DBG_ERR, "argument NULL\n");
        return EI_FAILURE;
    }

    if (SAMPLE_COMM_DOSS_GetPanelAttr(OTM1289A_MIPI_720_1280_60FPS, &stPanelAttr)) {
        EI_TRACE_DOSS(EI_DBG_ERR, "get panel attr fail\n");
        return EI_FAILURE;
    }

    pstVoConfig->enPanelType       = OTM1289A_MIPI_720_1280_60FPS;
    pstVoConfig->VoDev             = 0;

    pstVoConfig->VoLayer           = 0;
    pstVoConfig->enPixFormat       = PIX_FMT_YUV_SEMIPLANAR_420;
    pstVoConfig->u32Align          = 4;
    pstVoConfig->stDispRect        = stDefDispRect;
    pstVoConfig->stImageSize       = stDefImageSize;
    pstVoConfig->enVoPartMode      = VO_PART_MODE_MULTI;
    pstVoConfig->u32DisBufLen      = 3;
    pstVoConfig->enVoMode          = VO_MODE_1MUX;

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_DOSS_StartVO(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    RECT_S                stDefDispRect  = {0, 0, 720, 1280};
    SIZE_S                stDefImageSize = {720, 1280};
    VO_DEV                VoDev          = 0;
    VO_LAYER              VoLayer        = 0;
    SAMPLE_VO_MODE_E      enVoMode       = VO_MODE_1MUX;
    VO_PART_MODE_E        enVoPartMode   = VO_PART_MODE_MULTI;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr    = { 0 };
    EI_S32                s32Ret         = EI_SUCCESS;
    PANEL_TYPE_ATTR_S     stPanelAttr    = { 0 };

    if (!pstVoConfig) {
        EI_TRACE_DOSS(EI_DBG_ERR, "argument NULL\n");
        return EI_ERR_VO_NULL_PTR;
    }
    VoDev          = pstVoConfig->VoDev;
    VoLayer        = pstVoConfig->VoLayer;
    enVoMode       = pstVoConfig->enVoMode;
    enVoPartMode   = pstVoConfig->enVoPartMode;

    s32Ret = SAMPLE_COMM_DOSS_GetPanelAttr(pstVoConfig->enPanelType, &stPanelAttr);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_COMM_DOSS_Start failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_DOSS_StartDev(VoDev, &stPanelAttr);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_COMM_DOSS_Start failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_DOSS_GetWH(pstVoConfig->enPanelType, stPanelAttr.stVoPubAttr.enIntfSync,
            &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height, &stLayerAttr.u32DispFrmRt);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_COMM_DOSS_GetWH failed!\n");
        SAMPLE_COMM_DOSS_StopDev(VoDev);
        return s32Ret;
    }
    stLayerAttr.enPixFormat      = pstVoConfig->enPixFormat;
    stLayerAttr.u32Align         = pstVoConfig->u32Align;
    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    if (memcmp(&pstVoConfig->stDispRect, &stDefDispRect, sizeof(RECT_S))) {
        stLayerAttr.stDispRect = pstVoConfig->stDispRect;
    }
    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    if (memcmp(&pstVoConfig->stImageSize, &stDefImageSize, sizeof(SIZE_S))) {
        stLayerAttr.stImageSize = pstVoConfig->stImageSize;
    }

    if (pstVoConfig->u32DisBufLen) {
        s32Ret = EI_MI_VO_SetDisplayBufLen(VoLayer, pstVoConfig->u32DisBufLen);
        if (s32Ret) {
            EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_SetDisplayBufLen failed with %#x!\n", s32Ret);
            SAMPLE_COMM_DOSS_StopDev(VoDev);
            return s32Ret;
        }
    }

    s32Ret = EI_MI_VO_SetVideoLayerPartitionMode(VoLayer, enVoPartMode);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_SetVideoLayerPartitionMode failed!\n");
        SAMPLE_COMM_DOSS_StopDev(VoDev);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_DOSS_StartLayer(VoLayer, &stLayerAttr);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_COMM_DOSS_Start video layer failed!\n");
        SAMPLE_COMM_DOSS_StopDev(VoDev);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_DOSS_StartChn(VoLayer, enVoMode);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_COMM_DOSS_StartChn failed!\n");
        SAMPLE_COMM_DOSS_StopLayer(VoLayer);
        SAMPLE_COMM_DOSS_StopDev(VoDev);
        return s32Ret;
    }

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_DOSS_StopVO(SAMPLE_VO_CONFIG_S *pstVoConfig)
{
    VO_DEV            VoDev = 0;
    VO_LAYER                  VoLayer   = 0;
    SAMPLE_VO_MODE_E          enVoMode  = VO_MODE_BUTT;

    if (!pstVoConfig) {
        EI_TRACE_DOSS(EI_DBG_ERR, "argument NULL\n");
        return EI_ERR_VO_NULL_PTR;
    }
    VoDev     = pstVoConfig->VoDev;
    VoLayer   = pstVoConfig->VoLayer;
    enVoMode  = pstVoConfig->enVoMode;

    SAMPLE_COMM_DOSS_StopChn(VoLayer, enVoMode);
    SAMPLE_COMM_DOSS_StopLayer(VoLayer);
    SAMPLE_COMM_DOSS_StopDev(VoDev);

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_DOSS_GetDefLayerConfig(SAMPLE_COMM_VO_LAYER_CONFIG_S *pstVoLayerConfig)
{
    RECT_S  stDefDispRect  = {0, 0, 720, 1280};
    SIZE_S  stDefImageSize = {720, 1280};

    if (NULL == pstVoLayerConfig) {
        EI_TRACE_DOSS(EI_DBG_ERR, "argument NULL\n");
        return EI_ERR_VO_NULL_PTR;
    }
    pstVoLayerConfig->enPanelType  = OTM1289A_MIPI_720_1280_60FPS;
    pstVoLayerConfig->VoLayer      = 0;
    pstVoLayerConfig->stDispRect   = stDefDispRect;
    pstVoLayerConfig->stImageSize  = stDefImageSize;
    pstVoLayerConfig->enPixFormat  = PIX_FMT_YUV_SEMIPLANAR_420;
    pstVoLayerConfig->u32Align     = 4;
    pstVoLayerConfig->u32DisBufLen = 1;
    pstVoLayerConfig->enVoMode     = VO_MODE_1MUX;

    return EI_SUCCESS;
}

EI_S32 SAMPLE_COMM_DOSS_StartLayerChn(SAMPLE_COMM_VO_LAYER_CONFIG_S *pstVoLayerConfig)
{
    VO_LAYER              VoLayer        = 0;
    SAMPLE_VO_MODE_E      enVoMode       = VO_MODE_1MUX;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr    = { 0 };
    EI_S32                s32Ret         = EI_SUCCESS;

    if (!pstVoLayerConfig) {
        EI_TRACE_DOSS(EI_DBG_ERR, "argument NULL\n");
        return EI_ERR_VO_NULL_PTR;
    }
    VoLayer    = pstVoLayerConfig->VoLayer;
    enVoMode       = pstVoLayerConfig->enVoMode;
    stLayerAttr.enPixFormat  = pstVoLayerConfig->enPixFormat;
    stLayerAttr.u32Align     = pstVoLayerConfig->u32Align;
    stLayerAttr.stDispRect = pstVoLayerConfig->stDispRect;
    stLayerAttr.stImageSize = pstVoLayerConfig->stImageSize;

    if (pstVoLayerConfig->u32DisBufLen) {
        s32Ret = EI_MI_VO_SetDisplayBufLen(VoLayer, pstVoLayerConfig->u32DisBufLen);
        if (s32Ret) {
            EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_SetDisplayBufLen failed with %#x!\n", s32Ret);
            return s32Ret;
        }
    }

    s32Ret = EI_MI_VO_SetVideoLayerPartitionMode(VoLayer, pstVoLayerConfig->enVoPartMode);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "EI_MI_VO_SetVideoLayerPartitionMode failed with %#x!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_DOSS_StartLayer(VoLayer, &stLayerAttr);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_COMM_DOSS_Start video layer failed!\n");
        return s32Ret;
    }

    s32Ret = SAMPLE_COMM_DOSS_StartChn(VoLayer, enVoMode);
    if (s32Ret) {
        EI_TRACE_DOSS(EI_DBG_ERR, "SAMPLE_COMM_DOSS_StartChn failed!\n");
        SAMPLE_COMM_DOSS_StopLayer(VoLayer);
        return s32Ret;
    }

    return EI_SUCCESS;
}


EI_S32 SAMPLE_COMM_DOSS_StopLayerChn(SAMPLE_COMM_VO_LAYER_CONFIG_S *pstVoLayerConfig)
{
    VO_LAYER              VoLayer   = 0;
    SAMPLE_VO_MODE_E          enVoMode  = VO_MODE_BUTT;

    if (!pstVoLayerConfig) {
        EI_TRACE_DOSS(EI_DBG_ERR, "argument NULL\n");
        return EI_ERR_VO_NULL_PTR;
    }
    VoLayer   = pstVoLayerConfig->VoLayer;
    enVoMode  = pstVoLayerConfig->enVoMode;

    SAMPLE_COMM_DOSS_StopChn(VoLayer, enVoMode);
    SAMPLE_COMM_DOSS_StopLayer(VoLayer);

    return EI_SUCCESS;
}

EI_VOID SAMPLE_COMM_DOSS_Exit(EI_VOID)
{
    EI_S32 i = 0, j = 0;

    for (i = 0; i < VO_MAX_LAYER_NUM; i++) {
        for (j = 0; j < VO_MAX_CHN_NUM; j++) {
            EI_MI_VO_DisableChn(i, j);
        }
    }

    for (i = 0; i < VO_MAX_LAYER_NUM; i++) {
        EI_MI_VO_DisableVideoLayer(i);
    }

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        EI_MI_VO_Disable(i);
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
