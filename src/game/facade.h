#ifndef ARCANUM_GAME_FACADE_H_
#define ARCANUM_GAME_FACADE_H_

#include "game/context.h"

bool facade_init(GameInitInfo* init_info);
void facade_exit();
void facade_resize(ResizeInfo* resize_info);

#endif /* ARCANUM_GAME_FACADE_H_ */
