#include "game/stat.h"

#include "game/a_name.h"
#include "game/background.h"
#include "game/effect.h"
#include "game/light.h"
#include "game/location.h"
#include "game/magictech.h"
#include "game/mes.h"
#include "game/object.h"
#include "game/sector.h"
#include "game/skill.h"
#include "game/tile.h"
#include "game/timeevent.h"
#include "game/ui.h"

static bool sub_4B1310(TimeEvent* timeevent);
static bool sub_4B1350(int64_t obj, int value, bool a3);

// 0x5B5194
static int stat_min_values[STAT_COUNT] = {
    /*             STRENGTH */ 1,
    /*            DEXTERITY */ 1,
    /*         CONSTITUTION */ 1,
    /*               BEAUTY */ 1,
    /*         INTELLIGENCE */ 1,
    /*           PERCEPTION */ 1,
    /*            WILLPOWER */ 1,
    /*             CHARISMA */ 1,
    /*         CARRY_WEIGHT */ 300,
    /*         DAMAGE_BONUS */ -50,
    /*        AC_ADJUSTMENT */ -9,
    /*                SPEED */ 1,
    /*            HEAL_RATE */ 0,
    /*      POISON_RECOVERY */ 1,
    /*    REACTION_MODIFIER */ -65,
    /*        MAX_FOLLOWERS */ 1,
    /* MAGICK_TECH_APTITUDE */ -100,
    /*                LEVEL */ 0,
    /*    EXPERIENCE_POINTS */ 0,
    /*            ALIGNMENT */ -1000,
    /*          FATE_POINTS */ 0,
    /*       UNSPENT_POINTS */ 0,
    /*        MAGICK_POINTS */ 0,
    /*          TECH_POINTS */ 0,
    /*         POISON_LEVEL */ 0,
    /*                  AGE */ 20,
    /*               GENDER */ 0,
    /*                 RACE */ 0,
};

// 0x5B5204
static int stat_max_values[STAT_COUNT] = {
    /*             STRENGTH */ 20,
    /*            DEXTERITY */ 20,
    /*         CONSTITUTION */ 20,
    /*               BEAUTY */ 20,
    /*         INTELLIGENCE */ 20,
    /*           PERCEPTION */ 20,
    /*            WILLPOWER */ 20,
    /*             CHARISMA */ 20,
    /*         CARRY_WEIGHT */ 10000,
    /*         DAMAGE_BONUS */ 50,
    /*        AC_ADJUSTMENT */ 95,
    /*                SPEED */ 100,
    /*            HEAL_RATE */ 6,
    /*      POISON_RECOVERY */ 20,
    /*    REACTION_MODIFIER */ 200,
    /*        MAX_FOLLOWERS */ 7,
    /* MAGICK_TECH_APTITUDE */ 100,
    /*                LEVEL */ 51,
    /*    EXPERIENCE_POINTS */ 2000000000,
    /*            ALIGNMENT */ 1000,
    /*          FATE_POINTS */ 100,
    /*       UNSPENT_POINTS */ 56,
    /*        MAGICK_POINTS */ 210,
    /*          TECH_POINTS */ 210,
    /*         POISON_LEVEL */ 1000,
    /*                  AGE */ 1000,
    /*               GENDER */ 1,
    /*                 RACE */ 11,
};

