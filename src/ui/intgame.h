#ifndef ARCANUM_UI_INTGAME_H_
#define ARCANUM_UI_INTGAME_H_

#include "game/context.h"
#include "ui/types.h"

bool intgame_init(GameInitInfo* init_info);
void intgame_reset();
void intgame_exit();
bool intgame_save(TigFile* stream);
bool intgame_load(GameLoadInfo* load_info);
bool iso_interface_create(tig_window_handle_t window_handle);
void iso_interface_destroy();
void sub_54AA30();
bool sub_54AA60(tig_window_handle_t window_handle, TigRect* rect, UiButtonInfo* button_info, unsigned int flags);
bool sub_54AAE0(UiButtonInfo* button_info);
void sub_54AEE0(int a1);
void sub_54B3A0();
void sub_5506C0(int a1);
void sub_550720();

#endif /* ARCANUM_UI_INTGAME_H_ */
