#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <dirent.h>


static const char* bin_file_name[] = {
    "model_human_73733582.bin",
    "model_human_73733930.bin",
    "model_human_77403532.bin",
    "model_human_79074700.bin",
    "model_human_81677840.bin",
    "model_human_86185422.bin",
    "model_human_86185450.bin",
    "model_human_86185770.bin",
    NULL,
};

static const char* h_file_name[] = {
    "model_human_head_73733582.h",
    "model_human_body_73733930.h",
    "model_human_face_77403532.h",
    "model_human_frontface_79074700.h",
    "model_human_frontface_81677840.h",
    "model_human_head_86185422.h",
    "model_human_middlebody_86185450.h",
    "model_human_body_86185770.h",
    NULL,
};

#define MAX_BIN_FILE_NUM (100)
static char bin_file_list[MAX_BIN_FILE_NUM][256];

static int get_bin_list(void)
{
    int file_num = 0;
    int len;
    DIR *dir;
    struct dirent *entry;

    memset(bin_file_list, 0, sizeof(bin_file_list));

    dir = opendir("./");
    if (!dir)
    {
        printf("Error: opendir failed!\n");
        return -1;
    }

    while (1)
    {
        entry = readdir(dir);
        if (!entry)
            break;   	 

        len = strlen(entry->d_name);
        if (len > 4 && len <= 255 && !strcmp(entry->d_name+len-4, ".bin"))
        {
            if (file_num >= MAX_BIN_FILE_NUM)
            {
                printf("Error: too many bin files!\n");
                break;
            }

            strcpy(&bin_file_list[file_num][0], entry->d_name);
            file_num++;
        }
    }

    closedir(dir);

    return 0;
}

static int get_hfile_name(char *hfile_name, char *bin_name)
{
    int i = 0;

    while (1)
    {
        if (!bin_file_name[i])
            break;

        if (!strcmp(bin_name, bin_file_name[i]))
        {
            strcpy(hfile_name, h_file_name[i]);
            return 0;
        }
        i++;
    }

    return -1;	
}

static int get_macro_define(char *macro_define, char *h_fname)
{
    int len;
    char chr;

    macro_define[0] = '_';
    strcpy(&macro_define[1], h_fname);
    strcat(macro_define, "_");

    len = strlen(macro_define);	
    if (len > 4 && macro_define[len - 3] == '.')
    {
        macro_define[len - 3] = '_';
    }

    while (1)
    {
        chr = *macro_define;
        if (!chr)
            break;

        if (chr >= 'a' && chr <= 'z')
        {
            *macro_define = chr - 'a' + 'A';
        }

        macro_define++;
    }

    return 0;
}

static int get_array_name(char *array_name, char *h_fname)
{
    char *p;

    strcpy(array_name, h_fname);
    p = strrchr((const char *)array_name, '_');
    if (p)
    {
        *p = 0;
    }

    return 0;
}

static void nwrite(int fd, char *data, int len)
{
    int ret;

    if (len <= 0 || len > 4096)
    {
        printf("Error: internal error,it should not happen,len=%d.\n", len);
        exit(-1);
    }

    ret = write(fd, data, len);
    if (ret != len)
    {
        printf("Error: write failed,ret=%d,len=%d.\n", ret, len);
        exit(-1);
    }
}

static unsigned int check_sum(char *data, int len)
{
    unsigned int sum = 0;
    unsigned int *p = (unsigned int *)data;

    len /= 4;	
    while (len > 0)
    {
        sum += *(p++);
        len--;
    }

    return sum;
}

