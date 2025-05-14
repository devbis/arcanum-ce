#include "ui/scrollbar_ui.h"

#include "game/random.h"

#define MAX_CONTROLS 8

typedef enum ScrollbarUiControlFlags {
    SB_IN_USE = 0x1,
    SB_HIDDEN = 0x2,
} ScrollbarUiControlFlags;

typedef struct ScrollbarUiControl {
    /* 0000 */ ScrollbarId id;
    /* 0008 */ ScrollbarUiControlFlags flags;
    /* 000C */ ScrollbarUiControlInfo info;
    /* 0050 */ tig_window_handle_t window_handle;
    /* 0054 */ TigWindowData window_data;
    /* 0074 */ tig_button_handle_t button_up;
    /* 0078 */ tig_button_handle_t button_down;
    /* 007C */ int field_7C;
} ScrollbarUiControl;

static_assert(sizeof(ScrollbarUiControl) == 0x80, "wrong size");

static void sub_5807F0(int index, int a2);
static void scrollbar_ui_control_reset(ScrollbarUiControl* ctrl);
static void sub_5811F0(ScrollbarUiControl* ctrl, ScrollbarUiControlInfo* info);
static bool sub_581280(ScrollbarId* id);
static bool sub_5812E0(const ScrollbarId* id, ScrollbarUiControl** ctrl_ptr);
static bool sub_581360(int id, int x, int y);
static bool sub_5813E0(int id, int x, int y);
static bool sub_581460(int id, int x, int y);
static bool sub_5814E0(int id, int x, int y);
static float sub_581550(int id);
static int sub_5815A0(int id);
static int sub_5815D0(int id);
static int sub_581660(int id);
static bool sub_5816A0(int id, int a2);
static void sub_5816D0();
static void sub_5816E0();
static int sub_5816F0();

// 0x5CBF48
static TigRect stru_5CBF48 = { 0, 0, 11, 5 };

// 0x5CBF58
static TigRect stru_5CBF58 = { 0, 0, 11, 7 };

// 0x5CBF68
static TigRect stru_5CBF68 = { 0, 0, 11, 1 };

// 0x5CBF78
static int dword_5CBF78 = -1;

// 0x684250
static int scrollbar_ui_button_down_width;

// 0x684254
static tig_art_id_t scrollbar_ui_middle_art_id;

// 0x684258
static int scrollbar_ui_button_up_height;

// 0x68425C
static bool initialized;

// 0x684260
static tig_art_id_t scrollbar_ui_top_art_id;

// 0x684268
static ScrollbarUiControl scrollbar_ui_controls[MAX_CONTROLS];

// 0x684668
static tig_art_id_t scrollbar_ui_bottom_art_id;

// 0x68466C
static int scrollbar_ui_button_up_width;

// 0x684670
static int scrollbar_ui_button_down_height;

// 0x684674
static int dword_684674;

// 0x684678
static int dword_684678;

// 0x68467C
static int dword_68467C;

// 0x684680
static int dword_684680;

// 0x684684
static int dword_684684;

// 0x580410
bool scrollbar_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    scrollbar_ui_reset();

    if (tig_art_interface_id_create(238, 0, 0, 0, &scrollbar_ui_top_art_id) != TIG_OK) {
        return false;
    }

    if (tig_art_interface_id_create(240, 0, 0, 0, &scrollbar_ui_bottom_art_id) != TIG_OK) {
        return false;
    }

    if (tig_art_interface_id_create(787, 0, 0, 0, &scrollbar_ui_middle_art_id) != TIG_OK) {
        return false;
    }

    initialized = true;

    return true;
}

// 0x580480
void scrollbar_ui_exit()
{
    initialized = false;
}

// 0x580490
void scrollbar_ui_reset()
{
    int index;

    for (index = 0; index < MAX_CONTROLS; index++) {
        scrollbar_ui_control_reset(&(scrollbar_ui_controls[index]));
    }

    dword_684678 = random_between(0, 8192);
    dword_5CBF78 = -1;
    dword_684684 = 0;
}

