#include "game/gfade.h"

#include "game/li.h"

// 0x5FC4AC
static bool gfade_have_gamma_control;

// 0x5FC4B0
static IsoInvalidateRectFunc* gfade_iso_invalidate_rect;

// 0x5FC4B4
static TigVideoBuffer* fade_iso_video_buffer;

// 0x5FC4B8
static tig_font_handle_t dword_5FC4B8;

// 0x5FC4BC
static IsoRedrawFunc* gfade_iso_redraw;

// 0x5FC4C0
static int gfade_iso_window_handle;

// 0x4BDE70
bool gfade_init(GameInitInfo* init_info)
{
    TigFont font;

    gfade_iso_window_handle = init_info->iso_window_handle;
    gfade_iso_invalidate_rect = init_info->invalidate_rect_func;
    gfade_iso_redraw = init_info->redraw_func;

    if (tig_window_vbid_get(gfade_iso_window_handle, &fade_iso_video_buffer) != TIG_OK) {
        return false;
    }

    if (tig_video_check_gamma_control() != TIG_OK) {
        gfade_have_gamma_control = false;

        font.flags = TIG_FONT_SHADOW | TIG_FONT_CENTERED | TIG_FONT_NO_ALPHA_BLEND;
        tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
        font.color = tig_color_make(255, 255, 255);
        tig_font_create(&font, &dword_5FC4B8);
    } else {
        gfade_have_gamma_control = true;
    }

    return true;
}

// 0x4BDF60
void gfade_exit()
{
    if (!gfade_have_gamma_control) {
        tig_font_destroy(dword_5FC4B8);
    }
}

// 0x4BDF80
void gfade_resize(GameResizeInfo* resize_info)
{
    gfade_iso_window_handle = resize_info->window_handle;
    tig_window_vbid_get(gfade_iso_window_handle, &fade_iso_video_buffer);
}

// 0x4BDFA0
void gfade_run(FadeData* fade_data)
{
    tig_timestamp_t time;

    if (gfade_have_gamma_control) {
        tig_video_fade(fade_data->color, fade_data->steps, fade_data->duration, fade_data->flags);
    } else {
        tig_timer_now(&time);
        tig_window_fill(gfade_iso_window_handle, NULL, 0);
        tig_window_display();

        while (fade_data->duration * 1000.0 > tig_timer_elapsed(time)) {
        }

        gfade_iso_invalidate_rect(NULL);
    }
}

// 0x4BE050
bool gfade_timeevent_process(TimeEvent* timeevent)
{
    FadeData fade_data;

    fade_data.flags = timeevent->params[0].integer_value;
    fade_data.color = timeevent->params[1].integer_value;
    fade_data.duration = timeevent->params[2].float_value;
    fade_data.steps = timeevent->params[3].integer_value;
    fade_data.field_10 = 0;

    if ((fade_data.flags & FADE_IN) != 0) {
        gfade_iso_invalidate_rect(NULL);
        gfade_iso_redraw();
        tig_window_display();
        sub_4BBC00();
    }

    gfade_run(&fade_data);

    return true;
}
