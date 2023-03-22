#ifndef ARCANUM_GAMELIB_DESCRIPTION_H_
#define ARCANUM_GAMELIB_DESCRIPTION_H_

#include "game/context.h"

bool description_init(GameContext* ctx);
void description_exit();
bool description_mod_load();
int description_mod_unload();
const char* description_get_name(int num);
const char* description_get_key_name(int num);

#endif /* ARCANUM_GAMELIB_DESCRIPTION_H_ */
