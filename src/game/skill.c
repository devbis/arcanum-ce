#include "game/skill.h"

#include "game/ai.h"
#include "game/anim.h"
#include "game/background.h"
#include "game/critter.h"
#include "game/effect.h"
#include "game/fate.h"
#include "game/gamelib.h"
#include "game/item.h"
#include "game/light.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/player.h"
#include "game/random.h"
#include "game/stat.h"
#include "game/trap.h"

#define FIRST_BASIC_SKILL_NAME_ID 0
#define FIRST_TECH_SKILL_NAME_ID (FIRST_BASIC_SKILL_NAME_ID + BASIC_SKILL_COUNT)
#define FIRST_TRAINING_NAME_ID (FIRST_TECH_SKILL_NAME_ID + TECH_SKILL_COUNT)
#define FIRST_BASIC_SKILL_DESC_ID (FIRST_TRAINING_NAME_ID + TRAINING_COUNT)
#define FIRST_TECH_SKILL_DESC_ID (FIRST_BASIC_SKILL_DESC_ID + BASIC_SKILL_COUNT)

static int sub_4C5F70(int value);
static int sub_4C6560();
static bool skill_invocation_check_crit_hit(int a1, int a2, SkillInvocation* skill_invocation);
static bool skill_invocation_check_crit_miss(int a1, int a2, SkillInvocation* skill_invocation);
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
static unsigned int skill_flags[SKILL_COUNT] = {
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

// 0x5B70E4
static int dword_5B70E4[9] = {
    50,
    30,
    30,
    30,
    70,
    70,
    1000,
    1000,
    1000,
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
void skill_set_defaults(int64_t obj)
{
    int index;

    for (index = 0; index < BASIC_SKILL_COUNT; index++) {
        obj_arrayfield_int32_set(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, index, 0);
    }

    for (index = 0; index < TECH_SKILL_COUNT; index++) {
        obj_arrayfield_int32_set(obj, OBJ_F_CRITTER_TECH_SKILL_IDX, index, 0);
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

    if (skill == BASIC_SKILL_MELEE && critter_is_monstrous(obj)) {
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

    key_stat_level = stat_level_get(obj, basic_skill_get_stat(skill));
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

    key_stat_level = stat_level_get(obj, basic_skill_get_stat(skill));
    current_value = obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, skill);

    if (4 * value > sub_4C5F70(key_stat_level)) {
        return current_value & 63;
    }

    mp_obj_arrayfield_int32_set(obj, OBJ_F_CRITTER_BASIC_SKILL_IDX, skill, value | (current_value & ~63));

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

    if (skill == BASIC_SKILL_MELEE && critter_is_monstrous(obj)) {
        melee = basic_skill_level(obj, BASIC_SKILL_MELEE);
        level = stat_level_get(obj, STAT_LEVEL);

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
    if (!multiplayer_is_locked()) {
        SetSkillTrainingPacket pkt;

        if (!tig_net_is_host()) {
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

    background_educate_followers(obj);

    return training;
}

// 0x4C62B0
char* basic_skill_get_name(int skill)
{
    return basic_skill_names[skill];
}

// 0x4C62C0
char* basic_skill_get_description(int skill)
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

    if ((skill_flags[skill] & 0x70) != 0) {
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

    if (stat_atmax(obj, STAT_INTELLIGENCE)) {
        value += 10;
    }

    if (obj == player_get_local_pc_obj()) {
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

    if ((skill_flags[skill] & 0x800) != 0) {
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

    if ((skill_flags[skill] & 0x10) != 0) {
        difficulty = stat_level_get(other_obj, STAT_PERCEPTION);
    } else if ((skill_flags[skill] & 0x20) != 0) {
        difficulty = stat_level_get(other_obj, STAT_INTELLIGENCE);
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

    key_stat_level = stat_level_get(obj, tech_skill_get_stat(skill));
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

    if (tig_net_is_active()
        && !tig_net_is_host()
        && !multiplayer_is_locked()) {
        return 0;
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    if (skill < 0 || skill >= TECH_SKILL_COUNT) {
        return 0;
    }

    key_stat_level = stat_level_get(obj, tech_skill_get_stat(skill));
    current_value = obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_TECH_SKILL_IDX, skill);

    if (4 * value > sub_4C5F70(key_stat_level)) {
        return current_value & 63;
    }

    mp_obj_arrayfield_int32_set(obj, OBJ_F_CRITTER_TECH_SKILL_IDX, skill, value | (current_value & ~63));

    tech_points = stat_base_get(obj, STAT_TECH_POINTS);
    tech_points += value - (current_value & 63);
    stat_base_set(obj, STAT_TECH_POINTS, tech_points);

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
    if (!multiplayer_is_locked()) {
        SetSkillTrainingPacket pkt;

        if (!tig_net_is_host()) {
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

    background_educate_followers(obj);

    return training;
}

// 0x4C69A0
char* tech_skill_get_name(int skill)
{
    return tech_skill_names[skill];
}

// 0x4C69B0
char* tech_skill_get_description(int skill)
{
    return tech_skill_descriptions[skill];
}

// 0x4C69C0
int sub_4C69C0(int skill)
{
    return dword_5B6F44[skill];
}

// 0x4C69D0
char* training_get_name(int training)
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

    if (stat_atmax(obj, STAT_INTELLIGENCE)) {
        value += 10;
    }

    if (obj == player_get_local_pc_obj()) {
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

    if ((skill_flags[skill + BASIC_SKILL_COUNT] & 0x800) != 0) {
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
bool skill_check_stat(int64_t obj, int stat, int value)
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
bool sub_4C6F90(int64_t obj, int skill, int64_t target_obj, unsigned int flags)
{
    return anim_goal_use_skill_on(obj, target_obj, OBJ_HANDLE_NULL, skill, flags);
}

// 0x4C6FD0
bool sub_4C6FD0(int64_t obj, int64_t target_obj, int64_t item_obj)
{
    return anim_goal_use_skill_on(obj, target_obj, item_obj, SKILL_PICK_POCKET, 0);
}

// 0x4C7010
bool sub_4C7010(int64_t obj, int64_t target_obj, int64_t item_obj)
{
    return anim_goal_use_skill_on(obj, target_obj, item_obj, SKILL_PICK_POCKET, 1);
}

// 0x4C7050
bool sub_4C7050(int64_t obj, int a2, int64_t target_obj)
{
    return anim_goal_use_skill_on(obj, target_obj, OBJ_HANDLE_NULL, 15, 2 << a2);
}

// 0x4C7090
void skill_invocation_init(SkillInvocation* skill_invocation)
{
    skill_invocation->flags = 0;
    sub_4440E0(OBJ_HANDLE_NULL, &(skill_invocation->item));
    sub_4440E0(OBJ_HANDLE_NULL, &(skill_invocation->source));
    sub_4440E0(OBJ_HANDLE_NULL, &(skill_invocation->target));
    skill_invocation->target_loc = 0;
    skill_invocation->modifier = 0;
    skill_invocation->skill = -1;
}

// 0x4C7120
bool sub_4C7120(SkillInvocation* skill_invocation)
{
    return sub_444130(&(skill_invocation->source))
        && sub_444130(&(skill_invocation->target))
        && sub_444130(&(skill_invocation->item));
}

// 0x4C7160
bool skill_invocation_run(SkillInvocation* skill_invocation)
{
    int64_t source_obj;
    int64_t target_obj;
    int64_t target_loc;
    int64_t item_obj;
    int skill;
    int basic_skill;
    int tech_skill;
    int training;
    int effectiveness;
    int difficulty;
    bool is_success;
    bool is_critical;
    bool is_fate = false;

    if (tig_net_is_active()
        && !tig_net_is_host()) {
        return false;
    }

    source_obj = skill_invocation->source.obj;
    target_obj = skill_invocation->target.obj;
    target_loc = skill_invocation->target_loc;
    item_obj = skill_invocation->item.obj;
    skill = skill_invocation->skill;

    if (IS_TECH_SKILL(skill)) {
        basic_skill = -1;
        tech_skill = GET_TECH_SKILL(skill);
        effectiveness = sub_4C69F0(source_obj, tech_skill, target_obj);
        training = tech_skill_get_training(source_obj, tech_skill);
    } else {
        basic_skill = GET_BASIC_SKILL(skill);
        tech_skill = -1;
        effectiveness = sub_4C62E0(source_obj, basic_skill, target_obj);
        training = basic_skill_get_training(source_obj, basic_skill);
    }

    difficulty = skill_invocation->modifier + sub_4C8430(skill_invocation);

    int modifier = basic_skill == BASIC_SKILL_GAMBLING
        ? sub_4C6560()
        : random_between(1, 100);

    is_success = difficulty + modifier <= effectiveness
        || (skill_invocation->flags & 0x1000) != 0;

    int crit_roll = random_between(1, 100);
    if (is_success) {
        is_critical = skill_invocation_check_crit_hit(crit_roll, effectiveness, skill_invocation);
    } else {
        if (basic_skill == BASIC_SKILL_MELEE
            && training >= TRAINING_MASTER) {
            is_critical = false;
        } else {
            is_critical = skill_invocation_check_crit_miss(crit_roll, effectiveness, skill_invocation);
        }
    }

    if (obj_field_int32_get(source_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        if (basic_skill == BASIC_SKILL_GAMBLING
            && fate_resolve(source_obj, FATE_CRIT_SUCCESS_GAMBLING)) {
            is_success = true;
            is_critical = true;
        } else if (basic_skill == BASIC_SKILL_HEAL
            && fate_resolve(source_obj, FATE_CRIT_SUCCESS_HEAL)) {
            is_success = true;
            is_critical = true;
        } else if (basic_skill == BASIC_SKILL_PICK_POCKET
            && fate_resolve(source_obj, FATE_CRIT_SUCCESS_PICK_POCKET)) {
            is_success = true;
            is_critical = true;
        } else if (tech_skill == TECH_SKILL_REPAIR
            && fate_resolve(source_obj, FATE_CRIT_SUCCESS_REPAIR)) {
            is_success = true;
            is_critical = true;
            is_fate = true;
        } else if (tech_skill == TECH_SKILL_PICK_LOCKS
            && fate_resolve(source_obj, FATE_CRIT_SUCCESS_PICK_LOCKS)) {
            is_success = true;
            is_critical = true;
        } else if (tech_skill == TECH_SKILL_DISARM_TRAPS
            && fate_resolve(source_obj, FATE_CRIT_SUCCESS_DISARM_TRAPS)) {
            is_success = true;
            is_critical = true;
        }
    }

    switch (skill) {
    case SKILL_DODGE:
        if (critter_is_unconscious(source_obj)
            || (obj_field_int32_get(source_obj, OBJ_F_SPELL_FLAGS) & OSF_STONED) != 0
            || (obj_field_int32_get(source_obj, OBJ_F_CRITTER_FLAGS) & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
            is_success = false;
            is_critical = false;
        }
        break;
    case SKILL_PICK_POCKET: {
        if ((skill_invocation->flags & 0x2) == 0) {
            if (is_success) {
                int64_t parent_obj;
                if (item_parent(item_obj, &parent_obj) && parent_obj == target_obj) {
                    bool moved;
                    switch (obj_field_int32_get(item_obj, OBJ_F_TYPE)) {
                    case OBJ_TYPE_GOLD: {
                        int qty = item_gold_get(item_obj);
                        if (qty > 100) {
                            qty = 100;
                        }

                        moved = item_gold_transfer(target_obj, source_obj, qty, item_obj);
                        break;
                    }
                    case OBJ_TYPE_AMMO: {
                        int ammo_type = obj_field_int32_get(item_obj, OBJ_F_AMMO_TYPE);
                        int qty = item_ammo_quantity_get(item_obj, ammo_type);
                        if (qty > 20) {
                            qty = 20;
                        }
                        moved = item_ammo_transfer(target_obj, source_obj, qty, ammo_type, item_obj);
                        break;
                    }
                    default:
                        moved = item_transfer(item_obj, source_obj);
                        if (tig_net_is_active()) {
                            sub_407EF0(source_obj);
                            sub_407EF0(item_obj);
                        }
                        break;
                    }

                    if (moved) {
                        sub_45DC90(source_obj, target_obj, 0);

                        unsigned int critter_flags2 = obj_field_int32_get(target_obj, OBJ_F_CRITTER_FLAGS2);
                        critter_flags2 |= OCF2_ITEM_STOLEN;
                        obj_field_int32_set(target_obj, OBJ_F_CRITTER_FLAGS2, critter_flags2);

                        if (tig_net_is_active()) {
                            Packet129 pkt;

                            pkt.type = 129;
                            pkt.oid = sub_407EF0(target_obj);
                            pkt.fld = OBJ_F_CRITTER_FLAGS2;
                            pkt.subtype = 0;
                            pkt.val = critter_flags2;
                            tig_net_send_app_all(&pkt, sizeof(pkt));
                        }
                    } else {
                        is_success = false;
                        is_critical = false;
                    }
                } else {
                    is_success = false;
                    is_critical = false;
                }
            } else {
                if (is_critical
                    || (training == TRAINING_NONE && critter_check_stat(target_obj, STAT_PERCEPTION, 0))) {
                    object_script_execute(target_obj, source_obj, item_obj, SAP_CAUGHT_THIEF, 0);
                    if (object_script_execute(source_obj, target_obj, item_obj, SAP_CATCHING_THIEF_PC, 0)) {
                        if (obj_field_int32_get(target_obj, OBJ_F_TYPE) != OBJ_TYPE_NPC
                            || critter_leader_get(target_obj) != source_obj) {
                            ai_attack(source_obj, target_obj, LOUDNESS_NORMAL, 0);
                        }
                    }
                }
            }

            if (obj_field_int32_get(source_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
                if (skill_callbacks.field_4 != NULL) {
                    skill_callbacks.field_4(source_obj, target_obj, item_obj, is_success);
                }
            }
        } else {
            if (is_success) {
                int64_t parent_obj;
                if (item_parent(item_obj, &parent_obj) && parent_obj == source_obj) {
                    bool moved;
                    switch (obj_field_int32_get(item_obj, OBJ_F_TYPE)) {
                    case OBJ_TYPE_GOLD: {
                        int qty = item_gold_get(item_obj);
                        if (qty > 100) {
                            qty = 100;
                        }
                        moved = item_gold_transfer(source_obj, target_obj, qty, item_obj);
                        break;
                    }
                    case OBJ_TYPE_AMMO: {
                        int ammo_type = obj_field_int32_get(item_obj, OBJ_F_AMMO_TYPE);
                        int qty = item_ammo_quantity_get(item_obj, ammo_type);
                        if (qty > 20) {
                            qty = 20;
                        }
                        moved = item_ammo_transfer(source_obj, target_obj, qty, ammo_type, item_obj);
                        break;
                    }
                    default:
                        moved = item_transfer(item_obj, target_obj);
                        if (tig_net_is_active()) {
                            Packet128 pkt;

                            pkt.type = 128;
                            pkt.target_oid = sub_407EF0(target_obj);
                            pkt.item_oid = sub_407EF0(item_obj);
                            tig_net_send_app_all(&pkt, sizeof(pkt));
                        }
                        break;
                    }

                    if (!moved) {
                        is_success = false;
                        is_critical = false;
                    }
                }
            } else {
                if (training < TRAINING_MASTER
                    && (is_critical
                        || (training == TRAINING_NONE && critter_check_stat(target_obj, STAT_PERCEPTION, 0)))) {
                    object_script_execute(target_obj, source_obj, item_obj, SAP_CAUGHT_THIEF, 0);
                    if (object_script_execute(source_obj, target_obj, item_obj, SAP_CATCHING_THIEF_PC, 0)) {
                        if (obj_field_int32_get(target_obj, OBJ_F_TYPE) != OBJ_TYPE_NPC
                            || critter_leader_get(target_obj) != source_obj) {
                            ai_attack(source_obj, target_obj, LOUDNESS_NORMAL, 0);
                        }
                    }
                }
            }

            if (obj_field_int32_get(source_obj, OBJ_F_TYPE) == OBJ_TYPE_PC) {
                if (skill_callbacks.field_8 != NULL) {
                    skill_callbacks.field_8(source_obj, target_obj, item_obj, is_success);
                }
            }
        }
        break;
    }
    case SKILL_SPOT_TRAP:
        if (!is_success
            && !is_critical
            && training >= TRAINING_MASTER
            && difficulty + random_between(1, 100) <= effectiveness) {
            is_success = true;
        }
        if (is_success) {
            if (training < TRAINING_EXPERT
                && trap_type(target_obj) == TRAP_TYPE_MAGICAL) {
                is_success = false;
                is_critical = false;
            }
        }
        break;
    case SKILL_HEAL: {
        CombatContext combat;

        sub_4B2210(source_obj, target_obj, &combat);

        if (is_success) {
            int heal = basic_skill_level(source_obj, BASIC_SKILL_HEAL);
            if (training != TRAINING_NONE) {
                heal = 3 * heal / 2;
            }
            if (heal < 1) {
                heal = 1;
            }
            if (training == TRAINING_MASTER || is_critical) {
                combat.dam[DAMAGE_TYPE_NORMAL] = heal;
                effect_remove_one_caused_by(target_obj, EFFECT_CAUSE_INJURY);

                unsigned int critter_flags = obj_field_int32_get(target_obj, OBJ_F_CRITTER_FLAGS);
                if ((critter_flags & OCF_BLINDED) != 0) {
                    critter_flags &= ~OCF_BLINDED;
                } else if ((critter_flags & OCF_CRIPPLED_ARMS_BOTH) != 0) {
                    critter_flags &= ~OCF_CRIPPLED_ARMS_BOTH;
                } else if ((critter_flags & OCF_CRIPPLED_ARMS_ONE) != 0) {
                    critter_flags &= ~OCF_CRIPPLED_ARMS_ONE;
                } else if ((critter_flags & OCF_CRIPPLED_LEGS_BOTH) != 0) {
                    critter_flags &= ~OCF_CRIPPLED_LEGS_BOTH;
                }
                obj_field_int32_set(target_obj, OBJ_F_CRITTER_FLAGS, critter_flags);

                Packet129 pkt;
                pkt.type = 129;
                pkt.oid = sub_407EF0(target_obj);
                pkt.fld = OBJ_F_CRITTER_FLAGS;
                pkt.subtype = 0;
                pkt.val = critter_flags;
                tig_net_send_app_all(&pkt, sizeof(pkt));
            } else {
                combat.dam[DAMAGE_TYPE_NORMAL] = random_between(1, heal);
            }

            sub_4B58C0(&combat);
        } else {
            if (is_critical && training >= TRAINING_EXPERT) {
                is_critical = false;
            }
        }

        if (item_obj != OBJ_HANDLE_NULL
            && obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_GENERIC
            && (obj_field_int32_get(item_obj, OBJ_F_GENERIC_FLAGS) & OGF_IS_HEALING_ITEM) != 0) {
            int charges = obj_field_int32_get(item_obj, OBJ_F_GENERIC_USAGE_COUNT_REMAINING);
            if (!is_success && is_critical) {
                charges -= 5;
            } else {
                charges -= 1;
            }
            if (charges > 0) {
                obj_field_int32_set(item_obj, OBJ_F_GENERIC_USAGE_COUNT_REMAINING, charges);
            } else {
                object_destroy(item_obj);
            }
        }
        break;
    }
    case SKILL_REPAIR: {
        Packet88 pkt;
        int durability_dam_by_training[3];

        memset(&pkt, 0, sizeof(pkt));

        durability_dam_by_training[TRAINING_NONE] = 10;
        durability_dam_by_training[TRAINING_APPRENTICE] = 5;
        durability_dam_by_training[TRAINING_EXPERT] = 1;

        pkt.type = 88;
        pkt.source_oid = sub_407EF0(source_obj);
        pkt.target_oid = sub_407EF0(target_obj);
        pkt.field_40 = 1;

        int hp = object_hp_damage_get(target_obj);
        if (hp <= 0
            && (tig_art_item_id_destroyed_get(obj_field_int32_get(target_obj, OBJ_F_CURRENT_AID)) == 0
                || training != TRAINING_MASTER)) {
            pkt.field_4 |= 0x10;
            if (player_is_local_pc_obj(source_obj)) {
                if (skill_callbacks.lock_no_repair != NULL) {
                    skill_callbacks.lock_no_repair(source_obj, target_obj, is_success);
                }
            }
        } else {
            int v1;
            if (is_fate) {
                v1 = object_hp_max(target_obj);
            } else {
                v1 = effectiveness * object_hp_max(target_obj) / 100;
            }

            if (v1 <= 0) {
                if (effectiveness > 0 || (skill_invocation->flags & 0x1000) != 0) {
                    v1 = 1;
                } else {
                    v1 = 0;
                    is_success = false;
                }
            }

            int v2 = hp - v1;
            if (v2 < 0) {
                v2 = 0;
            }

            object_hp_damage_set(target_obj, v2);

            pkt.field_4 |= 0x01;
            pkt.field_8 = v2;

            int durability_dam;
            if (training == TRAINING_MASTER) {
                if (tig_art_item_id_destroyed_get(obj_field_int32_get(target_obj, OBJ_F_CURRENT_AID)) != 0) {
                    durability_dam = 5;

                    tig_art_id_t art_id = obj_field_int32_get(target_obj, OBJ_F_CURRENT_AID);
                    art_id = tig_art_item_id_destroyed_set(art_id, 0);
                    object_set_current_aid(target_obj, art_id);

                    is_success = true;
                    pkt.field_4 |= 0x02;
                } else if (is_critical && !is_success) {
                    durability_dam = 1;
                } else {
                    durability_dam = 0;
                }
            } else {
                durability_dam = durability_dam_by_training[training];
            }

            if (durability_dam > 0) {
                int dam = durability_dam * object_hp_max(target_obj) / 100;
                object_hp_adj_set(target_obj, object_hp_adj_get(target_obj) - dam);

                pkt.field_4 |= 0x04;
                pkt.field_C = object_hp_adj_get(target_obj);
            }

            pkt.field_4 |= 0x08;

            if (player_is_local_pc_obj(source_obj)) {
                if (skill_callbacks.field_14 != NULL) {
                    skill_callbacks.field_14(source_obj, target_obj, is_success);
                }
            }
        }

        if (tig_net_is_active()) {
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
        break;
    }
    case SKILL_PICK_LOCKS: {
        Packet89 pkt;

        pkt.type = 89;
        pkt.field_4 = 0;
        pkt.source_oid = sub_407EF0(source_obj);
        pkt.target_oid = sub_407EF0(target_obj);
        pkt.field_38 = is_success;

        if (object_locked_get(target_obj)) {
            if (is_success) {
                pkt.field_4 |= 0x04;
                object_locked_set(target_obj, false);
                sub_4AEE50(source_obj, target_obj, 0, 0);
            } else {
                if (is_critical) {
                    pkt.field_4 |= 0x02;
                    object_jammed_set(target_obj, true);
                }

                sub_4AEE50(source_obj, target_obj, 1, 0);
            }

            pkt.field_4 |= 0x08;
            if (sub_4C83E0(source_obj)) {
                if (skill_callbacks.lock_pick_output_func != NULL) {
                    skill_callbacks.lock_pick_output_func(source_obj, target_obj, is_success);
                }
            }
        } else {
            if (is_success) {
                pkt.field_4 |= 0x01;
                object_locked_set(target_obj, true);

                pkt.field_4 |= 0x08;

                if (player_is_local_pc_obj(source_obj)) {
                    if (skill_callbacks.lock_pick_output_func != NULL) {
                        skill_callbacks.lock_pick_output_func(source_obj, target_obj, is_success);
                    }
                }
            }
        }

        if (tig_net_is_active()) {
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
        break;
    }
    case SKILL_DISARM_TRAPS: {
        Packet91 pkt;

        pkt.type = 91;
        pkt.source_oid = sub_407EF0(source_obj);
        pkt.target_oid = sub_407EF0(target_obj);
        pkt.field_38 = is_success;
        pkt.field_3C = 0;

        if ((skill_invocation->flags & 0x04) != 0) {
            if (!is_success
                && !is_critical
                && training >= TRAINING_MASTER
                && difficulty + random_between(1, 100) <= effectiveness) {
                is_success = true;
            }

            sub_4BC7B0(source_obj, target_obj, &is_success, &is_critical);

            pkt.field_38 = is_success;
            pkt.field_3C |= 0x02;

            if (sub_4C83E0(source_obj)) {
                if (skill_callbacks.trap_output_func != NULL) {
                    skill_callbacks.trap_output_func(source_obj, target_obj, is_success);
                }
            }
        } else {
            pkt.field_3C |= 0x01;
            if (player_is_local_pc_obj(source_obj)) {
                if (skill_callbacks.field_C != NULL) {
                    skill_callbacks.field_C(source_obj, target_obj, is_success);
                }
            }
        }

        if (tig_net_is_active()) {
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
        break;
    }
    default:
        break;
    }

    if (is_success) {
        skill_invocation->flags |= SKILL_INVOCATION_SUCCESS;
    } else {
        skill_invocation->flags &= ~SKILL_INVOCATION_SUCCESS;
    }

    if (is_critical) {
        skill_invocation->flags |= SKILL_INVOCATION_CRITICAL;
    } else {
        skill_invocation->flags &= ~SKILL_INVOCATION_CRITICAL;
    }

    return true;
}

// 0x4C81A0
bool skill_invocation_check_crit_hit(int roll, int effectiveness, SkillInvocation* skill_invocation)
{
    int v1;
    int chance;

    v1 = (skill_flags[skill_invocation->skill] & 0x400) != 0 ? 20 : 2;
    chance = effectiveness / v1;

    if ((skill_invocation->flags & 0x8) != 0) {
        chance += sub_4B5F30(skill_invocation->hit_loc) / -5;
    }

    if (skill_invocation->item.obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(skill_invocation->item.obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
        && (skill_invocation->flags & 0x10000) == 0) {
        chance += item_adjust_magic(skill_invocation->item.obj,
            skill_invocation->source.obj,
            obj_field_int32_get(skill_invocation->item.obj, OBJ_F_WEAPON_MAGIC_CRIT_HIT_CHANCE));
    }

    if ((skill_invocation->flags & 0x8000) != 0) {
        chance += 2 * basic_skill_level(skill_invocation->source.obj, BASIC_SKILL_BACKSTAB);
        chance -= stat_level_get(skill_invocation->target.obj, STAT_LEVEL);

        if (basic_skill_get_training(skill_invocation->source.obj, BASIC_SKILL_BACKSTAB) == TRAINING_MASTER) {
            chance += 20;
        }
    }

    chance = effect_adjust_crit_hit_chance(skill_invocation->source.obj, chance);

    return roll <= chance;
}

// 0x4C82E0
bool skill_invocation_check_crit_miss(int roll, int effectiveness, SkillInvocation* skill_invocation)
{
    int v1;
    int chance;

    v1 = (skill_flags[skill_invocation->skill] & 0x400) != 0 ? 7 : 2;
    chance = (100 - effectiveness) / v1;

    if (skill_invocation->item.obj != OBJ_HANDLE_NULL) {
        int hp_curr = object_hp_current(skill_invocation->item.obj);
        int hp_max = object_hp_max(skill_invocation->item.obj);
        if (hp_curr < hp_max) {
            chance += 100 * (hp_max - hp_curr) / (v1 * hp_max);
        }

        if (obj_field_int32_get(skill_invocation->item.obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
            && (skill_invocation->flags & 0x10000) == 0) {
            chance += item_adjust_magic(skill_invocation->item.obj,
                skill_invocation->source.obj,
                obj_field_int32_get(skill_invocation->item.obj, OBJ_F_WEAPON_MAGIC_CRIT_MISS_CHANCE));
        }
    }

    chance = effect_adjust_crit_fail_chance(skill_invocation->source.obj, chance);

    if (chance < 2) {
        chance = 2;
    }

    return roll <= chance;
}

// 0x4C83E0
bool sub_4C83E0(int64_t obj)
{
    int64_t leader_obj;

    if (player_is_local_pc_obj(obj)) {
        return true;
    }

    leader_obj = critter_leader_get(obj);
    if (leader_obj != OBJ_HANDLE_NULL && player_is_local_pc_obj(leader_obj)) {
        return true;
    }

    return false;
}

// 0x4C8430
int sub_4C8430(SkillInvocation* skill_invocation)
{
    int64_t source_obj;
    int64_t target_obj;
    int64_t item_obj;
    int skill;
    int basic_skill;
    int tech_skill;
    int effectiveness;
    int training;
    int difficulty;

    source_obj = skill_invocation->source.obj;
    target_obj = skill_invocation->target.obj;
    item_obj = skill_invocation->item.obj;
    skill = skill_invocation->skill;

    if (IS_TECH_SKILL(skill)) {
        basic_skill = -1;
        tech_skill = GET_TECH_SKILL(skill);
        effectiveness = sub_4C69F0(source_obj, tech_skill, target_obj);
        training = tech_skill_get_training(source_obj, tech_skill);
    } else {
        basic_skill = GET_BASIC_SKILL(skill);
        tech_skill = -1;
        effectiveness = sub_4C62E0(source_obj, basic_skill, target_obj);
        training = basic_skill_get_training(source_obj, basic_skill);
    }

    difficulty = skill_invocation->modifier;
    if (target_obj != OBJ_HANDLE_NULL) {
        skill_invocation->target_loc = obj_field_int64_get(target_obj, OBJ_F_LOCATION);
    }

    if (item_obj != OBJ_HANDLE_NULL
        && obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON) {
        int to_hit = obj_field_int32_get(item_obj, OBJ_F_WEAPON_BONUS_TO_HIT);
        if ((skill_invocation->flags & 0x10000) == 0) {
            to_hit += item_adjust_magic(item_obj,
                source_obj,
                obj_field_int32_get(item_obj, OBJ_F_WEAPON_MAGIC_HIT_ADJ));
        }

        difficulty -= to_hit;

        int strength = stat_level_get(source_obj, STAT_STRENGTH) - item_weapon_min_strength(item_obj, source_obj);
        if (strength < 0) {
            difficulty += -strength * 5;
            skill_invocation->flags |= SKILL_INVOCATION_PENALTY_MSR;
        }
    }

    int target_type;
    if (target_obj != OBJ_HANDLE_NULL) {
        target_type = obj_field_int32_get(target_obj, OBJ_F_TYPE);
    }

    if ((skill_flags[skill] & 0x08) != 0
        && target_obj != OBJ_HANDLE_NULL
        && obj_type_is_critter(target_type)) {
        unsigned int critter_flags = obj_field_int32_get(target_obj, OBJ_F_CRITTER_FLAGS);

        if ((critter_flags & OCF_PARALYZED) != 0) {
            difficulty -= 50;
        } else if (critter_is_unconscious(target_obj)) {
            difficulty -= 50;
        } else if (tig_art_id_anim_get(obj_field_int32_get(target_obj, OBJ_F_CURRENT_AID)) == 7
            || (critter_flags & OCF_STUNNED) != 0) {
            difficulty -= 30;
        } else if ((critter_flags & OCF_SLEEPING) != 0) {
            difficulty -= 50;
        } else if (basic_skill != BASIC_SKILL_PROWLING
            && !combat_critter_is_combat_mode_active(target_obj)
            && ai_can_see(target_obj, source_obj) != 0
            && ai_can_hear(target_obj, source_obj, 1) != 0) {
            difficulty -= 30;
        }
    }

    if ((skill_flags[skill] & 0x01) != 0 && target_obj != OBJ_HANDLE_NULL) {
        if ((skill_invocation->flags & 0x08) != 0) {
            int v1 = sub_4B5F30(skill_invocation->hit_loc);
            if (tech_skill == TECH_SKILL_FIREARMS
                && training >= TRAINING_EXPERT) {
                v1 = 2 * v1 / 3;
            }
            difficulty -= v1;
        }

        if ((skill_invocation->flags & 0x8000) == 0
            || basic_skill_get_training(source_obj, BASIC_SKILL_BACKSTAB) == TRAINING_NONE) {
            difficulty += effectiveness * (object_get_ac(target_obj, false) / 2) / 100;
        }
    }

    if ((skill_flags[skill] & 0x02) != 0) {
        int64_t dist = location_dist(skill_invocation->target_loc, obj_field_int64_get(source_obj, OBJ_F_LOCATION));
        if (dist > INT_MAX) {
            return 1000000;
        }

        if (item_obj != OBJ_HANDLE_NULL) {
            int range;
            if (basic_skill == BASIC_SKILL_THROWING) {
                range = item_throwing_distance(item_obj, source_obj);
            } else {
                range = obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
                    ? item_weapon_range(item_obj, source_obj)
                    : 10;
            }

            if (dist > range) {
                difficulty += 1000000;
                skill_invocation->flags |= SKILL_INVOCATION_PENALTY_RANGE;
            }
        }

        if (basic_skill != BASIC_SKILL_BOW
            && basic_skill != BASIC_SKILL_THROWING
            && (tech_skill != TECH_SKILL_FIREARMS || training < TRAINING_MASTER)) {
            int extra_dist = (int)dist - stat_level_get(source_obj, STAT_PERCEPTION) / 2;
            if (extra_dist > 0) {
                difficulty += 5 * extra_dist;
                skill_invocation->flags |= SKILL_INVOCATION_PENALTY_PERCEPTION;
            }
        }

        int64_t blocking_obj;
        int v2 = sub_4ADE00(source_obj, skill_invocation->target_loc, &blocking_obj);
        if (blocking_obj != OBJ_HANDLE_NULL) {
            difficulty += 1000000;
            skill_invocation->flags |= SKILL_INVOCATION_BLOCKED_SHOT;
        } else if (v2 > 0) {
            difficulty += v2;
            skill_invocation->flags |= SKILL_INVOCATION_PENALTY_COVER;
        }
    }

    unsigned int critter_flags = obj_field_int32_get(source_obj, OBJ_F_CRITTER_FLAGS);
    if ((critter_flags & OCF_UNDEAD) == 0
        && ((skill_flags[skill] & 0x04) != 0 || (skill_invocation->flags & 0x4000) != 0)) {
        bool v3 = true;
        if ((basic_skill == BASIC_SKILL_MELEE
            || tech_skill == TECH_SKILL_PICK_LOCKS) && training >= TRAINING_EXPERT) {
            v3 = false;
        }

        if ((((skill_invocation->flags & 0x4000) == 0
                && basic_skill != BASIC_SKILL_SPOT_TRAP
                && tech_skill != TECH_SKILL_DISARM_TRAPS)
            || training < TRAINING_APPRENTICE) && v3) {
            int v4;
            if ((skill_invocation->flags & 0x4000) != 0) {
                v4 = sub_4DCE10(source_obj) & 0xFF;
            } else if (target_obj != OBJ_HANDLE_NULL) {
                v4 = sub_4DCE10(target_obj) & 0xFF;
            } else {
                v4 = sub_4D9240(skill_invocation->target_loc, 0, 0) & 0xFF;
            }

            int v5 = 30 * (255 - v4) / 255;

            if ((critter_flags & OCF_LIGHT_XLARGE) != 0) {
                v5 /= 4;
            } else if ((critter_flags & OCF_LIGHT_LARGE) != 0) {
                v5 /= 3;
            } else if ((critter_flags & OCF_LIGHT_MEDIUM) != 0) {
                v5 /= 2;
            } else if ((critter_flags & OCF_LIGHT_SMALL) != 0) {
                v5 = 2 * v5 / 3;
            }

            if ((skill_invocation->flags & 0x4000) != 0) {
                if (!critter_has_dark_sight(target_obj)) {
                    v5 = 30 - v5;
                }
            } else {
                if (critter_has_dark_sight(source_obj)) {
                    v5 = 30 - v5;
                }
            }

            if (v5 > 0) {
                difficulty += v5;
                skill_invocation->flags |= SKILL_INVOCATION_PENALTY_LIGHT;
            }
        }
    }

    if ((skill_flags[skill] & 0x80) != 0
        && (critter_flags & OCF_BLINDED) != 0) {
        difficulty += 30;
        skill_invocation->flags |= SKILL_INVOCATION_PENALTY_INJURY;
    }

    if ((skill_flags[skill] & 0x100) != 0) {
        if ((critter_flags & OCF_CRIPPLED_ARMS_BOTH) != 0) {
            difficulty += 50;
            skill_invocation->flags |= SKILL_INVOCATION_PENALTY_INJURY;
        } else if ((critter_flags & OCF_CRIPPLED_ARMS_ONE) != 0) {
            difficulty += 20;
            skill_invocation->flags |= SKILL_INVOCATION_PENALTY_INJURY;
        }
    }

    if ((skill_flags[skill] & 0x200) != 0) {
        if ((critter_flags & OCF_CRIPPLED_LEGS_BOTH) != 0) {
            difficulty += 30;
            skill_invocation->flags |= SKILL_INVOCATION_PENALTY_INJURY;
        }
    }

    switch (skill) {
    case SKILL_PICK_POCKET: {
        int inventory_location = item_inventory_location_get(item_obj);
        if (IS_WEAR_INV_LOC(inventory_location)) {
            difficulty += dword_5B70E4[inventory_location - FIRST_WEAR_INV_LOC];
        }

        int width;
        int height;
        item_inv_icon_size(item_obj, &width, &height);

        int penalty = 5 * width * height;
        if (training >= TRAINING_EXPERT) {
            penalty /= 2;
        }

        difficulty += penalty;
        break;
    }
    case SKILL_PROWLING: {
        if ((skill_invocation->flags & 0x4000) != 0) {
            int64_t blocking_obj;
            int v6 = sub_4ADE00(target_obj,
                obj_field_int64_get(source_obj, OBJ_F_LOCATION),
                &blocking_obj);
            if (blocking_obj != OBJ_HANDLE_NULL) {
                difficulty = -100;
                break;
            }

            if (training < TRAINING_EXPERT && v6 == 0) {
                difficulty += 50;
            }
        } else if ((skill_invocation->flags & 0x2000) != 0) {
            for (int inventory_location = FIRST_WEAR_INV_LOC; inventory_location <= LAST_WEAR_INV_LOC; inventory_location++) {
                int64_t inv_item_obj = item_wield_get(source_obj, inventory_location);
                if (inv_item_obj != OBJ_HANDLE_NULL
                    && obj_field_int32_get(inv_item_obj, OBJ_F_TYPE) == OBJ_TYPE_ARMOR) {
                    int penalty = obj_field_int32_get(inv_item_obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ);
                    penalty += item_adjust_magic(inv_item_obj,
                        source_obj,
                        obj_field_int32_get(inv_item_obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ));
                    if (training >= TRAINING_APPRENTICE) {
                        penalty /= 2;
                    }
                    difficulty -= penalty;
                }
            }
        }
        break;
    }
    case SKILL_HEAL:
        if (item_obj != OBJ_HANDLE_NULL
            && obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_GENERIC
            && (obj_field_int32_get(item_obj, OBJ_F_GENERIC_FLAGS) & OGF_IS_HEALING_ITEM) != 0) {
            difficulty -= obj_field_int32_get(item_obj, OBJ_F_GENERIC_USAGE_BONUS);
        }
        break;
    case SKILL_PICK_LOCKS:
        if (target_obj != OBJ_HANDLE_NULL
            && (target_type == OBJ_TYPE_PORTAL || target_type == OBJ_TYPE_CONTAINER)) {
            if (item_obj != OBJ_HANDLE_NULL
                && obj_field_int32_get(item_obj, OBJ_F_TYPE) == OBJ_TYPE_GENERIC
                && (obj_field_int32_get(item_obj, OBJ_F_GENERIC_FLAGS) & OGF_IS_LOCKPICK) != 0) {
                difficulty -= obj_field_int32_get(item_obj, OBJ_F_GENERIC_USAGE_BONUS);
            }

            int lock_difficulty;
            if (target_type == OBJ_TYPE_PORTAL) {
                lock_difficulty = obj_field_int32_get(target_obj, OBJ_F_PORTAL_LOCK_DIFFICULTY);
            } else {
                lock_difficulty = obj_field_int32_get(target_obj, OBJ_F_CONTAINER_LOCK_DIFFICULTY);
            }

            if (training >= TRAINING_MASTER && lock_difficulty > 0) {
                lock_difficulty /= 2;
            }

            difficulty += lock_difficulty;
        } else {
            difficulty += 1000000;
        }
        break;
    case SKILL_DISARM_TRAPS:
        if (target_obj != OBJ_HANDLE_NULL
            && target_type == OBJ_TYPE_TRAP) {
            difficulty += obj_field_int32_get(target_obj, OBJ_F_TRAP_DIFFICULTY);
        }
        break;
    }

    return difficulty;
}

// 0x4C8E60
void skill_perform_repair_service(int64_t item_obj, int64_t npc_obj, int64_t pc_obj, int cost)
{
    SkillInvocation skill_invocation;

    if (tig_net_is_active() && !tig_net_is_host()) {
        PacketPerformRepairService pkt;

        pkt.type = 126;
        sub_4F0640(item_obj, &(pkt.item_oid));
        sub_4F0640(npc_obj, &(pkt.npc_oid));
        sub_4F0640(pc_obj, &(pkt.pc_oid));
        pkt.cost = cost;
        tig_net_send_app_all(&pkt, sizeof(pkt));
        return;
    }

    skill_invocation_init(&skill_invocation);
    skill_invocation.flags |= 0x1000;
    skill_invocation.skill = SKILL_REPAIR;
    sub_4440E0(npc_obj, &(skill_invocation.source));
    sub_4440E0(item_obj, &(skill_invocation.target));
    skill_invocation_run(&skill_invocation);

    item_gold_transfer(pc_obj, npc_obj, cost, OBJ_HANDLE_NULL);

    sub_4EE3A0(pc_obj, item_obj);
}

// 0x4C8FA0
bool get_follower_skills(int64_t obj)
{
    int player;

    if (!tig_net_is_active()) {
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

    if (tig_net_is_active()) {
        player = sub_4A2B10(player_get_local_pc_obj());
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
void skill_pick_best_follower(SkillInvocation* skill_invocation)
{
    ObjectList objects;
    ObjectNode* node;
    int efficiency;
    int best_efficiency;
    int64_t best_follower_obj;

    if (obj_field_int32_get(skill_invocation->source.obj, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    if (!get_follower_skills(skill_invocation->source.obj)) {
        return;
    }

    if (IS_TECH_SKILL(skill_invocation->skill)) {
        best_efficiency = sub_4C69F0(skill_invocation->source.obj, GET_TECH_SKILL(skill_invocation->skill), skill_invocation->target.obj) - sub_4C8430(skill_invocation);
        best_follower_obj = skill_invocation->source.obj;
        object_list_all_followers(best_follower_obj, &objects);
        node = objects.head;
        while (node != NULL) {
            sub_4440E0(node->obj, &(skill_invocation->source));
            efficiency = sub_4C69F0(skill_invocation->source.obj, GET_TECH_SKILL(skill_invocation->skill), skill_invocation->target.obj) - sub_4C8430(skill_invocation);
            if (efficiency > best_efficiency) {
                best_efficiency = efficiency;
                best_follower_obj = node->obj;
            }
            node = node->next;
        }

        // FIX: Original code leaks `objects`.
        object_list_destroy(&objects);
    } else {
        best_efficiency = sub_4C62E0(skill_invocation->source.obj, GET_BASIC_SKILL(skill_invocation->skill), skill_invocation->target.obj) - sub_4C8430(skill_invocation);
        best_follower_obj = skill_invocation->source.obj;
        object_list_all_followers(best_follower_obj, &objects);
        node = objects.head;
        while (node != NULL) {
            sub_4440E0(node->obj, &(skill_invocation->source));
            efficiency = sub_4C62E0(skill_invocation->source.obj, GET_BASIC_SKILL(skill_invocation->skill), skill_invocation->target.obj) - sub_4C8430(skill_invocation);
            if (efficiency > best_efficiency) {
                best_efficiency = efficiency;
                best_follower_obj = node->obj;
            }

            node = node->next;
        }

        // FIX: Original code leaks `objects`.
        object_list_destroy(&objects);
    }

    sub_4440E0(best_follower_obj, &(skill_invocation->source));
}

// 0x4C91F0
int64_t skill_supplementary_item(int64_t obj, int skill)
{
    int64_t item_obj = OBJ_HANDLE_NULL;
    int64_t substitute_inventory_obj;

    switch (skill) {
    case SKILL_HEAL:
        item_obj = item_find_first_generic(obj, OGF_IS_HEALING_ITEM);
        if (item_obj == OBJ_HANDLE_NULL) {
            substitute_inventory_obj = critter_substitute_inventory_get(obj);
            if (substitute_inventory_obj != OBJ_HANDLE_NULL) {
                item_obj = item_find_first_generic(substitute_inventory_obj, OGF_IS_HEALING_ITEM);
            }
        }
        break;
    case SKILL_PICK_LOCKS:
        item_obj = item_find_first_generic(obj, OGF_IS_LOCKPICK);
        if (item_obj == OBJ_HANDLE_NULL) {
            substitute_inventory_obj = critter_substitute_inventory_get(obj);
            if (substitute_inventory_obj != OBJ_HANDLE_NULL) {
                item_obj = item_find_first_generic(substitute_inventory_obj, OGF_IS_LOCKPICK);
            }
        }
        break;
    }

    return item_obj;
}
