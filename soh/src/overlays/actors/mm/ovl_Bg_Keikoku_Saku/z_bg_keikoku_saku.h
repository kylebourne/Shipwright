// MM port: generated from the Majora's Mask decomp's ovl_Bg_Keikoku_Saku/z_bg_keikoku_saku.h by port_actor.py.
#ifndef MM_Z_BG_KEIKOKU_SAKU_H
#define MM_Z_BG_KEIKOKU_SAKU_H

#include "global.h"
#include "mm_compat.h" // MM port

#define MMBGKEIKOKUSAKU_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)

struct MmBgKeikokuSaku;

typedef void (*MmBgKeikokuSakuActionFunc)(struct MmBgKeikokuSaku*, PlayState*);

typedef struct MmBgKeikokuSaku {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ MmBgKeikokuSakuActionFunc actionFunc;
    /* 0x160 */ s16 switchFlag;
    /* 0x162 */ s16 timer;
} MmBgKeikokuSaku; // Size = 0x164

#endif // MM_Z_BG_KEIKOKU_SAKU_H
