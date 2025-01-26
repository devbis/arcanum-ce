#include "game/tech.h"

#include "game/effect.h"
#include "game/gamelib.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/object.h"
#include "game/player.h"
#include "game/stat.h"
#include "game/ui.h"

// TODO: Refactor.
#define SEVENTEEN 17

static int tech_degree_set(int64_t obj, int tech, int value);

// 0x5B5124
int dword_5B5124[DEGREE_COUNT] = {
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
int dword_5B5144[DEGREE_COUNT] = {
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
int tech_min_intelligence[DEGREE_COUNT] = {
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
static char* tech_degree_names[DEGREE_COUNT];

// 0x5F84C8
static char* tech_degree_descriptions[TECH_COUNT][DEGREE_COUNT];

// 0x5F85C8
static mes_file_handle_t tech_mes_file;

// 0x5F85CC
static char* tech_discipline_descriptions[TECH_COUNT];

// 0x5F85EC
static char* tech_discipline_names[TECH_COUNT];

// 0x4AFCD0
bool tech_init(GameInitInfo* init_info)
{
    MesFileEntry mes_file_entry;
    int tech;
    int degree;

    (void)init_info;

    if (!mes_load("mes\\tech.mes", &tech_mes_file)) {
        return false;
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        mes_file_entry.num = tech;
        mes_get_msg(tech_mes_file, &mes_file_entry);
        tech_discipline_names[tech] = mes_file_entry.str;
    }

    for (degree = 0; degree < DEGREE_COUNT; degree++) {
        mes_file_entry.num = degree + TECH_COUNT;
        mes_get_msg(tech_mes_file, &mes_file_entry);
        tech_degree_names[degree] = mes_file_entry.str;
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        mes_file_entry.num = tech + TECH_COUNT + DEGREE_COUNT;
        mes_get_msg(tech_mes_file, &mes_file_entry);
        tech_discipline_descriptions[tech] = mes_file_entry.str;
    }

    // NOTE: Original code is slightly different. It does not use nested loop,
    // instead it simply runs 64 iterations, implying `tech_degree_descriptions`
    // is one-dimensional array. Making it two-dimensional slightly increase
    // code readability.
    for (tech = 0; tech < TECH_COUNT; tech++) {
        for (degree = 0; degree < DEGREE_COUNT; degree++) {
            mes_file_entry.num = tech * DEGREE_COUNT + degree + 24;
            mes_get_msg(tech_mes_file, &mes_file_entry);
            tech_degree_descriptions[tech][degree] = mes_file_entry.str;
        }
    }

    return true;
}

// 0x4AFDD0
void tech_exit()
{
    mes_unload(tech_mes_file);
}

// 0x4AFDE0
void tech_set_defaults(int64_t obj)
{
    int tech;

    for (tech = 0; tech < TECH_COUNT; tech++) {
        obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, tech + SEVENTEEN, 0);
    }
}

// 0x4AFE10
char* tech_discipline_name_get(int tech)
{
    return tech_discipline_names[tech];
}

// 0x4AFE20
char* tech_discipline_description_get(int tech)
{
    return tech_discipline_descriptions[tech];
}

// 0x4AFE30
char* tech_degree_name_get(int degree)
{
    return tech_degree_names[degree];
}

// 0x4AFE40
char* tech_degree_description_get(int degree, int tech)
{
    return tech_degree_descriptions[tech][degree];
}

// 0x4AFE60
int tech_degree_get(int64_t obj, int tech)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    return obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, tech + SEVENTEEN);
}

// 0x4AFEC0
int tech_degree_inc(int64_t obj, int tech)
{
    int degree;
    int cost;
    int tech_points;

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    degree = tech_degree_get(obj, tech);
    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0
        && !multiplayer_is_locked()) {
        return degree;
    }

    if (degree + 1 >= DEGREE_COUNT) {
        return degree;
    }

    if (tech_degree_min_intelligence_get(degree + 1) > stat_level_get(obj, STAT_INTELLIGENCE)) {
        return degree;
    }

    cost = tech_degree_cost_get(degree + 1);
    tech_points = stat_base_get(obj, STAT_TECH_POINTS);
    stat_base_set(obj, STAT_TECH_POINTS, tech_points + cost);

    return tech_degree_set(obj, tech, degree + 1);
}

