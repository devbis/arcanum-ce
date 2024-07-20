#include "game/lib/effect.h"

#include "game/lib/message.h"
#include "game/lib/skill.h"
#include "game/lib/stat.h"
#include "game/lib/tech.h"
#include "tig/debug.h"

#define EFFECT_LIST_CAPACITY 400

typedef enum EffectSpecial {
    EFFECT_SPECIAL_MAX_HIT_POINTS,
    EFFECT_SPECIAL_MAX_FATIGUE,
    EFFECT_SPECIAL_REACTION,
    EFFECT_SPECIAL_CRIT_HIT_CHANCE,
    EFFECT_SPECIAL_CRIT_HIT_EFFECT,
    EFFECT_SPECIAL_CRIT_FAIL_CHANCE,
    EFFECT_SPECIAL_CRIT_FAIL_EFFECT,
    EFFECT_SPECIAL_BAD_REACTION_ADJUSTMENT,
    EFFECT_SPECIAL_GOOD_REACTION_ADJUSTMENT,
    EFFECT_SPECIAL_XP_GAIN,
    EFFECT_SPECIAL_COUNT,
};

typedef enum EffectOperator {
    EFFECT_OPERATOR_ADD,
    EFFECT_OPERATOR_MULTIPLY,
    EFFECT_OPERATOR_DIVIDE,
    EFFECT_OPERATOR_MIN,
    EFFECT_OPERATOR_MAX,
    EFFECT_OPERATOR_PERCENT,
};

typedef struct Effect {
    int count;
    int ids[7];
    int params[7];
    int operators[7];
};

// See 0x4E99F0.
static_assert(sizeof(Effect) == 0x58, "wrong size");

static void effect_parse(int num, char* text);

// 0x5B9BA8
static const char* off_5B9BA8[] = {
    "st",
    "dx",
    "cn",
    "be",
    "in",
    "pe",
    "wp",
    "ch",
    "carry",
    "damage",
    "ac",
    "speed",
    "healrate",
    "poisonrate",
    "beautyreaction",
    "maxfollowers",
    "aptitude",
    "level",
    "xpsUNUSED",
    "alignment",
    "fateUNUSED",
    "unspentUNUSED",
    "magicpts",
    "techpts",
    "poisonlevel",
    "age",
    "gender",
    "race",
};

static_assert(sizeof(off_5B9BA8) / sizeof(off_5B9BA8[0]) == STAT_COUNT, "wrong size");

