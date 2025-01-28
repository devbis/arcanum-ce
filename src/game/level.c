#include "game/level.h"

#include <stdio.h>

#include "game/background.h"
#include "game/critter.h"
#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/skill.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/tech.h"
#include "game/ui.h"

#define LEVEL_MAX 51
#define TEN 10

#define STANDARD_PC_SCHEME 70

#define LEVEL_SCHEME_STATS_MAX 8
#define LEVEL_SCHEME_BASIC_SKILLS_MAX BASIC_SKILL_COUNT
#define LEVEL_SCHEME_TECH_SKILLS_MAX TECH_SKILL_COUNT
#define LEVEL_SCHEME_SPELLS_MAX COLLEGE_COUNT
#define LEVEL_SCHEME_TECH_MAX TECH_COUNT
#define LEVEL_SCHEME_MISC_MAX 2

typedef int(MiscGetter)(int64_t obj);
typedef int(MiscSetter)(int64_t obj, int value);

static int sub_4A6980(int old_level, int new_level);
static void sub_4A6CB0(int64_t obj, int a3, int a4);
static bool sub_4A7030(int64_t obj, char* str);
static bool sub_4A7340(int64_t obj, const char* str);
static int sub_4A75E0(int64_t obj, int stat, int value);
static int sub_4A76B0(int64_t obj, int skill, int value);
static int sub_4A77A0(int64_t obj, int skill, int score);
static int sub_4A7890(int64_t obj, int college, int score);
static int sub_4A79C0(int64_t obj, int tech, int degree);
static int sub_4A7AA0(int64_t obj, int type, int score);
static void sub_4A7B90(const char* str, int score);

// 0x5B4CE8
static const char* level_scheme_stats[LEVEL_SCHEME_STATS_MAX] = {
    "st",
    "dx",
    "cn",
    "be",
    "in",
    "pe",
    "wp",
    "ch",
};

// 0x5B4D08
static const char* level_scheme_basic_skills[LEVEL_SCHEME_BASIC_SKILLS_MAX] = {
    "bow",
    "dodge",
    "melee",
    "throwing",
    "backstab",
    "pickpocket",
    "prowling",
    "spottrap",
    "gambling",
    "haggle",
    "heal",
    "persuasion",
};

// 0x5B4D38
static const char* level_scheme_tech_skills[LEVEL_SCHEME_TECH_SKILLS_MAX] = {
    "repair",
    "firearms",
    "picklock",
    "armtrap",
};

// 0x5B4D48
static const char* level_scheme_spells[LEVEL_SCHEME_SPELLS_MAX] = {
    "conveyance",
    "divination",
    "air",
    "earth",
    "fire",
    "water",
    "force",
    "mental",
    "meta",
    "morph",
    "nature",
    "necro_evil",
    "necro_good",
    "phantasm",
    "summoning",
    "temporal",
};

// 0x5B4D88
static const char* level_scheme_tech[LEVEL_SCHEME_TECH_MAX] = {
    "anatomical",
    "chemistry",
    "electric",
    "explosives",
    "gun_smithy",
    "mechanical",
    "smithy",
    "therapeutics",
};

// 0x5B4DA8
static const char* level_scheme_misc[LEVEL_SCHEME_MISC_MAX] = {
    "maxhps",
    "maxfatigue",
};

// 0x4A7ABE
static MiscGetter* off_4A7ABE[LEVEL_SCHEME_MISC_MAX] = {
    object_hp_max,
    critter_fatigue_max,
};

// 0x4A7AF3
static MiscGetter* off_4A7AF3[LEVEL_SCHEME_MISC_MAX] = {
    object_hp_pts_get,
    critter_fatigue_pts_get,
};

// 0x4A7AFE
static MiscSetter* off_4A7AFE[LEVEL_SCHEME_MISC_MAX] = {
    object_hp_pts_set ,
    critter_fatigue_pts_set,
};