// 0x5B5274
static int stat_default_values[STAT_COUNT] = {
    /*             STRENGTH */ 8,
    /*            DEXTERITY */ 8,
    /*         CONSTITUTION */ 8,
    /*               BEAUTY */ 8,
    /*         INTELLIGENCE */ 8,
    /*           PERCEPTION */ 8,
    /*            WILLPOWER */ 8,
    /*             CHARISMA */ 8,
    /*         CARRY_WEIGHT */ 0,
    /*         DAMAGE_BONUS */ 0,
    /*        AC_ADJUSTMENT */ 0,
    /*                SPEED */ 0,
    /*            HEAL_RATE */ 0,
    /*      POISON_RECOVERY */ 0,
    /*    REACTION_MODIFIER */ 0,
    /*        MAX_FOLLOWERS */ 0,
    /* MAGICK_TECH_APTITUDE */ 0,
    /*                LEVEL */ 1,
    /*    EXPERIENCE_POINTS */ 0,
    /*            ALIGNMENT */ 0,
    /*          FATE_POINTS */ 0,
    /*       UNSPENT_POINTS */ 5,
    /*        MAGICK_POINTS */ 0,
    /*          TECH_POINTS */ 0,
    /*         POISON_LEVEL */ 0,
    /*                  AGE */ 20,
    /*               GENDER */ 1,
    /*                 RACE */ 0,
};

// 0x5B52E4
static int dword_5B52E4[20] = {
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
};

// 0x5B5334
static int dword_5B5334[20] = {
    -65,
    -52,
    -42,
    -33,
    -25,
    -18,
    -12,
    -7,
    -3,
    0,
    3,
    7,
    12,
    18,
    25,
    33,
    42,
    52,
    65,
    75,
};

