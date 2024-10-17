#include "game/reputation.h"

#include <stdio.h>

#include "game/mes.h"
#include "game/obj.h"

#define THOUSAND 1000
#define FOUR 4

typedef struct ReputationData {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
} ReputationData;

// See 0x4C15D0.
static_assert(sizeof(ReputationData) == 0x4C, "wrong size");

static bool sub_4C1730(const char* path, int start, int end);

// 0x5B69C0
static const char* off_5B69C0[FOUR] = {
    "game_rp_npc_m2m",
    "game_rp_npc_m2f",
    "game_rp_npc_f2f",
    "game_rp_npc_f2m",
};

// 0x5FC8AC
static mes_file_handle_t dword_5FC8AC;

// 0x5FC8B0
static ReputationData* dword_5FC8B0;

// 0x5FC8B4
static int* dword_5FC8B4;

// 0x4C15D0
bool reputation_init(GameInitInfo* init_info)
{
    (void)init_info;

    dword_5FC8B0 = (ReputationData*)CALLOC(THOUSAND, sizeof(*dword_5FC8B0));
    dword_5FC8B4 = (int*)CALLOC(FOUR, sizeof(*dword_5FC8B4));

    return true;
}

// 0x4C1600
void reputation_exit()
{
    FREE(dword_5FC8B0);
    FREE(dword_5FC8B4);
}

// 0x4C1620
bool reputation_mod_load()
{
    int index;
    char path[TIG_MAX_PATH];

    for (index = 0; index < FOUR; index++) {
        dword_5FC8B4[index] = -1;
    }

    dword_5FC8AC = MES_FILE_HANDLE_INVALID;

    if (!sub_4C1730("rules\\gamerep.mes", 232, 1999)) {
        return true;
    }

    if (!mes_load("mes\\gamereplog.mes", &dword_5FC8AC)) {
        return true;
    }

    for (index = 0; index < FOUR; index++) {
        sprintf(path, "mes\\%s.mes", off_5B69C0[index]);
        mes_load(path, &(dword_5FC8B4[index]));
    }

    return true;
}

// 0x4C16E0
void reputation_mod_unload()
{
    int index;

    for (index = 0; index < FOUR; index++) {
        mes_unload(dword_5FC8B4[index]);
        dword_5FC8B4[index] = MES_FILE_HANDLE_INVALID;
    }

    mes_unload(dword_5FC8AC);
    dword_5FC8AC = MES_FILE_HANDLE_INVALID;
}


// 0x4C1730
bool sub_4C1730(const char* path, int start, int end)
{
    // TODO: Incomplete.
}

// 0x4C1AC0
int sub_4C1AC0(int64_t pc_obj, int64_t npc_obj)
{
    // TODO: Incomplete.
}

// 0x4C1BD0
void sub_4C1BD0(int index, char* buffer)
{
    MesFileEntry mes_file_entry;

    buffer[0] = '\0';

    if (index >= 1000) {
        mes_file_entry.num = index;
        if (mes_search(dword_5FC8AC, &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);
        }
    }
}

// 0x4C1C30
void sub_4C1C30()
{
    // TODO: Incomplete.
}

// 0x4C1CB0
bool sub_4C1CB0(int64_t obj, int reputation)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_REPUTATION_IDX);
    for (index = 0; index < cnt; index++) {
        if (sub_407470(obj, OBJ_F_PC_REPUTATION_IDX, index) == reputation) {
            return true;
        }
    }

    return false;
}

// 0x4C1D20
void sub_4C1D20()
{
    // TODO: Incomplete.
}

// 0x4C1E10
void sub_4C1E10()
{
    // TODO: Incomplete.
}

// 0x4C1F80
void sub_4C1F80()
{
    // TODO: Incomplete.
}

// 0x4C2100
void sub_4C2100()
{
    // TODO: Incomplete.
}

// 0x4C21E0
void sub_4C21E0()
{
    // TODO: Incomplete.
}