// 0x5F0E60
static char byte_5F0E60[10][2000];

// 0x5F5C80
static int* dword_5F5C80;

// 0x5F5C84
static mes_file_handle_t game_level_name_mes_file;

// 0x5F5C88
static mes_file_handle_t user_level_name_mes_file;

// 0x5F5C8C
static mes_file_handle_t game_level_mes_file;

// 0x5F5C90
static mes_file_handle_t user_level_mes_file;

// 0x5F5C94
static int* dword_5F5C94;

// 0x5F5C98
static mes_file_handle_t level_mes_file;

// 0x5F5C9C
static int dword_5F5C9C;

// 0x4A6620
bool level_init(GameInitInfo* init_info)
{
    mes_file_handle_t xp_level_mes_file;
    MesFileEntry msg;
    int index;

    (void)init_info;

    if (!mes_load("rules\\xp_level.mes", &xp_level_mes_file)) {
        return false;
    }

    dword_5F5C94 = (int*)CALLOC(LEVEL_MAX, sizeof(*dword_5F5C94));
    dword_5F5C80 = CALLOC(TEN, sizeof(*dword_5F5C80));

    for (index = 0; index < LEVEL_MAX; index++) {
        msg.num = index + 1;
        if (mes_search(xp_level_mes_file, &msg)) {
            dword_5F5C94[index] = atoi(msg.str);
        } else {
            dword_5F5C94[index] = 0;
        }
    }

    mes_unload(xp_level_mes_file);

    dword_5F5C94[0] = 0;

    if (!mes_load("rules\\gamelevel.mes", &game_level_mes_file)) {
        FREE(dword_5F5C94);
        FREE(dword_5F5C80);
        return false;
    }

    if (!mes_load("rules\\userlevel.mes", &user_level_mes_file)) {
        mes_unload(game_level_mes_file);
        FREE(dword_5F5C94);
        FREE(dword_5F5C80);
        return false;
    }

    if (!mes_load("mes\\gamelevelname.mes", &game_level_name_mes_file)) {
        mes_unload(user_level_mes_file);
        mes_unload(game_level_mes_file);
        FREE(dword_5F5C94);
        FREE(dword_5F5C80);
        return false;
    }

    if (!mes_load("mes\\userlevelname.mes", &user_level_name_mes_file)) {
        mes_unload(game_level_name_mes_file);
        mes_unload(user_level_mes_file);
        mes_unload(game_level_mes_file);
        FREE(dword_5F5C94);
        FREE(dword_5F5C80);
        return false;
    }

    if (!mes_load("mes\\level.mes", &level_mes_file)) {
        mes_unload(user_level_name_mes_file);
        mes_unload(game_level_name_mes_file);
        mes_unload(user_level_mes_file);
        mes_unload(game_level_mes_file);
        FREE(dword_5F5C94);
        FREE(dword_5F5C80);
        return false;
    }

    return true;
}

// 0x4A6850
void level_exit()
{
    mes_unload(game_level_mes_file);
    mes_unload(user_level_mes_file);
    mes_unload(game_level_name_mes_file);
    mes_unload(user_level_name_mes_file);
    mes_unload(level_mes_file);
    FREE(dword_5F5C94);
    FREE(dword_5F5C80);
}

// 0x4A68B0
int level_get_experience_points_to_next_level(int64_t obj)
{
    int level;

    level = stat_level_get(obj, STAT_LEVEL);
    if (level >= LEVEL_MAX) {
        return 999999;
    }

    return dword_5F5C94[level] - stat_level_get(obj, STAT_EXPERIENCE_POINTS);
}

// 0x4A6900
int level_get_experience_points_to_next_level_in_percent(int64_t obj)
{
    int level;
    int xp;

    level = stat_level_get(obj, STAT_LEVEL);
    if (level >= LEVEL_MAX) {
        return 0;
    }

    if (level != 0 && dword_5F5C94[level] != 0) {
        xp = level_get_experience_points_to_next_level(obj);
        if (xp >= 0) {
            return 1000 - 1000 * xp / (dword_5F5C94[level] - dword_5F5C94[level - 1]);
        }
    }

    return 999;
}

