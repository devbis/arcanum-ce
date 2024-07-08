#include "game/lib/reaction.h"

#include "game/lib/message.h"

// 0x5FC88C
static int reaction_msg_file;

// 0x5FC890
static char* reaction_names[REACTION_COUNT];

// 0x4C0BD0
bool reaction_init(GameContext* ctx)
{
    if (!message_load("mes\\reaction.mes", &reaction_msg_file)) {
        return false;
    }

    MessageListItem message_list_item;
    for (int index = 0; index < REACTION_COUNT; index++) {
        message_list_item.num = index;
        sub_4D43A0(reaction_msg_file, &message_list_item);
        reaction_names[index] = message_list_item.text;
    }

    return true;
}

// 0x4C0C30
void reaction_exit()
{
    message_unload(reaction_msg_file);
}

// 0x4C0FC0
int reaction_translate(int value)
{
    if (value <= 0) return REACTION_HATRED;
    if (value <= 20) return REACTION_DISLIKE;
    if (value <= 40) return REACTION_SUSPICIOUS;
    if (value <= 60) return REACTION_NEUTRAL;
    if (value <= 80) return REACTION_COURTEOUS;
    if (value <= 100) return REACTION_AMIABLE;
    return REACTION_LOVE;
}

// 0x4C1010
const char* reaction_get_name(int reaction)
{
    return reaction_names[reaction];
}
