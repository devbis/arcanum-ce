#ifndef ARCANUM_UI_MAINMENU_UI_H_
#define ARCANUM_UI_MAINMENU_UI_H_

#include "game/context.h"
#include "game/timeevent.h"
#include "game/mes.h"
#include "ui/types.h"

bool mainmenu_ui_init(GameInitInfo* init_info);
void mainmenu_ui_exit();
void mainmenu_ui_start(int window_type);
void sub_5412D0();
bool mainmenu_ui_handle();
bool sub_541680();
bool sub_541690();
void sub_541710();
void sub_5417A0(bool a1);
void sub_541810(int a1);
bool sub_543220();
void sub_546330();
void mainmenu_ui_create_window_func(bool should_display);
bool mainmenu_ui_process_callback(TimeEvent* timeevent);
void sub_5482A0(TigRect* rect);
bool sub_549310(tig_button_handle_t button_handle);
void sub_5493C0(char* buffer, int size);
void sub_5495F0(int a1);
void sub_549620(int a1);
MainMenuWindowInfo* sub_5496C0(int index);
int sub_5496D0();
void sub_5496E0();
void sub_549750();
void sub_549760();
void sub_549770();
void sub_549780();
tig_window_handle_t sub_549820();
void sub_549830(int a1);
mes_file_handle_t sub_549840();
tig_font_handle_t sub_549940(int group, int index);
void sub_549960();
void sub_549990(int* a1, int num);
bool sub_5499B0(const char* text);
void sub_549A40();
void sub_549A50();
int sub_549A60();
void sub_549A70();
void sub_549B10();
void sub_549B30();

#endif /* ARCANUM_UI_MAINMENU_UI_H_ */
