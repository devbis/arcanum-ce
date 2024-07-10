#ifndef ARCANUM_GAME_ANTITELEPORT_H_
#define ARCANUM_GAME_ANTITELEPORT_H_

#include "game/context.h"

bool antiteleport_init(GameInitInfo* init_info);
void antiteleport_exit();
bool antiteleport_mod_load();
void antiteleport_mod_unload();
bool sub_4BDD30(int64_t a1, int64_t a2);

#endif /* ARCANUM_GAME_ANTITELEPORT_H_ */
