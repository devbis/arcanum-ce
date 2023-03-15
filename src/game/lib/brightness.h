#ifndef ARCANUM_GAME_LIB_BRIGHTNESS_H_
#define ARCANUM_GAME_LIB_BRIGHTNESS_H_

#include "game/context.h"

bool brightness_init(GameContext* ctx);
void brightness_exit();
void brightness_changed();

#endif /* ARCANUM_GAME_LIB_BRIGHTNESS_H_ */
