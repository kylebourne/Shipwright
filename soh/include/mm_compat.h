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

// ============================================================================================
// v2 (M3): shims found by compiling every Majora actor against Ocarina's headers.
// ============================================================================================

// --- Math macros ---
#ifndef ABS_ALT
#define ABS_ALT(x) ((x) < 0 ? -(x) : (x))
#endif
#define TRUNCF_BINANG(f) (s16)(s32)(f)
#define DEG_TO_BINANG_ALT(degrees) TRUNCF_BINANG(((degrees) / 180.0f) * 0x8000)
#define DEG_TO_BINANG_ALT2(degrees) TRUNCF_BINANG(((degrees) * 0x10000) / 360.0f)
#define DEG_TO_BINANG_ALT3(degrees) ((degrees) * (0x8000 / 180.0f))
#ifndef BINANG_ADD
#define BINANG_ADD(a, b) ((s16)((a) + (b)))
#endif
#ifndef BINANG_SUB
#define BINANG_SUB(a, b) ((s16)((a) - (b)))
#endif

typedef void* TexturePtr;
typedef union {
    struct {
        u8 r, g, b, a;
    };
    u32 rgba;
} Color_RGBAu32;

// --- Segments ---
#define Lib_SegmentedToVirtual(ptr) SEGMENTED_TO_VIRTUAL(ptr)
#define Lib_SegmentedToVirtualNull(ptr) SEGMENTED_TO_VIRTUAL(ptr)

// --- Matrices: Majora's binary-angle (S) and float (F) forms of Ocarina's radian rotations ---
#define Matrix_RotateXS(x, mode) Matrix_RotateX(BINANG_TO_RAD(x), mode)
#define Matrix_RotateYS(y, mode) Matrix_RotateY(BINANG_TO_RAD(y), mode)
#define Matrix_RotateZS(z, mode) Matrix_RotateZ(BINANG_TO_RAD(z), mode)
#define Matrix_RotateXF(x, mode) Matrix_RotateX(x, mode)
#define Matrix_RotateYF(y, mode) Matrix_RotateY(y, mode)
#define Matrix_RotateZF(z, mode) Matrix_RotateZ(z, mode)
#define Matrix_RotateXFApply(x) Matrix_RotateX(x, MTXMODE_APPLY)
#define Matrix_RotateXFNew(x) Matrix_RotateX(x, MTXMODE_NEW)
#define Matrix_RotateAxisS(angle, axis, mode) Matrix_RotateAxis(BINANG_TO_RAD(angle), axis, mode)
#define Matrix_RotateAxisF(angle, axis, mode) Matrix_RotateAxis(angle, axis, mode)
#define Matrix_MtxFToYXZRot(src, dest, flag) Matrix_MtxFToYXZRotS(src, dest, flag)

static inline void MM_Matrix_MultVecXYZ(f32 x, f32 y, f32 z, Vec3f* dest) {
    Vec3f src = { x, y, z };
    Matrix_MultVec3f(&src, dest);
}
#define Matrix_MultZero(dest) MM_Matrix_MultVecXYZ(0.0f, 0.0f, 0.0f, dest)
#define Matrix_MultVecX(x, dest) MM_Matrix_MultVecXYZ(x, 0.0f, 0.0f, dest)
#define Matrix_MultVecY(y, dest) MM_Matrix_MultVecXYZ(0.0f, y, 0.0f, dest)
#define Matrix_MultVecZ(z, dest) MM_Matrix_MultVecXYZ(0.0f, 0.0f, z, dest)

// --- Graphics setup (Majora names -> Ocarina names) ---
#define Gfx_SetupDL37_Opa(gfxCtx) Gfx_SetupDL_37Opa(gfxCtx)
#define Gfx_SetupDL44_Xlu(gfxCtx) Gfx_SetupDL_44Xlu(gfxCtx)
#define Gfx_SetupDL39_Overlay(gfxCtx) Gfx_SetupDL_39Overlay(gfxCtx)
#define Gfx_SetupDL20_NoCD(gfxCtx) Gfx_SetupDL_20NoCD(gfxCtx)
#define Gfx_SetupDL57(gfxCtx) Gfx_SetupDL_57(gfxCtx)
#define Gfx_SetupDL66(gfxCtx) Gfx_SetupDL_66(gfxCtx)
#define GRAPH_ALLOC(gfxCtx, size) Graph_Alloc(gfxCtx, size)
#define func_800B8050(actor, play, flag) func_8002EBCC(actor, play, flag) // hilite, opaque
#define func_800B8118(actor, play, flag) func_8002ED80(actor, play, flag) // hilite, translucent

