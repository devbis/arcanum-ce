#include "game/lib/level.h"

#include "game/lib/message.h"
#include "game/lib/skill.h"
#include "game/lib/spell.h"
#include "game/lib/stat.h"
#include "game/lib/tech.h"

#define LEVEL_MAX 51
#define TEN 10

// 0x5F5C80
static int* dword_5F5C80;

// 0x5F5C84
static int game_level_name_msg_file;

// 0x5F5C88
static int user_level_name_msg_file;

// 0x5F5C8C
static int game_level_msg_file;

// 0x5F5C90
static int user_level_msg_file;

// 0x5F5C94
static int* dword_5F5C94;

// 0x5F5C98
static int level_msg_file;

// 0x4A6620
bool level_init(GameContext* ctx)
{
    int xp_level_msg_file;
    if (!message_load("rules\\xp_level.mes", &xp_level_msg_file)) {
        return false;
    }

    dword_5F5C94 = (int*)calloc(LEVEL_MAX, sizeof(*dword_5F5C94));
    dword_5F5C80 = calloc(TEN, sizeof(*dword_5F5C80));

    for (int index = 0; index < LEVEL_MAX; index++) {
        MessageListItem msg;
        msg.num = index + 1;
        if (message_find(xp_level_msg_file, &msg)) {
            dword_5F5C94[index] = atoi(msg.text);
        } else {
            dword_5F5C94[index] = 0;
        }
    }

    message_unload(xp_level_msg_file);

    dword_5F5C94[0] = 0;

    if (!message_load("rules\\gamelevel.mes", &game_level_msg_file)) {
        free(dword_5F5C94);
        free(dword_5F5C80);
        return false;
    }

    if (!message_load("rules\\userlevel.mes", &user_level_msg_file)) {
        message_unload(game_level_msg_file);
        free(dword_5F5C94);
        free(dword_5F5C80);
        return false;
    }

    if (!message_load("mes\\gamelevelname.mes", &game_level_name_msg_file)) {
        message_unload(user_level_msg_file);
        message_unload(game_level_msg_file);
        free(dword_5F5C94);
        free(dword_5F5C80);
        return false;
    }

    if (!message_load("mes\\userlevelname.mes", &user_level_name_msg_file)) {
        message_unload(game_level_name_msg_file);
        message_unload(user_level_msg_file);
        message_unload(game_level_msg_file);
        free(dword_5F5C94);
        free(dword_5F5C80);
        return false;
    }

    if (!message_load("mes\\level.mes", &level_msg_file)) {
        message_unload(user_level_name_msg_file);
        message_unload(game_level_name_msg_file);
        message_unload(user_level_msg_file);
        message_unload(game_level_msg_file);
        free(dword_5F5C94);
        free(dword_5F5C80);
        return false;
    }

    return true;
}

// 0x4A6850
void level_exit()
{
    message_unload(game_level_msg_file);
    message_unload(user_level_msg_file);
    message_unload(game_level_name_msg_file);
    message_unload(user_level_name_msg_file);
    message_unload(level_msg_file);
    free(dword_5F5C94);
    free(dword_5F5C80);
}

// 0x4A68B0
int level_get_experience_points_to_next_level(object_id_t object_id)
{
    int level = sub_4B0490(object_id, STAT_LEVEL);
    if (level >= LEVEL_MAX) {
        return 999999;
    }

    return dword_5F5C94[level] - sub_4B0490(object_id, STAT_EXPERIENCE_POINTS);
}

