#ifndef ARCANUM_GAME_EFFECT_H_
#define ARCANUM_GAME_EFFECT_H_

#include "game/context.h"
#include "game/obj.h"

extern const char* off_5B9CB4[];

bool effect_init(GameInitInfo* init_info);
void effect_exit();
bool effect_mod_load();
void effect_mod_unload();
void sub_4E9F70(int64_t obj, int effect, int type);
void sub_4EA100(int64_t obj, int effect);
void sub_4EA200(int64_t obj, int effect);
bool sub_4EA2E0(int64_t obj, int effect_id);
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

#endif /* ARCANUM_GAME_EFFECT_H_ */
