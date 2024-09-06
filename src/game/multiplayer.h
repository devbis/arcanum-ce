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
int multiplayer_mm_init(int a1);
void multiplayer_mm_exit();
int multiplayer_mm_is_active();
int multiplayer_mm_ping();
int multiplayer_mm_motd_get(int a1, int a2, int a3, int a4);
int multiplayer_mm_login(int a1, int a2);
int multiplayer_mm_create_account(int a1, int a2, int a3);
int multiplayer_mm_version_needs_upgrade(int a1);
int multiplayer_mm_ad_rgb_get(int a1, int a2, int a3);
int multiplayer_mm_ad_release();
int multiplayer_mm_ad_clicked();
int multiplayer_mm_register(int a1);
int multiplayer_mm_gamelist_get(void** games, int* count);
int multiplayer_mm_gamelist_free(void* games);
int multiplayer_mm_chatserver_list_get(void** chatservers, int* count);
int multiplayer_mm_chatserver_list_free(void* chatservers);
int multiplayer_mm_chatserver_join(int a1);
int multiplayer_mm_chatroom_list_get(void** chatrooms, int* count);
int multiplayer_mm_chatroom_list_free(void* chatrooms);
int multiplayer_mm_chatroom_join(int a1, int a2);
int multiplayer_mm_chatroom_members_get(void** members, int* count);
int multiplayer_mm_chatroom_members_free(void* members);
int multiplayer_mm_chatroom_create(const char* a1, const char* a2);
int multiplayer_mm_chatroom_mesg(const char* msg);
void multiplayer_ping();
int sub_4A55D0(int player);
void sub_4A5610();

#endif /* ARCANUM_GAME_MULTIPLAYER_H_ */
