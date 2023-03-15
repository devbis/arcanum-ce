#include "game/lib/wall.h"

#include "tig/color.h"
#include "tig/rect.h"
#include "tig/window.h"

// 0x603420
static TigRect stru_603420;

// 0x603430
static bool wall_is_editor;

// 0x603434
static int dword_603434;

// 0x603438
static GameContextF8* dword_603438;

// 0x60343C
static int wall_iso_window_handle;

// 0x603440
static ViewOptions wall_view_options;

// 0x603448
static int dword_603448;

// 0x4DF390
bool wall_init(GameContext* ctx)
{
    wall_iso_window_handle = ctx->iso_window_handle;
    dword_603438 = ctx->field_8;

    TigWindowData window_data;
    if (tig_window_data(ctx->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    stru_603420.x = 0;
    stru_603420.y = 0;
    stru_603420.width = window_data.rect.width;
    stru_603420.height = window_data.rect.height;

    wall_view_options.type = VIEW_TYPE_ISOMETRIC;
    wall_is_editor = ctx->editor;
    dword_603448 = 1;
    dword_603434 = tig_color_rgb_make(255, 0, 100);

    return true;
}

// 0x4DF460
void wall_exit()
{
    wall_iso_window_handle = -1;
    dword_603438 = NULL;
}

// 0x4DF480
void wall_resize(ResizeContext* ctx)
{
    wall_iso_window_handle = ctx->iso_window_handle;
    stru_603420 = ctx->field_14;
}

// 0x4DF4C0
bool wall_update_view(ViewOptions* view_options)
{
    wall_view_options = *view_options;
    return true;
}

// 0x4DF500
void sub_4DF500(int a1)
{
    if (wall_view_options.type == VIEW_TYPE_TOP_DOWN) {
        if (dword_603448) {
            sub_4E1C00(a1);
            sub_4E1EB0(a1);
        }
    }
}
