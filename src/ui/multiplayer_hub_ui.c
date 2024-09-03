#include "ui/multiplayer_hub_ui.h"

#include <stdio.h>

#include "game/gamelib.h"
#include "game/mes.h"
#include "ui/scrollbar_ui.h"
#include "ui/textedit_ui.h"

static void sub_582E50(TigRect* rect);
static void sub_581F80();
static void sub_581FC0(TextEdit* textedit);
static void sub_582790(TigRect* rect);
static void sub_5829D0(TigRect* rect);
static bool sub_582D10(const char* str);
static bool sub_582D20(const char* a, const char* b);
static const char* sub_582D40(const char* str);
static const char* sub_582D50(const char* str);
static void sub_582D60(TigRect* rect);
static void sub_5830F0();
static void sub_5837A0(TigRect* rect);
static const char* sub_584A40(int value);
static const char* sub_584A80();
static const char* sub_584A90();
static void sub_584AA0(const char* str)
static void sub_584AC0(const char* str);
static void sub_584C30(TigRect* rect);
static void sub_585970(char* buffer);
static void sub_585A20();
static void sub_585BA0();
static void sub_585BB0();

// 0x5CBFD8
static TigRect stru_5CBFD8 = { 37, 188, 246, 368 };

// 0x5CBFE8
static TigRect stru_5CBFE8 = { 336, 99, 427, 1170 };

// 0x5CBFF8
static TigRect stru_5CBFF8 = { 354, 536, 406, 35 };

// 0x5CC008
static TigRect stru_5CC008 = { 35, 32, 254, 258 };

// 0x5CC018
static TigRect stru_5CC018 = { 77, 562, 166, 15 };

// 0x5CC028
static TigRect stru_5CC028 = { 19, 178, 286, 394 };

// 0x5CC038
static TigRect stru_5CC038 = { 69, 274, 183, 18 };

// 0x5CC048
static TigRect stru_5CC048 = { 69, 365, 183, 108 };

// 0x68653C
static char won_account[128];

// 0x6865BC
static char char won_password[128];

// 0x68663C
static char byte_68663C[128];

// 0x6866BC
static char byte_6866BC[128];

// 0x6862D8
static mes_file_handle_t dword_6862D8;

// 0x6861E0
static TextEdit stru_6861E0;

// 0x686248
static ScrollbarId stru_686248;

// 0x6862D0
static ScrollbarId stru_6862D0;

// 0x6862DC
static void* dword_6862DC;

// 0x686740
static GameModuleList stru_686740;

// 0x686964
static int dword_686964;

// 0x581700
void multiplayer_hub_ui_init()
{
    // TODO: Incomplete.
}

// 0x5818C0
void multiplayer_hub_ui_exit()
{
    mes_unload(dword_6862D8);
    scrollbar_ui_control_destroy(stru_686248);
    scrollbar_ui_control_destroy(stru_6862D0);
    textedit_ui_unfocus(&stru_6861E0);
    stru_6861E0.buffer = NULL;
    sub_581F80();
}

// 0x581910
void sub_581910(TigRect* rect)
{
    sub_582E50(rect);
    sub_5806F0(stru_686248);
    sub_5806F0(stru_6862D0);
}

// 0x581950
void sub_581950()
{
    // TODO: Incomplete.
}

// 0x5819D0
bool sub_5819D0(int a1)
{
    switch (a1) {
    case 0:
        sub_5417A0(false);
        sub_583200();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    case 1:
        sub_5417A0(false);
        sub_585D50();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    case 2:
        sub_5417A0(false);
        sub_584AE0();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    }

    return false;
}

// 0x581A60
void sub_581A60()
{
    sub_549830(24);
    sub_546330();
    sub_581B10(NULL);
}

// 0x581A80
void sub_581A80()
{
}

// 0x581A90
void sub_581A90()
{
    // TODO: Incomplete.
}

// 0x581B10
void sub_581B10()
{
    // TODO: Incomplete.
}

