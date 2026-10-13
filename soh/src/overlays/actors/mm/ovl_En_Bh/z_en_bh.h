// MM port: generated from the Majora's Mask decomp's ovl_En_Bh/z_en_bh.h by port_actor.py.
#ifndef MM_Z_EN_BH_H
#define MM_Z_EN_BH_H

#include "global.h"
#include "mm/objects/object_bh/object_bh.h"
#include "mm_compat.h" // MM port

struct MmEnBh;

typedef void (*MmEnBhActionFunc)(struct MmEnBh*, PlayState*);

typedef struct MmEnBh {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_BH_LIMB_MAX];
    /* 0x1AC */ Vec3s morphTable[OBJECT_BH_LIMB_MAX];
    /* 0x1D0 */ Vec3f pos;
    /* 0x1DC */ s16 timer;
    /* 0x1DE */ s16 timer2;
    /* 0x1E0 */ s16 step;
    /* 0x1E2 */ s16 unk1E2;
    /* 0x1E4 */ s16 unk1E4;
    /* 0x1E8 */ MmEnBhActionFunc actionFunc;
} MmEnBh; // size = 0x1EC

#endif // MM_Z_EN_BH_H
