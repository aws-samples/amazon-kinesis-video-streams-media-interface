#include "sample_common.h"
#include "model.h"
#if __LINUX_OS__
/* 获取人形检测模型数据
 *  input:   file_name  框像素宽度
 *  output： **data     模型数据存储地址
 *           *data_len  模型数据长度
 *  return：  0         成功
 *           -1         失败
 */
FH_SINT32 load_hd_model(FH_UINT8 **data, FH_UINT32 *data_len, FH_UINT32 model_serial)
{
	FH_UINT32      fsize;
    FH_SINT32      ret;
    FILE          *fp;          /*检测模型参数文件*/
    FH_CHAR        file_name[256];
    MEM_DESC       MODEL_FILE; /* 模型1数据buf，用于推送至arc端 */

    snprintf(file_name, sizeof(file_name), "model_human_%d.bin", model_serial);

    if ((fp = fopen(file_name, "rb")) == NULL)
    {
        printf("Error: open file %s failed!\n", file_name);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ret = buffer_malloc(&MODEL_FILE, fsize, DEFALT_ALIGN);
    if (ret)
    {
        printf("Error: no vmm memory for human!\n");
        fclose(fp);
        return -1;
    }

    *data = (FH_UINT8 *)MODEL_FILE.base;
    *data_len = fsize;
    fread((FH_UINT8 *)MODEL_FILE.vbase, fsize, 1, fp); /* 将模型数据读取至vmm空间*/
    fclose(fp);

    return 0;
}
#endif /*__LINUX_OS__*/
