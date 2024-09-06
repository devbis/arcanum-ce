#ifndef ARCANUM_GAME_MATCHMAKER_H_
#define ARCANUM_GAME_MATCHMAKER_H_

int matchmaker_init(int a1);
void matchmaker_exit();
int matchmaker_is_active();
int matchmaker_motd_get(int a1, int a2, int a3, int a4);
int matchmaker_login(int a1, int a2);
int matchmaker_create_account(int a1, int a2, int a3);
int matchmaker_version_needs_upgrade(int a1);
int matchmaker_ad_rgb_get(int a1, int a2, int a3);
int matchmaker_ad_release();
int matchmaker_ad_clicked();
int matchmaker_gamelist_get(void** games, int* count);
int matchmaker_gamelist_free(void* games);
int matchmaker_chatserver_list_get(void** chatservers, int* count);
int matchmaker_chatserver_list_free(void* chatservers);
int matchmaker_chatserver_join(int a1);
int matchmaker_chatroom_list_get(void** chatrooms, int* count);
int matchmaker_chatroom_list_free(void* chatrooms);
int matchmaker_chatroom_join(int a1, int a2);
int matchmaker_chatroom_get(int a1);
int matchmaker_chatroom_members_get(void** members, int* count);
int matchmaker_chatroom_members_free(void* members);
int matchmaker_chatroom_create(const char* a1, const char* a2);
int matchmaker_chatroom_mesg(const char* str);
int matchmaker_register(int a1);
int matchmaker_ping();

#endif /* ARCANUM_GAME_MATCHMAKER_H_ */
