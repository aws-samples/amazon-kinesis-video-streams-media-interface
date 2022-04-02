#ifndef __rect_merge_by_gaus_h__
#define __rect_merge_by_gaus_h__

struct _rect
{
    FH_UINT32 u32X;
    FH_UINT32 u32Y;
    FH_UINT32 u32Width;
    FH_UINT32 u32Height;
};

typedef struct
{
    FH_UINT8 *md_ex_result_addr;/*IN,函数FHAdv_MD_Ex_GetResult返回的检测结果的起始地址*/
    FH_UINT32 horizontal_count; /*IN,水平方向上检测子区域的个数*/
    FH_UINT32 vertical_count;   /*IN,垂直方向上检测子区域的个数*/
    FH_UINT32 area_th;  /*IN, 融合后框的最小面积,以子区域个数为单位计算*/

    FH_UINT32 rect_num; /*IN/OUT,输入时,表示允许的最多的融合框数量, 输出时,表示实际的融合框数量*/
    struct _rect *rect;   /*IN/OUT,输入时,调用者提供正确的rect内存,输出时,rect已填好实际的融合框*/
} Motion_BGM_RUNTB_RECT;

/*
 * 由于MD EX输出结果中,一般会包含很多大小相同的子区域,美观效果上看起来比较难看,
 * 因此,通过高斯融合,把这些子区域融合为更大的大区域. *
 */
extern void getOrdFromGau(Motion_BGM_RUNTB_RECT *run);

#endif /* __rect_merge_by_gaus_h__ */
