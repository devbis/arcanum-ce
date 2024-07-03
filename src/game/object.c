#include "game/object.h"

#include "game/lib/object.h"

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
