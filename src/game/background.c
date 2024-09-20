#include "game/background.h"

#include <stdio.h>

#include "game/effect.h"
#include "game/mes.h"
#include "game/stat.h"

#define FIRST_DESCRIPTION_ID 1000
#define BACKGROUND_BLOCK_SIZE 10
#define BACKGROUND_F_DESCRIPTION 0
#define BACKGROUND_F_EFFECT 1
#define BACKGROUND_F_CONDITIONS 2
#define BACKGROUND_F_MONEY 3
#define BACKGROUND_F_ITEMS 4

#define BACKGROUND_NAME_LENGTH 32

static int background_get_base_money();
static bool sub_4C2B50(int64_t obj, char* str);

// 0x5B6AEC
static const char* off_5B6AEC[] = {
    "HU",
    "DW",
    "EL",
    "HE",
    "GN",
    "HA",
    "HO",
    "HG",
};

// 0x5B6B0C
static const char* off_5B6B0C = "FM";

// 0x5FD858
static char* background_description_name;

// 0x5FD85C
static mes_file_handle_t background_mes_file;

// 0x5FD860
static mes_file_handle_t background_description_mes_file;

// 0x4C2270
bool background_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!mes_load("rules\\backgrnd.mes", &background_mes_file)) {
        return false;
    }

    if (!mes_load("mes\\gameback.mes", &background_description_mes_file)) {
        mes_unload(background_mes_file);
        return false;
    }

    background_description_name = (char*)CALLOC(BACKGROUND_NAME_LENGTH, sizeof(*background_description_name));

    return true;
}

// 0x4C22D0
void background_exit()
{
    FREE(background_description_name);
    mes_unload(background_mes_file);
    mes_unload(background_description_mes_file);
}

// 0x4C2300
bool background_find_first(int64_t obj, int* background_ptr)
{
    MesFileEntry mes_file_entry;
    int background = 0;

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_CONDITIONS;
    while (mes_search(background_mes_file, &mes_file_entry)) {
        if (sub_4C2B50(obj, mes_file_entry.str)) {
            *background_ptr = background;
            return true;
        }
        background++;
        mes_file_entry.num += BACKGROUND_BLOCK_SIZE;
    }

    return false;
}

// 0x4C2390
bool background_find_next(int64_t obj, int* background_ptr)
{
    MesFileEntry mes_file_entry;
    int background = *background_ptr + 1;

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_CONDITIONS;
    while (mes_search(background_mes_file, &mes_file_entry)) {
        if (sub_4C2B50(obj, mes_file_entry.str)) {
            *background_ptr = background;
            return true;
        }
        background++;
        mes_file_entry.num += BACKGROUND_BLOCK_SIZE;
    }

    return false;
}

// 0x4C2420
bool background_find_prev(int64_t obj, int* background_ptr)
{
    MesFileEntry mes_file_entry;
    int background = *background_ptr - 1;

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_CONDITIONS;
    while (mes_search(background_mes_file, &mes_file_entry)) {
        if (sub_4C2B50(obj, mes_file_entry.str)) {
            *background_ptr = background;
            return true;
        }
        background--;
        mes_file_entry.num -= BACKGROUND_BLOCK_SIZE;
    }

    return false;
}

// 0x4C24B0
int background_get_description(int background)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_DESCRIPTION;
    mes_get_msg(background_mes_file, &mes_file_entry);
    return atoi(mes_file_entry.str);
}

// 0x4C24E0
const char* background_description_get_text(int num)
{
    MesFileEntry mes_file_entry;

    if (num < FIRST_DESCRIPTION_ID) {
        if (num != 0) {
            return NULL;
        }
        num = FIRST_DESCRIPTION_ID;
    }

    mes_file_entry.num = num;
    if (!mes_search(background_description_mes_file, &mes_file_entry)) {
        return NULL;
    }

    return mes_file_entry.str;
}

// NOTE: Why this function does not use `background_description_get_text` under
// the hood like `background_description_get_name` do?
//
// 0x4C2530
const char* background_description_get_body(int num)
{
    MesFileEntry mes_file_entry;
    char* pch;

    if (num < FIRST_DESCRIPTION_ID) {
        if (num != 0) {
            return NULL;
        }
        num = FIRST_DESCRIPTION_ID;
    }

    mes_file_entry.num = num;
    if (!mes_search(background_description_mes_file, &mes_file_entry)) {
        return NULL;
    }

    pch = strchr(mes_file_entry.str, '\n');
    if (pch != NULL) {
        return pch + 1;
    } else {
        return mes_file_entry.str;
    }
}

// 0x4C2590
const char* background_description_get_name(int num)
{
    const char* text;
    char* pch;

    text = background_description_get_text(num);
    strncpy(background_description_name, text, BACKGROUND_NAME_LENGTH);

    pch = strchr(background_description_name, '\n');
    if (pch != NULL) {
        *pch = '\0';
    } else {
        tig_debug_printf("Background.c: ERROR: Background text has no name!");
    }

    return background_description_name;
}

// 0x4C25E0
void sub_4C25E0(int64_t obj, int background, int background_text)
{
    MesFileEntry mes_file_entry;

    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, background);
    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, background_text);

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_EFFECT;
    if (mes_file_entry.num != 0) {
        mes_get_msg(background_mes_file, &mes_file_entry);
        sub_4E9F70(obj, atoi(mes_file_entry.str), 1);
    }
}

// 0x4C2650
void background_obj_clear(object_id_t obj)
{
    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 0);
    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 0);
    sub_4EA2E0(obj, 1);
}

// 0x4C2690
int background_obj_get_background(object_id_t obj)
{
    if (obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        return obj_field_int32_get(obj, OBJ_F_PC_BACKGROUND);
    }

    return 0;
}

// 0x4C26D0
int background_obj_get_background_text(object_id_t obj)
{
    return obj_field_int32_get(obj, OBJ_F_PC_BACKGROUND_TEXT);
}

// 0x4C2A70
static int background_get_base_money()
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = BACKGROUND_F_MONEY;
    mes_get_msg(background_mes_file, &mes_file_entry);
    return atoi(mes_file_entry.str);
}

// 0x4C2AA0
int background_adjust_money(int amount, int background)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_MONEY;
    mes_get_msg(background_mes_file, &mes_file_entry);

    return amount * (100 * atoi(mes_file_entry.str) / background_get_base_money()) / 100;
}

// 0x4C2B10
bool background_get_items(char* dest, size_t size, int background)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_ITEMS;
    strncpy(dest, mes_file_entry.str, size);

    return true;
}

// 0x4C2B50
bool sub_4C2B50(int64_t obj, char* str)
{
    int gender;
    int race;
    char buffer[4];

    if (str != NULL && *str != '\0') {
        race = stat_get_base(obj, STAT_RACE);
        gender = stat_get_base(obj, STAT_GENDER);
        sprintf(buffer, "%s%c", off_5B6AEC[race], off_5B6B0C[gender]);
        str = strupr(str);
        if (strstr(str, buffer) == NULL
            && strstr(str, "ANY") == NULL
            && ((obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC
                || strstr(str, "NPC") == NULL))) {
            return false;
        }
    }

    return true;
}
