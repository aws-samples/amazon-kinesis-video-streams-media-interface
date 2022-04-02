enum DCI_CMD{
	DCI_NO_COMMAND =0,
	DCI_SAVE_DRV_REG =1,

};
enum space_type{
userspace =0,
defaultsapce = 1,
};

void DCI_Init();
void DCI_DrvCommand();
int API_DCI_GET_PARAM(unsigned char *cmd,unsigned char *cmd_param,unsigned int *param0,unsigned int *param1);

/**
* FH_DCI_Version
*@brief 按SDK的统一格式定义的版本信息
*@param [in] print_enable:是否打印版本信息
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
char *FH_DCI_Version(unsigned int print_enable);