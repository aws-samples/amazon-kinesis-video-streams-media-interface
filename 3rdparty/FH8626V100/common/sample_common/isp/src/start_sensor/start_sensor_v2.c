#include <isp/isp_sensor_if.h>
#include "sample_common.h"

#if (!defined(CONFIG_ARCH_YG) && !defined(CONFIG_ARCH_FH8626V100))

#include "FHAdv_Isp_mpi_v2.h"

extern struct isp_sensor_if *Sensor_Create(void);
struct isp_sensor_if *start_sensor(void)
{
    int ret;
    struct isp_sensor_if *sensor;
    sensor = Sensor_Create();
    if (sensor)
    {
        ret = FHAdv_Isp_SensorInit(sensor);
        if (ret)
        {
            printf("Error: FHAdv_Isp_SensorInit failed, ret = %d\n", ret);
        }
    }

    return sensor;
}
#endif /* (!defined(CONFIG_ARCH_YG) && !defined(CONFIG_ARCH_FH8626V100)) */