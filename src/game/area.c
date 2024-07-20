#include "game/area.h"

#include "game/mes.h"
#include "game/obj.h"

// 0x5FF5A8
static int dword_5FF5A8;

// 0x5FF5AC
static int64_t* dword_5FF5AC;

// 0x5FF5B0
static char** dword_5FF5B0;

// 0x5FF5B4
static mes_file_handle_t dword_5FF5B4;

// 0x5FF5B8
static unsigned char* dword_5FF5B8[8];

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

// 0x5FF5EC
static int dword_5FF5EC;

// 0x5FF5F0
static int dword_5FF5F0[8];

// 0x4CA940
bool area_init(GameInitInfo* init_info)
{
    (void)init_info;

    dword_5FF5A8 = 0;

    return true;
}

// 0x4CA950
void area_reset()
{
    int index;
    int player;

    for (index = 0; index < dword_5FF5A8; index++) {
        dword_5FF5E8[index] = 0;
    }

    for (player = 0; player < 8; player++) {
        for (index = 0; index < dword_5FF5A8; index++) {
            dword_5FF5B8[player][index] = 0;
        }
    }
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

// 0x4CAD50
bool area_load(GameLoadInfo* load_info)
{
    int player;

    if (tig_file_fread(dword_5FF5E8, dword_5FF5A8, 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_5FF5EC, sizeof(dword_5FF5EC), 1, load_info->stream) != 1) return false;

    for (player = 0; player < 8; player++) {
        if (tig_file_fread(dword_5FF5B8[player], dword_5FF5A8, 1, load_info->stream) != 1) return false;
        if (tig_file_fread(&(dword_5FF5F0[player]), sizeof(dword_5FF5F0[0]), 1, load_info->stream) != 1) return false;
    }

    return true;
}

// 0x4CADF0
bool area_save(TigFile* stream)
{
    int player;

    if (tig_file_fwrite(dword_5FF5E8, dword_5FF5A8, 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_5FF5EC, sizeof(dword_5FF5EC), 1, stream) != 1) return false;

    for (player = 0; player < 8; player++) {
        if (tig_file_fwrite(dword_5FF5B8[player], dword_5FF5A8, 1, stream) != 1) return false;
        if (tig_file_fwrite(&(dword_5FF5F0[player]), sizeof(dword_5FF5F0[0]), 1, stream) != 1) return false;
    }

    return true;
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

// 0x4CAF00
void sub_4CAF00(int index, int* a2, int* a3)
{
    // FIXME: No overflow precondition.
    *a2 = dword_5FF5DC[index];
    *a3 = dword_5FF5E0[index];
}

// 0x4CB100
int sub_4CB100(int64_t obj_handle)
{
    int v1;

    if (obj_handle != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj_handle, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if ((tig_net_flags & 0x1) != 0) {
            v1 = sub_4A2B10(obj_handle);
            if (v1 != -1) {
                return dword_5FF5F0[v1];
            }
        } else {
            if (obj_handle == player_get_pc_obj()) {
                return dword_5FF5EC;
            }
        }
    }

    return 0;
}
