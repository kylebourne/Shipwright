// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero3/z_en_ending_hero3.h by port_actor.py.
#ifndef MM_Z_EN_ENDING_HERO3_H
#define MM_Z_EN_ENDING_HERO3_H

#include "global.h"
#include "mm/objects/object_toryo/object_toryo.h"
#include "mm_compat.h" // MM port

struct MmEnEndingHero3;

typedef void (*MmEnEndingHero3ActionFunc)(struct MmEnEndingHero3*, PlayState*);

typedef struct MmEnEndingHero3 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_TORYO_LIMB_MAX];
    /* 0x1EE */ Vec3s morphTable[OBJECT_TORYO_LIMB_MAX];
    /* 0x254 */ MmEnEndingHero3ActionFunc actionFunc;
    /* 0x258 */ u16 isIdle;
} MmEnEndingHero3; // size = 0x25C

#endif // MM_Z_EN_ENDING_HERO3_H
