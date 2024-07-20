#include "game/curse.h"

#include "game/mes.h"

#define CURSE_F_NAME 0
#define CURSE_F_EFFECT 1
#define CURSE_F_DESCRIPTION 2

static void curse_copy_field(int curse, int field, char* buffer);

// 0x5FF40C
static int curse_mes_file;

// 0x4C3C60
bool curse_mod_load()
{
    mes_load("mes\\gamecurse.mes", &curse_mes_file);
    return true;
}

// 0x4C3C80
void curse_mod_unload()
{
    mes_unload(curse_mes_file);
    curse_mes_file = MES_FILE_HANDLE_INVALID;
}

// 0x4C3CA0
void curse_copy_name(int curse, char* buffer)
{
    curse_copy_field(curse, CURSE_F_NAME, buffer);
}

// 0x4C3CC0
void curse_copy_field(int curse, int field, char* buffer)
{
    MesFileEntry mes_file_entry;

    buffer[0] = '\0';

    if (curse >= 50) {
        mes_file_entry.num = curse * 10 + field;
        if (mes_search(curse_mes_file, &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);
        }
    }
}

// 0x4C3D30
void curse_copy_description(int curse, char* buffer)
{
    curse_copy_field(curse, CURSE_F_DESCRIPTION, buffer);
}

// 0x4C3D50
int sub_4C3D50(object_id_t obj, CurseInfo* curses)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    for (index = 0; index < cnt; index++) {
        curses[index].id = sub_407470(obj, OBJ_F_PC_CURSE_IDX, index);
        curses[index].ts = obj_arrayfield_int64_get(obj, OBJ_F_PC_CURSE_TS_IDX, index);
    }

    return cnt;
}

// 0x4C3DD0
bool curse_is_added_to(object_id_t obj, int curse)
{
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return false;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_PC_CURSE_IDX);
    for (index = 0; index < cnt; index++) {
        if (sub_407470(obj, OBJ_F_PC_CURSE_IDX, index) == curse) {
            return true;
        }
    }

    return true;
}

// 0x4C3E40
void curse_add(object_id_t obj, int curse)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (curse_is_added_to(obj, curse)) {
        return;
    }

    // TODO: Incomplete.
}

// 0x4C3F70
int curse_get_effect(int curse)
{
    char buffer[80];
    curse_copy_field(curse, CURSE_F_EFFECT, buffer);
    return atoi(buffer);
}

// 0x4C3FA0
void curse_remove(object_id_t obj, int curse)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    // TODO: Incomplete.
}
