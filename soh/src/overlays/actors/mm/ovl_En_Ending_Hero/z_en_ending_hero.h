// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero/z_en_ending_hero.h by port_actor.py.
#ifndef MM_Z_EN_ENDING_HERO_H
#define MM_Z_EN_ENDING_HERO_H

#include "global.h"
#include "mm/objects/object_dt/object_dt.h"
#include "mm_compat.h" // MM port

struct MmEnEndingHero;

typedef void (*MmEnEndingHeroActionFunc)(struct MmEnEndingHero*, PlayState*);

typedef struct MmEnEndingHero {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_DT_LIMB_MAX];
    /* 0x1E2 */ Vec3s morphTable[OBJECT_DT_LIMB_MAX];
    /* 0x23C */ MmEnEndingHeroActionFunc actionFunc;
    /* 0x240 */ s16 unk240;
    /* 0x242 */ s16 unk242;
    /* 0x244 */ u16 isIdle;
} MmEnEndingHero; // size = 0x248

#endif // MM_Z_EN_ENDING_HERO_H