// --- Actor movement ---
#define Actor_MoveWithoutGravity(actor) Actor_MoveXYZ(actor)
#define Actor_UpdateVelocityWithoutGravity(actor) Actor_UpdateVelocityXYZ(actor)
#define Actor_UpdateVelocityWithGravity(actor) Actor_UpdateVelocityXZGravity(actor)

// --- Dyna poly ---
static inline void MM_DynaPolyActor_LoadMesh(PlayState* play, DynaPolyActor* dyna, CollisionHeader* meshHeader) {
    CollisionHeader* header = NULL;

    CollisionHeader_GetVirtual(meshHeader, &header);
    dyna->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &dyna->actor, header);
}
#define DynaPolyActor_LoadMesh(play, dyna, mesh) MM_DynaPolyActor_LoadMesh(play, dyna, (CollisionHeader*)(mesh))
#define DynaPoly_DisableCollision(play, dyna, bgId) func_8003EBF8(play, dyna, bgId)
#define DynaPoly_EnableCollision(play, dyna, bgId) func_8003EC50(play, dyna, bgId)

// --- Talking ---
#define Actor_TalkOfferAccepted(actor, gameState) Actor_ProcessTalkRequest(actor, (PlayState*)(gameState))

// --- Animation ---
#define Actor_ChangeAnimationByInfo(skelAnime, animInfo, index) Animation_ChangeByInfo(skelAnime, animInfo, index)

// --- Player knockback: Majora numbers the strengths one higher than Ocarina ---
#define func_800B8D50(play, actor, speed, yaw, yVel, damage) \
    Actor_SetPlayerKnockbackLarge(play, actor, speed, yaw, yVel, damage)
#define func_800B8D98(play, actor, speed, yaw, yVel) Actor_SetPlayerKnockbackLargeNoDamage(play, actor, speed, yaw, yVel)
#define func_800B8DD4(play, actor, speed, yaw, yVel, damage) \
    Actor_SetPlayerKnockbackSmall(play, actor, speed, yaw, yVel, damage)
#define func_800B8E1C(play, actor, speed, yaw, yVel) Actor_SetPlayerKnockbackSmallNoDamage(play, actor, speed, yaw, yVel)

// --- Sound ---
#define Audio_PlaySfx(sfxId) Sfx_PlaySfxCentered(sfxId)

// --- Colliders: remaining Majora names, one-to-one with Ocarina's ---
#define COL_MATERIAL_HIT0 COLTYPE_HIT0
#define COL_MATERIAL_HIT1 COLTYPE_HIT1
#define COL_MATERIAL_HIT2 COLTYPE_HIT2
#define COL_MATERIAL_HIT4 COLTYPE_HIT4
#define COL_MATERIAL_HIT5 COLTYPE_HIT5
#define COL_MATERIAL_HIT6 COLTYPE_HIT6
#define COL_MATERIAL_HIT7 COLTYPE_HIT7
#define COL_MATERIAL_HIT8 COLTYPE_HIT8
#define COL_MATERIAL_METAL COLTYPE_METAL
#define COL_MATERIAL_NONE COLTYPE_NONE
#define COL_MATERIAL_WOOD COLTYPE_WOOD
#define COL_MATERIAL_HARD COLTYPE_HARD
#define ELEM_MATERIAL_UNK2 ELEMTYPE_UNK2
#define ELEM_MATERIAL_UNK3 ELEMTYPE_UNK3
#define ELEM_MATERIAL_UNK4 ELEMTYPE_UNK4
#define ELEM_MATERIAL_UNK5 ELEMTYPE_UNK5
#define ELEM_MATERIAL_UNK6 ELEMTYPE_UNK6
#define ELEM_MATERIAL_UNK7 ELEMTYPE_UNK7
#define ATELEM_HIT TOUCH_HIT
#define ATELEM_NEAREST TOUCH_NEAREST
#define ATELEM_SFX_WOOD TOUCH_SFX_WOOD
#define ATELEM_SFX_NONE TOUCH_SFX_NONE
#define ATELEM_AT_HITMARK TOUCH_AT_HITMARK
#define ATELEM_DREW_HITMARK TOUCH_DREW_HITMARK
#define ATELEM_UNK7 TOUCH_UNK7
#define ACELEM_NONE 0
#define ACELEM_HIT BUMP_HIT
#define ACELEM_HOOKABLE BUMP_HOOKABLE
#define ACELEM_NO_AT_INFO BUMP_NO_AT_INFO
#define ACELEM_NO_DAMAGE BUMP_NO_DAMAGE
#define ACELEM_NO_SWORD_SFX BUMP_NO_SWORD_SFX
#define ACELEM_NO_HITMARK BUMP_NO_HITMARK
#define ACELEM_DRAW_HITMARK BUMP_DRAW_HITMARK
#ifndef OCELEM_UNK2
#define OCELEM_UNK2 (1 << 2)
#endif

