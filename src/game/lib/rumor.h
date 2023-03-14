#ifndef ARCANUM_GAME_LIB_RUMOR_H_
#define ARCANUM_GAME_LIB_RUMOR_H_

#include "game/context.h"

bool rumor_init(GameContext* ctx);
void rumor_reset();
void rumor_exit();
bool rumor_mod_load();
void rumor_mod_unload();
bool rumor_load(LoadContext* ctx);
bool rumor_save(TigFile* stream);

#endif /* ARCANUM_GAME_LIB_RUMOR_H_ */