// 0x581E60
void sub_581E60(int x, int y)
{
    MainMenuWIndowInfo* window_info;

    window_info = sub_5496C0(sub_5496D0());
    x += window_info->field_5C.x;
    y += window_info->field_5C.y;

    if (x >= stru_5CC038.x
        && y >= stru_5CC038.y
        && x < stru_5CC038.x + stru_5CC038.width
        && y < stru_5CC038.y + stru_5CC038.height) {
        sub_5493C0(byte_686250, 23);
    } else if (x >= stru_5CC048.x
        && y >= stru_5CC048.y
        && x < stru_5CC048.x + stru_5CC048.width
        && y < stru_5CC048.y + stru_5CC048.height) {
        sub_5493C0(byte_684688, 23);
    }
}

// 0x581F10
void sub_581F10()
{
    if (dword_6862DC) {
        sub_4A5040(dword_6862DC);
        dword_6862DC = NULL;
    }
}

// 0x581F30
void sub_581F30()
{
    // TODO: Incomplete.
}

// 0x581F80
void sub_581F80()
{
}

// 0x581F90
void sub_581F90(int a1)
{
    dword_5CBF80 = a1;
    sub_582AD0(NULL);
}

// 0x581FB0
void sub_581FB0()
{
    sub_582D60(NULL);
}

// 0x581FC0
void sub_581FC0(TextEdit* textedit)
{
    MesFileEntry mes_file_entry;

    if (byte_6861F8[0] == '\0') {
        sub_5826D0(NULL, 3, NULL);
    } else if (!sub_4A50C0(byte_6861F8)) {
        if (byte_6861F8[0] == '/') {
            sub_582060(&(byte_6861F8[1]));
        } else {
            mes_file_entry.num = 10053;
            mes_get_msg(sub_549840(), &mes_file_entry);
            sub_5826D0(NULL, 2, mes_file_entry.str)
        }
    }

    sub_582AD0(NULL);
    sub_5672A0();
}

// 0x582060
void sub_582060()
{
    // TODO: Incomplete.
}

// 0x582440
void sub_582440()
{
    // TODO: Incomplete.
}

// 0x5824C0
void sub_5824C0()
{
    // TODO: Incomplete.
}

// 0x582510
void sub_582510()
{
    // TODO: Incomplete.
}

// 0x5825B0
void sub_5825B0(const char* name)
{
    MesFileEntry mes_file_entry;
    char str[80];

    mes_file_entry.num = 10055; // "%s has left."
    mes_get_msg(sub_549840(), &mes_file_entry);

    snprintf(str, sizeof(str), mes_file_entry.str, name);
    sub_5826D0(NULL, 10, str);
    sub_582AD0(NULL);

    if (dword_6862DC != NULL) {
        sub_4A5040(dword_6862DC);
    }

    if (!sub_4A5020(&dword_6862DC, (int)&dword_6862E0)
        && sub_5499B0(dword_6861DC)) {
        dword_6862DC = NULL;
    }

    sub_582860(NULL);
}

// 0x582650
void sub_582650(const char* a1, const char* a2)
{
    sub_5826D0(a1, 0, a2);
    sub_582AD0(NULL);
}

// 0x582670
void sub_582670(const char* a1, const char* a2)
{
    sub_5826D0(a1, 1, a2);
    sub_582AD0(NULL);
}

// 0x582690
void sub_582690(const char* a1, const char* a2)
{
    sub_5826D0(a1, 6, a2);
    sub_582AD0(NULL);
}

// 0x5826B0
void sub_5826B0()
{
}

// 0x5826C0
void sub_5826C0()
{
    // TODO: Incomplete.
}

// 0x5826D0
void sub_5826D0()
{
    // TODO: Incomplete.
}

