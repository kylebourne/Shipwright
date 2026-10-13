// MM port: generated from the Majora's Mask decomp's ovl_Bg_Botihasira/z_bg_botihasira.h by port_actor.py.
#ifndef MM_Z_BG_BOTIHASIRA_H
#define MM_Z_BG_BOTIHASIRA_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmBgBotihasira;

typedef void (*MmBgBotihasiraActionFunc)(struct MmBgBotihasira*, PlayState*);

typedef struct MmBgBotihasira {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ MmBgBotihasiraActionFunc actionFunc;
    /* 0x160 */ ColliderCylinder collider;
} MmBgBotihasira; // size = 0x1AC

#endif // MM_Z_BG_BOTIHASIRA_H
