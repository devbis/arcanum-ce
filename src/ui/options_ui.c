#include "ui/options_ui.h"

#include "game/gamelib.h"
#include "game/player.h"

static void sub_589540(int* a1, bool* enabled_ptr);
static void sub_589560(int* value_ptr, bool* enabled_ptr);
static void sub_589580(int value);
static void sub_5895A0(int* value_ptr, bool* enabled_ptr);
static void sub_5895D0(int value);
static void sub_589610(int* value_ptr, bool* enabled_ptr);
static void sub_5896A0(int value);
static void sub_589710(int* value_ptr, bool* enabled_ptr);
static void sub_589740(int value);
static void sub_589750(int* value_ptr, bool* enabled_ptr);
static void sub_589780(int value);
static void sub_589790(int* value_ptr, bool* enabled_ptr);
static void sub_5897C0(int value);
static void sub_5897D0(int* value_ptr, bool* enabled_ptr);
static void sub_589800(int value);
static void sub_589810(int* value_ptr, bool* enabled_ptr);
static void sub_589840(int value);
static void sub_589860(int* value_ptr, bool* enabled_ptr);
static void sub_589880(int value)

// 0x589530
int sub_589530(int a1)
{
    return sub_57F6D0(a1);
}

// 0x589540
void sub_589540(int* a1, bool* enabled_ptr)
{
    *a1 = dword_68726C;
    *enabled_ptr = dword_687260 == 0;
}

// 0x589560
void sub_589560(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "difficulty");
    *enabled_ptr = true;
}

// 0x589580
void sub_589580(int value)
{
    settings_set_value(&settings, "difficulty", value);
}

// 0x5895A0
void sub_5895A0(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "violence filter");
    *enabled_ptr = true;
}

// 0x5895D0
void sub_5895D0(int value)
{
    settings_set_value(&settings, "difficulty", value ? 1 : 0);
}

// 0x589610
void sub_589610(int* value_ptr, bool* enabled_ptr)
{
    if ((tig_net_flags & 0x1) != 0) {
        if (combat_is_turn_based()) {
            settings_set_value(&settings, "turn-based", 0);
            settings_set_value(&settings, "fast turn-based", 0);
        }

        *value_ptr = 0;
        *enabled_ptr = false;
    } else {
        if (combat_is_turn_based()) {
            *value_ptr = settings_get_value(&settings, "fast turn-based") ? 2 : 1;
        } else {
            *value_ptr = 0;
        }
        *enabled_ptr = true;
    }
}

// 0x5896A0
void sub_5896A0(int value)
{
    if ((tig_net_flags & 0x1) == 0) {
        switch (value) {
        case 0:
            settings_set_value(&settings, "turn-based", 0);
            settings_set_value(&settings, "fast turn-based", 0);
            break;
        case 1:
            settings_set_value(&settings, "turn-based", 1);
            settings_set_value(&settings, "fast turn-based", 0);
            break;
        case 2:
            settings_set_value(&settings, "turn-based", 1);
            settings_set_value(&settings, "fast turn-based", 1);
            break;
        }
    }
}

// 0x589710
void sub_589710(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = sub_4B8230(player_get_pc_obj()) != 0;
    *enabled_ptr = true;
}

// 0x589740
void sub_589740(int value)
{
    sub_4B8280(value);
}

// 0x589750
void sub_589750(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = sub_4B8330(player_get_pc_obj()) != 0;
    *enabled_ptr = true;
}

// 0x589780
void sub_589780(int value)
{
    sub_4B8380(value);
}

// 0x589790
void sub_589790(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = sub_4304C0(player_get_pc_obj()) != 0;
    *enabled_ptr = true;
}

// 0x5897C0
void sub_5897C0(int value)
{
    sub_430580(value);
}

// 0x5897D0
void sub_5897D0(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = sub_4C8FA0(player_get_pc_obj()) != 0;
    *enabled_ptr = true;
}

// 0x589800
void sub_589800(int value)
{
    sub_4C8FF0(value);
}

// 0x589810
void sub_589810(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "brightness");
    *enabled_ptr = tig_video_check_gamma_control() == TIG_OK;
}

// 0x589840
void sub_589840(int value)
{
    settings_set_value(&settings, "brightness", value);
}

// 0x589860
void sub_589860(int* value_ptr, bool* enabled_ptr)
{
    *value_ptr = settings_get_value(&settings, "text duration");
    *enabled_ptr = true;
}

// 0x589880
void sub_589880(int value)
{
    settings_set_value(&settings, "text duration", value);
}
