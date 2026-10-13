// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero4/z_en_ending_hero4.h by port_actor.py.
#ifndef MM_Z_EN_ENDING_HERO4_H
#define MM_Z_EN_ENDING_HERO4_H

#include "global.h"
#include "mm/objects/object_sdn/object_sdn.h"
#include "mm_compat.h" // MM port

struct MmEnEndingHero4;

typedef void (*MmEnEndingHero4ActionFunc)(struct MmEnEndingHero4*, PlayState*);

typedef struct MmEnEndingHero4 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[SOLDIER_LIMB_MAX];
    /* 0x1EE */ Vec3s morphTable[SOLDIER_LIMB_MAX];
    /* 0x254 */ MmEnEndingHero4ActionFunc actionFunc;
    /* 0x258 */ u16 isIdle;
} MmEnEndingHero4; // size = 0x25C

#endif // MM_Z_EN_ENDING_HERO4_H
