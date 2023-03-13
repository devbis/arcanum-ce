#include "tig/video.h"

#include <d3d.h>
#include <stdio.h>

#include "tig/art.h"
#include "tig/color.h"
#include "tig/debug.h"
#include "tig/message.h"
#include "tig/mouse.h"
#include "tig/timer.h"
#include "tig/window.h"

typedef struct TigVideoBuffer {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int x;
    /* 0008 */ int y;
    /* 000C */ int width;
    /* 0010 */ int height;
    /* 0014 */ int texture_width;
    /* 0018 */ int texture_height;
    /* 001C */ int pitch;
    /* 0020 */ int background_color;
    /* 0024 */ int color_key;
    /* 0028 */ LPDIRECTDRAWSURFACE7 surface;
    /* 002C */ DDSURFACEDESC2 surface_desc;
    /* 00A8 */ union {
        void* surface_data;
        uint8_t* surface_data_8bpp;
        uint16_t* surface_data_16bpp;
        uint32_t* surface_data_32bpp;
    };
    /* 00AC */ int lock_count;
};

static_assert(sizeof(TigVideoBuffer) == 0xB0, "wrong size");

typedef struct TigVideoState {
    /* 0000 */ HINSTANCE instance;
    /* 0004 */ HWND wnd;
    /* 0008 */ LPDIRECTDRAW7 ddraw;
    /* 000C */ LPDIRECTDRAWSURFACE7 primary_surface;
    /* 0010 */ LPDIRECTDRAWSURFACE7 offscreen_surface;
    /* 0014 */ LPDIRECTDRAWSURFACE7 main_surface;
    /* 0018 */ LPDIRECTDRAWPALETTE palette;
    /* 001C */ LPDIRECT3D7 d3d;
    /* 0020 */ LPDIRECT3DDEVICE7 d3d_device;
    /* 0024 */ DDSURFACEDESC2 current_surface_desc;
    /* 00A0 */ LPDIRECTDRAWGAMMACONTROL gamma_control;
    /* 00A4 */ bool have_gamma_control;
    /* 00A8 */ DDGAMMARAMP gamma_ramp;
    // TODO: Better name.
    /* 06A8 */ DDGAMMARAMP gamma_ramp2;
};

static_assert(sizeof(TigVideoState) == 0x110C, "wrong size");

// 0x5BF3D8
static int dword_5BF3D8 = -1;

// 0x60F250
static TigVideoState tig_video_state;

// 0x60FEF8
static float tig_video_gamma;

// 0x60FEFC
static RECT tig_video_client_rect;

// 0x61030C
static int tig_video_bpp;

// 0x610310
static bool tig_video_fullscreen;

// 0x610314
static bool tig_video_double_buffered;

// 0x610318
static int dword_610318;

// 0x61031C
static bool tig_video_3d_initialized;

// 0x610320
static bool tig_video_3d_is_hardware;

// 0x610324
static bool tig_video_3d_texture_must_power_of_two;

// 0x610328
static bool tig_video_3d_texture_must_be_square;

// 0x61032C
static int tig_video_3d_extra_surface_caps;

// 0x610330
static int tig_video_3d_extra_surface_caps2;

// 0x610334
static TigVideoBuffer* tig_video_3d_viewport;

// 0x610338
static DDPIXELFORMAT tig_video_3d_pixel_format;

// 0x610388
static TigRect stru_610388;

// 0x610398
static bool tig_video_main_surface_locked;

// 0x61039C
static bool tig_video_initialized;

// 0x6103A0
static bool tig_video_show_fps;

// 0x6103A4
static int dword_6103A4;

// 0x51F330
int tig_video_init(TigContext* ctx)
{
    memset(&tig_video_state, 0, sizeof(tig_video_state));

    if ((ctx->flags & TIG_CONTEXT_FLAG_0x20) != 0) {
        if (ctx->default_window_proc == NULL) {
            return TIG_ERR_12;
        }

        tig_message_set_default_window_proc(ctx->default_window_proc);
    }

    if (!tig_video_platform_window_init(ctx)) {
        return TIG_ERR_16;
    }

    if (!tig_video_ddraw_init(ctx)) {
        tig_video_platform_window_exit();
        return TIG_ERR_16;
    }

    tig_video_show_fps = (ctx->flags & TIG_CONTEXT_FLAG_FPS) != 0;
    dword_610318 = (ctx->flags & TIG_CONTEXT_FLAG_0x04) != 0;
    tig_video_bpp = ctx->bpp;
    tig_video_main_surface_locked = false;

    ShowCursor((ctx->flags & TIG_CONTEXT_FLAG_0x20) != 0 ? TRUE : FALSE);

    dword_6103A4 = 0;
    tig_video_initialized = true;
    dword_5BF3D8 = -1;

    return TIG_OK;
}

// 0x51F3F0
void tig_video_exit()
{
    tig_video_ddraw_exit();
    tig_video_platform_window_exit();
    tig_video_initialized = false;
}

// 0x51F410
int tig_video_handle(HWND* hWnd)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    *hWnd = tig_video_state.wnd;

    return TIG_OK;
}

// 0x51F450
int tig_video_main_surface_get(LPDIRECTDRAWSURFACE7* surface_ptr)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    *surface_ptr = tig_video_state.main_surface;

    return TIG_OK;
}

// 0x51F470
void tig_video_set_client_rect(LPRECT rect)
{
    tig_video_client_rect = *rect;
}

// 0x51F4A0
void tig_video_display_fps()
{
    // 0x60F248
    static unsigned int curr_time;

    // 0x61035C
    static char fps_string_buffer[32];

    // 0x61037C
    static float fps;

    // 0x610380
    static unsigned int prev_time;

    // 0x610384
    static unsigned int counter;

    if (tig_video_show_fps) {
        if (counter >= 10) {
            tig_timer_start(&curr_time);
            unsigned int elapsed = tig_timer_between(prev_time, curr_time);
            fps = (double)counter / ((double)elapsed / 1000.0);
            sprintf(fps_string_buffer, "%10.0f", fps);
            prev_time = curr_time;
            counter = 0;
        }

        HDC hdc;
        IDirectDrawSurface7_GetDC(tig_video_state.main_surface, &hdc);
        TextOutA(hdc, 0, 0, fps_string_buffer, strlen(fps_string_buffer));
        IDirectDrawSurface7_ReleaseDC(tig_video_state.main_surface, hdc);
    }
}

// 0x51F590
int tig_video_main_surface_lock(void** surface_data_ptr)
{
    if (tig_video_main_surface_locked) {
        return TIG_ERR_6;
    }

    if (!tig_video_surface_lock(tig_video_state.main_surface, &(tig_video_state.current_surface_desc), surface_data_ptr)) {
        return TIG_ERR_7;
    }

    tig_video_main_surface_locked = true;

    return TIG_OK;
}

