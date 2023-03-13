#include "tig/net.h"

#include <stdio.h>

// clang-format off
// Order is important.
#include <winsock2.h>
#include <ws2tcpip.h>
// clang-format on

#include "tig/debug.h"
#include "tig/list.h"
#include "tig/timer.h"
#include "tig/video.h"

#define TIG_NET_PROTOCOL_VERSION 25
#define TIG_NET_PORT_31434 31434
#define TIG_NET_PORT_31435 31435
#define TIG_NET_BUF_SIZE 0x2000

#define TIG_NET_FLAG_0x01 0x01
#define TIG_NET_FLAG_0x02 0x02
#define TIG_NET_FLAG_0x04 0x04
#define TIG_NET_FLAG_0x08 0x08
#define TIG_NET_AUTO_JOIN 0x10
#define TIG_NET_NO_BROADCAST 0x20

#define TIG_NET_CLIENT_CONNECTED 0x01
#define TIG_NET_CLIENT_FLAG_0x02 0x02
#define TIG_NET_CLIENT_FLAG_0x04 0x04
#define TIG_NET_CLIENT_FLAG_0x08 0x08
#define TIG_NET_CLIENT_FLAG_0x10 0x10

#define TIG_NET_BOOKMARK_FLAG_0x10 0x10

typedef int(__stdcall WSAStartupFunc)(WORD, LPWSADATA);
typedef int(__stdcall WSACleanupFunc)();

// NOTE: There are no hints such struct exists in the original code.
typedef struct TigNetWinSock {
    void* __WSAFDIsSet;
    void* accept;
    void* bind;
    int(__stdcall* closesocket)(SOCKET s);
    void* connect;
    void* ioctlsocket;
    void* getpeername;
    void* getsockname;
    void* getsockopt;
    void* htonl;
    u_short(__stdcall* htons)(u_short hostshort);
    void* inet_addr;
    void* inet_ntoa;
    void* listen;
    void* ntohl;
    void* ntohs;
    void* recv;
    int(__stdcall* recvfrom)(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen);
    void* select;
    int(__stdcall* send)(SOCKET s, const char* buf, int len, int flags);
    void* sendto;
    void* setsockopt;
    void* shutdown;
    void* socket;
    void* gethostbyaddr;
    void* gethostbyname;
    int(__stdcall* gethostname)(char* name, int namelen);
    void* getservbyport;
    void* getservbyname;
    void* getprotobynumber;
    void* getprotobyname;
    void* WSAStartup;
    void* WSACleanup;
    void* WSASetLastError;
    int(__stdcall* WSAGetLastError)();
    void* WSAIsBlocking;
    void* WSAUnhookBlockingHook;
    void* WSASetBlockingHook;
    void* WSACancelBlockingCall;
    void* WSAAsyncGetServByName;
    void* WSAAsyncGetServByPort;
    void* WSAAsyncGetProtoByName;
    void* WSAAsyncGetProtoByNumber;
    void* WSAAsyncGetHostByName;
    void* WSAAsyncGetHostByAddr;
    void* WSACancelAsyncRequest;
    int(__stdcall* WSAAsyncSelect)(SOCKET s, HWND hWnd, u_int uMsg, int lEvent);
    void* WSAAccept;
    void* WSACloseEvent;
    void* WSAConnect;
    void* WSACreateEvent;
    void* WSADuplicateSocketA;
    void* WSADuplicateSocketW;
    void* WSAEnumNetworkEvents;
    void* WSAEnumProtocolsA;
    void* WSAEnumProtocolsW;
    void* WSAEventSelect;
    void* WSAEventSelect_0;
    void* WSAGetQOSByName;
    void* WSAHtonl;
    void* WSAHtons;
    void* WSAIoctl;
    void* WSAJoinLeaf;
    void* WSANtohl;
    void* WSANtohs;
    void* WSARecv;
    void* WSARecvDisconnect;
    void* WSARecvFrom;
    void* WSAResetEvent;
    void* WSASend;
    void* WSASendDisconnect;
    void* WSASendTo;
    void* WSASetEvent;
    void* WSASocketA;
    void* WSASocketW;
    void* WSAWaitForMultipleEvents;
    void* WSAAddressToStringA;
    void* WSAAddressToStringW;
    void* WSAStringToAddressA;
    void* WSAStringToAddressW;
    void* WSALookupServiceBeginA;
    void* WSALookupServiceBeginW;
    void* WSALookupServiceNextA;
    void* WSALookupServiceNextW;
    void* WSALookupServiceEnd;
    void* WSAInstallServiceClassA;
    void* WSAInstallServiceClassW;
    void* WSARemoveServiceClass;
    void* WSAGetServiceClassInfoA;
    void* WSAGetServiceClassInfoW;
    void* WSAEnumNameSpaceProvidersA;
    void* WSAEnumNameSpaceProvidersW;
    void* WSAGetServiceClassNameByClassIdA;
    void* WSAGetServiceClassNameByClassIdW;
    void* WSASetServiceA;
    void* WSASetServiceW;
};

typedef struct TigNetPacketHeader {
    uint8_t version;
    uint8_t type;
    uint16_t size;
};

typedef struct TigNetClientF2044 {
    unsigned int time;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
};

static_assert(sizeof(TigNetClientF2044) == 0x24, "wrong size");

typedef struct TigNetClient {
    int client_id;
    char name[TIG_NET_CLIENT_NAME_LENGTH];
    int flags;
    SOCKET socket;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    uint8_t field_34[8192];
    int field_2034;
    uint8_t* field_2038;
    uint8_t* field_203C;
    int field_2040;
    TigNetClientF2044 field_2044;
    int field_2068;
    unsigned int field_206C;
};

static_assert(sizeof(TigNetClient) == 0x2070, "wrong size");

typedef struct TigNetMsg_25_2 {
    TigNetPacketHeader header;
    uint32_t field_4;
};

// See 0x52B270.
static_assert(sizeof(TigNetMsg_25_2) == 0x08, "wrong size");

static int sub_526AB0(SOCKET s, fd_set* fds);
static void sub_526AD0(TigNetClient* client);
static void tig_net_client_info_init(TigNetClient* client);
static void sub_527170(TigNetClientF2044* a1);
static bool tig_net_ws_exit();
static void sub_527280();
static bool tig_net_ws_init();
static bool tig_net_ws_locate_funcs_1_1();
static bool tig_net_ws_locate_funcs_2_0();
static void sub_527AC0(int client_id);
static int sub_527AF0();
static void tig_net_bookmark_init(TigNetBookmark* bookmark);
static void sub_5280E0();
static void tig_net_bookmarks_load(const char* path);
static void tig_net_bookmarks_save(const char* path);

// 0x5C0648
static int dword_5C0648 = 127;

// 0x5C064C
static int dword_5C064C = -1;

// 0x6123A8
static struct sockaddr_in stru_6123A8;

// 0x6123B8
static SOCKET tig_net_broadcast_socket;

// 0x6123C0
static TigNetBookmark tig_net_local_bookmark;

// 0x612508
static struct sockaddr_in stru_612508;

// 0x612518
static SOCKET tig_net_listen_socket;

// 0x612520
static TigList tig_net_bookmarks;

// 0x0x612530
static TigNetWinSock tig_net_ws_impl;

// 0x612530
static int(__stdcall* tig_net_ws_WSAFDIsSet)(SOCKET s, fd_set* fds);

// 0x61253C
static int(__stdcall* tig_net_ws_closesocket)(SOCKET s);

// 0x61255C
static unsigned long(__stdcall* tig_net_ws_inet_addr)(const char* cp);

// 0x6125AC
static WSAStartupFunc* tig_net_ws_WSAStartup;

// 0x6125B0
static WSACleanupFunc* tig_net_ws_WSACleanup;

// 0x6126B0
static WSADATA tig_net_ws_data;

// 0x612840
static HMODULE tig_net_ws;

// 0x614848
static TigNetClient tig_net_local_client;

// 0x6188B8
static TigNetClient tig_net_clients[TIG_NET_MAX_PLAYERS];

// 0x62AC38
static char* dword_62AC38;

// 0x62AC40
static int dword_62AC40;

// 0x62AC44
static TigNetBookmark* dword_62AC44;

// 0x62AC4C
static int dword_62AC4C;

// 0x62AC50
static int dword_62AC50;

