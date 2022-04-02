#ifndef __common_h__
#define __common_h__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pthread.h"
#include "time.h"
#include <signal.h>
#include <fcntl.h>
#include <stddef.h>
#include <config.h>
#include "types/type_def.h"
#include "types/bufCtrl.h"

#include <libdbi_over_tcp/include/dbi_over_tcp.h>
#include <librtsp/include/librtsp.h>
#include <libdmc/include/libdmc.h>
#include <libdmc/include/libdmc_pes.h>
#include <libdmc/include/libdmc_rtsp.h>
#include <libdmc/include/libdmc_record_raw.h>
#include <libdmc/include/libdmc_http_mjpeg.h>

#include <libpes/include/libpes.h>
#include <librtsp/include/librtsp.h>
#include <libscaler/include/fh_scaler.h>
#include <librect_merge_by_gaus/include/rect_merge_by_gaus.h>
#include <libmisc/include/libmisc.h>

#include <isp/include/sample_common_isp.h>
#include <dsp/include/sample_common_dsp.h>
#include <overlay/sample_overlay.h>
#include <motion_and_cover/sample_md_cd.h>
#include <bgm/include/sample_bgm.h>
#include <venc/sample_venc.h>
#include <human_detect/include/human_detect.h>
#include <human_detect/include/sample_human_detect.h>
#include <human_detect_nn/human_detect_nn.h>
#include <fh_nn_detect/sample_nna_detect.h>
#include <audio/sample_audio.h>
#include <isp_strategy/sample_isp.h>
#include <af_demo/sample_af.h>
#include <ivs_demo/sample_ivs.h>

#include "FHAdv_OSD_mpi.h"

#endif /*__common_h__*/