// 0x51F5D0
int tig_video_main_surface_unlock()
{
    if (tig_video_main_surface_locked) {
        tig_video_surface_unlock(tig_video_state.main_surface, &(tig_video_state.current_surface_desc));
        tig_video_main_surface_locked = false;
    }

    return TIG_OK;
}

// 0x51F600
int tig_video_blit(TigVideoBuffer* src_video_buffer, TigRect* src_rect, TigRect* dst_rect, bool to_primary_surface)
{
    HRESULT hr;

    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    TigRect clamped_dst_rect;
    int rc = tig_rect_intersection(dst_rect, &stru_610388, &clamped_dst_rect);
    if (rc != TIG_OK) {
        return rc;
    }

    LPDIRECTDRAWSURFACE7 dst_surface = to_primary_surface
        ? tig_video_state.primary_surface
        : tig_video_state.main_surface;

    RECT native_src_rect;
    native_src_rect.left = clamped_dst_rect.x - dst_rect->x + src_rect->x;
    native_src_rect.top = clamped_dst_rect.y - dst_rect->y + src_rect->y;
    native_src_rect.right = native_src_rect.left + clamped_dst_rect.width - dst_rect->width + src_rect->width;
    native_src_rect.bottom = native_src_rect.top + clamped_dst_rect.height - dst_rect->height + src_rect->height;

    if (tig_video_fullscreen) {
        DWORD flags = DDBLTFAST_WAIT;
        if ((src_video_buffer->flags & TIG_VIDEO_BUFFER_HAVE_TRANSPARENCY) != 0) {
            flags |= DDBLTFAST_SRCCOLORKEY;
        }

        hr = IDirectDrawSurface7_BltFast(dst_surface,
            clamped_dst_rect.x,
            clamped_dst_rect.y,
            src_video_buffer->surface,
            &native_src_rect,
            flags);
        if (hr == DDERR_SURFACELOST) {
            IDirectDrawSurface7_Restore(dst_surface);
            hr = IDirectDrawSurface7_BltFast(dst_surface,
                clamped_dst_rect.x,
                clamped_dst_rect.y,
                src_video_buffer->surface,
                &native_src_rect,
                flags);
        }
    } else {
        RECT native_dst_rect;
        native_dst_rect.left = tig_video_client_rect.left + clamped_dst_rect.x;
        native_dst_rect.top = tig_video_client_rect.top + clamped_dst_rect.y;
        native_dst_rect.right = native_dst_rect.left + clamped_dst_rect.width;
        native_dst_rect.bottom = native_dst_rect.top + clamped_dst_rect.height;

        DWORD flags = DDBLT_WAIT;
        if ((src_video_buffer->flags & TIG_VIDEO_BUFFER_HAVE_TRANSPARENCY) != 0) {
            flags |= DDBLT_KEYSRC;
        }

        hr = IDirectDrawSurface7_Blt(dst_surface,
            &native_dst_rect,
            src_video_buffer->surface,
            &native_src_rect,
            flags,
            NULL);
        if (hr == DDERR_SURFACELOST) {
            IDirectDrawSurface7_Restore(dst_surface);
            hr = IDirectDrawSurface7_Blt(dst_surface,
                &native_dst_rect,
                src_video_buffer->surface,
                &native_src_rect,
                flags,
                NULL);
        }
    }

    if (FAILED(hr)) {
        tig_debug_printf("DirectX error %d in tig_video_blit()\n", hr);
        return TIG_ERR_11;
    }

    return TIG_OK;
}

// 0x51F7C0
int tig_video_fill(TigRect* rect, int color)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    int rc;
    TigRect clamped_rect;
    if (rect != NULL) {
        rc = tig_rect_intersection(rect, &stru_610388, &clamped_rect);
    } else {
        rc = tig_rect_intersection(&stru_610388, &stru_610388, &clamped_rect);
    }

    if (rc != TIG_OK) {
        return rc;
    }

    if (!tig_video_fullscreen) {
        clamped_rect.x += tig_video_client_rect.left;
        clamped_rect.y += tig_video_client_rect.top;
    }

    if (tig_video_surface_blt(tig_video_state.main_surface, &clamped_rect, color) != TIG_OK) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x51F860
int sub_51F860()
{
    // TODO: Unclear.
    HRESULT hr = IDirectDrawSurface7_IsLost(tig_video_state.main_surface);
    if (FAILED(hr)) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x51F880
int sub_51F880()
{
    HRESULT hr;

    if (sub_51F860() == TIG_OK) {
        return TIG_OK;
    }

    hr = IDirectDraw7_TestCooperativeLevel(tig_video_state.ddraw);
    if (FAILED(hr)) {
        return TIG_ERR_16;
    }

    hr = IDirectDraw7_RestoreAllSurfaces(tig_video_state.ddraw);
    if (FAILED(hr)) {
        tig_video_print_dd_result(hr);
        return TIG_ERR_16;
    }

    sub_4FFB40();
    tig_window_set_needs_display_in_rect(NULL);

    TigMessage message;
    tig_timer_start(&(message.time));
    message.type = TIG_MESSAGE_TYPE_8;
    return tig_message_enqueue(&message);
}

// 0x51F8F0
int tig_video_flip()
{
    HRESULT hr;

    if (tig_video_double_buffered) {
        if (tig_video_fullscreen) {
            hr = IDirectDrawSurface7_Flip(tig_video_state.primary_surface, NULL, 1);
            if (FAILED(hr)) {
                tig_video_print_dd_result(hr);
                return TIG_ERR_16;
            }

            RECT src_rect;
            src_rect.left = stru_610388.x;
            src_rect.top = stru_610388.y;
            src_rect.right = stru_610388.x + stru_610388.width;
            src_rect.bottom = stru_610388.y + stru_610388.height;

            hr = IDirectDrawSurface7_BltFast(tig_video_state.offscreen_surface,
                0,
                0,
                tig_video_state.primary_surface,
                &src_rect,
                DDBLTFAST_WAIT);
            if (FAILED(hr)) {
                tig_video_print_dd_result(hr);
                return TIG_ERR_16;
            }
        } else {
            RECT src_rect;
            src_rect.left = stru_610388.x;
            src_rect.top = stru_610388.y;
            src_rect.right = stru_610388.x + stru_610388.width;
            src_rect.bottom = stru_610388.y + stru_610388.height;

            hr = IDirectDrawSurface7_Blt(tig_video_state.primary_surface,
                &tig_video_client_rect,
                tig_video_state.offscreen_surface,
                &src_rect,
                DDBLT_WAIT,
                NULL);
            if (FAILED(hr)) {
                tig_video_print_dd_result(hr);
                return TIG_ERR_16;
            }
        }
    }

    return TIG_OK;
}

// 0x51FBF0
int tig_video_get_video_memory_status(size_t* total_memory, size_t* available_memory)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    DDSCAPS2 ddscaps2;
    ddscaps2.dwCaps = tig_video_3d_initialized
        ? DDSCAPS_TEXTURE
        : DDSCAPS_LOCALVIDMEM | DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
    ddscaps2.dwCaps2 = 0;
    ddscaps2.dwCaps3 = 0;
    ddscaps2.dwCaps4 = 0;

    DWORD total;
    DWORD available;
    HRESULT hr = IDirectDraw7_GetAvailableVidMem(tig_video_state.ddraw, &ddscaps2, &total, &available);
    if (FAILED(hr)) {
        tig_video_print_dd_result(hr);
        tig_debug_printf("Error determining approx. available surface memory.\n");
        return TIG_ERR_5;
    }

    *total_memory = (size_t)total;
    *available_memory = (size_t)available;

    return TIG_OK;
}

