#include "game/tc.h"

static void tc_render_internal(TigRectListNode* node);

// 0x5B7218
static int tc_highlighted_idx = -1;

// 0x5FF4F8
static TigRect tc_content_rect;

// 0x5FF508
static TigRect tc_intermediate_video_buffer_rect;

// 0x5FF518
static TigVideoBuffer* tc_scratch_video_buffer;

// 0x5FF520
static TigRect tc_iso_window_rect;

// 0x5FF530
static tig_font_handle_t tc_yellow_font;

// 0x5FF534
static IsoInvalidateRectFunc* tc_iso_window_invalidate_rect;

// 0x5FF538
static tig_font_handle_t tc_white_font;

// 0x5FF53C
static TigVideoBuffer* tc_backdrop_video_buffer;

// 0x5FF540
static int tc_line_height;

// 0x5FF544
static bool dword_5FF544;

// 0x5FF548
static bool tc_editor;

// 0x5FF54C
static tig_font_handle_t tc_red_font;

// 0x5FF550
static tig_window_handle_t tc_iso_window_handle;

// 0x5FF554
static const char* tc_options[5];

// 0x5FF568
static bool tc_active;

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
        tc_iso_window_invalidate_rect = init_info->invalidate_rect_func;

        tc_intermediate_video_buffer_rect.x = 0;
        tc_intermediate_video_buffer_rect.y = 0;
        tc_intermediate_video_buffer_rect.width = window_data.rect.width;
        tc_intermediate_video_buffer_rect.height = 100;

        vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_COLOR_KEY | TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY;
        vb_create_info.width = window_data.rect.width;
        vb_create_info.height = 100;
        vb_create_info.color_key = tig_color_make(0, 0, 0);
        vb_create_info.background_color = vb_create_info.color_key;
        if (tig_video_buffer_create(&vb_create_info, &tc_scratch_video_buffer) != TIG_OK) {
            return false;
        }

        vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY;
        vb_create_info.width = tc_intermediate_video_buffer_rect.width;
        vb_create_info.height = tc_intermediate_video_buffer_rect.height;
        vb_create_info.background_color = 0;
        if (tig_video_buffer_create(&vb_create_info, &tc_backdrop_video_buffer) != TIG_OK) {
            tig_video_buffer_destroy(tc_scratch_video_buffer);
            return false;
        }

        tig_art_interface_id_create(229, 0, 0, 0, &art_id);

        font.flags = TIG_FONT_SHADOW;
        font.art_id = art_id;
        font.color = tig_color_make(255, 255, 255);
        tig_font_create(&font, &tc_white_font);

        font.color = tig_color_make(255, 255, 0);
        tig_font_create(&font, &tc_yellow_font);

        font.color = tig_color_make(255, 0, 0);
        tig_font_create(&font, &tc_red_font);

        tc_line_height = (tc_intermediate_video_buffer_rect.height - 18) / 5;
    }

    return true;
}

// 0x4C9540
void tc_exit()
{
    if (!tc_editor) {
        tig_font_destroy(tc_red_font);
        tig_font_destroy(tc_yellow_font);
        tig_font_destroy(tc_white_font);
        tig_video_buffer_destroy(tc_backdrop_video_buffer);
        tig_video_buffer_destroy(tc_scratch_video_buffer);
        tc_iso_window_handle = TIG_WINDOW_HANDLE_INVALID;
        tc_iso_window_invalidate_rect = NULL;
    }
}

// 0x4C95A0
void tc_resize(GameResizeInfo* resize_info)
{
    bool was_active;

    was_active = tc_active;
    if (was_active) {
        tc_hide();
    }

    tc_iso_window_handle = resize_info->window_handle;
    tc_iso_window_rect = resize_info->window_rect;

    if (was_active) {
        tc_show();
    }
}

// 0x4C95F0
void tc_render(UnknownContext* render_info)
{
    if (tc_editor) {
        return;
    }

    if (!tc_active) {
        return;
    }

    tc_render_internal(*render_info->rects);
}

