#include "ui/matchmaker_ui.h"

#include "game/multiplayer.h"
#include "ui/intgame.h"

static bool matchmaker_ui_create();
static bool matchmaker_ui_message_filter(TigMessage* msg);

// 0x5CA308
static tig_window_handle_t matchmaker_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5CA310
static TigRect stru_5CA310 = { 0, 0, 800, 600 };

// 0x67BB0C
static bool matchmaker_ui_initialized;

// 0x67BB10
static bool matchmaker_ui_created;

// 0x569AC0
bool matchmaker_ui_init(GameInitInfo* init_info)
{
    (void)init_info;

    matchmaker_ui_initialized = true;

    return true;
}

// 0x569AD0
void matchmaker_ui_exit()
{
    matchmaker_ui_initialized = false;
}

// 0x569AE0
void matchmaker_ui_reset()
{
}

// 0x569AF0
void sub_569AF0()
{
    if (!matchmaker_ui_created) {
        if (sub_551A80(17)) {
            if (tig_net_is_active() || multiplayer_start()) {
                sub_569B50();
            } else {
                tig_debug_printf("MMUI: Could not start multiplayer.. aborting.\n");
            }
        }
    }
}

// 0x569B40
bool sub_569B40()
{
    return sub_551A80(0);
}

// 0x569B50
bool sub_569B50()
{
    matchmaker_ui_create();

    return true;
}

// 0x569B60
bool matchmaker_ui_create()
{
    tig_art_id_t art_id;
    TigWindowData window_data;
    TigRect rect;
    TigArtBlitInfo blit_info;

    if (!matchmaker_ui_created) {
        if (tig_art_interface_id_create(218, 0, 0, 0, &art_id) != TIG_OK) {
            return false;
        }

        window_data.flags = TIG_WINDOW_MESSAGE_FILTER;
        window_data.rect.x = stru_5CA310.x + 1;
        window_data.rect.y = stru_5CA310.y + 1;
        window_data.rect.width = stru_5CA310.width - 1;
        window_data.rect.height = stru_5CA310.height - 1;
        window_data.message_filter = matchmaker_ui_message_filter;

        if (tig_window_create(&window_data, &matchmaker_ui_window) != TIG_OK) {
            // FIXME: Should be EXIT_FAILURE.
            exit(EXIT_SUCCESS);
        }

        // FIXME: Incomplete, width/height are never set.
        rect.x = 0;
        rect.y = 0;

        // FIXME: Incomplete, flags are not set.
        blit_info.art_id = art_id;
        blit_info.src_rect = &rect;
        blit_info.dst_rect = &rect;
        tig_window_blit_art(matchmaker_ui_window, &blit_info);

        matchmaker_ui_created = true;
    }

    return true;
}

// 0x569C30
bool matchmaker_ui_message_filter(TigMessage* msg)
{
    (void)msg;

    return true;
}
