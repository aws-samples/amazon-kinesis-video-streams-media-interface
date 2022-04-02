/*
 * fh_crypto_api.h
 *
 *  Created on: Dec 6, 2016
 *      Author: duobao
 */

#ifndef FH_CRYPTO_API_H_
#define FH_CRYPTO_API_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <getopt.h>
#include <sys/socket.h>
#include "linux/if_alg.h"

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

#ifndef AF_ALG
#define AF_ALG 38
#endif
#ifndef SOL_ALG
#define SOL_ALG 279
#endif

#define CRYPTO_CPU_SET_KEY                      (1<<0)
#define CRYPTO_EX_MEM_SET_KEY                   (1<<1)
#define CRYPTO_EX_MEM_INDEP_POWER               (1<<2)
/*bit 8~ex mem bit field..*/
#define CRYPTO_EX_MEM_SWITCH_KEY                (1<<8)
/*if set ex mem set switch key..then parse below..*/
#define CRYPTO_EX_MEM_4_ENTRY_1_KEY             (1<<9)
/*
成员名称				描述
CRYPTO_ALG_DES			DES 算法
CRYPTO_ALG_3DES			3DES 算法
CRYPTO_ALG_AES			AES 算法
*/
typedef enum FH_CRYPTO_ALG_E
{
	CRYPTO_ALG_DES,
	CRYPTO_ALG_3DES,
	CRYPTO_ALG_AES,
	CRYPTO_ALG_BUTT
}CRYPTO_ALG_E;


/*
成员名称				描述
CRYPTO_WORK_MODE_ECB		ECB（Electronic CodeBook）模式
CRYPTO_WORK_MODE_CBC		CBC（Cipher Block Chaining）模式
CRYPTO_WORK_MODE_CFB		CFB（Cipher FeedBack）模式
CRYPTO_WORK_MODE_OFB		OFB（Output FeedBack）模式
CRYPTO_WORK_MODE_CTR		CTR（Counter）模式
CRYPTO_WORK_MODE_CBC_CTS	CBC CTS（Community Tissue Services）模式
CRYPTO_WORK_MODE_BUTT		无效模式
*/
typedef enum FH_CRYPTO_WORK_MODE_E
{
	CRYPTO_WORK_MODE_ECB,
	CRYPTO_WORK_MODE_CBC,
	CRYPTO_WORK_MODE_CFB,
	CRYPTO_WORK_MODE_OFB,
	CRYPTO_WORK_MODE_CTR,
	CRYPTO_WORK_MODE_BUTT    = 0xffffffff
}CRYPTO_WORK_MODE_E;


/*
成员名称				描述
CRYPTO_BIT_WIDTH_64BIT		64bit 位宽
CRYPTO_BIT_WIDTH_8BIT		8bit 位宽
CRYPTO_BIT_WIDTH_1BIT		1bit 位宽
CRYPTO_BIT_WIDTH_128BIT		128bit 位宽
*/
typedef enum FH_CRYPTO_BIT_WIDTH_E
{
	CRYPTO_BIT_WIDTH_64BIT,
	CRYPTO_BIT_WIDTH_8BIT,
	CRYPTO_BIT_WIDTH_1BIT,
	CRYPTO_BIT_WIDTH_128BIT,
}CRYPTO_BIT_WIDTH_E;


/*
成员名称				描述
CRYPTO_KEY_AES_128BIT		AES 运算方式下采用 128bit 密钥长度
CRYPTO_KEY_AES_192BIT		AES 运算方式下采用 192bit 密钥长度
CRYPTO_KEY_AES_256BIT		AES 运算方式下采用 256bit 密钥长度
CRYPTO_KEY_DES_3KEY		3DES 运算方式下采用 3 个 key
*/
typedef enum FH_CRYPTO_KEY_LENGTH_E
{
	CRYPTO_KEY_AES_128BIT = 0x0,
	CRYPTO_KEY_AES_192BIT = 0x1,
	CRYPTO_KEY_AES_256BIT = 0x2,
	CRYPTO_KEY_DES_3KEY = 0x3,
	CRYPTO_KEY_DES = 0x4,
}CRYPTO_KEY_LENGTH_E;


typedef enum FH_CRYPTO_IV_LENGTH_E
{
	CRYPTO_IV_KEY_AES_0BIT = 0x0,
	CRYPTO_IV_KEY_AES_64BIT = 0x1,
	CRYPTO_IV_KEY_AES_128BIT = 0x2,
} CRYPTO_IV_LENGTH_E;

/*
成员名称				描述
CRYPTO_KEY_SRC_USER		用户配置的 Key
CRYPTO_KEY_SRC_EFUSE_0		Efuse 的第 0 组 Key
CRYPTO_KEY_SRC_EFUSE_1		Efuse 的第 1 组 Key
CRYPTO_KEY_SRC_EFUSE_2		Efuse 的第 2 组 Key
CRYPTO_KEY_SRC_EFUSE_3		Efuse 的第 3 组 Key
CRYPTO_KEY_SRC_BUTT		无效类型
*/
typedef enum FH_CRYPTO_KEY_SRC_E
{
	CRYPTO_KEY_SRC_USER = CRYPTO_CPU_SET_KEY,
	CRYPTO_KEY_SRC_EFUSE = CRYPTO_EX_MEM_SET_KEY,
	CRYPTO_KEY_SRC_BUTT
} CRYPTO_KEY_SRC_E;


