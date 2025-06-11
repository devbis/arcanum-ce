#include "ui/roller_ui.h"

#include <stdio.h>

// 0x67BB18
static tig_font_handle_t roller_ui_red_font;

// 0x67BB1C
static int roller_ui_sign_width;

// 0x67BB20
static tig_font_handle_t roller_ui_black_font;

// 0x67BB24
static TigVideoBuffer* roller_ui_video_buffer;

// 0x67BB28
static tig_font_handle_t roller_ui_green_font;

// 0x67BB2C
static int roller_ui_digit_width;

// 0x67BB30
static int roller_ui_digit_height;

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
    tig_font_create(&font_desc, &roller_ui_black_font);

    font_desc.color = tig_color_make(0, 110, 0);
    tig_font_create(&font_desc, &roller_ui_green_font);

    font_desc.color = tig_color_make(192, 32, 32);
    tig_font_create(&font_desc, &roller_ui_red_font);

    tig_font_push(roller_ui_black_font);

    font_desc.str = "+";
    font_desc.width = 0;
    tig_font_measure(&font_desc);
    roller_ui_sign_width = font_desc.width;

    font_desc.str = "0";
    font_desc.width = 0;
    tig_font_measure(&font_desc);
    roller_ui_digit_width = font_desc.width;
    roller_ui_digit_height = font_desc.height;

    tig_font_pop();

    vb_create_info.flags = TIG_VIDEO_BUFFER_VIDEO_MEMORY;
    vb_create_info.width = roller_ui_sign_width + roller_ui_digit_width;
    vb_create_info.height = roller_ui_digit_height;
    vb_create_info.background_color = 0;

    if (tig_video_buffer_create(&vb_create_info, &roller_ui_video_buffer) != TIG_OK) {
        tig_font_destroy(roller_ui_black_font);
        tig_font_destroy(roller_ui_green_font);
        tig_font_destroy(roller_ui_red_font);
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
        tig_font_destroy(roller_ui_black_font);
        tig_font_destroy(roller_ui_green_font);
        tig_font_destroy(roller_ui_red_font);
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

    tig_font_destroy(roller_ui_black_font);
    tig_font_destroy(roller_ui_green_font);
    tig_font_destroy(roller_ui_red_font);
    tig_video_buffer_destroy(roller_ui_video_buffer);

    roller_ui_initialized = false;
}

// 0x56A300
void roller_ui_draw(int value, tig_window_handle_t window_handle, int x, int y, int num_digits, RollerFlags flags)
{
    const char* sign;
    TigRect dst_rect;
    TigRect src_rect;
    int dig;
    char buffer[2];

    if ((flags & ROLLER_GREEN) != 0) {
        tig_font_push(roller_ui_green_font);
    } else if ((flags & ROLLER_RED) != 0) {
        tig_font_push(roller_ui_red_font);
    } else {
        tig_font_push(roller_ui_black_font);
    }

    dst_rect.x = x;
    dst_rect.y = y;

    if ((flags & ROLLER_SIGN) != 0) {
        sign = value >= 0 ? "+" : "-";
        dst_rect.width = roller_ui_sign_width;
        dst_rect.height = roller_ui_digit_height;

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = roller_ui_sign_width;
        src_rect.height = roller_ui_digit_height;

        tig_window_copy_from_vbuffer(window_handle,
            &dst_rect,
            roller_ui_video_buffer,
            &src_rect);
        tig_window_text_write(window_handle, sign, &dst_rect);

        dst_rect.x += roller_ui_sign_width;
    }

    dst_rect.x += roller_ui_digit_width * (num_digits - 1);
    dst_rect.width = roller_ui_digit_width;
    dst_rect.height = roller_ui_digit_height;

    src_rect.x = roller_ui_sign_width;
    src_rect.y = 0;
    src_rect.width = roller_ui_digit_width;
    src_rect.height = roller_ui_digit_height;

    for (dig = 0; dig < num_digits; dig++) {
        snprintf(buffer, sizeof(buffer), "%d", value % 10);
        tig_window_copy_from_vbuffer(window_handle,
            &dst_rect,
            roller_ui_video_buffer,
            &src_rect);
        if (dig == 0 || value != 0 || (flags & ROLLER_ZERO) != 0) {
            tig_window_text_write(window_handle, buffer, &dst_rect);
        }
        dst_rect.x -= roller_ui_digit_width;
        value /= 10;
    }

    tig_font_pop();
}
