// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero5/z_en_ending_hero5.c by port_actor.py.
/*
 * File: z_en_ending_hero5.c
 * Overlay: ovl_En_Ending_Hero5
 * Description: Carpenters watching moon disappearance and Indigo-Go's
 */

#include "z_en_ending_hero5.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY)

void MmEnEndingHero5_Init(Actor* thisx, PlayState* play);
void MmEnEndingHero5_Destroy(Actor* thisx, PlayState* play);
void MmEnEndingHero5_Update(Actor* thisx, PlayState* play);
void MmEnEndingHero5_Draw(Actor* thisx, PlayState* play);

void MmEnEndingHero5_SetupIdle(MmEnEndingHero5* this);
void MmEnEndingHero5_Idle(MmEnEndingHero5* this, PlayState* play);

ActorInit Mm_En_Ending_Hero5_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnEndingHero5),
    MmEnEndingHero5_Init,
    MmEnEndingHero5_Destroy,
    MmEnEndingHero5_Update,
    MmEnEndingHero5_Draw,
};

void MmEnEndingHero5_Init(Actor* thisx, PlayState* play) {
    MmEnEndingHero5* this = (MmEnEndingHero5*)thisx;

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.targetMode = ATTENTION_RANGE_6;
    this->actor.gravity = -3.0f;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_daiku_Skel_00A850, &object_daiku_Anim_002FA0, this->jointTable,
                       this->morphTable, OBJECT_DAIKU_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    this->unk25C = this->actor.params;
    MmEnEndingHero5_SetupIdle(this);
}

void MmEnEndingHero5_Destroy(Actor* thisx, PlayState* play) {
}

void MmEnEndingHero5_SetupIdle(MmEnEndingHero5* this) {
    this->isIdle = true;
    this->actionFunc = MmEnEndingHero5_Idle;
}

void MmEnEndingHero5_Idle(MmEnEndingHero5* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
}

void MmEnEndingHero5_Update(Actor* thisx, PlayState* play) {
    MmEnEndingHero5* this = (MmEnEndingHero5*)thisx;

    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 50.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
}

Gfx* D_80C23BF0[] = { object_daiku_DL_0070C0, object_daiku_DL_006FB0, object_daiku_DL_006E80, object_daiku_DL_006D70,
                      object_daiku_DL_00A390 };

void MmEnEndingHero5_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    MmEnEndingHero5* this = (MmEnEndingHero5*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    if (limbIndex == OBJECT_DAIKU_LIMB_0F) {
        gSPDisplayList(POLY_OPA_DISP++, D_80C23BF0[this->unk25C]);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

void MmEnEndingHero5_Draw(Actor* thisx, PlayState* play) {
    MmEnEndingHero5* this = (MmEnEndingHero5*)thisx;

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    OPEN_DISPS(play->state.gfxCtx);

    switch (this->unk25C) {
        case 0:
            gDPSetEnvColor(POLY_OPA_DISP++, 170, 10, 70, 255);
            break;

        case 1:
            gDPSetEnvColor(POLY_OPA_DISP++, 170, 200, 255, 255);
            break;

        case 2:
            gDPSetEnvColor(POLY_OPA_DISP++, 0, 230, 70, 255);
            break;

        case 3:
            gDPSetEnvColor(POLY_OPA_DISP++, 200, 0, 150, 255);
            break;

        case 4:
            gDPSetEnvColor(POLY_OPA_DISP++, 245, 155, 0, 255);
            break;

        default:
            break;
    }

    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          MmEnEndingHero5_PostLimbDraw, &this->actor);

    CLOSE_DISPS(play->state.gfxCtx);
}
