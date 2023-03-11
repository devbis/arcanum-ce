#include "tig/mouse.h"

#include "tig/art.h"
#include "tig/color.h"
#include "tig/dxinput.h"
#include "tig/message.h"
#include "tig/tig.h"
#include "tig/timer.h"
#include "tig/video.h"
#include "tig/window.h"

static int tig_mouse_device_init();
static int sub_4FF2A0();
static void tig_mouse_exit();
static void sub_4FF340();
static bool tig_mouse_update_state();
static bool tig_mouse_device_update_state();
static void sub_4FFBF0();
static bool sub_4FFD60(unsigned int art_id, int a2, int a3);
static bool sub_4FFEA0();
static bool sub_4FFEC0(unsigned int art_id, int a2, int a3);
static void sub_500520();

// 0x5BE840
static int dword_5BE840[TIG_MOUSE_BUTTON_COUNT] = {
    0x2,
    0x10,
    0x80,
};

// 0x5BE84C
static int dword_5BE84C[TIG_MOUSE_BUTTON_COUNT] = {
    0x4,
    0x20,
    0x100,
};

// 0x5BE858
static int dword_5BE858[TIG_MOUSE_BUTTON_COUNT] = {
    0x8,
    0x40,
    0x200,
};

// 0x5BE864
static int dword_5BE864[TIG_MOUSE_BUTTON_COUNT] = {
    0x0,
    0x2,
    0x4,
};

// 0x5BE870
static int dword_5BE870[TIG_MOUSE_BUTTON_COUNT] = {
    0x1,
    0x3,
    0x5,
};

// 0x5BE87C
static bool tig_mouse_z_axis_enabled = true;

// 0x604640
static int dword_604640;

// 0x604644
static TigVideoBuffer* dword_604644;

// 0x604648
static int dword_604648;

// 0x60464C
static int dword_60464C;

// 0x604650
static int dword_604650;

// 0x604654
static unsigned int dword_604654;

// 0x604658
static int dword_604658;

// 0x60465C
static unsigned int dword_60465C;

// 0x604660
static int dword_604660;

// 0x604664
static int dword_604664;

// 0x604668
static int dword_604668;

// 0x60466C
static TigVideoBuffer* dword_60466C;

// 0x604670
static int dword_604670;

// 0x604674
static int dword_604674;

// 0x604678
static bool tig_mouse_active;

// 0x60467C
static int dword_60467C;

// 0x604680
static unsigned int dword_604680[TIG_MOUSE_BUTTON_COUNT];

// 0x604690
static TigMouseState tig_mouse_state;

// 0x6046B8
static int dword_6046B8;

// 0x6046C0
static TigRect stru_6046C0;

// 0x6046D0
static unsigned int dword_6046D0;

// 0x6046D4
static bool tig_mouse_buttons_swapped;

// 0x6046D8
static TigRect stru_6046D8;

// 0x6046E8
static int dword_6046E8;

// 0x6046EC
static int dword_6046EC[3];

// 0x6046F8
static DIMOUSESTATE tig_mouse_device_state;

// 0x604708
static LPDIRECTINPUTDEVICEA tig_mouse_device;

// 0x60470C
static bool tig_mouse_initialized;

// 0x4FF020
int tig_mouse_init(TigContext* ctx)
{
    if (tig_mouse_initialized) {
        return TIG_ALREADY_INITIALIZED;
    }

    dword_604650 = ctx->width - 1;
    dword_60464C = ctx->height - 1;

    tig_mouse_state.rect.x = ctx->width / 2;
    tig_mouse_state.rect.y = ctx->height / 2;
    tig_mouse_state.rect.width = 0;
    tig_mouse_state.rect.height = 0;
    tig_mouse_state.x = ctx->width / 2;
    tig_mouse_state.y = ctx->height / 2;
    tig_mouse_state.flags = 0;

    if ((ctx->flags & TIG_CONTEXT_FLAG_0x20) != 0) {
        return sub_4FF2A0();
    } else {
        return tig_mouse_device_init();
    }
}