// NOTE: This is definitely part of `stat.c` due it's order. However the only
// place where it is used is `magictech.c`, implying public access.
//
// 0x5B5384
const char* off_5B5384[] = {
    "stat_strength",
    "stat_dexterity",
    "stat_constitution",
    "stat_beauty",
    "stat_intelligence",
    "stat_perception",
    "stat_willpower",
    "stat_charisma",
    "stat_carry_weight",
    "stat_damage_bonus",
    "stat_ac_adjustment",
    "stat_speed",
    "stat_heal_rate",
    "stat_poison_recovery_rate",
    "stat_reaction_modifier",
    "stat_max_followers",
    "stat_magic_tech_aptitude",
    "stat_level",
    "stat_experience_points",
    "stat_alignment",
    "stat_fate_points",
    "stat_poison_level",
    "stat_age",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

static_assert(sizeof(off_5B5384) / sizeof(off_5B5384[0]) == STAT_COUNT, "wrong size");

// 0x5B53F4
static int dword_5B53F4 = -1;

// 0x5F8644
static mes_file_handle_t stat_msg_file;

// 0x5F8610
static char* gender_names[GENDER_COUNT];

// 0x5F8618
static char* race_names[RACE_COUNT];

// 0x5F8648
static char* stat_names[STAT_COUNT];

// 0x5F86B8
static char* stat_short_names[STAT_COUNT];

// 0x5F8728
static int64_t qword_5F8728;

// 0x4B0340
bool stat_init(GameContext* init_info)
{
    MesFileEntry mes_file_entry;
    int index;

    (void)init_info;

    if (!mes_load("mes\\stat.mes", &stat_msg_file)) {
        return false;
    }

    for (index = 0; index < STAT_COUNT; index++) {
        mes_file_entry.num = index;
        mes_get_msg(stat_msg_file, &mes_file_entry);
        stat_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < STAT_COUNT; index++) {
        mes_file_entry.num = index + 500;
        mes_get_msg(stat_msg_file, &mes_file_entry);
        stat_short_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < GENDER_COUNT; index++) {
        mes_file_entry.num = index + 28;
        mes_get_msg(stat_msg_file, &mes_file_entry);
        gender_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < RACE_COUNT; index++) {
        mes_file_entry.num = index + 30;
        mes_get_msg(stat_msg_file, &mes_file_entry);
        race_names[index] = mes_file_entry.str;
    }

    return true;
}

// 0x4B0440
void stat_exit()
{
    mes_unload(stat_msg_file);
}

// 0x4B0450
void stat_set_defaults(object_id_t object_id)
{
    int index;

    for (index = 0; index < STAT_COUNT; index++) {
        obj_arrayfield_int32_set(object_id, OBJ_F_CRITTER_STAT_BASE_IDX, index, stat_default_values[index]);
    }
}

// 0x4B0490
int stat_level(object_id_t obj, int stat)
{
    int value;
    location_t location;
    tig_art_id_t art_id;
    int poison;
    int min_value;
    int max_value;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    if (stat < 0 || stat >= STAT_COUNT) {
        return 0;
    }

    value = stat_get_base(obj, stat);

    switch (stat) {
    case STAT_SPEED:
        if (sub_458A80(0x4000000)) {
            if ((obj_field_int32_get(obj, OBJ_F_SPELL_FLAGS) & OSF_TEMPUS_FUGIT) != 0) {
                value += 10;
            } else {
                value -= 10;
            }
        }

        if ((obj_field_int32_get(obj, OBJ_F_CRITTER_FLAGS) & OCF_CRIPPLED_LEGS_BOTH) != 0) {
            value -= 5;
        }

        break;
    case STAT_MAX_FOLLOWERS:
        if (basic_skill_get_training(obj, BASIC_SKILL_PERSUATION) >= TRAINING_EXPERT) {
            value += 1;
        }
        break;
    case STAT_INTELLIGENCE:
    case STAT_DEXTERITY:
    case STAT_STRENGTH:
    case STAT_WILLPOWER:
        // TODO: Many jumps, check.
        switch (background_obj_get_background(obj)) {
        case BACKGROUND_AGORAPHOBIC:
            // Indoor:
            // - Intelligence +2
            //
            // Outdoor:
            // - Dexterity -2
            // - Intelligence -2
            // - Willpower -2
            // - Strength +2
            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
            art_id = sub_4D70B0(location);
            if (tig_art_tile_id_type_get(art_id) == 0) {
                if (stat == STAT_INTELLIGENCE) {
                    value += 2;
                }
            } else {
                if (stat == STAT_STRENGTH) {
                    value += 2;
                } else {
                    value -= 2;
                }
            }
            break;
        case BACKGROUND_HYDROPHOBIC:
            // Land:
            // - Persuation +2
            //
            // Water:
            // - Dexterity -2
            // - Intelligence -2
            // - Willpower -2
            // - Strength +2
            //
            // NOTE: Persuation bonus is applied via effects.
            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
            art_id = sub_4D70B0(location);
            if (sub_4EBB30(art_id)) {
                if (stat == STAT_STRENGTH) {
                    value += 2;
                } else {
                    value -= 2;
                }
            }
            break;
        case BACKGROUND_AFRAID_OF_THE_DARK:
            // Light:
            // - Perception +2
            //
            // Dark:
            // - Dexterity -2
            // - Intelligence -2
            // - Willpower -2
            // - Strength +2
            //
            // NOTE: Perception bonus is applied via effects.
            if (sub_4DCE10(obj) < 128) {
                if (stat == STAT_STRENGTH) {
                    value += 2;
                } else {
                    value -= 2;
                }
            }
            break;
        }

        if (stat == STAT_STRENGTH || stat == STAT_DEXTERITY) {
            poison = stat_level(obj, STAT_POISON_LEVEL) / 100;
            if (poison > 3) {
                value -= 3;
            } else if (poison > 0) {
                value -= poison;
            }
        }

        break;
    case STAT_MAGICK_POINTS:
        switch (background_obj_get_background(obj)) {
        case BACKGROUND_DAY_MAGE:
            if (game_time_is_day()) {
                value += 4;
            } else {
                value -= 4;
            }
            break;
        case BACKGROUND_NIGHT_MAGE:
            if (game_time_is_day()) {
                value -= 4;
            } else {
                value += 4;
            }
            break;
        case BACKGROUND_SKY_MAGE:
            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
            art_id = sub_4D70B0(location);
            if (tig_art_tile_id_type_get(art_id) == 0) {
                value -= 4;
            } else {
                value += 4;
            }
            break;
        case BACKGROUND_NATURE_MAGE:
            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
            art_id = sub_4D70B0(location);
            if (!sub_4EBBA0(art_id)) {
                value -= 4;
            } else {
                value += 4;
            }
            break;
        }
        break;
    }

    value = effect_adjust_stat_level(obj, stat, value);
    min_value = stat_get_min_value(obj, stat);
    max_value = stat_get_max_value(obj, stat);

    if (value < min_value) {
        return min_value;
    }

    if (value > max_value) {
        return max_value;
    }

    return value;
}

// 0x4B0740
int stat_get_base(object_id_t obj, int stat)
{
    int value;
    int bonus;
    location_t location;

    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    if (stat < 0 || stat >= STAT_COUNT) {
        return 0;
    }

    if (stat >= FIRST_DERIVED_STAT && stat <= LAST_DERIVED_STAT) {
        switch (stat) {
        case STAT_CARRY_WEIGHT:
            value = 500 * stat_level(obj, STAT_STRENGTH);
            break;
        case STAT_DAMAGE_BONUS:
            value = stat_level(obj, STAT_STRENGTH) - 10;
            if (value < 0) {
                value /= 2;
            }

            if (stat_is_maximized(obj, stat)) {
                value *= 2;
            }

            break;
        case STAT_AC_ADJUSTMENT:
            value = stat_level(obj, stat) - 10;
            break;
        case STAT_SPEED:
            value = stat_level(obj, STAT_DEXTERITY);

            if (stat_is_maximized(obj, stat)) {
                value += 5;
            }

            break;
        case STAT_HEAL_RATE:
            value = stat_level(obj, STAT_CONSTITUTION) + 1;
            value /= 3;
            break;
        case STAT_POISON_RECOVERY:
            value = stat_level(obj, STAT_CONSTITUTION);
            break;
        case STAT_REACTION_MODIFIER:
            value = stat_level(obj, STAT_BEAUTY);
            if (stat_is_maximized(obj, STAT_BEAUTY)) {
                value = 2 * (5 * value - 50);
            } else {
                value = dword_5B5334[value - 1];
            }
            break;
        case STAT_MAX_FOLLOWERS:
            value = stat_level(obj, STAT_CHARISMA) / 4;
            break;
        case STAT_MAGICK_TECH_APTITUDE:
            bonus = (50 * stat_level(obj, STAT_MAGICK_POINTS) - 55 * stat_level(obj, STAT_TECH_POINTS)) / 10;
            location = obj_field_int64_get(obj, OBJ_F_LOCATION);
            value = magictech_get_aptitude_adj(sub_4CFC50(location)) + bonus;
            break;
        default:
            // Unreachable.
            __assume(0);
        }
    } else {
        value = obj_arrayfield_int32_get(obj, OBJ_F_CRITTER_STAT_BASE_IDX, stat);
    }

    return value;
}

// 0x4B0980
int stat_set_base(object_id_t obj, int stat, int value)
{
    // TODO: Incomplete.
    (void)obj;
    (void)stat;

    return value;
}

// 0x4B0EE0
bool stat_is_maximized(object_id_t obj, int stat)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC
        && obj_field_int32_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return false;
    }

    if (stat < 0 || stat >= STAT_COUNT) {
        return false;
    }

    if (stat > LAST_PRIMARY_STAT) {
        return false;
    }

    if (stat_level(obj, stat) < 20) {
        return false;
    }

    return true;
}

