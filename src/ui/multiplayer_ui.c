#include "ui/multiplayer_ui.h"

#include <stdio.h>

#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/mp_utils.h"
#include "game/party.h"
#include "game/player.h"
#include "game/portrait.h"
#include "game/timeevent.h"
#include "ui/intgame.h"
#include "ui/mainmenu_ui.h"
#include "ui/mp_ctrl_ui.h"
#include "ui/textedit_ui.h"

typedef struct S681000 {
    void* field_0;
    int field_4;
} S681000;

static_assert(sizeof(S681000) == 0x8, "wrong size");

static void sub_5706D0();
static bool sub_570A40(tig_window_handle_t window_handle);
static void sub_570E10(int a1);
static void sub_570E40();
static void sub_570EF0();
static void sub_571100();
static bool sub_5713F0(TigMessage* msg);
static void sub_571570(int screen_x, int screen_y, int* window_x, int* window_y);
static bool sub_5715A0(int x, int y, int* index_ptr);
static char sub_571610(int a1);
static tig_font_handle_t sub_571630(int a1);
static void sub_571660(TextEdit* textedit);
static void sub_571730(TextEdit* textedit);

// 0x599458
static char byte_599458[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

// 0x5CABE8
static tig_window_handle_t dword_5CABE8;

// 0x5CABEC
static int dword_5CABEC = -1;

// 0x5CABF0
static int dword_5CABF0 = -1;

// 0x680F18
static char byte_680F18[128];

// 0x5CABF8
static TextEdit stru_5CABF8 = {
    0,
    byte_680F18,
    128,
    sub_571730,
    sub_571660,
    NULL,
};

// 0x680EF0
static tig_button_handle_t multiplayer_ui_kick_button_handles[10];

// 0x680F98
static tig_button_handle_t dword_680F98;

// 0x680FD0
static TigArtFrameData stru_680FD0[2];

// 0x680F9C
static int dword_680F9C[2];

// 0x680FA4
static int dword_680FA4[10];

// 0x680FCC
static tig_button_handle_t multiplayer_ui_ban_btn;

// 0x681000
static S681000* off_681000;

// 0x681004
static tig_button_handle_t multiplayer_ui_add_button_handles[10];

// 0x68102C
static tig_button_handle_t multiplayer_ui_kick_btn;

// 0x681030
static mes_file_handle_t multiplayer_ui_mes_file;

// 0x681034
static bool multiplayer_ui_initialized;

// 0x681038
static bool dword_681038;

// 0x68103C
static int dword_68103C;

// 0x56FF80
bool multiplayer_ui_init(GameInitInfo* init_info)
{
    int index;
    tig_art_id_t art_id;

    (void)init_info;

    if (!mes_load("mes\\MultiPlayer.mes", &multiplayer_ui_mes_file)) {
        sub_549B10();
        return true;
    }

    for (index = 0; index < 2; index++) {
        if (tig_art_interface_id_create(index == 0 ? 640 : 643, 0, 0, 0, &art_id) != TIG_OK
            || tig_art_exists(art_id) != TIG_OK
            || tig_art_frame_data(art_id, &(stru_680FD0[index])) != TIG_OK) {
            sub_549B10();
            return true;
        }
    }

    off_681000 = (S681000*)MALLOC(sizeof(S681000) * 10);
    for (index = 0; index < 10; index++) {
        off_681000[index].field_0 = NULL;
        off_681000[index].field_4 = -1;
    }

    multiplayer_ui_initialized = true;

    return true;
}

// 0x570050
void multiplayer_ui_exit()
{
    int index;

    mes_unload(multiplayer_ui_mes_file);

    if (off_681000 != NULL) {
        for (index = 0; index < 10; index++) {
            if (off_681000[index].field_0 != NULL) {
                FREE(off_681000[index].field_0);
            }
        }
        FREE(off_681000);
    }

    multiplayer_ui_initialized = false;
}

// 0x5700B0
void mutliplayer_ui_reset()
{
}

// 0x5700C0
void sub_5700C0()
{
    DateTime datetime;
    TimeEvent timeevent;

    if (dword_681038) {
        sub_570130();
        return;
    }

    dword_68103C = 0;
    sub_5706D0();
    dword_681038 = true;
    sub_570E10(0);

    datetime.days = 0;
    datetime.milliseconds = 10000;

    timeevent.type = TIMEEVENT_TYPE_MP_CTRL_UI;
    timeevent.params[0].integer_value = 0;
    sub_45B800(&timeevent, &datetime);
    sub_569DA0();
}

// 0x570130
void sub_570130()
{
    int index;

    if (dword_681038) {
        if (dword_5CABE8 != TIG_WINDOW_HANDLE_INVALID) {
            for (index = 0; index < dword_68103C; index++) {
                tig_button_destroy(multiplayer_ui_add_button_handles[index]);
                tig_button_destroy(multiplayer_ui_kick_button_handles[index]);
            }
            tig_window_destroy(dword_5CABE8);
            dword_5CABE8 = TIG_WINDOW_HANDLE_INVALID;
        }
        dword_681038 = false;
    }
    sub_569DA0();
}

// 0x5701A0
bool sub_5701A0(tig_window_handle_t window_handle, const TigRect* rect)
{
    TigButtonData button_data;

    button_data.window_handle = window_handle;
    button_data.flags = TIG_BUTTON_FLAG_HIDDEN;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    button_data.x = 286 - rect->x;
    button_data.y = 524 - rect->y;
    tig_art_interface_id_create(843, 0, 0, 0, &(button_data.art_id));
    tig_button_create(&button_data, &multiplayer_ui_kick_btn);

    button_data.x = 455 - rect->x;
    button_data.y = 524 - rect->y;
    tig_art_interface_id_create(844, 0, 0, 0, &(button_data.art_id));
    tig_button_create(&button_data, &multiplayer_ui_ban_btn);

    return true;
}

// 0x570260
bool sub_570260(tig_window_handle_t window_handle)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    TigFont font_desc;
    TigRect rect;

    tig_button_show(multiplayer_ui_kick_btn);
    tig_button_show(multiplayer_ui_ban_btn);

    // Disconnect
    tig_font_push(mainmenu_ui_font(MM_FONT_MORPH15, MM_COLOR_WHITE));
    mes_load("mes\\MultiPlayer.mes", &mes_file); // FIXME: No error checking.
    mes_file_entry.num = 1090;
    mes_get_msg(mes_file, &mes_file_entry);
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);
    rect.x = (163 - font_desc.width) / 2 + 235;
    rect.y = 54;
    rect.width = font_desc.width;
    rect.height = font_desc.height;
    tig_window_text_write(window_handle, mes_file_entry.str, &rect);
    tig_font_pop();

    // Ban
    tig_font_push(mainmenu_ui_font(MM_FONT_MORPH15, MM_COLOR_WHITE));
    mes_file_entry.num = 1091;
    mes_get_msg(mes_file, &mes_file_entry);
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);
    rect.x = (163 - font_desc.width) / 2 + 402;
    rect.y = 54;
    rect.width = font_desc.width;
    rect.height = font_desc.height;
    tig_window_text_write(window_handle, mes_file_entry.str, &rect);
    tig_font_pop();

    mes_unload(mes_file);

    return true;
}

