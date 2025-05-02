#include "ui/multiplayer_hub_ui.h"

#include <stdio.h>

#include "game/gamelib.h"
#include "game/mes.h"
#include "game/multiplayer.h"
#include "game/stat.h"
#include "ui/mainmenu_ui.h"
#include "ui/mp_ctrl_ui.h"
#include "ui/scrollbar_ui.h"
#include "ui/server_list_ui.h"
#include "ui/textedit_ui.h"

typedef enum ChatEntryType {
    CHAT_ENTRY_TYPE_INCOMING_MSG,
    CHAT_ENTRY_TYPE_INCOMING_EMOTE,
    CHAT_ENTRY_TYPE_SYSTEM_INFO,
    CHAT_ENTRY_TYPE_BLANK,
    CHAT_ENTRY_TYPE_OUTGOING_MSG,
    CHAT_ENTRY_TYPE_OUTGOING_EMOTE,
    CHAT_ENTRY_TYPE_INCOMING_WHISPER,
    CHAT_ENTRY_TYPE_OUTGOING_WHISPER,
    CHAT_ENTRY_TYPE_WHO_LIST,
    CHAT_ENTRY_TYPE_JOIN,
    CHAT_ENTRY_TYPE_LEAVE,
    CHAT_ENTRY_TYPE_COUNT,
} ChatEntryType;

typedef struct S5CC5F8 {
    int num;
    int x;
    int y;
} S5CC5F8;

static_assert(sizeof(S5CC5F8) == 0xC, "wrong size");

typedef struct S684708 {
    /* 0000 */ char* str;
    /* 0004 */ int type;
    /* 0008 */ char field_8[80];
} S684708;

static_assert(sizeof(S684708) == 0x58, "wrong size");

static void sub_581F30();
static void sub_581F80();
static void sub_581F90(int a1);
static void sub_581FB0(TextEdit* textedit);
static void sub_581FC0(TextEdit* textedit);
static void sub_582060(const char* str);
static const char* sub_582440(const char* src, char* dst);
static void sub_5824C0(const char** src);
static bool sub_5826D0(const char* a1, int a2, const char* a3);
static void sub_582790(TigRect* rect);
static void sub_582860(TigRect* rect);
static void sub_5829D0(TigRect* rect);
static void sub_582AD0(TigRect* rect);
static bool sub_582D10(const char* str);
static bool sub_582D20(const char* a, const char* b);
static const char* sub_582D40(const char* str);
static const char* sub_582D50(const char* str);
static void sub_582D60(TigRect* rect);
static void sub_582E50(TigRect* rect);
static void sub_582E80();
static void sub_5830F0();
static tig_font_handle_t sub_583140(int type);
static void sub_5836A0();
static void sub_5837A0(TigRect* rect);
static void sub_583830(TigRect* rect);
static const char* sub_584A40(int value);
static void sub_584C30(TigRect* rect);
static const char* sub_585630(int num);
static void sub_585970(char* buffer);
static void sub_585A20();
static void sub_585BA0(TextEdit* textedit);
static void sub_585BB0(TextEdit* textedit);

// 0x599484
static int dword_599484[3] = {
    2,
    16,
    21,
};

// 0x599494
static int dword_599494[2] = {
    2,
    16,
};

// 0x5994A0
static int dword_5994A0[2] = {
    2,
    16,
};

// 0x5994AC
static int dword_5994AC[3] = {
    2,
    16,
    21,
};

// 0x5CBF80
static int dword_5CBF80 = 52;

// 0x5CBF88
static struct {
    const char* str;
    int cmd;
} stru_5CBF88[] = {
    { NULL, 0 },
    { NULL, 1 },
    { NULL, 2 },
    { NULL, -1 },
};

// 0x5CBFA8
static MainMenuColor dword_5CBFA8[] = {
    MM_COLOR_GOLD,
    MM_COLOR_GOLD,
    MM_COLOR_RED,
    MM_COLOR_WHITE,
    MM_COLOR_WHITE,
    MM_COLOR_WHITE,
    MM_COLOR_GREEN,
    MM_COLOR_GREEN,
    MM_COLOR_BLUE,
    MM_COLOR_WHITE,
    MM_COLOR_WHITE,
    MM_COLOR_WHITE,
};

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

// 0x5CC058
MainMenuButtonInfo stru_5CC058[3] = {
    { 162, 34, -1, TIG_BUTTON_HANDLE_INVALID, 0x15, 0, 8, { 0 }, -1 },
    { 162, 73, -1, TIG_BUTTON_HANDLE_INVALID, 0x11, 0, 8, { 0 }, -1 },
    { 162, 122, -1, TIG_BUTTON_HANDLE_INVALID, 0x12, 0, 8, { 0 }, -1 },
};

// 0x5CC0E8
MainMenuButtonInfo stru_5CC0E8[5] = {
    { 162, 34, -1, TIG_BUTTON_HANDLE_INVALID, -1, 0, 8, { 0 }, -1 },
    { 162, 73, -1, TIG_BUTTON_HANDLE_INVALID, 0x11, 0, 8, { 0 }, -1 },
    { 162, 122, -1, TIG_BUTTON_HANDLE_INVALID, 0x12, 0, 8, { 0 }, -1 },
    { 92, 464, 33, TIG_BUTTON_HANDLE_INVALID, 0x17, 0, 0xC, { 0 }, -1 },
    { 197, 464, 32, TIG_BUTTON_HANDLE_INVALID, 0x17, 0, 0xC, { 0 }, -1 },
};

// 0x5CC258
static TigRect stru_5CC258 = { 289, 388, 13, 164 };

// 0x5CC268
static TigRect stru_5CC268 = { 0, 364, 333, 236 };

// 0x5CC278
MainMenuButtonInfo stru_5CC278[7] = {
    { 162, 34, -1, TIG_BUTTON_HANDLE_INVALID, 0x10, 0, 0, { 0 }, -1 },
    { 162, 73, -1, TIG_BUTTON_HANDLE_INVALID, 0x11, 0, 0, { 0 }, -1 },
    { 162, 122, -1, TIG_BUTTON_HANDLE_INVALID, 0x12, 0, 0, { 0 }, -1 },
    { 162, 161, -1, TIG_BUTTON_HANDLE_INVALID, 0x17, 0, 0, { 0 }, -1 },
    { 162, 200, -1, TIG_BUTTON_HANDLE_INVALID, 0x16, 0, 0, { 0 }, -1 },
    { 162, 239, -1, TIG_BUTTON_HANDLE_INVALID, 0x15, 0, 0, { 0 }, -1 },
    { 162, 278, -1, TIG_BUTTON_HANDLE_INVALID, 0x15, 0, 0, { 0 }, -1 },
};

// 0x5CC470
static TigRect stru_5CC470 = { 327, 120, 165, 15 };

// 0x5CC480
static TigRect stru_5CC480 = { 327, 175, 165, 15 };

// 0x5CC490
static TigRect stru_5CC490 = { 327, 230, 165, 15 };

// 0x5CC4A0
static TigRect stru_5CC4A0 = { 327, 338, 165, 15 };

// 0x5CC4B0
static TigRect stru_5CC4B0 = { 327, 310, 164, 14 };

// 0x5CC4C0
MainMenuButtonInfo stru_5CC4C0[2] = {
    { 343, 376, 33, TIG_BUTTON_HANDLE_INVALID, 21, 0, 0, { 0 }, -1 },
    { 446, 376, 32, TIG_BUTTON_HANDLE_INVALID, -2, 0, 0, { 0 }, -1 },
};

// 0x5CC520
MainMenuButtonInfo stru_5CC520[2] = {
    { 343, 376, 33, TIG_BUTTON_HANDLE_INVALID, 21, 0, 0, { 0 }, -1 },
    { 446, 376, 32, TIG_BUTTON_HANDLE_INVALID, -2, 0, 0, { 0 }, -1 },
};

// 0x5CC5F0
static mes_file_handle_t dword_5CC5F0 = MES_FILE_HANDLE_INVALID;

// 0x5CC5F8
static S5CC5F8 stru_5CC5F8[15] = {
    { 2300, 61, 233 },
    { 2301, 61, 302 },
    { 2302, 61, 373 },
    { 2303, 333, 270 },
    { 2305, 333, 340 },
    { 2306, 333, 410 },
    { 2307, 451, 61 },
    { 2308, 573, 200 },
    { 2309, 573, 270 },
    { 2310, 573, 340 },
    { 2311, 573, 410 },
    { 2313, 573, 130 },
    { 2312, 333, 130 },
    { 2314, 333, 200 },
    { -1, 0, 0 },
};

// 0x5CC6AC
static int dword_5CC6AC = -1;

// 0x68674C
static char byte_68674C[80];

// 0x5CC6B0
static TextEdit stru_5CC6B0 = {
    0,
    byte_68674C,
    sizeof(byte_68674C),
    sub_585BA0,
    sub_585BB0,
    NULL,
};

