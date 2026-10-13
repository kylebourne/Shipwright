#ifndef MM_COMPAT_H
#define MM_COMPAT_H

/*
 * MM port compatibility layer.
 *
 * Majora's Mask actors ported into Ship of Harkinian include this header. It maps the Majora
 * decomp's names onto Ocarina's equivalents so ported code can stay close to the original.
 * Struct field renames (objectSlot, speed, cullingVolumeDistance, ...) are NOT handled here;
 * those are rewritten in the ported source itself.
 *
 * Include it LAST in a ported actor: where Majora reused an Ocarina function name with a different
 * signature, the macro below only affects code that comes after it.
 *
 * Grows as more actors are ported. Keep entries grouped by subsystem.
 */

#include "global.h"

// Majora's assets are extracted into mm.o2r with every path under mm/.
#define MM_ASSET(path) "__OTR__mm/" path

#ifndef M_PIf
#define M_PIf 3.14159265358979323846f
#endif

// --- Sound ---
// Sound effect ids resolve against Ocarina's sfx.h, so only sounds that exist in Ocarina play.
#define Actor_PlaySfx(actor, sfxId) Audio_PlayActorSound2((actor), (sfxId))

// Majora-only sounds get the closest Ocarina stand-in until Majora's sound banks are loaded.
#define NA_SE_EN_KUROSUKE_MOVE NA_SE_EN_FLOORMASTER_SLIDING // Boe skittering
#define NA_SE_EN_KUROSUKE_ATTACK NA_SE_EN_STAL_JUMP         // Boe lunge

// --- Graphics setup ---
#define Gfx_SetupDL25_Opa(gfxCtx) Gfx_SetupDL_25Opa(gfxCtx)
#define Gfx_SetupDL25_Xlu(gfxCtx) Gfx_SetupDL_25Xlu(gfxCtx)
#define MATRIX_FINALIZE_AND_LOAD(pkt, gfxCtx) \
    gSPMatrix(pkt, MATRIX_NEWMTX(gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW)

// --- Matrices ---
// Majora edits the current matrix in place through Matrix_GetCurrent(); Ocarina keeps it private,
// so read it, change the translation, and write it back.
static inline void MM_Matrix_SetTranslation(f32 x, f32 y, f32 z) {
    MtxF mf;

    Matrix_Get(&mf);
    mf.mf[3][0] = x;
    mf.mf[3][1] = y;
    mf.mf[3][2] = z;
    Matrix_Put(&mf);
}

// --- Shared constants ---
static Vec3f MM_gZeroVec3f = { 0.0f, 0.0f, 0.0f };
#define gZeroVec3f MM_gZeroVec3f

// --- Actor movement and background checks ---
#define Actor_MoveWithGravity(actor) Actor_MoveXZGravity(actor)
#define UPDBGCHECKINFO_FLAG_1 (1 << 0)  // check walls
#define UPDBGCHECKINFO_FLAG_2 (1 << 1)  // check ceiling
#define UPDBGCHECKINFO_FLAG_4 (1 << 2)  // check floor and water
#define UPDBGCHECKINFO_FLAG_8 (1 << 3)
#define UPDBGCHECKINFO_FLAG_10 (1 << 4)

// --- Dyna poly ---
#define DYNA_TRANSFORM_POS DPM_PLAYER

// --- Collision check: Majora renamed Ocarina's collider constants ---
#define COL_MATERIAL_HIT3 COLTYPE_HIT3
#define COL_MATERIAL_TREE COLTYPE_TREE
#define ELEM_MATERIAL_UNK0 ELEMTYPE_UNK0
#define ELEM_MATERIAL_UNK1 ELEMTYPE_UNK1
#define ATELEM_NONE TOUCH_NONE
#define ATELEM_ON TOUCH_ON
#define ATELEM_SFX_NORMAL TOUCH_SFX_NORMAL
#define ATELEM_SFX_HARD TOUCH_SFX_HARD
#define ACELEM_ON BUMP_ON

// Majora's 2-argument drop flag helper. Ocarina's takes a third "freeze" flag; Majora never froze here.
static inline void MM_Actor_SetDropFlag(Actor* actor, ColliderInfo* info) {
    Actor_SetDropFlag(actor, info, false);
}
#define Actor_SetDropFlag(actor, info) MM_Actor_SetDropFlag(actor, info)

// --- Effects ---
// Majora's func_800B0DE0 is Ocarina's func_8002829C: a normal dust puff.
#define func_800B0DE0 func_8002829C

// Majora passes colors by pointer and plays no sound from the effect itself.
static inline void MM_EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel,
                                           Color_RGBA8* prim, Color_RGBA8* env, s16 scale, s16 scaleStep,
                                           s32 life) {
    EffectSsDeadDb_Spawn(play, pos, velocity, accel, scale, scaleStep, prim->r, prim->g, prim->b, prim->a, env->r,
                         env->g, env->b, 1, life, false);
}
#define EffectSsDeadDb_Spawn MM_EffectSsDeadDb_Spawn

// --- Player state Majora added ---
// Ocarina has no Deku flower dive or Stone Mask, so these checks are always false.
#define PLAYER_STATE3_100 0                // Deku flower dive (Majora)
#define PLAYER_MASK_STONE 0xFF             // Stone Mask (Majora); never equals Player_GetMask()
#define PLAYER_STATE2_80 PLAYER_STATE2_GRABBED_BY_ENEMY // same bit (1 << 7) in both games

#endif // MM_COMPAT_H