// 0x4FF0B0
int tig_mouse_device_init()
{
    LPDIRECTINPUTA dinput;
    if (tig_dxinput_get_instance(&dinput) != TIG_OK) {
        return TIG_ERR_7;
    }

    if (FAILED(IDirectInput_CreateDevice(dinput, GUID_SysMouse, &tig_mouse_device, NULL))) {
        return TIG_ERR_7;
    }

    if (FAILED(IDirectInputDevice_SetDataFormat(tig_mouse_device, &c_dfDIMouse))) {
        tig_mouse_device_exit();
        return TIG_ERR_7;
    }

    HWND hwnd;
    if (tig_video_get_hwnd(&hwnd) != TIG_OK) {
        tig_mouse_device_exit();
        return TIG_ERR_7;
    }

    if (FAILED(IDirectInputDevice_SetCooperativeLevel(tig_mouse_device, hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND))) {
        tig_mouse_device_exit();
        return TIG_ERR_7;
    }

    unsigned int art_id;
    int rc = sub_501DD0(0, 0, &art_id);
    if (rc != TIG_OK) {
        return rc;
    }

    if (sub_4FFD60(art_id, 0, 0) != TIG_OK) {
        tig_mouse_device_exit();
        return TIG_ERR_16;
    }

    if (sub_4FFEC0(art_id, 0, 0) != TIG_OK) {
        tig_mouse_device_exit();
        return TIG_ERR_16;
    }

    dword_6046E8 = 0;
    tig_mouse_initialized = true;
    dword_604658 = false;

    tig_mouse_buttons_swapped = GetSystemMetrics(SM_SWAPBUTTON) != 0;
    if (tig_mouse_buttons_swapped) {
        int temp;

        temp = dword_5BE840[1];
        dword_5BE840[1] = dword_5BE840[0];
        dword_5BE840[0] = temp;

        temp = dword_5BE84C[1];
        dword_5BE84C[1] = dword_5BE84C[0];
        dword_5BE84C[0] = temp;

        temp = dword_5BE858[1];
        dword_5BE858[1] = dword_5BE858[0];
        dword_5BE858[0] = temp;

        temp = dword_5BE864[1];
        dword_5BE864[1] = dword_5BE864[0];
        dword_5BE864[0] = temp;

        temp = dword_5BE870[1];
        dword_5BE870[1] = dword_5BE870[0];
        dword_5BE870[0] = temp;
    }

    return TIG_OK;
}

// 0x4FF2A0
int sub_4FF2A0()
{
    dword_6046E8 = 0;
    tig_mouse_state.rect.height = 0;
    tig_mouse_state.rect.width = 0;
    tig_mouse_initialized = true;
    dword_604658 = true;

    return TIG_OK;
}

// 0x4FF2D0
void tig_mouse_exit()
{
    if (tig_mouse_initialized) {
        if (dword_604658) {
            sub_4FF340();
        } else {
            tig_mouse_device_exit();
            sub_4FFEA0();
        }
        tig_mouse_initialized = false;
    }
}

// 0x4FF310
void tig_mouse_device_exit()
{
    if (tig_mouse_device != NULL) {
        tig_mouse_set_active(false);
        IDirectInputDevice_Release(tig_mouse_device);
        tig_mouse_device = NULL;
    }
}

// 0x4FF340
void sub_4FF340()
{
}

// 0x4FF350
void tig_mouse_set_active(bool is_active)
{
    if (tig_mouse_device != NULL) {
        if (is_active) {
            IDirectInputDevice_Acquire(tig_mouse_device);
            tig_mouse_active = is_active;
        } else {
            IDirectInputDevice_Release(tig_mouse_device);
            tig_mouse_active = is_active;
        }
    } else {
        tig_mouse_active = is_active;
    }
}

