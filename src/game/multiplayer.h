#ifndef ARCANUM_GAME_MULTIPLAYER_H_
#define ARCANUM_GAME_MULTIPLAYER_H_

#include "game/context.h"

bool multiplayer_init(GameInitInfo* init_info);
bool multiplayer_mod_load();
void multiplayer_mod_unload();

#endif /* ARCANUM_GAME_MULTIPLAYER_H_ */
