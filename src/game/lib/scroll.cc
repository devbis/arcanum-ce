#include "game/lib/scroll.h"

#include "game/lib/gamelib.h"
#include "game/lib/location.h"
#include "game/lib/object.h"
#include "game/lib/player.h"
#include "game/lib/stat.h"
#include "tig/art.h"
#include "tig/mouse.h"
#include "tig/rect.h"
#include "tig/timer.h"
#include "tig/window.h"

// 0x59F050
static unsigned int scroll_fps = 1000;

// 0x5D1168
static GameContext scroll_game_context;

// 0x5D1180
static location_t scroll_center;

// 0x5D1188
static int dword_5D1188;

// 0x5D1190
static TigRect scroll_rect;

// 0x5D11A0
static int dword_5D11A0;

// 0x5D11A4
static int dword_5D11A4;

// 0x5D11A8
static ViewOptions scroll_view_options;

// 0x5D11C0
static bool dword_5D11C0;

// 0x5D11C4
static int scroll_distance;

// 0x5D11C8
static ScrollFunc* scroll_func;

// 0x40DF50
bool scroll_init(GameContext* ctx)
{
    TigWindowData window_data;
    if (tig_window_data(ctx->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    scroll_rect.width = window_data.width;
    scroll_rect.height = window_data.height;
    scroll_rect.y = 0;
    scroll_rect.x = 0;

    scroll_game_context = *ctx;

    scroll_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_5D1188 = 1;

    sub_40E940();
    location_set_func_5FC2F8(sub_40E910);

    return true;
}

// 0x40E000
void scroll_exit()
{
}

// 0x40E010
void scroll_reset()
{
    scroll_func = NULL;
}

// 0x40E020
void scroll_resize(ResizeContext* ctx)
{
    scroll_rect = ctx->field_14;
    scroll_game_context.iso_window_handle = ctx->iso_window_handle;
}

// TODO: Review type.
// 0x40E060
bool scroll_update_view(ViewOptions* view_options)
{
    scroll_view_options = *view_options;
    sub_40E940();
    return true;
}

// TODO: Review name.
// 0x40E0A0
void scroll_start_scrolling_in_direction(int direction)
{
    // 0x5D117C
    static unsigned int dword_5D117C;

    if (!scroll_game_context.editor) {
        if (tig_timer_between(dword_5D117C, gamelib_ping_time) < scroll_fps) {
            return;
        }

        dword_5D117C = gamelib_ping_time;
    }

    if (scroll_func != NULL) {
        scroll_func(direction);
        return;
    }

    int dx = 0;
    int dy = 0;

    switch (direction) {
    case SCROLL_DIRECTION_UP:
        dy = dword_5D11A0;
        break;
    case SCROLL_DIRECTION_UP_RIGHT:
        dx = -4 - dword_5D11A4;
        dy = dword_5D11A0 + 2;
        break;
    case SCROLL_DIRECTION_RIGHT:
        dx = -dword_5D11A4;
        break;
    case SCROLL_DIRECTION_DOWN_RIGHT:
        dx = -4 - dword_5D11A4;
        dy = -2 - dword_5D11A0;
        break;
    case SCROLL_DIRECTION_DOWN:
        dy = -dword_5D11A0;
        break;
    case SCROLL_DIRECTION_DOWN_LEFT:
        dx = 4 + dword_5D11A4;
        dy = -2 - dword_5D11A0;
        break;
    case SCROLL_DIRECTION_LEFT:
        dx = dword_5D11A4;
        break;
    case SCROLL_DIRECTION_UP_LEFT:
        dx = dword_5D11A4 + 4;
        dy = dword_5D11A0 + 2;
        break;
    default:
        break;
    }

    // TODO: Incomplete.

    art_id_t art_id;
    tig_art_interface_id_create(678, 0, 0, 0, &art_id);
    if (sub_40EA50(art_id)) {
        TigArtFrameData art_frame_data;
        if (tig_art_frame_data(art_id, &art_frame_data) == TIG_OK) {
            switch (direction) {
            case SCROLL_DIRECTION_UP:
                mouse_set_offset(art_frame_data.width / 2, 0);
                break;
            case SCROLL_DIRECTION_UP_RIGHT:
                mouse_set_offset(art_frame_data.width - 1, 0);
                break;
            case SCROLL_DIRECTION_RIGHT:
                mouse_set_offset(art_frame_data.width - 1, art_frame_data.height / 2);
                break;
            case SCROLL_DIRECTION_DOWN_RIGHT:
                mouse_set_offset(art_frame_data.width - 1, art_frame_data.height - 1);
                break;
            case SCROLL_DIRECTION_DOWN:
                mouse_set_offset(art_frame_data.width / 2, art_frame_data.height - 1);
                break;
            case SCROLL_DIRECTION_DOWN_LEFT:
                mouse_set_offset(art_frame_data.width / 2, art_frame_data.height - 1);
                break;
            case SCROLL_DIRECTION_LEFT:
                mouse_set_offset(0, art_frame_data.height / 2);
                break;
            case SCROLL_DIRECTION_UP_LEFT:
                mouse_set_offset(0, 0);
                break;
            default:
                mouse_set_offset(direction, direction);
                break;
            }
        }
    }
}

// TODO: Review name.
// 0x40E610
void scroll_stop_scrolling()
{
    if (dword_5D11C0) {
        sub_460AF0();
        dword_5D11C0 = false;
    }
}

// 0x40E890
void scroll_set_fps(int fps)
{
    scroll_fps = 1000 / fps;
}

// 0x40E8A0
void scroll_set_distance(int distance)
{
    scroll_distance = distance;
}

// 0x40E8B0
int scroll_get_distance()
{
    if (scroll_distance == 0) {
        return 0;
    }

    object_id_t pc = sub_40DA50();
    if (pc == OBJECT_HANDLE_INVALID) {
        return 0;
    }

    return sub_4B0490(pc, STAT_PERCEPTION) / 2 + 3;
}

// 0x40E8E0
void scroll_set_center(location_t location)
{
    scroll_center = location;
}

// 0x40E900
void scroll_set_scroll_func(ScrollFunc* func)
{
    scroll_func = func;
}

// 0x40E910
void sub_40E910(int a1, int a2)
{
    if (!scroll_game_context.editor) {
        // TODO: Incomplete.
    }
}

// 0x40E940
void sub_40E940()
{
    if (scroll_view_options.type == VIEW_TYPE_ISOMETRIC) {
        switch (dword_5D1188) {
        case 0:
            dword_5D11A4 = 8;
            dword_5D11A0 = 4;
            break;
        case 1:
            dword_5D11A4 = 20;
            dword_5D11A0 = 10;
            break;
        case 2:
            dword_5D11A4 = 28;
            dword_5D11A0 = 14;
            break;
        case 3:
            dword_5D11A4 = 56;
            dword_5D11A0 = 28;
            break;
        }
    } else {
        switch (dword_5D1188) {
        case 0:
            dword_5D11A4 = scroll_view_options.zoom / 2;
            dword_5D11A0 = scroll_view_options.zoom / 4;
            break;
        case 1:
            dword_5D11A4 = scroll_view_options.zoom;
            dword_5D11A0 = scroll_view_options.zoom / 2;
            break;
        case 2:
            dword_5D11A0 = scroll_view_options.zoom;
            dword_5D11A4 = scroll_view_options.zoom * 2;
            break;
        case 3:
            dword_5D11A4 = scroll_view_options.zoom * 4;
            dword_5D11A0 = scroll_view_options.zoom * 2;
            break;
        }
    }
}

// 0x40EA50
bool sub_40EA50(unsigned int art_id)
{
    if (sub_41D510(art_id) == sub_41D510(sub_500150())) {
        return true;
    }

    if (sub_4FFFE0(art_id) == TIG_OK) {
        return true;
    }

    return false;
}