// 0x5703B0
bool sub_5703B0()
{
    tig_button_hide(multiplayer_ui_kick_btn);
    tig_button_hide(multiplayer_ui_ban_btn);

    return true;
}

// 0x5703D0
void sub_5703D0(const char* str, int player)
{
    int index;
    MesFileEntry mes_file_entry;

    if (dword_5CABEC == -1) {
        dword_5CABF0 = player;
        sub_5506C0(3);
    }

    if (off_681000[9].field_0 != NULL) {
        FREE(off_681000[9].field_0);
    }

    for (index = 9; index > 0; index--) {
        off_681000[index] = off_681000[index - 1];
    }

    off_681000[0].field_0 = (char*)MALLOC(128);
    if (player == sub_529520()) {
        strncpy(off_681000[0].field_0, str, 128);
    } else {
        mes_file_entry.num = 1200;
        mes_get_msg(multiplayer_ui_mes_file, &mes_file_entry);
        snprintf(off_681000[0].field_0,
            128,
            mes_file_entry.str,
            tig_net_client_info_get_name(player),
            str);
    }

    off_681000[0].field_4 = player;

    if (dword_5CABEC != -1) {
        sub_570A40(dword_5CABEC);
    }
}

// 0x5704E0
void sub_5704E0(int64_t a1, int64_t a2, int type)
{
    int num;
    int confirmation_num = -1;
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
    char* pc_player_name;
    char str[128];
    int client_id;

    switch (type) {
    case 0:
        // %s joined the party.
        num = 2000;
        // %s has joined you to their party.
        confirmation_num = 2001;
        break;
    case 1:
        // Join Failed: %s.
        num = 2005;
        break;
    case 2:
        // Join Failed: %s cannot join self.
        num = 2006;
        break;
    case 4:
        // Join Failed: %s is already in a party.
        num = 2007;
        break;
    case 5:
        // Disband Failed: %s.
        num = 2010;
        break;
    case 6:
        // Disband Failed: %s is not in a party.
        num = 2011;
        break;
    case 7:
        // Disband Failed: %s is not in your party.
        num = 2012;
        break;
    default:
        return;
    }

    mes_file_entry.num = num;
    mes_get_msg(multiplayer_ui_mes_file, &mes_file_entry);

    obj_field_string_get(a2, OBJ_F_PC_PLAYER_NAME, &pc_player_name);
    sprintf(str, mes_file_entry.str, pc_player_name);
    FREE(pc_player_name);

    ui_message.type = UI_MSG_TYPE_EXCLAMATION;
    ui_message.str = str;
    if (player_is_local_pc_obj(a1)) {
        sub_550750(&ui_message);
    } else {
        client_id = multiplayer_find_slot_from_obj(a1);
        if (client_id != -1) {
            sub_4EDA60(&ui_message, client_id, 0);
        }
    }

    if (confirmation_num != -1) {
        mes_file_entry.num = confirmation_num;
        mes_get_msg(multiplayer_ui_mes_file, &mes_file_entry);

        obj_field_string_get(a1, OBJ_F_PC_PLAYER_NAME, &pc_player_name);
        sprintf(str, mes_file_entry.str, pc_player_name);
        FREE(pc_player_name);

        ui_message.type = UI_MSG_TYPE_EXCLAMATION;
        ui_message.str = str;
        if (player_is_local_pc_obj(a2)) {
            sub_550750(&ui_message);
        } else {
            client_id = multiplayer_find_slot_from_obj(a2);
            if (client_id != -1) {
                sub_4EDA60(&ui_message, client_id, 0);
            }
        }
    }
}

