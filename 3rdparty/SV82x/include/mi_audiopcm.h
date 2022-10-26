/*
 *------------------------------------------------------------------------------
 * @File      :    mi_audiopcm.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Media Interface for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

/** @file */

/** @defgroup libaudiocommon PCM Interface
 * @brief All macros, structures and functions that make up the PCM interface.
 */

#ifndef __MI_AUDIO_PCM_H__
#define __MI_AUDIO_PCM_H__

#include <sys/time.h>
#include <stddef.h>
#include "ei_type.h"
#include "ei_comm_aio.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define DEFAULT_PERIOD_COUNT 4

#define PCM_ERROR_MAX 128

#ifndef INT_MAX
#define INT_MAX     ((int)(~0U>>1))
#endif

#define UINT_MAX    (~0U)

#define FORMAT_PCM 1

typedef struct eiAIO_PCM AIO_PCM_S;

/** A flag that specifies that the PCM is an output.
 * May not be bitwise AND'd with @ref PCM_IN.
 * Used in @ref pcm_open.
 * @ingroup libaudiocommon
 */
#define PCM_OUT 0x00000000

/** Specifies that the PCM is an input.
 * May not be bitwise AND'd with @ref PCM_OUT.
 * Used in @ref pcm_open.
 * @ingroup libaudiocommon
 */
#define PCM_IN 0x10000000

/** Specifies that the PCM will use mmap read and write methods.
 * Used in @ref pcm_open.
 * @ingroup libaudiocommon
 */
#define PCM_MMAP 0x00000001

/** Specifies no interrupt requests.
 * May only be bitwise AND'd with @ref PCM_MMAP.
 * Used in @ref pcm_open.
 * @ingroup libaudiocommon
 */
#define PCM_NOIRQ 0x00000002

/** When set, calls to @ref pcm_write
 * for a playback stream will not attempt
 * to restart the stream in the case of an
 * underflow, but will return -EPIPE instead.
 * After the first -EPIPE error, the stream
 * is considered to be stopped, and a second
 * call to pcm_write will attempt to restart
 * the stream.
 * Used in @ref pcm_open.
 * @ingroup libaudiocommon
 */
#define PCM_NORESTART 0x00000004

/** Specifies monotonic timestamps.
 * Used in @ref pcm_open.
 * @ingroup libaudiocommon
 */
#define PCM_MONOTONIC 0x00000008

/** For inputs, this means the PCM is recording audio samples.
 * For outputs, this means the PCM is playing audio samples.
 * @ingroup libaudiocommon
 */
#define PCM_STATE_RUNNING   0x03

/** For inputs, this means an overrun occured.
 * For outputs, this means an underrun occured.
 */
#define PCM_STATE_XRUN 0x04

/** For outputs, this means audio samples are played.
 * A PCM is in a draining state when it is coming to a stop.
 */
#define PCM_STATE_DRAINING 0x05

/** Means a PCM is suspended.
 * @ingroup libaudiocommon
 */
#define PCM_STATE_SUSPENDED 0x07

/** Means a PCM has been disconnected.
 * @ingroup libaudiocommon
 */
#define PCM_STATE_DISCONNECTED 0x08

/** Audio sample format of a PCM.
 * The first letter specifiers whether the sample is signed or unsigned.
 * The letter 'S' means signed. The letter 'U' means unsigned.
 * The following number is the amount of bits that the sample occupies in memory.
 * Following the underscore, specifiers whether the sample is big endian or little endian.
 * The letters 'LE' mean little endian.
 * The letters 'BE' mean big endian.
 * This enumeration is used in the @ref pcm_config structure.
 * @ingroup libaudiocommon
 */
typedef enum eiAIO_PCM_FORMAT {
    /** Signed, 8-bit */
    PCM_FORMAT_S8 = 1,
    /** Signed 16-bit, little endian */
    PCM_FORMAT_S16_LE = 0,
    /** Signed, 16-bit, big endian */
    PCM_FORMAT_S16_BE = 2,
    /** Signed, 24-bit (32-bit in memory), little endian */
    PCM_FORMAT_S24_LE,
    /** Signed, 24-bit (32-bit in memory), big endian */
    PCM_FORMAT_S24_BE,
    /** Signed, 24-bit, little endian */
    PCM_FORMAT_S24_3LE,
    /** Signed, 24-bit, big endian */
    PCM_FORMAT_S24_3BE,
    /** Signed, 32-bit, little endian */
    PCM_FORMAT_S32_LE,
    /** Signed, 32-bit, big endian */
    PCM_FORMAT_S32_BE,
    /** Max of the enumeration list, not an actual format. */
    PCM_FORMAT_MAX
} AIO_PCM_FORMAT_S;

