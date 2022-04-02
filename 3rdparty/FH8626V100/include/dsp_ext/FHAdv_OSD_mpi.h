/**
 * @file     FHAdv_OSD_mpi.h
 * @brief    FHAdv OSD moduel interface
 * @version  V1.0.1
 * @date     11-Dec-2018
 * @author   Software Team
 *
 * @note
 * Copyright (C) 2018 Shanghai Fullhan Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * @par
 * Fullhan is supplying this software which provides customers with programming
 * information regarding the products. Fullhan has no responsibility or
 * liability for the use of the software. Fullhan not guarantee the correctness
 * of this software. Fullhan reserves the right to make changes in the software
 * without notification.
 *
 */

#ifndef _FHADV_OSD_MPI_H_
#define _FHADV_OSD_MPI_H_

#include "FH_typedef.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define FHT_OSD_TAG_CODE_BASE   (0xe401) /*TAG字符起始编号*/
#define FHT_OSD_YEAR4           (0xe401) /*4位年：2019*/
#define FHT_OSD_YEAR2           (0xe402) /*2位年：19*/
#define FHT_OSD_MONTH2          (0xe403) /*2位月份：01～12*/
#define FHT_OSD_DAY             (0xe404) /*2位日期：01～31*/
#define FHT_OSD_WEEK3           (0xe405) /*3位星期：MON～SUN*/
#define FHT_OSD_CWEEK1          (0xe406) /*1位星期：日,一～六*/
#define FHT_OSD_HOUR24          (0xe407) /*2位24时制小时：00～23*/
#define FHT_OSD_HOUR12          (0xe408) /*2位12时制小时：01～12*/
#define FHT_OSD_MINUTE          (0xe409) /*2位分钟: 00～59*/
#define FHT_OSD_SECOND          (0xe40a) /*2位秒: 00～59*/
#define FHT_OSD_AMPM            (0xe40b) /*1位(AM/PM中只取A/P): A/P*/
#define FHT_OSD_CAMPM           (0xe40c) /*一位(上午/下午中只取上/下): 上/下*/
#define FHT_OSD_USER1           (0xe40d) /*user defined*/
#define FHT_OSD_USER2           (0xe40e) /*user defined*/
#define FHT_OSD_USER3           (0xe40f) /*user defined*/
#define FHT_OSD_USER4           (0xe410) /*user defined*/
#define FHT_OSD_USER5           (0xe411) /*user defined*/
#define FHT_OSD_USER6           (0xe412) /*user defined*/
#define FHT_OSD_USER7           (0xe413) /*user defined*/
#define FHT_OSD_USER8           (0xe414) /*user defined*/
#define FHT_OSD_USER9           (0xe415) /*user defined*/
#define FHT_OSD_USER10          (0xe416) /*user defined*/
#define FHT_OSD_USER11          (0xe417) /*user defined*/
#define FHT_OSD_USER12          (0xe418) /*user defined*/
#define FHT_OSD_USER13          (0xe419) /*user defined*/
#define FHT_OSD_USER14          (0xe41a) /*user defined*/
#define FHT_OSD_USER15          (0xe41b) /*user defined*/
#define FHT_OSD_USER16          (0xe41c) /*user defined*/
#define FHT_OSD_USER_MAX        (FHT_OSD_USER16)
#define FHT_OSD_TAG_CNT         (28)     /*最大支持28个TAG*/
#define FHT_OSD_MAX_CHARS_OF_TAG (32)    /*一个TAG所能包含的最大字符数*/

#define FHT_UPDATED_YEAR   (1<<0) /*年变化的时候才更新*/
#define FHT_UPDATED_MONTH  (1<<1) /*月变化的时候才更新*/
#define FHT_UPDATED_DAY    (1<<2) /*日变化的时候才更新*/
#define FHT_UPDATED_HOUR   (1<<5) /*时变化的时候才更新*/
#define FHT_UPDATED_MINUTE (1<<6) /*分变化的时候才更新*/
#define FHT_UPDATED_SECOND (1<<7) /*秒变化的时候才更新*/