// Returns number of bonus points when critter advancing from `old_level` to
// `new_level`.
//
// 0x4A6980
int sub_4A6980(int old_level, int new_level)
{
    int points = 0;
    int level;

    for (level = old_level + 1; level <= new_level; level++) {
        points++;
        if ((level % 5) == 0) {
            points++;
        }
    }

    return points;
}

// 0x4A69C0
void sub_4A69C0(int64_t pc_obj)
{
    UiMessage ui_message;
    int old_level;
    int cur_level;
    int max_level;
    int exp_to_next_level;
    ObjectList objects;
    ObjectNode* node;
    char str[MAX_STRING];
    int unspent_points;
    MesFileEntry mes_file_entry;

    if (obj_field_int32_get(pc_obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    cur_level = stat_base_get(pc_obj, STAT_LEVEL);
    max_level = stat_level_max(pc_obj, STAT_LEVEL);
    if (cur_level >= max_level) {
        return;
    }

    exp_to_next_level = level_get_experience_points_to_next_level(pc_obj);
    if (exp_to_next_level > 0) {
        return;
    }

    ui_message.type = UI_MSG_TYPE_LEVEL;
    ui_message.str = str;

    old_level = cur_level;

    do {
        cur_level++;
        if (cur_level == max_level) {
            stat_base_set(pc_obj, STAT_EXPERIENCE_POINTS, dword_5F5C94[cur_level - 1]);
        } else {
            stat_base_set(pc_obj, STAT_LEVEL, cur_level);

            unspent_points = stat_base_get(pc_obj, STAT_UNSPENT_POINTS);
            unspent_points += sub_4A6980(cur_level - 1, cur_level);
            stat_base_set(pc_obj, STAT_UNSPENT_POINTS, unspent_points);

            ui_message.field_8 = cur_level;

            if (level_auto_level_scheme_get(pc_obj) != 0) {
                ui_message.field_C = -1;
                if (sub_4A7030(pc_obj, str)) {
                    mes_file_entry.num = 1;
                    mes_get_msg(level_mes_file, &mes_file_entry);
                    strcat(str, " ");
                    strcat(str, mes_file_entry.str);
                }
            } else {
                ui_message.field_C = unspent_points;
                str[0] = '\0';
            }

            if (player_is_pc_obj(pc_obj)) {
                sub_460630(&ui_message);
                sub_460790(0, 1);
                sub_460240(pc_obj);
                sub_460260(pc_obj);
            }

            exp_to_next_level = level_get_experience_points_to_next_level(pc_obj);
        }
    } while (cur_level < max_level && exp_to_next_level <= 0);

    sub_441260(pc_obj, &objects);
    node = objects.head;
    while (node != NULL) {
        sub_4A6CB0(node->obj, old_level, cur_level);
        node = node->next;
    }
    object_list_destroy(&objects);

    sub_440FC0(pc_obj, OBJ_TM_NPC, &objects);
    node = objects.head;
    while (node != NULL) {
        if (!critter_is_dead(node->obj)
            && (obj_field_int32_get(node->obj, OBJ_F_NPC_FLAGS) & ONF_AI_WAIT_HERE) != 0
            && critter_pc_leader_get(node->obj) == pc_obj) {
            sub_4A6CB0(node->obj, old_level, cur_level);
        }
        node = node->next;
    }
    object_list_destroy(&objects);

    sub_4C2950(pc_obj);
}

// 0x4A6CB0
void sub_4A6CB0(int64_t obj, int a3, int a4)
{
    int curr_level;
    int max_level;
    int new_level;
    int bonus_points;
    int curr_points;

    curr_level = stat_base_get(obj, STAT_LEVEL);
    max_level = stat_level_max(obj, STAT_LEVEL);
    if (curr_level < max_level) {
        if (curr_level <= a3) {
            new_level = a4 + curr_level - a3;
        } else {
            new_level = a4;
        }

        if (new_level >= max_level) {
            new_level = max_level - 1;
        }

        if (new_level > curr_level) {
            stat_base_set(obj, STAT_LEVEL, new_level);

            bonus_points = sub_4A6980(curr_level, new_level);
            curr_points = stat_base_get(obj, STAT_UNSPENT_POINTS);
            stat_base_set(obj, STAT_UNSPENT_POINTS, bonus_points + curr_points);
            sub_4A7030(obj, NULL);
        }
    }
}

// 0x4A6D40
int level_auto_level_scheme_get(int64_t obj)
{
    int type;

    if (obj == OBJ_HANDLE_NULL) {
        return -1;
    }

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC && type != OBJ_TYPE_NPC) {
        return -1;
    }

    return obj_field_int32_get(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME);
}

// 0x4A6D90
int level_auto_level_scheme_set(int64_t obj, int value)
{
    int type;

    if (obj == OBJ_HANDLE_NULL) {
        return -1;
    }

    type = obj_field_int32_get(obj, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC && type != OBJ_TYPE_NPC) {
        return -1;
    }

    obj_field_int32_set(obj, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, value);

    return value;
}

// 0x4A6DE0
const char* level_advancement_scheme_get_name(int scheme)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    if (scheme == 0) {
        mes_file = game_level_name_mes_file;
    } else if (scheme >= 1 && scheme < 200) {
        if (scheme < 50) {
            mes_file = user_level_name_mes_file;
        } else {
            mes_file = game_level_name_mes_file;
        }
    } else {
        return NULL;
    }

    mes_file_entry.num = scheme;
    if (!mes_search(mes_file, &mes_file_entry)) {
        return NULL;
    }

    return mes_file_entry.str;
}

