// MM port: generated from the Majora's Mask decomp's ovl_En_Nnh/z_en_nnh.h by port_actor.py.
#ifndef MM_Z_EN_NNH_H
#define MM_Z_EN_NNH_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmEnNnh;

typedef void (*MmEnNnhActionFunc)(struct MmEnNnh*, PlayState*);

typedef struct MmEnNnh {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ MmEnNnhActionFunc actionFunc;
} MmEnNnh; // size = 0x194

#endif // MM_Z_EN_NNH_H
