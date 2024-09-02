#include "ui/server_list_ui.h"

#include <stdio.h>

#include <tig/tig.h>

#include "game/mes.h"
#include "ui/scrollbar_ui.h"

static const char* sub_588E10(int seconds);
static void sub_588EC0(TigNetServer* servers, int count);
static void sub_588EF0();

// 0x5994BC
static int dword_5994BC[3] = {
    2,
    16,
    21,
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

// 0x686A50
static char byte_686A50[2048];

// 0x687254
static bool dword_687254;

// 0x687258
static int dword_687258;

// 0x585BC0
void serverlist_ui_init()
{
    // TODO: Incomplete.
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
    mainmenu_ui_create_window_func(0);

    if (sub_4A4ED0()) {
        tig_button_show(stru_5CCAF8[1].field_C);
        tig_net_on_server_bcast(sub_585F00);
        sub_588990();
    } else {
        tig_button_hide(stru_5CCAF8[1].field_C);
        tig_button_hide(stru_5CCAF8[6].field_C);
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
    sb_create_info.field_4.x = 771;
    sb_create_info.field_4.y = 135;
    sb_create_info.field_4.width = 13;
    sb_create_info.field_4.height = 420;
    sb_create_info.rect.x = 0;
    sb_create_info.rect.y = 0;
    sb_create_info.rect.width = 800;
    sb_create_info.rect.height = 600;
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
void sub_585F00()
{
    // TODO: Incomplete.
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
void sub_586150()
{
    // TODO: Incomplete.
}

// 0x5862D0
void sub_5862D0()
{
    // TODO: Incomplete.
}

// 0x586AB0
void sub_586AB0()
{
    // TODO: Incomplete.
}

// 0x586AD0
void sub_586AD0()
{
    // TODO: Incomplete.
}

// 0x586D20
void sub_586D20()
{
    // TODO: Incomplete.
}

// 0x586ED0
void sub_586ED0()
{
    // TODO: Incomplete.
}

// 0x587570
void sub_587570()
{
    // TODO: Incomplete.
}

// 0x587960
void sub_587960()
{
    // TODO: Incomplete.
}

// 0x587D60
void sub_587D60()
{
    // TODO: Incomplete.
}

// 0x588180
void sub_588180()
{
    // TODO: Incomplete.
}

// 0x5883E0
void sub_5883E0()
{
    // TODO: Incomplete.
}

// 0x588650
void sub_588650()
{
    // TODO: Incomplete.
}

// 0x5887F0
void sub_5887F0()
{
    // TODO: Incomplete.
}

// 0x588990
void sub_588990()
{
    // TODO: Incomplete.
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
void sub_588B70()
{
    // TODO: Incomplete.
}

// 0x588C80
TigNetServer* sub_588C80(TigNetServer* server, TigNetServer* servers, int cnt)
{
    // TODO: Incomplete.
}

// 0x588CC0
void sub_588CC0()
{
    // TODO: Incomplete.
}

// 0x588D00
TigNetServer* sub_588D00()
{
    // TODO: Incomplete.
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
void sub_588F00()
{
}

// 0x588F10
void sub_588F10(int a1)
{
    switch (a1) {
    case 0:
        tig_net_reset_connection();
        sub_5417A0(false);
        sub_583200();
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
        sub_5883E0(stru_5CCAF8[3].field_C, sub_549820());
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
void sub_588FF0()
{
    // TODO: Incomplete.
}
