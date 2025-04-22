#ifndef ARCANUM_GAME_SKILL_H_
#define ARCANUM_GAME_SKILL_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/object.h"

typedef enum BasicSkill {
    BASIC_SKILL_BOW,
    BASIC_SKILL_DODGE,
    BASIC_SKILL_MELEE,
    BASIC_SKILL_THROWING,
    BASIC_SKILL_BACKSTAB,
    BASIC_SKILL_PICK_POCKET,
    BASIC_SKILL_PROWLING,
    BASIC_SKILL_SPOT_TRAP,
    BASIC_SKILL_GAMBLING,
    BASIC_SKILL_HAGGLE,
    BASIC_SKILL_HEAL,
    BASIC_SKILL_PERSUATION,
    BASIC_SKILL_COUNT,
} BasicSkill;

typedef enum TechSkill {
    TECH_SKILL_REPAIR,
    TECH_SKILL_FIREARMS,
    TECH_SKILL_PICK_LOCKS,
    TECH_SKILL_DISARM_TRAPS,
    TECH_SKILL_COUNT,
} TechSkill;

typedef enum Skill {
    SKILL_BOW,
    SKILL_DODGE,
    SKILL_MELEE,
    SKILL_THROWING,
    SKILL_BACKSTAB,
    SKILL_PICK_POCKET,
    SKILL_PROWLING,
    SKILL_SPOT_TRAP,
    SKILL_GAMBLING,
    SKILL_HAGGLE,
    SKILL_HEAL,
    SKILL_PERSUATION,
    SKILL_REPAIR,
    SKILL_FIREARMS,
    SKILL_PICK_LOCKS,
    SKILL_DISARM_TRAPS,
    SKILL_COUNT,
} Skill;

#define IS_TECH_SKILL(sk) ((sk) >= BASIC_SKILL_COUNT)
#define GET_BASIC_SKILL(sk) (sk)
#define GET_TECH_SKILL(sk) ((sk) - BASIC_SKILL_COUNT)

typedef enum Training {
    TRAINING_NONE,
    TRAINING_APPRENTICE,
    TRAINING_EXPERT,
    TRAINING_MASTER,
    TRAINING_COUNT,
} Training;

typedef bool(SkillCallbacksF0)(int64_t, int, int, bool);
typedef bool(SkillCallbacksF4)(int64_t, int64_t, int64_t, bool);
typedef bool(SkillCallbacksF8)(int64_t, int64_t, int64_t, bool);
typedef bool(SkillCallbacksFC)(int64_t, int64_t, bool);
typedef bool(SkillCallbacksF10)(int64_t, int64_t, bool);
typedef bool(SkillCallbacksF14)(int64_t, int64_t, bool);
typedef bool(SkillCallbacksF18)(int64_t, int64_t, bool);
typedef bool(SkillCallbacksF1C)(int64_t, int64_t, bool);
typedef bool(SkillCallbacksF20)(int64_t);

typedef struct SkillCallbacks {
    SkillCallbacksF0* field_0;
    SkillCallbacksF4* field_4;
    SkillCallbacksF8* field_8;
    SkillCallbacksFC* field_C;
    SkillCallbacksF10* trap_output_func;
    SkillCallbacksF14* field_14;
    SkillCallbacksF18* lock_no_repair;
    SkillCallbacksF1C* lock_pick_output_func;
    SkillCallbacksF20* no_lock_output_func;
} SkillCallbacks;

static_assert(sizeof(SkillCallbacks) == 0x24, "wrong size");

// clang-format off
#define SKILL_INVOCATION_SUCCESS            0x00000001u
#define SKILL_INVOCATION_CRITICAL           0x00000010u
#define SKILL_INVOCATION_PENALTY_MSR        0x00000040u
#define SKILL_INVOCATION_PENALTY_RANGE      0x00000080u
#define SKILL_INVOCATION_PENALTY_PERCEPTION 0x00000100u
#define SKILL_INVOCATION_PENALTY_COVER      0x00000200u
#define SKILL_INVOCATION_PENALTY_LIGHT      0x00000400u
#define SKILL_INVOCATION_PENALTY_INJURY     0x00000800u
#define SKILL_INVOCATION_BLOCKED_SHOT       0x00020000u
#define SKILL_INVOCATION_MAGIC_TECH_PENALTY 0x00040000u
// clang-format on

