// MM port: generated from the Majora's Mask decomp's ovl_En_Nnh/z_en_nnh.c by port_actor.py.
/*
 * File: z_en_nnh.c
 * Overlay: ovl_En_Nnh
 * Description: Twisted Corpse of Deku Butler's Son
 */

#include "z_en_nnh.h"
#include "mm/objects/object_nnh/object_nnh.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void MmEnNnh_Init(Actor* thisx, PlayState* play);
void MmEnNnh_Destroy(Actor* thisx, PlayState* play);
void MmEnNnh_Update(Actor* thisx, PlayState* play);
void MmEnNnh_Draw(Actor* thisx, PlayState* play);

void MmEnNnh_SetupWaitForDialogue(MmEnNnh* this);
void MmEnNnh_WaitForDialogue(MmEnNnh* this, PlayState* play);
void MmEnNnh_SetupDialogue(MmEnNnh* this);
void MmEnNnh_Dialogue(MmEnNnh* this, PlayState* play);

ActorInit Mm_En_Nnh_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnNnh),
    MmEnNnh_Init,
    MmEnNnh_Destroy,
    MmEnNnh_Update,
    MmEnNnh_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_TREE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 20, 50, 0, { 0, 0, 0 } },
};

void MmEnNnh_Init(Actor* thisx, PlayState* play) {
    MmEnNnh* this = (MmEnNnh*)thisx;

    Actor_SetScale(&this->actor, 0.01f);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->actor.targetMode = ATTENTION_RANGE_1;
    this->actor.focus.pos = this->actor.world.pos;
    this->actor.focus.pos.y += 30.0f;
    MmEnNnh_SetupWaitForDialogue(this);
}

void MmEnNnh_Destroy(Actor* thisx, PlayState* play) {
    MmEnNnh* this = (MmEnNnh*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void MmEnNnh_SetupWaitForDialogue(MmEnNnh* this) {
    this->actionFunc = MmEnNnh_WaitForDialogue;
}

void MmEnNnh_WaitForDialogue(MmEnNnh* this, PlayState* play) {
    if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        Message_StartTextbox(play, 0x228, &this->actor);
        MmEnNnh_SetupDialogue(this);
    } else {
        Actor_OfferTalk(&this->actor, play, 100.0f);
    }
}

void MmEnNnh_SetupDialogue(MmEnNnh* this) {
    this->actionFunc = MmEnNnh_Dialogue;
}

void MmEnNnh_Dialogue(MmEnNnh* this, PlayState* play) {
    if (Actor_TextboxIsClosing(&this->actor, play)) {
        MmEnNnh_SetupWaitForDialogue(this);
    }
}

void MmEnNnh_Update(Actor* thisx, PlayState* play) {
    MmEnNnh* this = (MmEnNnh*)thisx;
    s32 pad;

    this->actionFunc(this, play);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void MmEnNnh_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, gButlerSonMainBodyDL);

    CLOSE_DISPS(play->state.gfxCtx);
}
