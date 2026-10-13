// MM port: generated from the Majora's Mask decomp's ovl_Obj_Hana/z_obj_hana.c by port_actor.py.
/*
 * File: z_obj_hana.c
 * Overlay: ovl_Obj_Hana
 * Description: Orange Graveyard Flower
 */

#include "z_obj_hana.h"
#include "mm/objects/object_hana/object_hana.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS 0x00000000

void MmObjHana_Init(Actor* thisx, PlayState* play);
void MmObjHana_Destroy(Actor* thisx, PlayState* play);
void MmObjHana_Update(Actor* thisx, PlayState* play);
void MmObjHana_Draw(Actor* thisx, PlayState* play);

ActorInit Mm_Obj_Hana_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmObjHana),
    MmObjHana_Init,
    MmObjHana_Destroy,
    MmObjHana_Update,
    MmObjHana_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 10, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 900, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 40, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 40, ICHAIN_STOP),
};

void MmObjHana_Init(Actor* thisx, PlayState* play) {
    MmObjHana* this = (MmObjHana*)thisx;

    Actor_ProcessInitChain(&this->actor, sInitChain);
}

void MmObjHana_Destroy(Actor* thisx, PlayState* play) {
}

void MmObjHana_Update(Actor* thisx, PlayState* play) {
}

void MmObjHana_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, gGraveyardFlowersDL);
}
