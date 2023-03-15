#ifndef ARCANUM_GAME_LIB_JUMPPOINT_H_
#define ARCANUM_GAME_LIB_JUMPPOINT_H_

#include "game/context.h"

bool jumppoint_init(GameContext* ctx);
void jumppoint_reset();
void jumppoint_exit();
void jumppoint_resize(ResizeContext* ctx);
bool sub_4E3050(void* a1);
bool jumppoint_open(const char* a1, const char* a2);
void jumppoint_close();
bool sub_4E3270();
bool sub_4E32D0(void* a1);

#endif /* ARCANUM_GAME_LIB_JUMPPOINT_H_ */
