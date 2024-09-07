#include "game/gfade.h"

#include "game/li.h"

// 0x5FC4AC
static bool fade_have_gamma_control;

// 0x5FC4B0
static GameContextF8* dword_5FC4B0;

// 0x5FC4B4
static TigVideoBuffer* fade_iso_video_buffer;

// 0x5FC4B8
static tig_font_handle_t dword_5FC4B8;

// 0x5FC4BC
static GameContextFC* dword_5FC4BC;

// 0x5FC4C0
static int fade_iso_window_handle;

// 0x4BDE70
int gfade_init(GameContext* init_info)
{
    TigFont font;

    fade_iso_window_handle = init_info->iso_window_handle;
    dword_5FC4B0 = init_info->field_8;
    dword_5FC4BC = init_info->field_C;

    if (tig_window_vbid_get(fade_iso_window_handle, &fade_iso_video_buffer) != TIG_OK) {
        return false;
    }

    if (tig_video_check_gamma_control() != TIG_OK) {
        fade_have_gamma_control = false;

        font.flags = TIG_FONT_SHADOW | TIG_FONT_CENTERED | TIG_FONT_BLEND_ALPHA_SRC;
        tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
        font.color = tig_color_make(255, 255, 255);
        tig_font_create(&font, &dword_5FC4B8);
    } else {
        fade_have_gamma_control = true;
    }

    return true;
}

// 0x4BDF60
void gfade_exit()
{
    if (!fade_have_gamma_control) {
        tig_font_destroy(dword_5FC4B8);
    }
}

// 0x4BDF80
void gfade_resize(ResizeInfo* resize_info)
{
    fade_iso_window_handle = resize_info->iso_window_handle;
    tig_window_vbid_get(fade_iso_window_handle, &fade_iso_video_buffer);
}

// 0x4BDFA0
void sub_4BDFA0(FadeData* fade_data)
{
    tig_timestamp_t time;

    if (fade_have_gamma_control) {
        tig_video_fade(fade_data->color, fade_data->steps, fade_data->duration, fade_data->field_0);
    } else {
        tig_timer_now(&time);
        tig_window_fill(fade_iso_window_handle, 0, 0);
        tig_window_display();

        while (fade_data->duration * 1000.0 > tig_timer_elapsed(time)) {
        }

        dword_5FC4B0(NULL);
    }
}

// 0x4BE050
bool gfade_timeevent_process(TimeEvent* timeevent)
{
    FadeData fade_data;

    fade_data.field_0 = timeevent->params[0].integer_value;
    fade_data.color = timeevent->params[1].integer_value;
    fade_data.duration = timeevent->params[2].float_value;
    fade_data.steps = timeevent->params[3].integer_value;
    fade_data.field_10 = 0;

    if ((fade_data.field_0 & 1) != 0) {
        dword_5FC4B0(NULL);
        dword_5FC4BC();
        tig_window_display();
        sub_4BBC00();
    }

    sub_4BDFA0(&fade_data);

    return true;
}
