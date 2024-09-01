#ifndef ARCANUM_GAME_MULTIPLAYER_H_
#define ARCANUM_GAME_MULTIPLAYER_H_

#include "game/context.h"

typedef void(Func5F0E08)();
typedef void(Func5F0DF8)(int);

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
void sub_4A2BC0();
void sub_4A2BD0();
int64_t multiplayer_find_first_player_obj();
int64_t multiplayer_find_next_player_obj();
void sub_4A3890();
int sub_4A38A0();
void sub_4A39D0(Func5F0DF8* func, int ctx);
void* sub_4A4230(int player);
void sub_4A4270();
void sub_4A4280();
void multiplayer_ping();
int sub_4A55D0(int player);
void sub_4A5610();

#endif /* ARCANUM_GAME_MULTIPLAYER_H_ */
