#include "game/skill.h"

#include "game/anim.h"
#include "game/background.h"
#include "game/critter.h"
#include "game/effect.h"
#include "game/gamelib.h"
#include "game/item.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/player.h"
#include "game/random.h"
#include "game/stat.h"

#define FIRST_BASIC_SKILL_NAME_ID 0
#define FIRST_TECH_SKILL_NAME_ID (FIRST_BASIC_SKILL_NAME_ID + BASIC_SKILL_COUNT)
#define FIRST_TRAINING_NAME_ID (FIRST_TECH_SKILL_NAME_ID + TECH_SKILL_COUNT)
#define FIRST_BASIC_SKILL_DESC_ID (FIRST_TRAINING_NAME_ID + TRAINING_COUNT)
#define FIRST_TECH_SKILL_DESC_ID (FIRST_BASIC_SKILL_DESC_ID + BASIC_SKILL_COUNT)

static int sub_4C5F70(int value);
static int sub_4C6560();
static bool sub_4C81A0(int a1, int a2, Tanya* a3);
static bool sub_4C82E0(int a1, int a2, Tanya* a3);
static bool sub_4C83E0(int64_t obj);

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

// 0x5B6F44
static int dword_5B6F44[TECH_SKILL_COUNT] = {
    0,
    1,
    9,
    18,
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

// 0x5B6FF4
const char* off_5B6FF4[BASIC_SKILL_COUNT] = {
    "bs_bow",
    "bs_dodge",
    "bs_melee",
    "bs_throwing",
    "bs_backstab",
    "bs_pick_pocket",
    "bs_prowling",
    "bs_spot_trap",
    "bs_gambling",
    "bs_haggle",
    "bs_heal",
    "bs_persuasion",
};

// 0x5B7024
const char* off_5B7024[TECH_SKILL_COUNT] = {
    "ts_repair",
    "ts_firearms",
    "ts_pick_lock",
    "ts_disarm_trap",
};

// 0x5B7034
const char* off_5B7034[TRAINING_COUNT] = {
    "st_untrained",
    "st_beginner",
    "st_expert",
    "st_master",
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

    (void)init_info;

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
        mes_get_msg(skill_mes_file, &mes_file_entry);
        basic_skill_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < TECH_SKILL_COUNT; index++) {
        mes_file_entry.num = index + FIRST_TECH_SKILL_NAME_ID;
        mes_get_msg(skill_mes_file, &mes_file_entry);
        tech_skill_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < TRAINING_COUNT; index++) {
        mes_file_entry.num = index + FIRST_TRAINING_NAME_ID;
        mes_get_msg(skill_mes_file, &mes_file_entry);
        training_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < BASIC_SKILL_COUNT; index++) {
        mes_file_entry.num = index + FIRST_BASIC_SKILL_DESC_ID;
        mes_get_msg(skill_mes_file, &mes_file_entry);
        basic_skill_descriptions[index] = mes_file_entry.str;
    }

    for (index = 0; index < TECH_SKILL_COUNT; index++) {
        mes_file_entry.num = index + FIRST_TECH_SKILL_DESC_ID;
        mes_get_msg(skill_mes_file, &mes_file_entry);
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
    mes_unload(skill_mes_file);
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
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= BASIC_SKILL_COUNT) {
        return 0;
    }

    return 4 * basic_skill_get_base(obj, skill);
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
int basic_skill_get_base(int64_t obj, int skill)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= BASIC_SKILL_COUNT) {
        return 0;
    }

    return obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, skill) & 63;
}

// 0x4C6000
int basic_skill_set_base(int64_t obj, int skill, int value)
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
    int melee;
    int level;
    int index;

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= BASIC_SKILL_COUNT) {
        return 0;
    }

    if (skill == BASIC_SKILL_MELEE && sub_45F730(obj)) {
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

// 0x4C6170
int basic_skill_set_training(int64_t obj, int skill, int training)
{
    int skill_value;
    int current_training;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    if (skill < 0 || skill >= BASIC_SKILL_COUNT) {
        return 0;
    }

    if (training < 0 || training >= TRAINING_COUNT) {
        return 0;
    }

    skill_value = obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, skill);
    current_training = (skill_value >> 6) & 3;
    if (!sub_4A2BA0()) {
        SetSkillTrainingPacket pkt;

        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return current_training;
        }

        pkt.type = 56;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.skill = skill;
        pkt.training = training;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    if (training > current_training && current_training != training - 1) {
        return current_training;
    }

    if (basic_skill_level(obj, skill) < dword_5B6F44[skill]) {
        return current_training;
    }

    obj_arrayfield_int32_set(obj,
        OBJ_F_CRITTER_BASIC_SKILL_IDX,
        skill,
        (skill_value & ~(3 << 6)) | (training << 6));

    sub_4C2950(obj);

    return training;
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
    (void)a1;

    return (a3 + 1) * (a2 + 2);
}