/*
成员名称				描述
bit1IV				向量变更
bitsResv  			保留
*/
typedef struct FH_CRYPTO_CTRL_CHANGE_FLAG_S
{
	uint32_t   bit1IV:     1;
	uint32_t   bitsResv:  31;
} CRYPTO_CTRL_CHANGE_FLAG_S;


/*
成员名称				描述
key[8]				密钥
iv[4]				初始向量
alg  				加密算法
bit_width  			加密或解密的位宽
work_mode  			工作模式
key_len				密钥长度
iv_len
change_flag  			更新标志位，表示 IV 等是否需要更新
key_src				密钥的来源
*/
typedef struct FH_CRYPTO_CTRL_S
{
	uint32_t u32Key[8];
	uint32_t u32IV[4];
	CRYPTO_ALG_E enAlg;
	CRYPTO_BIT_WIDTH_E  enBitWidth;
	CRYPTO_WORK_MODE_E  enWorkMode;
	CRYPTO_KEY_LENGTH_E enKeyLen;
	CRYPTO_IV_LENGTH_E enIVLen;
	CRYPTO_CTRL_CHANGE_FLAG_S enChangeFlag;
	CRYPTO_KEY_SRC_E enKeySrc;
} CRYPTO_CTRL_S;

#define MAX_EX_KEY_MAP_SIZE              8
struct ex_key_map {
    uint32_t crypto_key_no;
    uint32_t ex_mem_entry;
} ;

struct ex_key_map_para {
    uint32_t map_size;
    struct ex_key_map map[MAX_EX_KEY_MAP_SIZE];
} ;

struct crypto_adv_info{
    struct ex_key_map_para ex_key_para;
};

#define ALG_USR_DEF			8
struct FH_EFUSE_PARA{
	uint32_t	mode;
/*if key_flag set efuse...then parse the para below...*/
    struct crypto_adv_info adv;
};

typedef struct FH_CRYPTO_HANDLE
{
	int32_t tfmfd;
	int32_t opfd;
	int8_t *ps8AlgType;
	int8_t *ps8AlgMode;
	int8_t *ps8AlgSrc;
	int8_t *ps8AlgDst;
	int8_t *ps8KeyBuf;
	int8_t *ps8IVBuf;
	uint32_t u32KeyLen;
	uint32_t u32IVLen;
	uint32_t u32TextLen;
	CRYPTO_KEY_SRC_E enKeySrc;
	struct FH_EFUSE_PARA efuse_para;
} CRYPTO_HANDLE;

/*
功能说明
	初始化加密模块设备。
输入参数
	无
输出参数
	无
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
*/
int32_t FH_CRYPTO_Init(void);

/*
功能说明
	去初始化加密模块设备。
输入参数
	无。
输出参数
	无
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
*/
int32_t FH_CRYPTO_DeInit(void);

/*
功能说明
	创建一路加密通道，并获取句柄。
输入参数
	phCipher：加解密通道句柄；
输出参数
	无。
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
*/
int32_t FH_CRYPTO_CreateHandle(CRYPTO_HANDLE *phCipher);

/*
功能说明
	销毁一路加密句柄。
输入参数
	phCipher：加解密通道句柄；
输出参数
	无。
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
*/
int32_t FH_CRYPTO_DestroyHandle(CRYPTO_HANDLE *phCipher);

/*
功能说明
	配置加解密控制信息。详细配置请参见结构体CRYPTO_CTRL_S。
输入参数
	phCipher：加解密通道句柄；
	pstCtrl：控制信息指针。
输出参数
	无。
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
*/
int32_t FH_CRYPTO_ConfigHandle(CRYPTO_HANDLE *phCipher, CRYPTO_CTRL_S* pstCtrl);

/*
功能说明
	烧写 Key 到 OTP 区域。
输入参数
	u32OptId：烧写的 OTP 区域，取值范围[0, 2]；
	pu8Key：待烧写的 Key 数据；
	u32KeyLen：Key 数据长度，最大不超过 16*3，单位是 byte。
输出参数
	无；
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
注意
	调用此接口之前，加解密模块必须已初始化。
	可烧写的 OTP 区域共有 3 个，分别为 0，1，2。
	每个 OTP 区域只能烧写一次，且不能读取。烧写的 Key 长度最大不超过 16*3 个字节。
*/
int32_t FH_CRYPTO_WriteOTPKey(uint32_t u32OptId, const uint8_t *pu8Key, uint32_t u32KeyLen);

/*
功能说明
	烧写 Device ID 到 OTP 区域。
输入参数
	pu8Key：待烧写的Device ID数据；
	u32KeyLen：Key 数据长度，最大不超过 8，单位是 byte。
输出参数
	无；
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
注意
	调用此接口之前，加解密模块必须已初始化。
	可烧写的 OTP 区域共有 1 个，
	OTP 区域只能烧写一次.烧写的Device ID长度最大不超过 8 个字节。
*/
int32_t FH_CRYPTO_WriteOTPDeviceID(const uint8_t *pu8Key, uint32_t u32KeyLen);