// 0x582790
void sub_582790(TigRect* rect)
{
    TigArtBlitSpec blt;
    TigRect screen_rect;

    tig_art_interface_id_create(754, 0, 0, 0, &(blt.art_id));
    blt.flags = 0;

    if (rect != NULL) {
        blt.src_rect = rect;
        blt.dst_rect = rect;
    } else {
        screen_rect.x = 0;
        screen_rect.y = 0;
        screen_rect.width = 800;
        screen_rect.height = 600;
        blt.src_rect = &screen_rect;
        blt.dst_rect = &screen_rect;
    }

    tig_window_blit_art(sub_549820(), &blt);

    if (rect == NULL
        || (rect->x < stru_5CC008.x + stru_5CC008.width
            && rect->y < stru_5CC008.y + stru_5CC008.height
            && stru_5CC008.x < rect->x + rect->width
            && stru_5CC008.y < rect->y + rect->height)) {
        sub_549960();
    }
}

// 0x582860
void sub_582860()
{
    // TODO: Incomplete.
}

// 0x5829D0
void sub_5829D0(TigRect* rect)
{
    TigRect text_rect;
    TigFont font;
    MesFileEntry mes_file_entry;

    if (rect == NULL
        || rect->x < stru_5CC018.x + stru_5CC018.width
        && rect->y < stru_5CC018.y + stru_5CC018.height
        && stru_5CC018.x < rect->x + rect->width
        && stru_5CC018.y < rect->y + rect->height) {
        sub_582790(&stru_5CC018);
        tig_font_push(sub_549940(0, 2));

        mes_file_entry.num = 2400;
        mes_get_msg(sub_549840(), &mes_file_entry);

        font.str = mes_file_entry.str;
        font.width = 0;
        sub_535390(&font);

        text_rect.x = (166 - font.width) / 2 + 77;
        text_rect.y = 562;
        text_rect.width = font.width;
        text_rect.height = font.height;
        tig_window_text_write(sub_549820(), str, text_rect);

        tig_font_pop();
    }
}

// 0x582AD0
void sub_582AD0()
{
    // TODO: Incomplete.
}

// 0x582D10
bool sub_582D10(const char* str)
{
    return str == NULL;
}

// 0x582D20
bool sub_582D20(const char* a, const char* b)
{
    return strcmpi(a, b) == 0;
}

// 0x582D40
const char* sub_582D40(const char* str)
{
    return str;
}

// 0x582D50
const char* sub_582D50(const char* str)
{
    return str;
}

// 0x582D60
void sub_582D60(TigRect* rect)
{
    TigRect text_rect;
    TigFont font;
    char str[80];

    if (rect == NULL
        || rect->x < stru_5CBFF8.x + stru_5CBFF8.width
        && rect->y < stru_5CBFF8.y + stru_5CBFF8.height
        && stru_5CBFF8.x < rect->x + rect->width
        && stru_5CBFF8.y < rect->y + rect->height) {
        sub_582790(&stru_5CBFF8);
        tig_font_push(sub_549940(0, 3));

        text_rect.x = 354;
        text_rect.y = 536;

        snprintf(str, sizeof(str), "%s|", byte_6861F8);
        font.str = str;
        font.width = 0;
        sub_535390(&font);

        text_rect.width = font.width;
        text_rect.height = font.height;
        tig_window_text_write(sub_549820(), str, text_rect);

        tig_font_pop();
    }
}

// 0x582E50
void sub_582E50(TigRect* rect)
{
    sub_582790(rect);
    sub_582860(rect);
    sub_5829D0(rect);
    sub_582AD0(rect);
    sub_582D60(rect);
}

// 0x582E80
void sub_582E80()
{
    // TODO: Incomplete.
}

// 0x5830F0
void sub_5830F0()
{
    if (dword_6862DC != NULL) {
        sub_4A5040(dword_6862DC);
        dword_6862DC = NULL;
    }

    if (sub_4A5020(&dword_6862DC, &dword_6862E0)) {
        if (sub_5496D0() == 23) {
            sub_582860(NULL);
        }
    }
}

// 0x583140
void sub_583140()
{
    // TODO: Incomplete.
}

// 0x583200
void sub_583200()
{
    // TODO: Incomplete.
}

