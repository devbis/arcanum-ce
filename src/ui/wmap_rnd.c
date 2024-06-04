#include "ui/wmap_rnd.h"

#include <tig/tig.h>

#include "game/lib/random.h"
#include "game/lib/timeevent.h"
#include "ui/sleep_ui.h"

// 0x5581B0
bool wmap_rnd_init(GameInitInfo* init_info)
{
    (void)init_info;

    return true;
}

// 0x5581C0
void wmap_rnd_exit()
{
}

// 0x5581D0
void wmap_rnd_reset()
{
}

// 0x5581E0
void wmap_rnd_mod_load()
{
    // TODO: Incomplete.
}

// 0x558700
void wmap_rnd_mod_unload()
{
    // TODO: Incomplete.
}

// 0x558730
void wmap_rnd_save()
{
    // TODO: Incomplete.
}

// 0x558880
void wmap_rnd_load()
{
    // TODO: Incomplete.
}

// 0x5589D0
void sub_5589D0()
{
    dword_64C794 = 1;
}

// 0x5589E0
bool wmap_rnd_terrain_clear(int a1)
{
    int v1;
    int v2;

    v1 = sub_4E8DC0(a1);
    v2 = sub_4E8DD0(a1);
    if (v1 < 0 || v1 >= 19
        || v2 < 0 || v2 >= 19) {
        tig_debug_println("Error:  Unknown terrain encountered in wmap_rnd_terrain_clear");
        return false;
    }

    if (dword_5C79C4[v1] == 0
        || dword_5C79C4[v2] == 0) {
        return false;
    }

    return true;
}

// 0x558A40
void sub_558A40()
{
    // TODO: Incomplete.
}

// 0x558AC0
void sub_558AC0()
{
    // TODO: Incomplete.
}

// 0x558AD0
void sub_558AD0()
{
    // TODO: Incomplete.
}

// 0x558AF0
void sub_558AF0()
{
    // TODO: Incomplete.
}

// 0x558B50
void sub_558B50()
{
    // TODO: Incomplete.
}

// 0x558B80
void sub_558B80()
{
    // TODO: Incomplete.
}

// 0x558C90
void sub_558C90()
{
    // TODO: Incomplete.
}

// 0x558CE0
void sub_558CE0()
{
    // TODO: Incomplete.
}

// 0x558D00
void sub_558D00()
{
    // TODO: Incomplete.
}

// 0x558D20
void sub_558D20()
{
    // TODO: Incomplete.
}

// 0x558D40
void sub_558D40()
{
    // TODO: Incomplete.
}

// 0x558DE0
void sub_558DE0()
{
    // TODO: Incomplete.
}

// 0x558F30
void sub_558F30()
{
    // TODO: Incomplete.
}

// 0x558FB0
int wmap_rnd_determine_terrain(long long location)
{
    long long v1;
    int v2;
    int v3;

    v1 = sub_4CFC50(location);
    v2 = sub_4E87F0(v1);
    if (!wmap_rnd_terrain_clear(v2)) {
        return 0;
    }

    v3 = sub_4E8DC0(v2);
    if (v3 < 0 || v3 >= 19) {
        tig_debug_println("Error:  Unknown terrain base encountered inwmap_rnd_determine_terrain");
        return 0;
    }

    return dword_5C79C4[v3];
}

// 0x559010
void wmap_rnd_encounter_check()
{
    // TODO: Incomplete.
}

// 0x559150
void sub_559150()
{
    // TODO: Incomplete.
}

// 0x5591B0
void sub_5591B0()
{
    // TODO: Incomplete.
}

// 0x559200
void sub_559200()
{
    // TODO: Incomplete.
}

// 0x559260
void sub_559260()
{
    // TODO: Incomplete.
}

// 0x5594E0
void sub_5594E0()
{
    // TODO: Incomplete.
}

// 0x559550
void wmap_rnd_encounter_build_object()
{
    // TODO: Incomplete.
}

// 0x5595B0
bool sub_5595B0()
{
    long long location;

    sub_559640();

    if (sub_40FF40() == sub_40FF50(1)
        && !sub_4CB6A0(player_get_pc_obj())) {
        sub_566CC0(&location);
        if (sub_558DE0(location)) {
            if (sub_560F90()) {
                sub_560F40();
                sub_560720();
            } else {
                if (sleep_ui_is_created()) {
                    sub_57B450();
                }
            }
        }
    }

    return true;
}

// 0x559640
void sub_559640()
{
    TimeEvent timeevent;
    DateTime datetime;

    timeevent.type = TIMEEVENT_TYPE_RANDOM_ENCOUNTER;
    sub_45A950(&datetime, 60000 * random_between(300, 700));
    sub_45B800(&timeevent, &datetime);
}
