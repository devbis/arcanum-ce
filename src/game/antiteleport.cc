#include "game/lib/antiteleport.h"

#include "game/lib/message.h"
#include "tig/debug.h"
#include "tig/str_parse.h"

typedef struct S5FC490 {
    void* field_0;
    int field_4;
};

typedef struct S5FC498 {
    void* field_0;
    int field_4;
};

// 0x5FC490
static S5FC490 stru_5FC490;

// 0x5FC498
static S5FC498 stru_5FC498;

// 0x5FC4A0
static bool dword_5FC4A0;

// 0x5FC4A4
static bool dword_5FC4A4;

// 0x5FC4A8
static bool dword_5FC4A8;

// 0x4BD980
bool antiteleport_init(GameContext* ctx)
{
    if (dword_5FC4A4) {
        return true;
    }

    sub_4BDB10(&stru_5FC498);
    sub_4BDC40(&stru_5FC490);

    dword_5FC4A0 = true;

    return true;
}

// 0x4BD9C0
void antiteleport_exit()
{
    if (!dword_5FC4A4) {
        sub_4BDAC0();
        dword_5FC4A0 = false;
    }
}

// 0x4BD9E0
bool antiteleport_mod_load()
{
    if (dword_5FC4A4) {
        return true;
    }

    int antiteleport_msg_file;
    if (!message_load("Rules\\AntiTeleport.mes", &antiteleport_msg_file)) {
        // FIXME: Typo in function name.
        tig_debug_printf("AntiTeleport: antiteleport_init: Note: No Message File Detected.\n");
        return true;
    }

    tig_str_parse_set_separator(',');

    if (!sub_4BDB30(antiteleport_msg_file, &stru_5FC498, 100)) {
        tig_debug_println("Disabling Anti-Teleport Regions because of bad message file (Region List).");
        message_unload(antiteleport_msg_file);
        dword_5FC4A4 = true;
        return true;
    }

    if (!sub_4BDC60(antiteleport_msg_file, &stru_5FC490, 100)) {
        tig_debug_println("Disabling Anti-Teleport Regions because of bad message file (Region List).");
        message_unload(antiteleport_msg_file);
        dword_5FC4A4 = true;
        return true;
    }

    message_unload(antiteleport_msg_file);
    dword_5FC4A8 = true;

    return true;
}

// 0x4BDAB0
void antiteleport_mod_unload()
{
    sub_4BDAC0();
    dword_5FC4A8 = 0;
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