// 0x5706D0
void sub_5706D0()
{
    int index;
    TigWindowData window_data;
    TigButtonData add_button_data;
    TigButtonData kick_button_data;

    for (index = 0; index < 8; index++) {
        if (dword_68103C >= 10) {
            break;
        }

        if (!tig_net_client_is_active(index)) {
            dword_680FA4[dword_68103C++] = index;
        }
    }

    if (dword_68103C != 0) {
        window_data.flags = TIG_WINDOW_MESSAGE_FILTER;
        window_data.rect.x = 566;
        window_data.rect.y = 41;
        window_data.rect.width = stru_680FD0[0].width;
        window_data.rect.height = stru_680FD0[0].height * dword_68103C;
        window_data.message_filter = sub_5713F0;
        if (tig_window_create(&window_data, &dword_5CABE8) != TIG_OK) {
            // FIXME: Should be `EXIT_FAILURE`.
            exit(EXIT_SUCCESS);
        }

        add_button_data.flags = TIG_BUTTON_FLAG_0x01;
        add_button_data.window_handle = dword_5CABE8;
        add_button_data.x = 4;
        tig_art_interface_id_create(638, 0, 0, 0, &(add_button_data.art_id));
        add_button_data.mouse_down_snd_id = -1;
        add_button_data.mouse_up_snd_id = -1;
        add_button_data.mouse_enter_snd_id = -1;
        add_button_data.mouse_exit_snd_id = -1;

        kick_button_data.flags = TIG_BUTTON_FLAG_0x01;
        kick_button_data.window_handle = dword_5CABE8;
        kick_button_data.x = 4;
        tig_art_interface_id_create(638, 0, 0, 0, &(kick_button_data.art_id));
        kick_button_data.mouse_down_snd_id = -1;
        kick_button_data.mouse_up_snd_id = -1;
        kick_button_data.mouse_enter_snd_id = -1;
        kick_button_data.mouse_exit_snd_id = -1;

        for (index = 0; index < dword_68103C; index++) {
            add_button_data.y = index * stru_680FD0[index % 2].height + 4;
            if (tig_button_create(&add_button_data, &(multiplayer_ui_add_button_handles[index])) != TIG_OK) {
                // FIXME: Should be `EXIT_FAILURE`.
                exit(EXIT_SUCCESS);
            }

            kick_button_data.y = index * stru_680FD0[index % 2].height + 4;
            if (tig_button_create(&kick_button_data, &(multiplayer_ui_kick_button_handles[index])) != TIG_OK) {
                // FIXME: Should be `EXIT_FAILURE`.
                exit(EXIT_SUCCESS);
            }
        }
    }
}