// 0x4B0F50
int sub_4B0F50(int value)
{
    if (value < 1) {
        value = 1;
    } else if (value > 20) {
        value = 20;
    }

    return dword_5B52E4[value - 1];
}

// 0x4B0F80
const char* stat_get_name(int stat)
{
    return stat_names[stat];
}

// 0x4B0F90
const char* stat_get_short_name(int stat)
{
    return stat_short_names[stat];
}

// 0x4B0FA0
const char* gender_get_name(int gender)
{
    return gender_names[gender];
}

// 0x4B0FB0
const char* race_get_name(int race)
{
    return race_names[race];
}

// 0x4B0FC0
int stat_get_min_value(object_id_t obj, int stat)
{
    (void)obj;

    return stat_min_values[stat];
}

// 0x4B0FD0
int stat_get_max_value(object_id_t obj, int stat)
{
    int race;

    if (obj != OBJ_HANDLE_NULL) {
        race = stat_get_base(obj, STAT_RACE);
    } else {
        race = RACE_HUMAN;
    }

    switch (race) {
    case RACE_DWARF:
    case RACE_HALF_ORC:
        if (stat == STAT_STRENGTH || stat == STAT_UNSPENT_POINTS) {
            return 21;
        }
        break;
    case RACE_ELF:
    case RACE_DARK_ELF:
        if (stat == STAT_DEXTERITY || stat == STAT_BEAUTY || stat == STAT_WILLPOWER) {
            return 21;
        }
        break;
    case RACE_HALF_ELF:
        if (stat == STAT_DEXTERITY) {
            return 21;
        }
        break;
    case RACE_GNOME:
        if (stat == STAT_WILLPOWER) {
            return 22;
        }
        break;
    case RACE_HALFLING:
        if (stat == STAT_DEXTERITY) {
            return 22;
        }
        break;
    case RACE_HALF_OGRE:
        if (stat == STAT_STRENGTH) {
            return 24;
        }
        break;
    case RACE_OGRE:
        if (stat == STAT_STRENGTH) {
            return 26;
        }
        break;
    case RACE_ORC:
        if (stat == STAT_STRENGTH || stat == STAT_CONSTITUTION) {
            return 22;
        }
        break;
    }

    return stat_max_values[stat];
}

