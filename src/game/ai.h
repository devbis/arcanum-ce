#ifndef ARCANUM_GAME_AI_H_
#define ARCANUM_GAME_AI_H_

#include "game/context.h"

void ai_exit();
void ai_mod_unload();
void sub_4AA4A0(int64_t obj);
void sub_4AA580(int64_t obj);
void sub_4AA8C0(int64_t obj, bool force);
bool ai_npc_wait_here_timeevent_process(TimeEvent* timeevent);
bool ai_is_fighting(int64_t obj);
bool sub_4AFB30(int64_t obj, int64_t a2);
void ai_target_lock(int64_t obj, int64_t tgt);
void ai_target_unlock(int64_t obj);

#endif /* ARCANUM_GAME_AI_H_ */