// 0x62AC54
static TigNetFunc62AC54* dword_62AC54;

// 0x62AC58
static TigNetFunc62AC58* dword_62AC58;

// 0x62AC5C
static TigNetFunc62AC5C* dword_62AC5C;

// 0x62AC60
static TigNetFunc62AC60* dword_62AC60;

// 0x62AC64
static TigNetFunc62AC64* dword_62AC64;

// 0x62AC68
static int dword_62AC68;

// 0x62AC6C
static unsigned int tig_net_flags;

// 0x62AC70
static bool tig_net_ws_initialized;

// 0x62AC88
static int dword_62AC88;

// 0x526AB0
int sub_526AB0(SOCKET s, fd_set* fds)
{
    return tig_net_ws_WSAFDIsSet(s, fds);
}

// 0x526AD0
void sub_526AD0(TigNetClient* client)
{
}

// 0x526AE0
bool tig_net_client_is_active(int client_id)
{
    return (tig_net_clients[client_id].flags & TIG_NET_CLIENT_CONNECTED) != 0;
}

// 0x526B00
bool tig_net_client_is_local(int client_id)
{
    return client_id == tig_net_local_client.client_id;
}

// 0x526B20
bool sub_526B20(int client_id)
{
    if (tig_net_client_is_active(client_id)) {
        return (tig_net_clients[client_id].flags & TIG_NET_CLIENT_FLAG_0x02) != 0;
    } else {
        return false;
    }
}

// 0x526B50
const char* tig_net_client_info_get_name(int client_id)
{
    if (tig_net_client_is_active(client_id)) {
        return tig_net_clients[client_id].name;
    } else {
        tig_debug_printf("TCP-NET: Error: calling tig_net_client_info_get_name on inactive slot %d\n", client_id);
        return "*INACTIVE*";
    }
}

// 0x526B90
bool tig_net_client_info_set_name(int client_id, const char* name)
{
    strncpy(tig_net_clients[client_id].name, name, TIG_NET_CLIENT_NAME_LENGTH - 1);
    strncpy(tig_net_local_bookmark.field_64[client_id].name, name, TIG_NET_CLIENT_NAME_LENGTH - 1);
    return true;
}

// 0x526CB0
int sub_526CB0(int client_id)
{
    return tig_net_clients[client_id].field_2044.field_8;
}

// 0x526CD0
int sub_526CD0(int client_id)
{
    return tig_net_clients[client_id].field_2044.field_4;
}

// 0x526CF0
int sub_526CF0(int client_id)
{
    return tig_timer_elapsed(tig_net_clients[client_id].field_2044.time);
}

// 0x526D20
bool sub_526D20(int client_id)
{
    memset(&(tig_net_clients[client_id].field_2044), 0, sizeof(TigNetClientF2044));
    tig_timer_start(&(tig_net_clients[client_id].field_2044.time));
    return true;
}

// 0x526D60
int sub_526D60(int client_id)
{
    if (tig_net_clients[client_id].field_206C != 0) {
        // NOTE: Rare case - result of `tig_timer_elapsed` performs unsigned
        // compare.
        unsigned int elapsed = tig_timer_elapsed(tig_net_clients[client_id].field_206C);
        if (elapsed > 1400) {
            tig_net_clients[client_id].field_2068 = elapsed;
            if ((tig_net_clients[client_id].flags & TIG_NET_CLIENT_FLAG_0x10) == 0) {
                tig_net_clients[client_id].flags |= TIG_NET_CLIENT_FLAG_0x10;

                if (dword_62AC54 != NULL) {
                    dword_62AC54(2, client_id, 0, 0);
                }
            }
        } else {
            if ((tig_net_clients[client_id].flags & TIG_NET_CLIENT_FLAG_0x10) != 0) {
                tig_net_clients[client_id].flags &= ~TIG_NET_CLIENT_FLAG_0x10;

                if (dword_62AC54 != NULL) {
                    dword_62AC54(3, client_id, 0, 0);
                }
            }
        }
    }

    return tig_net_clients[client_id].field_2068;
}

// 0x526E00
void sub_526E00()
{
    tig_net_flags &= ~(TIG_NET_FLAG_0x01 | TIG_NET_FLAG_0x02 | TIG_NET_FLAG_0x08);
}

// 0x526E10
int tig_net_inet_string(unsigned long addr, char* dest)
{
    return sprintf(dest, "%d.%d.%d.%d", addr & 0xFF, (addr >> 8) & 0xFF, (addr >> 16) & 0xFF, (addr >> 24) & 0xFF);
}

// 0x526E40
bool tig_net_inet_addr(const char* cp, unsigned long* addr)
{
    *addr = tig_net_ws_inet_addr(cp);
    return *addr != -1;
}

// 0x526E60
int tig_net_init(TigContext* ctx)
{
    tig_debug_printf("TIG-NET: Initializing...\n");

    tig_debug_printf("         %d Max Players, AutoJoin: %s, NoBroadcast: %s.\n",
        TIG_NET_MAX_PLAYERS,
        tig_net_auto_join_is_enabled() ? "On" : "Off",
        (tig_net_flags & TIG_NET_NO_BROADCAST) != 0 ? "On" : "Off");

    dword_62AC4C = 0;
    dword_5C0648 = 127;

    for (int index = 0; index < TIG_NET_MAX_PLAYERS; index++) {
        tig_net_client_info_init(&(tig_net_clients[index]));
    }

    tig_net_client_info_init(&tig_net_local_client);

    tig_net_broadcast_socket = INVALID_SOCKET;
    tig_net_listen_socket = INVALID_SOCKET;

    tig_net_bookmark_init(&tig_net_local_bookmark);
    tig_net_local_bookmark_set_name("Arcanum");
    tig_net_local_client_set_name("Player");

    tig_list_init(&tig_net_bookmarks, sizeof(TigNetBookmark));

    dword_62AC44 = NULL;
    sub_53AB00();
    dword_62AC38 = NULL;
    tig_debug_printf("         Done Initializing.\n");

    return TIG_OK;
}

// 0x526F50
void tig_net_client_info_init(TigNetClient* client)
{
    client->client_id = -1;
    client->name[0] = '\0';
    client->flags = 0;
    client->socket = INVALID_SOCKET;
    client->field_2034 = sizeof(client->field_34);
    memset(client->field_34, 0, sizeof(client->field_34));
    client->field_2040 = client->field_2034;
    client->field_2038 = client->field_34;
    client->field_203C = client->field_34;
    memset(&(client->field_2044), 0, sizeof(TigNetClientF2044));
    client->field_2068 = 0;
    client->field_206C = 0;
}

// 0x526FC0
int tig_net_reset()
{
    tig_debug_printf("TIG-NET: Resetting...\n");

    tig_debug_printf("         %d Max Players, AutoJoin: %s, NoBroadcast: %s.\n",
        TIG_NET_MAX_PLAYERS,
        tig_net_auto_join_is_enabled() ? "On" : "Off",
        (tig_net_flags & TIG_NET_NO_BROADCAST) != 0 ? "On" : "Off");

    dword_62AC4C = 0;
    dword_5C0648 = 127;

    for (int index = 0; index < TIG_NET_MAX_PLAYERS; index++) {
        tig_net_client_info_init(&(tig_net_clients[index]));
    }

    tig_net_client_info_init(&tig_net_local_client);

    tig_net_broadcast_socket = INVALID_SOCKET;
    tig_net_listen_socket = INVALID_SOCKET;
    tig_net_bookmark_init(&tig_net_local_bookmark);
    tig_net_local_bookmark_set_name("Arcanum");
    tig_net_local_client_set_name("Player");

    tig_list_exit(&tig_net_bookmarks);
    tig_list_init(&tig_net_bookmarks, sizeof(TigNetBookmark));

    if (dword_62AC38 != NULL) {
        free(dword_62AC38);
    }
    dword_62AC38 = NULL;

    tig_net_flags = 0;
    sub_5280F0();

    return TIG_OK;
}