// NOTE: Rare case among `ping` functions - returns `bool`. Other functions
// usually `void`. This value is never used by the callers.
//
// 0x4FF390
bool tig_mouse_ping()
{
    if (!tig_mouse_active) {
        return false;
    }

    if (dword_604658) {
        return true;
    }

    return tig_mouse_update_state();
}

// 0x4FF3B0
void tig_mouse_set_position(int x, int y, int z)
{
    TigMessage message;

    dword_604668 = false;

    tig_mouse_state.x = x;
    tig_mouse_state.y = y;

    tig_timer_start(&(message.time));
    message.type = TIG_MESSAGE_TYPE_0;
    message.mouse.x = x;
    message.mouse.y = y;
    message.mouse.z = z;
    message.field_14 = 6;
    message.field_18 = 0;
    tig_message_enqueue(&message);
}

// 0x4FF410
void tig_mouse_set_button(int button, bool pressed)
{
    TigMessage message;

    unsigned int flags = tig_mouse_state.flags;
    tig_mouse_state.flags = 0;

    message.type = TIG_MESSAGE_TYPE_0;

    if (pressed) {
        if (((dword_5BE84C[button] | dword_5BE840[button]) & flags) != 0) {
            tig_mouse_state.flags |= dword_5BE84C[button];

            if (tig_timer_between(dword_604680[button], tig_ping_time) > 250) {
                dword_604680[button] = tig_ping_time;

                tig_mouse_state.flags |= dword_5BE840[button];

                message.time = tig_ping_time;
                message.mouse.x = tig_mouse_state.x;
                message.mouse.y = tig_mouse_state.y;
                message.mouse.z = 0;
                message.field_14 = dword_5BE864[button];
                message.field_18 = 1;
                tig_message_enqueue(&message);
            }
        } else {
            dword_604680[button] = tig_ping_time;

            tig_mouse_state.flags |= dword_5BE840[button];

            message.time = tig_ping_time;
            message.mouse.x = tig_mouse_state.x;
            message.mouse.y = tig_mouse_state.y;
            message.mouse.z = 0;
            message.field_14 = dword_5BE864[button];
            message.field_18 = 0;
            tig_message_enqueue(&message);
        }
    } else {
        if (((dword_5BE840[button] | dword_5BE84C[button]) & flags) != 0) {
            dword_604680[button] = tig_ping_time;

            tig_mouse_state.flags |= dword_5BE858[button];

            message.time = tig_ping_time;
            message.mouse.x = tig_mouse_state.x;
            message.mouse.y = tig_mouse_state.y;
            message.mouse.z = 0;
            message.field_14 = dword_5BE870[button];
            message.field_18 = 0;
            tig_message_enqueue(&message);
        }
    }

    if ((flags & TIG_MOUSE_STATE_HIDDEN) != 0) {
        tig_mouse_state.flags |= TIG_MOUSE_STATE_HIDDEN;
    }
}

