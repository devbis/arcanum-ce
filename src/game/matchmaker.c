#include "game/matchmaker.h"

#include <tig/tig.h>

typedef int(MatchmakerInit)(int);
typedef void(MatchmakerExit)();
typedef int(MatchmakerIsActive)();
typedef int(MatchmakerMotdGet)();
typedef int(MatchmakerLogin)(int, int);
typedef int(MatchmakerCreateAccount)(int, int, int);
typedef int(MatchmakerVersionNeedsUpgrade)(int);
typedef int(MatchmakerGameListGet)(void**, int*);
typedef int(MatchmakerGameListFree)(void*);
typedef int(MatchmakerChatServerListGet)(void**, int*);
typedef int(MatchmakerChatServerListFree)(void*);
typedef int(MatchmakerChatServerJoin)(int);
typedef int(MatchmakerChatRoomListGet)(void**, int*);
typedef int(MatchmakerChatRoomListFree)(void*);
typedef int(MatchmakerChatRoomJoin)(int, int);
typedef int(MatchmakerChatRoomGet)(int);
typedef int(MatchmakerChatRoomMembersGet)(void**, int*);
typedef int(MatchmakerChatRoomMembersFree)(void*);
typedef int(MatchmakerChatRoomCreate)(const char*, const char*);
typedef int(MatchmakerChatRoomMesg)(const char*);
typedef int(MatchmakerRegister)(int);
typedef int(MatchmakerAdRgbGet)(int, int, int);
typedef int(MatchmakerAdRelease)();
typedef int(MatchmakerAdClicked)();
typedef int(MatchmakerPing)();

typedef struct MatchmakerState {
    /* 0000 */ MatchmakerInit* init;
    /* 0004 */ MatchmakerExit* exit;
    /* 0008 */ MatchmakerIsActive* is_active;
    /* 000C */ void* field_C;
    /* 0010 */ MatchmakerMotdGet* motd_get;
    /* 0014 */ MatchmakerLogin* login;
    /* 0018 */ void* field_18;
    /* 001C */ MatchmakerCreateAccount* create_account;
    /* 0020 */ MatchmakerVersionNeedsUpgrade* version_needs_upgrade;
    /* 0024 */ MatchmakerGameListGet* gamelist_get;
    /* 0028 */ MatchmakerGameListFree* gamelist_free;
    /* 002C */ MatchmakerRegister* reg;
    /* 0030 */ MatchmakerChatServerListGet* chatserver_list_get;
    /* 0034 */ MatchmakerChatServerListFree* chatserver_list_free;
    /* 0038 */ MatchmakerChatServerJoin* chatserver_join;
    /* 003C */ void* field_3C;
    /* 0040 */ void* field_40;
    /* 0044 */ MatchmakerChatRoomListGet* chatroom_list_get;
    /* 0048 */ MatchmakerChatRoomListFree* chatroom_list_free;
    /* 004C */ MatchmakerChatRoomJoin* chatroom_join;
    /* 0050 */ void* field_50;
    /* 0054 */ MatchmakerChatRoomGet* chatroom_get;
    /* 0058 */ MatchmakerChatRoomMesg* chatroom_mesg;
    /* 005C */ MatchmakerChatRoomMembersGet* chatroom_members_get;
    /* 0060 */ MatchmakerChatRoomMembersFree* chatroom_members_free;
    /* 0064 */ MatchmakerChatRoomCreate* chatroom_create;
    /* 0068 */ MatchmakerAdRgbGet* ad_rgb_get;
    /* 006C */ MatchmakerAdRelease* ad_release;
    /* 0070 */ MatchmakerAdClicked* ad_clicked;
    /* 0074 */ MatchmakerPing* ping;
    /* 0078 */ HMODULE module;
    /* 007C */ bool initialized;
} MatchmakerState;

static_assert(sizeof(MatchmakerState) == 0x80, "wrong size");

// 0x603D50
static MatchmakerState mm;

