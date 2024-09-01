#include "game/tc.h"

// 0x5FF4F8
static TigRect stru_5FF4F8;

// 0x5FF518
static TigVideoBuffer* dword_5FF518;

// 0x5FF530
static tig_font_handle_t dword_5FF530;

// 0x5FF534
static GameContextF8* dword_5FF534;

// 0x5FF538
static tig_font_handle_t dword_5FF538;

// 0x5FF53C
static TigVideoBuffer* dword_5FF53C;

// 0x5FF548
static bool tc_editor;

// 0x5FF54C
static tig_font_handle_t dword_5FF54C;

// 0x5FF550
static tig_window_handle_t dword_5FF550;

// 0x5FF568
static bool dword_5FF568;

// 0x4C9280
void tc_init()
{
    // TODO: Incomplete.
}

// 0x4C9540
void tc_exit()
{
    if (!tc_editor) {
        tig_font_destroy(dword_5FF54C);
        tig_font_destroy(dword_5FF530);
        tig_font_destroy(dword_5FF538);
        tig_video_buffer_destroy(dword_5FF53C);
        tig_video_buffer_destroy(dword_5FF518);
        dword_5FF550 = TIG_WINDOW_HANDLE_INVALID;
        dword_5FF534 = NULL;
    }
}

// 0x4C95A0
void tc_resize()
{
    // TODO: Incomplete.
}

// 0x4C95F0
void sub_4C95F0()
{
    // TODO: Incomplete.
}

// 0x4C9620
void sub_4C9620()
{
    // TODO: Incomplete.
}

// 0x4C96C0
void sub_4C96C0()
{
    // TODO: Incomplete.
}

// 0x4C96F0
void sub_4C96F0()
{
    if (!tc_editor) {
        if (dword_5FF568) {
            dword_5FF568 = false;
            dword_5FF534(&stru_5FF4F8);
        }
    }
}

// 0x4C9720
void sub_4C9720()
{
    // TODO: Incomplete.
}

// 0x4C9810
void sub_4C9810()
{
    // TODO: Incomplete.
}

// 0x4C9A10
void sub_4C9A10()
{
    // TODO: Incomplete.
}

// 0x4C9B90
void sub_4C9B90()
{
    // TODO: Incomplete.
}

// 0x4C9BE0
void sub_4C9BE0()
{
    // TODO: Incomplete.
}
