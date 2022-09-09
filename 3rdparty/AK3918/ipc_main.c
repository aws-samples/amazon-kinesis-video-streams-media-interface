#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/statfs.h>
#include <signal.h>

#include "ak_platform_app.h"
#include "fun_stream_mgr.h"
//#include "aas.h"
#include "akae_stdlib.h"
#include "glist.h"
#ifdef CLOUD_LINK_DEF
	#include "aa_yidong.h"
#endif

#include "fun_osd.h"
#include "fun_msg_mgr.h"
#include "fun_record.h"
#include "fun_replay.h"
#include "console.h"
#include "fun_nightvision.h"


#include "fun.h"
#include "fun_led.h"
#include "ak_log.h"
#include "aas_common.h"
#include "aas_thread.h"

#include "fun_ptz.h"
#include "fun_isp_config.h"
#include "fun_mrd.h"
#include "fun_tfcard.h"
#include "fun_ota.h"
#include "fun.h"


void AK_Set_Log_Level(char log_level_name)
{
	int log_level = LOG_LEVEL_ERROR; //LOG_LEVEL_ERROR 默认打错误日志
	switch(log_level_name)
	{
		case 'd':
			log_level = 6/*LOG_LEVEL_DEBUG*/;break;
		case 'i':
			log_level = 5/*LOG_LEVEL_INFO*/;break;
		case 'w':
			log_level = 2/*LOG_LEVEL_WARNING*/;break;
		case 'e':
			log_level = 1/*LOG_LEVEL_ERROR*/;break;
		case 'f':
			log_level = 0/*LOG_LEVEL_RESERVED*/;break;
	}

	//int module_level ;
	//for(module_level = MODULE_ID_MEMORY; module_level <= MODULE_ID_ALL; ++module_level)
	//{
	ak_print_set_level(MODULE_ID_APP, log_level);
	//}

	
	
	char strlevel[32]={0};
	sprintf(strlevel,"%c",log_level_name);
	ak_SetIniString("log", "level", strlevel , "others");
}


int ipc_platform_sdk_init()
{
	/* start the application */
	sdk_run_config config;
	memset(&config, 0, sizeof(config));
	config.mem_trace_flag = SDK_RUN_NORMAL;
	config.isp_tool_server_flag = 0;
	
	return ak_sdk_init( &config );

}

int ota_cmd(const COMMAND_PKT *pParams, char *pcAck)
{
#ifdef CFG_OTA_DEF
    printf("param : %d \n", pParams->n);
	int iCnt = pParams->n;
	int i = 0;
	for(i = 0;i < iCnt ;i++ )
	{
		printf("param%d :%s \n",i,pParams->p[i]);;
	}
    ota_info_t info = {0};
    int maj, vermin, verrev;
    int ret = AK_false;
    if(atoi(pParams->p[0]) == 1)
    {
        if (pParams->n >= 3) {
			hal_ota_stop ();
            snprintf( info.download_url , sizeof(info.download_url), "%s", pParams->p[1]);
            if (sscanf(pParams->p[2], "v%d.%d.%d", &maj, &vermin, &verrev) == 3) {
                info.fw_version = maj * 10000 + vermin * 100 + verrev;
				printf("ooooooooooo ota url=%s", info.download_url);
                printf("maj=%d, min=%d, rev=%d \n", maj, vermin, verrev);
                printf("info.fw_version %d \n", info.fw_version);
                ret = hal_ota_start (info);
            }
        }

    }else if (atoi(pParams->p[0]) == 2){
        hal_ota_stop ();
    }
    pcAck = ret? "success": "failed";
#endif
    return 0;
}
int wdt_cmd(const COMMAND_PKT *pParams, char *pcAck)
{
    printf("param : %d \n", pParams->n);
    if(atoi(pParams->p[0]) == 1)
    {
        if (pParams->n >= 2) {
            printf("timeout : %d \n", atoi(pParams->p[1]));
            hal_watchdog_init(atoi(pParams->p[1]));
        }
    }else if (atoi(pParams->p[0]) == 2){
        hal_watchdog_deinit ();
    }else if (atoi(pParams->p[0]) == 3){
        hal_watchdog_reboot_device ();
    }

    return 0;

}
int ipc_mem_get_info(const COMMAND_PKT *pParams, char *pcAck)
{

	char str[128] = {0};

	aas_mem_get_info(str,sizeof(str));
	strcpy(pcAck,"get info ok");
	printf("mem info:%s\n",str);
    return 0;

}
int ipc_thread_get_info(const COMMAND_PKT *pParams, char *pcAck)
{

	char str[1024*2] = {0};

	aas_thread_get_info(str,sizeof(str));
	strcpy(pcAck,"get info ok");
	printf("thread info:\n%s\n",str);
    return 0;

}


