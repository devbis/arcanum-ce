#include "ui/server_list_ui.h"

#include <stdio.h>

#include "game/gamelib.h"
#include "game/mes.h"
#include "game/multiplayer.h"
#include "ui/intgame.h"
#include "ui/mainmenu_ui.h"
#include "ui/multiplayer_hub_ui.h"
#include "ui/scrollbar_ui.h"

static void sub_585E40();
static void sub_585F00(TigNetServer* server);
static void sub_585FF0(TigNetServer* servers, int cnt);
static void sub_586AB0(TigRect* rect);
static void sub_586AD0(TigRect* rect, tig_window_handle_t window_handle);
static void sub_586D20(TigRect* rect, tig_window_handle_t window_handle);
static void sub_586ED0(TigRect* rect, tig_window_handle_t window_handle);
static void sub_587570(TigRect* rect, tig_window_handle_t window_handle);
static void sub_587960(TigRect* rect, tig_window_handle_t window_handle);
static void sub_587D60(TigRect* rect, tig_window_handle_t window_handle);
static void sub_588180(TigRect* rect, tig_window_handle_t window_handle);
static void sub_5883E0(tig_button_handle_t button_handle, tig_window_handle_t window_handle);
static void sub_588650();
static void sub_5887F0();
static void sub_588990();
static void sub_588B10(TigNetServer* server);
static int sub_588B70(const void* va, const void* vb);
static TigNetServer* sub_588C80(TigNetServer* server, TigNetServer* servers, int cnt);
static void sub_588CC0(TigNetServer* server, bool a2);
static TigNetServer* sub_588D00();
static void sub_588D70(int value);
static void sub_588D90(const char* str);
static const char* sub_588E10(int seconds);
static void sub_588EC0(TigNetServer* servers, int count);
static void sub_588EF0();
static void sub_588FF0(const char* path, int progress, tig_window_handle_t window_handle);

// 0x5994BC
static int dword_5994BC[3] = {
    2,
    16,
    21,
};

// 0x5CCAF8
MainMenuButtonInfo stru_5CCAF8[7] = {
    { 162, 32, -1, TIG_BUTTON_HANDLE_INVALID, 0x15, 0, 0, { 0 }, -1 },
    { 162, 72, -1, TIG_BUTTON_HANDLE_INVALID, 0x17, 0, 0, { 0 }, -1 },
    { 162, 112, -1, TIG_BUTTON_HANDLE_INVALID, 0x12, 0, 0, { 0 }, -1 },
    { 58, 533, 357, TIG_BUTTON_HANDLE_INVALID, 0x11, -1, 0x0C, { 0 }, -1 },
    { 72, 491, 751, TIG_BUTTON_HANDLE_INVALID, 0x11, -1, 0x0C, { 0 }, -1 },
    { 232, 492, 752, TIG_BUTTON_HANDLE_INVALID, 0x11, -1, 0x0C, { 0 }, -1 },
    { 764, 101, 762, TIG_BUTTON_HANDLE_INVALID, 0x11, -1, 0x0C, { 0 }, -1 },
};

// 0x5CCC48
static TigRect stru_5CCC48 = { 342, 131, 409, 420 };

// 0x5CCC58
static TigRect stru_5CCC58 = { 351, 566, 744, 581 };

// 0x5CCC68
static TigRect stru_5CCC68 = { 348, 104, 745, 15 };

// 0x5CCC78
static TigRect stru_5CCC78 = { 92, 496, 138, 15 };

// 0x5CCC88
static TigRect stru_5CCC88 = { 30, 550, 263, 15 };

// 0x5CCC98
static TigRect stru_5CCC98 = { 48, 185, 284, 476 };

// 0x5CCCA8
static TigRect stru_5CCCA8 = { 348, 104, 745, 119 };

// 0x5CCCB8
static TigRect stru_5CCCB8 = { 351, 566, 744, 581 };

// 0x5CCCC8
static TigRect stru_5CCCC8 = { 323, 17, 468, 60 };

// 0x5CCAD0
static mes_file_handle_t serverlist_ui_mainmenu_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5CCAD4
static mes_file_handle_t serverlist_ui_multiplayer_mes_file = MES_FILE_HANDLE_INVALID;

// 0x5CCAD8
static int dword_5CCAD8 = -1;

// 0x5CCADC
static int dword_5CCADC[5] = {
    5,
    0,
    0,
    0,
    0,
};

// 0x5CCAF0
static tig_button_handle_t dword_5CCAF0;

// 0x686968
static MesFileEntry stru_686968;

// 0x68698C
static tig_timestamp_t dword_68698C;

// 0x686990
static ScrollbarId stru_686990;

// 0x686998
static bool serverlist_ui_initialized;

// 0x68699C
static bool dword_68699C;

// 0x6869A0
static TigNetServer* dword_6869A0;

// 0x6869A4
static int dword_6869A4;

// 0x6869A8
static int dword_6869A8;

// 0x6869AC
static unsigned long dword_6869AC;

// 0x6869B0
static char byte_6869B0[80];

// 0x686A00
static char byte_686A00[80];

// 0x686A50
static char byte_686A50[2048];

// 0x687250
static unsigned int dword_687250;

// 0x687254
static bool dword_687254;

// 0x687258
static int dword_687258;

// 0x68725C
static int dword_68725C;

// 0x585BC0
bool serverlist_ui_init()
{
    dword_687254 = 0;
    dword_6869A0 = 0;
    dword_6869A4 = 0;
    dword_6869A8 = 0;
    sub_588CC0(0, 0);
    dword_5CCADC[0] = 5;
    dword_5CCADC[1] = 5;
    dword_5CCADC[2] = 5;
    dword_5CCADC[3] = 5;
    dword_5CCADC[4] = 5;

    if (!mes_load("mes\\mainmenu.mes", &serverlist_ui_mainmenu_mes_file)) {
        tig_debug_printf("ServerList_UI: init: ERROR could not load '%s'. Aborting init.\n", "mes\\mainmenu.mes");
        return false;
    }

    if (!mes_load("mes\\multiplayer.mes", &serverlist_ui_multiplayer_mes_file)) {
        tig_debug_printf("ServerList_UI: init: ERROR could not load '%s'. Aborting init.\n", "mes\\multiplayer.mes");
        return false;
    }

    dword_6869AC = 0;
    byte_6869B0[0] = '\0';
    byte_686A50[0] = '\0';
    byte_686A00[0] = '\0';
    tig_net_local_server_clear_password();
    dword_687258 = 0;
    dword_68725C = 0;

    stru_686968.num = 3000;
    mes_get_msg(serverlist_ui_multiplayer_mes_file, &stru_686968);

    serverlist_ui_initialized = true;

    return true;
}

// 0x585CB0
void serverlist_ui_exit()
{
    mes_unload(serverlist_ui_mainmenu_mes_file);
    mes_unload(serverlist_ui_multiplayer_mes_file);
    if (dword_6869A0 != NULL) {
        FREE(dword_6869A0);
    }
    serverlist_ui_initialized = false;
}

