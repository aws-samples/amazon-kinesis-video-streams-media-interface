#ifndef __lib_misc_h__
#define __lib_misc_h__

typedef struct
{
    struct timeval start_time;
    int            count;
    int            limit;
    char           msg[32];
} misc_perf_t;

extern void libmisc_perf_init(misc_perf_t *perf, int limit, char *msg);
extern void libmisc_perf_add(misc_perf_t *perf, char *msg);

extern int libmisc_get_outer_box(
        unsigned char *pdata,
        unsigned int   dataW,
        unsigned int   dataH,
        unsigned int  *winX,
        unsigned int  *winY,
        unsigned int  *winW,
        unsigned int  *winH,
        unsigned char  thresh);

#endif /*__lib_misc_h__*/
