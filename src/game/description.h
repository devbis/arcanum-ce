#ifndef ARCANUM_GAME_DESCRIPTION_H_
#define ARCANUM_GAME_DESCRIPTION_H_

#include "game/context.h"

#define DESCRIPTION_JUNK_PILE 3023

#define DESCRIPTION_FIRST_CRITTER_NAME 17067
#define DESCRIPTION_LAST_CRITTER_NAME 17316

bool description_init(GameInitInfo* init_info);
void description_exit();
bool description_mod_load();
void description_mod_unload();
const char* description_get_name(int num);
const char* description_get_key_name(int num);

#endif /* ARCANUM_GAME_DESCRIPTION_H_ */
