#ifndef __VLCVIEW_H__
#define __VLCVIEW_H__

#include "type_def.h"

extern int _vlcview(char *dst_ip, unsigned int port);
extern int _vlcview_exit(void);

extern FH_SINT32 sample_video_init(void);
extern FH_SINT32 sample_video_exit(FH_VOID);
extern FH_SINT32 change_video(FH_UINT32 chan, FH_UINT32 format, FH_UINT32 res);

#endif /*__VLCVIEW_H__*/