/*OSD时间格式*/
typedef enum
{
    FHTEN_OSD_TimeFmt0  = 0,      /* YYYY-MM-DD 00:00:00 */
    FHTEN_OSD_TimeFmt1  = 1,      /* MM-DD-YYYY 00:00:00 */
    FHTEN_OSD_TimeFmt2  = 2,      /* DD-MM-YYYY 00:00:00 */
    FHTEN_OSD_TimeFmt3  = 3,      /* YYYY/MM/DD 00:00:00 */
    FHTEN_OSD_TimeFmt4  = 4,      /* MM/DD/YYYY 00:00:00 */
    FHTEN_OSD_TimeFmt5  = 5,      /* DD/MM/YYYY 00:00:00 */
    FHTEN_OSD_TimeFmt6  = 6,      /* YYYY-MM-DD 00:00:00 weekFlag 打开时显示中文星期 */
    FHTEN_OSD_TimeFmt7  = 7,      /* MM-DD-YYYY 00:00:00 weekFlag 打开时显示中文星期 */
    FHTEN_OSD_TimeFmt8  = 8,      /* DD-MM-YYYY 00:00:00 weekFlag 打开时显示中文星期 */
    FHTEN_OSD_TimeFmt9  = 9,      /* xxxx年xx月xx日 00:00:00 */
    FHTEN_OSD_TimeFmt10 = 10,    /* xx月xx日xxxx年 00:00:00 */
    FHTEN_OSD_TimeFmt11 = 11,    /* xx日xx月xxxx年 00:00:00 */
    FHTEN_OSD_TimeFmt12 = 12,     /* YYYY/MM/DD 00:00:00 显示中文星期 */
    FHTEN_OSD_TimeFmt13 = 13,     /* MM/DD/YYYY 00:00:00 显示中文星期 */
    FHTEN_OSD_TimeFmt14 = 14,     /* DD/MM/YYYY 00:00:00 显示中文星期 */
    FHTEN_OSD_TimeFmt_BUTT,      /* 判断限值 非交互项 上层不必关心 */
} FHT_OSD_TimeFmt_e;

typedef enum
{
    FHTEN_OSD_GB2312  = 0, /* gb2312 */
    FHTEN_OSD_UNICODE = 1, /* unicode(4字节) */
    FHTEN_OSD_UTF8    = 2, /* utf-8 */
}FHT_OSD_EncType_e;        /* 自定义字库类型枚举 */

typedef struct
{
    FH_SINT32 width;    /* 汉字字符宽度像素， 取值范围[16, 32, 48, 64] */
    FH_SINT32 height;   /* 汉字字符宽度像素， 取值范围[16, 32, 48, 64] */
} FHT_OSD_Size_t;

typedef struct
{
    FH_UINT8 high_level;                           /* 大于等于此值，进行反色 */
    FH_UINT8 low_level;                            /* 小于等于此值，恢复正常颜色 */
}FHT_OSD_Invert_Attr_t;

typedef struct
{
    FH_SINT16 year;  /*The real year, for example, now is year 2019*/
    FH_SINT16 month; /*The number of months since January,in the range 1 to 12*/
    FH_SINT16 day;  /*The day of the month, in the range 1 to 31*/
    FH_SINT16 dayOfWeek; /*The number of days since Sunday, in the range 0 to 6*/
    FH_SINT16 hour; /*The number of hours past midnight, in the range 0 to 23*/
    FH_SINT16 minute;/*The number of minutes after the hour, in the range 0 to 59*/
    FH_SINT16 second;/*The number of seconds after the minute, in the range 0 to 59*/
} FHT_DATE_TIME;

/*字库配置*/
typedef struct
{
    FH_UINT32 libSize;    /* 字库大小 */
    FH_ADDR   pLibData;  /* 字库数据地址 */
} FHT_OSD_FontLib_t;

enum
{
    FH_OSD_INVERT_DISABLE  = 0,
    FH_OSD_INVERT_BY_CHAR = 1,
    FH_OSD_INVERT_BY_LINE = 2,
};

#define	FHT_OSD_GRAPH_CTRL_DEFAULT (0)
#define	FHT_OSD_GRAPH_CTRL_LOGO_BEFORE_VP (1<<0)
#define	FHT_OSD_GRAPH_CTRL_TOSD_BEFORE_VP (1<<1)
#define	FHT_OSD_GRAPH_CTRL_GBOX_BEFORE_VP (1<<2)
#define	FHT_OSD_GRAPH_CTRL_MASK_BEFORE_VP (1<<3)
#define	FHT_OSD_GRAPH_CTRL_LOGO_AFTER_VP (1<<16)
#define	FHT_OSD_GRAPH_CTRL_TOSD_AFTER_VP (1<<17)
#define	FHT_OSD_GRAPH_CTRL_GBOX_AFTER_VP (1<<18)
#define	FHT_OSD_GRAPH_CTRL_MASK_AFTER_VP (1<<19)

