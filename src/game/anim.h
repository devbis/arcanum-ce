#ifndef ARCANUM_GAME_ANIM_H_
#define ARCANUM_GAME_ANIM_H_

#include "game/context.h"

bool anim_init(GameInitInfo* init_info);
void anim_exit();
void anim_reset();
void anim_break_nodes_to_map(const char* map);
void anim_save_nodes_to_map(const char* map);
void anim_load_nodes_from_map(const char* map);
void sub_4232F0();
void sub_423FC0();
bool sub_424250();
void sub_430460();

#endif /* ARCANUM_GAME_ANIM_H_ */
