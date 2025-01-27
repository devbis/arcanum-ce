#include "game/effect.h"

#include "game/anim.h"
#include "game/critter.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/resistance.h"
#include "game/skill.h"
#include "game/stat.h"
#include "game/tech.h"

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
} EffectSpecial;

typedef enum EffectOperator {
    EFFECT_OPERATOR_ADD,
    EFFECT_OPERATOR_MULTIPLY,
    EFFECT_OPERATOR_DIVIDE,
    EFFECT_OPERATOR_MIN,
    EFFECT_OPERATOR_MAX,
    EFFECT_OPERATOR_PERCENT,
} EffectOperator;

typedef struct Effect {
    int count;
    int ids[7];
    int params[7];
    int operators[7];
} Effect;

// See 0x4E99F0.
static_assert(sizeof(Effect) == 0x58, "wrong size");

static void effect_parse(int num, char* text);
static void sub_4EA520(int64_t obj, int start);
static int sub_4EA6C0(int64_t obj, int id, int value, Effect* effect, bool a5);

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

static_assert(sizeof(off_5B9C18) / sizeof(off_5B9C18[0]) == BASIC_SKILL_COUNT, "wrong size");

// 0x5B9C48
static const char* off_5B9C48[] = {
    "repair",
    "firearms",
    "picklock",
    "armtrap",
};

static_assert(sizeof(off_5B9C48) / sizeof(off_5B9C48[0]) == TECH_SKILL_COUNT, "wrong size");

// 0x5B9C58
static const char* off_5B9C58[] = {
    "resistdamage",
    "resistfire",
    "resistelectrical",
    "resistpoison",
    "resistmagic",
};

static_assert(sizeof(off_5B9C58) / sizeof(off_5B9C58[0]) == RESISTANCE_TYPE_COUNT, "wrong size");

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

// 0x5B9CB4
const char* off_5B9CB4[EFFECT_CAUSE_COUNT] = {
    "Race",
    "Background",
    "Class",
    "Bless",
    "Curse",
    "Item",
    "Spell",
    "Injury",
    "Tech",
    "Gender",
};

static_assert(sizeof(off_5B9CB4) / sizeof(off_5B9CB4[0]) == EFFECT_CAUSE_COUNT, "wrong size");

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
bool effect_init(GameInitInfo* init_info)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    (void)init_info;

    if (!mes_load("rules\\effect.mes", &mes_file)) {
        return false;
    }

    effect_stat_effects = (Effect*)CALLOC(EFFECT_LIST_CAPACITY, sizeof(*effect_stat_effects));
    effect_basic_skill_effects = (Effect*)CALLOC(EFFECT_LIST_CAPACITY, sizeof(*effect_basic_skill_effects));
    effect_tech_skill_effects = (Effect*)CALLOC(EFFECT_LIST_CAPACITY, sizeof(*effect_tech_skill_effects));
    effect_resistance_effects = (Effect*)CALLOC(EFFECT_LIST_CAPACITY, sizeof(*effect_resistance_effects));
    effect_tech_effects = (Effect*)CALLOC(EFFECT_LIST_CAPACITY, sizeof(*effect_tech_effects));
    effect_special_effects = (Effect*)CALLOC(EFFECT_LIST_CAPACITY, sizeof(*effect_special_effects));

    for (mes_file_entry.num = 50; mes_file_entry.num < 400; mes_file_entry.num++) {
        if (mes_search(mes_file, &mes_file_entry)) {
            effect_parse(mes_file_entry.num, mes_file_entry.str);
        }
    }

    mes_unload(mes_file);
    effect_initialized = true;

    return true;
}

// 0x4E9AE0
void effect_exit()
{
    FREE(effect_stat_effects);
    FREE(effect_basic_skill_effects);
    FREE(effect_tech_skill_effects);
    FREE(effect_resistance_effects);
    FREE(effect_tech_effects);
    FREE(effect_special_effects);
    effect_initialized = false;
}

// 0x004E9B40
bool effect_mod_load()
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    if (mes_load("rules\\gameeffect.mes", &mes_file)) {
        for (mes_file_entry.num = 0; mes_file_entry.num < 50; mes_file_entry.num++) {
            if (mes_search(mes_file, &mes_file_entry)) {
                effect_parse(mes_file_entry.num, mes_file_entry.str);
            }
        }

        mes_unload(mes_file);
    }

    return true;
}

