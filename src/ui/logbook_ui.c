#include "ui/logbook_ui.h"

#include <stdio.h>

#include "game/background.h"
#include "game/bless.h"
#include "game/critter.h"
#include "game/curse.h"
#include "game/description.h"
#include "game/gsound.h"
#include "game/location.h"
#include "game/mes.h"
#include "game/player.h"
#include "game/quest.h"
#include "game/reputation.h"
#include "game/rumor.h"
#include "game/timeevent.h"
#include "game/ui.h"
#include "ui/intgame.h"
#include "ui/types.h"

static void logbook_ui_create();
static void logbook_ui_destroy();
static bool logbook_ui_message_filter(TigMessage* msg);
static void sub_53F490(int a1, int a2);
static void sub_53F5F0(int a1, int a2);
static void sub_53F640();
static void sub_53F6A0();
static void sub_53F6E0();
static int sub_53F8F0(int a1, int a2);
static void sub_53F9E0(int index, TigRect* rect, int a3, int a4);
static int sub_53FAD0(char* buffer, tig_font_handle_t font, TigRect* rect, bool a4, bool a5, bool a6);
static void sub_53FBB0();
static void sub_540310(char* buffer, int index);
static void sub_5403C0(char* buffer, int index);
static void sub_540470(char* buffer, int index);
static void sub_540510(char* buffer, int index);
static void sub_540550(char* buffer, int index);
static void sub_5405C0(char* buffer, int index);
static void sub_540760(char* buffer, int index);
static void sub_5407B0(char* buffer, int index);

// 0x5C33F0
static tig_window_handle_t logbook_ui_window = TIG_WINDOW_HANDLE_INVALID;

// 0x5C33F8
static TigRect stru_5C33F8[2] = {
    { 0, 41, 800, 400 },
    { 150, 11, 501, 365 },
};

// 0x5C3418
static TigRect stru_5C3418 = { 25, 24, 89, 89 };

