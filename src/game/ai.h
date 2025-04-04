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

typedef enum AiSpeak {
    AI_SPEAK_OK,
    AI_SPEAK_DEAD,
    AI_SPEAK_COMBAT,
    AI_SPEAK_ANIM,
    AI_SPEAK_REACTION,
    AI_SPEAK_CANNOT,
    AI_SPEAK_SLEEP_UNCONSCIOUS,
} AiSpeak;

typedef enum AiFollow {
    AI_FOLLOW_OK,

    // The NPC will not follow the PC because the PC is too good.
    AI_FOLLOW_TOO_GOOD,

    // The NPC will not follow the PC because the PC is too evil.
    AI_FOLLOW_TOO_BAD,

    // The NPC will not follow the PC because the NPC dislikes the PC.
    AI_FOLLOW_DISLIKE,

    // The NPC will not follow the PC because the NPC is already grouped with
    // another PC who is at least as charismatic.
    AI_FOLLOW_ALREADY_IN_GROUP,

    // The NPC will not follow the PC because the PC is at his charisma limit
    // (which is greater zero).
    AI_FOLLOW_LIMIT_REACHED,

    // The NPC will not follow the PC because the PC's charisma does not allow
    // for followers.
    AI_FOLLOW_NOT_ALLOWED,

    // The NPC is too high level for the PC to be a follower.
    AI_FOLLOW_LOW_LEVEL,
} AiFollow;

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
void ai_process(int64_t obj);
void sub_4A9530(AiRedirect* redirect, int64_t a2, int64_t a3);
void sub_4A9560(AiRedirect* redirect);
void sub_4A9650(int64_t source_obj, int64_t target_obj, int loudness, unsigned int flags);
void sub_4A9AD0(int64_t attacker_obj, int64_t target_obj);
void sub_4AA0D0(int64_t obj);
void sub_4AA1B0(int64_t a1, int64_t a2);
void sub_4AA300(int64_t a1, int64_t a2);
void ai_stop_attacking(int64_t obj);
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
int ai_can_speak(int64_t npc_obj, int64_t pc_obj, bool a3);
int ai_check_follow(int64_t npc_obj, int64_t pc_obj, bool ignore_charisma_limits);
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
bool ai_shitlist_has(int64_t npc_obj, int64_t shit_obj);
int sub_4AFBB0(int64_t obj);
void ai_target_lock(int64_t obj, int64_t tgt);
void ai_target_unlock(int64_t obj);

#endif /* ARCANUM_GAME_AI_H_ */
