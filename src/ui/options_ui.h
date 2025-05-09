#ifndef ARCANUM_UI_OPTIONS_UI_H_
#define ARCANUM_UI_OPTIONS_UI_H_

#include "game/context.h"

void options_ui_init(int type, tig_window_handle_t window_handle, bool a3);
bool sub_589430();
void options_ui_exit();
int options_ui_handle_button_pressed(tig_button_handle_t button_handle);

#endif /* ARCANUM_UI_OPTIONS_UI_H_ */
