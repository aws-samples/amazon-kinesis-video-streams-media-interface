#include "isp/isp_enum.h"
#include "dsp/fh_system_mpi.h"
#include "dsp/fh_vpu_mpi.h"
#include "dsp/fh_venc_mpi.h"
#include "sample_common.h"
#include "sample_overlay.h"

#if defined(CONFIG_ARCH_FH8626V100) || defined(CONFIG_ARCH_YG) || \
    defined(CONFIG_ARCH_FH885xV200) || defined(CONFIG_ARCH_FH865x)

#define OSD_FONT_DISP_SIZE (32)

#include "font_array.h"
#include "logo_array.h"

#if defined(CONFIG_ARCH_FH8626V100)
/* FH8626V100 共有9个GOSD图层，通道前1个，通道0和通道1各4个 */
#define SAMPLE_LOGO_GRAPH  FHT_OSD_GRAPH_CTRL_LOGO_BEFORE_VP  /* 设置LOGO在VPU分通道前的GOSD层 */
#define SAMPLE_TOSD_GRAPH  FHT_OSD_GRAPH_CTRL_TOSD_AFTER_VP   /* 设置TOSD在VPU分通道后的GOSD层 */
#define SAMPLE_GBOX_GRAPH  FHT_OSD_GRAPH_CTRL_GBOX_AFTER_VP   /* 设置GBOX在VPU分通道后的GOSD层 */
#define SAMPLE_MASK_GRAPH  FHT_OSD_GRAPH_CTRL_MASK_AFTER_VP   /* 设置MASK在VPU分通道后的GOSD层 */
#elif defined(CONFIG_ARCH_YG)
/* YG 共有24个GOSD图层，平均分布于VPU通道0、通道1和通道3，通道2(AI VO)无GOSD */
#define SAMPLE_LOGO_GRAPH  FHT_OSD_GRAPH_CTRL_LOGO_AFTER_VP   /* 设置LOGO在VPU分通道前的GOSD层 */
#define SAMPLE_TOSD_GRAPH  FHT_OSD_GRAPH_CTRL_TOSD_AFTER_VP   /* 设置TOSD在VPU分通道后的GOSD层 */
#define SAMPLE_GBOX_GRAPH  FHT_OSD_GRAPH_CTRL_GBOX_AFTER_VP   /* 设置GBOX在VPU分通道后的GOSD层 */
#define SAMPLE_MASK_GRAPH  FHT_OSD_GRAPH_CTRL_MASK_AFTER_VP   /* 设置MASK在VPU分通道后的GOSD层 */
#elif defined(CONFIG_ARCH_FH885xV200)
/* FH885xV200共有20个GOSD图层，通道前2个，三个通道上各6个 */
#define SAMPLE_LOGO_GRAPH  FHT_OSD_GRAPH_CTRL_LOGO_BEFORE_VP  /* 设置LOGO在VPU分通道前的GOSD层 */
#define SAMPLE_TOSD_GRAPH  FHT_OSD_GRAPH_CTRL_TOSD_AFTER_VP  /* 设置TOSD在VPU分通道后的GOSD层 */
#define SAMPLE_GBOX_GRAPH  FHT_OSD_GRAPH_CTRL_GBOX_AFTER_VP  /* 设置GBOX在VPU分通道后的GOSD层 */
#define SAMPLE_MASK_GRAPH  FHT_OSD_GRAPH_CTRL_MASK_AFTER_VP  /* 设置MASK在VPU分通道后的GOSD层 */
#elif defined(CONFIG_ARCH_FH865x)
/* FH865x有20个GOSD图层，通道前2个，三个通道上各6个 */
#define SAMPLE_LOGO_GRAPH  FHT_OSD_GRAPH_CTRL_LOGO_BEFORE_VP  /* 设置LOGO在VPU分通道前的GOSD层 */
#define SAMPLE_TOSD_GRAPH  FHT_OSD_GRAPH_CTRL_TOSD_AFTER_VP  /* 设置TOSD在VPU分通道后的GOSD层 */
#define SAMPLE_GBOX_GRAPH  FHT_OSD_GRAPH_CTRL_GBOX_AFTER_VP  /* 设置GBOX在VPU分通道后的GOSD层 */
#define SAMPLE_MASK_GRAPH  FHT_OSD_GRAPH_CTRL_MASK_AFTER_VP  /* 设置MASK在VPU分通道后的GOSD层 */
#else
#error "Please select correct chip!\n"
#endif /* CONFIG_ARCH_FH8626V100 */