// 0x585D50
void sub_585D50()
{
    TigNetServer* servers = NULL;
    int servers_cnt = 0;

    dword_687258 = 0;
    dword_687254 = 0;
    sub_549830(17);
    mainmenu_ui_create_window_func(false);

    if (multiplayer_mm_is_active()) {
        tig_button_show(stru_5CCAF8[1].button_handle);
        tig_net_on_server_bcast(sub_585F00);
        sub_588990();
    } else {
        tig_button_hide(stru_5CCAF8[1].button_handle);
        tig_button_hide(stru_5CCAF8[6].button_handle);
        tig_net_on_servers_change(sub_588EC0);
        tig_net_get_servers(&servers, &servers_cnt);
        sub_585FF0(servers, servers_cnt);
    }

    sub_585E40();
    sub_4A3890();
    sub_4A2B00(sub_588EF0);
    sub_549990(dword_5994BC, 3);
    sub_5860D0(0);
}

// 0x585E20
void sub_585E20()
{
    tig_net_on_servers_change(NULL);
    sub_4A2B00(NULL);
    sub_54B3C0();
    scrollbar_ui_control_destroy(stru_686990);
    dword_68699C = false;
}

// 0x585E40
void sub_585E40()
{
    ScrollbarUiControlInfo sb_create_info;

    sb_create_info.flags = 927;
    sb_create_info.scrollbar_rect.x = 771;
    sb_create_info.scrollbar_rect.y = 135;
    sb_create_info.scrollbar_rect.width = 13;
    sb_create_info.scrollbar_rect.height = 420;
    sb_create_info.content_rect.x = 0;
    sb_create_info.content_rect.y = 0;
    sb_create_info.content_rect.width = 800;
    sb_create_info.content_rect.height = 600;
    sb_create_info.field_24 = 0;
    sb_create_info.field_28 = 0;
    sb_create_info.field_2C = 1;
    sb_create_info.field_3C = sub_588D70;
    sb_create_info.field_38 = 0;
    sb_create_info.field_40 = sub_586AB0;
    scrollbar_ui_control_create(&stru_686990, &sb_create_info, sub_549820());
    sub_5806F0(stru_686990);

    dword_68699C = true;
}

// 0x585F00
void sub_585F00(TigNetServer* server)
{
    TigNetServer mut_server;
    int index;

    if (dword_6869A0 != NULL && server != NULL) {
        if (sub_588D00() != NULL) {
            mut_server = *sub_588D00();
        }

        for (index = 0; index < dword_6869A4; index++) {
            if (dword_6869A0[index].addr.sin_addr.s_addr == server->addr.sin_addr.s_addr) {
                break;
            }
        }

        if (index >= dword_6869A4) {
            return;
        }

        dword_6869A0[index] = *server;

        if (sub_588D00() != NULL) {
            sub_588B10(&mut_server);
        } else {
            sub_588B10(NULL);
        }

        if (dword_6869A4 > 21) {
            sub_5810D0(stru_686990, 1, dword_6869A4 - 21);
        } else {
            sub_5810D0(stru_686990, 1, 0);
        }

        sub_5860D0(NULL);
    }
}

// 0x585FF0
void sub_585FF0(TigNetServer* servers, int cnt)
{
    TigNetServer server;

    if (sub_588D00() != NULL) {
        server = *sub_588D00();
    }

    if (dword_6869A0 != NULL) {
        FREE(dword_6869A0);
    }

    dword_6869A0 = (TigNetServer*)CALLOC(cnt, sizeof(*dword_6869A0));
    memcpy(dword_6869A0, servers, sizeof(*dword_6869A0) * cnt);
    dword_6869A4 = cnt;

    if (sub_588D00() != NULL) {
        sub_588B10(&server);
    } else {
        sub_588B10(NULL);
    }

    if (cnt > 21) {
        sub_5810D0(stru_686990, 1, cnt - 21);
    } else {
        sub_5810D0(stru_686990, 1, 0);
    }

    sub_5860D0(NULL);
}

// 0x5860D0
void sub_5860D0(TigRect* rect)
{
    if (dword_68699C) {
        sub_588180(rect, sub_549820());
        sub_587570(rect, sub_549820());
        sub_587960(rect, sub_549820());
        sub_587D60(rect, sub_549820());
        sub_586ED0(rect, sub_549820());
        sub_586D20(rect, sub_549820());
        sub_586AD0(rect, sub_549820());
        sub_5806F0(stru_686990);
    }
}

// 0x586150
void sub_586150(int a1, int a2, void* a3)
{
    MesFileEntry mes_file_entry;
    TigWindowModalDialogInfo modal_dialog_info;
    TigWindowModalDialogChoice choice;
    char path[TIG_MAX_PATH];
    int progress;

    if (!dword_68699C) {
        return;
    }

    mes_file_entry.num = -1;

    modal_dialog_info.type = TIG_WINDOW_MODAL_DIALOG_TYPE_OK;
    modal_dialog_info.redraw = sub_4045A0;
    modal_dialog_info.process = NULL;
    modal_dialog_info.x = 237;
    modal_dialog_info.y = 232;

    switch (a1) {
    case 8:
        if ((sub_529520() == -1 || sub_529520() == a2) && a3 != NULL) {
            // TODO: Get rid of cast.
            switch (*(int*)a3) {
            case 0:
                mes_file_entry.num = 2242; // "Could not connect to server."
                break;
            case 1:
                mes_file_entry.num = 2290; // "Your supplied password is invalid."
                break;
            case 2:
                mes_file_entry.num = 2291; // "You have been banned from this server, you are not allowed to join."
                break;
            case 3:
                mes_file_entry.num = 2292; // "Your protocol version does not match the servers."
                break;
            case 4:
                mes_file_entry.num = 2100; // "Server refused join.  May be full."
                break;
            case 5:
                mes_file_entry.num = 2293; // "You do not meet the necessary critera to join the game or a duplicate player exists."
                break;
            }

            if (mes_file_entry.num != -1) {
                tig_net_reset_connection();
                mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

                modal_dialog_info.text = mes_file_entry.str;
                tig_window_modal_dialog(&modal_dialog_info, &choice);
            }
        }

        tig_button_show(dword_5CCAF0);
        dword_5CCAF0 = TIG_BUTTON_HANDLE_INVALID;
        break;
    case 14:
        if (a3 != NULL) {
            // TODO: Get rid of cast.
            tig_net_xfer_status(*(int*)a3, path, &progress);
            sub_588FF0(path, progress, sub_549820());
        }
        break;
    }
}

