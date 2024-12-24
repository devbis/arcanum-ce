#include "game/tc.h"

static void sub_4C9BE0(TigRectListNode* node);

// 0x5B7218
static int dword_5B7218 = -1;

// 0x5FF4F8
static TigRect stru_5FF4F8;

// 0x5FF508
static TigRect stru_5FF508;

// 0x5FF518
static TigVideoBuffer* dword_5FF518;

// 0x5FF520
static TigRect tc_iso_window_rect;

// 0x5FF530
static tig_font_handle_t dword_5FF530;

// 0x5FF534
static GameContextF8* dword_5FF534;

// 0x5FF538
static tig_font_handle_t dword_5FF538;

// 0x5FF53C
static TigVideoBuffer* dword_5FF53C;

// 0x5FF540
static int dword_5FF540;

// 0x5FF544
static int dword_5FF544;

// 0x5FF548
static bool tc_editor;

// 0x5FF54C
static tig_font_handle_t dword_5FF54C;

// 0x5FF550
static tig_window_handle_t tc_iso_window_handle;

// 0x5FF554
static const char* dword_5FF554[5];

// 0x5FF568
static bool dword_5FF568;

// 0x4C9280
bool tc_init(GameInitInfo* init_info)
{
    TigWindowData window_data;
    TigVideoBufferCreateInfo vb_create_info;
    tig_art_id_t art_id;
    TigFont font;

    tc_editor = init_info->editor;
    if (!tc_editor) {
        if (tig_window_data(init_info->iso_window_handle, &window_data) != TIG_OK) {
            return false;
        }

        tc_iso_window_handle = init_info->iso_window_handle;
        tc_iso_window_rect = window_data.rect;
        dword_5FF534 = init_info->field_8;

        stru_5FF508.x = 0;
        stru_5FF508.y = 0;
        stru_5FF508.width = window_data.rect.width;
        stru_5FF508.height = 100;

        vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_COLOR_KEY | TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY;
        vb_create_info.width = window_data.rect.width;
        vb_create_info.height = 100;
        vb_create_info.color_key = tig_color_make(0, 0, 0);
        vb_create_info.background_color = vb_create_info.color_key;
        if (tig_video_buffer_create(&vb_create_info, &dword_5FF518) != TIG_OK) {
            return false;
        }

        vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY;
        vb_create_info.width = stru_5FF508.width;
        vb_create_info.height = stru_5FF508.height;
        vb_create_info.background_color = 0;
        if (tig_video_buffer_create(&vb_create_info, &dword_5FF53C) != TIG_OK) {
            tig_video_buffer_destroy(dword_5FF518);
            return false;
        }

        tig_art_interface_id_create(229, 0, 0, 0, &art_id);

        font.flags = TIG_FONT_SHADOW;
        font.art_id = art_id;
        font.color = tig_color_make(255, 255, 255);
        tig_font_create(&font, &dword_5FF538);

        font.color = tig_color_make(255, 255, 0);
        tig_font_create(&font, &dword_5FF530);

        font.color = tig_color_make(255, 0, 0);
        tig_font_create(&font, &dword_5FF54C);

        dword_5FF540 = (stru_5FF508.height - 18) / 5;
    }

    return true;
}

// 0x4C9540
void tc_exit()
{
    if (!tc_editor) {
        tig_font_destroy(dword_5FF54C);
        tig_font_destroy(dword_5FF530);
        tig_font_destroy(dword_5FF538);
        tig_video_buffer_destroy(dword_5FF53C);
        tig_video_buffer_destroy(dword_5FF518);
        tc_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
        dword_5FF534 = NULL;
    }
}

// 0x4C95A0
void tc_resize(ResizeInfo* resize_info)
{
    bool v1;

    v1 = dword_5FF568;
    if (v1) {
        sub_4C96F0();
    }

    tc_iso_window_handle = resize_info->iso_window_handle;
    tc_iso_window_rect = resize_info->field_4;

    if (v1) {
        sub_4C96C0();
    }
}

// 0x4C95F0
void tc_render(UnknownContext* render_info)
{
    if (tc_editor) {
        return;
    }

    if (!dword_5FF568) {
        return;
    }

    sub_4C9BE0(*render_info->rects);
}

// 0x4C9620
void sub_4C9620(int dx, int dy)
{
    TigRect rect;

    if (!tc_editor && dword_5FF568) {
        rect = stru_5FF4F8;

        if (dx < 0) {
            rect.x += dx;
            rect.width -= dx;
        } else if (dx > 0) {
            rect.width += dx;
        }

        if (dy < 0) {
            rect.y += dy;
            rect.height -= dy;
        } else if (dy > 0) {
            rect.height += dy;
        }

        dword_5FF534(&rect);
    }
}

