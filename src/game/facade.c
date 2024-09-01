#include "game/facade.h"

// 0x5FF580
static int dword_5FF580;

// 0x5FF584
static bool facade_editor;

// 0x5FF58C
static tig_window_handle_t facade_iso_window_handle;

// 0x5FF590
static bool facade_initialized;

// 0x5FF598
static int dword_5FF598;

// 0x4C9DA0
bool facade_init(GameInitInfo* init_info)
{
    facade_iso_window_handle = init_info->iso_window_handle;
    dword_5FF580 = init_info->field_8;
    facade_editor = init_info->editor;
    dword_5FF598 = 0;
    facade_initialized = true;

    return true;
}

// 0x4C9DE0
void facade_exit()
{
    sub_4CA240();
    facade_initialized = false;
}

// 0x4C9DF0
void facade_resize(ResizeInfo* resize_info)
{
    facade_iso_window_handle = resize_info->iso_window_handle;
}

// 0x4C9E00
void sub_4C9E00()
{
    // TODO: Incomplete.
}

// 0x4C9E70
void sub_4C9E70()
{
    // TODO: Incomplete.
}

// 0x4CA0F0
void sub_4CA0F0()
{
    // TODO: Incomplete.
}

// 0x4CA240
void sub_4CA240()
{
    // TODO: Incomplete.
}

// 0x4CA2C0
void sub_4CA2C0()
{
    // TODO: Incomplete.
}

// 0x4CA6B0
void sub_4CA6B0()
{
    // TODO: Incomplete.
}

// 0x4CA7C0
void sub_4CA7C0()
{
    // TODO: Incomplete.
}
