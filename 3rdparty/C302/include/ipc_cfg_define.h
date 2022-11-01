#ifndef __IPC_CFG_DEFINE_H__
#define __IPC_CFG_DEFINE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX_SENSOR_NUM      1
#define CHN_STREMA_NUM      1

#define MAX_STREAM_NUM      (MAX_SENSOR_NUM * CHN_STREMA_NUM)
//#define MAX_CAPTURE_NUM     (MAX_SENSOR_NUM * 1)
//#define MAX_VENC_NUM        (MAX_STREAM_NUM + MAX_CAPTURE_NUM)

#define STR_COMM_LENGTH8    8
#define STR_COMM_LENGTH16   16
#define STR_COMM_LENGTH32   32
#define STR_COMM_LENGTH64   64
#define STR_COMM_LENGTH128  128

#define MAX_CFG_NUM         64
#define MAX_CFG_AREA_NUM    4


typedef enum IPC_CFG_DATA_TYPE
{
    CFG_DATA_TYPE_U32 = 0,
    CFG_DATA_TYPE_U16,
    CFG_DATA_TYPE_U8,
    CFG_DATA_TYPE_S32,
    CFG_DATA_TYPE_S16,
    CFG_DATA_TYPE_S8,
    CFG_DATA_TYPE_FLOAT,
    CFG_DATA_TYPE_STR,
    CFG_DATA_TYPE_ARRSTR,
    //CFG_DATA_TYPE_TIME,
    //CFG_DATA_TYPE_POINT,
    //CFG_DATA_TYPE_USER,
    //CFG_DATA_TYPE_RECT,
    //CFG_DATA_TYPE_MASK,
    CFG_DATA_TYPE_BUTT
} IPC_CFG_DATA_TYPE;

typedef enum IPC_CFG_RW_MODE
{
    CFG_READ_ONLY = 0,
    CFG_READ_WRITE
} IPC_CFG_RW_MODE;

typedef struct IPC_CFG_OPTIONS
{
    char*               pName;
    void*               pData;
    IPC_CFG_DATA_TYPE   enType;
    IPC_CFG_RW_MODE     enMode;
    char*               pDefValue;
    double              dMin;
    double              dMax;
    char*               pDescription;
} IPC_CFG_OPTIONS;

typedef struct IPC_CFG_OPTIONS_ARRAY
{
    int             sIdx;
    int             sArea;
    int             sRow[MAX_CFG_AREA_NUM];
    int             sCol[MAX_CFG_AREA_NUM];
    char            AreaName[MAX_CFG_AREA_NUM][8];
    IPC_CFG_OPTIONS stCfg[MAX_CFG_NUM];
} IPC_CFG_OPTIONS_ARRAY;

typedef enum IPC_FRAMERATE_MODE
{
    FRAMERATE_MODE_15 = 0,
    FRAMERATE_MODE_25,
    FRAMERATE_MODE_30,
    FRAMERATE_MODE_50,
    FRAMERATE_MODE_60
} IPC_FRAMERATE_MODE;

typedef enum IPC_INPUT_LANE
{
    SENSOR_INPUT_4LANE = 0,
    SENSOR_INPUT_2LANE,
} IPC_INPUT_LANE;

typedef enum IPC_RESOLUTION
{
    PIC_CIF = 0,    // 352x288
    PIC_360P,       // 640x360
    PIC_D1,         // 704x576
    PIC_720P,       // 1280x720
    PIC_1080P,      // 1920x1080
    PIC_2048x1536,
    PIC_2560x1440,
    PIC_2592x1944,
    PIC_3840x2160,
    PIC_BUTT
} IPC_RESOLUTION;

typedef enum IPC_VFORMAT_TYPE
{
    FORMAT_NONE = 0,
    FORMAT_NV21
} IPC_VFORMAT_TYPE;

typedef enum IPC_VCODEC_TYPE
{
    CODEC_H264 = 0,
    CODEC_H265,
    CODEC_MJPEG,
    CODEC_VBUTT
} IPC_VCODEC_TYPE;

typedef enum IPC_ACODEC_TYPE
{
    CODEC_NONE = 0,
    CODEC_G711A,
    CODEC_G711U,
    CODEC_G726,
    CODEC_AAC,
    CODEC_ABUTT
} IPC_ACODEC_TYPE;

