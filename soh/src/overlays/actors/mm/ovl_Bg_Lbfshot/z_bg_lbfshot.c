// MM port: generated from the Majora's Mask decomp's ovl_Bg_Lbfshot/z_bg_lbfshot.c by port_actor.py.
/*
 * File: z_bg_lbfshot.c
 * Overlay: ovl_Bg_Lbfshot
 * Description: Rainbow Hookshot Pillar
 */

#include "z_bg_lbfshot.h"
#include "mm/objects/object_lbfshot/object_lbfshot.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS 0x00000000

void MmBgLbfshot_Init(Actor* thisx, PlayState* play);
void MmBgLbfshot_Destroy(Actor* thisx, PlayState* play);
void MmBgLbfshot_Draw(Actor* thisx, PlayState* play);

ActorInit Mm_Bg_Lbfshot_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmBgLbfshot),
    MmBgLbfshot_Init,
    MmBgLbfshot_Destroy,
    Actor_Noop,
    MmBgLbfshot_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void MmBgLbfshot_Init(Actor* thisx, PlayState* play) {
    MmBgLbfshot* this = (MmBgLbfshot*)thisx;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    this->dyna.actor.uncullZoneForward = 4000.0f;
    DynaPolyActor_Init(&this->dyna, DYNA_TRANSFORM_POS);
    DynaPolyActor_LoadMesh(play, &this->dyna, &object_lbfshot_Colheader_0014D8);
}
void MmBgLbfshot_Destroy(Actor* thisx, PlayState* play) {
    MmBgLbfshot* this = (MmBgLbfshot*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}
void MmBgLbfshot_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, object_lbfshot_DL_000228);
}