static inline s32 MM_Collider_InitAndSetCylinder(PlayState* play, ColliderCylinder* dest, Actor* actor,
                                                 ColliderCylinderInit* src) {
    Collider_InitCylinder(play, dest);
    Collider_SetCylinder(play, dest, actor, src);
    return 1;
}
#define Collider_InitAndSetCylinder MM_Collider_InitAndSetCylinder
static inline s32 MM_Collider_InitAndSetJntSph(PlayState* play, ColliderJntSph* dest, Actor* actor,
                                               ColliderJntSphInit* src, ColliderJntSphElement* elements) {
    Collider_InitJntSph(play, dest);
    Collider_SetJntSph(play, dest, actor, src, elements);
    return 0;
}
#define Collider_InitAndSetJntSph MM_Collider_InitAndSetJntSph
static inline s32 MM_Collider_InitAndSetTris(PlayState* play, ColliderTris* dest, Actor* actor, ColliderTrisInit* src,
                                             ColliderTrisElement* elements) {
    Collider_InitTris(play, dest);
    Collider_SetTris(play, dest, actor, src, elements);
    return 0;
}
#define Collider_InitAndSetTris MM_Collider_InitAndSetTris
static inline s32 MM_Collider_InitAndSetQuad(PlayState* play, ColliderQuad* dest, Actor* actor, ColliderQuadInit* src) {
    Collider_InitQuad(play, dest);
    Collider_SetQuad(play, dest, actor, src);
    return 0;
}
#define Collider_InitAndSetQuad MM_Collider_InitAndSetQuad

s32 Collider_InitJntSphElement(PlayState* play, ColliderJntSphElement* element); // defined in z_collision_check.c

// Majora's sphere collider, which Ocarina doesn't have, as a one-element jointed sphere with the same
// memory layout: `.base` is the Collider, `.info` and `.dim` alias the single element.
typedef struct ColliderSphere {
    Collider base;
    s32 count;
    ColliderJntSphElement* elements;
    union {
        ColliderJntSphElement element;
        struct {
            ColliderInfo info;
            ColliderJntSphElementDim dim;
        };
    };
} ColliderSphere;

typedef struct ColliderSphereInit {
    ColliderInit base;
    ColliderInfoInit info; // with dim below, laid out exactly like ColliderJntSphElementInit
    ColliderJntSphElementDimInit dim;
} ColliderSphereInit;

#define COLSHAPE_SPHERE COLSHAPE_JNTSPH

static inline s32 MM_Collider_InitSphere(PlayState* play, ColliderSphere* sph) {
    Collider_InitJntSph(play, (ColliderJntSph*)sph);
    sph->count = 1;
    sph->elements = &sph->element;
    Collider_InitJntSphElement(play, &sph->element);
    return 1;
}
static inline s32 MM_Collider_SetSphere(PlayState* play, ColliderSphere* sph, Actor* actor, ColliderSphereInit* src) {
    ColliderJntSphInit init;

    init.base = src->base;
    init.count = 1;
    init.elements = (ColliderJntSphElementInit*)&src->info;
    return Collider_SetJntSph(play, (ColliderJntSph*)sph, actor, &init, &sph->element);
}
static inline s32 MM_Collider_InitAndSetSphere(PlayState* play, ColliderSphere* sph, Actor* actor,
                                               ColliderSphereInit* src) {
    MM_Collider_InitSphere(play, sph);
    return MM_Collider_SetSphere(play, sph, actor, src);
}
#define Collider_InitSphere MM_Collider_InitSphere
#define Collider_SetSphere MM_Collider_SetSphere
#define Collider_InitAndSetSphere MM_Collider_InitAndSetSphere
#define Collider_DestroySphere(play, sph) Collider_DestroyJntSph(play, (ColliderJntSph*)(sph))
#define Collider_UpdateSphere(limb, sph) Collider_UpdateSpheres(limb, (ColliderJntSph*)(sph))

