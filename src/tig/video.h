#ifndef ARCANUM_TIG_VIDEO_H_
#define ARCANUM_TIG_VIDEO_H_

#include "tig/types.h"

#include <ddraw.h>

typedef enum TigVideoBufferSpecFlags {
    TIG_VIDEO_BUFFER_SPEC_TRANSPARENCY_ENABLED = 0x01,
    TIG_VIDEO_BUFFER_SPEC_VIDEO_MEMORY = 0x02,
    TIG_VIDEO_BUFFER_SPEC_SYSTEM_MEMORY = 0x04,
    TIG_VIDEO_BUFFER_SPEC_FLUSH_ENABLED = 0x08,
    TIG_VIDEO_BUFFER_SPEC_BACKGROUND_COLOR_ENABLED = 0x10,
};

typedef enum TigVideoBufferFlags {
    // Specifies that the video buffer is currently locked.
    TIG_VIDEO_BUFFER_LOCKED = 0x01,
    TIG_VIDEO_BUFFER_HAVE_TRANSPARENCY = 0x02,
    TIG_VIDEO_BUFFER_VIDEO_MEMORY = 0x04,
    TIG_VIDEO_BUFFER_FLAG_0x08 = 0x08,
    TIG_VIDEO_BUFFER_CAN_FLUSH = 0x10,
    TIG_VIDEO_BUFFER_HAVE_BACKGROUND_COLOR = 0x20,
};

// Opaque handle.
typedef struct TigVideoBuffer;

typedef struct TigVideoBufferSpec {
    unsigned int flags;
    int width;
    int height;
    int background_color;
    int color_key;
};

static_assert(sizeof(TigVideoBufferSpec) == 0x14, "wrong size");

typedef struct TigVideoBufferData {
    unsigned int flags;
    int width;
    int height;
    int pitch;
    int background_color;
    int color_key;
    int bpp;
    union {
        void* surface_data;
        uint8_t* surface_data_8bpp;
        uint16_t* surface_data_16bpp;
        uint32_t* surface_data_32bpp;
    };
};

static_assert(sizeof(TigVideoBufferData) == 0x1C, "wrong size");

typedef struct TigVideoBufferBlitSpec {
    int field_0;
    TigVideoBuffer* src_video_buffer;
    TigRect* src_rect;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    TigVideoBuffer* dst_video_buffer;
    TigRect* dst_rect;
};

static_assert(sizeof(TigVideoBufferBlitSpec) == 0x2C, "wrong size");

int tig_video_init(TigContext* ctx);
void tig_video_exit();
int tig_video_handle(HWND* hWnd);
int tig_video_main_surface_get(LPDIRECTDRAWSURFACE7* surface_ptr);
void tig_video_set_client_rect(LPRECT rect);
void tig_video_display_fps();
int tig_video_main_surface_lock(void** surface_data_ptr);
int tig_video_main_surface_unlock();
int tig_video_blit(TigVideoBuffer* src_video_buffer, TigRect* src_rect, TigRect* dst_rect, bool to_primary_surface);
int tig_video_fill(TigRect* rect, int color);
int sub_51F860();
int tig_video_get_video_memory_status(size_t* total, size_t* available);
int tig_video_buffer_create(TigVideoBufferSpec* video_buffer_spec, TigVideoBuffer** video_buffer);
int tig_video_buffer_destroy(TigVideoBuffer* video_buffer);
int tig_video_buffer_data(TigVideoBuffer* video_buffer, TigVideoBufferData* video_buffer_data);
int tig_video_buffer_lock(TigVideoBuffer* video_buffer);
int tig_video_buffer_unlock(TigVideoBuffer* video_buffer);
int tig_video_buffer_fill(TigVideoBuffer* video_buffer, TigRect* rect, int color);
int tig_video_buffer_line(TigVideoBuffer* video_buffer);
int tig_video_buffer_blit(TigVideoBufferBlitSpec* blit_spec);
int tig_video_buffer_get_pixel_color(TigVideoBuffer* video_buffer, int x, int y, int* color);

#endif /* ARCANUM_TIG_VIDEO_H_ */
