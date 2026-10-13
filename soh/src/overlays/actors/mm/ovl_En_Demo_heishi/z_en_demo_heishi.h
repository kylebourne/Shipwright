// MM port: generated from the Majora's Mask decomp's ovl_En_Demo_heishi/z_en_demo_heishi.h by port_actor.py.
#ifndef MM_Z_EN_DEMO_HEISHI_H
#define MM_Z_EN_DEMO_HEISHI_H

#include "global.h"
#include "mm/objects/object_sdn/object_sdn.h"
#include "mm_compat.h" // MM port

struct MmEnDemoheishi;

typedef void (*MmEnDemoheishiActionFunc)(struct MmEnDemoheishi*, PlayState*);

typedef struct MmEnDemoheishi {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[SOLDIER_LIMB_MAX];
    /* 0x1EE */ Vec3s morphTable[SOLDIER_LIMB_MAX];
    /* 0x254 */ MmEnDemoheishiActionFunc actionFunc;
    /* 0x258 */ s16 headRotY;
    /* 0x25A */ s16 headRotX;
    /* 0x25C */ s16 headRotZ;
    /* 0x25E */ s16 headRotYTarget;
    /* 0x260 */ s16 headRotXTarget;
    /* 0x262 */ s16 headRotZTarget;
    /* 0x264 */ s32 animIndex;
    /* 0x268 */ s16 timer;
    /* 0x26C */ f32 animEndFrame;
    /* 0x270 */ s16 isTalking;
    /* 0x272 */ s16 textIdIndex;
    /* 0x274 */ ColliderCylinder colliderCylinder;
} MmEnDemoheishi; // size = 0x2C0

#endif // MM_Z_EN_DEMO_HEISHI_H
