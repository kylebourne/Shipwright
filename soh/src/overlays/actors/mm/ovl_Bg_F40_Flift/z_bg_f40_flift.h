// MM port: generated from the Majora's Mask decomp's ovl_Bg_F40_Flift/z_bg_f40_flift.h by port_actor.py.
#ifndef MM_Z_BG_F40_FLIFT_H
#define MM_Z_BG_F40_FLIFT_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmBgF40Flift;

typedef void (*MmBgF40FliftActionFunc)(struct MmBgF40Flift*, PlayState*);

typedef struct MmBgF40Flift {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ MmBgF40FliftActionFunc actionFunc;
    /* 0x160 */ s32 timer;
} MmBgF40Flift; // size = 0x164

#endif // MM_Z_BG_F40_FLIFT_H
