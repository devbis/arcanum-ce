#ifndef ARCANUM_GAMELIB_TILE_H_
#define ARCANUM_GAMELIB_TILE_H_

#include "game/context.h"

bool tile_init(GameContext* ctx);
void tile_exit();
void tile_resize(ResizeContext* ctx);
bool tile_update_view(ViewOptions* view_options);

#endif /* ARCANUM_GAMELIB_TILE_H_ */
