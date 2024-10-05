#include "game/wallcheck.h"

// 0x5A3E90
static bool dword_5A3E90 = true;

// 0x5E0A00
static GameContextF8* dword_5E0A00;

// 0x5E0A08
static int64_t qword_5E0A08;

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
    // TODO: Incomplete.
}

// 0x437E00
void wallcheck_exit()
{
    // TODO: Incomplete.
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
void sub_438530()
{
    // TODO: Incomplete.
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