// 0x4FF5A0
bool tig_mouse_update_state()
{
    TigMessage message;

    message.type = TIG_MESSAGE_TYPE_0;
    message.time = tig_ping_time;

    if (dword_6046B8 > 1) {
        if (tig_timer_between(dword_60465C, tig_ping_time) >= dword_604654) {
            sub_500520();
            dword_60465C = tig_ping_time;
        }
    }

    if (tig_timer_between(dword_6046D0, tig_ping_time) < 0) {
        return false;
    }

    dword_6046D0 = tig_ping_time;

    if (!tig_mouse_device_update_state()) {
        return false;
    }

    if (tig_mouse_device_state.lX != 0 || tig_mouse_device_state.lY != 0) {
        tig_window_set_needs_display_in_rect(&(tig_mouse_state.rect));

        if (tig_mouse_device_state.lX != 0) {
            tig_mouse_state.rect.x += tig_mouse_device_state.lX;

            if (tig_mouse_state.rect.x < -tig_mouse_state.field_14) {
                tig_mouse_state.rect.x = -tig_mouse_state.field_14;
            } else if (tig_mouse_state.rect.x > dword_604650 - tig_mouse_state.field_14) {
                tig_mouse_state.rect.x = dword_604650 - tig_mouse_state.field_14;
            }

            tig_mouse_state.x = tig_mouse_state.rect.x + tig_mouse_state.field_14;
        }

        if (tig_mouse_device_state.lY != 0) {
            tig_mouse_state.rect.y += tig_mouse_device_state.lY;

            if (tig_mouse_state.rect.y < -tig_mouse_state.field_18) {
                tig_mouse_state.rect.y = -tig_mouse_state.field_18;
            } else if (tig_mouse_state.rect.y > dword_60464C - tig_mouse_state.field_18) {
                tig_mouse_state.rect.y = dword_60464C - tig_mouse_state.field_18;
            }

            tig_mouse_state.y = tig_mouse_state.rect.y + tig_mouse_state.field_18;
        }
    }

    // TODO: Original code is slightly different, check.
    if (tig_mouse_device_state.lX != 0 || tig_mouse_device_state.lY != 0) {
        dword_60467C = tig_ping_time;
        dword_604668 = false;

        message.mouse.x = tig_mouse_state.x;
        message.mouse.y = tig_mouse_state.y;
        message.mouse.z = tig_mouse_device_state.lZ;
        message.field_14 = 6;
        tig_message_enqueue(&message);

        tig_window_set_needs_display_in_rect(&(tig_mouse_state.rect));
    } else {
        if (!dword_604668 && tig_timer_between(dword_60467C, tig_ping_time) > 35) {
            dword_604668 = true;

            message.mouse.x = tig_mouse_state.x;
            message.mouse.y = tig_mouse_state.y;
            message.mouse.z = tig_mouse_device_state.lZ;
            message.field_14 = 7;
            tig_message_enqueue(&message);

            tig_window_set_needs_display_in_rect(&(tig_mouse_state.rect));
        }
    }

    if (tig_mouse_z_axis_enabled) {
        if (tig_mouse_device_state.lZ != 0) {
            message.mouse.x = tig_mouse_state.x;
            message.mouse.y = tig_mouse_state.y;
            message.mouse.z = tig_mouse_device_state.lZ;
            message.field_14 = 8;
            tig_message_enqueue(&message);
        }
    } else {
        if (tig_mouse_device_state.lZ != tig_mouse_state.z) {
            message.mouse.x = tig_mouse_state.x;
            message.mouse.y = tig_mouse_state.y;
            message.mouse.z = tig_mouse_device_state.lZ - tig_mouse_state.z;
            message.field_14 = 8;
            tig_message_enqueue(&message);
            tig_mouse_state.z = tig_mouse_device_state.lZ;
        }
    }

    unsigned int flags = tig_mouse_state.flags;
    tig_mouse_state.flags = 0;

    for (int button = 0; button < TIG_MOUSE_BUTTON_COUNT; button++) {
        // TODO: Optimize without `continue`.
        if ((tig_mouse_device_state.rgbButtons[button] & 0x80) == 0) {
            if (((dword_5BE840[button] | dword_5BE84C[button]) & flags) != 0) {
                dword_6046EC[button] = tig_ping_time;

                tig_mouse_state.flags |= dword_5BE858[button];

                message.time = tig_ping_time;
                message.mouse.x = tig_mouse_state.x;
                message.mouse.y = tig_mouse_state.y;
                message.mouse.z = tig_mouse_device_state.lZ;
                message.field_14 = dword_5BE870[button];
                message.field_18 = 0;
                tig_message_enqueue(&message);
            }

            continue;
        } else {
            if (((dword_5BE84C[button] | dword_5BE840[button]) & flags) == 0) {
                dword_6046EC[button] = tig_ping_time;

                tig_mouse_state.flags |= dword_5BE840[button];

                message.time = tig_ping_time;
                message.mouse.x = tig_mouse_state.x;
                message.mouse.y = tig_mouse_state.y;
                message.mouse.z = tig_mouse_device_state.lZ;
                message.field_14 = dword_5BE864[button];
                message.field_18 = 0;
                tig_message_enqueue(&message);

                continue;
            }
        }

        tig_mouse_state.flags |= dword_5BE84C[button];

        if (tig_timer_between(dword_6046EC[button], tig_ping_time) > 250) {
            dword_6046EC[button] = tig_ping_time;

            tig_mouse_state.flags |= dword_5BE840[button];

            message.time = tig_ping_time;
            message.mouse.x = tig_mouse_state.x;
            message.mouse.y = tig_mouse_state.y;
            message.mouse.z = tig_mouse_device_state.lZ;
            message.field_14 = dword_5BE864[button];
            message.field_18 = 1;
            tig_message_enqueue(&message);
        }
    }

    if ((flags & TIG_MOUSE_STATE_HIDDEN) != 0) {
        tig_mouse_state.flags |= TIG_MOUSE_STATE_HIDDEN;
    }

    return true;
}

