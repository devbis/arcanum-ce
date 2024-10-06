#ifndef ARCANUM_GAME_MT_AI_H_
#define ARCANUM_GAME_MT_AI_H_

#include "game/context.h"

bool mt_ai_init(GameInitInfo* init_info);
void mt_ai_reset();
void mt_ai_exit();
void sub_4CCBF0(int a1);
void sub_4CCC00(int64_t obj);
void sub_4CCC40(int64_t a1, int64_t a2);

#endif /* ARCANUM_GAME_MT_AI_H_ */
