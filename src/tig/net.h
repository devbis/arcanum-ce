#ifndef ARCANUM_TIG_NET_H_
#define ARCANUM_TIG_NET_H_

#include "tig/types.h"

#include <winsock2.h>

#define TIG_NET_CLIENT_NAME_LENGTH 24
#define TIG_NET_MAX_PLAYERS 8

#define TIG_NET_MSG_0x401 (WM_USER + 1)
#define TIG_NET_MSG_0x402 (WM_USER + 2)

typedef struct TigNetPlayer {
    char name[TIG_NET_CLIENT_NAME_LENGTH];
    int field_18;
} TigNetPlayer;

static_assert(sizeof(TigNetPlayer) == 0x1C, "wrong size");

typedef struct TigNetBookmark {
    char name[TIG_NET_CLIENT_NAME_LENGTH - 1];
    char description[TIG_NET_CLIENT_NAME_LENGTH - 1];
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    struct sockaddr_in field_44;
    int field_54;
    int field_58;
    unsigned int flags;
    unsigned int time;
    TigNetPlayer field_64[TIG_NET_MAX_PLAYERS];
} TigNetBookmark;

// See 0x527B90.
static_assert(sizeof(TigNetBookmark) == 0x144, "wrong size");

typedef void(TigNetFunc62AC54)(int a1, int client_id, void* a3, int a4);
typedef void(TigNetFunc62AC58)(uint8_t* data);
typedef void(TigNetFunc62AC5C)(bool a1);
typedef void(TigNetFunc62AC60)(TigNetBookmark** bookmarks, int count);
typedef void(TigNetFunc62AC64)(uint8_t* data);

bool tig_net_client_is_active(int client_id);
bool tig_net_client_is_local(int client_id);
bool sub_526B20(int client_id);
const char* tig_net_client_info_get_name(int client_id);
bool tig_net_client_info_set_name(int client_id, const char* name);
int sub_526CB0(int client_id);
int sub_526CD0(int client_id);
int sub_526CF0(int client_id);
bool sub_526D20(int client_id);
int sub_526D60(int client_id);
void sub_526E00();
int tig_net_inet_string(unsigned long addr, char* dest);
bool tig_net_inet_addr(const char* cp, unsigned long* addr);
int tig_net_init(TigContext* ctx);
int tig_net_reset();
void tig_net_exit();
int sub_5271D0();
void tig_net_set_func_62AC54(TigNetFunc62AC54* func);
void tig_net_set_func_62AC58(TigNetFunc62AC58* func);
void tig_net_set_func_62AC5C(TigNetFunc62AC5C* func);
void tig_net_set_func_62AC60(TigNetFunc62AC60* func);
void tig_net_set_func_62AC64(TigNetFunc62AC64* func);
void tig_net_auto_join_enable();
void tig_net_auto_join_disable();
bool tig_net_auto_join_is_enabled();
void sub_528790();
void sub_5287A0();
void tig_net_ping();
void sub_528E80(HWND hWnd, WPARAM wParam, LPARAM lParam);
void sub_528FD0(HWND hWnd, WPARAM wParam, LPARAM lParam);
bool tig_net_local_client_set_name(const char* name);
const char* tig_net_local_client_get_name();
void tig_net_local_bookmark_set_name(const char* name);
void tig_net_local_bookmark_get_name(char* dest, size_t size);
int sub_529520();
void send_generic(void* data, int size, uint8_t type, int client_id);
int sub_52A530();
void tig_net_no_broadcast_enable();
void tig_net_no_broadcast_disable();

#endif /* ARCANUM_TIG_NET_H_ */
