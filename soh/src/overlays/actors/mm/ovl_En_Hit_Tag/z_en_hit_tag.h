// MM port: generated from the Majora's Mask decomp's ovl_En_Hit_Tag/z_en_hit_tag.h by port_actor.py.
#ifndef MM_Z_EN_HIT_TAG_H
#define MM_Z_EN_HIT_TAG_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmEnHitTag;

typedef void (*MmEnHitTagActionFunc)(struct MmEnHitTag*, PlayState*);

#define MMENHITTAG_GET_SWITCH_FLAG(thisx) (((thisx)->params & 0xFE00) >> 9)

typedef struct MmEnHitTag {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ MmEnHitTagActionFunc actionFunc;
} MmEnHitTag; // size = 0x194

#endif // MM_Z_EN_HIT_TAG_H
