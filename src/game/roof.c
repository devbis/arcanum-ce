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
