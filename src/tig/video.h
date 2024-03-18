#ifndef ARCANUM_TIG_VIDEO_H_
#define ARCANUM_TIG_VIDEO_H_

#include <stdint.h>

#define DIRECTDRAW_VERSION 0x0700
#include <ddraw.h>

#include "tig/rect.h"
#include "tig/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TigVideoBufferSpecFlags {
    TIG_VIDEO_BUFFER_SPEC_TRANSPARENCY_ENABLED = 0x01,
    TIG_VIDEO_BUFFER_SPEC_VIDEO_MEMORY = 0x02,
    TIG_VIDEO_BUFFER_SPEC_SYSTEM_MEMORY = 0x04,
    TIG_VIDEO_BUFFER_SPEC_FLUSH_ENABLED = 0x08,
    TIG_VIDEO_BUFFER_SPEC_BACKGROUND_COLOR_ENABLED = 0x10,
} TigVideoBufferSpecFlags;

typedef enum TigVideoBufferFlags {
    // Specifies that the video buffer is currently locked.
    TIG_VIDEO_BUFFER_LOCKED = 0x01,
    TIG_VIDEO_BUFFER_HAVE_TRANSPARENCY = 0x02,
    TIG_VIDEO_BUFFER_VIDEO_MEMORY = 0x04,
    TIG_VIDEO_BUFFER_FLAG_0x08 = 0x08,
    TIG_VIDEO_BUFFER_CAN_FLUSH = 0x10,
    TIG_VIDEO_BUFFER_HAVE_BACKGROUND_COLOR = 0x20,
} TigVideoBufferFlags;

typedef enum TigVideoBufferBlitSpecFlags {
    TIG_VIDEO_BUFFER_BLIT_MIRRORLEFTRIGHT = 0x01,
    TIG_VIDEO_BUFFER_BLIT_MIRRORUPDOWN = 0x02,
    TIG_VIDEO_BUFFER_BLIT_0x04 = 0x04,
    TIG_VIDEO_BUFFER_BLIT_0x10 = 0x10,
    TIG_VIDEO_BUFFER_BLIT_0x40 = 0x40,
    TIG_VIDEO_BUFFER_BLIT_0x100 = 0x100,
    TIG_VIDEO_BUFFER_BLIT_0x200 = 0x200,
    TIG_VIDEO_BUFFER_BLIT_0x400 = 0x400,
    TIG_VIDEO_BUFFER_BLIT_MIRRORANY = TIG_VIDEO_BUFFER_BLIT_MIRRORLEFTRIGHT | TIG_VIDEO_BUFFER_BLIT_MIRRORUPDOWN,
} TigVideoBufferBlitSpecFlags;

// Opaque handle.
typedef struct TigVideoBuffer TigVideoBuffer;

typedef struct TigVideoBufferSpec {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int width;
    /* 0008 */ int height;
    /* 000C */ int background_color;
    /* 0010 */ int color_key;
} TigVideoBufferSpec;

static_assert(sizeof(TigVideoBufferSpec) == 0x14, "wrong size");

typedef struct TigVideoBufferData {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int width;
    /* 0008 */ int height;
    /* 000C */ int pitch;
    /* 0010 */ int background_color;
    /* 0014 */ int color_key;
    /* 0018 */ int bpp;
    /* 001C */ union {
        void* pixels;
        uint8_t* p8;
        uint16_t* p16;
        uint32_t* p32;
    } surface_data;
} TigVideoBufferData;

static_assert(sizeof(TigVideoBufferData) == 0x20, "wrong size");

typedef struct TigVideoBufferBlitSpec {
    unsigned int flags;
    TigVideoBuffer* src_video_buffer;
    TigRect* src_rect;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    TigRect* field_20;
    TigVideoBuffer* dst_video_buffer;
    TigRect* dst_rect;
} TigVideoBufferBlitSpec;

static_assert(sizeof(TigVideoBufferBlitSpec) == 0x2C, "wrong size");

typedef struct TigVideoScreenshotSettings {
    /* 0000 */ int key;
    /* 0004 */ int field_4;
} TigVideoScreenshotSettings;

int tig_video_init(TigContext* ctx);
void tig_video_exit();
int tig_video_platform_window_get(HWND* wnd_ptr);
int tig_video_instance_get(HINSTANCE* instance_ptr);
int tig_video_main_surface_get(LPDIRECTDRAWSURFACE7* surface_ptr);
void tig_video_set_client_rect(LPRECT rect);
void tig_video_display_fps();
int tig_video_main_surface_lock(void** surface_data_ptr);
int tig_video_main_surface_unlock();
int tig_video_blit(TigVideoBuffer* src_video_buffer, TigRect* src_rect, TigRect* dst_rect, bool to_primary_surface);
int tig_video_fill(TigRect* rect, int color);
int sub_51F860();
int sub_51F880();
int tig_video_flip();
int tig_video_screenshot_set_settings(TigVideoScreenshotSettings* settings);
int tig_video_screenshot_make();
int sub_51FA40(TigRect* rect);
int tig_video_get_bpp(int* bpp);
int tig_video_get_palette(int* colors);
int tig_video_get_pitch(int* pitch);
int tig_video_3d_check_initialized();
int tig_video_3d_check_hardware();
int tig_video_3d_begin_scene();
int tig_video_3d_end_scene();
int tig_video_get_video_memory_status(size_t* total, size_t* available);
int tig_video_check_gamma_control();
int tig_video_fade(int color, int steps, float duration, unsigned int flags);
int tig_video_set_gamma(float gamma);
int tig_video_buffer_create(TigVideoBufferSpec* video_buffer_spec, TigVideoBuffer** video_buffer);
int tig_video_buffer_destroy(TigVideoBuffer* video_buffer);
int tig_video_buffer_data(TigVideoBuffer* video_buffer, TigVideoBufferData* video_buffer_data);
int tig_video_buffer_set_color_key(TigVideoBuffer* video_buffer, int color_key);
int tig_video_buffer_lock(TigVideoBuffer* video_buffer);
int tig_video_buffer_unlock(TigVideoBuffer* video_buffer);
int tig_video_buffer_outline(TigVideoBuffer* video_buffer, TigRect* rect, int color);
int tig_video_buffer_fill(TigVideoBuffer* video_buffer, TigRect* rect, int color);
int tig_video_buffer_line(TigVideoBuffer* video_buffer, TigRect* rect, int a3, unsigned int color);
int tig_video_buffer_blit(TigVideoBufferBlitSpec* blit_spec);
int tig_video_buffer_get_pixel_color(TigVideoBuffer* video_buffer, int x, int y, unsigned int* color);

#ifdef __cplusplus
}
#endif

#endif /* ARCANUM_TIG_VIDEO_H_ */
