#include "ui/sleep_ui.h"

#include "game/critter.h"
#include "game/gfade.h"
#include "game/magictech.h"
#include "game/mes.h"
#include "game/object.h"
#include "game/timeevent.h"
#include "ui/anim_ui.h"
#include "ui/intgame.h"

static bool sleep_ui_create();
static void sleep_ui_destroy();
static bool sleep_ui_message_filter(TigMessage* msg);
static void sub_57B9E0();
static void sub_57BA70();
static void sub_57BAC0();

// 0x5CB2B8
static tig_window_handle_t sleep_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5CB2C0
static UiButtonInfo stru_5CB2C0[8] = {
    { 8, 3, 293, TIG_BUTTON_HANDLE_INVALID },
    { 8, 21, 293, TIG_BUTTON_HANDLE_INVALID },
    { 8, 39, 293, TIG_BUTTON_HANDLE_INVALID },
    { 8, 57, 293, TIG_BUTTON_HANDLE_INVALID },
    { 8, 75, 293, TIG_BUTTON_HANDLE_INVALID },
    { 8, 93, 293, TIG_BUTTON_HANDLE_INVALID },
    { 8, 111, 293, TIG_BUTTON_HANDLE_INVALID },
    { 8, 129, 293, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5CB340
static int dword_5CB340[8] = {
    1,
    2,
    4,
    8,
    24,
    -1,
    -2,
    -3,
};

// 0x6834A0
static tig_font_handle_t dword_6834A0;

// 0x6834A8
static int64_t qword_6834A8;

// 0x6834B0
static bool dword_6834B0;

// 0x6834B8
static Ryan stru_6834B8;

// 0x6834E0
static bool dword_6834E0;

// 0x6834E4
static mes_file_handle_t sleep_ui_mes_file;

// 0x6834E8
static int dword_6834E8;

// 0x6834EC
static bool sleep_ui_initialized;

// 0x6834F0
static bool sleep_ui_created;

// 0x57B080
bool sleep_ui_init(GameInitInfo* init_info)
{
    TigFont font;

    (void)init_info;

    if (!mes_load("mes\\sleepUI.mes", &sleep_ui_mes_file)) {
        return false;
    }

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create(&font, &dword_6834A0);

    dword_6834E0 = false;
    dword_6834B0 = 0;
    sleep_ui_initialized = true;

    return true;
}

// 0x57B140
void sleep_ui_exit()
{
    mes_unload(sleep_ui_mes_file);
    tig_font_destroy(dword_6834A0);
    sleep_ui_initialized = 0;
    dword_6834E0 = false;
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
void sub_57B180(int64_t obj)
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
            if (sub_443F80(&obj, &stru_6834B8) && obj != OBJ_HANDLE_NULL) {
                critter_leave_bed(qword_6834A8, obj);
            }
        }

        dword_6834E8 = 0;
        qword_6834A8 = 0;
        dword_6834B0 = 0;
    }
}

// 0x57B4E0
bool sleep_ui_is_created()
{
    return sleep_ui_created;
}

// 0x57B4F0
bool sleep_ui_create()
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigWindowData window_data;
    TigRect rect;
    TigRect text_rect;
    int cnt;
    int index;
    MesFileEntry mes_file_entry;

    if (sleep_ui_created) {
        return false;
    }

    if (tig_art_interface_id_create(565, 0, 0, 0, &art_id) != TIG_OK) {
        return false;
    }

    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    rect.x = 573;
    rect.y = 42;
    rect.width = art_frame_data.width;
    rect.height = art_frame_data.height;

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.message_filter = sleep_ui_message_filter;
    window_data.rect = rect;
    if (tig_window_create(&window_data, &sleep_ui_window) != TIG_OK) {
        tig_debug_printf("sleep_ui_create: ERROR: window create failed!\n");
        exit(EXIT_SUCCESS); // FIXME: Shoule be EXIT_FAILURE
    }

    rect.x = 0;
    rect.y = 0;

    art_blit_info.art_id = art_id;
    art_blit_info.flags = 0;
    art_blit_info.src_rect = &rect;
    art_blit_info.dst_rect = &rect;
    tig_window_blit_art(sleep_ui_window, &art_blit_info);

    tig_font_push(dword_6834A0);

    text_rect.x = 36;
    text_rect.y = 2;
    text_rect.width = rect.width - 36;
    text_rect.height = 18;

    cnt = dword_6834B0 ? 7 : 8;
    for (index = 0; index < cnt; index++) {
        mes_file_entry.num = dword_6834B0 ? index + 12 : index;
        mes_get_msg(sleep_ui_mes_file, &mes_file_entry);
        tig_window_text_write(sleep_ui_window, mes_file_entry.str, &text_rect);
        text_rect.y += text_rect.height;
    }

    tig_font_pop();

    for (index = 0; index < cnt; index++) {
        sub_54AA60(sleep_ui_window, &rect, &(stru_5CB2C0[index]), true);
    }

    sleep_ui_created = true;

    return true;
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
bool sleep_ui_message_filter(TigMessage* msg)
{
    int cnt;
    int index;
    TimeEvent timeevent;
    DateTime datetime;

    if (msg->type != TIG_MESSAGE_BUTTON) {
        return false;
    }

    if (msg->data.button.state != TIG_BUTTON_STATE_RELEASED) {
        return false;
    }

    cnt = dword_6834B0 ? 7 : 8;
    for (index = 0; index < cnt; index++) {
        if (msg->data.button.button_handle == stru_5CB2C0[index].button_handle) {
            break;
        }
    }

    if (index >= cnt) {
        return false;
    }

    timeevent_clear_all_typed(TIMEEVENT_TYPE_SLEEPING);

    timeevent.type = TIMEEVENT_TYPE_SLEEPING;
    if (dword_5CB340[index] >= 0) {
        timeevent.params[0].integer_value = 3600000 * dword_5CB340[index] / 3600000;
    } else {
        timeevent.params[0].integer_value = dword_5CB340[index];
    }

    sub_45A950(&datetime, 50);
    sub_45B800(&timeevent, &datetime);

    sub_57B9E0();
    sub_57BAC0();
    sub_457450(qword_6834A8);

    return true;
}

// 0x57B7F0
bool sleep_ui_process_callback(TimeEvent* timeevent)
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
    ObjectList objects;
    ObjectNode* node;

    object_get_followers(qword_6834A8, &objects);

    node = objects.head;
    while (node != NULL) {
        sub_441980(qword_6834A8, node->obj, OBJ_HANDLE_NULL, 29, 0);
        node = node->next;
    }

    object_list_destroy(&objects);
}
