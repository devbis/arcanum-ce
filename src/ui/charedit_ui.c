#include "ui/charedit_ui.h"

// 0x64C7B0
static tig_window_handle_t dword_64C7B0;

// 0x64CA60
static tig_window_handle_t dword_64CA60;

// 0x64CA6C
static tig_window_handle_t dword_64CA6C;

// 0x64CA8C
static tig_window_handle_t dword_64CA8C;

// 0x559690
bool charedit_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!sub_55E110()) {
        return false;
    }

    if (!sub_55D060()) {
        sub_55EBA0();
        return false;
    }

    if (!sub_55C890()) {
        sub_55EBA0();
        tig_window_destroy(dword_64CA60);
        return false;
    }

    if (!sub_55C110()) {
        sub_55EBA0();
        tig_window_destroy(dword_64C7B0);
        tig_window_destroy(dword_64CA60);
        return false;
    }

    if (!sub_55BAB0()) {
        sub_55EBA0();
        tig_window_destroy(dword_64C7B0);
        tig_window_destroy(dword_64CA8C);
        tig_window_destroy(dword_64CA60);
        return false;
    }

    tig_window_hide(dword_64CA6C);
    tig_window_hide(dword_64C7B0);
    tig_window_hide(dword_64CA8C);
    tig_window_hide(dword_64CA60);

    return true;
}

// 0x559770
void charedit_exit()
{
    // TODO: Incomplete.
}

// 0x5597B0
void charedit_reset()
{
    // TODO: Incomplete.
}

// 0x5597C0
void sub_5597C0()
{
    // TODO: Incomplete.
}

// 0x55A150
void sub_55A150()
{
    // TODO: Incomplete.
}

// 0x55A220
void sub_55A220()
{
    // TODO: Incomplete.
}

// 0x55A230
void sub_55A230()
{
    // TODO: Incomplete.
}

// 0x55A240
void sub_55A240()
{
    // TODO: Incomplete.
}

// 0x55A5C0
void sub_55A5C0()
{
    // TODO: Incomplete.
}

// 0x55AE70
void sub_55AE70()
{
    // TODO: Incomplete.
}

// 0x55B0E0
void sub_55B0E0()
{
    // TODO: Incomplete.
}

// 0x55B150
void sub_55B150()
{
    // TODO: Incomplete.
}

// 0x55B1B0
void sub_55B1B0()
{
    // TODO: Incomplete.
}

// 0x55B280
void sub_55B280()
{
    // TODO: Incomplete.
}

// 0x55B2A0
void sub_55B2A0()
{
    // TODO: Incomplete.
}

// 0x55B410
void sub_55B410()
{
    // TODO: Incomplete.
}

// 0x55B4D0
void sub_55B4D0()
{
    // TODO: Incomplete.
}

// 0x55B720
void sub_55B720()
{
    // TODO: Incomplete.
}

// 0x55B880
void sub_55B880()
{
    // TODO: Incomplete.
}

// 0x55BAB0
void sub_55BAB0()
{
    // TODO: Incomplete.
}

// 0x55BD10
void sub_55BD10()
{
    // TODO: Incomplete.
}

// 0x55BF20
void sub_55BF20()
{
    // TODO: Incomplete.
}

// 0x55C110
void sub_55C110()
{
    // TODO: Incomplete.
}

// 0x55C2E0
void sub_55C2E0()
{
    // TODO: Incomplete.
}

// 0x55C3A0
void sub_55C3A0()
{
    // TODO: Incomplete.
}

// 0x55C890
void sub_55C890()
{
    // TODO: Incomplete.
}

// 0x55CA70
void sub_55CA70()
{
    // TODO: Incomplete.
}

// 0x55CBC0
void spells_print_all()
{
    // TODO: Incomplete.
}

// 0x55D060
void sub_55D060()
{
    // TODO: Incomplete.
}

// 0x55D210
void sub_55D210()
{
    // TODO: Incomplete.
}

// 0x55D3A0
void sub_55D3A0()
{
    // TODO: Incomplete.
}

// 0x55D6F0
void sub_55D6F0()
{
    // TODO: Incomplete.
}

// 0x55D940
void sub_55D940()
{
    // TODO: Incomplete.
}

// 0x55DC60
void sub_55DC60()
{
    // TODO: Incomplete.
}

// 0x55DF90
void sub_55DF90()
{
    // TODO: Incomplete.
}

// 0x55E110
void sub_55E110()
{
    // TODO: Incomplete.
}

// 0x55EBA0
void sub_55EBA0()
{
    // TODO: Incomplete.
}

// 0x55EC90
void sub_55EC90()
{
    // TODO: Incomplete.
}

// 0x55EFB0
void sub_55EFB0()
{
    // TODO: Incomplete.
}

// 0x55EFE0
void sub_55EFE0()
{
    // TODO: Incomplete.
}

// 0x55EFF0
void sub_55EFF0()
{
    // TODO: Incomplete.
}

// 0x55F0D0
void sub_55F0D0()
{
    // TODO: Incomplete.
}

// 0x55F0E0
void sub_55F0E0()
{
    // TODO: Incomplete.
}

// 0x55F110
void sub_55F110()
{
    // TODO: Incomplete.
}

// 0x55F160
void sub_55F160()
{
    // TODO: Incomplete.
}

// 0x55F180
void sub_55F180()
{
    // TODO: Incomplete.
}

// 0x55F1A0
void sub_55F1A0()
{
    // TODO: Incomplete.
}

// 0x55F1E0
void sub_55F1E0()
{
    // TODO: Incomplete.
}

// 0x55F200
void sub_55F200()
{
    // TODO: Incomplete.
}

// 0x55F320
void sub_55F320()
{
    // TODO: Incomplete.
}

// 0x55F340
void sub_55F340()
{
    // TODO: Incomplete.
}

// 0x55F360
void sub_55F360()
{
    // TODO: Incomplete.
}

// 0x55F450
void sub_55F450()
{
    // TODO: Incomplete.
}

// 0x55F5F0
void sub_55F5F0()
{
    // TODO: Incomplete.
}
