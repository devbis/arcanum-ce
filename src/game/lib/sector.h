#ifndef ARCANUM_GAME_LIB_SECTOR_H_
#define ARCANUM_GAME_LIB_SECTOR_H_

#include "game/context.h"

bool sector_init(GameContext* ctx);
void sector_reset();
void sector_resize(ResizeContext* ctx);

#endif /* ARCANUM_GAME_LIB_SECTOR_H_ */
