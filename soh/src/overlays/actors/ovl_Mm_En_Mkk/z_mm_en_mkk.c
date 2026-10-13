/*
 * File: z_mm_en_mkk.c
 * Overlay: ovl_Mm_En_Mkk (MM port)
 * Description: Black and White Boe (Name origin: Makkurokurosuke)
 *
 * Ported from the Majora's Mask decomp, ovl_En_Mkk. Differences from the original:
 * - Names are prefixed Mm so they can't clash with Ocarina code.
 * - Majora's single sphere collider is a one-sphere jointed collider, positioned by hand.
 * - The damage table and collider damage flags use Ocarina's layout (any weapon kills it).
 * - Majora-only checks (Stone Mask, Deku flower dive) are always false via mm_compat.h.
 * - Tatl hints are dropped; Majora's Boe sounds use Ocarina stand-ins from mm_compat.h.
 * - Field renames: speed -> speedXZ, playerHeightRel -> yDistToPlayer, terminalVelocity ->
 *   minVelocityY, lockOnArrowOffset -> targetArrowOffset.
 * - Registered at runtime through ActorDB (see soh/Enhancements/mm-port).
 * - If mm.o2r is missing the actor removes itself.
 */

#include "z_mm_en_mkk.h"
#include "mm_object_mkk.h"
#include "soh/ResourceManagerHelpers.h"
#include "mm_compat.h" // last: some Majora names shadow Ocarina ones

void MmEnMkk_WaitToAppear(MmEnMkk* this, PlayState* play);
void MmEnMkk_FadeIn(MmEnMkk* this, PlayState* play);
void MmEnMkk_Idle(MmEnMkk* this, PlayState* play);
void MmEnMkk_Wander(MmEnMkk* this, PlayState* play);
void MmEnMkk_Lunge(MmEnMkk* this, PlayState* play);
void MmEnMkk_Die(MmEnMkk* this, PlayState* play);
void MmEnMkk_Dissolve(MmEnMkk* this, PlayState* play);
void MmEnMkk_SetupFadeIn(MmEnMkk* this);
void MmEnMkk_SetupIdle(MmEnMkk* this);
void MmEnMkk_SetupWander(MmEnMkk* this);
void MmEnMkk_SetupLunge(MmEnMkk* this);
void MmEnMkk_SetupWaitToAppear(MmEnMkk* this);
void MmEnMkk_SetupDissolve(MmEnMkk* this);
void MmEnMkk_UpdateActionOnly(Actor* thisx, PlayState* play);
void MmEnMkk_DrawDissolve(Actor* thisx, PlayState* play);
void MmEnMkk_SetupDie(MmEnMkk* this);
void MmEnMkk_UpdateTail(MmEnMkk* this);
void MmEnMkk_CheckDamage(MmEnMkk* this, PlayState* play);

