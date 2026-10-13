// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero3/z_en_ending_hero3.c by port_actor.py.
/*
 * File: z_en_ending_hero3.c
 * Overlay: ovl_En_Ending_Hero3
 * Description: Mutoh watching moon disappearance and wedding
 */

#include "z_en_ending_hero3.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY)

void MmEnEndingHero3_Init(Actor* thisx, PlayState* play);
void MmEnEndingHero3_Destroy(Actor* thisx, PlayState* play);
void MmEnEndingHero3_Update(Actor* thisx, PlayState* play);
void MmEnEndingHero3_Draw(Actor* thisx, PlayState* play);

void MmEnEndingHero3_SetupIdle(MmEnEndingHero3* this);
void MmEnEndingHero3_Idle(MmEnEndingHero3* this, PlayState* play);

ActorInit Mm_En_Ending_Hero3_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnEndingHero3),
    MmEnEndingHero3_Init,
    MmEnEndingHero3_Destroy,
    MmEnEndingHero3_Update,
    MmEnEndingHero3_Draw,
};

void MmEnEndingHero3_Init(Actor* thisx, PlayState* play) {
    MmEnEndingHero3* this = (MmEnEndingHero3*)thisx;

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.targetMode = ATTENTION_RANGE_6;
    this->actor.gravity = -3.0f;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_toryo_Skel_007150, &object_toryo_Anim_000E50, this->jointTable,
                       this->morphTable, OBJECT_TORYO_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    MmEnEndingHero3_SetupIdle(this);
}

void MmEnEndingHero3_Destroy(Actor* thisx, PlayState* play) {
}

void MmEnEndingHero3_SetupIdle(MmEnEndingHero3* this) {
    this->isIdle = true;
    this->actionFunc = MmEnEndingHero3_Idle;
}

void MmEnEndingHero3_Idle(MmEnEndingHero3* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
}

void MmEnEndingHero3_Update(Actor* thisx, PlayState* play) {
    MmEnEndingHero3* this = (MmEnEndingHero3*)thisx;

    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 50.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
}

void MmEnEndingHero3_Draw(Actor* thisx, PlayState* play) {
    MmEnEndingHero3* this = (MmEnEndingHero3*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          NULL, &this->actor);
}
