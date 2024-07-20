#include "game/tile_script.h"

// 0x5FC868
static tig_art_id_t dword_5FC868;

// 0x5FC86C
static bool tile_script_editor;

// 0x5FC870
static GameContextF8* dword_5FC870;

// 0x5FC874
static int dword_5FC874;

// 0x5FC878
static int dword_5FC878;

// 0x5FC87C
static tig_window_handle_t tile_script_iso_window_handle;

// 0x5FC880
static ViewOptions tile_script_view_options;

// 0x5FC888
static tig_art_id_t dword_5FC888;

// 0x4C0530
bool tile_script_init(GameInitInfo* init_info)
{
    tile_script_iso_window_handle = init_info->iso_window_handle;
    dword_5FC870 = init_info->field_8;
    tile_script_editor = init_info->editor;
    tile_script_view_options.type = VIEW_TYPE_ISOMETRIC;

    tig_art_interface_id_create(475, 0, 0, 0, &dword_5FC868);
    tig_art_interface_id_create(476, 0, 0, 0, &dword_5FC888);

    dword_5FC874 = 1;
    dword_5FC878 = 1;

    sub_4F6310();

    return true;
}

// 0x4C05B0
void tile_script_reset()
{
}

// 0x4C05C0
void tile_script_exit()
{
    void* next;

    dword_5FC874 = 0;

    while (off_603DD0 != NULL) {
        next = curr->field_14;
        FREE(off_603DD0);
        off_603DD0 = next;
    }

    off_603DD0 = NULL;
}

// 0x4C05D0
void tile_script_resize(ResizeInfo* resize_info)
{
    tile_script_iso_window_handle = resize_info->iso_window_handle;
}

// 0x4C05E0
bool sub_4C05E0(ViewOptions* view_options)
{
    tile_script_view_options = *view_options;

    return true;
}