// Ocarina damage flags: hurt by, and hurting with, the usual "everything" set.
static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xFFCFFFFF, 0x00, 0x04 },
            { 0xFFCFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_HARD,
            ACELEM_ON,
            OCELEM_ON,
        },
        { 0, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_HIT3,
        AT_NONE | AT_TYPE_ENEMY,
        AC_NONE | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

typedef struct MmEnMkkDlists {
    /* 0x00 */ Gfx* material;
    /* 0x04 */ Gfx* model;
    /* 0x08 */ Gfx* end;
    /* 0x0C */ Gfx* eyes;
} MmEnMkkDlists;

static CollisionCheckInfoInit sColChkInfoInit = { 1, 15, 30, 10 };

// Ocarina's damage table layout. A Boe has 1 health, so any non-zero entry kills it.
static DamageTable sDamageTable = {
    /* Deku nut      */ DMG_ENTRY(1, 0x0),
    /* Deku stick    */ DMG_ENTRY(1, 0x0),
    /* Slingshot     */ DMG_ENTRY(1, 0x0),
    /* Explosive     */ DMG_ENTRY(1, 0x0),
    /* Boomerang     */ DMG_ENTRY(1, 0x0),
    /* Normal arrow  */ DMG_ENTRY(1, 0x0),
    /* Hammer swing  */ DMG_ENTRY(1, 0x0),
    /* Hookshot      */ DMG_ENTRY(1, 0x0),
    /* Kokiri sword  */ DMG_ENTRY(1, 0x0),
    /* Master sword  */ DMG_ENTRY(1, 0x0),
    /* Giant's Knife */ DMG_ENTRY(1, 0x0),
    /* Fire arrow    */ DMG_ENTRY(2, 0x0),
    /* Ice arrow     */ DMG_ENTRY(2, 0x0),
    /* Light arrow   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 1   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 2   */ DMG_ENTRY(2, 0x0),
    /* Unk arrow 3   */ DMG_ENTRY(2, 0x0),
    /* Fire magic    */ DMG_ENTRY(2, 0x0),
    /* Ice magic     */ DMG_ENTRY(2, 0x0),
    /* Light magic   */ DMG_ENTRY(2, 0x0),
    /* Shield        */ DMG_ENTRY(0, 0x0),
    /* Mirror Ray    */ DMG_ENTRY(0, 0x0),
    /* Kokiri spin   */ DMG_ENTRY(1, 0x0),
    /* Giant spin    */ DMG_ENTRY(1, 0x0),
    /* Master spin   */ DMG_ENTRY(1, 0x0),
    /* Kokiri jump   */ DMG_ENTRY(1, 0x0),
    /* Giant jump    */ DMG_ENTRY(1, 0x0),
    /* Master jump   */ DMG_ENTRY(1, 0x0),
    /* Unknown 1     */ DMG_ENTRY(0, 0x0),
    /* Unblockable   */ DMG_ENTRY(0, 0x0),
    /* Hammer jump   */ DMG_ENTRY(1, 0x0),
    /* Unknown 2     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32_DIV1000(gravity, -500, ICHAIN_CONTINUE),
    ICHAIN_F32(minVelocityY, -5, ICHAIN_CONTINUE),
    ICHAIN_F32(targetArrowOffset, 3000, ICHAIN_STOP),
};

static Color_RGBA8 sDustPrimColor = { 250, 250, 250, 255 };

static Color_RGBA8 sDustEnvColor = { 180, 180, 180, 255 };

static Vec3f sDustAccel = { 0.0f, 0.45f, 0.0f };

static Color_RGBA8 sEffPrimColors[] = { { 50, 50, 50, 255 }, { 255, 255, 255, 255 } };

static Color_RGBA8 sEffEnvColors[] = { { 200, 200, 200, 255 }, { 255, 255, 255, 255 } };

static MmEnMkkDlists sBoeDLists[] = {
    { (Gfx*)gBlackBoeBodyMaterialDL, (Gfx*)gBlackBoeBodyModelDL, (Gfx*)gBlackBoeEndDL, (Gfx*)gBlackBoeEyesDL },
    { (Gfx*)gWhiteBoeBodyMaterialDL, (Gfx*)gWhiteBoeBodyModelDL, (Gfx*)gWhiteBoeEndDL, (Gfx*)gWhiteBoeEyesDL },
};

// Eye blink colors. primColorSelect counts down from 6; the original read one entry past the end
// on the frame a blink starts, so the index is clamped.
static Color_RGBA8 sEyePrimColors[] = {
    { 255, 255, 255, 255 }, { 128, 128, 128, 255 }, { 0, 0, 0, 255 },
    { 0, 0, 0, 255 },       { 128, 128, 128, 255 }, { 255, 255, 255, 255 },
};

// Majora's func_800BE568: knock the Boe away from what hit it.
static void MmEnMkk_SetDeathYaw(MmEnMkk* this) {
    Actor* attacker = this->collider.base.ac;
    ColliderInfo* hitBy = this->collider.elements[0].info.acHitInfo;

    if (attacker == NULL) {
        return;
    }
    if ((hitBy != NULL) && (hitBy->toucher.dmgFlags & (0x10000 | 0x2000 | 0x1000 | 0x800 | 0x20))) {
        // Projectiles knock it along their direction of travel.
        this->actor.world.rot.y = attacker->shape.rot.y;
    } else {
        this->actor.world.rot.y = Actor_WorldYawTowardActor(attacker, &this->actor);
    }
}

void MmEnMkk_Init(Actor* thisx, PlayState* play) {
    MmEnMkk* this = (MmEnMkk*)thisx;
    s32 paramsFF00;
    s32 params2;

    Collider_InitJntSph(play, &this->collider);

    // MM port: without mm.o2r there is nothing to draw.
    if (!ResourceMgr_FileExists(gBlackBoeBodyModelDL)) {
        this->actionFunc = MmEnMkk_Idle;
        Actor_Kill(&this->actor);
        return;
    }

    Actor_ProcessInitChain(&this->actor, sInitChain);
    Collider_SetJntSph(play, &this->collider, &this->actor, &sJntSphInit, this->colliderElements);
    this->collider.elements[0].dim.worldSphere.radius = sJntSphElementsInit[0].dim.modelSphere.radius;
    CollisionCheck_SetInfo(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    ActorShape_Init(&this->actor.shape, 1000.0f, NULL, 0.0f);

    this->primColorSelect = 0;
    this->unk_149 = 0;
    this->unk_152 = (this->actor.shape.rot.x >= 0) ? this->actor.shape.rot.x : this->actor.shape.rot.x * -0x50;

    this->unk_14C = CLAMP(this->actor.shape.rot.z, 0, 16);

    this->actor.shape.rot.z = 0;
    this->actor.world.rot.z = 0;
    this->actor.shape.rot.x = 0;
    this->actor.world.rot.x = 0;

    Math_Vec3f_Copy(&this->unk_154, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk_160, &this->actor.world.pos);
    this->unk_14B = MMENMKK_GET_4(thisx) ? 8 : 0;

    paramsFF00 = MMENMKK_GET_FF00(&this->actor);
    params2 = MMENMKK_GET_2(&this->actor);
    this->actor.params &= 1;

    if ((paramsFF00 == 0) || (paramsFF00 == 255)) {
        MmEnMkk_SetupFadeIn(this);
        this->unk_178 = 30000.0f;
        if (params2 > 0) {
            this->unk_14B |= 4;
        }
    } else {
        this->unk_178 = paramsFF00 * 40.0f * 0.1f;
        MmEnMkk_SetupWaitToAppear(this);
        this->unk_14E = 0;
    }
}

void MmEnMkk_Destroy(Actor* thisx, PlayState* play) {
    MmEnMkk* this = (MmEnMkk*)thisx;

    Collider_DestroyJntSph(play, &this->collider);
}

void MmEnMkk_SetupFadeIn(MmEnMkk* this) {
    this->alpha = 0;
    this->unk_14B |= 3;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actionFunc = MmEnMkk_FadeIn;
}

void MmEnMkk_FadeIn(MmEnMkk* this, PlayState* play) {
    s32 newAlpha;

    if (this->unk_14B & 4) {
        newAlpha = this->alpha + 15;
    } else {
        newAlpha = this->alpha + 5;
    }
    if (newAlpha >= 255) {
        this->primColorSelect = 3;
        this->collider.base.acFlags |= AC_ON;
        this->alpha = 255;
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->actor.shape.rot.y = this->actor.yawTowardsPlayer;
        this->unk_14B &= ~4;
        MmEnMkk_SetupIdle(this);
    } else {
        this->alpha = newAlpha;
    }
}

void MmEnMkk_SetupIdle(MmEnMkk* this) {
    this->unk_14E = (s32)(Rand_ZeroOne() * 20.0f) + 10;
    if (this->unk_149 != 0) {
        this->unk_149--;
    }
    this->actionFunc = MmEnMkk_Idle;
}

void MmEnMkk_Idle(MmEnMkk* this, PlayState* play) {
    this->unk_14E--;
    if (this->unk_14E == 0) {
        MmEnMkk_SetupWander(this);
    }
}

void MmEnMkk_SpawnDust(MmEnMkk* this, PlayState* play) {
    Vec3f pos;

    pos.x = this->actor.world.pos.x;
    pos.y = this->actor.world.pos.y + 15.0f;
    pos.z = this->actor.world.pos.z;
    func_800B0DE0(play, &pos, &gZeroVec3f, &sDustAccel, &sDustPrimColor, &sDustEnvColor, 350, 20);
}

void MmEnMkk_SetupWander(MmEnMkk* this) {
    this->unk_14E = 30;
    this->unk_150 = this->actor.shape.rot.y;
    this->unk_14B &= ~1;
    this->actionFunc = MmEnMkk_Wander;
}

void MmEnMkk_Wander(MmEnMkk* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 stopped;

    this->unk_14E--;
    if ((this->actor.params == 1) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
        (this->actor.speedXZ > 2.5f) && ((play->gameplayFrames % 3) == 0)) {
        MmEnMkk_SpawnDust(this, play);
    }
    if (this->unk_14E > 0) {
        Math_StepToF(&this->actor.speedXZ, 5.0f, 0.7f);
        stopped = false;
    } else {
        stopped = Math_StepToF(&this->actor.speedXZ, 0.0f, 0.7f);
    }
    if ((player->stateFlags3 & PLAYER_STATE3_100) || (Player_GetMask(play) == PLAYER_MASK_STONE)) {
        Math_ScaledStepToS(&this->unk_150, Actor_WorldYawTowardPoint(&this->actor, &this->actor.home.pos), 0x400);
    } else if ((player->stateFlags2 & PLAYER_STATE2_80) || (player->actor.freezeTimer > 0)) {
        Math_ScaledStepToS(&this->unk_150, this->actor.yawTowardsPlayer + 0x8000, 0x400);
    } else {
        Math_ScaledStepToS(&this->unk_150, this->actor.yawTowardsPlayer, 0x400);
    }
    this->actor.shape.rot.y =
        (s32)(Math_SinF(this->unk_14E * ((2 * M_PIf) / 15)) * (614.4f * this->actor.speedXZ)) + this->unk_150;
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_KUROSUKE_MOVE - SFX_FLAG);
    if (stopped) {
        this->unk_14B &= ~2;
        MmEnMkk_SetupIdle(this);
    } else if ((this->unk_149 == 0) && !(player->stateFlags3 & PLAYER_STATE3_100) &&
               (Player_GetMask(play) != PLAYER_MASK_STONE) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) &&
               Actor_IsFacingPlayer(&this->actor, 0x1800) && (this->actor.xzDistToPlayer < 120.0f) &&
               (fabsf(this->actor.yDistToPlayer) < 100.0f)) {
        MmEnMkk_SetupLunge(this);
    }
}

void MmEnMkk_SetupLunge(MmEnMkk* this) {
    this->unk_14B |= 1;
    this->actor.speedXZ = 3.0f;
    this->actor.velocity.y = 5.0f;
    Actor_PlaySfx(&this->actor, NA_SE_EN_KUROSUKE_ATTACK);
    this->collider.base.atFlags |= AT_ON;
    Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0x800);
    this->actionFunc = MmEnMkk_Lunge;
}

void MmEnMkk_Lunge(MmEnMkk* this, PlayState* play) {
    if (this->collider.base.atFlags & AT_HIT) {
        this->collider.base.atFlags &= ~(AT_ON | AT_HIT);
    }
    if ((this->actor.velocity.y < 0.0f) && (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        this->unk_149 = 2;
        this->collider.base.atFlags &= ~AT_ON;
        MmEnMkk_SetupWander(this);
    }
}

void MmEnMkk_SetupDie(MmEnMkk* this) {
    this->unk_14B |= 1;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->collider.base.acFlags &= ~AC_ON;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    Actor_PlaySfx(&this->actor, NA_SE_EN_PO_DEAD);
    this->alpha = 254;
    MmEnMkk_SetDeathYaw(this);
    this->actor.speedXZ = 7.0f;
    this->actor.shape.rot.y = this->actor.world.rot.y;
    this->actor.velocity.y = 5.0f;
    this->actor.gravity = -1.3f;
    this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
    this->actionFunc = MmEnMkk_Die;
}

void MmEnMkk_Die(MmEnMkk* this, PlayState* play) {
    Vec3f temp;

    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (this->actor.velocity.y > -1.0f) {
            temp.x = this->actor.world.pos.x;
            temp.y = this->actor.world.pos.y + 15.0f;
            temp.z = this->actor.world.pos.z;
            EffectSsDeadDb_Spawn(play, &temp, &gZeroVec3f, &gZeroVec3f, &sEffPrimColors[this->actor.params],
                                 &sEffEnvColors[this->actor.params], 0x46, 4, 0xC);
            Actor_PlaySfx(&this->actor, NA_SE_EN_EXTINCT);
            if (this->unk_14C != 0) {
                Item_DropCollectibleRandom(play, &this->actor, &this->actor.world.pos, this->unk_14C * 0x10);
            }
            MmEnMkk_SetupDissolve(this);
        } else {
            this->actor.velocity.y *= -0.8f;
            this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        }
    }
}

// The Black Boe drags a two-puff tail behind it.
void MmEnMkk_UpdateTail(MmEnMkk* this) {
    if (this->unk_14B & 3) {
        Vec3f prevTail;
        f32 dist = Math_Vec3f_DistXYZ(&this->actor.world.pos, &this->actor.prevPos);

        Math_Vec3f_Copy(&prevTail, &this->unk_154);
        if (dist < 0.001f) {
            Math_Vec3f_Copy(&this->unk_154, &this->actor.prevPos);
        } else {
            dist = 1.0f / dist;

            this->unk_154.x = ((this->actor.prevPos.x - this->actor.world.pos.x) * dist * 10.0f) + this->actor.world.pos.x;
            this->unk_154.y = ((this->actor.prevPos.y - this->actor.world.pos.y) * dist * 10.0f) + this->actor.world.pos.y;
            this->unk_154.z = ((this->actor.prevPos.z - this->actor.world.pos.z) * dist * 10.0f) + this->actor.world.pos.z;
        }
        dist = Math_Vec3f_DistXYZ(&this->unk_154, &prevTail);
        if (dist < 0.001f) {
            Math_Vec3f_Copy(&this->unk_160, &prevTail);
        } else {
            dist = 1.0f / dist;

            this->unk_160.x = ((prevTail.x - this->unk_154.x) * dist * 12.0f) + this->unk_154.x;
            this->unk_160.y = ((prevTail.y - this->unk_154.y) * dist * 12.0f) + this->unk_154.y;
            this->unk_160.z = ((prevTail.z - this->unk_154.z) * dist * 12.0f) + this->unk_154.z;
        }
    } else {
        this->unk_160.y = this->unk_154.y;
        this->unk_154.y = this->actor.world.pos.y;
        this->unk_154.x =
            this->actor.world.pos.x -
            10.0f * Math_SinS(this->actor.shape.rot.y +
                              (s32)(1228.8f * this->actor.speedXZ * Math_SinF(this->unk_14E * (M_PIf / 5))));
        this->unk_154.z =
            this->actor.world.pos.z -
            10.0f * Math_CosS(this->actor.shape.rot.y +
                              (s32)(1228.8f * this->actor.speedXZ * Math_SinF(this->unk_14E * (M_PIf / 5))));
        this->unk_160.x =
            this->unk_154.x -
            12.0f * Math_SinS(this->actor.shape.rot.y -
                              (s32)(1228.8f * this->actor.speedXZ * Math_SinF(this->unk_14E * (M_PIf / 5))));
        this->unk_160.z =
            this->unk_154.z -
            12.0f * Math_CosS(this->actor.shape.rot.y -
                              (s32)(1228.8f * this->actor.speedXZ * Math_SinF(this->unk_14E * (M_PIf / 5))));
    }
}

void MmEnMkk_CheckDamage(MmEnMkk* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;
        Actor_SetDropFlag(&this->actor, &this->collider.elements[0].info);
        Enemy_StartFinishingBlow(play, &this->actor);
        MmEnMkk_SetupDie(this);
    }
}

void MmEnMkk_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    Player* player;
    MmEnMkk* this = (MmEnMkk*)thisx;
    Sphere16* sphere;

    if (this->primColorSelect > 0) {
        this->primColorSelect--;
    } else if ((Rand_ZeroOne() < 0.075f) && (this->actor.params != 1)) {
        this->primColorSelect = 6;
    }
    MmEnMkk_CheckDamage(this, play);
    this->actionFunc(this, play);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 20.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                UPDBGCHECKINFO_FLAG_10);
    if (this->actor.params == 0) {
        MmEnMkk_UpdateTail(this);
    }
    if (Actor_IsFacingPlayer(&this->actor, 0x3000)) {
        player = GET_PLAYER(play);
        this->actor.shape.rot.x = Actor_WorldPitchTowardPoint(&this->actor, &player->actor.focus.pos);
        this->actor.shape.rot.x = CLAMP(this->actor.shape.rot.x, -0x1800, 0x1800);
    }
    Actor_SetFocus(&this->actor, 10.0f);
    sphere = &this->collider.elements[0].dim.worldSphere;
    sphere->center.x = this->actor.focus.pos.x;
    sphere->center.y = this->actor.focus.pos.y;
    sphere->center.z = this->actor.focus.pos.z;
    if (this->collider.base.atFlags & AT_ON) {
        CollisionCheck_SetAT(play, &play->colChkCtx, &this->collider.base);
    }
    if (this->collider.base.acFlags & AC_ON) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void MmEnMkk_SetupWaitToAppear(MmEnMkk* this) {
    this->alpha = 0;
    this->unk_14B |= 3;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    this->actor.draw = NULL;
    this->actor.update = MmEnMkk_UpdateActionOnly;
    this->actor.gravity = -0.5f;
    this->unk_14E = this->unk_152;
    this->actionFunc = MmEnMkk_WaitToAppear;
}

void MmEnMkk_WaitToAppear(MmEnMkk* this, PlayState* play) {
    if (this->unk_14E > 0) {
        this->unk_14E--;
        if (this->unk_14E == 0) {
            this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        }
    } else if ((this->actor.xzDistToPlayer < this->unk_178) && (Player_GetMask(play) != PLAYER_MASK_STONE)) {
        this->actor.update = MmEnMkk_Update;
        this->actor.draw = MmEnMkk_Draw;
        this->actor.flags &= ~ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        MmEnMkk_SetupFadeIn(this);
    }
}

void MmEnMkk_SetupDissolve(MmEnMkk* this) {
    Math_Vec3f_Copy(&this->unk_16C, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk_154, &this->actor.world.pos);
    Math_Vec3f_Copy(&this->unk_160, &this->actor.world.pos);
    Actor_SetScale(&this->actor, 0.005f);
    this->actor.update = MmEnMkk_UpdateActionOnly;
    this->actor.draw = MmEnMkk_DrawDissolve;
    this->actionFunc = MmEnMkk_Dissolve;
}

void MmEnMkk_Dissolve(MmEnMkk* this, PlayState* play) {
    s32 newAlpha = this->alpha - 20;

    if (newAlpha <= 0) {
        if (this->unk_14B & 8) {
            // Respawning Boes go back home and wait to reappear.
            Actor_SetScale(&this->actor, 0.01f);
            this->primColorSelect = 0;
            this->unk_149 = 0;
            Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
            Math_Vec3f_Copy(&this->unk_154, &this->actor.world.pos);
            Math_Vec3f_Copy(&this->unk_160, &this->actor.world.pos);
            this->actor.speedXZ = 0.0f;
            this->actor.velocity.y = 0.0f;
            MmEnMkk_SetupWaitToAppear(this);
        } else {
            Actor_Kill(&this->actor);
        }
    } else {
        this->alpha = newAlpha;
        this->actor.world.pos.x += 0.3f + (3.0f * Rand_ZeroOne());
        this->actor.world.pos.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk_16C.x -= 0.3f + (3.0f * Rand_ZeroOne());
        this->unk_16C.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk_154.z += 0.3f + (3.0f * Rand_ZeroOne());
        this->unk_154.y += 0.5f + (3.5f * Rand_ZeroOne());
        this->unk_160.z -= 0.3f + (3.0f * Rand_ZeroOne());
        this->unk_160.y += 0.5f + (3.5f * Rand_ZeroOne());
    }
}

void MmEnMkk_UpdateActionOnly(Actor* thisx, PlayState* play) {
    MmEnMkk* this = (MmEnMkk*)thisx;

    this->actionFunc(this, play);
}

void MmEnMkk_Draw(Actor* thisx, PlayState* play) {
    MmEnMkk* this = (MmEnMkk*)thisx;
    MmEnMkkDlists* dLists = &sBoeDLists[thisx->params];

    if (this->actor.projectedPos.z > 0.0f) {
        OPEN_DISPS(play->state.gfxCtx);

        if (this->alpha == 255) {
            Color_RGBA8* primColors = &sEyePrimColors[MIN(this->primColorSelect, ARRAY_COUNT(sEyePrimColors) - 1)];

            Gfx_SetupDL25_Opa(play->state.gfxCtx);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0xFF, primColors->r, primColors->g, primColors->b, primColors->a);
            gSPSegment(POLY_OPA_DISP++, 0x08, (uintptr_t)gEmptyDL);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_OPA_DISP++, dLists->eyes);
        }

        Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, this->alpha);
        gSPDisplayList(POLY_XLU_DISP++, dLists->material);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        gSPDisplayList(POLY_XLU_DISP++, dLists->model);
        if (thisx->params == 0) {
            MM_Matrix_SetTranslation(this->unk_154.x, this->unk_154.y + 8.5f, this->unk_154.z);

            Matrix_Scale(0.85f, 0.85f, 0.85f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, dLists->model);

            MM_Matrix_SetTranslation(this->unk_160.x, this->unk_160.y + 7.2250004f, this->unk_160.z);

            Matrix_Scale(0.85f, 0.85f, 0.85f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
            gSPDisplayList(POLY_XLU_DISP++, dLists->model);
        }
        gSPDisplayList(POLY_XLU_DISP++, dLists->end);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void MmEnMkk_DrawDissolve(Actor* thisx, PlayState* play) {
    MmEnMkk* this = (MmEnMkk*)thisx;
    MmEnMkkDlists* dLists = &sBoeDLists[thisx->params];

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 255, this->alpha);
    gSPDisplayList(POLY_XLU_DISP++, dLists->material);
    Matrix_ReplaceRotation(&play->billboardMtxF);
    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, dLists->model);

    MM_Matrix_SetTranslation(this->unk_154.x, this->unk_154.y + 5.0f, this->unk_154.z);

    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, dLists->model);

    MM_Matrix_SetTranslation(this->unk_160.x, this->unk_160.y + 5.0f, this->unk_160.z);

    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, dLists->model);

    MM_Matrix_SetTranslation(this->unk_16C.x, this->unk_16C.y + 5.0f, this->unk_16C.z);

    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
    gSPDisplayList(POLY_XLU_DISP++, dLists->model);
    gSPDisplayList(POLY_XLU_DISP++, dLists->end);

    CLOSE_DISPS(play->state.gfxCtx);
}