// 0x5804E0
bool scrollbar_ui_control_create(ScrollbarId* id, ScrollbarUiControlInfo* info, tig_window_handle_t window_handle)
{
    ScrollbarUiControl* ctrl;
    TigArtFrameData art_frame_data;
    TigButtonData button_data;

    if ((info->flags & 1) == 0
        || !sub_581280(id)
        || !sub_5812E0(id, &ctrl)) {
        return false;
    }

    ctrl->id = *id;
    sub_5811F0(ctrl, info);

    if (ctrl->info.field_38 < ctrl->info.field_28
        || ctrl->info.field_38 > ctrl->info.field_24) {
        ctrl->info.field_38 = ctrl->info.field_28;
    }

    ctrl->window_handle = window_handle;
    tig_window_data(ctrl->window_handle, &(ctrl->window_data));

    if (tig_art_interface_id_create(317, 0, 0, 0, &(button_data.art_id)) != TIG_OK) {
        return false;
    }

    tig_art_frame_data(button_data.art_id, &art_frame_data);
    scrollbar_ui_button_up_height = art_frame_data.height;
    scrollbar_ui_button_up_width = art_frame_data.width;

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = ctrl->window_handle;
    button_data.x = ctrl->info.scrollbar_rect.x + (ctrl->info.scrollbar_rect.width - art_frame_data.width) / 2;
    button_data.y = ctrl->info.scrollbar_rect.y;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;
    button_data.mouse_down_snd_id = 3014;
    button_data.mouse_up_snd_id = 3015;
    tig_button_create(&button_data, &(ctrl->button_up));

    if (tig_art_interface_id_create(318, 0, 0, 0, &(button_data.art_id)) != TIG_OK) {
        return false;
    }

    tig_art_frame_data(button_data.art_id, &art_frame_data);
    scrollbar_ui_button_down_height = art_frame_data.height;
    scrollbar_ui_button_down_width = art_frame_data.width;

    button_data.y = ctrl->info.scrollbar_rect.y + ctrl->info.scrollbar_rect.height - art_frame_data.height;
    tig_button_create(&button_data, &(ctrl->button_down));

    if (ctrl->info.field_3C != NULL) {
        ctrl->info.field_3C(ctrl->info.field_38);
    }

    return true;
}

// 0x580690
void scrollbar_ui_control_destroy(ScrollbarId id)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return;
    }

    tig_button_destroy(ctrl->button_up);
    tig_button_destroy(ctrl->button_down);

    if (dword_5CBF78 == ctrl->id.index) {
        dword_5CBF78 = -1;
    }

    scrollbar_ui_control_reset(ctrl);
}

// 0x5806F0
void sub_5806F0(ScrollbarId id)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return;
    }

    sub_5807F0(ctrl->id.index, 0);
}

// 0x580720
bool scrollbar_ui_control_show(ScrollbarId id)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return false;
    }

    if ((ctrl->flags & SB_HIDDEN) == 0) {
        return false;
    }

    ctrl->flags &= ~SB_HIDDEN;

    tig_button_show(ctrl->button_down);
    tig_button_show(ctrl->button_up);
    sub_5807F0(ctrl->id.index, 0);

    return true;
}

// 0x580780
bool scrollbar_ui_control_hide(ScrollbarId id)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return false;
    }

    if ((ctrl->flags & SB_HIDDEN) != 0) {
        return false;
    }

    ctrl->flags |= SB_HIDDEN;

    tig_button_hide(ctrl->button_down);
    tig_button_hide(ctrl->button_up);

    if (ctrl->info.field_40 != NULL) {
        ctrl->info.field_40(&(ctrl->info.scrollbar_rect));
    }

    return true;
}

