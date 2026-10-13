// Majora's Mask object_bh, extracted into mm.o2r (see soh/assets/yml/mm_ntsc_u_1-0/objects/object_bh.yml).
// Copied from 2 Ship 2 Harkinian's generated header with asset paths moved under mm/.
#ifndef MM_OBJECTS_OBJECT_BH_H
#define MM_OBJECTS_OBJECT_BH_H 1

#include "align_asset_macro.h"

#define dgBhFlyingAnim "__OTR__mm/objects/object_bh/gBhFlyingAnim"
static const ALIGN_ASSET(2) char gBhFlyingAnim[] = dgBhFlyingAnim;

#define dgBhBodyDL "__OTR__mm/objects/object_bh/gBhBodyDL"
static const ALIGN_ASSET(2) char gBhBodyDL[] = dgBhBodyDL;

#define dgBhRightWingToEdgeDL "__OTR__mm/objects/object_bh/gBhRightWingToEdgeDL"
static const ALIGN_ASSET(2) char gBhRightWingToEdgeDL[] = dgBhRightWingToEdgeDL;

#define dgBhRightWingToBodyDL "__OTR__mm/objects/object_bh/gBhRightWingToBodyDL"
static const ALIGN_ASSET(2) char gBhRightWingToBodyDL[] = dgBhRightWingToBodyDL;

#define dgBhLeftWingToEdgeDL "__OTR__mm/objects/object_bh/gBhLeftWingToEdgeDL"
static const ALIGN_ASSET(2) char gBhLeftWingToEdgeDL[] = dgBhLeftWingToEdgeDL;

#define dgBhLeftWingToBodyDL "__OTR__mm/objects/object_bh/gBhLeftWingToBodyDL"
static const ALIGN_ASSET(2) char gBhLeftWingToBodyDL[] = dgBhLeftWingToBodyDL;

#define dgBhBodyTex "__OTR__mm/objects/object_bh/gBhBodyTex"
static const ALIGN_ASSET(2) char gBhBodyTex[] = dgBhBodyTex;

#define dgBhWingTex "__OTR__mm/objects/object_bh/gBhWingTex"
static const ALIGN_ASSET(2) char gBhWingTex[] = dgBhWingTex;

#define dgBhSkel "__OTR__mm/objects/object_bh/gBhSkel"
static const ALIGN_ASSET(2) char gBhSkel[] = dgBhSkel;

typedef enum ObjectBhLimb {
    /* 0x00 */ OBJECT_BH_LIMB_NONE,
    /* 0x01 */ OBJECT_BH_LIMB_BODY,
    /* 0x02 */ OBJECT_BH_LIMB_RIGHT_WING_TO_BODY,
    /* 0x03 */ OBJECT_BH_LIMB_RIGHT_WING_TO_EDGE,
    /* 0x04 */ OBJECT_BH_LIMB_LEFT_WING_TO_BODY,
    /* 0x05 */ OBJECT_BH_LIMB_LEFT_WING_TO_EDGE,
    /* 0x06 */ OBJECT_BH_LIMB_MAX
} ObjectBhLimb;

#endif // MM_OBJECTS_OBJECT_BH_H
