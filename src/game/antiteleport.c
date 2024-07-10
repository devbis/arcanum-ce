#include "game/antiteleport.h"

#include <tig/tig.h>

#include "game/mes.h"

typedef struct S5FC490 {
    void* field_0;
    int field_4;
} S5FC490;

typedef struct S5FC498 {
    void* field_0;
    int field_4;
} S5FC498;

static void sub_4BDAC0();
static void sub_4BDB10(S5FC498* a1);
static void sub_4BDC40(S5FC490* a1);

// 0x5FC490
static S5FC490 stru_5FC490;

// 0x5FC498
static S5FC498 stru_5FC498;

// 0x5FC4A0
static bool antiteleport_initialized;

// 0x5FC4A4
static bool dword_5FC4A4;

// 0x5FC4A8
static bool antiteleport_mod_loaded;

// 0x4BD980
bool antiteleport_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (dword_5FC4A4) {
        return true;
    }

    sub_4BDB10(&stru_5FC498);
    sub_4BDC40(&stru_5FC490);

    antiteleport_initialized = true;

    return true;
}

// 0x4BD9C0
void antiteleport_exit()
{
    if (!dword_5FC4A4) {
        sub_4BDAC0();
        antiteleport_initialized = false;
    }
}

// 0x4BD9E0
bool antiteleport_mod_load()
{
    mes_file_handle_t antiteleport_mes_file;

    if (dword_5FC4A4) {
        return true;
    }

    if (!mes_load("Rules\\AntiTeleport.mes", &antiteleport_mes_file)) {
        // FIXME: Typo in function name.
        tig_debug_printf("AntiTeleport: antiteleport_init: Note: No Message File Detected.\n");
        return true;
    }

    tig_str_parse_set_separator(',');

    if (!sub_4BDB30(antiteleport_mes_file, &stru_5FC498, 100)) {
        tig_debug_println("Disabling Anti-Teleport Regions because of bad message file (Region List).");
        mes_unload(antiteleport_mes_file);
        dword_5FC4A4 = true;
        return true;
    }

    if (!sub_4BDC60(antiteleport_mes_file, &stru_5FC490, 1000)) {
        tig_debug_println("Disabling Anti-Teleport Regions because of bad message file (Map List).");
        mes_unload(antiteleport_mes_file);
        dword_5FC4A4 = true;
        return true;
    }

    mes_unload(antiteleport_mes_file);
    antiteleport_mod_loaded = true;

    return true;
}

// 0x4BDAB0
void antiteleport_mod_unload()
{
    sub_4BDAC0();
    antiteleport_mod_loaded = false;
}

// 0x4BDAC0
void sub_4BDAC0()
{
    if (stru_5FC498.field_0 != NULL) {
        free(stru_5FC498.field_0);
        stru_5FC498.field_0 = 0;
        stru_5FC498.field_4 = 0;
    }

    if (stru_5FC490.field_0 != NULL) {
        free(stru_5FC490.field_0);
        stru_5FC490.field_0 = NULL;
        stru_5FC490.field_4 = 0;
    }
}

// 0x4BDB10
void sub_4BDB10(S5FC498* a1)
{
    a1->field_0 = NULL;
    a1->field_4 = 0;
}

// 0x4BDC40
void sub_4BDC40(S5FC490* a1)
{
    a1->field_0 = NULL;
    a1->field_4 = 0;
}
