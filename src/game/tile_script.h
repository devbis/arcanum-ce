#ifndef ARCANUM_GAME_TILE_SCRIPT_H_
#define ARCANUM_GAME_TILE_SCRIPT_H_

#include "game/context.h"

bool tile_script_init(GameInitInfo* init_info);
void tile_script_reset();
void tile_script_exit();
void tile_script_resize(ResizeInfo* resize_info);
bool sub_4C05E0(ViewOptions* view_options);
void sub_4C0630(UnknownContext* a1);
void sub_4C08E0(int64_t old_loc, int64_t new_loc);
void sub_4C0980(int64_t loc, int64_t triggerer_obj);

#endif /* ARCANUM_GAME_TILE_SCRIPT_H_ */