// --- Damage flags: Majora names on Ocarina's bits (no Ocarina equivalent -> 0) ---
#define DMG_HORSE_TRAMPLE 0
#define DMG_EXPLOSIVES DMG_EXPLOSIVE
#define DMG_ZORA_BOOMERANG DMG_BOOMERANG
#define DMG_NORMAL_ARROW DMG_ARROW_NORMAL
#define DMG_GORON_PUNCH DMG_HAMMER_SWING
#define DMG_GORON_POUND DMG_HAMMER_JUMP
#define DMG_FIRE_ARROW DMG_ARROW_FIRE
#define DMG_ICE_ARROW DMG_ARROW_ICE
#define DMG_LIGHT_ARROW DMG_ARROW_LIGHT
#define DMG_GORON_SPIKES 0
#define DMG_DEKU_SPIN 0
#define DMG_DEKU_BUBBLE DMG_SLINGSHOT
#define DMG_DEKU_LAUNCH 0
#define DMG_ZORA_BARRIER 0
#define DMG_NORMAL_SHIELD DMG_SHIELD
#define DMG_LIGHT_RAY DMG_MIR_RAY
#define DMG_THROWN_OBJECT 0
#define DMG_ZORA_PUNCH 0
#define DMG_SWORD_BEAM 0
#define DMG_NORMAL_ROLL 0
#define DMG_POWDER_KEG DMG_EXPLOSIVE
#define DMG_UNK_0x06 0
#define DMG_UNK_0x12 0
#define DMG_UNK_0x1B 0
#define DMG_UNK_0x1C 0
#define DMG_UNK_0x1E 0

// --- Lock-on ranges: Majora's 0-9 match Ocarina's target modes; 10 is new ---
#define ATTENTION_RANGE_0 0
#define ATTENTION_RANGE_1 1
#define ATTENTION_RANGE_2 2
#define ATTENTION_RANGE_3 3
#define ATTENTION_RANGE_4 4
#define ATTENTION_RANGE_5 5
#define ATTENTION_RANGE_6 6
#define ATTENTION_RANGE_7 7
#define ATTENTION_RANGE_8 8
#define ATTENTION_RANGE_9 9
#define ATTENTION_RANGE_10 5 // 2500 in Majora; Ocarina's longest regular range

// --- Colour filter flags (same values in both games) ---
#define COLORFILTER_COLORFLAG_NONE 0xC000
#define COLORFILTER_COLORFLAG_GRAY 0x8000
#define COLORFILTER_COLORFLAG_RED 0x4000
#define COLORFILTER_COLORFLAG_BLUE 0x0000
#define COLORFILTER_INTENSITY_FLAG 0x8000
#define COLORFILTER_BUFFLAG_XLU 0x2000
#define COLORFILTER_BUFFLAG_OPA 0x0000

// --- Skeleton drawing: Majora's limb callbacks take Actor*, Ocarina's take void*; same calls otherwise ---
#define SkelAnime_DrawOpa(play, skel, jointTable, override, post, actor) \
    SkelAnime_DrawOpa(play, skel, jointTable, (OverrideLimbDrawOpa)(override), (PostLimbDrawOpa)(post), actor)
#define SkelAnime_DrawFlexOpa(play, skel, jointTable, dListCount, override, post, actor)                   \
    SkelAnime_DrawFlexOpa(play, skel, jointTable, dListCount, (OverrideLimbDrawOpa)(override), \
                          (PostLimbDrawOpa)(post), actor)
#define SkelAnime_DrawLod(play, skel, jointTable, override, post, actor, lod) \
    SkelAnime_DrawLod(play, skel, jointTable, (OverrideLimbDrawOpa)(override), (PostLimbDrawOpa)(post), actor, lod)
