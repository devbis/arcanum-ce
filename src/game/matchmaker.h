#ifndef ARCANUM_GAME_MATCHMAKER_H_
#define ARCANUM_GAME_MATCHMAKER_H_

#include <winsock2.h>

typedef struct MatchmakerInitInfo {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
} MatchmakerInitInfo;

static_assert(sizeof(MatchmakerInitInfo) == 0x34, "wrong size");

typedef struct MatchmakerRegisterInfo {
    /* 0000 */ int field_0;
    /* 0004 */ char name[128];
    /* 0084 */ struct sockaddr_in addr;
    /* 0094 */ char hostname[132];
} MatchmakerRegisterInfo;

static_assert(sizeof(MatchmakerRegisterInfo) == 0x118, "wrong size");

int matchmaker_init(MatchmakerInitInfo* init_info);
void matchmaker_exit();
int matchmaker_is_active();
int matchmaker_motd_get(int a1, int a2, int a3, int a4);
int matchmaker_login(const char* a1, const char* a2);
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
