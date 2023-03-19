#ifndef ARCANUM_GAME_LIB_SKILL_H_
#define ARCANUM_GAME_LIB_SKILL_H_

#include "game/context.h"
#include "game/lib/object.h"

typedef enum PrimarySkill {
    PRIMARY_SKILL_BOW,
    PRIMARY_SKILL_DODGE,
    PRIMARY_SKILL_MELEE,
    PRIMARY_SKILL_THROWING,
    PRIMARY_SKILL_BACKSTAB,
    PRIMARY_SKILL_PICK_POCKET,
    PRIMARY_SKILL_PROWLING,
    PRIMARY_SKILL_SPOT_TRAP,
    PRIMARY_SKILL_GAMBLING,
    PRIMARY_SKILL_HAGGLE,
    PRIMARY_SKILL_HEAL,
    PRIMARY_SKILL_PERSUATION,
    PRIMARY_SKILL_COUNT,
};

typedef enum SecondarySkill {
    SECONDARY_SKILL_REPAIR,
    SECONDARY_SKILL_FIREARMS,
    SECONDARY_SKILL_PICK_LOCKS,
    SECONDARY_SKILL_DISARM_TRAPS,
    SECONDARY_SKILL_COUNT,
};

typedef enum Training {
    TRAINING_NONE,
    TRAINING_APPRENTICE,
    TRAINING_EXPERT,
    TRAINING_MASTER,
    TRAINING_COUNT,
};

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
    SkillCallbacksF10* field_10;
    SkillCallbacksF14* field_14;
    SkillCallbacksF18* field_18;
    SkillCallbacksF1C* field_1C;
    SkillCallbacksF20* field_20;
};

static_assert(sizeof(SkillCallbacks) == 0x24, "wrong size");

bool skill_init(GameContext* ctx);
void skill_set_callbacks(SkillCallbacks* callbacks);
void skill_exit();
bool skill_load(LoadContext* ctx);
bool skill_save(TigFile* stream);
void skill_set_defaults(object_id_t object_id);
const char* primary_skill_get_name(int skill);
const char* primary_skill_get_description(int skill);
int sub_4C62D0(int a1, int a2, int a3);
int sub_4C64B0();
int sub_4C64C0();
int primary_skill_get_stat(int skill);
int sub_4C6510();
const char* secondary_skill_get_name(int skill);
const char* secondary_skill_get_description(int skill);
const char* training_get_name(int training);
int sub_4C69E0(int a1, int a2, int a3);
int sub_4C6AF0();
int sub_4C6B00();
int secondary_skill_get_stat(int skill);

#endif /* ARCANUM_GAME_LIB_SKILL_H_ */
