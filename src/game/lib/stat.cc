#include "game/lib/stat.h"

#include "game/lib/background.h"
#include "game/lib/effect.h"
#include "game/lib/light.h"
#include "game/lib/location.h"
#include "game/lib/message.h"
#include "game/lib/object.h"
#include "game/lib/sector.h"
#include "game/lib/skill.h"
#include "tig/art.h"

// 0x5B5194
int stat_min_values[STAT_COUNT] = {
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
int stat_max_values[STAT_COUNT] = {
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
int stat_default_values[STAT_COUNT] = {
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
int dword_5B52E4[20] = {
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
int dword_5B5334[20] = {
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

// 0x5F8644
static int stat_msg_file;

// 0x5F8610
static char* gender_names[GENDER_COUNT];

// 0x5F8618
static char* race_names[RACE_COUNT];

// 0x5F8648
static char* stat_names[STAT_COUNT];

// 0x5F86B8
static char* stat_short_names[STAT_COUNT];

// 0x4B0340
bool stat_init(GameContext* ctx)
{
    MessageListItem message_list_item;

    if (!message_load("mes\\stat.mes", &stat_msg_file)) {
        return false;
    }

    for (int stat = 0; stat < STAT_COUNT; stat++) {
        message_list_item.num = stat;
        sub_4D43A0(stat_msg_file, &message_list_item);
        stat_names[stat] = message_list_item.text;
    }

    for (int stat = 0; stat < STAT_COUNT; stat++) {
        message_list_item.num = stat + 500;
        sub_4D43A0(stat_msg_file, &message_list_item);
        stat_short_names[stat] = message_list_item.text;
    }

    for (int gender = 0; gender < GENDER_COUNT; gender++) {
        message_list_item.num = gender + 28;
        sub_4D43A0(stat_msg_file, &message_list_item);
        gender_names[gender] = message_list_item.text;
    }

    for (int race = 0; race < RACE_COUNT; race++) {
        message_list_item.num = race + 30;
        sub_4D43A0(stat_msg_file, &message_list_item);
        race_names[race] = message_list_item.text;
    }

    return true;
}

// 0x4B0440
void stat_exit()
{
    message_unload(stat_msg_file);
}

// 0x4B0450
void stat_set_defaults(object_id_t object_id)
{
    for (int stat = 0; stat < STAT_COUNT; stat++) {
        sub_407340(object_id, OBJ_F_CRITTER_STAT_BASE_IDX, stat, stat_default_values[stat]);
    }
}

// 0x4B0490
int stat_level(object_id_t obj, int stat)
{
    if (object_field_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC && object_field_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    if (stat < 0 || stat >= STAT_COUNT) {
        return 0;
    }

    int value = stat_get_base(obj, stat);
    switch (stat) {
    case STAT_SPEED:
        if (sub_458A80(0x4000000)) {
            if ((object_field_get(obj, OBJ_F_SPELL_FLAGS) & 0x4000000) != 0) {
                value += 10;
            } else {
                value -= 10;
            }
        }

        if ((object_field_get(obj, OBJ_F_CRITTER_FLAGS) & 0x400) != 0) {
            value -= 5;
        }

        break;
    case STAT_MAX_FOLLOWERS:
        if (sub_4C60C0(obj, PRIMARY_SKILL_PERSUATION) >= 2) {
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
            if (true) {
                location_t location = object_field_get_64(obj, OBJ_F_LOCATION);
                art_id_t art_id = sub_4D70B0(location);
                if (tig_art_tile_id_indoor_outdoor_type(art_id) == 0) {
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
            if (true) {
                location_t location = object_field_get_64(obj, OBJ_F_LOCATION);
                art_id_t art_id = sub_4D70B0(location);
                if (sub_4EBB30(art_id)) {
                    if (stat == STAT_STRENGTH) {
                        value += 2;
                    } else {
                        value -= 2;
                    }
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
            if (true) {
                if (sub_4DCE10(obj) < 128) {
                    if (stat == STAT_STRENGTH) {
                        value += 2;
                    } else {
                        value -= 2;
                    }
                }
            }
            break;
        }

        if (stat == STAT_STRENGTH || stat == STAT_DEXTERITY) {
            int poison = stat_level(obj, STAT_POISON_LEVEL) / 100;
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
            if (1) {
                location_t location = object_field_get_64(obj, OBJ_F_LOCATION);
                art_id_t art_id = sub_4D70B0(location);
                if (tig_art_tile_id_indoor_outdoor_type(art_id) == 0) {
                    value -= 4;
                } else {
                    value += 4;
                }
            }
            break;
        case BACKGROUND_NATURE_MAGE:
            if (1) {
                location_t location = object_field_get_64(obj, OBJ_F_LOCATION);
                art_id_t art_id = sub_4D70B0(location);
                if (!sub_4EBBA0(art_id)) {
                    value -= 4;
                } else {
                    value += 4;
                }
            }
            break;
        }
        break;
    }

    int adjusted_value = effect_adjust_stat_level(obj, stat, value);
    int min_value = stat_get_min_value(obj, stat);
    int max_value = stat_get_max_value(obj, stat);

    if (adjusted_value < min_value) {
        return min_value;
    }

    if (adjusted_value > max_value) {
        return max_value;
    }

    return adjusted_value;
}

// 0x4B0740
int stat_get_base(object_id_t obj, int stat)
{
    if (object_field_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC && object_field_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
        return 0;
    }

    if (stat < 0 || stat >= STAT_COUNT) {
        return 0;
    }

    int value;
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
            if (1) {
                int bonus = (50 * stat_level(obj, STAT_MAGICK_POINTS) - 55 * stat_level(obj, STAT_TECH_POINTS)) / 10;
                location_t location = object_field_get_64(obj, OBJ_F_LOCATION);
                value = sub_45A060(sub_4CFC50(location)) + bonus;
            }
            break;
        }
    } else {
        value = sub_4072D0(obj, OBJ_F_CRITTER_STAT_BASE_IDX, stat);
    }

    return value;
}

// 0x4B0980
int stat_set_base(object_id_t obj, int stat)
{
    // TODO: Incomplete.
}

// 0x4B0EE0
bool stat_is_maximized(object_id_t obj, int stat)
{
    if (object_field_get(obj, OBJ_F_TYPE) != OBJ_TYPE_PC && object_field_get(obj, OBJ_F_TYPE) != OBJ_TYPE_NPC) {
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
int stat_get_min_value(object_id_t object_id, int stat)
{
    return stat_min_values[stat];
}

// 0x4B0FD0
int stat_get_max_value(object_id_t object_id, int stat)
{
    int race;
    if (object_id != 0) {
        race = sub_4B0740(object_id, STAT_RACE);
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