// 0x4AFF90
int tech_degree_set(int64_t obj, int tech, int value)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    mp_obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, tech + SEVENTEEN, value);

    return value;
}

// 0x4AFFF0
int tech_degree_dec(int64_t obj, int tech)
{
    int degree;
    int cost;
    int tech_points;

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    degree = tech_degree_get(obj, tech);
    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0
        && !multiplayer_is_locked()) {
            return degree;
    }

    if (degree <= 0) {
        return degree;
    }

    cost = tech_degree_cost_get(degree);
    tech_points = stat_base_get(obj, STAT_TECH_POINTS);
    stat_base_set(obj, STAT_TECH_POINTS, tech_points - cost);

    return tech_degree_set(obj, tech, degree - 1);
}

// 0x4B00A0
int tech_degree_cost_get(int degree)
{
    return dword_5B5124[degree];
}

// 0x4B00B0
int tech_degree_level_get(int64_t obj, int tech)
{
    int degree;
    int intelligence;

    degree = tech_degree_get(obj, tech);
    intelligence = stat_level_get(obj, STAT_INTELLIGENCE);
    while (intelligence < tech_degree_min_intelligence_get(degree)) {
        degree--;
    }

    return effect_adjust_tech_level(obj, tech, dword_5B5144[degree]);
}

// 0x4B0110
int tech_degree_min_intelligence_get(int degree)
{
    return tech_min_intelligence[degree];
}

// 0x4B0120
void tech_learn_schematic(int64_t pc_obj, int64_t written_obj)
{
    int schematic;
    int cnt;
    int index;
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
    int player;

    schematic = obj_field_int32_get(written_obj, OBJ_F_WRITTEN_TEXT_START_LINE);

    cnt = obj_arrayfield_length_get(pc_obj, OBJ_F_PC_SCHEMATICS_FOUND_IDX);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_uint32_get(pc_obj, OBJ_F_PC_SCHEMATICS_FOUND_IDX, index) == schematic) {
            break;
        }
    }

    if (index >= cnt) {
        mp_obj_arrayfield_uint32_set(pc_obj, OBJ_F_PC_SCHEMATICS_FOUND_IDX, index, schematic);
        object_destroy(written_obj);

        mes_file_entry.num = 89; // "You have learned a new schematic!"
        mes_get_msg(tech_mes_file, &mes_file_entry);

        ui_message.type = UI_MSG_TYPE_SCHEMATIC;
        ui_message.str = mes_file_entry.str;
        ui_message.field_8 = schematic;

        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            player = sub_4A2B10(pc_obj);
            if (player == -1) {
                return;
            }

            if (player != 0) {
                sub_4EDA60(&ui_message, player, 0);
                return;
            }
        }
    } else {
        mes_file_entry.num = 88; // "You already know this schematic."
        mes_get_msg(tech_mes_file, &mes_file_entry);

        ui_message.type = UI_MSG_TYPE_SCHEMATIC;
        ui_message.str = mes_file_entry.str;
        ui_message.field_8 = schematic;

        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
            player = sub_4A2B10(pc_obj);
            if (player == -1) {
                return;
            }

            if (player != 0) {
                sub_4EDA60(&ui_message, player, 0);
                return;
            }
        }
    }

    sub_460630(&ui_message);
}

// 0x4B02B0
bool tech_check_intelligence(int64_t obj, int intelligence)
{
    int tech;
    int degree;

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return true;
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        degree = tech_degree_get(obj, tech);
        if (tech_degree_min_intelligence_get(degree) > intelligence) {
            return false;
        }
    }

    return true;
}

// 0x4B0320
int tech_schematic_base_lineno(int tech, int schematic)
{
    return 10 * schematic + 200 * tech + 1990;
}
