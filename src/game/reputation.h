#ifndef ARCANUM_GAME_REPUTATION_H_
#define ARCANUM_GAME_REPUTATION_H_

#include "game/context.h"

typedef struct ReputationStateEntry {
    int reputation;
    uint64_t timestamp;
} ReputationStateEntry;

bool reputation_init(GameInitInfo* init_info);
void reputation_exit();
bool reputation_mod_load();
void reputation_mod_unload();
int reputation_reaction_adj(int64_t pc_obj, int64_t npc_obj);
void reputation_name(int reputation, char* buffer);
int reputation_copy_state(int64_t pc_obj, ReputationStateEntry* entries);
bool reputation_has(int64_t pc_obj, int reputation);
void reputation_add(int64_t pc_obj, int reputation);
void reputation_remove(int64_t pc_obj, int reputation);
int sub_4C1F80(int64_t pc_obj, int64_t npc_obj);
void sub_4C2100(int64_t pc_obj, int64_t npc_obj, int reputation, char* buffer);
bool sub_4C21E0(int64_t pc_obj, int faction);

#endif /* ARCANUM_GAME_REPUTATION_H_ */