// 0x4A6E40
const char* level_advancement_scheme_get_rule(int scheme)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    if (scheme == 0 || scheme < 1 || scheme >= 200) {
        return NULL;
    }

    if (scheme < 50) {
        mes_file = user_level_mes_file;
    } else {
        mes_file = game_level_mes_file;
    }

    mes_file_entry.num = scheme;
    if (!mes_search(mes_file, &mes_file_entry)) {
        return NULL;
    }

    return mes_file_entry.str;
}

// 0x4A6F00
void level_set_level(int64_t obj, int level)
{
    int type;
    int gender;
    int race;
    int alignment;
    int age;
    int unspent_points;
    int bonus;

    // NOTE: Unused.
    type = obj_field_int32_get(obj, OBJ_F_TYPE);

    if (level < 0) {
        level = 0;
    } else if (level > LEVEL_MAX) {
        level = LEVEL_MAX;
    }

    gender = stat_base_get(obj, STAT_GENDER);
    race = stat_base_get(obj, STAT_RACE);
    alignment = stat_base_get(obj, STAT_ALIGNMENT);
    age = stat_base_get(obj, STAT_AGE);

    stat_set_defaults(obj);
    skill_set_defaults(obj);
    spell_set_defaults(obj);
    tech_set_defaults(obj);

    stat_base_set(obj, STAT_GENDER, gender);
    stat_base_set(obj, STAT_RACE, race);
    stat_base_set(obj, STAT_ALIGNMENT, alignment);
    stat_base_set(obj, STAT_AGE, age);
    stat_base_set(obj, STAT_LEVEL, level);

    object_hp_pts_set(obj, 0);
    critter_fatigue_pts_set(obj, 0);

    if (level >= 1) {
        stat_base_set(obj, STAT_EXPERIENCE_POINTS, dword_5F5C94[level - 1]);

        unspent_points = stat_base_get(obj, STAT_UNSPENT_POINTS);
        bonus = sub_4A6980(1, level);
        stat_base_set(obj, STAT_UNSPENT_POINTS, unspent_points + bonus);
    }

    sub_4A7030(obj, NULL);
    object_hp_damage_set(obj, 0);
}

