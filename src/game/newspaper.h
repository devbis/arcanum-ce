#ifndef ARCANUM_GAME_NEWSPAPER_H_
#define ARCANUM_GAME_NEWSPAPER_H_

#include "game/context.h"

bool newspaper_init(GameInitInfo* init_info);
void newspaper_reset();
void newspaper_exit();
bool newspaper_load(GameLoadInfo* load_info);
bool newspaper_save(TigFile* stream);
void newspaper_queue(int num, bool priority);
bool sub_4BF1D0(int a1);
int sub_4BF200(int index);

#endif /* ARCANUM_GAME_NEWSPAPER_H_ */
