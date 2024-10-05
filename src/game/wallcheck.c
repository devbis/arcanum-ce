#include "game/wallcheck.h"

#include "game/player.h"

typedef struct S5E0E20 {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
} S5E0E20;

static_assert(sizeof(S5E0E20) == 0x20, "wrong size");

// 0x5A3E90
static bool dword_5A3E90 = true;

// 0x5E0A00
static GameContextF8* dword_5E0A00;

// 0x5E0A08
static int64_t qword_5E0A08;

// 0x5E0E10
static int dword_5E0E10;

// 0x5E0E18
static int64_t qword_5E0E18;

// 0x5E0E20
static S5E0E20 stru_5E0E20[256];

// 0x5E2E24
static int dword_5E2E24;

// 0x5E2E28
static int dword_5E2E28;

// 0x5DE6E8
static bool dword_5DE6E8;

// 0x437D90
bool wallcheck_init(GameInitInfo* init_info)
{
    dword_5E2E24 = 0;
    dword_5E2E28 = 0;
    dword_5E0A00 = init_info->field_8;

    return true;
}

// 0x437DB0
void wallcheck_reset()
{
    int index;

    if (dword_5E2E24 > 0) {
        for (index = 0; index < dword_5E2E24; index++) {
            stru_5E0E20[index].field_0 &= ~0x2;
            stru_5E0E20[index].field_0 |= 0x1;
        }

        sub_438830();
    }

    qword_5E0E18 = 0;
    dword_5E0E10 = 0;
    dword_5DE6E8 = 0;
}

// 0x437E00
void wallcheck_exit()
{
    wallcheck_reset();
    dword_5E0A00 = 0;
}

// 0x437E10
void sub_437E10()
{
    if (dword_5DE6E8) {
        sub_439D30(qword_5E0A08);
        sub_437E50(qword_5E0A08);
        dword_5DE6E8 = false;
    }
}

// 0x437E50
void sub_437E50()
{
    // TODO: Incomplete.
}

// 0x4384F0
void wallcheck_flush()
{
    // TODO: Incomplete.
}

// 0x438500
void sub_438500(bool enabled)
{
    if (dword_5A3E90 != enabled) {
        dword_5A3E90 = enabled;
    }
}

// 0x438520
bool sub_438520()
{
    return dword_5A3E90;
}

// 0x438530
void sub_438530(int64_t obj)
{
    if (player_is_pc_obj(obj)) {
        qword_5E0A08 = obj_field_int64_get(obj, OBJ_F_LOCATION);
        dword_5DE6E8 = true;
    }
}

// 0x438570
void sub_438570()
{
    // TODO: Incomplete.
}

// 0x4386B0
void sub_4386B0()
{
    // TODO: Incomplete.
}

// 0x438720
void sub_438720()
{
    // TODO: Incomplete.
}

// 0x4387C0
void sub_4387C0()
{
    // TODO: Incomplete.
}

// 0x438830
void sub_438830()
{
    // TODO: Incomplete.
}

// 0x438A50
void wallcheck_roof_faded_clear()
{
    // TODO: Incomplete.
}
