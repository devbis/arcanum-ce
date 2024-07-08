#ifndef ARCANUM_GAMELIB_ITEM_EFFECT_H_
#define ARCANUM_GAMELIB_ITEM_EFFECT_H_

#include "game/context.h"

bool item_effect_init(GameContext* ctx);
void item_effect_exit();
bool item_effect_mod_load();
void item_effect_mod_unload();
const char* item_effect_get_text(int num);

#endif /* ARCANUM_GAMELIB_ITEM_EFFECT_H_ */
