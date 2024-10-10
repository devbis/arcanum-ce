#ifndef ARCANUM_UI_SERVER_LIST_UI_H_
#define ARCANUM_UI_SERVER_LIST_UI_H_

#include "game/context.h"

bool serverlist_ui_init();
void serverlist_ui_exit();
void sub_585D50();
void sub_585E20();
void sub_5860D0(TigRect* rect);
void sub_586150(int a1, int a2, int a3);
bool sub_5862D0(TigMessage* msg, tig_window_handle_t window_handle);
void sub_588F00();
bool sub_588F10(int a1);

#endif /* ARCANUM_UI_SERVER_LIST_UI_H_ */
