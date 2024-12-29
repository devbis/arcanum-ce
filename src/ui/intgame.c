#include "ui/intgame.h"

#include "game/gamelib.h"
#include "game/item.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/timeevent.h"
#include "ui/compact_ui.h"
#include "ui/gameuilib.h"
#include "ui/hotkey_ui.h"
#include "ui/roller_ui.h"
#include "ui/textedit_ui.h"

typedef struct IntgameIsoWindowTypeInfo {
    /* 0000 */ TigRect rect;
    /* 0010 */ tig_window_handle_t window_handle;
} IntgameIsoWindowTypeInfo;

static_assert(sizeof(IntgameIsoWindowTypeInfo) == 0x14, "wrong size");

typedef struct S64C540 {
    /* 0000 */ int field_0;
    /* 0004 */ void* field_4;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
} S64C540;

static_assert(sizeof(S64C540) == 0x18, "wrong size");

static bool sub_54AB20(UiButtonInfo* button_info, unsigned int flags);
static bool sub_54ABD0(UiButtonInfo* button_info, int width, int height);
static void intgame_ammo_icon_refresh(tig_art_id_t art_id);
static void sub_54DBF0(int btn, int window_type);
static void sub_5509C0(const char* str, TigRect* rect);
static void sub_550A10(tig_window_handle_t window_handle, const char* str, TigRect* rect, tig_font_handle_t font, unsigned int flags);
static void sub_550C60(int group);
static void sub_550CD0(int group);
static bool sub_550D20();
static void sub_550D60();
static void iso_interface_window_enable(int window_type);
static void sub_551660();
static int sub_551740(int x, int y);
static void sub_5517F0();
static void sub_5518C0(int x, int y);
static void sub_551A10(int64_t obj);
static void sub_551F20();
static void sub_551F40();
static void sub_5520D0(int window_type, int a2);
static void sub_552130(int window_type);
static void sub_552740(int64_t obj, int a2);
static void sub_5528E0();
static void sub_552930();
static void sub_552960(bool play_sound);
static void sub_553960();
static void sub_554560(tig_window_handle_t window_handle, int art_num);
static void sub_554B00(tig_window_handle_t window_handle, int art_num, int x, int y);
static void sub_555780(char* buffer, int num, int min, int max, int a5, bool a6);
static void sub_555910(int64_t obj, char* buffer);
static void sub_555B50(int64_t obj, char* buffer);
static void sub_555D80(int64_t a1, int64_t a2, const char* str);
static void sub_556B90(int a1);
static void sub_556EA0(int64_t item_obj);
static void intgame_mt_button_enable();
static void intgame_mt_button_disable();
static bool intgame_big_window_create();
static void intgame_big_window_destroy();
static bool intgame_big_window_message_filter(TigMessage* msg);
static void sub_557AC0(int group, int index, UiButtonInfo* button_info);

// 0x5C6378
static tig_window_handle_t dword_5C6378[5] = {
    TIG_WINDOW_HANDLE_INVALID,
    TIG_WINDOW_HANDLE_INVALID,
    TIG_WINDOW_HANDLE_INVALID,
    TIG_WINDOW_HANDLE_INVALID,
    TIG_WINDOW_HANDLE_INVALID,
};

// 0x5C6390
static TigRect stru_5C6390[2] = {
    { 0, 0, 800, 41 },
    { 0, 441, 800, 159 },
};

// 0x5C63B0
static TigRect stru_5C63B0 = { 311, 96, 178, 178 };

// 0x5C63C0
static TigRect stru_5C63C0 = { 311, 196, 178, 178 };

// 0x5C63D0
static int dword_5C63D0 = -1;

// 0x5C63D8
static TigRect stru_5C63D8 = { 14, 472, 28, 88 };

// 0x5C63E8
static TigRect stru_5C63E8 = { 754, 473, 28, 88 };

// 0x5C63F8
static IntgameIsoWindowTypeInfo stru_5C63F8[6] = {
    { { 15, 578, 29, 12 }, TIG_WINDOW_HANDLE_INVALID },
    { { 755, 578, 27, 12 }, TIG_WINDOW_HANDLE_INVALID },
    { { 190, 17, 24, 12 }, TIG_WINDOW_HANDLE_INVALID },
    { { 104, 512, 50, 20 }, TIG_WINDOW_HANDLE_INVALID },
    { { 264, 562, 50, 20 }, TIG_WINDOW_HANDLE_INVALID },
    { { 15, 500, 29, 12 }, TIG_WINDOW_HANDLE_INVALID },
};

// 0x5C6470
static TigRect stru_5C6470 = { 61, 509, 251, -1 };

