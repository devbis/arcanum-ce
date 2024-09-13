#include "ui/mainmenu_ui.h"

#include "game/gamelib.h"
#include "game/mes.h"
#include "game/obj.h"
#include "ui/scrollbar_ui.h"
#include "ui/server_list_ui.h"
#include "ui/textedit_ui.h"

// 0x5993D0
static int dword_5993D0[4] = {
    229,
    26,
    27,
    841,
};

// 0x5993E0
static int dword_5993E0[10][3] = {
    { 255, 255, 255 },
    { 255, 0, 0 },
    { 0, 0, 255 },
    { 255, 210, 0 },
    { 32, 15, 0 },
    { 0, 255, 0 },
    { 150, 0, 150 },
    { 60, 160, 255 },
    { 255, 128, 0 },
    { 128, 128, 128 },
};

// 0x5C3FB4
static mes_file_handle_t mainmenu_ui_mainmenu_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5C4000
static bool dword_5C4000 = true;

// 0x5C4004
static bool dword_5C4004 = true;

// 0x5C402C
static mes_file_handle_t mainmenu_ui_multiplayer_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5C4030
static int dword_5C4030[2] = {
    229,
    27,
};

// 0x5C4038
static int dword_5C4038 = 171;

// 0x5C403C
static int dword_5C403C = 327;

// 0x5C4040
static int dword_5C4040[3][3] = {
    { 0, 0, 0 },
    { 97, 61, 42 },
    { 100, 0, 0 },
};

// 0x5C4064
static int dword_5C4064[3] = {
    200,
    200,
    200,
};

// 0x5C4070
static int dword_5C4070[3] = {
    -1,
    2,
    -1,
};

// 0x64BC04
static tig_font_handle_t dword_64BC04[3];

// 0x64BC10
static tig_font_handle_t dword_64BC10[3];

// 0x64C0CC
static tig_font_handle_t dword_64C0CC[3];

// 0x64C0D8
static tig_font_handle_t dword_64C0D8[3];

// 0x64C0E8
static MesFileEntry stru_64C0E8;

// 0x64C210
static tig_font_handle_t dword_64C210[2];

// 0x64C218
static tig_font_handle_t dword_64C218[2];

// 0x64C228
static tig_font_handle_t dword_64C228[3];

// 0x64C234
static tig_font_handle_t dword_64C234[3];

// 0x64C240
static tig_font_handle_t dword_64C240;

// 0x64C244
static bool dword_64C244;

// 0x64C248
static ObjectID stru_64C248;

// 0x64C2F4
static mes_file_handle_t mainmenu_ui_rules_mainmenu_mes_file;

// 0x64C2F8
static char byte_64C2F8[128];

// 0x64C380
static bool mainmenu_ui_initialized;

// 0x64C37C
static char* dword_64C37C;

// 0x64C384
static bool dword_64C384;

// 0x64C388
static bool dword_64C388;

// 0x64C390
static int dword_64C390;

// 0x64C414
static int dword_64C414;

// 0x64C418
static bool dword_64C418;

// 0x64C41C
static void* dword_64C41C;

// 0x64C42C
static int dword_64C42C[3];

// 0x64C438
static bool mainmenu_ui_was_compact_interface;

