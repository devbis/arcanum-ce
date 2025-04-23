#include "game/background.h"

#include <stdio.h>

#include "game/effect.h"
#include "game/item.h"
#include "game/mes.h"
#include "game/object.h"
#include "game/proto.h"
#include "game/skill.h"
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
static bool background_is_legal(int64_t obj, char* str);

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
        if (background_is_legal(obj, mes_file_entry.str)) {
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
        if (background_is_legal(obj, mes_file_entry.str)) {
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
        if (background_is_legal(obj, mes_file_entry.str)) {
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
char* background_description_get_text(int num)
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
char* background_description_get_body(int num)
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
char* background_description_get_name(int num)
{
    char* text;
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
void background_set(int64_t obj, int background, int background_text)
{
    MesFileEntry mes_file_entry;

    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, background);
    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, background_text);

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_EFFECT;
    if (mes_file_entry.num != 0) {
        mes_get_msg(background_mes_file, &mes_file_entry);
        effect_add(obj, atoi(mes_file_entry.str), EFFECT_CAUSE_BACKGROUND);
    }
}

// 0x4C2650
void background_clear(int64_t obj)
{
    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND, 0);
    obj_field_int32_set(obj, OBJ_F_PC_BACKGROUND_TEXT, 0);
    effect_remove_one_caused_by(obj, EFFECT_CAUSE_BACKGROUND);
}

// 0x4C2690
int background_get(int64_t obj)
{
    if (obj == OBJ_HANDLE_NULL) {
        return 0;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return 0;
    }

    return obj_field_int32_get(obj, OBJ_F_PC_BACKGROUND);
}

// 0x4C26D0
int background_text_get(int64_t obj)
{
    return obj_field_int32_get(obj, OBJ_F_PC_BACKGROUND_TEXT);
}

// 0x4C26F0
void background_generate_inventory(int64_t obj)
{
    MesFileEntry mes_file_entry;
    char str[MAX_STRING];
    int background;
    char* tok;
    int64_t proto_obj;
    int64_t loc;
    int64_t item_obj;
    ObjectItemFlags item_flags;

    background = obj_field_int32_get(obj, OBJ_F_PC_BACKGROUND);
    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_MONEY;
    mes_get_msg(background_mes_file, &mes_file_entry);

    item_gold_transfer(OBJ_HANDLE_NULL, obj, atoi(mes_file_entry.str), OBJ_HANDLE_NULL);

    mes_file_entry.num = BACKGROUND_BLOCK_SIZE * background + BACKGROUND_F_ITEMS;
    if (mes_search(background_mes_file, &mes_file_entry)) {
        strcpy(str, mes_file_entry.str);

        tok = strtok(str, " \t\n");
        while (tok != NULL) {
            proto_obj = sub_4685A0(atoi(tok));
            loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
            if (object_create(proto_obj, loc, &item_obj)) {
                item_flags = obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS);
                item_flags |= OIF_IDENTIFIED;
                obj_field_int32_set(item_obj, OBJ_F_ITEM_FLAGS, item_flags);
                item_transfer(item_obj, obj);
            }
            tok = strtok(NULL, " \t\n");
        }
    }
}

// 0x4C2950
void background_educate_followers(int64_t obj)
{
    ObjectList followers;
    ObjectNode* node;
    int skill;
    int educator_training;
    int follower_training;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        || background_get(obj) != BACKGROUND_EDUCATOR) {
        return;
    }

    object_list_followers(obj, &followers);

    for (skill = 0; skill < BASIC_SKILL_COUNT; skill++) {
        educator_training = basic_skill_training_get(obj, skill);
        if (educator_training > TRAINING_APPRENTICE) {
            node = followers.head;
            while (node != NULL) {
                follower_training = basic_skill_training_get(node->obj, skill) + 1;
                while (follower_training < educator_training) {
                    basic_skill_training_set(node->obj, skill, follower_training);
                    follower_training++;
                }
                node = node->next;
            }
        }
    }

    for (skill = 0; skill < TECH_SKILL_COUNT; skill++) {
        educator_training = tech_skill_training_get(obj, skill);
        if (educator_training > TRAINING_APPRENTICE) {
            node = followers.head;
            while (node != NULL) {
                follower_training = tech_skill_training_get(node->obj, skill) + 1;
                while (follower_training < educator_training) {
                    tech_skill_training_set(node->obj, skill, follower_training);
                    follower_training++;
                }
                node = node->next;
            }
        }
    }

    object_list_destroy(&followers);
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
    mes_get_msg(background_mes_file, &mes_file_entry);
    strncpy(dest, mes_file_entry.str, size);

    return true;
}

// 0x4C2B50
bool background_is_legal(int64_t obj, char* str)
{
    int gender;
    int race;
    char buffer[4];

    if (str != NULL && *str != '\0') {
        race = stat_base_get(obj, STAT_RACE);
        gender = stat_base_get(obj, STAT_GENDER);
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
