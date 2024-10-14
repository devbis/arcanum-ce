#ifndef ARCANUM_GAME_TEXT_FLOATER_H_
#define ARCANUM_GAME_TEXT_FLOATER_H_

#include "game/context.h"

bool text_floater_init(GameInitInfo* init_info);
void text_floater_resize(ResizeInfo* resize_info);
void text_floater_exit();
bool text_floater_update_view(ViewOptions* view_options);
void text_floater_map_close();
int text_floaters_set(int value);
int text_floaters_get();
void sub_4D5450(int64_t obj, int type, const char* str);
void sub_4D56C0(int64_t obj);

#endif /* ARCANUM_GAME_TEXT_FLOATER_H_ */
