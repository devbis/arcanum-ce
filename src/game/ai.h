#ifndef ARCANUM_GAME_AI_H_
#define ARCANUM_GAME_AI_H_

#include "game/context.h"

typedef void(Func5F8488)(int64_t, int64_t, const char*, int);
typedef void(Func5F848C)(int64_t, int);

void ai_exit();
void ai_mod_unload();
void sub_4A84D0(Func5F848C* a1, Func5F8488* a2);
void sub_4A84F0(int64_t obj);
void sub_4AA4A0(int64_t obj);
void sub_4AA580(int64_t obj);
void sub_4AA8C0(int64_t obj, bool force);
bool ai_npc_wait_here_timeevent_process(TimeEvent* timeevent);
bool ai_is_fighting(int64_t obj);
int sub_4AB400(int64_t obj);
int sub_4AB430(int64_t obj);
int64_t sub_4ABBC0(int64_t obj);
void sub_4AD6E0(int64_t obj);
bool sub_4AFB30(int64_t obj, int64_t a2);
void ai_target_lock(int64_t obj, int64_t tgt);
void ai_target_unlock(int64_t obj);

#endif /* ARCANUM_GAME_AI_H_ */