// 0x5C3428
static UiButtonInfo stru_5C3428[2] = {
    { 213, 77, 272, TIG_BUTTON_HANDLE_INVALID },
    { 675, 77, 273, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C3448
static UiButtonInfo stru_5C3448[7] = {
    { 696, 83, 265, TIG_BUTTON_HANDLE_INVALID },
    { 696, 129, 267, TIG_BUTTON_HANDLE_INVALID },
    { 696, 175, 271, TIG_BUTTON_HANDLE_INVALID },
    { 695, 221, 266, TIG_BUTTON_HANDLE_INVALID },
    { 696, 267, 270, TIG_BUTTON_HANDLE_INVALID },
    { 696, 313, 268, TIG_BUTTON_HANDLE_INVALID },
    { 696, 359, 269, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C34B8
static TigRect stru_5C34B8 = { 222, 72, 215, 270 };

// 0x5C34C8
static TigRect stru_5C34C8 = { 468, 72, 215, 270 };

// 0x5C34D8
static TigRect stru_5C34D8[2] = {
    { 236, 43, 187, 19 },
    { 482, 43, 187, 19 },
};

// 0x5C34F8
static TigRect stru_5C34F8[2] = {
    { 222, 346, 215, 15 },
    { 468, 346, 215, 15 },
};

// 0x63CBF0
static int dword_63CBF0;

// 0x63CBF4
static int dword_63CBF4[3000];

// 0x63FAD8
static int64_t qword_63FAD8;

// 0x63FAE0
static tig_font_handle_t dword_63FAE0;

// 0x63FAE4
static int dword_63FAE4[3000];

// 0x6429C4
static tig_font_handle_t dword_6429C4;

// 0x6429C8
static tig_font_handle_t dword_6429C8;

// 0x6429CC
static mes_file_handle_t quotes_mes_file;

// 0x6429D0
static tig_font_handle_t dword_6429D0;

// 0x6429D8
static DateTime stru_6429D8[3000];

// 0x648798
static int dword_648798;

// 0x64879C
static int dword_64879C[100];

// 0x64892C
static tig_font_handle_t dword_64892C;

// 0x648930
static tig_font_handle_t dword_648930;

// 0x648934
static tig_font_handle_t dword_648934;

// 0x64893C
static tig_font_handle_t dword_64893C;

// 0x648938
static int dword_648938;

// 0x648940
static int dword_648940[13];

// 0x648974
static int dword_648974;

// 0x648978
static int dword_648978;

// 0x64897C
static int dword_64897C;

// 0x648980
static int dword_648980;

// 0x648984
static mes_file_handle_t logbook_ui_mes_file;

// 0x648988
static tig_font_handle_t dword_648988[3000];

// 0x64B868
static bool logbook_ui_initialized;

// 0x64B86C
static bool logbook_ui_created;

// 0x53EB10
bool logbook_ui_init(GameInitInfo* init_info)
{
    TigFont font_info;
    int index;

    (void)init_info;

    if (!mes_load("mes\\logbk_ui.mes", &logbook_ui_mes_file)) {
        return false;
    }

    if (!mes_load("mes\\quotes.mes", &quotes_mes_file)) {
        return false;
    }

    font_info.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 0, 0);
    tig_font_create(&font_info, &dword_648934);

    font_info.flags = TIG_FONT_CENTERED;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 0, 0);
    tig_font_create(&font_info, &dword_6429C8);

    font_info.flags = TIG_FONT_STRIKE_THROUGH;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 0, 0);
    font_info.strike_through_color = font_info.color;
    tig_font_create(&font_info, &dword_6429C4);

    font_info.flags = TIG_FONT_STRIKE_THROUGH;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 120, 0);
    font_info.strike_through_color = font_info.color;
    tig_font_create(&font_info, &dword_63FAE0);

    font_info.flags = TIG_FONT_STRIKE_THROUGH;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(150, 0, 0);
    font_info.strike_through_color = font_info.color;
    tig_font_create(&font_info, &dword_64892C);

    font_info.flags = TIG_FONT_CENTERED;
    tig_art_interface_id_create(27, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 0, 0);
    tig_font_create(&font_info, &dword_648930);

    font_info.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(0, 0, 150);
    tig_font_create(&font_info, &dword_64893C);

    font_info.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_info.art_id));
    font_info.str = NULL;
    font_info.color = tig_color_make(150, 0, 0);
    tig_font_create(&font_info, &dword_6429D0);

    tig_font_push(dword_648934);
    font_info.str = "test";
    font_info.width = 0;
    sub_535390(&font_info);
    dword_63CBF0 = font_info.height;
    tig_font_pop();

    dword_64897C = -1;

    for (index = 0; index < 100; index++) {
        dword_64879C[index] = -1;
    }
    dword_64879C[0] = 0;

    dword_648978 = 0;
    qword_63FAD8 = 0;
    dword_648798 = 1;
    logbook_ui_initialized = true;

    return true;
}

// 0x53EF40
void logbook_ui_exit()
{
    mes_unload(logbook_ui_mes_file);
    mes_unload(quotes_mes_file);
    tig_font_destroy(dword_648934);
    tig_font_destroy(dword_6429C8);
    tig_font_destroy(dword_6429C4);
    tig_font_destroy(dword_63FAE0);
    tig_font_destroy(dword_64892C);
    tig_font_destroy(dword_648930);
    tig_font_destroy(dword_64893C);
    tig_font_destroy(dword_6429D0);
    logbook_ui_initialized = false;
}

// 0x53EFD0
void logbook_ui_reset()
{
    int index;

    if (logbook_ui_created) {
        sub_53F090();
    }

    dword_648978 = 0;

    for (index = 0; index < 100; index++) {
        dword_64879C[index] = -1;
    }
    dword_64879C[0] = 0;

    qword_63FAD8 = 0;
    dword_64897C = -1;
    dword_648798 = 1;
}

// 0x53F020
void sub_53F020(int64_t obj)
{
    if (logbook_ui_created) {
        sub_53F090();
        return;
    }

    if (obj != OBJ_HANDLE_NULL
        && !sub_45D8D0(player_get_pc_obj())
        && sub_551A80(0)
        && sub_551A80(7)) {
        qword_63FAD8 = obj;
        logbook_ui_create();
    }


}

// 0x53F090
void sub_53F090()
{
    if (logbook_ui_created
        && sub_551A80(0)) {
        logbook_ui_destroy();
        qword_63FAD8 = OBJ_HANDLE_NULL;
    }
}

// 0x53F0D0
bool logbook_ui_is_created()
{
    return logbook_ui_created;
}

