#ifndef ARCANUM_GAME_WALL_H_
#define ARCANUM_GAME_WALL_H_

#include "game/context.h"

bool wall_init(GameInitInfo* init_info);
void wall_exit();
void wall_resize(ResizeInfo* resize_info);
bool wall_update_view(ViewOptions* view_options);
void sub_4DF500(int a1);
void sub_4E18F0(int64_t obj, bool a2);

#endif /* ARCANUM_GAME_WALL_H_ */
