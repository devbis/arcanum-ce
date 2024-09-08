#ifndef ARCANUM_GAME_TRAP_H_
#define ARCANUM_GAME_TRAP_H_

#include "game/context.h"
#include "game/timeevent.h"

bool trap_init(GameInitInfo* init_info);
void trap_exit();
bool trap_timeevent_process(TimeEvent* timeevent);
bool trap_is_trap_device(int64_t obj);

#endif /* ARCANUM_GAME_TRAP_H_ */