// 0x540930
bool mainmenu_ui_init(GameInitInfo* init_info)
{
    int index;
    TigFont font_desc;
    MesFileEntry mes_file_entry;

    (void)init_info;

    if (mainmenu_ui_mainmenu_mes_file == MES_FILE_HANDLE_INVALID) {
        if (!mes_load("mes\\mainmenu.mes", &mainmenu_ui_mainmenu_mes_file)) {
            return false;
        }
    }

    if (!mes_load("rules\\mainmenu.mes", &mainmenu_ui_rules_mainmenu_mes_file)) {
        return false;
    }

    if (!mes_load("mes\\multiplayer.mes", &mainmenu_ui_multiplayer_mes_file)) {
        return false;
    }

    stru_64C0E8.num = 3000;
    mes_get_msg(mainmenu_ui_multiplayer_mes_file, &stru_64C0E8);

    settings_add(&settings, "won_account", "", won_account_changed);
    settings_add(&settings, "won_password", "", won_password_changed);
    settings_add(&settings, "show version", "0", NULL);
    won_account_changed();
    won_password_changed();

    settings_add(&settings, "selected_char_id", "", selected_char_id_changed);
    selected_char_id_changed();

    sub_549850();

    for (index = 0; index < 3; index++) {
        font_desc.flags = 0;
        if (index == 1) {
            font_desc.flags = TIG_FONT_BLEND_ADD;
        }
        tig_art_interface_id_create(dword_5C403C, 0, 0, 0, &(font_desc.art_id));
        font_desc.str = NULL;
        font_desc.color = tig_color_make(dword_5C4040[index][0], dword_5C4040[index][1], dword_5C4040[index][2]);
        tig_font_create(&font_desc, &(dword_64C228[index]));

        tig_art_interface_id_create(dword_5C403C, 0, 0, 0, &(font_desc.art_id));
        font_desc.color = index < 2
            ? tig_color_make(dword_5C4040[index][0], dword_5C4040[index][1], dword_5C4040[index][2])
            : tig_color_make(240, 15, 15);
        tig_font_create(&font_desc, &(dword_64C234[index]));
    }

    for (index = 0; index < 3; index++) {
        font_desc.flags = 0;
        tig_art_interface_id_create(dword_5C4038, 0, 0, 0, &(font_desc.art_id));
        font_desc.str = NULL;
        font_desc.color = tig_color_make(dword_64C42C[index], dword_64C42C[index], dword_64C42C[index]);
        tig_font_create(&font_desc, &(dword_64C0CC[index]));

        tig_art_interface_id_create(dword_5C4038, 0, 0, 0, &(font_desc.art_id));
        font_desc.color = index < 2
            ? tig_color_make(dword_64C42C[index], dword_64C42C[index], dword_64C42C[index])
            : tig_color_make(240, 15, 15);
        tig_font_create(&font_desc, &(dword_64C0D8[index]));
    }

    for (index = 0; index < 3; index++) {
        font_desc.flags = 0;
        tig_art_interface_id_create(dword_5C4038, 0, 0, 0, &(font_desc.art_id));
        font_desc.str = NULL;
        font_desc.color = tig_color_make(dword_5C4064[index], dword_5C4064[index], dword_5C4064[index]);
        tig_font_create(&font_desc, &(dword_64BC04[index]));

        tig_art_interface_id_create(dword_5C4038, 0, 0, 0, &(font_desc.art_id));
        font_desc.color = index < 2
            ? tig_color_make(dword_5C4064[index], dword_5C4064[index], dword_5C4064[index])
            : tig_color_make(240, 15, 15);
        tig_font_create(&font_desc, &(dword_64BC10[index]));
    }

    for (index = 0; index < 2; index++) {
        font_desc.flags = 0;
        tig_art_interface_id_create(dword_5C4030[index], 0, 0, 0, &(font_desc.art_id));
        font_desc.str = NULL;
        font_desc.color = tig_color_make(250, 250, 250);
        tig_font_create(&font_desc, &(dword_64C210[index]));

        font_desc.flags = 0;
        tig_art_interface_id_create(dword_5C4030[index], 0, 0, 0, &(font_desc.art_id));
        font_desc.str = NULL;
        font_desc.color = tig_color_make(255, 50, 50);
        tig_font_create(&font_desc, &(dword_64C218[index]));
    }

    font_desc.flags = 0;
    tig_art_interface_id_create(dword_5C4030[0], 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 210, 0);
    tig_font_create(&font_desc, &dword_64C240);

    mes_file_entry.num = 500;
    mes_get_msg(mainmenu_ui_mainmenu_mes_file, &mes_file_entry);
    strncpy(byte_64C2F8, mes_file_entry.str, 23);

    mainmenu_ui_initialized = true;
    dword_64C414 = 0;

    dword_64C388 = true;
    mainmenu_ui_start(0);
    dword_64C388 = false;

    sub_4044A0(468, 300);
    dword_64C418 = 0;
    serverlist_ui_init();
    stru_64C248.type = 0;
    sub_549A80();
    dword_64C37C = NULL;

    return true;
}

// 0x541050
void mainmenu_ui_exit()
{
    int index;

    sub_5412E0(1);
    settings_set_obj_value(&settings, "selected_char_id", stru_64C248);
    serverlist_ui_exit();
    sub_549910();

    for (index = 0; index < 3; index++) {
        tig_font_destroy(dword_64C228[index]);
        tig_font_destroy(dword_64C234[index]);
        tig_font_destroy(dword_64C0CC[index]);
        tig_font_destroy(dword_64C0D8[index]);
        tig_font_destroy(dword_64BC04[index]);
        tig_font_destroy(dword_64BC10[index]);
    }

    for (index = 0; index < 2; index++) {
        tig_font_destroy(dword_64C210[index]);
        tig_font_destroy(dword_64C218[index]);
    }

    tig_font_destroy(dword_64C240);
    mes_unload(mainmenu_ui_rules_mainmenu_mes_file);
    mes_unload(mainmenu_ui_mainmenu_mes_file);
    mes_unload(mainmenu_ui_multiplayer_mes_file);

    if (dword_64C41C != NULL) {
        FREE(dword_64C41C);
    }

    mainmenu_ui_initialized = false;
}