// 0x5C6480
static UiButtonInfo stru_5C6480[] = {
    { 693, 456, 472, TIG_BUTTON_HANDLE_INVALID },
    { 649, 494, 473, TIG_BUTTON_HANDLE_INVALID },
    { 86, 457, 470, TIG_BUTTON_HANDLE_INVALID },
    { 693, 539, 471, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C64C0
static UiButtonInfo stru_5C64C0[] = {
    { 41, 2, 187, TIG_BUTTON_HANDLE_INVALID },
    { 4, 2, 169, TIG_BUTTON_HANDLE_INVALID },
    { 115, 2, 186, TIG_BUTTON_HANDLE_INVALID },
    { 78, 2, 193, TIG_BUTTON_HANDLE_INVALID },
    { 157, 9, 137, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6510
static int dword_5C6510[] = {
    561,
    560,
    558,
    195,
    559,
};

// 0x5C6524
static int dword_5C6524[5] = {
    169,
    187,
    193,
    194,
    186,
};

// 0x5C6538
static UiButtonInfo stru_5C6538 = { 605, 9, 137, TIG_BUTTON_HANDLE_INVALID };

// 0x5C6548
static UiButtonInfo stru_5C6548[] = {
    { 196, 492, 354, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 8, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 274, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 642, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 290, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 354, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 200, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 291, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 564, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 298, TIG_BUTTON_HANDLE_INVALID },
    { 196, 492, 842, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C65F8
static UiButtonInfo stru_5C65F8[] = {
    { 210, 545, 0, TIG_BUTTON_HANDLE_INVALID },
    { 210, 504, 0, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6618
static UiButtonInfo stru_5C6618[] = {
    { 201, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 226, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 251, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 276, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 301, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 326, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 351, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 376, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 400, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 425, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 450, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 475, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 500, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 525, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 550, 497, -1, TIG_BUTTON_HANDLE_INVALID },
    { 575, 497, -1, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6718
static UiButtonInfo stru_5C6718[80] = {
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 284, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 334, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 384, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 435, 528, -1, TIG_BUTTON_HANDLE_INVALID },
    { 485, 528, -1, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6C18
static UiButtonInfo stru_5C6C18[] = {
    { 353, 542, -1, TIG_BUTTON_HANDLE_INVALID },
    { 402, 542, -1, TIG_BUTTON_HANDLE_INVALID },
    { 451, 542, -1, TIG_BUTTON_HANDLE_INVALID },
    { 501, 542, -1, TIG_BUTTON_HANDLE_INVALID },
    { 550, 542, -1, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6C68
static UiButtonInfo stru_5C6C68[4] = {
    { 288, 514, -1, TIG_BUTTON_HANDLE_INVALID },
    { 351, 514, -1, TIG_BUTTON_HANDLE_INVALID },
    { 414, 514, -1, TIG_BUTTON_HANDLE_INVALID },
    { 477, 514, -1, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6CA8
static UiButtonInfo stru_5C6CA8[] = {
    { 396, 550, 201, TIG_BUTTON_HANDLE_INVALID },
    { 511, 550, 202, TIG_BUTTON_HANDLE_INVALID },
    { 281, 549, 203, TIG_BUTTON_HANDLE_INVALID },
    { 213, 509, 146, TIG_BUTTON_HANDLE_INVALID },
    { 213, 536, 148, TIG_BUTTON_HANDLE_INVALID },
    { 213, 563, 145, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6D08
static UiButtonInfo stru_5C6D08[] = {
    { 364, 548, 299, TIG_BUTTON_HANDLE_INVALID },
    { 479, 547, 805, TIG_BUTTON_HANDLE_INVALID },
    { 479, 559, 804, TIG_BUTTON_HANDLE_INVALID },
    { 545, 506, 33, TIG_BUTTON_HANDLE_INVALID },
    { 545, 557, 32, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6D58
static int dword_5C6D58 = -1;

// 0x5C6D60
struct IntgameIsoWindowTypeInfo stru_5C6D60[] = {
    { { 211, 503, 383, 82 }, TIG_WINDOW_HANDLE_INVALID },
    { { 208, 574, 387, 18 }, TIG_WINDOW_HANDLE_INVALID },
    { { 208, 567, 387, 18 }, TIG_WINDOW_HANDLE_INVALID },
    { { 291, 566, 268, 19 }, TIG_WINDOW_HANDLE_INVALID },
    { { 208, 574, 387, 18 }, TIG_WINDOW_HANDLE_INVALID },
    { { 211, 507, 383, 84 }, TIG_WINDOW_HANDLE_INVALID },
    { { 262, 508, 303, 24 }, TIG_WINDOW_HANDLE_INVALID },
    { { 220, 503, 350, 82 }, TIG_WINDOW_HANDLE_INVALID },
    { { 355, 506, 227, 18 }, TIG_WINDOW_HANDLE_INVALID },
    { { 0, 0, 0, 0 }, TIG_WINDOW_HANDLE_INVALID },
    { { 0, 0, 0, 0 }, TIG_WINDOW_HANDLE_INVALID },
};

// 0x5C6E40
static UiButtonInfo stru_5C6E40[] = {
    { 281, 3, 188, TIG_BUTTON_HANDLE_INVALID },
    { 331, 3, 189, TIG_BUTTON_HANDLE_INVALID },
    { 381, 3, 190, TIG_BUTTON_HANDLE_INVALID },
    { 431, 3, 191, TIG_BUTTON_HANDLE_INVALID },
    { 481, 3, 192, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6E90
static UiButtonInfo stru_5C6E90[] = {
    { 281, 3, 188, TIG_BUTTON_HANDLE_INVALID },
    { 331, 3, 189, TIG_BUTTON_HANDLE_INVALID },
    { 381, 3, 190, TIG_BUTTON_HANDLE_INVALID },
    { 431, 3, 191, TIG_BUTTON_HANDLE_INVALID },
    { 481, 3, 192, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6EE0
static TigRect stru_5C6EE0[5] = {
    { 281, 3, 32, 32 },
    { 331, 3, 32, 32 },
    { 381, 3, 32, 32 },
    { 431, 3, 32, 32 },
    { 481, 3, 32, 32 },
};

// 0x5C6F30
static UiButtonInfo stru_5C6F30 = { 616, 455, 182, TIG_BUTTON_HANDLE_INVALID };

// 0x5C6F40
static UiButtonInfo stru_5C6F40[] = {
    { 69, 548, -1, TIG_BUTTON_HANDLE_INVALID },
    { 114, 548, -1, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6F60
static int dword_5C6F60[] = {
    1,
    0,
};

// 0x5C6F68
static UiButtonInfo intgame_mt_button_info = { 161, 443, 563, TIG_BUTTON_HANDLE_INVALID };

// 0x5C6F78
static int dword_5C6F78 = 6;

// 0x5C6F80
static TigRect stru_5C6F80 = { 648, 5, 128, 30 };

// 0x5C6F90
static UiButtonInfo stru_5C6F90 = { 0, 0, -1, TIG_BUTTON_HANDLE_INVALID };

// 0x5C70C8
static TigRect stru_5C70C8 = { 290, 63, 291, 19 };

// 0x5C70D8
static TigRect stru_5C70D8 = { 290, 87, 291, 18 };

// 0x5C70E8
static TigRect stru_5C70E8 = { 290, 105, 291, 18 };

// 0x5C70F8
static TigRect stru_5C70F8 = { 290, 123, 291, 18 };

// 0x5C7108
static TigRect stru_5C7108 = { 290, 63, 291, 78 };

// 0x5C7118
static TigRect stru_5C7118 = { 290, 63, 291, 55 };

// 0x5C7128
static TigRect stru_5C7128 = { 290, 105, 291, 36 };

// 0x5C7138
static TigRect stru_5C7138 = { 290, 87, 291, 54 };

// 0x5C7148
static TigRect stru_5C7148 = { 217, 63, 364, 19 };

// 0x5C7158
static TigRect stru_5C7158 = { 217, 63, 364, 78 };

// 0x5C7168
static TigRect stru_5C7168 = { 217, 87, 364, 54 };

// 0x5C7178
static int dword_5C7178[22] = {
    -1,
    21,
    352,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    21,
    0,
    -1,
    0,
    0,
    0,
};

// 0x5C71D0
static UiButtonInfo stru_5C71D0[10] = {
    { 211, 37, 773, TIG_BUTTON_HANDLE_INVALID },
    { 249, 37, 774, TIG_BUTTON_HANDLE_INVALID },
    { 287, 37, 775, TIG_BUTTON_HANDLE_INVALID },
    { 327, 37, 776, TIG_BUTTON_HANDLE_INVALID },
    { 365, 37, 777, TIG_BUTTON_HANDLE_INVALID },
    { 403, 37, 778, TIG_BUTTON_HANDLE_INVALID },
    { 439, 37, 779, TIG_BUTTON_HANDLE_INVALID },
    { 478, 37, 780, TIG_BUTTON_HANDLE_INVALID },
    { 516, 37, 781, TIG_BUTTON_HANDLE_INVALID },
    { 555, 37, 782, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C7288
static tig_window_handle_t dword_5C7288 = TIG_WINDOW_HANDLE_INVALID;

// 0x5C728C
static int dword_5C728C[] = {
    250,
    251,
    252,
    253,
    0,
    199,
    495,
    80,
    48,
};

// 0x5C72B0
static int dword_5C72B0 = 1;

// 0x5C72B4
static int dword_5C72B4[] = {
    1,
    1,
    1,
    1,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    1,
    0,
    0,
};

// 0x5C7308
static int dword_5C7308 = -1;

// 0x5C739C
static int intgame_iso_window_width = 800;

// 0x5C73A0
static int intgame_iso_window_height = 600;

// 0x5C73A4
static tig_window_handle_t intgame_iso_window = TIG_WINDOW_HANDLE_INVALID;

// 0x64C470
static tig_font_handle_t dword_64C470;

// 0x64C474
static TigVideoBuffer* dword_64C474;

// 0x64C478
static int dword_64C478;

// 0x64C47C
static int dword_64C47C[2];

// 0x64C484
static int dword_64C484[5];

// 0x64C498
static tig_font_handle_t dword_64C498;

// 0x64C49C
static tig_font_handle_t dword_64C49C;

// 0x64C4A0
static tig_font_handle_t dword_64C4A0;

// 0x64C4A8
static UiButtonInfo stru_64C4A8[5];

// 0x64C4F8
static tig_window_handle_t dword_64C4F8[2];

// 0x64C500
static tig_font_handle_t dword_64C500;

// 0x64C504
static mes_file_handle_t intgame_mes_file;

// 0x64C508
static tig_window_handle_t intgame_big_window_handle;

// 0x64C510
static TigRect stru_64C510;

// 0x64C520
static tig_window_handle_t dword_64C520;

// 0x64C524
static TigRect* dword_64C524;

// 0x64C528
static tig_art_id_t dword_64C528;

// 0x64C52C
static tig_window_handle_t dword_64C52C;

// 0x64C530
static int dword_64C530;

// 0x64C534
static int dword_64C534;

// 0x64C538
static tig_font_handle_t dword_64C538;

// 0x64C540
static S64C540 stru_64C540[10];

// 0x64C630
static bool intgame_big_window_locked;

// 0x64C634
static int dword_64C634[11];

// 0x64C660
static TigRect stru_64C660;

// 0x64C670
static tig_font_handle_t dword_64C670;

// 0x64C674
static int dword_64C674;

// 0x64C678
static int dword_64C678;

// 0x64C67C
static bool intgame_compact_interface;

// 0x64C680
static bool intgame_fullscreen;

// 0x64C688
static int64_t qword_64C688;

// 0x64C690
static int64_t qword_64C690;

// 0x64C698
static TigRect stru_64C698;

// 0x64C6A8
static int intgame_iso_window_type;

// 0x64C6AC
static int dword_64C6AC;

// 0x64C6B0
static bool dword_64C6B0;

// 0x64C6B4
static bool dword_64C6B4;

// 0x64C6B8
static int dword_64C6B8;

// 0x64C6BC
static int dword_64C6BC;

// 0x64C6C0
static int dword_64C6C0;

// 0x64C6C4
static int dword_64C6C4;

// 0x64C6C8
static int dword_64C6C8;

// 0x64C6CC
static void(*dword_64C6CC)(TigMessage* msg);

// 0x64C6D0
static int dword_64C6D0;

// 0x64C6D4
static void(*dword_64C6D4)(John* a1);

// 0x64C6D8
static int dword_64C6D8;

// 0x64C6DC
static bool dword_64C6DC;

// 0x64C6E0
static bool dword_64C6E0;

// 0x64C6E4
static TigVideoBuffer* dword_64C6E4;

// 0x64C6F0
static unsigned int intgame_iso_window_flags;

// 0x739F88
static tig_font_handle_t dword_739F88;

// 0x549B70
bool intgame_init(GameInitInfo* init_info)
{
    TigFont font;

    (void)init_info;

    if (!mes_load("mes\\intgame.mes", &intgame_mes_file)) {
        return false;
    }

    if (!intgame_big_window_create()) {
        mes_unload(intgame_mes_file);
        return false;
    }

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create_(&font, &dword_739F88);

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(100, 100, 255);
    tig_font_create_(&font, &dword_64C470);

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 114, 0);
    tig_font_create_(&font, &dword_64C538);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create_(&font, &dword_64C498);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 0, 0);
    tig_font_create_(&font, &dword_64C49C);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(0, 0, 255);
    tig_font_create_(&font, &dword_64C500);

    font.flags = 0;
    tig_art_interface_id_create(230, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create_(&font, &dword_64C4A0);

    memcpy(dword_64C484, dword_5C6524, sizeof(dword_64C484));
    dword_64C534 = 2;
    dword_64C674 = -1;

    return true;
}

// 0x549F00
void intgame_reset()
{
    int index;

    dword_64C6D8 = 0;
    sub_553990();
    sub_57DAB0();
    intgame_clock_process_callback();
    sub_552130(0);

    for (index = 0; index < 10; index++) {
        sub_57F210(index);
    }

    dword_64C534 = 2;
    memcpy(dword_64C484, dword_5C6524, sizeof(dword_64C484));
    sub_54AA30();
}

// 0x549F60
void intgame_resize(ResizeInfo* resize_info)
{
    int index;
    TigWindowData window_data;
    TigRect rect;

    sub_57DA50();
    dword_64C52C = resize_info->iso_window_handle;

    if (dword_5C7288 != TIG_WINDOW_HANDLE_INVALID) {
        tig_window_destroy(dword_5C7288);
        dword_5C7288 = TIG_WINDOW_HANDLE_INVALID;
    }

    if (resize_info->field_14.height == 400) {
        hotkey_ui_start(dword_64C4F8[1], &(stru_5C6390[1]), dword_64C4F8[1], false);

        for (index = 0; index < 5; index++) {
            tig_window_hide(dword_5C6378[index]);
        }
    } else {
        window_data.flags = TIG_WINDOW_FLAG_0x08;
        window_data.rect.x = 196;
        window_data.rect.y = 563;
        window_data.rect.width = 411;
        window_data.rect.height = 37;
        window_data.color_key = tig_color_make(5, 5, 5);
        if (tig_window_create(&window_data, &dword_5C7288) != TIG_OK) {
            tig_debug_printf("intgame_resize: ERROR: couldn't create window!");
            tig_exit();
        }

        rect.x = 0;
        rect.y = 0;
        rect.width = window_data.rect.width;
        rect.height = window_data.rect.height;
        tig_window_fill(dword_5C7288,
            &rect,
            tig_color_make(5, 5, 5));

        hotkey_ui_start(dword_5C7288, &(window_data.rect), TIG_WINDOW_HANDLE_INVALID, true);

        for (index = 0; index < 5; index++) {
            if (sub_57C520(index)) {
                tig_window_show(dword_5C6378[index]);
            }
        }
    }
}

// 0x54A130
void intgame_exit()
{
    tig_font_destroy(dword_739F88);
    tig_font_destroy(dword_64C470);
    tig_font_destroy(dword_64C538);
    tig_font_destroy(dword_64C498);
    tig_font_destroy(dword_64C49C);
    tig_font_destroy(dword_64C500);
    tig_font_destroy(dword_64C4A0);
    intgame_big_window_destroy();
    mes_unload(intgame_mes_file);
}

// 0x54A1A0
bool intgame_save(TigFile* stream)
{
    if (stream == NULL) return false;
    if (tig_file_fwrite(&intgame_iso_window_type, sizeof(intgame_iso_window_type), 1, stream) != 1) return false;
    if (tig_file_fwrite(&dword_64C530, sizeof(dword_64C530), 1, stream) != 1) return false;
    if (!sub_57DB40(stream)) return false;
    if (tig_file_fwrite(dword_64C484, sizeof(*dword_64C484), 5, stream) != 5) return false;
    if (tig_file_fwrite(&dword_64C534, sizeof(dword_64C534), 1, stream) != 1) return false;

    return true;
}

// 0x54A220
bool intgame_load(GameLoadInfo* load_info)
{
    int v1;
    int index;
    int64_t obj;

    if (load_info->stream == NULL) return false;
    if (tig_file_fread(&v1, sizeof(v1), 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_64C530, sizeof(dword_64C530), 1, load_info->stream) != 1) return false;

    if (intgame_iso_window_type == 1) {
        tig_button_state_change(stru_5C6618[dword_64C530].button_handle, TIG_BUTTON_STATE_PRESSED);
    }

    if (!sub_57DBA0(load_info)) return false;
    if (tig_file_fread(dword_64C484, sizeof(*dword_64C484), 5, load_info->stream) != 5) return false;
    if (tig_file_fread(&dword_64C534, sizeof(dword_64C534), 1, load_info->stream) != 1) return false;

    for (index = 0; index < 5; index++) {
        sub_556B90(index);
    }

    sub_54B3C0();
    sub_553990();
    intgame_mt_button_enable();

    obj = player_get_pc_obj();
    if (obj != OBJ_HANDLE_NULL) {
        sub_556C20(obj);
        sub_54AD00(2, stat_level(obj, STAT_FATE_POINTS), 2);
    }

    return true;
}

// 0x54A330
bool iso_interface_create(tig_window_handle_t window_handle)
{
    TigWindowData window_data;
    TigVideoBufferCreateInfo vb_create_info;
    TigArtBlitInfo art_blit_info;
    int index;
    tig_art_id_t art_id;
    TigArtAnimData art_anim_data;
    TigArtFrameData art_frame_data;
    int iwid;
    TigFont font_desc;

    stru_64C510 = stru_5C63B0;
    dword_64C52C = window_handle;
    dword_64C6B0 = 1;
    dword_64C6B4 = false;

    tig_window_data(window_handle, &window_data);

    vb_create_info.flags = 0;
    vb_create_info.width = stru_5C63D8.width / 2;
    vb_create_info.height = stru_5C63D8.height;
    vb_create_info.background_color = 0;
    if (tig_video_buffer_create(&vb_create_info, &dword_64C474) != TIG_OK) {
        tig_debug_printf("iso_interface_create: ERROR: couldn't create video buffer!\n");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE
    }

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.message_filter = sub_54C8E0;

    art_blit_info.src_rect = &(window_data.rect);
    art_blit_info.dst_rect = &(window_data.rect);

    for (index = 0; index < 2; index++) {
        if (index == 0) {
            tig_art_interface_id_create(185u, 0, 0, 0, &art_id);
        } else {
            tig_art_interface_id_create(184u, 0, 0, 0, &art_id);
        }

        if (tig_art_anim_data(art_id, &art_anim_data) != TIG_OK) {
            tig_debug_printf("iso_interface_create: ERROR: couldn't grab art anim data!\n");
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE
        }

        window_data.background_color = art_anim_data.color_key;
        window_data.rect = stru_5C6390[index];
        art_blit_info.art_id = art_id;

        if (tig_window_create(&window_data, &(dword_64C4F8[index])) != TIG_OK) {
            tig_debug_printf("iso_interface_create: ERROR: couldn't create window!\n");
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE
        }

        window_data.rect.x = 0;
        window_data.rect.y = 0;
        tig_window_blit_art(dword_64C4F8[index], &art_blit_info);
    }

    sub_569F20(dword_64C4F8[0]);

    for (index = 0; index < 5; index++) {
        sub_54AAE0(&(stru_5C6E40[index]));
        tig_button_hide(stru_5C6E40[index].button_handle);
    }

    for (index = 0; index < 4; index++) {
        if (index == 0 || index == 1) {
            sub_54AB20(&(stru_5C6480[index]), 0x2);
        } else {
            sub_54AAE0(&(stru_5C6480[index]));
        }
    }

    for (index = 0; index < 5; index++) {
        sub_54AAE0(&(stru_5C64C0[index]));
    }

    sub_54AAE0(&stru_5C6538);
    sub_5501C0();

    dword_64C6B4 = true;
    dword_64C530 = 0;

    for (index = 0; index < 11; index++) {
        iwid = sub_551740(stru_5C6D60[index].rect.x, stru_5C6D60[index].rect.y);
        if (iwid == -1) {
            tig_debug_printf("iso_interface_create: ERROR: find iwid match!\n");
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE
        }

        stru_5C6D60[index].rect.x -= stru_5C6390[iwid].x;
        stru_5C6D60[index].rect.y -= stru_5C6390[iwid].y;
        stru_5C6D60[index].window_handle = dword_64C4F8[iwid];
    }

    for (index = 0; index < 6; index++) {
        iwid = sub_551740(stru_5C63F8[index].rect.x, stru_5C63F8[index].rect.y);
        if (iwid == -1) {
            tig_debug_printf("iso_interface_create: ERROR: find iwid match!\n");
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE
        }

        stru_5C63F8[index].rect.x -= stru_5C6390[iwid].x;
        stru_5C63F8[index].rect.y -= stru_5C6390[iwid].y;
        stru_5C63F8[index].window_handle = dword_64C4F8[iwid];
    }

    hotkey_ui_start(dword_64C4F8[1], &(stru_5C6390[1]), dword_64C4F8[1], false);
    sub_54AAE0(&stru_5C6F30);
    sub_54AAE0(&intgame_mt_button_info);
    intgame_mt_button_disable();

    dword_64C6B8 = 0;
    dword_64C634[0] = 0;
    sub_4F25B0(4, 0x8200);
    dword_64C6BC = 0;

    font_desc.str = NULL;
    sub_535390(&font_desc);
    tig_art_interface_id_create(171, 0, 0, 0, &(font_desc.art_id));
    tig_font_create(&font_desc, &dword_64C670);

    stru_5C6F90.x = stru_5C6F80.x;
    stru_5C6F90.y = stru_5C6F80.y;
    sub_54ABD0(&stru_5C6F90, stru_5C6F80.width, stru_5C6F80.height);

    if (tig_art_interface_id_create(207, 0, 0, 0, &art_id) != TIG_OK
        || tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        tig_debug_printf("iso_interface_create: ERROR: clock stuff failed!\n");
        exit(EXIT_FAILURE);
    }
    dword_64C47C[0] = art_frame_data.width;

    if (tig_art_interface_id_create(208, 0, 0, 0, &art_id) != TIG_OK
        || tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        tig_debug_printf("iso_interface_create: ERROR: clock stuff failed!\n");
        exit(EXIT_FAILURE);
    }
    dword_64C47C[1] = art_frame_data.width;

    intgame_clock_process_callback();
    sub_54AEE0(0);
    sub_54AEE0(1);

    // NOTE: Looks meaningless.
    font_desc.str = NULL;
    sub_535390(&font_desc);
    font_desc.flags = ~TIG_FONT_SHADOW;
    tig_art_interface_id_create(230, 0, 0, 0, &(font_desc.art_id));

    sub_54B3C0();

    for (index = 0; index < 10; index++) {
        stru_64C540[index].field_4 = MALLOC(200);
    }

    dword_5C63D0 = sub_551740(stru_5C6548[8].x, stru_5C6548[8].y);
    if (dword_5C63D0 == -1) {
        tig_debug_printf("Intgame: ERROR: Couldn't match magic-tech window!\n");
        exit(EXIT_FAILURE);
    }

    if (intgame_is_compact_interface()) {
        for (index = 0; index < 2; index++) {
            tig_window_hide(dword_64C4F8[index]);
        }
    }

    for (index = 0; index < 5; index++) {
        window_data.flags = TIG_WINDOW_FLAG_0x08;
        window_data.rect = stru_5C6EE0[index];
        window_data.color_key = tig_color_make(5, 5, 5);
        if (tig_window_create(&window_data, &(dword_5C6378[index])) != TIG_OK) {
            tig_debug_printf("intgame_resize: ERROR: Couldn't create spellFSWid: %d!\n", index);
            tig_exit();
            return;
        }

        tig_window_fill(dword_5C6378[index],
            &(stru_5C6EE0[index]),
            tig_color_make(0, 0, 0));
    }

    for (index = 0; index < 5; index++) {
        sub_54AA60(dword_5C6378[index],
            &(stru_5C6EE0[index]),
            &(stru_5C6E90[index]),
            true);
        tig_button_hide(stru_5C6E90[index].button_handle);
    }
}

// 0x54A9A0
void iso_interface_destroy()
{
    int index;

    if (dword_64C6B4) {
        for (index = 0; index < 2; index++) {
            tig_window_destroy(dword_64C4F8[index]);
        }

        tig_font_destroy(dword_64C670);

        for (index = 0; index < 10; index++) {
            FREE(stru_64C540[index].field_4);
        }

        for (index = 0; index < 5; index++) {
            if (dword_5C6378[index] != TIG_WINDOW_HANDLE_INVALID) {
                tig_window_destroy(dword_5C6378[index]);
                dword_5C6378[index] = TIG_WINDOW_HANDLE_INVALID;
            }
        }
    }

    tig_video_buffer_destroy(dword_64C474);
}

// 0x54AA30
void sub_54AA30()
{
    dword_64C6B8 = 0;
    dword_64C634[0] = 0;
    dword_64C6C0 = 0;
    dword_64C6C4 = 0;
    dword_64C6C8 = 0;
}

// 0x54AA60
bool sub_54AA60(tig_window_handle_t window_handle, TigRect* rect, UiButtonInfo* button_info, unsigned int flags)
{
    TigButtonData button_data;

    button_data.flags = flags;
    button_data.window_handle = window_handle;
    button_data.x = button_info->x - rect->x;
    button_data.y = button_info->y - rect->y;
    tig_art_interface_id_create(button_info->art_num, 0, 0, 0, &(button_data.art_id));
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_exit_snd_id = TIG_SOUND_HANDLE_INVALID;
    return tig_button_create(&button_data, &(button_info->button_handle)) == TIG_OK;
}

// 0x54AAE0
bool sub_54AAE0(UiButtonInfo* button_info)
{
    int index;

    index = sub_551740(button_info->x, button_info->y);
    if (index == -1) {
        return false;
    }

    return sub_54AA60(dword_64C4F8[index], &(stru_5C6390[index]), button_info, TIG_BUTTON_FLAG_0x01);
}

// 0x54AB20
bool sub_54AB20(UiButtonInfo* button_info, unsigned int flags)
{
    int index;

    index = sub_551740(button_info->x, button_info->y);
    if (index == -1) {
        return false;
    }

    return sub_54AA60(dword_64C4F8[index], &(stru_5C6390[index]), button_info, flags);
}

// 0x54ABD0
bool sub_54ABD0(UiButtonInfo* button_info, int width, int height)
{
    int index;
    TigButtonData button_data;

    index = sub_551740(button_info->x, button_info->y);
    if (index == -1) {
        return false;
    }

    button_data.window_handle = dword_64C4F8[index];
    button_data.x = button_info->x - stru_5C6390[index].x;
    button_data.y = button_info->y - stru_5C6390[index].y;
    button_data.width = width;
    button_data.height = height;
    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.art_id = TIG_ART_ID_INVALID;
    button_data.mouse_down_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_up_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_enter_snd_id = TIG_SOUND_HANDLE_INVALID;
    button_data.mouse_exit_snd_id = TIG_SOUND_HANDLE_INVALID;
    return tig_button_create(&button_data, &(button_info->button_handle)) == TIG_OK;
}

// 0x54AC70
void sub_54AC70(UiButtonInfo* button_info)
{
    tig_button_destroy(button_info->button_handle);
    button_info->button_handle = TIG_BUTTON_HANDLE_INVALID;
}

// 0x54AD00
void sub_54AD00()
{
    // TODO: Incomplete.
}

// 0x54AEE0
void sub_54AEE0(int a1)
{
    switch (a1) {
    case 0:
        sub_54AF10(&stru_5C63D8);
        break;
    case 1:
        sub_54AF10(&stru_5C63E8);
        break;
    }
}

// 0x54AF10
void sub_54AF10()
{
    // TODO: Incomplete.
}

// 0x54B3A0
void sub_54B3A0()
{
    sub_54AEE0(0);
    sub_54AEE0(1);
}

// 0x54B3C0
void sub_54B3C0()
{
    int qty;
    int art_num;
    int64_t pc_obj;
    int64_t item_obj;
    int ammo_type;
    tig_art_id_t art_id;
    int mana;

    qty = 0;
    art_num = 474;
    pc_obj = player_get_pc_obj();

    if (pc_obj != OBJ_HANDLE_NULL) {
        item_obj = item_wield_get(pc_obj, 1004);
        if (item_obj != OBJ_HANDLE_NULL) {
            ammo_type = item_weapon_ammo_type(item_obj);
            if (ammo_type != 10000) {
                qty = item_ammo_quantity_get(pc_obj, ammo_type);
                art_num = dword_5C728C[ammo_type];
            } else {
                qty = obj_field_int32_get(item_obj, OBJ_F_ITEM_MANA_STORE);
                art_num = 469;
            }
        }

        if (qty <= 0) {
            qty = item_gold_get(pc_obj);
        }
    }

    sub_54AD00(3, qty, 6);
    tig_art_interface_id_create(art_num, 0, 0, 0, &art_id);
    intgame_ammo_icon_refresh(art_id);

    if (qword_64C688 != OBJ_HANDLE_NULL
        && intgame_iso_window_type == 8) {
        if ((obj_field_in32_get(qword_64C688, OBJ_F_ITEM_FLAGS) & OIF_IDENTIFIED) != 0) {
            mana = obj_field_int32_get(qword_64C688, OBJ_F_ITEM_SPELL_MANA_STORE);
            if (mana >= 0) {
                sub_54AD00(4, mana, 3);
            } else {
                sub_54AD00(4, -1, 3);
            }
        } else {
            sub_54AD00(4, -2, 3);
        }
    }
}

// 0x54B500
void intgame_ammo_icon_refresh(tig_art_id_t art_id)
{
    int index;
    TigRect src_rect;
    TigRect dst_rect;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo blt;

    index = sub_551740(stru_5C6470.x, stru_5C6470.y);
    if (index == -1) {
        tig_debug_printf("intgame_ammo_icon_refresh: ERROR: couldn't find iwid match!\n");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE.
    }

    blt.art_id = art_id;
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = stru_5C6470.x - stru_5C6390[index].x;
    dst_rect.y = stru_5C6470.y - stru_5C6390[index].y;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    blt.flags = 0;
    blt.src_rect = &src_rect;
    blt.dst_rect = &dst_rect;
    tig_window_blit_art(dword_64C4F8[index], &blt);
}

// 0x54B5D0
void sub_54B5D0(TigMessage* msg)
{
    MesFileEntry mes_file_entry;
    John v1;
    int button_state;
    TigWindowData window_data;
    int index;
    DateTime datetime;
    tig_art_id_t art_id;
    char time_str_buffer[80];
    char buffer[80];

    if (sub_580B10(msg)
        || sub_54DC80(msg)
        || sub_57DC60(msg)) {
        return true;
    }

    if (dword_64C6CC != NULL) {
        if (msg->type != TIG_MESSAGE_KEYBOARD) {
            return dword_64C6CC(msg);
        }

        if (msg->data.keyboard.pressed) {
            return dword_64C6CC(msg);
        }

        if (msg->data.keyboard.key != DIK_ESCAPE && msg->data.keyboard.key != DIK_O) {
            return dword_64C6CC(msg);
        }

        return false;
    }

    if (sub_4B6D70()) {
        if (!player_is_pc_obj(sub_4B6D80())) {
            if (msg->type != TIG_MESSAGE_KEYBOARD
                && msg->type == TIG_MESSAGE_CHAR
                && msg->data.character.ch == ' ') {
                sub_4B6D20();
                return true;
            }
            return false;
        }
    }

    if (msg->type == TIG_MESSAGE_MOUSE) {
        if (msg->data.mouse.event = TIG_MESSAGE_MOUSE_MOVE || sub_541680()) {
            return false;
        }

        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_IDLE) {
            if (dword_64C674 != -1) {
                mes_file_entry.num = dword_64C674;
                v1.type = 6;
                if (mes_search(intgame_mes_file, &mes_file_entry)) {
                    v1.str = mes_file_entry.str;
                    sub_550750(&v1);
                }
            }

            if (msg->data.mouse.y < stru_5C6390[1].y) {
                if (intgame_iso_window_type == 1
                    || intgame_iso_window_type == 2) {
                    sub_5506C0(0);
                    return false;
                }
            } else if (msg->data.mouse.x >= 10
                && msg->data.mouse.x <= 790
                && msg->data.mouse.y <= 590
                && intgame_iso_window_type == 0) {
                if (tig_button_state_get(stru_5C6480[1].button_handle, &button_state) == TIG_OK
                    && button_state == TIG_BUTTON_STATE_PRESSED) {
                    sub_5506C0(1);
                } else if (tig_button_state_get(stru_5C6480[0].button_handle, &button_state) == TIG_OK
                    && button_state == TIG_BUTTON_STATE_PRESSED) {
                    sub_5506C0(2);
                } else {
                    return false;
                }

                if (!sub_573620()) {
                    sub_553990();
                    return false;
                }
            }

            return false;
        }

        switch (msg->data.mouse.event) {
        case TIG_MESSAGE_MOUSE_RIGHT_BUTTON_DOWN:
            if (sub_573620()) {
                return false;
            }

            switch (sub_551A00()) {
            case 5:
            case 7:
            case 9:
            case 14:
            case 15:
                if (tig_window_data(dword_64C52C, &window_data) == TIG_OK) {
                    if (msg->data.mouse.x < window_data.rect.x
                        || msg->data.mouse.x - window_data.rect.x > window_data.rect.width) {
                        sub_551A80(0);
                    }
                    if (msg->data.mouse.y < window_data.rect.y
                        || msg->data.mouse.y - window_data.rect.y > window_data.rect.height) {
                        sub_551A80(0);
                    }
                }
                break;
            }

            if (sub_57E5D0()) {
                return true;
            }
            break;
        case TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP:
            if (sub_573620()) {
                sub_57DC20();
            }
            if (sub_57E8D0(3)) {
                return true;
            }
            break;
        case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
            if (sub_573620()) {
                sub_57DC20();
            }
            if (sub_57E8D0(1)) {
                return true;
            }
            break;
        }

        return false;
    }

    if (msg->type == TIG_MESSAGE_BUTTON) {
        if (msg->data.button.state == TIG_BUTTON_STATE_RELEASED) {
            if (msg->data.button.button_handle == stru_5C6480[1].button_handle) {
                sub_5506C0(1);
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6480[0].button_handle) {
                sub_5506C0(2);
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6480[2].button_handle) {
                sub_54EBF0();
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6480[3].button_handle) {
                sub_56D130(player_get_pc_obj(), player_get_pc_obj());
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[0].button_handle) {
                sub_53F020(player_get_pc_obj());
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[1].button_handle) {
                sub_552740(player_get_pc_obj(), 1);
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[2].button_handle) {
                sub_572240(player_get_pc_obj(), OBJ_HANDLE_NULL, 0);
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[3].button_handle) {
                sub_560760();
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[4].button_handle) {
                sub_56FBF0(player_get_pc_obj());
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6538.button_handle) {
                if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
                    sub_5700C0();
                } else {
                    sub_57B180(0, 0);
                }
                return true;
            }

            if (msg->data.button.button_handle == intgame_mt_button_info.button_handle) {
                sub_556EA0(OBJ_HANDLE_NULL);
                return true;
            }

            switch (intgame_iso_window_type) {
            case 0:
                if (msg->data.button.button_handle == stru_5C65F8[1].button_handle) {
                    if (!sub_573620()) {
                        sub_5528E0();
                        return true;
                    }
                    sub_575770();
                    sub_553990();
                    return true;
                }
                if (msg->data.button.button_handle == stru_5C65F8[0].button_handle) {
                    if (!sub_573620()) {
                        sub_552930();
                        return true;
                    }
                    sub_575770();
                    sub_553990();
                    return true;
                }
                break;
            case 1:
                for (index = 0; index < 5; index++) {
                    if (stru_5C6718[5 * dword_64C530 + index].art_num != -1
                        && msg->data.button.button_handle == stru_5C6718[5 * dword_64C530 + index].button_handle) {
                        if (sub_4B1950(player_get_pc_obj(), 5 * dword_64C530 + index)) {
                            sub_57EFA0(3, 5 * dword_64C530 + index, 0, 0);
                            sub_57BC70(player_get_pc_obj(), 5 * dword_64C530 + index);
                        }
                    }
                }
                break;
            case 2:
                for (index = 0; index < 4; index++) {
                    if (msg->data.button.button_handle == stru_5C6C68[index].button_handle) {
                        sub_57EFA0(2, index, 0, 0);
                        sub_579FA0(player_get_pc_obj(), index);
                        return true;
                    }
                }
                break;
            case 3:
                return sub_570A10(msg);
            case 10:
                return sub_570BC0(msg);
            case 8:
                for (index = 0; index < 5; index++) {
                    if (stru_5C6C18[index].art_num != -1
                        && msg->data.button.button_handle == stru_5C6C18[index].button_handle) {
                        sub_57C040(qword_64C688, index);
                        return true;
                    }
                }
                break;
            case 6:
                if (msg->data.button.button_handle == stru_5C6CA8[0].button_handle) {
                    sub_5672A0();
                    return true;
                }

                if (msg->data.button.button_handle == stru_5C6CA8[1].button_handle) {
                    sub_5672D0();
                    return true;
                }

                if (msg->data.button.button_handle == stru_5C6CA8[2].button_handle) {
                    sub_567320();
                    return true;
                }
                break;
            case 9:
                if (msg->data.button.button_handle == stru_5C6D08[0].button_handle) {
                    dword_64C678 = dword_64C478;
                    sub_553960();
                    return true;
                }

                if (msg->data.button.button_handle == stru_5C6D08[3].button_handle) {
                    sub_578B80(dword_64C678);
                    sub_551A80(0);
                    return true;
                }

                if (msg->data.button.button_handle == stru_5C6D08[4].button_handle) {
                    sub_551A80(0);
                    return true;
                }
                break;
            }

            for (index = 0; index < 5; index++) {
                if (msg->data.button.button_handle == stru_5C6E40[index].button_handle) {
                    sub_57C370(index);
                    return true;
                }
            }

            for (index = 0; index < 5; index++) {
                if (msg->data.button.button_handle == stru_5C6E90[index].button_handle) {
                    sub_57C370(index);
                    return true;
                }
            }

            return false;
        } // msg->data.button.state == TIG_BUTTON_STATE_RELEASED

        if (msg->data.button.state == TIG_BUTTON_STATE_PRESSED) {
            if (msg->data.button.state == stru_5C6480[1].button_handle) {
                if (tig_button_state_get(stru_5C6480[0].button_handle, &button_state) == TIG_OK
                    && button_state == TIG_BUTTON_STATE_PRESSED) {
                    tig_button_state_change(stru_5C6480[0].button_handle, TIG_BUTTON_STATE_RELEASED);
                }
                sub_5506C0(1);
                return true;
            }

            if (msg->data.button.state == stru_5C6480[0].button_handle) {
                if (tig_button_state_get(stru_5C6480[1].button_handle, &button_state) == TIG_OK
                    && button_state == TIG_BUTTON_STATE_PRESSED) {
                    tig_button_state_change(stru_5C6480[1].button_handle, TIG_BUTTON_STATE_RELEASED);
                }
                sub_5506C0(2);
                return true;
            }

            switch (intgame_iso_window_type) {
            case 1:
                for (index = 0; index < COLLEGE_COUNT; index++) {
                    if (college_get_art_num(index) != -1
                        && msg->data.button.state == stru_5C6618[index].button_handle) {
                        sub_550CD0(dword_64C530);
                        dword_64C530 = index;
                        sub_550C60(index);
                        return true;
                    }
                }
                break;
            case 9:
                if (msg->data.button.state == stru_5C6D08[1].button_handle) {
                    if (dword_64C678 < dword_64C478) {
                        dword_64C678++;
                        sub_553960();
                    }
                    return true;
                }
                if (msg->data.button.state == stru_5C6D08[2].button_handle) {
                    if (dword_64C678 > 0) {
                        dword_64C678--;
                        sub_553960();
                    }
                    return true;
                }
                break;
            case 6:
                for (index = 3; index < 6; index++) {
                    if (msg->data.button.state == stru_5C6CA8[index].button_handle) {
                        sub_564000(index - 3);
                        return true;
                    }
                }
                break;
            }

            return false;
        } // msg->data.button.state == TIG_BUTTON_STATE_PRESSED

        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_INSIDE) {
            if (msg->data.button.button_handle == stru_5C6F90.button_handle) {
                datetime = sub_45A7C0();

                mes_file_entry.num = 22;
                mes_get_msg(intgame_mes_file, &mes_file_entry);
                datetime_format_time(&datetime, time_str_buffer);
                sprintf(buffer, "%s: %s", mes_file_entry.str, time_str_buffer);

                mes_file_entry.num = 23;
                mes_get_msg(intgame_mes_file, &mes_file_entry);
                datetime_format_date(&datetime, time_str_buffer);
                sprintf(&(buffer[strlen(buffer)]), "   %s: %s", mes_file_entry.str, time_str_buffer);

                v1.type = 6;
                v1.str = buffer;
                sub_550750(&v1);
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6480[1].button_handle) {
                dword_64C674 = 1000;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6480[0].button_handle) {
                dword_64C674 = 1001;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6480[2].button_handle) {
                dword_64C674 = 1002;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6480[3].button_handle) {
                dword_64C674 = 1003;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[0].button_handle) {
                dword_64C674 = 1004;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[1].button_handle) {
                dword_64C674 = 1005;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[2].button_handle) {
                dword_64C674 = 1006;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[3].button_handle) {
                dword_64C674 = 1007;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C64C0[4].button_handle) {
                dword_64C674 = 1008;
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6538.button_handle) {
                dword_64C674 = ((tig_net_flags & TIG_NET_CONNECTED) != 0) ? 1010 : 1009;
            }

            switch (intgame_iso_window_type) {
            case 1:
                for (index = 0; index < 16; index++) {
                    if (msg->data.button.button_handle == stru_5C6618[index].button_handle) {
                        sub_550860(index);
                        return true;
                    }
                }
                for (index = 0; index < 5; index++) {
                    if (msg->data.button.button_handle == stru_5C6718[5 * dword_64C530 + index].button_handle) {
                        sub_5507E0(5 * dword_64C530 + index);
                        return true;
                    }
                }
                break;
            case 8:
                for (index = 0; index < 5; index++) {
                    if (msg->data.button.button_handle == stru_5C6C18[index].button_handle) {
                        sub_5507E0(mt_item_spell(qword_64C688, index));
                        return true;
                    }
                }
                break;
            case 2:
                for (index = 0; index < 4; index++) {
                    if (msg->data.button.button_handle == stru_5C6C68[index].button_handle) {
                        sub_5508C0(index);
                        return true;
                    }
                }
                break;
            case 0:
                if (msg->data.button.button_handle == stru_5C65F8[1].button_handle) {
                    if (!sub_573620()) {
                        art_id = tig_mouse_get_cursor_art_id();
                        switch (tig_art_num_get(art_id)) {
                        case 0:
                        case 353:
                            tig_art_interface_id_create(498, 0, 0, 0, &art_id);
                            tig_mouse_hide();
                            tig_mouse_cursor_set_art_id(art_id);
                            tig_mouse_show();
                            break;
                        }
                    }
                    return true;
                }
                if (msg->data.button.button_handle == stru_5C65F8[0].button_handle) {
                    if (!sub_573620()) {
                        art_id = tig_mouse_get_cursor_art_id();
                        switch (tig_art_num_get(art_id)) {
                        case 0:
                        case 353:
                            tig_art_interface_id_create(499, 0, 0, 0, &art_id);
                            tig_mouse_hide();
                            tig_mouse_cursor_set_art_id(art_id);
                            tig_mouse_show();
                            break;
                        }
                    }
                    return true;
                }
                break;
            }

            for (index = 0; index < 5; index++) {
                if (msg->data.button.button_handle == stru_5C6E40[index].button_handle) {
                    sub_57C3F0(index);
                    return true;
                }
            }

            for (index = 0; index < 5; index++) {
                if (msg->data.button.button_handle == stru_5C6E90[index].button_handle) {
                    sub_57C3F0(index);
                    return true;
                }
            }

            return false;
        } // msg->data.button.state == TIG_BUTTON_STATE_MOUSE_INSIDE

        return false;
    } // msg->type == TIG_MESSAGE_BUTTON

    if (msg->type == TIG_MESSAGE_KEYBOARD) {
        if (textedit_ui_is_focused()) {
            return textedit_ui_process_message(msg);
        }

        // NOTE: Explicit check for 1, 0, and everything else.
        if (msg->data.keyboard.pressed == 1) {
            switch (msg->data.keyboard.key) {
            case DIK_BACK:
            case DIK_DELETE:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 /= 10;
                    sub_553960();
                }
                return true;
            case DIK_COMMA:
            case DIK_PERIOD:
            case DIK_SLASH:
                sub_553990();
                return false;
            case DIK_NUMPAD7:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 7;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD8:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 8;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD9:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 9;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD4:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 4;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD5:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 5;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD6:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 6;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD1:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 1;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD2:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 2;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD3:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678 + 3;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            case DIK_NUMPAD0:
                if (intgame_iso_window_type == 9) {
                    dword_64C678 = 10 * dword_64C678;
                    if (dword_64C678 > dword_64C478) {
                        dword_64C678 = dword_64C478;
                    }
                    sub_553960();
                }
                return true;
            }

            return false;
        } else if (msg->data.keyboard.pressed == 0) {
            switch (msg->data.keyboard.key) {
            case DIK_K:
                if (!sub_541680()) {
                    sub_54DBF0(0, 2);
                }
                return true;
            case DIK_M:
                if (!sub_541680()) {
                    sub_54DBF0(1, 1);
                }
                return true;
            case DIK_COMMA:
            case DIK_PERIOD:
            case DIK_SLASH:
                sub_553990();
                return false;
            }

            return false;
        }

        return false;
    } // msg->type == TIG_MESSAGE_KEYBOARD

    if (msg->type == TIG_MESSAGE_CHAR) {
        bool v2;

        if (textedit_ui_is_focused()) {
            return textedit_ui_process_message(msg);
        }

        v2 = false;
        if (msg->data.character.ch == '\r') {
            if (intgame_iso_window_type == 9) {
                sub_578B80(dword_64C678);
                sub_551A80(0);
                v2 = true;
            } else if (!sub_541680()) {
                sub_5718A0();
                v2 = true;
            }
        }

        if (sub_541680()) {
            return false;
        }

        switch (msg->data.character.ch) {
        case ' ':
            sub_4B6D20();
            break;
        case 'C':
        case 'c':
            sub_552740(player_get_pc_obj(), 1);
            break;
        case 'F':
        case 'f':
            sub_56FBF0(player_get_pc_obj());
            break;
        case 'I':
        case 'i':
            sub_572240(player_get_pc_obj(), OBJ_HANDLE_NULL, 0);
            break;
        case 'R':
        case 'r':
            sub_54EBF0();
            break;
        case 'S':
        case 's':
            sub_57B180(0, 0);
            break;
        case 'T':
        case 't':
            sub_56D130(player_get_pc_obj(), player_get_pc_obj());
            break;
        case 'W':
        case 'w':
            sub_560760();
            break;
        default:
            if (!v2) {
                return false;
            }
            break;
        }

        gsound_play_sfx_id(0, 1);
        return true;
    } // msg->type == TIG_MESSAGE_CHAR

    return false;
}

// 0x54C8E0
void sub_54C8E0()
{
    // TODO: Incomplete.
}

// 0x54DBF0
void sub_54DBF0(int btn, int window_type)
{
    int state;

    tig_button_state_get(stru_5C6480[btn].button_handle, &state);
    if (state != TIG_BUTTON_STATE_PRESSED) {
        tig_button_state_change(stru_5C6480[btn != 1 ? 1 : 0].button_handle, TIG_BUTTON_STATE_RELEASED);
        sub_551F20();
        tig_button_state_change(stru_5C6480[btn].button_handle, TIG_BUTTON_STATE_PRESSED);
        sub_5506C0(window_type);
    } else {
        sub_5506C0(0);
        tig_button_state_change(stru_5C6480[btn].button_handle, TIG_BUTTON_STATE_RELEASED);
        sub_551F40();
    }
}

// 0x54DC80
bool sub_54DC80(TigMessage* msg)
{
    // TODO: Incomplete.
}

// 0x54DE50
void sub_54DE50(TigMessage* msg)
{
    // TODO: Incomplete.
}

// 0x54EA80
void sub_54EA80()
{
    // TODO: Incomplete.
}

// 0x54EB50
void sub_54EB50()
{
    // TODO: Incomplete.
}

// 0x54EB60
void sub_54EB60()
{
    // TODO: Incomplete.
}

// 0x54EBF0
void sub_54EBF0()
{
    // TODO: Incomplete.
}

// 0x54ECD0
void sub_54ECD0()
{
    // TODO: Incomplete.
}

// 0x54ED30
void sub_54ED30()
{
    // TODO: Incomplete.
}

// 0x54FCF0
void sub_54FCF0()
{
    // TODO: Incomplete.
}

// 0x550000
void sub_550000()
{
    // TODO: Incomplete.
}

// 0x550150
void sub_550150()
{
    // TODO: Incomplete.
}

// 0x5501C0
void sub_5501C0()
{
    // TODO: Incomplete.
}

// 0x5503F0
void sub_5503F0()
{
    // TODO: Incomplete.
}

// 0x5504F0
void iso_interface_window_disable()
{
    // TODO: Incomplete.
}

// 0x5506C0
void sub_5506C0(int window_type)
{
    sub_571910();
    if (intgame_iso_window_type == 9) {
        sub_551A80(0);
    }

    dword_5C6F78 = 6;
    if (intgame_iso_window_type == window_type) {
        dword_64C6AC = 0;
        sub_552130(0);
    } else {
        dword_64C6AC = window_type;
        sub_552130(window_type);
    }
}

// 0x550720
void sub_550720()
{
    if (dword_64C6B4 && intgame_iso_window_type == 0) {
        if (dword_64C6D4 != NULL) {
            dword_64C6D4(0);
        } else {
            sub_550930();
        }
    }
}

// 0x550750
void sub_550750(John* a1)
{
    sub_552770(a1);
    if (dword_64C6D4 != NULL) {
        dword_64C6D4(a1);
    }
}

// 0x550770
void sub_550770(int a1, const char* str)
{
    John v1;

    (void)a1;

    if (dword_64C6D4 != NULL) {
        v1.type = 6;
        v1.str = str;
        dword_64C6D4(&v1);
    } else if (dword_64C6B4) {
        sub_5509C0(str, &(stru_5C6D60[intgame_iso_window_type].rect));
    }
}

// 0x5507D0
void sub_5507D0(void(*func)(John* a1))
{
    dword_64C6D4 = func;
}

// 0x5507E0
void sub_5507E0(int spl)
{
    John v1;

    if (intgame_iso_window_type != 0) {
        sub_5509C0(spell_get_name(spl), &(stru_5C6D60[intgame_iso_window_type].rect));
    } else {
        v1.type = 8;
        v1.field_8 = spl;
        v1.field_C = 0;
        v1.field_10 = player_get_pc_obj();
        sub_550750(&v1);
        sub_552770(&v1);
    }
}

// 0x550860
void sub_550860(int college)
{
    John v1;

    if (intgame_iso_window_type != 0) {
        sub_5509C0(college_get_name(college), &(stru_5C6D60[intgame_iso_window_type].rect));
    } else {
        v1.type = 9;
        v1.field_8 = college;
        sub_550750(&v1);
        sub_552770(&v1);
    }
}

// 0x5508C0
void sub_5508C0(int value)
{
    John v1;

    if (intgame_iso_window_type != 0) {
        sub_5509C0(sub_57A700(value), &(stru_5C6D60[intgame_iso_window_type].rect));
    } else {
        v1.type = 7;
        v1.field_8 = sub_57A6A0(value);
        v1.field_C = 0;
        sub_550750(&v1);
    }
}

// 0x550930
void sub_550930()
{
    if (dword_64C6B4) {
        if (intgame_is_compact_interface()) {
            sub_568D20();
            sub_568F40();
        } else {
            tig_window_fill(stru_5C6D60[intgame_iso_window_type].window_handle,
                &(stru_5C6D60[intgame_iso_window_type].rect),
                tig_color_make(0, 0, 0));
        }
    }
}

// 0x5509C0
void sub_5509C0(const char* str, TigRect* rect)
{
    if (dword_64C6B4) {
        if (stru_5C6D60[intgame_iso_window_type].rect.width != 0) {
            sub_550930();
            sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
                str,
                rect,
                dword_64C498,
                12);
        }
    }
}

// 0x550A10
void sub_550A10(tig_window_handle_t window_handle, const char* str, TigRect* rect, tig_font_handle_t font, unsigned int flags)
{
    // TODO: Incomplete.
}

// 0x550BD0
void sub_550BD0()
{
    // TODO: Incomplete.
}

// 0x550C60
void sub_550C60(int group)
{
    int64_t pc_obj;
    int spl;
    int index;

    pc_obj = player_get_pc_obj();
    if (pc_obj != OBJ_HANDLE_NULL) {
        spl = group * 5;
        for (index = 0; index < 5; index++) {
            if (!sub_4B1950(pc_obj, spl)) {
                break;
            }

            if (stru_5C6718[spl].button_handle != TIG_BUTTON_HANDLE_INVALID) {
                tig_button_show(stru_5C6718[spl].button_handle);
            }
        }
    }
}

// 0x550CD0
void sub_550CD0(int group)
{
    int spl;
    int index;

    spl = group * 5;
    for (index = 0; index < 5; index++) {
        if (stru_5C6718[spl].button_handle != TIG_BUTTON_HANDLE_INVALID) {
            tig_button_hide(stru_5C6718[spl].button_handle);
        }
        spl++;
    }

    sub_5503F0(intgame_iso_window_type, 100);
}

// 0x550D20
bool sub_550D20()
{
    int index;

    for (index = 0; index < 5; index++) {
        stru_5C6C18[index].art_num = sub_4B1570(0);
        if (stru_5C6C18[index].art_num != -1) {
            if (!sub_54AAE0(&(stru_5C6C18[index]))) {
                return false;
            }
        }
    }

    sub_550D60();
    return true;
}

// 0x550D60
void sub_550D60()
{
    int index;

    for (index = 0; index < 5; index++) {
        if (stru_5C6C18[index].button_handle != TIG_BUTTON_HANDLE_INVALID) {
            tig_button_hide(stru_5C6C18[index].button_handle);
        }
    }

    sub_5503F0(intgame_iso_window_type, 100);
}

// 0x550DA0
void sub_550DA0(int a1, S550DA0* a2)
{
    TigVideoBufferCreateInfo vb_create_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;

    if (dword_64C6BC == a1) {
        return;
    }

    switch (dword_64C6BC) {
    case 1:
        if (tig_video_buffer_destroy(dword_64C6E4) == TIG_OK) {
            dword_64C6E4 = NULL;
        }
        break;
    case 2:
        sub_4D81F0();
        break;
    }

    switch (a1) {
    case 0:
        dword_64C520 = TIG_WINDOW_HANDLE_INVALID;
        sub_558130(NULL);
        iso_redraw();
        break;
    case 1:
        sub_551A10(player_get_pc_obj());
        vb_create_info.width = stru_64C510.width;
        vb_create_info.height = stru_64C510.height;
        vb_create_info.flags = 0;
        vb_create_info.background_color = 0;
        dword_64C520 = a2->window_handle;
        dword_64C528 = a2->art_id;
        dword_64C524 = &stru_64C660;
        stru_64C660 = *a2->rect;
        if (tig_video_buffer_create(&vb_create_info, &dword_64C6E4) == TIG_OK) {
            sub_551080();
        }
        sub_558130(&stru_64C510);
        break;
    case 2:
        sub_4D81F0();
        dword_64C520 = a2->window_handle;
        dword_64C528 = a2->art_id;
        dword_64C524 = &stru_64C660;
        stru_64C660 = *a2->rect;

        tig_window_fill(dword_64C520,
            &stru_64C660,
            tig_color_make(0, 0, 0));

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = stru_64C660.width;
        src_rect.height = stru_64C660.height;

        dst_rect.x = dword_64C524->x;
        dst_rect.y = dword_64C524->y;
        dst_rect.width = stru_64C660.width;
        dst_rect.height = stru_64C660.height;

        blit_info.flags = 0;
        blit_info.art_id = dword_64C528;
        blit_info.src_rect = &src_rect;
        blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(dword_64C520, &blit_info);

        sub_558130(&stru_64C698);
        sub_402F90();
        dword_64C6BC = 2;
        dword_5C72B0--;
        return;
    }

    if (dword_5C72B0 < 1) {
        sub_402FA0();
        dword_5C72B0++;
    }

    dword_64C6BC = a1;
}

// 0x551000
bool sub_551000(int x, int y)
{
    TigWindowData window_data;

    if (dword_64C520 == TIG_WINDOW_HANDLE_INVALID) {
        return false;
    }

    if (tig_window_data(dword_64C520, &window_data) != TIG_OK) {
        return false;
    }

    if (dword_64C524 == NULL) {
        return false;
    }

    return x >= window_data.rect.x + dword_64C524->x
        && x < window_data.rect.x + dword_64C524->x + dword_64C524->width
        && y >= window_data.rect.y + dword_64C524->y
        && y < window_data.rect.y + dword_64C524->y + dword_64C524->height;
}

// 0x551080
void sub_551080()
{
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    if (dword_64C6E4 != NULL) {
        tig_window_copy(dword_64C520,
            dword_64C524, dword_64C52C,
            &stru_64C510);

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = dword_64C524->width;
        src_rect.height = dword_64C524->height;

        if (tig_window_copy_to_vbuffer(dword_64C52C, &stru_64C510, dword_64C6E4, &src_rect) == TIG_OK) {
            dst_rect = src_rect;
            dst_rect.x = dword_64C524->x;
            dst_rect.y = dword_64C524->y;

            blit_info.art_id = dword_64C528;
            blit_info.flags = 0;
            blit_info.src_rect = &src_rect;
            blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(dword_64C520, &blit_info);
        }
    }
}

// 0x551160
void sub_551160()
{
    int64_t pc_obj;
    int64_t obj;

    if (dword_64C6B4) {
        pc_obj = player_get_pc_obj();

        iso_interface_window_disable(intgame_iso_window_type);
        iso_interface_window_enable(intgame_iso_window_type);

        if (intgame_iso_window_type == 0) {
            if (pc_obj != OBJ_HANDLE_NULL) {
                if (qword_64C690 != OBJ_HANDLE_NULL) {
                    sub_57CCF0(pc_obj, qword_64C690);
                } else if ((obj = sub_43C570()) != OBJ_HANDLE_NULL) {
                    sub_57CCF0(pc_obj, obj);
                } else {
                    sub_552960(false);
                }
            } else {
                sub_552960(false);
            }
        }

        if (pc_obj != OBJ_HANDLE_NULL) {
            sub_54AD00(2, stat_level(pc_obj, STAT_FATE_POINTS), 2);
            sub_556C20(pc_obj);
        }
    }
}

// 0x551210
void iso_interface_window_enable(int window_type)
{
    int64_t pc_obj;
    int64_t obj;
    int fld;
    int qty_fld;
    int index;
    TigArtBlitInfo blit_info;
    TigArtFrameData art_frame_data;
    TigRect src_rect;
    TigRect dst_rect;

    if (dword_64C6E0) {
        iso_interface_window_disable(intgame_iso_window_type);
    }

    sub_5503F0(window_type, 100);
    intgame_iso_window_type = window_type;

    pc_obj = player_get_pc_obj();

    switch (window_type) {
    case 0:
        for (index = 0; index < 2; index++) {
            tig_button_show(stru_5C65F8[index].button_handle);
        }
        break;
    case 1:
        for (index = 0; index < COLLEGE_COUNT; index++) {
            if (college_get_art_num(index) != -1
                && sub_4B1B00(pc_obj, index)) {
                tig_button_show(stru_5C6618[index].button_handle);
            }
        }
        tig_button_state_change(stru_5C6618[dword_64C530].button_handle, TIG_BUTTON_STATE_PRESSED);
        sub_550C60(dword_64C530);
        break;
    case 2:
        for (index = 0; index < 4; index++) {
            if (sub_579F50(index) != -1) {
                tig_button_show(stru_5C6C68[index].button_handle);
            }
        }
        break;
    case 3:
        index = sub_551740(219, 510);
        if (index != -1) {
            sub_570940(dword_64C4F8[index]);
        }
        break;
    case 4:
        break;
    case 5:
        for (index = 0; index < 5; index++) {
            tig_button_show(stru_64C4A8[index].button_handle);
        }
        break;
    case 6:
        for (index = 0; index < 6; index++) {
            tig_button_show(stru_5C6CA8[index].button_handle);
        }
        break;
    case 7:
        break;
    case 8:
        sub_551660();
        break;
    case 9:
        for (index = 0; index < 5; index++) {
            tig_button_show(stru_5C6D08[index].button_handle);
        }

        dword_64C678 = 0;

        obj = sub_579760();
        fld = sub_462410(obj, &qty_fld);
        if (fld != -1) {
            dword_64C478 = obj_field_int32_get(obj, qty_fld);
            switch (fld) {
            case OBJ_F_CRITTER_GOLD:
                tig_art_item_id_create(0, 2, 0, 0, 0, 3, 0, 0, &(blit_info.art_id));
                break;
            case OBJ_F_CRITTER_ARROWS:
                tig_art_item_id_create(0, 2, 0, 0, 0, 1, 0, 0, &(blit_info.art_id));
                break;
            case OBJ_F_CRITTER_BULLETS:
                tig_art_item_id_create(1, 2, 0, 0, 0, 1, 0, 0, &(blit_info.art_id));
                break;
            case OBJ_F_CRITTER_POWER_CELLS:
                tig_art_item_id_create(2, 2, 0, 0, 0, 1, 0, 0, &(blit_info.art_id));
                break;
            case OBJ_F_CRITTER_FUEL:
                tig_art_item_id_create(3, 2, 0, 0, 0, 1, 0, 0, &(blit_info.art_id));
                break;
            }

            tig_art_frame_data(blit_info.art_id, &art_frame_data);

            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.height;

            dst_rect.x = 269;
            dst_rect.y = 93;
            dst_rect.width = art_frame_data.width;
            dst_rect.height = art_frame_data.height;

            blit_info.flags = 0;
            blit_info.src_rect = &src_rect;
            blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(dword_64C4F8[1], &blit_info);
        } else {
            dword_64C478 = 1;
        }
        sub_553960();
        break;
    case 10:
        index = sub_551740(286, 524);
        if (index != -1) {
            sub_570260(dword_64C4F8[index]);
        }
        break;
    default:
        tig_debug_printf("iso_interface_window_enable: ERROR: window type out of range!");
        break;
    }

    dword_64C6E0 = true;
}

// 0x551660
void sub_551660()
{
    int index;
    int spl;
    tig_art_id_t art_id;

    if (player_get_pc_obj() == OBJ_HANDLE_NULL
        || qword_64C688 == OBJ_HANDLE_NULL) {
        return;
    }

    if ((obj_field_int32_get(qword_64C688, OBJ_F_ITEM_FLAGS) & OIF_IDENTIFIED) != 0
        && obj_field_int32_get(qword_64C688, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) > 0) {
        for (index = 0; index < 5; index++) {
            spl = mt_item_spell(qword_64C688, index);
            if (spl != -1
                && !sub_459FC0(spl)
                && stru_5C6C18[index].art_num != -1) {
                stru_5C6C18[index].art_num = sub_4B1570(spl);
                tig_art_interface_id_create(stru_5C6C18[index].art_num, 0, 0, 0, &art_id);
                tig_button_set_art(stru_5C6C18[index].button_handle, art_id);
                tig_button_show(stru_5C6C18[index].button_handle);
            }
        }
    }

    sub_54B3C0();
}

// 0x551740
int sub_551740(int x, int y)
{
    int index;

    for (index = 0; index < 2; index++) {
        if (x >= stru_5C6390[index].x
            && y >= stru_5C6390[index].y
            && x < stru_5C6390[index].x + stru_5C6390[index].width
            && y < stru_5C6390[index].y + stru_5C6390[index].height) {
            return index;
        }
    }

    return -1;
}

// 0x5517A0
bool sub_5517A0(TigMessage* msg)
{
    tig_window_handle_t window_handle;
    TigWindowData window_data;

    if (tig_window_get_at_position(msg->data.mouse.x, msg->data.mouse.y, &window_handle) != TIG_OK) {
        return false;
    }

    if (window_handle != dword_64C52C) {
        // FIXME: Meaningless.
        tig_window_data(window_handle, &window_data);
        return false;
    }

    return true;
}

// 0x5517F0
void sub_5517F0()
{
    TigMouseState mouse_state;
    TigMessage msg;

    if (tig_mouse_get_state(&mouse_state) == TIG_OK) {
        // NOTE: Fake TigMessage object, albeit incomplete - underlying code
        // is only interested in mouse coordinates.
        msg.data.mouse.x = mouse_state.x;
        msg.data.mouse.y = mouse_state.y;
        sub_551910(&msg);
    }
}

// 0x551830
void sub_551830()
{
    // TODO: Incomplete.
}

// 0x5518C0
void sub_5518C0(int x, int y)
{
    tig_window_handle_t window_handle;
    TigWindowData window_data;

    if (tig_window_get_at_position(x, y, &window_handle) != TIG_OK) {
        return false;
    }

    if (window_handle != dword_64C52C) {
        // FIXME: Meaningless.
        tig_window_data(window_handle, &window_data);
        return false;
    }

    return true;
}

// 0x551910
void sub_551910()
{
    // TODO: Incomplete.
}

// 0x551A00
int sub_551A00()
{
    return dword_64C634[dword_64C6B8];
}

// 0x551A10
void sub_551A10(int64_t obj)
{
    int64_t location;
    int64_t x;
    int64_t y;

    if (obj != OBJ_HANDLE_NULL) {
        location = obj_field_int64_get(obj, OBJ_F_LOCATION);
        sub_4B8940(location, &x, &y);
        if (x != 0 || y != 0) {
            sub_4B8CE0(location);
            iso_redraw();
        }
    }
}

// 0x551A80
bool sub_551A80(int a1)
{
    // TODO: Incomplete.
}

// 0x551F20
void sub_551F20()
{
    if (intgame_is_compact_interface()) {
        intgame_toggle_interface();
        dword_64C6DC = true;
    }
}

// 0x551F40
void sub_551F40()
{
    if (dword_64C6DC) {
        if (!intgame_is_compact_interface()) {
            intgame_toggle_interface();
            dword_64C6DC = false;
        }
    }
}

// 0x551F70
int sub_551F70(int a1)
{
    return dword_5C72B4[a1];
}

// 0x551F80
void sub_551F80()
{
    // TODO: Incomplete.
}

// 0x552050
void sub_552050()
{
    // TODO: Incomplete.
}

// 0x552070
void sub_552070()
{
    return intgame_iso_window_type;
}

// 0x552080
void sub_552080(int window_type)
{
    if (window_type != -1) {
        sub_5520D0(window_type, dword_5C6F78);

        if (dword_5C6F78 < 6) {
            dword_5C6F78++;
            sub_57D350(8, window_type);
        } else {
            dword_5C6D58 = intgame_iso_window_type;
        }
    }
}

// 0x5520D0
void sub_5520D0(int window_type, int a2)
{
    if (intgame_iso_window_type != window_type) {
        if (a2 == 0) {
            iso_interface_window_disable(intgame_iso_window_type);
        } else if (a2 == 6) {
            iso_interface_window_enable(window_type);
        } else {
            sub_5503F0(window_type, 100 * a2 / 6);
        }
    }
}

// 0x552130
void sub_552130(int window_type)
{
    iso_interface_window_disable(intgame_iso_window_type);
    dword_5C6F78 = 6;
    iso_interface_window_enable(window_type);
    dword_5C6D58 = intgame_iso_window_type;
}

// 0x552160
void sub_552160(const char* str, tig_font_handle_t font)
{
    intgame_text_edit_refresh_color(str,
        font,
        tig_color_make(0, 0, 0),
        0);
}

// 0x5521B0
void intgame_text_edit_refresh_color()
{
    // TODO: Incomplete.
}

// 0x5522F0
void intgame_clock_refresh()
{
    // TODO: Incomplete.
}

// 0x5526F0
bool intgame_clock_process_callback()
{
    DateTime datetime;
    TimeEvent timeevent;

    if (dword_64C6B4) {
        intgame_clock_refresh();
        timeevent.type = TIMEEVENT_TYPE_CLOCK;
        sub_45A950(&datetime, 3600000);
        timeevent_clear_one_typed(TIMEEVENT_TYPE_CLOCK);
        sub_45B800(&timeevent, &datetime);
    }

    return true;
}

// 0x552740
void sub_552740(int64_t obj, int a2)
{
    if (dword_64C6B4) {
        if (obj != OBJ_HANDLE_NULL) {
            charedit_create(obj, a2);
        }
    }
}

// 0x552770
void sub_552770()
{
    // TODO: Incomplete.
}

// 0x5528E0
void sub_5528E0()
{
    int v1;

    if (intgame_iso_window_type == 0) {
        v1 = (dword_64C6C8 + 9) % 10;
        if (v1 != dword_64C6C4 && (dword_64C6C4 - v1 + 10) % 10 < dword_64C6C0) {
             dword_64C6C8 = v1;
        }
    }

    sub_552960(false);
}

// 0x552930
void sub_552930()
{
    if (intgame_iso_window_type == 0) {
        if (dword_64C6C8 != dword_64C6C4) {
            dword_64C6C8 = (dword_64C6C8 + 1) % 10;
        }
    }

    sub_552960(false);
}

// 0x552960
void sub_552960(bool play_sound)
{
    if (dword_64C6B4) {
        sub_5506C0(0);

        if (intgame_iso_window_type == 0) {
            if (dword_64C6C0 > 0) {
                sub_550930();
                sub_5529C0(stru_5C6D60[intgame_iso_window_type].window_handle,
                    &(stru_64C540[dword_64C6C8]),
                    play_sound);
            }
        }
    }
}

// 0x5529C0
void sub_5529C0()
{
    // TODO: Incomplete.
}

// 0x553320
bool sub_553320(void(*func)(TigMessage* msg))
{
    dword_64C6CC = func;
    tc_show();
    sub_551A80(0);
    sub_551A80(3);

    return true;
}

// 0x553350
void sub_553350()
{
    dword_64C6CC = NULL;
    tc_hide();
    sub_551A80(0);
}

// 0x553370
void sub_553370()
{
    tc_clear();
}

// 0x553380
void sub_553380(int a1, const char* str)
{
    sub_4C9810(a1, str);
}

// 0x5533A0
void sub_5533A0(TigMessage* msg)
{
    sub_4C9A10(msg);
}

// 0x5533B0
int sub_5533B0()
{
    return intgame_iso_window_type;
}

// 0x5533C0
void sub_5533C0()
{
    // TODO: Incomplete.
}

// 0x553620
void sub_553620(int index, tig_art_id_t art_id)
{
    if (dword_64C6B4) {
        sub_5533C0(&(stru_5C6E40[index]), index, art_id, dword_64C4F8[0]);
        sub_5533C0(&(stru_5C6E90[index]), index, art_id, dword_5C6378[index]);
    }
}

// 0x553670
void intgame_spell_maintain_refresh_func()
{
    // TODO: Incomplete.
}

// 0x553910
void sub_553910(int index, bool a2)
{
    intgame_spell_maintain_refresh_func(stru_5C6E40[index].button_handle,
        &(stru_5C6E40[index]),
        index,
        a2,
        dword_64C4F8[0]);
    intgame_spell_maintain_refresh_func(stru_5C6E90[index].button_handle,
        &(stru_5C6E90[index]),
        index,
        a2,
        dword_5C6378[index]);
}

// 0x553960
void sub_553960()
{
    roller_ui_draw(dword_64C678, dword_64C4F8[1], 404, 104, 6, 0);
}

// 0x553990
void sub_553990()
{
    bool v1 = false;
    int64_t pc_obj;
    tig_art_id_t art_id;
    int art_num;

    pc_obj = player_get_pc_obj();
    if (pc_obj != OBJ_HANDLE_NULL) {
        art_id = obj_field_int32_get(pc_obj, OBJ_F_CURRENT_AID);
        if (sub_5040D0(art_id) != 0) {
            v1 = true;
        }
    }

    if (!sub_57DE00()) {
        // FIXME: Meaningless.
        tig_mouse_get_cursor_art_id();

        art_num = dword_5C7178[sub_551A00()];
        if (art_num == -1) {
            if (v1) {
                if (tig_kb_is_key_pressed(DIK_COMMA)) {
                    art_num = 818;
                } else if (tig_kb_is_key_pressed(DIK_PERIOD)) {
                    art_num = 819;
                } else if (tig_kb_is_key_pressed(DIK_SLASH)) {
                    art_num = 820;
                } else {
                    art_num = 353;
                }
            } else {
                art_num = 0;
            }
        }

        tig_art_interface_id_create(art_num, 0, 0, 0, &art_id);
        tig_mouse_cursor_set_art_id(art_id);

        sub_5736E0();
    }
}

// 0x553A60
void sub_553A60(int art_num)
{
    dword_5C7178[16] = art_num;
}

// 0x553A70
void sub_553A70()
{
    // TODO: Incomplete.
}

// 0x553BE0
void sub_553BE0(int64_t a1, int64_t a2, const char* str)
{
    int type;

    if (intgame_iso_window_type != 3) {
        if (intgame_iso_window_type != 0) {
            sub_550770(-1, str);
        } else {
            type = obj_field_int32_get(a2, OBJ_F_TYPE);
            switch (type) {
            case OBJ_TYPE_WALL:
                break;
            case OBJ_TYPE_PORTAL:
                sub_555EC0(a1, a2, str);
                break;
            case OBJ_TYPE_CONTAINER:
                sub_556040(a1, a2, str);
                break;
            case OBJ_TYPE_SCENERY:
                sub_555D80(a1, a2, str);
                break;
            case OBJ_TYPE_PROJECTILE:
                break;
            case OBJ_TYPE_WEAPON:
            case OBJ_TYPE_AMMO:
            case OBJ_TYPE_ITEM_ARMOR:
            case OBJ_TYPE_ITEM_GOLD:
            case OBJ_TYPE_ITEM_FOOD:
            case OBJ_TYPE_ITEM_SCROLL:
            case OBJ_TYPE_ITEM_KEY:
            case OBJ_TYPE_ITEM_KEY_RING:
            case OBJ_TYPE_ITEM_WRITTEN:
            case OBJ_TYPE_ITEM_GENERIC:
                sub_554F10(a1, a2, str);
                break;
            case OBJ_TYPE_PC:
            case OBJ_TYPE_NPC:
                sub_553F70(a1, a2, str);
                break;
            default:
                sub_550770(-1, str);
                break;
            }
        }
    }
}

// 0x553D10
void sub_553D10()
{
    // TODO: Incomplete.
}

// 0x553F70
void sub_553F70()
{
    // TODO: Incomplete.
}

// 0x554560
void sub_554560(tig_window_handle_t window_handle, int art_num)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    if (intgame_is_compact_interface()) {
        window_handle = sub_568D20();
    }

    tig_art_interface_id_create(art_num, 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = 217;
    dst_rect.y = 69;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    if (intgame_is_compact_interface()) {
        dst_rect.x -= 210;
        dst_rect.y -= 59;
    }

    blit_info.art_id = art_id;
    blit_info.flags = 0;
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(window_handle, &blit_info);
}

// 0x554640
void sub_554640()
{
    // TODO: Incomplete.
}

// 0x554830
void sub_554830()
{
    // TODO: Incomplete.
}

// 0x554B00
void sub_554B00(tig_window_handle_t window_handle, int art_num, int x, int y)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    if (intgame_is_compact_interface()) {
        window_handle = sub_568D20();
        x -= 210;
        y -= 59;
    }

    tig_art_interface_id_create(art_num, 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    blit_info.art_id = art_id;
    blit_info.flags = 0;
    blit_info.src_rect = &src_rect;
    blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(window_handle, &blit_info);

    sub_568E70();
}

// 0x554BE0
tig_art_id_t sub_554BE0(int64_t obj)
{
    tig_art_id_t art_id = TIG_ART_ID_INVALID;
    int art_num;

    if (obj != OBJ_HANDLE_NULL) {
        art_num = sub_554C20(obj);
        tig_art_interface_id_create(art_num, 0, 0, 0, &art_id);
    }

    return art_id;
}

// 0x554C20
void sub_554C20()
{
    // TODO: Incomplete.
}

// 0x554F10
void sub_554F10()
{
    // TODO: Incomplete.
}

// 0x555780
void sub_555780(char* buffer, int num, int min, int max, int adj, bool a6)
{
    MesFileEntry mes_file_entry;
    char tmp[80];

    if (min == 0 && max == 0 && adj == 0) {
        return;
    }

    mes_file_entry.num = num;
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    strcat(buffer, mes_file_entry.str);
    strcat(buffer, ":");

    if (max != 0) {
        sprintf(tmp, "%d-%d", min, max);
        strcat(buffer, tmp);
    } else if (min != 0) {
        if (a6) {
            sprintf(tmp, "%+d", min);
        } else {
            sprintf("%d", min);
        }
        strcat(buffer, tmp);
    }

    if (adj != 0) {
        sprintf(tmp, "(%+d)", adj);
        strcat(buffer, tmp);
    }

    strcat(buffer, "  ");
}

// 0x555910
void sub_555910(int64_t obj, char* buffer)
{
    bool identified;
    int min;
    int max;
    int adj;

    identified = obj_field_int32_get(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) > 0
        && item_is_identified(obj);
    buffer[0] = '\0';

    // D
    min = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 0);
    max = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 0);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 0);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 43, min, max, adj, false);

    // FT
    min = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 4);
    max = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 4);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 4);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 44, min, max, adj, false);

    // TH
    min = obj_field_int32_get(obj, OBJ_F_WEAPON_BONUS_TO_HIT);
    if (identified) {
        adj = obj_field_int32_get(obj, OBJ_F_WEAPON_MAGIC_HIT_ADJ);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 45, min, 0, adj, true);

    // RNG
    min = obj_field_int32_get(obj, OBJ_F_WEAPON_RANGE);
    if (identified) {
        adj = obj_field_int32_get(obj, OBJ_F_WEAPON_MAGIC_RANGE_ADJ);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 46, min, 0, adj, false);

    // PD
    min = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 1);
    max = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 1);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 1);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 47, min, max, adj, false);

    // FD
    min = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 3);
    max = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 3);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 3);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 48, min, max, adj, false);

    // ED
    min = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_LOWER_IDX, 2);
    max = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_DAMAGE_UPPER_IDX, 2);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_WEAPON_MAGIC_DAMAGE_ADJ_IDX, 2);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 49, min, max, adj, false);
}

// 0x555B50
void sub_555B50(int64_t obj, char* buffer)
{
    bool identified;
    int value;
    int adj;

    identified = obj_field_int32_get(obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) > 0
        && item_is_identified(obj);
    buffer[0] = '\0';

    // AC
    value = obj_field_int32_get(obj, OBJ_F_ARMOR_AC_ADJ);
    if (identified) {
        adj = obj_field_int32_get(obj, OBJ_F_ARMOR_MAGIC_AC_ADJ);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 50, value, 0, adj, false);

    // DR
    value = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 0);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 0);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 51, value, 0, adj, true);

    // PR
    value = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 3);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 3);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 52, value, 0, adj, true);

    // FR
    value = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 1);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 1);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 53, value, 0, adj, true);

    // ER
    value = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 2);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 2);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 54, value, 0, adj, true);

    // MR
    value = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_RESISTANCE_ADJ_IDX, 4);
    if (identified) {
        adj = obj_arrayfield_int32_get(obj, OBJ_F_ARMOR_MAGIC_RESISTANCE_ADJ_IDX, 4);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 55, value, 0, adj, true);

    // NP
    value = obj_field_int32_get(obj, OBJ_F_ARMOR_SILENT_MOVE_ADJ);
    if (identified) {
        adj = obj_field_int32_get(obj, OBJ_F_ARMOR_MAGIC_SILENT_MOVE_ADJ);
    } else {
        adj = 0;
    }
    sub_555780(buffer, 56, value, 0, adj, true);

    // D
    if (item_armor_coverage(obj) == TIG_ART_ARMOR_COVERAGE_GAUNTLETS) {
        value = obj_field_int32_get(obj, OBJ_F_ARMOR_UNARMED_BONUS_DAMAGE);
    } else {
        value = 0;
    }
    sub_555780(buffer, 43, value, 0, 0, true);
}

