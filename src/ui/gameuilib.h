#ifndef ARCANUM_UI_GAMEUILIB_H_
#define ARCANUM_UI_GAMEUILIB_H_

#include "game/context.h"

bool gameuilib_init(GameInitInfo* init_info);
void gameuilib_exit();
void gameuilib_reset();
void gameuilib_resize(GameResizeInfo* resize_info);
bool gameuilib_mod_load();
void gameuilib_mod_unload();
bool gameuilib_save();
bool gameuilib_load();
bool sub_53EAD0();
void sub_53EAF0();
void sub_53EB00();

#endif /* ARCANUM_UI_GAMEUILIB_H_ */