// 0x4A7030
bool sub_4A7030(int64_t obj, char* str)
{
    int scheme;
    MesFileEntry mes_file_entry;
    char buffer[2000];
    const char* rule;
    int index;
    bool ret;

    scheme = level_auto_level_scheme_get(obj);
    if (scheme == 0) {
        return false;
    }

    if (str != NULL) {
        mes_file_entry.num = 2;
        mes_get_msg(level_mes_file, &mes_file_entry);
        strcpy(str, mes_file_entry.str);
        dword_5F5C9C = 0;
    } else {
        dword_5F5C9C = -1;
    }

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        && (obj_field_int32_get(obj, OBJ_F_PC_FLAGS) & OPCF_USE_ALT_DATA) != 0) {
        if (!sub_4A47D0(obj, buffer)) {
            return false;
        }
        rule = buffer;
    } else {
        rule = level_advancement_scheme_get_rule(scheme);
    }

    if (rule == NULL) {
        if (str != NULL) {
            mes_file_entry.num = 4;
            mes_get_msg(level_mes_file, &mes_file_entry);
            strcat(str, mes_file_entry.str);
        }
        return false;
    }

    ret = sub_4A7340(obj, rule);
    if (ret) {
        rule = level_advancement_scheme_get_rule(STANDARD_PC_SCHEME);
        if (rule != NULL) {
            ret = sub_4A7340(obj, rule);
        }
    }

    if (dword_5F5C9C == 0) {
        // FIXME: No `str != NULL` check as seen above.
        mes_file_entry.num = 3;
        mes_get_msg(level_mes_file, &mes_file_entry);
        strcat(str, mes_file_entry.str);
    } else if (dword_5F5C9C > 0) {
        for (index = 0; index < dword_5F5C9C; index++) {
            strcat(str, byte_5F0E60[index]);
            if (dword_5F5C80[index] > 0) {
                sprintf(&(str[strlen(str)]), " %d", dword_5F5C80[index]);
            } else if (dword_5F5C80[index] < 0) {
                // Special case - negative value indicates degree level.
                strcat(str, " ");
                strcat(str, tech_degree_name_get(-dword_5F5C80[index]));
            }
            strcat(str, ", ");
        }

        // Replace final comma with dot.
        str[strlen(str) - 2] = '.';
    }

    return ret;
}

// 0x4A7340
bool sub_4A7340(int64_t obj, const char* str)
{
    char buffer[2000];
    char* tok;
    int score;
    int index;

    if (tig_net_is_active()
        && !tig_net_is_host()) {
        return false;
    }

    strcpy(buffer, str);

    for (tok = strtok(buffer, " "); tok != NULL; tok = strtok(NULL, " ")) {
        score = atoi(strtok(NULL, ","));

        for (index = 0; index < LEVEL_SCHEME_STATS_MAX; index++) {
            if (strcmpi(tok, level_scheme_stats[index]) == 0) {
                if (sub_4A75E0(obj, index, score) == 1) {
                    return false;
                }
                break;
            }
        }
        if (index < LEVEL_SCHEME_STATS_MAX) continue;

        for (index = 0; index < LEVEL_SCHEME_BASIC_SKILLS_MAX; index++) {
            if (strcmpi(tok, level_scheme_basic_skills[index]) == 0) {
                if (sub_4A76B0(obj, index, score) == 1) {
                    return false;
                }
                break;
            }
        }
        if (index < LEVEL_SCHEME_BASIC_SKILLS_MAX) continue;

        for (index = 0; index < LEVEL_SCHEME_TECH_SKILLS_MAX; index++) {
            if (strcmpi(tok, level_scheme_tech_skills[index]) == 0) {
                if (sub_4A77A0(obj, index, score) == 1) {
                    return false;
                }
                break;
            }
        }
        if (index < LEVEL_SCHEME_TECH_SKILLS_MAX) continue;

        for (index = 0; index < LEVEL_SCHEME_SPELLS_MAX; index++) {
            if (strcmpi(tok, level_scheme_spells[index]) == 0) {
                if (sub_4A7890(obj, index, score) == 1) {
                    return false;
                }
                break;
            }
        }
        if (index < LEVEL_SCHEME_SPELLS_MAX) continue;

        for (index = 0; index < LEVEL_SCHEME_TECH_MAX; index++) {
            if (strcmpi(tok, level_scheme_tech[index]) == 0) {
                if (sub_4A79C0(obj, index, score) == 1) {
                    return false;
                }
                break;
            }
        }
        if (index < LEVEL_SCHEME_TECH_MAX) continue;

        for (index = 0; index < LEVEL_SCHEME_MISC_MAX; index++) {
            if (strcmpi(tok, level_scheme_misc[index]) == 0) {
                if (sub_4A7AA0(obj, index, score) == 1) {
                    return false;
                }
                break;
            }
        }
        if (index < LEVEL_SCHEME_MISC_MAX) continue;

        tig_debug_printf("Error processing auto level scheme, cannot find match for: %s\n", tok);
        return false;
    }

    return true;
}