// 0x5807F0
void sub_5807F0(int index, int a2)
{
    ScrollbarUiControl* ctrl;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    float v1;
    bool hidden;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = 0;
    src_rect.height = 0;

    if (sub_5816F0() != 0) {
        return;
    }

    ctrl = &(scrollbar_ui_controls[index]);
    if ((ctrl->flags & SB_HIDDEN) != 0) {
        return;
    }

    if (ctrl->info.field_40 != NULL) {
        ctrl->info.field_40(&(ctrl->info.scrollbar_rect));
    }

    v1 = sub_581550(ctrl->id.index);

    dst_rect.x = ctrl->info.scrollbar_rect.x;
    if (ctrl->info.field_38 != ctrl->info.field_24 || a2 != 0) {
        dst_rect.y = sub_5815D0(ctrl->id.index) + a2;
    } else {
        dst_rect.y = ctrl->info.scrollbar_rect.y + ctrl->info.scrollbar_rect.height - scrollbar_ui_button_down_height - sub_5815A0(index);
    }

    if (dst_rect.y < ctrl->info.scrollbar_rect.y + scrollbar_ui_button_up_height) {
        dst_rect.y = ctrl->info.scrollbar_rect.y + scrollbar_ui_button_up_height;
    } else if (dst_rect.y > ctrl->info.scrollbar_rect.y + ctrl->info.scrollbar_rect.height - scrollbar_ui_button_down_height - sub_5815A0(index)) {
        dst_rect.y = ctrl->info.scrollbar_rect.y + ctrl->info.scrollbar_rect.height - scrollbar_ui_button_down_height - sub_5815A0(index);
    }

    dst_rect.width = ctrl->info.scrollbar_rect.width;
    dst_rect.height = 5;

    art_blit_info.flags = 0;
    art_blit_info.art_id = scrollbar_ui_top_art_id;
    art_blit_info.src_rect = &stru_5CBF48;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(ctrl->window_handle, &art_blit_info);

    v1 -= 5.0f;

    dst_rect.y += 5;
    dst_rect.width = ctrl->info.scrollbar_rect.width;
    dst_rect.height = 1;

    art_blit_info.flags = 0;
    art_blit_info.art_id = scrollbar_ui_middle_art_id;
    art_blit_info.src_rect = &stru_5CBF68;
    art_blit_info.dst_rect = &dst_rect;

    while (v1 > 7.0f) {
        tig_window_blit_art(ctrl->window_handle, &art_blit_info);
        dst_rect.y++;
        v1 -= 7.0f;
    }

    dst_rect.width = ctrl->info.scrollbar_rect.width;
    dst_rect.height = 7;

    art_blit_info.art_id = scrollbar_ui_bottom_art_id;
    art_blit_info.flags = 0;
    art_blit_info.src_rect = &stru_5CBF58;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(ctrl->window_handle, &art_blit_info);

    tig_button_is_hidden(ctrl->button_up, &hidden);
    if (hidden) {
        dst_rect.x = ctrl->info.scrollbar_rect.x + (ctrl->info.scrollbar_rect.width - scrollbar_ui_button_up_width) / 2;
        dst_rect.y = ctrl->info.scrollbar_rect.y;
        dst_rect.width = scrollbar_ui_button_up_width;
        dst_rect.height = scrollbar_ui_button_up_height;

        src_rect.width = scrollbar_ui_button_up_width;
        src_rect.height = scrollbar_ui_button_up_height;

        tig_art_interface_id_create(317, 0, 0, 0, &art_blit_info.art_id);
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(ctrl->window_handle, &art_blit_info);
    }

    tig_button_is_hidden(ctrl->button_down, &hidden);
    if (hidden) {
        dst_rect.x = ctrl->info.scrollbar_rect.x + (ctrl->info.scrollbar_rect.width - scrollbar_ui_button_up_width) / 2;
        dst_rect.y = ctrl->info.scrollbar_rect.y + ctrl->info.scrollbar_rect.height - scrollbar_ui_button_down_height;
        dst_rect.width = scrollbar_ui_button_down_width;
        dst_rect.height = scrollbar_ui_button_down_height;

        src_rect.width = scrollbar_ui_button_down_width;
        src_rect.height = scrollbar_ui_button_down_height;

        tig_art_interface_id_create(318, 0, 0, 0, &(art_blit_info.art_id));
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(ctrl->window_handle, &art_blit_info);
    }
}

