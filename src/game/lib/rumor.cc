#include "game/lib/rumor.h"

#include <stdio.h>

#include "game/lib/message.h"

#define TWO_FIVE_ONE 251

#define FIVE 5

// 0x5B6E98
static const char* off_5B6E98[FIVE] = {
    "game_rd_npc_m2m",
    "game_rd_npc_m2f",
    "game_rd_npc_f2f",
    "game_rd_npc_f2m",
    "game_rd_npc_m2m_dumb",
};

// 0x5FF420
static int* dword_5FF420;

// 0x6876C8
static char* dword_6876C8;

// 0x4C5500
bool rumor_init(GameContext* ctx)
{
    dword_6876C8 = (char*)calloc(TWO_FIVE_ONE, sizeof(*dword_6876C8));
    dword_5FF420 = (int*)calloc(FIVE, sizeof(*dword_5FF420));
    return true;
}

// 0x4C5530
void rumor_reset()
{
    for (int index = 0; index < TWO_FIVE_ONE; index++) {
        dword_6876C8[index] = 0;
    }
}

// 0x4C5550
void rumor_exit()
{
    free(dword_6876C8);
    free(dword_5FF420);
}

// 0x4C5570
bool rumor_mod_load()
{
    for (int index = 0; index < FIVE; index++) {
        char path[MAX_PATH];
        sprintf(path, "mes\\%s.mes", off_5B6E98[index]);
        message_load(path, &(dword_5FF420[index]));
    }

    rumor_reset();

    return true;
}

// 0x4C55C0
void rumor_mod_unload()
{
    for (int index = 0; index < FIVE; index++) {
        message_unload(dword_5FF420[index]);
        dword_5FF420[index] = -1;
    }
}

// 0x4C55F0
bool rumor_load(LoadContext* ctx)
{
    if (tig_file_fread(dword_6876C8, TWO_FIVE_ONE, 1, ctx->stream) != 1) return false;

    return true;
}

// 0x4C5620
bool rumor_save(TigFile* stream)
{
    if (tig_file_fwrite(dword_6876C8, TWO_FIVE_ONE, 1, stream) != 1) return false;

    return true;
}
