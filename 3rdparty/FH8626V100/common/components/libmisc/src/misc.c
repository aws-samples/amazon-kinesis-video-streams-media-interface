#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <libmisc/include/libmisc.h>

int libmisc_get_outer_box(
        unsigned char *pdata,
        unsigned int   dataW,
        unsigned int   dataH,
        unsigned int  *winX,
        unsigned int  *winY,
        unsigned int  *winW,
        unsigned int  *winH,
        unsigned char  thresh)
{
    unsigned int i;
    unsigned int j;
    unsigned int datanum;
    unsigned int minx = 0;
    unsigned int miny = 0;
    unsigned int maxx = 0;
    unsigned int maxy = 0;
    unsigned char *ptemp;

    datanum = dataW * dataH;
    for (i = 0; i < datanum; i++)
    {
        if (pdata[i] >= thresh)
        {
            miny = i / dataW;
            minx = i - miny * dataW;
            break;
        }
    }

    if (i == datanum)
    {
        maxx = maxy = -1;
        goto Exit;
    }

    for (j = datanum - 1; j >= i; j--)
    {
        if (pdata[j] >= thresh)
        {
            maxy = j / dataW;
            maxx = j - maxy * dataW;
            break;
        }
    }

    if (maxx < minx) /*swap*/
    {
        i = minx;
        minx = maxx;
        maxx = i;
    }

    ptemp = pdata + miny * dataW;
    for (i = miny; i <= maxy; i++)
    {
        for (j=0; j < minx; j++)
        {
            if (ptemp[j] >= thresh)
            {
                minx = j;
                break;
            }
        }
        ptemp += dataW;
    }

    ptemp = pdata + miny * dataW;
    for (i = miny; i <= maxy; i++)
    {
        for (j=dataW-1; j > maxx; j--)
        {
            if (ptemp[j] >= thresh)
            {
                maxx = j;
                break;
            }
        }
        ptemp += dataW;
    }

Exit:
    *winX = minx;
    *winY = miny;
    *winW = maxx - minx + 1;
    *winH = maxy - miny + 1;

    return 0;
}


void libmisc_perf_init(misc_perf_t *perf, int limit, char *msg)
{
    if (!msg)
        msg = "";

    perf->count = 0;
    perf->limit = limit;
    strncpy(perf->msg, msg, sizeof(perf->msg));
    perf->msg[sizeof(perf->msg) - 1] = 0; /*ensure NULL terminated*/
    gettimeofday(&perf->start_time, NULL);
}

void libmisc_perf_add(misc_perf_t *perf, char *msg)
{
    struct timeval now;
    int    diff;

    if (++perf->count >= perf->limit)
    {
        if (!msg)
            msg = "";

        gettimeofday(&now, NULL);
        diff = (now.tv_sec - perf->start_time.tv_sec) * 1000000 +
            now.tv_usec - perf->start_time.tv_usec;
        if (perf->count > 1)
        {
            printf("%s%s: %dus, count=%d.\n", perf->msg, msg, diff, perf->count);
        }
        else
        {
            printf("%s%s: %dus.\n", perf->msg, msg, diff);
        }

        gettimeofday(&perf->start_time, NULL);
        perf->count = 0;
    }
}
