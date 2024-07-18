#include "game/area.h"

#include "game/mes.h"

// 0x5FF5B0
static char** dword_5FF5B0;

// 0x5FF5AC
static int64_t* dword_5FF5AC;

// 0x5FF5B4
static mes_file_handle_t dword_5FF5B4;

// 0x5FF5B8
static void* dword_5FF5B8[8];

// 0x5FF5D8
static char** dword_5FF5D8;

// 0x5FF5DC
static int* dword_5FF5DC;

// 0x5FF5E0
static int* dword_5FF5E0;

// 0x5FF5E4
static int* dword_5FF5E4;

// 0x5FF5E8
static uint8_t* dword_5FF5E8;

void area_mod_unload()
{
    int index;

    if (dword_5FF5B4 != MES_FILE_HANDLE_INVALID) {
        FREE(dword_5FF5B0);
        FREE(dword_5FF5D8);
        FREE(dword_5FF5AC);
        FREE(dword_5FF5E8);

        for (index = 0; index < 8; index++) {
            FREE(dword_5FF5B8[index]);
        }

        FREE(dword_5FF5E4);
        FREE(dword_5FF5DC);
        FREE(dword_5FF5E0);

        mes_unload(dword_5FF5B4);
        dword_5FF5B4 = MES_FILE_HANDLE_INVALID;
    }
}
