// MM port: generated from the Majora's Mask decomp's ovl_En_Talk/z_en_talk.c by port_actor.py.
/*
 * File: z_en_talk.c
 * Overlay: ovl_En_Talk
 * Description: Green Target Spot (e.g., Indigo-Go's Poster)
 */

#include "z_en_talk.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY)

void MmEnTalk_Init(Actor* thisx, PlayState* play);
void MmEnTalk_Destroy(Actor* thisx, PlayState* play);
void MmEnTalk_Update(Actor* thisx, PlayState* play);
void MmEnTalk_func_80BDE058(MmEnTalk* this, PlayState* play);
void MmEnTalk_func_80BDE090(MmEnTalk* this, PlayState* play);

ActorInit Mm_En_Talk_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnTalk),
    MmEnTalk_Init,
    MmEnTalk_Destroy,
    MmEnTalk_Update,
    NULL,
};

void MmEnTalk_Init(Actor* thisx, PlayState* play) {
    MmEnTalk* this = (MmEnTalk*)thisx;
    s8 attentionRangeType = MMENTALK_GET_ATTENTION_RANGE_TYPE(&this->actor);

    if ((attentionRangeType >= ATTENTION_RANGE_0) && (attentionRangeType < ATTENTION_RANGE_7)) {
        this->actor.targetMode = attentionRangeType;
    }

    Actor_SetScale(&this->actor, 1.0f);
    this->actionFunc = MmEnTalk_func_80BDE090;
    this->actor.textId = MMENTALK_GET_TEXT_ID(&this->actor);
}

void MmEnTalk_Destroy(Actor* thisx, PlayState* play) {
}

void MmEnTalk_func_80BDE058(MmEnTalk* this, PlayState* play) {
    if (Actor_TextboxIsClosing(&this->actor, play)) {
        this->actionFunc = MmEnTalk_func_80BDE090;
    }
}

void MmEnTalk_func_80BDE090(MmEnTalk* this, PlayState* play) {
    if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        this->actionFunc = MmEnTalk_func_80BDE058;
        return;
    }

    if (((this->actor.xzDistToPlayer < 40.0f) && Player_IsFacingActor(&this->actor, 0x3000, play)) ||
        this->actor.isTargeted) {
        Actor_OfferTalk(&this->actor, play, 120.0f);
    }
}

void MmEnTalk_Update(Actor* thisx, PlayState* play) {
    MmEnTalk* this = (MmEnTalk*)thisx;

    this->actionFunc(this, play);
}
