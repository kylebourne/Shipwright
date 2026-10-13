#include "MMPort.h"

#include "soh/ActorDB.h"
#include "global.h" // C++-aware; must come before the extern "C" block below

extern "C" {
#include "src/overlays/actors/ovl_Mm_Obj_Tree/z_mm_obj_tree.h"
#include "src/overlays/actors/ovl_Mm_En_Mkk/z_mm_en_mkk.h"
#include "macros.h"
}

namespace MMPort {

// Each ported actor gets the next free id (-1). Its asset paths are self-contained strings, so it
// borrows gameplay_keep as its object instead of needing an entry in Ocarina's object table.
void RegisterActors() {
    static bool registered = false;
    if (registered) {
        return;
    }
    registered = true;

    ActorDBInit tree = {
        "Mm_Obj_Tree",
        "MM: Clock Town Tree",
        -1,
        ACTORCAT_PROP,
        ACTOR_FLAG_UPDATE_DURING_OCARINA,
        OBJECT_GAMEPLAY_KEEP,
        sizeof(MmObjTree),
        (ActorFunc)MmObjTree_Init,
        (ActorFunc)MmObjTree_Destroy,
        (ActorFunc)MmObjTree_Update,
        (ActorFunc)MmObjTree_Draw,
        nullptr,
    };
    ActorDB::Instance->AddEntry(tree);

    // params: 0 = Black Boe, 1 = White Boe. Add 4 to make it respawn after dying.
    ActorDBInit boe = {
        "Mm_En_Mkk",
        "MM: Boe (Black/White)",
        -1,
        ACTORCAT_ENEMY,
        ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE,
        OBJECT_GAMEPLAY_KEEP,
        sizeof(MmEnMkk),
        (ActorFunc)MmEnMkk_Init,
        (ActorFunc)MmEnMkk_Destroy,
        (ActorFunc)MmEnMkk_Update,
        (ActorFunc)MmEnMkk_Draw,
        nullptr,
    };
    ActorDB::Instance->AddEntry(boe);
}

} // namespace MMPort
