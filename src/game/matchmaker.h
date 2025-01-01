#ifndef ARCANUM_GAME_MATCHMAKER_H_
#define ARCANUM_GAME_MATCHMAKER_H_

#include <winsock2.h>

typedef struct MatchmakerInitInfo {
    /* 0000 */ const char* product;
    /* 0004 */ const char* locale;
    /* 0008 */ const char* default_locale;
    /* 000C */ const char* default_server;
    /* 0010 */ int default_port;
    /* 0014 */ const char* preferred_servers;
    /* 0018 */ void* field_18;
    /* 001C */ void* field_1C;
    /* 0020 */ void* field_20;
    /* 0024 */ void* field_24;
    /* 0028 */ void* field_28;
    /* 002C */ void* field_2C;
    /* 0030 */ void* field_30;
} MatchmakerInitInfo;

static_assert(sizeof(MatchmakerInitInfo) == 0x34, "wrong size");

typedef struct MatchmakerRegisterInfo {
    /* 0000 */ int field_0;
    /* 0004 */ char name[128];
    /* 0084 */ struct sockaddr_in addr;
    /* 0094 */ char hostname[132];
} MatchmakerRegisterInfo;

static_assert(sizeof(MatchmakerRegisterInfo) == 0x118, "wrong size");

typedef struct MatchmakerChatroom {
    /* 0000 */ int field_0;
    /* 0004 */ char name[80];
    /* 0054 */ int field_54;
    /* 0058 */ int field_58;
    /* 005C */ int field_5C;
    /* 0060 */ int field_60;
    /* 0064 */ int field_64;
    /* 0068 */ int field_68;
    /* 006C */ int field_6C;
    /* 0070 */ int field_70;
    /* 0074 */ int field_74;
    /* 0078 */ int field_78;
    /* 007C */ int field_7C;
    /* 0080 */ int field_80;
    /* 0084 */ int field_84;
    /* 0088 */ int flags;
    /* 008C */ int field_8C;
} MatchmakerChatroom;

static_assert(sizeof(MatchmakerChatroom) == 0x90, "wrong size");

typedef struct MatchmakerChatroomMember {
    /* 0000 */ char name[20];
    /* 0014 */ unsigned int flags;
} MatchmakerChatroomMember;

static_assert(sizeof(MatchmakerChatroomMember) == 0x18, "wrong size");

int matchmaker_init(MatchmakerInitInfo* init_info);
void matchmaker_exit();
int matchmaker_is_active();
int matchmaker_motd_get(char* a1, int a2, char* a3, int a4);
int matchmaker_login(const char* account, const char* password);
int matchmaker_create_account(const char* account, const char* password, const char* a3);
int matchmaker_version_needs_upgrade(const char* version);
int matchmaker_ad_rgb_get(unsigned char** rgb_ptr, int* width_ptr, int* height_ptr);
int matchmaker_ad_release();
int matchmaker_ad_clicked();
int matchmaker_gamelist_get(void** games, int* count);
int matchmaker_gamelist_free(void* games);
int matchmaker_chatserver_list_get(void** chatservers, int* count);
int matchmaker_chatserver_list_free(void* chatservers);
int matchmaker_chatserver_join(int a1);
int matchmaker_chatroom_list_get(void** chatrooms, int* count);
int matchmaker_chatroom_list_free(void* chatrooms);
int matchmaker_chatroom_join(MatchmakerChatroom* room, int a2);
int matchmaker_chatroom_get(int a1);
int matchmaker_chatroom_members_get(void** members, int* count);
int matchmaker_chatroom_members_free(void* members);
int matchmaker_chatroom_create(const char* a1, const char* a2);
int matchmaker_chatroom_mesg(const char* str);
int matchmaker_register(int a1);
int matchmaker_ping();

#endif /* ARCANUM_GAME_MATCHMAKER_H_ */
