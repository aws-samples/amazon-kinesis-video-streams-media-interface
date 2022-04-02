#if defined(CONFIG_ARCH_FH8626V100) || defined(CONFIG_ARCH_YG)
#include "sample_common.h"
#include <isp/isp_sensor_if.h>
#include <FHAdv_Isp_mpi.h>

#ifdef FH_USING_MULTI_SENSOR
#include "multi_sensor.c"
#else
#include "single_sensor.c"
#endif
#endif /* (defined(CONFIG_ARCH_YG) || defined(CONFIG_ARCH_FH8626V100)) */
