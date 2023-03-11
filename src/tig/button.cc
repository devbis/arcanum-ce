#include "tig/button.h"

#include "tig/art.h"
#include "tig/debug.h"
#include "tig/sound.h"
#include "tig/window.h"

#define TIG_BUTTON_MAX 400

#define TIG_BUTTON_USAGE_FLAG_0x01 0x01
#define TIG_BUTTON_USAGE_FLAG_0x04 0x04

typedef struct TigButton {
    int field_0;
    int flags;
    int window_handle;
    TigRect rect;
    int art_id;
    int state;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
};

static_assert(sizeof(TigButton) == 0x38, "wrong size");

static int tig_button_free_index();
static int tig_button_handle_to_index(int button_handle);
static int tig_button_index_to_handle(int index);
static void tig_button_play_sound(int button_handle, int event);
static int sub_538760(int a1);
static void sub_5387B0(int a1);
static void sub_5387D0();

// 0x5C26F8
static int dword_5C26F8 = -1;

// 0x5C26FC
static int dword_5C26FC = -1;

// 0x630D60
static TigButton buttons[TIG_BUTTON_MAX];

// 0x537B00
int tig_button_init(TigContext* ctx)
{
    for (int index = 0; index < TIG_BUTTON_MAX; index++) {
        buttons[index].field_0 = TIG_BUTTON_USAGE_FLAG_0x01;
    }

    return TIG_OK;
}

// 0x537B20
void tig_button_exit()
{
    for (int index = 0; index < TIG_BUTTON_MAX; index++) {
        if ((buttons[index].field_0 & TIG_BUTTON_FLAG_0x01) == 0) {
            int handle = tig_button_index_to_handle(index);
            tig_button_destroy(handle);
        }
    }
}

// 0x537B50
int tig_button_create(TigButtonData* button_data, int* button_handle)
{
    int rc;

    int button_index = tig_button_free_index();
    if (button_index == -1) {
        return 3;
    }

    buttons[button_index].window_handle = button_data->window_handle;
    buttons[button_index].field_34 = -1;

    TigWindowData window_data;
    rc = tig_window_data(button_data->window_handle, &window_data);
    if (rc != TIG_OK) {
        return rc;
    }

    buttons[button_index].flags = button_data->flags;

    if ((window_data.flags & TIG_WINDOW_FLAG_HIDDEN) != 0) {
        buttons[button_index].flags |= TIG_BUTTON_FLAG_HIDDEN;
        buttons[button_index].field_0 |= TIG_BUTTON_USAGE_FLAG_0x04;
    }

    if ((buttons[button_index].flags & TIG_BUTTON_FLAG_0x04) != 0) {
        buttons[button_index].flags |= TIG_BUTTON_FLAG_0x02;
    }

    buttons[button_index].rect.x = window_data.x + button_data->x;
    buttons[button_index].rect.y = window_data.y + button_data->y;

    if (button_data->art_id != -1) {
        TigArtFrameData art_frame_data;
        rc = tig_art_frame_data(button_data->art_id, &art_frame_data);
        if (rc != TIG_OK) {
            return rc;
        }

        buttons[button_index].rect.width = art_frame_data.width;
        buttons[button_index].rect.height = art_frame_data.height;
    } else {
        buttons[button_index].rect.width = button_data->width;
        buttons[button_index].rect.height = button_data->height;
    }

    buttons[button_index].art_id = button_data->art_id;

    if ((button_data->flags & TIG_BUTTON_FLAG_0x10) != 0) {
        buttons[button_index].state = 0;
    } else {
        buttons[button_index].state = 3;
    }

    buttons[button_index].field_24 = button_data->field_1C;
    buttons[button_index].field_28 = button_data->field_20;
    buttons[button_index].field_2C = button_data->field_24;
    buttons[button_index].field_30 = button_data->field_28;

    *button_handle = tig_button_index_to_handle(button_index);

    rc = tig_window_button_add(button_data->window_handle, *button_handle);
    if (rc != TIG_OK) {
        return rc;
    }

    if ((button_data->flags & TIG_BUTTON_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&(buttons[button_index].rect));
        tig_button_refresh_rect(buttons[button_index].window_handle, &(buttons[button_index].rect));
    }

    buttons[button_index].field_0 &= ~TIG_BUTTON_USAGE_FLAG_0x01;

    return TIG_OK;
}