// 0x4C96C0
void sub_4C96C0()
{
    if (!tc_editor) {
        if (!dword_5FF568) {
            dword_5FF568 = true;
            dword_5FF534(&stru_5FF4F8);
        }
    }
}

// 0x4C96F0
void sub_4C96F0()
{
    if (!tc_editor) {
        if (dword_5FF568) {
            dword_5FF568 = false;
            dword_5FF534(&stru_5FF4F8);
        }
    }
}

// 0x4C9720
void sub_4C9720()
{
    if (!tc_editor) {
        tig_video_buffer_fill(dword_5FF518,
            &stru_5FF508,
            tig_color_make(0, 0, 0));

        dword_5FF554[0] = NULL;
        dword_5FF554[1] = NULL;
        dword_5FF554[2] = NULL;
        dword_5FF554[3] = NULL;
        dword_5FF554[4] = NULL;

        dword_5FF534(&stru_5FF4F8);

        dword_5B7218 = -1;
        dword_5FF544 = 0;

        stru_5FF4F8.width = 400;
        stru_5FF4F8.height = 100;
        stru_5FF4F8.x = (tc_iso_window_rect.width - 400) / 2;

        if (tc_iso_window_rect.height == 400) {
            stru_5FF4F8.y = 299;
        } else {
            stru_5FF4F8.y = tc_iso_window_rect.height - 181;
        }
    }
}

// 0x4C9810
void sub_4C9810(int index, const char* str)
{
    TigRect rect;
    TigRect dirty_rect;
    TigMouseState mouse_state;
    bool mouse_inside;

    if (!tc_editor) {
        dword_5FF554[index] = str;

        rect.x = 6;
        rect.y = index * (dword_5FF540 + 3) + 3;
        rect.width = stru_5FF508.width - 12;
        rect.height = dword_5FF540;

        tig_video_buffer_fill(dword_5FF518,
            &rect,
            tig_color_make(0, 0, 0));

        mouse_inside = false;
        // TODO: Probably missing horizontal rect constraints.
        if (tig_mouse_get_state(&mouse_state) == TIG_OK
            && mouse_state.x - tc_iso_window_rect.x >= stru_5FF4F8.x
            && mouse_state.y - tc_iso_window_rect.y >= stru_5FF4F8.y
            && mouse_state.x - tc_iso_window_rect.x < stru_5FF4F8.x + stru_5FF4F8.width
            && mouse_state.y - tc_iso_window_rect.y < stru_5FF4F8.y + stru_5FF4F8.height
            && mouse_state.y - tc_iso_window_rect.y - stru_5FF4F8.y >= rect.y
            && mouse_state.y - tc_iso_window_rect.y - stru_5FF4F8.y < rect.y + rect.height) {
            mouse_inside = true;
            if (dword_5FF544) {
                tig_font_push(dword_5FF54C);
            } else {
                tig_font_push(dword_5FF530);
            }
        } else {
            tig_font_push(dword_5FF538);
        }

        tig_font_write(dword_5FF518, str, &rect, &dirty_rect);
        tig_font_pop();

        if (stru_5FF4F8.width < dirty_rect.width + 12) {
            stru_5FF4F8.width = dirty_rect.width + 12;
            stru_5FF4F8.x = (tc_iso_window_rect.width - stru_5FF4F8.width) / 2;
        }

        if (dword_5FF568) {
            dword_5FF534(&stru_5FF4F8);
        }

        if (mouse_inside && dword_5B7218 != index) {
            if (dword_5B7218 != -1) {
                if (dword_5FF554[dword_5B7218] != NULL) {
                    sub_4C9810(dword_5B7218, dword_5FF554[dword_5B7218]);
                }
            }
            dword_5B7218 = index;
        }
    }
}

