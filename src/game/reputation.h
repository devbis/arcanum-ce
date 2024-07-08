#ifndef ARCANUM_GAME_LIB_REPUTATION_H_
#define ARCANUM_GAME_LIB_REPUTATION_H_

#include "game/context.h"

bool reputation_init(GameContext* ctx);
void reputation_exit();
bool reputation_mod_load();
void reputation_mod_unload();

#endif /* ARCANUM_GAME_LIB_REPUTATION_H_ */
