#include "game/skill.h"

#include "game/effect.h"
#include "game/gamelib.h"
#include "game/mes.h"
#include "game/stat.h"

#define FIRST_BASIC_SKILL_NAME_ID 0
#define FIRST_TECH_SKILL_NAME_ID (FIRST_BASIC_SKILL_NAME_ID + BASIC_SKILL_COUNT)
#define FIRST_TRAINING_NAME_ID (FIRST_TECH_SKILL_NAME_ID + TECH_SKILL_COUNT)
#define FIRST_BASIC_SKILL_DESC_ID (FIRST_TRAINING_NAME_ID + TRAINING_COUNT)
#define FIRST_TECH_SKILL_DESC_ID (FIRST_BASIC_SKILL_DESC_ID + BASIC_SKILL_COUNT)

static int sub_4C5F70(int value);

// 0x5B6F04
static int basic_skill_stats[BASIC_SKILL_COUNT] = {
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_PERCEPTION,
    STAT_PERCEPTION,
    STAT_INTELLIGENCE,
    STAT_WILLPOWER,
    STAT_INTELLIGENCE,
    STAT_CHARISMA,
};

// 0x5B6F34
static int tech_skill_stats[TECH_SKILL_COUNT] = {
    STAT_INTELLIGENCE,
    STAT_PERCEPTION,
    STAT_DEXTERITY,
    STAT_PERCEPTION,
};

// 0x5B6F48
static int dword_5B6F48[3] = { 1, 9, 18 };

// 0x5B6F58
static int dword_5B6F58[3] = { 10, 20, 30 };

// 0x5B6F64
static int dword_5B6F64[BASIC_SKILL_COUNT] = {
    0x58F,
    0xE80,
    0x58D,
    0x58F,
    0x880,
    0x998,
    0x298,
    0x884,
    0x8C0,
    0x800,
    0x980,
    0x820,
};

// 0x5B6F94
static int dword_5B6F94[TECH_SKILL_COUNT] = {
    0x980,
    0x58F,
    0x984,
    0x984,
};

// 0x5B6FA4
static int dword_5B6FA4[20] = {
    3,
    3,
    3,
    3,
    3,
    7,
    7,
    7,
    11,
    11,
    11,
    15,
    15,
    15,
    19,
    19,
    19,
    20,
    20,
    20,
};

// 0x5B7044
static int dword_5B7044[19] = {
    55,
    66,
    78,
    91,
    105,
    120,
    136,
    153,
    171,
    190,
    209,
    227,
    244,
    260,
    275,
    289,
    302,
    314,
    325,
};

// 0x5B7090
static int dword_5B7090[21] = {
    0,
    25,
    50,
    75,
    100,
    150,
    200,
    250,
    300,
    400,
    500,
    600,
    700,
    900,
    1100,
    1300,
    1500,
    2000,
    3000,
    4000,
    5000,
};

// 0x5FF424
static char* basic_skill_descriptions[BASIC_SKILL_COUNT];

// 0x5FF454
static char* basic_skill_names[BASIC_SKILL_COUNT];

// 0x5FF484
static mes_file_handle_t skill_mes_file;

// 0x5FF488
static char* tech_skill_descriptions[TECH_SKILL_COUNT];

// 0x5FF498
static char* training_names[TRAINING_COUNT];

// 0x5FF4E8
static char* tech_skill_names[TECH_SKILL_COUNT];

// 0x6876A0
static SkillCallbacks skill_callbacks;

// 0x6876C4
static int dword_6876C4;

