#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/types.h>
#include "sample_comm.h"

#include "com/amazonaws/kinesis/video/player/AudioPlayer.h"
#include "com/amazonaws/kinesis/video/capability/AudioCapability.h"

#define AUDIO_FRAME_BUFFER_SIZE_BYTES    (1024UL)
#define HUNDREDS_OF_NANOS_IN_A_MICROSECOND 10LL

static AudioPlayerHandle audioPlayerHandle;

typedef struct tagSAMPLE_ADEC_S {
	EI_BOOL bStart;
	EI_BOOL bStartOut;
	EI_S32 AdChn;
	EI_S32 AoDev;
	EI_S32 AoChn;
	EI_BOOL bSendAo;
	FILE *pfd;
	FILE *pfdOut;
	pthread_t stAdPid;
	pthread_t stAdPidout;
} SAMPLE_ADEC_S;

static SAMPLE_ADEC_S gs_stSampleFileAdec[ADEC_MAX_CHN_NUM];

int AudioFilePlay(void *parg)
{
	void *pFrameBuffer = NULL;
	size_t frameSize = 0;
	SAMPLE_ADEC_S *pstAdecCtl = (SAMPLE_ADEC_S *)parg;
	FILE *pfd = pstAdecCtl->pfd;
	EI_U32 u32Len = 1024 * 4;
	frameSize = MAX_AUDIO_STREAM_LEN;

	fseek(pfd, 0, SEEK_END);
	int fileLen = ftell(pfd);

	fseek(pfd, 0, SEEK_SET);
	pFrameBuffer = (char *)malloc(MAX_AUDIO_STREAM_LEN);

	while (fileLen > 0 && pstAdecCtl->bStart == EI_TRUE) {
		frameSize = fread(pFrameBuffer, 1, u32Len, pfd);
		if (frameSize <= 0) {
			audioPlayerWriteFrame(audioPlayerHandle, pFrameBuffer, frameSize);
			break;
		}
		audioPlayerWriteFrame(audioPlayerHandle, pFrameBuffer, frameSize);
	}

	free(pFrameBuffer);
	return EI_SUCCESS;

CleanUp:
	printf("audioPlayerAcquireStream failed\n");
	return 0;
}

static EI_S32 SAMPLE_AUDIO_CreatTrdFileAdec(ADEC_CHN AdChn, FILE *pAdcFd)
{
	SAMPLE_ADEC_S *pstAdec = NULL;

	if (pAdcFd == NULL) {
		return EI_FAILURE;
	}

	pstAdec = &gs_stSampleFileAdec[AdChn];
	pstAdec->AdChn = AdChn;
	pstAdec->pfd = pAdcFd;
	pstAdec->bStart = EI_TRUE;
	pthread_create(&pstAdec->stAdPid, 0, AudioFilePlay, pstAdec);

	return EI_SUCCESS;
}

static FILE *SAMPLE_AUDIO_OpenPlayFile(EI_CHAR *paszFileName)
{
	FILE *pfd;
	EI_CHAR aszFileName[128] = {0};

	snprintf(aszFileName, 128, "/data/%s", paszFileName);

	if (aszFileName == NULL)
		return NULL;

	pfd = fopen(aszFileName, "rb");
	if (pfd == NULL) {
		printf("%s: open file %s failed\n", __func__, aszFileName);
		return NULL;
	}
	printf("open stream file:\"%s\" for play ok\n", aszFileName);
	return pfd;
}

void Audio_Player_Usage(void)
{
	printf("Usage: sample_audioplayer <aszFileName>\n");
}


int main(int argc, char *argv[])
{
	int retint =   EI_SUCCESS;
	EI_CHAR *aszFileName = argv[1];
	FILE *pfd = NULL;
	SAMPLE_ADEC_S *pstAdec = NULL;

	EI_MI_MBASE_Init();
	EI_MI_VBUF_Init();
	EI_MI_MLINK_Init();

	if (argc < 2) {
		Audio_Player_Usage();
		return EI_FAILURE;
	}

	AudioCapability audioCapability = {0};

	audioPlayerHandle = audioPlayerCreate();
	if (!audioPlayerHandle) {
		printf("AudioPlayer init failed");
	} else {
		audioPlayerGetCapability(audioPlayerHandle, &audioCapability);
		printf("Capability[%x], channels[%x],sampleRates[%x],bitDepths[%x]",
			audioCapability.formats, audioCapability.channels, audioCapability.sampleRates, audioCapability.bitDepths);

		if (audioPlayerSetFormat(audioPlayerHandle, AUD_FMT_PCM, AUD_CHN_STEREO, AUD_SAM_48K, AUD_BIT_16)) {
			printf("Unable to set AudioPlayer format");
			audioPlayerDestory(audioPlayerHandle);
			audioPlayerHandle = NULL;
		}
	}

	if (audioPlayerAcquireStream(audioPlayerHandle)) {
		printf("audioPlayerAcquireStream failed\n");
		return -1;
	}

	pfd = SAMPLE_AUDIO_OpenPlayFile(aszFileName);
	if (pfd == NULL) {
		printf("SAMPLE_AUDIO_OpenPlayFile failed\n");
		return -1;
	}

	SAMPLE_AUDIO_CreatTrdFileAdec(0, pfd);
	printf("\nplease press twice ENTER to exit this sample\n");
	getchar();
	getchar();

	pstAdec = &gs_stSampleFileAdec[0];
	if (pstAdec->bStart) {
		pstAdec->bStart = EI_FALSE;
		pthread_join(pstAdec->stAdPid, 0);
	}

	audioPlayerDestory(audioPlayerHandle);

	EI_MI_MLINK_Exit();
	EI_MI_VBUF_Exit();
	EI_MI_MBASE_Exit();
	printf("Audio Player exited\n");
	return retint;
}