/*
功能说明
	OTP 区域 输出Device ID。
输入参数
	pu8Key：待输出的数据buffer；
	u32KeyLen：数据buffer长度，最大不超过 8，单位是 byte。
输出参数
	无；
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
注意
	调用此接口之前，加解密模块必须已初始化。
*/
int32_t FH_CRYPTO_ReadOTPDeviceID(const uint8_t *pu8Key, uint32_t u32KeyLen);

/*
功能说明
	OTP 区域 输出IV。
输入参数
	iv_buf：待输出的数据buffer；
	ivLen：数据buffer长度，4字节对齐。
输出参数
	无；
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
注意
	默认从OTP区域ENTRY 16开始读取数据
	调用此接口之前，加解密模块必须已初始化。
*/
int32_t FH_CRYPTO_ReadOTPIV(const uint8_t *iv_buf, uint32_t ivLen);

/*
功能说明
	OTP 区域 输出OTP连续entry信息。
输入参数
	startEntry: 起始entry序号，4字节对齐
	pu8Buf：待输出的数据buffer；
	pu8BufLen：数据buffer长度，单位是 byte，4字节对齐。
输出参数
	无；
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
注意
	调用此接口之前，加解密模块必须已初始化。
*/
int32_t FH_CRYPTO_ReadOTPEntry(uint32_t startEntry, const uint8_t *pu8Buf, uint32_t pu8BufLen);

/*
功能说明
	对数据进行加密。
输入参数
	phCipher：加解密通道句柄；
	u32SrcPhyAddr：源数据（待加密的数据）的物理地址；
	u32DestPhyAddr：存放加密结果的物理地址；
	u32ByteLength：数据的长度（单位：字节）；
输出参数
	无。
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
注意
	加密通道句柄必须已创建。
	可多次调用。
	数据的长度至少为 16 字节，且不能大于或等于 1024*1024 字节。
*/
int32_t FH_CRYPTO_Encrypt(CRYPTO_HANDLE *phCipher, uint32_t u32SrcPhyAddr,
		uint32_t u32DestPhyAddr, uint32_t u32ByteLength);

/*
功能说明
	对数据进行解密。
输入参数
	phCipher：加解密通道句柄；
	u32SrcPhyAddr：源数据（待解密的数据）的物理地址；
	u32DestPhyAddr：存放解密结果的物理地址；
	u32ByteLength：数据的长度（单位：字节）；
输出参数
	无。
返回值
	0：函数调用成功。
	其他：失败，函数调用失败的原因见出错信息。
注意
	加密通道句柄必须已创建。
	可多次调用。
	数据的长度至少为 16 字节，且不能大于或等于 1024*1024 字节。
*/
int32_t FH_CRYPTO_Decrypt(CRYPTO_HANDLE *phCipher, uint32_t u32SrcPhyAddr,
		uint32_t u32DestPhyAddr, uint32_t u32ByteLength);

/*
功能说明
    EFUSE传输不同的key到AES进行加密。
输入参数
    phCipher：加解密通道句柄；
    u32MapBuf：映射配置参数地址；
    u32MapBufSize：映射配置参数长度；
输出参数
    无。
返回值
    0：函数调用成功。
    其他：失败，函数调用失败的原因见出错信息。
注意
    加密通道句柄必须已创建。
    可多次调用。
    8830 & 8810 & 8630 没有此项功能
示例
    u32MapBuf[0] = 0;
    u32MapBuf[1] = 8;
    u32MapBuf[2] = 4;
    u32MapBuf[3] = 12;
    u32MapBufSize = 4;
    参数意思如下:
    [efuse_entry(0~3)]=>[aes_key(0)]
    [efuse_entry(8~11)]=>[aes_key(1)]
    [efuse_entry(4~7)]=>[aes_key(2)]
    [efuse_entry(12~15)]=>[aes_key(3)]
*/
int32_t FH_CRYPTO_Mapswitch(CRYPTO_HANDLE *phCipher, uint32_t *u32MapBuf, uint32_t u32MapBufSize);
/*
功能说明
    对efuse进行lock，
输入参数
    u32Efuse_LockBits: 对efuse区域进行lock，每一个bit对应一个区域,如下所示。
    bit0 : efuse entry[0~15]
    bit1 : efuse entry[16~31]
    bit2 : efuse entry[32~47]

输出参数
    无。
返回值
    0：函数调用成功。
    其他：失败，函数调用失败的原因见出错信息。
注意
    lock信息也是只能写一次，且不能修改，一定要一次写进去。
示例：
    crypto_efuse_set_lock(3);
    意思就是把efuse的[0 ~ 31]entry lock
*/
int FH_CRYPTO_SetLock(unsigned int u32Efuse_LockBits);

char *FH_CRYPTO_Version(unsigned int print_enable);
#endif /* FH_CRYPTO_API_H_ */