// 0x570890
void sub_570890()
{
    if (dword_681038) {
        sub_570130();
        sub_5700C0();
    }
}

// 0x5708B0
void sub_5708B0(int a1)
{
    sub_570E10(a1);
}

// 0x5708C0
bool sub_5708C0(tig_window_handle_t window_handle, const TigRect* rect)
{
    TigButtonData button_data;

    button_data.window_handle = window_handle;
    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.x = 574 - rect->x;
    button_data.y = 506 - rect->y;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;
    tig_art_interface_id_create(641, 0, 0, 0, &(button_data.art_id));

    if (tig_button_create(&button_data, &dword_680F98) != TIG_OK) {
        return false;
    }

    return true;
}

// 0x570940
bool sub_570940(int a1)
{
    if (dword_5CABF0 == -1) {
        return false;
    }

    dword_5CABEC = a1;
    sub_570A40(a1);
    tig_button_show(dword_680F98);
    textedit_ui_focus(&stru_5CABF8);
    intgame_text_edit_refresh_color(stru_5CABF8.buffer,
        mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE),
        tig_color_make(2, 0, 116),
        true);

    return true;
}

// 0x5709E0
bool sub_5709E0()
{
    tig_button_hide(dword_680F98);
    textedit_ui_unfocus(&stru_5CABF8);
    dword_5CABF0 = -1;
    dword_5CABEC = -1;
    return true;
}

// 0x570A10
bool sub_570A10(TigMessage* msg)
{
    switch (msg->type) {
    case TIG_MESSAGE_BUTTON:
        if (msg->data.button.state == TIG_BUTTON_STATE_RELEASED
            && msg->data.button.button_handle == dword_680F98) {
            sub_5506C0(3);
            return true;
        }
        break;
    }

    return false;
}

// 0x570A40
bool sub_570A40(tig_window_handle_t window_handle)
{
    TigRect frame_rect;
    TigRect text_rect;
    TigFont font_desc;
    int64_t obj;
    int portrait;
    int index;

    frame_rect.x = 293;
    frame_rect.y = 60;
    frame_rect.width = 266;
    frame_rect.height = 60;

    tig_window_fill(window_handle, &frame_rect, tig_color_make(0, 0, 0));

    if (dword_5CABF0 != -1) {
        obj = sub_4A2B60(dword_5CABF0);
        if (obj != OBJ_HANDLE_NULL) {
            portrait = portrait_get(obj);
            if (portrait != 0) {
                portrait_draw_native(obj, portrait, window_handle, 219, 69);
            }
        }
    }

    for (index = 0; off_681000[index].field_0 != NULL; index++) {
        tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));

        font_desc.str = off_681000[index].field_0;
        font_desc.width = frame_rect.width;
        font_desc.height = 0;
        tig_font_measure(&font_desc);

        text_rect.x = frame_rect.x;
        text_rect.y = frame_rect.y + frame_rect.height;
        text_rect.width = font_desc.width;
        text_rect.height = font_desc.height;

        frame_rect.height -= font_desc.height;
        if (frame_rect.height < 0) {
            tig_font_pop();
            break;
        }

        tig_window_text_write(window_handle, off_681000[index].field_0, &text_rect);
        tig_font_pop();
    }

    return true;
}

