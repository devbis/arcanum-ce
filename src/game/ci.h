#ifndef ARCANUM_GAME_CI_H_
#define ARCANUM_GAME_CI_H_

#include "game/context.h"

bool ci_init(GameInitInfo* init_info);
void ci_exit();
void sub_4BB8E0();
void sub_4BB8F0();
int sub_4BB900();
void sub_4BB910();
void ci_redraw();

#endif /* ARCANUM_GAME_CI_H_ */
