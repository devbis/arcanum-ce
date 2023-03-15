#ifndef ARCANUM_GAME_LIB_WALL_H_
#define ARCANUM_GAME_LIB_WALL_H_

#include "game/context.h"

bool wall_init(GameContext* ctx);
void wall_exit();
void wall_resize(ResizeContext* ctx);
bool wall_update_view(ViewOptions* ctx);
void sub_4DF500(int a1);

#endif /* ARCANUM_GAME_LIB_WALL_H_ */
