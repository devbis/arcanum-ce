#ifndef ARCANUM_GAME_EFFECT_H_
#define ARCANUM_GAME_EFFECT_H_

#include "game/context.h"
#include "game/obj.h"

#define EFFECT_SCARRING 50

typedef enum EffectCause {
    EFFECT_CAUSE_RACE,
    EFFECT_CAUSE_BACKGROUND,
    EFFECT_CAUSE_CLASS,
    EFFECT_CAUSE_BLESS,
    EFFECT_CAUSE_CURSE,
    EFFECT_CAUSE_ITEM,
    EFFECT_CAUSE_SPELL,
    EFFECT_CAUSE_INJURY,
    EFFECT_CAUSE_TECH,
    EFFECT_CAUSE_GENDER,
    EFFECT_CAUSE_COUNT,
} EffectCause;

extern const char* off_5B9CB4[];

bool effect_init(GameInitInfo* init_info);
void effect_exit();
bool effect_mod_load();
void effect_mod_unload();
void effect_add(int64_t obj, int effect, int cause);
void effect_remove_one_typed(int64_t obj, int effect);
void effect_remove_all_typed(int64_t obj, int effect);
void effect_remove_one_caused_by(int64_t obj, int cause);
void effect_remove_all_caused_by(int64_t obj, int cause);
int sub_4EA4A0(int64_t obj, int effect_id);
int effect_adjust_stat_level(int64_t obj, int stat, int value);
int sub_4EA930(int64_t obj, int stat, int value);
int effect_adjust_basic_skill_level(int64_t obj, int skill, int value);
int effect_adjust_tech_skill_level(int64_t obj, int skill, int value);
int effect_adjust_resistance(int64_t obj, int resistance, int value);
int effect_adjust_tech_level(int64_t obj, int tech, int value);
int effect_adjust_max_hit_points(int64_t obj, int value);
int effect_adjust_max_fatigue(int64_t obj, int value);
int effect_adjust_reaction(int64_t obj, int value);
int effect_adjust_good_bad_reaction(int64_t obj, int value);
int effect_adjust_crit_hit_chance(int64_t obj, int value);
int effect_adjust_crit_hit_effect(int64_t obj, int value);
int effect_adjust_crit_fail_chance(int64_t obj, int value);
int effect_adjust_crit_fail_effect(int64_t obj, int value);
int effect_adjust_xp_gain(int64_t obj, int value);
void effect_debug_obj(int64_t obj);

#endif /* ARCANUM_GAME_EFFECT_H_ */
