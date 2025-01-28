#include "ui/mp_ctrl_ui.h"

#include "game/gsound.h"
#include "game/obj_private.h"
#include "ui/multiplayer_ui.h"
#include "ui/server_list_ui.h"

static void sub_569D60();
static bool sub_569DC0(TimeEvent* timeevent);
static bool sub_569F40(int num);
static void sub_56A000();

// 0x5CA350
static tig_window_handle_t dword_5CA350 = TIG_WINDOW_HANDLE_INVALID;

// 0x67BB14
static bool mp_ctrl_ui_initialized;

// 0x569C50
bool mp_ctrl_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    mp_ctrl_ui_initialized = true;

    return true;
}

// 0x569C60
void mp_ctrl_ui_exit()
{
    mp_ctrl_ui_initialized = false;
}

// 0x569C70
void sub_569C70(int a1, int a2, void* a3)
{
    sub_586150(a1, a2, a3);

    switch (a1) {
    case 5:
        sub_570890();
        sub_569D60();
        break;
    case 7:
    case 9:
        sub_570890();
        break;
    case 12:
    case 115:
        sub_5708B0(0);
        break;
    case 116:
        sub_5703D0(a3, a2);
        break;
    }
}

// 0x569D60
void sub_569D60()
{
    TimeEvent timeevent;
    DateTime datetime;

    timeevent.type = TIMEEVENT_TYPE_MP_CTRL_UI;
    timeevent.params[0].integer_value = 2;
    timeevent.params[1].integer_value = 0;

    datetime.days = 0;
    datetime.milliseconds = 2000;

    sub_45B800(&timeevent, &datetime);
}

// 0x569DA0
void sub_569DA0()
{
    timeevent_clear_all_ex(TIMEEVENT_TYPE_MP_CTRL_UI, sub_569DC0);
    sub_569F30();
}

// 0x569DC0
bool sub_569DC0(TimeEvent* timeevent)
{
    return timeevent->params[0].integer_value == 2;
}

// 0x569DD0
bool mp_ctrl_ui_process_callback(TimeEvent* timeevent)
{
    TimeEvent next_timeevent;
    DateTime next_datetime;

    switch (timeevent->params[0].integer_value) {
    case 0:
        sub_5708B0(0);
        break;
    case 1:
        if (sub_4E5470(timeevent->params[1].object_value)
            && sub_4E5470(timeevent->params[2].object_value)) {
            sub_5704E0(timeevent->params[1].object_value,
                timeevent->params[2].object_value,
                timeevent->params[3].integer_value);
        }
        break;
    case 2:
        if (!tig_net_auto_join_is_enabled()) {
            next_timeevent.type = TIMEEVENT_TYPE_MP_CTRL_UI;
            next_timeevent.params[0].integer_value = 2;

            next_datetime.days = 0;

            switch (timeevent->params[1].integer_value) {
            case 0:
                sub_56A000();
                next_timeevent.params[1].integer_value = 1;
                next_datetime.milliseconds = 250;
                break;
            case 1:
                sub_569F30();
                next_timeevent.params[1].integer_value = 2;
                next_datetime.milliseconds = 250;
                break;
            case 2:
                sub_56A000();
                next_timeevent.params[1].integer_value = 3;
                next_datetime.milliseconds = 250;
                break;
            case 3:
                sub_569F30();
                next_timeevent.params[1].integer_value = 0;
                next_datetime.milliseconds = 1500;
                break;
            default:
                return true;
            }

            sub_45B800(&next_timeevent, &next_datetime);
        }
        break;
    }

    return true;
}

// 0x569F20
bool sub_569F20(tig_window_handle_t window_handle)
{
    dword_5CA350 = window_handle;
    return true;
}

// 0x569F30
void sub_569F30()
{
    sub_569F40(809);
}

// 0x569F40
bool sub_569F40(int num)
{
    TigArtFrameData art_frame_data;
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;

    if (!tig_net_is_active()) {
        return false;
    }

    if (dword_5CA350 == TIG_WINDOW_HANDLE_INVALID) {
        return false;
    }

    if (tig_art_interface_id_create(num, 0, 0, 0, &(blit_info.art_id)) != TIG_OK) {
        return false;
    }

    if (tig_art_frame_data(blit_info.art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    src_rect.y = 0;
    src_rect.x = 0;
    src_rect.width = art_frame_data.width;
    src_rect.width = art_frame_data.width;

    dst_rect.x = 571;
    dst_rect.y = 2;
    dst_rect.width = art_frame_data.width;
    dst_rect.width = art_frame_data.width;

    blit_info.flags = 0;
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;

    if (tig_window_blit_art(dword_5CA350, &blit_info) != TIG_OK) {
        return false;
    }

    return true;
}

// 0x56A000
void sub_56A000()
{
    gsound_play_sfx_id(8815, 1);
    sub_569F40(810);
}
