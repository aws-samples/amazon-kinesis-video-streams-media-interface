/*
 *------------------------------------------------------------------------------
 * @File      :    ei_defines.h
 * @Date      :    2021-3-16
 * @Author    :    lomboswer <lomboswer@lombotech.com>
 * @Brief     :    Common file for MDP(Media Development Platform).
 *
 * Copyright (C) 2020-2021, LomboTech Co.Ltd. All rights reserved.
 *------------------------------------------------------------------------------
 */

#ifndef __EI_DEFINES_H__
#define __EI_DEFINES_H__

#ifdef __EOS__
#include <rtthread.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MDP_VER_PRIX "_MDP_V"


#define ALIGN_NUM 4

#define DEFAULT_ALIGN                32
#define MAX_ALIGN                    1024

#define MAX_POOL_NAME_LEN             32
#define VB_MAX_POOLS                 512

#define VISS_MAX_DEV_NUM                 3
#define VISS_MAX_PHY_CHN_NUM             8
#define VISS_MAX_EXT_CHN_NUM             4
#define VISS_MAX_VC_CHN_NUM              2
#define VISS_MAX_VC_BIND_PHY_NUM             4
#define VISS_MAX_CHN_NUM                 (VISS_MAX_PHY_CHN_NUM + VISS_MAX_EXT_CHN_NUM + VISS_MAX_VC_CHN_NUM)

/* For AUDIO */
#define AI_MAX_DEV_NUM                 1 /* max dev num */
#define AI_MAX_CHN_NUM                 8 /* max chn num */
#define AO_MIN_DEV_NUM                 0
#define AO_MAX_DEV_NUM                 1
#define AIO_MAX_NUM                    2
#define AENC_MAX_CHN_NUM               32
#define ADEC_MAX_CHN_NUM               32
#define AO_MAX_CHN_NUM                 2
#define AO_SYSCHN_CHNID                (AO_MAX_CHN_NUM - 1)

#define AO_MAX_VOLUME_DB                 8
#define AO_MIN_VOLUME_DB                 -76

#define AI_MAX_VOLUME_DB                 20
#define AI_MIN_VOLUME_DB                 -64

#define AIO_MAX_CHN_NUM                ((AO_MAX_CHN_NUM > AI_MAX_CHN_NUM) ? AO_MAX_CHN_NUM : AI_MAX_CHN_NUM)

/* For Region */
#define RGN_MIN_WIDTH               2
#define RGN_MIN_HEIGHT              2

#define RGN_COVEREX_MIN_X           0
#define RGN_COVEREX_MIN_Y           0
#define RGN_COVEREX_MAX_X           4096
#define RGN_COVEREX_MAX_Y           4096
#define RGN_COVEREX_MIN_WIDTH       2
#define RGN_COVEREX_MIN_HEIGHT      2
#define RGN_COVEREX_MAX_WIDTH       4096
#define RGN_COVEREX_MAX_HEIGHT      4096

#define RGN_OVERLAYEX_MIN_X         0
#define RGN_OVERLAYEX_MIN_Y         0
#define RGN_OVERLAYEX_MAX_X         4096
#define RGN_OVERLAYEX_MAX_Y         4096
#define RGN_OVERLAYEX_MIN_WIDTH     2
#define RGN_OVERLAYEX_MIN_HEIGHT    2
#define RGN_OVERLAYEX_MAX_WIDTH     4096
#define RGN_OVERLAYEX_MAX_HEIGHT    4096

#define RGN_MOSAICEX_MIN_X          0
#define RGN_MOSAICEX_MIN_Y          0
#define RGN_MOSAICEX_MAX_X          4096
#define RGN_MOSAICEX_MAX_Y          4096
#define RGN_MOSAICEX_MIN_WIDTH      32
#define RGN_MOSAICEX_MIN_HEIGHT     32
#define RGN_MOSAICEX_MAX_WIDTH      4096
#define RGN_MOSAICEX_MAX_HEIGHT     4096

#define RGN_RECTANGLEEX_MIN_X          0
#define RGN_RECTANGLEEX_MIN_Y          0
#define RGN_RECTANGLEEX_MAX_X          4096
#define RGN_RECTANGLEEX_MAX_Y          4096
#define RGN_RECTANGLEEX_MIN_WIDTH      2
#define RGN_RECTANGLEEX_MIN_HEIGHT     2
#define RGN_RECTANGLEEX_MAX_WIDTH      4096
#define RGN_RECTANGLEEX_MAX_HEIGHT     4096

#define RGN_ALIGN                   2
#define RGN_HANDLE_MAX              128
#define RGN_MAX_BUF_NUM             6

#define COVEREX_MAX_NUM_IPPU        16
#define OVERLAYEX_MAX_NUM_IPPU      16
#define MOSAICEX_MAX_NUM_IPPU       16
#define RECTANGLEEX_MAX_NUM_IPPU    128
#define COVEREX_MAX_NUM_VO          8
#define OVERLAYEX_MAX_NUM_VO        8

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __EI_DEFINES_H__ */