// 0x570BC0
bool sub_570BC0(TigMessage* msg)
{
    Packet46 pkt;
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
    char str[200];

    if (msg->type == TIG_MESSAGE_BUTTON
        && msg->data.button.state == TIG_BUTTON_STATE_RELEASED
        && dword_5CABF0 != -1
        && tig_net_is_host()
        && dword_5CABF0 != 0) {
        if (msg->data.button.button_handle == multiplayer_ui_kick_btn) {
            if (tig_net_client_is_waiting(dword_680FA4[dword_5CABF0])
                || tig_net_client_is_loading(dword_680FA4[dword_5CABF0])) {
                tig_net_shutdown(dword_680FA4[dword_5CABF0]);
            } else {
                pkt.type = 46;
                pkt.player = dword_680FA4[dword_5CABF0];
                pkt.field_8 = 0;
                pkt.field_C = 0;
                pkt.field_10 = 0;
                pkt.field_14 = 0;
                tig_net_send_app_all(&pkt, sizeof(pkt));
            }

            mes_file_entry.num = 1101; // "Player %s(%d) has been kicked from the game."
            mes_get_msg(multiplayer_ui_mes_file, &mes_file_entry);
            snprintf(str, sizeof(str),
                mes_file_entry.str,
                tig_net_client_info_get_name(dword_5CABF0),
                dword_5CABF0);

            ui_message.type = UI_MSG_TYPE_EXCLAMATION;
            ui_message.str = str;
            sub_550750(&ui_message);
            dword_5CABF0 = -1;
            sub_570890();
            return true;
        }

        if (msg->data.button.button_handle == multiplayer_ui_ban_btn) {
            tig_net_ban(dword_680FA4[dword_5CABF0]);

            if (tig_net_client_is_waiting(dword_680FA4[dword_5CABF0])
                || tig_net_client_is_loading(dword_680FA4[dword_5CABF0])) {
                tig_net_shutdown(dword_680FA4[dword_5CABF0]);
            } else {
                pkt.type = 46;
                pkt.player = dword_680FA4[dword_5CABF0];
                pkt.field_8 = 0;
                pkt.field_C = 0;
                pkt.field_10 = 0;
                pkt.field_14 = 0;
                tig_net_send_app_all(&pkt, sizeof(pkt));
            }

            mes_file_entry.num = 1102; // "Player %s(%d) has been banned, no connections from this host will be allowed."
            mes_get_msg(multiplayer_ui_mes_file, &mes_file_entry);
            snprintf(str, sizeof(str),
                mes_file_entry.str,
                tig_net_client_info_get_name(dword_5CABF0),
                dword_5CABF0);

            ui_message.type = UI_MSG_TYPE_EXCLAMATION;
            ui_message.str = str;
            sub_550750(&ui_message);
            dword_5CABF0 = -1;
            sub_570890();
            return true;
        }
    }

    return false;
}

// 0x570E10
void sub_570E10(int a1)
{
    (void)a1;

    if (dword_681038) {
        sub_570E40();
        sub_570EF0();
        sub_571100();
    }
}

// 0x570E40
void sub_570E40()
{
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;
    int index;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = stru_680FD0[0].width;
    src_rect.height = stru_680FD0[0].height;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = stru_680FD0[0].width;
    dst_rect.height = stru_680FD0[0].height;

    for (index = 0; index < dword_68103C; index++) {
        blit_info.flags = 0;
        blit_info.dst_rect = &dst_rect;
        blit_info.src_rect = &src_rect;
        blit_info.art_id = dword_680F9C[index % 2];
        tig_window_blit_art(dword_5CABE8, &blit_info);
        dst_rect.y += stru_680FD0[index % 2].height;
    }
}