static FH_UINT8 user_tag_codes[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'k', 'b', 'p', 's' /* 0 - 9, kpbs*/
}; /* 设置FHT_OSD_USER1对应的字符集合 */

/* USER_TAG回调函数，作用为告诉ADVAPI库，自定义字符会用到的所有字符*/
static FH_UINT32 sample_osd_char_traverse_callback(FH_UINT32 tag_code, FH_UINT32 sequence)
{
    if (sequence >= sizeof(user_tag_codes))
        return 0; /*如果sequence大于等于集合字符数,表示枚举结束,返回0*/

    return (FH_UINT32)(user_tag_codes[sequence]); /* 返回字符集合中第sequence个字符的编码 */
}

/* 把8位的ascii字符转换为32位 */
static FH_VOID stringformchartoint(FH_UINT32 *code, FH_CHAR *s,FH_UINT32 cnt)
{
    FH_SINT32 i;

    for (i = 0; i < cnt; i++)
    {
        code[i] = (FH_UINT32)((FH_UINT8)(s[i]));
    }
}

/* 自定义字符回调函数，自动刷新FHT_OSD_USER1对应的字符内容，显示实时码率，调用频率和时间刷新频率相同 */
static FH_UINT32 sample_osd_char_refresh_callback(FH_UINT32 tagCode, FHT_DATE_TIME *time, FH_UINT32 *pCodeArray)
{
    FH_SINT32 ret;
    FH_UINT32 cnt;
    FH_SINT32 channel;
    FH_CHN_STATUS chnstat;
    FH_CHAR data[FHT_OSD_MAX_CHARS_OF_TAG];

    channel = tagCode - FHT_OSD_USER1; /*每个通道使用不同的TAG,通过TAG获得对应的channel*/
    ret = FH_VENC_GetChnStatus(channel, &chnstat);
    if (ret != RETURN_OK)
    {
        chnstat.bps = 0;
    }
    cnt = snprintf(data, sizeof(data), "%5d kbps", chnstat.bps / 1000);
    stringformchartoint(pCodeArray, data, cnt);

    /*
     * 注意: 1) 每次调用该函数的返回值必须保持一致,并且不能大于FHT_OSD_MAX_CHARS_OF_TAG,
     *       2) 最多向数组pCodeArray中写入FHT_OSD_MAX_CHARS_OF_TAG个FH_UINT32的值,否则会出现堆栈溢出等问题
     */
    return cnt;
}

/* 加载字库 */
static FH_SINT32 load_font_lib(FHT_OSD_FontType_e type, FH_UINT8 *font_array, FH_SINT32 font_array_size)
{
    FH_SINT32 ret;
    FHT_OSD_FontLib_t font_lib;

    font_lib.pLibData = font_array;
    font_lib.libSize  = font_array_size;
    ret = FHAdv_Osd_LoadFontLib(type, &font_lib);
    if (ret != 0)
    {
        printf("Error: FHAdv_Osd_LoadFontLib failed, ret=%d\n", ret);
        return ret;
    }

    return 0;
}

