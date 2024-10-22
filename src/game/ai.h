#ifndef ARCANUM_GAME_AI_H_
#define ARCANUM_GAME_AI_H_

#include "game/context.h"
#include "game/timeevent.h"
#include "game/object.h"

typedef void(Func5F8488)(int64_t, int64_t, const char*, int);
typedef void(Func5F848C)(int64_t, int);

void ai_exit();
void ai_mod_unload();
void sub_4A84D0(Func5F848C* a1, Func5F8488* a2);
void sub_4A84F0(int64_t obj);
void sub_4A9650(int64_t obj, int64_t tgt, int a3, unsigned int flags);
void sub_4A9AD0(int64_t a1, int64_t a2);
void sub_4AA0D0(int64_t obj);
void sub_4AA300(int64_t a1, int64_t a2);
void sub_4AA4A0(int64_t obj);
void sub_4AA580(int64_t obj);
void sub_4AA8C0(int64_t obj, bool force);
bool ai_npc_wait_here_timeevent_process(TimeEvent* timeevent);
bool ai_is_fighting(int64_t obj);
int sub_4AB400(int64_t obj);
int sub_4AB430(int64_t obj);
int64_t sub_4ABBC0(int64_t obj);
void sub_4AD6E0(int64_t obj);
void sub_4AD7D0(int64_t obj);
int sub_4AD800(int64_t a1, int64_t a2, bool a3);
int sub_4AD950(int64_t a1, int64_t a2, bool a3);
void sub_4AD790(int64_t obj, int a2);
int sub_4ADE00(int64_t a1, int64_t a2, int64_t* a3);
void sub_4ADFF0(int64_t obj);
void sub_4AE020(int64_t obj, int* cnt_ptr, int* lvl_ptr);
int sub_4AE120(int64_t a1, int64_t a2);
void sub_4AE4E0(int64_t obj, int radius, ObjectList* objects, unsigned int flags);
int sub_4AE570(int64_t a1, int64_t a2, int64_t a3, int skill);
void sub_4AE9E0(int64_t a1, bool a2);
void sub_4AEAB0(int64_t a1, int64_t a2);
bool ai_critter_can_open_portals(int64_t obj);
int sub_4AEB70(int64_t obj, int64_t portal, int a3);
int sub_4AED80(int64_t a1, int64_t a2);
void sub_4AF130(int64_t a1, int64_t a2);
void sub_4AF170(int64_t obj);
void sub_4AF1D0(int64_t obj);
bool sub_4AF210(int64_t obj, int64_t* danger_source_ptr);
int sub_4AF260(int64_t a1, int64_t a2);
int sub_4AF470(int64_t a1, int64_t a2, int a3);
void sub_4AF860();
bool sub_4AFB30(int64_t obj, int64_t a2);
void ai_target_lock(int64_t obj, int64_t tgt);
void ai_target_unlock(int64_t obj);

#endif /* ARCANUM_GAME_AI_H_ */
