#ifndef __IPC_VIDEO_DEFINE_H__
#define __IPC_VIDEO_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "ipc_cfg_api.h"
#include "ipc_media_buffer.h"

typedef enum IPC_SNS_TYPE
{
    SONY_IMX307 = 0,
    SONY_IMX335,
    GALAXYCORE_GC4653,
    SONY_IMX415,
    SNS_TYPE_BUTT
} IPC_SNS_TYPE;

typedef enum IPC_SENSOR_ID {
    SENSOR_IMX307 = 0,                             // 200W
    SENSOR_IMX290_MIPI_2M_30FSP_12BIT = 2,         // 200W
    SENSOR_IMX290_MIPI_2M_30FSP_12BIT_WDR2TO1 = 3, // 200W
    SENSOR_GC4653_MIPI_4M_30FPS_10BIT = 4,
    SENSOR_GC4653_MIPI_2M_30FPS_10BIT_WDR2TO1
} IPC_SENSOR_ID;

typedef enum IPC_PPU_PIXEL_FORMAT {
    PPU_PIXEL_FORMAT_RGB_888 = 0,
    PPU_PIXEL_FORMAT_RGB_BAYER_8BPP,
    PPU_PIXEL_FORMAT_RGB_BAYER_16BPP,
    PPU_PIXEL_FORMAT_YVU_SEMIPLANAR_420,   //nv21
    //PPU_PIXEL_FORMAT_YUYV_PACKAGE_422,
    PPU_PIXEL_FORMAT_BUTT
} IPC_PPU_PIXEL_FORMAT;

typedef enum IPC_H264_PROFILE {
    H264_BASELINE = 0,
    H264_MAIN_PROFILE,
    H264_HIGH_PROFILE,
    H264_SVC_T
} IPC_H264_PROFILE;

typedef enum IPC_H265_PROFILE {
    H265_MAIN_PROFILE = 0,
    H265_MAIN10_PROFILE
} IPC_H265_PROFILE;

typedef enum IPC_JPEG_PROFILE {
    JPEG_BASELINE = 0
} IPC_JPEG_PROFILE;

typedef struct IPC_RESOLUTION_SIZE
{
    unsigned int     u32Width;
    unsigned int     u32Height;
} IPC_RESOLUTION_SIZE;

typedef struct IPC_VIDEO_FRAME
{
    unsigned char*       u8data;
    unsigned int         u32size;
    unsigned int         u32len;
    unsigned long long   u64pts;
} IPC_VIDEO_FRAME;

//=====================================sensor api=============================================
int IPC_SENSOR_TypeCheck(IPC_VIDEO_CONFIG *pstVideoConfig);
IPC_SNS_TYPE IPC_VIDEO_GetSnsType(char *pSnsName);
int IPC_VIDEO_GetSnsSize(IPC_SNS_TYPE enSnsType, IPC_RESOLUTION_SIZE *pstSize);
unsigned int IPC_VIDEO_GetInputFps(IPC_FRAMERATE_MODE enMode);
unsigned int IPC_VIDEO_GetMipiLane(IPC_INPUT_LANE enInLane);
int IPC_VIDEO_GetResolution(IPC_RESOLUTION enResolution, IPC_RESOLUTION_SIZE *pstSize);
IPC_RESOLUTION IPC_VIDEO_GetResItem(IPC_RESOLUTION_SIZE *pstSize);

//=====================================isp api=============================================
int IPC_ISP_SetSnsBus(int nViPipe, IPC_SNS_TYPE enSnsType, char s8SnsDev);
int IPC_ISP_SnsRegisterCallback(int nViPipe, IPC_SNS_TYPE enSnsType);
int IPC_ISP_SnsUnRegisterCallback(int nViPipe);
int IPC_ISP_AelibCallback(int nViPipe);
int IPC_ISP_AelibUnCallback(int nViPipe);
int IPC_ISP_AwblibCallback(int nViPipe);
int IPC_ISP_AwblibUnCallback(int nViPipe);
int IPC_ISP_AlglibCallback(int nViPipe);
int IPC_ISP_AlglibUnCallback(int nViPipe);

int IPC_ISP_SetCSCAttr(int sChn, IPC_IMAGE_CONFIG *pstImageCfg);
int IPC_ISP_SetHLCAttr(int sChn, IPC_IMAGE_CONFIG *pstImageCfg);
int IPC_ISP_SetExpAttr(int sChn, IPC_IMAGE_CONFIG *pstImageCfg);
int IPC_ISP_SetWBAttr(int sChn, IPC_IMAGE_CONFIG *pstImageCfg);
int IPC_ISP_SetMirrorAndFlip(int sChn, unsigned char uMirror, unsigned char uFlip);

//=====================================ppu api=============================================
int IPC_PPU_GetChnStatus(int sPpuGrp, int sPpuChn);
int IPC_PPU_SetLdcAttr(int sPpuGrp, IPC_IMAGE_CONFIG *pstImageCfg, IPC_STREAM_CONFIG *pstStreamCfg);


//=====================================vi api=============================================
int IPC_VI_GetViNum();
int IPC_VI_GetViChn(int sViId);
int IPC_VI_GetViPipe(int sViId);
int IPC_VI_SetInputFps(int sViId, unsigned int uFps);
int IPC_VI_GetWdr(int sViId);
int IPC_VI_SetWdr(int sViId, int sWDR);

//=====================================ircut api=============================================
int IPC_IRCUT_SetCfg(int sChn, IPC_IMAGE_CONFIG *pstImageCfg);


//=======================================base api=================================================
int IPC_SYS_Init(IPC_VIDEO_CONFIG *pstVideoConfig);
void IPC_SYS_UnInit();

int IPC_VI_Start();
void IPC_VI_Stop();
int IPC_VI_Init(IPC_VIDEO_CONFIG *pstVideoConfig);
void IPC_VI_UnInit();

int IPC_ISP_RestartSns(int nViPipe, IPC_SNS_TYPE enSnsType);
int IPC_ISP_StandbySns(int nViPipe, IPC_SNS_TYPE enSnsType);
unsigned int IPC_ISP_CalcFrameSize(int sPpuGrp);
int IPC_ISP_ChnStart();
void IPC_ISP_ChnStop();
int IPC_ISP_ChnInit(IPC_VIDEO_CONFIG *pstVideoCfg);
void IPC_ISP_ChnUnInit();

int IPC_PPU_GetFrame(int sPpuGrp, void* pdata, unsigned int u32size,
                             unsigned int *u32len, unsigned long long *u64pts);
int IPC_PPU_Start();
void IPC_PPU_Stop();
int IPC_PPU_Init(IPC_VIDEO_CONFIG *pstVideoCfg);
void IPC_PPU_UnInit();

int IPC_IRCUT_Init(IPC_VIDEO_CONFIG *pstVideoCfg);
void IPC_IRCUT_UnInit();

void IPC_VFRAME_Init(IPC_VIDEO_FRAME *pvframe);
int IPC_VFRAME_Alloc(IPC_VIDEO_FRAME *pvframe, unsigned int nsize);
void IPC_VFRAME_Relase(IPC_VIDEO_FRAME *pvframe);
int IPC_VENC_ReqIFrame(int sStreamId);
int IPC_VENC_Start();
void IPC_VENC_Stop();
int IPC_VENC_Init(IPC_VIDEO_CONFIG *pstVideoCfg, IPC_MEDIA_BUFFER *pmbuffer[]);
void IPC_VENC_UnInit();

#ifdef __cplusplus
#if __cplusplus
        }
#endif
#endif /* __cplusplus */

#endif /* __IPC_VIDEO_DEFINE_H__ */