// 0x4C5BD0
bool skill_init(GameInitInfo* init_info)
{
    MesFileEntry mes_file_entry;
    int index;

    settings_add(&settings, "follower skills", "1", NULL);

    skill_callbacks.field_0 = NULL;
    skill_callbacks.field_4 = NULL;
    skill_callbacks.field_8 = NULL;
    skill_callbacks.field_C = NULL;
    skill_callbacks.trap_output_func = NULL;
    skill_callbacks.field_14 = NULL;
    skill_callbacks.lock_no_repair = NULL;
    skill_callbacks.lock_pick_output_func = NULL;
    skill_callbacks.no_lock_output_func = NULL;

    if (!mes_load("mes\\skill.mes", &skill_mes_file)) {
        return false;
    }

    for (index = 0; index < BASIC_SKILL_COUNT; index++) {
        mes_file_entry.num = index + FIRST_BASIC_SKILL_NAME_ID;
        sub_4D43A0(skill_mes_file, &mes_file_entry);
        basic_skill_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < TECH_SKILL_COUNT; index++) {
        mes_file_entry.num = index + FIRST_TECH_SKILL_NAME_ID;
        sub_4D43A0(skill_mes_file, &mes_file_entry);
        tech_skill_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < TRAINING_COUNT; index++) {
        mes_file_entry.num = index + FIRST_TRAINING_NAME_ID;
        sub_4D43A0(skill_mes_file, &mes_file_entry);
        training_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < BASIC_SKILL_COUNT; index++) {
        mes_file_entry.num = index + FIRST_BASIC_SKILL_DESC_ID;
        sub_4D43A0(skill_mes_file, &mes_file_entry);
        basic_skill_descriptions[index] = mes_file_entry.str;
    }

    for (index = 0; index < TECH_SKILL_COUNT; index++) {
        mes_file_entry.num = index + FIRST_TECH_SKILL_DESC_ID;
        sub_4D43A0(skill_mes_file, &mes_file_entry);
        tech_skill_descriptions[index] = mes_file_entry.str;
    }

    sub_4C6560();

    return true;
}

// 0x4C5D50
void skill_set_callbacks(SkillCallbacks* callbacks)
{
    skill_callbacks = *callbacks;
}

// 0x4C5DB0
void skill_exit()
{
    message_unload(skill_mes_file);
}

// 0x4C5DC0
bool skill_load(GameLoadInfo* load_info)
{
    if (tig_file_fread(&dword_6876C4, sizeof(dword_6876C4), 1, load_info->stream) != 1) {
        return false;
    }

    return true;
}

// 0x4C5DE0
bool skill_save(TigFile* stream)
{
    if (tig_file_fwrite(&dword_6876C4, sizeof(dword_6876C4), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4C5E00
void skill_set_defaults(object_id_t object_id)
{
    int index;

    for (index = 0; index < BASIC_SKILL_COUNT; index++) {
        obj_arrayfield_int32_set(object_id, OBJ_F_CRITTER_BASIC_SKILL_IDX, index, 0);
    }

    for (index = 0; index < TECH_SKILL_COUNT; index++) {
        obj_arrayfield_int32_set(object_id, OBJ_F_CRITTER_TECH_SKILL_IDX, index, 0);
    }
}

// 0x4C5E50
int sub_4C5E50(int64_t obj, int skill)
{
    if (obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        && skill >= 0
        && skill < BASIC_SKILL_COUNT) {
        return 4 * sub_4C5FA0(obj, skill);
    } else {
        return 0;
    }
}

// 0x4C5EB0
int basic_skill_level(int64_t obj, int skill)
{
    int v1;
    int skill_level;
    int key_stat_level;

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= BASIC_SKILL_COUNT) {
        return 0;
    }

    if (skill == BASIC_SKILL_MELEE && sub_45F730(obj)) {
        v1 = 20;
    } else {
        v1 = sub_4C5E50(obj, skill);
    }

    skill_level = effect_adjust_basic_skill_level(obj, skill, v1);
    if (skill_level < 0) {
        skill_level = 0;
    } else if (skill_level > 20) {
        skill_level = 20;
    }

    key_stat_level = stat_level(obj, basic_skill_get_stat(skill));
    if (skill_level > sub_4C5F70(key_stat_level)) {
        skill_level = sub_4C5F70(key_stat_level);
    }

    return skill_level;
}

// 0x4C5F70
int sub_4C5F70(int value)
{
    if (value < 1) {
        value = 1;
    } else if (value > 20) {
        value = 20;
    }

    return dword_5B6FA4[value];
}

// 0x4C5FA0
int sub_4C5FA0(int64_t obj, int skill)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= BASIC_SKILL_COUNT) {
        return 0;
    }

    return obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, skill) & 63;
}

