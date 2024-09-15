#ifndef ARCANUM_GAME_ROOF_H_
#define ARCANUM_GAME_ROOF_H_

#include "game/context.h"

bool roof_init(GameInitInfo* init_info);
void roof_exit();
void roof_resize(ResizeInfo* resize_info);
bool sub_439100(ViewOptions* view_options);
void sub_43A110(int a1);
int sub_43A130();

#endif /* ARCANUM_GAME_ROOF_H_ */
