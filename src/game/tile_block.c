#include "game/tile_block.h"

#include <tig/tig.h>

static void sub_4BB590(int64_t a1, int a2, TigRect* rect);

// 0x5FC370
static tig_art_id_t tb_td_art_id;

// 0x5FC374
static bool tb_editor;

// 0x5FC378
static tig_art_id_t tb_iso_art_id;

// 0x5FC37C
static GameContextF8* iso_5FC37C;

// 0x5FC380
static bool dword_5FC380;

// 0x5FC388
static ViewOptions stru_5FC388;

// 0x5FC390
static tig_window_handle_t tb_iso_window;

// 0x4BAFF0
bool tileblock_init(GameInitInfo* init_info)
{
    tb_iso_window = init_info->iso_window_handle;
    iso_5FC37C = init_info->field_8;
    tb_editor = init_info->editor;
    stru_5FC388.type = VIEW_TYPE_ISOMETRIC;

    tig_art_interface_id_create(614, 0, 0, 0, &tb_iso_art_id);
    tig_art_interface_id_create(615, 0, 0, 0, &tb_td_art_id);

    dword_5FC380 = true;

    return true;
}

// 0x4BB050
void tileblock_exit()
{
}

// 0x4BB060
bool sub_4BB060(ViewOptions* view_info)
{
    stru_5FC388 = *view_info;
    return true;
}

// 0x4BB080
void tileblock_resize(ResizeInfo* resize_info)
{
    tb_iso_window = resize_info->iso_window_handle;
}

// 0x4BB090
bool sub_4BB090()
{
    return dword_5FC380;
}

// 0x4BB0A0
void sub_4BB0A0()
{
    dword_5FC380 = !dword_5FC380;
}

// 0x4BB0C0
void sub_4BB0C0()
{
    // TODO: Incomplete.
}

// 0x4BB410
void sub_4BB410()
{
    // TODO: Incomplete.
}

// 0x4BB490
void sub_4BB490()
{
    // TODO: Incomplete.
}

// 0x4BB550
void sub_4BB550(int64_t a1, TigRect* rect)
{
    sub_4BB590(sub_4CFC50(a1),
        sub_4D7090(a1),
        rect);
}

// 0x4BB590
void sub_4BB590(int64_t a1, int a2, TigRect* rect)
{
    // TODO: Incomplete.
}
