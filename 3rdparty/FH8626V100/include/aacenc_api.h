#ifndef __aacenc_api_h__
#define __aacenc_api_h__

typedef void* AAC_ENC_HANDLE;

#ifdef __cplusplus
extern "C"
{
#endif

extern AAC_ENC_HANDLE fh_aacenc_create(int channels, int sample_rate, int bit_rate);
extern void fh_aacenc_destroy(AAC_ENC_HANDLE h);
extern int fh_aacenc_encode(AAC_ENC_HANDLE h, unsigned char* pcm, int length, unsigned char* pout, int outbuf_length);

#ifdef __cplusplus
}
#endif

#endif //__aacenc_api_h__
