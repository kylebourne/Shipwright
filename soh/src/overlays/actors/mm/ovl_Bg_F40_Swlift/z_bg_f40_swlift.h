// MM port: generated from the Majora's Mask decomp's ovl_Bg_F40_Swlift/z_bg_f40_swlift.h by port_actor.py.
#ifndef MM_Z_BG_F40_SWLIFT_H
#define MM_Z_BG_F40_SWLIFT_H

#define BG_F40_SWLIFT_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0xFF)
#define BG_F40_SWLIFT_GET_INDEX(thisx) (((thisx)->params >> 8) & 0xFF)

#include "global.h"
#include "mm_compat.h" // MM port

struct MmBgF40Swlift;

typedef struct MmBgF40Swlift {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ s32 timer;
} MmBgF40Swlift; // size = 0x160

#endif // MM_Z_BG_F40_SWLIFT_H
