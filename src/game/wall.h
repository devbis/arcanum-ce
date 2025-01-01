#ifndef ARCANUM_GAME_WALL_H_
#define ARCANUM_GAME_WALL_H_

#include "game/context.h"

bool wall_init(GameInitInfo* init_info);
void wall_exit();
void wall_resize(GameResizeInfo* resize_info);
bool wall_update_view(ViewOptions* view_options);
void wall_toggle();
void wall_render(UnknownContext* render_info);
void sub_4E1490(int64_t a1, int64_t a2);
void sub_4E18F0(int64_t obj, bool a2);

#endif /* ARCANUM_GAME_WALL_H_ */
