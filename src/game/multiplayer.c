#include "game/multiplayer.h"

#include "game/combat.h"
#include "game/gamelib.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/matchmaker.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/obj_private.h"
#include "game/object.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/tech.h"
#include "game/timeevent.h"
#include "game/ui.h"

#define NUM_PLAYERS 8

#define BEGIN_SENTINEL 0x4ACEBABE
#define END_SENTINEL 0xBABE01CE

typedef struct S5F0DEC {
    /* 0000 */ ObjectID oid;
    /* 0018 */ struct S5F0DEC* next;
} S5F0DEC;

static_assert(sizeof(S5F0DEC) == 0x20, "wrong size");

typedef struct S5E8AD0 {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int field_4;
    /* 0008 */ ObjectID field_8;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ ObjectID field_38;
} S5E8AD0;

static_assert(sizeof(S5E8AD0) == 0x50, "wrong size");

static void sub_49CB80(S5E8AD0* a1);
static bool sub_49D570(TimeEvent* timeevent);
static void sub_4A1F30(int64_t obj, int64_t location, int dx, int dy);
static bool sub_4A1F60(int player, int64_t* obj_ptr);
static void sub_4A1FC0();
static void sub_4A2020(void* msg);
static void sub_4A2040(int a1);
static void sub_4A2A30();
static void sub_4A2A40(int64_t obj);
static void sub_4A2AE0(int player);
static void sub_4A3660(int player);
static void sub_4A3780();
static bool sub_4A40D0(int player);
static void sub_4A54E0();
static void sub_4A5670(int64_t obj);

// 0x5B3FD8
static int dword_5B3FD8 = 10;

// 0x5B3FEC
static int dword_5B3FEC = -1;

// 0x5B4070
static int dword_5B4070 = -1;

// 0x5B40D8
static int dword_5B40D8 = -1;

// 0x5E8940
static TigIdxTable stru_5E8940;

// 0x5E8AD0
static S5E8AD0 stru_5E8AD0[NUM_PLAYERS];

// 0x5F0BE8
static mes_file_handle_t multiplayer_mes_file;

// 0x5F0BC8
static void* off_5F0BC8[NUM_PLAYERS];

// 0x5F0DE0
static int dword_5F0DE0;

// 0x5F0DE4
static void* dword_5F0DE4;

// 0x5F0DE8
static int dword_5F0DE8;

// 0x5F0DEC
static S5F0DEC* dword_5F0DEC;

// 0x5F0DF8
static Func5F0DF8* dword_5F0DF8;

// 0x5F0E00
static bool dword_5F0E00;

// 0x5F0E04
static void(*off_5F0E04)();

// 0x5F0E08
static Func5F0E08* dword_5F0E08;

// 0x5F0E0C
static int dword_5F0E0C;

// 0x5F0E10
static int dword_5F0E10;

// 0x5F0E14
static bool dword_5F0E14;

// 0x49C670
bool multiplayer_init(GameInitInfo* init_info)
{
    TigFileInfo file_info;
    int index;

    (void)init_info;

    if (tig_file_exists("Players", &file_info)) {
        if ((file_info.attributes & TIG_FILE_ATTRIBUTE_SUBDIR) == 0) {
            tig_debug_printf("MP: init: ERROR: players folder (%s) could not be made (already a file).\n", "Players");
            return false;
        }
    } else {
        tig_file_mkdir_ex("Players");
    }

    dword_5F0E14 = false;

    if (!tig_net_local_client_set_name("Player")) {
        return false;
    }

    if (!tig_net_local_server_set_max_players(NUM_PLAYERS)) {
        return false;
    }

    if (!tig_net_local_server_set_description("Description")) {
        return false;
    }

    for (index = 0; index < NUM_PLAYERS; index++) {
        sub_49CB80(&(stru_5E8AD0[index]));
    }

    mes_load("mes\\MultiPlayer.mes", &multiplayer_mes_file);
    tig_idxtable_init(&stru_5E8940, 16);
    dword_5F0DEC = NULL;
    memset(off_5F0BC8, 0, sizeof(off_5F0BC8));
    sub_4A5290();
    sub_4A5380();
    sub_4A2E90();

    return true;
}

