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
bool sub_4C0C40(int64_t pc, int64_t npc);
int sub_4C0CC0(int64_t a1, int64_t a2);
int reaction_translate(int value);
const char* reaction_get_name(int reaction);
void sub_4C1020(int64_t pc, int64_t npc);
void sub_4C10A0(int64_t pc, int64_t npc);
int64_t sub_4C1110(int64_t npc);

#endif /* ARCANUM_GAME_REACTION_H_ */
