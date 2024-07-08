#ifndef ARCANUM_GAME_LIB_QUEST_H_
#define ARCANUM_GAME_LIB_QUEST_H_

#include "game/context.h"
#include "game/lib/object.h"

typedef enum QuestState {
    QUEST_STATE_UNKNOWN,
    QUEST_STATE_MENTIONED,
    QUEST_STATE_ACCEPTED,
    QUEST_STATE_ACHIEVED,
    QUEST_STATE_COMPLETED,
    QUEST_STATE_OTHER_COMPLETED,
    QUEST_STATE_BOTCHED,
    QUEST_STATE_COUNT,
};

bool quest_init(GameContext* ctx);
void quest_reset();
void quest_exit();
bool quest_mod_load();
void quest_mod_unload();
bool quest_load(LoadContext* ctx);
bool quest_save(TigFile* stream);
int quest_get_state(int id);
int quest_set_state(int id, int state);
void quest_copy_description(object_id_t object_id, int quest_id, char* buffer);
int quest_get_xp(int xp_level);

#endif /* ARCANUM_GAME_LIB_QUEST_H_ */
