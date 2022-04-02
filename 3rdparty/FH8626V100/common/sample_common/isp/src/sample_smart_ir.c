#include "isp/isp_common.h"
#include "isp/isp_api.h"
#include "FHAdv_SmartIR_mpi.h"
#include "sample_common.h"
#include "_sensor_.h"
#include "sample_smart_ir.h"

static FH_CHAR *sensor_param_day;
static FH_CHAR *sensor_param_night;
static FH_SINT32 g_smartir_inited = 0;

FH_SINT32 sample_SmartIR_init(FH_CHAR *sensor_name)
{
    FH_SINT32 ret = -1;
    FH_SINT32 hex_file_len;

    if (g_smartir_inited)
    {
        printf("SmartIR: already inited!\n");
    	return 0;
    }

    sensor_param_day   = get_isp_sensor_param((char *)sensor_name, SAMPLE_SENSOR_FLAG_NORMAL, &hex_file_len);
    if (!sensor_param_day)
    {
        printf("Error: Cann't load sensor hex Day file!\n");
        goto Exit;
    }

    sensor_param_night = get_isp_sensor_param((char *)sensor_name, SAMPLE_SENSOR_FLAG_NIGHT, &hex_file_len);
    if (!sensor_param_night)
    {
        printf("Error: Cann't load sensor hex Night file!\n");
        goto Exit;
    }

    ret = FHAdv_SmartIR_Init();
    if ( ret != 0 )
    {
        printf("Error: FHAdv_SmartIR_Init failed with %d\n", ret);
        goto Exit;
    }

    ret = FHAdv_SmartIR_SetAttr(FHADV_IR_BAYER);
    if ( ret != 0 )
    {
        printf("Error: FHAdv_SmartIR_SetAttr failed with %d\n", ret);
        goto Exit;
    }

#ifdef CONFIG_ARCH_FH8626V100
    /*ret = FHAdv_SmartIR_SetSensitivity(SMARTIR_LEVEL_MID);
    if ( ret != 0 )
    {
        printf("Error: FHAdv_SmartIR_SetSensitivity failed with %d\n", ret);
        goto Exit;
    }*/
#else
    FH_SINT32 offset = 0; /*It's the default value*/
    ret = FHAdv_SmartIR_SetCustom_offset(offset, 0);
    if ( ret != 0 )
    {
        printf("Error: FHAdv_SmartIR_SetCustom_offset failed with %d\n", ret);
        goto Exit;
    }
#endif

    g_smartir_inited = 1;

    return 0;

Exit:
	if (sensor_param_day)
    {
    	free_isp_sensor_param(sensor_param_day);
    	sensor_param_day = NULL;
    }

 	if (sensor_param_night)
    {
    	free_isp_sensor_param(sensor_param_night);
    	sensor_param_night = NULL;
    }

    return ret;
}

FH_SINT32 sample_SmartIR_deinit(FH_VOID)
{
 	if (sensor_param_day)
    {
    	free_isp_sensor_param(sensor_param_day);
    	sensor_param_day = NULL;
    }

 	if (sensor_param_night)
    {
    	free_isp_sensor_param(sensor_param_night);
    	sensor_param_night = NULL;
    }

	g_smartir_inited = 0;

	return 0;
}

FH_VOID sample_SmartIR_Ctrl(FH_VOID)
{
    static FH_SINT32 sirStaPrev = FHADV_IR_DAY; /*the initial value must be FHADV_IR_DAY*/
    FH_SINT32 irStaCurr;
    FH_SINT32 ret;

    if (!g_smartir_inited)
    	return;

    irStaCurr = FHAdv_SmartIR_GetDayNightStatus(sirStaPrev);
    if (irStaCurr == sirStaPrev)
        return;

    if (irStaCurr == FHADV_IR_DAY)
    {
        printf("[SmartIR]: Switch to Day\n");
        ret = API_ISP_LoadIspParam(sensor_param_day);
        if (ret)
        {
            printf("Error(%d): API_ISP_LoadIspParam!\n", ret);
        }
    }
    else /*night*/
    {
        printf("[SmartIR]: switch to Night\n");
        ret = API_ISP_LoadIspParam(sensor_param_night);
        if (ret)
        {
            printf("Error(%d): API_ISP_LoadIspParam!\n", ret);
        }
    }

    sirStaPrev = irStaCurr;
}