// 0x555D80
void sub_555D80(int64_t a1, int64_t a2, const char* str)
{
    int art_num;
    char buffer[2000];

    if (str[0] != '\0') {
        sub_550930();

        if (sub_553D10(a1, a2, &art_num)) {
            sub_5561D0(a2, art_num, stru_5C6D60[intgame_iso_window_type].window_handle, 217, 69);
        } else {
            sub_554560(stru_5C6D60[intgame_iso_window_type].window_handle, art_num);
        }

        sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
            str,
            &stru_5C7118,
            dword_739F88,
            1);

        if ((obj_field_int32_get(a2, OBJ_F_FLAGS) & OF_INVULNERABLE) == 0) {
            sprintf(buffer, sub_43D600(a2), sub_43D5A0(a2));
            sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
                buffer,
                &stru_5C70F8,
                dword_64C49C,
                2);
        }

        if ((obj_field_int32_get(a2, OBJ_F_SCENERY_FLAGS) & OSCF_MARKS_TOWNMAP) != 0) {
            sub_564AF0(a2);
        }
    }
}

// 0x555EC0
void sub_555EC0()
{
    // TODO: Incomplete.
}

// 0x556040
void sub_556040()
{
    // TODO: Incomplete.
}

// 0x5561D0
void sub_5561D0()
{
    // TODO: Incomplete.
}