// 0x583510
void sub_583510()
{
    scrollbar_ui_control_destroy(stru_686520);
    mes_unload(dword_686528);
}

// 0x583540
void sub_583540(TigRect* rect)
{
    sub_5837A0(rect);
    sub_5482A0(rect);
    sub_583830(rect);
}

// 0x583560
bool mainmenu_ui_execute_multiplayer_hub(int button)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    switch (button) {
    case 0:
        sub_5417A0(1);
        return false;
    case 1:
        sub_5836A0();
        sub_5417A0(false);
        sub_585D50();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        return false;
    case 2:
        sub_5836A0();
        sub_5417A0(false);
        sub_584AE0();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        return false;
    case 5:
        if (mes_load("mes\\urls.mes", &mes_file)) {
            mes_file_entry.num = 1;
            mes_get_msg(mes_file, &mes_file_entry);
            sub_595A60(mes_file_entry.str);
            mes_unload(mes_file);
        }
        return false;
    case 6:
        if (mes_load("mes\\urls.mes", &mes_file)) {
            mes_file_entry.num = 2;
            mes_get_msg(mes_file, &mes_file_entry);
            sub_595A60(mes_file_entry.str);
            mes_unload(mes_file);
        }
        return false;
    }

    tig_debug_printf("MainMenuUI: mainmenu_ui_execute_multiplayer_hub unknown button Idx %d.\n", button);
    return true;
}

// 0x5836A0
void sub_5836A0()
{
    // TODO: Incomplete.
}

// 0x5837A0
void sub_5837A0(TigRect* rect)
{
    TigArtBlitSpec blt;
    TigRect screen_rect;

    tig_art_interface_id_create(759, 0, 0, 0, &(blt.art_id));
    blt.flags = 0;

    if (rect != NULL) {
        blt.src_rect = rect;
        blt.dst_rect = rect;
    } else {
        screen_rect.x = 0;
        screen_rect.y = 0;
        screen_rect.width = 800;
        screen_rect.height = 600;
        blt.src_rect = &screen_rect;
        blt.dst_rect = &screen_rect;
    }

    tig_window_blit_art(sub_549820(), &blt);
    sub_549960();
}

// 0x583830
void sub_583830()
{
    // TODO: Incomplete.
}

