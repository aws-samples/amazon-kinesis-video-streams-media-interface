#include <stddef.h>
#include <sys/time.h>

#include "AK3918Port.h"

__attribute__((weak)) uint64_t getEpochTimestampInUs(void)
{
    uint64_t timestamp = 0;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    timestamp = (uint64_t) (tv.tv_sec) * 1000 * 1000 + (uint64_t) (tv.tv_usec);

    return timestamp;
}
