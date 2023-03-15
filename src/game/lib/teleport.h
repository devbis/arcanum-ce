#ifndef ARCANUM_GAME_LIB_TELEPORT_H_
#define ARCANUM_GAME_LIB_TELEPORT_H_

#include "game/context.h"

bool teleport_init(GameContext* ctx);
bool teleport_reset();
void teleport_exit();

#endif /* ARCANUM_GAME_LIB_TELEPORT_H_ */
