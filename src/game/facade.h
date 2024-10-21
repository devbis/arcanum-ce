#ifndef ARCANUM_GAME_FACADE_H_
#define ARCANUM_GAME_FACADE_H_

#include "game/context.h"
#include "game/location.h"

bool facade_init(GameInitInfo* init_info);
void facade_exit();
void facade_resize(ResizeInfo* resize_info);
bool sub_4C9E00(ViewOptions* view_options);
void sub_4C9E70(UnknownContext* info);
bool sub_4CA6B0(LocRect* loc_rect, int* a2, int* a3);

#endif /* ARCANUM_GAME_FACADE_H_ */