// 0x53F0E0
void logbook_ui_create()
{
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    int index;
    tig_button_handle_t button_handles[7];
    S550DA0 v1;

    if (logbook_ui_created) {
        return;
    }

    if (!intgame_big_window_lock(logbook_ui_message_filter, &logbook_ui_window)) {
        tig_debug_printf("logbk_ui_create: ERROR: intgame_big_window_lock failed!\n");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    if (tig_kb_is_key_pressed(DIK_LCONTROL) && tig_kb_is_key_pressed(DIK_LMENU)) {
        dword_648978 = 0;
        dword_648798 = 1;
        dword_648980 = 1;
        gsound_play_sfx_id(3002, 1);
    }

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = stru_5C33F8[0].width;
    src_rect.height = stru_5C33F8[0].height;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = src_rect.width;
    dst_rect.height = src_rect.height;

    blit_info.flags = 0;
    tig_art_interface_id_create(264, 0, 0, 0, &(blit_info.art_id));
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(logbook_ui_window, &blit_info);

    for (index = 0; index < 2; index++) {
        sub_54AA60(logbook_ui_window, &(stru_5C33F8[0]), &(stru_5C3428[index]), 9);
    }

    for (index = 0; index < 7; index++) {
        sub_54AA60(logbook_ui_window, &(stru_5C33F8[0]), &(stru_5C3448[index]), 9);
        button_handles[index] = stru_5C3448[index].button_handle;
    }
    tig_button_radio_group_create(7, button_handles, dword_648978);

    sub_4B8CE0(obj_field_int64_get(qword_63FAD8, OBJ_F_LOCATION));

    v1.window_handle = logbook_ui_window;
    v1.rect = &stru_5C3418;
    tig_art_interface_id_create(198, 0, 0, 0, &(v1.art_id));
    sub_550DA0(1, &v1);
    sub_53F490(261, 1);
    sub_460790(1, 0);
    gsound_play_sfx_id(3008, 1);

    logbook_ui_created = true;
}

// 0x53F2F0
void logbook_ui_destroy()
{
    if (!logbook_ui_created) {
        return;
    }

    sub_550DA0(0, 0);
    intgame_big_window_unlock();
    logbook_ui_window = TIG_WINDOW_HANDLE_INVALID;
    gsound_play_sfx_id(3009, 1);
    logbook_ui_created = false;

    if (dword_648980) {
        dword_648798 = 1;
    }
    dword_648980 = false;
}

// 0x53F350
bool logbook_ui_message_filter(TigMessage* msg)
{
    int index;
    MesFileEntry mes_file_entry;
    John v1;

    if (msg->type == TIG_MESSAGE_MOUSE) {
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP
            && sub_551000(msg->data.mouse.x, msg->data.mouse.y)) {
            sub_53F090();
            return true;
        }
        return false;
    }

    if (msg->type == TIG_MESSAGE_BUTTON) {
        switch (msg->data.button.state) {
        case TIG_BUTTON_STATE_PRESSED:
            for (index = 0; index < 7; index++) {
                if (stru_5C3448[index].button_handle == msg->data.button.button_handle) {
                    sub_53F5F0(index, 0);
                    return true;
                }
            }
            return false;
        case TIG_BUTTON_STATE_RELEASED:
            if (stru_5C3428[0].button_handle == msg->data.button.button_handle) {
                sub_53F6A0();
                return true;
            }
            if (stru_5C3428[1].button_handle == msg->data.button.button_handle) {
                sub_53F640();
                return true;
            }
            return false;
        case TIG_BUTTON_STATE_MOUSE_INSIDE:
            for (index = 0; index < 7; index++) {
                if (stru_5C3448[index].button_handle == msg->data.button.button_handle) {
                    mes_file_entry.num = index;
                    mes_get_msg(logbook_ui_mes_file, &mes_file_entry);

                    v1.type = 6;
                    v1.str = mes_file_entry.str;
                    sub_550750(&v1);

                    return true;
                }
            }
            return false;
        case TIG_BUTTON_STATE_MOUSE_OUTSIDE:
            for (index = 0; index < 7; index++) {
                if (stru_5C3448[index].button_handle == msg->data.button.button_handle) {
                    sub_550720();
                    return true;
                }
            }
            return false;
        }
        return false;
    }

    return false;
}

