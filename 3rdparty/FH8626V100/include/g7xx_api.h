#ifndef __g7xx_api_h__
#define __g7xx_api_h__

typedef void* G726_HANDLE;

#ifdef __cplusplus
extern "C"
{
#endif

extern G726_HANDLE fh_g726_create(void);
extern void fh_g726_destroy(G726_HANDLE h);

extern int fh_pcm_2_g711A(unsigned char* pcm, int length, unsigned char* pout);
extern int fh_pcm_2_g711U(unsigned char* pcm, int length, unsigned char* pout);
extern int fh_pcm_2_g726_16K(G726_HANDLE h, unsigned char* pcm, int length, unsigned char* pout);
extern int fh_pcm_2_g726_32K(G726_HANDLE h, unsigned char* pcm, int length, unsigned char* pout);

extern int fh_g711A_2_pcm(unsigned char* pdata, int length, unsigned char* pout);
extern int fh_g711U_2_pcm(unsigned char* pdata, int length, unsigned char* pout);
extern int fh_g726_16K_2_pcm(G726_HANDLE h, unsigned char* pdata, int length, unsigned char* pout);
extern int fh_g726_32K_2_pcm(G726_HANDLE h, unsigned char* pdata, int length, unsigned char* pout);

#ifdef __cplusplus
}
#endif

#endif //__g7xx_api_h__
