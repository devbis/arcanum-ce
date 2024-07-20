#ifndef ARCANUM_GAME_TILE_SCRIPT_H_
#define ARCANUM_GAME_TILE_SCRIPT_H_

#include "game/context.h"

bool tile_script_init(GameInitInfo* init_info);
void tile_script_reset();
void tile_script_exit();
void tile_script_resize(ResizeInfo* resize_info);
bool sub_4C05E0(ViewOptions* view_options);

#endif /* ARCANUM_GAME_TILE_SCRIPT_H_ */
