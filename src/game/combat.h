#ifndef ARCANUM_GAME_COMBAT_H_
#define ARCANUM_GAME_COMBAT_H_

#include "game/context.h"

bool combat_init(GameInitInfo* init_info);
void combat_exit();
void combat_reset();
int64_t sub_4B23B0(int64_t obj);

#endif /* ARCANUM_GAME_COMBAT_H_ */