// 0x570EF0
void sub_570EF0()
{
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    int index;
    int64_t obj;
    int portrait;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = 32;
    src_rect.height = 32;

    dst_rect.x = 4;
    dst_rect.y = 0;
    dst_rect.width = 32;
    dst_rect.height = 32;

    for (index = 0; index < dword_68103C; index++) {
        if (tig_net_client_is_waiting(dword_680FA4[index])) {
            tig_button_show(multiplayer_ui_add_button_handles[index]);
        } else {
            tig_button_hide(multiplayer_ui_add_button_handles[index]);

            obj = sub_4A2B60(dword_680FA4[index]);
            if (obj != OBJ_HANDLE_NULL
                && (obj_field_int32_get(obj, OBJ_F_FLAGS) & OF_OFF) == 0) {
                portrait = portrait_get(obj);
                if (portrait != 0) {
                    portrait_draw_32x32(obj, portrait, dword_5CABE8, 4, index * stru_680FD0[index % 2].height + 4);
                    if (index == dword_5CABF0) {
                        art_blit_info.flags = TIG_ART_BLT_BLEND_ALPHA_CONST;
                        tig_art_interface_id_create(646, 0, 0, 0, &(art_blit_info.art_id));
                        art_blit_info.alpha[0] = 128;
                        art_blit_info.src_rect = &src_rect;
                        art_blit_info.dst_rect = &dst_rect;

                        dst_rect.y = index * stru_680FD0[0].height + 4;

                        tig_window_blit_art(dword_5CABE8, &art_blit_info);
                    }
                }
            }
        }
    }
}

// 0x571100
void sub_571100()
{
    int index;
    MesFileEntry mes_file_entry;
    TigRect rect;
    TigFont font_desc;
    int64_t obj;
    int v1;
    int ping;
    int font;
    char str[12];

    for (index = 0; index < dword_68103C; index++) {
        tig_font_push(mainmenu_ui_font(MM_FONT_MORPH15, MM_COLOR_WHITE));
        font_desc.width = 0;
        font_desc.str = tig_net_client_info_get_name(dword_680FA4[index]);
        tig_font_measure(&font_desc);
        rect.x = 40;
        rect.y = stru_680FD0[index % 2].height * index + 3;
        rect.width = font_desc.width;
        rect.height = font_desc.height;
        tig_window_text_write(dword_5CABE8, font_desc.str, &rect);
        tig_font_pop();

        obj = sub_4A2B60(dword_680FA4[index]);
        if (obj != OBJ_HANDLE_NULL) {
            v1 = party_id_from_obj(obj);
            if (v1 != -1) {
                tig_font_push(sub_571630(v1));
                rect.x += font_desc.width;
                sprintf(str, " %c", sub_571610(v1));
                font_desc.width = 0;
                font_desc.str = str;
                tig_font_measure(&font_desc);
                rect.width = font_desc.width;
                rect.height = font_desc.height;
                rect.y += 3;
                tig_window_text_write(dword_5CABE8, font_desc.str, &rect);
                rect.y -= 3;
                tig_font_pop();
            }
        }

        tig_font_push(mainmenu_ui_font(MM_FONT_MORPH15, MM_COLOR_WHITE));
        if (tig_net_client_is_waiting(dword_680FA4[index])) {
            mes_file_entry.num = 1002; // "Waiting"
        } else if (tig_net_client_is_loading(dword_680FA4[index])) {
            mes_file_entry.num = 1003; // "Loading"
        } else {
            mes_file_entry.num = 1001; // "Ready"
        }
        mes_get_msg(multiplayer_ui_mes_file, &mes_file_entry);
        font_desc.str = mes_file_entry.str;
        font_desc.width = 0;
        tig_font_measure(&font_desc);
        rect.x = 40;
        rect.y = stru_680FD0[index % 2].height * index + 22;
        rect.width = font_desc.width;
        rect.height = font_desc.height;
        tig_window_text_write(dword_5CABE8, font_desc.str, &rect);
        tig_font_pop();

        if ((tig_net_is_host()
                && dword_680FA4[index] != 0)
            || (!tig_net_is_host()
                && dword_680FA4[index] == 0)) {
            ping = sub_526D60(dword_680FA4[index]);
            if (ping < 251) {
                font = mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_GREEN);
            } else if (ping < 501) {
                font = mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_GOLD);
            } else {
                font = mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED);
            }
            tig_font_push(font);
            rect.x = font_desc.width + 40;
            snprintf(str, sizeof(str), "(%d)", ping);
            font_desc.width = 0;
            font_desc.str = str;
            tig_font_measure(&font_desc);
            rect.width = font_desc.width;
            rect.height = font_desc.height;
            tig_window_text_write(dword_5CABE8, font_desc.str, &rect);
            tig_font_pop();
        }
    }
}

