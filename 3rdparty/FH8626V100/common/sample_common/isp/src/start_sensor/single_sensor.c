extern struct isp_sensor_if *Sensor_Create(void);
extern unsigned int *GetMirrorFlipBayerFormat(void);
extern unsigned int *GetSensorAwbGain(int idx);
extern unsigned int *GetSensorLtmCurve(int idx);
extern unsigned int *GetDefaultParam(void);

static FHADV_ISP_SENSOR_INFO_t g_sensor_infos[] = {
    {
        .sensor_name = "fixed",
        .funcs = {
            .sensor_create = (void *(*)(void))Sensor_Create,
            .get_bayer_format = GetMirrorFlipBayerFormat,
            .get_awb_gain = GetSensorAwbGain,
            .get_ltm_curve = GetSensorLtmCurve,
            .get_default_param = GetDefaultParam,
        },
    }
};

struct isp_sensor_if *start_sensor(void)
{
    int ret;
    FHADV_ISP_SENSOR_INFO_t probed_sensor;
    FHADV_ISP_SENSOR_PROBE_INFO_t sensor_info;

    sensor_info.sensor_infos = g_sensor_infos;
    sensor_info.sensor_num   = 1;
    ret = FHAdv_Isp_SensorInit(&sensor_info, &probed_sensor);
    if (ret)
    {
        printf("Error: FHAdv_Isp_SensorInit failed, ret = %d\n", ret);
    }

    return probed_sensor.sensor_handle;
}