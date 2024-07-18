#ifndef ARCANUM_GAME_AREA_H_
#define ARCANUM_GAME_AREA_H_

#include "game/context.h"

bool area_init(GameInitInfo* init_info);
void area_exit();
void area_mod_unload();
int sub_4CAE80();

#endif /* ARCANUM_GAME_AREA_H_ */
