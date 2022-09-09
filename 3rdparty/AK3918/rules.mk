include config.mk

#系统SDK所在目录,保存编译后的应用程序
DIR_TOP = $(shell pwd)
DIR_APP_TOP = $(shell pwd)/..

#platform define
ifeq ($(PLATFORM_TYPE), AK3918EV33x)
$(info '--------- build ev33x ---------')
SYSTEM_SDK = $(abspath ${DIR_TOP}/../../system/AK3918EV33x)
CFLAGS = -DPLATFORM_EV33X
else ifeq ($(PLATFORM_TYPE), AK3918AV100)
$(info '--------- build av100 ---------')
SYSTEM_SDK = $(abspath ${DIR_TOP}/../../system/AK3918AV100)
CFLAGS = -DPLATFORM_AV100
endif

LDFLAGS = -Wl,--gc-sections

# 安凯平台platform目录 
PLATFORM_INSTALL = ${SYSTEM_SDK}/platform

#编译之后保存的目录: ~/nfs 或 ${SYSTEM_SDK}/rootfs/resource/usr/andlink/
BUILD_DIR    = $(DIR_TOP)/chip/$(PLATFORM_TYPE)/$(PRODUCT_MODEL)/output

$(shell (if [ ! -e $(BUILD_DIR) ];then mkdir -p $(BUILD_DIR);fi))

EXE_INSTALL_PATH = ${BUILD_DIR}
LIB_INSTALL_PATH = ${BUILD_DIR}

export SYSTEM_SDK
export EXE_INSTALL_PATH
export LIB_INSTALL_PATH
export PLATFORM_INSTALL
$(info AAAAAAAAAAAA $(BUILD_DIR) )

$(info $(CURDIR))
ALLOBJS :=
ALLOBJS_CPP :=
ALLHEADDIR := -Ichip/$(PLATFORM_TYPE)/include
ALLLIBS :=
TMPSOLIST :=

#LIBS for shared lib(.so); LIBS-Static-y for static lib(.a);

#common
OBJS-y =
OBJS_CPP-y =
HEADDIR-y =
LIBS-y =
LIBS-Static-y =
include $(DIR_APP_TOP)/third_party/Makefile
ALLOBJS 	+= $(OBJS-y:%=$(DIR_APP_TOP)/third_party/%)
ALLOBJS_CPP += $(OBJS_CPP-y:%=$(DIR_APP_TOP)/third_party/%)
ALLHEADDIR 	+= $(HEADDIR-y:%=-I$(DIR_APP_TOP)/third_party/%)
TMPSOLIST 	+= $(LIBS-y:%=$(DIR_APP_TOP)/third_party/%)
ALLLIBS 	+= $(LIBS-Static-y:%=$(DIR_APP_TOP)/third_party/%)

#fun
OBJS-y =
OBJS_CPP-y =
HEADDIR-y =
LIBS-y =
LIBS-Static-y =
include $(DIR_APP_TOP)/framework/Makefile
ALLOBJS 	+= $(OBJS-y:%=$(DIR_APP_TOP)/framework/%)
ALLOBJS_CPP += $(OBJS_CPP-y:%=$(DIR_APP_TOP)/framework/%)
ALLHEADDIR	+= $(HEADDIR-y:%=-I$(DIR_APP_TOP)/framework/%)
TMPSOLIST 	+= $(LIBS-y:%=$(DIR_APP_TOP)/framework/%)
#$(shell cp $(TMPSOLIST) output)
ALLLIBS 	+= $(LIBS-Static-y:%=$(DIR_APP_TOP)/framework/%)

#$(info '&&&&&&&&&&&&&&&&&&&&&&')
#$(info $(ALLLIBS))
#$(info '&&&&&&&&&&&&&&&&&&&&&&')

#ALLLIBS += $(OUTPUT_SO:output/lib%.so=-l%)

#rtsp
ifeq ($(RTSP_LINK),y)
CFLAGS += -DRTSP_LINK_DEF
endif

#alarm
ifeq ($(CFG_ALARM),y)
CFLAGS += -DCFG_ALARM_DEF
endif

#ota
ifeq ($(CFG_OTA),y)
CFLAGS += -DCFG_OTA_DEF
endif



#cloud module y for yes,n for no;------config.mk
ifeq ($(CLOUD_LINK),y)
CFLAGS += -DCLOUD_LINK_DEF

