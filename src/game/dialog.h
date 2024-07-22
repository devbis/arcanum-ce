#ifndef ARCANUM_GAME_DIALOG_H_
#define ARCANUM_GAME_DIALOG_H_

#include "game/context.h"

bool dialog_init(GameInitInfo* init_info);
void dialog_exit();
bool sub_412E10(const char* path, int* a2);
void sub_412F40(int index);
void sub_4182C0();

#endif /* ARCANUM_GAME_DIALOG_H_ */
