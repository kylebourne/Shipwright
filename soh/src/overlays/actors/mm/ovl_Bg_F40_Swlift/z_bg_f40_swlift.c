// MM port: generated from the Majora's Mask decomp's ovl_Bg_F40_Swlift/z_bg_f40_swlift.c by port_actor.py.
/*
 * File: z_bg_f40_swlift.c
 * Overlay: ovl_Bg_F40_Swlift
 * Description: Unused Stone Tower vertically-oscillating platform
 */

#include "z_bg_f40_swlift.h"
#include "mm/objects/object_f40_obj/object_f40_obj.h"
#include "mm_compat.h" // MM port: last, Majora names over Ocarina ones

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED)

void MmBgF40Swlift_Init(Actor* thisx, PlayState* play);
void MmBgF40Swlift_Destroy(Actor* thisx, PlayState* play);
void MmBgF40Swlift_Update(Actor* thisx, PlayState* play2);
void MmBgF40Swlift_Draw(Actor* thisx, PlayState* play);

static s32 sSwitchFlags[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
static s32 sHeights[4];

ActorInit Mm_Bg_F40_Swlift_InitVars = {
    0 /* assigned by ActorDB */,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP /* assets load from mm.o2r by path */,
    sizeof(MmBgF40Swlift),
    MmBgF40Swlift_Init,
    MmBgF40Swlift_Destroy,
    MmBgF40Swlift_Update,
    MmBgF40Swlift_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(uncullZoneScale, 550, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 5000, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

void MmBgF40Swlift_Init(Actor* thisx, PlayState* play) {
    MmBgF40Swlift* this = (MmBgF40Swlift*)thisx;
    s32 index;
    s32 pad;

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, DYNA_TRANSFORM_POS);
    index = BG_F40_SWLIFT_GET_INDEX(thisx);
    // #region 2S2H [Port] - Change index >= 5 to index >= 4 to avoid OOB array access
    if ((index < 0) || (index >= 4)) { //! @bug An index greater than 3 will cause an out of bounds array access.
        Actor_Kill(&this->dyna.actor);
        return;
    }

    sHeights[index] = this->dyna.actor.world.pos.y;
    sSwitchFlags[index] = BG_F40_SWLIFT_GET_SWITCH_FLAG(thisx);
    if (index != 0) {
        Actor_Kill(&this->dyna.actor);
        return;
    }

    DynaPolyActor_LoadMesh(play, &this->dyna, &gStoneTowerVerticallyOscillatingPlatformCol);
    this->dyna.actor.params = 0;
}

void MmBgF40Swlift_Destroy(Actor* thisx, PlayState* play) {
    MmBgF40Swlift* this = (MmBgF40Swlift*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void MmBgF40Swlift_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    MmBgF40Swlift* this = (MmBgF40Swlift*)thisx;
    s32 i;

    for (i = 1; i < ARRAY_COUNT(sSwitchFlags); i++) {
        if ((sSwitchFlags[i] == 0xFF) || !Flags_GetSwitch(play, sSwitchFlags[i])) {
            break;
        }
    }

    i--;
    if (i != this->dyna.actor.params) {
        f32 heightOffset;

        this->dyna.actor.params = -1;
        heightOffset = (sHeights[i] - this->dyna.actor.world.pos.y) * 0.1f;
        if (heightOffset > 0.0f) {
            heightOffset = CLAMP(heightOffset, 0.5f, 15.0f);
        } else {
            heightOffset = CLAMP(heightOffset, -15.0f, -0.5f);
        }

        if (Math_StepToF(&this->dyna.actor.speedXZ, heightOffset, 1.0f) && (fabsf(heightOffset) <= 0.5f)) {
            if (Math_StepToF(&this->dyna.actor.world.pos.y, sHeights[i], fabsf(this->dyna.actor.speedXZ))) {
                this->dyna.actor.params = i;
                this->timer = 48;
                this->dyna.actor.speedXZ = 0.0f;
            }
        } else {
            this->dyna.actor.world.pos.y += this->dyna.actor.speedXZ;
        }
    } else {
        if (this->timer == 0) {
            this->timer = 48;
        }
        this->timer--;
        this->dyna.actor.world.pos.y =
            sHeights[this->dyna.actor.params] + (Math_SinF(this->timer * (M_PIf / 24.0f)) * 5.0f);
    }
}

void MmBgF40Swlift_Draw(Actor* thisx, PlayState* play) {
    MmBgF40Swlift* this = (MmBgF40Swlift*)thisx;

    Gfx_DrawDListOpa(play, gStoneTowerVerticallyOscillatingPlatformDL);
}