// 0x580B10
bool scrollbar_ui_process_event(TigMessage* msg)
{
    int index;
    ScrollbarUiControl* ctrl;

    if (dword_684684 != 0) {
        return false;
    }

    switch (msg->type) {
    case TIG_MESSAGE_MOUSE:
        switch (msg->data.mouse.event) {
        case TIG_MESSAGE_MOUSE_LEFT_BUTTON_DOWN:
            if (dword_5CBF78 == -1) {
                for (index = 0; index < 8; index++) {
                    ctrl = &(scrollbar_ui_controls[index]);
                    if ((ctrl->flags & SB_IN_USE) != 0
                        && (ctrl->flags & SB_HIDDEN) == 0) {
                        if (sub_581360(index, msg->data.mouse.x, msg->data.mouse.y)) {
                            dword_5CBF78 = index;
                            dword_68467C = msg->data.mouse.y
                                - scrollbar_ui_controls[index].window_data.rect.y
                                - sub_5815D0(index);
                            tig_button_hide(scrollbar_ui_controls[index].button_up);
                            tig_button_hide(scrollbar_ui_controls[index].button_down);
                            return true;
                        }

                        if (sub_5813E0(index, msg->data.mouse.x, msg->data.mouse.y)) {
                            ctrl->info.field_38 -= ctrl->info.field_30;
                            if (ctrl->info.field_38 < ctrl->info.field_28) {
                                ctrl->info.field_38 = ctrl->info.field_28;
                            }
                            if (ctrl->info.field_3C != NULL) {
                                ctrl->info.field_3C(ctrl->info.field_38);
                            }
                            sub_5806F0(ctrl->id);
                        } else if (sub_581460(index, msg->data.mouse.x, msg->data.mouse.y)) {
                            ctrl->info.field_38 += ctrl->info.field_30;
                            if (ctrl->info.field_38 > ctrl->info.field_24) {
                                ctrl->info.field_38 = ctrl->info.field_24;
                            }
                            if (ctrl->info.field_3C != NULL) {
                                ctrl->info.field_3C(ctrl->info.field_38);
                            }
                            sub_5806F0(ctrl->id);
                        }
                    }
                }
            }
            return false;
        case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
            if (dword_5CBF78 != -1) {
                if (sub_5816A0(dword_5CBF78, scrollbar_ui_controls[dword_5CBF78].info.field_38) + dword_684680) {
                    scrollbar_ui_controls[dword_5CBF78].info.field_38 += dword_684680;
                }
                dword_684680 = 0;
                sub_5807F0(dword_5CBF78, 0);
                tig_button_show(scrollbar_ui_controls[dword_5CBF78].button_up);
                tig_button_show(scrollbar_ui_controls[dword_5CBF78].button_down);
                dword_5CBF78 = -1;
                return true;
            }
            return false;
        case TIG_MESSAGE_MOUSE_MOVE:
            if (dword_5CBF78 != -1) {
                int v2;
                float v3;
                float v4;

                ctrl = &(scrollbar_ui_controls[dword_5CBF78]);
                v2 = msg->data.mouse.y
                    - scrollbar_ui_controls[dword_5CBF78].window_data.rect.y
                    - dword_68467C
                    - sub_5815D0(dword_5CBF78);
                v3 = sub_581550(dword_5CBF78);
                v4 = v2 - dword_684680 * v3;
                if (v4 > v3 && ctrl->info.field_38 + dword_684680 < ctrl->info.field_24) {
                    do {
                        v4 -= v3;
                        dword_684680 += ctrl->info.field_2C;
                    } while (v4 > v3 && ctrl->info.field_38 + dword_684680 < ctrl->info.field_24);
                } else if (v4 < -v3 && ctrl->info.field_38 + dword_684680 > ctrl->info.field_28) {
                    do {
                        v4 += -v3;
                        dword_684680 -= ctrl->info.field_2C;
                    } while (v4 < -v3 && ctrl->info.field_38 + dword_684680 > ctrl->info.field_28);
                } else {
                    sub_5807F0(dword_5CBF78, v2);
                    return true;
                }

                if (ctrl->info.field_3C != NULL) {
                    if (sub_5816A0(dword_5CBF78, ctrl->info.field_38 + dword_684680)) {
                        sub_5816D0();
                        ctrl->info.field_3C(ctrl->info.field_38 + dword_684680);
                        sub_5816E0();
                    } else {
                        if (ctrl->info.field_38 + dword_684680 > ctrl->info.field_24) {
                            ctrl->info.field_3C(ctrl->info.field_24);
                            sub_5807F0(dword_5CBF78, ctrl->info.scrollbar_rect.height - ((int)v3 * (ctrl->info.field_38 + 1)));
                            return true;
                        }

                        if (ctrl->info.field_38 + dword_684680 < ctrl->info.field_28) {
                            ctrl->info.field_3C(ctrl->info.field_28);
                            sub_5807F0(dword_5CBF78, -(ctrl->info.field_38 * (int)v3));
                            return true;
                        }
                    }
                }

                sub_5807F0(dword_5CBF78, v2);
                return true;
            }
            return false;
        case TIG_MESSAGE_MOUSE_WHEEL:
            for (index = 0; index < 8; index++) {
                ctrl = &(scrollbar_ui_controls[index]);
                if ((ctrl->flags & SB_IN_USE) != 0
                    && (ctrl->flags & SB_HIDDEN) == 0
                    && sub_5814E0(index, msg->data.mouse.x, msg->data.mouse.y)) {
                    if (msg->data.mouse.z > 0) {
                        ctrl->info.field_38 -= ctrl->info.field_34;
                        if (ctrl->info.field_38 < ctrl->info.field_28) {
                            ctrl->info.field_38 = ctrl->info.field_28;
                        }
                        if (ctrl->info.field_3C != NULL) {
                            ctrl->info.field_3C(ctrl->info.field_38);
                        }
                        sub_5806F0(ctrl->id);
                        return true;
                    } else if (msg->data.mouse.z < 0) {
                        ctrl->info.field_38 += ctrl->info.field_34;
                        if (ctrl->info.field_38 > ctrl->info.field_24) {
                            ctrl->info.field_38 = ctrl->info.field_24;
                        }
                        if (ctrl->info.field_3C != NULL) {
                            ctrl->info.field_3C(ctrl->info.field_38);
                        }
                        sub_5806F0(ctrl->id);
                        return true;
                    }
                    return false;
                }
            }
            return false;
        default:
            return false;
        }
    case TIG_MESSAGE_BUTTON:
        switch (msg->data.button.state) {
        case TIG_BUTTON_STATE_PRESSED:
            for (index = 0; index < 8; index++) {
                ctrl = &(scrollbar_ui_controls[index]);
                if ((ctrl->flags & SB_IN_USE) != 0
                    && (ctrl->flags & SB_HIDDEN) == 0) {
                    if (msg->data.button.button_handle == ctrl->button_up) {
                        ctrl->info.field_38 -= ctrl->info.field_2C;
                        if (ctrl->info.field_38 < ctrl->info.field_28) {
                            ctrl->info.field_38 = ctrl->info.field_28;
                        }
                        if (ctrl->info.field_3C != NULL) {
                            ctrl->info.field_3C(ctrl->info.field_38);
                        }
                        sub_5806F0(ctrl->id);
                        if (dword_5CBF78 == ctrl->id.index) {
                            dword_5CBF78 = -1;
                        }
                        return true;
                    }

                    if (msg->data.button.button_handle == ctrl->button_down) {
                        ctrl->info.field_38 += ctrl->info.field_2C;
                        if (ctrl->info.field_38 > ctrl->info.field_24) {
                            ctrl->info.field_38 = ctrl->info.field_24;
                        }
                        if (ctrl->info.field_3C != NULL) {
                            ctrl->info.field_3C(ctrl->info.field_38);
                        }
                        sub_5806F0(ctrl->id);
                        if (dword_5CBF78 == ctrl->id.index) {
                            dword_5CBF78 = -1;
                        }
                        return true;
                    }
                }
            }
            return false;
        default:
            return false;
        }
    default:
        return false;
    }
}