// 0x5270C0
void tig_net_exit()
{
    tig_net_bookmarks_save(".\\data\\bookmark.txt");

    for (int index = 0; index < TIG_NET_MAX_PLAYERS; index++) {
        if (tig_net_client_is_active(index)) {
            sub_527170(&(tig_net_clients[index].field_2044));
        }

        sub_526AD0(&(tig_net_clients[index]));
    }

    sub_526AD0(&tig_net_local_client);

    tig_net_ws_exit();
    tig_list_exit(&tig_net_bookmarks);

    if (dword_62AC44 != NULL) {
        free(dword_62AC44);
        dword_62AC44 = NULL;
    }

    tig_net_flags = 0;

    sub_53AB20();

    if (dword_62AC38 != NULL) {
        free(dword_62AC38);
    }
}

// 0x527170
void sub_527170(TigNetClientF2044* a1)
{
}

// 0x527180
bool tig_net_ws_exit()
{
    if (tig_net_ws_initialized) {
        tig_debug_printf("TCP-NET: Closing WinSock services...\n");

        tig_net_ws_WSACleanup();

        FreeLibrary(tig_net_ws);
        tig_net_ws = NULL;

        tig_net_ws_initialized = false;
    }
}

// 0x5271D0
int sub_5271D0()
{
    if ((tig_net_flags & TIG_NET_FLAG_0x01) != 0) {
        return TIG_OK;
    }

    if (!tig_net_ws_init()) {
        return TIG_ERR_14;
    }

    tig_net_bookmarks_load(".\\data\\bookmark.txt");

    // TODO: Incomplete.

    if (sub_527AF0() == TIG_ERR_14) {
        return TIG_ERR_14;
    }

    if ((tig_net_flags & TIG_NET_NO_BROADCAST) == 0) {
        stru_6123A8.sin_family = AF_INET;
        stru_6123A8.sin_port = htons(TIG_NET_PORT_31434);
        stru_6123A8.sin_addr.s_addr = INADDR_ANY;
        memset(stru_6123A8.sin_zero, 0, sizeof(stru_6123A8.sin_zero));
    }

    sub_527280();
    sub_527AC0(0);
    tig_net_client_info_set_name(0, "ServerPlayer");

    tig_debug_printf("TCP-NET: [ Starting ]\n");
    if ((tig_net_flags & TIG_NET_NO_BROADCAST) == 0) {
        sub_527BD0();
    }

    return TIG_OK;
}

// 0x527280
void sub_527280()
{
    tig_net_flags |= TIG_NET_FLAG_0x01;
}

// 0x527290
bool tig_net_ws_init()
{
    if (tig_net_ws_initialized) {
        tig_debug_printf("TCP-NET: Error initializing winsock, it is already initialized, try again anyway.\n");
    }

    tig_net_ws_initialized = true;

    tig_net_ws = LoadLibraryA("ws2_32.dll");
    if (tig_net_ws == NULL) {
        tig_net_ws = LoadLibraryA("wsock32.dll");
        if (tig_net_ws == NULL) {
            tig_debug_printf("TCP-NET: Error no acceptible WinSock implementation available.\n");
            return true;
        }
    }

    tig_net_ws_WSAStartup = (WSAStartupFunc*)GetProcAddress(tig_net_ws, "WSAStartup");
    if (tig_net_ws_WSAStartup == NULL) {
        tig_debug_printf("TCP-NET: Error no acceptible WinSock implementation available.");
        return false;
    }

    if (tig_net_ws_WSAStartup(MAKEWORD(2, 0), &tig_net_ws_data) == 0) {
        if (HIBYTE(tig_net_ws_data.wVersion) == 2 && LOBYTE(tig_net_ws_data.wVersion) == 0) {
            tig_debug_printf("TCP-NET: WSAStartup complete, winsock version 2.0\n");
            tig_net_ws_locate_funcs_1_1();
            tig_net_ws_locate_funcs_2_0();
            return true;
        }
    }

    if (tig_net_ws_WSAStartup(MAKEWORD(1, 1), &tig_net_ws_data) == 0) {
        if (HIBYTE(tig_net_ws_data.wVersion) == 1 && LOBYTE(tig_net_ws_data.wVersion) == 1) {
            tig_debug_printf("TCP-NET: WSAStartup complete, winsock version 1.1\n");
            tig_net_ws_locate_funcs_1_1();
            return true;
        }
    }

    // FIXME: This pointer is not bound yet, calling it will crash the game.
    tig_net_ws_WSACleanup();
    tig_debug_printf("TCP-NET: WSAStartup could not find a valid WinSock implementation\n");

    return false;
}

// 0x5273B0
bool tig_net_ws_locate_funcs_1_1()
{
    tig_net_ws_impl.__WSAFDIsSet = GetProcAddress(tig_net_ws, "__WSAFDIsSet");
    tig_net_ws_impl.accept = GetProcAddress(tig_net_ws, "accept");
    tig_net_ws_impl.bind = GetProcAddress(tig_net_ws, "bind");
    tig_net_ws_impl.closesocket = GetProcAddress(tig_net_ws, "closesocket");
    tig_net_ws_impl.connect = GetProcAddress(tig_net_ws, "connect");
    tig_net_ws_impl.ioctlsocket = GetProcAddress(tig_net_ws, "ioctlsocket");
    tig_net_ws_impl.getpeername = GetProcAddress(tig_net_ws, "getpeername");
    tig_net_ws_impl.getsockname = GetProcAddress(tig_net_ws, "getsockname");
    tig_net_ws_impl.getsockopt = GetProcAddress(tig_net_ws, "getsockopt");
    tig_net_ws_impl.htonl = GetProcAddress(tig_net_ws, "htonl");
    tig_net_ws_impl.htons = GetProcAddress(tig_net_ws, "htons");
    tig_net_ws_impl.inet_addr = GetProcAddress(tig_net_ws, "tig_net_ws_inet_addr");
    tig_net_ws_impl.inet_ntoa = GetProcAddress(tig_net_ws, "inet_ntoa");
    tig_net_ws_impl.listen = GetProcAddress(tig_net_ws, "listen");
    tig_net_ws_impl.ntohl = GetProcAddress(tig_net_ws, "ntohl");
    tig_net_ws_impl.ntohs = GetProcAddress(tig_net_ws, "ntohs");
    tig_net_ws_impl.recv = GetProcAddress(tig_net_ws, "recv");
    tig_net_ws_impl.recvfrom = GetProcAddress(tig_net_ws, "recvfrom");
    tig_net_ws_impl.select = GetProcAddress(tig_net_ws, "select");
    tig_net_ws_impl.send = GetProcAddress(tig_net_ws, "send");
    tig_net_ws_impl.sendto = GetProcAddress(tig_net_ws, "sendto");
    tig_net_ws_impl.setsockopt = GetProcAddress(tig_net_ws, "setsockopt");
    tig_net_ws_impl.shutdown = GetProcAddress(tig_net_ws, "shutdown");
    tig_net_ws_impl.socket = GetProcAddress(tig_net_ws, "socket");
    tig_net_ws_impl.gethostbyaddr = GetProcAddress(tig_net_ws, "gethostbyaddr");
    tig_net_ws_impl.gethostbyname = GetProcAddress(tig_net_ws, "gethostbyname");
    tig_net_ws_impl.gethostname = GetProcAddress(tig_net_ws, "gethostname");
    tig_net_ws_impl.getservbyport = GetProcAddress(tig_net_ws, "getservbyport");
    tig_net_ws_impl.getservbyname = GetProcAddress(tig_net_ws, "getservbyname");
    tig_net_ws_impl.getprotobynumber = GetProcAddress(tig_net_ws, "getprotobynumber");
    tig_net_ws_impl.getprotobyname = GetProcAddress(tig_net_ws, "getprotobyname");
    tig_net_ws_impl.WSACleanup = GetProcAddress(tig_net_ws, "WSACleanup");
    tig_net_ws_impl.WSASetLastError = GetProcAddress(tig_net_ws, "WSASetLastError");
    tig_net_ws_impl.WSAGetLastError = GetProcAddress(tig_net_ws, "WSAGetLastError");
    tig_net_ws_impl.WSAIsBlocking = GetProcAddress(tig_net_ws, "WSAIsBlocking");
    tig_net_ws_impl.WSAUnhookBlockingHook = GetProcAddress(tig_net_ws, "WSAUnhookBlockingHook");
    tig_net_ws_impl.WSASetBlockingHook = GetProcAddress(tig_net_ws, "WSASetBlockingHook");
    tig_net_ws_impl.WSACancelBlockingCall = GetProcAddress(tig_net_ws, "WSACancelBlockingCall");
    tig_net_ws_impl.WSAAsyncGetServByName = GetProcAddress(tig_net_ws, "WSAAsyncGetServByName");
    tig_net_ws_impl.WSAAsyncGetServByPort = GetProcAddress(tig_net_ws, "WSAAsyncGetServByPort");
    tig_net_ws_impl.WSAAsyncGetProtoByName = GetProcAddress(tig_net_ws, "WSAAsyncGetProtoByName");
    tig_net_ws_impl.WSAAsyncGetProtoByNumber = GetProcAddress(tig_net_ws, "WSAAsyncGetProtoByNumber");
    tig_net_ws_impl.WSAAsyncGetHostByName = GetProcAddress(tig_net_ws, "WSAAsyncGetHostByName");
    tig_net_ws_impl.WSAAsyncGetHostByAddr = GetProcAddress(tig_net_ws, "WSAAsyncGetHostByAddr");
    tig_net_ws_impl.WSACancelAsyncRequest = GetProcAddress(tig_net_ws, "WSACancelAsyncRequest");
    tig_net_ws_impl.WSAAsyncSelect = GetProcAddress(tig_net_ws, "WSAAsyncSelect");
    return true;
}

