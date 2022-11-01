#ifndef __IPC_MEDIA_BUFFER_H__
#define __IPC_MEDIA_BUFFER_H__

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

typedef enum IPC_MEDIA_FRAME_TYPE
{
    VIDEO_I_FRAME = 0,
    VIDEO_P_FRAME = 1,
    VIDEO_J_FRAME = 2,
    AUDIO_A_FRAME = 3,
} IPC_MEDIA_FRAME_TYPE;

typedef struct IPC_MEDIA_FRAME_HEADER
{
    IPC_MEDIA_FRAME_TYPE enType;
    unsigned int         u32size;
    unsigned long long   u64pts;
} IPC_MEDIA_FRAME_HEADER;

typedef struct IPC_MEDIA_BUFFER
{
    void*           pBuf;
    int             s32BufSize;
    int             s32MaxLen;
    int             s32CurIndex;
    int             s32CurOffset;
    int             s32FirstIndex;
    int             s32FirstOffset;
    int             s32LastIndex;
    int             s32LastOffset;
    int             s32NewestIFrameIndex;
    int             s32NewestIFrameOffset;
    int             s32EndOffset;
    int             s32DeadlineOffset;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
} IPC_MEDIA_BUFFER;

int IPC_MEDIA_BUFFER_Read(IPC_MEDIA_BUFFER *pmbuffer, void* pdata, unsigned int u32size,
                                    unsigned int *u32len, unsigned long long *u64pts);
int IPC_MEDIA_BUFFER_Write(IPC_MEDIA_BUFFER *pmbuffer, IPC_MEDIA_FRAME_TYPE mft,
                                      void* pdata, unsigned int u32len, unsigned long long u64pts);

IPC_MEDIA_BUFFER *IPC_MEDIA_BUFFER_Init(int s32Size);
void IPC_MEDIA_BUFFER_UnInit(IPC_MEDIA_BUFFER *pmbuffer);


#ifdef __cplusplus
#if __cplusplus
        }
#endif
#endif /* __cplusplus */

#endif /* __IPC_MEDIA_BUFFER_H__ */