// 0x4A75E0
int sub_4A75E0(int64_t obj, int stat, int value)
{
    int current_value;
    int cost;
    int unspent_points;
    int new_value;

    current_value = stat_base_get(obj, stat);
    while (current_value < value) {
        cost = stat_cost(current_value + 1);
        unspent_points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        if (cost > unspent_points) {
            return 1;
        }

        stat_base_set(obj, stat, current_value + 1);
        new_value = stat_base_get(obj, stat);
        if (new_value < current_value + 1) {
            return 2;
        }

        stat_base_set(obj, STAT_UNSPENT_POINTS, unspent_points - cost);

        current_value = stat_level_get(obj, stat);
        sub_4A7B90(stat_name(stat), stat_level_get(obj, stat));
    }

    return 0;
}

// 0x4A76B0
int sub_4A76B0(int64_t obj, int skill, int value)
{
    int current_value;
    int cost;
    int unspent_points;
    int new_value;
    int stat;
    int rc;

    current_value = basic_skill_level(obj, skill);
    while (current_value < value) {
        cost = sub_4C64B0(obj, skill);
        unspent_points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        if (cost > unspent_points) {
            return 1;
        }

        new_value = basic_skill_get_base(obj, skill) + cost;
        if (basic_skill_set_base(obj, skill, new_value) == new_value) {
            stat_base_set(obj, STAT_UNSPENT_POINTS, unspent_points - cost);
            current_value = basic_skill_level(obj, skill);
            sub_4A7B90(basic_skill_get_name(skill), new_value);
        } else {
            stat = basic_skill_get_stat(skill);

            rc = sub_4A75E0(obj, stat, stat_base_get(obj, stat) + 1);
            if (rc != 0) {
                return rc;
            }
        }
    }

    return 0;
}

// 0x4A77A0
int sub_4A77A0(int64_t obj, int skill, int score)
{
    int level;
    int cost;
    int unspent_points;
    int new_level;
    int stat;

    level = tech_skill_level(obj, skill);
    while (level < score) {
        cost = sub_4C6AF0(obj, skill);
        unspent_points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        if (cost > unspent_points) {
            return 1;
        }

        new_level = tech_skill_get_base(obj, skill) + cost;
        if (tech_skill_set_base(obj, skill, new_level) == new_level) {
            stat_base_set(obj, STAT_UNSPENT_POINTS, unspent_points - cost);
            level = tech_skill_level(obj, skill);
            sub_4A7B90(tech_skill_get_name(skill), new_level);
        } else {
            stat = tech_skill_get_stat(skill);
            if (sub_4A75E0(obj, stat, stat_base_get(obj, stat) + 1)) {
                return 1;
            }
        }
    }

    return 0;
}