// 0x5862D0
bool sub_5862D0(TigMessage* msg, tig_window_handle_t window_handle)
{
    char ch[2];

    switch (msg->type) {
    case TIG_MESSAGE_MOUSE:
        switch (msg->data.mouse.event) {
        case TIG_MESSAGE_MOUSE_LEFT_BUTTON_DOWN:
            if (msg->data.mouse.x >= stru_5CCCC8.x
                && msg->data.mouse.y >= stru_5CCCC8.y
                && msg->data.mouse.x < stru_5CCCC8.x + stru_5CCCC8.width
                && msg->data.mouse.y < stru_5CCCC8.y + stru_5CCCC8.height) {
                if (multiplayer_mm_is_active()) {
                    multiplayer_mm_ad_clicked();
                    return true;
                }
                return false;
            }
            if (msg->data.mouse.x >= stru_5CCC88.x
                && msg->data.mouse.y >= stru_5CCC88.y
                && msg->data.mouse.x < stru_5CCC88.x + stru_5CCC88.width
                && msg->data.mouse.y < stru_5CCC88.y + stru_5CCC88.height) {
                byte_686A00[0] = '\0';
                dword_687258 = false;
                sub_5493C0(byte_686A00, 80);
                return false;
            }
            if (msg->data.mouse.x >= stru_5CCC48.x
                && msg->data.mouse.y >= stru_5CCC48.y
                && msg->data.mouse.x < stru_5CCC48.x + stru_5CCC48.width
                && msg->data.mouse.y < stru_5CCC48.y + stru_5CCC48.height) {
                int index;

                index = dword_6869A8 + (msg->data.mouse.y - 131) / 20;
                if (index < dword_6869A4) {
                    sub_588CC0(&dword_6869A0[index], 1);
                    sub_5860D0(0);
                    dword_687258 = 0;
                    return true;
                }

                sub_588CC0(0, 1);
                sub_5860D0(0);

                return false;
            }
            if (msg->data.mouse.x >= stru_5CCC58.x
                && msg->data.mouse.y >= stru_5CCC58.y
                && msg->data.mouse.x < stru_5CCC58.x + stru_5CCC58.width
                && msg->data.mouse.y < stru_5CCC58.y + stru_5CCC58.height) {
                TigNetServer* servers;
                int num_servers;

                if (msg->data.mouse.x < 401) {
                    if (dword_68725C) {
                        dword_687250 |= 7;
                    } else {
                        dword_687250 = 7;
                    }
                } else if (msg->data.mouse.x < 454) {
                    if (dword_68725C) {
                        dword_687250 ^= TIG_NET_SERVER_FILTER_FREE_FOR_ALL;
                    } else {
                        dword_687250 = TIG_NET_SERVER_FILTER_FREE_FOR_ALL;
                    }
                } else if (msg->data.mouse.x < 552) {
                    if (dword_68725C) {
                        dword_687250 ^= TIG_NET_SERVER_FILTER_COOPERATIVE;
                    } else {
                        dword_687250 = TIG_NET_SERVER_FILTER_COOPERATIVE;
                    }
                } else if (msg->data.mouse.x < 675) {
                    if (dword_68725C) {
                        dword_687250 ^= TIG_NET_SERVER_FILTER_ROLEPLAY;
                    } else {
                        dword_687250 = TIG_NET_SERVER_FILTER_ROLEPLAY;
                    }
                } else if (msg->data.mouse.x < 744) {
                    if (dword_68725C) {
                        dword_687250 ^= TIG_NET_SERVER_FILTER_BOOKMARKED;
                    } else {
                        dword_687250 = TIG_NET_SERVER_FILTER_BOOKMARKED;
                    }
                }

                if (multiplayer_mm_is_active()) {
                    MatchmakerRegisterInfo* games;
                    int index;

                    if (!multiplayer_mm_gamelist_get(&games, &num_servers)) {
                        return true;
                    }

                    // NOTE: Twice?
                    if (!sub_5499B0(stru_686968.str) || !sub_5499B0(stru_686968.str)) {
                        // FIXME: Leaks `games`.
                        return true;
                    }

                    servers = (TigNetServer*)CALLOC(sizeof(*servers), num_servers);

                    // FIXME: Meaningless.
                    memset(servers, 0, sizeof(*servers) * num_servers);

                    for (index = 0; index < num_servers; index++) {
                        strcpy(servers[index].name, games[index].name);
                        servers[index].addr = games[index].addr;
                    }

                    multiplayer_mm_gamelist_free(games);
                } else {
                    tig_net_disable_server_filter(0xFF);
                    tig_net_enable_server_filter(dword_687250);
                    tig_net_get_servers(&servers, &num_servers);
                }

                sub_585FF0(servers, num_servers);
                sub_5860D0(0);
                return true;
            }
            if (msg->data.mouse.x >= stru_5CCC68.x
                && msg->data.mouse.y >= stru_5CCC68.y
                && msg->data.mouse.x < stru_5CCC68.x + stru_5CCC68.width
                && msg->data.mouse.y < stru_5CCC68.y + stru_5CCC68.height) {
                int index;

                if (msg->data.mouse.x < 488) {
                    dword_5CCAD8 = 0;
                } else if (msg->data.mouse.x < 567) {
                    dword_5CCAD8 = 1;
                } else if (msg->data.mouse.x < 605) {
                    dword_5CCAD8 = 2;
                } else if (msg->data.mouse.x < 675) {
                    dword_5CCAD8 = 3;
                } else if (msg->data.mouse.x < 745) {
                    dword_5CCAD8 = 4;
                } else {
                    return false;
                }

                for (index = 0; index < 5; index++) {
                    dword_5CCADC[index]--;
                    if (dword_5CCADC[index] < 0) {
                        dword_5CCADC[index] = 0;
                    }
                }

                dword_5CCADC[dword_5CCAD8] = 5;

                if (sub_588D00() != NULL) {
                    TigNetServer copy;

                    copy = *sub_588D00();
                    sub_588B10(&copy);
                } else {
                    sub_588B10(NULL);
                }

                sub_587570(&stru_5CCC68, window_handle);
                sub_587D60(&stru_5CCC48, window_handle);
                sub_5806F0(stru_686990);
                return true;
            }
            if (msg->data.mouse.x >= stru_5CCC68.x
                && msg->data.mouse.y >= stru_5CCC68.y
                && msg->data.mouse.x < stru_5CCC68.x + stru_5CCC68.width
                && msg->data.mouse.y < stru_5CCC68.y + stru_5CCC68.height) {
                dword_687254 = true;
                sub_586D20(&stru_5CCC78, window_handle);
                return true;
            }
            break;
        }
        break;
    case TIG_MESSAGE_CHAR:
        if (dword_687254) {
            if (!iscntrl(msg->data.character.ch)) {
                ch[0] = msg->data.character.ch;
                ch[1] = '\0';
                strcat(byte_6869B0, ch);
                sub_586D20(&stru_5CCC78, window_handle);
                return true;
            }
        }
        break;
    case TIG_MESSAGE_KEYBOARD:
        if (msg->data.keyboard.key == DIK_LCONTROL
            || msg->data.keyboard.key == DIK_RCONTROL) {
            dword_68725C = msg->data.keyboard.pressed == 1;
        }

        if (!msg->data.keyboard.pressed && dword_687254) {
            switch (msg->data.keyboard.key) {
            case DIK_ESCAPE:
                dword_687254 = false;
                byte_6869B0[0] = '\0';
                sub_586D20(&stru_5CCC78, window_handle);
                return true;
            case DIK_BACK:
            case DIK_DELETE:
                byte_6869B0[0] = '\0';
                sub_586D20(&stru_5CCC78, window_handle);
                return true;
            case DIK_RETURN:
            case DIK_NUMPADENTER:
                dword_687254 = false;
                sub_586D20(&stru_5CCC78, window_handle);
                return true;
            }
        }
        break;
    }

    return false;
}

