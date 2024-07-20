#ifndef ARCANUM_GAME_ONAME_H_
#define ARCANUM_GAME_ONAME_H_

#include "game/context.h"

bool o_name_init(GameInitInfo* init_info);
void o_name_exit();
bool o_name_mod_load();
void o_name_mod_unload();
int sub_4E73F0();
const char* sub_4E7400(int num);

#endif /* ARCANUM_GAME_ONAME_H_ */
