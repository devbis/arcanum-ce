#include "game/gfade.h"

#include "game/li.h"

// 0x5FC4B0
static IsoInvalidateRectFunc* gfade_iso_invalidate_rect;

// 0x5FC4BC
static IsoRedrawFunc* gfade_iso_draw;

// 0x4BDE70
bool gfade_init(GameInitInfo* init_info)
{
    gfade_iso_invalidate_rect = init_info->invalidate_rect_func;
    gfade_iso_draw = init_info->draw_func;

    return true;
}

// 0x4BDF60
void gfade_exit()
{
}

// 0x4BDF80
void gfade_resize(GameResizeInfo* resize_info)
{
}

// 0x4BDFA0
void gfade_run(FadeData* fade_data)
{
    tig_video_fade(fade_data->color, fade_data->steps, fade_data->duration, fade_data->flags);
}

// 0x4BE050
bool gfade_timeevent_process(TimeEvent* timeevent)
{
    FadeData fade_data;

    fade_data.flags = timeevent->params[0].integer_value;
    fade_data.color = timeevent->params[1].integer_value;
    fade_data.duration = timeevent->params[2].float_value;
    fade_data.steps = timeevent->params[3].integer_value;

    if ((fade_data.flags & FADE_IN) != 0) {
        gfade_iso_invalidate_rect(NULL);
        gfade_iso_draw();
        tig_window_display();
        sub_4BBC00();
    }

    gfade_run(&fade_data);

    return true;
}