// 0x4C62E0
int sub_4C62E0(int64_t obj, int skill, int64_t other_obj)
{
    int value;
    int level;
    int game_difficulty;

    if ((dword_5B6F64[skill] & 0x70) != 0) {
        value = sub_4C6410(obj, skill, other_obj);
    } else {
        level = basic_skill_level(obj, skill);
        switch (skill) {
        case BASIC_SKILL_BOW:
            value = 5 * level + 25;
            break;
        case BASIC_SKILL_DODGE:
            value = 5 * level;
            break;
        case BASIC_SKILL_MELEE:
            value = 5 * level + 25;
            break;
        case BASIC_SKILL_THROWING:
            value = 7 * level + 25;
            break;
        case BASIC_SKILL_PROWLING:
            if (level != 0) {
                value = 5 * level + 25;
            } else {
                value = 0;
            }
            break;
        case BASIC_SKILL_SPOT_TRAP:
            value = 4 * level;
            break;
        case BASIC_SKILL_HAGGLE:
            value = 4 * level + 10;
            break;
        case BASIC_SKILL_HEAL:
            value = 5 * level;
            break;
        default:
            value = 0;
            break;
        }
    }

    if (stat_is_maximized(obj, STAT_INTELLIGENCE)) {
        value += 10;
    }

    if (obj == player_get_pc_obj()) {
        game_difficulty = gamelib_get_game_difficulty();
        switch (game_difficulty) {
        case 0:
            value += value / 2;
            break;
        case 2:
            value -= value / 4;
            break;
        }
    }

    if ((dword_5B6F64[skill] & 0x800) != 0) {
        if (value > 95) {
            value = 95;
        }
    }

    if (value < 0) {
        value = 0;
    }

    return value;
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

// 0x4C64E0
int sub_4C64E0(int a1)
{
    int value;

    for (value = 0; value < 20; value++) {
        if (sub_4C5F70(value + 1) >= a1) {
            return value + 1;
        }
    }

    return 0;
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

// 0x4C6560
int sub_4C6560()
{
    int value;

    value = dword_6876C4;
    dword_6876C4 = random_between(1, 100);

    return value;
}

// 0x4C6580
int sub_4C6580(int64_t obj, int skill)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= TECH_SKILL_COUNT) {
        return 0;
    }

    return 4 * tech_skill_get_base(obj, skill);
}

// 0x4C65E0
int tech_skill_level(int64_t obj, int skill)
{
    int v1;
    int skill_level;
    int key_stat_level;

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= TECH_SKILL_COUNT) {
        return 0;
    }

    v1 = sub_4C6580(obj, skill);
    skill_level = effect_adjust_tech_skill_level(obj, skill, v1);
    if (skill_level < 0) {
        skill_level = 0;
    } else if (skill_level > 20) {
        skill_level = 20;
    }

    key_stat_level = stat_level(obj, tech_skill_get_stat(skill));
    if (skill_level > sub_4C5F70(key_stat_level)) {
        skill_level = sub_4C5F70(key_stat_level);
    }

    return skill_level;
}

// 0x4C6680
int tech_skill_get_base(int64_t obj, int skill)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= TECH_SKILL_COUNT) {
        return 0;
    }

    return obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_TECH_SKILL_IDX, skill) & 63;
}

// 0x4C66E0
int tech_skill_set_base(int64_t obj, int skill, int value)
{
    int key_stat_level;
    int current_value;
    int tech_points;

    if (value < 0) {
        return 0;
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
        && (tig_net_flags & TIG_NET_HOST) == 0
        && !sub_4A2BA0()) {
        return 0;
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    if (skill < 0 || skill >= TECH_SKILL_COUNT) {
        return 0;
    }

    key_stat_level = stat_level(obj, tech_skill_get_stat(skill));
    current_value = obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_TECH_SKILL_IDX, skill);

    if (4 * value > sub_4C5F70(key_stat_level)) {
        return current_value & 63;
    }

    sub_4F0150(obj, OBJ_F_CRITTER_TECH_SKILL_IDX, skill, value | current_value & ~63);

    tech_points = stat_get_base(obj, STAT_TECH_POINTS);
    tech_points += value - (current_value & 63);
    stat_set_base(obj, STAT_TECH_POINTS, tech_points);

    return value;
}

