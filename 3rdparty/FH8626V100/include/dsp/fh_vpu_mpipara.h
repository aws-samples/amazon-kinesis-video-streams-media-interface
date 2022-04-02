#ifndef __FH_VPU_MPIPARA_H__
#define __FH_VPU_MPIPARA_H__
/**|VPU|**/
#include "types/type_def.h"
#include "fh_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef enum
{
	VPU_MODE_ISP            = 0, // ISP输入模式 | [ ]
	VPU_MODE_MEM            = 1, // 内存输入模式 | [ ]
}FH_VPU_VI_MODE;

///VPU 输出模式
typedef enum
{
	VPU_VOMODE_BLK          = 0, // 宏块格式 | [ ]
	VPU_VOMODE_SCAN         = 1, // 光栅格式(NV12) | [ ]
}FH_VPU_VO_MODE;

///YUV格式
typedef enum
{
	PIXEL_FORMAT_NOCHROME   = 0, // 亮度 | [ ]
	PIXEL_FORMAT_420        = 1, // YUV420 | [ ]
	PIXEL_FORMAT_422        = 2, // YUV422 | [ ]
	PIXEL_FORMAT_444        = 3, // YUV444 | [ ]
}FH_VPU_PIXELFORMAT;

typedef struct
{
	FH_SIZE                 vi_size;      // 输入幅面 | [16-最大处理幅面]
}FH_VPU_SIZE;

typedef struct
{
	FH_SIZE                 vpu_chn_size; // 通道输出幅面 | [16-最大处理幅面]
}FH_VPU_CHN_CONFIG;

typedef struct
{
	FH_SIZE                 pic_size;     // 图像幅面 | [ ]
	FH_ADDR                 yluma;        // 图像亮度物理地址 | [ ]
	FH_ADDR                 chroma;       // 图像色度物理地址 | [ ]
	FH_UINT32               ystride;      // 图像亮度行偏移 | [ ]
	FH_UINT32               cstride;      // 图像色度行偏移 | [ ]
}FH_VPU_USER_PIC;

typedef struct
{
	FH_VPU_PIXELFORMAT      pixelFormat;  // 图像格式 | [ ]
	FH_UINT32               stride;       // 图像跨度 | [ ]
	FH_SIZE                 size;         // 图像尺寸 | [ ]
	FH_MEM_INFO             yluma;        // 亮度地址 | [ ]
	FH_MEM_INFO             chroma;       // 色度地址 | [ ]
	FH_UINT64               time_stamp;   // 时间戳 | [ ]
}FH_VPU_STREAM;

typedef struct
{
	FH_SIZE                 pic;          // 幅面 | [ ]
	FH_MEM_INFO             data;         // 数据地址信息 | [ ]
	FH_UINT32               stride;       // 行偏移 | [ ]
	FH_UINT64               time_stamp;   // 时间戳 | [ ]
}FH_PIC_DATA;

typedef struct
{
	FH_UINT32              masaic_enable; // 隐私屏蔽形式 0:单色 1:马赛克 | [0-1]
	FH_UINT32              masaic_size;   // 马赛克单块大小 0:16x16,1:32x32 | [0-1]
}FH_MASK_MASAIC;

typedef struct
{
	FH_BOOL                 mask_enable[MAX_MASK_AREA]; // 隐私屏蔽使能 | [0-1]
	FH_AREA                 area_value[MAX_MASK_AREA];  // 隐私屏蔽区域 | [ ]
	FH_UINT32               color;                      // 隐私显示颜色(格式AYCbCr,Alpha无效) | [ ]
	FH_MASK_MASAIC          masaic;                     // 马赛克属性 | [ ]
}FH_VPU_MASK;

typedef struct
{
	FH_UINT32               alpha_value;// 透明度 | [0-127]
	FH_UINT32               dtvmode;    // 0为SDTV模式，1为HDTV模式 | [0-1]
	FH_UINT32               rgbmode;    // 0为stdio RGB，1为computer RGB | [0-1]
	FH_SIZE                 logo_size;  // 图像叠加覆盖大小 | [ ]
}FH_LOGO_CFG;

typedef struct
{
	FH_UINT32               logo_enable;   // 图片叠加使能 | [0-1]
	FH_UINT8 *              logo_addr;     // 图片叠加数据的物理地址 | [ ]
	FH_LOGO_CFG             logo_cfg;      // 图片叠加配置 | [ ]
	FH_POINT                logo_startpos; // 图片叠加左上角起始点 | [ ]
	FH_POINT                logo_cutpos;   // 图片叠加切割显示起始点 | [ ]
	FH_UINT32               stride_value;  // 行偏移 | [ ]
}FH_VPU_LOGO;

typedef struct
{
	FH_UINT32               magic_color_en; // 是否使用magic_color,仅在16bit模式下有效 | [0-1]
	FH_UINT32               magic_color;    // magic_color的值,16bit,此颜色将作为透明 | [0-0xffff]
	FH_UINT32               global_alpha_en;// 全局透明度使能 | [0-1]
	FH_UINT32               global_alpha;   // 全局透明度 | [0-255]
	FH_UINT32               rgb16_type;     // 位宽16模式时颜色格式:1:ARGB0565,2:ARGB1555,3:ARGB4444 | [1-3]
	FH_UINT32               extmode;        // 0为LSB填0，1为MSB复制到LSB | [0-1]
	FH_UINT32               dtvmode;        // 0为SDTV模式，1为HDTV模式 | [0-1]
	FH_UINT32               rgbmode;        // 0为stdio RGB，1为computer RGB | [0-1]
	FH_SIZE                 logo_size;      // 图像叠加覆盖大小 | [ ]
}FH_LOGOV2_CFG;