// 0x541150
void sub_541150()
{
    dword_64C414 = 2;
    dword_64C418 = 0;
    dword_64C424 = 0;
}

// 0x541170
void sub_541170()
{
    dword_5C4000 = true;
}

// 0x541180
void mainmenu_ui_start(int window_type)
{
    tig_art_id_t art_id;

    if (!dword_64C384) {
        dword_64C390 = 0;

        if (window_type != 4 && (tig_net_flags & TIG_NET_CONNECTED) == 0) {
            sub_45B320();
        }

        tig_art_interface_id_create(0, 0, 0, 0, &art_id);
        tig_mouse_cursor_set_art_id(art_id);
        inven_ui_destroy();
        sub_570130();

        if (window_type == 0 && !dword_5C4000) {
            window_type = 1;
        }

        switch (window_type) {
        case 2:
            dword_64C414 = 3;
            dword_5C4004 = false;
            sub_541740();
            sub_43C270(OBJ_HANDLE_NULL);
            break;
        case 3:
            dword_64C414 = 4;
            dword_5C4004 = false;
            sub_541740();
            sub_43C270(OBJ_HANDLE_NULL);
            break;
        case 4:
            dword_64C414 = 6;
            dword_5C4004 = false;
            sub_541740();
            sub_43C270(OBJ_HANDLE_NULL);
            break;
        case 5:
            dword_64C414 = 26;
            dword_5C4004 = false;
            sub_541740();
            sub_43C270(OBJ_HANDLE_NULL);
            break;
        default:
            if (tig_mouse_hide() != TIG_OK) {
                tig_debug_printf("mainmenu_ui_start: ERROR: tig_mouse_hide failed!\n");
                exit(EXIT_FAILURE);
            }
            dword_64C414 = 0;
            dword_5C4004 = true;
            sub_541740();
            sub_43C270(OBJ_HANDLE_NULL);
            break;
        }
    }
}

// 0x5412D0
void sub_5412D0()
{
    sub_5412E0(0);
}

// 0x5412E0
void sub_5412E0()
{
    // TODO: Incomplete.
}

// 0x541590
bool mainmenu_ui_handle()
{
    tig_timestamp_t timestamp;
    TigMessage msg;

    mainmenu_ui_was_compact_interface = intgame_is_compact_interface();
    if (mainmenu_ui_was_compact_interface) {
        intgame_toggle_interface();
    }

    if (dword_64C414 < 2) {
        sub_5417A0(0);
        dword_64C414 = 2;
        sub_541740();

        if (tig_mouse_show() != TIG_OK) {
            tig_debug_printf("mainmenu_ui_handle: ERROR: tig_mouse_show failed!\n");
        }
    }

    sub_571910();

    while (dword_64C384) {
        tig_ping();
        tig_timer_now(&timestamp);
        timeevent_ping(timestamp);
        gamelib_ping();
        iso_redraw();
        tig_window_display();

        while (tig_message_dequeue(&msg) == TIG_OK) {
            switch (msg.type) {
            case TIG_MESSAGE_QUIT:
                if (!sub_541690()) {
                    return false;
                }
                break;
            case TIG_MESSAGE_REDRAW:
                sub_4045A0();
                break;
            }
        }

        tig_window_display();
    }

    return true;
}

// 0x541680
void sub_541680()
{
    return dword_64C384;
}

// 0x541690
bool sub_541690()
{
    return sub_5416A0(5100);
}

// 0x5416A0
TigWindowModalDialogChoice sub_5416A0(int num)
{
    MesFileEntry mes_file_entry;
    TigWindowModalDialogInfo modal_info;
    TigWindowModalDialogChoice choice;

    mes_file_entry.num = num;
    mes_get_msg(dword_5C3FB4, &mes_file_entry);

    modal_info.type = TIG_WINDOW_MODAL_DIALOG_TYPE_OK_CANCEL;
    modal_info.x = 237;
    modal_info.y = 232;
    modal_info.text = mes_file_entry.text;
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_OK] = 'y';
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_CANCEL] = 'n';
    modal_info.process = NULL;
    modal_info.redraw = sub_4045A0;
    tig_window_modal_dialog(&modal_info, &choice);

    return choice;
}

// 0x541710
void sub_541710()
{
    inven_ui_destroy();
    sub_55A150();
    sub_57B450();
    sub_560F40();
    sub_53F090();
    sub_56FC40();
    sub_56D2D0();
    gamelib_reset();
    gameuilib_reset();
}

// 0x541740
void sub_541740()
{
    // TODO: Incomplete.
}

// 0x5417A0
void sub_5417A0()
{
    // TODO: Incomplete.
}