// 0x5810D0
void sub_5810D0(ScrollbarId id, int a2, int a3)
{
    ScrollbarUiControl* ctrl;

    if (!sub_5812E0(&id, &ctrl)) {
        return;
    }

    switch (a2) {
    case 0:
        ctrl->info.field_28 = a3;
        if (ctrl->info.field_38 < ctrl->info.field_28) {
            ctrl->info.field_38 = ctrl->info.field_28;
        }
        break;
    case 1:
        ctrl->info.field_24 = a3;
        if (ctrl->info.field_38 > ctrl->info.field_24) {
            ctrl->info.field_38 = ctrl->info.field_24;
        }
        break;
    case 2:
        if (a3 >= ctrl->info.field_28 && a3 <= ctrl->info.field_24) {
            ctrl->info.field_38 = a3;
        } else {
            ctrl->info.field_38 = ctrl->info.field_28;
        }
        break;
    }

    if (ctrl->info.field_3C != NULL) {
        ctrl->info.field_3C(ctrl->info.field_38);
    }

    sub_5806F0(ctrl->id);
}

// 0x581180
void sub_581180()
{
    dword_684684++;
}

// 0x581190
void sub_581190()
{
    dword_684684--;
}

// 0x5811A0
void sub_5811A0()
{
    dword_684684 = 1;
}

