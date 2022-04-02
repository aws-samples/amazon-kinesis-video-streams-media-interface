#ifndef __libpes_h__
#define __libpes_h__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NALU_COUNT  20

    enum vlcview_enc_type
    {
        VLCVIEW_ENC_H264,
        VLCVIEW_ENC_H265,
    };

    enum vlcview_enc_frame_type
    {
        VLCVIEW_ENC_I_FRAME,
        VLCVIEW_ENC_P_FRAME,
    };

    struct vlcview_nalu
    {
        unsigned int  len;
        unsigned char *start;
    };

    struct vlcview_enc_stream_element
    {
        enum vlcview_enc_frame_type frame_type;
        int frame_len;
        int nalu_count;
        unsigned long long time_stamp;
        struct vlcview_nalu nalu[MAX_NALU_COUNT];
        enum vlcview_enc_type enc_type;
        unsigned char *frame_start;    /*not used yet...*/
    };

#ifdef __FH_USING_SRT__
    int  srt_libpes_init(int port, int max_channel);
#else
    int  libpes_init(void);
#endif
    int  libpes_uninit(void);
    int  libpes_stream_pack(int channel, struct vlcview_enc_stream_element stream_element);
    int  libpes_send_to_vlc(int channel, char *ip, int port);


#ifdef __cplusplus
}
#endif

#endif /*__libpes_h__*/
