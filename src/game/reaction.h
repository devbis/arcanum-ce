#ifndef ARCANUM_GAME_LIB_REACTION_H_
#define ARCANUM_GAME_LIB_REACTION_H_

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
};

bool reaction_init(GameContext* ctx);
void reaction_exit();
int reaction_translate(int value);
const char* reaction_get_name(int reaction);

#endif /* ARCANUM_GAME_LIB_REACTION_H_ */
