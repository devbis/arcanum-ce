#include "game/lib/stat.h"

#include "game/lib/message.h"
#include "game/lib/object.h"

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
