#include "game/scroll.h"

#include "game/gamelib.h"
#include "game/gsound.h"
#include "game/location.h"
#include "game/name.h"
#include "game/object.h"
#include "game/player.h"
#include "game/stat.h"
#include "game/tc.h"
#include "game/ui.h"

static void sub_40E630(int64_t dx, int64_t dy);
static void sub_40E910(int64_t a1);
static void sub_40E940();
static bool sub_40EA50(tig_art_id_t art_id);

// 0x59F050
static unsigned int scroll_fps = 1000;

// 0x5D1168
static GameInitInfo scroll_init_info;

// 0x5D1180
static int64_t scroll_center;

// 0x5D1188
static int dword_5D1188;

// 0x5D1190
static TigRect scroll_iso_content_rect;

// 0x5D11A0
static int dword_5D11A0;

// 0x5D11A4
static int dword_5D11A4;

// 0x5D11A8
static ViewOptions scroll_view_options;

// 0x5D11B8
static int64_t qword_5D11B8;

// 0x5D11C0
static bool dword_5D11C0;

// 0x5D11C4
static int scroll_distance;

// 0x5D11C8
static ScrollFunc* scroll_func;

// 0x40DF50
bool scroll_init(GameInitInfo* init_info)
{
    TigWindowData window_data;

    if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    scroll_iso_content_rect.width = window_data.rect.width;
    scroll_iso_content_rect.height = window_data.rect.height;
    scroll_iso_content_rect.y = 0;
    scroll_iso_content_rect.x = 0;

    scroll_init_info = *init_info;

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
void scroll_resize(GameResizeInfo* resize_info)
{
    scroll_iso_content_rect = resize_info->content_rect;
    scroll_init_info.iso_window_handle = resize_info->window_handle;
}

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

    int dx;
    int dy;
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    int distance;
    int64_t center_x;
    int64_t center_y;
    int v1;
    int v2;
    int v3;
    int v4;
    bool v5;
    int64_t loc;
    int rot;

    if (!scroll_init_info.editor) {
        if ((unsigned int)tig_timer_between(dword_5D117C, gamelib_ping_time) < scroll_fps) {
            return;
        }

        dword_5D117C = gamelib_ping_time;
    }

    if (scroll_func != NULL) {
        scroll_func(direction);
        return;
    }

    dx = 0;
    dy = 0;

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

    if (scroll_init_info.editor) {
        sub_40E630(dx, dy);
        return;
    }

    dword_5D11C0 = true;

    distance = scroll_get_distance();
    if (distance == 0) {
        tig_art_interface_id_create(direction + 679, 0, 0, 0, &art_id);
        sub_40EA50(art_id);
        sub_40E630(dx, dy);
        return;
    }

    location_xy(scroll_center, &center_x, &center_y);
    center_x = LOCATION_MAKE(LOCATION_GET_X(center_x) + 40, LOCATION_GET_Y(center_x) + 40);
    center_y = LOCATION_MAKE(LOCATION_GET_X(center_y) + 20, LOCATION_GET_Y(center_y) + 20);

    v1 = scroll_iso_content_rect.width / 2;
    v2 = scroll_iso_content_rect.height / 2;

    v3 = abs(v1 - dx - (int)center_x);
    v4 = abs(v2 - dy - (int)center_y);

    if (v3 < 80 * distance && v4 < 40 * distance) {
        tig_art_interface_id_create(direction + 679, 0, 0, 0, &art_id);
        sub_40EA50(art_id);
        sub_40E630(dx, dy);
        return;
    }

    location_at(v1, v2, &loc);

    rot = location_rot(loc, scroll_center);
    if (rot == (direction - 1) % 8
        || rot == (direction + 1) % 8
        || rot == direction) {
        tig_art_interface_id_create(direction + 679, 0, 0, 0, &art_id);
        sub_40EA50(art_id);
        sub_40E630(dx, dy);
        return;
    }

    v5 = false;
    if (v3 >= 80 * distance) {
        switch (direction) {
        case 1:
            direction = 0;
            dx += dword_5D11A4 + 4;
            break;
        case 2:
        case 6:
            v5 = true;
            break;
        case 3:
            direction = 4;
            dx += dword_5D11A4 + 4;
            break;
        case 5:
            direction = 4;
            dx -= dword_5D11A4 + 4;
            break;
        case 7:
            direction = 0;
            dx -= dword_5D11A4 + 4;
            break;
        }
    }

    if (v4 >= 40 * distance) {
        switch (direction) {
        case 0:
        case 4:
            v5 = true;
            break;
        case 1:
            direction = 2;
            dy -= dword_5D11A0 + 2;
            break;
        case 3:
            direction = 2;
            dy += dword_5D11A0 + 2;
            break;
        case 5:
            direction = 6;
            dy += dword_5D11A0 + 2;
            break;
        case 7:
            direction = 6;
            dy -= dword_5D11A0 + 2;
            break;
        }
    }

    if (!v5) {
        tig_art_interface_id_create(direction + 679, 0, 0, 0, &art_id);
        sub_40EA50(art_id);
        sub_40E630(dx, dy);
        return;
    }

    tig_art_interface_id_create(678, 0, 0, 0, &art_id);
    if (sub_40EA50(art_id)
        && tig_art_frame_data(art_id, &art_frame_data) == TIG_OK) {
        switch (direction) {
        case SCROLL_DIRECTION_UP:
            tig_mouse_cursor_set_offset(art_frame_data.width / 2, 0);
            break;
        case SCROLL_DIRECTION_UP_RIGHT:
            tig_mouse_cursor_set_offset(art_frame_data.width - 1, 0);
            break;
        case SCROLL_DIRECTION_RIGHT:
            tig_mouse_cursor_set_offset(art_frame_data.width - 1, art_frame_data.height / 2);
            break;
        case SCROLL_DIRECTION_DOWN_RIGHT:
            tig_mouse_cursor_set_offset(art_frame_data.width - 1, art_frame_data.height - 1);
            break;
        case SCROLL_DIRECTION_DOWN:
            tig_mouse_cursor_set_offset(art_frame_data.width / 2, art_frame_data.height - 1);
            break;
        case SCROLL_DIRECTION_DOWN_LEFT:
            tig_mouse_cursor_set_offset(art_frame_data.width / 2, art_frame_data.height - 1);
            break;
        case SCROLL_DIRECTION_LEFT:
            tig_mouse_cursor_set_offset(0, art_frame_data.height / 2);
            break;
        case SCROLL_DIRECTION_UP_LEFT:
            tig_mouse_cursor_set_offset(0, 0);
            break;
        default:
            tig_mouse_cursor_set_offset(direction, direction);
            break;
        }
    }
}

