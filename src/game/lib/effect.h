#ifndef ARCANUM_GAMELIB_EFFECT_H_
#define ARCANUM_GAMELIB_EFFECT_H_

#include "game/context.h"
#include "game/lib/object.h"

bool effect_init(GameContext* ctx);
void effect_exit();
bool effect_mod_load();
void effect_mod_unload();
int sub_4EA4A0(object_id_t obj, int effect_id);
int effect_adjust_stat_level(object_id_t obj, int stat, int value);
int sub_4EA930(object_id_t obj, int stat, int value);
int effect_adjust_basic_skill_level(object_id_t obj, int skill, int value);
int effect_adjust_tech_skill_level(object_id_t obj, int skill, int value);
int effect_adjust_resistance(object_id_t obj, int resistance, int value);
int effect_adjust_tech_level(object_id_t obj, int tech, int value);
int effect_adjust_max_hit_points(object_id_t obj, int value);
int effect_adjust_max_fatigue(object_id_t obj, int value);
int effect_adjust_reaction(object_id_t obj, int value);
int effect_adjust_good_bad_reaction(object_id_t obj, int value);
int effect_adjust_crit_hit_chance(object_id_t obj, int value);
int effect_adjust_crit_hit_effect(object_id_t obj, int value);
int effect_adjust_crit_fail_chance(object_id_t obj, int value);
int effect_adjust_crit_fail_effect(object_id_t obj, int value);
int effect_adjust_xp_gain(object_id_t obj, int value);
void effect_debug_obj(object_id_t obj);

#endif /* ARCANUM_GAMELIB_EFFECT_H_ */