static FH_SINT32 sample_osd_init(FH_VOID)
{
    FH_SINT32 ret;
    FH_UINT32 graph_ctrl = 0;

#ifdef FH_APP_OVERLAY_LOGO
    graph_ctrl |= SAMPLE_LOGO_GRAPH;
#endif

#ifdef FH_APP_OVERLAY_TOSD
    graph_ctrl |= SAMPLE_TOSD_GRAPH;
#endif

    graph_ctrl |= SAMPLE_GBOX_GRAPH;

#ifdef FH_APP_OVERLAY_MASK
    graph_ctrl |= SAMPLE_MASK_GRAPH;
#endif

    /* 初始化OSD */
    ret = FHAdv_Osd_Init(FHT_OSD_DEBUG_LEVEL_ERROR, graph_ctrl, 0, 0);
    if (ret != FH_SUCCESS)
    {
        printf("FHAdv_Osd_Init failed with %x\n", ret);
        return ret;
    }

    return 0;
}

static FH_SINT32 sample_set_textline_use_user_tag(FH_SINT32 channel, FHT_OSD_TextLine_t *text_line)
{
    FH_CHAR user_tag_data[] = {
        0xe4, 0x0d+channel, /*FHT_OSD_USER1,此处用于显示当前码率*/
        0x0a,               /*换行*/
        0xe4, 0x01, /*FHT_OSD_YEAR4, 4位年,比如2019*/
        '-',
        0xe4, 0x03, /*FHT_OSD_MONTH2, 2位月份,取值01～12*/
        '-',
        0xe4, 0x04, /*FHT_OSD_DAY, 2位日期,取值01～31*/
        0x20,       /*空格*/
        0xe4, 0x07, /*FHT_OSD_HOUR24, 24时制小时,取值00～23*/
        ':',
        0xe4, 0x09, /*FHT_OSD_MINUTE, 2位分钟,取值00～59*/
        ':',
        0xe4, 0x0a, /*FHT_OSD_SECOND, 2位秒,取值00～59*/
        0,          /*null terminated string*/
    };

    sprintf(text_line->textInfo, "Channel-%d ", channel);
    strcat(text_line->textInfo, user_tag_data);
    text_line->textEnable    = 1;                          /* 使能自定义text */
    text_line->timeOsdEnable = 0;                          /* 去使能时间显示 */
    text_line->textLineWidth = (OSD_FONT_DISP_SIZE/2) * 36;/* 每行最多显示36个像素宽度为16的字符 */
    text_line->textPositionX = 0;                          /* 左上角起始点宽度像素位置 */
    text_line->textPositionY = 0;                          /* 左上角起始点高度像素位置 */
    text_line->textMaxWidth  = (OSD_FONT_DISP_SIZE/2) * 36;/* 使用过程中该行块的最大像素宽度 最大字符宽度 * 最多字符数，仅第一次有效 */
    text_line->textMaxHeight = OSD_FONT_DISP_SIZE * 2;     /* 使用过程中该行块的最大像素高度 最大字符高度 * 最大字符行数，仅第一次有效 */

    return 0;
}

static FH_SINT32 sample_set_textline_use_time_format(FH_SINT32 channel, FHT_OSD_TextLine_t *text_line)
{
    FH_CHAR user_tag_data[] = {
        0xe4, 0x0d+channel, /*FHT_OSD_USER1,这里用来显示码率*/
        0x0a,               /*换行*/
        0,                  /*null terminated string*/
    };

    sprintf(text_line->textInfo, "Channel-%d ", channel);
    strcat(text_line->textInfo, user_tag_data);
    text_line->textEnable    = 1;             /* 使能自定义text */
    text_line->timeOsdEnable = 1;             /* 使能时间显示 */

    /* 12小时制式、显示星期、时间在自定义字符后面显示 */
    text_line->timeOsdFlags  = FHT_OSD_TIME_BIT_HOUR12 | FHT_OSD_TIME_BIT_WEEK | FHT_OSD_TIME_BIT_AFTER;

    text_line->timeOsdFormat = FHTEN_OSD_TimeFmt0;         /* 使用时间格式FHTEN_OSD_TimeFmt0  YYYY-MM-DD 00:00:00  */

    text_line->textLineWidth = (OSD_FONT_DISP_SIZE/2) * 36;/* 每行最多显示36个像素宽度为16的字符, 超过后自动换行 */
    text_line->textPositionX = 0;                          /* 左上角起始点水平方向像素位置 */
    text_line->textPositionY = 0;                          /* 左上角起始点垂直方向像素位置 */
    text_line->textMaxWidth  = (OSD_FONT_DISP_SIZE/2) * 36;/* 使用过程中该行块的最大像素宽度 最大字符宽度 * 一行最多字符数，仅第一次有效 */
    text_line->textMaxHeight = OSD_FONT_DISP_SIZE * 2;     /* 使用过程中该行块的最大像素高度 最大字符高度 * 最大字符行数，仅第一次有效 */

    return 0;
}

