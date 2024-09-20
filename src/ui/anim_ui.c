#include "ui/anim_ui.h"

#include "game/light_scheme.h"
#include "game/timeevent.h"
#include "ui/intgame.h"
#include "ui/mainmenu_ui.h"
#include "ui/mp_ctrl_ui.h"
#include "ui/sleep_ui.h"
#include "ui/wmap_ui.h"

static bool sub_57D3B0(TimeEvent* timeevent);

// 0x5CB408
static bool dword_5CB408;

// 0x5CB40C
static int dword_5CB40C;

// 0x5CB410
static int dword_5CB410;

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
void anim_ui_bkg_process_callback()
{
    // TODO: Incomplete.
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
void ambient_lighting_process_callback()
{
    int hour;
    DateTime datetime;
    TimeEvent timeevent;

    hour = datetime_current_hour();
    if (sub_57D650()) {
        light_scheme_set_hour(hour);
    }

    timeevent_clear_all_typed(TIMEEVENT_TYPE_AMBIENT_LIGHTING);
    timeevent.type = TIMEEVENT_TYPE_AMBIENT_LIGHTING;
    sub_45A950(&datetime, 3600000);
    sub_45B800(&timeevent, &datetime);

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