static char *read_whole_file(char *fname, int *filelen)
{
    int ret;
    int fd;
    int flen;
    char *fdata;
    unsigned int sum1;
    unsigned int sum2;
    unsigned int padlen;
    struct stat status;

    fd = open(fname, O_RDONLY);
    if (fd < 0)
    {
        printf("Error: open %s failed!\n", fname);
        exit(-1);
    }

    ret = fstat(fd, &status);
    if (ret != 0)
    {
        printf("Error: fstat %s failed!\n", fname);
        exit(-1);
    }

    flen = status.st_size;
    if (!(flen >= 4096 && flen <= 5*1024*1024 && (flen & 3) == 0))
    {
        printf("Error: invalid bin file %s with size=%d!\n", fname, flen);
        exit(-1);
    }

    fdata = malloc(flen + 256/*reserved for 128 Bytes aligned*/);
    if (!fdata)
    {
        printf("Error: malloc failed!\n");
        exit(-1);		
    }

    ret = read(fd, fdata, flen);

    close(fd);

    if (ret != flen)
    {
        printf("Error: read %s failed,ret=%d!\n", fname, ret);
        exit(-1);
    }

    sum1 = *((unsigned int *)(fdata + flen - 4));
    sum2 = check_sum(fdata, flen - 4);
    if (sum1 != sum2)
    {
        printf("Error: %s checksum failed,sum1=%08x,sum2=%08x!\n", fname, sum1, sum2);
        exit(-1);
    }

    /*Add padding to ensure 128 bytes aligned*/	
    if ((flen & 127) != 0) 
    {		
        padlen = flen + 8; /*pad 8bytes at least*/
        padlen = (padlen + 127) & (~127);

        *((unsigned int *)(fdata + flen)) = -sum1;
        flen += 4;

        while (flen < padlen)
        {
            *((unsigned int *)(fdata + flen)) = 0;
            flen += 4;
        }

        *((unsigned int *)(fdata + flen - 4)) = sum1;
    }

    *filelen = flen;
    return fdata;
}

static int trans_one_file(char *bin_fname)
{
    int i;
    int len;
    int flen = 0;
    char *fdata;
    int hdrfd = -1;
    char h_fname[256];
    char macro_define[300];
    char array_name[256];
    char fcontent[8192];
    char line[256];    

    if (get_hfile_name(h_fname, bin_fname) != 0)
    {
        printf("Warning: skip translate file %s\n", bin_fname);
        return -1;
    }

    printf("translate file %s\n", bin_fname);

    get_macro_define(macro_define, h_fname);
    get_array_name(array_name, h_fname);

    hdrfd = open(h_fname, O_RDWR | O_CREAT | O_TRUNC);
    if (hdrfd < 0)
    {	
        printf("Error: create file %s failed.\n", h_fname);
        goto Exit;
    }

    fchmod(hdrfd, 0644);

    memset(fcontent, 0, sizeof(fcontent));
    strcat(fcontent, "#ifndef ");
    strcat(fcontent, macro_define);
    strcat(fcontent, "\n");

    strcat(fcontent, "#define ");
    strcat(fcontent, macro_define);
    strcat(fcontent, "\n");

    strcat(fcontent, "\n");

    strcat(fcontent, "#ifdef __cplusplus\n");
    strcat(fcontent, "#if __cplusplus\n");
    strcat(fcontent, "extern \"C\" {\n");
    strcat(fcontent, "#endif\n");
    strcat(fcontent, "#endif\n");
    strcat(fcontent, "\n");

    strcat(fcontent, "unsigned char __attribute__((aligned(128))) ");
    strcat(fcontent, array_name);
    strcat(fcontent, "[] = {\n" );

    nwrite(hdrfd, fcontent, strlen(fcontent));

    fdata = read_whole_file(bin_fname, &flen);

    for (i=0; i<flen; i++)
    {
        len = sprintf(line, "0x%02x, ", (unsigned char)fdata[i]);
        nwrite(hdrfd, line, len);

        if ((i+1) % 16 == 0)
        {
            nwrite(hdrfd, "\n", 1);
        }
    }

    free(fdata);

    memset(fcontent, 0, sizeof(fcontent));
    strcat(fcontent, "};\n");
    strcat(fcontent, "\n");
    strcat(fcontent, "#ifdef __cplusplus\n");
    strcat(fcontent, "#if __cplusplus\n");
    strcat(fcontent, "}\n");
    strcat(fcontent, "#endif\n");
    strcat(fcontent, "#endif\n");
    strcat(fcontent, "\n");
    strcat(fcontent, "#endif");

    nwrite(hdrfd, fcontent, strlen(fcontent));

    close(hdrfd);

    return 0;

Exit:
    printf("translate %s Fail!\n", bin_fname);
    if (hdrfd >= 0)
        close(hdrfd);
    exit(-1);
    return -1;
}

int main(int argc, char *argv[])
{
    int ret;
    int index = 0;

    ret = get_bin_list();
    if (ret)
    {
        return ret;
    }

    for(index = 0; index < MAX_BIN_FILE_NUM; index++)
    {
        if (bin_file_list[index][0] == 0)
            continue;

        trans_one_file(&bin_file_list[index][0]);
    }

    printf("translate success!\n");

    return 0;
}