// 0x556220
void sub_556220()
{
    // TODO: Incomplete.
}

// 0x5566B0
void sub_5566B0(int64_t obj)
{
    MesFileEntry mes_file_entry;
    char buffer[MAX_STRING];
    int value;

    // Total Defense
    mes_file_entry.num = 63;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = sub_464700(obj);
    sprintf(buffer,
        "%s: %d",
        mes_file_entry.str,
        value);

    if (intgame_iso_window_type != 0) {
        sub_550770(-1, buffer);
        return;
    }

    sub_550930();
    sub_554560(stru_5C6D60[intgame_iso_window_type].window_handle, 674);
    sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70C8,
        dword_739F88,
        0x01);

    // Total AC
    mes_file_entry.num = 70;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = object_get_ac(obj, 1);
    if (value != 0) {
        sprintf(buffer,
            "%s: %d",
            mes_file_entry.str,
            value);
    } else {
        sprintf(buffer,
            "%s: 0",
            mes_file_entry.str);
    }
    sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70D8,
        dword_64C498,
        0x01);

    // Magic Resistance
    mes_file_entry.num = 65;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = sub_43D6D0(obj, RESISTANCE_TYPE_MAGIC, true);
    if (value != 0) {
        sprintf(buffer,
            "%s: %d%%",
            mes_file_entry.str,
            value);
    } else {
        sprintf(buffer,
            "%s: 0",
            mes_file_entry.str);
    }
    sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70E8,
        dword_64C498,
        0x01);

    // Fire Resistance
    mes_file_entry.num = 67;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = sub_43D6D0(obj, RESISTANCE_TYPE_FIRE, true);
    if (value != 0) {
        sprintf(buffer,
            "%s: %d%%",
            mes_file_entry.str,
            value);
    } else {
        sprintf(buffer,
            "%s: 0",
            mes_file_entry.str);
    }
    sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70F8,
        dword_64C498,
        0x01);

    // Damage Resistance
    mes_file_entry.num = 64;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = sub_43D6D0(obj, RESISTANCE_TYPE_NORMAL, true);
    if (value != 0) {
        sprintf(buffer,
            "%s: %d%%",
            mes_file_entry.str,
            value);
    } else {
        sprintf(buffer,
            "%s: 0",
            mes_file_entry.str);
    }
    sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70D8,
        dword_64C498,
        0x11);

    // Electrical Resistance
    mes_file_entry.num = 66;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = sub_43D6D0(obj, RESISTANCE_TYPE_ELECTRICAL, true);
    if (value != 0) {
        sprintf(buffer,
            "%s: %d%%",
            mes_file_entry.str,
            value);
    } else {
        sprintf(buffer,
            "%s: 0",
            mes_file_entry.str);
    }
    sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70E8,
        dword_64C498,
        0x11);

    // Poison Resistance
    mes_file_entry.num = 68;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = sub_43D6D0(obj, RESISTANCE_TYPE_POISON, true);
    if (value != 0) {
        sprintf(buffer,
            "%s: %d%%",
            mes_file_entry.str,
            value);
    } else {
        sprintf(buffer,
            "%s: 0",
            mes_file_entry.str);
    }
    sub_550A10(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70F8,
        dword_64C498,
        0x11);
}

