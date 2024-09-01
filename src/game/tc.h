#ifndef ARCANUM_GAME_TC_H_
#define ARCANUM_GAME_TC_H_

#include "game/context.h"

bool tc_init(GameInitInfo* init_info);
void tc_exit();
void tc_resize(ResizeInfo* resize_info);
void sub_4C95F0(UnknownContext* info);
void sub_4C9620(int dx, int dy);
void sub_4C96C0();
void sub_4C96F0();
void sub_4C9720();
void sub_4C9810(int index, const char* str);
int sub_4C9B90(const char* str);

#endif /* ARCANUM_GAME_TC_H_ */
