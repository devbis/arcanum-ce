#include "tig/video.h"

#include <d3d.h>
#include <stdio.h>

#include "tig/art.h"
#include "tig/color.h"
#include "tig/debug.h"
#include "tig/file.h"
#include "tig/memory.h"
#include "tig/message.h"
#include "tig/mouse.h"
#include "tig/tig.h"
#include "tig/timer.h"
#include "tig/window.h"

typedef struct TigVideoBuffer {
    /* 0000 */ unsigned int flags;
    /* 0004 */ TigRect frame;
    /* 0014 */ int texture_width;
    /* 0018 */ int texture_height;
    /* 001C */ int pitch;
    /* 0020 */ int background_color;
    /* 0024 */ int color_key;
    /* 0028 */ LPDIRECTDRAWSURFACE7 surface;
    /* 002C */ DDSURFACEDESC2 surface_desc;
    /* 00A8 */ union {
        void* pixels;
        uint8_t* p8;
        uint16_t* p16;
        uint32_t* p32;
    } surface_data;
    /* 00AC */ int lock_count;
} TigVideoBuffer;

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
    /* 0CA8 */ unsigned char field_CA8[1124];
} TigVideoState;

static_assert(sizeof(TigVideoState) == 0x110C, "wrong size");

static void tig_video_set_client_rect(LPRECT rect);
static void tig_video_display_fps();
static int tig_video_get_gamma_ramp(LPDDGAMMARAMP gamma_ramp);
static int tig_video_set_gamma_ramp(LPDDGAMMARAMP gamma_ramp);
static bool tig_video_platform_window_init(TigContext* ctx);
static void tig_video_platform_window_exit();
static bool tig_video_ddraw_init(TigContext* ctx);
static bool tig_video_ddraw_init_windowed(TigContext* ctx);
static bool tig_video_ddraw_init_fullscreen(TigContext* ctx);
static bool sub_524830();
static int tig_video_ddraw_exit();
static bool tig_video_d3d_init(TigContext* ctx);
static HRESULT CALLBACK tig_video_3d_check_pixel_format(LPDDPIXELFORMAT lpDDPixFmt, LPVOID lpContext);
static void tig_video_d3d_exit();
static bool tig_video_ddraw_palette_create(LPDIRECTDRAW7 ddraw);
static void tig_video_ddraw_palette_destroy();
static bool tig_video_surface_create(LPDIRECTDRAW7 ddraw, int width, int height, unsigned int caps, LPDIRECTDRAWSURFACE7* surface_ptr);
static void tig_video_surface_destroy(LPDIRECTDRAWSURFACE7* surface_ptr);
static bool tig_video_surface_lock(LPDIRECTDRAWSURFACE7 surface, LPDDSURFACEDESC2 surface_desc, void** surface_data_ptr);
static bool tig_video_surface_unlock(LPDIRECTDRAWSURFACE7 surface, LPDDSURFACEDESC2 surface_desc);
static bool tig_video_surface_blt(LPDIRECTDRAWSURFACE7 surface, TigRect* rect, int color);
static int tig_video_screenshot_make_internal(int a1);
static unsigned int tig_video_color_to_mask(COLORREF color);
static void tig_video_print_dd_result(HRESULT hr);
static int sub_525ED0(TigVideoBufferData* video_buffer_data, TigRect* rect, const char* file_name, bool a4);
static int tig_video_3d_set_viewport(TigVideoBuffer* video_buffer);
static void sub_526530(const TigRect* a, const TigRect* b, D3DCOLOR* alpha);
static void sub_526690(TigRect* a, TigRect* b, D3DCOLOR* color);

// 0x5BF3D8
static int tig_video_screenshot_key = -1;

// 0x60F250
static TigVideoState tig_video_state;

// 0x60FEF8
static float tig_video_gamma;

// 0x60FEFC
static RECT tig_video_client_rect;

// 0x60FF0C
static unsigned int tig_video_palette[256];

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

// 0x610358
static bool tig_video_3d_scene_started;

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

    if ((ctx->flags & TIG_CONTEXT_0x0020) != 0) {
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

    tig_video_show_fps = (ctx->flags & TIG_CONTEXT_FPS) != 0;
    dword_610318 = (ctx->flags & TIG_CONTEXT_0x0004) != 0;
    tig_video_bpp = ctx->bpp;
    tig_video_main_surface_locked = false;

    ShowCursor((ctx->flags & TIG_CONTEXT_0x0020) != 0 ? TRUE : FALSE);

    tig_video_screenshot_key = -1;
    dword_6103A4 = 0;

    tig_video_initialized = true;

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
int tig_video_platform_window_get(HWND* wnd_ptr)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    *wnd_ptr = tig_video_state.wnd;

    return TIG_OK;
}