// 0x4F5820
int matchmaker_init(int a1)
{
    int err;
    if (mm.initialized) {
        tig_debug_printf("MM: Could not initialize, already initialized!\n");
        return 0;
    }

    mm.module = LoadLibrary("mm_won.dll");
    if (mm.module == NULL) {
        err = GetLastError();
        tig_debug_printf("MM: Could not initialize WON module, aborting (%s:%d).\n",
            strerror(err),
            err);
        return 0;
    }

    mm.init = (MatchmakerInit*)GetProcAddress(mm.module, "matchmaker_init");
    if (mm.init == NULL) {
        err = GetLastError();
        tig_debug_printf("MM: Could not find matchmaker_init, aborting (%s:%d).\n",
            strerror(err),
            err);
        FreeLibrary(mm.module);
        memset(&mm, 0, sizeof(mm));
        return 0;
    }

    if (!mm.init(a1)) {
        tig_debug_printf("MM: mm.init failed, aborting.\n");
        FreeLibrary(mm.module);
        memset(&mm, 0, sizeof(mm));
    }

    mm.initialized = true;
    return 1;
}

// 0x4F5910
void matchmaker_exit()
{
    if (!mm.initialized) {
        tig_debug_printf("MM: Could not exit, not currently active!\n");
        return;
    }

    if (mm.exit == NULL) {
        mm.exit = (MatchmakerExit*)GetProcAddress(mm.module, "matchmaker_exit");
    }

    if (mm.exit != NULL) {
        mm.exit();
    } else {
        tig_debug_printf("MM: Could not find matchmaker_exit, aborting.\n");
    }

    FreeLibrary(mm.module);
    memset(&mm, 0, sizeof(mm));
}

