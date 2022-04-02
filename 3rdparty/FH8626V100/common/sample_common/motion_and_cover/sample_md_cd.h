#ifndef __SAMPLE_CD_H__
#define __SAMPLE_CD_H__

extern FH_SINT32 sample_md_cd_start(FH_VOID);
extern FH_SINT32 sample_md_cd_stop(FH_VOID);

extern FH_SINT32 sample_md_get_outer_motion_area(
	FH_UINT32 *winX, 
	FH_UINT32 *winY,
	FH_UINT32 *winW,
	FH_UINT32 *winH,
	FH_UINT32 *picW,
	FH_UINT32 *picH);

extern FH_SINT32 sample_md_get_statistics_sum(
	FH_UINT32 detect_frame_w, 
	                                    FH_UINT32 detect_frame_h, 
	                                    FH_UINT32 x, 
	                                    FH_UINT32 y, 
	                                    FH_UINT32 w, 
	                                    FH_UINT32 h, 
	                                    FH_UINT32 *sum,
	                                    FH_UINT32 *blk_num);


#endif /* __SAMPLE_CD_H__ */
