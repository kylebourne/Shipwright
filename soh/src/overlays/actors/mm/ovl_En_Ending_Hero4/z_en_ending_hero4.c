// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero4/z_en_ending_hero4.c by port_actor.py.
/*
 * File: z_en_ending_hero4.c
 * Overlay: ovl_En_Ending_Hero4
 * Description: Soldiers watching moon disappearance and Indigo-Go's
 */

#include "z_en_ending_hero4.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY)

void MmEnEndingHero4_Init(Actor* thisx, PlayState* play);
void MmEnEndingHero4_Destroy(Actor* thisx, PlayState* play);
void MmEnEndingHero4_Update(Actor* thisx, PlayState* play);
void MmEnEndingHero4_Draw(Actor* thisx, PlayState* play);

void MmEnEndingHero4_SetupIdle(MmEnEndingHero4* this);
void MmEnEndingHero4_Idle(MmEnEndingHero4* this, PlayState* play);

ActorInit Mm_En_Ending_Hero4_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnEndingHero4),
    MmEnEndingHero4_Init,
    MmEnEndingHero4_Destroy,
    MmEnEndingHero4_Update,
    MmEnEndingHero4_Draw,
};

void MmEnEndingHero4_Init(Actor* thisx, PlayState* play) {
    MmEnEndingHero4* this = (MmEnEndingHero4*)thisx;

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.targetMode = ATTENTION_RANGE_6;
    this->actor.gravity = -3.0f;
    SkelAnime_InitFlex(play, &this->skelAnime, &gSoldierSkel, &gSoldierCheerWithSpearAnim, this->jointTable,
                       this->morphTable, SOLDIER_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    MmEnEndingHero4_SetupIdle(this);
}

void MmEnEndingHero4_Destroy(Actor* thisx, PlayState* play) {
}

void MmEnEndingHero4_SetupIdle(MmEnEndingHero4* this) {
    this->isIdle = true;
    this->actionFunc = MmEnEndingHero4_Idle;
}

void MmEnEndingHero4_Idle(MmEnEndingHero4* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
}

void MmEnEndingHero4_Update(Actor* thisx, PlayState* play) {
    MmEnEndingHero4* this = (MmEnEndingHero4*)thisx;

    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 50.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
}

void MmEnEndingHero4_Draw(Actor* thisx, PlayState* play) {
    MmEnEndingHero4* this = (MmEnEndingHero4*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          NULL, &this->actor);
}
