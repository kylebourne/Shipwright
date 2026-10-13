// MM port: generated from the Majora's Mask decomp's ovl_Obj_Funen/z_obj_funen.c by port_actor.py.
/*
 * File: z_obj_funen.c
 * Overlay: ovl_Obj_Funen
 * Description: Unused(?) Stone Tower smoke
 */

#include "z_obj_funen.h"
#include "mm/objects/object_funen/object_funen.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

void MmObjFunen_Init(Actor* thisx, PlayState* play);
void MmObjFunen_Draw(Actor* thisx, PlayState* play);

ActorInit Mm_Obj_Funen_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmObjFunen),
    MmObjFunen_Init,
    Actor_Noop,
    Actor_Noop,
    MmObjFunen_Draw,
};

f32 D_80A198D0[] = { 0.1f, 0.024390244f };

void MmObjFunen_Init(Actor* thisx, PlayState* play) {
    MmObjFunen* this = (MmObjFunen*)thisx;

    Actor_SetScale(&this->actor, D_80A198D0[this->actor.params & 1]);
}

void MmObjFunen_Draw(Actor* thisx, PlayState* play) {
    s32 temp;
    s32 pad[2];

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    Matrix_RotateYS((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)) - 0x8000), MTXMODE_APPLY);

    MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);

    temp = -(play->gameplayFrames & 0x7FFFFFFF) & 0x7F;

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScrollEx(play->state.gfxCtx, 0, 0, temp, 0x20, 0x20, 1, 0, temp, 0x20, 0x20, 0, -1, 0, -1));
    gSPDisplayList(POLY_XLU_DISP++, gStoneTowerSmokeDL);
    CLOSE_DISPS(play->state.gfxCtx);
}