// TODO: Review name.
// 0x40E610
void scroll_stop_scrolling()
{
    if (dword_5D11C0) {
        ui_refresh_cursor();
        dword_5D11C0 = false;
    }
}

// 0x40E630
void sub_40E630(int64_t dx, int64_t dy)
{
    int64_t old_origin_x;
    int64_t old_origin_y;
    int64_t new_origin_x;
    int64_t new_origin_y;
    TigRect rect;

    scroll_init_info.redraw_func();

    location_origin_get(&old_origin_x, &old_origin_y);
    location_origin_scroll(dx, dy);
    location_origin_get(&new_origin_x, &new_origin_y);

    if (old_origin_x == new_origin_x && old_origin_y == new_origin_y) {
        return;
    }

    dx = new_origin_x - old_origin_x;
    dy = new_origin_y - old_origin_y;
    tig_window_scroll(scroll_init_info.iso_window_handle, (int)dx, (int)dy);

    if (dx > 0) {
        rect = scroll_iso_content_rect;
        rect.width = (int)dx;
        scroll_init_info.invalidate_rect_func(&rect);
    } else if (dx < 0) {
        rect = scroll_iso_content_rect;
        rect.x = rect.width + (int)dx;
        rect.width = -((int)dx);
        scroll_init_info.invalidate_rect_func(&rect);
    }

    if (dx != 0 && dy != 0) {
        scroll_init_info.redraw_func();
    }

    if (dy < 0) {
        rect = scroll_iso_content_rect;
        rect.y += rect.height + (int)dy;
        rect.height = -(int)dy;
        scroll_init_info.invalidate_rect_func(&rect);
    } else if (dy > 0) {
        rect = scroll_iso_content_rect;
        rect.height = (int)dy;
        scroll_init_info.invalidate_rect_func(&rect);
    }

    tc_scroll((int)dx, (int)dy);

    if (!scroll_init_info.editor) {
        int64_t loc;

        location_at(scroll_iso_content_rect.width / 2, scroll_iso_content_rect.height / 2, &loc);
        if (loc != qword_5D11B8) {
            gsound_listener_set(loc);
            qword_5D11B8 = loc;
        }
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

    int64_t pc = player_get_pc_obj();
    if (pc == OBJ_HANDLE_NULL) {
        return 0;
    }

    return stat_level_get(pc, STAT_PERCEPTION) / 2 + 3;
}

// 0x40E8E0
void scroll_set_center(int64_t location)
{
    scroll_center = location;
}

// 0x40E900
void scroll_set_scroll_func(ScrollFunc* func)
{
    scroll_func = func;
}

// 0x40E910
void sub_40E910(int64_t a1)
{
    if (!scroll_init_info.editor) {
        gsound_listener_set(a1);
        qword_5D11B8 = a1;
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
bool sub_40EA50(tig_art_id_t art_id)
{
    if (sub_41D510(art_id) == sub_41D510(tig_mouse_cursor_get_art_id())) {
        return true;
    }

    if (tig_mouse_cursor_set_art_id(art_id) == TIG_OK) {
        return true;
    }

    return false;
}
