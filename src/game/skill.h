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

typedef void(SkillCallbacksF0)();
typedef void(SkillCallbacksF4)();
typedef void(SkillCallbacksF8)();
typedef void(SkillCallbacksFC)();
typedef void(SkillCallbacksF10)();
typedef void(SkillCallbacksF14)();
typedef void(SkillCallbacksF18)();
typedef void(SkillCallbacksF1C)();
typedef void(SkillCallbacksF20)();

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

extern const char* off_5B6FF4[BASIC_SKILL_COUNT];
extern const char* off_5B7024[TECH_SKILL_COUNT];
extern const char* off_5B7034[TRAINING_COUNT];

bool skill_init(GameInitInfo* init_info);
void skill_set_callbacks(SkillCallbacks* callbacks);
void skill_exit();
bool skill_load(GameLoadInfo* load_info);
bool skill_save(TigFile* stream);
void skill_set_defaults(object_id_t object_id);
int sub_4C5E50(int64_t obj, int skill);
int basic_skill_level(int64_t obj, int skill);
int basic_skill_get_base(int64_t obj, int skill);
int basic_skill_set_base(int64_t obj, int skill, int value);
int basic_skill_get_training(int64_t obj, int skill);
int basic_skill_set_training(int64_t obj, int skill, int training);
const char* basic_skill_get_name(int skill);
const char* basic_skill_get_description(int skill);
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
const char* tech_skill_get_name(int skill);
const char* tech_skill_get_description(int skill);
int sub_4C69C0(int skill);
const char* training_get_name(int training);
int sub_4C69E0(int a1, int a2, int a3);
int sub_4C69F0(int64_t obj, int skill, int64_t other_obj);
int sub_4C6AF0(int64_t obj, int skill);
int sub_4C6B00(int64_t obj, int skill);
int tech_skill_get_stat(int skill);
int sub_4C6B20(int a1);
bool sub_4C6B50(int64_t obj, int stat, int value);
bool sub_4C6F90(int64_t a1, int a2, int64_t a3, int a4);
bool sub_4C6FD0(int64_t a1, int64_t a2, int64_t a3);
bool sub_4C7010(int64_t a1, int64_t a2, int64_t a3);
bool sub_4C7050(int64_t a1, int a2, int64_t a3);
void sub_4C7090(Tanya* a1);
bool sub_4C7120(Tanya* a1);
bool sub_4C7160(Tanya* a1);
int sub_4C8430(Tanya* a1);
void sub_4C8E60(int64_t a1, int64_t a2, int64_t a3, int a4);
bool get_follower_skills(int64_t obj);
void set_follower_skills(bool enabled);
void sub_4C9050(Tanya* a1);
int64_t sub_4C91F0(int64_t obj, int skill);

#endif /* ARCANUM_GAME_SKILL_H_ */
