#ifndef ARCANUM_GAMELIB_TEXT_FLOATER_H_
#define ARCANUM_GAMELIB_TEXT_FLOATER_H_

#include "game/context.h"

bool text_floater_init(GameContext* ctx);
void text_floater_resize(ResizeContext* ctx);
void text_floater_exit();
bool text_floater_update_view(ViewOptions* view_options);
void text_floater_map_close();
int text_floaters_set(int value);
int text_floaters_get();

#endif /* ARCANUM_GAMELIB_TEXT_FLOATER_H_ */