int log_cmd(const COMMAND_PKT *pParams, char *pcAck)
{
    printf("param : %d \n", pParams->n);
    if( pParams->n < 1)
		return 0;


    char log_level = pParams->p[0][0];
	AK_Set_Log_Level(log_level);
	printf("AK_Set_Log_Level : [%c] ok\n", log_level);

    return 0;
}


static void ipc_RecordInit()
{
	int nRet = Record_init();
	nRet |= Record_Replay_Init();

	return  ;

}
static void ipc_RecordDeinit()
{
	int nRet = Record_Deinit();
	nRet |= Record_Replay_Deinit();

	return  ;
}

static int ptz_init (void)
{
    int ret = -1;
	int verse = ak_isp_get_MirrorValue();
	platform_ptz_inverse ( verse == 0 ? AK_true : AK_false);

	T_PLATFORM_PTZ_PARAM Param;

	akae_memzero (&Param, sizeof (Param));
	Param.Pan.range_degree   = 	ak_GetIniInt("pan", "range_degree", 355,NULL);
	Param.Pan.default_degree = 	ak_GetIniInt("pan", "default_degree", 175,NULL);
	Param.Pan.max_clock_hz   = 	ak_GetIniInt("pan", "max_clock_hz", 1000,NULL);
	Param.Pan.min_clock_hz   = 	ak_GetIniInt("pan", "min_clock_hz", 0,NULL);
	Param.Pan.speed 	     = 	ak_GetIniInt("pan", "speed", 4,NULL);

	Param.Tilt.range_degree   = ak_GetIniInt("tile", "range_degree", 65,NULL);
	Param.Tilt.default_degree = ak_GetIniInt("tile", "default_degree", 15,NULL);
	Param.Tilt.max_clock_hz   = ak_GetIniInt("tile", "max_clock_hz", 1000,NULL);
	Param.Tilt.min_clock_hz   = ak_GetIniInt("tile", "min_clock_hz", 0,NULL);
	Param.Tilt.speed          = ak_GetIniInt("tile", "speed", 1,NULL);
	

	Param.step_timeout = 300; ///< 根据xx方案测试出来的一个不卡顿的步进超时值。
    	//if(0 != access("/etc/config/ptzpos.txt", F_OK)){
	//snprintf (Param.savepos_filepath, sizeof (Param.savepos_filepath), "/etc/config/ptzpos.txt");
	//}
	Param.powersave = AK_true;

	/// 初始化云台服务。
	if (platform_ptz_init2 (&Param)) {
        platform_ptz_calibrate (AK_false);
        ret = 0;
    }

    return ret;
}

static int ipc_init()
{
	AK_CHECK0_RET_ERROR(ipc_platform_sdk_init());

	ak_print_set_level(MODULE_ID_ALL, LOG_LEVEL_ERROR);
	char cLogLevel[1];
	ak_GetIniString("log", "level", cLogLevel, 1, "d","other");
	AK_Set_Log_Level(cLogLevel[0]);

	AK_CHECK0_RET_ERROR(Console_Init("/tmp"));

	//AK_CHECK0_RET_ERROR(nightvision_init());
	AK_CHECK0_RET_ERROR(AA_LS_Init());

	char font_file[256] = {0};
	sprintf(font_file,"%s/ak_font_16.bin", ak_GetResPath());
	char config_file[256] = {0};
	snprintf(config_file, sizeof(config_file), "%s/osd_cfg.json", ak_GetConfigPath());
	AK_CHECK0_RET_ERROR(ak_app_osd_init(font_file, config_file, ak_GetResPath()));

#ifdef CFG_ALARM_DEF
	AK_CHECK0_RET_ERROR(ak_msg_init());
#endif



	//AK_CHECK0_RET_ERROR(HAL_InitTfCard(MOUNT_DIR,ipc_RecordInit,NULL));

	ptz_init();
	//hal_ptz_preset_list_init();
	hal_mrd_init();


#if defined CLOUD_LINK_DEF
	//AK_CHECK0_RET_ERROR(aa_yidong_init());
#endif
	Console_CMD_Register("ak-ota-cmd", ota_cmd, "ak-ota-cmd-help");
	hal_humdetect_init();
	printf("11111111111111111111111    hal_humdetect_init ture!!!!!!!!!!!!!!\n");

	return 0;
}