// 0x5CC6C8
MainMenuButtonInfo stru_5CC6C8[20] = {
    { 162, 22, -1, TIG_BUTTON_HANDLE_INVALID, 0x15, 0, 0, { 0 }, -1 },
    { 162, 56, -1, TIG_BUTTON_HANDLE_INVALID, 0x11, 0, 0, { 0 }, -1 },
    { 162, 90, -1, TIG_BUTTON_HANDLE_INVALID, 0x17, 0, 0, { 0 }, -1 },
    { 460, 505, 357, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 54, 325, 757, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 250, 325, 758, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 54, 395, 757, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 250, 395, 758, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 565, 222, 757, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 761, 222, 758, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 565, 362, 757, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 761, 362, 758, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 565, 432, 757, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 761, 432, 758, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 565, 152, 757, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 761, 152, 758, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 325, 152, 757, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 521, 152, 758, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 325, 222, 757, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
    { 521, 222, 758, TIG_BUTTON_HANDLE_INVALID, 0x12, -1, 0x0C, { 0 }, -1 },
};

// 0x684688
static char byte_684688[128];

// 0x684708
static S684708 stru_684708[78];

// 0x6861D8
static MesFileEntry stru_6861D8;

// 0x6861E0
static TextEdit stru_6861E0;

// 0x6861F8
static char byte_6861F8[80];

// 0x686248
static ScrollbarId stru_686248;

// 0x686250
static char byte_686250[128];

// 0x6862D0
static ScrollbarId stru_6862D0;

// 0x6862D8
static mes_file_handle_t dword_6862D8;

// 0x6862DC
static MatchmakerChatroom* dword_6862DC;

// 0x6862E0
static int dword_6862E0;

// 0x6862E4
static tig_font_handle_t dword_6862E4[CHAT_ENTRY_TYPE_COUNT];

// 0x686310
static MesFileEntry stru_686310;

// 0x686318
static MesFileEntry stru_686318;

// 0x686320
static char byte_686320[256];

// 0x686420
static char byte_686420[256];

// 0x686520
static ScrollbarId stru_686520;

// 0x686528
static mes_file_handle_t dword_686528;

// 0x68652C
static bool dword_68652C;

// 0x686530
static MesFileEntry stru_686530;

// 0x686538
static mes_file_handle_t dword_686538;

// 0x68653C
static char won_account[128];

// 0x6865BC
static char won_password[128];

// 0x68663C
static char won_password_confirmation[128];

// 0x6866BC
static char byte_6866BC[128];

// 0x68673C
static int dword_68673C;

// 0x686740
static GameModuleList stru_686740;

// 0x6867A0
static MesFileEntry stru_6867A0;

// 0x6867A8
static char byte_6867A8[80];

// 0x6867F8
static char byte_6867F8[256];

// 0x6868F8
static char byte_6868F8[80];

// 0x686948
static char byte_686948[24];

// 0x686960
static bool dword_686960;

// 0x686964
static int dword_686964;

// 0x581700
void multiplayer_hub_ui_init()
{
    ScrollbarUiControlInfo sb_create_info;

    sub_549830(23);

    if (!mes_load("mes\\Multiplayer.mes", &dword_6862D8)) {
        tig_debug_printf("MultiplayerHUB: could not '%s', aborting create.\n", "mes\\Multiplayer.mes");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    stru_6861D8.num = 3000;
    mes_get_msg(dword_6862D8, &stru_6861D8);

    mainmenu_ui_create_window();
    sub_581F30();

    sb_create_info.flags = 0xFFFF;
    sb_create_info.scrollbar_rect.x = 288;
    sb_create_info.scrollbar_rect.y = 182;
    sb_create_info.scrollbar_rect.width = 13;
    sb_create_info.scrollbar_rect.height = 389;
    sb_create_info.content_rect.x = 37;
    sb_create_info.content_rect.y = 188;
    sb_create_info.content_rect.width = 259;
    sb_create_info.content_rect.height = 368;
    sb_create_info.field_24 = 0;
    sb_create_info.field_28 = 0;
    sb_create_info.field_2C = 1;
    sb_create_info.field_30 = 1;
    sb_create_info.field_34 = 1;
    sb_create_info.field_38 = 0;
    sb_create_info.field_3C = 0;
    sb_create_info.field_40 = sub_582E50;
    scrollbar_ui_control_create(&stru_686248, &sb_create_info, sub_549820());

    sb_create_info.scrollbar_rect.x = 771;
    sb_create_info.scrollbar_rect.y = 95;
    sb_create_info.scrollbar_rect.width = 13;
    sb_create_info.scrollbar_rect.height = 408;
    sb_create_info.content_rect.x = 336;
    sb_create_info.content_rect.y = 99;
    sb_create_info.content_rect.width = 440;
    sb_create_info.content_rect.height = 1170;
    sb_create_info.field_24 = 52;
    sb_create_info.field_38 = 52;
    sb_create_info.field_3C = sub_581F90;
    scrollbar_ui_control_create(&stru_6862D0, &sb_create_info, sub_549820());

    stru_6861E0.flags = 0;
    stru_6861E0.size = 80;
    stru_6861E0.on_enter = sub_581FC0;
    stru_6861E0.on_change = sub_581FB0;
    stru_6861E0.on_tab = 0;
    stru_6861E0.buffer = byte_6861F8;
    textedit_ui_focus(&stru_6861E0);

    sub_582E80();
    sub_581910(NULL);
    sub_549990(dword_599484, 3);
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
void sub_581950(int x, int y)
{
    MainMenuWindowInfo* window;

    window = sub_5496C0(sub_5496D0());
    x += window->content_rect.x;
    y += window->content_rect.y;

    if (x >= stru_5CC018.x && x < stru_5CC018.x + stru_5CC018.width
        && y >= stru_5CC018.y && y < stru_5CC018.y + stru_5CC018.height) {
        sub_5417A0(0);
        multiplayer_hub_ui_create_chat();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
    }
}

// 0x5819D0
bool sub_5819D0(int a1)
{
    switch (a1) {
    case 0:
        sub_5417A0(false);
        mainmenu_ui_create_multiplayer_hub();
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
void multiplayer_hub_ui_create_chat()
{
    sub_549830(24);
    mainmenu_ui_create_window();
    multiplayer_hub_ui_refresh_chat(NULL);
}

// 0x581A80
void multiplayer_hub_ui_destroy_chat()
{
}

// 0x581A90
bool multiplayer_hub_ui_execute_chat(int a1)
{
    MesFileEntry mes_file_entry;

    if (a1 != 3) {
        return true;
    }

    if (!multiplayer_mm_chatroom_create(byte_686250, byte_684688)
        || !sub_5499B0(stru_6861D8.str)) {
        mes_file_entry.num = 10064; // "Cannot create chatroom."
        mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
        sub_5826D0(NULL, CHAT_ENTRY_TYPE_SYSTEM_INFO, mes_file_entry.str);
    }

    sub_5417A0(1);
    sub_5830F0();

    return false;
}

// 0x581B10
void multiplayer_hub_ui_refresh_chat(TigRect* rect)
{
    tig_art_id_t art_id;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    MesFileEntry mes_file_entry;
    TigRect text_rect;
    TigFont font_desc;
    char str[256];
    char* copy;
    size_t pos;

    sub_582790(rect);
    sub_5829D0(rect);

    if (rect == NULL
        || (rect->x < stru_5CC028.x + stru_5CC028.width
            && rect->y < stru_5CC028.y + stru_5CC028.height
            && stru_5CC028.x < rect->x + rect->width
            && stru_5CC028.y < rect->y + rect->height)) {
        tig_art_interface_id_create(756, 0, 0, 0, &art_id);

        src_rect.y = 0;
        src_rect.x = 0;
        src_rect.width = 286;
        src_rect.height = 394;

        art_blit_info.flags = 0;
        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &stru_5CC028;
        tig_window_blit_art(sub_549820(), &art_blit_info);

        mes_file_entry.num = 2401; // "Room Name"
        mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (214 - font_desc.width) / 2 + 55;
        text_rect.y = 247;
        text_rect.width = font_desc.width;
        text_rect.height = font_desc.height;
        tig_window_text_write(sub_549820(), mes_file_entry.str, &text_rect);

        if (sub_549520() == byte_686250) {
            sprintf(str, "%s|", byte_686250);
        } else {
            strcpy(str, byte_686250);
        }
        font_desc.width = 0;
        font_desc.str = str;
        tig_font_measure(&font_desc);
        text_rect.x = (183 - font_desc.width) / 2 + 69;
        text_rect.y = 274;
        text_rect.width = font_desc.width;
        text_rect.height = font_desc.height;
        tig_window_text_write(sub_549820(), str, &text_rect);

        mes_file_entry.num = 2402; // "Password"
        mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
        font_desc.width = 0;
        font_desc.str = mes_file_entry.str;
        tig_font_measure(&font_desc);
        text_rect.x = (214 - font_desc.width) / 2 + 55;
        text_rect.y = 337;
        text_rect.width = font_desc.width;
        text_rect.height = font_desc.height;
        tig_window_text_write(sub_549820(), mes_file_entry.str, &text_rect);

        copy = STRDUP(byte_684688);
        for (pos = 0; copy[pos] != '\0'; pos++) {
            copy[pos] = '*';
        }

        if (sub_549520() == byte_684688) {
            sprintf(str, "%s|", copy);
        } else {
            strcpy(str, copy);
        }
        font_desc.str = str;
        font_desc.width = 0;
        tig_font_measure(&font_desc);
        text_rect.x = (183 - font_desc.width) / 2 + 69;
        text_rect.y = 365;
        text_rect.width = font_desc.width;
        text_rect.height = font_desc.height;
        tig_window_text_write(sub_549820(), font_desc.str, &text_rect);
        FREE(copy);
    }
}

// 0x581E60
void sub_581E60(int x, int y)
{
    MainMenuWindowInfo* window_info;

    window_info = sub_5496C0(sub_5496D0());
    x += window_info->content_rect.x;
    y += window_info->content_rect.y;

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
        multiplayer_mm_chatroom_list_free(dword_6862DC);
        dword_6862DC = NULL;
    }
}

// 0x581F30
void sub_581F30()
{
    MesFileEntry mes_file_entry;
    int index = 0;

    while (stru_5CBF88[index].cmd != -1) {
        mes_file_entry.num = 10100 + index;
        mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
        stru_5CBF88[index].str = mes_file_entry.str;
    }
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
void sub_581FB0(TextEdit* textedit)
{
    (void)textedit;

    sub_582D60(NULL);
}

// 0x581FC0
void sub_581FC0(TextEdit* textedit)
{
    MesFileEntry mes_file_entry;

    (void)textedit;

    if (byte_6861F8[0] == '\0') {
        sub_5826D0(NULL, CHAT_ENTRY_TYPE_BLANK, NULL);
        sub_582AD0(NULL);
        textedit_ui_clear();
        return;
    }

    if (!multiplayer_mm_chatroom_mesg(byte_6861F8)) {
        if (byte_6861F8[0] == '/') {
            sub_582060(&(byte_6861F8[1]));
        } else {
            mes_file_entry.num = 10053; // "Unknown Command. Try /HELP."
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            sub_5826D0(NULL, CHAT_ENTRY_TYPE_SYSTEM_INFO, mes_file_entry.str);
        }
    }

    sub_582AD0(NULL);
    textedit_ui_clear();
}

// 0x582060
void sub_582060(const char* str)
{
    char* copy;
    const char* v1;
    char v2[80];
    char v3[80];
    int index;
    int cmd = -1;
    MesFileEntry mes_file_entry;
    MatchmakerChatroomMember* members;
    int num_members;
    int bit;
    int room;

    copy = STRDUP(str);
    v1 = sub_582440(copy, v2);

    for (index = 0; stru_5CBF88[index].str != NULL; index++) {
        if (strnicmp(v2, stru_5CBF88[index].str, strlen(v2)) == 0) {
            cmd = stru_5CBF88[index].cmd;
            break;
        }
    }

    switch (cmd) {
    case 0:
        while (isspace(*v1)) {
            v1++;
        }
        if (isdigit(*v1)) {
            room = atoi(v1);
            if (room > 0 && room <= dword_6862E0) {
                if (!multiplayer_mm_chatroom_join(&(dword_6862DC[room - 1]), 0)
                    || !sub_5499B0(stru_6861D8.str)) {
                    mes_file_entry.num = 10050; // "Could not join chatroom %d."
                    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
                    snprintf(v2, sizeof(v2), mes_file_entry.str, room);
                    sub_5826D0(NULL, CHAT_ENTRY_TYPE_SYSTEM_INFO, v2);
                }
            } else {
                mes_file_entry.num = 10052; // "Unknown chatroom %s."
                mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
                snprintf(v2, sizeof(v2), mes_file_entry.str, v1);
                sub_5826D0(NULL, CHAT_ENTRY_TYPE_SYSTEM_INFO, v2);
            }
        }
        if (dword_6862DC != NULL) {
            multiplayer_mm_chatroom_list_free(dword_6862DC);
        }
        if (!multiplayer_mm_chatroom_list_get(&dword_6862DC, &dword_6862E0)
            && sub_5499B0(stru_6861D8.str)) {
            dword_6862DC = NULL;
        }
        multiplayer_hub_ui_refresh_chat(NULL);
        break;
    case 1:
        mes_file_entry.num = 10000; // "Arcanum and WON.net CHAT Help File"
        while (mes_search(mainmenu_ui_mes_file(), &mes_file_entry)) {
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            sub_5826D0(NULL, CHAT_ENTRY_TYPE_SYSTEM_INFO, mes_file_entry.str);
            mes_file_entry.num++;
        }
        break;
    case 2:
        if (multiplayer_mm_chatroom_members_get(&members, &num_members)) {
            if (num_members > 0) {
                for (index = 0; index < num_members; index++) {
                    snprintf(v3, sizeof(v3), "%s ", members[index].name);
                    for (bit = 0; bit < 32; bit++) {
                        if ((members[index].flags & (1 << bit)) != 0) {
                            mes_file_entry.num = 10200 + bit;
                            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
                            strncat(v3, mes_file_entry.str, sizeof(v3));
                        }
                    }
                    sub_5826D0(NULL, CHAT_ENTRY_TYPE_WHO_LIST, v3);
                }
            } else {
                mes_file_entry.num = 10062; // "There is nobody in this chatroom? Odd, what about YOURSELF!"
                mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
                sub_5826D0(NULL, CHAT_ENTRY_TYPE_SYSTEM_INFO, mes_file_entry.str);
            }
            multiplayer_mm_chatroom_members_free(members);
        } else {
            mes_file_entry.num = 10063; // "Could not get a list of people in the ChatRoom."
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            sub_5826D0(NULL, CHAT_ENTRY_TYPE_SYSTEM_INFO, mes_file_entry.str);
        }
        break;
    }
}

// 0x582440
const char* sub_582440(const char* src, char* dst)
{
    sub_5824C0(&src);

    while (*src != '\0' && !isspace(*src)) {
        *dst++ = (char)(unsigned char)tolower(*src);
        src++;
    }

    *dst = '\0';

    return src;
}

// 0x5824C0
void sub_5824C0(const char** src)
{
    while (**src != '\0' && isspace(**src)) {
        (*src)++;
    }
}

// 0x582510
void sub_582510(const char* name)
{
    MesFileEntry mes_file_entry;
    char str[80];

    mes_file_entry.num = 10054; // "%s has entered the room."
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

    snprintf(str, sizeof(str), mes_file_entry.str, name);
    sub_5826D0(NULL, CHAT_ENTRY_TYPE_JOIN, str);
    sub_582AD0(NULL);

    if (dword_6862DC != NULL) {
        multiplayer_mm_chatroom_list_free(dword_6862DC);
    }

    if (!multiplayer_mm_chatroom_list_get(&dword_6862DC, &dword_6862E0)
        && sub_5499B0(stru_6861D8.str)) {
        dword_6862DC = NULL;
    }

    sub_582860(NULL);
}

// 0x5825B0
void sub_5825B0(const char* name)
{
    MesFileEntry mes_file_entry;
    char str[80];

    mes_file_entry.num = 10055; // "%s has left."
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

    snprintf(str, sizeof(str), mes_file_entry.str, name);
    sub_5826D0(NULL, CHAT_ENTRY_TYPE_LEAVE, str);
    sub_582AD0(NULL);

    if (dword_6862DC != NULL) {
        multiplayer_mm_chatroom_list_free(dword_6862DC);
    }

    if (!multiplayer_mm_chatroom_list_get(&dword_6862DC, &dword_6862E0)
        && sub_5499B0(stru_6861D8.str)) {
        dword_6862DC = NULL;
    }

    sub_582860(NULL);
}

// 0x582650
void sub_582650(const char* a1, const char* a2)
{
    sub_5826D0(a1, CHAT_ENTRY_TYPE_INCOMING_MSG, a2);
    sub_582AD0(NULL);
}

// 0x582670
void sub_582670(const char* a1, const char* a2)
{
    sub_5826D0(a1, CHAT_ENTRY_TYPE_INCOMING_EMOTE, a2);
    sub_582AD0(NULL);
}

// 0x582690
void sub_582690(const char* a1, const char* a2)
{
    sub_5826D0(a1, CHAT_ENTRY_TYPE_INCOMING_WHISPER, a2);
    sub_582AD0(NULL);
}

// 0x5826B0
void sub_5826B0(int a1, int a2)
{
    (void)a1;
    (void)a2;
}

// 0x5826C0
void sub_5826C0()
{
    sub_5830F0();
}

// 0x5826D0
bool sub_5826D0(const char* a1, int type, const char* a3)
{
    int index;

    if (stru_684708[0].str != NULL) {
        FREE(stru_684708[0].str);
    }

    for (index = 0; index < 77; index++) {
        stru_684708[index] = stru_684708[index + 1];
    }

    stru_684708[77].type = type;

    switch (type) {
    case CHAT_ENTRY_TYPE_INCOMING_MSG:
    case CHAT_ENTRY_TYPE_INCOMING_EMOTE:
    case CHAT_ENTRY_TYPE_INCOMING_WHISPER:
        strncpy(stru_684708[77].field_8, a3, 80);
        stru_684708[77].str = STRDUP(a1);
        break;
    case CHAT_ENTRY_TYPE_SYSTEM_INFO:
    case CHAT_ENTRY_TYPE_WHO_LIST:
    case CHAT_ENTRY_TYPE_JOIN:
    case CHAT_ENTRY_TYPE_LEAVE:
        strncpy(stru_684708[77].field_8, a3, 80);
        stru_684708[77].str = NULL;
        break;
    }

    return true;
}

// 0x582790
void sub_582790(TigRect* rect)
{
    TigArtBlitInfo blt;
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
void sub_582860(TigRect* rect)
{
    int index;
    TigRect text_rect;
    TigFont font_desc;
    int y;
    char str[128];

    if (rect == NULL
        || (rect->x < stru_5CBFD8.x + stru_5CBFD8.width
            && rect->y < stru_5CBFD8.y + stru_5CBFD8.height
            && stru_5CBFD8.x < rect->x + rect->width
            && stru_5CBFD8.y < rect->y + rect->height)) {
        sub_582790(&stru_5CBFD8);

        if (dword_6862DC != NULL) {
            y = 188;
            for (index = 0; index < dword_6862E0; index++) {
                tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));
                snprintf(str, sizeof(str),
                    "#%d: %s (%d) %c%c",
                    dword_6862DC[index].field_0 + 1,
                    dword_6862DC[index].name,
                    dword_6862DC[index].field_84,
                    (dword_6862DC[index].flags & 2) != 0 ? 'I' : ' ',
                    (dword_6862DC[index].flags & 1) != 0 ? '\x7F' : ' ');
                font_desc.width = 0;
                font_desc.str = str;
                tig_font_measure(&font_desc);
                text_rect.x = 37;
                text_rect.y = y;
                text_rect.width = font_desc.width;
                text_rect.height = font_desc.height;
                tig_window_text_write(sub_549820(), str, &text_rect);
                tig_font_pop();
                y += 16;
            }
        }
    }
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
        tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_BLUE));

        mes_file_entry.num = 2400;
        mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

        font.str = mes_file_entry.str;
        font.width = 0;
        tig_font_measure(&font);

        text_rect.x = (166 - font.width) / 2 + 77;
        text_rect.y = 562;
        text_rect.width = font.width;
        text_rect.height = font.height;
        tig_window_text_write(sub_549820(), mes_file_entry.str, &text_rect);

        tig_font_pop();
    }
}

// 0x582AD0
void sub_582AD0(TigRect* rect)
{
    TigRect text_rect;
    TigFont font_desc;
    int index;
    tig_font_handle_t font;
    char str[216];

    if (sub_5496D0() != 23) {
        return;
    }

    if (rect == NULL
        || rect->x < stru_5CBFE8.x + stru_5CBFE8.width
        && rect->y < stru_5CBFE8.y + stru_5CBFE8.height
        && stru_5CBFE8.x < rect->x + rect->width
        && stru_5CBFE8.y < rect->y + rect->height) {
        sub_582790(&stru_5CBFE8);

        text_rect.x = 336;
        text_rect.height = 15;

        for (index = dword_5CBF80; index < dword_5CBF80 + 26; index++) {
            switch (stru_684708[dword_5CBF80].type) {
            case CHAT_ENTRY_TYPE_INCOMING_MSG:
            case CHAT_ENTRY_TYPE_INCOMING_EMOTE:
            case CHAT_ENTRY_TYPE_INCOMING_WHISPER:
                if (!sub_582D10(stru_684708[dword_5CBF80].str)) {
                    if (sub_582D20(stru_684708[dword_5CBF80].str, sub_582D50(sub_584A80()))) {
                        switch (stru_684708[dword_5CBF80].type) {
                        case 0:
                            font = sub_583140(CHAT_ENTRY_TYPE_OUTGOING_MSG);
                            break;
                        case 1:
                            font = sub_583140(CHAT_ENTRY_TYPE_OUTGOING_EMOTE);
                            break;
                        case 6:
                            font = sub_583140(CHAT_ENTRY_TYPE_OUTGOING_WHISPER);
                            break;
                        default:
                            // Should be unreachable.
                            assert(0);
                        }
                    } else {
                        font = sub_583140(stru_684708[dword_5CBF80].type);
                    }

                    tig_font_push(font);
                    switch (stru_684708[dword_5CBF80].type) {
                    case 0:
                        sprintf(str,
                            "%s: %s",
                            sub_582D40(stru_684708[dword_5CBF80].str),
                            stru_684708[dword_5CBF80].field_8);
                        break;
                    case 1:
                        sprintf(str,
                            "* %s %s",
                            sub_582D40(stru_684708[dword_5CBF80].str),
                            stru_684708[dword_5CBF80].field_8);
                        break;
                    case 6:
                        sprintf(str,
                            "~%s~: %s",
                            sub_582D40(stru_684708[dword_5CBF80].str),
                            stru_684708[dword_5CBF80].field_8);
                        break;
                    }
                    font_desc.str = str;
                    font_desc.width = 0;
                    tig_font_measure(&font_desc);
                    text_rect.width = font_desc.width;
                    text_rect.y = 15 * (index - dword_5CBF80) + 99;
                    tig_window_text_write(sub_549820(), font_desc.str, &text_rect);
                    tig_font_pop();
                }
                break;
            case CHAT_ENTRY_TYPE_SYSTEM_INFO:
            case CHAT_ENTRY_TYPE_WHO_LIST:
            case CHAT_ENTRY_TYPE_JOIN:
            case CHAT_ENTRY_TYPE_LEAVE:
                tig_font_push(sub_583140(stru_684708[dword_5CBF80].type));
                font_desc.str = stru_684708[dword_5CBF80].field_8;
                font_desc.width = 0;
                tig_font_measure(&font_desc);
                text_rect.width = font_desc.width;
                text_rect.y = 15 * (index - dword_5CBF80) + 99;
                tig_window_text_write(sub_549820(), font_desc.str, &text_rect);
                tig_font_pop();
                break;
            }
        }
    }
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
        tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_GOLD));

        text_rect.x = 354;
        text_rect.y = 536;

        snprintf(str, sizeof(str), "%s|", byte_6861F8);
        font.str = str;
        font.width = 0;
        tig_font_measure(&font);

        text_rect.width = font.width;
        text_rect.height = font.height;
        tig_window_text_write(sub_549820(), str, &text_rect);

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
        multiplayer_mm_chatroom_list_free(dword_6862DC);
        dword_6862DC = NULL;
    }

    if (multiplayer_mm_chatroom_list_get(&dword_6862DC, &dword_6862E0)) {
        if (sub_5496D0() == 23) {
            sub_582860(NULL);
        }
    }
}