// 0x4E9BB0
void effect_mod_unload()
{
    int index;

    for (index = 0; index < 50; index++) {
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
    char* tok;
    Effect* effect_info;
    int type;
    int index;

    tok = strtok(text, " ,");
    while (tok != NULL) {
        // NOTE: Original code is different. Probably uses an inlined routine or
        // a tree of loops.
        effect_info = NULL;
        for (type = 0; type < 6 && effect_info == NULL; type++) {
            switch (type) {
            case 0:
                for (index = 0; index < STAT_COUNT; index++) {
                    if (_strcmpi(tok, off_5B9BA8[index]) == 0) {
                        effect_info = &(effect_stat_effects[num]);
                        effect_info->ids[effect_info->count] = index;
                        break;
                    }
                }
                break;
            case 1:
                for (index = 0; index < BASIC_SKILL_COUNT; index++) {
                    if (_strcmpi(tok, off_5B9C18[index]) == 0) {
                        effect_info = &(effect_basic_skill_effects[num]);
                        effect_info->ids[effect_info->count] = index;
                        break;
                    }
                }
                break;
            case 2:
                for (index = 0; index < TECH_SKILL_COUNT; index++) {
                    if (_strcmpi(tok, off_5B9C48[index]) == 0) {
                        effect_info = &(effect_tech_skill_effects[num]);
                        effect_info->ids[effect_info->count] = index;
                        break;
                    }
                }
                break;
            case 3:
                for (index = 0; index < 5; index++) {
                    if (_strcmpi(tok, off_5B9C58[index]) == 0) {
                        effect_info = &(effect_resistance_effects[num]);
                        effect_info->ids[effect_info->count] = index;
                        break;
                    }
                }
                break;
            case 4:
                for (index = 0; index < TECH_COUNT; index++) {
                    if (_strcmpi(tok, off_5B9C6C[index]) == 0) {
                        effect_info = &(effect_tech_effects[num]);
                        effect_info->ids[effect_info->count] = index;
                        break;
                    }
                }
                break;
            case 5:
                for (index = 0; index < EFFECT_SPECIAL_COUNT; index++) {
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

            if (tok[strlen(tok) - 1] == '%') {
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

// 0x4E9F70
void effect_add(int64_t obj, int effect, int cause)
{
    int strength;
    int encumbrance_level;
    int v1;
    int v2;
    int diff;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            Packet86 pkt;

            pkt.type = 86;
            pkt.subtype = 0;
            pkt.oid = sub_407EF0(obj);
            pkt.field_20 = effect;
            pkt.field_24 = cause;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        } else {
            if (!multiplayer_is_locked()) {
                return;
            }
        }
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return;
    }

    strength = stat_level_get(obj, STAT_STRENGTH);
    encumbrance_level = critter_encumbrance_level_get(obj);
    v1 = object_hp_max(obj);
    v2 = critter_fatigue_max(obj);

    obj_arrayfield_uint32_set(obj,
        OBJ_F_CRITTER_EFFECTS_IDX,
        obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX),
        effect);
    obj_arrayfield_uint32_set(obj,
        OBJ_F_CRITTER_EFFECT_CAUSE_IDX,
        obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX),
        cause);

    if (strength != stat_level_get(obj, STAT_STRENGTH)) {
        critter_encumbrance_level_recalc(obj, encumbrance_level);
    }

    diff = object_hp_max(obj) - v1;
    if (diff != 0) {
        object_hp_damage_set(obj, object_hp_damage_get(obj) + diff);
    }

    diff = critter_fatigue_max(obj) - v2;
    if (diff != 0) {
        critter_fatigue_damage_set(obj, critter_fatigue_damage_get(obj) + diff);
    }

    sub_436FA0(obj);
}

// 0x4EA100
void effect_remove_one_typed(int64_t obj, int effect)
{
    int cnt;
    int index;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            Packet86 pkt;

            pkt.type = 86;
            pkt.subtype = 1;
            pkt.oid = sub_407EF0(obj);
            pkt.field_20 = effect;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        } else {
            if (!multiplayer_is_locked()) {
                return;
            }
        }
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECTS_IDX, index) == effect) {
            sub_4EA520(obj, index);
            break;
        }
    }

    sub_436FA0(obj);
}

// 0x4EA200
void effect_remove_all_typed(int64_t obj, int effect)
{
    int cnt;
    int index;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            Packet86 pkt;

            pkt.type = 86;
            pkt.subtype = 2;
            pkt.oid = sub_407EF0(obj);
            pkt.field_20 = effect;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        } else {
            if (!multiplayer_is_locked()) {
                return;
            }
        }
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECTS_IDX, index) == effect) {
            sub_4EA520(obj, index);
            index--;
            cnt--;
        }
    }
}

// 0x4EA2E0
void effect_remove_one_caused_by(int64_t obj, int cause)
{
    int cnt;
    int index;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            Packet86 pkt;

            pkt.type = 86;
            pkt.subtype = 3;
            pkt.oid = sub_407EF0(obj);
            pkt.field_20 = cause;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        } else {
            if (!multiplayer_is_locked()) {
                return;
            }
        }
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX, index) == cause) {
            sub_4EA520(obj, index);
            break;
        }
    }
}