// 0x5417E0
void sub_5417E0()
{
    // TODO: Incomplete.
}

// 0x541810
void sub_541810()
{
    // TODO: Incomplete.
}

// 0x541830
void sub_541830()
{
    // TODO: Incomplete.
}

// 0x5418A0
void sub_5418A0()
{
    // TODO: Incomplete.
}

// 0x541AA0
void sub_541AA0()
{
    dword_64C414 = 2;
    sub_546330();
    ++dword_64C43C;
    sub_541AC0();
}

// 0x541AC0
void sub_541AC0()
{
    TigRect rect;
    char version[40];

    rect.x = 10;
    rect.y = 575;
    rect.width = 400;
    rect.height = 20;

    if (settings_get_value(game_config, "show version") != 0
        && gamelib_copy_version(version, 0, 0)) {
        tig_font_push(dword_64BC04[0]);
        if (tig_window_text_write(dword_5C3624, version, &rect) != TIG_OK) {
            tig_debug_printf("MainMenuUI: ERROR: GameLib_Version_String Failed to draw!\n");
        }
        tig_font_pop();
    }
}

// 0x541B50
void sub_541B50()
{
    // TODO: Incomplete.
}

// 0x541BE0
void sub_541BE0()
{
    // TODO: Incomplete.
}

// 0x541C70
void sub_541C70()
{
    // TODO: Incomplete.
}

// 0x541D40
void sub_541D40()
{
    inven_ui_destroy();
    sub_55A150();
    sub_57B450();
    sub_560F40();
    sub_53F090();
    sub_56FC40();
    sub_56D2D0();
}

// 0x541D70
void sub_541D70()
{
    sub_5894C0();
    sub_550DA0(0, 0);
}

// 0x541D90
void sub_541D90()
{
    // TODO: Incomplete.
}

// 0x541E20
void sub_541E20()
{
    // TODO: Incomplete.
}

// 0x541F20
void mainmenu_ui_create_load_game()
{
    // TODO: Incomplete.
}

// 0x542200
void sub_542200()
{
    inven_ui_destroy();
    sub_55A150();
    sub_57B450();
    sub_560F40();
    sub_53F090();
    sub_56FC40();
    sub_56D2D0();
}

// 0x542230
void sub_542230()
{
    // TODO: Incomplete.
}

// 0x542280
void sub_542280()
{
    // TODO: Incomplete.
}

// 0x5422A0
void sub_5422A0()
{
    // TODO: Incomplete.
}

// 0x5422C0
void sub_5422C0()
{
    // TODO: Incomplete.
}

// 0x542420
void sub_542420()
{
    // TODO: Incomplete.
}

// 0x542460
void sub_542460()
{
    // TODO: Incomplete.
}

// 0x5424F0
void sub_5424F0()
{
    // TODO: Incomplete.
}

// 0x542560
void sub_542560()
{
    // TODO: Incomplete.
}

// 0x5425C0
void mmUIMPLoadGameRefreshFunc()
{
    // TODO: Incomplete.
}

// 0x542D00
void sub_542D00()
{
    // TODO: Incomplete.
}

// 0x542DF0
void sub_542DF0()
{
    // TODO: Incomplete.
}

// 0x542EA0
void sub_542EA0()
{
    // TODO: Incomplete.
}

// 0x542F50
void mmUITextWriteCenteredToArray()
{
    // TODO: Incomplete.
}

// 0x543040
void sub_543040()
{
    // TODO: Incomplete.
}

// 0x543060
void sub_543060()
{
    // TODO: Incomplete.
}

// 0x5430D0
void sub_5430D0()
{
    // TODO: Incomplete.
}

// 0x543160
void sub_543160()
{
    // TODO: Incomplete.
}

// 0x543220
void sub_543220()
{
    // TODO: Incomplete.
}

// 0x5432B0
void sub_5432B0()
{
    // TODO: Incomplete.
}

// 0x543380
void mainmenu_ui_create_save_game()
{
    // TODO: Incomplete.
}

// 0x543580
void sub_543580()
{
    // TODO: Incomplete.
}

// 0x5435D0
void sub_5435D0()
{
    // TODO: Incomplete.
}

// 0x543850
void sub_543850()
{
    // TODO: Incomplete.
}

// 0x543890
void sub_543890()
{
    // TODO: Incomplete.
}

// 0x543920
void sub_543920()
{
    // TODO: Incomplete.
}

// 0x543990
void mmUIMPSaveGameRefreshFunc()
{
    // TODO: Incomplete.
}

// 0x544100
void sub_544100()
{
    // TODO: Incomplete.
}

// 0x544210
void sub_544210()
{
    // TODO: Incomplete.
}

