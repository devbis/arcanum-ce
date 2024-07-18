#ifndef ARCANUM_GAME_AREA_H_
#define ARCANUM_GAME_AREA_H_

#include "game/context.h"

bool area_init(GameInitInfo* init_info);
void area_exit();
void area_mod_unload();
int sub_4CAE80();
char* sub_4CAE90(int index);
char* sub_4CAEB0(int index);
int64_t sub_4CAED0(int index);

#endif /* ARCANUM_GAME_AREA_H_ */
