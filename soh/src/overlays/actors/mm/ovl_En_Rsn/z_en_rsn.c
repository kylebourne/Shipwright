// MM port: generated from the Majora's Mask decomp's ovl_En_Rsn/z_en_rsn.c by port_actor.py.
/*
 * File: z_en_rsn.c
 * Overlay: ovl_En_Rsn
 * Description: Bomb Shop Man in the credits
 */

#include "z_en_rsn.h"
#include "mm/objects/object_rsn/object_rsn.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS                                                                                  \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_UPDATE_DURING_OCARINA)

void MmEnRsn_Init(Actor* thisx, PlayState* play);
void MmEnRsn_Destroy(Actor* thisx, PlayState* play);
void MmEnRsn_Update(Actor* thisx, PlayState* play);
void MmEnRsn_Draw(Actor* thisx, PlayState* play);

void MmEnRsn_DoNothing(MmEnRsn* this, PlayState* play);

ActorInit Mm_En_Rsn_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnRsn),
    MmEnRsn_Init,
    MmEnRsn_Destroy,
    MmEnRsn_Update,
    MmEnRsn_Draw,
};

typedef enum RsnAnimation {
    /* 0 */ RSN_ANIM_SWAY,
    /* 1 */ RSN_ANIM_MAX
} RsnAnimation;

static AnimationInfo sAnimationInfo[RSN_ANIM_MAX] = {
    { &gBombShopkeeperSwayAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // RSN_ANIM_SWAY
};

void MmEnRsn_func_80C25D40(MmEnRsn* this) {
    Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, RSN_ANIM_SWAY);
    this->actionFunc = MmEnRsn_DoNothing;
}

void MmEnRsn_DoNothing(MmEnRsn* this, PlayState* play) {
}

void MmEnRsn_Init(Actor* thisx, PlayState* play) {
    MmEnRsn* this = (MmEnRsn*)thisx;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gBombShopkeeperSkel, &gBombShopkeeperWalkAnim, NULL, NULL, 0);
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    MmEnRsn_func_80C25D40(this);
}

void MmEnRsn_Destroy(Actor* thisx, PlayState* play) {
    MmEnRsn* this = (MmEnRsn*)thisx;

    SkelAnime_Free(&this->skelAnime, play);
}

void MmEnRsn_Update(Actor* thisx, PlayState* play) {
    MmEnRsn* this = (MmEnRsn*)thisx;

    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->actor);
    SkelAnime_Update(&this->skelAnime);
    Actor_TrackPlayer(play, &this->actor, &this->headRot, &this->torsoRot, this->actor.focus.pos);
}

s32 MmEnRsn_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    MmEnRsn* this = (MmEnRsn*)thisx;

    if (limbIndex == BOMB_SHOPKEEPER_LIMB_RIGHT_HAND) {
        Matrix_RotateXS(this->headRot.y, MTXMODE_APPLY);
    }
    return false;
}

void MmEnRsn_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx) {
    MmEnRsn* this = (MmEnRsn*)thisx;
    Vec3f zeroVec = { 0.0f, 0.0f, 0.0f };

    if (limbIndex == BOMB_SHOPKEEPER_LIMB_RIGHT_HAND) {
        Matrix_MultVec3f(&zeroVec, &this->actor.focus.pos);
    }
}

void MmEnRsn_Draw(Actor* thisx, PlayState* play) {
    MmEnRsn* this = (MmEnRsn*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL37_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(gBombShopkeeperEyeTex));
    SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                          MmEnRsn_OverrideLimbDraw, MmEnRsn_PostLimbDraw, &this->actor);

    CLOSE_DISPS(play->state.gfxCtx);
}