// 0x583A00
void sub_583A00()
{
    won_account[0] = '\0';
    won_password[0] = '\0';
    byte_68663C[0] = '\0';
    byte_6866BC[0] = '\0';

    if (!mes_load("mes\\Multiplayer.mes", &dword_686538)) {
        tig_debug_printf("MultiplayerHUB: could not '%s', aborting create.\n", "mes\\Multiplayer.mes");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    stru_686530.num = 3000;
    mes_get_msg(dword_686538, &stru_686530);

    sub_549830(19);
    sub_546330();
}

// 0x583A80
void sub_583A80()
{
    mes_unload(dword_686538);
}

// 0x583A90
void sub_583A90()
{
    dword_68673C = 0;
    won_password[0] = '\0';

    if (!mes_load("mes\\Multiplayer.mes", &dword_686538)) {
        tig_debug_printf("MultiplayerHUB: could not '%s', aborting create.\n", "mes\\Multiplayer.mes");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    stru_686530.num = 3000;
    mes_get_msg(dword_686538, &stru_686530);

    sub_549830(20);
    sub_546330();
    sub_549990(dword_5994A0, 2);
}

// 0x583B10
void sub_583B10()
{
    mes_unload(dword_686538);
}

// 0x583B20
void sub_583B20(int x, int y)
{
    MainMenuWindowInfo* window_info;

    window_info = sub_5496C0(sub_5496D0());
    x += window_info->field_5C.x;
    y += window_info->field_5C.y;

    if (x >= stru_5CC470.x
        && y >= stru_5CC470.y
        && x < stru_5CC470.x + stru_5CC470.width
        && y < stru_5CC470.y + stru_5CC470.height) {
        dword_68673C = 0;
        sub_5493C0(won_account, 128);
    } else if (x >= stru_5CC480.x
        && y >= stru_5CC480.y
        && x < stru_5CC480.x + stru_5CC480.width
        && y < stru_5CC480.y + stru_5CC480.height) {
        dword_68673C = 0;
        sub_5493C0(won_password, 128);
    } else if (x >= stru_5CC490.x
        && y >= stru_5CC490.y
        && x < stru_5CC490.x + stru_5CC490.width
        && y < stru_5CC490.y + stru_5CC490.height) {
        dword_68673C = 0;
        sub_5493C0(byte_68663C, 128);
    } else if (x >= stru_5CC4A0.x
        && y >= stru_5CC4A0.y
        && x < stru_5CC4A0.x + stru_5CC4A0.width
        && y < stru_5CC4A0.y + stru_5CC4A0.height) {
        dword_68673C = 0;
        sub_5493C0(byte_6866BC, 128);
    }
}

// 0x583C80
void sub_583C80(int x, int y)
{
    MainMenuWindowInfo* window_info;

    window_info = sub_5496C0(sub_5496D0());
    x += window_info->field_5C.x;
    y += window_info->field_5C.y;

    if (x >= stru_5CC470.x
        && y >= stru_5CC470.y
        && x < stru_5CC470.x + stru_5CC470.width
        && y < stru_5CC470.y + stru_5CC470.height) {
        dword_68673C = 0;
        sub_5493C0(won_account, 128);
    } else if (x >= stru_5CC480.x
        && y >= stru_5CC480.y
        && x < stru_5CC480.x + stru_5CC480.width
        && y < stru_5CC480.y + stru_5CC480.height) {
        dword_68673C = 0;
        sub_5493C0(won_password, 128);
    } else if (x >= stru_5CC4B0.x
        && y >= stru_5CC4B0.y
        && x < stru_5CC4B0.x + stru_5CC4B0.width
        && y < stru_5CC4B0.y + stru_5CC4B0.height) {
        sub_5417A0(false);
        sub_583A00();
    }
}

// 0x583D90
void sub_583D90()
{
    // TODO: Incomplete.
}

// 0x584150
void sub_584150()
{
    // TODO: Incomplete.
}

// 0x5845E0
void sub_5845E0()
{
    // TODO: Incomplete.
}

// 0x5847D0
void sub_5847D0()
{
    // TODO: Incomplete.
}

// 0x584A40
const char* sub_584A40(int value)
{
    MesFileEntry mes_file_entry;

    if (value == 0) {
        return NULL;
    }

    mes_file_entry.num = value + 2010;
    mes_get_msg(sub_549840(), &mes_file_entry);
    return mes_file_entry.str;
}

// 0x584A80
const char* sub_584A80()
{
    return won_account;
}

// 0x584A90
const char* sub_584A90()
{
    return won_password;
}

// 0x584AA0
void sub_584AA0(const char* str)
{
    strncpy(won_account, str, sizeof(won_account));
}

// 0x584AC0
void sub_584AC0(const char* str)
{
    strncpy(won_password, str, sizeof(won_password));
}

// 0x584AE0
void sub_584AE0()
{
    MesFileEntry mes_file_entry;
    char name[80];

    dword_686960 = false;
    memset(byte_68674C, 0, sizeof(byte_68674C));
    sub_549830(18);

    if (!mes_load("mes\\multiplayer.mes", &dword_5CC5F0)) {
        tig_debug_printf("ServerList_UI: init: ERROR could not load '%s'. Aborting init.\n", "mes\\multiplayer.mes");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    stru_6867A0.num = 3000;
    mes_get_msg(dword_5CC5F0, &stru_6867A0);

    mes_file_entry.num = 2333;
    mes_get_msg(sub_549840(), &mes_file_entry);
    strncpy(byte_6868F8, mes_file_entry.str, sizeof(byte_6868F8));

    tig_net_local_server_clear_password();
    gamelib_modlist_create(&stru_686740, 1);
    dword_686964 = 0;

    tig_net_local_server_get_name(name, sizeof(name));
    mainmenu_ui_create_window_func(false);
    if (sub_4A4ED0()) {
        tig_button_show(stru_5CC6C8[2].field_C);
    } else {
        tig_button_hide(stru_5CC6C8[2].field_C);
    }
    tig_net_local_server_set_name(name);
    sub_549990(dword_5994AC, 3);
    sub_584CB0(NULL);
}

// 0x584C00
void sub_584C00()
{
    mes_unload(dword_5CC5F0);
    gamelib_modlist_destroy(&stru_686740);
    stru_686740.names = NULL;
    dword_686964 = 0;
}

// 0x584C30
void sub_584C30(TigRect* rect)
{
    TigArtBlitSpec blt;
    TigRect screen_rect;

    tig_art_interface_id_create(798, 0, 0, 0, &(blt.art_id));
    blt.flags = 0;

    if (rect != NULL) {
        blt.src_rect = rect;
        blt.dst_rect = rect;
    } else {
        screen_rect.x = 0;
        screen_rect.y = 0;
        screen_rect.width = 800;
        screen_rect.height = 600;
        blt.src_rect = &screen_rect;
        blt.dst_rect = &screen_rect;
    }

    tig_window_blit_art(sub_549820(), &blt);
}

// 0x584CB0
void sub_584CB0()
{
    // TODO: Incomplete.
}

// 0x5850C0
void sub_5850C0()
{
    // TODO: Incomplete.
}

// 0x585270
void sub_585270()
{
    // TODO: Incomplete.
}

// 0x585630
void sub_585630()
{
    // TODO: Incomplete.
}

// 0x585970
void sub_585970(char* buffer)
{
    if (!dword_686960) {
        if (byte_68674C[0] != '\0') {
            strcpy(byte_6867A8, buffer);
        }

        switch (dword_5CC6AC) {
        case 2303:
        case 2306:
        case 2305:
            stru_5CC6B0.flags = 0;
            break;
        case 2309:
            stru_5CC6B0.flags = TEXTEDIT_0x01 | TEXTEDIT_0x02;
            break;
        }

        stru_5CC6B0.size = 23;
        stru_5CC6B0.buffer = buffer;
        textedit_ui_focus(&stru_5CC6B0);
        dword_686960 = true;
        sub_584CB0(0);
        sub_549A40();
    }
}

// 0x585A20
void sub_585A20()
{
    int value;
    int min_level;
    int max_level;

    if (dword_686960) {
        if (stru_5CC6B0.buffer[0] == '\0') {
            strcpy(stru_5CC6B0.buffer, byte_6867A8);
        }

        dword_686960 = false;
        textedit_ui_unfocus(&stru_5CC6B0);

        switch (dword_5CC6AC) {
        case 2300:
            tig_net_local_server_set_name(byte_686948);
            break;
        case 2303:
            value = atoi(byte_686948);
            tig_net_local_server_set_max_players(value);
            break;
        case 2305:
            value = atoi(byte_686948);
            tig_net_local_server_get_level_range(NULL, max_level);
            if (value >= 0) {
                tig_net_local_server_set_level_range(value, max_level);
            }
            break;
        case 2306:
            value = atoi(byte_686948);
            tig_net_local_server_get_level_range(&min_level, NULL);
            if (value > 0) {
                tig_net_local_server_set_level_range(min_level, value);
            }
            break;
        case 2307:
            tig_net_local_server_set_password(byte_686948);
            break;
        case 2309:
            value = atoi(byte_686948);
            if (value >= tig_net_local_server_get_max_players()) {
                sub_4A5600(value);
            }
            break;
        }

        dword_5CC6AC = -1;
        sub_584CB0(NULL);
        sub_549A50();
    }
}

// 0x585BA0
void sub_585BA0()
{
    sub_585A20();
}

// 0x585BB0
void sub_585BB0()
{
    sub_584CB0(NULL);
}
