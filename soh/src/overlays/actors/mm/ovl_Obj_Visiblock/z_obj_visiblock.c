// MM port: generated from the Majora's Mask decomp's ovl_Obj_Visiblock/z_obj_visiblock.c by port_actor.py.
/*
 * File: z_obj_visiblock.c
 * Overlay: ovl_Obj_Visiblock
 * Description: Lens of Truth Platform
 */

#include "z_obj_visiblock.h"
#include "mm/objects/object_visiblock/object_visiblock.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_REACT_TO_LENS)

void MmObjVisiblock_Init(Actor* thisx, PlayState* play);
void MmObjVisiblock_Destroy(Actor* thisx, PlayState* play);
void MmObjVisiblock_Draw(Actor* thisx, PlayState* play);

ActorInit Mm_Obj_Visiblock_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmObjVisiblock),
    MmObjVisiblock_Init,
    MmObjVisiblock_Destroy,
    Actor_Noop,
    MmObjVisiblock_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 200, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 150, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void MmObjVisiblock_Init(Actor* thisx, PlayState* play) {
    MmObjVisiblock* this = (MmObjVisiblock*)thisx;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, 0);
    DynaPolyActor_LoadMesh(play, &this->dyna, &gLensOfTruthPlatformCol);
}

void MmObjVisiblock_Destroy(Actor* thisx, PlayState* play) {
    MmObjVisiblock* this = (MmObjVisiblock*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void MmObjVisiblock_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListXlu(play, gLensOfTruthPlatformDL);
}