// 0x4C9A10
int sub_4C9A10(TigMessage* msg)
{
    int index;

    if (tc_editor) {
        return -1;
    }

    if (!dword_5FF568) {
        return -1;
    }

    if (msg->type != TIG_MESSAGE_MOUSE) {
        return -1;
    }

    if (msg->data.mouse.x < tc_iso_window_rect.x + stru_5FF4F8.x
        || msg->data.mouse.y < tc_iso_window_rect.y + stru_5FF4F8.y
        || msg->data.mouse.x >= tc_iso_window_rect.x + stru_5FF4F8.x + stru_5FF4F8.width
        || msg->data.mouse.y >= tc_iso_window_rect.y + stru_5FF4F8.y + stru_5FF4F8.height) {
        if (dword_5B7218 != -1) {
            sub_4C9810(dword_5B7218, dword_5FF554[dword_5B7218]);
            dword_5B7218 = -1;
        }
        return -1;
    }

    switch (msg->data.mouse.event) {
    case TIG_MESSAGE_MOUSE_LEFT_BUTTON_DOWN:
        if (msg->data.mouse.repeat) {
            return -1;
        }

        index = (msg->data.mouse.y - (tc_iso_window_rect.y + stru_5FF4F8.y)) / (stru_5FF508.height / 5);
        if (dword_5FF554[index] != NULL) {
            return index;
        }
        return -1;
    case TIG_MESSAGE_MOUSE_MOVE:
        index = (msg->data.mouse.y - (tc_iso_window_rect.y + stru_5FF4F8.y)) / (stru_5FF508.height / 5);
        if (dword_5B7218 != index && dword_5B7218 != -1) {
            sub_4C9810(dword_5B7218, dword_5FF554[dword_5B7218]);
        }
        if (dword_5FF554[index] != NULL) {
            sub_4C9810(index, dword_5FF554[index]);
            dword_5B7218 = index;
        } else {
            dword_5B7218 = -1;
        }
        return -1;
    default:
        return -1;
    }
}

// 0x4C9B90
int sub_4C9B90(const char* str)
{
    TigFont font;
    int width;

    font.width = 0;
    font.str = str;

    tig_font_push(dword_5FF538);
    sub_535390(&font);
    tig_font_pop();

    width = font.width - stru_5FF508.width + 12;
    if (width < 0) {
        width = 0;
    }

    return width;
}

// 0x4C9BE0
void sub_4C9BE0(TigRectListNode* node)
{
    TigRect src_rect;
    TigRect dst_rect;
    TigWindowBlitInfo win_to_vb_blt;
    TigWindowBlitInfo vb_to_win_blt;
    TigRectListNode* new_head;
    TigRectListNode* new_node;

    win_to_vb_blt.type = TIG_WINDOW_BLT_WINDOW_TO_VIDEO_BUFFER;
    win_to_vb_blt.vb_blit_flags = 0;
    win_to_vb_blt.src_window_handle = tc_iso_window_handle;
    win_to_vb_blt.src_video_buffer = dword_5FF53C;
    win_to_vb_blt.src_rect = &src_rect;
    win_to_vb_blt.dst_video_buffer = dword_5FF53C;
    win_to_vb_blt.dst_rect = &dst_rect;

    new_head = NULL;

    vb_to_win_blt.type = TIG_WINDOW_BLIT_VIDEO_BUFFER_TO_WINDOW;
    vb_to_win_blt.vb_blit_flags = 0;
    vb_to_win_blt.src_video_buffer = dword_5FF53C;
    vb_to_win_blt.src_rect = &dst_rect;
    vb_to_win_blt.dst_window_handle = tc_iso_window_handle;
    vb_to_win_blt.dst_rect = &src_rect;

    while (node != NULL) {
        if (tig_rect_intersection(&stru_5FF4F8, &(node->rect), &src_rect) == TIG_OK) {
            dst_rect.x = src_rect.x - stru_5FF4F8.x;
            dst_rect.y = src_rect.y - stru_5FF4F8.y;
            dst_rect.width = src_rect.width;
            dst_rect.height = src_rect.height;

            tig_window_blit(&win_to_vb_blt);
            tig_video_buffer_tint(dword_5FF53C,
                &dst_rect,
                tig_color_make(18, 18, 18),
                TIG_VIDEO_BUFFER_TINT_MODE_SUB);
            tig_window_blit(&vb_to_win_blt);

            new_node = tig_rect_node_create();
            new_node->rect = dst_rect;
            new_node->next = new_head;
            new_head = new_node;
        }
        node = node->next;
    }

    vb_to_win_blt.src_video_buffer = dword_5FF518;

    while (new_head != NULL) {
        vb_to_win_blt.src_rect = &(new_head->rect);

        src_rect = new_head->rect;
        src_rect.x += stru_5FF4F8.x;
        src_rect.y += stru_5FF4F8.y;
        tig_window_blit(&vb_to_win_blt);

        new_node = new_head->next;
        tig_rect_node_destroy(new_head);
        new_head = new_node;
    }
}