// 0x4F59E0
int matchmaker_is_active()
{
    if (!mm.initialized) {
        return 0;
    }

    if (mm.is_active != NULL) {
        return mm.is_active();
    }

    mm.is_active = (MatchmakerIsActive*)GetProcAddress(mm.module, "matchmaker_is_active");
    if (mm.is_active != NULL) {
        return mm.is_active();
    }

    tig_debug_printf("MM: Could not find matchmaker_is_active, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5A30
int matchmaker_motd_get(int a1, int a2, int a3, int a4)
{
    if (mm.motd_get != NULL) {
        return mm.motd_get(a1, a2, a3, a4);
    }

    mm.motd_get = (MatchmakerMotdGet*)GetProcAddress(mm.module, "matchmaker_motd_get");
    if (mm.motd_get != NULL) {
        return mm.motd_get(a1, a2, a3, a4);
    }

    tig_debug_printf("MM: Could not find matchmaker_motd_get, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5A90
int matchmaker_login(int a1, int a2)
{
    if (mm.login != NULL) {
        return mm.login(a1, a2);
    }

    mm.login = (MatchmakerLogin*)GetProcAddress(mm.module, "matchmaker_login");
    if (mm.login != NULL) {
        return mm.login(a1, a2);
    }

    tig_debug_printf("MM: Could not find matchmaker_login, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5B20
int matchmaker_create_account(int a1, int a2, int a3)
{
    if (mm.create_account != NULL) {
        return mm.create_account(a1, a2, a3);
    }

    mm.create_account = (MatchmakerCreateAccount*)GetProcAddress(mm.module, "matchmaker_create_account");
    if (mm.create_account != NULL) {
        return mm.create_account(a1, a2, a3);
    }

    tig_debug_printf("MM: Could not find matchmaker_create_account, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5B70
int matchmaker_version_needs_upgrade(int a1)
{
    if (mm.version_needs_upgrade != NULL) {
        return mm.version_needs_upgrade(a1);
    }

    mm.version_needs_upgrade = (MatchmakerVersionNeedsUpgrade*)GetProcAddress(mm.module, "matchmaker_version_needs_upgrade");
    if (mm.version_needs_upgrade != NULL) {
        return mm.version_needs_upgrade(a1);
    }

    tig_debug_printf("MM: Could not find matchmaker_version_needs_upgrade, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5BC0
int matchmaker_ad_rgb_get(int a1, int a2, int a3)
{
    if (mm.ad_rgb_get != NULL) {
        return mm.ad_rgb_get(a1, a2, a3);
    }

    mm.ad_rgb_get = (MatchmakerAdRgbGet*)GetProcAddress(mm.module, "matchmaker_ad_rgb_get");
    if (mm.ad_rgb_get != NULL) {
        return mm.ad_rgb_get(a1, a2, a3);
    }

    tig_debug_printf("MM: Could not find matchmaker_ad_rgb_get, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5C10
int matchmaker_ad_release()
{
    if (mm.ad_release != NULL) {
        return mm.ad_release();
    }

    mm.ad_release = (MatchmakerAdRelease*)GetProcAddress(mm.module, "matchmaker_ad_release");
    if (mm.ad_release != NULL) {
        return mm.ad_release();
    }

    tig_debug_printf("MM: Could not find matchmaker_ad_release, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5C50
int matchmaker_ad_clicked()
{
    if (mm.ad_clicked != NULL) {
        return mm.ad_clicked();
    }

    mm.ad_clicked = (MatchmakerAdClicked*)GetProcAddress(mm.module, "matchmaker_ad_clicked");
    if (mm.ad_clicked != NULL) {
        return mm.ad_clicked();
    }

    tig_debug_printf("MM: Could not find matchmaker_ad_clicked, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5C90
int matchmaker_gamelist_get(void** games, int* count)
{
    if (mm.gamelist_get != NULL) {
        return mm.gamelist_get(games, count);
    }

    mm.gamelist_get = (MatchmakerGameListGet*)GetProcAddress(mm.module, "matchmaker_gamelist_get");
    if (mm.gamelist_get != NULL) {
        return mm.gamelist_get(games, count);
    }

    tig_debug_printf("MM: Could not find matchmaker_gamelist_get, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5CE0
int matchmaker_gamelist_free(void* games)
{
    if (mm.gamelist_free != NULL) {
        return mm.gamelist_free(games);
    }

    mm.gamelist_free = (MatchmakerGameListFree*)GetProcAddress(mm.module, "matchmaker_gamelist_free");
    if (mm.gamelist_free != NULL) {
        return mm.gamelist_free(games);
    }

    tig_debug_printf("MM: Could not find matchmaker_gamelist_free, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5D30
int matchmaker_chatserver_list_get(void** chatservers, int* count)
{
    if (mm.chatserver_list_get != NULL) {
        return mm.chatserver_list_get(chatservers, count);
    }

    mm.chatserver_list_get = (MatchmakerChatServerListGet*)GetProcAddress(mm.module, "matchmaker_chatserver_list_get");
    if (mm.chatserver_list_get != NULL) {
        return mm.chatserver_list_get(chatservers, count);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatserver_list_get, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5D80
int matchmaker_chatserver_list_free(void* chatservers)
{
    if (mm.chatserver_list_free != NULL) {
        return mm.chatserver_list_free(chatservers);
    }

    mm.chatserver_list_free = (MatchmakerChatServerListFree*)GetProcAddress(mm.module, "matchmaker_chatserver_list_free");
    if (mm.chatserver_list_free != NULL) {
        return mm.chatserver_list_free(chatservers);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatserver_list_free, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5DD0
int matchmaker_chatserver_join(int a1)
{
    if (mm.chatserver_join != NULL) {
        return mm.chatserver_join(a1);
    }

    mm.chatserver_join = (MatchmakerChatServerJoin*)GetProcAddress(mm.module, "matchmaker_chatserver_join");
    if (mm.chatserver_join != NULL) {
        return mm.chatserver_join(a1);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatserver_join, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5E20
int matchmaker_chatroom_list_get(void** chatrooms, int* count)
{
    if (mm.chatroom_list_get != NULL) {
        return mm.chatroom_list_get(chatrooms, count);
    }

    mm.chatroom_list_get = (MatchmakerChatRoomListGet*)GetProcAddress(mm.module, "matchmaker_chatroom_list_get");
    if (mm.chatroom_list_get != NULL) {
        return mm.chatroom_list_get(chatrooms, count);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatroom_list_get, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5E70
int matchmaker_chatroom_list_free(void* chatrooms)
{
    if (mm.chatroom_list_free != NULL) {
        return mm.chatroom_list_free(chatrooms);
    }

    mm.chatroom_list_free = (MatchmakerChatRoomListFree*)GetProcAddress(mm.module, "matchmaker_chatroom_list_free");
    if (mm.chatroom_list_free != NULL) {
        return mm.chatroom_list_free(chatrooms);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatroom_list_free, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5EC0
int matchmaker_chatroom_join(int a1, int a2)
{
    if (mm.chatroom_join != NULL) {
        return mm.chatroom_join(a1, a2);
    }

    mm.chatroom_join = (MatchmakerChatRoomJoin*)GetProcAddress(mm.module, "matchmaker_chatroom_join");
    if (mm.chatroom_join != NULL) {
        return mm.chatroom_join(a1, a2);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatroom_join, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5F50
int matchmaker_chatroom_get(int a1)
{
    if (mm.chatroom_get != NULL) {
        return mm.chatroom_get(a1);
    }

    mm.chatroom_get = (MatchmakerChatRoomGet*)GetProcAddress(mm.module, "matchmaker_chatroom_get");
    if (mm.chatroom_get != NULL) {
        return mm.chatroom_get(a1);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatroom_get, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5FA0
int matchmaker_chatroom_members_get(void** members, int* count)
{
    if (mm.chatroom_members_get != NULL) {
        return mm.chatroom_members_get(members, count);
    }

    mm.chatroom_members_get = (MatchmakerChatRoomMembersGet*)GetProcAddress(mm.module, "matchmaker_chatroom_members_get");
    if (mm.chatroom_members_get != NULL) {
        return mm.chatroom_members_get(members, count);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatroom_members_get, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F5FF0
int matchmaker_chatroom_members_free(void* members)
{
    if (mm.chatroom_members_free != NULL) {
        return mm.chatroom_members_free(members);
    }

    mm.chatroom_members_free = (MatchmakerChatRoomMembersFree*)GetProcAddress(mm.module, "matchmaker_chatroom_members_free");
    if (mm.chatroom_members_free != NULL) {
        return mm.chatroom_members_free(members);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatroom_members_free, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F6040
int matchmaker_chatroom_create(const char* a1, const char* a2)
{
    if (mm.chatroom_create != NULL) {
        return mm.chatroom_create(a1, a2);
    }

    mm.chatroom_create = (MatchmakerChatRoomCreate*)GetProcAddress(mm.module, "matchmaker_chatroom_create");
    if (mm.chatroom_create != NULL) {
        return mm.chatroom_create(a1, a2);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatroom_create, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F6090
int matchmaker_chatroom_mesg(const char* str)
{
    if (mm.chatroom_mesg != NULL) {
        return mm.chatroom_mesg(str);
    }

    mm.chatroom_mesg = (MatchmakerChatRoomMesg*)GetProcAddress(mm.module, "matchmaker_chatroom_mesg");
    if (mm.chatroom_mesg != NULL) {
        return mm.chatroom_mesg(str);
    }

    tig_debug_printf("MM: Could not find matchmaker_chatroom_mesg, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F60E0
int matchmaker_register(int a1)
{
    if (mm.reg != NULL) {
        return mm.reg(a1);
    }

    mm.reg = (MatchmakerRegister*)GetProcAddress(mm.module, "matchmaker_register");
    if (mm.reg != NULL) {
        return mm.reg(a1);
    }

    tig_debug_printf("MM: Could not find matchmaker_register, aborting.\n");
    matchmaker_exit();

    return 0;
}

// 0x4F6130
int matchmaker_ping()
{
    if (mm.ping != NULL) {
        return mm.ping();
    }

    mm.ping = (MatchmakerPing*)GetProcAddress(mm.module, "matchmaker_ping");
    if (mm.ping != NULL) {
        return mm.ping();
    }

    tig_debug_printf("MM: Could not find matchmaker_ping, aborting.\n");
    matchmaker_exit();

    return 1;
}
