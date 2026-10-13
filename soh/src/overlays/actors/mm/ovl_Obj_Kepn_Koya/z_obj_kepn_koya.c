// MM port: generated from the Majora's Mask decomp's ovl_Obj_Kepn_Koya/z_obj_kepn_koya.c by port_actor.py.
/*
 * File: z_obj_kepn_koya.c
 * Overlay: ovl_Obj_Kepn_Koya
 * Description: Gorman Bros. Buildings
 */

#include "z_obj_kepn_koya.h"
#include "mm/objects/object_kepn_koya/object_kepn_koya.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS 0x00000000

void MmObjKepnKoya_Init(Actor* thisx, PlayState* play);
void MmObjKepnKoya_Destroy(Actor* thisx, PlayState* play);
void MmObjKepnKoya_Update(Actor* thisx, PlayState* play);
void MmObjKepnKoya_Draw(Actor* thisx, PlayState* play);

ActorInit Mm_Obj_Kepn_Koya_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmObjKepnKoya),
    MmObjKepnKoya_Init,
    MmObjKepnKoya_Destroy,
    MmObjKepnKoya_Update,
    MmObjKepnKoya_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 900, ICHAIN_STOP),
};

void MmObjKepnKoya_Init(Actor* thisx, PlayState* play) {
    MmObjKepnKoya* this = (MmObjKepnKoya*)thisx;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    Actor_SetScale(&this->dyna.actor, 0.1f);
    DynaPolyActor_Init(&this->dyna, 0);
    DynaPolyActor_LoadMesh(play, &this->dyna, &object_kepn_koya_Colheader_00805C);
    if (this->dyna.bgId == BG_ACTOR_MAX) {
        Actor_Kill(&this->dyna.actor);
    }
}

void MmObjKepnKoya_Destroy(Actor* thisx, PlayState* play) {
    MmObjKepnKoya* this = (MmObjKepnKoya*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void MmObjKepnKoya_Update(Actor* thisx, PlayState* play) {
}

void MmObjKepnKoya_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, object_kepn_koya_DL_003478);
}