// 0x544250
void sub_544250()
{
    // TODO: Incomplete.
}

// 0x544290
void sub_544290()
{
    // TODO: Incomplete.
}

// 0x544320
void sub_544320()
{
    // TODO: Incomplete.
}

// 0x544440
void sub_544440()
{
    // TODO: Incomplete.
}

// 0x5445F0
void sub_5445F0()
{
    // TODO: Incomplete.
}

// 0x544640
void sub_544640()
{
    // TODO: Incomplete.
}

// 0x544690
void nullsub_68()
{
    // TODO: Incomplete.
}

// 0x5446A0
void sub_5446A0()
{
    // TODO: Incomplete.
}

// 0x5446D0
void sub_5446D0()
{
    dword_64C454 = 0;
    dword_64C414 = 11;
    sub_546330();
    sub_541AC0();
}

// 0x5446F0
void sub_5446F0()
{
    // TODO: Incomplete.
}

// 0x5447B0
void sub_5447B0()
{
    // TODO: Incomplete.
}

// 0x5448E0
void mmUINewCharRefreshFunc()
{
    // TODO: Incomplete.
}

// 0x544FF0
void sub_544FF0()
{
    // TODO: Incomplete.
}

// 0x5452C0
void sub_5452C0()
{
    // TODO: Incomplete.
}

// 0x545300
void sub_545300()
{
    // TODO: Incomplete.
}

// 0x545350
void sub_545350()
{
    // TODO: Incomplete.
}

// 0x5453A0
void sub_5453A0()
{
    // TODO: Incomplete.
}

// 0x545440
void sub_545440()
{
    // TODO: Incomplete.
}

// 0x545490
void sub_545490()
{
    // TODO: Incomplete.
}

// 0x545550
void sub_545550()
{
    // TODO: Incomplete.
}

// 0x5455D0
void sub_5455D0()
{
    // TODO: Incomplete.
}

// 0x545690
void sub_545690()
{
    // TODO: Incomplete.
}

// 0x5456A0
void sub_5456A0()
{
    // TODO: Incomplete.
}

// 0x5456B0
void sub_5456B0()
{
    // TODO: Incomplete.
}

// 0x545780
void sub_545780()
{
    // TODO: Incomplete.
}

// 0x545870
void sub_545870()
{
    // TODO: Incomplete.
}

// 0x5458D0
void sub_5458D0()
{
    // TODO: Incomplete.
}

// 0x5459F0
void sub_5459F0()
{
    // TODO: Incomplete.
}

// 0x545B60
void mmUIPregenCharExecuteFunc()
{
    // TODO: Incomplete.
}

// 0x545C50
void sub_545C50()
{
    // TODO: Incomplete.
}

// 0x545C60
void sub_545C60()
{
    // TODO: Incomplete.
}

// 0x545C70
void sub_545C70()
{
    // TODO: Incomplete.
}

// 0x545DF0
void sub_545DF0()
{
    // TODO: Incomplete.
}

// 0x545E20
void sub_545E20()
{
    // TODO: Incomplete.
}

// 0x545E30
void sub_545E30()
{
    // TODO: Incomplete.
}

// 0x545E40
void sub_545E40()
{
    // TODO: Incomplete.
}

// 0x545E80
void sub_545E80()
{
    // TODO: Incomplete.
}

// 0x545F60
void sub_545F60()
{
    // TODO: Incomplete.
}

// 0x545FD0
void sub_545FD0()
{
    // TODO: Incomplete.
}

// 0x5460A0
void sub_5460A0()
{
    tig_art_interface_id_create(753, 0, 0, 0, &dword_64C2A4);
    dword_64C414 = 22;

    stru_64C2B0.flags = 781;
    stru_64C2B0.field_4.x = 287;
    stru_64C2B0.field_4.y = 204;
    stru_64C2B0.field_4.width = 16;
    stru_64C2B0.field_4.height = 344;
    stru_64C2B0.field_24 = dword_64C420 > 17 ? dword_64C420 - 17 : 0;
    stru_64C2B0.field_28 = 0;
    stru_64C2B0.field_40 = sub_548B60;
    stru_64C2B0.field_3C = sub_546180;

    sub_546330();
    scrollbar_ui_control_create(&stru_64C2A8, &stru_64C2B0, sub_549820());
    sub_5806F0(stru_64C2A8);
}

// 0x546160
void sub_546160()
{
    scrollbar_ui_control_destroy(stru_64C2A8);
}

// 0x546180
bool sub_546180(int a1)
{
    dword_64C378 = a1;
    sub_548BE0(dword_5C5640);
    return false;
}

