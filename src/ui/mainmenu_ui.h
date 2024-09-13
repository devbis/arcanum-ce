#ifndef ARCANUM_UI_MAINMENU_UI_H_
#define ARCANUM_UI_MAINMENU_UI_H_

#include "game/context.h"

bool mainmenu_ui_init(GameInitInfo* init_info);
void mainmenu_ui_exit();
void mainmenu_ui_start(int window_type);
bool mainmenu_ui_handle();
void sub_541680();
void sub_549A70();

#endif /* ARCANUM_UI_MAINMENU_UI_H_ */
