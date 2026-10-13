// MM port: generated from the Majora's Mask decomp's ovl_En_Estone/z_en_estone.h by port_actor.py.
#ifndef MM_Z_EN_ESTONE_H
#define MM_Z_EN_ESTONE_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmEnEstone;

typedef void (*MmEnEstoneActionFunc)(struct MmEnEstone*, PlayState*);

#define MMENESTONE_EFFECT_COUNT 200

typedef enum {
    /* 0 */ MMENESTONE_TYPE_LARGE,
    /* 1 */ MMENESTONE_TYPE_SMALL
} MmEnEstoneType;

typedef struct {
    /* 0x00 */ u8 isActive;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ Vec3s rot;
    /* 0x30 */ f32 scale;
    /* 0x34 */ s16 timer;
} MmEnEstoneEffect; // size = 0x38

typedef struct MmEnEstone {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ MmEnEstoneActionFunc actionFunc;
    /* 0x0148 */ f32 scale;
    /* 0x014C */ Vec3f rot;
    /* 0x0158 */ Vec3f rotVel;
    /* 0x0164 */ s16 timer;
    /* 0x0166 */ s16 inactive;
    /* 0x0168 */ ColliderCylinder collider;
    /* 0x01B4 */ MmEnEstoneEffect effects[MMENESTONE_EFFECT_COUNT];
} MmEnEstone; // size = 0x2D74

#endif // MM_Z_EN_ESTONE_H
