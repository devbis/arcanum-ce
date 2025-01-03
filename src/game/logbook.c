#include "game/logbook.h"

#include "game/critter.h"
#include "game/obj.h"
#include "game/stat.h"

// NOTE: All compares are signed even though `OBJ_F_PC_LOGBOOK_EGO_IDX` is
// designated as unsigned.
//
// 0x4F5330
void sub_4F5330(int64_t a1, int64_t a2)
{
    int desc;
    int value;

    desc = critter_description_get(a2, a1);

    value = obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 0);
    obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 0, value + 1);

    value = stat_level(a2, STAT_LEVEL);
    if (value >= (int)obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 2)
        || obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 1) == 0) {
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 2, value);
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 1, desc);
    }
    if (value >= (int)obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 4)
        || obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 3) == 0) {
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 4, value);
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 3, desc);
    }

    value = stat_level(a2, STAT_ALIGNMENT);
    if (value > 0
        && (value >= (int)obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 6)
            || obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 5) == 0)) {
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 6, value);
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 5, desc);
    }
    if (value < 0
        && (value <= (int)obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 8)
            || obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 7) == 0)) {
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 8, value);
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 7, desc);
    }

    value = stat_level(a2, STAT_MAGICK_TECH_APTITUDE);
    if (value > 0
        && (value >= (int)obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 10)
            || obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 9) == 0)) {
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 10, value);
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 9, desc);
    }
    if (value < 0
        && (value <= (int)obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 12)
            || obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 11) == 0)) {
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 12, value);
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 11, desc);
    }
}

// 0x4F5590
void sub_4F5590(int64_t obj, int* a2)
{
    a2[0] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 0);
    a2[1] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 1);
    a2[2] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 2);
    a2[3] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 3);
    a2[4] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 4);
    a2[5] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 5);
    a2[6] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 6);
    a2[7] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 7);
    a2[8] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 8);
    a2[9] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 9);
    a2[10] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 10);
    a2[11] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 11);
    a2[12] = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 12);
}

// 0x4F5690
void sub_4F5690(int64_t a1, int64_t a2, int a3)
{
    int type;
    int desc;
    int index;

    if (a2 == OBJ_HANDLE_NULL) {
        return;
    }

    if (obj_field_int32_get(a1, OBJ_F_TYPE) != OBJ_TYPE_PC) {
        return;
    }

    type = obj_field_int32_get(a2, OBJ_F_TYPE);
    if (type != OBJ_TYPE_PC && type != OBJ_TYPE_NPC) {
        return;
    }

    if (a1 == a2) {
        desc = 30286;
    } else if (type == OBJ_TYPE_PC) {
        desc = 30287;
    } else {
        desc = critter_description_get(a2, a1);
    }

    index = 64;
    while (obj_arrayfield_uint32_get(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index)) {
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index++, desc);
        obj_arrayfield_uint32_set(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index++, a3);
    }
}

// 0x4F5770
int sub_4F5770(int64_t obj, int* a2, int* a3)
{
    *a2 = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 64);
    if (*a2 == 0) {
        return 0;
    }

    *a3 = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 65);
    return 64;
}

// 0x4F57C0
int sub_4F57C0(int64_t obj, int a2, int* a3, int* a4)
{
    *a3 = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, a2 + 2);
    if (*a3 == 0) {
        return 0;
    }

    *a4 = obj_arrayfield_uint32_get(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, a2 + 3);
    return a2 + 2;
}
