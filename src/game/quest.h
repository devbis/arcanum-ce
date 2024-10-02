#ifndef ARCANUM_GAME_QUEST_H_
#define ARCANUM_GAME_QUEST_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/timeevent.h"

typedef enum QuestState {
    QUEST_STATE_UNKNOWN,
    QUEST_STATE_MENTIONED,
    QUEST_STATE_ACCEPTED,
    QUEST_STATE_ACHIEVED,
    QUEST_STATE_COMPLETED,
    QUEST_STATE_OTHER_COMPLETED,
    QUEST_STATE_BOTCHED,
    QUEST_STATE_COUNT,
} QuestState;

typedef struct QuestInfo {
    /* 0000 */ int num;
    /* 0004 */ int64_t timestamp;
    /* 0010 */ int state;
} QuestInfo;

static_assert(sizeof(QuestInfo) == 0x18, "wrong size");

bool quest_init(GameInitInfo* init_info);
void quest_reset();
void quest_exit();
bool quest_mod_load();
void quest_mod_unload();
bool quest_load(GameLoadInfo* load_info);
bool quest_save(TigFile* stream);
int sub_4C4C00(int64_t a1, int64_t a2, int num);
int sub_4C4CB0(int64_t obj, int num);
void sub_4C4D20(int64_t obj, int num, int state, int64_t a4);
int sub_4C4E60(int64_t obj, int num, int state, int64_t a4);
int sub_4C5070(int64_t obj, int num);
int quest_get_state(int id);
int quest_set_state(int id, int state);
void quest_copy_description(object_id_t object_id, int quest_id, char* buffer);
int quest_get_xp(int xp_level);
bool sub_4C5400(int64_t a1, int64_t a2);

#endif /* ARCANUM_GAME_QUEST_H_ */