// 0x583140
tig_font_handle_t sub_583140(int type)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;
    int color;

    if (dword_6862E4[type] == TIG_FONT_HANDLE_INVALID) {
        if (mes_load("Rules\\ChatColors.mes", &mes_file)) {
            mes_file_entry.num = 100 + type;
            if (mes_search(mes_file, &mes_file_entry)) {
                mes_get_msg(mes_file, &mes_file_entry);
                color = atoi(mes_file_entry.str);
                if (color >= 0 && color < MM_COLOR_COUNT) {
                    dword_6862E4[type] = mainmenu_ui_font(MM_FONT_FLARE12, color);
                }
            }
            mes_unload(mes_file);
        }

        if (dword_6862E4[type] == TIG_FONT_HANDLE_INVALID) {
            dword_6862E4[type] = mainmenu_ui_font(MM_FONT_FLARE12, dword_5CBFA8[type]);
        }
    }

    return dword_6862E4[type];
}

// 0x583200
void mainmenu_ui_create_multiplayer_hub()
{
    ScrollbarUiControlInfo sb_create_info;
    int index;

    sub_549830(21);

    if (!mes_load("mes\\Multiplayer.mes", &dword_686528)) {
        tig_debug_printf("MultiplayerHUB: could not '%s', aborting create.\n", "mes\\Multiplayer.mes");
        exit(EXIT_SUCCESS); // FIXME: Should be `EXIT_FAILURE`.
    }

    stru_686310.num = 3001; // "Waiting for Message Of The Day."
    mes_get_msg(dword_686528, &stru_686310);

    stru_686318.num = 3002; // "Checking for Arcanum updates."
    mes_get_msg(dword_686528, &stru_686318);

    sb_create_info.scrollbar_rect = stru_5CC258;
    sb_create_info.content_rect = stru_5CC268;
    sb_create_info.flags = 0xFFFF;
    sb_create_info.field_38 = 0;
    sb_create_info.field_24 = 0;
    sb_create_info.field_28 = 0;
    sb_create_info.field_34 = 1;
    sb_create_info.field_30 = 1;
    sb_create_info.field_2C = 1;
    sb_create_info.field_40 = sub_5837A0;
    sb_create_info.field_3C = 0;

    mainmenu_ui_create_window_func(false);

    scrollbar_ui_control_create(&stru_686520, &sb_create_info, sub_549820());

    if (multiplayer_mm_is_active()) {
        if (!dword_68652C) {
            char version[36];

            dword_68652C = true;
            gamelib_copy_version(NULL, version, NULL);

            if (multiplayer_mm_version_needs_upgrade(version)) {
                tig_debug_printf("Version may need upgrade...\n");

                if (sub_5499B0(stru_686318.str)) {
                    MesFileEntry new_vesion_available_msg;
                    MesFileEntry yes_msg;
                    MesFileEntry no_msg;
                    TigWindowModalDialogInfo modal_dialog_info;
                    TigWindowModalDialogChoice choice;

                    tig_debug_printf("Version does need upgrade...\n");

                    new_vesion_available_msg.num = 3003;
                    mes_get_msg(dword_686528, &new_vesion_available_msg);

                    yes_msg.num = 1900;
                    mes_get_msg(dword_686528, &yes_msg);

                    no_msg.num = 1901;
                    mes_get_msg(dword_686528, &no_msg);

                    modal_dialog_info.type = TIG_WINDOW_MODAL_DIALOG_TYPE_OK_CANCEL;
                    modal_dialog_info.x = 237;
                    modal_dialog_info.y = 232;
                    modal_dialog_info.text = new_vesion_available_msg.str;
                    modal_dialog_info.redraw = sub_4045A0;
                    modal_dialog_info.process = NULL;
                    modal_dialog_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_OK] = yes_msg.str[0];
                    modal_dialog_info.keys[TIG_WINDOW_MODAL_DIALOG_CHOICE_CANCEL] = no_msg.str[0];
                    tig_window_modal_dialog(&modal_dialog_info, &choice);

                    if (choice == TIG_WINDOW_MODAL_DIALOG_CHOICE_OK) {
                        system("SierraUp.exe");
                        mainmenu_ui_exit_game();
                    }
                }
            }
        }

        if (byte_686320[0] == '\0' && byte_686420[0] == '\0') {
            multiplayer_mm_motd_get(byte_686320, sizeof(byte_686320), byte_686420, sizeof(byte_686420));
            sub_5499B0(stru_686310.str);
        }

        for (index = 3; index < 7; index++) {
            tig_button_show(stru_5CC278[index].button_handle);
        }
    } else {
        for (index = 3; index < 7; index++) {
            tig_button_hide(stru_5CC278[index].button_handle);
        }
        scrollbar_ui_control_hide(stru_686520);
    }

    tig_button_show(stru_5CC278[4].button_handle);
    mainmenu_ui_refresh_multiplayer_hub(NULL);
    sub_549990(dword_599494, 2);
    sub_569F30();
}