// 0x5461A0
bool main_menu_button_create(MainMenuButtonInfo *info, int width, int height)
{
    return main_menu_button_create_ex(info, width, height, TIG_BUTTON_FLAG_0x01);
}

// 0x5461C0
bool main_menu_button_create_ex(MainMenuButtonInfo *info, int width, int height, unsigned int flags)
{
    // TODO: Incomplete.
}

// 0x546330
void sub_546330()
{
    // TODO: Incomplete.
}

// 0x546340
void mainmenu_ui_create_window_func()
{
    // TODO: Incomplete.
}

// 0x546B40
void mainmenu_ui_refresh_text()
{
    // TODO: Incomplete.
}

// 0x546DD0
void sub_546DD0()
{
    // TODO: Incomplete.
}

// 0x546E80
void mainmenu_ui_create_shared_radio_buttons()
{
    // TODO: Incomplete.
}

// 0x546EE0
void sub_546EE0()
{
    // TODO: Incomplete.
}

// 0x547E00
void mainmenu_ui_refresh_button_text()
{
    // TODO: Incomplete.
}

// 0x547EF0
void sub_547EF0()
{
    // TODO: Incomplete.
}

// 0x547F90
void mainmenu_ui_process_callback()
{
    // TODO: Incomplete.
}

// 0x5480C0
void sub_5480C0()
{
    // TODO: Incomplete.
}

// 0x548210
void sub_548210()
{
    // TODO: Incomplete.
}

// 0x5482A0
void sub_5482A0()
{
    // TODO: Incomplete.
}

// 0x548B60
void sub_548B60(const TigRect* rect)
{
    TigRect screen_rect;
    TigArtBlitSpec blit_info;

    screen_rect.x = 0;
    screen_rect.y = 0;
    screen_rect.width = 800;
    screen_rect.height = 600;
    tig_art_interface_id_create(753, 0, 0, 0, &(blit_info.art_id));

    blit_info.flags = 0;
    if (rect != NULL) {
        blit_info.src_rect = rect;
        blit_info.dst_rect = rect;
    } else {
        blit_info.src_rect = &screen_rect;
        blit_info.dst_rect = &screen_rect;
    }

    tig_window_blit_art(sub_549820(), &blit_info);
    sub_549960();
}

// 0x548BE0
void sub_548BE0()
{
    // TODO: Incomplete.
}

// 0x548F10
void mmUIWinRefreshScrollBar()
{
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitSpec blit_info;
    TigArtFrameData art_frame_data;
    tig_art_id_t art_id;
    MainMenuWindowInfo* curr_window_info;

    curr_window_info = main_menu_window_info[dword_64C414];
    if (curr_window_info->field_78 > 0) {
        tig_art_interface_id_create(316, 0, 0, 0, &art_id);
        if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
            tig_debug_printf("mmUIWinRefreshScrollBar: ERROR: tig_art_frame_data failed!\n");
            exit(EXIT_FAILURE);
        }

        src_rect.x = 0;
        src_rect.y = curr_window_info->field_84 + art_frame_data.height / 2 - curr_window_info->field_7C / 2;
        src_rect.width = art_frame_data.width;
        src_rect.height = curr_window_info->field_7C;

        dst_rect.x = curr_window_info->field_70;
        dst_rect.y = curr_window_info->field_74;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = curr_window_info->field_7C;

        blit_info.flags = 0;
        blit_info.art_id = art_id;
        blit_info.src_rect = &src_rect;
        blit_info.dst_rect = &dst_rect;

        tig_window_blit_art(dword_5C3624, &blit_info);
    }
}

// 0x548FF0
void sub_548FF0(int a1)
{
    MainMenuWindowInfo* curr_window_info;

    curr_window_info = main_menu_window_info[dword_64C414];
    if (a1 < 0) {
        a1 = 0;
    } else if (a1 > curr_window_info->field_88) {
        a1 = curr_window_info->field_88;
    }

    if (curr_window_info->field_84 != a1) {
        curr_window_info->field_84 = a1;
        mmUIWinRefreshScrollBar();
        if (curr_window_info->field_54 != NULL) {
            curr_window_info->field_54(NULL);
        }
    }
}

// 0x549040
void sub_549040()
{
    // TODO: Incomplete.
}

// 0x549290
bool mainmenu_ui_execute_multiplayer_select_char(int index)
{
    switch (index) {
    case 0:
        sub_57C4B0();
        sub_5417A0(0);
        sub_583200();
        if (dword_64C384) {
            sub_541810(dword_64C414);
        }
        break;
    case 1:
        sub_5417A0(0);
        sub_5446F0();
        if (dword_64C384) {
            sub_541810(dword_64C414);
        }
        break;
    default:
        tig_debug_printf("MainMenuUI: mainmenu_ui_execute_multiplayer_select_char unknown button Idx %d.\n", index);
        break;
    }

    return false;
}

