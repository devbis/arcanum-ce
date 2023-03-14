#ifndef ARCANUM_GAME_LIB_CURSE_H_
#define ARCANUM_GAME_LIB_CURSE_H_

#include "game/context.h"

bool curse_mod_load();
void curse_mod_unload();
void curse_copy_name(int curse, char* dest);
void curse_copy_description(int curse, char* dest);
int curse_get_effect(int curse);

#endif /* ARCANUM_GAME_LIB_CURSE_H_ */