// 0x5811B0
void sub_5811B0()
{
    dword_684684 = 0;
}

// 0x5811C0
void scrollbar_ui_control_reset(ScrollbarUiControl* ctrl)
{
    ctrl->id.index = -1;
    ctrl->id.index = -1;
    ctrl->flags = 0;
    memset(&(ctrl->info), 0, sizeof(ctrl->info));
    ctrl->window_handle = TIG_WINDOW_HANDLE_INVALID;
    ctrl->button_up = TIG_BUTTON_HANDLE_INVALID;
    ctrl->button_down = TIG_BUTTON_HANDLE_INVALID;
}

// 0x5811F0
void sub_5811F0(ScrollbarUiControl* ctrl, ScrollbarUiControlInfo* info)
{
    ctrl->info = *info;

    if ((ctrl->info.flags & 0x2) == 0) {
        ctrl->info.content_rect.x = 0;
        ctrl->info.content_rect.y = 0;
        ctrl->info.content_rect.width = 0;
        ctrl->info.content_rect.height = 0;
    }

    if ((ctrl->info.flags & 0x4) == 0) {
        ctrl->info.field_24 = 10;
    }

    if ((ctrl->info.flags & 0x8) == 0) {
        ctrl->info.field_28 = 0;
    }

    if ((ctrl->info.flags & 0x10) == 0) {
        ctrl->info.field_2C = 1;
    }

    if ((ctrl->info.flags & 0x20) == 0) {
        ctrl->info.field_30 = 5;
    }

    if ((ctrl->info.flags & 0x40) == 0) {
        ctrl->info.field_34 = 3;
    }

    if ((ctrl->info.flags & 0x80) == 0) {
        ctrl->info.field_38 = 0;
    }

    if ((ctrl->info.flags & 0x100) == 0) {
        ctrl->info.field_3C = 0;
    }

    if ((ctrl->info.flags & 0x200) == 0) {
        ctrl->info.field_40 = 0;
    }
}

// 0x581280
bool sub_581280(ScrollbarId* id)
{
    int index;

    for (index = 0; index < MAX_CONTROLS; index++) {
        if ((scrollbar_ui_controls[index].flags & SB_IN_USE) == 0) {
            id->index = index;
            id->global_index = dword_684678++;
            scrollbar_ui_controls[index].id = *id;
            scrollbar_ui_controls[index].flags = SB_IN_USE;
            return true;
        }
    }

    return false;
}

// 0x5812E0
bool sub_5812E0(const ScrollbarId* id, ScrollbarUiControl** ctrl_ptr)
{
    int index;

    if (id->index >= 0 && id->index < MAX_CONTROLS
        && scrollbar_ui_controls[id->index].id.global_index == id->global_index
        && (scrollbar_ui_controls[id->index].flags & SB_IN_USE) != 0) {
        *ctrl_ptr = &(scrollbar_ui_controls[id->index]);
        return true;
    }

    for (index = 0; index < MAX_CONTROLS; index++) {
        if (scrollbar_ui_controls[index].id.global_index == id->global_index
            && (scrollbar_ui_controls[id->index].flags & SB_IN_USE) != 0) {
            *ctrl_ptr = &(scrollbar_ui_controls[id->index]);
            return true;
        }
    }

    return false;
}

