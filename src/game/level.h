#ifndef ARCANUM_GAME_LEVEL_H_
#define ARCANUM_GAME_LEVEL_H_

#include "game/context.h"

bool level_init(GameInitInfo* init_info);
void level_exit();
int level_get_experience_points_to_next_level(int64_t obj);
int level_get_experience_points_to_next_level_in_percent(int64_t obj);
int level_auto_level_scheme_get(int64_t obj);
int level_auto_level_scheme_set(int64_t obj, int value);
const char* level_advancement_scheme_get_name(int scheme);
const char* level_advancement_scheme_get_rule(int scheme);
void level_set_level(int64_t obj, int level);

#endif /* ARCANUM_GAME_LEVEL_H_ */