// 0x556A90
void sub_556A90(int a1, bool a2)
{
    if (a2) {
        switch (a1) {
        case 0:
            if (charedit_is_created()) {
                return;
            }
            break;
        case 1:
            if (logbook_ui_is_created()) {
                return;
            }
            break;
        case 2:
        case 3:
            if (wmap_ui_is_created()) {
                return;
            }
            break;
        case 4:
            if (inven_ui_is_created()) {
                return;
            }
            break;
        }

        dword_64C484[a1] = dword_5C6510[a1];
        sub_556B90(a1);
    } else {
        dword_64C484[a1] = dword_5C6524[a1];
        sub_556B90(a1);
    }
}

// 0x556B70
void sub_556B70(int a1)
{
    dword_64C534 = a1;
    sub_556B90(a1);
}

// 0x556B90
void sub_556B90(int a1)
{
    tig_button_handle_t button_handle;
    tig_art_id_t art_id;

    switch (a1) {
    case 0:
        button_handle = stru_5C64C0[1].button_handle;
        break;
    case 1:
        button_handle = stru_5C64C0[0].button_handle;
        break;
    case 2:
        if (dword_64C534 != 2) {
            return;
        }

        button_handle = stru_5C64C0[3].button_handle;
        break;
    case 3:
        if (dword_64C534 != 3) {
            return;
        }

        button_handle = stru_5C64C0[3].button_handle;
        break;
    case 4:
        button_handle = stru_5C64C0[2].button_handle;
        break;
    }

    tig_art_interface_id_create(dword_64C484[a1], 0, 0, 0, &art_id);
    tig_button_set_art(button_handle, art_id);
}