// 0x583510
void mainmenu_ui_destroy_multiplayer_hub()
{
    scrollbar_ui_control_destroy(stru_686520);
    mes_unload(dword_686528);
}

// 0x583540
void mainmenu_ui_refresh_multiplayer_hub(TigRect* rect)
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
        sub_5417A0(true);
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
            system(mes_file_entry.str);
            mes_unload(mes_file);
        }
        return false;
    case 6:
        if (mes_load("mes\\urls.mes", &mes_file)) {
            mes_file_entry.num = 2;
            mes_get_msg(mes_file, &mes_file_entry);
            system(mes_file_entry.str);
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
    uint8_t* data;
    int size;
    char* player_name;

    if (dword_5C3618 >= 0 && dword_5C3618 < dword_64C420) {
        multiplayer_unlock();
        sub_442050(&data, &size, dword_64C41C[dword_5C3618]);
        obj_field_string_get(dword_64C41C[dword_5C3618], OBJ_F_PC_PLAYER_NAME, &player_name);

        tig_net_local_client_set_name(player_name);
        tig_net_client_info_set_name(0, player_name);
        tig_net_local_server_set_name(player_name);
        FREE(player_name);

        sub_4A40F0(0,
            obj_get_id(dword_64C41C[dword_5C3618]),
            stat_level_get(dword_64C41C[dword_5C3618], STAT_LEVEL),
            data,
            size);
        FREE(data);
    }
}