// 0x4A6900
int level_get_experience_points_to_next_level_in_percent(object_id_t object_id)
{
    int level = sub_4B0490(object_id, STAT_LEVEL);
    if (level >= LEVEL_MAX) {
        return 0;
    }

    if (level != 0 && dword_5F5C94[level] != 0) {
        int xp = level_get_experience_points_to_next_level(object_id);
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
    for (int level = old_level + 1; level <= new_level; level++) {
        points++;
        if ((level % 5) == 0) {
            points++;
        }
    }
    return points;
}

// 0x4A6CB0
void sub_4A6CB0(object_id_t object_id, int a3, int a4)
{
    int curr_level = sub_4B0490(object_id, STAT_LEVEL);
    int max_level = stat_get_max_value(object_id, STAT_LEVEL);
    if (curr_level < max_level) {
        int new_level;
        if (curr_level <= a3) {
            new_level = a4 + curr_level - a3;
        } else {
            new_level = a4;
        }

        if (new_level >= max_level) {
            new_level = max_level - 1;
        }

        if (new_level > curr_level) {
            sub_4B0980(object_id, STAT_LEVEL, new_level);

            int bonus_points = sub_4A6980(curr_level, new_level);
            int curr_points = sub_4B0490(object_id, STAT_UNSPENT_POINTS);
            sub_4B0980(object_id, bonus_points + curr_points);
            sub_4A7030(object_id, NULL);
        }
    }
}

// 0x4A6D40
int level_auto_level_scheme_get(object_id_t object_id)
{
    if (object_id == 0) {
        return -1;
    }

    int type = obj_field_int32_get(object_id, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC && type != OBJ_TYPE_NPC) {
        return -1;
    }

    return obj_field_int32_get(object_id, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME);
}

// 0x4A6D90
int level_auto_level_scheme_set(object_id_t object_id, int value)
{
    if (object_id == 0) {
        return -1;
    }

    int type = obj_field_int32_get(object_id, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC && type != OBJ_TYPE_NPC) {
        return -1;
    }

    obj_field_int32_set(object_id, OBJ_F_CRITTER_AUTO_LEVEL_SCHEME, value);

    return value;
}

// 0x4A6DE0
const char* level_advancement_scheme_get_name(int scheme)
{
    int msg_file;
    MessageListItem msg;

    if (scheme == 0) {
        msg_file = game_level_name_msg_file;
    } else if (scheme >= 1 && scheme < 200) {
        if (scheme < 50) {
            msg_file = user_level_name_msg_file;
        } else {
            msg_file = game_level_name_msg_file;
        }
    } else {
        return NULL;
    }

    msg.num = scheme;
    if (!message_find(msg_file, &msg)) {
        return NULL;
    }

    return msg.text;
}

// 0x4A6E40
const char* level_advancement_scheme_get_rule(int scheme)
{
    int msg_file;
    MessageListItem msg;

    if (scheme == 0 || scheme < 1 || scheme >= 200) {
        return NULL;
    }

    if (scheme < 50) {
        msg_file = user_level_msg_file;
    } else {
        msg_file = game_level_msg_file;
    }

    msg.num = scheme;
    if (!message_find(msg_file, &msg)) {
        return NULL;
    }

    return msg.text;
}

// 0x4A6F00
void level_set_level(object_id_t obj, int level)
{
    // NOTE: Unused.
    int type = obj_field_int32_get(obj, OBJ_F_TYPE);

    if (level < 0) {
        level = 0;
    } else if (level > LEVEL_MAX) {
        level = LEVEL_MAX;
    }

    int gender = stat_get_value(obj, STAT_GENDER);
    int race = stat_get_value(obj, STAT_RACE);
    int alignment = stat_get_value(obj, STAT_ALIGNMENT);
    int age = stat_get_value(obj, STAT_AGE);

    stat_set_defaults(obj);
    skill_set_defaults(obj);
    spell_set_defaults(obj);
    tech_set_defaults(obj);

    stat_set_value(obj, STAT_GENDER, gender);
    stat_set_value(obj, STAT_RACE, race);
    stat_set_value(obj, STAT_ALIGNMENT, alignment);
    stat_set_value(obj, STAT_AGE, age);
    stat_set_value(obj, STAT_LEVEL, level);

    sub_43D430(obj, 0);
    sub_45D3E0(obj, 0);

    if (level >= 1) {
        stat_set_value(obj, STAT_EXPERIENCE_POINTS, dword_5F5C94[level - 1]);

        int unspent_points = stat_get_value(obj, STAT_UNSPENT_POINTS);
        int bonus = sub_4A6980(1, level);
        stat_set_value(obj, STAT_UNSPENT_POINTS, unspent_points + bonus);
    }

    sub_4A7030(obj, NULL);
    sub_43D530(obj, 0);
}
