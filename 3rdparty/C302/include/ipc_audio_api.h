#ifndef __IPC_AUDIO_API_H__
#define __IPC_AUDIO_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "ipc_cfg_api.h"

typedef struct IPC_AUDIO_FRAME
{
    unsigned char*       u8data;
    unsigned int         u32size;
    unsigned int         u32len;
    unsigned long long   u64pts;
} IPC_AUDIO_FRAME;

void IPC_AFRAME_Init(IPC_AUDIO_FRAME *paframe);
int IPC_AFRAME_Alloc(IPC_AUDIO_FRAME *paframe, unsigned int nsize);
void IPC_AFRAME_Release(IPC_AUDIO_FRAME *paframe);
int IPC_AUDIO_GetFrame(IPC_AUDIO_FRAME *paframe);

unsigned int IPC_AUDIO_GetFrmSize();
int IPC_AUDIO_SendFrame(IPC_AUDIO_FRAME *paframe);

int IPC_AUDIO_CheckEnable(unsigned int uflag);
void IPC_AUDIO_PrintFormat(unsigned int uflag);
void IPC_AUDIO_GetFormat(unsigned int uflag, unsigned int *usamplerate,
                                unsigned int *uchannel, unsigned int *ubitwidth);

int IPC_AUDIO_Enable(unsigned int uflag);
void IPC_AUDIO_Disable(unsigned int uflag);

int IPC_AUDIO_SetConfig(unsigned int uflag, IPC_ACODEC_TYPE codec_type,
                                 unsigned int nchannel, IPC_AUDIO_SAMPLERATE sample_rate,
                                 IPC_AUDIO_BITWIDTH bit_width, unsigned int nvolume);

int IPC_AUDIO_UpdateAttr(unsigned int uflag);
int IPC_AUDIO_Init(unsigned int uflag);
void IPC_AUDIO_UnInit(unsigned int uflag);

int IPC_AUDIO_SYS_Init();
void IPC_AUDIO_SYS_UnInit();

#ifdef __cplusplus
#if __cplusplus
        }
#endif
#endif /* __cplusplus */

#endif /* __IPC_AUDIO_API_H__ */
