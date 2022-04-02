#ifdef __LINUX_OS__
#include "sample_common.h"

static int write_proc(char *path, char *s)
{
    int fd;

    fd = open(path, O_RDWR);
    if(fd < 0)
    {
        printf("Error: open %s!\n", path);
        return -1;
    }

    write(fd, s, strlen(s));
    close(fd);
    return 0;
}

int vpu_write_proc(char *s)
{
    return write_proc("/proc/driver/vpu", s);
}

int enc_write_proc(char *s)
{
    return write_proc("/proc/driver/enc", s);
}

int jpeg_write_proc(char *s)
{
    return write_proc("/proc/driver/jpeg", s);
}

int isp_write_proc(char *s)
{
    return write_proc("/proc/driver/isp", s);
}
#endif
