#ifndef ARCANUM_GAME_REACTION_H_
#define ARCANUM_GAME_REACTION_H_

#include "game/context.h"

typedef enum Reaction {
    REACTION_LOVE,
    REACTION_AMIABLE,
    REACTION_COURTEOUS,
    REACTION_NEUTRAL,
    REACTION_SUSPICIOUS,
    REACTION_DISLIKE,
    REACTION_HATRED,
    REACTION_COUNT,
} Reaction;

bool reaction_init(GameInitInfo* init_info);
void reaction_exit();
int reaction_translate(int value);
const char* reaction_get_name(int reaction);
void sub_4C1020(int64_t pc, int64_t npc);

#endif /* ARCANUM_GAME_REACTION_H_ */
