#ifndef ARCANUM_GAME_LEVEL_H_
#define ARCANUM_GAME_LEVEL_H_

#include "game/context.h"
#include "game/obj.h"

bool level_init(GameInitInfo* init_info);
void level_exit();
int level_get_experience_points_to_next_level(object_id_t object_id);
int level_get_experience_points_to_next_level_in_percent(object_id_t object_id);
int level_auto_level_scheme_get(object_id_t object_id);
int level_auto_level_scheme_set(object_id_t object_id, int value);
const char* level_advancement_scheme_get_name(int scheme);
const char* level_advancement_scheme_get_rule(int scheme);
void level_set_level(object_id_t obj, int level);

#endif /* ARCANUM_GAME_LEVEL_H_ */
