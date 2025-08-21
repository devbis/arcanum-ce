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
bool gameuilib_wants_mainmenu();
void gameuilib_wants_mainmenu_set();
void gameuilib_wants_mainmenu_unset();

#endif /* ARCANUM_UI_GAMEUILIB_H_ */