// 0x5837A0
void sub_5837A0(TigRect* rect)
{
    TigArtBlitInfo blt;
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
void sub_583830(TigRect* dirty_rect)
{
    MesFileEntry mes_file_entry;
    TigFont font_desc;
    TigArtBlitInfo art_blit_info;
    TigRect rect;
    tig_art_id_t art_id;

    if (dirty_rect == NULL
        || (dirty_rect->x < stru_5CC268.x + stru_5CC268.width
            && dirty_rect->y < stru_5CC268.y + stru_5CC268.height
            && stru_5CC268.x < dirty_rect->x + dirty_rect->width
            && stru_5CC268.y < dirty_rect->y + dirty_rect->height)) {
        if (multiplayer_mm_is_active()) {
            sub_5837A0(&stru_5CC268);

            mes_file_entry.num = 2040;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

            tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_GOLD));
            font_desc.width = 0;
            font_desc.str = mes_file_entry.str;
            tig_font_measure(&font_desc);

            rect.x = (172 - font_desc.width) / 2 + 83;
            rect.y = 373;
            rect.width = font_desc.width;
            rect.height = font_desc.height;
            tig_window_text_write(sub_549820(), mes_file_entry.str, &rect);
            tig_font_pop();

            if (dirty_rect == NULL
                || (dirty_rect->x < stru_5CC258.x + stru_5CC258.width
                    && dirty_rect->y < stru_5CC258.y + stru_5CC258.height
                    && stru_5CC258.x < dirty_rect->x + dirty_rect->width
                    && stru_5CC258.y < dirty_rect->y + dirty_rect->height)) {
                sub_5806F0(stru_686520);
            }
        } else {
            tig_art_interface_id_create(760, 0, 0, 0, &art_id);

            rect.x = 0;
            rect.y = 0;
            rect.width = 333;
            rect.height = 236;

            art_blit_info.flags = 0;
            art_blit_info.art_id = art_id;
            art_blit_info.src_rect = &rect;
            art_blit_info.dst_rect = &stru_5CC268;
            tig_window_blit_art(sub_549820(), &art_blit_info);
        }
    }
}

// 0x583A00
void sub_583A00()
{
    won_account[0] = '\0';
    won_password[0] = '\0';
    won_password_confirmation[0] = '\0';
    byte_6866BC[0] = '\0';

    if (!mes_load("mes\\Multiplayer.mes", &dword_686538)) {
        tig_debug_printf("MultiplayerHUB: could not '%s', aborting create.\n", "mes\\Multiplayer.mes");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    stru_686530.num = 3000;
    mes_get_msg(dword_686538, &stru_686530);

    sub_549830(19);
    mainmenu_ui_create_window();
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
    mainmenu_ui_create_window();
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
    x += window_info->content_rect.x;
    y += window_info->content_rect.y;

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
        sub_5493C0(won_password_confirmation, 128);
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
    x += window_info->content_rect.x;
    y += window_info->content_rect.y;

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
void sub_583D90(TigRect* rect)
{
    MesFileEntry mes_file_entry;
    TigRect dst_rect;
    TigRect src_rect;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigFont font_desc;
    char str[132];
    tig_art_id_t art_id;
    size_t pos;

    (void)rect;

    tig_art_interface_id_create(755, 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = 293;
    dst_rect.y = 72;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.art_id = art_id;
    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(sub_549820(), &art_blit_info);

    tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));

    mes_file_entry.num = 2000; // "Login Name"
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);

    dst_rect.x = (165 - font_desc.width) / 2 + 327;
    dst_rect.y = 94;
    dst_rect.width = font_desc.width;
    dst_rect.height = font_desc.height;
    tig_window_text_write(sub_549820(), mes_file_entry.str, &dst_rect);

    if (sub_549520() == won_account) {
        sprintf(str, "%s|", won_account);
    } else {
        strcpy(str, won_account);
    }

    dst_rect.x = 327;
    dst_rect.y = 120;
    dst_rect.width = 165;
    dst_rect.height = 15;
    tig_window_text_write(sub_549820(), str, &dst_rect);

    mes_file_entry.num = 2001; // "Password"
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);

    dst_rect.x = (196 - font_desc.width) / 2 + 312;
    dst_rect.y = 149;
    dst_rect.width = font_desc.width;
    dst_rect.height = 15;
    tig_window_text_write(sub_549820(), mes_file_entry.str, &dst_rect);

    if (sub_549520() == won_password) {
        pos = strlen(won_password);
        str[pos] = '|';
        str[pos + 1] = '\0';
    } else {
        pos = strlen(won_password);
        str[pos] = '\0';
    }

    while (pos > 0) {
        str[pos - 1] = '*';
        pos--;
    }

    font_desc.width = 0;
    font_desc.str = str;
    tig_font_measure(&font_desc);

    dst_rect.x = 327;
    dst_rect.y = 175;
    dst_rect.width = font_desc.width;
    dst_rect.height = font_desc.height;
    tig_window_text_write(sub_549820(), str, &dst_rect);

    if (dword_68673C != 0) {
        font_desc.width = 0;
        font_desc.str = sub_584A40(dword_68673C);

        dst_rect.x = (134 - font_desc.width) / 2 + 343;
        dst_rect.y = 230;
        dst_rect.width = font_desc.width;
        dst_rect.height = font_desc.height;
        tig_window_text_write(sub_549820(), font_desc.str, &dst_rect);
    }

    mes_file_entry.num = 2005; // "New WON Account"
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);

    dst_rect.x = (164 - font_desc.width) / 2 + 327;
    dst_rect.y = 310;
    dst_rect.width = font_desc.width;
    dst_rect.height = font_desc.height;
    tig_window_text_write(sub_549820(), mes_file_entry.str, &dst_rect);

    tig_font_pop();
}