// 0x5B9C18
static const char* off_5B9C18[] = {
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

static_assert(sizeof(off_5B9C18) / sizeof(off_5B9C18[0]) == PRIMARY_SKILL_COUNT, "wrong size");

// 0x5B9C48
static const char* off_5B9C48[] = {
    "repair",
    "firearms",
    "picklock",
    "armtrap",
};

static_assert(sizeof(off_5B9C48) / sizeof(off_5B9C48[0]) == SECONDARY_SKILL_COUNT, "wrong size");

// 0x5B9C58
static const char* off_5B9C58[] = {
    "resistdamage",
    "resistfire",
    "resistelectrical",
    "resistpoison",
    "resistmagic",
};

// TODO: Figure out proper enum.
static_assert(sizeof(off_5B9C58) / sizeof(off_5B9C58[0]) == 0, "wrong size");

// 0x5B9C6C
static const char* off_5B9C6C[] = {
    "expertiseanatomical",
    "expertisechemistry",
    "expertiseelectric",
    "expertiseexplosives",
    "expertisegun_smithy",
    "expertisemechanical",
    "expertisesmithy",
    "expertisetherapeutics",
};

static_assert(sizeof(off_5B9C6C) / sizeof(off_5B9C6C[0]) == TECH_COUNT, "wrong size");

// 0x5B9C8C
static const char* off_5B9C8C[] = {
    "maxhps",
    "maxfatigue",
    "reaction",
    "crithitchance",
    "crithiteffect",
    "critfailchance",
    "critfaileffect",
    "badreactionadj",
    "goodreactionadj",
    "xpgain",
};

// TODO: Figure out proper enum.
static_assert(sizeof(off_5B9C8C) / sizeof(off_5B9C8C[0]) == EFFECT_SPECIAL_COUNT, "wrong size");

// 0x603ADC
static bool effect_initialized;

// 0x603AC4
static Effect* effect_resistance_effects;

// 0x603AC8
static Effect* effect_stat_effects;

// 0x603ACC
static Effect* effect_tech_effects;

// 0x603AD0
static Effect* effect_tech_skill_effects;

// 0x603AD4
static Effect* effect_basic_skill_effects;

// 0x603AD8
static Effect* effect_special_effects;

// 0x4E99F0
bool effect_init(GameContext* ctx)
{
    int msg_file;
    MessageListItem msg;

    if (!message_load("rules\\effect.mes", &msg_file)) {
        return false;
    }

    effect_stat_effects = (Effect*)calloc(EFFECT_LIST_CAPACITY, sizeof(*effect_stat_effects));
    effect_basic_skill_effects = (Effect*)calloc(EFFECT_LIST_CAPACITY, sizeof(*effect_basic_skill_effects));
    effect_tech_skill_effects = (Effect*)calloc(EFFECT_LIST_CAPACITY, sizeof(*effect_tech_skill_effects));
    effect_resistance_effects = (Effect*)calloc(EFFECT_LIST_CAPACITY, sizeof(*effect_resistance_effects));
    effect_tech_effects = (Effect*)calloc(EFFECT_LIST_CAPACITY, sizeof(*effect_tech_effects));
    effect_special_effects = (Effect*)calloc(EFFECT_LIST_CAPACITY, sizeof(*effect_special_effects));

    for (msg.num = 50; msg.num < 400; msg.num++) {
        if (message_find(msg_file, &msg)) {
            effect_parse(msg.num, msg.text);
        }
    }

    message_unload(msg_file);
    effect_initialized = true;

    return true;
}

// 0x4E9AE0
void effect_exit()
{
    free(effect_stat_effects);
    free(effect_basic_skill_effects);
    free(effect_tech_skill_effects);
    free(effect_resistance_effects);
    free(effect_tech_effects);
    free(effect_special_effects);
    effect_initialized = false;
}

// 0x004E9B40
bool effect_mod_load()
{
    int msg_file;
    MessageListItem msg;

    if (message_load("rules\\gameeffect.mes", &msg_file)) {
        for (msg.num = 0; msg.num < 50; msg.num++) {
            if (message_find(msg_file, &msg)) {
                effect_parse(msg.num, msg.text);
            }
        }

        message_unload(msg_file);
    }

    return true;
}

// 0x4E9BB0
void effect_mod_unload()
{
    for (int index = 0; index < 50; index++) {
        effect_stat_effects[index].count = 0;
        effect_basic_skill_effects[index].count = 0;
        effect_tech_skill_effects[index].count = 0;
        effect_resistance_effects[index].count = 0;
        effect_tech_effects[index].count = 0;
        effect_special_effects[index].count = 0;
    }
}

// 0x4E9C20
static void effect_parse(int num, char* text)
{
    char* tok = strtok(text, " ,");
    while (tok != NULL) {
        // NOTE: Original code is different. Probably uses an inlined routine or
        // a tree of loops.
        Effect* effect_info = NULL;
        for (int type = 0; type < 6 && effect_info == NULL; type++) {
            switch (type) {
            case 0:
                for (int stat = 0; stat < STAT_COUNT; stat++) {
                    if (_strcmpi(tok, off_5B9BA8[stat]) == 0) {
                        effect_info = &(effect_stat_effects[num]);
                        effect_info->ids[effect_info->count] = stat;
                        break;
                    }
                }
                break;
            case 1:
                for (int skill = 0; skill < PRIMARY_SKILL_COUNT; skill++) {
                    if (_strcmpi(tok, off_5B9C18[skill]) == 0) {
                        effect_info = &(effect_basic_skill_effects[num]);
                        effect_info->ids[effect_info->count] = skill;
                        break;
                    }
                }
                break;
            case 2:
                for (int skill = 0; skill < SECONDARY_SKILL_COUNT; skill++) {
                    if (_strcmpi(tok, off_5B9C48[skill]) == 0) {
                        effect_info = &(effect_tech_skill_effects[num]);
                        effect_info->ids[effect_info->count] = skill;
                        break;
                    }
                }
                break;
            case 3:
                for (int index = 0; index < 5; index++) {
                    if (_strcmpi(tok, off_5B9C58[index]) == 0) {
                        effect_info = &(effect_resistance_effects[num]);
                        effect_info->ids[effect_info->count] = index;
                        break;
                    }
                }
                break;
            case 4:
                for (int tech = 0; tech < TECH_COUNT; tech++) {
                    if (_strcmpi(tok, off_5B9C6C[tech]) == 0) {
                        effect_info = &(effect_tech_effects[num]);
                        effect_info->ids[effect_info->count] = tech;
                        break;
                    }
                }
                break;
            case 5:
                for (int index = 0; index < EFFECT_SPECIAL_COUNT; index++) {
                    if (_strcmpi(tok, off_5B9C8C[index]) == 0) {
                        effect_info = &(effect_special_effects[num]);
                        effect_info->ids[effect_info->count] = index;
                        break;
                    }
                }
                break;
            }
        }

        if (effect_info == NULL) {
            tig_debug_printf("Effect: ERROR: Invalid symbol: %s.\n", tok);
            return;
        }

        tok = strtok(NULL, " ,");
        if (_strcmpi(tok, "min") == 0 || _strcmpi(tok, "max") == 0) {
            // FIXME: Case-sensitive compare to distinguish between min and max.
            if (tok[1] == 'i') {
                effect_info->operators[effect_info->count] = EFFECT_OPERATOR_MIN;
            } else {
                effect_info->operators[effect_info->count] = EFFECT_OPERATOR_MAX;
            }

            tok = strtok(NULL, " ,");
            effect_info->params[effect_info->count] = atoi(tok);
        } else if (tok[0] == '+' || tok[0] == '-') {
            effect_info->operators[effect_info->count] = EFFECT_OPERATOR_ADD;
            effect_info->params[effect_info->count] = atoi(tok);

            size_t tok_length = strlen(tok);
            if (tok[tok_length - 1] == '%') {
                effect_info->operators[effect_info->count] = EFFECT_OPERATOR_PERCENT;
            }
        } else if (tok[0] == '*') {
            effect_info->operators[effect_info->count] = EFFECT_OPERATOR_MULTIPLY;
            effect_info->params[effect_info->count] = atoi(tok);
        } else if (tok[0] == '/') {
            effect_info->operators[effect_info->count] = EFFECT_OPERATOR_DIVIDE;
            effect_info->params[effect_info->count] = atoi(tok);
        } else {
            tig_debug_printf("Effect: ERROR: Invalid symbol: %s.\n", tok);
        }

        effect_info->count++;
        tok = strtok(NULL, " ,");
    }
}

// 0x4EA4A0
int sub_4EA4A0(object_id_t obj, int effect_id)
{
    int count = 0;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC && obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        int effects_count = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX);
        for (int index = 0; index < effects_count; index++) {
            if (sub_407470(obj, OBJ_F_CRITTER_EFFECTS_IDX, index) == effect_id) {
                count++;
            }
        }
    }

    return count;
}