// 0x537CE0
int tig_button_destroy(int button_handle)
{
    if (button_handle == -1) {
        tig_debug_printf("tig_button_destroy: ERROR: Attempt to reference Empty ButtonID!\n");
        return 12;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    int rc = tig_window_button_remove(buttons[button_index].window_handle, button_handle);
    if (rc != TIG_OK) {
        return rc;
    }

    if (dword_5C26F8 == button_handle || dword_5C26FC == button_handle) {
        sub_5387D0();
    }

    tig_window_set_needs_display_in_rect(&(buttons[button_index].rect));
    tig_button_refresh_rect(buttons[button_index].window_handle, &(buttons[button_index].rect));
    buttons[button_index].field_0 = 1;

    return TIG_OK;
}

// 0x537D70
int tig_button_data(int button_handle, TigButtonData* button_data)
{
    if (button_handle == -1) {
        tig_debug_printf("tig_button_data: ERROR: Attempt to reference Empty ButtonID!\n");
        return 12;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    button_data->flags = buttons[button_index].flags;
    button_data->window_handle = buttons[button_index].window_handle;

    TigWindowData window_data;
    int rc = tig_window_data(buttons[button_index].window_handle, &window_data);
    if (rc != TIG_OK) {
        return rc;
    }

    button_data->x = buttons[button_index].rect.x - window_data.x;
    button_data->y = buttons[button_index].rect.y - window_data.y;
    button_data->art_id = buttons[button_index].art_id;
    button_data->width = buttons[button_index].rect.width;
    button_data->height = buttons[button_index].rect.height;
    button_data->field_1C = buttons[button_index].field_24;
    button_data->field_20 = buttons[button_index].field_28;
    button_data->field_24 = buttons[button_index].field_2C;
    button_data->field_28 = buttons[button_index].field_30;

    return TIG_OK;
}

// 0x537E20
int tig_button_refresh_rect(int window_handle, TigRect* rect)
{
    // 0x6364E0
    static bool busy;

    if (busy) {
        return TIG_OK;
    }

    int* window_buttons;
    int window_buttons_count = tig_window_button_list(window_handle, &window_buttons);

    TigWindowData window_data;
    int rc = tig_window_data(window_handle, &window_data);
    if (rc != TIG_OK) {
        return rc;
    }

    if (window_buttons_count != 0) {
        busy = true;

        for (int index = 0; index < window_buttons_count; index++) {
            int button_index = tig_button_handle_to_index(window_buttons[index]);

            TigRect src_rect;
            if (tig_rect_intersection(&(buttons[button_index].rect), rect, &src_rect) == TIG_OK) {
                if ((buttons[button_index].flags & TIG_BUTTON_FLAG_HIDDEN) == 0) {
                    TigRect dest_rect;
                    dest_rect.width = src_rect.width;
                    dest_rect.height = src_rect.height;
                    dest_rect.x = src_rect.x - window_data.x;
                    dest_rect.y = src_rect.y - window_data.y;

                    src_rect.x -= buttons[button_index].rect.x;
                    src_rect.y -= buttons[button_index].rect.y;

                    int art_id = buttons[button_index].art_id;
                    if (art_id != -1) {
                        if (buttons[button_index].state != 3) {
                            art_id = sub_502BC0(art_id);

                            if (buttons[button_index].state == 2) {
                                art_id = sub_502BC0(art_id);
                            }
                        }

                        TigArtBlitSpec blit_spec;
                        blit_spec.src_art_id = art_id;
                        blit_spec.src_rect = &src_rect;
                        blit_spec.field_0 = 0;
                        blit_spec.dst_rect = &dest_rect;
                        tig_window_blit_art(window_handle, &blit_spec);
                    }
                }
            }
        }

        busy = false;
    }

    return TIG_OK;
}

// 0x537FA0
int tig_button_free_index()
{
    for (int index = 0; index < TIG_BUTTON_MAX; index++) {
        if ((buttons[index].field_0 & TIG_BUTTON_USAGE_FLAG_0x01) != 0) {
            return index;
        }
    }

    return -1;
}

// 0x537FC0
int tig_button_handle_to_index(int button_handle)
{
    return button_handle;
}

// 0x537FD0
int tig_button_index_to_handle(int index)
{
    return index;
}

// 0x537FE0
void tig_button_state_change(int button_handle, int state)
{
    if (button_handle == -1) {
        tig_debug_printf("tig_button_state_change: ERROR: Attempt to reference Empty ButtonID!\n");
        return;
    }

    int button_index = tig_button_handle_to_index(button_handle);

    // TODO: Incomplete
}

// 0x5380A0
int tig_button_state_get(int button_handle, int* state)
{
    if (button_handle != -1) {
        tig_debug_printf("tig_button_state_get: ERROR: Attempt to reference Empty ButtonID!\n");
        return TIG_ERR_12;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    if (state == NULL) {
        return TIG_ERR_12;
    }

    if ((buttons[button_index].field_0 & 1) != 0) {
        return TIG_ERR_12;
    }

    *state = buttons[button_index].state;

    return TIG_OK;
}

// 0x5380F0
int sub_5380F0(int a1, int a2)
{
    // TODO: Incomplete.
}

// 0x538220
int sub_538220(void* a1)
{
    // TODO: Incomplete.
}

// 0x5385C0
void tig_button_play_sound(int button_handle, int event)
{
    if (button_handle == -1) {
        // FIXME: Wrong function name.
        tig_debug_printf("tig_button_state_change: ERROR: Attempt to reference Empty ButtonID!\n");
    }

    int index = tig_button_handle_to_index(button_handle);
    switch (event) {
    case 0:
        tig_sound_quick_play(buttons[index].field_24);
        break;
    case 1:
        tig_sound_quick_play(buttons[index].field_28);
        break;
    case 2:
        tig_sound_quick_play(buttons[index].field_2C);
        break;
    case 3:
        tig_sound_quick_play(buttons[index].field_30);
        break;
    }
}

// 0x538670
int tig_button_radio_group(int count, int* button_handles, int a3)
{
    // TODO: Incomplete.
}

// 0x538730
int sub_538730(int button_handle)
{
    int button_index = tig_button_handle_to_index(button_handle);
    return sub_538760(buttons[button_index].field_34);
}

// 0x538760
int sub_538760(int a1)
{
    // TODO: Incomplete.
}

// 0x5387B0
void sub_5387B0(int a1)
{
    int button_handle = sub_538760(a1);
    if (button_handle != -1) {
        tig_button_state_change(button_handle, 1);
    }
}

// 0x5387D0
void sub_5387D0()
{
    dword_5C26F8 = -1;
    dword_5C26FC = -1;
}

// 0x5387E0
int sub_5387E0(int button_handle)
{
    if (button_handle == -1) {
        return TIG_OK;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    if ((buttons[button_index].flags & TIG_BUTTON_FLAG_HIDDEN) == 0) {
        return 12;
    }

    buttons[button_index].flags &= ~TIG_BUTTON_FLAG_HIDDEN;

    tig_button_refresh_rect(buttons[button_index].window_handle, &(buttons[button_index].rect));
    tig_window_set_needs_display_in_rect(&(buttons[button_index].rect));

    return TIG_OK;
}

// 0x538840
int sub_538840(int button_handle)
{
    if (button_handle == -1) {
        return TIG_OK;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    if ((buttons[button_index].flags & TIG_BUTTON_FLAG_HIDDEN) != 0) {
        if ((buttons[button_index].field_0 & TIG_BUTTON_USAGE_FLAG_0x04) != 0) {
            buttons[button_index].field_0 &= ~TIG_BUTTON_USAGE_FLAG_0x04;
        }
        return 12;
    }

    buttons[button_index].flags |= TIG_BUTTON_FLAG_HIDDEN;

    tig_button_refresh_rect(buttons[button_index].window_handle, &(buttons[button_index].rect));
    tig_window_set_needs_display_in_rect(&(buttons[button_index].rect));

    return TIG_OK;
}

// 0x5388B0
int tig_button_is_hidden(int button_handle, bool* hidden)
{
    if (button_handle == -1) {
        return TIG_ERR_12;
    }

    if (hidden == NULL) {
        return TIG_ERR_12;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    *hidden = (buttons[button_index].flags & TIG_BUTTON_FLAG_HIDDEN) != 0;

    return TIG_OK;
}

// 0x538900
int tig_button_show(int button_handle)
{
    if (button_handle == -1) {
        return TIG_OK;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    if ((buttons[button_index].flags & TIG_BUTTON_FLAG_HIDDEN) == 0) {
        return TIG_ERR_12;
    }

    if ((buttons[button_index].field_0 & TIG_BUTTON_USAGE_FLAG_0x04) == 0) {
        return TIG_ERR_12;
    }

    buttons[button_index].flags &= ~TIG_BUTTON_FLAG_HIDDEN;
    buttons[button_index].field_0 &= ~TIG_BUTTON_USAGE_FLAG_0x04;

    tig_button_refresh_rect(buttons[button_index].window_handle, &(buttons[button_index].rect));
    tig_window_set_needs_display_in_rect(&(buttons[button_index].rect));

    return TIG_OK;
}

// 0x538980
int tig_button_hide(int button_handle)
{
    if (button_handle == -1) {
        return TIG_OK;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    if ((buttons[button_index].flags & TIG_BUTTON_FLAG_HIDDEN) != 0) {
        return 12;
    }

    buttons[button_index].flags |= TIG_BUTTON_FLAG_HIDDEN;
    buttons[button_index].field_0 |= TIG_BUTTON_USAGE_FLAG_0x04;

    tig_button_refresh_rect(buttons[button_index].window_handle, &(buttons[button_index].rect));
    tig_window_set_needs_display_in_rect(&(buttons[button_index].rect));

    return TIG_OK;
}

// 0x5389F0
void tig_button_set_art(int button_handle, int art_id)
{
    if (button_handle == -1) {
        return;
    }

    int button_index = tig_button_handle_to_index(button_handle);
    if (art_id != -1) {
        TigArtFrameData art_frame_data;
        tig_art_frame_data(art_id, &art_frame_data);

        buttons[button_index].rect.width = art_frame_data.width;
        buttons[button_index].rect.height = art_frame_data.height;
    }

    buttons[button_index].art_id = art_id;

    if ((buttons[button_index].flags & TIG_BUTTON_FLAG_HIDDEN) == 0) {
        tig_window_set_needs_display_in_rect(&(buttons[button_index].rect));
        tig_button_refresh_rect(buttons[button_index].window_handle, &(buttons[button_index].rect));
    }
}