// 0x4C6000
int sub_4C6000(int64_t obj, int skill, int value)
{
    int key_stat_level;
    int current_value;

    if (value < 0
        || !obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= BASIC_SKILL_COUNT) {
        return 0;
    }

    key_stat_level = stat_level(obj, basic_skill_get_stat(skill));
    current_value = obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, skill);

    if (4 * value > sub_4C5F70(key_stat_level)) {
        return current_value & 63;
    }

    sub_4F0150(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, skill, value | current_value & ~63);

    return value;
}

// 0x4C60C0
int basic_skill_get_training(int64_t obj, int skill)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= BASIC_SKILL_COUNT) {
        return 0;
    }

    if (skill == BASIC_SKILL_MELEE && sub_45F730(obj)) {
        int melee;
        int level;
        int index;

        melee = basic_skill_level(obj, BASIC_SKILL_MELEE);
        level = stat_level(obj, STAT_LEVEL);

        for (index = 0; index < 3; index++) {
            if (melee < dword_5B6F48[index]) {
                break;
            }

            if (level < dword_5B6F58[index]) {
                break;
            }
        }

        return index;
    }

    return (obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, skill) >> 6) & 3;
}

// 0x4C62B0
const char* basic_skill_get_name(int skill)
{
    return basic_skill_names[skill];
}

// 0x4C62C0
const char* basic_skill_get_description(int skill)
{
    return basic_skill_descriptions[skill];
}

// 0x4C62D0
int sub_4C62D0(int a1, int a2, int a3)
{
    return (a3 + 1) * (a2 + 2);
}

// 0x4C6410
int sub_4C6410(int64_t obj, int skill, int64_t other_obj)
{
    int skill_level;
    int difficulty;
    int delta;

    if (other_obj == OBJ_HANDLE_NULL) {
        return 100;
    }

    skill_level = basic_skill_level(obj, skill);

    if ((dword_5B6F64[skill] & 0x10) != 0) {
        difficulty = stat_level(other_obj, STAT_PERCEPTION);
    } else if ((dword_5B6F64[skill] & 0x20) != 0) {
        difficulty = stat_level(other_obj, STAT_INTELLIGENCE);
    } else {
        difficulty = basic_skill_level(other_obj, skill);
    }

    delta = skill_level - difficulty;
    if (delta >= 10) {
        return 100;
    }

    if (delta <= -10) {
        return 0;
    }

    return dword_5B7044[delta + 9] / 4;
}

// 0x4C64B0
int sub_4C64B0(int64_t obj, int skill)
{
    (void)obj;
    (void)skill;

    return 1;
}

// 0x4C64C0
int sub_4C64C0(int64_t obj, int skill)
{
    (void)obj;
    (void)skill;

    return 1;
}

// 0x4C64D0
int basic_skill_get_stat(int skill)
{
    return basic_skill_stats[skill];
}

// 0x4C6510
int sub_4C6510(int64_t obj)
{
    (void)obj;

    return 100;
}

// 0x4C6520
int sub_4C6520(int64_t obj)
{
    int gambling;
    int amount;

    gambling = basic_skill_level(obj, BASIC_SKILL_GAMBLING);
    amount = dword_5B7090[gambling];

    if (basic_skill_get_training(obj, BASIC_SKILL_GAMBLING) >= TRAINING_APPRENTICE) {
        amount *= 2;
    }

    return amount;
}

// 0x4C69A0
const char* tech_skill_get_name(int skill)
{
    return tech_skill_names[skill];
}

// 0x4C69B0
const char* tech_skill_get_description(int skill)
{
    return tech_skill_descriptions[skill];
}

// 0x4C69D0
const char* training_get_name(int training)
{
    return training_names[training];
}

// 0x4C69E0
int sub_4C69E0(int a1, int a2, int a3)
{
    return (a3 + 1) * (a2 + 2);
}

// 0x4C6AF0
int sub_4C6AF0()
{
    return 1;
}

// 0x4C6B00
int sub_4C6B00()
{
    return 1;
}

// 0x4C6B10
int tech_skill_get_stat(int skill)
{
    return tech_skill_stats[skill];
}
