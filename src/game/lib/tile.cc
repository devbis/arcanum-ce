#include "game/lib/tile.h"

#include "game/lib/gamelib.h"
#include "tig/art.h"
#include "tig/debug.h"
#include "tig/video.h"
#include "tig/window.h"

#define TILE_CACHE_CAPACITY 64

typedef struct TileCacheEntry {
    unsigned int art_id;
    TigVideoBuffer* video_buffer;
    unsigned int time;
};

static_assert(sizeof(TileCacheEntry) == 0xC, "wrong size");

// 0x602AE0
static TileCacheEntry stru_602AE0[TILE_CACHE_CAPACITY];

// 0x602DE0
static TigVideoBuffer* dword_602DE0;

// 0x602DE4
static uint8_t* dword_602DE4;

// 0x602DE8
static uint8_t* dword_602DE8;

// 0x602DEC
static GameContextF8* dword_602DEC;

// 0x602DF0
static TigVideoBuffer* dword_602DF0;

// 0x602DF4
static bool dword_602DF4;

// 0x602DF8
static ViewOptions tile_view_options;

// 0x602E00
static bool dword_602E00;

// 0x602E04
static int dword_602E04;

// 0x602E08
static bool dword_602E08;

// 0x4D6840
bool tile_init(GameContext* ctx)
{
    if (tig_window_vbid_get(ctx->iso_window_handle, &dword_602DF0) != TIG_OK) {
        return false;
    }

    dword_602E00 = tig_video_3d_check_initialized() == TIG_OK;
    dword_602E04 = ctx->iso_window_handle;
    dword_602DEC = ctx->field_8;
    tile_view_options.type = VIEW_TYPE_ISOMETRIC;
    dword_602DF4 = true;

    return true;
}

// 0x4D68A0
void tile_exit()
{
    sub_4D7980();
    dword_602E04 = -1;
    dword_602DEC = 0;
}

// 0x4D68C0
void tile_resize(ResizeContext* ctx)
{
    if (tig_window_vbid_get(ctx->iso_window_handle, &dword_602DF0) != TIG_OK) {
        tig_debug_printf("tile_resize: ERROR: couldn't grab window vbid!");
        exit(EXIT_FAILURE);
    }

    dword_602E04 = ctx->iso_window_handle;
}

// 0x4D6900
bool tile_update_view(ViewOptions* view_options)
{
    sub_4D79C0(view_options);
    tile_view_options = *view_options;
    return true;
}

// 0x4D6930
void sub_4D6930()
{
    dword_602DF4 = 1 - dword_602DF4;
}

// 0x4D7090
int sub_4D7090(int a1, int a2)
{
    return (a1 & 0x3F) | ((a2 & 0x3F) << 6);
}

// 0x4D7590
void sub_4D7590(unsigned int art_id, TigVideoBuffer* video_buffer)
{
    if (tile_view_options.type == VIEW_TYPE_TOP_DOWN) {
        TigRect rect = { 0, 0, 80, 40 };

        TigArtBlitSpec art_blit_spec;
        art_blit_spec.field_0 = 4;
        art_blit_spec.src_art_id = art_id;
        art_blit_spec.src_rect = &rect;
        art_blit_spec.dst_video_buffer = dword_602DE0;
        art_blit_spec.dst_rect = &rect;
        tig_art_blit(&art_blit_spec);

        TigVideoBufferData src_video_buffer_data;
        tig_video_buffer_lock(dword_602DE0);
        tig_video_buffer_data(dword_602DE0, &src_video_buffer_data);

        TigVideoBufferData dst_video_buffer_data;
        tig_video_buffer_lock(video_buffer);
        tig_video_buffer_data(video_buffer, &dst_video_buffer_data);

        switch (src_video_buffer_data.bpp) {
        case 8:
            for (int y = 0; y < tile_view_options.zoom; y++) {
                for (int x = 0; x < tile_view_options.zoom; x++) {
                    int index = y * tile_view_options.zoom + x;
                    int src_index = dword_602DE4[index] * src_video_buffer_data.pitch + dword_602DE8[index];
                    int dst_index = y * dst_video_buffer_data.pitch + x;
                    dst_video_buffer_data.surface_data_8bpp[dst_index] = src_video_buffer_data.surface_data_8bpp[src_index];
                }
            }
            break;
        case 16:
            for (int y = 0; y < tile_view_options.zoom; y++) {
                for (int x = 0; x < tile_view_options.zoom; x++) {
                    int index = y * tile_view_options.zoom + x;
                    int src_index = dword_602DE4[index] * src_video_buffer_data.pitch / 2 + dword_602DE8[index];
                    int dst_index = y * dst_video_buffer_data.pitch / 2 + x;
                    dst_video_buffer_data.surface_data_16bpp[dst_index] = src_video_buffer_data.surface_data_16bpp[src_index];
                }
            }
            break;
        case 24:
            // TODO: Same implementation as in 32bpp, check.
            for (int y = 0; y < tile_view_options.zoom; y++) {
                for (int x = 0; x < tile_view_options.zoom; x++) {
                    int index = y * tile_view_options.zoom + x;
                    int src_index = dword_602DE4[index] * src_video_buffer_data.pitch / 4 + dword_602DE8[index];
                    int dst_index = y * dst_video_buffer_data.pitch / 4 + x;
                    dst_video_buffer_data.surface_data_32bpp[dst_index] = src_video_buffer_data.surface_data_32bpp[src_index];
                }
            }
            break;
        case 32:
            for (int y = 0; y < tile_view_options.zoom; y++) {
                for (int x = 0; x < tile_view_options.zoom; x++) {
                    int index = y * tile_view_options.zoom + x;
                    int src_index = dword_602DE4[index] * src_video_buffer_data.pitch / 4 + dword_602DE8[index];
                    int dst_index = y * dst_video_buffer_data.pitch / 4 + x;
                    dst_video_buffer_data.surface_data_32bpp[dst_index] = src_video_buffer_data.surface_data_32bpp[src_index];
                }
            }
            break;
        }

        tig_video_buffer_unlock(video_buffer);
        tig_video_buffer_unlock(dword_602DE0);
    }
}

