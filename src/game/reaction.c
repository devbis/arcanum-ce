#include "game/reaction.h"

#include "game/mes.h"

// 0x5FC88C
static int reaction_msg_file;

// 0x5FC890
static char* reaction_names[REACTION_COUNT];

// 0x4C0BD0
bool reaction_init(GameInitInfo* init_info)
{
    MesFileEntry mes_file_entry;
    int index;

    (void)init_info;

    if (!message_load("mes\\reaction.mes", &reaction_msg_file)) {
        return false;
    }

    for (index = 0; index < REACTION_COUNT; index++) {
        mes_file_entry.num = index;
        mes_get_msg(reaction_msg_file, &mes_file_entry);
        reaction_names[index] = mes_file_entry.str;
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
