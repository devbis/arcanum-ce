#ifndef ARCANUM_GAME_TILE_BLOCK_H_
#define ARCANUM_GAME_TILE_BLOCK_H_

#include "game/context.h"

bool tileblock_init(GameInitInfo* init_info);
void tileblock_exit();
bool sub_4BB060(ViewOptions* view_info);
void tileblock_resize(ResizeInfo* resize_info);
bool tb_is_visible();
void tb_toggle();
void sub_4BB0C0(UnknownContext* info);
bool tb_is_blocked(int64_t loc);
void tb_set_blocked(int64_t loc, bool blocked);
void tb_get_rect(int64_t loc, TigRect* rect);

#endif /* ARCANUM_GAME_TILE_BLOCK_H_ */
