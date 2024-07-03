#include "game/object.h"

#include <tig/tig.h>

#include "game/lib/effect.h"
#include "game/lib/object.h"
#include "game/lib/stat.h"

// 0x43D410
int object_get_hp_pts(object_id_t obj)
{
    return obj_f_get_int32(obj, OBJ_F_HP_PTS);
}

// 0x43D4C0
int object_get_hp_adj(object_id_t obj)
{
    return obj_f_get_int32(obj, OBJ_F_HP_ADJ);
}

// 0x43D4E0
int object_set_hp_adj(object_id_t obj, int value)
{
    obj_f_set_int32(obj, OBJ_F_HP_ADJ, value);
    sub_460240(obj);
    return value;
}

// 0x43D510
int object_get_hp_damage(object_id_t obj)
{
    return obj_f_get_int32(obj, OBJ_F_HP_DAMAGE);
}

// 0x43D530
int object_set_hp_damage(object_id_t obj, int value)
{
    int type;

    if ((tig_net_flags & 0x1) != 0 && (tig_net_flags & 0x2) == 0) {
        return 0;
    }

    if (value < 0) {
        value = 0;
    }

    sub_4EFDD0(obj, OBJ_F_HP_DAMAGE, value);

    if (value > 0) {
        type = obj_f_get_int32(obj, OBJ_F_TYPE);
        if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
            sub_45EAB0(obj);
        }
    }

    sub_4EE1D0(obj);

    return value;
}

// 0x43D5A0
int sub_43D5A0(object_id_t obj)
{
    int value;
    int type;

    value = object_get_hp_adj(obj) + sub_43D690(obj);
    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        value = effect_adjust_max_hit_points(obj, sub_43D630(obj) + value);
    }
    return value;
}

// 0x43D600
int sub_43D600(object_id_t obj)
{
    return sub_43D5A0(obj) - object_get_hp_damage(obj);
}

// 0x43D630
int sub_43D630(object_id_t obj)
{
    int type;

    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        return stat_level(obj, STAT_WILLPOWER)
            + 2 * (stat_level(obj, STAT_STRENGTH) + stat_level(obj, STAT_LEVEL))
            + 4;
    }

    return 0;
}

// 0x43D690
int sub_43D690(object_id_t obj)
{
    int type;

    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        return 4 * object_get_hp_pts(obj);
    }

    return object_get_hp_pts(obj);
}

// 0x43D880
int object_get_ac(object_id_t obj, bool a2)
{
    int ac;
    int type;
    int index;
    object_id_t item_obj;

    ac = obj_f_get_int32(obj, OBJ_F_AC);
    type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        for (index = 1000; index <= 1008; index++) {
            item_obj = item_wield_get(obj, index);
            if (item_obj != OBJ_HANDLE_NULL) {
                ac += sub_465BA0(obj, item_obj, a2);
            }
        }
    } else if (type == OBJ_TYPE_ITEM_ARMOR) {
        ac += sub_465BA0(obj, OBJ_HANDLE_NULL, a2);
    }

    if (ac < 0) {
        ac = 0;
    } else if (ac > 95) {
        ac = 95;
    }

    return ac;
}

// 0x43D940
bool sub_43D940(object_id_t obj)
{
    int type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PROJECTILE
        || type == OBJ_TYPE_CONTAINER
        || type == OBJ_TYPE_PC
        || type == OBJ_TYPE_NPC
        || (type >= OBJ_TYPE_WEAPON && type <= OBJ_TYPE_ITEM_GENERIC)) {
        return false;
    }
    return true;
}

// 0x43D990
bool sub_43D990(object_id_t obj)
{
    int type = obj_f_get_int32(obj, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PROJECTILE
        || type == OBJ_TYPE_CONTAINER
        || type == OBJ_TYPE_PC
        || type == OBJ_TYPE_NPC
        || (type >= OBJ_TYPE_WEAPON && type <= OBJ_TYPE_ITEM_GENERIC)) {
        return false;
    }
    return (obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_DYNAMIC) == 0;
}

// 0x43EAF0
void object_set_offset(object_id_t obj, int offset_x, int offset_y)
{
    if ((obj_f_get_int32(obj, OBJ_F_FLAGS) & OF_DESTROYED) == 0) {
        sub_4423E0(obj, offset_x, offset_y);
    }
}
