#ifndef ARCANUM_GAME_RUMOR_H_
#define ARCANUM_GAME_RUMOR_H_

#include "game/context.h"
#include "game/obj.h"

bool rumor_init(GameInitInfo* init_info);
void rumor_reset();
void rumor_exit();
bool rumor_mod_load();
void rumor_mod_unload();
bool rumor_load(GameLoadInfo* load_info);
bool rumor_save(TigFile* stream);

#endif /* ARCANUM_GAME_RUMOR_H_ */