// 0x581360
bool sub_581360(int id, int x, int y)
{
    ScrollbarUiControl* ctrl;
    TigWindowData window_data;

    ctrl = &(scrollbar_ui_controls[id]);
    tig_window_data(ctrl->window_handle, &window_data);

    return x - window_data.rect.x >= ctrl->info.scrollbar_rect.x
        && x - window_data.rect.x <= ctrl->info.scrollbar_rect.x + ctrl->info.scrollbar_rect.width
        && y - window_data.rect.y >= sub_5815D0(id)
        && y - window_data.rect.y <= sub_581660(id);
}

// 0x5813E0
bool sub_5813E0(int id, int x, int y)
{
    ScrollbarUiControl* ctrl;
    TigWindowData window_data;

    ctrl = &(scrollbar_ui_controls[id]);
    tig_window_data(ctrl->window_handle, &window_data);

    return x - window_data.rect.x >= ctrl->info.scrollbar_rect.x
        && x - window_data.rect.x <= ctrl->info.scrollbar_rect.x + ctrl->info.scrollbar_rect.width
        && y - window_data.rect.y >= ctrl->info.content_rect.y + scrollbar_ui_button_up_height
        && y - window_data.rect.y <= sub_5815D0(id);
}

// 0x581460
bool sub_581460(int id, int x, int y)
{
    ScrollbarUiControl* ctrl;
    TigWindowData window_data;

    ctrl = &(scrollbar_ui_controls[id]);
    tig_window_data(ctrl->window_handle, &window_data);

    return x - window_data.rect.x >= ctrl->info.scrollbar_rect.x
        && x - window_data.rect.x <= ctrl->info.scrollbar_rect.x + ctrl->info.scrollbar_rect.width
        && y - window_data.rect.y >= sub_581660(id)
        && y - window_data.rect.y <= ctrl->info.content_rect.y + ctrl->info.content_rect.height - scrollbar_ui_button_down_height;
}

// 0x5814E0
bool sub_5814E0(int id, int x, int y)
{
    ScrollbarUiControl* ctrl;
    TigWindowData window_data;

    ctrl = &(scrollbar_ui_controls[id]);
    tig_window_data(ctrl->window_handle, &window_data);

    return x - window_data.rect.x >= ctrl->info.content_rect.x
        && x - window_data.rect.x <= ctrl->info.content_rect.x + ctrl->info.content_rect.width
        && y - window_data.rect.y >= ctrl->info.content_rect.y
        && y - window_data.rect.y <= ctrl->info.content_rect.y + ctrl->info.content_rect.height;
}

// 0x581550
float sub_581550(int id)
{
    ScrollbarUiControl* ctrl;

    ctrl = &(scrollbar_ui_controls[id]);
    return (float)(ctrl->info.scrollbar_rect.height - scrollbar_ui_button_down_height - scrollbar_ui_button_up_height)
        / (float)(ctrl->info.field_2C + ctrl->info.field_24 - ctrl->info.field_28)
        * (float)ctrl->info.field_2C;
}

// 0x5815A0
int sub_5815A0(int id)
{
    float v1;

    v1 = sub_581550(id);
    if (v1 > 12.0f) {
        return (int)v1;
    }
    return 12;
}

// 0x5815D0
int sub_5815D0(int id)
{
    return scrollbar_ui_button_up_height
        + scrollbar_ui_controls[id].info.scrollbar_rect.y
        + (int)(sub_581550(id)
            * (float)(scrollbar_ui_controls[id].info.field_38 - scrollbar_ui_controls[id].info.field_28)
            / (float)(scrollbar_ui_controls[id].info.field_2C));
}

// 0x581660
int sub_581660(int id)
{
    int v1;

    v1 = (int)sub_581550(id);
    return sub_5815D0(id) + (v1 >= 12 ? v1 : 12);
}

// 0x5816A0
bool sub_5816A0(int id, int a2)
{
    return a2 >= scrollbar_ui_controls[id].info.field_28
        && a2 <= scrollbar_ui_controls[id].info.field_24;
}

// 0x5816D0
void sub_5816D0()
{
    dword_684674++;
}

// 0x5816E0
void sub_5816E0()
{
    dword_684674--;
}

// 0x5816F0
int sub_5816F0()
{
    return dword_684674;
}