// 0x549310
bool sub_549310(tig_button_handle_t button_handle)
{
    if (button_handle != TIG_BUTTON_HANDLE_INVALID) {
        tig_button_show(button_handle);
    }

    if (dword_64C384) {
        sub_5417A0(0);
        dword_64C414 = 2;
        dword_64C390 = 0;
        dword_64C244 = !dword_5C4000;
        sub_541740();
    } else {
        sub_53EAF0();
    }

    if (!gamelib_mod_load(gamelib_get_default_module_name())
        || !gameuilib_mod_load()) {
        tig_debug_printf("MainMenu: Unable to load default module %s.\n",
            gamelib_get_default_module_name());
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
        tig_net_flags |= TIG_NET_RESET;
    }

    sub_541710();

    return true;
}

// 0x5493C0
void sub_5493C0(char* buffer, int size)
{
    MainMenuWindowInfo* curr_window_info;

    if (!dword_64C428) {
        if (byte_64C2F8[0] != '\0') {
            strcpy(byte_64C0F0, a1);
        }

        stru_5C4008.size = size;
        stru_5C4008.flags = dword_64C414 == 8;
        stru_5C4008.buffer = buffer;
        textedit_ui_focus(&stru_5C4008);
        dword_64C428 = true;

        curr_window_info = main_menu_window_info[dword_64C414];
        curr_window_info->field_54(&(curr_window_info->field_5C));

        sub_549A40();
    }
}

// 0x549450
void sub_549450()
{
    MainMenuWindowInfo* curr_window_info;

    if (dword_64C428) {
        if (stru_5C4008.buffer[0] == '\0') {
            strcpy(stru_5C4008.buffer, byte_64C0F0);
        }
        textedit_ui_unfocus(&stru_5C4008);
        dword_64C428 = false;

        curr_window_info = main_menu_window_info[dword_64C414];
        curr_window_info->field_54(&(curr_window_info->field_5C));

        sub_549A50();
    }
}

// 0x5494C0
void sub_5494C0(TextEdit* textedit)
{
    MesFileEntry mes_file_entry;

    if (textedit->buffer[0] == '\0' && dword_64C414 != 8) {
        mes_file_entry.num = 500;
        mes_get_msg(dword_5C3FB4, &mes_file_entry);
        strncpy(&byte_64C2F8, mes_file_entry.text, 23);
    }

    sub_549450();

    if (dword_64C414 == 8) {
        sub_5435D0(-1);
    }
}

// 0x549520
char* sub_549520()
{
    return dword_64C428 != 0 ? stru_5C4008.buffer : NULL;
}

// 0x549540
void sub_549540()
{
    MainMenuWindowInfo* curr_window_info;

    curr_window_info = main_menu_window_info[dword_64C414];
    curr_window_info->field_54(&(curr_window_info->field_5C));

    if (dword_64C414 == 8) {
        sub_5806F0(stru_64C220);
    }
}

// 0x549580
void mainmenu_ui_exit_game()
{
    tig_debug_printf("mainmenu_ui_exit_game: Exiting Game!\n");
    gameuilib_mod_unload();
    gamelib_mod_unload();
    gameuilib_exit();
    gamelib_exit();
    tig_exit();
    tig_memory_print_stats(TIG_MEMORY_STATS_PRINT_GROUPED_BLOCKS);
    exit(EXIT_SUCCESS);
}

// 0x5495F0
void sub_5495F0(int a1)
{
    dword_64C448 = a1;
    if (dword_64C384) {
        if (main_menu_window_info[dword_64C414]->field_54 != NULL) {
            main_menu_window_info[dword_64C414]->field_54(&stru_5C4538);
        }
    }
}

// 0x549620
void sub_549620(int a1)
{
    dword_64C44C = a1;
    if (dword_64C384) {
        if (main_menu_window_info[dword_64C414]->field_54 != NULL) {
            main_menu_window_info[dword_64C414]->field_54(&stru_5C4538);
        }
    }
}

// 0x549650
void won_account_changed()
{
    sub_584AA0(settings_get_str_value(game_config, "won_account"));
}

// 0x549670
void won_password_changed()
{
    sub_584AC0(settings_get_str_value(game_config, "won_password"));
}

// 0x549690
void selected_char_id_changed()
{
    stru_64C248 = settings_get_obj_value(game_config, "selected_char_id");
}

// 0x5496C0
MainMenuWindowInfo* sub_5496C0(int index)
{
    return main_menu_window_info[index];
}

// 0x5496D0
int sub_5496D0()
{
    return dword_64C414;
}

