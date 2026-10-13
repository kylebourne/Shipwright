// MM port: generated from the Majora's Mask decomp's ovl_En_Ending_Hero/z_en_ending_hero.c by port_actor.py.
/*
 * File: z_en_ending_hero.c
 * Overlay: ovl_En_Ending_Hero
 * Description: Mayor Dotour at wedding during the credits
 */

#include "z_en_ending_hero.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY)

void MmEnEndingHero_Init(Actor* thisx, PlayState* play);
void MmEnEndingHero_Destroy(Actor* thisx, PlayState* play);
void MmEnEndingHero_Update(Actor* thisx, PlayState* play);
void MmEnEndingHero_Draw(Actor* thisx, PlayState* play);

void EnEndingHero1_SetupIdle(MmEnEndingHero* this);
void EnEndingHero1_Idle(MmEnEndingHero* this, PlayState* play);

ActorInit Mm_En_Ending_Hero_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnEndingHero),
    MmEnEndingHero_Init,
    MmEnEndingHero_Destroy,
    MmEnEndingHero_Update,
    MmEnEndingHero_Draw,
};

void MmEnEndingHero_Init(Actor* thisx, PlayState* play) {
    MmEnEndingHero* this = (MmEnEndingHero*)thisx;

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->actor, 0.01f);
    this->actor.targetMode = ATTENTION_RANGE_6;
    this->actor.gravity = -3.0f;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_dt_Skel_00B0CC, &gDotourUprightAnim, this->jointTable,
                       this->morphTable, OBJECT_DT_LIMB_MAX);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 25.0f);
    EnEndingHero1_SetupIdle(this);
}

void MmEnEndingHero_Destroy(Actor* thisx, PlayState* play) {
}

void EnEndingHero1_SetupIdle(MmEnEndingHero* this) {
    this->isIdle = true;
    this->actionFunc = EnEndingHero1_Idle;
}

void EnEndingHero1_Idle(MmEnEndingHero* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
}

void MmEnEndingHero_Update(Actor* thisx, PlayState* play) {
    MmEnEndingHero* this = (MmEnEndingHero*)thisx;

    if (this->unk240 == 0) {
        this->unk242++;
        if (this->unk242 > 2) {
            this->unk242 = 0;
            this->unk240 = TRUNCF_BINANG(Rand_ZeroFloat(60.0f)) + 0x14;
        }
    }
    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 50.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
}

static TexturePtr sEyeTextures[] = {
    gDotourEyeShockTex, gDotourEyeOpenTex, gDotourEyeClosedTex, gDotourEyeLookDownTex, gDotourEyeSquintTex,
};
static TexturePtr sEyebrowTextures[] = {
    gDotourEyebrowHighTex,
    gDotourEyebrowMidTex,
    gDotourEyebrowLowTex,
};

void MmEnEndingHero_Draw(Actor* thisx, PlayState* play) {
    MmEnEndingHero* this = (MmEnEndingHero*)thisx;
    s32 index = 0;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(sEyeTextures[this->unk242]));

    if (this->unk242 < 3) {
        index = this->unk242;
    }

    gSPSegment(POLY_OPA_DISP++, 0x09, Lib_SegmentedToVirtual(sEyebrowTextures[index]));
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount, NULL,
                          NULL, &this->actor);

    CLOSE_DISPS(play->state.gfxCtx);
}