// 0x584150
void sub_584150(TigRect* rect)
{
    MesFileEntry mes_file_entry;
    TigRect dst_rect;
    TigRect src_rect;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigFont font_desc;
    char str[132];
    tig_art_id_t art_id;
    size_t pos;

    (void)rect;

    tig_art_interface_id_create(761, 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = 293;
    dst_rect.y = 72;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(sub_549820(), &art_blit_info);

    tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));

    mes_file_entry.num = 2000; // "Login Name"
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);

    dst_rect.x = (165 - font_desc.width) / 2 + 327;
    dst_rect.y = 94;
    dst_rect.width = font_desc.width;
    dst_rect.height = font_desc.height;
    tig_window_text_write(sub_549820(), mes_file_entry.str, &dst_rect);

    if (sub_549520() == won_account) {
        sprintf(str, "%s|", won_account);
    } else {
        strcpy(str, won_account);
    }

    dst_rect.x = 327;
    dst_rect.y = 120;
    dst_rect.width = 165;
    dst_rect.height = 15;
    tig_window_text_write(sub_549820(), str, &dst_rect);

    mes_file_entry.num = 2000; // "Password"
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);

    dst_rect.x = (196 - font_desc.width) / 2 + 312;
    dst_rect.y = 149;
    dst_rect.width = font_desc.width;
    dst_rect.height = 15;
    tig_window_text_write(sub_549820(), mes_file_entry.str, &dst_rect);

    if (sub_549520() == won_password) {
        pos = strlen(won_password);
        str[pos] = '|';
        str[pos + 1] = '\0';
    } else {
        pos = strlen(won_password);
        str[pos] = '\0';
    }

    while (pos > 0) {
        str[pos - 1] = '*';
        pos--;
    }

    dst_rect.x = 327;
    dst_rect.y = 175;
    dst_rect.width = 164;
    dst_rect.height = 15;
    tig_window_text_write(sub_549820(), str, &dst_rect);

    mes_file_entry.num = 2002; // "Verify Password"
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
    font_desc.width = 0;
    font_desc.str = mes_file_entry.str;
    tig_font_measure(&font_desc);

    dst_rect.x = (181 - font_desc.width) / 2 + 327;
    dst_rect.y = 204;
    dst_rect.width = font_desc.width;
    dst_rect.height = 15;
    tig_window_text_write(sub_549820(), mes_file_entry.str, &dst_rect);

    if (sub_549520() == won_password_confirmation) {
        pos = strlen(won_password_confirmation);
        str[pos] = '|';
        str[pos + 1] = '\0';
    } else {
        pos = strlen(won_password_confirmation);
        str[pos] = '\0';
    }

    while (pos > 0) {
        str[pos - 1] = '*';
        pos--;
    }

    dst_rect.x = 327;
    dst_rect.y = 230;
    dst_rect.width = 181;
    dst_rect.height = 15;
    tig_window_text_write(sub_549820(), str, &dst_rect);

    if (sub_549520() == byte_6866BC) {
        sprintf(str, "%s|", byte_6866BC);
    } else {
        strcpy(str, byte_6866BC);
    }

    dst_rect.x = 319;
    dst_rect.y = 338;
    dst_rect.width = 189;
    dst_rect.height = 62;
    tig_window_text_write(sub_549820(), str, &dst_rect);

    tig_font_pop();

    tig_font_push(mainmenu_ui_font(MM_FONT_ARIAL10, MM_COLOR_WHITE));

    switch (dword_68673C) {
    case 3:
        mes_file_entry.num = 2007; // "Passwords do not match."
        break;
    case 4:
        mes_file_entry.num = 2008; // "Cannot create account, it already exists."
        break;
    default:
        mes_file_entry.num = 2003; // "Enter your email address below..." (cont.)
        break;
    }

    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

    dst_rect.x = 319;
    dst_rect.y = 265;
    dst_rect.width = 189;
    dst_rect.height = 62;
    tig_window_text_write(sub_549820(), mes_file_entry.str, &dst_rect);

    tig_font_pop();
}

// 0x5845E0
bool sub_5845E0(int btn)
{
    MatchmakerInitInfo mm_init_info;
    MesFileEntry mes_file_entry;
    TigWindowModalDialogInfo modal_dialog_info;
    TigWindowModalDialogChoice choice;

    switch (btn) {
    case 0:
        settings_set_str_value(&settings, "won_account", won_account);
        settings_set_str_value(&settings, "won_password", won_password);
        multiplayer_mm_setup(&mm_init_info);
        if (!multiplayer_mm_is_active()) {
            if (!multiplayer_mm_init(&mm_init_info)) {
                mes_file_entry.num = 2052;
                mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

                modal_dialog_info.type = TIG_WINDOW_MODAL_DIALOG_CHOICE_OK;
                modal_dialog_info.redraw = sub_4045A0;
                modal_dialog_info.process = NULL;
                modal_dialog_info.x = 237;
                modal_dialog_info.y = 232;
                modal_dialog_info.text = mes_file_entry.str;
                tig_debug_printf("MainMenu_UI: Could not initialize Matchmaker. Aborting.\n");
                tig_window_modal_dialog(&modal_dialog_info, &choice);
                multiplayer_mm_exit();
                exit(EXIT_SUCCESS); // FIXME: Should be `EXIT_FAILURE`.
            }

            if (!sub_5499B0(stru_686530.str)) {
                mes_file_entry.num = 2052;
                mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

                modal_dialog_info.type = TIG_WINDOW_MODAL_DIALOG_CHOICE_OK;
                modal_dialog_info.redraw = sub_4045A0;
                modal_dialog_info.process = NULL;
                modal_dialog_info.x = 237;
                modal_dialog_info.y = 232;
                modal_dialog_info.text = mes_file_entry.str;
                tig_debug_printf("MainMenu_UI: Could not initialize Matchmaker. Aborting.\n");
                tig_window_modal_dialog(&modal_dialog_info, &choice);
                multiplayer_mm_exit();
                return false;
            }
        }
        sub_583D90(NULL);
        if (!multiplayer_mm_login(sub_584A80(), sub_584A90())
            || !sub_5499B0(stru_686530.str)) {
            dword_68673C = 1;
            sub_583D90(NULL);
            return false;
        }
        sub_5417A0(false);
        mainmenu_ui_create_multiplayer_hub();
        if (!sub_541680()) {
            return false;
        }
        sub_541810(sub_5496D0());
        return false;
    case 1:
        sub_5417A0(true);
        return false;
    default:
        tig_debug_printf("MainMenuUI: logon won execute unknown button Idx %d.\n", btn);
        return false;
    }
}

