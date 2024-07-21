#ifndef ARCANUM_GAME_COMBAT_H_
#define ARCANUM_GAME_COMBAT_H_

#include "game/context.h"

bool combat_init(GameInitInfo* init_info);
void combat_exit();
void combat_reset();
int64_t sub_4B23B0(int64_t obj);
bool combat_critter_is_combat_mode_active(int64_t obj);
void combat_critter_deactivate_combat_mode(int64_t obj);
void combat_critter_activate_combat_mode(int64_t obj);
tig_art_id_t sub_4B6B10(tig_art_id_t aid, int v2);
bool combat_auto_switch_weapons_get(int64_t obj);
void combat_auto_switch_weapons_set(int value);

#endif /* ARCANUM_GAME_COMBAT_H_ */
