#ifndef ARCANUM_UI_ROLLER_UI_H_
#define ARCANUM_UI_ROLLER_UI_H_

#include "game/context.h"

bool roller_ui_init(GameInitInfo* init_info);
void roller_ui_exit();
void roller_ui_draw(int value, tig_window_handle_t window_handle, int x, int y, int max_digits, int flags);

#endif /* ARCANUM_UI_ROLLER_UI_H_ */
