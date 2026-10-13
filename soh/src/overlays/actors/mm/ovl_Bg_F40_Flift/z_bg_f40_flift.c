// MM port: generated from the Majora's Mask decomp's ovl_Bg_F40_Flift/z_bg_f40_flift.c by port_actor.py.
/*
 * File: z_bg_f40_flift.c
 * Overlay: ovl_Bg_F40_Flift
 * Description: Stone Tower Temple - Grey Square Stone Elevator
 */

#include "z_bg_f40_flift.h"
#include "mm/objects/object_f40_obj/object_f40_obj.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void MmBgF40Flift_Init(Actor* thisx, PlayState* play);
void MmBgF40Flift_Destroy(Actor* thisx, PlayState* play);
void MmBgF40Flift_Update(Actor* thisx, PlayState* play);
void MmBgF40Flift_Draw(Actor* thisx, PlayState* play);

void MmBgF40Flift_func_808D75F0(MmBgF40Flift* this, PlayState* play);
void MmBgF40Flift_func_808D7714(MmBgF40Flift* this, PlayState* play);

ActorInit Mm_Bg_F40_Flift_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmBgF40Flift),
    MmBgF40Flift_Init,
    MmBgF40Flift_Destroy,
    MmBgF40Flift_Update,
    MmBgF40Flift_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 5000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void MmBgF40Flift_Init(Actor* thisx, PlayState* play) {
    MmBgF40Flift* this = (MmBgF40Flift*)thisx;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, DYNA_TRANSFORM_POS);
    DynaPolyActor_LoadMesh(play, &this->dyna, &gStoneTowerTempleSquareElevatorCol);
    this->dyna.actor.params = 1;
    this->actionFunc = MmBgF40Flift_func_808D75F0;
}

void MmBgF40Flift_Destroy(Actor* thisx, PlayState* play) {
    MmBgF40Flift* this = (MmBgF40Flift*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void MmBgF40Flift_func_808D75F0(MmBgF40Flift* this, PlayState* play) {
    if (((this->dyna.actor.params == 1) && DynaPolyActor_IsPlayerOnTop(&this->dyna)) ||
        ((this->dyna.actor.params == -1) && !DynaPolyActor_IsPlayerOnTop(&this->dyna))) {
        this->timer = 96;
        this->actionFunc = MmBgF40Flift_func_808D7714;
    } else {
        if (this->timer == 0) {
            this->timer = 48;
        }
        this->timer--;
        if (this->dyna.actor.params == 1) {
            this->dyna.actor.world.pos.y =
                (Math_SinF(this->timer * (M_PIf / 24.0f)) * 5.0f) + this->dyna.actor.home.pos.y;
        } else {
            this->dyna.actor.world.pos.y =
                (Math_SinF(this->timer * (M_PIf / 24.0f)) * 5.0f) + (926.8f + this->dyna.actor.home.pos.y);
        }
    }
}

void MmBgF40Flift_func_808D7714(MmBgF40Flift* this, PlayState* play) {
    if (this->timer != 0) {
        this->timer--;
        this->dyna.actor.world.pos.y =
            (((Math_CosF(this->timer * (M_PIf / 96.0f)) * this->dyna.actor.params) + 1.0f) * 463.4f) +
            this->dyna.actor.home.pos.y;
    } else {
        this->dyna.actor.params = -this->dyna.actor.params;
        this->actionFunc = MmBgF40Flift_func_808D75F0;
    }
}

void MmBgF40Flift_Update(Actor* thisx, PlayState* play) {
    MmBgF40Flift* this = (MmBgF40Flift*)thisx;

    this->actionFunc(this, play);
}

void MmBgF40Flift_Draw(Actor* thisx, PlayState* play) {
    Gfx_DrawDListOpa(play, gStoneTowerTempleSquareElevatorDL);
}
