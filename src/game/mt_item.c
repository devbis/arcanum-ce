#include "game/mt_item.h"

// 0x5FF610
static bool mt_item_initialized;

// 0x4CB720
bool mt_item_init(GameInitInfo* init_info)
{
    (void)init_info;

    mt_item_initialized = true;

    return true;
}