static FH_SINT32 sample_set_channel_tosd(FH_SINT32 channel)
{
    FH_SINT32 ret;
    FHT_OSD_CONFIG_t osd_cfg;
    FHT_OSD_TextLine_t text_line_cfg[1];
    FH_CHAR text_data[128]; /*it should be enough*/
    FH_SINT32 user_defined_time = 1;

    memset(&osd_cfg, 0, sizeof(osd_cfg));
    memset(&text_line_cfg[0], 0, sizeof(text_line_cfg));

    /* 设置字符大小,像素单位 */
    osd_cfg.osdSize.width     = OSD_FONT_DISP_SIZE;
    osd_cfg.osdSize.height    = OSD_FONT_DISP_SIZE;

    /* 设置字符颜色为白色 */
    osd_cfg.normalColor.fAlpha = 255;
    osd_cfg.normalColor.fRed   = 255;
    osd_cfg.normalColor.fGreen = 255;
    osd_cfg.normalColor.fBlue  = 255;

    /* 设置字符反色颜色为黑色 */
    osd_cfg.invertColor.fAlpha = 255;
    osd_cfg.invertColor.fRed   = 0;
    osd_cfg.invertColor.fGreen = 0;
    osd_cfg.invertColor.fBlue  = 0;

    /* 设置字符钩边颜色为黑色 */
    osd_cfg.edgeColor.fAlpha = 255;
    osd_cfg.edgeColor.fRed   = 0;
    osd_cfg.edgeColor.fGreen = 0;
    osd_cfg.edgeColor.fBlue  = 0;

    /* 不显示背景 */
    osd_cfg.bkgColor.fAlpha = 0;

    /* 不旋转 */
    osd_cfg.osdRotate        = 0;

    /* 钩边像素为1 */
    osd_cfg.edgePixel        = 1;

    /* 反色控制 */
    osd_cfg.osdInvertEnable  = FH_OSD_INVERT_DISABLE; /*disable反色功能*/
    /*osd_cfg.osdInvertEnable  = FH_OSD_INVERT_BY_CHAR;*/ /*以字符为单位进行反色控制*/
    /*osd_cfg.osdInvertEnable  = FH_OSD_INVERT_BY_LINE;*/ /*以行块为单位进行反色控制*/
    osd_cfg.osdInvertThreshold.high_level = 200;
    osd_cfg.osdInvertThreshold.low_level  = 160;


    text_line_cfg[0].textInfo = text_data;
    if (!user_defined_time)
    {
        /* 使用ADVAPI定义的时间格式 */
        sample_set_textline_use_time_format(channel, &text_line_cfg[0]);
    }
    else
    {
        /* 使用时间符号设置自定义时间格式*/
        sample_set_textline_use_user_tag(channel, &text_line_cfg[0]);
    }

    osd_cfg.pTextLineInfo = text_line_cfg;

    /* 设置text行块个数 */
    osd_cfg.nTextLineNum     = 1; /*我们的demo中只演示了一个行块*/

    ret = FHAdv_Osd_Ex_SetText(channel, &osd_cfg);
    if (ret != FH_SUCCESS)
    {
        if (channel == 0) /*TOSD可能位于分通道前,这样channel 1可能没有,因此就不打印出错*/
        {
            printf("FHAdv_Osd_Ex_SetText failed with %d\n", ret);
        }
        return ret;
    }

    return 0;
}