// 0x4D7980
void sub_4D7980()
{
    if (dword_602DE0 != NULL) {
        tig_video_buffer_destroy(dword_602DE0);
        dword_602DE0 = NULL;
    }

    sub_4D7C70();
    sub_4D7A90();

    dword_602E08 = false;
}

// 0x4D79C0
void sub_4D79C0(ViewOptions* view_options)
{
    if (view_options->type == VIEW_TYPE_TOP_DOWN) {
        if (view_options->zoom != tile_view_options.zoom) {
            if (!dword_602E08) {
                sub_4D7A00();
            }

            sub_4D7A40(view_options->zoom);
            sub_4D7AC0(view_options->zoom);
        }
    }
}

// 0x4D7A00
void sub_4D7A00()
{
    TigVideoBufferCreateInfo vb_create_info;
    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY;
    vb_create_info.width = 80;
    vb_create_info.height = 40;
    vb_create_info.background_color = 0;
    tig_video_buffer_create(&vb_create_info, &dword_602DE0);

    dword_602E08 = true;
}

// 0x4D7A40
void sub_4D7A40(int zoom)
{
    sub_4D7A90();

    TigVideoBufferCreateInfo vb_create_info;
    vb_create_info.flags = TIG_VIDEO_BUFFER_CREATE_SYSTEM_MEMORY;
    vb_create_info.width = zoom;
    vb_create_info.height = zoom;
    vb_create_info.background_color = 0;

    for (int index = 0; index < TILE_CACHE_CAPACITY; index++) {
        stru_602AE0[index].art_id = -1;
        tig_video_buffer_create(&vb_create_info, &(stru_602AE0[index].video_buffer));
    }
}

// 0x4D7A90
void sub_4D7A90()
{
    for (int index = 0; index < TILE_CACHE_CAPACITY; index++) {
        if (stru_602AE0[index].video_buffer != NULL) {
            tig_video_buffer_destroy(stru_602AE0[index].video_buffer);
        }

        stru_602AE0[index].video_buffer = NULL;
        stru_602AE0[index].art_id = -1;
    }
}

// 0x4D7AC0
void sub_4D7AC0(int zoom)
{
    sub_4D7C70();

    dword_602DE8 = (uint8_t*)malloc(sizeof(*dword_602DE8) * (zoom * zoom));
    dword_602DE4 = (uint8_t*)malloc(sizeof(*dword_602DE4) * (zoom * zoom));

    // TODO: Unclear, check in debugger.

    double scale;
    if (zoom < 8) {
        scale = 0.5;
    } else if (zoom < 16) {
        scale = 0.65;
    } else if (zoom < 24) {
        scale = 0.7;
    } else if (zoom < 32) {
        scale = 0.8;
    } else if (zoom < 40) {
        scale = 0.9;
    } else if (zoom < 48) {
        scale = 0.91;
    } else if (zoom < 56) {
        scale = 0.92;
    } else if (zoom < 60) {
        scale = 0.93;
    } else if (zoom < 62) {
        scale = 0.94;
    } else {
        scale = 0.95;
    }

    for (int y = 0; y < zoom; y++) {
        for (int x = 0; x < zoom; x++) {
            // TODO: Unclear, check in debugger.
        }
    }
}

// 0x4D7C70
void sub_4D7C70()
{
    if (dword_602DE8 != NULL) {
        free(dword_602DE8);
        dword_602DE8 = NULL;
    }

    if (dword_602DE4 != NULL) {
        free(dword_602DE4);
        dword_602DE4 = NULL;
    }
}

// 0x4D7E90
TigVideoBuffer* sub_4D7E90(unsigned int art_id)
{
    int candidate = -1;
    int found = -1;

    for (int index = 0; index < TILE_CACHE_CAPACITY; index++) {
        if (stru_602AE0[index].art_id == -1) {
            found = index;
        } else {
            if (stru_602AE0[index].art_id == art_id) {
                stru_602AE0[index].time = gamelib_ping_time;
                return stru_602AE0[index].video_buffer;
            }

            if (candidate == -1) {
                candidate = index;
            } else {
                if (stru_602AE0[index].time < stru_602AE0[candidate].time) {
                    candidate = index;
                }
            }
        }
    }

    if (found == -1) {
        found = candidate;
    }

    sub_4D7590(art_id, stru_602AE0[found].video_buffer);
    stru_602AE0[found].art_id = art_id;
    stru_602AE0[found].time = gamelib_ping_time;
    return stru_602AE0[found].video_buffer;
}
