#ifndef ARCANUM_GAME_SCRIPT_NAME_H_
#define ARCANUM_GAME_SCRIPT_NAME_H_

#include "game/context.h"

bool script_name_init(GameInitInfo* ctx);
void script_name_exit();
bool script_name_mod_load();
void script_name_mod_unload();
bool script_name_build_scr_name(int index, char* buffer);
bool script_name_build_dlg_name(int index, char* buffer);

#endif /* ARCANUM_GAME_SCRIPT_NAME_H_ */