// 0x4B10A0
bool sub_4B10A0(int64_t obj, int stat, int value)
{
    int max_value;
    int base_value;
    int iter = 1;

    max_value = stat_get_max_value(obj, stat);

    base_value = stat_get_base(obj, stat);
    stat_set_base(obj, stat, base_value + 1);

    while (value > stat_level(obj, stat)) {
        if (stat_get_base(obj, stat) == base_value) {
            break;
        }

        if (stat_level(obj, stat) >= max_value) {
            break;
        }

        if (iter >= 100) {
            break;
        }

        iter++;

        base_value = stat_get_base(obj, stat);
        stat_set_base(obj, stat, base_value + 1);
    }

    return value <= stat_level(obj, stat);
}

// 0x4B1310
bool sub_4B1310(TimeEvent* timeevent)
{
    return timeevent->params[1].object_value == qword_5F8728
        && timeevent->params[0].integer_value == dword_5B53F4;
}

// 0x4B1350
bool sub_4B1350(int64_t obj, int value, bool a3)
{
    DateTime datetime;
    TimeEvent timeevent;

    (void)value;

    qword_5F8728 = obj;
    dword_5B53F4 = 0;

    timeevent.type = TIMEEVENT_TYPE_POISON;
    timeevent.params[0].integer_value = 0;
    timeevent.params[1].object_value = obj;
    timeevent.params[2].integer_value = sub_45A7F0();

    if (!sub_45C140(TIMEEVENT_TYPE_POISON, sub_4B1310)) {
        sub_45A950(&datetime, 15000);
        if (!sub_45B800(&timeevent, &datetime)) {
            return false;
        }
    }

    if (a3) {
        dword_5B53F4 = 1;
        if (!sub_45C140(TIMEEVENT_TYPE_POISON, sub_4B1310)) {
            sub_45A950(&datetime, 120000);

            timeevent.params[0].integer_value = 1;
            if (!sub_45B800(&timeevent, &datetime)) {
                return false;
            }
        }
    }

    sub_460240(obj);

    return true;
}
