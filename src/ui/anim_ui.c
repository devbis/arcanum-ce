#include "ui/anim_ui.h"

#include "game/critter.h"
#include "game/gamelib.h"
#include "game/gfade.h"
#include "game/light_scheme.h"
#include "game/multiplayer.h"
#include "game/player.h"
#include "game/timeevent.h"
#include "game/ui.h"
#include "ui/combat_ui.h"
#include "ui/compact_ui.h"
#include "ui/gameuilib.h"
#include "ui/intgame.h"
#include "ui/inven_ui.h"
#include "ui/mainmenu_ui.h"
#include "ui/mp_ctrl_ui.h"
#include "ui/sleep_ui.h"
#include "ui/slide_ui.h"
#include "ui/wmap_ui.h"

static bool sub_57D3B0(TimeEvent* timeevent);
static bool anim_ui_bkg_process_callback(TimeEvent* timeevent);
static bool sub_57D650();
static bool ambient_lighting_process_callback(TimeEvent* timeevent);
static void sub_57D6C0();

// 0x5CB408
static bool dword_5CB408 = true;

// 0x5CB40C
static int dword_5CB40C = -1;

// 0x5CB410
static int dword_5CB410 = -1;

// 0x57D240
bool anim_ui_init(GameInitInfo* init_info)
{
    DateTime datetime;
    TimeEvent timeevent;
    TimeEventFuncs callbacks;

    (void)init_info;

    callbacks.bkg_anim_func = anim_ui_bkg_process_callback;
    callbacks.worldmap_func = wmap_ui_bkg_process_callback;
    callbacks.ambient_lighting_func = ambient_lighting_process_callback;
    callbacks.sleeping_func = sleep_ui_process_callback;
    callbacks.clock_func = intgame_clock_process_callback;
    callbacks.mainmenu_func = mainmenu_ui_process_callback;
    callbacks.mp_ctrl_ui_func = mp_ctrl_ui_process_callback;
    timeevent_set_funcs(&callbacks);

    timeevent.type = TIMEEVENT_TYPE_AMBIENT_LIGHTING;
    sub_45A950(&datetime, 1);
    sub_45B800(&timeevent, &datetime);

    return true;
}

// 0x57D2C0
void anim_ui_exit()
{
}

// 0x57D2D0
void anim_ui_reset()
{
    DateTime datetime;
    TimeEvent timeevent;

    timeevent.type = TIMEEVENT_TYPE_AMBIENT_LIGHTING;
    sub_45A950(&datetime, 1);
    sub_45B800(&timeevent, &datetime);
}

// 0x57D300
bool anim_ui_save(TigFile* stream)
{
    (void)stream;

    return true;
}

// 0x57D310
bool anim_ui_load(GameLoadInfo* load_info)
{
    DateTime datetime;
    TimeEvent timeevent;

    (void)load_info;

    timeevent_clear_all_typed(TIMEEVENT_TYPE_AMBIENT_LIGHTING);
    timeevent.type = TIMEEVENT_TYPE_AMBIENT_LIGHTING;
    sub_45A950(&datetime, 3600000);
    sub_45B800(&timeevent, &datetime);

    return true;
}

// 0x57D350
void sub_57D350(int a1, int a2)
{
    sub_57D370(a1, a2, 50);
}

// 0x57D370
void sub_57D370(int a1, int a2, int milliseconds)
{
    DateTime datetime;
    TimeEvent timeevent;

    timeevent.type = TIMEEVENT_TYPE_BKG_ANIM;
    timeevent.params[0].integer_value = a1;
    timeevent.params[1].integer_value = a2;
    sub_45A950(&datetime, milliseconds);
    sub_45B800(&timeevent, &datetime);
}

// 0x57D3B0
bool sub_57D3B0(TimeEvent* timeevent)
{
    return timeevent->params[0].integer_value == dword_5CB40C
        && timeevent->params[1].integer_value == dword_5CB410;
}

