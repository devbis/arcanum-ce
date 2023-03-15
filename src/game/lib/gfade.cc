#include "game/lib/gfade.h"

#include "tig/art.h"
#include "tig/color.h"
#include "tig/felicity.h"
#include "tig/timer.h"
#include "tig/video.h"
#include "tig/window.h"

// 0x5FC4AC
static bool fade_have_gamma_control;

// 0x5FC4B0
static GameContextF8* dword_5FC4B0;

// 0x5FC4B4
static TigVideoBuffer* fade_iso_video_buffer;

// 0x5FC4B8
static Felicity* dword_5FC4B8;

// 0x5FC4BC
static GameContextFC* dword_5FC4BC;

// 0x5FC4C0
static int fade_iso_window_handle;

// 0x4BDE70
int gfade_init(GameContext* ctx)
{
    fade_iso_window_handle = ctx->iso_window_handle;
    dword_5FC4B0 = ctx->field_8;
    dword_5FC4BC = ctx->field_C;

    if (tig_window_vbid_get(fade_iso_window_handle, &fade_iso_video_buffer) != TIG_OK) {
        return false;
    }

    if (sub_51FC90() != TIG_OK) {
        fade_have_gamma_control = false;

        Felicity v1;
        v1.field_0 = 152;
        tig_art_interface_id_create(229, 0, 0, 0, &(v1.art_id));
        v1.field_8 = tig_color_rgb_make(255, 255, 255);
        tig_felicity_create(&v1, &dword_5FC4B8);
    } else {
        fade_have_gamma_control = true;
    }

    return true;
}

// 0x4BDF60
void gfade_exit()
{
    if (!fade_have_gamma_control) {
        tig_felicity_destroy(dword_5FC4B8);
    }
}

// 0x4BDF80
void gfade_resize(ResizeContext* ctx)
{
    fade_iso_window_handle = ctx->iso_window_handle;
    tig_window_vbid_get(fade_iso_window_handle, &fade_iso_video_buffer);
}

// 0x4BDFA0
void sub_4BDFA0(FadeData* fade_data)
{
    if (fade_have_gamma_control) {
        sub_51FCA0(fade_data->field_4, fade_data->field_8, fade_data->duration, fade_data.field_0);
    } else {
        unsigned int time;
        tig_timer_start(&time);
        tig_window_fill(fade_iso_window_handle, 0, 0);
        tig_window_display();

        while (fade_data->duration * 1000.0 > tig_timer_elapsed(&time)) {
        }

        dword_5FC4B0(0);
    }
}

// 0x4BE050
void sub_4BE050()
{
    // TODO: Incomplete.
}
