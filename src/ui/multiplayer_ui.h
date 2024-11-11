#ifndef ARCANUM_UI_MULTIPLAYER_UI_H_
#define ARCANUM_UI_MULTIPLAYER_UI_H_

#include "game/context.h"

bool multiplayer_ui_init(GameInitInfo* init_info);
void multiplayer_ui_exit();
void mutliplayer_ui_reset();
void sub_5700C0();
void sub_570130();
bool sub_5701A0(tig_window_handle_t window_handle, const TigRect* rect);
bool sub_570260(tig_window_handle_t window_handle);
bool sub_5703B0();
void sub_5703D0(const char* str, int player);
void sub_5704E0(int64_t a1, int64_t a2, int type);
void sub_570890();
void sub_5708B0(int a1);
bool sub_5708C0(tig_window_handle_t window_handle, const TigRect* rect);
bool sub_570940(int a1);
bool sub_5709E0();
bool sub_570A10(TigMessage* msg);
bool sub_570BC0(TigMessage* msg);

#endif /* ARCANUM_UI_MULTIPLAYER_UI_H_ */
