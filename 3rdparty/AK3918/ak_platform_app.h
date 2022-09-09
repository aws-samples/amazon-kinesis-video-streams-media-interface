#ifndef _AK_PLATFORM_APP_H
#define _AK_PLATFORM_APP_H

#include "ak_common.h"
#include "ak_common_audio.h"
#include "ak_common_graphics.h"
#include "ak_common_video.h"
#include "ak_adec.h"
#include "ak_aenc.h"
#include "ak_ai.h"
#include "ak_ao.h"
#include "ak_ats.h"
#include "ak_dbg.h"
#include "ak_demux.h"
#include "ak_its.h"
#include "ak_log.h"

//#include "ak_md.h"
#include "ak_mem.h"
#include "ak_mem_ex.h"
#include "ak_mux.h"

#include "ak_osd.h"
#include "ak_osd_ex.h"
#include "ak_thread.h"
#include "ak_venc.h"

#include "ak_vi.h"
#include "ak_vpss.h"
#include "ak_vqe.h"
#include "fun_cfg.h"


#define log_error_app(fmt, arg...) \
	ak_print(MODULE_ID_APP, LOG_LEVEL_ERROR, "[%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
#define log_warn_app(fmt, arg...) \
	ak_print(MODULE_ID_APP, LOG_LEVEL_WARNING, "[%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
#define log_notice_app(fmt, arg...) \
	ak_print(MODULE_ID_APP, LOG_LEVEL_NOTICE, "[%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
#define log_info_app(fmt, arg...) \
	ak_print(MODULE_ID_APP, LOG_LEVEL_INFO, "[%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
#define log_debug_app(fmt, arg...) \
	ak_print(MODULE_ID_APP, LOG_LEVEL_DEBUG,  "[%s:%d] "fmt"\n", __func__, __LINE__, ##arg)

//#define PLATFORM_AV100
#define AK_HERE ak_print_normal_ex(MODULE_ID_APP, "i am here \n");

#define AK_CHECK_RET { if(iRet != 0){ ak_print_error_ex(MODULE_ID_APP, "ret=%x must stop run !\n", iRet); \
	return (iRet);} }

#endif

