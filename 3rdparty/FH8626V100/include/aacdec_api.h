#ifndef __aacdec_api_h__
#define __aacdec_api_h__

//the error code...
#define AACDEC_OK              (0)
#define AACDEC_NOT_ENOUGH_BITS (-1)
#define AACDEC_INTERNAL_ERROR  (-2)

typedef void* AAC_DEC_HANDLE;

typedef enum
{
	AAC_TT_MP4_ADTS = 2, //now only ADTS is supported...
} AAC_TRANSPORT_TYPE;

typedef struct
{
	int channels;
	int frame_size;
	int sample_rate;
	int bit_rate;
} bitstream_info_t;

#ifdef __cplusplus
extern "C"
{
#endif

extern AAC_DEC_HANDLE fh_aacdec_create(AAC_TRANSPORT_TYPE t);
extern void fh_aacdec_destroy(AAC_DEC_HANDLE h);
extern int fh_aacdec_fill_bitstream(AAC_DEC_HANDLE h, unsigned char* p_bitstream, int length, int* p_valid_bytes);
extern int fh_aacdec_decode_frame(AAC_DEC_HANDLE h, unsigned char* p_outbuf, int n_outbuf_length, bitstream_info_t* pinfo);

#ifdef __cplusplus
}
#endif

#endif //__aacdec_api_h__
