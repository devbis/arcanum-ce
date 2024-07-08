#ifndef ARCANUM_GAME_LIB_NEWSPAPER_H_
#define ARCANUM_GAME_LIB_NEWSPAPER_H_

#include "game/context.h"

bool newspaper_init(GameContext* ctx);
void newspaper_reset();
void newspaper_exit();
bool newspaper_load(LoadContext* ctx);
bool newspaper_save(TigFile* stream);
bool sub_4BF1D0(int a1);
int sub_4BF200(int index);

#endif /* ARCANUM_GAME_LIB_NEWSPAPER_H_ */
