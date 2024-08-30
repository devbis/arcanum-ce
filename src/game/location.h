#ifndef ARCANUM_GAME_LOCATION_H_
#define ARCANUM_GAME_LOCATION_H_

#include "game/context.h"

typedef long long location_t;

typedef void(LocationFunc5FC2F8)(int a1, int a2);

bool location_init(GameInitInfo* init_info);
void location_exit();
void location_resize(ResizeInfo* resize_info);
bool sub_4B85A0(void* a1);
void location_set_func_5FC2F8(LocationFunc5FC2F8* func);
bool location_set_limits(int64_t x, int64_t y);
void location_get_limits(int64_t* x, int64_t* y);
int64_t sub_4B9810();

#endif /* ARCANUM_GAME_LOCATION_H_ */
