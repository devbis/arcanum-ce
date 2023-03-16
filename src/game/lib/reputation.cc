#include "game/lib/reputation.h"

#include <stdio.h>

#include "game/lib/message.h"

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
};

// See 0x4C15D0.
static_assert(sizeof(ReputationData) == 0x4C, "wrong size");

// 0x5B69C0
static const char* off_5B69C0[FOUR] = {
    "game_rp_npc_m2m",
    "game_rp_npc_m2f",
    "game_rp_npc_f2f",
    "game_rp_npc_f2m",
};

// 0x5FC8AC
static int dword_5FC8AC;

// 0x5FC8B0
static ReputationData* dword_5FC8B0;

// 0x5FC8B4
static int* dword_5FC8B4;

// 0x4C15D0
bool reputation_init(GameContext* ctx)
{
    dword_5FC8B0 = (ReputationData*)calloc(THOUSAND, sizeof(*dword_5FC8B0));
    dword_5FC8B4 = (int*)calloc(FOUR, sizeof(*dword_5FC8B4));
    return true;
}

// 0x4C1600
void reputation_exit()
{
    free(dword_5FC8B0);
    free(dword_5FC8B4);
}

// 0x4C1620
bool reputation_mod_load()
{
    for (int index = 0; index < FOUR; index++) {
        dword_5FC8B4[index] = -1;
    }

    dword_5FC8AC = -1;

    if (!sub_4C1730("rules\\gamerep.mes", 232, 1999)) {
        return true;
    }

    if (!message_load("mes\\gamereplog.mes", &dword_5FC8AC)) {
        return true;
    }

    for (int index = 0; index < FOUR; index++) {
        char path[MAX_PATH];
        sprintf(path, "mes\\%s.mes", off_5B69C0[index]);
        message_load(path, &(dword_5FC8B4[index]));
    }
}

// 0x4C16E0
void reputation_mod_unload()
{
    for (int index = 0; index < FOUR; index++) {
        message_unload(dword_5FC8B4[index]);
        dword_5FC8B4[index] = -1;
    }

    message_unload(dword_5FC8AC);
    dword_5FC8AC = -1;
}
