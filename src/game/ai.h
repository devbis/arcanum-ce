#ifndef ARCANUM_GAME_AI_H_
#define ARCANUM_GAME_AI_H_

#include "game/context.h"
#include "game/timeevent.h"
#include "game/object.h"

typedef void(Func5F8488)(int64_t, int64_t, const char*, int);
typedef void(Func5F848C)(int64_t, int);

typedef enum AiDangerSourceType {
    AI_DANGER_SOURCE_TYPE_NONE,
    AI_DANGER_SOURCE_TYPE_COMBAT_FOCUS,
    AI_DANGER_SOURCE_TYPE_FLEE,
    AI_DANGER_SOURCE_TYPE_SURRENDER,
} AiDangerSourceType;

typedef enum AiAttemptOpenPortal {
    AI_ATTEMPT_OPEN_PORTAL_OK,
    AI_ATTEMPT_OPEN_PORTAL_LOCKED,
    AI_ATTEMPT_OPEN_PORTAL_JAMMED,
    AI_ATTEMPT_OPEN_PORTAL_MAGICALLY_HELD,
    AI_ATTEMPT_OPEN_PORTAL_NOT_ALLOWED,
    AI_ATTEMPT_OPEN_PORTAL_NOT_CRITTER,
} AiAttemptOpenPortal;

typedef enum AiAttemptOpenContainer {
    AI_ATTEMPT_OPEN_CONTAINER_OK,
    AI_ATTEMPT_OPEN_CONTAINER_LOCKED,
    AI_ATTEMPT_OPEN_CONTAINER_JAMMED,
    AI_ATTEMPT_OPEN_CONTAINER_MAGICALLY_HELD,
    AI_ATTEMPT_OPEN_CONTAINER_NOT_ALLOWED,
} AiAttemptOpenContainer;

typedef struct AiRedirect {
    /* 0000 */ int64_t field_0;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int min_iq;
    /* 0014 */ unsigned int critter_flags;
} AiRedirect;

bool ai_init(GameInitInfo* init_info);
void ai_exit();
bool ai_mod_load();
void ai_mod_unload();
void sub_4A84D0(Func5F848C* a1, Func5F8488* a2);
void sub_4A84F0(int64_t obj);
void sub_4A9530(AiRedirect* redirect, int64_t a2, int64_t a3);
void sub_4A9560(AiRedirect* redirect);
void sub_4A9650(int64_t source_obj, int64_t target_obj, int loudness, unsigned int flags);
void sub_4A9AD0(int64_t attacker_obj, int64_t target_obj);
void sub_4AA0D0(int64_t obj);
void sub_4AA1B0(int64_t a1, int64_t a2);
void sub_4AA300(int64_t a1, int64_t a2);
void sub_4AA4A0(int64_t obj);
void sub_4AA580(int64_t obj);
void sub_4AA7A0(int64_t obj);
void sub_4AA8C0(int64_t obj, bool force);
bool ai_npc_wait_here_timeevent_process(TimeEvent* timeevent);
bool ai_is_fighting(int64_t obj);
int ai_object_hp_ratio(int64_t obj);
int ai_critter_fatigue_ratio(int64_t obj);
int64_t sub_4ABBC0(int64_t obj);
bool ai_timeevent_process(TimeEvent* timeevent);
void sub_4AD6E0(int64_t obj);
void sub_4AD7D0(int64_t obj);
int sub_4AD800(int64_t npc_obj, int64_t pc_obj, bool a3);
int sub_4AD950(int64_t npc_obj, int64_t pc_obj, bool a3);
void sub_4AD790(int64_t obj, int a2);
int sub_4ADE00(int64_t source_obj, int64_t target_loc, int64_t* block_obj_ptr);
void sub_4ADFF0(int64_t obj);
void sub_4AE020(int64_t obj, int* cnt_ptr, int* lvl_ptr);
int sub_4AE120(int64_t a1, int64_t a2);
void sub_4AE4E0(int64_t obj, int radius, ObjectList* objects, unsigned int flags);
int sub_4AE570(int64_t a1, int64_t a2, int64_t a3, int skill);
void sub_4AE9E0(int64_t a1, bool a2);
void sub_4AEAB0(int64_t a1, int64_t a2);
bool ai_critter_can_open_portals(int64_t obj);
int ai_attempt_open_portal(int64_t obj, int64_t portal_obj, int dir);
int ai_attempt_open_container(int64_t obj, int64_t container_obj);
void sub_4AEE50(int64_t critter_obj, int64_t target_obj, int a3, int loudness);
void sub_4AF130(int64_t a1, int64_t a2);
void sub_4AF170(int64_t obj);
void sub_4AF1D0(int64_t obj);
bool sub_4AF210(int64_t obj, int64_t* danger_source_ptr);
int sub_4AF260(int64_t a1, int64_t a2);
int sub_4AF470(int64_t a1, int64_t a2, int loudness);
void sub_4AF860();
bool sub_4AFB30(int64_t obj, int64_t a2);
int sub_4AFBB0(int64_t obj);
void ai_target_lock(int64_t obj, int64_t tgt);
void ai_target_unlock(int64_t obj);

#endif /* ARCANUM_GAME_AI_H_ */
