#include "game/target.h"

#include "game/critter.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/stat.h"

static void sub_4F52D0(int64_t obj, int index);

// 0x5BC48C
static int dword_5BC48C[12] = {
    0x0000,
    0x0001,
    0x0002,
    0x0004,
    0x0008,
    0x0010,
    0x0020,
    0x0040,
    0x0080,
    0x0100,
    0x0200,
    0x0400,
};

// 0x603BB0
static TigRect stru_603BB0;

// 0x603CB8
static S603CB8 stru_603CB8;

// 0x603D20
static S603D20 stru_603D20;

// 0x603D38
static bool target_initialized;

// 0x603D40
static int64_t qword_603D40;

// 0x603D48
static void(*dword_603D48)(int64_t obj, int index);

// 0x4F24F0
bool target_init(GameInitInfo* init_info)
{
    TigWindowData window_data;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    stru_603BB0 = window_data.rect;
    sub_4F2600(&stru_603CB8, &stru_603D20, OBJ_HANDLE_NULL);
    target_initialized = true;

    return true;
}

// 0x4F2560
void target_exit()
{
    target_initialized = false;
}

// 0x4F2570
void target_reset()
{
    qword_603D40 = OBJ_HANDLE_NULL;
}

// 0x4F2580
void target_resize(ResizeInfo* resize_info)
{
    stru_603BB0 = resize_info->field_14;
}

// 0x4F25B0
void sub_4F25B0()
{
    // TODO: Incomplete.
}

// 0x4F25D0
void sub_4F25D0()
{
    // TODO: Incomplete.
}

// 0x4F25E0
void sub_4F25E0(S603D20* a1)
{
    a1->field_0 = 0;
    a1->field_4 = 0;
    a1->field_14 = 0;
    a1->field_10 = 0;
    a1->field_C = 0;
    a1->field_8 = 0;
}

// 0x4F2600
void sub_4F2600(S603CB8 *a1, S603D20 *a2, int64_t a3)
{
    a1->field_38 = 0;
    a1->field_30 = 0;
    a1->field_40 = 0;
    a1->field_10 = a3;
    a1->field_8 = a3;
    a1->field_34 = 0;
    a1->field_0 = a2;
    a1->field_54 = 0;
    a1->field_58 = 0;
    a1->field_50 = 0;
    if (a3 != OBJ_HANDLE_NULL) {
        a1->field_18 = obj_field_int64_get(a3, OBJ_F_LOCATION);
    }
    a1->field_48 = 0;
    a1->field_28 = 0;
    a1->field_20 = 0;
    a1->field_4C = 0;
    a1->field_2C = 0;
    a1->field_60 = 0;

    if (a2 != NULL) {
        sub_4F25E0(a2);
    }
}

// 0x4F2680
void sub_4F2680()
{
    // TODO: Incomplete.
}

// 0x4F27F0
void sub_4F27F0()
{
    // TODO: Incomplete.
}

// 0x4F2810
void sub_4F2810()
{
    // TODO: Incomplete.
}

// 0x4F2830
void sub_4F2830()
{
    // TODO: Incomplete.
}

// 0x4F28A0
void sub_4F28A0()
{
    // TODO: Incomplete.
}

// 0x4F2C60
void sub_4F2C60()
{
    // TODO: Incomplete.
}

// 0x4F2CB0
void sub_4F2CB0()
{
    // TODO: Incomplete.
}

// 0x4F2D10
int64_t sub_4F2D10()
{
    return qword_603D40;
}

// 0x4F2D20
void sub_4F2D20()
{
    // TODO: Incomplete.
}

// 0x4F3F10
void sub_4F3F10()
{
    // TODO: Incomplete.
}

// 0x4F3FD0
void sub_4F3FD0()
{
    // TODO: Incomplete.
}

// 0x4F4050
void sub_4F4050()
{
    // TODO: Incomplete.
}

// 0x4F40B0
void sub_4F40B0()
{
    // TODO: Incomplete.
}

// 0x4F4E40
void sub_4F4E40()
{
    // TODO: Incomplete.
}

// 0x4F5090
bool sub_4F5090(int64_t obj, int index)
{
    return (obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE) & dword_5BC48C[index]) != 0;
}

// 0x4F50C0
void sub_4F50C0()
{
    // TODO: Incomplete.
}

// 0x4F51B0
void sub_4F51B0()
{
    // TODO: Incomplete.
}

// 0x4F5270
void sub_4F5270(int64_t obj, int index)
{
    unsigned int flags;

    flags = obj_field_int32_get(obj, OBJ_F_PC_FLAGS_FATE);
    if ((flags & dword_5BC48C[index]) == 0) {
        return false;
    }

    flags &= ~dword_5BC48C[index];
    obj_field_int32_set(obj, OBJ_F_PC_FLAGS_FATE, flags);

    if (dword_603D48 != NULL) {
        dword_603D48(obj, index);
    }

    return true;
}

// 0x4F52D0
void sub_4F52D0(int64_t obj, int index)
{
    if (index == 0) {
        object_set_hp_damage(obj, 0);
        stat_set_base(obj, STAT_POISON_LEVEL, 0);
        critter_fatigue_damage_set(obj, 0);
    }

    if (dword_603D48 != NULL) {
        dword_603D48(obj, index);
    }
}

// 0x4F5320
void sub_4F5320(void(*func)(int64_t obj, int index))
{
    dword_603D48 = func;
}

// 0x4F5330
void sub_4F5330(int64_t a1, int64_t a2)
{
    int desc;
    int value;

    desc = critter_description_get(a2, a1);

    value = sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 0);
    sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 0, value + 1);

    value = stat_level(a2, STAT_LEVEL);
    if (value >= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 2)
        || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 1) == 0) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 2, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 1, desc);
    }
    if (value >= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 4)
        || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 3) == 0) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 4, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 3, desc);
    }

    value = stat_level(a2, STAT_ALIGNMENT);
    if (value > 0
        && (value >= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 6)
            || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 5) == 0)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 6, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 5, desc);
    }
    if (value < 0
        && (value <= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 8)
            || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 7) == 0)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 8, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 7, desc);
    }

    value = stat_level(a2, STAT_MAGICK_TECH_APTITUDE);
    if (value > 0
        && (value >= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 10)
            || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 9) == 0)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 10, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 9, desc);
    }
    if (value < 0
        && (value <= sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 12)
            || sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 11) == 0)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 12, value);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, 11, desc);
    }
}

// 0x4F5590
void sub_4F5590(int64_t obj, int* a2)
{
    a2[0] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 0);
    a2[1] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 1);
    a2[2] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 2);
    a2[3] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 3);
    a2[4] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 4);
    a2[5] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 5);
    a2[6] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 6);
    a2[7] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 7);
    a2[8] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 8);
    a2[9] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 9);
    a2[10] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 10);
    a2[11] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 11);
    a2[12] = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 12);
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
    while (sub_407470(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index)) {
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index++, desc);
        sub_4074E0(a1, OBJ_F_PC_LOGBOOK_EGO_IDX, index++, a3);
    }
}

// 0x4F5770
int sub_4F5770(int64_t obj, int* a2, int* a3)
{
    *a2 = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 64);
    if (*a2 == 0) {
        return 0;
    }

    *a3 = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, 65);
    return 64;
}

// 0x4F57C0
int sub_4F57C0(int64_t obj, int a2, int* a3, int* a4)
{
    *a3 = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, a2 + 2);
    if (*a3 == 0) {
        return 0;
    }

    *a4 = sub_407470(obj, OBJ_F_PC_LOGBOOK_EGO_IDX, a2 + 3);
    return a2 + 2;
}