/* 获取汉字在字库中的地址偏移 */
static FH_UINT32 GET_HZ_OFFSET(FH_UINT32 x)
{
    // GB2312
    return ((94 * ((x >> 8) - 0xa0 - 1) + ((x & 0xff) - 0xa0 - 1)) * 32);
}

/* 自定义字库回调函数，主要功能用于返回字符code对应的点阵地址及点阵的宽高 */
/* 注意: 返回的字符点阵地址最好为静态地址，保证在advapi拷贝完整正确的点阵数据前，这一地址上的数据不被修改 */
/* advapi内部会缓存所有设置过的字符点阵，如果确认后续不会再使用的新的字符，可以在设置结束后释放字库资源 */
FH_UINT8* get_font_addr(FH_UINT32 code, FH_UINT32 *pWidth, FH_UINT32 *pHeight)
{
    if (code < 0xff)
    {
        *pWidth = 8;   /* 该自定义字库中每个asc字符的点阵宽度为8个像素点 */
        *pHeight = 16; /* 该自定义字库中每个asc字符的点阵高度为16个像素点 */
        return asc16 + 16 * code; /* 返回字符code对应的点阵存储地址 */
    }
    else
    {
       *pWidth = 16;    /* 该自定义字库中每个中文字符的点阵宽度为16个像素点 */
       *pHeight = 16;   /* 该自定义字库中每个中文字符的点阵高度为16个像素点 */
       return gb2312 + GET_HZ_OFFSET(code); /* 返回字符code对应的点阵存储地址 */
    }
}

static FH_SINT32 sample_set_tosd(FH_VOID)
{
    FH_SINT32 ret;
    FH_UINT32 use_ext_font_lib = 0;
    struct channel_info chinfo;

    if (use_ext_font_lib)
    {
        /* 注册自定义字库回调函数，此处设置编码为GB2312, 其他编码方式使用方法相同 */
        ret = FHAdv_Osd_Font_RegisterCallback(get_font_addr, FHTEN_OSD_GB2312);
        if (ret != 0)
        {
            printf("FHAdv_Osd_Font_RegisterCallback failed with %d\n", ret);
        }
    }
    else
    {
        /* 加载asc字库 */
        ret = load_font_lib(FHEN_FONT_TYPE_ASC, asc16, sizeof(asc16));
        if (ret != 0)
        {
            printf("Load ASC font lib failed with %d\n", ret);
        }

        /* 加载gb2312字库 */
        ret = load_font_lib(FHEN_FONT_TYPE_CHINESE, gb2312, sizeof(gb2312));
        if (ret != 0)
        {
            printf("Load CHINESE font lib failed with %d\n", ret);
        }
    }

    /* 注册FHT_OSD_USER1的回调函数，其他用户自定义字符和FHT_OSD_USER1设置方式相同 */
    /* 第二个参数设置该自定义字符的刷新频率,这里为设置为每秒刷新一次*/
    /* 最多注册16个自定义字符 */
    /* 所有使用到的user TAG要先通过函数FHAdv_Osd_SetTagCallback注册好,否则会报错*/
    FHAdv_Osd_SetTagCallback(FHT_OSD_USER1+0, FHT_UPDATED_SECOND, sample_osd_char_traverse_callback, sample_osd_char_refresh_callback);
    FHAdv_Osd_SetTagCallback(FHT_OSD_USER1+1, FHT_UPDATED_SECOND, sample_osd_char_traverse_callback, sample_osd_char_refresh_callback);

    /*先判断,如果channel 0 enable,那么配置channel 0 TOSD*/
    if (sample_common_dsp_get_channel_info(0, &chinfo) == 0 && chinfo.enable)
    {
        sample_set_channel_tosd(0);
    }

    /*先判断,如果channel 1 enable,那么配置channel 1 TOSD*/
    if (sample_common_dsp_get_channel_info(1, &chinfo) == 0 && chinfo.enable)
    {
#if (SAMPLE_TOSD_GRAPH == FHT_OSD_GRAPH_CTRL_TOSD_BEFORE_VP)
        /*注意: 如果TOSD配置为分通道前，那么只有通道0可以配置，并且其它所有通道的TOSD信息和通道0保持一致*/
        sample_set_channel_tosd(0);
#else
        sample_set_channel_tosd(1);
#endif
    }

    ret = FHAdv_Osd_UnLoadFontLib(FHEN_FONT_TYPE_ASC);
    if (ret != FH_SUCCESS)
    {
        printf("Unload ASC lib failed with %d\n", ret);
    }

    ret = FHAdv_Osd_UnLoadFontLib(FHEN_FONT_TYPE_CHINESE);
    if (ret != FH_SUCCESS)
    {
        printf("Unload GB2312 lib failed with %d\n", ret);
    }

    return 0;
}

