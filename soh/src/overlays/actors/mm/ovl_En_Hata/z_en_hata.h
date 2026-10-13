// MM port: generated from the Majora's Mask decomp's ovl_En_Hata/z_en_hata.h by port_actor.py.
#ifndef MM_Z_EN_HATA_H
#define MM_Z_EN_HATA_H

#include "global.h"
#include "mm/objects/object_hata/object_hata.h"
#include "mm_compat.h" // MM port

struct MmEnHata;

typedef struct MmEnHata {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ SkelAnime skelAnime;
    /* 0x1A0 */ Vec3s jointTable[FLAGPOLE_LIMB_MAX];
    /* 0x21E */ Vec3s morphTable[FLAGPOLE_LIMB_MAX];
    /* 0x29C */ s16 unk_29C;
    /* 0x29E */ UNK_TYPE1 pad_29E[2];
    /* 0x2A0 */ s16 unk_2A0;
    /* 0x2A4 */ Vec3f unk_2A4;
} MmEnHata; /* size = 0x2B0 */

#endif // MM_Z_EN_HATA_H