// 0x5713F0
bool sub_5713F0(TigMessage* msg)
{
    int index;

    switch (msg->type) {
    case TIG_MESSAGE_MOUSE:
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP
            && (tig_net_local_server_get_options() & TIG_NET_SERVER_PRIVATE_CHAT) != 0) {
            int x;
            int y;

            sub_571570(msg->data.mouse.x,
                msg->data.mouse.y,
                &x,
                &y);

            if (sub_5715A0(x, y, &index)) {
                if (dword_5CABF0 == -1) {
                    dword_5CABF0 = index;
                    sub_5506C0(3);
                    sub_5708B0(0);
                    return false;
                }

                if (dword_5CABF0 != index) {
                    dword_5CABF0 = index;
                    sub_570A40(dword_5CABEC);
                    sub_5708B0(0);
                    return false;
                }
            } else {
                if (dword_5CABF0 != -1) {
                    sub_5506C0(3);
                    sub_5708B0(0);
                    return false;
                }
                dword_5CABF0 = -1;
            }
            sub_5708B0(0);
        }
        return false;
    case TIG_MESSAGE_BUTTON:
        if (tig_net_is_host()) {
            switch (msg->data.button.state) {
            case TIG_BUTTON_STATE_RELEASED:
                for (index = 0; index < dword_68103C; index++) {
                    if (msg->data.button.button_handle == multiplayer_ui_add_button_handles[index]) {
                        sub_52A9E0(dword_680FA4[index]);
                        return true;
                    }

                    if (msg->data.button.button_handle == multiplayer_ui_kick_button_handles[index]) {
                        dword_5CABF0 = index;
                        sub_5506C0(10);
                        return true;
                    }
                }
                return false;
            default:
                return false;
            }
        }
        return false;
    default:
        return false;
    }
}

// 0x571570
void sub_571570(int screen_x, int screen_y, int* window_x, int* window_y)
{
    if (window_x != NULL) {
        *window_x = screen_x - 566;
    }

    if (window_y != NULL) {
        *window_y = screen_y - 41;
    }
}

// 0x5715A0
bool sub_5715A0(int x, int y, int* index_ptr)
{
    int index;

    if (x < 4 || x > 36
        || y <= 0 || y >= 10 * stru_680FD0[0].height) {
        return false;
    }

    if (y % stru_680FD0[0].height < 4
        || y % stru_680FD0[0].height > 36) {
        return false;
    }

    index = y / stru_680FD0[0].height;
    if (tig_net_client_is_waiting(dword_680FA4[index])) {
        return false;
    }

    if (index_ptr != NULL) {
        *index_ptr = index;
    }

    return true;
}

// 0x571610
char sub_571610(int a1)
{
    return byte_599458[a1 % 10];
}

// 0x571630
tig_font_handle_t sub_571630(int a1)
{
    return mainmenu_ui_font(MM_FONT_MPICONS, (a1 + a1 / MM_COLOR_COUNT) % MM_COLOR_COUNT);
}

// 0x571660
void sub_571660(TextEdit* textedit)
{
    tig_color_t color;

    color = tig_color_make(2, 0, 116);
    if (textedit->buffer[0] != '\0') {
        intgame_text_edit_refresh_color(textedit->buffer,
            mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE),
            color,
            1);
    } else {
        intgame_text_edit_refresh_color("",
            mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE),
            color,
            1);
    }
}

// 0x571730
void sub_571730(TextEdit* textedit)
{
    Packet83 pkt;

    if (textedit->buffer[0] != '\0') {
        pkt.type = 83;
        pkt.field_4 = sub_529520();
        pkt.field_8 = dword_5CABF0;
        strncpy(pkt.field_C, textedit->buffer, 128);

        if (pkt.field_8 != pkt.field_4) {
            if (tig_net_is_host() && pkt.field_8 != 0) {
                tig_net_send_app(&pkt, sizeof(pkt), pkt.field_8);
            } else {
                tig_net_send_app_all(&pkt, sizeof(pkt));
            }
        }

        sub_5703D0(textedit->buffer, sub_529520());
        *textedit->buffer = '\0';

        sub_571660(textedit);
        textedit_ui_focus(textedit);
    }
}