#define FHT_OSD_DEBUG_LEVEL_OFF   (0) /*关闭debug功能*/
#define FHT_OSD_DEBUG_LEVEL_ERROR (1) /*打印错误信息*/
#define FHT_OSD_DEBUG_LEVEL_ALL   (7) /*打印所有相关信息*/


#define FHT_OSD_TIME_BIT_HOUR12 (1<<0)
#define FHT_OSD_TIME_BIT_WEEK   (1<<1)
#define FHT_OSD_TIME_BIT_AFTER  (1<<2)
typedef struct {
	FH_CHAR*  textInfo;
	FH_UINT8  textEnable;

    FH_UINT8  timeOsdEnable;
    /*
     * timeOsdFlags的取值为这几个宏定义的bit or值, FHT_OSD_TIME_BIT_XXX
     * bit0:12小时制[注：0:24小时制 1:12小时制]，bit1:显示星期，bit2:时间OSD在后面
     */
    FH_UINT8  timeOsdFlags;
    FH_UINT8  timeOsdFormat;

    FH_UINT16 textLineWidth; /*本次一行显示的最大像素宽度,超过时自动换行*/
    FH_UINT16 reserved;

	FH_UINT16 textPositionX;
	FH_UINT16 textPositionY;
	FH_UINT16 textMaxWidth;  /*整个行块显示的像素最大宽度*/
	FH_UINT16 textMaxHeight; /*整个行块显示的像素最大高度*/
}FHT_OSD_TextLine_t;


/*OSD配置*/
typedef struct
{
    FHT_OSD_Size_t        osdSize;                 /* 字符大小 */
    FHT_RgbColor_t        normalColor;               /* 钩边色 支持反色 */
    FHT_RgbColor_t        invertColor;                    /* 钩边色 支持反色 */
    FHT_RgbColor_t        edgeColor;                    /* 钩边色 */
    FHT_RgbColor_t        bkgColor;                     /* 背景色*/

    FH_UINT8              osdRotate;                    /* OSD旋转角度， 0: 不旋转  1: 90度  2: 180度  3: 270度*/
    FH_UINT8              edgePixel;                    /* 镶边像素，0: 不镶边，1：1个像素， 2：两个像素。*/
	FH_UINT8              osdInvertEnable;              /*FH_OSD_INVERT_XXX*/
    FHT_OSD_Invert_Attr_t osdInvertThreshold;           /* 设置osd反色阈值 */

	FH_UINT8              nTextLineNum;
	FHT_OSD_TextLine_t   *pTextLineInfo;
} FHT_OSD_CONFIG_t;


typedef FH_UINT8 FHT_OSD_Layer_Type_t; /* 此类型取值为 FHT_OSD_LAYER_TYPE_XXX */

/*logo配置*/
/*logo配置*/
typedef struct
{
    FH_UINT32  enable;                      /* logo使能 */
    FH_UINT32  rotate;                      /* logo旋转角度 */
    FH_UINT32  maxW;                        /* logo最大宽度 */
    FH_UINT32  maxH;                        /* logo最大高度 */
    FH_UINT32  alpha;                       /* logo透明度 */
    FHT_Rect_t area;                        /* logo位置及大小 */
    FH_ADDR    pData;                       /* logo数据 */
} FHT_OSD_Logo_t;

typedef struct
{
    FH_UINT32  enable;                      /* logo使能 */
    FH_UINT32  rotate;                      /* logo旋转角度 */
    FH_UINT32  maxW;                        /* logo最大宽度 */
    FH_UINT32  maxH;                        /* logo最大高度 */
    FH_UINT32  alpha;                       /* logo透明度 */
    FHT_Rect_t area;                        /* logo位置及大小 */
    FH_ADDR    pData;                       /* logo数据 */
    FH_UINT32  autoInvertEn;                /* logo自动反色使能 */
    FH_ADDR    pDataInvert;                 /* 反色logo数据 */
    FHT_OSD_Invert_Attr_t logoInvThreshold; /* 设置osd反色阈值 */
} FHT_OSD_Logo_Invert_t;

/*mask配置*/
typedef struct
{
    FH_UINT32      enable;   /* mask使能 0: 关闭  1: 开启 */
    FH_UINT32      maskId;   /* mask id */
    FH_UINT32      rotate;   /*旋转控制,目前无效,填0即可*/
    FHT_Rect_t     area;     /* OSD位置及大小 */
    FHT_RgbColor_t osdColor; /* OSD颜色 */
} FHT_OSD_Mask_t;

