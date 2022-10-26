#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "com/amazonaws/kinesis/video/capturer/VideoCapturer.h"
#include "sample_comm.h"

#define VIDEO_FRAME_BUFFER_SIZE_BYTES      (256 * 1024UL)

typedef struct {
	int video_chn;
	VideoCapturerHandle handle;
	VideoFormat codetype;
	EI_BOOL VencThreadStart;
	pthread_t VencPid;
} get_venc_frame_thread_info;

static VideoCapturerHandle videoCapturerHandle[3] = {};
static get_venc_frame_thread_info venc_frame_info[3] = { {0} };
FILE *flip_out[3] = {NULL};

void *get_venc_frame_thread(void *p)
{
	void *pFrameBuffer = NULL;
	uint64_t timestamp = 0;
	size_t frameSize = 0;
	char aszFileName[128] = {0};
	get_venc_frame_thread_info *pstAttrSampl;

	pstAttrSampl = (get_venc_frame_thread_info *) p;
	pFrameBuffer = malloc(VIDEO_FRAME_BUFFER_SIZE_BYTES);

	switch (pstAttrSampl->codetype) {
		case VID_FMT_H264:
			snprintf(aszFileName, 32, "/data/stream_chn%d.h264", pstAttrSampl->video_chn);
			break;
		case VID_FMT_H265:
			snprintf(aszFileName, 32, "/data/stream_chn%d.h265", pstAttrSampl->video_chn);
			break;
		case VID_FMT_RAW:
			snprintf(aszFileName, 32, "/data/stream2_chn%d.yuv", pstAttrSampl->video_chn);
			break;
		default:
			break;
	}
	flip_out[pstAttrSampl->video_chn] = fopen(aszFileName, "wb");

	while (pstAttrSampl->VencThreadStart) {
		videoCapturerGetFrame(pstAttrSampl->handle, pFrameBuffer, VIDEO_FRAME_BUFFER_SIZE_BYTES, &timestamp, &frameSize);
		//printf("frameSize:%d timestamp:%llu\n", frameSize, timestamp);
		fwrite(pFrameBuffer, 1, frameSize, flip_out[pstAttrSampl->video_chn]);
	}
	fclose(flip_out[pstAttrSampl->video_chn]);
	return NULL;
}

void venc_get_frame_thread_start(int channel)
{
	pthread_create(&venc_frame_info[channel].VencPid, 0,
		get_venc_frame_thread, (void *)&venc_frame_info[channel]);
}


void venc_get_frame_thread_stop(int channel)
{
	if (venc_frame_info[channel].VencThreadStart == EI_TRUE) {
		venc_frame_info[channel].VencThreadStart = EI_FALSE;
		pthread_join(venc_frame_info[channel].VencPid, 0);
	}
}


void sample_help()
{
	printf("this is sample videocapturer\n");
	printf("0 : videocapturer_creat_multiple_chn\n");
}

int videocapturer_creat_multiple_chn(int loop_time)
{
	int ret = -1;
	printf("video chn0 start\n");
    if ((videoCapturerHandle[0] = videoCapturerCreate()) == NULL)
    {
        printf("Failed to create video capturer\n");
    } else if (videoCapturerSetFormat(videoCapturerHandle[0], VID_FMT_H264, VID_RES_1080P)) {
		printf("Failed to set video format\n");
	}
	ret = videoCapturerAcquireStream(videoCapturerHandle[0]);
	if(ret != 0) {
		printf("videoCapturerAcquireStream failed\n");
		return ret;
	}
	printf("video_get_chn0_frame\n");
	venc_frame_info[0].codetype = VID_FMT_H264;
	venc_frame_info[0].handle = videoCapturerHandle[0];
	venc_frame_info[0].VencThreadStart = EI_TRUE;
	venc_frame_info[0].video_chn = 0;
	venc_get_frame_thread_start(0);

	printf("video chn1 start\n");
	if ((videoCapturerHandle[1] = videoCapturerCreate()) == NULL)
	{
		printf("Failed to create video capturer\n");
	} else if (videoCapturerSetFormat(videoCapturerHandle[1], VID_FMT_H265, VID_RES_720P)) {
		printf("Failed to set video format\n");
	}
	ret = videoCapturerAcquireStream(videoCapturerHandle[1]);
	if(ret != 0) {
		printf("videoCapturerAcquireStream failed\n");
		return ret;
	}
	printf("video_get_chn1_frame\n");
	venc_frame_info[1].codetype = VID_FMT_H265;
	venc_frame_info[1].handle = videoCapturerHandle[1];
	venc_frame_info[1].VencThreadStart = EI_TRUE;
	venc_frame_info[1].video_chn = 1;
	venc_get_frame_thread_start(1);

	printf("video chn2 start\n");
	if ((videoCapturerHandle[2] = videoCapturerCreate()) == NULL)
	{
		printf("Failed to create video capturer\n");
	} else if (videoCapturerSetFormat(videoCapturerHandle[2], VID_FMT_RAW, VID_RES_360P)) {
		printf("Failed to set video format\n");
	}
	printf("video_get_chn2_frame\n");
	venc_frame_info[2].codetype = VID_FMT_RAW;
	venc_frame_info[2].handle = videoCapturerHandle[2];
	venc_frame_info[2].VencThreadStart = EI_TRUE;
	venc_frame_info[2].video_chn = 2;
	venc_get_frame_thread_start(2);
	sleep(5);
	printf("chn0 status:%d\n",videoCapturerGetStatus(videoCapturerHandle[0]));
	sleep(5);
	VideoCapability Capability = {0};
	videoCapturerGetCapability(videoCapturerHandle[1], &Capability);
	printf("format:%x resolution:%x\n", Capability.formats, Capability.resolutions);
	sleep(5);
	VideoFormat format[3];
	VideoResolution Resolution[3];

	videoCapturerGetFormat(videoCapturerHandle[0], &format[0], &Resolution[0]);
	printf("format:%d resolution:%d\n", format[0], Resolution[0]);

	videoCapturerGetFormat(videoCapturerHandle[1], &format[1], &Resolution[1]);
	printf("format:%d resolution:%d\n", format[1], Resolution[1]);

	while (loop_time--) {
		usleep(1000 * 1000);
		//printf("loop_time:%d\n", loop_time);
	}
	venc_get_frame_thread_stop(2);
	venc_get_frame_thread_stop(1);
	venc_get_frame_thread_stop(0);
	videoCapturerDestory(videoCapturerHandle[2]);
	videoCapturerDestory(videoCapturerHandle[1]);
	videoCapturerDestory(videoCapturerHandle[0]);
	return 0;
}

int main(int argc,char**argv)
{
	int u32Mode, u32Opt;

	if (argc < 3) {
		u32Mode = 20;
		u32Opt = 0;
	} else {
		u32Mode = atoi(argv[1]);
		u32Opt = atoi(argv[2]);
	}

	switch (u32Mode) {
	case 0:
		videocapturer_creat_multiple_chn(u32Opt);
		break;
	default:
		sample_help();
		break;
	}
	return 0;
}
