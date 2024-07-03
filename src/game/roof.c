#include "game/roof.h"

#include <tig/tig.h>

// 0x5E2E34
static GameContextF8* dword_5E2E34;

// 0x5E2E44
static tig_window_handle_t dword_5E2E44;

// 0x4390D0
void roof_exit()
{
    dword_5E2E44 = TIG_WINDOW_HANDLE_INVALID;
    dword_5E2E34 = NULL;
}

// 0x4395C0
int sub_4395C0(int a1)
{
    return ((a1 >> 2) & 0xF) + ((a1 >> 8) << 4);
}

// 0x4395E0
int64_t sub_4395E0(int64_t a1)
{
    return ((a1 & 0xFFFFFFFF) - (a1 & 0xFFFFFFFF) % 4 + 2)
        | (((a1 >> 32) - (a1 >> 32) % 4 + 2) << 32);
}