// 0x49C780
void multiplayer_exit()
{
    S5F0DEC* node;
    int index;

    if (dword_5F0E00) {
        sub_49CC20();
    }

    while (dword_5F0DEC != NULL) {
        node = dword_5F0DEC;
        dword_5F0DEC = dword_5F0DEC->next;
        FREE(node);
    }

    for (index = 0; index < NUM_PLAYERS; index++) {
        if (off_5F0BC8[index] != NULL) {
            FREE(off_5F0BC8[index]);
            off_5F0BC8[index] = NULL;
        }
    }

    tig_idxtable_exit(&stru_5E8940);
    mes_unload(multiplayer_mes_file);

    if (dword_5F0DE4) {
        FREE(dword_5F0DE4);
    }

    if (matchmaker_is_active()) {
        matchmaker_exit();
    }
}

// 0x49C820
void multiplayer_reset()
{
    S5F0DEC* node;
    int index;

    if (!dword_5F0E10) {
        if (dword_5F0E00) {
            sub_49CC20();
        }
        dword_5F0E00 = false;

        tig_idxtable_exit(&stru_5E8940);
        tig_idxtable_init(&stru_5E8940, 16);

        while (dword_5F0DEC != NULL) {
            node = dword_5F0DEC;
            dword_5F0DEC = dword_5F0DEC->next;
            FREE(node);
        }

        tig_net_local_server_set_name("Arcanum");
        tig_net_local_client_set_name("Player");
        tig_net_local_server_set_max_players(NUM_PLAYERS);
        tig_net_local_server_set_description("Description");

        for (index = 0; index < NUM_PLAYERS; index++) {
            sub_49CB80(&(stru_5E8AD0[index]));
        }

        for (index = 0; index < NUM_PLAYERS; index++) {
            if (off_5F0BC8[index] != NULL) {
                FREE(off_5F0BC8[index]);
                off_5F0BC8[index] = NULL;
            }
        }

        if (dword_5F0DE4 != NULL) {
            sub_4A3D00(0);
            dword_5F0DE4 = NULL;
            dword_5F0DE8 = 0;
        }

        sub_4A5290();
        sub_4A5380();
        timeevent_clear_all_typed(TIMEEVENT_TYPE_MULTIPLAYER);
    }
}

