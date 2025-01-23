#include "ui/roller_ui.h"

#include <stdio.h>

// 0x67BB18
static tig_font_handle_t dword_67BB18;

// 0x67BB1C
static int dword_67BB1C;

// 0x67BB20
static tig_font_handle_t dword_67BB20;

// 0x67BB24
static TigVideoBuffer* roller_ui_video_buffer;

// 0x67BB28
static tig_font_handle_t dword_67BB28;

// 0x67BB2C
static int dword_67BB2C;

// 0x67BB30
static int dword_67BB30;

// 0x67BB34
static bool roller_ui_initialized;

// 0x56A020
bool roller_ui_init(GameInitInfo* init_info)
{
    TigFont font_desc;
    TigVideoBufferCreateInfo vb_create_info;
    TigRect rect;
    TigArtBlitInfo blit_info;

    (void)init_info;

    font_desc.flags = 0;
    tig_art_interface_id_create(25, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;

    font_desc.color = tig_color_make(0, 0, 0);
    tig_font_create(&font_desc, &dword_67BB20);

    font_desc.color = tig_color_make(0, 110, 0);
    tig_font_create(&font_desc, &dword_67BB28);

    font_desc.color = tig_color_make(192, 32, 32);
    tig_font_create(&font_desc, &dword_67BB18);

    tig_font_push(dword_67BB20);

    font_desc.str = "+";
    font_desc.width = 0;
    tig_font_measure(&font_desc);
    dword_67BB1C = font_desc.width;

    font_desc.str = "0";
    font_desc.width = 0;
    tig_font_measure(&font_desc);
    dword_67BB2C = font_desc.width;
    dword_67BB30 = font_desc.height;

    tig_font_pop();

    vb_create_info.flags = TIG_VIDEO_BUFFER_VIDEO_MEMORY;
    vb_create_info.width = dword_67BB1C + dword_67BB2C;
    vb_create_info.height = dword_67BB30;
    vb_create_info.background_color = 0;

    if (tig_video_buffer_create(&vb_create_info, &roller_ui_video_buffer) != TIG_OK) {
        tig_font_destroy(dword_67BB20);
        tig_font_destroy(dword_67BB28);
        tig_font_destroy(dword_67BB18);
        return false;
    }

    rect.x = 0;
    rect.y = 0;
    rect.width = vb_create_info.width;
    rect.height = vb_create_info.height;

    blit_info.flags = 0;
    tig_art_interface_id_create(314, 0, 0, 0, &(blit_info.art_id));
    blit_info.src_rect = &rect;
    blit_info.dst_rect = &rect;
    blit_info.dst_video_buffer = roller_ui_video_buffer;

    if (tig_art_blit(&blit_info) != TIG_OK) {
        tig_font_destroy(dword_67BB20);
        tig_font_destroy(dword_67BB28);
        tig_font_destroy(dword_67BB18);
        tig_video_buffer_destroy(roller_ui_video_buffer);
        return false;
    }

    roller_ui_initialized = true;

    return true;
}

// 0x56A2B0
void roller_ui_exit()
{
    if (!roller_ui_initialized) {
        return;
    }

    tig_font_destroy(dword_67BB20);
    tig_font_destroy(dword_67BB28);
    tig_font_destroy(dword_67BB18);
    tig_video_buffer_destroy(roller_ui_video_buffer);

    roller_ui_initialized = false;
}

// 0x56A300
void roller_ui_draw(int value, tig_window_handle_t window_handle, int x, int y, int max_digits, int flags)
{
    const char* sign;
    TigRect dst_rect;
    TigRect src_rect;
    int dig;
    char buffer[2];

    if ((flags & 0x4) != 0) {
        tig_font_push(dword_67BB28);
    } else if ((flags & 0x8) != 0) {
        tig_font_push(dword_67BB18);
    } else {
        tig_font_push(dword_67BB20);
    }

    dst_rect.x = x;
    dst_rect.y = y;

    if ((flags & 0x1) != 0) {
        sign = value >= 0 ? "+" : "-";
        dst_rect.width = dword_67BB1C;
        dst_rect.height = dword_67BB30;

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = dword_67BB1C;
        src_rect.height = dword_67BB30;

        tig_window_copy_from_vbuffer(window_handle,
            &dst_rect,
            roller_ui_video_buffer,
            &src_rect);
        tig_window_text_write(window_handle, sign, &dst_rect);
    }

    dst_rect.width = dword_67BB2C;
    dst_rect.height = dword_67BB30;

    src_rect.x = dword_67BB1C;
    src_rect.y = 0;
    src_rect.width = dword_67BB2C;
    src_rect.height = dword_67BB30;

    for (dig = 0; dig < max_digits; dig++) {
        snprintf(buffer, sizeof(buffer), "%d", value % 10);
        tig_window_copy_from_vbuffer(window_handle,
            &dst_rect,
            roller_ui_video_buffer,
            &src_rect);
        if (dig == 0 || value != 0 || (flags & 0x2) != 0) {
            tig_window_text_write(window_handle, buffer, &dst_rect);
        }
        dst_rect.x -= dword_67BB2C;
        value /= 10;
    }

    tig_font_pop();
}
