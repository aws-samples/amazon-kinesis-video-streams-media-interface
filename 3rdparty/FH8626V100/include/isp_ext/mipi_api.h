#ifndef _MIPI_API_H_
#define _MIPI_API_H_


/**
* FH_MIPI_Version
*@brief 按SDK的统一格式定义的版本信息
*@param [in] print_enable:是否打印版本信息
*@param [out] 无
*@return 是否成功
* - RETURN_OK(0):  成功
* - 其他：失败,错误详见错误号
*/
char *FH_MIPI_Version(unsigned int print_enable);
#endif