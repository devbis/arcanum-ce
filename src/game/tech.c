#include "game/lib/tech.h"

#include "game/lib/effect.h"
#include "game/lib/message.h"
#include "game/lib/object.h"
#include "game/lib/stat.h"

// TODO: Refactor.
#define SEVENTEEN 17

// 0x5B5124
int dword_5B5124[8] = {
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
};

// 0x5B5144
int dword_5B5144[] = {
    0,
    10,
    20,
    35,
    50,
    65,
    80,
    100,
};

static_assert(sizeof(dword_5B5144) / sizeof(dword_5B5144[0]) == DEGREE_COUNT, "wrong size");

// 0x5B5164
int tech_min_intelligence[] = {
    0,
    5,
    8,
    11,
    13,
    15,
    17,
    19,
};

static_assert(sizeof(tech_min_intelligence) / sizeof(tech_min_intelligence[0]) == DEGREE_COUNT, "wrong size");

// 0x5F84A8
static char* degree_names[DEGREE_COUNT];

// 0x5F84C8
static char* degree_descriptions[TECH_COUNT][DEGREE_COUNT];

// 0x5F85C8
static int tech_msg_file;

// 0x5F85CC
static char* tech_descriptions[TECH_COUNT];

// 0x5F85EC
static char* tech_names[TECH_COUNT];

// 0x4AFCD0
bool tech_init(GameContext* ctx)
{
    MessageListItem message_list_item;

    if (!message_load("mes\\tech.mes", &tech_msg_file)) {
        return false;
    }

    for (int tech = 0; tech < TECH_COUNT; tech++) {
        message_list_item.num = tech;
        sub_4D43A0(tech_msg_file, &message_list_item);
        tech_names[tech] = message_list_item.text;
    }

    for (int degree = 0; degree < DEGREE_COUNT; degree++) {
        message_list_item.num = degree + 8;
        sub_4D43A0(tech_msg_file, &message_list_item);
        degree_names[degree] = message_list_item.text;
    }

    for (int tech = 0; tech < TECH_COUNT; tech++) {
        message_list_item.num = tech + 16;
        sub_4D43A0(tech_msg_file, &message_list_item);
        tech_descriptions[tech] = message_list_item.text;
    }

    // NOTE: Original code is slightly different. It does not use nested loop,
    // instead it simply runs 64 iterations, implying `degree_descriptions`
    // is one-dimensional array. Making it two-dimensional slightly increase
    // code readability.
    for (int tech = 0; tech < TECH_COUNT; tech++) {
        for (int degree = 0; degree < DEGREE_COUNT; degree++) {
            message_list_item.num = tech * DEGREE_COUNT + degree + 24;
            sub_4D43A0(tech_msg_file, &message_list_item);
            degree_descriptions[tech][degree] = message_list_item.text;
        }
    }

    return true;
}

// 0x4AFDD0
void tech_exit()
{
    message_unload(tech_msg_file);
}

// 0x4AFDE0
void tech_set_defaults(object_id_t obj)
{
    // TODO: Use enum.
    for (int index = 17; index < 25; index++) {
        sub_4074E0(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, index, 0);
    }
}

// 0x4AFE10
const char* tech_get_name(int tech)
{
    return tech_names[tech];
}

// 0x4AFE20
const char* tech_get_description(int tech)
{
    return tech_descriptions[tech];
}

// 0x4AFE30
const char* degree_get_name(int degree)
{
    return degree_names[degree];
}

// 0x4AFE40
const char* degree_get_description(int degree, int tech)
{
    return degree_descriptions[tech][degree];
}

// 0x4AFE60
int sub_4AFE60(long long object_id, int tech)
{
    if (obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_PC || obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        return sub_407470(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, tech + SEVENTEEN);
    } else {
        return 0;
    }
}

// 0x4AFF90
int sub_4AFF90(long long object_id, int tech, int value)
{
    if (obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_PC || obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        sub_4F0270(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, tech + SEVENTEEN, value);
    } else {
        return 0;
    }
}

// 0x4B00A0
int sub_4B00A0(int a1)
{
    return dword_5B5124[a1];
}

// 0x4B00B0
int sub_4B00B0(object_id_t obj, int tech)
{
    int degree = sub_4AFE60(obj, tech);

    int intelligence = stat_level(obj, STAT_INTELLIGENCE);
    while (intelligence < tech_get_min_intelligence_for_degree(degree)) {
        degree--;
    }

    return effect_adjust_tech_level(obj, tech, dword_5B5144[degree]);
}

// 0x4B0110
int tech_get_min_intelligence_for_degree(int degree)
{
    return tech_min_intelligence[degree];
}

// 0x4B02B0
bool sub_4B02B0(object_id_t obj, int intelligence)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return true;
    }

    for (int tech = 0; tech < TECH_COUNT; tech++) {
        int degree = sub_4AFE60(obj, tech);
        if (tech_get_min_intelligence_for_degree(degreee) > intelligence) {
            return false;
        }
    }

    return true;
}
