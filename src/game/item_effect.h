#ifndef ARCANUM_GAME_ITEM_EFFECT_H_
#define ARCANUM_GAME_ITEM_EFFECT_H_

#include "game/context.h"

bool item_effect_init(GameInitInfo* init_info);
void item_effect_exit();
bool item_effect_mod_load();
void item_effect_mod_unload();
const char* item_effect_get_text(int num);

#endif /* ARCANUM_GAME_ITEM_EFFECT_H_ */