// 0x5200F0
int tig_video_buffer_create(TigVideoBufferSpec* video_buffer_spec, TigVideoBuffer** video_buffer_ptr)
{
    TigVideoBuffer* video_buffer = (TigVideoBuffer*)malloc(sizeof(*video_buffer));
    memset(video_buffer, 0, sizeof(*video_buffer));
    *video_buffer_ptr = video_buffer;

    unsigned int caps = DDSCAPS_OFFSCREENPLAIN;

    if (dword_610318) {
        if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_VIDEO_MEMORY) != 0) {
            caps |= DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;
        } else if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_SYSTEM_MEMORY) != 0) {
            caps |= DDSCAPS_SYSTEMMEMORY;
        }
    } else {
        caps |= DDSCAPS_SYSTEMMEMORY;
    }

    if (tig_video_3d_initialized) {
        if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_FLUSH_ENABLED) != 0) {
            caps |= DDSCAPS_3DDEVICE;
        }

        if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_BACKGROUND_COLOR_ENABLED) != 0) {
            caps &= ~DDSCAPS_OFFSCREENPLAIN;
            caps |= DDSCAPS_TEXTURE;
        }
    }

    int texture_width = video_buffer_spec->width;
    int texture_height = video_buffer_spec->height;

    if ((caps & DDSCAPS_TEXTURE) != 0) {
        if (tig_video_3d_texture_must_power_of_two) {
            texture_width = 1;
            while (texture_width < video_buffer_spec->width) {
                texture_width *= 2;
            }

            texture_height = 1;
            while (texture_height < video_buffer_spec->height) {
                texture_height *= 2;
            }
        }

        if (tig_video_3d_texture_must_be_square) {
            if (texture_height < texture_width) {
                texture_height = texture_width;
            } else {
                texture_width = texture_height;
            }
        }
    }

    if (!tig_video_surface_create(tig_video_state.ddraw, texture_width, texture_height, caps, &(video_buffer->surface))) {
        if ((caps & DDSCAPS_VIDEOMEMORY) == 0) {
            tig_debug_printf("*Request to allocate surface (?] memory failed (%dx%d) [%d].\n",
                texture_width,
                texture_height,
                caps);
            free(video_buffer);
            return TIG_ERR_5;
        }

        caps &= ~DDSCAPS_LOCALVIDMEM;
        caps |= DDSCAPS_NONLOCALVIDMEM;
        if (!tig_video_surface_create(tig_video_state.ddraw, texture_width, texture_height, caps, &(video_buffer->surface))) {
            if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_FLUSH_ENABLED) != 0) {
                tig_debug_printf("tig_video_buffer_create: Error trying to create surface for 3D, flushing...\n");
                tig_art_flush();

                if (!tig_video_surface_create(tig_video_state.ddraw, texture_width, texture_height, caps, &(video_buffer->surface))) {
                    tig_debug_printf("tig_video_buffer_create: Error, flushing FAILED to fix problem!\n");

                    caps &= ~(DDSCAPS_NONLOCALVIDMEM | DDSCAPS_VIDEOMEMORY);
                    caps |= DDSCAPS_SYSTEMMEMORY;
                    tig_debug_printf("*Request to allocate surface in video memory failed.\n");
                    if (!tig_video_surface_create(tig_video_state.ddraw, texture_width, texture_height, caps, &(video_buffer->surface))) {
                        tig_debug_printf(
                            "*Request to allocate surface in SYSTEM memory failed (%dx%d) [%d].\n",
                            texture_width,
                            texture_height,
                            caps);
                        free(video_buffer);
                        return TIG_ERR_5;
                    }
                }
            } else {
                caps &= ~(DDSCAPS_NONLOCALVIDMEM | DDSCAPS_VIDEOMEMORY);
                caps |= DDSCAPS_SYSTEMMEMORY;
                tig_debug_printf("*Request to allocate surface in video memory failed.\n");
                if (!tig_video_surface_create(tig_video_state.ddraw, texture_width, texture_height, caps, &(video_buffer->surface))) {
                    tig_debug_printf(
                        "*Request to allocate surface in SYSTEM memory failed (%dx%d) [%d].\n",
                        texture_width,
                        texture_height,
                        caps);
                    free(video_buffer);
                    return TIG_ERR_5;
                }
            }
        }
    }

    if ((caps & DDSCAPS_VIDEOMEMORY) != 0) {
        video_buffer->flags |= TIG_VIDEO_BUFFER_VIDEO_MEMORY;
    } else if ((caps & (DDSCAPS_TEXTURE | DDSCAPS_3DDEVICE)) == 0) {
        video_buffer->flags |= TIG_VIDEO_BUFFER_FLAG_0x08;
    }

    if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_TRANSPARENCY_ENABLED) != 0) {
        video_buffer->flags |= TIG_VIDEO_BUFFER_HAVE_TRANSPARENCY;
        tig_video_buffer_set_color_key(*video_buffer_ptr, video_buffer_spec->color_key);
    }

    if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_FLUSH_ENABLED) != 0) {
        video_buffer->flags |= TIG_VIDEO_BUFFER_CAN_FLUSH;
    }

    if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_BACKGROUND_COLOR_ENABLED) != 0) {
        video_buffer->flags |= TIG_VIDEO_BUFFER_HAVE_BACKGROUND_COLOR;
    }

    video_buffer->x = 0;
    video_buffer->y = 0;
    video_buffer->width = video_buffer_spec->width;
    video_buffer->height = video_buffer_spec->height;
    video_buffer->texture_width = texture_width;
    video_buffer->texture_height = texture_height;
    video_buffer->background_color = video_buffer_spec->background_color;

    if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_BACKGROUND_COLOR_ENABLED) != 0) {
        tig_video_surface_blt(video_buffer->surface, NULL, video_buffer_spec->background_color);
    }

    video_buffer->pitch = 0;
    video_buffer->surface_data = NULL;
    video_buffer->lock_count = 0;

    return TIG_OK;
}

// 0x520390
int tig_video_buffer_destroy(TigVideoBuffer* video_buffer)
{
    if (video_buffer == NULL) {
        return TIG_ERR_16;
    }

    if (tig_video_3d_viewport == video_buffer) {
        tig_video_3d_viewport = NULL;
    }

    tig_video_surface_destroy(&(video_buffer->surface));
    free(video_buffer);

    return TIG_OK;
}

