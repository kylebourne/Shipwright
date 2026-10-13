#pragma once

// MM port: Majora's Mask content running inside Ship of Harkinian.
// Assets come from mm.o2r (extracted from the player's own MM ROM); actors are registered
// at startup through ActorDB so they get fresh ids and show up in the Actor Viewer and the
// debug console's `spawn` command.

#ifdef __cplusplus
namespace MMPort {
void RegisterActors();
}
#endif
