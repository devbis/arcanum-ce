#include "ui/scrollbar_ui.h"

#include <tig/tig.h>

#define MAX_SCROLLBARS 8

// 0x5CBF48
static TigRect stru_5CBF48 = { 0, 0, 11, 5 };

// 0x5CBF58
static TigRect stru_5CBF58 = { 0, 0, 11, 7 };

// 0x5CBF68
static TigRect stru_5CBF68 = { 0, 0, 11, 1 };

// 0x5CBF78
static int dword_5CBF78 = -1;

// 0x684254
static tig_art_id_t scrollbar_ui_middle_art_id;

// 0x68425C
static bool initialized;

// 0x684260
static tig_art_id_t scrollbar_ui_top_art_id;

// 0x684268
static Scrollbar scrollbars[MAX_SCROLLBARS];

// 0x684668
static tig_art_id_t scrollbar_ui_bottom_art_id;

// 0x684674
static int dword_684674;

// 0x684678
static int dword_684678;

// 0x684684
static int dword_684684;

// 0x580410
bool scrollbar_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    scrollbar_reset();

    if (tig_art_interface_id_create(238, 0, 0, 0, &scrollbar_ui_top_art_id) != TIG_OK) {
        return false;
    }

    if (tig_art_interface_id_create(240, 0, 0, 0, &scrollbar_ui_bottom_art_id) != TIG_OK) {
        return false;
    }

    if (tig_art_interface_id_create(787, 0, 0, 0, &scrollbar_ui_middle_art_id) != TIG_OK) {
        return false;
    }

    initialized = true;

    return true;
}

// 0x580480
void scrollbar_ui_exit()
{
    initialized = false;
}

// 0x580490
void scrollbar_ui_reset()
{
    int index;

    for (index = 0; index < MAX_SCROLLBARS; index++) {
        scrollbar_obj_reset(&(scrollbars[index]));
    }

    dword_684678 = random_between(0, 8192);
    dword_5CBF78 = -1;
    dword_684684 = 0;
}

// 0x5804E0
void scrollbar_create()
{
    // TODO: Incomplete.
}

// 0x580690
void scrollbar_destroy()
{
    // TODO: Incomplete.
}

// 0x5806F0
void sub_5806F0()
{
    // TODO: Incomplete.
}

// 0x580720
void sub_580720()
{
    // TODO: Incomplete.
}

// 0x580780
void sub_580780()
{
    // TODO: Incomplete.
}

// 0x5807F0
void sub_5807F0()
{
    // TODO: Incomplete.
}

// 0x580B10
void sub_580B10()
{
    // TODO: Incomplete.
}

// 0x5810D0
void sub_5810D0()
{
    // TODO: Incomplete.
}

// 0x581180
void sub_581180()
{
    dword_684684++;
}

// 0x581190
void sub_581190()
{
    dword_684684--;
}

// 0x5811A0
void sub_5811A0()
{
    dword_684684 = 1;
}

// 0x5811B0
void sub_5811B0()
{
    dword_684684 = 0;
}

// 0x5811C0
void scrollbar_entry_reset()
{
    // TODO: Incomplete.
}

// 0x5811F0
void sub_5811F0()
{
    // TODO: Incomplete.
}

// 0x581280
void sub_581280()
{
    // TODO: Incomplete.
}

// 0x5812E0
void sub_5812E0()
{
    // TODO: Incomplete.
}

// 0x581360
void sub_581360()
{
    // TODO: Incomplete.
}

// 0x5813E0
void sub_5813E0()
{
    // TODO: Incomplete.
}

// 0x581460
void sub_581460()
{
    // TODO: Incomplete.
}

// 0x5814E0
void sub_5814E0()
{
    // TODO: Incomplete.
}

// 0x581550
void sub_581550()
{
    // TODO: Incomplete.
}

// 0x5815A0
void sub_5815A0()
{
    // TODO: Incomplete.
}

// 0x5815D0
void sub_5815D0()
{
    // TODO: Incomplete.
}

// 0x581660
void sub_581660()
{
    // TODO: Incomplete.
}

// 0x5816A0
void sub_5816A0()
{
    // TODO: Incomplete.
}

// 0x5816D0
void sub_5816D0()
{
    dword_684674++;
}

// 0x5816E0
void sub_5816E0()
{
    dword_684674--;
}

// 0x5816F0
int sub_5816F0()
{
    return dword_684674;
}