typedef enum IPC_GOP_MODE
{
    GOP_MODE_NORMALP = 0,
    GOP_MODE_DUALP,
    GOP_MODE_SMARTP
} IPC_GOP_MODE;

typedef enum IPC_RC
{
    IPC_RC_CBR = 0,
    IPC_RC_VBR,
    IPC_RC_AVBR,
    IPC_RC_QVBR,
    IPC_RC_CVBR,
    IPC_RC_QPMAP,
    IPC_RC_FIXQP
} IPC_RC;

typedef enum IPC_IRCUT_TYPE
{
    IRCUT_DAY = 0,
    IRCUT_NIGHT,
    IRCUT_AUTO,
    IRCUT_TIMING
} IPC_IRCUT_TYPE;

typedef enum IPC_AUDIO_SAMPLERATE
{
    AUDIO_SAMPLERATE_8K = 0,
    AUDIO_SAMPLERATE_12K,
    AUDIO_SAMPLERATE_16K,
    AUDIO_SAMPLERATE_24K,
    AUDIO_SAMPLERATE_32K,
    AUDIO_SAMPLERATE_48K,
    AUDIO_SAMPLERATE_64K,
    AUDIO_SAMPLERATE_96K,
    AUDIO_SAMPLERATE_BUTT
} IPC_AUDIO_SAMPLERATE;

typedef enum IPC_AUDIO_BITWIDTH
{
    AUDIO_BITWIDTH_8 = 0,
    AUDIO_BITWIDTH_16,
    AUDIO_BITWIDTH_24,
    AUDIO_BITWIDTH_32,
    AUDIO_BITWIDTH_BUTT,
} IPC_AUDIO_BITWIDTH;

#pragma pack(push, 4)

/*----------------------------------video parameters----------------------------------*/
/* video input */
typedef struct IPC_VI_CONFIG
{
    char                        szSnsType[STR_COMM_LENGTH16];                       // sensor type
    IPC_FRAMERATE_MODE          enFramerate;                                        // input framerate mode
    IPC_INPUT_LANE              enInputLane;                                        // input lane
} IPC_VI_CONFIG;

/* video encode stream */
typedef struct IPC_STREAM_CONFIG
{
    int                         s32ViId;
    int                         s32IspId;
    IPC_RESOLUTION              enResolution;                                       // resolution
    IPC_VFORMAT_TYPE            enFormat;                                           // 0-need encode，1-NV21
    IPC_VCODEC_TYPE             enCodec;                                            // encode type
    unsigned int                u32Framerate;                                       // framerate
    unsigned int                u32Profile;                                         // H.264:0-baseline, 1-MP, 2-HP 3-SVC-T; H.265:0-MP, 1-Main; Jpege/MJpege:0-baseline
    unsigned int                u32H26xGop;                                         // H264 H265 Gop
    IPC_GOP_MODE                enGopMode;                                          // Gop mode
    IPC_RC                      enRcMode;                                           // RC mode
    unsigned int                u32Bitrate;                                         // CBR QVBR(TargetBitRate) valid
    unsigned int                u32MaxBitrate;                                      // VBR AVBR CVBR(&LongTermMaxBitrate) valid
    unsigned int                u32MinBitrate;                                      // CVBR(LongTermMinBitrate) valid
    unsigned int                u32IQp;                                             // H26X FIXQP valid
    unsigned int                u32PQp;                                             // H26X FIXQP valid
    unsigned int                u32BQp;                                             // H26X FIXQP valid
    unsigned int                u32QpMapMode;                                       // H265 QPMAP valid
    unsigned int                u32Quality;                                         // MJPEG valid
} IPC_STREAM_CONFIG;