// 0x4C9620
void tc_scroll(int dx, int dy)
{
    TigRect rect;

    if (tc_editor) {
        return;
    }

    if (!tc_active) {
        return;
    }

    rect = tc_content_rect;

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

    tc_iso_window_invalidate_rect(&rect);
}

// 0x4C96C0
void tc_show()
{
    if (tc_editor) {
        return;
    }

    if (tc_active) {
        return;
    }

    tc_active = true;
    tc_iso_window_invalidate_rect(&tc_content_rect);
}

// 0x4C96F0
void tc_hide()
{
    if (tc_editor) {
        return;
    }

    if (!tc_active) {
        return;
    }

    tc_active = false;
    tc_iso_window_invalidate_rect(&tc_content_rect);
}

// 0x4C9720
void tc_clear()
{
    if (tc_editor) {
        return;
    }

    tig_video_buffer_fill(tc_scratch_video_buffer,
        &tc_intermediate_video_buffer_rect,
        tig_color_make(0, 0, 0));

    tc_options[0] = NULL;
    tc_options[1] = NULL;
    tc_options[2] = NULL;
    tc_options[3] = NULL;
    tc_options[4] = NULL;

    tc_iso_window_invalidate_rect(&tc_content_rect);

    tc_highlighted_idx = -1;
    dword_5FF544 = false;

    tc_content_rect.width = 400;
    tc_content_rect.height = 100;
    tc_content_rect.x = (tc_iso_window_rect.width - tc_content_rect.width) / 2;

    if (tc_iso_window_rect.height == 400) {
        tc_content_rect.y = tc_iso_window_rect.height - 101;
    } else {
        tc_content_rect.y = tc_iso_window_rect.height - 181;
    }
}

// 0x4C9810
void tc_set_option(int index, const char* str)
{
    TigRect rect;
    TigRect dirty_rect;
    TigMouseState mouse_state;
    bool highlighted;

    if (!tc_editor) {
        tc_options[index] = str;

        rect.x = 6;
        rect.y = index * (tc_line_height + 3) + 3;
        rect.width = tc_intermediate_video_buffer_rect.width - 12;
        rect.height = tc_line_height;

        tig_video_buffer_fill(tc_scratch_video_buffer,
            &rect,
            tig_color_make(0, 0, 0));

        highlighted = false;
        // TODO: Probably missing horizontal rect constraints.
        if (tig_mouse_get_state(&mouse_state) == TIG_OK
            && mouse_state.x - tc_iso_window_rect.x >= tc_content_rect.x
            && mouse_state.y - tc_iso_window_rect.y >= tc_content_rect.y
            && mouse_state.x - tc_iso_window_rect.x < tc_content_rect.x + tc_content_rect.width
            && mouse_state.y - tc_iso_window_rect.y < tc_content_rect.y + tc_content_rect.height
            && mouse_state.y - tc_iso_window_rect.y - tc_content_rect.y >= rect.y
            && mouse_state.y - tc_iso_window_rect.y - tc_content_rect.y < rect.y + rect.height) {
            highlighted = true;
            if (dword_5FF544) {
                tig_font_push(tc_red_font);
            } else {
                tig_font_push(tc_yellow_font);
            }
        } else {
            tig_font_push(tc_white_font);
        }

        tig_font_write(tc_scratch_video_buffer, str, &rect, &dirty_rect);
        tig_font_pop();

        if (tc_content_rect.width < dirty_rect.width + 12) {
            tc_content_rect.width = dirty_rect.width + 12;
            tc_content_rect.x = (tc_iso_window_rect.width - tc_content_rect.width) / 2;
        }

        if (tc_active) {
            tc_iso_window_invalidate_rect(&tc_content_rect);
        }

        if (highlighted && tc_highlighted_idx != index) {
            if (tc_highlighted_idx != -1) {
                if (tc_options[tc_highlighted_idx] != NULL) {
                    tc_set_option(tc_highlighted_idx, tc_options[tc_highlighted_idx]);
                }
            }
            tc_highlighted_idx = index;
        }
    }
}

