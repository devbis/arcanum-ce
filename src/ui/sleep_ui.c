#include "ui/sleep_ui.h"

#include <tig/tig.h>

#include "game/lib/gfade.h"
#include "game/lib/timeevent.h"

// 0x5CB2B8
static tig_window_handle_t sleep_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x6834E4
static int sleep_ui_mes_file;

// 0x6834E8
static int dword_6834E8;

// 0x6834EC
static bool sleep_ui_initialized;

// 0x6834F0
static bool sleep_ui_created;

// 0x6834A0
static tig_font_handle_t dword_6834A0;

// 0x57B080
bool sleep_ui_init(GameInitInfo* init_info)
{
    TigFont font_info;

    if (!message_load("mes\\sleepUI.mes", &sleep_ui_mes_file)) {
        return false;
    }

    font_info.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_info, &dword_6834A0);

    dword_6834E0 = 0;
    dword_6834B0 = 0;
    sleep_ui_initialized = true;

    return true;
}

// 0x57B140
void sleep_ui_exit()
{
    message_unload(sleep_ui_mes_file);
    tig_font_destroy(dword_6834A0);
    sleep_ui_initialized = 0;
    dword_6834E0 = 0;
    dword_6834B0 = 0;
}

// 0x57B170
void sleep_ui_reset()
{
    if (sleep_ui_created) {
        sub_57B450();
    }
}

// 0x57B180
void sub_57B180()
{
    // TODO: Incomplete.
}

// 0x57B450
void sub_57B450()
{
    long long obj;

    if (!sleep_ui_created) {
        return;
    }

    if (sub_551A80(0)) {
        timeevent_clear_all_typed(TIMEEVENT_TYPE_SLEEPING);
        sub_57BA70();
        sleep_ui_destroy();

        if (dword_6834E8) {
            if (sub_443F80(&obj, &stru_6834B8) && obj != 0) {
                sub_45F3A0(qword_6834A8, obj);
            }
        }
    }

    dword_6834E8 = 0;
    qword_6834A8 = 0;
    dword_6834B0 = 0;
}

// 0x57B4E0
bool sleep_ui_is_created()
{
    return sleep_ui_created;
}

// 0x57B4F0
void sleep_ui_create()
{
    // TODO: Incomplete.
}

// 0x57B6E0
void sleep_ui_destroy()
{
    if (!sleep_ui_created) {
        return;
    }

    tig_window_destroy(sleep_ui_window);
    sleep_ui_created = false;
}

// 0x57B710
void sleep_ui_message_filter()
{
    // TODO: Incomplete.
}

// 0x57B7F0
void sleep_ui_process_callback()
{
    // TODO: Incomplete.
}

// 0x57B9E0
void sub_57B9E0()
{
    FadeData fade_data;

    if (dword_6834E0) {
        return;
    }

    fade_data.field_0 = 0;
    fade_data.field_10 = 0;
    fade_data.duration = 2.0f;
    fade_data.steps = 48;
    fade_data.color = tig_color_make(0, 0, 0);
    sub_4BDFA0(&fade_data);
    sub_57D640();
    dword_6834E0 = true;
}

// 0x57BA70
void sub_57BA70()
{
    FadeData fade_data;

    if (!dword_6834E0) {
        return;
    }

    sub_57D620();

    fade_data.field_0 = 1;
    fade_data.duration = 2.0f;
    fade_data.steps = 48;
    sub_4BDFA0(&fade_data);

    dword_6834E0 = false;
    sub_57B450();
}

// 0x57BAC0
void sub_57BAC0()
{
    ObjectNodeList l;
    ObjectNode* node;

    sub_4411C0(qword_6834A8, &l);

    node = l.head;
    whiel (node != NULL) {
        sub_441980(node->obj, 0, 0, 29, 0);
        node = node->next;
    }

    sub_4410E0(&l);
}
