#include "game/lib/teleport.h"

// 0x601844
static int dword_601844;

// 0x601848
static GameContextF8* dword_601848;

// 0x601850
static GameContextFC* dword_601850;

// 0x4D32D0
bool teleport_init(GameContext* ctx)
{
    dword_601848 = ctx->field_8;
    dword_601850 = ctx->field_C;
    dword_601844 = 0;
    return true;
}

// 0x4D3300
bool teleport_reset()
{
    sub_4D3F30();
    dword_601844 = 0;
    return true;
}

// 0x4D3320
void teleport_exit()
{
    teleport_reset();
}