// 0x5203E0
int tig_video_buffer_data(TigVideoBuffer* video_buffer, TigVideoBufferData* video_buffer_data)
{
    if (video_buffer == NULL) {
        return TIG_ERR_16;
    }

    video_buffer_data->flags = video_buffer->flags;
    video_buffer_data->width = video_buffer->width;
    video_buffer_data->height = video_buffer->height;

    if ((video_buffer->flags & TIG_VIDEO_BUFFER_LOCKED) != 0) {
        video_buffer_data->pitch = video_buffer->pitch;
    } else {
        video_buffer_data->pitch = 0;
    }

    video_buffer_data->background_color = video_buffer->background_color;
    video_buffer_data->color_key = video_buffer->color_key;
    video_buffer_data->bpp = tig_video_bpp;

    if ((video_buffer->flags & TIG_VIDEO_BUFFER_LOCKED) != 0) {
        video_buffer_data->surface_data = video_buffer->surface_data;
    } else {
        video_buffer_data->surface_data = NULL;
    }

    return TIG_OK;
}

// 0x520450
int tig_video_buffer_set_color_key(TigVideoBuffer* video_buffer, int color_key)
{
    if ((video_buffer->flags & TIG_VIDEO_BUFFER_HAVE_TRANSPARENCY) == 0) {
        return TIG_ERR_16;
    }

    DDCOLORKEY ddck;
    ddck.dwColorSpaceLowValue = color_key;
    ddck.dwColorSpaceHighValue = color_key;

    HRESULT hr = IDirectDrawSurface7_SetColorKey(video_buffer->surface, DDCKEY_SRCBLT, &ddck);
    if (FAILED(hr)) {
        tig_video_print_dd_result(hr);
        return TIG_ERR_16;
    }

    video_buffer->color_key = color_key;

    return TIG_OK;
}

// 0x5204B0
int tig_video_buffer_lock(TigVideoBuffer* video_buffer)
{
    if (video_buffer->lock_count == 0) {
        if (!tig_video_surface_lock(video_buffer->surface, &(video_buffer->surface_desc), &(video_buffer->surface_data))) {
            return TIG_ERR_7;
        }

        video_buffer->pitch = video_buffer->surface_desc.lPitch;
        video_buffer->flags |= TIG_VIDEO_BUFFER_LOCKED;
    }

    video_buffer->lock_count++;

    return TIG_OK;
}

// 0x520500
int tig_video_buffer_unlock(TigVideoBuffer* video_buffer)
{
    if (video_buffer->lock_count == 1) {
        tig_video_surface_unlock(video_buffer->surface, &(video_buffer->surface_desc));
        video_buffer->flags &= ~TIG_VIDEO_BUFFER_LOCKED;
    }

    video_buffer->lock_count--;

    return TIG_OK;
}

// 0x520630
int tig_video_buffer_fill(TigVideoBuffer* video_buffer, TigRect* rect, int color)
{
    // TODO: Check return value.
    if (!tig_video_surface_blt(video_buffer->surface, rect, color)) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x520660
int tig_video_buffer_line(TigVideoBuffer* video_buffer)
{
    // NOTE: Params incomplete, update declaration.
    // TODO: Incomplete.
}

// 0x521000
int tig_video_buffer_blit(TigVideoBufferBlitSpec* blit_spec)
{
    // TODO: Incomplete.
}

// 0x522F30
int tig_video_buffer_get_pixel_color(TigVideoBuffer* video_buffer, int x, int y, int* color)
{
    if (x < video_buffer->x
        || y < video_buffer->y
        || x >= video_buffer->x + video_buffer->width
        || y >= video_buffer->y + video_buffer->height) {
        return TIG_ERR_12;
    }

    int rc = tig_video_buffer_lock(video_buffer);
    if (rc != TIG_OK) {
        return rc;
    }

    switch (tig_video_bpp) {
    case 8:
        *color = video_buffer->surface_data_8bpp[y * video_buffer->pitch + x];
        break;
    case 16:
        *color = video_buffer->surface_data_16bpp[y * video_buffer->pitch + x];
        break;
    case 24:
        if (1) {
            // TODO: Check.
            uint8_t* bytes = (uint8_t*)video_buffer->surface_data + 3 * (x + y * video_buffer->pitch / 3);
            *color = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16);
        }
        break;
    default:
        *color = video_buffer->surface_data_32bpp[y * video_buffer->pitch + x];
        break;
    }

    tig_video_buffer_unlock(video_buffer);

    return TIG_OK;
}

// 0x524080
bool tig_video_platform_window_init(TigContext* ctx)
{
    // TODO: Incomplete.
}

// 0x5242F0
void tig_video_platform_window_exit()
{
    tig_video_state.wnd = NULL;
}

// 0x524300
bool tig_video_ddraw_init(TigContext* ctx)
{
    HRESULT hr = DirectDrawCreateEx(NULL, (LPVOID*)&(tig_video_state.ddraw), IID_IDirectDraw7, NULL);
    if (FAILED(hr)) {
        return false;
    }

    DDCAPS ddcaps = { 0 };
    ddcaps.dwSize = sizeof(ddcaps);

    if ((ctx->flags & TIG_CONTEXT_FLAG_ANY_3D) != 0) {
        tig_debug_printf("3D: Checking DirectDraw object caps for 3D support...");

        IDirectDraw7_GetCaps(tig_video_state.ddraw, &ddcaps, NULL);

        if (ddcaps.dwCaps != 0) {
            tig_debug_printf("supported.\n");
        } else {
            ctx->flags &= ~TIG_CONTEXT_FLAG_3D_HARDWARE;
            tig_debug_printf("unsupported.\n");
        }
    }

    if ((ctx->flags & TIG_CONTEXT_FLAG_0x20) != 0) {
        if (!tig_video_ddraw_init_windowed(ctx)) {
            IDirectDraw7_Release(tig_video_state.ddraw);
            return false;
        }
    } else {
        if (!tig_video_ddraw_init_fullscreen(ctx)) {
            IDirectDraw7_Release(tig_video_state.ddraw);
            return false;
        }
    }

    if (!sub_524830()) {
        IDirectDraw7_Release(tig_video_state.ddraw);
        return false;
    }

    if ((ctx->flags & TIG_CONTEXT_FLAG_ANY_3D) != 0) {
        if (!tig_video_d3d_init(ctx)) {
            tig_debug_printf("Error initializing Direct3D.  3D support disabled.\n");
            ctx->flags &= ~TIG_CONTEXT_FLAG_ANY_3D;
        }
    }

    if (ctx->bpp == 8) {
        if (!tig_video_ddraw_palette_create(tig_video_state.ddraw)) {
            IDirectDraw7_Release(tig_video_state.ddraw);
            return false;
        }

        tig_video_surface_blt(tig_video_state.primary_surface, NULL, 0);

        if (tig_video_double_buffered) {
            tig_video_surface_blt(tig_video_state.offscreen_surface, NULL, 0);
        }
    }

    return true;
}

