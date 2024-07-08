#ifndef ARCANUM_GAMELIB_LI_H_
#define ARCANUM_GAMELIB_LI_H_

#include "game/context.h"

bool li_init(GameContext* ctx);
void li_exit();
void li_resize(ResizeContext* ctx);
void sub_4BBC00();
void sub_4BBC10();
int sub_4BBC20();
void sub_4BBC30();
void sub_4BBC80();

#endif /* ARCANUM_GAMELIB_LI_H_ */