#define SKILL_INVOCATION_PENALTY_MASK (SKILL_INVOCATION_PENALTY_MSR \
    | SKILL_INVOCATION_PENALTY_RANGE \
    | SKILL_INVOCATION_PENALTY_PERCEPTION \
    | SKILL_INVOCATION_PENALTY_COVER \
    | SKILL_INVOCATION_PENALTY_LIGHT \
    | SKILL_INVOCATION_PENALTY_INJURY \
    | SKILL_INVOCATION_BLOCKED_SHOT \
    | SKILL_INVOCATION_MAGIC_TECH_PENALTY)

typedef struct SkillInvocation {
    /* 0000 */ FollowerInfo source;
    /* 0030 */ FollowerInfo target;
    /* 0060 */ int64_t target_loc;
    /* 0068 */ FollowerInfo item;
    /* 0098 */ unsigned int flags;
    /* 009C */ int skill;
    /* 00A0 */ int modifier;
    /* 00A4 */ int hit_loc;
} SkillInvocation;

static_assert(sizeof(SkillInvocation) == 0xA8, "wrong size");

extern const char* off_5B6FF4[BASIC_SKILL_COUNT];
extern const char* off_5B7024[TECH_SKILL_COUNT];
extern const char* off_5B7034[TRAINING_COUNT];

bool skill_init(GameInitInfo* init_info);
void skill_set_callbacks(SkillCallbacks* callbacks);
void skill_exit();
bool skill_load(GameLoadInfo* load_info);
bool skill_save(TigFile* stream);
void skill_set_defaults(int64_t obj);
int sub_4C5E50(int64_t obj, int skill);
int basic_skill_level(int64_t obj, int skill);
int basic_skill_get_base(int64_t obj, int skill);
int basic_skill_set_base(int64_t obj, int skill, int value);
int basic_skill_get_training(int64_t obj, int skill);
int basic_skill_set_training(int64_t obj, int skill, int training);
char* basic_skill_get_name(int skill);
char* basic_skill_get_description(int skill);
int sub_4C62D0(int a1, int a2, int a3);
int sub_4C62E0(int64_t obj, int skill, int64_t other_obj);
int sub_4C6410(int64_t obj, int skill, int64_t other_obj);
int sub_4C64B0(int64_t obj, int skill);
int sub_4C64C0(int64_t obj, int skill);
int basic_skill_get_stat(int skill);
int sub_4C64E0(int a1);
int sub_4C6510(int64_t obj);
int sub_4C6520(int64_t obj);
int sub_4C6580(int64_t obj, int skill);
int tech_skill_level(int64_t obj, int skill);
int tech_skill_get_base(int64_t obj, int skill);
int tech_skill_set_base(int64_t obj, int skill, int value);
int tech_skill_get_training(int64_t obj, int skill);
int tech_skill_set_training(int64_t obj, int skill, int training);
char* tech_skill_get_name(int skill);
char* tech_skill_get_description(int skill);
int sub_4C69C0(int skill);
char* training_get_name(int training);
int sub_4C69E0(int a1, int a2, int a3);
int sub_4C69F0(int64_t obj, int skill, int64_t other_obj);
int sub_4C6AF0(int64_t obj, int skill);
int sub_4C6B00(int64_t obj, int skill);
int tech_skill_get_stat(int skill);
int sub_4C6B20(int a1);
bool skill_check_stat(int64_t obj, int stat, int value);
bool sub_4C6F90(int64_t obj, int skill, int64_t target_obj, unsigned int flags);
bool sub_4C6FD0(int64_t obj, int64_t target_obj, int64_t item_obj);
bool sub_4C7010(int64_t obj, int64_t target_obj, int64_t item_obj);
bool sub_4C7050(int64_t obj, int a2, int64_t target_obj);
void skill_invocation_init(SkillInvocation* skill_invocation);
bool sub_4C7120(SkillInvocation* skill_invocation);
bool skill_invocation_run(SkillInvocation* skill_invocation);
int sub_4C8430(SkillInvocation* skill_invocation);
void skill_perform_repair_service(int64_t item_obj, int64_t npc_obj, int64_t pc_obj, int cost);
bool get_follower_skills(int64_t obj);
void set_follower_skills(bool enabled);
void skill_pick_best_follower(SkillInvocation* skill_invocation);
int64_t skill_supplementary_item(int64_t obj, int skill);

#endif /* ARCANUM_GAME_SKILL_H_ */