// 0x4FF9B0
bool tig_mouse_device_update_state()
{
    if (tig_mouse_device != NULL) {
        tig_mouse_set_active(true);
        if (SUCCEEDED(IDirectInputDevice_GetDeviceState(tig_mouse_device, sizeof(tig_mouse_device_state), &tig_mouse_device_state))) {
            return true;
        }
    }

    return false;
}

// 0x4FF9F0
int tig_mouse_get_state(TigMouseState* mouse_state)
{
    if (!tig_mouse_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    tig_mouse_ping();

    memcpy(mouse_state, &tig_mouse_state, sizeof(TigMouseState));
    mouse_state->flags = tig_mouse_state.flags & TIG_MOUSE_STATE_HIDDEN;

    return TIG_OK;
}

// 0x4FFA30
int tig_mouse_hide()
{
    if ((tig_mouse_state.flags & TIG_MOUSE_STATE_HIDDEN) == 0) {
        tig_mouse_state.flags |= TIG_MOUSE_STATE_HIDDEN;
        if (!dword_604658) {
            tig_window_set_needs_display_in_rect(&(tig_mouse_state.rect));
        }
    }

    return TIG_OK;
}

// 0x4FFA70
int tig_mouse_show()
{
    if ((tig_mouse_state.flags & TIG_MOUSE_STATE_HIDDEN) != 0) {
        tig_mouse_state.flags &= ~TIG_MOUSE_STATE_HIDDEN;
        if (!dword_604658) {
            tig_window_set_needs_display_in_rect(&(tig_mouse_state.rect));
        }
    }

    return TIG_OK;
}

// 0x4FFAB0
void tig_mouse_display()
{
    if (dword_604658) {
        return;
    }

    if ((tig_mouse_state.flags & TIG_MOUSE_STATE_HIDDEN) != 0) {
        return;
    }

    sub_51D050(&(tig_mouse_state.rect), 0, dword_604644, 0, 0, -2);

    TigVideoBufferBlitSpec blit_spec;
    blit_spec.field_0 = 0;
    blit_spec.src_video_buffer = dword_60466C;
    blit_spec.src_rect = &stru_6046D8;
    blit_spec.dst_video_buffer = dword_604644;
    blit_spec.dst_rect = &stru_6046D8;
    tig_video_buffer_blit(&blit_spec);

    tig_video_blit(dword_604644, &stru_6046D8, &(tig_mouse_state.rect), 0);
}

// 0x4FFB40
void sub_4FFB40()
{
    if (!dword_604658) {
        tig_video_buffer_fill(dword_60466C, &stru_6046D8, dword_604674);

        TigRect src_rect;
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = dword_604648;
        src_rect.height = dword_604640;

        TigRect dst_rect;
        dst_rect.x = dword_604660;
        dst_rect.y = dword_604664;
        dst_rect.width = dword_604648;
        dst_rect.height = dword_604640;

        TigArtBlitSpec art_blit_spec;
        art_blit_spec.field_0 = 0;
        art_blit_spec.src_art_id = dword_604670;
        art_blit_spec.src_rect = &src_rect;
        art_blit_spec.dst_video_buffer = dword_60466C;
        art_blit_spec.dst_rect = &dst_rect;

        if (tig_art_blit(&art_blit_spec) != TIG_OK) {
            sub_4FFBF0();
        }
    }
}

// 0x4FFBF0
void sub_4FFBF0()
{
    if (tig_video_buffer_lock(dword_60466C) == TIG_OK) {
        TigVideoBufferData video_buffer_data;
        if (tig_video_buffer_data(dword_60466C, &video_buffer_data) == TIG_OK) {
            // TODO: Check.
            switch (video_buffer_data.bpp) {
            case 8:
                *video_buffer_data.surface_data_8bpp = tig_color_rgb_make(255, 255, 255);
                break;
            case 16:
                *video_buffer_data.surface_data_16bpp = tig_color_rgb_make(255, 255, 255);
                break;
            case 24:
            case 32:
                *video_buffer_data.surface_data_32bpp = tig_color_rgb_make(255, 255, 255);
                break;
            }
        }
        tig_video_buffer_unlock(dword_60466C);
    }
}

// 0x4FFD60
bool sub_4FFD60(unsigned int art_id, int a2, int a3)
{
    TigArtData art_data;
    if (tig_art_data(art_id, &art_data) != TIG_OK) {
        return false;
    }

    int width;
    int height;
    if (sub_503510(art_id, &width, &height) != TIG_OK) {
        return false;
    }

    width += a2;
    height += a3;

    TigVideoBufferSpec video_buffer_spec;
    video_buffer_spec.width = width;
    video_buffer_spec.height = height;
    video_buffer_spec.flags = TIG_VIDEO_BUFFER_SPEC_VIDEO_MEMORY;
    video_buffer_spec.background_color = tig_color_rgb_make(0, 0, 0);
    if (tig_video_buffer_create(&video_buffer_spec, &dword_604644) != TIG_OK) {
        return false;
    }

    video_buffer_spec.width = width;
    video_buffer_spec.height = height;
    video_buffer_spec.flags = TIG_VIDEO_BUFFER_SPEC_VIDEO_MEMORY | TIG_VIDEO_BUFFER_SPEC_TRANSPARENCY_ENABLED;
    video_buffer_spec.background_color = art_data.field_14;
    video_buffer_spec.color_key = art_data.field_14;
    if (tig_video_buffer_create(&video_buffer_spec, &dword_60466C) != TIG_OK) {
        return false;
    }

    stru_6046C0.x = 0;
    stru_6046C0.y = 0;
    stru_6046C0.width = width;
    stru_6046C0.height = height;

    return true;
}

// 0x4FFEA0
bool sub_4FFEA0()
{
    tig_video_buffer_destroy(dword_604644);
    tig_video_buffer_destroy(dword_60466C);
    return true;
}

// 0x4FFEC0
bool sub_4FFEC0(unsigned int art_id, int a2, int a3)
{
    TigArtFrameData art_frame_data;
    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    TigArtData art_data;
    if (tig_art_data(art_id, &art_data) != TIG_OK) {
        return false;
    }

    dword_6046B8 = art_data.field_10;
    tig_mouse_state.field_18 = a3 + art_frame_data.field_C;
    dword_604670 = art_id;
    dword_604664 = a3;
    tig_mouse_state.rect.height = a3 + art_frame_data.height;
    stru_6046D8.height = a3 + art_frame_data.height;
    dword_604648 = art_frame_data.width;
    dword_604654 = 1000 / art_data.field_4;
    dword_604660 = a2;
    tig_mouse_state.field_14 = a2 + art_frame_data.field_8;
    tig_mouse_state.rect.x = tig_mouse_state.x - tig_mouse_state.field_14;
    dword_604640 = art_frame_data.height;
    tig_mouse_state.rect.width = art_frame_data.width + a2;
    stru_6046D8.width = art_frame_data.width + a2;
    tig_mouse_state.rect.y = tig_mouse_state.y - tig_mouse_state.field_18;
    stru_6046D8.x = 0;
    stru_6046D8.y = 0;

    dword_604674 = art_data.field_14;
    tig_video_buffer_set_color_key(dword_60466C, art_data.field_14);

    sub_4FFB40();

    return true;
}

// 0x4FFFE0
int sub_4FFFE0(unsigned int art_id)
{
    if (!dword_604658) {
        int width;
        int height;
        int rc = sub_503510(art_id, &width, &height);
        if (rc != TIG_OK) {
            return rc;
        }

        bool hidden = (tig_mouse_state.flags & TIG_MOUSE_STATE_HIDDEN) != 0;
        if (!hidden) {
            tig_mouse_hide();
        }

        if (width > stru_6046C0.width || height > stru_6046C0.height) {
            TigVideoBuffer* v1 = dword_604644;
            TigVideoBuffer* v2 = dword_60466C;

            if (!sub_4FFD60(art_id, 0, 0)) {
                dword_604644 = v1;
                dword_60466C = v2;

                if (!hidden) {
                    tig_mouse_show();
                }

                return TIG_ERR_16;
            }

            if (!sub_4FFEC0(art_id, 0, 0)) {
                sub_4FFEA0();

                dword_604644 = v1;
                dword_60466C = v2;

                if (!hidden) {
                    tig_mouse_show();
                }

                return TIG_ERR_16;
            }

            tig_video_buffer_destroy(v1);
            tig_video_buffer_destroy(v2);
        } else {
            if (!sub_4FFEC0(art_id, 0, 0)) {
                if (!hidden) {
                    tig_mouse_show();
                }

                return TIG_ERR_16;
            }
        }

        if (!hidden) {
            tig_mouse_show();
        }
    }

    return TIG_OK;
}

// 0x5000F0
void sub_5000F0(int a1, int a2)
{
    if (tig_mouse_state.field_14 != a1 || tig_mouse_state.field_18 != a2) {
        bool hidden = (tig_mouse_state.flags & TIG_MOUSE_STATE_HIDDEN) != 0;
        if (!hidden) {
            tig_mouse_hide();
        }

        tig_mouse_state.field_14 = a1;
        tig_mouse_state.field_18 = a2;
        tig_mouse_state.rect.x = tig_mouse_state.x - a1;
        tig_mouse_state.rect.y = tig_mouse_state.y - a2;

        if (!hidden) {
            tig_mouse_show();
        }
    }
}

// 0x500150
int sub_500150()
{
    return dword_604670;
}

// 0x500160
void sub_500160(unsigned int art_id, int a2, int a3)
{
    // TODO: Incomplete.
}

// 0x500520
void sub_500520()
{
    if (!dword_604658) {
        tig_window_set_needs_display_in_rect(&(tig_mouse_state.rect));
        dword_604670 = sub_502BC0(dword_604670);
        sub_4FFEC0(dword_604670, 0, 0);
        tig_window_set_needs_display_in_rect(&(tig_mouse_state.rect));
    }
}

// 0x500560
int sub_500560()
{
    return 0;
}

// 0x500570
void sub_500570()
{
}

// 0x500580
void tig_mouse_set_z_axis_enabled(bool enabled)
{
    tig_mouse_z_axis_enabled = enabled;
}
