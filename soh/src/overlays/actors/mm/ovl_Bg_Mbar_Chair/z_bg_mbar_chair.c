// MM port: generated from the Majora's Mask decomp's ovl_Bg_Mbar_Chair/z_bg_mbar_chair.c by port_actor.py.
/*
 * File: z_bg_mbar_chair.c
 * Overlay: ovl_Bg_Mbar_Chair
 * Description: Milk Bar - Chair
 */

#include "z_bg_mbar_chair.h"
#include "mm/objects/object_mbar_obj/object_mbar_obj.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS 0x00000000

void MmBgMbarChair_Init(Actor* thisx, PlayState* play);
void MmBgMbarChair_Destroy(Actor* thisx, PlayState* play);
void MmBgMbarChair_Update(Actor* thisx, PlayState* play);
void MmBgMbarChair_Draw(Actor* thisx, PlayState* play);

ActorInit Mm_Bg_Mbar_Chair_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmBgMbarChair),
    MmBgMbarChair_Init,
    MmBgMbarChair_Destroy,
    MmBgMbarChair_Update,
    MmBgMbarChair_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 60, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 80, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void MmBgMbarChair_Init(Actor* thisx, PlayState* play) {
    MmBgMbarChair* this = (MmBgMbarChair*)thisx;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, 0);
    DynaPolyActor_LoadMesh(play, &this->dyna, &object_mbar_obj_Colheader_0019B4);
}

void MmBgMbarChair_Destroy(Actor* thisx, PlayState* play) {
    MmBgMbarChair* this = (MmBgMbarChair*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void MmBgMbarChair_Update(Actor* thisx, PlayState* play) {
}

void MmBgMbarChair_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, object_mbar_obj_DL_000288);
}
