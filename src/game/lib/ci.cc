#include "game/lib/ci.h"

#include "tig/art.h"
#include "tig/color.h"
#include "tig/video.h"
#include "tig/window.h"

static int sub_4BB980();

// 0x5FC398
static unsigned int dword_5FC398;

// 0x5FC3A0
static TigRect stru_5FC3A0;

// 0x5FC3B0
static TigArtBlitSpec stru_5FC3B0;

// 0x5FC3DC
static int dword_5FC3DC;

// 0x5FC3E0
static TigVideoBuffer* dword_5FC3E0;

// 0x5FC3E8
static TigRect stru_5FC3E8;

// 0x4BB720
bool ci_init(GameContext* ctx)
{
    tig_art_interface_id_create(601, 0, 0, 0, &dword_5FC398);

    TigArtFrameData art_frame_data;
    if (tig_art_frame_data(dword_5FC398, &art_frame_data) != TIG_OK) {
        return false;
    }

    // TODO: There is a strange repetition of the above code, omitting for now,
    // needs testing.

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
    if (tig_video_buffer_create(&vb_create_info, &dword_5FC3E0) != TIG_OK) {
        return false;
    }

    stru_5FC3E8.x = 0;
    stru_5FC3E8.y = 0;
    stru_5FC3E8.width = art_frame_data.width;
    stru_5FC3E8.height = art_frame_data.height;

    stru_5FC3A0.x = window_data.rect.width - art_frame_data.width + window_data.rect.x - 2;
    stru_5FC3A0.y = window_data.rect.y + art_frame_data.height + 2 + 2;
    stru_5FC3A0.width = art_frame_data.width;
    stru_5FC3A0.height = art_frame_data.height;

    stru_5FC3B0.field_0 = 0;
    stru_5FC3B0.src_rect = &stru_5FC3E8;
    stru_5FC3B0.dst_video_buffer = dword_5FC3E0;
    stru_5FC3B0.dst_rect = &stru_5FC3E8;

    sub_4BB980();

    return true;
}

// 0x4BB8C0
void ci_exit()
{
    if (dword_5FC3E0 != NULL) {
        tig_video_buffer_destroy(dword_5FC3E0);
        dword_5FC3E0 = NULL;
    }
}

// 0x4BB8E0
void sub_4BB8E0()
{
    dword_5FC3DC++;
}

// 0x4BB8F0
void sub_4BB8F0()
{
    dword_5FC3DC--;
}

// 0x4BB900
int sub_4BB900()
{
    return dword_5FC3DC;
}

// 0x4BB910
void nullsub_11()
{
}

// 0x4BB920
void sub_4BB920()
{
    tig_video_buffer_fill(dword_5FC3E0, &stru_5FC3E8, tig_color_make(0, 0, 255));
    sub_4BB980();
}

// 0x4BB980
int sub_4BB980()
{
    stru_5FC3B0.src_art_id = dword_5FC398;
    tig_art_blit(&stru_5FC3B0);
}
