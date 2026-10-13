// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero2/z_en_ending_hero2.h by port_actor.py.
#ifndef MM_Z_EN_ENDING_HERO2_H
#define MM_Z_EN_ENDING_HERO2_H

#include "global.h"
#include "mm/objects/object_bai/object_bai.h"
#include "mm_compat.h" // MM port

struct MmEnEndingHero2;

typedef void (*MmEnEndingHero2ActionFunc)(struct MmEnEndingHero2*, PlayState*);

typedef struct MmEnEndingHero2 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_BAI_LIMB_MAX];
    /* 0x200 */ Vec3s morphTable[OBJECT_BAI_LIMB_MAX];
    /* 0x278 */ MmEnEndingHero2ActionFunc actionFunc;
    /* 0x27C */ u16 isIdle;
} MmEnEndingHero2; // size = 0x280

#endif // MM_Z_EN_ENDING_HERO2_H
