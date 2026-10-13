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
 * Grows as more actors are ported. Keep entries grouped by subsystem.
 */

#include "global.h"

// Majora's assets are extracted into mm.o2r with every path under mm/.
#define MM_ASSET(path) "__OTR__mm/" path

// --- Sound ---
// Sound effect ids resolve against Ocarina's sfx.h, so only sounds that exist in Ocarina play.
#define Actor_PlaySfx(actor, sfxId) Audio_PlayActorSound2((actor), (sfxId))

// --- Graphics setup ---
#define Gfx_SetupDL25_Opa(gfxCtx) Gfx_SetupDL_25Opa(gfxCtx)
#define Gfx_SetupDL25_Xlu(gfxCtx) Gfx_SetupDL_25Xlu(gfxCtx)
#define MATRIX_FINALIZE_AND_LOAD(pkt, gfxCtx) \
    gSPMatrix(pkt, MATRIX_NEWMTX(gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW)

// --- Dyna poly ---
#define DYNA_TRANSFORM_POS DPM_PLAYER

// --- Collision check: Majora renamed Ocarina's collider constants ---
#define COL_MATERIAL_TREE COLTYPE_TREE
#define ELEM_MATERIAL_UNK1 ELEMTYPE_UNK1
#define ATELEM_NONE TOUCH_NONE
#define ATELEM_SFX_NORMAL TOUCH_SFX_NORMAL
#define ACELEM_ON BUMP_ON

#endif // MM_COMPAT_H
