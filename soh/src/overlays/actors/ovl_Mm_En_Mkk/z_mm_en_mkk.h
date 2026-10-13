#ifndef Z_MM_EN_MKK_H
#define Z_MM_EN_MKK_H

#include <libultraship/libultra.h>
#include "global.h"

#define MMENMKK_GET_2(thisx) ((thisx)->params & 2)
#define MMENMKK_GET_4(thisx) ((thisx)->params & 4)
#define MMENMKK_GET_FF00(thisx) (((thisx)->params >> 8) & 0xFF)

struct MmEnMkk;

typedef void (*MmEnMkkActionFunc)(struct MmEnMkk*, PlayState*);

// Ported from Majora's Mask: ovl_En_Mkk (Black and White Boe).
// params: bit 0 = white Boe; bit 1 = appear faster when it first spawns; bit 2 = respawn after dying;
// bits 8-15 = trigger distance (0 or 255 = appear at once).
typedef struct MmEnMkk {
    /* 0x000 */ Actor actor;
    /* 0x14C */ MmEnMkkActionFunc actionFunc;
    /* 0x150 */ u8 primColorSelect;
    /* 0x151 */ u8 unk_149;
    /* 0x152 */ u8 alpha;
    /* 0x153 */ u8 unk_14B;
    /* 0x154 */ u8 unk_14C;
    /* 0x156 */ s16 unk_14E;
    /* 0x158 */ s16 unk_150;
    /* 0x15A */ s16 unk_152;
    /* 0x15C */ Vec3f unk_154;
    /* 0x168 */ Vec3f unk_160;
    /* 0x174 */ Vec3f unk_16C;
    /* 0x180 */ f32 unk_178;
    // Majora used a single sphere collider, which Ocarina doesn't have. A jointed-sphere collider
    // with one sphere behaves the same; the sphere is positioned by hand every frame.
    /* 0x184 */ ColliderJntSph collider;
    /* 0x1A4 */ ColliderJntSphElement colliderElements[1];
} MmEnMkk;

void MmEnMkk_Init(Actor* thisx, PlayState* play);
void MmEnMkk_Destroy(Actor* thisx, PlayState* play);
void MmEnMkk_Update(Actor* thisx, PlayState* play);
void MmEnMkk_Draw(Actor* thisx, PlayState* play);

#endif // Z_MM_EN_MKK_H