/** A bit mask of 256 bits (32 bytes) that describes some hardware parameters of a PCM */
typedef struct eiAIO_PCM_MASK {
    /** bits of the bit mask */
    EI_U32 bits[32 / sizeof(EI_U32)];
} AIO_PCM_MASK_S;

/** Encapsulates the hardware and software parameters of a PCM.
 * @ingroup libaudiocommon
 */
typedef struct eiAIO_PCM_CONFIG {
    /** The number of channels in a frame */
    EI_U32 u32Channels;
    /** The number of frames per second */
    EI_U32 u32Rate;
    /** The number of frames in a period */
    EI_U32 u32PeriodSize;
    /** The number of periods in a PCM */
    EI_U32 u32PeriodCount;
    /** The sample format of a PCM */
    AIO_PCM_FORMAT_S enFormat;
    /* Values to use for the ALSA start, stop and silence thresholds.  Setting
     * any one of these values to 0 will cause the default tinyalsa values to be
     * used instead. defaults are as follows.
     *
     * u32StartThreshold   : period_count * period_size
     * u32StopThreshold    : period_count * period_size
     * u32SilenceThreshold : 0
     */
    /** The minimum number of frames required to start the PCM */
    EI_U32 u32StartThreshold;
    /** The minimum number of frames required to stop the PCM */
    EI_U32 u32StopThreshold;
    /** The minimum number of frames to silence the PCM */
    EI_U32 u32SilenceThreshold;
} AIO_PCM_CONFIG_S;


/** Enumeration of a PCM's hardware parameters.
 * Each of these parameters is either a mask or an interval.
 * @ingroup libaudiocommon
 */
typedef enum eiAIO_PCM_PARAM {
    /** A mask that represents the type of read or write method available (e.g. interleaved, mmap). */
    PCM_PARAM_ACCESS,
    /** A mask that represents the @ref pcm_format available (e.g. @ref PCM_FORMAT_S32_LE) */
    PCM_PARAM_FORMAT,
    /** A mask that represents the subformat available */
    PCM_PARAM_SUBFORMAT,
    /** An interval representing the range of sample bits available (e.g. 8 to 32) */
    PCM_PARAM_SAMPLE_BITS,
    /** An interval representing the range of frame bits available (e.g. 8 to 64) */
    PCM_PARAM_FRAME_BITS,
    /** An interval representing the range of channels available (e.g. 1 to 2) */
    PCM_PARAM_CHANNELS,
    /** An interval representing the range of rates available (e.g. 44100 to 192000) */
    PCM_PARAM_RATE,
    PCM_PARAM_PERIOD_TIME,
    /** The number of frames in a period */
    PCM_PARAM_PERIOD_SIZE,
    /** The number of bytes in a period */
    PCM_PARAM_PERIOD_BYTES,
    /** The number of periods for a PCM */
    PCM_PARAM_PERIODS,
    PCM_PARAM_BUFFER_TIME,
    PCM_PARAM_BUFFER_SIZE,
    PCM_PARAM_BUFFER_BYTES,
    PCM_PARAM_TICK_TIME,
} AIO_PCM_PARAM_S; /* enum pcm_param */

AIO_PCM_PARAM_S *PcmParamsGet(EI_U32 Card, EI_U32 Device, EI_U32 Flags);

EI_VOID PcmParamsFree(AIO_PCM_PARAM_S *PcmParams);

const AIO_PCM_MASK_S *PcmParamsGetMask(const AIO_PCM_PARAM_S *PcmParams, AIO_PCM_PARAM_S Param);

EI_U32 PcmParamsGetMin(const AIO_PCM_PARAM_S *PcmParams, AIO_PCM_PARAM_S Param);

EI_U32 PcmParamsGetMax(const AIO_PCM_PARAM_S *PcmParams, AIO_PCM_PARAM_S Param);