// 0x527720
bool tig_net_ws_locate_funcs_2_0()
{
    tig_net_ws_impl.WSAAccept = GetProcAddress(tig_net_ws, "WSAAccept");
    tig_net_ws_impl.WSACloseEvent = GetProcAddress(tig_net_ws, "WSACloseEvent");
    tig_net_ws_impl.WSAConnect = GetProcAddress(tig_net_ws, "WSAConnect");
    tig_net_ws_impl.WSACreateEvent = GetProcAddress(tig_net_ws, "WSACreateEvent");
    tig_net_ws_impl.WSADuplicateSocketA = GetProcAddress(tig_net_ws, "WSADuplicateSocketA");
    tig_net_ws_impl.WSADuplicateSocketW = GetProcAddress(tig_net_ws, "WSADuplicateSocketW");
    tig_net_ws_impl.WSAEnumNetworkEvents = GetProcAddress(tig_net_ws, "WSAEnumNetworkEvents");
    tig_net_ws_impl.WSAEnumProtocolsA = GetProcAddress(tig_net_ws, "WSAEnumProtocolsA");
    tig_net_ws_impl.WSAEnumProtocolsW = GetProcAddress(tig_net_ws, "WSAEnumProtocolsW");
    tig_net_ws_impl.WSAEventSelect = GetProcAddress(tig_net_ws, "WSAEventSelect");
    tig_net_ws_impl.WSAEventSelect_0 = GetProcAddress(tig_net_ws, "WSAEventSelect");
    tig_net_ws_impl.WSAGetQOSByName = GetProcAddress(tig_net_ws, "WSAGetQOSByName");
    tig_net_ws_impl.WSAHtonl = GetProcAddress(tig_net_ws, "WSAHtonl");
    tig_net_ws_impl.WSAHtons = GetProcAddress(tig_net_ws, "WSAHtons");
    tig_net_ws_impl.WSAIoctl = GetProcAddress(tig_net_ws, "WSAIoctl");
    tig_net_ws_impl.WSAJoinLeaf = GetProcAddress(tig_net_ws, "WSAJoinLeaf");
    tig_net_ws_impl.WSANtohl = GetProcAddress(tig_net_ws, "WSANtohl");
    tig_net_ws_impl.WSANtohs = GetProcAddress(tig_net_ws, "WSANtohs");
    tig_net_ws_impl.WSARecv = GetProcAddress(tig_net_ws, "WSARecv");
    tig_net_ws_impl.WSARecvDisconnect = GetProcAddress(tig_net_ws, "WSARecvDisconnect");
    tig_net_ws_impl.WSARecvFrom = GetProcAddress(tig_net_ws, "WSARecvFrom");
    tig_net_ws_impl.WSAResetEvent = GetProcAddress(tig_net_ws, "WSAResetEvent");
    tig_net_ws_impl.WSASend = GetProcAddress(tig_net_ws, "WSASend");
    tig_net_ws_impl.WSASendDisconnect = GetProcAddress(tig_net_ws, "WSASendDisconnect");
    tig_net_ws_impl.WSASendTo = GetProcAddress(tig_net_ws, "WSASendTo");
    tig_net_ws_impl.WSASetEvent = GetProcAddress(tig_net_ws, "WSASetEvent");
    tig_net_ws_impl.WSASocketA = GetProcAddress(tig_net_ws, "WSASocketA");
    tig_net_ws_impl.WSASocketW = GetProcAddress(tig_net_ws, "WSASocketW");
    tig_net_ws_impl.WSAWaitForMultipleEvents = GetProcAddress(tig_net_ws, "WSAWaitForMultipleEvents");
    tig_net_ws_impl.WSAAddressToStringA = GetProcAddress(tig_net_ws, "WSAAddressToStringA");
    tig_net_ws_impl.WSAAddressToStringW = GetProcAddress(tig_net_ws, "WSAAddressToStringW");
    tig_net_ws_impl.WSAStringToAddressA = GetProcAddress(tig_net_ws, "WSAStringToAddressA");
    tig_net_ws_impl.WSAStringToAddressW = GetProcAddress(tig_net_ws, "WSAStringToAddressW");
    tig_net_ws_impl.WSALookupServiceBeginA = GetProcAddress(tig_net_ws, "WSALookupServiceBeginA");
    tig_net_ws_impl.WSALookupServiceBeginW = GetProcAddress(tig_net_ws, "WSALookupServiceBeginW");
    tig_net_ws_impl.WSALookupServiceNextA = GetProcAddress(tig_net_ws, "WSALookupServiceNextA");
    tig_net_ws_impl.WSALookupServiceNextW = GetProcAddress(tig_net_ws, "WSALookupServiceNextW");
    tig_net_ws_impl.WSALookupServiceEnd = GetProcAddress(tig_net_ws, "WSALookupServiceEnd");
    tig_net_ws_impl.WSAInstallServiceClassA = GetProcAddress(tig_net_ws, "WSAInstallServiceClassA");
    tig_net_ws_impl.WSAInstallServiceClassW = GetProcAddress(tig_net_ws, "WSAInstallServiceClassW");
    tig_net_ws_impl.WSARemoveServiceClass = GetProcAddress(tig_net_ws, "WSARemoveServiceClass");
    tig_net_ws_impl.WSAGetServiceClassInfoA = GetProcAddress(tig_net_ws, "WSAGetServiceClassInfoA");
    tig_net_ws_impl.WSAGetServiceClassInfoW = GetProcAddress(tig_net_ws, "WSAGetServiceClassInfoW");
    tig_net_ws_impl.WSAEnumNameSpaceProvidersA = GetProcAddress(tig_net_ws, "WSAEnumNameSpaceProvidersA");
    tig_net_ws_impl.WSAEnumNameSpaceProvidersW = GetProcAddress(tig_net_ws, "WSAEnumNameSpaceProvidersW");
    tig_net_ws_impl.WSAGetServiceClassNameByClassIdA = GetProcAddress(tig_net_ws, "WSAGetServiceClassNameByClassIdA");
    tig_net_ws_impl.WSAGetServiceClassNameByClassIdW = GetProcAddress(tig_net_ws, "WSAGetServiceClassNameByClassIdW");
    tig_net_ws_impl.WSASetServiceA = GetProcAddress(tig_net_ws, "WSASetServiceA");
    tig_net_ws_impl.WSASetServiceW = GetProcAddress(tig_net_ws, "WSASetServiceW");
    return true;
}

// 0x527AC0
void sub_527AC0(int client_id)
{
    tig_net_clients[client_id].flags |= TIG_NET_CLIENT_CONNECTED;
    tig_net_clients[client_id].client_id = client_id;
}

// 0x527AF0
int sub_527AF0()
{
    stru_612508.sin_family = AF_INET;
    stru_612508.sin_port = tig_net_ws_impl.htons(TIG_NET_PORT_31435);
    stru_612508.sin_addr.s_addr = INADDR_ANY;
    memset(stru_612508.sin_zero, 0, sizeof(stru_612508.sin_zero));

    char hostname[80];
    tig_net_ws_impl.gethostname(hostname, sizeof(hostname));

    struct hostent* h = gethostbyname(hostname);
    if (h == NULL) {
        tig_debug_printf("TCP-NET: Error: could not get hostname failing start\n");
        return TIG_ERR_14;
    }

    memcpy(&(stru_612508.sin_addr), h->h_addr, h->h_length);

    char address[16];
    tig_net_inet_string(stru_612508.sin_addr.s_addr, address);
    tig_debug_printf("TCP-NET: Hostname: %s, Address: %s\n", hostname, address);

    return TIG_OK;
}

