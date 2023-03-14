#ifndef ARCANUM_GAME_LIB_QUEST_H_
#define ARCANUM_GAME_LIB_QUEST_H_

#include "game/context.h"

bool quest_init(GameContext* ctx);
void quest_reset();
void quest_exit();
bool quest_mod_load();
void quest_mod_unload();
bool quest_load(LoadContext* ctx);
bool quest_save(TigFile* stream);
int sub_4C51A0(int a1);
int sub_4C53C0(int id);

#endif /* ARCANUM_GAME_LIB_QUEST_H_ */