// 0x556C20
void sub_556C20()
{
    // TODO: Incomplete.
}

// 0x556E60
void sub_556E60()
{
    int64_t parent_obj;

    if (qword_64C688 != OBJ_HANDLE_NULL) {
        parent_obj = obj_field_handle_get(qword_64C688, OBJ_F_ITEM_PARENT);
        if (parent_obj == OBJ_HANDLE_NULL || !player_is_pc_obj(parent_obj)) {
            qword_64C688 = OBJ_HANDLE_NULL;
        }
    }
}

// 0x556EA0
void sub_556EA0(int64_t item_obj)
{
    if (item_obj != OBJ_HANDLE_NULL) {
        qword_64C688 = item_obj;
    } else {
        qword_64C688 = item_wield_get(player_get_pc_obj(), 1004);
    }
    sub_5506C0(8);
}

// 0x556EF0
void intgame_mt_button_enable()
{
    bool hidden;
    int64_t obj;
    int64_t item_obj;
    int mana_store;
    unsigned int flags;

    if (tig_button_is_hidden(intgame_mt_button_info.button_handle, &hidden) == TIG_OK && hidden) {
        obj = player_get_pc_obj();
        item_obj = item_wield_get(obj, 1004);
        if (item_obj != OBJ_HANDLE_NULL) {
            mana_store = obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_MANA_STORE);
            flags = obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS);
            if ((mana_store != 0 || (flags & OIF_IS_MAGICAL) != 0)
                && obj_field_int32_get(item_obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) > 0) {
                tig_button_show(intgame_mt_button_info.button_handle);
            }
        }
    }
}