// 0x527B90
void tig_net_bookmark_init(TigNetBookmark* bookmark)
{
    memset(bookmark, 0, sizeof(*bookmark));
    bookmark->field_3C = 8;
    bookmark->flags = 0;
    bookmark->field_30 = 0;
    bookmark->field_38 = 50;
    tig_timer_start(&(bookmark->time));
}

// 0x5280E0
void sub_5280E0()
{
    tig_net_flags |= TIG_NET_FLAG_0x02;
}

// 0x527BD0
bool sub_527BD0()
{
    if ((tig_net_flags & TIG_NET_NO_BROADCAST) != 0) {
        return true;
    }

    tig_net_broadcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (tig_net_broadcast_socket == -1) {
        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Could not create the broadcast socket, WSAError: %s\n",
            strerror(err));
        return false;
    }

    HWND hWnd;
    if (tig_video_handle(&hWnd) != TIG_OK) {
        tig_debug_printf("TCP-NET: Error: Could not get tig_video_handle\n");
        return false;
    }

    if (tig_net_ws_impl.WSAAsyncSelect(tig_net_broadcast_socket, hWnd, TIG_NET_MSG_0x402, 1) == -1) {
        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Error: BCAST Socket WSAAsyncSelect failed, WSAError: %s\n",
            strerror(err));
        return false;
    }

    int broadcast = 1;
    if (setsockopt(tig_net_broadcast_socket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) == -1) {
        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Error: BCAST Socket setsockopt failed, WSAError: %s\n",
            strerror(err));
        return false;
    }

    if (bind(tig_net_broadcast_socket, (struct sockaddr*)&stru_6123A8, sizeof(stru_6123A8)) == -1) {
        char address[16];
        tig_net_inet_string(stru_6123A8.sin_addr.s_addr, address);

        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Error: BCAST Socket BIND to( %s/%d )failed, WSAError: %s(%d)\n",
            address,
            stru_6123A8.sin_port,
            strerror(err),
            err);
        return false;
    }

    INTERFACE_INFO interfaces[30];
    DWORD bytes_returned;
    if (WSAIoctl != NULL && WSAIoctl(tig_net_broadcast_socket, SIO_GET_INTERFACE_LIST, NULL, 0, interfaces, sizeof(interfaces), &bytes_returned, NULL, NULL) != -1) {
        int index;
        for (index = 0; index < bytes_returned / sizeof(INTERFACE_INFO); index++) {
            if ((interfaces[index].iiFlags & IFF_BROADCAST) != 0 && (interfaces[index].iiFlags & IFF_LOOPBACK) == 0) {
                break;
            }
        }

        // TODO: Not sure, check.
        stru_6123A8.sin_addr.s_addr = (~interfaces[index].iiNetmask.AddressIn.sin_addr.s_addr)
            | (interfaces[index].iiNetmask.AddressIn.sin_addr.s_addr & interfaces[index].iiAddress.sin_addr.s_addr);
    } else {
        // FIXME: If `WSAIoctl` is null (winsock 1.1), the error code returned
        // from `WSAGetLastError` will refer to previous call (`bind` in this
        // case).
        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Error: Primary Broadcast Method Failed: WSA Error %s\n",
            strerror(err));

        stru_6123A8.sin_addr.s_addr = INADDR_BROADCAST;
        memset(&(tig_net_local_bookmark.field_44), 0, sizeof(tig_net_local_bookmark.field_44));
    }

    char address[16];
    tig_net_inet_string(stru_6123A8.sin_addr.s_addr, address);
    tig_debug_printf("TCP-NET: Successfully bound broadcast address to %s/%d.\n",
        address,
        stru_6123A8.sin_port);

    return true;
}

// 0x527E80
bool sub_527E80()
{
    tig_debug_printf("TCP-NET: Changing to Host-Attempt State, creating sockets.\n");

    if ((tig_net_flags & TIG_NET_FLAG_0x01) == 0) {
        if (sub_5271D0() != TIG_OK) {
            // FIXME: Should be `false`.
            return TIG_ERR_14;
        }
    }

    if (sub_527AF0() == TIG_ERR_14) {
        // FIXME: Should be `false`.
        return TIG_ERR_14;
    }

    for (int index = 1; index < TIG_NET_MAX_PLAYERS; index++) {
        sub_526AD0(&(tig_net_clients[index]));
        tig_net_client_info_init(&(tig_net_clients[index]));
        // TODO: Unclear string value.
        tig_net_client_info_set_name(index, "");
    }

    tig_net_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tig_net_listen_socket == INVALID_SOCKET) {
        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Error could not create listen socket, WSAError: %s\n",
            strerror(err));
        return false;
    }

    int no_delay = 1;
    if (setsockopt(tig_net_listen_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&no_delay, sizeof(no_delay)) == -1) {
        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Error, could not set TCP_NODELAY on local socket, WSAError: %s(%d).\n",
            strerror(err),
            err);
        // NOTE: Processing not halted, not sure if it's a bug or intended
        // behaviour.
    }

    HWND hWnd;
    if (!tig_video_handle(&hWnd)) {
        tig_debug_printf("TCP-NET: Error: Could not get tig_video_handle\n");
        tig_net_ws_impl.closesocket(tig_net_listen_socket);
        tig_net_listen_socket = -1;
        return false;
    }

    if (WSAAsyncSelect(tig_net_listen_socket, hWnd, TIG_NET_MSG_0x401, 43) == -1) {
        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Error: Listen socket WSAAsyncSelect failed, WSAError: %s(%d).\n",
            strerror(err),
            err);
        tig_net_ws_impl.closesocket(tig_net_listen_socket);
        tig_net_listen_socket = -1;
        return false;
    }

    if (bind(tig_net_listen_socket, (struct sockaddr*)&stru_612508, sizeof(stru_612508)) == -1) {
        // NOTE: Calls `WSAGetLastError` twice.
        tig_debug_printf("TCP-NET: Error Listen Socket bind failed, WSAError: %s(%d)\n",
            strerror(tig_net_ws_impl.WSAGetLastError()),
            tig_net_ws_impl.WSAGetLastError());
        tig_net_ws_impl.closesocket(tig_net_listen_socket);
        tig_net_listen_socket = -1;
        return false;
    }

    if (listen(tig_net_listen_socket, 5) == -1) {
        int err = tig_net_ws_impl.WSAGetLastError();
        tig_debug_printf("TCP-NET: Error Listen Socket listen failed, WSAError: %s\n",
            strerror(err));
        tig_net_ws_impl.closesocket(tig_net_listen_socket);
        tig_net_listen_socket = -1;
        return false;
    }

    if (tig_net_local_bookmark.field_44.sin_port == 0) {
        int length = sizeof(tig_net_local_bookmark.field_44);
        getsockname(tig_net_listen_socket, (struct sockaddr*)&(tig_net_local_bookmark.field_44), &length);
        tig_net_local_bookmark.field_44.sin_family = AF_INET;
        tig_net_local_bookmark.field_44.sin_port = tig_net_ws_impl.htons(TIG_NET_PORT_31435);
    }

    sub_5280E0();

    return true;
}

// 0x5280F0
bool sub_5280F0()
{
    for (int index = 0; index < TIG_NET_MAX_PLAYERS; index++) {
        if ((tig_net_clients[index].flags & TIG_NET_CLIENT_CONNECTED) != 0) {
            sub_535080(index);
            tig_net_ws_closesocket(tig_net_clients[index].socket);
            tig_net_clients[index].socket = INVALID_SOCKET;
            tig_net_clients[index].flags &= ~TIG_NET_CLIENT_CONNECTED;
        }
    }

    if (tig_net_listen_socket != INVALID_SOCKET) {
        tig_net_ws_closesocket(tig_net_listen_socket);
        tig_net_listen_socket = INVALID_SOCKET;
    }

    if (tig_net_broadcast_socket != INVALID_SOCKET) {
        tig_net_ws_closesocket(tig_net_broadcast_socket);
        tig_net_broadcast_socket = INVALID_SOCKET;
    }

    tig_net_ws_exit();
    sub_526E00();

    return true;
}

