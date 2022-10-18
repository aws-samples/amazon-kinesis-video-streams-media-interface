#ifndef __IPC_VIDEO_API_H__
#define __IPC_VIDEO_API_H__

#include "ipc_cfg_api.h"
#include "ipc_video_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

int IPC_VIDEO_GetStreamNum();
unsigned int IPC_VIDEO_GetStreamYUVSize(int sStreamId);

int IPC_VIDEO_SetStreamConfig(int nStreamId, IPC_VFORMAT_TYPE fmt, IPC_RESOLUTION_SIZE *vres,
                                           IPC_VCODEC_TYPE codec, unsigned int framerate, unsigned int profile);

int IPC_VIDEO_ReqIFrame(int sStreamId);
int IPC_VIDEO_GetFrame(IPC_VIDEO_FRAME *pvframe, int sStreamId, unsigned int uYuvSize);

void IPC_VIDEO_UpdateIspAttr();
int IPC_VIDEO_Enable();
void IPC_VIDEO_Disable();

int IPC_VIDEO_Init();
void IPC_VIDEO_UnInit();


#ifdef __cplusplus
#if __cplusplus
        }
#endif
#endif /* __cplusplus */

#endif /* __IPC_VIDEO_API_H__ */
