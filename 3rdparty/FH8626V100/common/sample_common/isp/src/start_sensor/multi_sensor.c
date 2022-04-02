#define DECLARE_SENSOR_CREATE(n)     extern struct isp_sensor_if *Sensor_Create_##n(void);
#define DECLARE_GET_BAYER_FORMAT(n)  extern unsigned int *GetMirrorFlipBayerFormat_##n(void);
#define DECLARE_GET_AWB_GAIN(n)      extern unsigned int *GetSensorAwbGain_##n(int idx);
#define DECLARE_GET_LTM_CURVE(n)     extern unsigned int *GetSensorLtmCurve_##n(int idx);
#define DECLARE_GET_DEFAULT_PARAM(n) extern unsigned int *GetDefaultParam_##n(void);

#define ADD_SENSOR_INFO(suffix)                                       \
    {                                                                 \
        .sensor_name = ""#suffix,                                     \
        .funcs = {                                                    \
            .sensor_create = (void *(*)(void))(Sensor_Create_##suffix), \
            .get_bayer_format = GetMirrorFlipBayerFormat_##suffix,    \
            .get_awb_gain = GetSensorAwbGain_##suffix,                \
            .get_ltm_curve = GetSensorLtmCurve_##suffix,              \
            .get_default_param = GetDefaultParam_##suffix,            \
        },                                                            \
    }

#define DECLARE_SENSOR(SENSOR) \
    DECLARE_SENSOR_CREATE(SENSOR) \
    DECLARE_GET_BAYER_FORMAT(SENSOR) \
    DECLARE_GET_AWB_GAIN(SENSOR) \
    DECLARE_GET_LTM_CURVE(SENSOR) \
    DECLARE_GET_DEFAULT_PARAM(SENSOR)

#ifdef RT_USING_JXF23_MIPI
    DECLARE_SENSOR(JXF23_MIPI)
#endif

#ifdef RT_USING_JXF23_DVP
    DECLARE_SENSOR(JXF23_DVP)
#endif

#ifdef RT_USING_JXF22_MIPI
    DECLARE_SENSOR(JXF22_MIPI)
#endif

#ifdef RT_USING_OVOS05_MIPI
    DECLARE_SENSOR(OVOS05_MIPI)
#endif

#ifdef RT_USING_GC4623_MIPI
    DECLARE_SENSOR(GC4623_MIPI)
#endif

#ifdef RT_USING_GC4633_MIPI
    DECLARE_SENSOR(GC4633_MIPI)
#endif

#ifdef RT_USING_GC4622_MIPI
    DECLARE_SENSOR(GC4622_MIPI)
#endif

#ifdef RT_USING_OV2732_MIPI
    DECLARE_SENSOR(OV2732_MIPI)
#endif

#ifdef RT_USING_PS5270_MIPI
    DECLARE_SENSOR(PS5270_MIPI)
#endif

static FHADV_ISP_SENSOR_INFO_t g_multi_sensor_infos[] = {
#if defined(CONFIG_ARCH_FH8626V100)
    #ifdef RT_USING_GC4623_MIPI
    ADD_SENSOR_INFO(GC4623_MIPI),
    #endif
    #ifdef RT_USING_GC4633_MIPI
    ADD_SENSOR_INFO(GC4633_MIPI),
    #endif
    #ifdef RT_USING_JXF22_MIPI
    ADD_SENSOR_INFO(JXF22_MIPI),
    #endif
    #ifdef RT_USING_JXF23_MIPI
    ADD_SENSOR_INFO(JXF23_MIPI),
    #endif
    #ifdef RT_USING_JXF23_DVP
    ADD_SENSOR_INFO(JXF23_DVP),
    #endif
    #ifdef RT_USING_MN34425_MIPI
    ADD_SENSOR_INFO(MN34425_MIPI),
    #endif
    #ifdef RT_USING_OV2732_MIPI
    ADD_SENSOR_INFO(OV2732_MIPI),
    #endif
    #ifdef RT_USING_OVOS05_MIPI
    ADD_SENSOR_INFO(OVOS05_MIPI),
    #endif
    #ifdef RT_USING_PS5270_MIPI
    ADD_SENSOR_INFO(PS5270_MIPI),
    #endif
#elif defined(CONFIG_ARCH_YG)
    #ifdef RT_USING_OVOS05_MIPI
    ADD_SENSOR_INFO(OVOS05_MIPI),
    #endif
    #ifdef RT_USING_JXF22_MIPI
    ADD_SENSOR_INFO(JXF22_MIPI),
    #endif
    #ifdef RT_USING_JXF23_MIPI
    ADD_SENSOR_INFO(JXF23_MIPI),
    #endif
    #ifdef RT_USING_PS5270_MIPI
    ADD_SENSOR_INFO(PS5270_MIPI),
    #endif
    #ifdef RT_USING_GC4623_MIPI
    ADD_SENSOR_INFO(GC4623_MIPI),
    #endif
    #ifdef RT_USING_GC4633_MIPI
    ADD_SENSOR_INFO(GC4633_MIPI),
    #endif
    #ifdef RT_USING_MN34425_MIPI
    ADD_SENSOR_INFO(MN34425_MIPI),
    #endif
    #ifdef RT_USING_OV2732_MIPI
    ADD_SENSOR_INFO(OV2732_MIPI),
    #endif
#endif
};

struct isp_sensor_if *start_sensor(void)
{
    int ret;
    FHADV_ISP_SENSOR_INFO_t probed_sensor;
    FHADV_ISP_SENSOR_PROBE_INFO_t sensor_info;

    sensor_info.sensor_infos = g_multi_sensor_infos;
    sensor_info.sensor_num   = sizeof(g_multi_sensor_infos)/sizeof(g_multi_sensor_infos[0]);
    ret = FHAdv_Isp_SensorInit(&sensor_info, &probed_sensor);
    if (ret)
    {
        printf("Error: FHAdv_Isp_SensorInit failed, ret = %d\n", ret);
    }

    return probed_sensor.sensor_handle;
}