// 0x5286E0
bool sub_5286E0()
{
    tig_net_flags |= TIG_NET_FLAG_0x04;

    if (dword_62AC54 != NULL) {
        dword_62AC54(11, -1, 0, 0);
    }

    return true;
}

// 0x528710
void tig_net_set_func_62AC54(TigNetFunc62AC54* func)
{
    dword_62AC54 = func;
}

// 0x528720
void tig_net_set_func_62AC58(TigNetFunc62AC58* func)
{
    dword_62AC58 = func;
}

// 0x528730
void tig_net_set_func_62AC5C(TigNetFunc62AC5C* func)
{
    dword_62AC5C = func;
}

// 0x528740
void tig_net_set_func_62AC60(TigNetFunc62AC60* func)
{
    dword_62AC60 = func;
}

// 0x528750
void tig_net_set_func_62AC64(TigNetFunc62AC64* func)
{
    dword_62AC64 = func;
}

// 0x528760
void tig_net_auto_join_enable()
{
    tig_net_flags |= TIG_NET_AUTO_JOIN;
}

// 0x528770
void tig_net_auto_join_disable()
{
    tig_net_flags &= ~TIG_NET_AUTO_JOIN;
}

// 0x528780
bool tig_net_auto_join_is_enabled()
{
    return (tig_net_flags & TIG_NET_AUTO_JOIN) != 0;
}

// 0x528790
void sub_528790()
{
    dword_62AC4C++;
}

// 0x5287A0
void sub_5287A0()
{
    dword_62AC4C--;
}

// 0x5287B0
void tig_net_ping()
{
    // TODO: Incomplete.
}

// 0x5289B0
void sub_5289B0()
{
    // TODO: Incomplete.
}

// 0x528A50
void sub_528A50()
{
    // TODO: Incomplete.
}

// 0x528B70
void tig_net_msg_read(SOCKET s)
{
    // TODO: Incomplete.
}

// 0x528D60
TigNetClient* tig_net_get_client_by_socket(SOCKET s)
{
    if ((tig_net_flags & TIG_NET_FLAG_0x02) == 0 && s == tig_net_listen_socket) {
        return &tig_net_local_client;
    }

    for (int index = 0; TIG_NET_MAX_PLAYERS; index++) {
        if (tig_net_client_is_active(index) && tig_net_clients[index].socket == s) {
            return &(tig_net_clients[index]);
        }
    }

    return NULL;
}

// 0x528DD0
void sub_528DD0(SOCKET s)
{
    TigNetClient* client = tig_net_get_client_by_socket(s);

    tig_net_ws_impl.closesocket(s);

    if (client != NULL) {
        int client_id;
        if ((tig_net_flags & TIG_NET_FLAG_0x02) != 0) {
            client_id = client->client_id;
        } else {
            client = &(tig_net_clients[0]);
            client_id = 0;
        }

        if (dword_62AC54 != NULL) {
            if (client_id != -1) {
                dword_62AC54(8, client_id, NULL, 0);
            } else {
                int v1 = 0;
                dword_62AC54(8, -1, &v1, 4);
            }
        }

        sub_535080(client_id);

        tig_debug_printf("TCP-NET: Client %s (slot %d) is disconnecting.\n", client->name, client_id);

        // TODO: Odd value.
        tig_net_client_info_set_name(client_id, "");
        sub_526AD0(client);
        tig_net_client_info_init(client);

        if (dword_62AC54 != NULL) {
            dword_62AC54(0, client_id, NULL, 0);
        }
    }
}

// 0x528E80
void sub_528E80(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    uint8_t buffer[1024];
    struct sockaddr from;
    int from_len = sizeof(from);

    if ((tig_net_flags & TIG_NET_NO_BROADCAST) != 0) {
        tig_debug_printf("TCP-NET: -BCAST- -mpnobcast is set, yet we're getting a bcast message. WHAT? Exiting. (remove for release).\n");
        exit(EXIT_SUCCESS);
    }

    if (HIWORD(lParam) != 0) {
        tig_debug_printf("TCP-NET: -BCAST- AsyncSelect Error: %s (%d).\n",
            strerror(HIWORD(lParam)),
            HIWORD(lParam));
    }

    // TODO: Unclear, check.
    switch (LOWORD(lParam)) {
    case 1:
        if (tig_net_ws_impl.recvfrom((SOCKET)wParam, (char*)buffer, sizeof(buffer), 0, &from, &from_len) != -1) {
            sub_529C20(buffer, 0);
        } else {
            // NOTE: This value is ignored.
            tig_net_ws_impl.WSAGetLastError();
        }
        break;
    case 8:
        tig_debug_printf("TCP-NET: -BCAST- AsyncMessage: Broadcast Should Not Receive An Accept Message: Socket %d.\n", (SOCKET)wParam);
        break;
    case 16:
        tig_debug_printf("TCP-NET: -BCAST- AsyncMessage: Broadcast Should Not Receive A Connect Message: Socket %d.\n", (SOCKET)wParam);
        break;
    case 32:
        tig_debug_printf("TCP-NET: -BCAST- AsyncMessage: Broadcast Should Not Receive A Close Message: Socket %d.\n", (SOCKET)wParam);
        break;
    }
}

// 0x528FD0
void sub_528FD0(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    if (dword_62AC4C != 0) {
        PostMessageA(hWnd, TIG_NET_MSG_0x401, wParam, lParam);
        return;
    }

    if (HIWORD(lParam) != 0) {
        if (HIWORD(lParam) == 0x2745) {
            return;
        }

        tig_debug_printf("TCP-NET: AsyncSelect Error: %s (%d).\n",
            strerror(HIWORD(lParam)),
            HIWORD(lParam));
    }

    switch (LOWORD(lParam)) {
    case 1:
        tig_net_msg_read((SOCKET)wParam);
        break;
    case 2:
        tig_debug_printf("TCP-NET: AsyncMessage: Socket %d Write.\n", (SOCKET)wParam);
        break;
    case 8:
        sub_5290E0(hWnd, wParam);
        break;
    case 16:
        tig_debug_printf("TCP-NET: AsyncMessage: Socket %d Connect.\n", (SOCKET)wParam);
        break;
    case 32:
        sub_528DD0((SOCKET)wParam);
        break;
    }
}

// 0x5290E0
void sub_5290E0(HWND hWnd, WPARAM wParam)
{
    // TODO: Incomplete.
}

// 0x5292B0
int sub_5292B0()
{
    // TODO: Incomplete.
}

// 0x5292F0
bool sub_5292F0(const char* description)
{
    // TODO: Unclear.
    strncpy(tig_net_local_bookmark.description, description, TIG_NET_CLIENT_NAME_LENGTH - 1);
    return true;
}

// 0x529310
bool sub_529310(int a1)
{
    if (a1 > 0 && a1 <= 8) {
        tig_net_local_bookmark.field_3C = a1;
        return true;
    } else {
        return false;
    }
}

// 0x529330
int sub_529330()
{
    return tig_net_local_bookmark.field_3C;
}

// 0x529340
bool tig_net_local_client_set_name(const char* name)
{
    strncpy(tig_net_local_client.name, name, TIG_NET_CLIENT_NAME_LENGTH - 1);
    return true;
}

// 0x529360
const char* tig_net_local_client_get_name()
{
    return tig_net_local_client.name;
}

// 0x529370
void tig_net_local_bookmark_set_name(const char* name)
{
    strncpy(tig_net_local_bookmark.name, name, TIG_NET_CLIENT_NAME_LENGTH - 1);
}

// 0x529390
void tig_net_local_bookmark_get_name(char* dest, size_t size)
{
    strncpy(dest, tig_net_local_bookmark.name, size);
}

// 0x5293C0
void sub_5293C0(const char* a1)
{
    if (dword_62AC38 != NULL) {
        free(dword_62AC38);
        dword_62AC38 = NULL;
        sub_529500(8);
    }

    if (a1 != NULL && *a1 != '\0') {
        dword_62AC38 = strdup(a1);
        sub_5294E0(8);
    }
}