// 0x4EA690
int effect_adjust_stat_level(object_id_t obj, int stat, int value)
{
    return sub_4EA6C0(obj, stat, value, effect_stat_effects, false);
}

// 0x4EA930
int sub_4EA930(object_id_t obj, int stat, int value)
{
    return sub_4EA6C0(obj, stat, value, effect_stat_effects, true);
}

// 0x4EA960
int effect_adjust_basic_skill_level(object_id_t obj, int skill, int value)
{
    return sub_4EA6C0(obj, skill, value, effect_basic_skill_effects, false);
}

// 0x4EA990
int effect_adjust_tech_skill_level(object_id_t obj, int skill, int value)
{
    return sub_4EA6C0(obj, skill, value, effect_tech_skill_effects, false);
}

// 0x4EA9C0
int effect_adjust_resistance(object_id_t obj, int resistance, int value)
{
    return sub_4EA6C0(obj, resistance, value, effect_resistance_effects, false);
}

// 0x4EA9F0
int effect_adjust_tech_level(object_id_t obj, int tech, int value)
{
    return sub_4EA6C0(obj, tech, value, effect_tech_effects, false);
}

// 0x4EAA20
int effect_adjust_max_hit_points(object_id_t obj, int value)
{
    return sub_4EA6C0(obj, EFFECT_SPECIAL_MAX_HIT_POINTS, value, effect_special_effects, false);
}

