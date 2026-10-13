// MM port: generated from the Majora's Mask decomp's ovl_En_Rsn/z_en_rsn.h by port_actor.py.
#ifndef MM_Z_EN_RSN_H
#define MM_Z_EN_RSN_H

#include "global.h"
#include "mm_compat.h" // MM port

struct MmEnRsn;

typedef void (*MmEnRsnActionFunc)(struct MmEnRsn*, PlayState*);

typedef struct MmEnRsn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ UNK_TYPE1 unk144[0x4C];
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ MmEnRsnActionFunc actionFunc;
    /* 0x1D8 */ Vec3s headRot;
    /* 0x1DE */ Vec3s torsoRot;
} MmEnRsn; // size = 0x1E4

#endif // MM_Z_EN_RSN_H