// 0x529410
size_t sub_529410()
{
    return dword_62AC38 != NULL ? strlen(dword_62AC38) : 0;
}

// 0x529430
void sub_529430()
{
    sub_5293C0(0);
}

// 0x529440
void sub_529440(int* a1, int* a2)
{
    if (a1 != NULL) {
        *a1 = tig_net_local_bookmark.field_34;
    }

    if (a2 != NULL) {
        *a2 = tig_net_local_bookmark.field_38;
    }
}

// 0x529470
void sub_529470(int a1, int a2)
{
    if (a1 >= 0 && a2 > 0) {
        tig_net_local_bookmark.field_34 = a1 > a2 ? a2 : a1;
        tig_net_local_bookmark.field_38 = a2;
    }
}

// 0x5294A0
void sub_5294A0(int* a1)
{
    if (a1 != NULL) {
        *a1 = tig_net_local_bookmark.field_30;
    }
}

// 0x5294C0
void sub_5294C0(int a1)
{
    tig_net_local_bookmark.field_30 = a1;
}

// 0x5294D0
int sub_5294D0()
{
    return tig_net_local_bookmark.flags;
}

// 0x5294E0
void sub_5294E0(int a1)
{
    tig_net_local_bookmark.flags |= a1;
}

// 0x529500
void sub_529500(int a1)
{
    tig_net_local_bookmark.flags &= ~a1;
}

// 0x529520
int sub_529520()
{
    return (tig_net_flags & TIG_NET_FLAG_0x02) == 0 ? tig_net_local_client.client_id : 0;
}

// 0x529540
void send_generic(void* data, int size, uint8_t type, int client_id)
{
    // 0x612844
    static TigNetPacket packet;

    if (size + sizeof(TigNetPacketHeader) > TIG_NET_BUF_SIZE) {
        tig_debug_pritnf("TCP-NET: send_generic(): too big a buffer requested %d!\n", size);
        return;
    }

    packet.header.version = TIG_NET_PROTOCOL_VERSION;
    packet.header.type = type;
    packet.header.size = size + sizeof(TigNetPacketHeader);
    memcpy(packet.data, data, size);

    if ((tig_net_flags & TIG_NET_FLAG_0x02) != 0) {
        if (tig_net_client_is_active(client_id)) {
            if (!sub_52AC40(client_id)) {
                tig_net_send_to_client_internal(&packet, packet.header.size, client_id);
            }
        }
    } else {
        sub_529830(&packet, packet.header.size);
    }
}

// 0x5295F0
void sub_5295F0(void* data, int size)
{
    // 0x628C38
    static TigNetPacket packet;

    packet.header.version = TIG_NET_PROTOCOL_VERSION;
    packet.header.type = 3;
    packet.header.size = size + sizeof(TigNetPacketHeader);
    memcpy(packet.data, data, size);

    if ((tig_net_flags & TIG_NET_FLAG_0x02) != 0) {
        for (int index = 1; index < TIG_NET_MAX_PLAYERS; index++) {
            if (tig_net_client_is_active(index)) {
                if (!sub_52AC40(index)) {
                    tig_net_send_to_client_internal(&(packet.header), packet.header.size, index);
                }
            }
        }
    } else {
        sub_529830(&(packet.header), packet.header.size);
    }
}

// 0x529690
void sub_529690(void* data, int size, int ignored_client_id)
{
    // 0x6168B8
    static uint8_t packet[TIG_NET_BUF_SIZE];

    TigNetPacketHeader* header = (TigNetPacketHeader*)packet;
    header->version = TIG_NET_PROTOCOL_VERSION;
    header->type = 3;
    header->size = size + sizeof(*header);
    memcpy((uint8_t*)packet + sizeof(*header), data, size);

    if ((tig_net_flags & TIG_NET_FLAG_0x02) != 0) {
        for (int index = 1; index < TIG_NET_MAX_PLAYERS; index++) {
            if (index != ignored_client_id) {
                if (tig_net_client_is_active(index)) {
                    if (!sub_52AC40(index)) {
                        tig_net_send_to_client_internal(header, header->size, index);
                    }
                }
            }
        }
    }
}

// 0x529730
bool sub_529730(void* data, int size, int client_id)
{
    // 0x6103A8
    static uint8_t packet[TIG_NET_BUF_SIZE];

    TigNetPacketHeader* header = (TigNetPacketHeader*)packet;
    header->version = TIG_NET_PROTOCOL_VERSION;
    header->type = 3;
    header->size = size + sizeof(*header);
    memcpy((uint8_t*)packet + sizeof(*header), data, size);
    return tig_net_send_to_client_internal(header, header->size, client_id);
}

// 0x529790
bool tig_net_send_to_client_internal(TigNetHeader* header, int size, int client_id)
{
    header->version = TIG_NET_PROTOCOL_VERSION;
    header->size = size;
    tig_net_clients[client_id].field_2044.field_8 += size;

    if ((tig_net_flags & TIG_NET_FLAG_0x02) != 0) {
        tig_net_clients[0].field_2044.field_8 += size;
    }

    if (!tig_net_client_is_active(client_id)) {
        tig_debug_printf("TCP-NET: Error: tig_net_send_to_client_internal on non active client, slot %d\n", client_id);
        return false;
    }

    if (!tig_net_send_to_socket(header, size, tig_net_clients[client_id].socket)) {
        tig_net_clients[client_id].socket = INVALID_SOCKET;
        sub_535080(client_id);
        return false;
    }

    return true;
}

// 0x529830
bool sub_529830(TigNetPacketHeader* header, int size)
{
    header->version = TIG_NET_PROTOCOL_VERSION;
    header->size = size;

    if (!tig_net_send_to_socket(header, size, tig_net_listen_socket)) {
        tig_net_listen_socket = INVALID_SOCKET;
        sub_535080(0);

        return false;
    }

    return true;
}

// 0x529870
bool tig_net_send_to_socket(TigNetPacketHeader* header, int size, SOCKET s)
{
    // TODO: Incomplete.
}

// 0x529920
bool sub_529920(TigNetPacketHeader* header, const char* msg)
{
    // TODO: Incomplete.
}

// 0x529A00
bool tig_net_can_parse_packet(TigNetPacketHeader* header, const char* msg)
{
    bool can_parse = true;

    if (header->version != TIG_NET_PROTOCOL_VERSION) {
        tig_debug_printf("TCP-NET: Error: VALIDATION(%s): header->version (0x%X) != TIG_NET_PROTOCOL_VERSION (0x%X)\n",
            msg,
            header->version,
            TIG_NET_PROTOCOL_VERSION);
        can_parse = false;
    }

    if (header->size > TIG_NET_BUF_SIZE) {
        tig_debug_printf("TCP-NET: Error: VALIDATION(%s): header->size (%d) > TIG_NET_BUF_SIZE (%d)\n",
            msg,
            header->size,
            TIG_NET_BUF_SIZE);
        can_parse = false;
    } else if (header->size == 0) {
        tig_debug_printf("TCP-NET: Error: VALIDATION(%s): header->size == %d\n",
            msg,
            0);
        can_parse = false;
    }

    return can_parse;
}

// 0x529A80
bool tig_net_parse_packet(int client_id)
{
    if (client_id == tig_net_local_client.client_id) {
        // TODO: Incomplete.
    } else if (tig_net_client_is_active(client_id)) {
        // TODO: Incomplete.
    } else {
        tig_debug_printf("TCP-NET: Error: Inactive slot passed to tig_net_parse_packet, %d\n", client_id);
    }
    return false;
}

// 0x529B80
bool sub_529B80(int client_id)
{
    if (client_id == tig_net_local_client.client_id) {
        // TODO: Incomplete.
    } else {
        // TODO: Incomplete.
    }
}

// 0x529C20
void sub_529C20(int a1, int client_id)
{
    // TODO: Incomplete.
}

// 0x52A460
bool sub_52A460(int flags, int client_id)
{
    tig_net_clients[client_id].flags = flags;
    return true;
}

// 0x52A490
void sub_52A490(int client_id, int a2)
{
    tig_timer_start(&(tig_net_clients[client_id].field_206C));
    tig_net_clients[client_id].field_2068 = a2;
}

// 0x52A4C0
void sub_52A4C0()
{
    tig_net_flags |= TIG_NET_FLAG_0x08;
}

