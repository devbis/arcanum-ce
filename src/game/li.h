#ifndef ARCANUM_GAME_LI_H_
#define ARCANUM_GAME_LI_H_

#include "game/context.h"

bool li_init(GameInitInfo* init_info);
void li_exit();
void li_resize(GameResizeInfo* resize_info);
void sub_4BBC00();
void sub_4BBC10();
int sub_4BBC20();
void li_update();
void li_redraw();

#endif /* ARCANUM_GAME_LI_H_ */
