#ifndef ARCANUM_GAME_MULTIPLAYER_H_
#define ARCANUM_GAME_MULTIPLAYER_H_

#include "game/context.h"

typedef void(Func5F0E08)();

bool multiplayer_init(GameInitInfo* init_info);
bool multiplayer_mod_load();
void multiplayer_mod_unload();
bool sub_49CBD0();
bool sub_49CC20();
void sub_49CC50();
bool multiplayer_map_open_by_name(const char* name);
void sub_4A2B00(Func5F0E08* func);
int sub_4A2B10(int64_t obj);
int64_t sub_4A2B60(int player);
bool sub_4A2BA0();

#endif /* ARCANUM_GAME_MULTIPLAYER_H_ */
