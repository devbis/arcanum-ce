#include "game/lib/li.h"

#include "tig/art.h"
#include "tig/color.h"
#include "tig/debug.h"
#include "tig/video.h"
#include "tig/window.h"

static void sub_4BBCE0();

// 0x5FC3F8
static unsigned int dword_5FC3F8;

// 0x5FC400
static TigRect stru_5FC400;

// 0x5FC410
static TigArtBlitSpec stru_5FC410;

// 0x5FC43C
static int dword_5FC43C;

// 0x5FC440
static TigVideoBuffer* off_5FC440;

// 0x5FC448
static TigRect stru_5FC448;

// 0x4BB9A0
bool li_init(GameContext* ctx)
{
    tig_art_interface_id_create(601, 0, 0, 0, &dword_5FC3F8);

    TigArtFrameData art_frame_data;
    if (tig_art_frame_data(dword_5FC3F8, &art_frame_data) != TIG_OK) {
        return false;
    }

    TigWindowData window_data;
    if (tig_window_data(ctx->iso_window_handle, &window_data) != TIG_OK) {
        return false;
    }

    TigVideoBufferCreateInfo vb_create_info;
    vb_create_info.width = art_frame_data.width;
    vb_create_info.height = art_frame_data.height;
    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_COLOR_KEY | TIG_VIDEO_BUFFER_CREATE_VIDEO_MEMORY;
    vb_create_info.color_key = tig_color_make(0, 0, 255);
    vb_create_info.background_color = vb_create_info.color_key;
    if (tig_video_buffer_create(&vb_create_info, &off_5FC440) != TIG_OK) {
        return false;
    }

    stru_5FC448.x = 0;
    stru_5FC448.y = 0;
    stru_5FC448.width = art_frame_data.width;
    stru_5FC448.height = art_frame_data.height;

    stru_5FC400.x = window_data.rect.width - art_frame_data.width + window_data.rect.x - 2;
    stru_5FC400.y = window_data.rect.y + 2;
    stru_5FC400.width = art_frame_data.width;
    stru_5FC400.height = art_frame_data.height;

    stru_5FC410.field_0 = 0;
    stru_5FC410.src_rect = &stru_5FC448;
    stru_5FC410.dst_video_buffer = off_5FC440;
    stru_5FC410.dst_rect = &stru_5FC448;

    sub_4BBCE0();

    return true;
}

// 0x4BBB00
void li_exit()
{
    if (off_5FC440 != NULL) {
        tig_video_buffer_destroy(off_5FC440);
        off_5FC440 = NULL;
    }
}

// 0x4BBB20
void li_resize(ResizeContext* ctx)
{
    tig_art_interface_id_create(601, 0, 0, 0, &dword_5FC3F8);

    TigArtFrameData art_frame_data;
    if (tig_art_frame_data(dword_5FC3F8, &art_frame_data) != TIG_OK) {
        tig_debug_printf("li_resize: ERROR: couldn't grab frame data!\n");
        exit(EXIT_FAILURE);
    }

    TigWindowData window_data;
    if (tig_window_data(ctx->iso_window_handle, &window_data) != TIG_OK) {
        tig_debug_printf("li_resize: ERROR: couldn't grab window data!");
        exit(EXIT_FAILURE);
    }

    stru_5FC448.x = 0;
    stru_5FC448.y = 0;
    stru_5FC448.width = art_frame_data.width;
    stru_5FC448.height = art_frame_data.height;

    stru_5FC400.x = window_data.rect.width - art_frame_data.width + window_data.rect.x - 2;
    stru_5FC400.y = window_data.rect.y + 2;
    stru_5FC400.width = art_frame_data.width;
    stru_5FC400.height = art_frame_data.height;

    sub_4BBCE0();
}

// 0x4BBC00
void sub_4BBC00()
{
    dword_5FC43C++;
}

// 0x4BBC10
void sub_4BBC10()
{
    dword_5FC43C--;
}

// 0x4BBC20
int sub_4BBC20()
{
    return dword_5FC43C;
}

// 0x4BBC30
void sub_4BBC30()
{
    if (dword_5FC43C <= 0) {
        tig_video_blit(off_5FC440, &stru_5FC448, &stru_5FC400, 1);
        dword_5FC3F8 = tig_art_id_frame_inc(dword_5FC3F8);
        sub_4BBCE0();
        tig_window_set_needs_display_in_rect(&stru_5FC400);
    }
}

// 0x4BBC80
void sub_4BBC80()
{
    tig_video_buffer_fill(off_5FC440, &stru_5FC448, tig_color_make(0, 0, 255));
    sub_4BBCE0();
}

// 0x4BBCE0
static void sub_4BBCE0()
{
    stru_5FC410.src_art_id = dword_5FC3F8;
    tig_art_blit(&stru_5FC410);
}