// 0x586AB0
void sub_586AB0(TigRect* rect)
{
    if (dword_68699C) {
        sub_588180(rect, sub_549820());
    }
}

// 0x586AD0
void sub_586AD0(TigRect* rect, tig_window_handle_t window_handle)
{
    TigRect text_rect;
    TigFont font_desc;
    MesFileEntry mes_file_entry;
    char* str;

    // FIXME: Initialize to silence compiler warning.
    str = NULL;

    if (!dword_68699C) {
        return;
    }

    if (rect == NULL
        || (rect->x < stru_5CCC88.x + stru_5CCC88.width
            && rect->y < stru_5CCC88.y + stru_5CCC88.height
            && stru_5CCC88.x < rect->x + rect->width
            && stru_5CCC88.y < rect->y + rect->height)) {
        sub_588180(&stru_5CCC88, window_handle);

        if (sub_588D00() != NULL
            && (sub_588D00()->options & TIG_NET_SERVER_PASSWORD_PROTECTED) != 0) {
            char v1[2];
            v1[0] = '\x7F';
            v1[1] = '\0';

            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));
            font_desc.width = 0;
            font_desc.str = v1;
            tig_font_measure(&font_desc);
            text_rect.x = 30;
            text_rect.y = 550;
            text_rect.width = font_desc.width;
            text_rect.height = font_desc.height;
            tig_window_text_write(window_handle, v1, &text_rect);
            tig_font_pop();
        }

        switch (dword_687258) {
        case 1:
            mes_file_entry.num = 2208;
            break;
        case 2:
            mes_file_entry.num = 2209;
            break;
        case 3:
            mes_file_entry.num = 2250;
            break;
        default:
            if (sub_588D00() == NULL) {
                return;
            }
            if ((sub_588D00()->options & TIG_NET_SERVER_PASSWORD_PROTECTED) != 0) {
                if (sub_549520() == byte_686A00 || byte_686A00[0] != '\0') {
                    mes_file_entry.num = -1;
                    str = (char*)MALLOC(strlen(byte_686A00) + 2);
                    sprintf(str, "%s%c", byte_686A00, '|'); // TODO: Unclear.
                } else {
                    mes_file_entry.num = 2207;
                }
            } else {
                mes_file_entry.num = 2205;
            }
            break;
        }

        if (mes_file_entry.num != -1) {
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
            str = STRDUP(mes_file_entry.str);
        }

        if (str != NULL) {
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            font_desc.width = 0;
            font_desc.str = str;
            tig_font_measure(&font_desc);
            text_rect.x = (168 - font_desc.width) / 2 + 125;
            text_rect.y = 550;
            text_rect.width = font_desc.width;
            text_rect.height = 15;
            tig_window_text_write(window_handle, str, &text_rect);
            tig_font_pop();

            FREE(str);
        }
    }
}

// 0x586D20
void sub_586D20(TigRect* rect, tig_window_handle_t window_handle)
{
    tig_font_handle_t font;
    char* str;
    TigFont font_desc;
    TigRect text_rect;

    if (!dword_68699C) {
        return;
    }

    if (rect == NULL
        || (rect->x < stru_5CCC78.x + stru_5CCC78.width
            && rect->y < stru_5CCC78.y + stru_5CCC78.height
            && stru_5CCC78.x < rect->x + rect->width
            && stru_5CCC78.y < rect->y + rect->height)) {
        if (sub_588D00() != NULL && !dword_687254 && byte_6869B0[0] == '\0') {
            tig_net_inet_string(sub_588D00()->addr.sin_addr.s_addr, byte_6869B0);
        }

        sub_588180(&stru_5CCC78, window_handle);

        if (dword_687254) {
            font = mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_GOLD);
        } else {
            font = mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE);
        }

        tig_font_push(font);
        str = (char*)MALLOC(80);
        if (dword_687254) {
            snprintf(str, 80, "%s|", byte_6869B0);
        } else {
            strncpy(str, byte_6869B0, 80);
        }

        font_desc.width = 0;
        font_desc.str = str;
        tig_font_measure(&font_desc);

        while (font_desc.width >= 138) {
            memcpy(str, str + 1, 79);
            str[79] = '\0';
            font_desc.width = 0;
            font_desc.str = str;
            tig_font_measure(&font_desc);
        }

        text_rect.x = (138 - font_desc.width) / 2 + 92;
        text_rect.y = 396;
        text_rect.width = 138;
        text_rect.height = 15;
        tig_window_text_write(window_handle, str, &text_rect);
        FREE(str);
        tig_font_pop();
    }
}

