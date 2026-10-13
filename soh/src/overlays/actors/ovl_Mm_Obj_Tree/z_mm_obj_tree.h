#ifndef Z_MM_OBJ_TREE_H
#define Z_MM_OBJ_TREE_H

#include <libultraship/libultra.h>
#include "global.h"

struct MmObjTree;

typedef void (*MmObjTreeActionFunc)(struct MmObjTree*, PlayState*);

#define MMOBJTREE_ISLARGE(thisx) ((thisx)->params & 0x8000)

// Ported from Majora's Mask: ovl_Obj_Tree (single branching tree, e.g. North Clock Town).
typedef struct MmObjTree {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x164 */ ColliderCylinder collider;
    /* 0x1B0 */ MmObjTreeActionFunc actionFunc;
    /* 0x1B4 */ f32 swayAmplitude;
    /* 0x1B8 */ s16 swayAngle;
    /* 0x1BA */ s16 swayVelocity;
    /* 0x1BC */ s16 timer;
} MmObjTree;

void MmObjTree_Init(Actor* thisx, PlayState* play);
void MmObjTree_Destroy(Actor* thisx, PlayState* play);
void MmObjTree_Update(Actor* thisx, PlayState* play);
void MmObjTree_Draw(Actor* thisx, PlayState* play);

#endif // Z_MM_OBJ_TREE_H