#define SkelAnime_DrawFlexLod(play, skel, jointTable, dListCount, override, post, actor, lod)                   \
    SkelAnime_DrawFlexLod(play, skel, jointTable, dListCount, (OverrideLimbDrawOpa)(override), \
                          (PostLimbDrawOpa)(post), actor, lod)
#define SkelAnime_Draw(play, skel, jointTable, override, post, actor, gfx) \
    SkelAnime_Draw(play, skel, jointTable, (OverrideLimbDraw)(override), (PostLimbDraw)(post), actor, gfx)
#define SkelAnime_DrawFlex(play, skel, jointTable, dListCount, override, post, actor, gfx)                   \
    SkelAnime_DrawFlex(play, skel, jointTable, dListCount, (OverrideLimbDraw)(override), (PostLimbDraw)(post), \
                       actor, gfx)
#define OverrideLimbDrawFlex OverrideLimbDrawOpa
#define PostLimbDrawFlex PostLimbDrawOpa

// --- Water: Majora's lookup also reports which collision object owns the water; Ocarina only has scene water ---
static inline s32 MM_WaterBox_GetSurfaceImpl(PlayState* play, CollisionContext* colCtx, f32 x, f32 z, f32* ySurface,
                                             WaterBox** outWaterBox, s32* bgId) {
    *bgId = BGCHECK_SCENE;
    return WaterBox_GetSurfaceImpl(play, colCtx, x, z, ySurface, outWaterBox);
}
#define WaterBox_GetSurfaceImpl MM_WaterBox_GetSurfaceImpl

// --- Screen shake: Majora's quake API on Ocarina's (types 1-6 are the same six behaviours) ---
#define QUAKE_TYPE_NONE 0
#define QUAKE_TYPE_1 1
#define QUAKE_TYPE_2 2
#define QUAKE_TYPE_3 3
#define QUAKE_TYPE_4 4
#define QUAKE_TYPE_5 5
#define QUAKE_TYPE_6 6
#define Quake_Request(camera, type) Quake_Add(camera, type)
#define Quake_SetDuration(index, duration) Quake_SetCountdown(index, duration)
#define Quake_SetPerturbations(index, y, x, fov, roll) Quake_SetQuakeValues(index, y, x, fov, roll)
#define Quake_RemoveRequest(index) Quake_RemoveFromIdx(index)
#define Quake_GetTimeLeft(index) Quake_GetCountdown(index)

// --- Controller rumble: same call in both ports ---
#define Rumble_Override(distSq, intensity, decayTimer, decayStep) Rumble_Request(distSq, intensity, decayTimer, decayStep)
#define Rumble_ControllerOneHasRumblePak() 1

// --- 2 Ship 2 Harkinian hooks: the ported code is 2 Ship's, whose enhancement hooks have no
// counterpart here. Each takes Majora's original behaviour. ---
#define GameInteractor_Should(id, defaultResult, ...) (defaultResult)
#define GameInteractor_InvertControl(...) ((void)0)
#define GameInteractor_ExecuteOnBossDefeated(...) ((void)0)
#define GameInteractor_ExecuteOnPlayerReleaseHeldActor(...) ((void)0)
#define GameInteractor_ExecuteOnPassPlayerInputs(...) ((void)0)
#define GameInteractor_ExecuteOnGameCompletion(...) ((void)0)
#define FrameInterpolation_IgnoreActorMtx(...) ((void)0)
#define Ship_ExtendedCullingActorAdjustProjectedZ(...) ((void)0)
#define Ship_ExtendedCullingActorAdjustProjectedX(...) ((void)0)
#define Ship_ExtendedCullingActorRestoreProjectedPos(...) ((void)0)
#define Ship_CalcShouldDrawAndUpdate(...) (true)
#define Ship_HandleFirstPersonAiming(...) ((void)0)
#define Ship_HandleConsoleCrashAsReset(...) ((void)0)
#define Ship_GetExtendedAspectRatioMultiplier(...) (1.0f)
#define CosmeticEditor_ApplySillyMoonScale(...) ((void)0)
#define CosmeticEditor_SetStrayFairyMaterial(...) ((void)0)
#define CosmeticEditor_GetChangedColor(r, g, b, a, id) ((Color_RGBA8){ r, g, b, a })
#define CVAR_COSMETIC(name) "gMMPort.Cosmetic." name
#include "soh/frame_interpolation.h"
#include <libultraship/bridge/consolevariablebridge.h>

#endif // MM_COMPAT_H
