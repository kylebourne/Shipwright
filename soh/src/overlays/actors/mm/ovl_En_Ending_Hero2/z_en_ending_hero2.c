// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero2/z_en_ending_hero2.c by port_actor.py.
/*
 * File: z_en_ending_hero2.c
 * Overlay: ovl_En_Ending_Hero2
 * Description: Viscen watching moon disappearance and wedding
 */

#include "z_en_ending_hero2.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY)

void MmEnEndingHero2_Init(Actor* thisx, PlayState* play);
void MmEnEndingHero2_Destroy(Actor* thisx, PlayState* play);
void MmEnEndingHero2_Update(Actor* thisx, PlayState* play);
void MmEnEndingHero2_Draw(Actor* thisx, PlayState* play);

void MmEnEndingHero2_SetupIdle(MmEnEndingHero2* this);
void MmEnEndingHero2_Idle(MmEnEndingHero2* this, PlayState* play);

ActorInit Mm_En_Ending_Hero2_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnEndingHero2),
    MmEnEndingHero2_Init,
    MmEnEndingHero2_Destroy,
    MmEnEndingHero2_Update,
    MmEnEndingHero2_Draw,
};

void MmEnEndingHero2_Init(Actor* thisx, PlayState* play) {
    MmEnEndingHero2* this = (MmEnEndingHero2*)thisx;

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.targetMode = ATTENTION_RANGE_6;
    this->actor.gravity = -3.0f;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_bai_Skel_007908, &object_bai_Anim_0011C0, this->jointTable,
                       this->morphTable, OBJECT_BAI_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    MmEnEndingHero2_SetupIdle(this);
}

void MmEnEndingHero2_Destroy(Actor* thisx, PlayState* play) {
}

void MmEnEndingHero2_SetupIdle(MmEnEndingHero2* this) {
    this->isIdle = true;
    this->actionFunc = MmEnEndingHero2_Idle;
}

void MmEnEndingHero2_Idle(MmEnEndingHero2* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
}

void MmEnEndingHero2_Update(Actor* thisx, PlayState* play) {
    MmEnEndingHero2* this = (MmEnEndingHero2*)thisx;

    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 50.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
}

void MmEnEndingHero2_Draw(Actor* thisx, PlayState* play) {
    MmEnEndingHero2* this = (MmEnEndingHero2*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          NULL, &this->actor);
}
