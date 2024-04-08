#ifndef ARCANUM_MAIN_H_
#define ARCANUM_MAIN_H_

#include "game/lib/scroll.h"
#include "tig/dxinput.h"
#include "tig/kb.h"
#include "tig/net.h"
#include "tig/mouse.h"
#include "tig/tig.h"

static void handle_mouse_scroll();
static void handle_keyboard_scroll();

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    TigInitializeInfo ctx;
    ctx.texture_width = 1024;
    ctx.texture_height = 1024;
    ctx.flags = TIG_CONTEXT_FLAG_3D_HARDWARE | TIG_CONTEXT_FLAG_3D_SOFTWARE | TIG_CONTEXT_FLAG_0x04;

    char* pch = lpCmdLine;
    while (*pch != '\0') {
        *pch = tolower(*pch);
        pch++;
    }

    if (strstr(lpCmdLine, "-fps") != NULL) {
        ctx.flags |= TIG_CONTEXT_FLAG_FPS;
    }

    if (strstr(lpCmdLine, "-nosound") != NULL) {
        ctx.flags |= TIG_CONTEXT_FLAG_NO_SOUND;
    }

    if (strstr(lpCmdLine, "-no3d") != NULL) {
        ctx.flags &= ~(TIG_CONTEXT_FLAG_ANY_3D | TIG_CONTEXT_FLAG_0x04);
    }

    if (strstr(lpCmdLine, "-3dref") != NULL) {
        ctx.flags &= ~(TIG_CONTEXT_FLAG_3D_HARDWARE | TIG_CONTEXT_FLAG_3D_SOFTWARE);
        ctx.flags |= TIG_CONTEXT_FLAG_3D_REF_DEVICE;
    }

    if (strstr(lpCmdLine, "-doublebuffer") != NULL) {
        ctx.flags |= TIG_CONTEXT_FLAG_DOUBLE_BUFFER;
    }

    pch = strstr(lpCmdLine, "-vidfreed");
    if (pch != NULL) {
        int value = atoi(pch + 8);
        if (value > 0) {
            tig_art_cache_set_video_memory_fullness(value);
        }
    }

    if (strstr(lpCmdLine, "-animcatchup") != NULL) {
        sub_423FC0();
    }

    if (strstr(lpCmdLine, "-animdebug") != NULL) {
        sub_4232F0();
    }

    if (strstr(lpCmdLine, "-norandom") != NULL) {
        sub_5589D0();
    }

    if (strstr(lpCmdLine, "-nonmsmousez") != NULL) {
        tig_mouse_set_z_axis_enabled(false);
    } else if (strstr(lpCmdLine, "-msmousez") != NULL) {
        tig_mouse_set_z_axis_enabled(true);
    }

    sub_549A70();

    pch = strstr(lpCmdLine, "-pathlimit");
    if (pch != NULL) {
        int value = atoi(pch + 10);
        if (value > 0) {
            sub_421AC0(value);
        }
    }

    pch = strstr(lpCmdLine, "-pathtimelimit");
    if (pch != NULL) {
        // FIXME: Length is wrong, should be 14.
        int value = atoi(pch + 10);
        if (value > 0) {
            sub_421AE0(value);
        }
    }

    if (strstr(lpCmdLine, "-mpautojoin") != NULL) {
        tig_net_auto_join_enable();
    }

    if (strstr(lpCmdLine, "-mpnobcast") != NULL) {
        tig_net_no_broadcast_enable();
    }

    if (strstr(lpCmdLine, "-fullscreen") != NULL) {
        // TODO: Incomplete.
    }

    if (strstr(lpCmdLine, "-patchlvl") != NULL) {
        // TODO: Incomplete.
    }

    ctx.width = 800;
    ctx.height = 600;
    ctx.bpp = 16;
    ctx.instance = hInstance;

    // TODO: Incomplete.

    if (tig_init(&ctx) != TIG_OK) {
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

// 0x401F50
void handle_mouse_scroll()
{
    TigMouseState mouse_state;
    tig_mouse_get_state(&mouse_state);

    if (mouse_state.x == 0) {
        if (mouse_state.y == 0) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP_LEFT);
        } else if (mouse_state.y == 600 - 1) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN_LEFT);
        } else {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_LEFT);
        }
    } else if (mouse_state.x == 800 - 1) {
        if (mouse_state.y == 0) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP_RIGHT);
        } else if (mouse_state.y == 600 - 1) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN_RIGHT);
        } else {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_RIGHT);
        }
    } else {
        if (mouse_state.y == 0) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP);
        } else if (mouse_state.y == 600 - 1) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN);
        } else {
            scroll_stop_scrolling();
        }
    }
}

// 0x402010
void handle_keyboard_scroll()
{
    if (tig_kb_is_key_pressed(DIK_UP)) {
        if (tig_kb_is_key_pressed(DIK_LEFT)) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP_LEFT);
        } else if (tig_kb_is_key_pressed(DIK_RIGHT)) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP_RIGHT);
        } else {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_UP);
        }
    } else if (tig_kb_is_key_pressed(DIK_DOWN)) {
        if (tig_kb_is_key_pressed(DIK_LEFT)) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN_LEFT);
        } else if (tig_kb_is_key_pressed(DIK_RIGHT)) {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN_RIGHT);
        } else {
            scroll_start_scrolling_in_direction(SCROLL_DIRECTION_DOWN);
        }
    } else if (tig_kb_is_key_pressed(DIK_LEFT)) {
        scroll_start_scrolling_in_direction(SCROLL_DIRECTION_LEFT);
    } else if (tig_kb_is_key_pressed(DIK_RIGHT)) {
        scroll_start_scrolling_in_direction(SCROLL_DIRECTION_RIGHT);
    }
}

#endif /* ARCANUM_MAIN_H_ */