/*gbox配置*/
typedef struct
{
    FH_UINT32      enable;        /* gbox使能 0: 关闭  1: 开启 */
    FH_UINT32      gboxId;        /* gbox id */
    FH_UINT32      rotate;        /*旋转控制,目前无效,填0即可*/
    FH_UINT32      gboxLineWidth; /* gbox线条宽度 */
    FHT_Rect_t     area;          /* gbox位置及大小 */
    FHT_RgbColor_t osdColor;      /* gbox颜色 */
} FHT_OSD_Gbox_t;


/*
 *函数指针类型定义
 *参数:
 *   code[IN], 字符的ASC/GB2312/Unicode码
 *   pWidth[OUT], 用来接收此字符的像素点阵宽度
 *   pHeight[OUT], 用来接收此字符的像素点阵高度
 *返回值: 点阵像素的内存,当次有效,ADVAPI库内部会马上拷贝走.
 *        注意,点阵像素的内存不可使用函数内部的stack内存,因为函数返回会马上无效,最好使用一段static定义的内存.
 */
typedef FH_UINT8* (* FHT_OSD_GET_FONT_FUNC_t)(FH_UINT32 code, FH_UINT32 *pWidth, FH_UINT32 *pHeight);


/*
 * 参数:
 *   debug_level: 取值FHT_OSD_DEBUG_LEVEL_XXX
 *    graph_ctrl: 取值FHT_OSD_GRAPH_CTRL_XXX，一般取FHT_OSD_GRAPH_CTRL_DEFAULT即可
 *    reserved1:  保留值,填0即可
 *    reserved2:  保留值,填0即可
 */
FH_SINT32 FHAdv_Osd_Init(FH_UINT32 debug_level, FH_UINT32 graph_ctrl, FH_UINT32 reserved1, FH_UINT32 reserved2);


/*********
 * 函数名：  FHAdv_OSD_UnInit
 * 函数功能: uninit.
 * 输入参数: void
 * 输出参数: 无
 * return:  FH_SUCCESS
 *          FH_FAILURE
 *
 *********/
FH_SINT32 FHAdv_Osd_UnInit(FH_VOID);

/*
 *功能: 加载字库
 *参数:
 *   type,本次加载字库的类型
 *   pFontData,结构体包含字库点阵数据大小及指针,需要注意的是,此结构体中pLibData指向的
 *   内存在FHAdv_Osd_UnLoadFontLib被调用前不可释放,因为我们的ADVAPI库里面不做内存拷贝.
 *返回值: 见错误码ADV_ERR_XXX
 */
FH_SINT32 FHAdv_Osd_LoadFontLib(FHT_OSD_FontType_e type, FHT_OSD_FontLib_t *pFontData);

/*
 *功能: 卸载字库,与FHAdv_Osd_LoadFontLib对应,需要注意的是,此函数调用后,ADVAPI内部将不再
 *      引用之前加载的字库.
 *参数: type,本次卸载字库的类型
 *返回值: 见错误码ADV_ERR_XXX
 */
FH_SINT32 FHAdv_Osd_UnLoadFontLib(FHT_OSD_FontType_e type);

/*
 *功能: 释放当前Cache的所有Font,实现一些特殊功能,一般来说,这个函数不需要调用.
 *      注意,要调用此函数,需要先Disable所有的TOSD,否则它会失败.
 *返回值: 见错误码ADV_ERR_XXX
 */
FH_SINT32 FHAdv_Osd_InvalidateFont(FH_VOID);

/*
 *功能: 注册自定义字库回调函数
 *参数:
 *   func, 回调函数, 当func为NULL时,ADVAPI库内部将不再调用上一次注册的回调函数.
 *   enc_type, 结构体FHT_OSD_TextLine_t中textInfo指向字符的编码类型.
 *返回值: 见错误码ADV_ERR_XXX
 */
FH_SINT32 FHAdv_Osd_Font_RegisterCallback(FHT_OSD_GET_FONT_FUNC_t func, FHT_OSD_EncType_e enc_type);


typedef FH_UINT32 (* FHT_OSD_Traverse_Chars_FUNC_t)(FH_UINT32 tagCode, FH_UINT32 nSequence);
typedef FH_UINT32 (* FHT_OSD_Refresh_Chars_FUNC_t)(FH_UINT32 tagCode, FHT_DATE_TIME *time, FH_UINT32 *pCodeArray);
FH_SINT32 FHAdv_Osd_SetTagCallback(
        FH_UINT32 tagCode,
        FH_UINT32 refreshControl, /*bit or of these flags, FHT_UPDATED_XXX*/
        FHT_OSD_Traverse_Chars_FUNC_t funcTraverse,
        FHT_OSD_Refresh_Chars_FUNC_t funcRefresh);