// 0x49C930
bool multiplayer_save(TigFile* stream)
{
    unsigned int sentinel;
    S5F0DEC* node;
    int cnt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        return true;
    }

    sentinel = BEGIN_SENTINEL;
    if (tig_file_fwrite(&sentinel, sizeof(sentinel), 1, stream) != 1) {
        return false;
    }

    node = dword_5F0DEC;
    while (node != NULL) {
        cnt++;
        node = node->next;
    }

    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    node = dword_5F0DEC;
    while (node != NULL) {
        if (tig_file_fwrite(&(node->oid), sizeof(node->oid), 1, stream) != 1) {
            return false;
        }
        node = node->next;
    }

    if (tig_file_fwrite(stru_5E8AD0, sizeof(*stru_5E8AD0), NUM_PLAYERS, stream) != NUM_PLAYERS) {
        return false;
    }

    sentinel = END_SENTINEL;
    if (tig_file_fwrite(&sentinel, sizeof(sentinel), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x49CA20
bool mutliplayer_load(GameLoadInfo* load_info)
{
    unsigned int sentinel;
    S5F0DEC* node;
    int cnt;
    ObjectID oid;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        return true;
    }

    if (tig_file_fread(&sentinel, sizeof(sentinel), 1, load_info->stream) != 1) {
        return false;
    }

    if (sentinel != BEGIN_SENTINEL) {
        return false;
    }

    if (tig_file_fread(&cnt, sizeof(cnt), 1, load_info->stream) != 1) {
        return false;
    }

    while (cnt != 0) {
        if (tig_file_fread(&oid, sizeof(oid), 1, load_info->stream) != 1) {
            return false;
        }

        node = (S5F0DEC*)MALLOC(sizeof(*node));
        node->oid = oid;
        node->next = dword_5F0DEC;
        dword_5F0DEC = node;

        cnt--;
    }

    if (tig_file_fread(stru_5E8AD0, sizeof(*stru_5E8AD0), NUM_PLAYERS, load_info->stream) != NUM_PLAYERS) {
        return false;
    }

    if (tig_file_fread(&sentinel, sizeof(sentinel), 1, load_info->stream) != 1) {
        return false;
    }

    if (sentinel != END_SENTINEL) {
        return false;
    }

    return true;
}

// 0x49CB50
bool multiplayer_mod_load()
{
    return true;
}

// 0x49CB60
void multiplayer_mod_unload()
{
    sub_4A2BC0();
    sub_4A3D00(1);
    sub_4A2BD0();
}

// 0x49CB80
void sub_49CB80(S5E8AD0* a1)
{
    memset(a1, 0, sizeof(*a1));
    a1->flags = 0;
    a1->field_20 = -1;
    a1->field_24 = 1;
    a1->field_28 = 0;
    a1->field_2C = 0;
    a1->field_30 = 0;
    a1->field_34 = 0;
    memset(&(a1->field_8), 0, sizeof(a1->field_8));
    memset(&(a1->field_38), 0, sizeof(a1->field_38));
    a1->field_38.type = 0;
}

// 0x49CBD0
bool sub_49CBD0()
{
    dword_5F0E0C = 0;
    if (tig_net_start_client() != TIG_OK) {
        return FALSE;
    }

    sub_4A2AE0(0);
    tig_net_on_message(sub_49D690);
    tig_net_on_message_validation(sub_4A2020);
    tig_net_on_network_event(sub_4A2070);
    dword_5F0E00 = false;

    return true;
}

// 0x49CC20
bool sub_49CC20()
{
    bool v1;

    sub_4A2AE0(0);
    tig_net_on_message(NULL);
    v1 = sub_5280F0();
    dword_5F0E00 = false;
    dword_5F0E0C = 0;
    return !v1;
}

// 0x49CC50
void sub_49CC50()
{
    tig_net_start_server();
}

// 0x49CC70
void sub_49CC70()
{
    // TODO: Incomplete.
}

// 0x49D100
void multiplayer_start_play()
{
    // TODO: Incomplete.
}

// 0x49D320
void multiplayer_timeevent_process()
{
    // TODO: Incomplete.
}

// 0x49D570
bool sub_49D570(TimeEvent* timeevent)
{
    return timeevent->params[0].integer_value == 2
        && timeevent->params[1].integer_value == dword_5B4070;
}

// 0x49D590
bool multiplayer_map_open_by_name(const char* name)
{
    char path[TIG_MAX_PATH];
    char save_path[TIG_MAX_PATH];

    sprintf(path, ".\\%s.dat", name);
    if (tig_file_exists(path, NULL)) {
        tig_file_repository_add(path);
    }

    sub_52A940();
    map_flush(0);

    if (!obj_validate_system(1)) {
        tig_debug_println("Object system validate failed pre-load in multiplayer_map_open_by_name.");
        tig_message_post_quit(0);
    }

    sprintf(path, "maps\\%s", name);
    sprintf(save_path, "%s\\maps\\%s", "Save\\Current", name);
    tig_debug_printf("MP: Loading Map: %s\n", path);
    if (!map_open(path, save_path, 1)) {
        return false;
    }

    sub_52A950();
    sub_4605C0();

    if (!obj_validate_system(1)) {
        tig_debug_prinln("Object system validate failed post-load in multiplayer_map_open_by_name.");
        tig_message_post_quit(0);
    }

    return true;
}

// 0x49D690
void sub_49D690()
{
    // TODO: Incomplete.
}

// 0x4A1F30
void sub_4A1F30(int64_t obj, int64_t location, int dx, int dy)
{
    if (location != 0) {
        sub_43E770(obj, location, dx, dy);
    }
}

// 0x4A1F60
bool sub_4A1F60(int player, int64_t* obj_ptr)
{
    if (player >= 0 && player < NUM_PLAYERS) {
        *obj_ptr = objp_perm_lookup(stru_5E8AD0[player].field_8);
        return *obj_ptr != OBJ_HANDLE_NULL;
    }

    *obj_ptr = OBJ_HANDLE_NULL;
    return false;
}

// 0x4A1FC0
void sub_4A1FC0()
{
    Packet2 pkt;
    int cnt;
    int index;

    if ((tig_net_flags & TIG_NET_HOST) != 0) {
        pkt.type = 2;

        cnt = tig_net_local_server_get_max_players();
        for (index = 0; index < cnt; index++) {
            pkt.oids[index] = stru_5E8AD0[index].field_8;
        }

        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
}

// 0x4A2020
void sub_4A2020(void* msg)
{
    int type;

    type = *(int*)msg;
    return type > 0 && type < 131;
}

// 0x4A2040
void sub_4A2040(int a1)
{
    Packet67 pkt;

    pkt.type = 67;
    pkt.field_4 = a1;
    tig_net_send_app_all(&pkt, sizeof(pkt));
}

// 0x4A2070
void sub_4A2070()
{
    // TODO: Incomplete.
}

// 0x4A2A30
void sub_4A2A30()
{
    if (off_5F0E04 != NULL) {
        off_5F0E04();
    } else {
        sub_4606E0();
    }
}

// 0x4A2A40
void sub_4A2A40(int64_t obj)
{
    Packet68 pkt;

    if (obj != OBJ_HANDLE_NULL) {
        sub_433170(obj);

        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            pkt.type = 68;
            sub_4440E0(obj, &(pkt.field_8));
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
    }
}

// 0x4A2A90
void sub_4A2A90()
{
    // TODO: Incomplete.
}

// 0x4A2AE0
void sub_4A2AE0(int player)
{
    sub_49CB80(&stru_5E8AD0[player]);
}

// 0x4A2B00
void sub_4A2B00(Func5F0E08* func)
{
    dword_5F0E08 = func;
}

// 0x4A2B10
int sub_4A2B10(int64_t obj)
{
    int player;
    int64_t player_obj;

    for (player = 0; player < NUM_PLAYERS; player++) {
        if (tig_net_client_is_active(player)
            && sub_4A1F60(player, player_obj)
            && player_obj == obj) {
            return player;
        }
    }

    return -1;
}

// 0x4A2B60
int64_t sub_4A2B60(int player)
{
    int64_t obj;

    if (tig_net_client_is_active(player)
        && sub_4A1F60(player, &obj)) {
        return obj;
    }

    return OBJ_HANDLE_NULL;
}

// 0x4A2BA0
bool sub_4A2BA0()
{
    return (tig_net_flags & TIG_NET_CONNECTED) != 0 ? dword_5F0E0C > 0 : true;
}

// 0x4A2BC0
void sub_4A2BC0()
{
    dword_5F0E0C++;
}

// 0x4A2BD0
void sub_4A2BD0()
{
    if (dword_5F0E0C > 0) {
        dword_5F0E0C--;
    }
}

// 0x4A2BE0
int64_t multiplayer_find_first_player_obj()
{
    int64_t obj;

    dword_5B40D8 = -1;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        return player_get_pc_obj();
    }

    for (dword_5B40D8 = 0; dword_5B40D8 < NUM_PLAYERS; dword_5B40D8++) {
        if (tig_net_client_is_active(dword_5B40D8)
            && sub_4A1F60(dword_5B40D8, &obj)) {
            dword_5B40D8++;
            return obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x4A2C60
int64_t multiplayer_find_next_player_obj()
{
    int64_t obj;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        return OBJ_HANDLE_NULL;
    }

    for (; dword_5B40D8 < NUM_PLAYERS; dword_5B40D8++) {
        if (tig_net_client_is_active(dword_5B40D8)
            && sub_4A1F60(dword_5B40D8, &obj)) {
            dword_5B40D8++;
            return obj;
        }
    }

    return OBJ_HANDLE_NULL;
}

// 0x4A2CD0
void sub_4A2CD0()
{
    // TODO: Incomplete.
}

// 0x4A2D00
void sub_4A2D00()
{
    // TODO: Incomplete.
}

// 0x4A2E90
void sub_4A2E90()
{
    // TODO: Incomplete.
}

// 0x4A2EC0
void sub_4A2EC0()
{
    // TODO: Incomplete.
}

// 0x4A3030
void sub_4A3030()
{
    // TODO: Incomplete.
}

// 0x4A3080
void sub_4A3080()
{
    // TODO: Incomplete.
}

// 0x4A30D0
void sub_4A30D0()
{
    // TODO: Incomplete.
}

// 0x4A3170
void sub_4A3170()
{
    // TODO: Incomplete.
}

// 0x4A3230
void sub_4A3230()
{
    // TODO: Incomplete.
}

// 0x4A33F0
void sub_4A33F0()
{
    // TODO: Incomplete.
}

// 0x4A3660
void sub_4A3660(int player)
{
    char pattern[TIG_MAX_PATH];
    char path[TIG_MAX_PATH];
    TigFileList file_list;
    unsigned int index;

    sprintf(pattern, "%s\\Players\\*.*", ".\\data\\temp");
    tig_file_list_create(&file_list, pattern);

    for (index = 0; index < file_list.count; index++) {
        if (strcmp(file_list.entries[index].path, ".") != 0
            && strcmp(file_list.entries[index].path, "..") != 0) {
            sprintf(path, "%s\\Players\\%s", ".\\data\\temp", file_list.entries[index].path);
            tig_net_xfer_send(path, player, NULL);
        }
    }

    tig_file_list_destroy(&file_list);
}

// 0x4A3780
void sub_4A3780()
{
    char prefix[40];
    char name[TIG_MAX_PATH];
    char src[TIG_MAX_PATH];
    char dst[TIG_MAX_PATH];
    int player;
    const char* exts[] = {
        ".mpc",
        ".bmp",
        "_b.bmp",
    };
    int index;

    player = sub_529520();
    objid_id_to_str(prefix, stru_5E8AD0[player].field_8);

    for (index = 0; index < sizeof(exts) / sizeof(exts[0]); index++) {
        sprintf(name, "%s%s", prefix, exts[index]);
        sprintf(src, "Players\\%s", name);
        sprintf(dst, "%s\\Players", ".\\data\\temp");
        if (!tig_file_is_directory(dst)) {
            tig_file_mkdir(dst);
        }
        sprintf(dst, "%s\\Players\\%s", ".\\data\\temp", name);
        tig_file_copy(src, dst);
        tig_net_xfer_send_as(dst, dst, 0, NULL);
    }
}

// 0x4A3890
void sub_4A3890()
{
}

// 0x4A38A0
int sub_4A38A0()
{
    return dword_5F0DE0;
}

// 0x4A38B0
void sub_4A38B0()
{
    // TODO: Incomplete.
}

// 0x4A39D0
void sub_4A39D0(Func5F0DF8* func, int ctx)
{
    dword_5F0DF8 = func;
    dword_5B3FEC = ctx;
}

// 0x4A39F0
void sub_4A39F0()
{
    // TODO: Incomplete.
}

// 0x4A3D00
void sub_4A3D00()
{
    // TODO: Incomplete.
}

// 0x4A3D70
void sub_4A3D70()
{
    // TODO: Incomplete.
}

// 0x4A3F40
void sub_4A3F40()
{
    // TODO: Incomplete.
}

// 0x4A40D0
bool sub_4A40D0(int player)
{
    return off_5F0BC8[player] != NULL;
}

// 0x4A40F0
void sub_4A40F0()
{
    // TODO: Incomplete.
}

// 0x4A4180
void sub_4A4180()
{
    // TODO: Incomplete.
}

// 0x4A41B0
void sub_4A41B0()
{
    // TODO: Incomplete.
}

// 0x4A41E0
void sub_4A41E0()
{
    // TODO: Incomplete.
}

// 0x4A4230
void* sub_4A4230(int player)
{
    return off_5F0BC8[player];
}

// 0x4A4240
void sub_4A4240()
{
    // TODO: Incomplete.
}

// 0x4A4270
void sub_4A4270()
{
    dword_5F0E10++;
}

// 0x4A4280
void sub_4A4280()
{
    dword_5F0E10--;
}

// 0x4A4320
void sub_4A4320()
{
    // TODO: Incomplete.
}

// 0x4A43B0
void sub_4A43B0()
{
    // TODO: Incomplete.
}

// 0x4A44C0
void sub_4A44C0()
{
    // TODO: Incomplete.
}

// 0x4A45B0
void sub_4A45B0()
{
    // TODO: Incomplete.
}

// 0x4A47D0
bool sub_4A47D0(int64_t obj, char* str)
{
    // TODO: Incomplete.
}

// 0x4A49E0
void sub_4A49E0()
{
    // TODO: Incomplete.
}

// 0x4A4C40
bool sub_4A4C40(int64_t obj, int size, char* path)
{
    // TODO: Incomplete.
}

// 0x4A4D60
void sub_4A4D60()
{
    // TODO: Incomplete.
}

// 0x4A4EB0
int multiplayer_mm_init(int a1)
{
    return matchmaker_init(a1);
}

// 0x4A4EC0
void multiplayer_mm_exit()
{
    matchmaker_exit();
}

// 0x4A4ED0
int multiplayer_mm_is_active()
{
    return matchmaker_is_active();
}

// 0x4A4EE0
int multiplayer_mm_ping()
{
    return matchmaker_ping();
}

// 0x4A4EF0
int multiplayer_mm_motd_get(int a1, int a2, int a3, int a4)
{
    return matchmaker_motd_get(a1, a2, a3, a4);;
}

// 0x4A4F10
int multiplayer_mm_login(int a1, int a2)
{
    return matchmaker_login(a1, a2);
}

// 0x4A4F30
int multiplayer_mm_create_account(int a1, int a2, int a3)
{
    return matchmaker_create_account(a1, a2, a3);
}

// 0x4A4F50
int multiplayer_mm_version_needs_upgrade(int a1)
{
    return matchmaker_version_needs_upgrade(a1);
}

// 0x4A4F60
int multiplayer_mm_ad_rgb_get(int a1, int a2, int a3)
{
    return matchmaker_ad_rgb_get(a1, a2, a3);
}

// 0x4A4F80
int multiplayer_mm_ad_release()
{
    return matchmaker_ad_release();
}

// 0x4A4F90
int multiplayer_mm_ad_clicked()
{
    return matchmaker_ad_clicked();
}

// 0x4A4FA0
int multiplayer_mm_register(int a1)
{
    return matchmaker_register(a1);
}

// 0x4A4FB0
int multiplayer_mm_gamelist_get(void** games, int* count)
{
    return matchmaker_gamelist_get(games, count);
}

// 0x4A4FD0
int multiplayer_mm_gamelist_free(void* games)
{
    return matchmaker_gamelist_free(games);
}

// 0x4A4FE0
int multiplayer_mm_chatserver_list_get(void** chatservers, int* count)
{
    return matchmaker_chatserver_list_get(chatservers, count);
}

// 0x4A5000
int multiplayer_mm_chatserver_list_free(void* chatservers)
{
    return matchmaker_chatserver_list_free(chatservers);
}

// 0x4A5010
int multiplayer_mm_chatserver_join(int a1)
{
    return matchmaker_chatserver_join(a1);
}

// 0x4A5020
int multiplayer_mm_chatroom_list_get(void** chatrooms, int* count)
{
    return matchmaker_chatroom_list_get(chatrooms, count);
}

// 0x4A5040
int multiplayer_mm_chatroom_list_free(void* chatrooms)
{
    return matchmaker_chatroom_list_free(chatrooms);
}

// 0x4A5050
int multiplayer_mm_chatroom_join(int a1, int a2)
{
    return matchmaker_chatroom_join(a1, a2);
}

// 0x4A5070
int multiplayer_mm_chatroom_members_get(void** members, int* count)
{
    return matchmaker_chatroom_members_get(members, count);
}

// 0x4A5090
int multiplayer_mm_chatroom_members_free(void* members)
{
    return matchmaker_chatroom_members_free(members);
}

// 0x4A50A0
int multiplayer_mm_chatroom_create(const char* a1, const char* a2)
{
    return matchmaker_chatroom_create(a1, a2);
}

// 0x4A50C0
int multiplayer_mm_chatroom_mesg(const char* msg)
{
    return matchmaker_chatroom_mesg(msg);
}

// 0x4A50D0
void sub_4A50D0()
{
    // TODO: Incomplete.
}

// 0x4A51C0
void sub_4A51C0()
{
    // TODO: Incomplete.
}

// 0x4A5290
void sub_4A5290()
{
    // TODO: Incomplete.
}

// 0x4A52C0
void sub_4A52C0()
{
    // TODO: Incomplete.
}

// 0x4A5320
void sub_4A5320()
{
    // TODO: Incomplete.
}

// 0x4A5380
void sub_4A5380()
{
    // TODO: Incomplete.
}

// 0x4A53B0
void sub_4A53B0()
{
    // TODO: Incomplete.
}

// 0x4A5460
void sub_4A5460()
{
    // TODO: Incomplete.
}

// 0x4A5490
void multiplayer_ping()
{
    if (multiplayer_mm_is_active()) {
        multiplayer_mm_ping();
    }
}

// 0x4A54A0
void sub_4A54A0()
{
    // TODO: Incomplete.
}

// 0x4A54E0
void sub_4A54E0()
{
    int index;

    for (index = 0; index < 512; index++) {
        if ((magictech_locks[index].field_13C & 0x1) != 0) {
            sub_459500(index);
        }
    }
}

// 0x4A5510
void sub_4A5510(int64_t obj, int value)
{
    // TODO: Incomplete.
}

// 0x4A5570
void sub_4A5570(int64_t obj, int value)
{
    // TODO: Incomplete.
}

// 0x4A55D0
int sub_4A55D0(int player)
{
    return stru_5E8AD0[player].flags & 0xFF00;
}

// 0x4A55F0
int sub_4A55F0()
{
    return dword_5B3FD8;
}

// 0x4A5600
void sub_4A5600(int a1)
{
    dword_5B3FD8 = a1;
}

// 0x4A5610
void sub_4A5610()
{
    set_always_run(settings_get_value(&settings, "always run"));
    combat_auto_attack_set(settings_get_value(&settings, "auto attack"));
    combat_auto_switch_weapons_set(settings_get_value(&settings, "auto switch"));
    set_follower_skills(settings_get_value(&settings, "follower skills"));
}

// 0x4A5670
void sub_4A5670(int64_t obj)
{
    mes_file_handle_t mes_file;

    if (mes_load("Rules\\AutoEquip.mes", &mes_file)) {
        if ((tig_net_local_server_get_options() & TIG_NET_SERVER_AUTO_EQUIP) == 0) {
            tig_str_parse_set_separator(' ');
            sub_4A5710(obj, mes_file);
            sub_4A57F0(obj);
            sub_4A5EE0(obj);
            sub_4A5CA0(obj, mes_file);
            sub_4A59F0(obj, mes_file);
            sub_4A5840(obj, mes_file);
            sub_4654F0(obj, OBJ_HANDLE_NULL);
        }
    }
}

// 0x4A5710
int sub_4A5710(int64_t obj, mes_file_handle_t mes_file)
{
    int64_t location;
    int level;
    MesFileEntry mes_file_entry;
    int money_amt;
    int64_t money_obj;

    location = obj_field_int64_get(obj, OBJ_F_LOCATION);
    level = stat_level(obj, STAT_LEVEL);

    mes_file_entry.num = (level + 9) / 10;
    if (!mes_search(mes_file, &mes_file_entry)) {
        return 0;
    }

    mes_get_msg(mes_file, &mes_file_entry);
    money_amt = atoi(mes_file_entry.str);
    money_amt = background_adjust_money(money_amt, background_obj_get_background(obj));
    if (money_amt > 0) {
        sub_4ED8B0(9056, location, &money_obj);
        sub_4EFDD0(money_obj, OBJ_F_GOLD_QUANTITY, money_amt);
        sub_4617F0(money_obj, obj);
    }
    return money_amt;
}

// 0x4A57F0
int sub_4A57F0(int64_t obj)
{
    char buffer[2000];

    background_get_items(buffer, sizeof(buffer), background_obj_get_background(obj));

    return sub_4A5E10(obj, buffer);
}

// 0x4A5840
int sub_4A5840(int64_t obj, mes_file_handle_t mes_file)
{
    int v1 = 0;
    int level;
    int value;

    level = stat_level(obj, STAT_LEVEL);

    value = tech_skill_level(obj, TECH_SKILL_PICK_LOCKS);
    if (value > 0) {
        v1 += sub_4A5920(obj, mes_file, (value + level + 29) / 30 + 1100);
    }

    value = basic_skill_level(obj, BASIC_SKILL_HEAL);
    if (value > 0) {
        v1 += sub_4A5920(obj, mes_file, (value + level + 29) / 30 + 1200);
    }

    value = basic_skill_level(obj, BASIC_SKILL_BACKSTAB);
    if (value > 0) {
        v1 += sub_4A5920(obj, mes_file, (value + level + 29) / 30 + 1300);
    }

    return v1;
}

// 0x4A5920
void sub_4A5920()
{
    // TODO: Incomplete.
}

// 0x4A59F0
void sub_4A59F0(int64_t obj, mes_file_handle_t mes_file)
{
    int v1 = 0;
    int level;
    int value;
    MesFileEntry mes_file_entry;

    level = stat_level(obj, STAT_LEVEL);

    // Melee Weapons
    value = basic_skill_level(obj, BASIC_SKILL_MELEE);
    if (value > 0) {
        mes_file_entry.num = 600;
        if (mes_search(mes_file, &mes_file_entry)) {
            mes_get_msg(mes_file, &mes_file_entry);
            v1 += sub_4A5E10(obj, mes_file_entry.str);
        }
        v1 += sub_4A5920(obj, mes_file, (value + level + 9) / 10 + 600);
    }

    // Bows
    value = basic_skill_level(obj, BASIC_SKILL_BOW);
    if (value > 0) {
        mes_file_entry.num = 700;
        if (mes_search(mes_file, &mes_file_entry)) {
            mes_get_msg(mes_file, &mes_file_entry);
            v1 += sub_4A5E10(obj, mes_file_entry.str);
        }
        v1 += sub_4A5920(obj, mes_file, (value + level + 9) / 10 + 700);
    }

    // Throwing
    value = basic_skill_level(obj, BASIC_SKILL_THROWING);
    if (value > 0) {
        mes_file_entry.num = 800;
        if (mes_search(mes_file, &mes_file_entry)) {
            mes_get_msg(mes_file, &mes_file_entry);
            v1 += sub_4A5E10(obj, mes_file_entry.str);
        }
        v1 += sub_4A5920(obj, mes_file, (value + level + 9) / 10 + 800);
    }

    // Firearms
    value = tech_skill_level(obj, TECH_SKILL_FIREARMS);
    if (value > 0) {
        mes_file_entry.num = 900;
        if (mes_search(mes_file, &mes_file_entry)) {
            mes_get_msg(mes_file, &mes_file_entry);
            v1 += sub_4A5E10(obj, mes_file_entry.str);
        }
        v1 += sub_4A5920(obj, mes_file, (value + level + 9) / 10 + 900);
    }

    if (v1 != 0) {
        return v1;
    }


    // Fail safe weapons (if you didn't get any of the above).
    if (level <= 20) {
        // {6029} Dagger
        mes_file_entry.num = 1001;
    } else {
        if (stat_level(obj, STAT_MAGICK_POINTS) >= 75) {
            // {6163} Mage's Staff
            mes_file_entry.num = 1002;
        } else if (stat_level(obj, STAT_MAGICK_POINTS) >= 20) {
            // {6054} Magic Dagger
            mes_file_entry.num = 1003;
        } else if (stat_level(obj, STAT_TECH_POINTS) >= 75) {
            // {6053} Mechanical Dagger
            mes_file_entry.num = 1004;
        } else {
            // {6051} Quality Dagger
            mes_file_entry.num = 1005;
        }
    }

    mes_get_msg(mes_file, &mes_file_entry);

    return sub_4A5E10(obj, mes_file_entry.str);
}

// 0x4A5CA0
void sub_4A5CA0()
{
    // TODO: Incomplete.
}

// 0x4A5D80
void sub_4A5D80()
{
    // TODO: Incomplete.
}

// 0x4A5E10
void sub_4A5E10()
{
    // TODO: Incomplete.
}

// 0x4A5EE0
void sub_4A5EE0()
{
    // TODO: Incomplete.
}

// 0x4A6010
void sub_4A6010(int64_t obj)
{
    unsigned int flags;
    int poison;
    int index;

    object_set_hp_damage(obj, 0);
    sub_4ED720(obj, 0);
    sub_4EFEE0(obj, 2);
    sub_4EFEE0(obj, 4);

    flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS);
    flags &= ~0xFC03D0FF;
    sub_4EFDD0(obj, OBJ_F_CRITTER_FLAGS, flags);

    flags = obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS2);
    flags &= ~0x3FFFFF;
    sub_4EFDD0(obj, OBJ_F_CRITTER_FLAGS2, flags);

    if (combat_critter_is_combat_mode_active(obj)) {
        combat_critter_deactivate_combat_mode(obj);
    }

    poison = stat_get_base(obj, STAT_POISON_LEVEL);
    if (poison != 0) {
        stat_set_base(obj, STAT_POISON_LEVEL, poison);
    }

    for (index = 0; index < 10; index++) {
        if (index < 0 || index > 2) {
            sub_4EA2E0(obj, index);
        }
    }

    sub_4F0500(obj, OBJ_F_PC_QUEST_IDX);
    sub_4F0500(obj, OBJ_F_PC_RUMOR_IDX);
    sub_4F0500(obj, OBJ_F_PC_BLESSING_IDX);
    sub_4F0500(obj, OBJ_F_PC_BLESSING_TS_IDX);
    sub_4F0500(obj, OBJ_F_PC_CURSE_IDX);
    sub_4F0500(obj, OBJ_F_PC_CURSE_TS_IDX);
    sub_4F0500(obj, OBJ_F_PC_REPUTATION_IDX);
    sub_4F0500(obj, OBJ_F_PC_REPUTATION_TS_IDX);
    sub_4F0500(obj, OBJ_F_CRITTER_FOLLOWER_IDX);
    sub_4F0500(obj, OBJ_F_PC_GLOBAL_FLAGS);
    sub_4F0500(obj, OBJ_F_PC_GLOBAL_VARIABLES);
    sub_4F0500(obj, OBJ_F_SPELL_FLAGS);

    for (index = 0; index < 7; index++) {
        sub_4F0360(obj, OBJ_F_OVERLAY_FORE, index, TIG_ART_ID_INVALID);
        sub_4F0360(obj, OBJ_F_OVERLAY_BACK, index, TIG_ART_ID_INVALID);
    }
}

// 0x4A6190
void sub_4A6190()
{
    // TODO: Incomplete.
}

// 0x4A6470
void sub_4A6470()
{
    // TODO: Incomplete.
}

// 0x4A6560
void sub_4A6560()
{
    // TODO: Incomplete.
}
