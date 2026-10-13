// MM port: generated from the Majora's Mask decomp's ovl_Bg_Spout_Fire/z_bg_spout_fire.h by port_actor.py.
#ifndef MM_Z_BG_SPOUT_FIRE_H
#define MM_Z_BG_SPOUT_FIRE_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmBgSpoutFire;

typedef void (*MmBgSpoutFireActionFunc)(struct MmBgSpoutFire*, PlayState*);

typedef struct MmBgSpoutFire {
    /* 0x000 */ Actor actor;
    /* 0x144 */ MmBgSpoutFireActionFunc actionFunc;
    /* 0x148 */ s16 timer;
    /* 0x14A */ s16 flameTexIndex;
    /* 0x14C */ ColliderCylinder collider;
} MmBgSpoutFire; // size = 0x198

#endif // MM_Z_BG_SPOUT_FIRE_H