// 0x53F490
void sub_53F490(int a1, int a2)
{
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;
    tig_button_handle_t selected_button_handle;
    int selected_button_index;
    int index;

    if (dword_64897C != -1) {
        if (!a2 && a1 == 261) {
            return;
        }
    } else {
        a1 = 261;
    }

    dword_64897C = a1;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = stru_5C33F8[0].width;
    src_rect.height = stru_5C33F8[0].height;

    dst_rect.x = 165;
    dst_rect.y = 0;
    dst_rect.width = src_rect.width;
    dst_rect.height = src_rect.height;

    blit_info.flags = 0;
    tig_art_interface_id_create(260, 0, 0, 0, &(blit_info.art_id));
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(logbook_ui_window, &blit_info);

    tig_art_interface_id_create(dword_64897C, 0, 0, 0, &(blit_info.art_id));
    dst_rect.x = 172;
    dst_rect.y = 23;
    tig_window_blit_art(logbook_ui_window, &blit_info);

    selected_button_handle = sub_538730(stru_5C3448[0].button_handle);
    selected_button_index = 0;

    for (index = 0; index < 7; index++) {
        tig_button_show(stru_5C3448[index].button_handle);
        if (selected_button_handle == stru_5C3448[index].button_handle) {
            selected_button_index = index;
        }
    }

    gsound_play_sfx_id(3011, 1);
    sub_53F5F0(selected_button_index, a2);
}

// 0x53F5F0
void sub_53F5F0(int a1, int a2)
{
    int index;

    if (a2 == 0) {
        for (index = 0; index < 100; index++) {
            dword_64879C[index] = -1;
        }
        dword_64879C[0] = 0;
        dword_648798 = 1;
        dword_648978 = a1;
    }

    sub_53FBB0();
    sub_53F6E0();
    gsound_play_sfx_id(3010, 1);
}

// 0x53F640
void sub_53F640()
{
    if (dword_648974 < dword_648938 - 1
        && (dword_648798 - 1) / 2 < 100) {
        dword_648798 += 2;
        dword_64879C[(dword_648798 - 1) / 2] = dword_648974 + 1;
        sub_53F6E0();
        gsound_play_sfx_id(3010, 1);
    }
}

// 0x53F6A0
void sub_53F6A0()
{
    if (dword_64879C[(dword_648798 - 1) / 2] > 0) {
        dword_648798 -= 2;
        sub_53F6E0();
        gsound_play_sfx_id(3010, 1);
    }
}

// 0x53F6E0
void sub_53F6E0()
{
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    MesFileEntry mes_file_entry;
    int index;
    char buffer[80];

    tig_button_hide(stru_5C3428[0].button_handle);
    tig_button_hide(stru_5C3428[1].button_handle);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = stru_5C33F8[0].width;
    src_rect.height = stru_5C33F8[0].height;

    dst_rect.x = 165;
    dst_rect.y = 0;
    dst_rect.width = src_rect.width;
    dst_rect.height = src_rect.height;

    blit_info.flags = 0;
    tig_art_interface_id_create(260, 0, 0, 0, &(blit_info.art_id));
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;

    tig_window_blit_art(logbook_ui_window, &blit_info);

    if (dword_648938 != 0) {
        dword_648974 = sub_53F8F0(dword_64879C[(dword_648798 - 1) / 2], dword_648938 - 1);
    } else {
        dword_648974 = 0;
    }

    if (dword_64879C[(dword_648798 - 1) / 2] > 0) {
        tig_button_show(stru_5C3428[0].button_handle);
    }

    if (dword_648974 < dword_648938 - 1) {
        tig_button_show(stru_5C3428[1].button_handle);
    }

    mes_file_entry.num = dword_648978 || !dword_648980 ? dword_648978 : 100;
    mes_get_msg(logbook_ui_mes_file, &mes_file_entry);

    tig_font_push(dword_648930);
    for (index = 0; index < 2; index++) {
        snprintf(buffer, sizeof(buffer), "%c%s%c", '-', mes_file_entry.str, '-');
        tig_window_text_write(logbook_ui_window, buffer, &(stru_5C34D8[index]));
    }
    tig_font_pop();

    tig_font_push(dword_6429C8);
    for (index = 0; index < 2; index++) {
        snprintf(buffer, sizeof(buffer), "%c%d%c", '-', dword_648798 + index, '-');
        tig_window_text_write(logbook_ui_window, buffer, &(stru_5C34F8[index]));
    }
    tig_font_pop();
}