// 0x556F80
void intgame_mt_button_disable()
{
    bool hidden;
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect rect;

    if (tig_button_is_hidden(intgame_mt_button_info.button_handle, &hidden) == TIG_OK && !hidden) {
        tig_button_hide(intgame_mt_button_info.button_handle);

        if (tig_art_interface_id_create(intgame_mt_button_info.art_num, 0, 0, 0, &art_id) != TIG_OK) {
            tig_debug_printf("Intgame: intgame_mt_button_disable: ERROR: Can't find Interface Art: %d!\n", intgame_mt_button_info.art_num);
            return;
        }

        tig_art_frame_data(art_id, &art_frame_data);

        if (tig_art_interface_id_create(184, 0, 0, 0, &art_id) != TIG_OK) {
            tig_debug_printf("Intgame: intgame_mt_button_disable: ERROR: Can't find Interface Art: %d!\n", 184);
        }

        rect.x = intgame_mt_button_info.x - stru_5C6390[1].x;
        rect.y = intgame_mt_button_info.y - stru_5C6390[1].y;
        rect.width = art_frame_data.width;
        rect.height = art_frame_data.height;

        art_blit_info.flags = 0;
        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &rect;
        art_blit_info.dst_rect = &rect;
        tig_window_blit_art(dword_64C4F8[1], &art_blit_info);
    }
}