// 0x4C67F0
int tech_skill_get_training(int64_t obj, int skill)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))
        || skill < 0
        || skill >= TECH_SKILL_COUNT) {
        return TRAINING_NONE;
    }

    return (obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_TECH_SKILL_IDX, skill) >> 6) & 3;
}

// 0x4C6850
int tech_skill_set_training(int64_t obj, int skill, int training)
{
    int skill_value;
    int current_training;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    if (skill < 0 || skill >= TECH_SKILL_COUNT) {
        return 0;
    }

    if (training < 0 || training >= TRAINING_COUNT) {
        return 0;
    }

    skill_value = obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_TECH_SKILL_IDX, skill);
    current_training = (skill_value >> 6) & 3;
    if (!sub_4A2BA0()) {
        SetSkillTrainingPacket pkt;

        if ((tig_net_flags & TIG_NET_HOST) == 0) {
            return current_training;
        }

        pkt.type = 56;
        sub_4440E0(obj, &(pkt.field_8));
        pkt.skill = BASIC_SKILL_COUNT + skill;
        pkt.training = training;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    if (training > current_training && current_training != training - 1) {
        return current_training;
    }

    if (tech_skill_level(obj, skill) < dword_5B6F44[skill]) {
        return current_training;
    }

    obj_arrayfield_int32_set(obj,
        OBJ_F_CRITTER_TECH_SKILL_IDX,
        skill,
        (skill_value & ~(3 << 6)) | (training << 6));

    sub_4C2950(obj);

    return training;
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

// 0x4C69C0
int sub_4C69C0(int skill)
{
    return dword_5B6F44[skill];
}

// 0x4C69D0
const char* training_get_name(int training)
{
    return training_names[training];
}

// 0x4C69E0
int sub_4C69E0(int a1, int a2, int a3)
{
    (void)a1;

    return (a3 + 1) * (a2 + 2);
}

// 0x4C69F0
int sub_4C69F0(int64_t obj, int skill, int64_t other_obj)
{
    int value;
    int level;
    int game_difficulty;

    level = tech_skill_level(obj, skill);
    switch (skill) {
    case TECH_SKILL_REPAIR:
    value = 4 * level;
        break;
    case TECH_SKILL_FIREARMS:
        value = 5 * level + 25;
        if (other_obj != OBJ_HANDLE_NULL
            && (obj_field_int32_get(other_obj, OBJ_F_SPELL_FLAGS) & OSF_MAGNETIC_INVERSION) != 0) {
            value -= 20;
        }
        break;
    case TECH_SKILL_PICK_LOCKS:
        value = 6 * level;
        break;
    case TECH_SKILL_DISARM_TRAPS:
        if (level != 0) {
            value = 4 * level + 20;
        } else {
            value = 0;
        }
        break;
    default:
        value = 0;
        break;
    }

    if (stat_is_maximized(obj, STAT_INTELLIGENCE)) {
        value += 10;
    }

    if (obj == player_get_pc_obj()) {
        game_difficulty = gamelib_get_game_difficulty();
        switch (game_difficulty) {
        case 0:
            value += value / 2;
            break;
        case 2:
            value -= value / 4;
            break;
        }
    }

    if ((dword_5B6F94[skill] & 0x800) != 0) {
        if (value > 95) {
            value = 95;
        }
    }

    if (value < 0) {
        value = 0;
    }

    return value;
}

// 0x4C6AF0
int sub_4C6AF0(int64_t obj, int skill)
{
    (void)obj;
    (void)skill;

    return 1;
}

// 0x4C6B00
int sub_4C6B00(int64_t obj, int skill)
{
    (void)obj;
    (void)skill;

    return 1;
}

// 0x4C6B10
int tech_skill_get_stat(int skill)
{
    return tech_skill_stats[skill];
}

// 0x4C6B20
int sub_4C6B20(int a1)
{
    int value;

    for (value = 0; value < 20; value++) {
        if (sub_4C5F70(value + 1) >= a1) {
            return value + 1;
        }
    }

    return 0;
}

// 0x4C6B50
bool sub_4C6B50(int64_t obj, int stat, int value)
{
    int skill;

    for (skill = 0; skill < BASIC_SKILL_COUNT; skill++) {
        if (basic_skill_stats[skill] == stat) {
            if (sub_4C5E50(obj, skill) > sub_4C5F70(value)) {
                return false;
            }
        }
    }

    for (skill = 0; skill < TECH_SKILL_COUNT; skill++) {
        if (tech_skill_stats[skill] == stat) {
            if (sub_4C6580(obj, skill) > sub_4C5F70(value)) {
                return false;
            }
        }
    }

    return true;
}

// 0x4C6F90
bool sub_4C6F90(int64_t a1, int a2, int64_t a3, int a4)
{
    return sub_4350F0(a1, a3, OBJ_HANDLE_NULL, a2, a4);
}

// 0x4C6FD0
bool sub_4C6FD0(int64_t a1, int64_t a2, int64_t a3)
{
    return sub_4350F0(a1, a2, a3, 5, 0);
}

// 0x4C7010
bool sub_4C7010(int64_t a1, int64_t a2, int64_t a3)
{
    return sub_4350F0(a1, a2, a3, 5, 1);
}

// 0x4C7050
bool sub_4C7050(int64_t a1, int a2, int64_t a3)
{
    return sub_4350F0(a1, a3, OBJ_HANDLE_NULL, 15, 2 << a2);
}

// 0x4C7090
void sub_4C7090(Tanya* a1)
{
    a1->field_98 = 0;
    sub_4440E0(OBJ_HANDLE_NULL, &(a1->field_68));
    sub_4440E0(OBJ_HANDLE_NULL, &(a1->field_0));
    sub_4440E0(OBJ_HANDLE_NULL, &(a1->field_30));
    a1->field_60 = 0;
    a1->field_64 = 0;
    a1->field_A0 = 0;
    a1->field_9C = -1;
}

// 0x4C7120
bool sub_4C7120(Tanya* a1)
{
    return sub_444130(&(a1->field_0))
        && sub_444130(&(a1->field_30))
        && sub_444130(&(a1->field_68));
}

// 0x4C7160
bool sub_4C7160(Tanya* a1)
{
    // TODO: Incomplete.
}

// 0x4C81A0
bool sub_4C81A0(int a1, int a2, Tanya* a3)
{
    int v1;
    int v2;

    v1 = (dword_5B6F64[a3->field_9C] & 0x400) != 0 ? 20 : 2;
    v2 = a2 / v1;

    if ((a3->field_98 & 0x8) != 0) {
        v2 += sub_4B5F30(a3->field_A4) / -5;
    }

    if (a3->field_68.obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(a3->field_68.obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
        && (a3->field_98 & 0x10000) == 0) {
        v2 += sub_461590(a3->field_68.obj,
            a3->field_0.obj,
            obj_field_int32_get(a3->field_68.obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE));
    }

    if ((a3->field_98 & 0x8000) != 0) {
        v2 += 2 * basic_skill_level(a3->field_0.obj, BASIC_SKILL_BACKSTAB);
        v2 -= stat_level(a3->field_30.obj, STAT_LEVEL);

        if (basic_skill_get_training(a3->field_0.obj, BASIC_SKILL_BACKSTAB) == TRAINING_MASTER) {
            v2 += 20;
        }
    }

    v2 = effect_adjust_crit_hit_chance(a3->field_0.obj, v2);

    return a1 <= v2;
}

// 0x4C82E0
bool sub_4C82E0(int a1, int a2, Tanya* a3)
{
    int v1;
    int v2;
    int v3;
    int v4;

    v1 = (dword_5B6F64[a3->field_9C] & 0x400) != 0 ? 7 : 2;
    v2 = (100 - a2) / v1;

    if (a3->field_68.obj != OBJ_HANDLE_NULL) {
        v3 = sub_43D600(a3->field_68.obj);
        v4 = sub_43D5A0(a3->field_68.obj);
        if (v3 < v4) {
            v2 += 100 * (v4 - v3) / (v1 * v4);
        }

        if (obj_field_int32_get(a3->field_68.obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
            && (a3->field_98 & 0x10000) == 0) {
            v2 += sub_461590(a3->field_68.obj,
                a3->field_0.obj,
                obj_field_int32_get(a3->field_68.obj, OBJ_F_WEAPON_MAGIC_CRIT_MISS_CHANCE));
        }
    }

    v2 = effect_adjust_crit_fail_chance(a3->field_0.obj, v2);
    if (v2 < 2) {
        v2 = 2;
    }

    return a1 <= v2;
}

// 0x4C83E0
bool sub_4C83E0(int64_t obj)
{
    int64_t leader_obj;

    if (player_is_pc_obj(obj)) {
        return true;
    }

    leader_obj = critter_leader_get(obj);
    if (leader_obj != OBJ_HANDLE_NULL && player_is_pc_obj(leader_obj)) {
        return true;
    }

    return false;
}

// 0x4C8430
int sub_4C8430(Tanya* a1)
{
    // TODO: Incomplete.
}

// 0x4C8E60
void sub_4C8E60(int64_t a1, int64_t a2, int64_t a3, int a4)
{
    Tanya v1;
    Packet126 pkt;

    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
        || (tig_net_flags & TIG_NET_HOST) != 0) {
        sub_4C7090(&v1);
        v1.field_98 |= 0x1000;
        v1.field_9C = 12;
        sub_4440E0(a2, &(v1.field_0));
        sub_4440E0(a1, &(v1.field_30));
        sub_4C7160(&v1);
        sub_464830(a3, a2, a4, OBJ_HANDLE_NULL);
        sub_4EE3A0(a3, a1);
    } else {
        pkt.type = 126;
        sub_4F0640(a1, &(pkt.field_8));
        sub_4F0640(a2, &(pkt.field_20));
        sub_4F0640(a3, &(pkt.field_38));
        pkt.field_50 = a4;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }
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

// 0x4C9050
void sub_4C9050(Tanya* a1)
{
    int skill_level;
    ObjectList objects;
    ObjectNode* node;
    int v1;
    int64_t v2;

    if (obj_field_int32_get(a1->field_0.obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (!get_follower_skills(a1->field_0.obj)) {
        return;
    }

    if (IS_TECH_SKILL(a1->field_9C)) {
        skill_level = sub_4C69F0(a1->field_0.obj, GET_TECH_SKILL(a1->field_9C), a1->field_30.obj) - sub_4C8430(a1);
        v2 = a1->field_0.obj;
        sub_441260(v2, &objects);
        node = objects.head;
        while (node != NULL) {
            sub_4440E0(node->obj, &(a1->field_0));
            v1 = sub_4C69F0(a1->field_0.obj, GET_TECH_SKILL(a1->field_9C), a1->field_30.obj) - sub_4C8430(a1);
            if (v1 > skill_level) {
                skill_level = v1;
                v2 = node->obj;
            }
            node = node->next;
        }
        // FIXME: Probably leaks `objects`?
    } else {
        skill_level = sub_4C62E0(a1->field_0.obj, GET_BASIC_SKILL(a1->field_9C), a1->field_30.obj) - sub_4C8430(a1);
        v2 = a1->field_0.obj;
        sub_441260(v2, &objects);
        node = objects.head;
        while (node != NULL) {
            sub_4440E0(node->obj, &(a1->field_0));
            v1 = sub_4C62E0(a1->field_0.obj, GET_BASIC_SKILL(a1->field_9C), a1->field_30.obj) - sub_4C8430(a1);
            if (v1 > skill_level) {
                skill_level = v1;
                v2 = node->obj;
            }

            node = node->next;
        }
        // FIXME: Probably leaks `objects`?
    }

    sub_4440E0(v2, &(a1->field_0));
}

// 0x4C91F0
int64_t sub_4C91F0(int64_t obj, int skill)
{
    int64_t item_obj = OBJ_HANDLE_NULL;
    int64_t substitute_inventory_obj;

    switch (skill) {
    case SKILL_HEAL:
        item_obj = item_find_first_generic(obj, OGF_IS_HEALING_ITEM);
        if (item_obj == OBJ_HANDLE_NULL) {
            substitute_inventory_obj = sub_45F650(obj);
            if (substitute_inventory_obj != OBJ_HANDLE_NULL) {
                item_obj = item_find_first_generic(substitute_inventory_obj, OGF_IS_HEALING_ITEM);
            }
        }
        break;
    case SKILL_PICK_LOCKS:
        item_obj = item_find_first_generic(obj, OGF_IS_LOCKPICK);
        if (item_obj == OBJ_HANDLE_NULL) {
            substitute_inventory_obj = sub_45F650(obj);
            if (substitute_inventory_obj != OBJ_HANDLE_NULL) {
                item_obj = item_find_first_generic(substitute_inventory_obj, OGF_IS_LOCKPICK);
            }
        }
        break;
    }

    return item_obj;
}