#cloud
OBJS-y =
OBJS_CPP-y =
HEADDIR-y =
LIBS-y =
LIBS-Static-y =
include $(DIR_APP_TOP)/cloud/Makefile
ALLOBJS 	+= $(OBJS-y:%=$(DIR_APP_TOP)/cloud/%)
ALLOBJS_CPP	+= $(OBJS_CPP-y:%=$(DIR_APP_TOP)/cloud/%)
ALLHEADDIR 	+= $(HEADDIR-y:%=-I$(DIR_APP_TOP)/cloud/%)
TMPSOLIST 	+= $(LIBS-y:%=$(DIR_APP_TOP)/cloud/%)
ALLLIBS		+= $(LIBS-Static-y:%=$(DIR_APP_TOP)/cloud/%)
endif

#clean output
$(shell rm -rf $(BUILD_DIR)/*)
#cp *.so* to output expect for *.a
tmplib_filter=$(filter-out %.a, $(TMPSOLIST))
$(info $(tmplib_filter))
$(shell cp $(tmplib_filter) $(BUILD_DIR)/)
OUTPUT_SO = $(shell ls $(BUILD_DIR)/*.so*)
$(BUILD_DIR) += $(OUTPUT_SO:$(BUILD_DIR)/lib%.so=-l%)
#$(info $(OUTPUT_SO))


TARGET = $(EXE_INSTALL_PATH)/ipc_main

SRCDIR = ./

CC           = $(CROSS_COMPILE)gcc
LD           = $(CROSS_COMPILE)gcc

ALLOBJS 	+= src/ipc_main.o


CFLAGS 		+= -Wall -D_GNU_SOURCE -std=c99 -mlittle-endian  -fno-builtin 						\
			  -nostdlib -mlong-calls -O0 -Werror-implicit-function-declaration

CFLAGS-CPP 	= -fPIC -O3 -lstdc++ -fPIC -fpermissive -lgcc -Wall -DWEBRTC_CLOCK_TYPE_REALTIME	\
			-DWEBRTC_THREAD_RR 	-DWEBRTC_LINUX -DWEBRTC_POSIX -lc -std=c++11

ifeq ($(GDB_DEBUG),1)
CFLAGS 		+= -ggdb3
CFLAGS-CPP 	+= -ggdb3
endif

ALLLIBS += -L$(PLATFORM_INSTALL)/lib -lplat_common -lplat_log -lmpi_mux -lmpi_demux -lakmedia  			\
			-lplat_mem -lplat_thread -ldl -lpthread -lplat_osal -lmpi_venc -lplat_vi   	\
			-lplat_isp_sdk -lplat_vpss -lplat_dbg -lmpi_adec -lplat_vqe -lplat_ao -lakaudiofilter 		\
			-lakaudiocodec -lak_mrd -lplat_ai -lmpi_aenc -lapp_osd_ex -lmpi_osd -lstdc++

ifeq ($(PLATFORM_TYPE), AK3918AV100)
ALLLIBS += -L$(PLATFORM_INSTALL)/lib -lmpi_svp -lapp_video -lakv_cnn  -lapp_mem_ex 
#-lapp_its
#-lplat_timer
else ifeq ($(PLATFORM_TYPE), AK3918EV33x)
ALLLIBS += -L$(PLATFORM_INSTALL)/lib -lakv_encode
endif

ALLLIBS += -L$(BUILD_DIR) $(OUTPUT_SO)
$(info ooooooooo $(OUTPUT))

$(info ooooooooo so $(OUTPUT_SO))

ALLHEADDIR	+= -I./ -I$(PLATFORM_INSTALL)/include

all: $(TARGET)

$(TARGET):$(ALLOBJS) $(ALLOBJS_CPP)
	$(LD) $(LDFLAGS) $(ALLOBJS) $(ALLOBJS_CPP) $(ALLLIBS) -o $(TARGET)
ifeq ($(GDB_DEBUG),0)
	$(CROSS_COMPILE)strip $(TARGET)
endif
	cp -rf $(BUILD_DIR)/* ~/nfs/web/

$(ALLOBJS):%.o:%.c
	$(CC) $(CFLAGS) $(ALLHEADDIR) -c $< -o $@

$(ALLOBJS_CPP):%.o:%.cpp
	$(CC) $(CFLAGS-CPP) $(ALLHEADDIR) -c $< -o $@

clean:
	rm -rf $(ALLOBJS) $(TARGET) $(ALLOBJS_CPP) $(OUTPUT_SO) $(BUILD_DIR)

