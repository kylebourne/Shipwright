// MM port: generated from the Majora's Mask decomp's ovl_Bg_Botihasira/z_bg_botihasira.c by port_actor.py.
/*
 * File: z_bg_botihasira.c
 * Overlay: ovl_Bg_Botihasira
 * Description: Captain Keeta Race - Gatepost
 */

#include "z_bg_botihasira.h"
#include "mm/objects/object_botihasira/object_botihasira.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS 0x00000000

void MmBgBotihasira_Init(Actor* thisx, PlayState* play);
void MmBgBotihasira_Destroy(Actor* thisx, PlayState* play);
void MmBgBotihasira_Update(Actor* thisx, PlayState* play2);
void MmBgBotihasira_Draw(Actor* thisx, PlayState* play);

void MmBgBotihasira_DoNothing(MmBgBotihasira* this, PlayState* play);

ActorInit Mm_Bg_Botihasira_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmBgBotihasira),
    MmBgBotihasira_Init,
    MmBgBotihasira_Destroy,
    MmBgBotihasira_Update,
    MmBgBotihasira_Draw,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_METAL,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK2,
        { 0xFFCFFFFF, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 27, 80, 0, { 0, 0, 0 } },
};

void MmBgBotihasira_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    MmBgBotihasira* this = (MmBgBotihasira*)thisx;
    CollisionHeader* colHeader = NULL;

    if (this->dyna.actor.params == 0) {
        DynaPolyActor_Init(&this->dyna, 0);
        CollisionHeader_GetVirtual(&object_botihasira_Colheader_001BD8, &colHeader);
        this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    } else {
        Collider_InitAndSetCylinder(play, &this->collider, &this->dyna.actor, &sCylinderInit);
    }
    this->actionFunc = MmBgBotihasira_DoNothing;
    this->dyna.actor.scale.z = this->dyna.actor.scale.y = this->dyna.actor.scale.x = 0.1f;
}

void MmBgBotihasira_Destroy(Actor* thisx, PlayState* play) {
    MmBgBotihasira* this = (MmBgBotihasira*)thisx;

    if (this->dyna.actor.params == 0) {
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

void MmBgBotihasira_DoNothing(MmBgBotihasira* this, PlayState* play) {
}

void MmBgBotihasira_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    MmBgBotihasira* this = (MmBgBotihasira*)thisx;

    this->actionFunc(this, play);
    if (this->dyna.actor.params != 0) {
        this->dyna.actor.world.pos.x = this->dyna.actor.home.pos.x + (Math_SinS(this->dyna.actor.world.rot.y) * -27.0f);
        this->dyna.actor.world.pos.z = this->dyna.actor.home.pos.z + (Math_CosS(this->dyna.actor.world.rot.y) * 7.0f);
        Collider_UpdateCylinder(&this->dyna.actor, &this->collider);
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
        Math_Vec3f_Copy(&this->dyna.actor.world.pos, &this->dyna.actor.home.pos);
    }
}

void MmBgBotihasira_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_OPA_DISP++, object_botihasira_DL_000638);

    CLOSE_DISPS(play->state.gfxCtx);
}
