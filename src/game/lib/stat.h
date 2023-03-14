#ifndef ARCANUM_GAME_LIB_STAT_H_
#define ARCANUM_GAME_LIB_STAT_H_

#include "game/context.h"

typedef enum Stat {
    STAT_STRENGTH,
    STAT_DEXTERITY,
    STAT_CONSTITUTION,
    STAT_BEAUTY,
    STAT_INTELLIGENCE,
    STAT_PERCEPTION,
    STAT_WILLPOWER,
    STAT_CHARISMA,
    STAT_CARRY_WEIGHT,
    STAT_DAMAGE_BONUS,
    STAT_AC_ADJUSTMENT,
    STAT_SPEED,
    STAT_HEAL_RATE,
    STAT_POISON_RECOVERY,
    STAT_REACTION_MODIFIER,
    STAT_MAX_FOLLOWERS,
    STAT_MAGICK_TECH_APTITUDE,
    STAT_LEVEL,
    STAT_EXPERIENCE_POINTS,
    STAT_ALIGNMENT,
    STAT_FATE_POINTS,
    STAT_UNSPENT_POINTS,
    STAT_MAGICK_POINTS,
    STAT_TECH_POINTS,
    STAT_POISON_LEVEL,
    STAT_AGE,
    STAT_GENDER,
    STAT_RACE,
    STAT_COUNT,
};

static_assert(STAT_COUNT == 28, "wrong size");

typedef enum Gender {
    GENDER_FEMALE,
    GENDER_MALE,
    GENDER_COUNT,
};

static_assert(GENDER_COUNT == 2, "wrong size");

typedef enum Race {
    RACE_HUMAN,
    RACE_DWARF,
    RACE_ELF,
    RACE_HALF_ELF,
    RACE_GNOME,
    RACE_HALFLING,
    RACE_HALF_ORC,
    RACE_HALF_OGRE,
    RACE_DARK_ELF,
    RACE_OGRE,
    RACE_ORC,
    RACE_COUNT,
};

static_assert(RACE_COUNT == 11, "wrong size");

bool stat_init(GameContext* ctx);
void stat_exit();
const char* stat_get_name(int stat);
const char* stat_get_short_name(int stat);
const char* gender_get_name(int gender);
const char* race_get_name(int race);
int sub_4B0FC0(int a1, int a2, int a3);

#endif /* ARCANUM_GAME_LIB_STAT_H_ */
