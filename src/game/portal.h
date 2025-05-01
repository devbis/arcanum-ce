#ifndef ARCANUM_GAME_PORTAL_H_
#define ARCANUM_GAME_PORTAL_H_

#include "game/context.h"

void sub_4F06E0(int64_t a1, int64_t a2);
void sub_4F0790(int64_t obj, bool a2);
bool portal_is_open(int64_t obj);
void portal_toggle(int64_t obj);
bool portal_open(int64_t door_obj);
bool portal_close(int64_t door_obj);

#endif /* ARCANUM_GAME_PORTAL_H_ */
