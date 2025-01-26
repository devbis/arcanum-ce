#ifndef ARCANUM_GAME_MT_AI_H_
#define ARCANUM_GAME_MT_AI_H_

#include "game/context.h"

typedef enum AiAction {
    AI_ACTION_FLEE,
    AI_ACTION_SUMMON,
    AI_ACTION_DEFENSIVE,
    AI_ACTION_OFFENSIVE,
    AI_ACTION_HEAL_LIGHT,
    AI_ACTION_HEAL_MEDIUM,
    AI_ACTION_HEAL_HEAVY,
    AI_ACTION_CURE_POISON,
    AI_ACTION_FATIGUE_RECOVER,
    AI_ACTION_RESURRECT,
    AI_ACTION_COUNT,
} AiAction;

typedef struct AiActionListEntry {
    /* 0000 */ int spell;
    /* 0008 */ int64_t item_obj;
} AiActionListEntry;

// See 0x4CC3C0.
static_assert(sizeof(AiActionListEntry) == 0x10, "wrong size");

typedef struct S600A20 {
    /* 0000 */ int cnt;
    /* 0004 */ AiActionListEntry* entries;
} S600A20;

static_assert(sizeof(S600A20) == 0x8, "wrong size");

typedef struct AiActionList {
    /* 0000 */ int64_t obj;
    /* 0008 */ int64_t field_8;
    /* 0010 */ int action;
    /* 0014 */ int field_14;
    /* 0018 */ int64_t field_18;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ S600A20 actions[AI_ACTION_COUNT];
} AiActionList;

static_assert(sizeof(AiActionList) == 0x80, "wrong size");

bool mt_ai_init(GameInitInfo* init_info);
void mt_ai_reset();
void mt_ai_exit();
int sub_4CC2A0(int spl);
void mt_ai_action_list_create(AiActionList* ai_action_list, int64_t obj, int action);
void mt_ai_action_list_destroy(AiActionList* ai_action_list);
void mt_ai_notify_inventory_changed(int64_t obj);
void mt_ai_notify_item_exhausted(int64_t obj, int64_t item_obj);

#endif /* ARCANUM_GAME_MT_AI_H_ */
