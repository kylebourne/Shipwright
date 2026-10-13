// MM port: generated from the Majora's Mask decomp's ovl_Obj_Driftice/z_obj_driftice.h by port_actor.py.
#ifndef MM_Z_OBJ_DRIFTICE_H
#define MM_Z_OBJ_DRIFTICE_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmObjDriftice;

typedef void (*MmObjDrifticeActionFunc)(struct MmObjDriftice*, PlayState*);

#define MMOBJDRIFTICE_GET_3(thisx) ((thisx)->params & 3)
#define MMOBJDRIFTICE_GET_PATH_INDEX(thisx) (((thisx)->params >> 2) & 0x7F)
#define MMOBJDRIFTICE_GET_E00(thisx) (((thisx)->params >> 9) & 7)
#define MMOBJDRIFTICE_GET_1000(thisx) (((thisx)->params >> 0xC) & 1)
#define MMOBJDRIFTICE_GET_ROT(thisx) ((thisx)->home.rot.x & 3)

typedef struct {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ f32 unk_10;
} MmObjDrifticeStruct3; // size = 0x14

typedef struct {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ MmObjDrifticeStruct3 unk_0C[2];
} MmObjDrifticeStruct2; // size = 0x34

typedef struct {
    /* 0x00 */ s16 unk_00[6];
    /* 0x0C */ MmObjDrifticeStruct3 unk_0C[3];
} MmObjDrifticeStruct4; // size = 0x48

typedef struct {
    /* 0x00 */ MmObjDrifticeStruct3 unk_00;
    /* 0x14 */ MmObjDrifticeStruct3 unk_14[3];
    /* 0x50 */ MmObjDrifticeStruct2 unk_50;
    /* 0x84 */ MmObjDrifticeStruct4 unk_84;
} MmObjDrifticeStruct; // size = 0xCC

typedef struct MmObjDriftice {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ MmObjDrifticeActionFunc actionFunc;
    /* 0x160 */ s32 unk_160;
    /* 0x164 */ s32 unk_164;
    /* 0x168 */ s32 unk_168;
    /* 0x16C */ Vec3s* pathPoints;
    /* 0x170 */ MmObjDrifticeStruct unk_170;
    /* 0x23C */ f32 unk_23C;
    /* 0x240 */ f32 unk_240;
    /* 0x244 */ s16 unk_244;
    /* 0x248 */ s32 unk_248;
    /* 0x24C */ s32 unk_24C;
} MmObjDriftice; // size = 0x250

#endif // MM_Z_OBJ_DRIFTICE_H