// 0x586ED0
void sub_586ED0(TigRect* rect, tig_window_handle_t window_handle)
{
    TigRect text_rect;
    MesFileEntry mes_file_entry1;
    MesFileEntry on;
    MesFileEntry mes_file_entry3;
    MesFileEntry off;
    TigFont font_desc;
    char str[128];
    size_t pos;
    char* copy;
    int player;

    if (!dword_68699C) {
        return;
    }

    if (rect == NULL
        || (rect->x < stru_5CCC98.x + stru_5CCC98.width
            && rect->y < stru_5CCC98.y + stru_5CCC98.height
            && stru_5CCC98.x < rect->x + rect->width
            && stru_5CCC98.y < rect->y + rect->height)) {
        text_rect.x = 48;
        text_rect.y = 185;
        text_rect.width = 236;
        text_rect.height = 110;
        tig_window_fill(window_handle,
            &text_rect,
            tig_color_make(0, 0, 0));

        if (byte_686A50[0] != '\0') {
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));
            tig_window_text_write(window_handle, byte_686A50, &text_rect);
            tig_font_pop();
        } else if (sub_588D00() != NULL) {
            on.num = 2238; // "ON"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &on);

            off.num = 2239; // "OFF"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &off);

            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));

            copy = STRDUP(sub_588D00()->name);
            pos = strlen(copy);
            do {
                copy[pos] = '\0';
                font_desc.width = 0;
                font_desc.str = copy;
                tig_font_measure(&font_desc);
                pos--;
            } while (font_desc.width > 236);

            text_rect.x = 48;
            text_rect.y = 185;
            text_rect.width = 236;
            text_rect.height = 16;
            tig_window_text_write(window_handle, copy, &text_rect);
            FREE(copy);

            mes_file_entry1.num = 2230; // "Module: %s"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry1);

            copy = (char*)MALLOC(strlen(sub_588D00()->description) + strlen(mes_file_entry1.str));
            sprintf(copy, mes_file_entry1.str, sub_588D00()->description);

            pos = strlen(copy);
            do {
                copy[pos] = '\0';
                font_desc.width = 0;
                font_desc.str = copy;
                tig_font_measure(&font_desc);
                pos--;
            } while (font_desc.width > 236);

            text_rect.x = 48;
            text_rect.y = 201;
            text_rect.width = 236;
            text_rect.height = 16;
            tig_window_text_write(window_handle, copy, &text_rect);
            FREE(copy);

            mes_file_entry1.num = 2237; // "Type: %s"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry1);

            switch (sub_588D00()->type) {
            case TIG_NET_SERVER_TYPE_FREE_FOR_ALL:
                mes_file_entry3.num = 2221; // "FFA"
                break;
            case TIG_NET_SERVER_TYPE_COOPERATIVE:
                mes_file_entry3.num = 2222; // "Cooperative"
                break;
            case TIG_NET_SERVER_TYPE_ROLEPLAY:
                mes_file_entry3.num = 2223; // "Roleplay"
                break;
            }

            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry3);
            sprintf(str, mes_file_entry1.str, mes_file_entry3.str);
            text_rect.x = 48;
            text_rect.y = 217;
            text_rect.width = 236;
            text_rect.height = 16;
            tig_window_text_write(window_handle, str, &text_rect);

            mes_file_entry1.num = 2231; // "Level: %d-%d"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry1);
            sprintf(str, mes_file_entry1.str, sub_588D00()->min_level, sub_588D00()->max_level);
            text_rect.x = 48;
            text_rect.y = 233;
            text_rect.width = 236;
            text_rect.height = 16;
            tig_window_text_write(window_handle, str, &text_rect);

            mes_file_entry1.num = 2232; // "Player Killing: %s"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry1);
            if ((sub_588D00()->options & TIG_NET_SERVER_PLAYER_KILLING) != 0) {
                sprintf(str, mes_file_entry1.str, on.str);
            } else {
                sprintf(str, mes_file_entry1.str, off.str);
            }
            text_rect.x = 48;
            text_rect.y = 249;
            text_rect.width = 236;
            text_rect.height = 16;
            tig_window_text_write(window_handle, str, &text_rect);

            mes_file_entry1.num = 2233; // "Friendly Fire Damage: %s"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry1);
            if ((sub_588D00()->options & TIG_NET_SERVER_FRIENDLY_FIRE) != 0) {
                sprintf(str, mes_file_entry1.str, on.str);
            } else {
                sprintf(str, mes_file_entry1.str, off.str);
            }
            text_rect.x = 48;
            text_rect.y = 265;
            text_rect.width = 236;
            text_rect.height = 16;
            tig_window_text_write(window_handle, str, &text_rect);

            mes_file_entry1.num = 2234; // "Private Chat: %s"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry1);
            if ((sub_588D00()->options & TIG_NET_SERVER_PRIVATE_CHAT) != 0) {
                sprintf(str, mes_file_entry1.str, on.str);
            } else {
                sprintf(str, mes_file_entry1.str, off.str);
            }
            text_rect.x = 48;
            text_rect.y = 281;
            text_rect.width = 236;
            text_rect.height = 16;
            tig_window_text_write(window_handle, str, &text_rect);

            text_rect.x = 48;
            text_rect.y = 297;
            text_rect.width = 230;
            text_rect.height = 179;
            tig_window_fill(window_handle, &text_rect, tig_color_make(0, 0, 0));

            mes_file_entry1.num = 2235; // "Players: %d(%d)"
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry1);
            sprintf(str, mes_file_entry1.str, sub_588D00()->num_players, sub_588D00()->max_players);
            tig_window_text_write(window_handle, str, &text_rect);

            // TODO: Incomplete (but probably unusable piece of code).
            for (player = 0; player < 8; player++) {
                if (sub_588D00()->players[player].name[0] != '\0') {
                    sprintf(str,
                        "%s (%d)",
                        sub_588D00()->players[player].name,
                        sub_588D00()->players[player].field_18);

                    text_rect.x = 58;
                    text_rect.y = 16 * player + 318;
                    text_rect.width = 220;
                    text_rect.height = 158;
                    tig_window_text_write(window_handle, str, &text_rect);
                }
            }

            tig_font_pop();
        }
    }
}