// 0x52A4D0
void sub_52A4D0(int client_id)
{
    tig_net_clients[client_id].flags &= ~TIG_NET_CLIENT_CONNECTED;
}

// 0x52A500
bool sub_52A500(TigNetClient* a1, TigNetClient* a2)
{
    a1->client_id = a2->client_id;
    strncpy(a1->name, a2->name, TIG_NET_CLIENT_NAME_LENGTH - 1);
    a1->flags = a2->flags;

    return true;
}

// 0x52A530
int sub_52A530()
{
    return dword_5C064C;
}

// 0x52A540
void sub_52A540()
{
    // TODO: Incomplete.
}

// 0x52A630
void sub_52A630(int a1)
{
    // TODO: Incomplete.
}

// 0x52A6E0
bool sub_52A6E0()
{
    bool v1 = false;
    tig_list_enumerate(&tig_net_bookmarks, sub_52A750, &v1);

    // TODO: Incomplete.

    return v1;
}

// 0x52A750
bool sub_52A750(void* entry, int a2, void* context)
{
    // TODO: Incomplete.
    return true;
}

// 0x52A7A0
void sub_52A7A0(int a1)
{
    dword_5C0648 |= a1;
}

// 0x52A7C0
void sub_52A7C0(int a1)
{
    dword_5C0648 &= ~a1;
}

// 0x52A7F0
bool sub_52A7F0(void* entry, int index, void* context)
{
    // TODO: Incomplete.
}

// 0x52A890
bool sub_52A890(TigNetBookmark** a1, int* count)
{
    if (dword_62AC44 != NULL) {
        free(dword_62AC44);
    }

    dword_62AC44 = (TigNetBookmark*)malloc(sizeof(TigNetBookmark) * tig_list_get_count(&tig_net_bookmarks));
    dword_62AC40 = 0;
    tig_list_enumerate(&tig_net_bookmarks, sub_52A7F0, dword_62AC44);

    *a1 = dword_62AC44;
    *count = dword_62AC40;

    return true;
}

// 0x52A900
bool sub_52A900()
{
    // TODO: Incomplete.
}

// 0x52A940
void sub_52A940()
{
    dword_62AC88++;
}

// 0x52A950
void sub_52A950()
{
    dword_62AC88--;
}

// 0x52A960
int tig_net_num_active_clients()
{
    int count = 0;
    for (int index = 0; index < TIG_NET_MAX_PLAYERS; index++) {
        if (tig_net_client_is_active(index)) {
            count++;
        }
    }
    return count;
}

// 0x52A980
void sub_52A980(int client_id)
{
    tig_net_clients[client_id].flags |= TIG_NET_CLIENT_FLAG_0x02;
}

// 0x52A9B0
void sub_52A9B0(int client_id)
{
    tig_net_clients[client_id].flags &= ~TIG_NET_CLIENT_FLAG_0x02;
}

// 0x52A9E0
void sub_52A9E0(int client_id)
{
    // TODO: Incomplete.
}

// 0x52AAF0
unsigned int sub_52AAF0(int client_id)
{
    return tig_net_clients[client_id].flags;
}

// 0x52AB10
bool sub_52AB10(int client_id)
{
    // TODO: Incomplete.
}

// 0x52ABE0
void sub_52ABE0(int client_id)
{
    tig_net_clients[client_id].flags |= TIG_NET_CLIENT_FLAG_0x04;
}

// 0x52AC10
void sub_52AC10(int client_id)
{
    tig_net_clients[client_id].flags &= ~TIG_NET_CLIENT_FLAG_0x04;
}

// 0x52AC40
bool sub_52AC40(int client_id)
{
    return (tig_net_clients[client_id].flags & TIG_NET_CLIENT_FLAG_0x04) != 0;
}

// 0x52AC60
int sub_52AC60(int client_id)
{
    tig_net_clients[client_id].flags |= TIG_NET_CLIENT_FLAG_0x08;
    shutdown(tig_net_clients[client_id].socket, SD_SEND);
}

// 0x52ACA0
bool sub_52ACA0(int client_id)
{
    return (tig_net_clients[client_id].flags & TIG_NET_CLIENT_FLAG_0x08) != 0;
}

// 0x52AD40
bool sub_52AD40(int client_id)
{
    char buffer[16];
    tig_net_inet_string(tig_net_clients[client_id].field_28, buffer);
    sub_53AB90(NULL, buffer);
    return true;
}

// 0x52AD80
void tig_net_no_broadcast_enable()
{
    tig_net_flags |= TIG_NET_NO_BROADCAST;
}

// 0x52AD90
void tig_net_no_broadcast_disable()
{
    tig_net_flags &= ~TIG_NET_NO_BROADCAST;
}

// 0x52ADA0
void tig_net_bookmarks_load(const char* path)
{
    // TODO: Incomplete.
}

// 0x52AF20
void tig_net_bookmarks_save(const char* path)
{
    if (dword_62AC48) {
        TigFileStream* stream = tio_fopen(path, "wt");
        if (stream != NULL) {
            tig_list_enumerate(&tig_net_bookmarks, sub_52AF70, stream);
            tio_fclose(stream);
        } else {
            tig_debug_printf("TCP-NET: ERROR could not open bookmark file to save!\n");
        }
    }
}

// 0x52AF70
bool sub_52AF70(void* value, int index, void* context)
{
    TigNetBookmark* bookmark = (TigNetBookmark*)value;
    if ((bookmark->flags & TIG_NET_BOOKMARK_FLAG_0x10) != 0) {
        char buffer[128];
        tig_net_inet_string(bookmark->field_44.sin_addr.s_addr, buffer);
        tio_fputs(buffer, context);
        tio_fputs("\n", context);
    }
    return true;
}

// 0x52AFD0
void sub_52AFD0(unsigned long addr)
{
    TigNetBookmark bookmark;
    if (!tig_list_get_value(&tig_net_bookmarks, addr, &bookmark)) {
        tig_net_bookmark_init(&bookmark);
        bookmark.field_44.sin_addr.s_addr = addr;
        tig_net_inet_string(addr, bookmark.name);
    }

    bookmark.time = -1;
    bookmark.flags |= TIG_NET_BOOKMARK_FLAG_0x10;
    tig_list_set_value(&tig_net_bookmarks, addr, &bookmark);
}

// 0x52B050
void sub_52B050(unsigned long addr)
{
    TigNetBookmark bookmark;
    if (tig_list_get_value(&tig_net_bookmarks, addr, &bookmark)) {
        dword_62AC48 = true;

        bookmark.flags &= ~TIG_NET_BOOKMARK_FLAG_0x10;
        tig_timer_start(&(bookmark.time));
        bookmark.time -= 10000;
        tig_list_set_value(&tig_net_bookmarks, addr, &bookmark);
    }
}

// 0x52B0C0
void sub_52B0C0(unsigned long* addr)
{
    *addr = tig_net_local_bookmark.field_44.sin_addr.s_addr;
}

// 0x52B0F0
int sub_52B0F0(int a1)
{
    // TODO: Incomplete.
}

// 0x52B1E0
void sub_52B1E0(int a1)
{
    dword_62AC68 = a1;
}

// 0x52B1F0
u_long sub_52B1F0(u_long a1)
{
    return ntohl(a1);
}

// 0x52B210
void sub_52B210()
{
    tig_timer_start(&(tig_net_local_bookmark.time));
}

// 0x52B220
int sub_52B220(int client_id)
{
    TigNetMsg_25_2 packet;
    packet.header.version = TIG_NET_PROTOCOL_VERSION;
    packet.header.type = 2;
    packet.header.size = sizeof(packet);
    packet.field_4 = 7;
    return tig_net_ws_impl.send(tig_net_clients[client_id].socket, (char*)&packet, sizeof(packet), 0);
}

// 0x52B270
int sub_52B270(int client_id)
{
    TigNetMsg_25_2 packet;
    packet.header.version = TIG_NET_PROTOCOL_VERSION;
    packet.header.type = 2;
    packet.header.size = sizeof(packet);
    packet.field_4 = 8;
    return tig_net_ws_impl.send(tig_net_clients[client_id].socket, (char*)&packet, sizeof(packet), 0);
}

// 0x52B2C0
void sub_52B2C0()
{
    dword_62AC50++;
}

// 0x52B2D0
void sub_52B2D0()
{
    dword_62AC50--;
}
