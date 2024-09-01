#include "game/multiplayer.h"

#include "game/map.h"
#include "game/mes.h"
#include "game/timeevent.h"

#define NUM_PLAYERS 8

typedef struct S5E8AD0 {
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
    /* 0034 */ int field_34;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ int field_40;
    /* 0044 */ int field_44;
    /* 0048 */ int field_48;
    /* 004C */ int field_4C;
} S5E8AD0;

static_assert(sizeof(S5E8AD0) == 0x50, "wrong size");

static bool sub_49D570(TimeEvent* timeevent);
static void sub_4A1F30(int64_t a1, int64_t a2, int a3, int a4);

// 0x5B4070
static int dword_5B4070 = -1;

// 0x5E8940
static TigIdxTable stru_5E8940;

// 0x5E8AD0
static S5E8AD0 stru_5E8AD0[NUM_PLAYERS];

// 0x5F0BE8
static mes_file_handle_t multiplayer_mes_file;

// 0x5F0BC8
static void* off_5F0BC8[NUM_PLAYERS];

// 0x5F0DEC
static void* dword_5F0DEC;

// 0x5F0E00
static bool dword_5F0E00;

// 0x5F0E0C
static int dword_5F0E0C;

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
    // TODO: Incomplete.
}

// 0x49C820
void multiplayer_reset()
{
    // TODO: Incomplete.
}

// 0x49C930
void multiplayer_save()
{
    // TODO: Incomplete.
}