// 0x51F430
int tig_video_instance_get(HINSTANCE* instance_ptr)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    *instance_ptr = tig_video_state.instance;

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

    unsigned int elapsed;
    HDC hdc;

    if (tig_video_show_fps) {
        if (counter >= 10) {
            tig_timer_start(&curr_time);
            elapsed = tig_timer_between(prev_time, curr_time);
            fps = (float)counter / ((float)elapsed / 1000.0f);
            sprintf(fps_string_buffer, "%10.0f", fps);
            prev_time = curr_time;
            counter = 0;
        }

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
    TigRect clamped_dst_rect;
    int rc;
    LPDIRECTDRAWSURFACE7 dst_surface;
    RECT native_src_rect;
    RECT native_dst_rect;
    DWORD flags;

    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    rc = tig_rect_intersection(dst_rect, &stru_610388, &clamped_dst_rect);
    if (rc != TIG_OK) {
        return rc;
    }

    dst_surface = to_primary_surface
        ? tig_video_state.primary_surface
        : tig_video_state.main_surface;

    native_src_rect.left = clamped_dst_rect.x - dst_rect->x + src_rect->x;
    native_src_rect.top = clamped_dst_rect.y - dst_rect->y + src_rect->y;
    native_src_rect.right = native_src_rect.left + clamped_dst_rect.width - dst_rect->width + src_rect->width;
    native_src_rect.bottom = native_src_rect.top + clamped_dst_rect.height - dst_rect->height + src_rect->height;

    if (tig_video_fullscreen) {
        flags = DDBLTFAST_WAIT;
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
        native_dst_rect.left = tig_video_client_rect.left + clamped_dst_rect.x;
        native_dst_rect.top = tig_video_client_rect.top + clamped_dst_rect.y;
        native_dst_rect.right = native_dst_rect.left + clamped_dst_rect.width;
        native_dst_rect.bottom = native_dst_rect.top + clamped_dst_rect.height;

        flags = DDBLT_WAIT;
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
    int rc;
    TigRect clamped_rect;

    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

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
    HRESULT hr;

    hr = IDirectDrawSurface7_IsLost(tig_video_state.main_surface);
    if (FAILED(hr)) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x51F880
int sub_51F880()
{
    HRESULT hr;
    TigMessage message;

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

    tig_timer_start(&(message.time));
    message.type = TIG_MESSAGE_TYPE_8;
    return tig_message_enqueue(&message);
}

// 0x51F8F0
int tig_video_flip()
{
    HRESULT hr;
    RECT src_rect;

    if (tig_video_double_buffered) {
        if (tig_video_fullscreen) {
            hr = IDirectDrawSurface7_Flip(tig_video_state.primary_surface, NULL, 1);
            if (FAILED(hr)) {
                tig_video_print_dd_result(hr);
                return TIG_ERR_16;
            }

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

// 0x51F9E0
int tig_video_screenshot_set_settings(TigVideoScreenshotSettings* settings)
{
    int rc;

    if (tig_video_screenshot_key != -1) {
        rc = tig_message_set_key_handler(NULL, tig_video_screenshot_key);
        if (rc != TIG_OK) {
            return rc;
        }
    }

    tig_video_screenshot_key = settings->key;
    dword_6103A4 = settings->field_4;

    if (tig_video_screenshot_key != -1) {
        rc = tig_message_set_key_handler(tig_video_screenshot_make_internal, tig_video_screenshot_key);
        if (rc != TIG_OK) {
            return rc;
        }
    }

    return TIG_OK;
}

// 0x51FA30
int tig_video_screenshot_make()
{
    return tig_video_screenshot_make_internal(tig_video_screenshot_key);
}

// 0x51FA40
int sub_51FA40(TigRect* rect)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    *rect = stru_610388;

    return TIG_OK;
}

// 0x51FA80
int tig_video_get_bpp(int* bpp)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    *bpp = tig_video_bpp;

    return TIG_OK;
}

// 0x51FAA0
int tig_video_get_palette(int* colors)
{
    int index;

    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    if (tig_video_bpp == 8) {
        for (index = 0; index < 256; index++) {
            colors[index] = tig_video_palette[index];
        }
    } else {
        memset(colors, 0, sizeof(int) * 256);
    }

    return TIG_OK;
}

// 0x51FAF0
int tig_video_get_pitch(int* pitch)
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    *pitch = (int)tig_video_state.current_surface_desc.lPitch;

    return TIG_OK;
}

// 0x51FB10
int tig_video_3d_check_initialized()
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    if (!tig_video_3d_initialized) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x51FB30
int tig_video_3d_check_hardware()
{
    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    if (!tig_video_3d_is_hardware) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x51FB50
int tig_video_3d_begin_scene()
{
    if (sub_51F860() != TIG_OK) {
        return TIG_ERR_16;
    }

    if (tig_video_3d_initialized) {
        if (FAILED(IDirect3DDevice7_BeginScene(tig_video_state.d3d_device))) {
            return TIG_ERR_16;
        }
    }

    tig_video_3d_scene_started = true;
    return TIG_OK;
}

// 0x51FBA0
int tig_video_3d_end_scene()
{
    if (!tig_video_3d_scene_started) {
        return TIG_ERR_16;
    }

    if (tig_video_3d_initialized) {
        if (FAILED(IDirect3DDevice7_EndScene(tig_video_state.d3d_device))) {
            return TIG_ERR_16;
        }
    }

    tig_video_3d_scene_started = false;
    return TIG_OK;
}

// 0x51FBF0
int tig_video_get_video_memory_status(size_t* total_memory, size_t* available_memory)
{
    DDSCAPS2 ddscaps2;
    DWORD total;
    DWORD available;
    HRESULT hr;

    if (!tig_video_initialized) {
        return TIG_NOT_INITIALIZED;
    }

    ddscaps2.dwCaps = tig_video_3d_initialized
        ? DDSCAPS_TEXTURE
        : DDSCAPS_LOCALVIDMEM | DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
    ddscaps2.dwCaps2 = 0;
    ddscaps2.dwCaps3 = 0;
    ddscaps2.dwCaps4 = 0;

    hr = IDirectDraw7_GetAvailableVidMem(tig_video_state.ddraw, &ddscaps2, &total, &available);
    if (FAILED(hr)) {
        tig_video_print_dd_result(hr);
        tig_debug_printf("Error determining approx. available surface memory.\n");
        return TIG_ERR_5;
    }

    *total_memory = (size_t)total;
    *available_memory = (size_t)available;

    return TIG_OK;
}

// 0x51FC90
int tig_video_check_gamma_control()
{
    if (!tig_video_state.have_gamma_control) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x51FCA0
int tig_video_fade(int color, int steps, float duration, unsigned int flags)
{
    DDGAMMARAMP curr_gamma_ramp;
    DDGAMMARAMP temp_gamma_ramp;
    LPDDGAMMARAMP dest_gamma_ramp;

    if (!tig_video_state.have_gamma_control) {
        return TIG_ERR_16;
    }

    if (tig_video_get_gamma_ramp(&curr_gamma_ramp) != TIG_OK) {
        return TIG_ERR_16;
    }

    if ((flags & 0x1) != 0) {
        dest_gamma_ramp = &(tig_video_state.gamma_ramp2);
    } else {
        int index;
        unsigned int red = tig_color_get_red(color) << 8;
        unsigned int green = tig_color_get_green(color) << 8;
        unsigned int blue = tig_color_get_blue(color) << 8;

        for (index = 0; index < 256; index++) {
            temp_gamma_ramp.red[index] = (WORD)red;
            temp_gamma_ramp.green[index] = (WORD)green;
            temp_gamma_ramp.blue[index] = (WORD)blue;
        }

        dest_gamma_ramp = &temp_gamma_ramp;
    }

    if (steps < 1 || duration < 1.0) {
        if (tig_video_set_gamma_ramp(dest_gamma_ramp) != TIG_OK) {
            return TIG_ERR_16;
        }
    } else {
        int duration_per_step = (int)((double)duration / (double)steps);
        int step;
        unsigned int time;
        int index;

        for (step = 0; step < steps; step++) {
            tig_timer_start(&time);

            // TODO: Unclear.
            for (index = 0; index < 256; index++) {
                curr_gamma_ramp.red[index] = (WORD)((dest_gamma_ramp->red[index] - curr_gamma_ramp.red[index]) / (steps - step));
                curr_gamma_ramp.green[index] = (WORD)((dest_gamma_ramp->green[index] - curr_gamma_ramp.green[index]) / (steps - step));
                curr_gamma_ramp.blue[index] = (WORD)((dest_gamma_ramp->blue[index] - curr_gamma_ramp.blue[index]) / (steps - step));

                if (tig_video_set_gamma_ramp(&curr_gamma_ramp) != TIG_OK) {
                    return TIG_ERR_16;
                }
            }

            while (tig_timer_elapsed(time) < duration_per_step) {
            }
        }
    }

    return TIG_OK;
}

// 0x51FEC0
int tig_video_get_gamma_ramp(LPDDGAMMARAMP gamma_ramp)
{
    int v1;
    int index;

    if (!tig_video_state.have_gamma_control) {
        return TIG_ERR_16;
    }

    if (FAILED(IDirectDrawGammaControl_GetGammaRamp(tig_video_state.gamma_control, 0, gamma_ramp))) {
        return TIG_ERR_16;
    }

    // TODO: Unclear.
    v1 = 0;
    for (index = 0; index < 256; index++) {
        v1 |= gamma_ramp->red[index] | gamma_ramp->green[index] | gamma_ramp->blue[index];
    }

    if (HIBYTE(v1) == 0) {
        for (index = 0; index < 256; index++) {
            gamma_ramp->red[index] = LOBYTE(gamma_ramp->red[index]) << 8;
            gamma_ramp->green[index] = LOBYTE(gamma_ramp->green[index]) << 8;
            gamma_ramp->blue[index] = LOBYTE(gamma_ramp->blue[index]) << 8;
        }
    }

    return TIG_OK;
}

// 0x51FF60
int tig_video_set_gamma_ramp(LPDDGAMMARAMP gamma_ramp)
{
    int index;

    if (!tig_video_state.have_gamma_control) {
        return TIG_ERR_16;
    }

    // TODO: Unclear.
    for (index = 0; index < 256; index++) {
        gamma_ramp->red[index] = (HIBYTE(gamma_ramp->red[index]) << 8) | HIBYTE(gamma_ramp->red[index]);
        gamma_ramp->green[index] = (HIBYTE(gamma_ramp->green[index]) << 8) | HIBYTE(gamma_ramp->green[index]);
        gamma_ramp->blue[index] = (HIBYTE(gamma_ramp->blue[index]) << 8) | HIBYTE(gamma_ramp->blue[index]);
    }

    if (FAILED(IDirectDrawGammaControl_SetGammaRamp(tig_video_state.gamma_control, 0, gamma_ramp))) {
        return TIG_ERR_16;
    }

    return TIG_OK;
}

// 0x51FFE0
int tig_video_set_gamma(float gamma)
{
    int index;
    unsigned int red;
    unsigned int green;
    unsigned int blue;

    if (!tig_video_state.have_gamma_control) {
        return TIG_ERR_16;
    }

    if (gamma == tig_video_gamma) {
        return TIG_OK;
    }

    if (gamma < 0.0 || gamma >= 2.0) {
        return TIG_ERR_12;
    }

    for (index = 0; index < 256; index++) {
        red = (unsigned int)((float)tig_video_state.gamma_ramp.red[index] * gamma);
        tig_video_state.gamma_ramp2.red[index] = (WORD)min(red, 0xFF00);

        green = (unsigned int)((float)tig_video_state.gamma_ramp.green[index] * gamma);
        tig_video_state.gamma_ramp2.green[index] = (WORD)min(green, 0xFF00);

        blue = (unsigned int)((float)tig_video_state.gamma_ramp.blue[index] * gamma);
        tig_video_state.gamma_ramp2.blue[index] = (WORD)min(blue, 0xFF00);
    }

    tig_video_gamma = gamma;
    tig_video_fade(0, 0, 0.0, 1);

    return TIG_OK;
}

// 0x5200F0
int tig_video_buffer_create(TigVideoBufferSpec* video_buffer_spec, TigVideoBuffer** video_buffer_ptr)
{
    TigVideoBuffer* video_buffer;
    DWORD caps;
    int texture_width;
    int texture_height;

    video_buffer = (TigVideoBuffer*)MALLOC(sizeof(*video_buffer));
    memset(video_buffer, 0, sizeof(*video_buffer));
    *video_buffer_ptr = video_buffer;

    caps = DDSCAPS_OFFSCREENPLAIN;

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

    texture_width = video_buffer_spec->width;
    texture_height = video_buffer_spec->height;

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
            FREE(video_buffer);
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
                        FREE(video_buffer);
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
                    FREE(video_buffer);
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

    video_buffer->frame.x = 0;
    video_buffer->frame.y = 0;
    video_buffer->frame.width = video_buffer_spec->width;
    video_buffer->frame.height = video_buffer_spec->height;
    video_buffer->texture_width = texture_width;
    video_buffer->texture_height = texture_height;
    video_buffer->background_color = video_buffer_spec->background_color;

    if ((video_buffer_spec->flags & TIG_VIDEO_BUFFER_SPEC_BACKGROUND_COLOR_ENABLED) != 0) {
        tig_video_surface_blt(video_buffer->surface, NULL, video_buffer_spec->background_color);
    }

    video_buffer->pitch = 0;
    video_buffer->surface_data.pixels = NULL;
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
    FREE(video_buffer);

    return TIG_OK;
}

// 0x5203E0
int tig_video_buffer_data(TigVideoBuffer* video_buffer, TigVideoBufferData* video_buffer_data)
{
    if (video_buffer == NULL) {
        return TIG_ERR_16;
    }

    video_buffer_data->flags = video_buffer->flags;
    video_buffer_data->width = video_buffer->frame.width;
    video_buffer_data->height = video_buffer->frame.height;

    if ((video_buffer->flags & TIG_VIDEO_BUFFER_LOCKED) != 0) {
        video_buffer_data->pitch = video_buffer->pitch;
    } else {
        video_buffer_data->pitch = 0;
    }

    video_buffer_data->background_color = video_buffer->background_color;
    video_buffer_data->color_key = video_buffer->color_key;
    video_buffer_data->bpp = tig_video_bpp;

    if ((video_buffer->flags & TIG_VIDEO_BUFFER_LOCKED) != 0) {
        video_buffer_data->surface_data.pixels = video_buffer->surface_data.pixels;
    } else {
        video_buffer_data->surface_data.pixels = NULL;
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
        if (!tig_video_surface_lock(video_buffer->surface, &(video_buffer->surface_desc), &(video_buffer->surface_data.pixels))) {
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

// 0x520540
int tig_video_buffer_outline(TigVideoBuffer* video_buffer, TigRect* rect, int color)
{
    TigRect line;
    int rc;

    line.x = rect->x;
    line.y = rect->y;
    line.width = rect->width;
    line.height = 1;
    rc = tig_video_buffer_fill(video_buffer, &line, color);
    if (rc != TIG_OK) {
        return rc;
    }

    line.x = rect->x;
    line.y = rect->y;
    line.width = 1;
    line.height = rect->height;
    rc = tig_video_buffer_fill(video_buffer, &line, color);
    if (rc != TIG_OK) {
        return rc;
    }

    line.x = rect->x + rect->width - 1;
    line.y = rect->y;
    line.width = 1;
    line.height = rect->height;
    rc = tig_video_buffer_fill(video_buffer, &line, color);
    if (rc != TIG_OK) {
        return rc;
    }

    line.x = rect->x;
    line.y = rect->y + rect->height - 1;
    line.width = rect->width;
    line.height = 1;
    rc = tig_video_buffer_fill(video_buffer, &line, color);
    if (rc != TIG_OK) {
        return rc;
    }

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
int tig_video_buffer_line(TigVideoBuffer* video_buffer, TigRect* rect, int a3, unsigned int color)
{
    // TODO: Incomplete.
    (void)video_buffer;
    (void)rect;
    (void)a3;
    (void)color;

    return TIG_OK;
}

// NOTE: This function is pretty big, not sure if its possible to implement it
// one-to-one.
//
// 0x521000
int tig_video_buffer_blit(TigVideoBufferBlitSpec* blit_spec)
{
    // TODO: Incomplete.
    (void)blit_spec;

    return TIG_OK;
}

// 0x522F30
int tig_video_buffer_get_pixel_color(TigVideoBuffer* video_buffer, int x, int y, unsigned int* color)
{
    int index;
    int rc;

    if (x < video_buffer->frame.x
        || y < video_buffer->frame.y
        || x >= video_buffer->frame.x + video_buffer->frame.width
        || y >= video_buffer->frame.y + video_buffer->frame.height) {
        return TIG_ERR_12;
    }

    rc = tig_video_buffer_lock(video_buffer);
    if (rc != TIG_OK) {
        return rc;
    }

    switch (tig_video_bpp) {
    case 8:
        index = y * video_buffer->pitch + x;
        *color = video_buffer->surface_data.p8[index];
        break;
    case 16:
        index = y * video_buffer->pitch + x;
        *color = video_buffer->surface_data.p16[index];
        break;
    case 24:
        index = (y * video_buffer->pitch / 3 + x) * 3;
        *color = video_buffer->surface_data.p8[index] | (video_buffer->surface_data.p8[index + 1] << 8) | (video_buffer->surface_data.p8[index + 2] << 16);
        break;
    default:
        index = y * video_buffer->pitch + x;
        *color = video_buffer->surface_data.p32[index];
        break;
    }

    tig_video_buffer_unlock(video_buffer);

    return TIG_OK;
}

// 0x523050
int sub_523050(TigVideoBuffer* video_buffer, int x, int y, HWND wnd, TigRect* rect)
{
    HDC src;
    HDC dst;

    IDirectDrawSurface7_GetDC(video_buffer->surface, &src);
    dst = GetDC(wnd);
    BitBlt(dst,
        rect->x,
        rect->y,
        rect->width,
        rect->height,
        src,
        x,
        y,
        SRCCOPY);
    ReleaseDC(wnd, dst);
    IDirectDrawSurface7_ReleaseDC(video_buffer->surface, src);

    return TIG_OK;
}

// 0x5230C0
int sub_5230C0(TigVideoBuffer* video_buffer, int x, int y, HDC dst, TigRect* rect)
{
    HDC src;

    IDirectDrawSurface7_GetDC(video_buffer->surface, &src);
    BitBlt(dst,
        rect->x,
        rect->y,
        rect->width,
        rect->height,
        src,
        x,
        y,
        SRCCOPY);
    IDirectDrawSurface7_ReleaseDC(video_buffer->surface, src);

    return TIG_OK;
}

// 0x524080
bool tig_video_platform_window_init(TigContext* ctx)
{
    WNDCLASSA window_class;
    RECT window_rect;
    const char* name;
    int screen_width;
    int screen_height;
    HMENU menu;
    DWORD style;
    DWORD ex_style;

    tig_video_state.instance = ctx->instance;

    window_class.style = CS_DBLCLKS;
    window_class.lpfnWndProc = tig_message_wnd_proc;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = ctx->instance;
    window_class.hIcon = LoadIconA(ctx->instance, "TIGIcon");
    window_class.hCursor = LoadCursorA(NULL, MAKEINTRESOURCEA(0x7F00));
    window_class.hbrBackground = (HBRUSH)GetStockObject(4);
    window_class.lpszMenuName = NULL;
    window_class.lpszClassName = "TIGClass";

    if (RegisterClassA(&window_class) == 0) {
        return false;
    }

    screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
    screen_height = GetSystemMetrics(SM_CYFULLSCREEN);

    if ((ctx->flags & TIG_CONTEXT_0x0020) != 0) {
        if ((ctx->flags & TIG_CONTEXT_0x0100) != 0) {
            window_rect.left = ctx->x;
            window_rect.top = ctx->y;
            window_rect.right = window_rect.left + ctx->width;
            window_rect.bottom = window_rect.top + ctx->height;
        } else {
            window_rect.left = (screen_width - ctx->width) / 2;
            window_rect.top = (screen_height - ctx->height) / 2;
            window_rect.right = window_rect.left + ctx->width;
            window_rect.bottom = window_rect.top + ctx->height;
        }

        menu = LoadMenuA(ctx->instance, "TIGMenu");
        style = WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_GROUP;
        ex_style = 0;

        if ((ctx->flags & TIG_CONTEXT_0x0080) == 0) {
            LONG offset_x = (window_rect.right - window_rect.left - ctx->width) / 2;
            LONG offset_y = (window_rect.bottom - window_rect.top - ctx->height) / 2;

            AdjustWindowRectEx(&window_rect,
                style,
                menu != NULL,
                ex_style);

            window_rect.left += offset_x;
            window_rect.top += offset_y;
            window_rect.right += offset_x;
            window_rect.bottom += offset_y;
        }
    } else {
        window_rect.left = 0;
        window_rect.top = 0;
        window_rect.right = screen_width;
        window_rect.bottom = screen_height;
        menu = NULL;
        style = WS_POPUP;
        ex_style = WS_EX_APPWINDOW | WS_EX_TOPMOST;
        tig_video_client_rect = window_rect;
    }

    style |= WS_VISIBLE;

    stru_610388.x = 0;
    stru_610388.y = 0;
    stru_610388.width = ctx->width;
    stru_610388.height = ctx->height;

    name = (ctx->flags & TIG_CONTEXT_HAVE_WINDOW_NAME) != 0
        ? ctx->window_name
        : tig_get_executable(true);

    tig_video_state.wnd = CreateWindowExA(
        ex_style,
        "TIGClass",
        name,
        style,
        window_rect.left,
        window_rect.top,
        window_rect.right - window_rect.left,
        window_rect.bottom - window_rect.top,
        NULL,
        menu,
        ctx->instance,
        NULL);
    if (tig_video_state.wnd == NULL) {
        return false;
    }

    return true;
}

// 0x5242F0
void tig_video_platform_window_exit()
{
    tig_video_state.wnd = NULL;
}

// 0x524300
bool tig_video_ddraw_init(TigContext* ctx)
{
    HRESULT hr = DirectDrawCreateEx(NULL, (LPVOID*)&(tig_video_state.ddraw), &IID_IDirectDraw7, NULL);
    if (FAILED(hr)) {
        return false;
    }

    DDCAPS ddcaps = { 0 };
    ddcaps.dwSize = sizeof(ddcaps);

    if ((ctx->flags & TIG_CONTEXT_ANY_3D) != 0) {
        tig_debug_printf("3D: Checking DirectDraw object caps for 3D support...");

        IDirectDraw7_GetCaps(tig_video_state.ddraw, &ddcaps, NULL);

        if (ddcaps.dwCaps != 0) {
            tig_debug_printf("supported.\n");
        } else {
            ctx->flags &= ~TIG_CONTEXT_3D_HARDWARE;
            tig_debug_printf("unsupported.\n");
        }
    }

    if ((ctx->flags & TIG_CONTEXT_0x0020) != 0) {
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

    if ((ctx->flags & TIG_CONTEXT_ANY_3D) != 0) {
        if (!tig_video_d3d_init(ctx)) {
            tig_debug_printf("Error initializing Direct3D.  3D support disabled.\n");
            ctx->flags &= ~TIG_CONTEXT_ANY_3D;
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
    if ((ctx->flags & TIG_CONTEXT_ANY_3D) != 0) {
        caps |= DDSCAPS_3DDEVICE;
    }

    if (!tig_video_surface_create(tig_video_state.ddraw, ctx->width, ctx->height, caps, &(tig_video_state.primary_surface))) {
        return false;
    }

    tig_video_double_buffered = (ctx->flags & TIG_CONTEXT_DOUBLE_BUFFER) != 0;
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

    tig_video_double_buffered = (ctx->flags & TIG_CONTEXT_DOUBLE_BUFFER) != 0;

    unsigned int caps = DDSCAPS_PRIMARYSURFACE;
    if (tig_video_double_buffered) {
        caps |= DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    }
    if ((ctx->flags & TIG_CONTEXT_ANY_3D) != 0) {
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

        if (SUCCEEDED(IDirectDrawSurface7_QueryInterface(tig_video_state.primary_surface, &IID_IDirectDrawGammaControl, (LPVOID*)&(tig_video_state.gamma_control)))) {
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

    if (FAILED(IDirectDraw7_QueryInterface(tig_video_state.ddraw, &IID_IDirect3D7, (LPVOID*)&(tig_video_state.d3d)))) {
        tig_debug_printf("failed. 3D support disabled.\n");
        return false;
    }

    tig_debug_printf("succeeded.\n");

    if ((ctx->flags & TIG_CONTEXT_3D_REF_DEVICE) != 0) {
        tig_debug_printf("3D: Checking for reference device...");

        if (FAILED(IDirect3D7_CreateDevice(tig_video_state.d3d, &IID_IDirect3DRefDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
            tig_debug_printf("FAILED.\n");
            return false;
        }

        tig_debug_printf("found IID_IDirect3DRefDevice\n");
    } else {
        if ((ctx->flags & TIG_CONTEXT_3D_HARDWARE) != 0) {
            tig_debug_printf("3D: Checking for hardware devices...");

            if (SUCCEEDED(IDirect3D7_CreateDevice(tig_video_state.d3d, &IID_IDirect3DTnLHalDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
                tig_video_3d_is_hardware = true;
                tig_debug_printf("found IID_IDirect3DTnLHalDevice\n");
            } else if (SUCCEEDED(IDirect3D7_CreateDevice(tig_video_state.d3d, &IID_IDirect3DHALDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
                tig_video_3d_is_hardware = true;
                tig_debug_printf("found IID_IDirect3DHALDevice\n");
            } else {
                ctx->flags &= ~TIG_CONTEXT_3D_HARDWARE;
                tig_debug_printf("none found.\n");
            }
        }

        if (!tig_video_3d_is_hardware) {
            if ((ctx->flags & TIG_CONTEXT_3D_SOFTWARE) != 0) {
                tig_debug_printf("3D: Checking for RGB software devices...");

                if (SUCCEEDED(IDirect3D7_CreateDevice(tig_video_state.d3d, &IID_IDirect3DMMXDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
                    tig_debug_printf("found IID_IDirect3DMMXDevice.\n");
                } else if (SUCCEEDED(IDirect3D7_CreateDevice(tig_video_state.d3d, &IID_IDirect3DRGBDevice, tig_video_state.main_surface, &(tig_video_state.d3d_device)))) {
                    tig_debug_printf("found IID_IDirect3DRGBDevice.\n");
                } else {
                    ctx->flags &= ~TIG_CONTEXT_3D_SOFTWARE;
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

    memset(&tig_video_3d_pixel_format, 0, sizeof(tig_video_3d_pixel_format));

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
HRESULT CALLBACK tig_video_3d_check_pixel_format(LPDDPIXELFORMAT lpDDPixFmt, LPVOID lpContext)
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
    tig_video_state.palette = NULL;

    // NOTE: Why use heap?
    PALETTEENTRY* entries = (PALETTEENTRY*)MALLOC(sizeof(*entries) * 256);
    for (int index = 0; index < 256; index++) {
        int red = tig_color_get_red(index);
        int green = tig_color_get_green(index);
        int blue = tig_color_get_blue(index);

        entries[index].peRed = (uint8_t)red;
        entries[index].peGreen = (uint8_t)green;
        entries[index].peBlue = (uint8_t)blue;
        entries[index].peFlags = 0;

        tig_video_palette[index] = tig_color_to_24_bpp(red, green, blue);
    }

    if (FAILED(IDirectDraw7_CreatePalette(ddraw, DDPCAPS_8BIT | DDPCAPS_ALLOW256, entries, &(tig_video_state.palette), NULL))) {
        FREE(entries);
        return false;
    }

    if (FAILED(IDirectDrawSurface7_SetPalette(tig_video_state.primary_surface, tig_video_state.palette))) {
        FREE(entries);
        return false;
    }

    FREE(entries);
    return true;
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
int tig_video_screenshot_make_internal(int a1)
{
    int rc;

    if (tig_video_screenshot_key != a1) {
        return TIG_ERR_16;
    }

    int index;
    char path[MAX_PATH];

    for (index = 0; index < INT_MAX; index++) {
        sprintf(path, "screen%04d.bmp", index);
        if (!tig_file_exists(path, NULL)) {
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
    video_buffer_data.surface_data.pixels = surface_data;
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
    DDSURFACEDESC2 ddsd;

    if (SUCCEEDED(IDirectDrawSurface7_GetDC(tig_video_state.primary_surface, &hdc))) {
        saved_color = GetPixel(hdc, 0, 0);
        SetPixel(hdc, 0, 0, color);
        IDirectDrawSurface7_ReleaseDC(tig_video_state.primary_surface, hdc);
    }

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
    switch (hr) {
    case DDERR_ALREADYINITIALIZED:
        tig_debug_println("DDERR_ALREADYINITIALIZED");
        break;
    case DDERR_BLTFASTCANTCLIP:
        tig_debug_println("DDERR_BLTFASTCANTCLIP");
        break;
    case DDERR_CANNOTATTACHSURFACE:
        tig_debug_println("DDERR_CANNOTATTACHSURFACE");
        break;
    case DDERR_CANNOTDETACHSURFACE:
        tig_debug_println("DDERR_CANNOTDETACHSURFACE");
        break;
    case DDERR_CANTCREATEDC:
        tig_debug_println("DDERR_CANTCREATEDC");
        break;
    case DDERR_CANTDUPLICATE:
        tig_debug_println("DDERR_CANTDUPLICATE");
        break;
    case DDERR_CANTLOCKSURFACE:
        tig_debug_println("DDERR_CANTLOCKSURFACE");
        break;
    case DDERR_CANTPAGELOCK:
        tig_debug_println("DDERR_CANTPAGELOCK");
        break;
    case DDERR_CANTPAGEUNLOCK:
        tig_debug_println("DDERR_CANTPAGEUNLOCK");
        break;
    case DDERR_CLIPPERISUSINGHWND:
        tig_debug_println("DDERR_CLIPPERISUSINGHWND");
        break;
    case DDERR_COLORKEYNOTSET:
        tig_debug_println("DDERR_COLORKEYNOTSET");
        break;
    case DDERR_CURRENTLYNOTAVAIL:
        tig_debug_println("DDERR_CURRENTLYNOTAVAIL");
        break;
    case DDERR_DCALREADYCREATED:
        tig_debug_println("DDERR_DCALREADYCREATED");
        break;
    case DDERR_DEVICEDOESNTOWNSURFACE:
        tig_debug_println("DDERR_DEVICEDOESNTOWNSURFACE");
        break;
    case DDERR_DIRECTDRAWALREADYCREATED:
        tig_debug_println("DDERR_DIRECTDRAWALREADYCREATED");
        break;
    case DDERR_EXCEPTION:
        tig_debug_println("DDERR_EXCEPTION");
        break;
    case DDERR_EXCLUSIVEMODEALREADYSET:
        tig_debug_println("DDERR_EXCLUSIVEMODEALREADYSET");
        break;
    case DDERR_GENERIC:
        tig_debug_println("DDERR_GENERIC");
        break;
    case DDERR_HEIGHTALIGN:
        tig_debug_println("DDERR_HEIGHTALIGN");
        break;
    case DDERR_HWNDALREADYSET:
        tig_debug_println("DDERR_HWNDALREADYSET");
        break;
    case DDERR_HWNDSUBCLASSED:
        tig_debug_println("DDERR_HWNDSUBCLASSED");
        break;
    case DDERR_IMPLICITLYCREATED:
        tig_debug_println("DDERR_IMPLICITLYCREATED");
        break;
    case DDERR_INCOMPATIBLEPRIMARY:
        tig_debug_println("DDERR_INCOMPATIBLEPRIMARY");
        break;
    case DDERR_INVALIDCAPS:
        tig_debug_println("DDERR_INVALIDCAPS");
        break;
    case DDERR_INVALIDCLIPLIST:
        tig_debug_println("DDERR_INVALIDCLIPLIST");
        break;
    case DDERR_INVALIDDIRECTDRAWGUID:
        tig_debug_println("DDERR_INVALIDDIRECTDRAWGUID");
        break;
    case DDERR_INVALIDMODE:
        tig_debug_println("DDERR_INVALIDMODE");
        break;
    case DDERR_INVALIDOBJECT:
        tig_debug_println("DDERR_INVALIDOBJECT");
        break;
    case DDERR_INVALIDPARAMS:
        tig_debug_println("DDERR_INVALIDPARAMS");
        break;
    case DDERR_INVALIDPIXELFORMAT:
        tig_debug_println("DDERR_INVALIDPIXELFORMAT");
        break;
    case DDERR_INVALIDPOSITION:
        tig_debug_println("DDERR_INVALIDPOSITION");
        break;
    case DDERR_INVALIDRECT:
        tig_debug_println("DDERR_INVALIDRECT");
        break;
    case DDERR_INVALIDSURFACETYPE:
        tig_debug_println("DDERR_INVALIDSURFACETYPE");
        break;
    case DDERR_LOCKEDSURFACES:
        tig_debug_println("DDERR_LOCKEDSURFACES");
        break;
    case DDERR_MOREDATA:
        tig_debug_println("DDERR_MOREDATA");
        break;
    case DDERR_NO3D:
        tig_debug_println("DDERR_NO3D");
        break;
    case DDERR_NOALPHAHW:
        tig_debug_println("DDERR_NOALPHAHW");
        break;
    case DDERR_NOBLTHW:
        tig_debug_println("DDERR_NOBLTHW");
        break;
    case DDERR_NOCLIPLIST:
        tig_debug_println("DDERR_NOCLIPLIST");
        break;
    case DDERR_NOCLIPPERATTACHED:
        tig_debug_println("DDERR_NOCLIPPERATTACHED");
        break;
    case DDERR_NOCOLORCONVHW:
        tig_debug_println("DDERR_NOCOLORCONVHW");
        break;
    case DDERR_NOCOLORKEY:
        tig_debug_println("DDERR_NOCOLORKEY");
        break;
    case DDERR_NOCOLORKEYHW:
        tig_debug_println("DDERR_NOCOLORKEYHW");
        break;
    case DDERR_NOCOOPERATIVELEVELSET:
        tig_debug_println("DDERR_NOCOOPERATIVELEVELSET");
        break;
    case DDERR_NODC:
        tig_debug_println("DDERR_NODC");
        break;
    case DDERR_NODDROPSHW:
        tig_debug_println("DDERR_NODDROPSHW");
        break;
    case DDERR_NODIRECTDRAWHW:
        tig_debug_println("DDERR_NODIRECTDRAWHW");
        break;
    case DDERR_NODIRECTDRAWSUPPORT:
        tig_debug_println("DDERR_NODIRECTDRAWSUPPORT");
        break;
    case DDERR_NOEMULATION:
        tig_debug_println("DDERR_NOEMULATION");
        break;
    case DDERR_NOEXCLUSIVEMODE:
        tig_debug_println("DDERR_NOEXCLUSIVEMODE");
        break;
    case DDERR_NOFLIPHW:
        tig_debug_println("DDERR_NOFLIPHW");
        break;
    case DDERR_NOFOCUSWINDOW:
        tig_debug_println("DDERR_NOFOCUSWINDOW");
        break;
    case DDERR_NOGDI:
        tig_debug_println("DDERR_NOGDI");
        break;
    case DDERR_NOHWND:
        tig_debug_println("DDERR_NOHWND");
        break;
    case DDERR_NOMIPMAPHW:
        tig_debug_println("DDERR_NOMIPMAPHW");
        break;
    case DDERR_NOMIRRORHW:
        tig_debug_println("DDERR_NOMIRRORHW");
        break;
    case DDERR_NONONLOCALVIDMEM:
        tig_debug_println("DDERR_NONONLOCALVIDMEM");
        break;
    case DDERR_NOOPTIMIZEHW:
        tig_debug_println("DDERR_NOOPTIMIZEHW");
        break;
    case DDERR_NOOVERLAYDEST:
        tig_debug_println("DDERR_NOOVERLAYDEST");
        break;
    case DDERR_NOOVERLAYHW:
        tig_debug_println("DDERR_NOOVERLAYHW");
        break;
    case DDERR_NOPALETTEATTACHED:
        tig_debug_println("DDERR_NOPALETTEATTACHED");
        break;
    case DDERR_NOPALETTEHW:
        tig_debug_println("DDERR_NOPALETTEHW");
        break;
    case DDERR_NORASTEROPHW:
        tig_debug_println("DDERR_NORASTEROPHW");
        break;
    case DDERR_NOROTATIONHW:
        tig_debug_println("DDERR_NOROTATIONHW");
        break;
    case DDERR_NOSTRETCHHW:
        tig_debug_println("DDERR_NOSTRETCHHW");
        break;
    case DDERR_NOT4BITCOLOR:
        tig_debug_println("DDERR_NOT4BITCOLOR");
        break;
    case DDERR_NOT4BITCOLORINDEX:
        tig_debug_println("DDERR_NOT4BITCOLORINDEX");
        break;
    case DDERR_NOT8BITCOLOR:
        tig_debug_println("DDERR_NOT8BITCOLOR");
        break;
    case DDERR_NOTAOVERLAYSURFACE:
        tig_debug_println("DDERR_NOTAOVERLAYSURFACE");
        break;
    case DDERR_NOTEXTUREHW:
        tig_debug_println("DDERR_NOTEXTUREHW");
        break;
    case DDERR_NOTFLIPPABLE:
        tig_debug_println("DDERR_NOTFLIPPABLE");
        break;
    case DDERR_NOTFOUND:
        tig_debug_println("DDERR_NOTFOUND");
        break;
    case DDERR_NOTINITIALIZED:
        tig_debug_println("DDERR_NOTINITIALIZED");
        break;
    case DDERR_NOTLOADED:
        tig_debug_println("DDERR_NOTLOADED");
        break;
    case DDERR_NOTLOCKED:
        tig_debug_println("DDERR_NOTLOCKED");
        break;
    case DDERR_NOTPAGELOCKED:
        tig_debug_println("DDERR_NOTPAGELOCKED");
        break;
    case DDERR_NOTPALETTIZED:
        tig_debug_println("DDERR_NOTPALETTIZED");
        break;
    case DDERR_NOVSYNCHW:
        tig_debug_println("DDERR_NOVSYNCHW");
        break;
    case DDERR_NOZBUFFERHW:
        tig_debug_println("DDERR_NOZBUFFERHW");
        break;
    case DDERR_NOZOVERLAYHW:
        tig_debug_println("DDERR_NOZOVERLAYHW");
        break;
    case DDERR_OUTOFCAPS:
        tig_debug_println("DDERR_OUTOFCAPS");
        break;
    case DDERR_OUTOFMEMORY:
        tig_debug_println("DDERR_OUTOFMEMORY");
        break;
    case DDERR_OUTOFVIDEOMEMORY:
        tig_debug_println("DDERR_OUTOFVIDEOMEMORY");
        break;
    case DDERR_OVERLAYCANTCLIP:
        tig_debug_println("DDERR_OVERLAYCANTCLIP");
        break;
    case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
        tig_debug_println("DDERR_OVERLAYCOLORKEYONLYONEACTIVE");
        break;
    case DDERR_OVERLAYNOTVISIBLE:
        tig_debug_println("DDERR_OVERLAYNOTVISIBLE");
        break;
    case DDERR_PALETTEBUSY:
        tig_debug_println("DDERR_PALETTEBUSY");
        break;
    case DDERR_PRIMARYSURFACEALREADYEXISTS:
        tig_debug_println("DDERR_PRIMARYSURFACEALREADYEXISTS");
        break;
    case DDERR_REGIONTOOSMALL:
        tig_debug_println("DDERR_REGIONTOOSMALL");
        break;
    case DDERR_SURFACEALREADYATTACHED:
        tig_debug_println("DDERR_SURFACEALREADYATTACHED");
        break;
    case DDERR_SURFACEALREADYDEPENDENT:
        tig_debug_println("DDERR_SURFACEALREADYDEPENDENT");
        break;
    case DDERR_SURFACEBUSY:
        tig_debug_println("DDERR_SURFACEBUSY");
        break;
    case DDERR_SURFACEISOBSCURED:
        tig_debug_println("DDERR_SURFACEISOBSCURED");
        break;
    case DDERR_SURFACELOST:
        tig_debug_println("DDERR_SURFACELOST");
        break;
    case DDERR_SURFACENOTATTACHED:
        tig_debug_println("DDERR_SURFACENOTATTACHED");
        break;
    case DDERR_TOOBIGHEIGHT:
        tig_debug_println("DDERR_TOOBIGHEIGHT");
        break;
    case DDERR_TOOBIGSIZE:
        tig_debug_println("DDERR_TOOBIGSIZE");
        break;
    case DDERR_TOOBIGWIDTH:
        tig_debug_println("DDERR_TOOBIGWIDTH");
        break;
    case DDERR_UNSUPPORTED:
        tig_debug_println("DDERR_UNSUPPORTED");
        break;
    case DDERR_UNSUPPORTEDFORMAT:
        tig_debug_println("DDERR_UNSUPPORTEDFORMAT");
        break;
    case DDERR_UNSUPPORTEDMASK:
        tig_debug_println("DDERR_UNSUPPORTEDMASK");
        break;
    case DDERR_UNSUPPORTEDMODE:
        tig_debug_println("DDERR_UNSUPPORTEDMODE");
        break;
    case DDERR_VERTICALBLANKINPROGRESS:
        tig_debug_println("DDERR_VERTICALBLANKINPROGRESS");
        break;
    case DDERR_VIDEONOTACTIVE:
        tig_debug_println("DDERR_VIDEONOTACTIVE");
        break;
    case DDERR_WASSTILLDRAWING:
        tig_debug_println("DDERR_WASSTILLDRAWING");
        break;
    case DDERR_WRONGMODE:
        tig_debug_println("DDERR_WRONGMODE");
        break;
    case DDERR_XALIGN:
        tig_debug_println("DDERR_XALIGN");
        break;
    default:
        tig_debug_println("Unknown HRESULT from DDRAW error\n");
        break;
    }
}

// 0x525ED0
static int sub_525ED0(TigVideoBufferData* video_buffer_data, TigRect* rect, const char* file_name, bool a4)
{
    // TODO: Incomplete.
    (void)video_buffer_data;
    (void)rect;
    (void)file_name;
    (void)a4;

    return 0;
}

// 0x526450
int tig_video_3d_set_viewport(TigVideoBuffer* video_buffer)
{
    HRESULT hr;
    D3DVIEWPORT7 viewport;

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

// 0x526530
static void sub_526530(const TigRect* a1, const TigRect* a2, D3DCOLOR* alpha)
{
    // TODO: Incomplete.
    (void)a1;
    (void)a2;
    (void)alpha;
}

// 0x526690
static void sub_526690(TigRect* a, TigRect* b, D3DCOLOR* color)
{
    // TODO: Incomplete.
    (void)a;
    (void)b;
    (void)color;
}
