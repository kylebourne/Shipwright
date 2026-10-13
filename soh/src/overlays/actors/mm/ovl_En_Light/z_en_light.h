// MM port: generated from the Majora's Mask decomp's ovl_En_Light/z_en_light.h by port_actor.py.
#ifndef MM_Z_EN_LIGHT_H
#define MM_Z_EN_LIGHT_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmEnLight;

#define MMENLIGHT_GET_1(thisx) ((thisx)->params & 1)
#define MMENLIGHT_GET_F(thisx) ((thisx)->params & 0xF)
#define MMENLIGHT_GET_800(thisx) ((thisx)->params & 0x800)
#define MMENLIGHT_SWITCH_FLAG(thisx) (((thisx)->params & 0x7F0) >> 4)
#define MMENLIGHT_GET_1000(thisx) ((thisx)->params & 0x1000)
#define MMENLIGHT_GET_2000(thisx) ((thisx)->params & 0x2000)
#define MMENLIGHT_GET_4000(thisx) ((thisx)->params & 0x4000)

typedef struct MmEnLight {
    /* 0x000 */ Actor actor;
    /* 0x144 */ u8 unk_144;
    /* 0x148 */ LightNode* lightNode;
    /* 0x14C */ LightInfo lightInfo;
} MmEnLight; // size = 0x15C

#endif // MM_Z_EN_LIGHT_H
