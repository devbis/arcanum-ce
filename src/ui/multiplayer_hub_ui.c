#include "ui/multiplayer_hub_ui.h"

#include "game/mes.h"
#include "ui/scrollbar_ui.h"
#include "ui/textedit_ui.h"

static void sub_582E50(TigRect* rect);
static void sub_581F80();
static void sub_581FC0(TextEdit* textedit);

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
void sub_5825B0()
{
    // TODO: Incomplete.
}

// 0x582650
void sub_582650()
{
    // TODO: Incomplete.
}

// 0x582670
void sub_582670()
{
    // TODO: Incomplete.
}

// 0x582690
void sub_582690()
{
    // TODO: Incomplete.
}

// 0x5826B0
void nullsub_38()
{
    // TODO: Incomplete.
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
void sub_582790()
{
    // TODO: Incomplete.
}

// 0x582860
void sub_582860()
{
    // TODO: Incomplete.
}

// 0x5829D0
void sub_5829D0()
{
    // TODO: Incomplete.
}

// 0x582AD0
void sub_582AD0()
{
    // TODO: Incomplete.
}

// 0x582D10
void sub_582D10()
{
    // TODO: Incomplete.
}

// 0x582D20
void sub_582D20()
{
    // TODO: Incomplete.
}

// 0x582D40
void sub_582D40()
{
    // TODO: Incomplete.
}

// 0x582D50
void sub_582D50()
{
    // TODO: Incomplete.
}

// 0x582D60
void sub_582D60()
{
    // TODO: Incomplete.
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
    // TODO: Incomplete.
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
    // TODO: Incomplete.
}

// 0x583540
void sub_583540()
{
    // TODO: Incomplete.
}

// 0x583560
void sub_583560()
{
    // TODO: Incomplete.
}

// 0x5836A0
void sub_5836A0()
{
    // TODO: Incomplete.
}

// 0x5837A0
void sub_5837A0()
{
    // TODO: Incomplete.
}

// 0x583830
void sub_583830()
{
    // TODO: Incomplete.
}

// 0x583A00
void sub_583A00()
{
    // TODO: Incomplete.
}

// 0x583A80
void sub_583A80()
{
    // TODO: Incomplete.
}

// 0x583A90
void sub_583A90()
{
    // TODO: Incomplete.
}

// 0x583B10
void sub_583B10()
{
    // TODO: Incomplete.
}

// 0x583B20
void sub_583B20()
{
    // TODO: Incomplete.
}

// 0x583C80
void sub_583C80()
{
    // TODO: Incomplete.
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
void sub_584A40()
{
    // TODO: Incomplete.
}

// 0x584A80
void sub_584A80()
{
    // TODO: Incomplete.
}

// 0x584A90
void sub_584A90()
{
    // TODO: Incomplete.
}

// 0x584AA0
void sub_584AA0()
{
    // TODO: Incomplete.
}

// 0x584AC0
void sub_584AC0()
{
    // TODO: Incomplete.
}

// 0x584AE0
void sub_584AE0()
{
    // TODO: Incomplete.
}

// 0x584C00
void sub_584C00()
{
    // TODO: Incomplete.
}

// 0x584C30
void sub_584C30()
{
    // TODO: Incomplete.
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
void sub_585970()
{
    // TODO: Incomplete.
}

// 0x585A20
void sub_585A20()
{
    // TODO: Incomplete.
}

// 0x585BA0
void sub_585BA0()
{
    // TODO: Incomplete.
}

// 0x585BB0
void sub_585BB0()
{
    // TODO: Incomplete.
}