// 0x49CA20
void mutliplayer_load()
{
    // TODO: Incomplete.
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
void sub_49CB80()
{
    // TODO: Incomplete.
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
        tig_debug_prinln("Object system validate failed pre-load in multiplayer_map_open_by_name.");
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
void sub_4A1F30(int64_t a1, int64_t a2, int a3, int a4)
{
    if (a2 != 0) {
        sub_43E770(a1, a2, a3, a4);
    }
}

// 0x4A1F60
void sub_4A1F60()
{
    // TODO: Incomplete.
}

// 0x4A1FC0
void sub_4A1FC0()
{
    // TODO: Incomplete.
}

// 0x4A2020
void sub_4A2020()
{
    // TODO: Incomplete.
}

// 0x4A2040
void sub_4A2040()
{
    // TODO: Incomplete.
}

// 0x4A2070
void sub_4A2070()
{
    // TODO: Incomplete.
}

// 0x4A2A30
void sub_4A2A30()
{
    // TODO: Incomplete.
}

// 0x4A2A40
void sub_4A2A40()
{
    // TODO: Incomplete.
}

// 0x4A2A90
void sub_4A2A90()
{
    // TODO: Incomplete.
}

// 0x4A2AE0
void sub_4A2AE0()
{
    // TODO: Incomplete.
}

// 0x4A2B00
void sub_4A2B00()
{
    // TODO: Incomplete.
}

// 0x4A2B10
void sub_4A2B10()
{
    // TODO: Incomplete.
}

// 0x4A2B60
void sub_4A2B60()
{
    // TODO: Incomplete.
}

// 0x4A2BA0
void sub_4A2BA0()
{
    // TODO: Incomplete.
}

// 0x4A2BC0
void sub_4A2BC0()
{
    // TODO: Incomplete.
}

// 0x4A2BD0
void sub_4A2BD0()
{
    // TODO: Incomplete.
}

// 0x4A2BE0
void sub_4A2BE0()
{
    // TODO: Incomplete.
}

// 0x4A2C60
void sub_4A2C60()
{
    // TODO: Incomplete.
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
void sub_4A3660()
{
    // TODO: Incomplete.
}

// 0x4A3780
void sub_4A3780()
{
    // TODO: Incomplete.
}

// 0x4A3890
void nullsub_39()
{
    // TODO: Incomplete.
}

// 0x4A38A0
void sub_4A38A0()
{
    // TODO: Incomplete.
}

// 0x4A38B0
void sub_4A38B0()
{
    // TODO: Incomplete.
}

// 0x4A39D0
void sub_4A39D0()
{
    // TODO: Incomplete.
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
void sub_4A40D0()
{
    // TODO: Incomplete.
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
void sub_4A4230()
{
    // TODO: Incomplete.
}

// 0x4A4240
void sub_4A4240()
{
    // TODO: Incomplete.
}

// 0x4A4270
void sub_4A4270()
{
    // TODO: Incomplete.
}

// 0x4A4280
void sub_4A4280()
{
    // TODO: Incomplete.
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
void sub_4A47D0()
{
    // TODO: Incomplete.
}

// 0x4A49E0
void sub_4A49E0()
{
    // TODO: Incomplete.
}

// 0x4A4C40
void sub_4A4C40()
{
    // TODO: Incomplete.
}

// 0x4A4D60
void sub_4A4D60()
{
    // TODO: Incomplete.
}

// 0x4A4EB0
void sub_4A4EB0()
{
    // TODO: Incomplete.
}

// 0x4A4EC0
void sub_4A4EC0()
{
    // TODO: Incomplete.
}

// 0x4A4ED0
void sub_4A4ED0()
{
    // TODO: Incomplete.
}

// 0x4A4EE0
void sub_4A4EE0()
{
    // TODO: Incomplete.
}

// 0x4A4EF0
void sub_4A4EF0()
{
    // TODO: Incomplete.
}

// 0x4A4F10
void sub_4A4F10()
{
    // TODO: Incomplete.
}

// 0x4A4F30
void sub_4A4F30()
{
    // TODO: Incomplete.
}

// 0x4A4F50
void sub_4A4F50()
{
    // TODO: Incomplete.
}

// 0x4A4F60
void sub_4A4F60()
{
    // TODO: Incomplete.
}

// 0x4A4F80
void sub_4A4F80()
{
    // TODO: Incomplete.
}

// 0x4A4F90
void sub_4A4F90()
{
    // TODO: Incomplete.
}

// 0x4A4FA0
void sub_4A4FA0()
{
    // TODO: Incomplete.
}

// 0x4A4FB0
void sub_4A4FB0()
{
    // TODO: Incomplete.
}

// 0x4A4FD0
void sub_4A4FD0()
{
    // TODO: Incomplete.
}

// 0x4A4FE0
void sub_4A4FE0()
{
    // TODO: Incomplete.
}

// 0x4A5000
void sub_4A5000()
{
    // TODO: Incomplete.
}

// 0x4A5010
void sub_4A5010()
{
    // TODO: Incomplete.
}

// 0x4A5020
void sub_4A5020()
{
    // TODO: Incomplete.
}

// 0x4A5040
void sub_4A5040()
{
    // TODO: Incomplete.
}

// 0x4A5050
void sub_4A5050()
{
    // TODO: Incomplete.
}

// 0x4A5070
void sub_4A5070()
{
    // TODO: Incomplete.
}

// 0x4A5090
void sub_4A5090()
{
    // TODO: Incomplete.
}

// 0x4A50A0
void sub_4A50A0()
{
    // TODO: Incomplete.
}

// 0x4A50C0
void sub_4A50C0()
{
    // TODO: Incomplete.
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
    // TODO: Incomplete.
}

// 0x4A54A0
void sub_4A54A0()
{
    // TODO: Incomplete.
}

// 0x4A54E0
void sub_4A54E0()
{
    // TODO: Incomplete.
}

// 0x4A5510
void sub_4A5510()
{
    // TODO: Incomplete.
}

// 0x4A5570
void sub_4A5570()
{
    // TODO: Incomplete.
}

// 0x4A55D0
void sub_4A55D0()
{
    // TODO: Incomplete.
}

// 0x4A55F0
void sub_4A55F0()
{
    // TODO: Incomplete.
}

// 0x4A5600
void sub_4A5600()
{
    // TODO: Incomplete.
}

// 0x4A5610
void sub_4A5610()
{
    // TODO: Incomplete.
}

// 0x4A5670
void sub_4A5670()
{
    // TODO: Incomplete.
}

// 0x4A5710
void sub_4A5710()
{
    // TODO: Incomplete.
}

// 0x4A57F0
void sub_4A57F0()
{
    // TODO: Incomplete.
}

// 0x4A5840
void sub_4A5840()
{
    // TODO: Incomplete.
}

// 0x4A5920
void sub_4A5920()
{
    // TODO: Incomplete.
}

// 0x4A59F0
void sub_4A59F0()
{
    // TODO: Incomplete.
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
void sub_4A6010()
{
    // TODO: Incomplete.
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