// 0x587570
void sub_587570(TigRect* rect, tig_window_handle_t window_handle)
{
    TigFont font_desc;
    TigRect text_rect;
    MesFileEntry mes_file_entry;

    if (!dword_68699C) {
        return;
    }

    if (rect == NULL
        || (rect->x < stru_5CCCA8.x + stru_5CCCA8.width
            && rect->y < stru_5CCCA8.y + stru_5CCCA8.height
            && stru_5CCCA8.x < rect->x + rect->width
            && stru_5CCCA8.y < rect->y + rect->height)) {
        sub_588180(&stru_5CCCA8, window_handle);

        tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_BROWN));
        mes_file_entry.num = 2210; // "Name"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (140 - font_desc.width) / 2 + 348;
        text_rect.y = 104;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if (dword_5CCAD8 == 0) {
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            text_rect.x--;
            text_rect.y--;
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        mes_file_entry.num = 2211; // "Type"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.y = 104;
        text_rect.x = (68 - font_desc.width) / 2 + 499;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if (dword_5CCAD8 == 1) {
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            text_rect.x--;
            text_rect.y--;
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        mes_file_entry.num = 2212; // "Ping"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (30 - font_desc.width) / 2 + 575;
        text_rect.y = 104;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if (dword_5CCAD8 == 2) {
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            text_rect.x--;
            text_rect.y--;
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        mes_file_entry.num = 2213; // "Players"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (52 - font_desc.width) / 2 + 623;
        text_rect.y = 104;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if (dword_5CCAD8 == 3) {
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            text_rect.x--;
            text_rect.y--;
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        mes_file_entry.num = 2214; // "UpTime"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (51 - font_desc.width) / 2 + 694;
        text_rect.y = 104;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if (dword_5CCAD8 == 4) {
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            text_rect.x--;
            text_rect.y--;
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        tig_font_pop();
    }
}

// 0x587960
void sub_587960(TigRect* rect, tig_window_handle_t window_handle)
{
    TigFont font_desc;
    TigRect text_rect;
    MesFileEntry mes_file_entry;

    if (!dword_68699C) {
        return;
    }

    if (rect == NULL
        || (rect->x < stru_5CCCB8.x + stru_5CCCB8.width
            && rect->y < stru_5CCCB8.y + stru_5CCCB8.height
            && stru_5CCCB8.x < rect->x + rect->width
            && stru_5CCCB8.y < rect->y + rect->height)) {
        sub_588180(&stru_5CCCB8, window_handle);

        tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_BROWN));

        mes_file_entry.num = 2220; // "All"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (50 - font_desc.width) / 2 + 351;
        text_rect.y = 566;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if (dword_687250 == 7) {
            text_rect.x--;
            text_rect.y--;
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        mes_file_entry.num = 2221; // "FFA"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.y = 566;
        text_rect.x = (52 - font_desc.width) / 2 + 402;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if ((dword_687250 & TIG_NET_SERVER_FILTER_FREE_FOR_ALL) != 0
            && dword_687250 != 7) {
            text_rect.x--;
            text_rect.y--;
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        mes_file_entry.num = 2222; // "Cooperative"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (97 - font_desc.width) / 2 + 455;
        text_rect.y = 566;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if ((dword_687250 & TIG_NET_SERVER_FILTER_COOPERATIVE) != 0
            && dword_687250 != 7) {
            text_rect.x--;
            text_rect.y--;
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        mes_file_entry.num = 2223; // "Roleplay"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (122 - font_desc.width) / 2 + 553;
        text_rect.y = 566;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if ((dword_687250 & TIG_NET_SERVER_FILTER_ROLEPLAY) != 0
            && dword_687250 != 7) {
            text_rect.x--;
            text_rect.y--;
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        mes_file_entry.num = 2224; // "Bookmarked"
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.y = 566;
        text_rect.x = (104 - font_desc.width) / 2 + 640;
        text_rect.width = font_desc.width;
        text_rect.height = 15;
        tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

        if ((dword_687250 & TIG_NET_SERVER_FILTER_BOOKMARKED) != 0) {
            text_rect.x--;
            text_rect.y--;
            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
            tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
            tig_font_pop();
        }

        tig_font_pop();
    }
}

// 0x587D60
void sub_587D60(TigRect* rect, tig_window_handle_t window_handle)
{
    TigRect text_rect;
    int y;
    int index;
    TigNetServer* server;
    tig_font_handle_t font;
    TigFont font_desc;
    MesFileEntry mes_file_entry;
    char str[128];
    char* copy;
    size_t pos;

    if (!dword_68699C) {
        return;
    }

    if (rect == NULL
        || (rect->x < stru_5CCC48.x + stru_5CCC48.width
            && rect->y < stru_5CCC48.y + stru_5CCC48.height
            && stru_5CCC48.x < rect->x + rect->width
            && stru_5CCC48.y < rect->y + rect->height)) {
        index = 0;
        y = 131;

        while (y < 551) {
            text_rect.x = 342;
            text_rect.y = y;
            text_rect.width = 409;
            text_rect.height = 20;
            tig_window_fill(window_handle,
                &text_rect,
                tig_color_make(20 * ((index + 1) % 2), 20 * ((index + 1) % 2), 20 * ((index + 1) % 2)));

            if (dword_6869A0 != NULL && dword_6869A8 + index < dword_6869A4) {
                server = &(dword_6869A0[dword_6869A8 + index]);
                if (sub_588D00() == server) {
                    font = mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_GOLD);
                } else if ((server->options & TIG_NET_SERVER_BOOKMARKED) != 0) {
                    font = mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_BLUE);
                } else {
                    font = mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE);
                }

                tig_font_push(font);

                if ((server->options & TIG_NET_SERVER_PASSWORD_PROTECTED) != 0) {
                    str[0] = '\x7F';
                    str[1] = '\0';
                    font_desc.width = 0;
                    font_desc.str = str;
                    tig_font_measure(&font_desc);
                    text_rect.width = font_desc.width;
                    text_rect.x = 351;
                    text_rect.y = y;
                    text_rect.height = font_desc.height;
                    tig_window_text_write(window_handle, str, &text_rect);
                }

                copy = STRDUP(server->name);
                pos = strlen(copy);
                do {
                    copy[pos] = '\0';
                    font_desc.width = 0;
                    font_desc.str = copy;
                    tig_font_measure(&font_desc);
                    pos--;
                } while (font_desc.width > 139);

                text_rect.x = 366;
                text_rect.y = y;
                text_rect.width = 139;
                text_rect.height = 20;
                tig_window_text_write(window_handle, copy, &text_rect);
                FREE(copy);

                switch (server->type) {
                case TIG_NET_SERVER_TYPE_FREE_FOR_ALL:
                    mes_file_entry.num = 2221; // "FFA"
                    break;
                case TIG_NET_SERVER_TYPE_COOPERATIVE:
                    mes_file_entry.num = 2222; // "Cooperative"
                    break;
                case TIG_NET_SERVER_TYPE_ROLEPLAY:
                    mes_file_entry.num = 2223; // "Roleplay"
                    break;
                default:
                    // Should be unreachable.
                    assert(0);
                }

                mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
                font_desc.width = 0;
                font_desc.str = mes_file_entry.str;
                tig_font_measure(&font_desc);
                text_rect.x = (68 - font_desc.width) / 2 + 506;
                text_rect.y = y;
                text_rect.width = font_desc.width;
                text_rect.height = 20;
                tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);

                sprintf(str, "%d", server->field_58);
                font_desc.width = 0;
                font_desc.str = str;
                tig_font_measure(&font_desc);
                text_rect.x = 611 - font_desc.width;
                text_rect.y = y;
                text_rect.width = font_desc.width;
                text_rect.height = 20;
                tig_window_text_write(window_handle, str, &text_rect);

                copy = (char*)CALLOC(10, 1);
                snprintf(copy, 10, "%d(%d)", server->num_players, server->max_players);
                font_desc.width = 0;
                font_desc.str = copy;
                tig_font_measure(&font_desc);
                text_rect.x = (69 - font_desc.width) / 2 + 624;
                text_rect.y = y;
                text_rect.width = 69;
                text_rect.height = 20;
                tig_window_text_write(window_handle, copy, &text_rect);
                FREE(copy);

                font_desc.width = 0;
                font_desc.str = sub_588E10(server->field_54);
                tig_font_measure(&font_desc);
                text_rect.width = font_desc.width;
                text_rect.x = (66 - font_desc.width) / 2 + 694;
                text_rect.y = y;
                text_rect.height = font_desc.height;
                tig_window_text_write(window_handle, font_desc.str, &text_rect);
                tig_font_pop();
            }

            y += 20;
            index++;
        }
    }
}

// 0x588180
void sub_588180(TigRect* rect, tig_window_handle_t window_handle)
{
    TigArtBlitInfo art_blit_info;
    TigRect blit_rect;

    if (!dword_68699C) {
        return;
    }

    if (window_handle == TIG_WINDOW_HANDLE_INVALID) {
        return;
    }

    tig_art_interface_id_create(750, 0, 0, 0, &(art_blit_info.art_id));

    if (rect != NULL) {
        blit_rect = *rect;
    } else {
        blit_rect.x = 0;
        blit_rect.y = 0;
        blit_rect.width = 800;
        blit_rect.height = 600;
    }

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &blit_rect;
    art_blit_info.dst_rect = &blit_rect;

    tig_window_blit_art(window_handle, &art_blit_info);

    if (multiplayer_mm_is_active()) {
        if (rect == NULL
            || (rect->x < stru_5CCCC8.x + stru_5CCCC8.width
                && rect->y < stru_5CCCC8.y + stru_5CCCC8.height
                && stru_5CCCC8.x < rect->x + rect->width
                && stru_5CCCC8.y < rect->y + rect->height)) {
            uint8_t* rgb;
            int width;
            int height;

            if (multiplayer_mm_ad_rgb_get(&rgb, &width, &height)) {
                TigVideoBuffer* vb;
                TigVideoBufferData vb_data;
                int y;
                int x;

                tig_window_vbid_get(window_handle, &vb);
                tig_video_buffer_lock(vb);
                tig_video_buffer_data(vb, &vb_data);

                // NOTE: For unknown reason origina; code only supports 16 bpp.
                // This implementation also supports 16 bpp just for clarity.
                // Since WON servers are no longer available I guess we'll never
                // haver proper implementation any way.
                if (vb_data.bpp == 16) {
                    uint16_t* pixels = (uint16_t*)vb_data.surface_data.pixels
                        + stru_5CCCC8.y * (vb_data.pitch / 2)
                        + stru_5CCCC8.x;

                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            pixels[x] = (uint16_t)tig_color_make(rgb[0], rgb[1], rgb[2]);
                            rgb += 3;
                        }
                        pixels += vb_data.pitch / 2;
                    }
                }

                tig_video_buffer_unlock(vb);
                multiplayer_mm_ad_release();
                tig_window_set_needs_display_in_rect(&stru_5CCCC8);
            }
        }
    }

    sub_549960();
}

