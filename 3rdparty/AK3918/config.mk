# Cross Tools and libs

CROSS_COMPILE    ?= arm-anycloud-linux-uclibcgnueabi-
export CROSS_COMPILE

GDB_DEBUG = 0

#平台选择
PLATFORM_TYPE = AK3918AV100

#产品型号选择
PRODUCT_MODEL = AK3918AV100_Dome

#产品dtb选择
PRODUCT_DTB = CA100_YTJ_AK3918AV100X_GC4653_V1.0.0.dtb

#功能模块
CFG_AAS = y
CFG_LIVE_STREAM = y
CFG_WIFI = y
CFG_TFCORD = y
CFG_RECORD = y
CFG_PTZ = y
CFG_ALARM = y
CFG_SCAN_QR = y
CFG_INIPARSER = y
CFG_ICONV = y
CFG_QR_INCREASE = y
#ota功能打开需要与curl功能一起打开
CFG_OTA = y
CFG_CURL = y

#link模块
CLOUD_LINK = y
RTSP_LINK = n


#ovd版本号配置,支持 1.19 1.21 1.42
CFG_OVD_VER = 1.24

#iot版本号配置，支持1.5.3.1 1.5.8.0
CFG_IOT_VER = 1.5.8.0
