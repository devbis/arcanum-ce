#include "game/bless.h"

#include "game/mes.h"

#define BLESS_F_NAME 0
#define BLESS_F_EFFECT 1
#define BLESS_F_DESCRIPTION 2

static void bless_copy_field(int bless, int field, char* buffer);

// 0x5FF410
static mes_file_handle_t bless_mes_file;

// 0x4C4110
bool bless_mod_load()
{
    mes_load("mes\\gamebless.mes", &bless_mes_file);
    return true;
}

// 0x4C4130
void bless_mod_unload()
{
    mes_unload(bless_mes_file);
    bless_mes_file = MES_FILE_HANDLE_INVALID;
}

// 0x4C4150
void bless_copy_name(int bless, char* buffer)
{
    bless_copy_field(bless, BLESS_F_NAME, buffer);
}

// 0x4C4170
void bless_copy_field(int bless, int field, char* buffer)
{
    MesFileEntry mes_file_entry;

    buffer[0] = '\0';

    if (bless >= 50) {
        mes_file_entry.num = bless * 10 + field;
        if (mes_search(bless_mes_file, &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);
        }
    }
}

// 0x4C41E0
void bless_copy_description(int bless, char* buffer)
{
    bless_copy_field(bless, BLESS_F_DESCRIPTION, buffer);
}

// 0x4C4200
int sub_4C4200(object_id_t obj, BlessInfo* blessings)
{
    int cnt;
    int index;

    if (obj_f_get_int32(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    cnt = sub_4079C0(obj, OBJ_F_PC_BLESSING_IDX);
    for (index = 0; index < cnt; index++) {
        blessings[index].id = sub_407470(obj, OBJ_F_PC_BLESSING_IDX, index);
        blessings[index].ts = sub_407540(obj, OBJ_F_PC_BLESSING_TS_IDX, index);
    }

    return cnt;
}

// 0x4C4280
bool bless_is_added_to(object_id_t obj, int bless)
{
    int cnt;
    int index;

    if (obj_f_get_int32(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    cnt = sub_4079C0(obj, OBJ_F_PC_BLESSING_IDX);
    for (index = 0; index < cnt; index++) {
        if (sub_407470(obj, OBJ_F_PC_BLESSING_IDX, index) == bless) {
            return true;
        }
    }

    return true;
}

// 0x4C42F0
void bless_add(object_id_t obj, int bless)
{
    if (obj_f_get_int32(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    // TODO: Incomplete.
}

// 0x4C4420
int bless_get_effect(int bless)
{
    char buffer[80];
    bless_copy_field(bless, BLESS_F_EFFECT, buffer);
    return atoi(buffer);
}

// 0x4C4450
void bless_remove(object_id_t obj, int bless)
{
    if (obj_f_get_int32(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    // TODO: Incomplete.
}
