// MM port: generated from the Majora's Mask decomp's ovl_Bg_Keikoku_Saku/z_bg_keikoku_saku.c by port_actor.py.
/*
 * File: z_bg_keikoku_saku.c
 * Overlay: ovl_Bg_Keikoku_Saku
 * Description: Termina Field Spiked Fence
 */

#include "z_bg_keikoku_saku.h"
#include "mm/objects/object_keikoku_obj/object_keikoku_obj.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS 0x00000000

void MmBgKeikokuSaku_Init(Actor* thisx, PlayState* play);
void MmBgKeikokuSaku_Destroy(Actor* thisx, PlayState* play);
void MmBgKeikokuSaku_Update(Actor* thisx, PlayState* play);
void MmBgKeikokuSaku_Draw(Actor* thisx, PlayState* play);

void MmBgKeikokuSaku_func_80A5389C(MmBgKeikokuSaku* this, PlayState* play);
void MmBgKeikokuSaku_func_80A538E0(MmBgKeikokuSaku* this, PlayState* play);
void MmBgKeikokuSaku_func_80A53994(MmBgKeikokuSaku* this, PlayState* play);

ActorInit Mm_Bg_Keikoku_Saku_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmBgKeikokuSaku),
    MmBgKeikokuSaku_Init,
    MmBgKeikokuSaku_Destroy,
    MmBgKeikokuSaku_Update,
    MmBgKeikokuSaku_Draw,
};

void MmBgKeikokuSaku_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    MmBgKeikokuSaku* this = (MmBgKeikokuSaku*)thisx;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(&object_keikoku_obj_Colheader_002300, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    this->switchFlag = MMBGKEIKOKUSAKU_GET_SWITCH_FLAG(thisx);
    if (Flags_GetSwitch(play, this->switchFlag)) {
        this->dyna.actor.world.pos.z = 2659.0f;
    } else {
        this->actionFunc = MmBgKeikokuSaku_func_80A5389C;
    }
}

void MmBgKeikokuSaku_Destroy(Actor* thisx, PlayState* play) {
    MmBgKeikokuSaku* this = (MmBgKeikokuSaku*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void MmBgKeikokuSaku_func_80A5389C(MmBgKeikokuSaku* this, PlayState* play) {
    if (Flags_GetSwitch(play, this->switchFlag)) {
        this->actionFunc = MmBgKeikokuSaku_func_80A538E0;
    }
}

void MmBgKeikokuSaku_func_80A538E0(MmBgKeikokuSaku* this, PlayState* play) {
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_METALGATE_OPEN - SFX_FLAG);
    this->dyna.actor.world.pos.z -= 2.0f + BREG(8);
    if (this->dyna.actor.world.pos.z < (BREG(9) + 2660.0f)) {
        Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_BRIDGE_OPEN_STOP);
        this->timer = 30;
        this->actionFunc = MmBgKeikokuSaku_func_80A53994;
    }
}

void MmBgKeikokuSaku_func_80A53994(MmBgKeikokuSaku* this, PlayState* play) {
    if (this->timer == 0) {
        this->actionFunc = MmBgKeikokuSaku_func_80A5389C;
    }
}

void MmBgKeikokuSaku_Update(Actor* thisx, PlayState* play) {
    MmBgKeikokuSaku* this = (MmBgKeikokuSaku*)thisx;

    if (this->timer) {
        this->timer--;
    }
    this->dyna.actor.world.pos.x = BREG(5) + this->dyna.actor.home.pos.x;
    this->dyna.actor.world.pos.y = BREG(6) + this->dyna.actor.home.pos.y;
    this->dyna.actor.world.pos.z = BREG(7) + this->dyna.actor.home.pos.z;
    this->dyna.actor.scale.x = (BREG(10) / 1000.0f) + 0.1f;
    this->dyna.actor.scale.y = (BREG(11) / 1000.0f) + 0.1f;
    this->dyna.actor.scale.z = (BREG(12) / 1000.0f) + 0.1f;
    this->actionFunc(this, play);
}

void MmBgKeikokuSaku_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, object_keikoku_obj_DL_001640);

    CLOSE_DISPS(play->state.gfxCtx);
}
