#ifndef ARCANUM_UI_SERVER_LIST_UI_H_
#define ARCANUM_UI_SERVER_LIST_UI_H_

#include "game/context.h"
#include "ui/types.h"

extern MainMenuButtonInfo mainmenu_ui_serverlist_buttons[7];

bool serverlist_ui_init();
void serverlist_ui_exit();
void mainmenu_ui_serverlist_create();
void mainmenu_ui_serverlist_destroy();
void mainmenu_ui_serverlist_refresh(TigRect* rect);
void sub_586150(int a1, int a2, void* a3);
bool sub_5862D0(TigMessage* msg, tig_window_handle_t window_handle);
void mainmenu_ui_serverlist_mouse_up(int x, int y);
bool mainmenu_ui_serverlist_execute(int a1);

#endif /* ARCANUM_UI_SERVER_LIST_UI_H_ */
