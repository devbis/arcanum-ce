#include "game/tc.h"

static void sub_4C9BE0(TigRectListNode* node);

// 0x5FF4F8
static TigRect stru_5FF4F8;

// 0x5FF510
static int dword_5FF510;

// 0x5FF518
static TigVideoBuffer* dword_5FF518;

// 0x5FF520
static TigRect stru_5FF520;

// 0x5FF530
static tig_font_handle_t dword_5FF530;

// 0x5FF534
static GameContextF8* dword_5FF534;

// 0x5FF538
static tig_font_handle_t dword_5FF538;

// 0x5FF53C
static TigVideoBuffer* dword_5FF53C;

// 0x5FF548
static bool tc_editor;

// 0x5FF54C
static tig_font_handle_t dword_5FF54C;

// 0x5FF550
static tig_window_handle_t dword_5FF550;

// 0x5FF568
static bool dword_5FF568;

// 0x4C9280
void tc_init()
{
    // TODO: Incomplete.
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
        dword_5FF550 = TIG_WINDOW_HANDLE_INVALID;
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

    dword_5FF550 = resize_info->iso_window_handle;
    stru_5FF520 = resize_info->field_4;

    if (v1) {
        sub_4C96C0();
    }
}

// 0x4C95F0
void sub_4C95F0()
{
    // TODO: Incomplete.
}

// 0x4C9620
void sub_4C9620()
{
    // TODO: Incomplete.
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
    // TODO: Incomplete.
}

// 0x4C9810
void sub_4C9810()
{
    // TODO: Incomplete.
}

// 0x4C9A10
void sub_4C9A10()
{
    // TODO: Incomplete.
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

    width = font.width - dword_5FF510 + 12;
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
    win_to_vb_blt.src_window_handle = dword_5FF550;
    win_to_vb_blt.src_video_buffer = dword_5FF53C;
    win_to_vb_blt.src_rect = &src_rect;
    win_to_vb_blt.dst_video_buffer = dword_5FF53C;
    win_to_vb_blt.dst_rect = &dst_rect;

    new_head = NULL;

    vb_to_win_blt.type = TIG_WINDOW_BLIT_VIDEO_BUFFER_TO_WINDOW;
    vb_to_win_blt.vb_blit_flags = 0;
    vb_to_win_blt.src_video_buffer = dword_5FF53C;
    vb_to_win_blt.src_rect = &dst_rect;
    vb_to_win_blt.dst_window_handle = dword_5FF550;
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