static FH_SINT32 sample_set_logo(FH_VOID)
{
    FH_SINT32 ret;
    FHT_OSD_Logo_t logo_cfg;

    memset(&logo_cfg, 0, sizeof(logo_cfg));

    logo_cfg.enable = 1;                           /* 使能logo显示 */
    logo_cfg.rotate = 0;                           /* 不旋转 */
    logo_cfg.maxW   = AUTO_GEN_PIC_WIDTH;          /* 最大logo像素宽度，用于第一次设置内存分配 */
    logo_cfg.maxH   = AUTO_GEN_PIC_HEIGHT;         /* 最大logo像素高度，用于第一次设置内存分配 */
    logo_cfg.alpha  = 255;                         /* 不透明 */
    logo_cfg.area.fTopLeftX = 0;                   /* logo左上角起始点像素宽度位置 */
    logo_cfg.area.fTopLeftY = 64;                  /* logo左上角起始点像素高度位置 */
    logo_cfg.area.fWidth    = AUTO_GEN_PIC_WIDTH;  /* logo实际像素宽度 */
    logo_cfg.area.fHeigh    = AUTO_GEN_PIC_HEIGHT; /* logo实际像素高度 */
    logo_cfg.pData          = logo_data;           /* logo数据 */

    ret = FHAdv_Osd_SetLogo(&logo_cfg);
    if (ret != FH_SUCCESS)
    {
        printf("FHAdv_Osd_SetLogo failed with %x\n", ret);
        return ret;
    }

    return 0;
}

static FH_SINT32 sample_set_mask(FH_VOID)
{
    FH_SINT32 ret;
    FHT_OSD_Mask_t mask_cfg;

    memset(&mask_cfg, 0, sizeof(mask_cfg));

    /*配置mask 0*/
    mask_cfg.enable = 1;            /* 使能mask显示 */
    mask_cfg.maskId = 0;            /* mask ID 为0 */
    mask_cfg.rotate = 0;            /* 不旋转 */
    mask_cfg.area.fTopLeftX  = 16;  /* mask左上角起始点像素宽度位置 */
    mask_cfg.area.fTopLeftY  = 128; /* mask左上角起始点像素高度位置 */
    mask_cfg.area.fWidth     = 64;  /* mask宽度 */
    mask_cfg.area.fHeigh     = 64;  /* mask高度 */
    mask_cfg.osdColor.fAlpha = 255;
    mask_cfg.osdColor.fRed   = 0;
    mask_cfg.osdColor.fGreen = 255;
    mask_cfg.osdColor.fBlue  = 0;

    ret = FHAdv_Osd_SetMask(&mask_cfg);
    if (ret != FH_SUCCESS)
    {
        printf("FHAdv_Osd_SetMask failed with %x\n", ret);
        return ret;
    }

    /*配置mask 1*/
    mask_cfg.enable = 1;            /* 使能mask显示 */
    mask_cfg.maskId = 1;            /* mask ID 为1 */
    mask_cfg.rotate = 0;            /* 不旋转 */
    mask_cfg.area.fTopLeftX = 90;   /* mask左上角起始点像素宽度位置 */
    mask_cfg.area.fTopLeftY = 128;  /* mask左上角起始点像素高度位置 */
    mask_cfg.area.fWidth    = 64;   /* mask宽度 */
    mask_cfg.area.fHeigh    = 64;   /* mask高度 */

    ret = FHAdv_Osd_SetMask(&mask_cfg);
    if (ret != FH_SUCCESS)
    {
        printf("FHAdv_Osd_SetMask failed with %x\n", ret);
        return ret;
    }

    /* 清除mask 1 */
    mask_cfg.enable = 0; /* 去使能mask显示 */
    mask_cfg.maskId = 1; /* mask ID 为1 */
    ret = FHAdv_Osd_SetMask(&mask_cfg);
    if (ret != FH_SUCCESS)
    {
        printf("FHAdv_Osd_SetMask failed with %x\n", ret);
        return ret;
    }

    return 0;
}