// 0x5496E0
void sub_5496E0()
{
    sub_5496F0(5060);
}

// 0x5496F0
void sub_5496F0(int num)
{
    MesFileEntry mes_file_entry;
    John v1;

    mes_file_entry.num = num;
    if (message_find(dword_5C3FB4, &mes_file_entry)) {
        mes_get_msg(dword_5C3FB4, &mes_file_entry);
        v1.field_0 = 6;
        v1.field_4 = mes_file_entry.text;
        sub_550750(&v1);
        tig_window_display();
    }
}

// 0x549750
void sub_549750()
{
    sub_5496F0(5062);
}

// 0x549760
void sub_549760()
{
    sub_5496F0(5065);
}

// 0x549770
void sub_549770()
{
    sub_5496F0(5061);
}

// 0x549780
void sub_549780()
{
    sub_5496F0(5063);
}

// 0x549790
void sub_549790()
{
    // TODO: Incomplete.
}

// 0x549820
int sub_549820()
{
    return dword_5C3624;
}

// 0x549830
void sub_549830(int a1)
{
    dword_64C414 = a1;
}

// 0x549840
int sub_549840()
{
    return dword_5C3FB4;
}

// 0x549850
void sub_549850()
{
    int group;
    int index;
    TigFont font_info;

    for (group = 0; group < 4; group++) {
        tig_art_interface_id_create(dword_5993D0[group], 0, 0, 0, &(font_info.art_id));

        for (index = 0; index < 10; index++) {
            font_info.color = tig_color_make(dword_5993E0[index][0],
                dword_5993E0[index][1],
                dword_5993E0[index][2]);
            tig_font_create(&font_info, &(dword_64C170[group][index]));
        }
    }
}

// 0x549910
void sub_549910()
{
    int group;
    int index;

    for (group = 0; group < 4; group++) {
        for (index = 0; index < 10; index++) {
            tig_font_destroy(dword_64C170[group][index]);
        }
    }
}

// 0x549940
tig_font_handle_t sub_549940(int group, int index)
{
    return dword_64C170[group][index];
}

// 0x549960
void sub_549960()
{
    int index;

    for (index = 0; index < main_menu_window_info[dword_64C414].num_buttons; index++) {
        mainmenu_ui_refresh_button_text(index, 0);
    }
}

// 0x549990
void sub_549990(int* a1, int num)
{
    memcpy(&(dword_64C004[1]), a1, sizeof(*a1) * num);
    dword_64C390 = num + 1;
}

// 0x5499B0
bool sub_5499B0(const char* text)
{
    TigWindowModalDialogInfo modal_info;
    TigWindowModalDialogChoice choice;

    modal_info.type = TIG_WINDOW_MODAL_DIALOG_TYPE_CANCEL;
    modal_info.x = 237;
    modal_info.y = 232;
    modal_info.text = text;
    modal_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_CANCEL] = ' ';
    modal_info.process = sub_549A10;
    modal_info.redraw = sub_4045A0;
    tig_window_modal_dialog(&modal_info, &choice);

    return choice == TIG_WINDOW_MODAL_DIALOG_CHOICE_OK;
}

// 0x549A10
bool sub_549A10(bool* a1)
{
    switch (multiplayer_mm_ping()) {
    case 2:
        return false;
    case 1:
        *a1 = true;
        break;
    default:
        *a1 = false;
        break;
    }

    return true;
}

// 0x549A40
void sub_549A40()
{
    dword_64C468++;
}

// 0x549A50
void sub_549A50()
{
    dword_64C468--;
}

// 0x549A60
int sub_549A60()
{
    return dword_64C468;
}

// 0x549A70
void sub_549A70()
{
    dword_5C3620 = 0;
}

// 0x549A80
void sub_549A80()
{
    long long obj;

    if (!dword_5C3620) {
        obj = objp_perm_lookup(sub_407EF0(sub_4685A0(16075)));
        if (obj != OBJ_HANDLE_NULL
            && tig_art_exists(obj_field_int32_get(obj, OBJ_F_CURRENT_AID)) == TIG_OK) {
            dword_5C3620 = false;
            dword_5C361C = 13;
            stru_5C4E28.num_buttons = 10;
        } else {
            dword_5C3620 = true;
        }
    }
}

// 0x549B10
void sub_549B10()
{
    stru_5C4170.field_24 = 460;
    stru_5C4170.num_buttons = 4;
    stru_5C4170.buttons = stru_5C5818;
}

// 0x549B30
void sub_549B30()
{
    dword_5C3618 = -1;

    if (dword_64C41C != NULL) {
        FREE(dword_64C41C);
        dword_64C420 = 0;
    }

    sub_4A3D70(&dword_64C41C, &dword_64C420);
}