// 0x4EAA50
int effect_adjust_max_fatigue(object_id_t obj, int value)
{
    return sub_4EA6C0(obj, EFFECT_SPECIAL_MAX_FATIGUE, value, effect_special_effects, false);
}

// 0x4EAA80
int effect_adjust_reaction(object_id_t obj, int value)
{
    return sub_4EA6C0(obj, EFFECT_SPECIAL_REACTION, value, effect_special_effects, false);
}

// 0x4EAAB0
int effect_adjust_good_bad_reaction(object_id_t obj, int value)
{
    if (value < 0) {
        return sub_4EA6C0(obj, EFFECT_SPECIAL_BAD_REACTION_ADJUSTMENT, value, effect_special_effects, false);
    } else if (value > 0) {
        return sub_4EA6C0(obj, EFFECT_SPECIAL_GOOD_REACTION_ADJUSTMENT, value, effect_special_effects, false);
    } else {
        return 0;
    }
}

// 0x4EAB00
int effect_adjust_crit_hit_chance(object_id_t obj, int value)
{
    return sub_4EA6C0(obj, EFFECT_SPECIAL_CRIT_HIT_CHANCE, value, effect_special_effects, false);
}

// 0x4EAB30
int effect_adjust_crit_hit_effect(object_id_t obj, int value)
{
    return sub_4EA6C0(obj, EFFECT_SPECIAL_CRIT_HIT_EFFECT, value, effect_special_effects, false);
}

// 0x4EAB60
int effect_adjust_crit_fail_chance(object_id_t obj, int value)
{
    return sub_4EA6C0(obj, EFFECT_SPECIAL_CRIT_FAIL_CHANCE, value, effect_special_effects, false);
}

// 0x4EAB90
int effect_adjust_crit_fail_effect(object_id_t obj, int value)
{
    return sub_4EA6C0(obj, EFFECT_SPECIAL_CRIT_FAIL_EFFECT, value, effect_special_effects, false);
}

// 0x4EABC0
int effect_adjust_xp_gain(object_id_t obj, int value)
{
    return sub_4EA6C0(obj, EFFECT_SPECIAL_XP_GAIN, value, effect_special_effects, false);
}

// 0x4EABF0
void effect_debug_obj(object_id_t obj)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        int effect_count = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX);
        tig_debug_println("\tEffect Debug Obj:\n");
        tig_debug_printf("\tEffect Count: %d\n", effect_count);

        for (int index = 0; index < effect_count; index++) {
            int effect_id = sub_407470(obj, OBJ_F_CRITTER_EFFECTS_IDX, index);
            tig_debug_printf("\t\tEffect %d: ID: %d\n", index, effect_id);
        }
    }
}