// 0x524480
bool tig_video_ddraw_init_windowed(TigContext* ctx)
{
    HRESULT hr;

    hr = IDirectDraw7_SetCooperativeLevel(tig_video_state.ddraw, tig_video_state.wnd, DDSCL_NORMAL);
    if (FAILED(hr)) {
        return false;
    }

    DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);
    hr = IDirectDraw7_GetDisplayMode(tig_video_state.ddraw, &ddsd);
    if (FAILED(hr)) {
        return false;
    }

    // TODO: Check.
    if ((ddsd.dwFlags & DDSCAPS_VIDEOMEMORY) == 0
        || (ddsd.ddpfPixelFormat.dwRGBBitCount != 16
            && ddsd.ddpfPixelFormat.dwRGBBitCount != 24
            && ddsd.ddpfPixelFormat.dwRGBBitCount != 32)) {
        MessageBoxA(tig_video_state.wnd,
            "Sorry, this program requires the Windows Desktop to be in a 16 or 24 or 32-bit RGB mode.",
            "Invalid Desktop Settings",
            MB_ICONHAND);
    }

    ctx->bpp = ddsd.ddpfPixelFormat.dwRGBBitCount;

    unsigned int caps = DDSCAPS_PRIMARYSURFACE;
    if ((ctx->flags & TIG_CONTEXT_FLAG_ANY_3D) != 0) {
        caps |= DDSCAPS_3DDEVICE;
    }

    if (!tig_video_surface_create(tig_video_state.ddraw, ctx->width, ctx->height, caps, &(tig_video_state.primary_surface))) {
        return false;
    }

    tig_video_double_buffered = (ctx->flags & TIG_CONTEXT_FLAG_DOUBLE_BUFFER) != 0;
    if (tig_video_double_buffered) {
        if (!tig_video_surface_create(tig_video_state.ddraw, ctx->width, ctx->height, DDSCAPS_OFFSCREENPLAIN, &(tig_video_state.offscreen_surface))) {
            IDirectDrawSurface7_Release(tig_video_state.primary_surface);
            return false;
        }

        tig_video_state.main_surface = tig_video_state.offscreen_surface;
    } else {
        tig_video_state.main_surface = tig_video_state.primary_surface;
    }

    LPDIRECTDRAWCLIPPER clipper;
    hr = DirectDrawCreateClipper(0, &clipper, NULL);
    if (FAILED(hr)) {
        IDirectDrawSurface7_Release(tig_video_state.offscreen_surface);
        IDirectDrawSurface7_Release(tig_video_state.primary_surface);
        return false;
    }

    hr = IDirectDrawClipper_SetHWnd(clipper, 0, tig_video_state.wnd);
    if (FAILED(hr)) {
        IDirectDrawClipper_Release(clipper);
        IDirectDrawSurface7_Release(tig_video_state.offscreen_surface);
        IDirectDrawSurface7_Release(tig_video_state.primary_surface);
        return false;
    }

    hr = IDirectDrawSurface7_SetClipper(tig_video_state.primary_surface, clipper);
    if (FAILED(hr)) {
        IDirectDrawClipper_Release(clipper);
        IDirectDrawSurface7_Release(tig_video_state.offscreen_surface);
        IDirectDrawSurface7_Release(tig_video_state.primary_surface);
        return false;
    }

    IDirectDrawClipper_Release(clipper);

    tig_video_fullscreen = false;

    return true;
}

