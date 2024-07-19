#include "game/mt_item.h"

#include "game/magictech.h"
#include "game/obj.h"

// 0x5FF610
static bool mt_item_initialized;

// 0x4CB720
bool mt_item_init(GameInitInfo* init_info)
{
    (void)init_info;

    mt_item_initialized = true;

    return true;
}

// 0x4CB730
void mt_item_exit()
{
    if (mt_item_initialized) {
        mt_item_initialized = false;
    }
}

// 0x4CB760
int mt_item_triggers(int index)
{
    return index != 10000 ? magictech_spells[index].item_triggers : 0;
}

// 0x4CB790
int sub_4CB790(int a1)
{
    return a1 & 0xFF;
}

// 0x4CB7A0
int mt_item_spell(int64_t obj_handle, int index)
{
    int spl;

    if (obj_handle == OBJ_HANDLE_NULL) {
        return -1;
    }

    spl = obj_f_get_int32(obj_handle, OBJ_F_ITEM_SPELL_1 + index);
    if (spl == 10000) {
        return -1;
    }

    return spl;
}