FH_SINT32 sample_set_gbox(FH_UINT32 chan, FH_UINT32 enable,  FH_UINT32 box_id, FH_UINT32 x, FH_UINT32 y, FH_UINT32 w, FH_UINT32 h, FHT_RgbColor_t color)
{
    FH_SINT32 ret;
    FHT_OSD_Gbox_t gbox_cfg;

    memset(&gbox_cfg, 0, sizeof(gbox_cfg));

    /* 配置gbox 0 */
    gbox_cfg.enable = enable;      /* 使能gbox显示 */
    gbox_cfg.gboxId = box_id;      /* gbox ID 为0 */

    if (enable)
    {
        gbox_cfg.rotate = 0;           /* 不旋转 */
        gbox_cfg.gboxLineWidth  = 2;   /* gbox边框像素宽度为2*/
        gbox_cfg.area.fTopLeftX = x;   /* gbox左上角起始点像素宽度位置 */
        gbox_cfg.area.fTopLeftY = y;   /* gbox左上角起始点像素高度位置 */
        gbox_cfg.area.fWidth    = w;   /* gbox宽度 */
        gbox_cfg.area.fHeigh    = h;   /* gbox高度 */
        gbox_cfg.osdColor.fAlpha = color.fAlpha;
        gbox_cfg.osdColor.fRed   = color.fRed;
        gbox_cfg.osdColor.fGreen = color.fGreen;
        gbox_cfg.osdColor.fBlue  = color.fBlue;
    }
    else /*just work-around FHAdv_Osd_Ex_SetGbox,it will check fWidth and fHeight*/
    {
        gbox_cfg.gboxLineWidth  = 2;
        gbox_cfg.area.fTopLeftX = 0;
        gbox_cfg.area.fTopLeftY = 0;
        gbox_cfg.area.fWidth    = 16;
        gbox_cfg.area.fHeigh    = 16;
        gbox_cfg.osdColor.fAlpha = color.fAlpha;
        gbox_cfg.osdColor.fRed   = color.fRed;
        gbox_cfg.osdColor.fGreen = color.fGreen;
        gbox_cfg.osdColor.fBlue  = color.fBlue;
    }

    ret = FHAdv_Osd_Ex_SetGbox(chan, &gbox_cfg);
    if (ret != FH_SUCCESS)
    {
        printf("FHAdv_Osd_SetGbox failed with %x\n", ret);
        return ret;
    }

    return 0;
}

FH_VOID sample_overlay_start(FH_VOID)
{
    sample_osd_init();

#ifdef FH_APP_OVERLAY_LOGO
    sample_set_logo();
#endif

#ifdef FH_APP_OVERLAY_TOSD
    sample_set_tosd();
#endif

#ifdef FH_APP_OVERLAY_MASK
    sample_set_mask();
#endif
}

FH_VOID sample_overlay_stop(FH_VOID)
{
    FHAdv_Osd_UnInit();
}
#endif