static int ipc_uninit()
{
    AA_LS_UnInit();
	nightvision_deinit();
	ipc_RecordDeinit();
	Console_Deinit();


	//ak_config_release_ini();
//    AK_Stream_Storage_Exit();
   // osd_ex_exit();
   return 0;
}

static int ParserCmd(char inputCmd[], char* argv[])
{
	//printf("input: %s len=%d %d\n", inputCmd, strlen(inputCmd), inputCmd[strlen(inputCmd)-1]);

	int argc = 0;
	int i=0;
	int str_end = 1;
	while(inputCmd[i] != 0)
	{
		if(inputCmd[i] == ' ' || inputCmd[i] == '\r' || inputCmd[i] == '\n')
		{
			inputCmd[i] = '\0';
			str_end = 1;
		}
		else
		{
			if(str_end)
			{
				argv[argc] = inputCmd + i;
				argc ++;
				str_end = 0;
			}
		}

		i++;
	}

/*	printf("%d: {", argc);
	for(i=0;i<argc;i++)
		printf("%s,", argv[i]);
	printf("} \n");*/

	return argc;

}


int ipc_main()
{
	printf("\n\n\n/******************************************/\n");
	printf(" %s %s\n", __DATE__, __TIME__);
	printf("/******************************************/\n\n\n");
	
	//s1a.time.edu.cn 202.112.10.60
	//time2.cloud.tencent.com:111.230.189.174
	int iFailed = 0;
	while(1)
	{
	    int iRet = NTP_Set("111.230.189.174");
	    if(iRet == 0)
	    {
	        printf("oooooooooo ntp ok!\n");
	        break;
	    }
	    else
	    {
	        iFailed++;
	        printf("oooooooooooo ntp failed count =%d\n",iFailed);
	        sleep(10);
	    }
	}
	aas_mem_init();
	aas_thread_init();

	signal (SIGPIPE, SIG_IGN);

	
	
		ak_SetMainConfigPath(NULL);
	

	ak_led_ctrl(AK_STAT_LED_METH_OFF,AK_STAT_LED_METH_ON);



	VersionInfo_t verinfo;
	int iRet = 0;
    iRet = hal_version_get(&verinfo);
    if (iRet == 0) {
        printf("Firmware Version %d.%d.%d\n", verinfo.fw_version / 10000, (verinfo.fw_version / 100) % 100 , verinfo.fw_version % 100);
    } else {
        printf("Get firmware Version Error\n");
    }
    iRet = ipc_init();
	if(iRet != 0)
	{
		printf("\n\naaaaaaaaaaaaaaaaaaaa init failed !\n");
		///ak_sleep_ms(1000 * 60 * 5);
		return -1;
	}
	printf("\n\naaaaaaaaaaaaaaaaaaaa init ok !\n");
	if((0 != access("/etc/config/debug", F_OK)) && (0 != access("/etc/config/debug.sh", F_OK)))
		hal_watchdog_init(3);/// 打开watchdog

    Console_CMD_Register("ak-watdog-cmd", wdt_cmd, "watdog-help");
	Console_CMD_Register("ak-cmd-log", log_cmd, "log-help");
	Console_CMD_Register("ak-mem-info", ipc_mem_get_info, "ak-mem-info-help");
	Console_CMD_Register("ak-thread-info", ipc_thread_get_info, "ak-thread-info-help");

	//Console_Loop();

	/*iRet = ipc_uninit();
	if((0 != access("/etc/config/debug", F_OK)) && (0 != access("/etc/config/debug.sh", F_OK)))
		hal_watchdog_deinit();

	ak_sdk_exit();
	aas_mem_deinit();
	*/
	return 0;
}