// 0x5883E0
void sub_5883E0(tig_button_handle_t button_handle, tig_window_handle_t window_handle)
{
    MesFileEntry mes_file_entry;
    TigRect rect;
    TigRect text_rect;
    TigFont font_desc;
    bool rc;

    rect.x = 125;
    rect.y = 550;
    rect.width = 168;
    rect.height = 15;
    mes_file_entry.num = 2240; // "Connecting to server..."
    mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
    sub_588D90(mes_file_entry.str);
    tig_window_display();

    if (sub_588D00() != NULL
        && (sub_588D00()->options & TIG_NET_SERVER_PASSWORD_PROTECTED) != 0
        && byte_686A00[0] == '\0') {
        mes_file_entry.num = 2250; // "Invalid password."
        mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
        sub_588D90(mes_file_entry.str);
        dword_687258 = 3;
    } else {
        if (byte_6869B0[0] != '\0') {
            tig_net_local_server_set_password(byte_686A00);
            rc = tig_net_connect_directly(byte_6869B0,
                sub_4A4230(0),
                sub_4A4240(0));
        } else if (sub_588D00() != NULL) {
            tig_net_local_server_set_password(byte_686A00);
            rc = tig_net_connect_to_server(sub_588D00()->addr.sin_addr.s_addr,
                sub_4A4230(0),
                sub_4A4240(0));
        } else {
            rc = false;
        }

        if (rc) {
            mes_file_entry.num = 2241; // "Waiting for server acceptance..."
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
            sub_588D90(mes_file_entry.str);
            dword_687258 = 1;
            mes_file_entry.num = 2208; // "Waiting To Join"
            tig_button_hide(button_handle);
            dword_5CCAF0 = button_handle;
        } else {
            mes_file_entry.num = 2242; // "Could not connect to server."
            mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);
            sub_588D90(mes_file_entry.str);
            mes_file_entry.num = 2209; // "Join Failed"
            dword_687258 = 2;
        }
    }

    mes_get_msg(serverlist_ui_mainmenu_mes_file, &mes_file_entry);

    tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);
    tig_window_fill(window_handle,
        &rect,
        tig_color_make(0, 0, 0));
    text_rect.x = (168 - font_desc.width) / 2 + 125;
    text_rect.y = 550;
    text_rect.width = font_desc.width;
    text_rect.height = 15;
    tig_window_text_write(window_handle, mes_file_entry.str, &text_rect);
    tig_font_pop();
}

// 0x588650
void sub_588650()
{
    unsigned long addr;
    TigNetServer* servers;
    int num_servers;
    MatchmakerRegisterInfo* games;
    int index;

    if (sub_588D00() != NULL && !dword_687254) {
        tig_net_bookmark_add(sub_588D00()->addr.sin_addr.s_addr);
    } else if (byte_6869B0[0] != '\0' && tig_net_inet_addr(byte_6869B0, &addr)) {
        tig_net_bookmark_add(addr);
    }

    if (multiplayer_mm_is_active()) {
        if (!multiplayer_mm_gamelist_get(&games, &num_servers)) {
            return;
        }

        // NOTE: Twice?
        if (sub_5499B0(stru_686968.str) && sub_5499B0(stru_686968.str)) {
            servers = (TigNetServer*)CALLOC(sizeof(*servers), num_servers);

            // FIXME: Meaningless.
            memset(servers, 0, sizeof(*servers) * num_servers);

            for (index = 0; index < num_servers; index++) {
                strcpy(servers[index].name, games[index].name);
                servers[index].addr = games[index].addr;
            }

            // FIXME: Leaks `games`, should be outside this condition.
            multiplayer_mm_gamelist_free(games);
        } else {
            // FIXME: Initialize to silence compiler warnings, but probably
            // should stop immediately.
            servers = NULL;
            num_servers = 0;
        }
    } else {
        tig_net_get_servers(&servers, &num_servers);
    }

    sub_585FF0(servers, num_servers);
    sub_5860D0(NULL);
}

// 0x5887F0
void sub_5887F0()
{
    unsigned long addr;
    TigNetServer* servers;
    int num_servers;
    int index;
    MatchmakerRegisterInfo* games;

    // FIXME: Initialize to silence compiler warning.
    games = NULL;

    if (sub_588D00() != NULL && !dword_687254) {
        tig_net_bookmark_remove(sub_588D00()->addr.sin_addr.s_addr);
    } else {
        if (byte_6869B0[0] != '\0' && tig_net_inet_addr(byte_6869B0, &addr)) {
            tig_net_bookmark_remove(addr);
        }
    }

    if (multiplayer_mm_is_active()) {
        if (!multiplayer_mm_gamelist_get(&games, &num_servers)) {
            return;
        }

        if (sub_5499B0(stru_686968.str)) {
            servers = (TigNetServer*)CALLOC(sizeof(*servers), num_servers);

            // FIXME: Meaningless.
            memset(servers, 0, sizeof(*servers) * num_servers);

            for (index = 0; index < num_servers; index++) {
                strcpy(servers[index].name, games[index].name);
                servers[index].addr = games[index].addr;
            }

            // FIXME: Leaks `games`, should be outside of this block.
            multiplayer_mm_gamelist_free(games);
        } else {
            // FIXME: Initialize to silence compiler warnings, but probably
            // should stop immediately.
            servers = NULL;
            num_servers = 0;
        }
    } else {
        tig_net_get_servers(&servers, &num_servers);
    }

    sub_585FF0(servers, num_servers);

    if (multiplayer_mm_is_active()) {
        FREE(games);
    }

    sub_5860D0(NULL);
}

// 0x588990
void sub_588990()
{
    TigNetServer* servers;
    int num_servers;
    int index;
    MatchmakerRegisterInfo* games;

    if (multiplayer_mm_is_active()) {
        if (!multiplayer_mm_gamelist_get(&games, &num_servers)) {
            return;
        }

        // NOTE: Twice?
        if (sub_5499B0(stru_686968.str) && sub_5499B0(stru_686968.str)) {
            servers = (TigNetServer*)CALLOC(sizeof(*servers), num_servers);

            // FIXME: Meaningless.
            memset(servers, 0, sizeof(*servers) * num_servers);

            for (index = 0; index < num_servers; index++) {
                strcpy(servers[index].name, games[index].name);
                servers[index].addr = games[index].addr;
                servers[index].addr.sin_addr.s_addr = tig_net_ntohl(servers[index].addr.sin_addr.s_addr);
                sub_52B0F0(&games[index].addr);
            }

            // FIXME: Leaks `games`, should be outside of this block.
            multiplayer_mm_gamelist_free(games);
        } else {
            // FIXME: Initialize to silence compiler warnings, but probably
            // should stop immediately.
            servers = NULL;
            num_servers = 0;
        }
    } else {
        tig_net_get_servers(&servers, &num_servers);
    }

    sub_585FF0(servers, num_servers);
}