// 0x5847D0
bool sub_5847D0(int btn)
{
    MatchmakerInitInfo mm_init_info;
    MesFileEntry mes_file_entry;
    TigWindowModalDialogInfo modal_dialog_info;
    TigWindowModalDialogChoice choice;

    switch (btn) {
    case 0:
        settings_set_str_value(&settings, "won_account", won_account);
        settings_set_str_value(&settings, "won_password", won_password);
        multiplayer_mm_setup(&mm_init_info);
        if (!multiplayer_mm_is_active()) {
            if (!multiplayer_mm_init(&mm_init_info)) {
                mes_file_entry.num = 2052;
                mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

                modal_dialog_info.type = TIG_WINDOW_MODAL_DIALOG_CHOICE_OK;
                modal_dialog_info.redraw = sub_4045A0;
                modal_dialog_info.process = NULL;
                modal_dialog_info.x = 237;
                modal_dialog_info.y = 232;
                modal_dialog_info.text = mes_file_entry.str;
                tig_debug_printf("MainMenu_UI: Could not initialize Matchmaker. Aborting.\n");
                tig_window_modal_dialog(&modal_dialog_info, &choice);
                multiplayer_mm_exit();
                sub_584150(NULL);
                return false;
            }

            if (!sub_5499B0(stru_686530.str)) {
                mes_file_entry.num = 2052;
                mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);

                modal_dialog_info.type = TIG_WINDOW_MODAL_DIALOG_CHOICE_OK;
                modal_dialog_info.redraw = sub_4045A0;
                modal_dialog_info.process = NULL;
                modal_dialog_info.x = 237;
                modal_dialog_info.y = 232;
                modal_dialog_info.text = mes_file_entry.str;
                tig_debug_printf("MainMenu_UI: Could not initialize Matchmaker. Aborting.\n");
                tig_window_modal_dialog(&modal_dialog_info, &choice);
                multiplayer_mm_exit();
                sub_584150(NULL);
                return false;
            }
        }
        if (strcmp(won_password, won_password_confirmation) != 0) {
            dword_68673C = 3;
            sub_584150(NULL);
            return false;
        }
        if (!multiplayer_mm_create_account(won_account, won_password, byte_6866BC)
            || !sub_5499B0(stru_686530.str)) {
            dword_68673C = 4;
            sub_584150(NULL);
            return false;
        }
        if (!multiplayer_mm_login(won_account, won_password)
            || !sub_5499B0(stru_686530.str)) {
            dword_68673C = 1;
            sub_584150(NULL);
            return false;
        }
        return true;
    case 1:
        return true;
    default:
        tig_debug_printf("MainMenuUI: new won execute unknown button Idx %d.\n", btn);
        sub_584150(NULL);
        return false;
    }
}

// 0x584A40
const char* sub_584A40(int value)
{
    MesFileEntry mes_file_entry;

    if (value == 0) {
        return NULL;
    }

    mes_file_entry.num = value + 2010;
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
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
    mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
    strncpy(byte_6868F8, mes_file_entry.str, sizeof(byte_6868F8));

    tig_net_local_server_clear_password();
    gamelib_modlist_create(&stru_686740, 1);
    dword_686964 = 0;

    tig_net_local_server_get_name(name, sizeof(name));
    mainmenu_ui_create_window_func(false);
    if (multiplayer_mm_is_active()) {
        tig_button_show(stru_5CC6C8[2].button_handle);
    } else {
        tig_button_hide(stru_5CC6C8[2].button_handle);
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
    stru_686740.paths = NULL;
    dword_686964 = 0;
}

// 0x584C30
void sub_584C30(TigRect* rect)
{
    TigArtBlitInfo blt;
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
void sub_584CB0(TigRect* rect)
{
    int index;
    TigRect text_rect;
    TigFont font_desc;
    MesFileEntry mes_file_entry;
    char* copy;
    size_t pos;

    if (sub_549820() == -1) {
        return;
    }

    sub_584C30(rect);

    tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_WHITE));
    for (index = 0; index < 15; index++) {
        if (stru_5CC5F8[index].num == -1) {
            break;
        }

        text_rect.x = stru_5CC5F8[index].x;
        text_rect.y = stru_5CC5F8[index].y;
        text_rect.width = 204;
        text_rect.height = 15;

        if (rect == NULL
            || (rect->x < text_rect.x + text_rect.width
                && rect->y < text_rect.y + text_rect.height
                && text_rect.x < rect->x + rect->width
                && text_rect.y < rect->y + rect->height)) {
            sub_584C30(&text_rect);

            mes_file_entry.num = stru_5CC5F8[index].num;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            copy = STRDUP(mes_file_entry.str);

            font_desc.width = 0;
            font_desc.str = copy;
            tig_font_measure(&font_desc);

            while (font_desc.width > 204) {
                pos = strlen(copy);
                copy[pos - 1] = '\0';
                font_desc.width = 0;
                font_desc.str = copy;
                tig_font_measure(&font_desc);
                pos--;
            }

            text_rect.x = stru_5CC5F8[index].x + (204 - font_desc.width) / 2;
            text_rect.y = stru_5CC5F8[index].y;
            text_rect.width = font_desc.width;
            text_rect.height = font_desc.height;
            tig_window_text_write(sub_549820(), copy, &text_rect);
            FREE(copy);
        }
    }
    tig_font_pop();

    tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_RED));
    for (index = 0; index < 15; index++) {
        text_rect.x = stru_5CC5F8[index].x;
        text_rect.y = stru_5CC5F8[index].y + 27;
        text_rect.width = 204;
        text_rect.height = 15;

        if (rect == NULL
            || (rect->x < text_rect.x + text_rect.width
                && rect->y < text_rect.y + text_rect.height
                && text_rect.x < rect->x + rect->width
                && text_rect.y < rect->y + rect->height)) {
            sub_584C30(&text_rect);

            copy = STRDUP(sub_585630(stru_5CC5F8[index].num));

            font_desc.width = 0;
            font_desc.str = copy;
            tig_font_measure(&font_desc);

            while (font_desc.width > 172) {
                pos = strlen(copy);
                copy[pos - 1] = '\0';
                font_desc.width = 0;
                font_desc.str = copy;
                tig_font_measure(&font_desc);
                pos--;
            }

            text_rect.x = stru_5CC5F8[index].x + (204 - font_desc.width) / 2;
            text_rect.y = stru_5CC5F8[index].y + 27;
            text_rect.width = font_desc.width;
            text_rect.height = font_desc.height;
            tig_window_text_write(sub_549820(), copy, &text_rect);
            FREE(copy);
        }
    }
    tig_font_pop();

    text_rect.x = 530;
    text_rect.y = 522;
    text_rect.width = 163;
    text_rect.height = 15;

    if (rect == NULL
        || (rect->x < text_rect.x + text_rect.width
            && rect->y < text_rect.y + text_rect.height
            && text_rect.x < rect->x + rect->width
            && text_rect.y < rect->y + rect->height)) {
        sub_584C30(&text_rect);
        tig_font_push(mainmenu_ui_font(MM_FONT_FLARE12, MM_COLOR_GREEN));
        font_desc.width = 0;
        font_desc.str = byte_6868F8;
        tig_font_measure(&font_desc);
        text_rect.x = (163 - font_desc.width) / 2 + 530;
        text_rect.y = 522;
        text_rect.width = font_desc.width;
        text_rect.height = font_desc.height;
        tig_window_text_write(sub_549820(), font_desc.str, &text_rect);
        tig_font_pop();
    }

    text_rect.x = 0;
    text_rect.y = 0;
    text_rect.width = 320;
    text_rect.height = 180;

    if (rect == NULL
        || (rect->x < text_rect.x + text_rect.width
            && rect->y < text_rect.y + text_rect.height
            && text_rect.x < rect->x + rect->width
            && text_rect.y < rect->y + rect->height)) {
        sub_584C30(&text_rect);
        sub_549960();
    }
}

// 0x5850C0
void sub_5850C0(int x, int y)
{
    int index;
    int level;

    if (dword_686960) {
        sub_585A20();
    }

    index = 0;
    while (stru_5CC5F8[index].num != -1) {
        if (x >= stru_5CC5F8[index].x && x <= stru_5CC5F8[index].x + 204
            && y >= stru_5CC5F8[index].y && y <= stru_5CC5F8[index].y + 42) {
            break;
        }
        index++;
    }

    switch (stru_5CC5F8[index].num) {
    case 2300:
        dword_5CC6AC = 2300;
        tig_net_local_server_get_name(byte_686948, sizeof(byte_686948) - 1);
        sub_585970(byte_686948);
        break;
    case 2303:
        dword_5CC6AC = 2303;
        itoa(tig_net_local_server_get_max_players(), byte_686948, 10);
        sub_585970(byte_686948);
        break;
    case 2305:
        dword_5CC6AC = 2305;
        tig_net_local_server_get_level_range(&level, NULL);
        itoa(level, byte_686948, 10);
        sub_585970(byte_686948);
        break;
    case 2306:
        dword_5CC6AC = 2306;
        tig_net_local_server_get_level_range(NULL, &level);
        itoa(level, byte_686948, 10);
        sub_585970(byte_686948);
        break;
    case 2307:
        dword_5CC6AC = 2307;
        byte_686948[0] = '\0';
        sub_585970(byte_686948);
        break;
    case 2309:
        dword_5CC6AC = 2309;
        itoa(sub_4A55F0(), byte_686948, 10);
        sub_585970(byte_686948);
        break;
    }

    sub_584CB0(NULL);
    return;
}