// 0x524640
bool tig_video_ddraw_init_fullscreen(TigContext* ctx)
{
    HRESULT hr;

    hr = IDirectDraw7_SetCooperativeLevel(tig_video_state.ddraw, tig_video_state.wnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    if (FAILED(hr)) {
        return false;
    }

    hr = IDirectDraw7_SetDisplayMode(tig_video_state.ddraw, ctx->width, ctx->height, ctx->bpp, 0, 0);
    if (FAILED(hr)) {
        return false;
    }

    tig_video_double_buffered = (ctx->flags & TIG_CONTEXT_FLAG_DOUBLE_BUFFER) != 0;

    unsigned int caps = DDSCAPS_PRIMARYSURFACE;
    if (tig_video_double_buffered) {
        caps |= DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    }
    if ((ctx->flags & TIG_CONTEXT_FLAG_ANY_3D) != 0) {
        caps |= DDSCAPS_3DDEVICE;
    }

    if (!tig_video_surface_create(tig_video_state.ddraw, ctx->width, ctx->height, caps, &(tig_video_state.primary_surface))) {
        return false;
    }

    if (tig_video_double_buffered) {
        tig_video_state.main_surface = tig_video_state.offscreen_surface;
    } else {
        tig_video_state.main_surface = tig_video_state.primary_surface;
    }

    tig_video_fullscreen = true;

    DDCAPS ddcaps = { 0 };
    ddcaps.dwSize = sizeof(&ddcaps);
    if (FAILED(IDirectDraw7_GetCaps(tig_video_state.ddraw, &ddcaps, NULL))) {
        tig_debug_printf("Error retrieving caps from DirectDraw object.\n");
        return false;
    }

    tig_video_state.have_gamma_control = false;

    if ((ddcaps.dwCaps2 & DDCAPS2_PRIMARYGAMMA) != 0) {
        tig_debug_printf("Query for IID_IDirectDrawGammaControl ");

        if (SUCCEEDED(IDirectDrawSurface7_QueryInterface(tig_video_state.primary_surface, IID_IDirectDrawGammaControl, (LPVOID*)&(tig_video_state.gamma_control)))) {
            tig_debug_printf("succeeded.\n");
            tig_video_state.have_gamma_control = true;
        } else {
            tig_debug_printf("failed.\n");
        }
    }

    tig_debug_printf("Gamma control ");
    if (!tig_video_state.have_gamma_control) {
        tig_debug_printf("not ");
    }
    tig_debug_printf("supported.\n");

    if (tig_video_state.have_gamma_control) {
        if (SUCCEEDED(IDirectDrawGammaControl_GetGammaRamp(tig_video_state.gamma_control, 0, &(tig_video_state.gamma_ramp)))) {
            memcpy(&(tig_video_state.gamma_ramp2), &(tig_video_state.gamma_ramp), sizeof(DDGAMMARAMP));
            tig_video_gamma = 1.0;
        }
    }

    return true;
}

// 0x524830
bool sub_524830()
{
    for (int index = 0; index < 50; index++) {
        unsigned int red_mask = tig_video_color_to_mask(RGB(255, 0, 0));
        unsigned int green_mask = tig_video_color_to_mask(RGB(0, 255, 0));
        unsigned int blue_mask = tig_video_color_to_mask(RGB(0, 0, 255));

        // TODO: What the hell is this?
        if ((red_mask | green_mask | blue_mask) != 0) {
            if (((red_mask & green_mask) | (blue_mask & (red_mask | green_mask))) == 0) {
                tig_color_set_rgb_settings(red_mask, green_mask, blue_mask);
                return true;
            }
        }

        DDPIXELFORMAT ddpf;
        ddpf.dwSize = sizeof(ddpf);
        if (SUCCEEDED(IDirectDrawSurface7_GetPixelFormat(tig_video_state.primary_surface, &ddpf))) {
            if ((ddpf.dwFlags & DDPF_RGB) != 0) {
                red_mask = ddpf.dwRBitMask;
                green_mask = ddpf.dwGBitMask;
                blue_mask = ddpf.dwBBitMask;

                if ((red_mask | green_mask | blue_mask) != 0) {
                    if (((red_mask & green_mask) | (blue_mask & (red_mask | green_mask))) == 0) {
                        tig_color_set_rgb_settings(red_mask, green_mask, blue_mask);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

// 0x5248F0
int tig_video_ddraw_exit()
{
    tig_video_d3d_exit();

    if (tig_video_bpp == 8) {
        tig_video_ddraw_palette_destroy();
    }

    IDirectDraw7_Release(tig_video_state.ddraw);
    tig_video_state.ddraw = NULL;
    tig_video_state.primary_surface = NULL;
    tig_video_state.offscreen_surface = NULL;
    tig_video_state.main_surface = NULL;
    tig_video_state.palette = NULL;

    return TIG_OK;
}

// 0x524930
bool tig_video_d3d_init(TigContext* ctx)
{
    tig_video_3d_initialized = false;
    tig_video_3d_is_hardware = false;
    tig_video_3d_texture_must_power_of_two = false;
    tig_video_3d_texture_must_be_square = false;
    tig_video_3d_extra_surface_caps = 0;
    tig_video_3d_extra_surface_caps2 = 0;
    tig_video_3d_viewport = NULL;

    tig_debug_printf("3D: Query for IID_IDirect3D7 interface ");

    if (FAILED(IDirectDraw7_QueryInterface(tig_video_state.ddraw, IID_IDirect3D7, (LPVOID*)&(tig_video_state.d3d)))) {
        tig_debug_printf("failed. 3D support disabled.\n");
        return false;
    }

    tig_debug_printf("succeeded.\n");

    if ((ctx->flags & TIG_CONTEXT_FLAG_3D_REF_DEVICE) != 0) {
        tig_debug_printf("3D: Checking for reference device...");

        if (FAILED(IDirect3D7_CreateDevice(tig_video_state.d3d, IID_IDirect3DRefDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
            tig_debug_printf("FAILED.\n");
            return false;
        }

        tig_debug_printf("found IID_IDirect3DRefDevice\n");
    } else {
        if ((ctx->flags & TIG_CONTEXT_FLAG_3D_HARDWARE) != 0) {
            tig_debug_printf("3D: Checking for hardware devices...");

            if (SUCCEEDED(IDirect3D7_CreateDevice(tig_video_state.d3d, IID_IDirect3DTnLHalDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
                tig_video_3d_is_hardware = true;
                tig_debug_printf("found IID_IDirect3DTnLHalDevice\n");
            } else if (SUCCEEDED(IDirect3D7_CreateDevice(tig_video_state.d3d, IID_IDirect3DHALDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
                tig_video_3d_is_hardware = true;
                tig_debug_printf("found IID_IDirect3DHALDevice\n");
            } else {
                ctx->flags &= ~TIG_CONTEXT_FLAG_3D_HARDWARE;
                tig_debug_printf("none found.\n");
            }
        }

        if (!tig_video_3d_is_hardware) {
            if ((ctx->flags & TIG_CONTEXT_FLAG_3D_SOFTWARE) != 0) {
                tig_debug_printf("3D: Checking for RGB software devices...");

                if (SUCCEEDED(IDirect3D7_CreateDevice(tig_video_state.d3d, IID_IDirect3DMMXDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
                    tig_debug_printf("found IID_IDirect3DMMXDevice.\n");
                } else if (SUCCEEDED(IDirect3D7_CreateDevice(tig_video_state.d3d, IID_IDirect3DRGBDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
                    tig_debug_printf("found IID_IDirect3DRGBDevice.\n");
                } else {
                    ctx->flags &= ~TIG_CONTEXT_FLAG_3D_SOFTWARE;
                    tig_debug_printf("none found.\n");
                }
            } else {
                return false;
            }
        }
    }

    tig_debug_printf("3D: Retrieving caps from device...\n");

    D3DDEVICEDESC7 device_desc;
    if (FAILED(IDirect3DDevice7_GetCaps(tig_video_state.d3d_device, &device_desc))) {
        tig_debug_printf("3D: Error retrieving caps from device.\n");
        return false;
    }

    if (tig_video_3d_is_hardware) {
        tig_video_3d_extra_surface_caps = DDSCAPS_TEXTURE;
        tig_video_3d_extra_surface_caps2 = DDSCAPS2_TEXTUREMANAGE;
    } else {
        tig_video_3d_extra_surface_caps = DDSCAPS_SYSTEMMEMORY;
        tig_video_3d_extra_surface_caps2 = 0;
    }

    if (ctx->texture_width > device_desc.dwMaxTextureWidth || ctx->texture_height > device_desc.dwMaxTextureHeight) {
        tig_debug_printf(
            "3D: Card cannot support %dx%d sized textures - max is %dx%d\n",
            ctx->texture_width,
            ctx->texture_height,
            device_desc.dwMaxTextureWidth,
            device_desc.dwMaxTextureHeight);
        return false;
    }

    if ((device_desc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2) != 0) {
        tig_video_3d_texture_must_power_of_two = true;
        tig_debug_printf("3D: Textures must be power of 2.\n");
    }

    if ((device_desc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0) {
        tig_video_3d_texture_must_be_square = true;
        tig_debug_printf("3D: Textures must square.\n");
    }

    tig_debug_printf("3D: Setting default rendering states...\n");

    if (SUCCEEDED(IDirect3DDevice7_SetRenderState(tig_video_state.d3d_device, D3DRENDERSTATE_TEXTUREPERSPECTIVE, 0))) {
        tig_debug_printf("3D: Perspective correction disabled.\n");
    } else {
        tig_debug_printf("3D: Error disabling perspective correction.\n");
    }

    if (SUCCEEDED(IDirect3DDevice7_SetRenderState(tig_video_state.d3d_device, D3DRENDERSTATE_CULLMODE, 1))) {
        tig_debug_printf("3D: culling disabled.\n");
    } else {
        tig_debug_printf("3D: Error disabling culling.\n");
    }

    if (SUCCEEDED(IDirect3DDevice7_SetRenderState(tig_video_state.d3d_device, D3DRENDERSTATE_LIGHTING, 0))) {
        tig_debug_printf("3D: Lighting disabled.\n");
    } else {
        tig_debug_printf("3D: Error disabling lighting.\n");
    }

    if (FAILED(IDirect3DDevice7_SetTextureStageState(tig_video_state.d3d_device, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE))) {
        tig_debug_printf("3D: Error setting alpha arg 1.\n");
    }

    if (FAILED(IDirect3DDevice7_SetTextureStageState(tig_video_state.d3d_device, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE))) {
        tig_debug_printf("3D: Error setting alpha arg 2.\n");
    }

    if (FAILED(IDirect3DDevice7_SetTextureStageState(tig_video_state.d3d_device, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE))) {
        tig_debug_printf("3D: Error setting alpha op.\n");
    }

    if (FAILED(IDirect3DDevice7_SetTextureStageState(tig_video_state.d3d_device, 0, D3DTSS_MINFILTER, D3DTFN_POINT))) {
        tig_debug_printf("3D: Error setting texture filtering.\n");
    }

    if (FAILED(IDirect3DDevice7_SetTextureStageState(tig_video_state.d3d_device, 0, D3DTSS_MAGFILTER, D3DTFN_POINT))) {
        tig_debug_printf("3D: Error setting texture filtering.\n");
    }

    tig_video_3d_pixel_format = { 0 };

    if (FAILED(IDirect3DDevice7_EnumTextureFormats(tig_video_state.d3d_device, tig_video_3d_check_pixel_format, (LPVOID)ctx->bpp))) {
        tig_debug_printf("3D: Error enumerating texture formats.\n");
        return false;
    }

    // TODO: Figure out which named field is checked.
    if (tig_video_3d_pixel_format.dwRGBBitCount == 0) {
        tig_debug_printf("3D: Error getting texture pixel format.\n");
        return false;
    }

    if (SUCCEEDED(IDirect3DDevice7_SetRenderState(tig_video_state.d3d_device, D3DRENDERSTATE_ALPHATESTENABLE, TRUE))) {
        tig_debug_printf("3D: alpha test enabled.\n");
    } else {
        tig_debug_printf("3D: Error setting alpha test render state.\n");
        return false;
    }

    if (SUCCEEDED(IDirect3DDevice7_SetRenderState(tig_video_state.d3d_device, D3DRENDERSTATE_ALPHAREF, 1))) {
        tig_debug_printf("3D: alpha reference set.\n");
    } else {
        tig_debug_printf("3D: Error setting alpha reference.\n");
        return false;
    }

    if (SUCCEEDED(IDirect3DDevice7_SetRenderState(tig_video_state.d3d_device, D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL))) {
        tig_debug_printf("3D: alpha func set.\n");
    } else {
        tig_debug_printf("3D: Error setting alpha func.\n");
        return false;
    }

    tig_color_set_rgba_settings(tig_video_3d_pixel_format.dwRBitMask,
        tig_video_3d_pixel_format.dwGBitMask,
        tig_video_3d_pixel_format.dwBBitMask,
        tig_video_3d_pixel_format.dwRGBAlphaBitMask);

    tig_debug_printf("3D: Initialization successful.\n");
    tig_video_3d_initialized = true;

    return true;
}

// 0x524E90
HRESULT tig_video_3d_check_pixel_format(LPDDPIXELFORMAT lpDDPixFmt, LPVOID lpContext)
{
    if (lpDDPixFmt == NULL) {
        return S_FALSE;
    }

    if (lpContext == NULL) {
        return S_FALSE;
    }

    if ((lpDDPixFmt->dwFlags & (DDPF_LUMINANCE | DDPF_BUMPLUMINANCE | DDPF_BUMPDUDV)) != 0) {
        return S_FALSE;
    }

    // TODO: Not sure which named field is checked.
    if (lpDDPixFmt->dwRGBBitCount < 16) {
        return S_FALSE;
    }

    if (lpDDPixFmt->dwFourCC != 0) {
        return S_FALSE;
    }

    // TODO: Not sure which named field is checked.
    if (lpDDPixFmt->dwRGBAlphaBitMask == 0xF000) {
        return S_FALSE;
    }

    if ((lpDDPixFmt->dwFlags & DDPF_ALPHAPIXELS) == 0) {
        return S_FALSE;
    }

    if (lpDDPixFmt->dwRGBBitCount != (DWORD)lpContext) {
        return S_FALSE;
    }

    memcpy(&tig_video_3d_pixel_format, lpDDPixFmt, sizeof(DDPIXELFORMAT));

    return S_OK;
}

// 0x524EF0
void tig_video_d3d_exit()
{
}

// 0x524F00
bool tig_video_ddraw_palette_create(LPDIRECTDRAW7 ddraw)
{
    // TODO: Incomplete.
}

// 0x524FC0
void tig_video_ddraw_palette_destroy()
{
    if (tig_video_state.palette != NULL) {
        IDirectDrawPalette_Release(tig_video_state.palette);
    }
}

// 0x524FD0
bool tig_video_surface_create(LPDIRECTDRAW7 ddraw, int width, int height, unsigned int caps, LPDIRECTDRAWSURFACE7* surface_ptr)
{
    HRESULT hr;

    DDSURFACEDESC2 ddsd = { 0 };
    ddsd.dwSize = sizeof(ddsd);

    if ((caps & DDSCAPS_PRIMARYSURFACE) != 0) {
        ddsd.dwFlags |= DDSD_CAPS;
        ddsd.ddsCaps.dwCaps = caps;

        if ((caps & DDSCAPS_FLIP) != 0) {
            ddsd.dwFlags |= DDSD_BACKBUFFERCOUNT;
            ddsd.dwBackBufferCount = 1;
        }
    } else {
        ddsd.dwFlags |= DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
        ddsd.dwWidth = width;
        ddsd.dwHeight = height;
        ddsd.ddsCaps.dwCaps = caps;

        if ((caps & DDSCAPS_TEXTURE) != 0) {
            ddsd.dwFlags |= DDSD_PIXELFORMAT;
            memcpy(&(ddsd.ddpfPixelFormat), &tig_video_3d_pixel_format, sizeof(DDPIXELFORMAT));
            ddsd.ddsCaps.dwCaps |= tig_video_3d_extra_surface_caps;
            ddsd.ddsCaps.dwCaps2 |= tig_video_3d_extra_surface_caps2;
        }
    }

    hr = IDirectDraw7_CreateSurface(ddraw, &ddsd, surface_ptr, NULL);
    if (FAILED(hr)) {
        tig_video_print_dd_result(hr);
        return false;
    }

    if ((caps & DDSCAPS_FLIP) != 0) {
        DDSCAPS2 ddscaps2;
        ddscaps2.dwCaps = DDSCAPS_BACKBUFFER;
        ddscaps2.dwCaps2 = 0;
        ddscaps2.dwCaps3 = 0;
        ddscaps2.dwCaps3 = 0;

        hr = IDirectDrawSurface7_GetAttachedSurface(*surface_ptr, &ddscaps2, &(tig_video_state.offscreen_surface));
        if (FAILED(hr)) {
            tig_video_print_dd_result(hr);
            tig_video_surface_destroy(surface_ptr);
            return false;
        }
    }

    return true;
}

// 0x525120
void tig_video_surface_destroy(LPDIRECTDRAWSURFACE7* surface_ptr)
{
    if (surface_ptr != NULL) {
        IDirectDrawSurface7_Release(*surface_ptr);
        *surface_ptr = NULL;
    }
}

// 0x525140
bool tig_video_surface_lock(LPDIRECTDRAWSURFACE7 surface, LPDDSURFACEDESC2 surface_desc, void** surface_data_ptr)
{
    memset(surface_desc, 0, sizeof(*surface_desc));
    surface_desc->dwSize = sizeof(*surface_desc);

    HRESULT hr = IDirectDrawSurface7_Lock(surface, NULL, surface_desc, DDLOCK_OKTOSWAP | DDLOCK_WAIT, NULL);
    if (FAILED(hr)) {
        tig_video_print_dd_result(hr);
        return false;
    }

    *surface_data_ptr = surface_desc->lpSurface;

    return true;
}

// 0x525190
bool tig_video_surface_unlock(LPDIRECTDRAWSURFACE7 surface, LPDDSURFACEDESC2 surface_desc)
{
    HRESULT hr = IDirectDrawSurface7_Unlock(surface, surface_desc->lpSurface);
    if (FAILED(hr)) {
        tig_video_print_dd_result(hr);
        return false;
    }

    return true;
}

// 0x5251C0
bool tig_video_surface_blt(LPDIRECTDRAWSURFACE7 surface, TigRect* rect, int color)
{
    DDBLTFX fx;
    fx.dwSize = sizeof(fx);
    fx.dwFillColor = color;

    RECT win_rect;
    LPRECT win_rect_ptr;
    if (rect != NULL) {
        win_rect.left = rect->x;
        win_rect.top = rect->y;
        win_rect.right = win_rect.left + rect->width;
        win_rect.bottom = win_rect.top + rect->height;
        win_rect_ptr = &win_rect;
    } else {
        win_rect_ptr = NULL;
    }

    HRESULT hr = IDirectDrawSurface_Blt(surface, win_rect_ptr, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    if (FAILED(hr)) {
        tig_video_print_dd_result(hr);
        return false;
    }

    return true;
}

// 0x525250
int sub_525250(int a1)
{
    int rc;

    if (dword_5BF3D8 != a1) {
        return TIG_ERR_16;
    }

    int index;
    char path[MAX_PATH];

    for (index = 0; index < INT_MAX; index++) {
        sprintf(path, "screen%04d.bmp", index);
        if (!tio_fileexists(path, NULL)) {
            break;
        }
    }

    if (index == INT_MAX) {
        return TIG_ERR_13;
    }

    void* surface_data;
    rc = tig_video_main_surface_lock(&surface_data);
    if (rc != TIG_OK) {
        return rc;
    }

    TigVideoBufferData video_buffer_data;
    video_buffer_data.pitch = tig_video_state.current_surface_desc.lPitch;
    video_buffer_data.width = tig_video_state.current_surface_desc.dwWidth;
    video_buffer_data.height = tig_video_state.current_surface_desc.dwHeight;
    video_buffer_data.surface_data = surface_data;
    video_buffer_data.flags = TIG_VIDEO_BUFFER_LOCKED | TIG_VIDEO_BUFFER_VIDEO_MEMORY;
    video_buffer_data.background_color = 0;
    video_buffer_data.color_key = 0;

    TigRect rect;
    rect.x = 0;
    rect.y = 0;
    rect.width = tig_video_state.current_surface_desc.dwWidth;
    rect.height = tig_video_state.current_surface_desc.dwHeight;

    rc = sub_525ED0(&video_buffer_data, &rect, path, false);

    tig_video_main_surface_unlock();

    return rc;
}

// 0x525350
unsigned int tig_video_color_to_mask(COLORREF color)
{
    HDC hdc;
    COLORREF saved_color = 0;
    unsigned int mask = 0;

    if (SUCCEEDED(IDirectDrawSurface7_GetDC(tig_video_state.primary_surface, &hdc))) {
        saved_color = GetPixel(hdc, 0, 0);
        SetPixel(hdc, 0, 0, color);
        IDirectDrawSurface7_ReleaseDC(tig_video_state.primary_surface, hdc);
    }

    DDSURFACEDESC2 ddsd;
    ddsd.dwSize = sizeof(ddsd);

    if (SUCCEEDED(IDirectDrawSurface7_Lock(tig_video_state.primary_surface, NULL, &ddsd, DDLOCK_WAIT, NULL))) {
        mask = *((unsigned int*)ddsd.lpSurface);
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32) {
            mask &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;
        }
        IDirectDrawSurface7_Unlock(tig_video_state.primary_surface, NULL);
    }

    if (SUCCEEDED(IDirectDrawSurface7_GetDC(tig_video_state.primary_surface, &hdc))) {
        SetPixel(hdc, 0, 0, saved_color);
        IDirectDrawSurface7_ReleaseDC(tig_video_state.primary_surface, hdc);
    }

    return mask;
}

// 0x525430
void tig_video_print_dd_result(HRESULT hr)
{
    // TODO: Incomplete.
}

// 0x526450
int tig_video_3d_set_viewport(TigVideoBuffer* video_buffer)
{
    HRESULT hr;

    if (!tig_video_3d_initialized) {
        return TIG_ERR_16;
    }

    if (tig_video_3d_viewport == video_buffer) {
        return TIG_OK;
    }

    hr = IDirect3DDevice7_SetRenderTarget(tig_video_state.d3d_device, video_buffer->surface, 0);
    if (FAILED(hr)) {
        tig_debug_printf("3D: Error setting rendering target.\n");
        tig_video_print_dd_result(hr);
        return TIG_ERR_16;
    }

    tig_debug_printf("3D: Render target successfully set.\n");

    D3DVIEWPORT7 viewport;
    viewport.dwX = 0;
    viewport.dwY = 0;
    viewport.dwWidth = video_buffer->surface_desc.dwWidth;
    viewport.dwHeight = video_buffer->surface_desc.dwHeight;
    viewport.dvMinZ = 0.0;
    viewport.dvMaxZ = 1.0;

    hr = IDirect3DDevice7_SetViewport(tig_video_state.d3d_device, &viewport);
    if (FAILED(hr)) {
        tig_debug_printf("3D: Error setting viewport.\n");
        tig_video_print_dd_result(hr);
        return TIG_ERR_16;
    }

    tig_debug_printf("3D: Viewport successfully set.\n");

    return TIG_OK;
}