// 0x53F8F0
int sub_53F8F0(int a1, int a2)
{
    // TODO: Incomplete.
}

// 0x53F9E0
void sub_53F9E0(int index, TigRect* rect, int a3, int a4)
{
    char buffer[2000];
    bool v1;

    v1 = index < dword_648938 - 1;
    buffer[0] = '\0';

    switch (dword_648978) {
    case 0:
        sub_540310(buffer, index);
        break;
    case 1:
        sub_540470(buffer, index);
        break;
    case 2:
        sub_540510(buffer, index);
        break;
    case 3:
        sub_540550(buffer, index);
        break;
    case 4:
        sub_5405C0(buffer, index);
        v1 = false;
        break;
    case 5:
        sub_540760(buffer, index);
        break;
    case 6:
        sub_5407B0(buffer, index);
        break;
    default:
        sub_53FAD0(buffer, dword_648988[index], rect, a3, a4, v1);
        break;
    }
}

// 0x53FAD0
int sub_53FAD0(char* buffer, tig_font_handle_t font, TigRect* rect, bool a4, bool a5, bool a6)
{
    TigFont font_info;
    bool warned;
    size_t pos;

    if (buffer[0] == '\0') {
        return -1;
    }

    tig_font_push(font);

    font_info.str = buffer;
    font_info.width = rect->width;

    while (true) {
        sub_535390(&font_info);
        if (font_info.height <= rect->height) {
            break;
        }

        if (!a5 || (pos = strlen(buffer)) == 0) {
            tig_font_pop();
            return 0;
        }

        if (!warned) {
            tig_debug_printf("Had to shorten logbook entry: %s\n", buffer);
        }

        buffer[pos - 1] = '\0';
        warned = true;
    }

    if (!a4) {
        tig_window_text_write(logbook_ui_window, buffer, rect);
    }

    if (a6) {
        tig_font_pop();
        return dword_63CBF0 + font_info.height;
    }

    tig_font_pop();
    return font_info.height;
}

// 0x53FBB0
void sub_53FBB0()
{
    // TODO: Incomplete.
}

// 0x540310
void sub_540310(char* buffer, int index)
{
    MesFileEntry mes_file_entry;
    size_t pos;

    if (dword_648980) {
        mes_file_entry.num = index;
        if (mes_search(quotes_mes_file, &mes_file_entry)) {
            strcpy(buffer, mes_file_entry.str);
        } else {
            buffer[0] = '\0';
        }
    } else {
        sub_5403C0(buffer, index);

        pos = strlen(buffer);
        buffer[pos] = '\n';

        sub_4C5920(qword_63FAD8, dword_63FAE4[index], &(buffer[pos + 1]));
    }
}

// 0x5403C0
void sub_5403C0(char* buffer, int index)
{
    DateTime* datetime;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    char am_pm;
    MesFileEntry mes_file_entry;

    datetime = &(stru_6429D8[index]);
    month = datetime_get_month(datetime);
    day = datetime_get_day(datetime);
    year = datetime_get_year(datetime);
    hour = datetime_get_hour(datetime);
    minute = datetime_get_minute(datetime);

    if (hour < 12) {
        am_pm = 'a';
        if (hour == 0) {
            hour = 12;
        }
    } else {
        am_pm = 'p';
        if (hour > 12) {
            hour -= 12;
        }
    }

    mes_file_entry.num = month + 16;
    mes_get_msg(logbook_ui_mes_file, &mes_file_entry);

    sprintf(buffer, "%s %d, %d  %d:%.2d%cm    ",
        mes_file_entry.str,
        day,
        year,
        hour,
        minute,
        am_pm);
}

// 0x540470
void sub_540470(char* buffer, int index)
{
    MesFileEntry mes_file_entry;
    size_t pos;

    sub_5403C0(buffer, index);
    mes_file_entry.num = dword_63CBF4[index] + 19;
    mes_get_msg(logbook_ui_mes_file, &mes_file_entry);
    strcat(buffer, mes_file_entry.str);

    pos = strlen(buffer);
    buffer[pos] = '\n';

    quest_copy_description(qword_63FAD8, dword_63FAE4[index], &(buffer[pos + 1]));
}

