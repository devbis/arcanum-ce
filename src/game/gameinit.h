#ifndef ARCANUM_GAME_GAMEINIT_H_
#define ARCANUM_GAME_GAMEINIT_H_

#include "game/context.h"

bool gameinit_init(GameInitInfo* init_info);
void gameinit_reset();
void gameinit_exit();
bool gameinit_mod_load();
void gameinit_mod_unload();

#endif /* ARCANUM_GAME_GAMEINIT_H_ */