// 0x5570A0
void sub_5570A0(int64_t obj)
{
    qword_64C690 = obj;
    if (qword_64C690 != OBJ_HANDLE_NULL) {
        sub_57CCF0(player_get_pc_obj(), qword_64C690);
    }
}

// 0x5570D0
void sub_5570D0()
{
    // TODO: Incomplete.
}

// 0x5571C0
void sub_5571C0()
{
    // TODO: Incomplete.
}

// 0x557230
bool intgame_big_window_create()
{
    TigWindowData window_data;

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect.x = 0;
    window_data.rect.y = 41;
    window_data.rect.width = 800;
    window_data.rect.height = 400;
    window_data.background_color = 0;
    window_data.message_filter = intgame_big_window_message_filter;
    if (tig_window_create(&window_data, &intgame_big_window_handle) != TIG_OK) {
        return false;
    }

    tig_window_hide(intgame_big_window_handle);
    intgame_big_window_locked = false;

    return true;
}

// 0x5572B0
void intgame_big_window_destroy()
{
    tig_window_destroy(intgame_big_window_handle);
}

// 0x5572C0
bool intgame_big_window_message_filter(TigMessage* msg)
{
    return false;
}

// 0x5572D0
bool intgame_big_window_lock(TigWindowMessageFilterFunc* func, tig_window_handle_t* window_handle_ptr)
{
    if (intgame_big_window_locked) {
        return false;
    }

    intgame_big_window_locked = true;
    tig_window_message_filter_set(intgame_big_window_handle, func);
    tig_window_show(intgame_big_window_handle);
    sub_51E850(intgame_big_window_handle);
    *window_handle_ptr = intgame_big_window_handle;

    return true;
}

// 0x557330
void intgame_big_window_unlock()
{
    intgame_big_window_locked = false;
    tig_window_button_destroy(intgame_big_window_handle);
    tig_window_message_filter_set(intgame_big_window_handle, intgame_big_window_message_filter);
    tig_window_hide(intgame_big_window_handle);
}

// 0x557370
void sub_557370()
{
    // TODO: Incomplete.
}

// 0x557670
void sub_557670()
{
    MesFileEntry mes_file_entry;
    John v1;

    mes_file_entry.num = 2000;
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    v1.type = 6;
    v1.str = mes_file_entry.str;
    sub_550750(&v1);
}

// 0x5576B0
void sub_5576B0()
{
    TigRect rect;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &rect;
    art_blit_info.dst_rect = &rect;
    tig_art_interface_id_create(185, 0, 0, 0, &(art_blit_info.art_id));

    tig_art_frame_data(art_blit_info.art_id, &art_frame_data);

    rect.x = 0;
    rect.y = 0;
    rect.width = art_frame_data.width;
    rect.height = art_frame_data.height;
    tig_window_blit_art(dword_64C4F8[0], &art_blit_info);
}

// 0x557730
void sub_557730(int index)
{
    MesFileEntry mes_file_entry;
    John v1;

    mes_file_entry.num = index + 3000;
    if (mes_search(intgame_mes_file, &mes_file_entry)) {
        mes_get_msg(intgame_mes_file, &mes_file_entry);

        v1.type = 6;
        v1.str = mes_file_entry.str;
        sub_550750(&v1);
    }
}

// 0x557790
void sub_557790(int64_t obj)
{
    if (obj != OBJ_HANDLE_NULL
        && obj == sub_43C570()) {
        sub_57CCF0(player_get_pc_obj(), obj);
    }
}

// 0x5577D0
unsigned int intgame_get_iso_window_flags()
{
    return intgame_iso_window_flags;
}

// 0x5577E0
void intgame_set_iso_window_flags(unsigned int flags)
{
    intgame_iso_window_flags = flags;
}

// 0x5577F0
void intgame_set_iso_window_width(int width)
{
    intgame_iso_window_width = width;
}

// 0x557800
void intgame_set_iso_window_height(int height)
{
    intgame_iso_window_height = height;
}

// 0x557810
bool intgame_create_iso_window(tig_window_handle_t* window_handle_ptr)
{
    TigWindowData window_data;

    window_data.flags = intgame_iso_window_flags | TIG_WINDOW_FLAG_0x80 | TIG_WINDOW_VIDEO_MEMORY;
    window_data.rect.x = 0;
    window_data.rect.width = intgame_iso_window_width;

    if (intgame_is_compact_interface()) {
        window_data.rect.y = 0;
        window_data.rect.height = intgame_iso_window_height;
    } else {
        window_data.rect.y = 41;
        window_data.rect.height = 400;
    }

    window_data.background_color = 0;

    if (tig_window_create(&window_data, window_handle_ptr) != TIG_OK) {
        tig_debug_printf("intgame_create_iso_window: ERROR: window create failed!\n");
        tig_exit();
        return false;
    }

    intgame_iso_window = *window_handle_ptr;
    return true;
}

// 0x5578C0
bool intgame_is_compact_interface()
{
    return intgame_compact_interface;
}

// 0x5578D0
void intgame_set_fullscreen()
{
    intgame_fullscreen = true;
}

// 0x5578E0
void intgame_toggle_interface()
{
    TigWindowData window_data;
    ResizeInfo resize_info;
    int index;

    if (!intgame_fullscreen) {
        return;
    }

    tig_debug_printf("Resizing Iso View...");

    resize_info.iso_window_handle = dword_64C52C;
    intgame_compact_interface = !intgame_compact_interface;

    if (intgame_iso_window != TIG_WINDOW_HANDLE_INVALID) {
        if (tig_window_data(dword_64C52C, &window_data) != TIG_OK) {
            tig_debug_printf("intgame_toggle_interface: ERROR: tig_window_data failed!\n");
            exit(EXIT_FAILURE);
        }

        resize_info.field_4 = window_data.rect;
        resize_info.field_14 = window_data.rect;

        if (!intgame_is_compact_interface()) {
            resize_info.field_14.x = 0;
            resize_info.field_14.y = 41;
        }

        if (intgame_compact_interface) {
            stru_64C510 = stru_5C63C0;

            for (index = 0; index < 2; index++) {
                tig_window_hide(dword_64C4F8[index]);
            }

            gamelib_resize(&resize_info);
            gameuilib_resize(&resize_info);

            compact_ui_create();
        } else {
            stru_64C510 = stru_5C63B0;

            resize_info.field_4.height = 400;
            resize_info.field_14.height = 400;

            gamelib_resize(&resize_info);
            gameuilib_resize(&resize_info);

            for (index = 0; index < 2; index++) {
                tig_window_show(dword_64C4F8[index]);
            }

            compat_ui_destroy();
        }
    }

    tig_debug_printf("completed.\n");
}

// 0x557AA0
int sub_557AA0()
{
    return intgame_iso_window_type;
}

// 0x557AB0
int sub_557AB0()
{
    return dword_64C530;
}

// 0x557AC0
void sub_557AC0(int group, int index, UiButtonInfo* button_info)
{
    if (button_info != NULL) {
        *button_info = stru_5C6718[group * 5 + index];
    }
}

// 0x557B00
int64_t sub_557B00()
{
    return qword_64C688;
}

// 0x557B10
mes_file_handle_t sub_557B10()
{
    return intgame_mes_file;
}

// 0x557B20
UiButtonInfo* sub_557B20(int index)
{
    return &(stru_5C6F40[index]);
}

// 0x557B30
void sub_557B30(int index, int x, int y)
{
    stru_5C6F40[index].x = x;
    stru_5C6F40[index].y = y;
}

// 0x557B50
int sub_557B50(int index)
{
    return dword_5C6F60[index];
}

// 0x557B60
int sub_557B60()
{
    TigMouseState mouse_state;
    TigButtonData button_data;
    int x;
    int y;
    int index;

    if (intgame_iso_window_type == 2) {
        tig_mouse_get_state(&mouse_state);
        x = mouse_state.x - stru_5C6390[1].x;
        y = mouse_state.y - stru_5C6390[1].y;

        for (index = 0; index < 4; index++) {
            if (tig_button_data(stru_5C6C68[index].button_handle, &button_data) != TIG_OK) {
                break;
            }

            if (x >= button_data.x
                && y >= button_data.y
                && x < button_data.x + button_data.width
                && y < button_data.y + button_data.height) {
                return index;
            }
        }
    }

    return 4;
}

// 0x557C00
void sub_557C00()
{
    // TODO: Incomplete.
}

// 0x557CF0
void sub_557CF0()
{
    // TODO: Incomplete.
}