// 0x57D3E0
void sub_57D3E0(int list, int a2)
{
    dword_5CB40C = list;
    dword_5CB410 = a2;
    timeevent_clear_one_ex(list, sub_57D3B0);
    dword_5CB40C = -1;
    dword_5CB410 = -1;
}

// 0x57D410
bool anim_ui_bkg_process_callback(TimeEvent* timeevent)
{
    FadeData fade_data;

    switch (timeevent->params[0].integer_value) {
    case 0:
        intgame_draw_bar(INTGAME_BAR_HEALTH);
        break;
    case 1:
        intgame_draw_bar(INTGAME_BAR_FATIGUE);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    case 8:
        sub_552080(timeevent->params[1].integer_value);
        break;
    case 10:
        if (critter_is_dead(player_get_pc_obj())) {
            if (sub_573620() != OBJ_HANDLE_NULL) {
                sub_575770();
            }
            if (sub_541680()) {
                sub_5412D0();
                sub_57D370(10, -1, 300);
            } else {
                slide_ui_run(SLIDE_UI_TYPE_DEATH);

                tig_debug_printf("DEATH: Resetting game!\n");
                gamelib_reset();
                gameuilib_reset();
                mainmenu_ui_start(0);

                fade_data.flags = FADE_IN;
                fade_data.duration = 2.0f;
                fade_data.steps = 48;
                gfade_run(&fade_data);
            }
        }
        break;
    case 11:
        if (sub_573620() != OBJ_HANDLE_NULL) {
            sub_575770();
        }
        if (sub_541680()) {
            sub_5412D0();
            sub_57D370(11, -1, 300);
        } else {
            slide_ui_run(SLIDE_UI_TYPE_END_GAME);
            if (tig_net_is_active()) {
                fade_data.flags = FADE_IN;
                fade_data.duration = 2.0f;
                fade_data.steps = 48;
                gfade_run(&fade_data);

                if (sub_460BB0()) {
                    sub_4A4320();
                }
            }

            tig_debug_printf("EndGame: Resetting game!\n");
            gamelib_reset();
            gameuilib_reset();
            mainmenu_ui_start(0);

            fade_data.flags = FADE_IN;
            fade_data.duration = 2.0f;
            fade_data.steps = 48;
            gfade_run(&fade_data);
        }
        break;
    case 12:
        combat_ui_refresh();
        break;
    case 13:
        wmap_ui_encounter_end();
        break;
    case 14:
        sub_568F20();
        break;
    default:
        tig_debug_printf("AnimUI: anim_ui_bkg_process_callback: ERROR: Failed to match event type!\n");
        break;
    }

    return true;
}

// 0x57D620
void sub_57D620()
{
    if (!dword_5CB408) {
        dword_5CB408 = true;
        sub_57D6C0();
    }
}

// 0x57D640
void sub_57D640()
{
    dword_5CB408 = false;
}

// 0x57D650
bool sub_57D650()
{
    return dword_5CB408;
}

// 0x57D660
bool ambient_lighting_process_callback(TimeEvent* timeevent)
{
    int hour;
    DateTime datetime;
    TimeEvent next_timeevent;

    (void)timeevent;

    hour = datetime_current_hour();
    if (sub_57D650()) {
        light_scheme_set_hour(hour);
    }

    timeevent_clear_all_typed(TIMEEVENT_TYPE_AMBIENT_LIGHTING);
    next_timeevent.type = TIMEEVENT_TYPE_AMBIENT_LIGHTING;
    sub_45A950(&datetime, 3600000);
    sub_45B800(&next_timeevent, &datetime);

    return true;
}

// 0x57D6C0
void sub_57D6C0()
{
    int hour;
    DateTime datetime;
    TimeEvent timeevent;

    hour = datetime_current_hour();
    light_scheme_set_hour(hour);
    timeevent_clear_all_typed(TIMEEVENT_TYPE_AMBIENT_LIGHTING);
    timeevent.type = TIMEEVENT_TYPE_AMBIENT_LIGHTING;
    sub_45A950(&datetime, 3600000);
    sub_45B800(&timeevent, &datetime);
}