// 0x4EA3C0
void effect_remove_all_caused_by(int64_t obj, int cause)
{
    int cnt;
    int index;

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        if ((tig_net_flags & TIG_NET_HOST) != 0) {
            Packet86 pkt;

            pkt.type = 86;
            pkt.subtype = 4;
            pkt.oid = sub_407EF0(obj);
            pkt.field_20 = cause;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        } else {
            if (!multiplayer_is_locked()) {
                return;
            }
        }
    }

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX);
    for (index = 0; index < cnt; index++) {
        if (obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX, index) == cause) {
            sub_4EA520(obj, index);
            index--;
            cnt--;
        }
    }
}

// 0x4EA4A0
int sub_4EA4A0(object_id_t obj, int effect_id)
{
    int effect_count = 0;
    int index;
    int cnt;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        cnt = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX);
        for (index = 0; index < cnt; index++) {
            if (obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECTS_IDX, index) == effect_id) {
                effect_count++;
            }
        }
    }

    return effect_count;
}

// 0x4EA520
void sub_4EA520(int64_t obj, int start)
{
    int strength;
    int encumbrance_level;
    int v1;
    int v2;
    int end;
    int data;
    int diff;

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return;
    }

    strength = stat_level_get(obj, STAT_STRENGTH);
    encumbrance_level = critter_encumbrance_level_get(obj);
    v1 = object_hp_max(obj);
    v2 = critter_fatigue_max(obj);

    end = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX) - 1;
    while (start < end) {
        data = obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECTS_IDX, start + 1);
        obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_EFFECTS_IDX, start, data);

        data = obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX, start + 1);
        obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX, start, data);
    }

    obj_arrayfield_length_set(obj, OBJ_F_CRITTER_EFFECTS_IDX, end);
    obj_arrayfield_length_set(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX, end);

    if (strength != stat_level_get(obj, STAT_STRENGTH)) {
        critter_encumbrance_level_recalc(obj, encumbrance_level);
    }

    diff = object_hp_max(obj) - v1;
    if (diff != 0) {
        object_hp_damage_set(obj, object_hp_damage_get(obj) + diff);
    }

    diff = critter_fatigue_max(obj) - v2;
    if (diff != 0) {
        critter_fatigue_damage_set(obj, critter_fatigue_damage_get(obj) + diff);
    }
}

// 0x4EA690
int effect_adjust_stat_level(object_id_t obj, int stat, int value)
{
    return sub_4EA6C0(obj, stat, value, effect_stat_effects, false);
}

// 0x4EA6C0
int sub_4EA6C0(int64_t obj, int id, int value, Effect* tbl, bool a5)
{
    int cnt;
    int index;
    int effect;
    int cause;
    Effect* meta;
    int change;
    int adds = 0;
    int mults = 1;
    int divs = 1;
    int min = -1;
    int max = -1;
    int percents = 0;

    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    cnt = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX);
    if (cnt <= 0) {
        return value;
    }

    for (index = 0; index < cnt; index++) {
        effect = obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECTS_IDX, index);
        if (a5) {
            cause = obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECT_CAUSE_IDX, index);
            switch (cause) {
            case EFFECT_CAUSE_RACE:
            case EFFECT_CAUSE_BACKGROUND:
            case EFFECT_CAUSE_CLASS:
            case EFFECT_CAUSE_GENDER:
                continue;
            }
        }

        meta = &(tbl[effect]);
        for (change = 0; change < meta->count; change++) {
            if (meta->ids[change] == id) {
                switch (meta->operators[change]) {
                case EFFECT_OPERATOR_ADD:
                    adds += meta->params[change];
                    break;
                case EFFECT_OPERATOR_MULTIPLY:
                    mults *= meta->params[change];
                    break;
                case EFFECT_OPERATOR_DIVIDE:
                    divs *= meta->params[change];
                    break;
                case EFFECT_OPERATOR_MIN:
                    if (min == -1 || min < meta->params[change]) {
                        min = meta->params[change];
                    }
                    break;
                case EFFECT_OPERATOR_MAX:
                    if (max == -1 || max > meta->params[change]) {
                        max = meta->params[change];
                    }
                    break;
                case EFFECT_OPERATOR_PERCENT:
                    percents += meta->params[change];
                    break;
                }
            }
        }
    }

    if (percents != 0) {
        value = value * (percents + 100) / 100;
    }

    if (mults != 1) {
        value *= mults;
    }

    if (divs != 1) {
        value /= divs;
    }

    if (adds != 0) {
        value += adds;
    }

    if (min != -1 && value < min) {
        value = min;
    }

    if (max != -1 && value > max) {
        value = max;
    }

    return value;
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
    int cnt;
    int index;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        cnt = obj_arrayfield_length_get(obj, OBJ_F_CRITTER_EFFECTS_IDX);
        tig_debug_println("\tEffect Debug Obj:\n");
        tig_debug_printf("\tEffect Count: %d\n", cnt);

        for (index = 0; index < cnt; index++) {
            tig_debug_printf("\t\tEffect %d: ID: %d\n",
                index,
                obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_EFFECTS_IDX, index));
        }
    }
}
