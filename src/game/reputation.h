#ifndef ARCANUM_GAME_REPUTATION_H_
#define ARCANUM_GAME_REPUTATION_H_

#include "game/context.h"

bool reputation_init(GameInitInfo* init_info);
void reputation_exit();
bool reputation_mod_load();
void reputation_mod_unload();
void sub_4C1BD0(int index, char* buffer);
bool sub_4C1CB0(int64_t obj, int reputation);

#endif /* ARCANUM_GAME_REPUTATION_H_ */
