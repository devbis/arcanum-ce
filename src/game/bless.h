#ifndef ARCANUM_GAME_LIB_BLESS_H_
#define ARCANUM_GAME_LIB_BLESS_H_

#include "game/context.h"

bool bless_mod_load();
void bless_mod_unload();
void bless_copy_name(int bless, char* dest);
void bless_copy_description(int bless, char* dest);
int bless_get_effect(int bless);

#endif /* ARCANUM_GAME_LIB_BLESS_H_ */