AIO_PCM_FORMAT_S ToPcmFmt(AUDIO_BIT_WIDTH_E bitwidt);

AUDIO_BIT_WIDTH_E ToEiPcmFmt(EI_U32 bitwidt);

AIO_PCM_S *PcmOpen(EI_U32 Card,
    EI_U32 Device,
    EI_U32 Flags,
    const AIO_PCM_CONFIG_S *Config);

AIO_PCM_S *PcmOpenByName(const EI_S8 *Name,
    EI_U32 Flags,
    const AIO_PCM_CONFIG_S *Config);

EI_S32 PcmClose(AIO_PCM_S *Pcm);

EI_S32 PcmIsReady(const AIO_PCM_S *Pcm);

EI_U32 PcmGetChannels(const AIO_PCM_S *Pcm);

const AIO_PCM_CONFIG_S *PcmGetConfig(const AIO_PCM_S *Pcm);

EI_U32 PcmGetRate(const AIO_PCM_S *Pcm);

AIO_PCM_FORMAT_S PcmGetFormat(const AIO_PCM_S *Pcm);

EI_S32 PcmGetFileDescriptor(const AIO_PCM_S *Pcm);

const EI_S8 *PcmGetError(const AIO_PCM_S *Pcm);

EI_S32 PcmSetConfig(AIO_PCM_S *Pcm, const AIO_PCM_CONFIG_S *Config);

EI_U32 PcmFormatToBits(AIO_PCM_FORMAT_S Format);

EI_U32 PcmGetBufferSize(const AIO_PCM_S *Pcm);

EI_U32 PcmGetPeriodSize(const AIO_PCM_S *pstPcm);

EI_U32 PcmFramesToBytes(const AIO_PCM_S *Pcm, EI_U32 Frames);

EI_U32 PcmBytesToFrames(const AIO_PCM_S *Pcm, EI_U32 Bytes);

EI_S32 PcmGetHtimeStamp(AIO_PCM_S *Pcm, EI_U32 *Avail, struct timespec *Tstamp);

EI_U32 PcmGetSubdevice(const AIO_PCM_S *Pcm);

EI_S32 PcmWritei(AIO_PCM_S *Pcm, const EI_VOID *Data, EI_U32 Frame_count);

EI_S32 PcmReadi(AIO_PCM_S *Pcm, EI_VOID *Data, EI_U32 Frame_count);

#ifdef __GNUC__

EI_S32 PcmWrite(AIO_PCM_S *Pcm, const EI_VOID *Data, EI_U32 Count) __attribute((deprecated));

EI_S32 PcmRead(AIO_PCM_S *Pcm, EI_VOID *Data, EI_U32 Count) __attribute((deprecated));

#else

EI_S32 PcmWrite(AIO_PCM_S *Pcm, const EI_VOID *Data, EI_U32 Count);

EI_S32 PcmRead(AIO_PCM_S *Pcm, EI_VOID *Data, EI_U32 Count);

#endif

EI_S32 PcmMmapWrite(AIO_PCM_S *Pcm, const EI_VOID *Data, EI_U32 Count);

EI_S32 PcmMmapRead(AIO_PCM_S *Pcm, EI_VOID *Data, EI_U32 Count);

EI_S32 PcmMmapBegin(AIO_PCM_S *Pcm, EI_VOID **Areas, EI_U32 *Offset, EI_U32 *Frames);

EI_S32 PcmMmapCommit(AIO_PCM_S *Pcm, EI_U32 Offset, EI_U32 Frames);

EI_S32 PcmLink(AIO_PCM_S *Pcm1, AIO_PCM_S *Pcm2);

EI_S32 PcmUnlink(AIO_PCM_S *Pcm);

EI_S32 PcmPrepare(AIO_PCM_S *Pcm);

EI_S32 PcmStart(AIO_PCM_S *Pcm);

EI_S32 PcmStop(AIO_PCM_S *Pcm);

EI_S32 PcmClose(AIO_PCM_S *pstPcm);

EI_S32 PcmWait(AIO_PCM_S *Pcm, EI_S32 Timeout);

EI_SL PcmGetDelay(AIO_PCM_S *Pcm);

#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif

