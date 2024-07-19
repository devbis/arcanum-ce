#ifndef ARCANUM_GAME_AREA_H_
#define ARCANUM_GAME_AREA_H_

#include "game/context.h"

bool area_init(GameInitInfo* init_info);
void area_exit();
void area_mod_unload();
bool area_load(GameLoadInfo* load_info);
int sub_4CAE80();
char* sub_4CAE90(int index);
char* sub_4CAEB0(int index);
int64_t sub_4CAED0(int index);
void sub_4CAF00(int index, int* a2, int* a3);
int sub_4CB100(int64_t obj_handle);

#endif /* ARCANUM_GAME_AREA_H_ */