typedef struct
{
	FH_UINT32               logo_enable;   // 图片叠加使能 | [ ]
	FH_UINT32               logo_idx;      // 图像叠加索引号 | [见芯片能力定义]
	FH_UINT32               logo_pixdepth; // 图片像素位宽 | [见芯片能力定义]
	FH_UINT8 *              logo_addr;     // 图片叠加数据的物理地址 | [ ]
	FH_LOGOV2_CFG           logo_cfg;      // 图片叠加配置 | [ ]
	FH_POINT                logo_startpos; // 图片叠加起始坐标 | [ ]
	FH_UINT32               stride_value;  // 行偏移 | [ ]
	FH_UINT32               color[256];    // 颜色索引库,ARGB8888 | [ ]
}FH_VPU_LOGOV2;

typedef struct
{
	FH_UINT32               Vamp;      // 垂直硬件放大一倍 | [0-1]
	FH_UINT32               Hamp;      // 水平硬件放大一倍 | [0-1]
	FH_UINT32               txtrotate; // 整个字符旋转 | [0-3]
	FH_UINT32               txtw;      // 字符串宽度 | [0-64]
	FH_UINT32               txth;      // 字符串高度 | []
	FH_UINT32               fontsize;  // 字体占的内存大小 | [ ]
	FH_UINT32               fontw;     // 字体宽度,8对齐 | [8-32]
	FH_UINT32               fonth;     // 字体高度,8对齐 | [8-32]
}FH_OSD_CFG;

typedef struct
{
	FH_UINT32               norcolor;  // 正常时的字符颜色(AYCbCr) | [ ]
	FH_UINT32               invcolor;  // 反色时的字符颜色(AYCbCr) | [ ]
}FH_OSD_COLOR;

typedef struct
{
	FH_UINT32               charcolor;                   // 字符颜色(AYCbCr) | [ ]
	FH_UINT32               edgecolor;                   // 描边颜色(AYCbCr) | [ ]
	FH_UINT32               bkgcolor;                    // 背景颜色(AYCbCr) | [ ]
}FH_OSD_HLCOLOR;

typedef struct
{
	FH_UINT32               invert_enable;               // 反色使能 | [0-1]
	FH_UINT32               text_idex;                   // 字符叠加索引号 | [见芯片能力定义]
	FH_UINT32               invert_word[MAX_INVERT_CNT]; // 反色控制位 | [ ]
}FH_INVERT_CTRL;

typedef struct
{
	FH_UINT32               hl_enable;                   // 高亮使能 | [0-1]
	FH_OSD_HLCOLOR          hl_color;                    // 高亮颜色(AYCbCr) | [ ]
	FH_AREA                 hl_area;                     // 高亮区域 | [ ]
}FH_HIGHLIGHT_WINDOW;

typedef struct
{
	FH_UINT32               text_idex;                   // 字符叠加索引号 | [见芯片能力定义]
	FH_UINT32               edge_enable;                 // 描边使能 | [0-1]
	FH_UINT32               hlext_enable;                // 高亮使能 | [0-1]
	FH_OSD_HLCOLOR          nonhl_color;                 // 高亮颜色 | [ ]
	FH_HIGHLIGHT_WINDOW     hl_window0;                  // 高亮窗口0 | [ ]
	FH_HIGHLIGHT_WINDOW     hl_window1;                  // 高亮窗口1 | [ ]
}FH_HIGHLIGHT_CTRL;

typedef struct
{
	FH_UINT32               idex;                        // 字符叠加索引号 | [见芯片能力定义]
	FH_UINT32               osd_enable;                  // 字符叠加使能 | [0-1]

	FH_UINT8 *              osdtxtaddr;                  // 索引物理地址 | [ ]
	FH_UINT8 *              osdfontaddr;                 // 字库物理地址 | [ ]
	FH_INVERT_CTRL          invert_ctl;                  // 字符反色控制 | [ ]
	FH_OSD_CFG              osd_cfg;                     // 字符叠加配置  | [ ]
	FH_OSD_COLOR            osd_color;                   // 字体颜色(AYCbCr)  | [ ]
	FH_OSD_COLOR            osd_blgcolor;                // 背景颜色(AYCbCr)  | [ ]
	FH_POINT                osd_startpos;                // 字符叠加起始坐标  | [ ]
}FH_VPU_OSD;

typedef struct
{
	FH_UINT32               idex;                        // 字符叠加索引号 | [见芯片能力定义]
	FH_UINT8 *              osdtxtaddr;                  // 索引物理地址 | [ ]
	FH_UINT8 *              osdfontaddr;                 // 字库物理地址 | [ ]
}FH_VPU_OSD_ADDR;

typedef struct
{
	FH_UINT32               frame_id;                    // 时间戳(截短低32位) | [ ]
	FH_ADDR_INFO            ymean;                       // 亮度均值地址 | [ ]
	FH_ADDR_INFO            cmean;                       // 色度均值地址 | [ ]
}FH_VPU_YCMEAN;

typedef struct
{
	FH_AREA                 vpu_crop_area;               //　裁剪区域 | [ ]
}FH_VPU_CROP;

typedef struct
{
	FH_UINT32               base;
	void *                  vbase;
	FH_UINT32               size;
}FH_PKG_INFO;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
