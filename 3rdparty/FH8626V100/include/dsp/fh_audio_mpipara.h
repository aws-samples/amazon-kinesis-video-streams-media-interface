/*
 * fh_auido_mpipara.h
 *
 *  Created on: 2015年2月15日
 *      Author: fanggm
 */

#ifndef FH_AUIDO_MPIPARA_H_
#define FH_AUIDO_MPIPARA_H_

typedef enum
{
    AC_SR_8K   = 8000,
    AC_SR_16K  = 16000,
    AC_SR_32K  = 32000,
    AC_SR_48K  = 48000,
    
    AC_SR_110K = 11025,
    AC_SR_220K = 22050,
    AC_SR_441K = 44100,
}FH_AC_SAMPLE_RATE_E;

typedef enum
{
    AC_BW_8  = 8,
    AC_BW_16 = 16,
    AC_BW_24 = 24,
}FH_AC_BIT_WIDTH_E;


typedef struct
{
    FH_UINT32 len;
    FH_UINT8 *data;
}FH_AC_FRAME_S;

typedef enum
{
    FH_AC_MIC_IN = 0,
    FH_AC_LINE_IN = 1,
    FH_AC_SPK_OUT = 2,
    FH_AC_LINE_OUT = 3
}FH_AC_IO_TYPE_E;

typedef enum
{
    FH_AO_MODE_LEFT = 0,  /*It's the default value, supported by all chips*/
    FH_AO_MODE_RIGHT = 1, /*Now only support for FH8856*/
    FH_AO_MODE_BOTH = 2  /*left channel and right channel output the same audio samples*/
}FH_AC_AO_MODE_E;

typedef enum
{
    FH_PT_LPCM     = 0,
    FH_PT_G711A    = 1,
    FH_PT_G711U    = 2,
    FH_PT_G726_16K = 3,
    FH_PT_G726_32K = 4,
    FH_PT_AAC      = 5,
    FH_PT_MAX,
}FH_AC_PT_TYPE_E;

typedef enum
{
    FH_AC_WORKMODE_NORMAL   = 0, /*This is the default*/
    FH_AC_WORKMODE_HIGHFREQ = 1
}FH_AC_WORKMODE_E;

typedef struct
{
    FH_AC_IO_TYPE_E io_type;
    FH_AC_SAMPLE_RATE_E sample_rate;
    FH_AC_BIT_WIDTH_E bit_width;
    FH_AC_PT_TYPE_E   enc_type;  /*encoding type*/
    FH_UINT32 channels;         /*通道个数 */
    FH_UINT32 period_size;      /*一帧数据中的采样点个数 */
    FH_UINT32 volume;
}FH_AC_CONFIG;


typedef struct
{
    FH_UINT32 enable;
    FH_UINT32 msInSndCard;
}FH_AC_AEC_CONFIG;

typedef struct
{
    FH_UINT32 enable;
    FH_UINT32 level;
}FH_AC_NR_CONFIG;

typedef struct
{
    FH_UINT32 enable;
    FH_UINT32 gaindb;
}FH_AC_AGC_CONFIG;

typedef struct
{
    FH_UINT32 cmd;
    FH_UINT32 cmd_len; /*command length, not include the header itself...*/
    FH_UINT8  cmd_data[0];
}FH_AC_EXT_IOCTL_CMD;

#endif /* FH_AUIDO_MPIPARA_H_ */
