#include "game/broadcast.h"

#include "game/mes.h"
#include "game/player.h"
#include "game/text_filter.h"

typedef struct S5FF404 {
    char* field_0;
    int field_4;
} S5FF404;

static int broadcast_match_str_to_base_type(const char* str);
static int sub_4C3B40(const char* str);
static void sub_4C3B90();

// 0x5FDC4C
static const char* dword_5FDC4C[14];

// 0x5FDC84
static mes_file_handle_t dword_5FDC84;

// 0x5FDC88
static Func5FDC88* dword_5FDC88;

// 0x5FDC8C
static mes_file_handle_t dword_5FDC8C;

// 0x5FDC90
static char byte_5FDC90[6][1000];

// 0x5FF400
static bool broadcast_initialized;

// 0x5FF404
static S5FF404* dword_5FF404;

// 0x5FF408
static int dword_5FF408;

// 0x4C2C10
bool broadcast_init(GameInitInfo* init_info)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    int index;

    (void)init_info;

    dword_5FDC88 = NULL;

    if (!mes_load("mes\\broadcast.mes", &dword_5FDC84)) {
        tig_debug_printf("broadcast_init: ERROR: couldn't load message file: mes\\broadcast.mes!\n");
        exit(EXIT_FAILURE);
    }

    if (!mes_load("mes\\MultiPlayer.mes", &dword_5FDC8C)) {
        tig_debug_printf("broadcast_init: ERROR: couldn't load message file: %s!\n", "mes\\MultiPlayer.mes");
        exit(EXIT_FAILURE);
    }

    mes_file_entry.num = 100;
    while (mes_search(dword_5FDC84, &mes_file_entry)) {
        mes_get_msg(dword_5FDC84, &mes_file_entry);
        dword_5FF404 = (S5FF404*)REALLOC(dword_5FF404, sizeof(*dword_5FF404) * (dword_5FF408 + 1));
        if (dword_5FF404 == NULL) {
            tig_debug_printf("broadcast_init: ERROR: realloc failed!\n");
            exit(EXIT_FAILURE);
        }
        dword_5FF404[dword_5FF408++].field_0 = mes_file_entry.str;
        mes_file_entry.num++;
    }

    if (!mes_load("Rules\\BroadCastRules.mes", &mes_file)) {
        tig_debug_printf("broadcast_init: ERROR: couldn't load message file: Rules\\BroadCastRules.mes!\n");
        exit(EXIT_FAILURE);
    }

    tig_str_parse_set_separator(',');

    mes_file_entry.num = 0;
    for (index = 0; index < 14; index++) {
        mes_get_msg(mes_file, &mes_file_entry);
        dword_5FDC4C[index] = mes_file_entry.str;
        mes_file_entry.num++;
    }

    mes_file_entry.num = 100;
    for (index = 0; index < dword_5FF408; index++) {
        mes_get_msg(mes_file, &mes_file_entry);
        dword_5FF404[index].field_4 = broadcast_match_str_to_base_type(mes_file_entry.str);
        mes_file_entry.num++;
    }

    if (!mes_unload(mes_file)) {
        tig_debug_printf("broadcast_init: ERROR: couldn't unload message file: bcRuleMessID!\n");
    }

    sub_4C3B90();

    if (!text_filter_init()) {
        return false;
    }

    broadcast_initialized = true;

    return true;
}

// 0x4C2E20
void broadcast_exit()
{
    if (dword_5FF404 != NULL) {
        FREE(dword_5FF404);
        dword_5FF404 = NULL;
    }

    dword_5FF408 = 0;

    if (!mes_unload(dword_5FDC84)) {
        tig_debug_printf("broadcast_exit: ERROR: couldn't unload message file: bcMessID!\n");
    }

    if (!mes_unload(dword_5FDC8C)) {
        tig_debug_printf("broadcast_exit: ERROR: couldn't unload message file: mp_mes_id!\n");
    }

    text_filter_exit();

    broadcast_initialized = false;
}

// 0x4C2EA0
void sub_4C2EA0(Func5FDC88* func)
{
    dword_5FDC88 = func;
}

// 0x4C2EB0
int broadcast_match_str_to_base_type(const char* str)
{
    int index;

    for (index = 0; index < 14; index++) {
        if (strcmpi(str, dword_5FDC4C[index]) == 0) {
            return index;
        }
    }

    tig_debug_printf("broadcast_match_str_to_base_type: ERROR: couldn't map str to type!\n");
    return 0;
}

// 0x4C2F00
void sub_4C2F00(int64_t obj, Broadcast* bcast)
{
    struct {
        int type;
        ObjectID oid;
        Broadcast bcast;
    } packet;

    static_assert(sizeof(packet) == 0xA8, "wrong size");

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0 || (tig_net_flags & TIG_NET_HOST) != 0) {
        broadcast_msg_client(obj, bcast);
    } else {
        packet.type = 18;
        packet.oid = sub_407EF0(obj);
        packet.bcast = *bcast;
        tig_net_send_app_all(&packet, sizeof(packet));
    }
}

// 0x4C2FA0
void sub_4C2FA0()
{
    // TODO: Incomplete.
}

// 0x4C3040
void sub_4C3040()
{
    // TODO: Incomplete.
}

// 0x4C31A0
void broadcast_msg_client(int64_t obj, Broadcast* bcast)
{
    // TODO: Incomplete.
}

// 0x4C3B40
int sub_4C3B40(const char* str)
{
    int index;

    while (*str == ' ') {
        str++;
    }

    for (index = 0; index < dword_5FF408; index++) {
        if (strcmpi(str, dword_5FF404[index].field_0) == 0) {
            return dword_5FF404[index].field_4;
        }
    }

    return 0;
}

// 0x4C3B90
void sub_4C3B90()
{
    int index;
    MesFileEntry mes_file_entry;

    for (index = 0; index < 6; index++) {
        mes_file_entry.num = 500 + index;
        mes_get_msg(dword_5FDC84, &mes_file_entry);
        strncpy(byte_5FDC90[index], mes_file_entry.str, 1000);
    }
}

// 0x4C3BE0
void sub_4C3BE0(unsigned int a1, int64_t loc)
{
    Broadcast bcast;

    if (a1 < 6) {
        bcast.loc = loc;
        strcpy(bcast.field_8, byte_5FDC90[a1]);
        sub_4C2F00(player_get_pc_obj(), &bcast);
    }
}
