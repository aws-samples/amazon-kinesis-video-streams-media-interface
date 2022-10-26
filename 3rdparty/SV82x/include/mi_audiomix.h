/*
 *------------------------------------------------------------------------------
 * @File      :    mi_audiomix.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __MI_AUDIO_MIX_H__
#define __MI_AUDIO_MIX_H__

#include <stddef.h>
#include "ei_type.h"
#include "ei_comm_aio.h"


#if defined(__cplusplus)
extern "C" {
#endif

/* TLV header size*/
#define TLV_HEADER_SIZE (2 * sizeof(unsigned int))

typedef struct eiAIO_Mixer AIO_Mixer_S;
typedef struct eiAIO_Mixer_Ctl AIO_Mixer_Ctl_S;


/** Mixer control type.
 * @ingroup libtinyalsa-mixer
 */
typedef enum eiAIO_Mixer_Ctl_Type {
    /** boolean control type */
    MIXER_CTL_TYPE_BOOL,
    /** integer control type */
    MIXER_CTL_TYPE_INT,
    /** an enumerated control type */
    MIXER_CTL_TYPE_ENUM,
    MIXER_CTL_TYPE_BYTE,
    MIXER_CTL_TYPE_IEC958,
    /** a 64 bit integer control type */
    MIXER_CTL_TYPE_INT64,
    /** unknown control type */
    MIXER_CTL_TYPE_UNKNOWN,
    /** end of the enumeration (not a control type) */
    MIXER_CTL_TYPE_MAX,
} AIO_Mixer_Ctl_Type_S;

AIO_Mixer_S *MixerOpen(EI_U32 Card);

EI_VOID MixerClose(AIO_Mixer_S *pstMixer);

EI_S32 MixerAddNewCtls(AIO_Mixer_S *pstMixer);


AIO_Mixer_Ctl_S *MixerGetCtlByName(AIO_Mixer_S *pstMixer, const EI_CHAR *pName);

AIO_Mixer_Ctl_S *MixerGetCtlByNameAndIndex(AIO_Mixer_S *pstMixer,
    const EI_CHAR *pName,
    EI_U32 u32Index);


AIO_Mixer_Ctl_Type_S MixerCtlGetType(const AIO_Mixer_Ctl_S *pstCtl);

const char *mixer_ctl_get_type_string(const AIO_Mixer_Ctl_S *ctl);

EI_U32 MixerCtlGetNumValues(const AIO_Mixer_Ctl_S *pstCtl);

EI_S32 MixerCtlGetValue(const AIO_Mixer_Ctl_S *pstCtl, EI_U32 u32Id);

EI_S32 MixerCtlSetValue(AIO_Mixer_Ctl_S *pstCtl, EI_U32 u32Id, EI_S32 s32Value);

/* Determe range of integer mixer controls */
EI_S32 MixerCtlGetRangeMin(const AIO_Mixer_Ctl_S *pstCtl);

EI_S32 MixerCtlGetRangeMax(const AIO_Mixer_Ctl_S *pstCtl);

#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif

