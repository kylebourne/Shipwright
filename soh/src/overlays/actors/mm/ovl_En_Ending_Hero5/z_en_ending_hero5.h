// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero5/z_en_ending_hero5.h by port_actor.py.
#ifndef MM_Z_EN_ENDING_HERO5_H
#define MM_Z_EN_ENDING_HERO5_H

#include "global.h"
#include "mm/objects/object_daiku/object_daiku.h"
#include "mm_compat.h" // MM port

struct MmEnEndingHero5;

typedef void (*MmEnEndingHero5ActionFunc)(struct MmEnEndingHero5*, PlayState*);

typedef struct MmEnEndingHero5 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_DAIKU_LIMB_MAX];
    /* 0x1EE */ Vec3s morphTable[OBJECT_DAIKU_LIMB_MAX];
    /* 0x254 */ MmEnEndingHero5ActionFunc actionFunc;
    /* 0x258 */ u16 isIdle;
    /* 0x25C */ s32 unk25C;
} MmEnEndingHero5; // size = 0x260

#endif // MM_Z_EN_ENDING_HERO5_H
