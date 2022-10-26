include ../Makefile.param

SMP_SRCS += $(wildcard $(PWD)/*.c)
SMP_INC := -I$(PWD)/

CC:=arm-linux-gnueabihf-gcc


SMP_CMM_SRCS_DIR := ../../common

SMP_CMM_SRCS += $(wildcard $(SMP_CMM_SRCS_DIR)/*.c)
SMP_CMM_SRCS += $(wildcard ../../../../source/SV82x/*.c)

TARGET := sample_audioplayer

TARGET_PATH := $(PWD)

# target source
OBJS  := $(SMP_SRCS:%.c=%.o)
COMM_OBJ  := $(SMP_CMM_SRCS:%.c=%.o)

CFLAGS += $(COMM_INC)

#.PHONY : clean all

all: $(TARGET)

$(TARGET):$(COMM_OBJ) $(OBJS)
	@$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(SENSOR_LIBS) $(MI_LIBS) -Wl,--end-group

clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)
