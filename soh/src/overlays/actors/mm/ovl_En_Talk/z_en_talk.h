// MM port: generated from the Majora's Mask decomp's ovl_En_Talk/z_en_talk.h by port_actor.py.
#ifndef MM_Z_EN_TALK_H
#define MM_Z_EN_TALK_H

#include "global.h"
#include "mm_compat.h" // MM port

#define MMENTALK_GET_TEXT_ID(thisx) (((thisx)->params & 0x3F) + 0x1C00)
#define MMENTALK_GET_ATTENTION_RANGE_TYPE(thisx) (((thisx)->home.rot.x) - 1)

struct MmEnTalk;

typedef void (*MmEnTalkActionFunc)(struct MmEnTalk*, PlayState*);

typedef struct MmEnTalk {
    /* 0x000 */ Actor actor;
    /* 0x144 */ MmEnTalkActionFunc actionFunc;
} MmEnTalk; // size = 0x148

#endif // MM_Z_EN_TALK_H
