#include "game/mt_item.h"

#include "game/magictech.h"
#include "game/obj.h"

static void sub_4CB800(int64_t a1, int64_t a2, int64_t a3, unsigned int flags);
static void sub_4CB830(int64_t a1, int64_t a2, int64_t a3, int64_t a4, unsigned int flags);

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

// 0x4CB7D0
void sub_4CB7D0(int64_t a1, int64_t a2)
{
    sub_4CB800(a1, a2, OBJ_HANDLE_NULL, MTIT_WEAR);
}

// 0x4CB800
void sub_4CB800(int64_t a1, int64_t a2, int64_t a3, unsigned int flags)
{
    sub_4CB830(a1, a2, a3, OBJ_HANDLE_NULL, flags);
}

// 0x4CB830
void sub_4CB830(int64_t a1, int64_t a2, int64_t a3, int64_t a4, unsigned int flags)
{
    // TODO: Incomplete.
}

// 0x4CBAA0
void sub_4CBAA0(int64_t a1, int64_t a2)
{
    sub_4CB800(a1, a2, OBJ_HANDLE_NULL, MTIT_PICKUP);
}

// 0x4CBB80
void sub_4CBB80(int64_t a1, int64_t a2)
{
    int type;
    int index;
    int64_t item_obj;

    if (a2 == OBJ_HANDLE_NULL) {
        return;
    }

    type = obj_f_get_int32(a2, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    for (index = 0; index < 9; index++) {
        item_obj = item_wield_get(a2, 1000 + index);
        if (item_obj != OBJ_HANDLE_NULL) {
            sub_4CB800(item_obj, a2, a1, MTIT_PARENT_STUNNED);
        }
    }
}

// 0x4CBBF0
void sub_4CBBF0(int64_t a1, int64_t a2)
{
    int type;
    int index;
    int64_t item_obj;

    if (a2 == OBJ_HANDLE_NULL) {
        return;
    }

    type = obj_f_get_int32(a2, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    for (index = 0; index < 9; index++) {
        item_obj = item_wield_get(a2, 1000 + index);
        if (item_obj != OBJ_HANDLE_NULL) {
            sub_4CB800(item_obj, a2, a1, MTIT_PARENT_GOING_UNCONSCIOUS);
        }
    }
}

// 0x4CBC60
void sub_4CBC60(int64_t a1, int64_t a2)
{
    int type;
    int index;
    int64_t item_obj;

    type = obj_f_get_int32(a2, OBJ_F_TYPE);
    if (!obj_type_is_critter(type)) {
        return;
    }

    for (index = 0; index < 9; index++) {
        item_obj = item_wield_get(a2, 1000 + index);
        if (item_obj != OBJ_HANDLE_NULL) {
            sub_4CB800(item_obj, a2, a1, MTIT_PARENT_DYING);
        }
    }
}
