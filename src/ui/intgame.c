#include "ui/intgame.h"

#include "game/gamelib.h"
#include "game/item.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/mes.h"
#include "game/obj.h"
#include "game/player.h"
#include "game/spell.h"
#include "game/stat.h"
#include "ui/compact_ui.h"
#include "ui/gameuilib.h"
#include "ui/roller_ui.h"

typedef struct IntgameIsoWindowTypeInfo {
    /* 0000 */ TigRect rect;
    /* 0010 */ tig_window_handle_t window_handle;
} IntgameIsoWindowTypeInfo;

static_assert(sizeof(IntgameIsoWindowTypeInfo) == 0x14, "wrong size");

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
static void sub_556EA0(int64_t item_obj);
static void intgame_mt_button_enable();
static void intgame_mt_button_disable();
static bool intgame_big_window_create();
static void intgame_big_window_destroy();
static bool intgame_big_window_message_filter(TigMessage* msg);

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

// 0x5C6F68
static UiButtonInfo intgame_mt_button_info = { 161, 443, 563, TIG_BUTTON_HANDLE_INVALID };

// 0x5C6F78
static int dword_5C6F78 = 6;

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
    // TODO: Incomplete.
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
    // TODO: Incomplete.
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
void sub_54B5D0()
{
    // TODO: Incomplete.
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
void sub_54DC80()
{
    // TODO: Incomplete.
}

// 0x54DE50
void sub_54DE50()
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
void sub_551A80(int a1)
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
            sub_5597C0(obj, a2);
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
    sub_4C96C0();
    sub_551A80(0);
    sub_551A80(3);

    return true;
}

// 0x553350
void sub_553350()
{
    dword_64C6CC = NULL;
    sub_4C96F0();
    sub_551A80(0);
}

// 0x553370
void sub_553370()
{
    sub_4C9720();
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
void sub_554B00()
{
    // TODO: Incomplete.
}

// 0x554BE0
void sub_554BE0()
{
    // TODO: Incomplete.
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
void sub_555780()
{
    // TODO: Incomplete.
}

// 0x555910
void sub_555910()
{
    // TODO: Incomplete.
}

// 0x555B50
void sub_555B50()
{
    // TODO: Incomplete.
}

// 0x555D80
void sub_555D80()
{
    // TODO: Incomplete.
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
void sub_5566B0()
{
    // TODO: Incomplete.
}

// 0x556A90
void sub_556A90()
{
    // TODO: Incomplete.
}

// 0x556B70
void sub_556B70()
{
    // TODO: Incomplete.
}

// 0x556B90
void sub_556B90()
{
    // TODO: Incomplete.
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

        if (tig_art_interface_id(184, 0, 0, 0, &art_id) != TIG_OK) {
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
void sub_5570A0()
{
    // TODO: Incomplete.
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
    window_data.rect.y = 0;
    window_data.rect.width = 0;
    window_data.rect.height = 0;
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
    // TODO: Incomplete.
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
void sub_557730()
{
    // TODO: Incomplete.
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
void sub_557AC0()
{
    // TODO: Incomplete.
}

// 0x557B00
void sub_557B00()
{
    // TODO: Incomplete.
}

// 0x557B10
void sub_557B10()
{
    // TODO: Incomplete.
}

// 0x557B20
void sub_557B20()
{
    // TODO: Incomplete.
}

// 0x557B30
void sub_557B30()
{
    // TODO: Incomplete.
}

// 0x557B50
void sub_557B50()
{
    // TODO: Incomplete.
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
