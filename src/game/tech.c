#include "game/tech.h"

#include "game/effect.h"
#include "game/mes.h"
#include "game/object.h"
#include "game/stat.h"

// TODO: Refactor.
#define SEVENTEEN 17

static int tech_set_degree(long long object_id, int tech, int value);

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
static char* degree_names[DEGREE_COUNT];

// 0x5F84C8
static char* degree_descriptions[TECH_COUNT][DEGREE_COUNT];

// 0x5F85C8
static mes_file_handle_t tech_msg_file;

// 0x5F85CC
static char* tech_descriptions[TECH_COUNT];

// 0x5F85EC
static char* tech_names[TECH_COUNT];

// 0x4AFCD0
bool tech_init(GameInitInfo* init_info)
{
    MesFileEntry mes_file_entry;
    int tech;
    int degree;

    (void)init_info;

    if (!mes_load("mes\\tech.mes", &tech_msg_file)) {
        return false;
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        mes_file_entry.num = tech;
        mes_get_msg(tech_msg_file, &mes_file_entry);
        tech_names[tech] = mes_file_entry.str;
    }

    for (degree = 0; degree < DEGREE_COUNT; degree++) {
        mes_file_entry.num = degree + 8;
        mes_get_msg(tech_msg_file, &mes_file_entry);
        degree_names[degree] = mes_file_entry.str;
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        mes_file_entry.num = tech + 16;
        mes_get_msg(tech_msg_file, &mes_file_entry);
        tech_descriptions[tech] = mes_file_entry.str;
    }

    // NOTE: Original code is slightly different. It does not use nested loop,
    // instead it simply runs 64 iterations, implying `degree_descriptions`
    // is one-dimensional array. Making it two-dimensional slightly increase
    // code readability.
    for (tech = 0; tech < TECH_COUNT; tech++) {
        for (degree = 0; degree < DEGREE_COUNT; degree++) {
            mes_file_entry.num = tech * DEGREE_COUNT + degree + 24;
            mes_get_msg(tech_msg_file, &mes_file_entry);
            degree_descriptions[tech][degree] = mes_file_entry.str;
        }
    }

    return true;
}

// 0x4AFDD0
void tech_exit()
{
    mes_unload(tech_msg_file);
}

// 0x4AFDE0
void tech_set_defaults(object_id_t obj)
{
    int index;

    // TODO: Use enum.
    for (index = 17; index < 25; index++) {
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
int tech_get_degree(long long object_id, int tech)
{
    if (obj_field_int32_get(object_id, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(object_id, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    return sub_407470(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, tech + SEVENTEEN);
}

// 0x4AFEC0
int tech_inc_degree(int64_t obj, int tech)
{
    int degree;
    int cost;
    int tech_points;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    degree = tech_get_degree(obj, tech);
    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0
        && !sub_4A2BA0()) {
        return degree;
    }

    if (degree + 1 >= DEGREE_COUNT) {
        return degree;
    }

    if (tech_get_min_intelligence_for_degree(degree + 1) > stat_level(obj, STAT_INTELLIGENCE)) {
        return degree;
    }

    cost = tech_get_cost_for_degree(degree);
    tech_points = stat_get_base(obj, STAT_TECH_POINTS);
    stat_set_base(obj, STAT_TECH_POINTS, tech_points + cost);

    return tech_set_degree(obj, tech, degree + 1);
}

// 0x4AFF90
int tech_set_degree(long long object_id, int tech, int value)
{
    if (obj_field_int32_get(object_id, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(object_id, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    return sub_4F0270(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, tech + SEVENTEEN, value);
}

// 0x4AFFF0
int tech_dec_degree(int64_t obj, int tech)
{
    int degree;
    int cost;
    int tech_points;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    degree = tech_get_degree(obj, tech);
    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0
        && !sub_4A2BA0()) {
            return degree;
    }

    if (degree <= 0) {
        return degree;
    }

    cost = tech_get_cost_for_degree(degree);
    tech_points = stat_get_base(obj, STAT_TECH_POINTS);
    stat_set_base(obj, STAT_TECH_POINTS, tech_points - cost);

    return tech_set_degree(obj, tech, degree - 1);
}

// 0x4B00A0
int tech_get_cost_for_degree(int degree)
{
    return dword_5B5124[degree];
}

// 0x4B00B0
int sub_4B00B0(object_id_t obj, int tech)
{
    int degree;
    int intelligence;

    degree = tech_get_degree(obj, tech);
    intelligence = stat_level(obj, STAT_INTELLIGENCE);
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
    int tech;
    int degree;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return true;
    }

    for (tech = 0; tech < TECH_COUNT; tech++) {
        degree = tech_get_degree(obj, tech);
        if (tech_get_min_intelligence_for_degree(degree) > intelligence) {
            return false;
        }
    }

    return true;
}

// 0x4B0320
int sub_4B0320(int a1, int a2)
{
    return 10 * (a2 + 20 * a1 + 199);
}

// 0x4C8FA0
bool get_follower_skills(int64_t obj)
{
    int player;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0) {
        return settings_get_value(&settings, "follower skills");
    }

    player = sub_4A2B10(obj);
    if (player == -1) {
        return false;
    }

    return (sub_4A55D0(player) & 0x800) != 0;
}

// 0x4C8FF0
void set_follower_skills(bool enabled)
{
    int player;

    settings_set_value(&settings, "follower skills", enabled);

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        player = sub_4A2B10(player_get_pc_obj());
        if (player != -1) {
            if (enabled) {
                sub_4A5510(player, 0x800);
            } else {
                sub_4A5570(player, 0x800);
            }
        }
    }
}