// 0x4C9A10
int tc_handle_message(TigMessage* msg)
{
    int index;

    if (tc_editor) {
        return -1;
    }

    if (!tc_active) {
        return -1;
    }

    if (msg->type != TIG_MESSAGE_MOUSE) {
        return -1;
    }

    if (msg->data.mouse.x < tc_iso_window_rect.x + tc_content_rect.x
        || msg->data.mouse.y < tc_iso_window_rect.y + tc_content_rect.y
        || msg->data.mouse.x >= tc_iso_window_rect.x + tc_content_rect.x + tc_content_rect.width
        || msg->data.mouse.y >= tc_iso_window_rect.y + tc_content_rect.y + tc_content_rect.height) {
        if (tc_highlighted_idx != -1) {
            tc_set_option(tc_highlighted_idx, tc_options[tc_highlighted_idx]);
            tc_highlighted_idx = -1;
        }
        return -1;
    }

    switch (msg->data.mouse.event) {
    case TIG_MESSAGE_MOUSE_LEFT_BUTTON_DOWN:
        if (msg->data.mouse.repeat) {
            return -1;
        }

        index = (msg->data.mouse.y - (tc_iso_window_rect.y + tc_content_rect.y)) / (tc_intermediate_video_buffer_rect.height / 5);
        if (tc_options[index] != NULL) {
            return index;
        }
        return -1;
    case TIG_MESSAGE_MOUSE_MOVE:
        index = (msg->data.mouse.y - (tc_iso_window_rect.y + tc_content_rect.y)) / (tc_intermediate_video_buffer_rect.height / 5);
        if (tc_highlighted_idx != index && tc_highlighted_idx != -1) {
            tc_set_option(tc_highlighted_idx, tc_options[tc_highlighted_idx]);
        }
        if (tc_options[index] != NULL) {
            tc_set_option(index, tc_options[index]);
            tc_highlighted_idx = index;
        } else {
            tc_highlighted_idx = -1;
        }
        return -1;
    default:
        return -1;
    }
}

// 0x4C9B90
int tc_check_size(const char* str)
{
    TigFont font;
    int width;

    font.width = 0;
    font.str = str;

    tig_font_push(tc_white_font);
    tig_font_measure(&font);
    tig_font_pop();

    width = font.width - tc_intermediate_video_buffer_rect.width + 12;
    if (width < 0) {
        width = 0;
    }

    return width;
}

// 0x4C9BE0
void tc_render_internal(TigRectListNode* node)
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
    win_to_vb_blt.src_rect = &src_rect;
    win_to_vb_blt.dst_video_buffer = tc_backdrop_video_buffer;
    win_to_vb_blt.dst_rect = &dst_rect;

    new_head = NULL;

    vb_to_win_blt.type = TIG_WINDOW_BLIT_VIDEO_BUFFER_TO_WINDOW;
    vb_to_win_blt.vb_blit_flags = 0;
    vb_to_win_blt.src_video_buffer = tc_backdrop_video_buffer;
    vb_to_win_blt.src_rect = &dst_rect;
    vb_to_win_blt.dst_window_handle = tc_iso_window_handle;
    vb_to_win_blt.dst_rect = &src_rect;

    while (node != NULL) {
        if (tig_rect_intersection(&tc_content_rect, &(node->rect), &src_rect) == TIG_OK) {
            dst_rect.x = src_rect.x - tc_content_rect.x;
            dst_rect.y = src_rect.y - tc_content_rect.y;
            dst_rect.width = src_rect.width;
            dst_rect.height = src_rect.height;

            tig_window_blit(&win_to_vb_blt);
            tig_video_buffer_tint(tc_backdrop_video_buffer,
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

    vb_to_win_blt.src_video_buffer = tc_scratch_video_buffer;

    while (new_head != NULL) {
        vb_to_win_blt.src_rect = &(new_head->rect);

        src_rect = new_head->rect;
        src_rect.x += tc_content_rect.x;
        src_rect.y += tc_content_rect.y;
        tig_window_blit(&vb_to_win_blt);

        new_node = new_head->next;
        tig_rect_node_destroy(new_head);
        new_head = new_node;
    }
}