/* 图像 */
typedef struct IPC_IMAGE_CONFIG
{
    unsigned char               u8Flip;                                             // 0-not flip, 1-flip
    unsigned char               u8Mirror;                                           // 0-not mirror, 1-mirror
    unsigned char               u8ExpMode;                                          // exposure mode: 0-Auto(slow shutter) 1-Auto(fix frame) 2-Manual
    unsigned char               u8WbMode;                                           // whitebalance mode: 0-Manual 1-Auto
    unsigned int                u32ExpTime;                                         // manual exposure time
    unsigned int                u32ExpDGain;                                        // manual isp digital gain
    unsigned char               u8RGain;                                            // manual wb red gain
    unsigned char               u8BGain;                                            // manual wb green gain
    unsigned char               u8Denoise3d;                                        // 3D noise reduction
    unsigned char               u8Brightness;                                       // brightness
    unsigned char               u8Saturation;                                       // saturation
    unsigned char               u8Contrast;                                         // contrast
    unsigned char               u8Sharpen;                                          // sharp
    unsigned char               u8Hue;                                              // hue
    unsigned char               u8WDR;                                              // WDR
    unsigned char               u8LDC;                                              // distortion correction
    unsigned char               u8HLC;                                              // strong light suppression
    IPC_IRCUT_TYPE              enIrcutType;                                        // IRCUT type
    unsigned int                u32DayToNight;                                      // time for day to night(timing mode valid)
    unsigned int                u32NightToDay;                                      // time for night to day(timing mode valid)
} IPC_IMAGE_CONFIG;

typedef struct IPC_VIDEO_CONFIG
{
    int                         s32SensorNum;                                       // sensor number
    int                         s32StreamNum;                                       // video stream number
    IPC_VI_CONFIG               stViCfg[MAX_SENSOR_NUM];                            // video input
    IPC_STREAM_CONFIG           stStreamCfg[MAX_STREAM_NUM];                        // video encode stream
    IPC_IMAGE_CONFIG            stImageCfg[MAX_SENSOR_NUM];                         // image parameters
} IPC_VIDEO_CONFIG;
/*---------------------------------video parameters end--------------------------------*/

/*----------------------------------audio parameters----------------------------------*/
/* audio input */
typedef struct IPC_AI_CONFIG
{
    unsigned int                u32Enabled;                                         // enabled
    IPC_ACODEC_TYPE             enCodec;                                            // encode type
    unsigned int                u32Channels;                                        // channel number
    IPC_AUDIO_SAMPLERATE        enSampleRate;                                       // samplerate
    IPC_AUDIO_BITWIDTH          enBitWidth;                                         // bit width
    unsigned int                u32Volume;                                          // volume
    //unsigned int                u32VqeEnable;
} IPC_AI_CONFIG;

/* audio output */
typedef struct IPC_AO_CONFIG
{
    unsigned int                u32Enabled;                                         // enabled
    IPC_ACODEC_TYPE             enCodec;                                            // encode type
    unsigned int                u32Channels;                                        // channel number
    IPC_AUDIO_SAMPLERATE        enSampleRate;                                       // samplerate
    IPC_AUDIO_BITWIDTH          enBitWidth;                                         // bit width
    unsigned int                u32Volume;                                          // volume
} IPC_AO_CONFIG;

/*--------------------------------audio parameters end---------------------------------*/

int IPC_CFG_CheckCfgDir();
int IPC_CFG_CopyFile(char *pBackFile, char *pCfgFile);

int IPC_CFG_JsonToDataByOptions(IPC_CFG_OPTIONS *pOptionsArray, char *pSrc, char *pSection,
                                             int sAm, int sAn, IPC_CFG_OPTIONS pOptionsArrayCom[sAm][sAn]);
char *IPC_CFG_DataToJsonByOptions(IPC_CFG_OPTIONS *pOptionsArray, IPC_CFG_OPTIONS_ARRAY *pOptionsArrayCom, char *pSection);

void IPC_CFG_LoadDefValue(IPC_CFG_OPTIONS *pOptionsArray, int sAm, int sAn, IPC_CFG_OPTIONS pOptionsArrayCom[sAm][sAn]);

void IPC_CFG_PrintOptions(IPC_CFG_OPTIONS *pOptionsArray, int sAm, int sAn, IPC_CFG_OPTIONS pOptionsArrayCom[sAm][sAn]);

char *IPC_CFG_ReadFromFile(const char *pFilename);
int IPC_CFG_WriteToFile(const char *pFilename, const char *pData);


#ifdef __cplusplus
#if __cplusplus
        }
#endif
#endif /* __cplusplus */

#endif /* __IPC_CFG_DEFINE_H__ */
