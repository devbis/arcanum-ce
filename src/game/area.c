#include "game/area.h"

#include "game/mes.h"

// 0x5FF5A8
static int dword_5FF5A8;

// 0x5FF5AC
static int64_t* dword_5FF5AC;

// 0x5FF5B0
static char** dword_5FF5B0;

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

// 0x4CA940
bool area_init(GameInitInfo* init_info)
{
    (void)init_info;

    dword_5FF5A8 = 0;

    return true;
}

// 0x4CA9A0
void area_exit()
{
}

// 0x4CACB0
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

// 0x4CAE80
int sub_4CAE80()
{
  return dword_5FF5A8;
}

// 0x4CAE90
char* sub_4CAE90(int index)
{
    return index >= 0 && index < dword_5FF5A8
        ? dword_5FF5B0[index]
        : NULL;
}

// 0x4CAEB0
char* sub_4CAEB0(int index)
{
    return index >= 0 && index < dword_5FF5A8
        ? dword_5FF5D8[index]
        : NULL;
}

// 0x4CAED0
int64_t sub_4CAED0(int index)
{
    return index >= 0 && index < dword_5FF5A8
        ? dword_5FF5AC[index]
        : 0;
}
