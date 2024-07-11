#ifndef ARCANUM_GAME_TILE_BLOCK_H_
#define ARCANUM_GAME_TILE_BLOCK_H_

#include "game/context.h"

bool tileblock_init(GameInitInfo* init_info);
void tileblock_exit();
bool sub_4BB060(ViewOptions* view_info);
void tileblock_resize(ResizeInfo* resize_info);
bool sub_4BB090();
void sub_4BB0A0();
void sub_4BB550(int64_t location, TigRect* rect);

#endif /* ARCANUM_GAME_TILE_BLOCK_H_ */