// 0x540510
void sub_540510(char* buffer, int index)
{
    size_t pos;

    sub_5403C0(buffer, index);
    pos = strlen(buffer);
    buffer[pos] = '\n';

    sub_4C1BD0(dword_63FAE4[index], &(buffer[pos + 1]));
}

// 0x540550
void sub_540550(char* buffer, int index)
{
    size_t pos;

    sub_5403C0(buffer, index);
    pos = strlen(buffer);
    buffer[pos] = '\n';

    if (dword_648988[index] == dword_64893C) {
        bless_copy_name(dword_63FAE4[index], &(buffer[pos + 1]));
    } else {
        curse_copy_name(dword_63FAE4[index], &(buffer[pos + 1]));
    }
}

// 0x5405C0
void sub_5405C0(char* buffer, int index)
{
    MesFileEntry mes_file_entry;
    const char* extra;
    char tmp[80];
    int v1;

    if (index < 7) {
        mes_file_entry.num = 26 + index;
        mes_get_msg(logbook_ui_mes_file, &mes_file_entry);

        // NOTE: Original code is slightly different but does the same thing.
        if (index == 0) {
            itoa(dword_648940[0], tmp, 10);
            sprintf(buffer, "%s: %s", mes_file_entry.str, tmp);
        } else {
            switch (index) {
            case 1:
                v1 = dword_648940[1];
                break;
            case 2:
                v1 = dword_648940[3];
                break;
            case 3:
                v1 = dword_648940[5];
                break;
            case 4:
                v1 = dword_648940[7];
                break;
            case 5:
                v1 = dword_648940[9];
                break;
            case 6:
                v1 = dword_648940[11];
                break;
            default:
                __assume(0);
            }

            if (v1 > 0 && (extra = description_get_name(dword_63FAE4[index])) != NULL) {
                sprintf(buffer, "%s: %s", mes_file_entry.str, extra);
            } else {
                sprintf(buffer, "%s: -----", mes_file_entry.str);
            }
        }
    } else if (index == 7) {
        buffer[0] = '\0';
    } else if (index == 8) {
        mes_file_entry.num = 33;
        mes_get_msg(logbook_ui_mes_file, &mes_file_entry);
        strcpy(buffer, mes_file_entry.str);
    } else {
        mes_file_entry.num = stru_6429D8[index].milliseconds + 34;
        mes_get_msg(logbook_ui_mes_file, &mes_file_entry);

        extra = description_get_name(dword_63FAE4[index]);
        if (extra != NULL) {
            sprintf(buffer, "%s %s", mes_file_entry.str, extra);
        }
    }
}

// 0x540760
void sub_540760(char* buffer, int index)
{
    const char* body;
    int start;
    int end;

    body = background_description_get_body(dword_63FAE4[0]);
    if (index != 0) {
        start = dword_63FAE4[index];
    } else {
        start = 0;
    }

    end = dword_63FAE4[index + 1] - start;
    strncpy(buffer, &(body[start]), end);
    buffer[end] = '\0';
}

// 0x5407B0
void sub_5407B0(char* buffer, int index)
{
    strcpy(buffer, description_get_key_name(dword_63FAE4[index]));
}

// 0x5407F0
void sub_5407F0()
{
    int index;
    char buffer[2000];
    size_t pos;
    TigRect rect;

    stru_6429D8[0] = sub_45A7C0();

    for (index = 0; index < 3000; index++) {
        rect = stru_5C34B8;

        sub_5403C0(buffer, 0);
        pos = strlen(buffer);
        buffer[pos] = '\n';

        sub_4C5960(index, &(buffer[pos + 1]));
        if (buffer[pos + 1] != '\0') {
            tig_debug_printf("Checking rumor %d\n", index);
            sub_53FAD0(buffer, dword_648934, &rect, 1, 1, 1);
        }

        rect = stru_5C34B8;

        sub_5403C0(buffer, 0);
        pos = strlen(buffer);
        buffer[pos] = '\n';
        sub_4C59D0(index, &(buffer[pos + 1]));
        if (buffer[pos + 1] != '\0') {
            tig_debug_printf("Checking dumb rumor %d\n", index);
            sub_53FAD0(buffer, dword_648934, &rect, 1, 1, 1);
        }
    }
}
