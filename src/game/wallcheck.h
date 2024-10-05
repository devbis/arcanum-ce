#ifndef ARCANUM_GAME_WALLCHECK_H_
#define ARCANUM_GAME_WALLCHECK_H_

#include "game/context.h"

bool wallcheck_init(GameInitInfo* init_info);
void sub_437E10();
void wallcheck_flush();
void sub_438500(bool enabled);
bool sub_438520();

#endif /* ARCANUM_GAME_WALLCHECK_H_ */
