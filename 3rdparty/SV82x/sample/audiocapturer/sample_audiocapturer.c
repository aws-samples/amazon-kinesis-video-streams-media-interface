#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/types.h>
#include "sample_comm.h"

#include "com/amazonaws/kinesis/video/capturer/AudioCapturer.h"
#include "com/amazonaws/kinesis/video/capability/AudioCapability.h"

#define AUDIO_FRAME_BUFFER_SIZE_BYTES      (1024UL)

typedef struct eiAudio_Suffix_S {
	char *pSuffix;
	AudioFormat enType;
} Audio_Suffix_S;

Audio_Suffix_S g_AudioSuffix[] = {
	{"g711a", AUD_FMT_G711A},
	{"g711u", AUD_FMT_G711U},
	{"aac", AUD_FMT_AAC},
	{"pcm", AUD_FMT_PCM},
	{NULL, PT_BUTT},
};
typedef struct tagSAMPLE_AENC_S {
	EI_BOOL bStart;
	EI_BOOL bStartIn;
	pthread_t stAencPid;
	pthread_t stAencPidIn;
	EI_S32  AeChn;
	EI_S32  AdChn;
	FILE    *pfd;
	FILE *pfdIn;
	EI_BOOL bSendAdChn;
} SAMPLE_AENC_S;

static SAMPLE_AENC_S gs_stSampleAencAdec[AENC_MAX_CHN_NUM];
static AudioCapturerHandle audioCapturerHandle;

static char *SAMPLE_AUDIO_Pt2Str(AudioFormat enType)
{
	int i = 0;

	while (g_AudioSuffix[i].pSuffix != NULL) {
		if (g_AudioSuffix[i].enType == enType) {
			return g_AudioSuffix[i].pSuffix;
		}
		i++;
	}

	return "data";
}

static FILE *SAMPLE_AUDIO_OpenSaveFile(AENC_CHN AeChn, AudioFormat enType)
{
	FILE *pfd;
	EI_CHAR aszFileName[FILE_NAME_LEN] = {0};

	snprintf(aszFileName, FILE_NAME_LEN, "/data/audio_chn%d.%s", AeChn, SAMPLE_AUDIO_Pt2Str(enType));

	pfd = fopen(aszFileName, "w+");
	if (pfd == NULL) {
		printf("%s: open file %s failed\n", __func__, aszFileName);
		return NULL;
	}
	printf("open stream file:\"%s\" for aenc ok\n", aszFileName);
	return pfd;
}


void sendAudioPackets(void *parg)
{
	void *pFrameBuffer = NULL;
	EI_U64 timestamp = 0;
	EI_SIZE_T frameSize = 0;
	SAMPLE_AENC_S *pstAencCtl = (SAMPLE_AENC_S *)parg;

	pFrameBuffer = (void *)malloc(AUDIO_FRAME_BUFFER_SIZE_BYTES);

	if (!pFrameBuffer) {
		printf("%s malloc failed\n", __func__);
		return NULL;
	}

	while (pstAencCtl->bStart) {
		if (audioCapturerGetFrame(audioCapturerHandle, pFrameBuffer,
				AUDIO_FRAME_BUFFER_SIZE_BYTES, &timestamp, &frameSize)) {
			printf("audioCapturerGetFrame failed\n");
		}

		if (pstAencCtl->pfd) {
			printf("write to file pts=%llu,frameSize=%d\n", timestamp, frameSize);
			fwrite(pFrameBuffer, 1, frameSize, pstAencCtl->pfd);
		}
	}

	audioCapturerReleaseStream(audioCapturerHandle);
	printf("audioCapturerReleaseStream ok\n");
	free(pFrameBuffer);
	pFrameBuffer = NULL;
	fclose(pstAencCtl->pfd);
	pstAencCtl->bStart = EI_FALSE;
	return NULL;
}

static EI_S32 SAMPLE_AUDIO_CreatTrdAencFile(AENC_CHN AeChn, ADEC_CHN AdChn, FILE *pAecFd)
{
	SAMPLE_AENC_S *pstAenc = NULL;

	if (pAecFd == NULL) {
		return EI_FAILURE;
	}

	pstAenc = &gs_stSampleAencAdec[AeChn];
	pstAenc->AeChn = AeChn;
	pstAenc->AdChn = AdChn;
	pstAenc->bSendAdChn = EI_FALSE;
	pstAenc->pfd = pAecFd;
	pstAenc->bStart = EI_TRUE;
	pthread_create(&pstAenc->stAencPid, 0, sendAudioPackets, pstAenc);

	return EI_SUCCESS;
}

int main(int argc, char *argv[])
{
	int retint = EI_SUCCESS;
	FILE *pfd = NULL;
	SAMPLE_AENC_S *pstAenc = NULL;
	AudioCapability audioCapability = {0};

	EI_MI_MBASE_Init();
	EI_MI_VBUF_Init();
	EI_MI_MLINK_Init();

	audioCapturerHandle = audioCapturerCreate();
	if (!audioCapturerHandle) {
	} else {
		audioCapturerGetCapability(audioCapturerHandle, &audioCapability);
		printf("AudioCapturer Capability: formats[%x], channels[%x],sampleRates[%x], bitDepths[%x]\n",
			audioCapability.formats, audioCapability.channels, audioCapability.sampleRates, audioCapability.bitDepths);
		if (audioCapturerSetFormat(audioCapturerHandle, AUD_FMT_G711A, AUD_CHN_MONO, AUD_SAM_8K, AUD_BIT_16)) {
			printf("Unable to set AudioCapturer format");
			audioCapturerDestory(audioCapturerHandle);
			audioCapturerHandle = NULL;
		}
	}
	printf("Board AudioCapturer initialized");

	//start AI
	if (audioCapturerAcquireStream(audioCapturerHandle)) {
		printf("audioCapturerAcquireStream failed\n");
		return -1;
	}

	pfd = SAMPLE_AUDIO_OpenSaveFile(0, AUD_FMT_G711A);
	if (pfd == NULL) {
		printf("SAMPLE_AUDIO_OpenSaveFile failed\n");
		return -1;
	}
	retint = SAMPLE_AUDIO_CreatTrdAencFile(0, 0, pfd);
	if (retint == EI_FAILURE) {
		printf("SAMPLE_AUDIO_CreatTrdAencFile failed\n");
		return -1;
	}

	printf("\nplease press twice ENTER to exit this sample\n");
	getchar();
	getchar();

	pstAenc = &gs_stSampleAencAdec[0];
	if (pstAenc->bStart) {
		pstAenc->bStart = EI_FALSE;
		pthread_join(pstAenc->stAencPid, 0);
	}

	if (audioCapturerHandle) {
		audioCapturerDestory(audioCapturerHandle);
		audioCapturerHandle = NULL;
	}

	EI_MI_MLINK_Exit();
	EI_MI_VBUF_Exit();
	EI_MI_MBASE_Exit();

	return retint;
}
