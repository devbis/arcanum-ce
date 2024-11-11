#ifndef ARCANUM_GAME_ANTITELEPORT_H_
#define ARCANUM_GAME_ANTITELEPORT_H_

#include "game/context.h"

bool antiteleport_init(GameInitInfo* init_info);
void antiteleport_exit();
bool antiteleport_mod_load();
void antiteleport_mod_unload();
bool antiteleport_check_can_teleport(int64_t obj, int64_t extra_loc);

#endif /* ARCANUM_GAME_ANTITELEPORT_H_ */
