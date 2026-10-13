// MM port: generated from the Majora's Mask decomp's ovl_En_Hit_Tag/z_en_hit_tag.c by port_actor.py.
/*
 * File: z_en_hit_tag.c
 * Overlay: ovl_En_Hit_Tag
 * Description: Invisible hitbox that can spawn rupees
 */

#include "z_en_hit_tag.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void MmEnHitTag_Init(Actor* thisx, PlayState* play);
void MmEnHitTag_Destroy(Actor* thisx, PlayState* play);
void MmEnHitTag_Update(Actor* thisx, PlayState* play);

void MmEnHitTag_WaitForHit(MmEnHitTag* this, PlayState* play);

ActorInit Mm_En_Hit_Tag_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmEnHitTag),
    MmEnHitTag_Init,
    MmEnHitTag_Destroy,
    MmEnHitTag_Update,
    NULL,
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 16, 32, 0, { 0, 0, 0 } },
};

void MmEnHitTag_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    MmEnHitTag* this = (MmEnHitTag*)thisx;

    Actor_SetScale(&this->actor, 1.0f);
    this->actionFunc = MmEnHitTag_WaitForHit;
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    if (Flags_GetSwitch(play, MMENHITTAG_GET_SWITCH_FLAG(thisx))) {
        Actor_Kill(&this->actor);
    }
}

void MmEnHitTag_Destroy(Actor* thisx, PlayState* play) {
    MmEnHitTag* this = (MmEnHitTag*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void MmEnHitTag_WaitForHit(MmEnHitTag* this, PlayState* play) {
    Vec3f dropLocation;
    s32 i;

    if (this->collider.base.acFlags & AC_HIT) {
        Audio_PlaySfx(NA_SE_SY_GET_RUPY);
        Actor_Kill(&this->actor);
        dropLocation.x = this->actor.world.pos.x;
        dropLocation.y = this->actor.world.pos.y;
        dropLocation.z = this->actor.world.pos.z;

        for (i = 0; i < 3; i++) {
            Item_DropCollectible(play, &dropLocation, ITEM00_RUPEE_GREEN);
        }
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

void MmEnHitTag_Update(Actor* thisx, PlayState* play) {
    MmEnHitTag* this = (MmEnHitTag*)thisx;
    this->actionFunc(this, play);
}