// 0x588B10
void sub_588B10(TigNetServer* server)
{
    tig_timer_now(&dword_68698C);
    qsort(dword_6869A0, dword_6869A4, sizeof(*dword_6869A0), sub_588B70);

    if (sub_588D00() != NULL) {
        sub_588CC0(sub_588C80(server, dword_6869A0, dword_6869A4), false);
    }
}

// 0x588B70
int sub_588B70(const void* va, const void* vb)
{
    const TigNetServer* a = (const TigNetServer*)va;
    const TigNetServer* b = (const TigNetServer*)vb;
    int v1[5];
    bool cont;
    int index;
    int cmp;
    int type = 0;

    if (tig_timer_elapsed(dword_68698C) > 1000) {
        return 0;
    }

    memcpy(v1, dword_5CCADC, sizeof(dword_5CCADC));

    for (;;) {
        cont = false;
        for (index = 0; index < 5; index++) {
            if (v1[index] != 0) {
                cont = true;
                break;
            }
        }

        for (index = 0; index < 5; index++) {
            if (v1[index] > v1[type]) {
                type = index;
            }
        }

        switch (type) {
        case 0:
            cmp = strcmpi(a->name, b->name);
            if (cmp != 0) {
                return cmp;
            }
            break;
        case 1:
            cmp = a->type - b->type;
            if (cmp != 0) {
                return cmp;
            }
            break;
        case 2:
            cmp = a->field_58 - b->field_58;
            if (cmp != 0) {
                return cmp;
            }
            break;
        case 3:
            cmp = a->num_players - b->num_players;
            if (cmp != 0) {
                return cmp;
            }
            break;
        case 4:
            break;
        default:
            tig_debug_printf("ServerList_UI: FATAL: unknown sort by type %d.\n", type);
            exit(EXIT_SUCCESS); // FIXME: Should be `EXIT_FAILURE`.
        }

        v1[type] = 0;

        if (!cont) {
            break;
        }
    }

    return 0;
}

// 0x588C80
TigNetServer* sub_588C80(TigNetServer* server, TigNetServer* servers, int cnt)
{
    int index;

    for (index = 0; index < cnt; index++) {
        if (servers[index].addr.sin_addr.s_addr == server->addr.sin_addr.s_addr) {
            return &(servers[index]);
        }
    }

    return NULL;
}

// 0x588CC0
void sub_588CC0(TigNetServer* server, bool a2)
{
    if (server != NULL) {
        if (a2) {
            byte_6869B0[0] = '\0';
        }
    }

    if (a2) {
        dword_687254 = 0;
    }

    if (server != NULL) {
        dword_6869AC = server->addr.sin_addr.s_addr;
    }

    byte_686A50[0] = '\0';
}

// 0x588D00
TigNetServer* sub_588D00()
{
    int index;

    for (index = 0; index < dword_6869A4; index++) {
        if (dword_6869A0[index].addr.sin_addr.s_addr == dword_6869AC) {
            return &(dword_6869A0[index]);
        }
    }

    return NULL;
}

// 0x588D70
void sub_588D70(int value)
{
    dword_6869A8 = value;
    sub_587D60(0, sub_549820());
}

// 0x588D90
void sub_588D90(const char* str)
{
    strcat(byte_686A50, str);
    strcat(byte_686A50, "\n");
    sub_586ED0(&stru_5CCC98, sub_549820());
}

// 0x588E10
const char* sub_588E10(int seconds)
{
    // 0x686970
    static char byte_686970[25];

    if (seconds / 86400 != 0) {
        snprintf(byte_686970, sizeof(byte_686970),
            "%d days",
            seconds / 86400);
    } else {
        snprintf(byte_686970, sizeof(byte_686970),
            "%02d:%02d:%02d",
            seconds % 86400 / 3600,
            seconds % 86400 % 3600 / 60,
            seconds % 86400 % 3600 % 60);
    }

    return byte_686970;
}

// 0x588EC0
void sub_588EC0(TigNetServer* servers, int count)
{
    if (sub_5496D0() == 17) {
        sub_585FF0(servers, count);
        sub_5860D0(0);
    }
}

// 0x588EF0
void sub_588EF0()
{
    if (sub_541680()) {
        sub_5412D0();
    }
}

// 0x588F00
void sub_588F00(int x, int y)
{
    (void)x;
    (void)y;
}

// 0x588F10
bool sub_588F10(int a1)
{
    switch (a1) {
    case 0:
        tig_net_reset_connection();
        sub_5417A0(false);
        mainmenu_ui_create_multiplayer_hub();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    case 1:
        tig_net_reset_connection();
        sub_5417A0(false);
        multiplayer_hub_ui_init();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    case 2:
        tig_net_reset_connection();
        sub_5417A0(false);
        sub_584AE0();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        break;
    case 3:
        sub_5883E0(stru_5CCAF8[3].button_handle, sub_549820());
        break;
    case 4:
        sub_588650();
        break;
    case 5:
        sub_5887F0();
        break;
    case 6:
        sub_588990();
        break;
    }

    return 0;
}

// 0x588FF0
void sub_588FF0(const char* path, int progress, tig_window_handle_t window_handle)
{
    TigRect v5;
    TigRect v6;
    TigRect v7;
    TigFont font_desc;
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    char tmp[_MAX_FNAME + _MAX_EXT];

    v5.height = 15;
    v5.width = 210;
    v5.x = 60;
    v5.y = 450;

    v6.height = 15;
    v6.width = 210;
    v6.x = 60;
    v6.y = 450;

    v7.height = 17;
    v7.width = 212;
    v7.x = 59;
    v7.y = 449;

    if (dword_68699C && path != NULL) {
        tig_window_fill(window_handle, &v7, tig_color_make(255, 255, 255));
        tig_window_fill(window_handle, &v5, tig_color_make(0, 0, 0));
        tig_window_fill(window_handle, &v6, tig_color_make(255, 0, 0));

        _splitpath(path, NULL, NULL, fname, ext);
        sprintf(tmp, "%s%s %d%%", fname, ext, progress);

        tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));
        font_desc.width = 0;
        font_desc.str = tmp;
        tig_font_measure(&font_desc);
        while (font_desc.width > v5.width) {
            fname[strlen(fname) - 1] = '\0';
            sprintf(tmp, "%s..%s %d%%", fname, ext, progress);
            font_desc.width = 0;
            tig_font_measure(&font_desc);
        }
        v5.width = font_desc.width;
        v5.x = (210 - font_desc.width) / 2 + 60;
        tig_window_text_write(window_handle, tmp, &v5);
        tig_font_pop();
    }
}
