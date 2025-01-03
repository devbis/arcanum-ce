#ifndef ARCANUM_GAME_TILE_BLOCK_H_
#define ARCANUM_GAME_TILE_BLOCK_H_

#include "game/context.h"

bool tileblock_init(GameInitInfo* init_info);
void tileblock_exit();
bool tileblock_update_view(ViewOptions* view_info);
void tileblock_resize(GameResizeInfo* resize_info);
bool tileblock_is_visible();
void tileblock_toggle();
void tileblock_render(UnknownContext* info);
bool tileblock_is_tile_blocked(int64_t loc);
void tileblock_set_tile_blocked(int64_t loc, bool blocked);
void tileblock_get_rect(int64_t loc, TigRect* rect);

#endif /* ARCANUM_GAME_TILE_BLOCK_H_ */