/*
 *功能: 设置OSD字符，参数为OSD配置结构体指针.
 *参数: chn,通道号,如果TOSD被安排在VPU分通道前,那么chn填0即可,如果TOSD被安排在VPU分通道后,那么
 *          对于主码流,其chn等于0; 对于子码流,其chn等于1
 *      pOsdcfg, TEXT OSD配置参数
 *返回值: 见错误码ADV_ERR_XXX
 */
FH_SINT32 FHAdv_Osd_Ex_SetText(FH_UINT32 chn, FHT_OSD_CONFIG_t *pOsdcfg);

/*
 *功能: 等效于参数chn0为0的FHAdv_Osd_Ex_SetText调用
 *参数: pOsdcfg, TEXT OSD配置参数
 *返回值: 见错误码ADV_ERR_XXX
 */
FH_SINT32 FHAdv_Osd_SetText(FHT_OSD_CONFIG_t *pOsdcfg);

/********
 * 函数名：  FHAdv_Osd_SetLogo
 * 函数功能: 设置Grahp logo.
 * 输入参数: pLogoCfg  logo配置信息
 * 输出参数: 无
 * return:  FH_SUCCESS
 *          FH_FAILURE
 *
 *******/
FH_SINT32 FHAdv_Osd_SetLogo(FHT_OSD_Logo_t *pLogoCfg);

FH_SINT32 FHAdv_Osd_Ex_SetLogo(FH_UINT32 chn, FHT_OSD_Logo_t *pLogoCfg);

/********
 * 函数名：  FHAdv_Osd_SetLogo_Invert
 * 函数功能: 设置Grahp logo, 带有自动反色配置.
 * 输入参数: pLogoCfg  logo配置信息
 * 输出参数: 无
 * return:  FH_SUCCESS
 *          FH_FAILURE
 *
 *******/
FH_SINT32 FHAdv_Osd_SetLogo_Invert(FHT_OSD_Logo_Invert_t *pLogoCfg);
FH_SINT32 FHAdv_Osd_Ex_SetLogo_Invert(FH_UINT32 chn, FHT_OSD_Logo_Invert_t *pLogoCfg);

/*********
 * 函数名：  FHAdv_Osd_Ex_SetMask
 * 函数功能: 打开、关闭通道马赛克
 * 输入参数: chan     vpu通道
 *          pBoxcfg  box配置信息
 * 输出参数: 无
 * return:  FH_SUCCESS
 *          FH_FAILURE
 *
 ********/
FH_SINT32 FHAdv_Osd_Ex_SetMask(FH_UINT32 chan, FHT_OSD_Mask_t *pMaskcfg);

/*********
 * 函数名：  FHAdv_OSD_SetMask
 * 函数功能: 打开、关闭马赛克
 * 输入参数: pBoxcfg  box配置信息
 * 输出参数: 无
 * return:  FH_SUCCESS
 *          FH_FAILURE
 *
 ********/
FH_SINT32 FHAdv_Osd_SetMask(FHT_OSD_Mask_t *pMaskcfg);

/********
 * 函数名：  FHAdv_Osd_Ex_SetGbox
 * 函数功能: 打开、关闭矩形框
 * 输入参数: chan      vpu通道
 *          pBoxcfg   BOX配置信息
 * 输出参数: 无
 * return:  FH_SUCCESS
 *          FH_FAILURE
 *
 ********/
FH_SINT32 FHAdv_Osd_Ex_SetGbox(FH_UINT32 chn, FHT_OSD_Gbox_t *pBoxcfg);

/********
 * 函数名：  FHAdv_OSD_SetGbox
 * 函数功能: 打开、关闭矩形框
 * 输入参数: pBoxcfg   BOX配置信息
 * 输出参数: 无
 * return:  FH_SUCCESS
 *          FH_FAILURE
 *
 ********/
FH_SINT32 FHAdv_Osd_SetGbox(FHT_OSD_Gbox_t *pBoxcfg);

/********
 * 函数名：  FH_ADV_OSD_Version
 * 函数功能: 获取打印库版本信息
 * 输入参数: print_enable 使能版本打印
 * 输出参数: 无
 * return:  FH_SUCCESS
 *          FH_FAILURE
 *
 ********/
FH_CHAR* FH_ADV_OSD_Version(FH_UINT32 print_enable);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