// 0x4A7890
int sub_4A7890(int64_t obj, int college, int score)
{
    int current_value;
    int spl;
    int cost;
    int unspent_points;
    int intelligence;
    int willpower;
    int rc;

    current_value = spell_college_level_get(obj, college);
    while (current_value < score) {
        spl = current_value + 5 * college;
        cost = spell_cost(spl);
        unspent_points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        if (cost > unspent_points) {
            return 1;
        }

        if (spell_add(obj, spl, 0)) {
            stat_base_set(obj, STAT_UNSPENT_POINTS, unspent_points - cost);
            current_value++;
            spl++;
        } else {
            if (spell_min_level(spl) > stat_level_get(obj, STAT_LEVEL)) {
                return 2;
            }

            if (spell_min_intelligence(spl) > stat_level_get(obj, STAT_INTELLIGENCE)) {
                intelligence = stat_base_get(obj, STAT_INTELLIGENCE);
                rc = sub_4A75E0(obj, STAT_INTELLIGENCE, intelligence + 1);
            } else {
                willpower = stat_base_get(obj, STAT_WILLPOWER);
                rc = sub_4A75E0(obj, STAT_WILLPOWER, willpower + 1);
            }

            if (rc != 0) {
                return rc;
            }
        }
    }

    return 0;
}

// 0x4A79C0
int sub_4A79C0(int64_t obj, int tech, int degree)
{
    int current_degree;
    int cost;
    int unspent_points;
    int new_degree;
    int intelligence;
    int rc;

    current_degree = tech_degree_get(obj, tech);
    while (current_degree < degree) {
        cost = tech_degree_cost_get(current_degree + 1);
        unspent_points = stat_level_get(obj, STAT_UNSPENT_POINTS);
        if (cost > unspent_points) {
            return 1;
        }

        new_degree = tech_degree_inc(obj, tech);
        if (new_degree == current_degree) {
            intelligence = stat_base_get(obj, STAT_INTELLIGENCE);
            rc = sub_4A75E0(obj, STAT_INTELLIGENCE, intelligence + 1);
            if (rc != 0) {
                return rc;
            }
        } else {
            stat_base_set(obj, STAT_UNSPENT_POINTS, unspent_points - cost);
            current_degree = new_degree;
            sub_4A7B90(tech_discipline_name_get(tech), -new_degree);
        }
    }

    return 0;
}

// 0x4A7AA0
int sub_4A7AA0(int64_t obj, int type, int score)
{
    int current_value;
    int unspent_points;
    int new_value;
    MesFileEntry mes_file_entry;

    current_value = off_4A7ABE[type](obj);
    while (current_value < score) {
        unspent_points = stat_base_get(obj, STAT_UNSPENT_POINTS);
        if (unspent_points < 1) {
            return 1;
        }

        off_4A7AFE[type](obj, off_4A7AF3[type](obj) + 1);
        new_value = off_4A7ABE[type](obj);
        if (new_value < current_value + 1) {
            return 2;
        }

        stat_base_set(obj, STAT_UNSPENT_POINTS, unspent_points - 1);

        mes_file_entry.num = 5 + type;
        mes_get_msg(level_mes_file, &mes_file_entry);
        sub_4A7B90(mes_file_entry.str, new_value);

        current_value = new_value;
    }

    return 0;
}

// 0x4A7B90
void sub_4A7B90(const char* str, int score)
{
    int index;

    if (dword_5F5C9C != -1) {
        for (index = 0; index < dword_5F5C9C; index++) {
            if (strcmp(byte_5F0E60[index], str) == 0) {
                dword_5F5C80[index] = score;
                return;
            }
        }

        if (dword_5F5C9C < 10) {
            strcpy(byte_5F0E60[dword_5F5C9C], str);
            dword_5F5C80[dword_5F5C9C] = score;
            dword_5F5C9C++;
        }
    }
}
