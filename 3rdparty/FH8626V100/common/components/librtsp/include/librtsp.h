#ifndef __LIB_RTSP_H__
#define __LIB_RTSP_H__

enum rtp_transport
{
    RTP_TRANSPORT_UDP,
    RTP_TRANSPORT_TCP
};

typedef enum
{
    FH_SDP_LPCM_L8_R8   = 0, // 8bite 8k pcm
    FH_SDP_LPCM_L16_R8  = 1, // 16bite 8k pcm
    FH_SDP_LPCM_L16_R16 = 2, // 16bite 16k pcm
    FH_SDP_G711A_8K     = 3, // ������Ƶ֧�־��ޣ����¾�Ϊ16biteλ��
    FH_SDP_G711A_16K    = 4,
    FH_SDP_G711U_8K     = 5,
    FH_SDP_G711U_16K    = 6,
    FH_SDP_G726_16K     = 7, // Э���֧��8k����
    FH_SDP_G726_32K     = 8, // Э���֧��8k����
    FH_SDP_AAC_8K       = 9,
    FH_SDP_AAC_16K      = 10,
    FH_SDP_AAC_32K      = 11,
    FH_SDP_AAC_441K     = 12,
    FH_SDP_AAC_48K      = 13,
    FH_SDP_AC_MAX       = 14,
    FH_VENC_JPEG        = 15,
    FH_VENC_MJPEG       = 16,
    FH_VENC_H264        = 17,
    FH_VENC_H265        = 18,
    FH_VENC_MAX,
}FH_MEDIA_FORMAT_TYPE;


#define FH_AUDIO_FORMAT     0
#define FH_VIDEO_FORMAT     1

typedef void* RTSP_HANDLE;

RTSP_HANDLE librtsp_start_server(enum rtp_transport transport, int port);
void librtsp_stop_server(RTSP_HANDLE h);
void librtsp_push_data(RTSP_HANDLE h, void *data, int size, unsigned long long pts,
                        unsigned int channel_id, FH_MEDIA_FORMAT_TYPE media_format);
void librtsp_play_sethook(RTSP_HANDLE h, void (*hook)(void *), void *param);

#endif /*__LIB_RTSP_H__*/