// 0x585270
bool sub_585270(int num)
{
    TigRect rect;
    MesFileEntry mes_file_entry;
    char name[80];
    char tmp[130];
    struct sockaddr_in addr;
    int server_type;

    rect.x = 0;
    rect.y = 0;
    rect.width = 800;
    rect.height = 600;

    if (dword_5CC6AC != -1) {
        sub_585A20();
    }

    switch (num) {
    case 0:
        sub_5417A0(false);
        mainmenu_ui_create_multiplayer_hub();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        return false;
    case 1:
        sub_5417A0(false);
        sub_585D50();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        return false;
    case 2:
        sub_5417A0(false);
        multiplayer_hub_ui_init();
        if (sub_541680()) {
            sub_541810(sub_5496D0());
        }
        return false;
    case 3:
        rect.width = 0;
        if (stru_686740.count != 0) {
            sub_4A4270();
            multiplayer_lock();
            sub_4A3D00(1);
            multiplayer_unlock();
            sub_541710();
            sub_4A4280();
            sub_49CC50();
            if (sub_49CC70(stru_686740.paths[dword_686964], 0)) {
                if (multiplayer_mm_is_active()) {
                    tig_net_local_server_get_name(name, sizeof(name));
                    sub_52B0C0(&addr);
                    tig_net_inet_string(addr.sin_addr.S_un.S_addr, tmp);
                    sprintf(&(tmp[sizeof(tmp)]), ":%d", 31435);
                    // multiplayer_mm_register();
                    sub_5499B0(stru_6867A0.str);
                }
                sub_5412D0();
            } else {
                sub_5280F0();
                tig_net_start_client();
                sub_549B30();
            }
            sub_584CB0(&rect);
        } else {
            mes_file_entry.num = 2332;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            strcpy(byte_6868F8, mes_file_entry.str);
        }
        return false;
    case 4:
        if (stru_686740.count != 0) {
            dword_686964--;
            if (dword_686964 < 0) {
                dword_686964 = stru_686740.count - 1;
            }
        }
        sub_584CB0(&rect);
        return false;
    case 5:
        if (stru_686740.count != 0) {
            dword_686964 = (dword_686964 + 1) % stru_686740.count;
        }
        sub_584CB0(&rect);
        return false;
    case 6:
        tig_net_local_server_type_get(&server_type);
        server_type--;
        if (server_type < 0) {
            server_type = TIG_NET_SERVER_TYPE_ROLEPLAY;
        }
        tig_net_local_bookmark_set_type(server_type);
        sub_584CB0(&rect);
        return false;
    case 7:
        tig_net_local_server_type_get(&server_type);
        server_type = (server_type + 1) % 3;
        tig_net_local_bookmark_set_type(server_type);
        sub_584CB0(&rect);
        return false;
    case 8:
    case 9:
        if ((tig_net_local_server_get_options() & TIG_NET_SERVER_PRIVATE_CHAT) != 0) {
            tig_net_local_server_disable_option(TIG_NET_SERVER_PRIVATE_CHAT);
        } else {
            tig_net_local_server_enable_option(TIG_NET_SERVER_PRIVATE_CHAT);
        }
        sub_584CB0(&rect);
        return false;
    case 10:
    case 11:
        if ((tig_net_local_server_get_options() & TIG_NET_SERVER_PLAYER_KILLING) != 0) {
            tig_net_local_server_disable_option(TIG_NET_SERVER_PLAYER_KILLING);
        } else {
            tig_net_local_server_enable_option(TIG_NET_SERVER_PLAYER_KILLING);
        }
        sub_584CB0(&rect);
        return false;
    case 12:
    case 13:
        if ((tig_net_local_server_get_options() & TIG_NET_SERVER_FRIENDLY_FIRE) != 0) {
            tig_net_local_server_disable_option(TIG_NET_SERVER_FRIENDLY_FIRE);
        } else {
            tig_net_local_server_enable_option(TIG_NET_SERVER_FRIENDLY_FIRE);
        }
        sub_584CB0(&rect);
        return false;
    case 14:
    case 15:
        if ((tig_net_local_server_get_options() & TIG_NET_SERVER_AUTO_EQUIP) != 0) {
            tig_net_local_server_disable_option(TIG_NET_SERVER_AUTO_EQUIP);
        } else {
            tig_net_local_server_enable_option(TIG_NET_SERVER_AUTO_EQUIP);
        }
        sub_584CB0(&rect);
        return false;
    case 16:
    case 17:
        if (tig_net_auto_join_is_enabled()) {
            tig_net_auto_join_disable();
        } else {
            tig_net_auto_join_enable();
        }
        sub_584CB0(&rect);
        return false;
    case 18:
        if ((tig_net_local_server_get_options() & TIG_NET_SERVER_KEY_SHARING) != 0) {
            if ((tig_net_local_server_get_options() & 0x80) == 0) {
                tig_net_local_server_disable_option(TIG_NET_SERVER_KEY_SHARING);
            } else {
                tig_net_local_server_disable_option(0x80);
            }
        } else {
            tig_net_local_server_enable_option(TIG_NET_SERVER_KEY_SHARING | 0x80);
        }
        sub_584CB0(&rect);
        return false;
    case 19:
        if ((tig_net_local_server_get_options() & TIG_NET_SERVER_KEY_SHARING) != 0) {
            if ((tig_net_local_server_get_options() & 0x80) == 0) {
                tig_net_local_server_enable_option(0x80);
            } else {
                tig_net_local_server_disable_option(TIG_NET_SERVER_KEY_SHARING | 0x80);
            }
        } else {
            tig_net_local_server_enable_option(TIG_NET_SERVER_KEY_SHARING);
        }
        sub_584CB0(&rect);
        return false;
    default:
        sub_584CB0(&rect);
        return false;
    }
}

// 0x585630
const char* sub_585630(int num)
{
    MesFileEntry mes_file_entry;
    int server_type;
    int min_level;
    int max_level;
    size_t len;

    strncpy(byte_6867F8, "xXxXx", 256);

    if (num != dword_5CC6AC || num == 2307) {
        switch (num) {
        case 2300:
            tig_net_local_server_get_name(byte_6867F8, 256);
            return byte_6867F8;
        case 2301:
            if (stru_686740.count != 0 && stru_686740.paths != NULL) {
                return stru_686740.paths[dword_686964];
            }

            mes_file_entry.num = 2332;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            strcpy(byte_6867F8, mes_file_entry.str);
            return byte_6867F8;
        case 2302:
            tig_net_local_server_type_get(&server_type);
            mes_file_entry.num = 2320 + server_type;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            strcpy(byte_6867F8, mes_file_entry.str);
            return byte_6867F8;
        case 2303:
            itoa(tig_net_local_server_get_max_players(), byte_6867F8, 10);
            return byte_6867F8;
        case 2305:
            tig_net_local_server_get_level_range(&min_level, NULL);
            itoa(min_level, byte_6867F8, 10);
            return byte_6867F8;
        case 2306:
            tig_net_local_server_get_level_range(NULL, &max_level);
            itoa(max_level, byte_6867F8, 10);
            return byte_6867F8;
        case 2307:
            if (dword_5CC6AC == 2307) {
                len = strlen(byte_686948);
                byte_6867F8[len] = '|';
                byte_6867F8[len + 1] = '\0';
            } else {
                len = tig_net_local_server_get_password_length();
                byte_6867F8[len] = '\0';
            }

            while (len != 0) {
                byte_6867F8[--len] = '*';
            }
            break;
        case 2308:
            mes_file_entry.num = (tig_net_local_server_get_options() & TIG_NET_SERVER_PRIVATE_CHAT) != 0 ? 2331 : 2330;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            strcpy(byte_6867F8, mes_file_entry.str);
            return byte_6867F8;
        case 2309:
            itoa(sub_4A55F0(), byte_6867F8, 10);
            return byte_6867F8;
        case 2310:
            mes_file_entry.num = (tig_net_local_server_get_options() & TIG_NET_SERVER_PLAYER_KILLING) != 0 ? 2331 : 2330;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            strcpy(byte_6867F8, mes_file_entry.str);
            return byte_6867F8;
        case 2311:

        case 2312:
            mes_file_entry.num = tig_net_auto_join_is_enabled() ? 2331 : 2330;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            strcpy(byte_6867F8, mes_file_entry.str);
            return byte_6867F8;
        case 2313:
            mes_file_entry.num = (tig_net_local_server_get_options() & TIG_NET_SERVER_AUTO_EQUIP) != 0 ? 2331 : 2330;
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            strcpy(byte_6867F8, mes_file_entry.str);
            return byte_6867F8;
        case 2314:
            if ((tig_net_local_server_get_options() & TIG_NET_SERVER_KEY_SHARING) != 0) {
                mes_file_entry.num = (tig_net_local_server_get_options() & 0x80) != 0 ? 2334 : 2331;
            } else {
                mes_file_entry.num = 2330;
            }
            mes_get_msg(mainmenu_ui_mes_file(), &mes_file_entry);
            strcpy(byte_6867F8, mes_file_entry.str);
            return byte_6867F8;
        default:
            return byte_6867F8;
        }
    }

    sprintf(byte_6867F8, "%s|", byte_686948);
    return byte_6867F8;
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
            tig_net_local_server_get_level_range(NULL, &max_level);
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
void sub_585BA0(TextEdit* textedit)
{
    (void)textedit;

    sub_585A20();
}

// 0x585BB0
void sub_585BB0(TextEdit* textedit)
{
    (void)textedit;

    sub_584CB0(NULL);
}
