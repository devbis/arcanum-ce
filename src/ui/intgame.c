#include "ui/intgame.h"

#include <stdio.h>

#include "game/ai.h"
#include "game/anim.h"
#include "game/bless.h"
#include "game/broadcast.h"
#include "game/combat.h"
#include "game/critter.h"
#include "game/curse.h"
#include "game/gamelib.h"
#include "game/gsound.h"
#include "game/item_effect.h"
#include "game/item.h"
#include "game/level.h"
#include "game/light.h"
#include "game/magictech.h"
#include "game/map.h"
#include "game/mt_item.h"
#include "game/obj_private.h"
#include "game/obj.h"
#include "game/object.h"
#include "game/player.h"
#include "game/portrait.h"
#include "game/proto.h"
#include "game/reaction.h"
#include "game/resistance.h"
#include "game/scroll.h"
#include "game/skill.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/target.h"
#include "game/tc.h"
#include "game/tech.h"
#include "game/timeevent.h"
#include "ui/anim_ui.h"
#include "ui/broadcast_ui.h"
#include "ui/charedit_ui.h"
#include "ui/compact_ui.h"
#include "ui/dialog_ui.h"
#include "ui/fate_ui.h"
#include "ui/follower_ui.h"
#include "ui/gameuilib.h"
#include "ui/hotkey_ui.h"
#include "ui/inven_ui.h"
#include "ui/iso.h"
#include "ui/item_ui.h"
#include "ui/logbook_ui.h"
#include "ui/mainmenu_ui.h"
#include "ui/mp_ctrl_ui.h"
#include "ui/multiplayer_ui.h"
#include "ui/roller_ui.h"
#include "ui/schematic_ui.h"
#include "ui/scrollbar_ui.h"
#include "ui/skill_ui.h"
#include "ui/sleep_ui.h"
#include "ui/spell_ui.h"
#include "ui/tb_ui.h"
#include "ui/textedit_ui.h"
#include "ui/wmap_ui.h"
#include "ui/written_ui.h"

typedef enum IntgameCounter {
    INTGAME_COUNTER_HEALTH,
    INTGAME_COUNTER_FATIGUE,
    INTGAME_COUNTER_FATE,
    INTGAME_COUNTER_MONEY,
    INTGAME_COUNTER_MANA,
    INTGAME_COUNTER_POISON,
    INTGAME_COUNTER_COUNT,
} IntgameCounter;

typedef enum IntgamePrimaryButton {
    INTGAME_PRIMARY_BUTTON_LOGBOOK,
    INTGAME_PRIMARY_BUTTON_CHAR,
    INTGAME_PRIMARY_BUTTON_INVENTORY,
    INTGAME_PRIMARY_BUTTON_MAP,
    INTGAME_PRIMARY_BUTTON_FATE,
    INTGAME_PRIMARY_BUTTON_COUNT,
} IntgamePrimaryButton;

typedef enum IntgamePenalty {
    INTGAME_PENALTY_MSR,
    INTGAME_PENALTY_RANGE,
    INTGAME_PENALTY_PERCEPTION,
    INTGAME_PENALTY_COVER,
    INTGAME_PENALTY_LIGHT,
    INTGAME_PENALTY_INJURY,
    INTGAME_PENALTY_BLOCKED_SHOT,
    INTGAME_PENALTY_MAGIC_TECH,
    INTGAME_PENALTY_COUNT,
} IntgamePenalty;

#define INTGAME_PENALTY_SLOTS 6

typedef struct IntgameIsoWindowTypeInfo {
    /* 0000 */ TigRect rect;
    /* 0010 */ tig_window_handle_t window_handle;
} IntgameIsoWindowTypeInfo;

static_assert(sizeof(IntgameIsoWindowTypeInfo) == 0x14, "wrong size");

#define MSG_TEXT_HALIGN_LEFT 0x01u
#define MSG_TEXT_HALIGN_RIGHT 0x02u
#define MSG_TEXT_HALIGN_CENTER 0x04u
#define MSG_TEXT_VALIGN_CENTER 0x08u
#define MSG_TEXT_SECONDARY 0x10u
#define MSG_TEXT_TRUNCATE 0x20u

static bool button_create_flags(UiButtonInfo* button_info, unsigned int flags);
static bool button_create_no_art(UiButtonInfo* button_info, int width, int height);
static void intgame_draw_counter(int counter, int value, int digits);
static void intgame_draw_bar_rect(TigRect* rect);
static void intgame_ammo_icon_refresh(tig_art_id_t art_id);
static bool iso_interface_message_filter(TigMessage* msg);
static void sub_54DBF0(int btn, int window_type);
static void sub_54EB60();
static void sub_54EBF0();
static void sub_54ECD0();
static void sub_54ED30(S4F2810* a1);
static void sub_550000(int64_t critter_obj, Hotkey* a2, int inventory_location);
static bool sub_5501C0();
static bool sub_5503F0(int a1, int a2);
static void iso_interface_window_disable(int window_type);
static void intgame_message_window_write_text_centered(char* str, TigRect* rect);
static bool intgame_message_window_write_text(tig_window_handle_t window_handle, char* str, TigRect* rect, tig_font_handle_t font, unsigned int flags);
static bool intgame_spells_init();
static void intgame_spells_show_college_spells(int group);
static void intgame_spells_hide_college_spells(int group);
static bool sub_550D20();
static void sub_550D60();
static void iso_interface_window_enable(int window_type);
static void sub_551660();
static int sub_551740(int x, int y);
static void sub_5517F0();
static bool sub_5518C0(int x, int y);
static void sub_551910(TigMessage* msg);
static void sub_551A10(int64_t obj);
static void intgame_force_fullscreen();
static void intgame_unforce_fullscreen();
static void sub_551F80();
static bool sub_552050(TigMouseMessageData* a1, S4F2810* a2);
static void sub_5520D0(int window_type, int a2);
static void sub_552130(int window_type);
static void intgame_clock_refresh();
static void sub_552740(int64_t obj, ChareditMode mode);
static void sub_552770(UiMessage* ui_message);
static void sub_5528E0();
static void sub_552930();
static void sub_552960(bool play_sound);
static void sub_5529C0(tig_window_handle_t window_handle, UiMessage* ui_message, bool play_sound);
static void sub_5533C0(UiButtonInfo* button, int index, tig_art_id_t art_id, tig_window_handle_t window_handle);
static void intgame_spell_maintain_refresh_func(tig_button_handle_t button_handle, UiButtonInfo* info, int num, bool a4, tig_window_handle_t window_handle);
static void sub_553960();
static void sub_553A70(TigMessage* msg);
static void intgame_examine_critter(int64_t pc_obj, int64_t critter_obj, char* str);
static void intgame_message_window_draw_image(tig_window_handle_t window_handle, int num);
static void sub_554640(int a1, int a2, TigRect* rect, int value);
static void sub_554830(int64_t a1, int64_t a2);
static void sub_554B00(tig_window_handle_t window_handle, int art_num, int x, int y);
static int sub_554C20(int64_t item_obj);
static void intgame_examine_item(int64_t pc_obj, int64_t item_obj, char* str);
static void sub_555780(char* buffer, int num, int min, int max, int a5, bool a6);
static void sub_555910(int64_t obj, char* buffer);
static void sub_555B50(int64_t obj, char* buffer);
static void intgame_examine_scenery(int64_t pc_obj, int64_t scenery_obj, char* str);
static void intgame_examine_portal(int64_t pc_obj, int64_t portal_obj, char* str);
static void intgame_examine_container(int64_t pc_obj, int64_t container_obj, char* str);
static void intgame_draw_portrait(int64_t obj, int portrait, tig_window_handle_t window_handle, int x, int y);
static void intgame_refresh_primary_button(UiPrimaryButton btn);
static void intgame_refresh_experience_gauges(int64_t obj);
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
static TigRect intgame_health_bar_frame = { 14, 472, 28, 88 };

// 0x5C63E8
static TigRect intgame_fatigue_bar_frame = { 754, 473, 28, 88 };

// 0x5C63F8
static IntgameIsoWindowTypeInfo intgame_number_boxes[INTGAME_COUNTER_COUNT] = {
    /*  HEALTH */ { { 15, 578, 29, 12 }, TIG_WINDOW_HANDLE_INVALID },
    /* FATIGUE */ { { 755, 578, 27, 12 }, TIG_WINDOW_HANDLE_INVALID },
    /*    FATE */ { { 190, 17, 24, 12 }, TIG_WINDOW_HANDLE_INVALID },
    /*   MONEY */ { { 104, 512, 50, 20 }, TIG_WINDOW_HANDLE_INVALID },
    /*    MANA */ { { 264, 562, 50, 20 }, TIG_WINDOW_HANDLE_INVALID },
    /*  POISON */ { { 15, 500, 29, 12 }, TIG_WINDOW_HANDLE_INVALID },
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
static UiButtonInfo intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_COUNT] = {
    /*   INTGAME_PRIMARY_BUTTON_LOGBOOK */ { 41, 2, 187, TIG_BUTTON_HANDLE_INVALID },
    /*      INTGAME_PRIMARY_BUTTON_CHAR */ { 4, 2, 169, TIG_BUTTON_HANDLE_INVALID },
    /* INTGAME_PRIMARY_BUTTON_INVENTORY */ { 115, 2, 186, TIG_BUTTON_HANDLE_INVALID },
    /*       INTGAME_PRIMARY_BUTTON_MAP */ { 78, 2, 193, TIG_BUTTON_HANDLE_INVALID },
    /*      INTGAME_PRIMARY_BUTTON_FATE */ { 157, 9, 137, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C6510
static int intgame_ui_primary_button_highlighted_icons[UI_PRIMARY_BUTTON_COUNT] = {
    /*      UI_PRIMARY_BUTTON_CHAR */ 561, // "char_on.art"
    /*   UI_PRIMARY_BUTTON_LOGBOOK */ 560, // "log_on.art"
    /*   UI_PRIMARY_BUTTON_TOWNMAP */ 558, // "tmap_on.art"
    /*  UI_PRIMARY_BUTTON_WORLDMAP */ 195, // "wmap_on.art"
    /* UI_PRIMARY_BUTTON_INVENTORY */ 559, // "invn_on.art"
};

// 0x5C6524
static int intgame_ui_primary_button_normal_icons[UI_PRIMARY_BUTTON_COUNT] = {
    /*      UI_PRIMARY_BUTTON_CHAR */ 169, // "char_but.art"
    /*   UI_PRIMARY_BUTTON_LOGBOOK */ 187, // "log_but.art"
    /*   UI_PRIMARY_BUTTON_TOWNMAP */ 193, // "tmap_but.art"
    /*  UI_PRIMARY_BUTTON_WORLDMAP */ 194, // "wmap_but.art"
    /* UI_PRIMARY_BUTTON_INVENTORY */ 186, // "invn_but.art"
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
static UiButtonInfo intgame_college_buttons[COLLEGE_COUNT] = {
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
static UiButtonInfo intgame_spell_buttons[SPELL_COUNT] = {
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

// 0x5C6FA0
static int dword_5C6FA0[RACE_COUNT] = {
    375, // racehumanicon.art
    378, // racehelficon.art
    376, // raceelficon.art
    377, // racehelficon.art
    379, // racegnomeicon.art
    380, // racehalflingicon.art
    381, // racehorcicon.art
    382, // racehogreicon.art
    376, // raceelficon.art
    382, // racehogreicon.art
    381, // racehorcicon.art
};

// 0x5C6FCC
static int intgame_alignment_icons[6] = {
    634, // "icon_lawfulevil.art"
    390, // "icon_evil.art"
    389, // "icon_chaotic.art"
    388, // "icon_lawful.art"
    387, // "icon_good.art"
    386, // "icon_lawfulgood.art"
};

// 0x5C6FE4
static int dword_5C6FE4[15] = {
    391,
    391,
    391,
    391,
    391,
    391,
    392,
    391,
    395,
    391,
    392,
    391,
    391,
    391,
    391,
};

// 0x5C7020
static int dword_5C7020[4] = {
    398,
    399,
    400,
    401,
};

// 0x5C7030
static int dword_5C7030[9] = {
    402,
    402,
    405,
    405,
    405,
    408,
    405,
    405,
    402,
};

// 0x5C7054
static int dword_5C7054[7] = {
    0,
    405,
    405,
    405,
    405,
    411,
    414,
};

// 0x5C7070
static int intgame_written_icons[WRITTEN_TYPE_COUNT] = {
    /*      WRITTEN_TYPE_BOOK */ 428, // "booksicon.art"
    /*      WRITTEN_TYPE_NOTE */ 429, // "note_icon.art"
    /* WRITTEN_TYPE_NEWSPAPER */ 431, // "newpaper_icon.art"
    /*  WRITTEN_TYPE_TELEGRAM */ 430, // "telegram_icon.art"
    /*     WRITTEN_TYPE_IMAGE */ 428, // "booksicon.art"
    /* WRITTEN_TYPE_SCHEMATIC */ 462, // "schematic_icon.art"
    /*    WRITTEN_TYPE_PLAQUE */ 428, // "booksicon.art"
};

// 0x5C708C
static int dword_5C708C[] = {
    438, // "levelupicon.art" - Level Up Icon
    439, // "poisoned_icon.art" - Poisoned Icon
    440, // "hexedicon.art" - Cursed Icon
    441, // "blessicon.art" - Blessed Icon
    442, // "exclaimation_icon.art" - Exclamation Icon
    443, // "question_icon.art" - Question Icon
    444, // "levelupicon.art" - Arcanum Icon
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

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

// 0x5C7270
static UiButtonInfo stru_5C7270 = { 216, 47, 772, TIG_BUTTON_HANDLE_INVALID };

// 0x5C7280
static uint64_t qword_5C7280 = Tgt_Object;

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

// 0x5C730C
static int dword_5C730C[8] = {
    0,
    4,
    9,
    13,
    14,
    18,
    23,
    27,
};

// 0x5C732C
static unsigned int intgame_penalty_flags[INTGAME_PENALTY_COUNT] = {
    /*          INTGAME_PENALTY_MSR */ SKILL_INVOCATION_PENALTY_MSR,
    /*        INTGAME_PENALTY_RANGE */ SKILL_INVOCATION_PENALTY_RANGE,
    /*   INTGAME_PENALTY_PERCEPTION */ SKILL_INVOCATION_PENALTY_PERCEPTION,
    /*        INTGAME_PENALTY_COVER */ SKILL_INVOCATION_PENALTY_COVER,
    /*        INTGAME_PENALTY_LIGHT */ SKILL_INVOCATION_PENALTY_LIGHT,
    /*       INTGAME_PENALTY_INJURY */ SKILL_INVOCATION_PENALTY_INJURY,
    /* INTGAME_PENALTY_BLOCKED_SHOT */ SKILL_INVOCATION_BLOCKED_SHOT,
    /*   INTGAME_PENALTY_MAGIC_TECH */ SKILL_INVOCATION_MAGIC_TECH_PENALTY,
};

// 0x5C734C
static int intgame_penalty_icons[INTGAME_PENALTY_COUNT] = {
    /*          INTGAME_PENALTY_MSR */ 586, // "pen_msr.art"
    /*        INTGAME_PENALTY_RANGE */ 588, // "pen_range.art"
    /*   INTGAME_PENALTY_PERCEPTION */ 587, // "pen_perception.art"
    /*        INTGAME_PENALTY_COVER */ 583, // "pen_cover.art"
    /*        INTGAME_PENALTY_LIGHT */ 585, // "pen_light.art"
    /*       INTGAME_PENALTY_INJURY */ 584, // "pen_injury.art"
    /* INTGAME_PENALTY_BLOCKED_SHOT */ 845, // "blockedshot.art"
    /*   INTGAME_PENALTY_MAGIC_TECH */ 846, // "magic-tech-penalty.art"
};

// 0x5C736C
static int intgame_penalty_slot_x[INTGAME_PENALTY_SLOTS] = {
    210,
    248,
    210,
    248,
    210,
    248,
};

// 0x5C7384
static int intgame_penalty_slot_y[INTGAME_PENALTY_SLOTS] = {
    84,
    84,
    104,
    104,
    124,
    124,
};

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
static int intgame_ui_primary_button_icons[UI_PRIMARY_BUTTON_COUNT];

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
static PcLens intgame_pc_lens;

// 0x64C52C
static tig_window_handle_t dword_64C52C;

// 0x64C530
static int dword_64C530;

// 0x64C534
static UiPrimaryButton intgame_map_button;

// 0x64C538
static tig_font_handle_t dword_64C538;

// 0x64C540
static UiMessage stru_64C540[10];

// 0x64C630
static bool intgame_big_window_locked;

// 0x64C634
static int dword_64C634[11];

// 0x64C660
static TigRect intgame_pc_lens_rect;

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
static int intgame_pc_lens_mode;

// 0x64C6C0
static int dword_64C6C0;

// 0x64C6C4
static int dword_64C6C4;

// 0x64C6C8
static int dword_64C6C8;

// 0x64C6CC
static bool(*dword_64C6CC)(TigMessage* msg);

// 0x64C6D0
static int dword_64C6D0;

// 0x64C6D4
static void(*dword_64C6D4)(UiMessage* ui_message);

// 0x64C6D8
static int dword_64C6D8;

// 0x64C6DC
static bool intgame_fullscreen_forced;

// 0x64C6E0
static bool dword_64C6E0;

// 0x64C6E4
static TigVideoBuffer* intgame_pc_lens_video_buffer;

// 0x64C6E8
static bool dword_64C6E8;

// 0x64C6F0
static unsigned int intgame_iso_window_flags;

// 0x739F88
tig_font_handle_t dword_739F88;

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
    tig_font_create(&font, &dword_739F88);

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(100, 100, 255);
    tig_font_create(&font, &dword_64C470);

    font.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 114, 0);
    tig_font_create(&font, &dword_64C538);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create(&font, &dword_64C498);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 0, 0);
    tig_font_create(&font, &dword_64C49C);

    font.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(0, 0, 255);
    tig_font_create(&font, &dword_64C500);

    font.flags = 0;
    tig_art_interface_id_create(230, 0, 0, 0, &(font.art_id));
    font.str = NULL;
    font.color = tig_color_make(255, 255, 255);
    tig_font_create(&font, &dword_64C4A0);

    memcpy(intgame_ui_primary_button_icons, intgame_ui_primary_button_normal_icons, sizeof(intgame_ui_primary_button_icons));
    intgame_map_button = UI_PRIMARY_BUTTON_TOWNMAP;
    dword_64C674 = -1;

    return true;
}

// 0x549F00
void intgame_reset()
{
    int index;

    dword_64C6D8 = 0;
    intgame_refresh_cursor();
    sub_57DAB0();
    intgame_clock_process_callback(NULL);
    sub_552130(0);

    for (index = 0; index < 10; index++) {
        sub_57F210(index);
    }

    intgame_map_button = UI_PRIMARY_BUTTON_TOWNMAP;
    memcpy(intgame_ui_primary_button_icons, intgame_ui_primary_button_normal_icons, sizeof(intgame_ui_primary_button_icons));
    sub_54AA30();
}

// 0x549F60
void intgame_resize(GameResizeInfo* resize_info)
{
    int index;
    TigWindowData window_data;
    TigRect rect;

    sub_57DA50();
    dword_64C52C = resize_info->window_handle;

    if (dword_5C7288 != TIG_WINDOW_HANDLE_INVALID) {
        tig_window_destroy(dword_5C7288);
        dword_5C7288 = TIG_WINDOW_HANDLE_INVALID;
    }

    if (resize_info->content_rect.height == 400) {
        hotkey_ui_start(dword_64C4F8[1], &(stru_5C6390[1]), dword_64C4F8[1], false);

        for (index = 0; index < 5; index++) {
            tig_window_hide(dword_5C6378[index]);
        }
    } else {
        window_data.flags = TIG_WINDOW_ALWAYS_ON_TOP;
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
    if (!hotkey_ui_save(stream)) return false;
    if (tig_file_fwrite(intgame_ui_primary_button_icons, sizeof(*intgame_ui_primary_button_icons), UI_PRIMARY_BUTTON_COUNT, stream) != UI_PRIMARY_BUTTON_COUNT) return false;
    if (tig_file_fwrite(&intgame_map_button, sizeof(intgame_map_button), 1, stream) != 1) return false;

    return true;
}

// 0x54A220
bool intgame_load(GameLoadInfo* load_info)
{
    int v1;
    int btn;
    int64_t obj;

    if (load_info->stream == NULL) return false;
    if (tig_file_fread(&v1, sizeof(v1), 1, load_info->stream) != 1) return false;
    if (tig_file_fread(&dword_64C530, sizeof(dword_64C530), 1, load_info->stream) != 1) return false;

    if (intgame_iso_window_type == 1) {
        tig_button_state_change(intgame_college_buttons[dword_64C530].button_handle, TIG_BUTTON_STATE_PRESSED);
    }

    if (!hotkey_ui_load(load_info)) return false;
    if (tig_file_fread(intgame_ui_primary_button_icons, sizeof(*intgame_ui_primary_button_icons), UI_PRIMARY_BUTTON_COUNT, load_info->stream) != UI_PRIMARY_BUTTON_COUNT) return false;
    if (tig_file_fread(&intgame_map_button, sizeof(intgame_map_button), 1, load_info->stream) != 1) return false;

    for (btn = 0; btn < UI_PRIMARY_BUTTON_COUNT; btn++) {
        intgame_refresh_primary_button(btn);
    }

    sub_54B3C0();
    intgame_refresh_cursor();
    intgame_mt_button_enable();

    obj = player_get_local_pc_obj();
    if (obj != OBJ_HANDLE_NULL) {
        intgame_refresh_experience_gauges(obj);
        intgame_draw_counter(INTGAME_COUNTER_FATE,
            stat_level_get(obj, STAT_FATE_POINTS),
            2);
    }

    return true;
}

// 0x54A330
void iso_interface_create(tig_window_handle_t window_handle)
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
    vb_create_info.width = intgame_health_bar_frame.width / 2;
    vb_create_info.height = intgame_health_bar_frame.height;
    vb_create_info.background_color = 0;
    if (tig_video_buffer_create(&vb_create_info, &dword_64C474) != TIG_OK) {
        tig_debug_printf("iso_interface_create: ERROR: couldn't create video buffer!\n");
        exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE
    }

    window_data.flags = TIG_WINDOW_MESSAGE_FILTER;
    window_data.message_filter = iso_interface_message_filter;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &(window_data.rect);
    art_blit_info.dst_rect = &(window_data.rect);

    for (index = 0; index < 2; index++) {
        if (index == 0) {
            tig_art_interface_id_create(185, 0, 0, 0, &art_id);
        } else {
            tig_art_interface_id_create(184, 0, 0, 0, &art_id);
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
        intgame_button_create(&(stru_5C6E40[index]));
        tig_button_hide(stru_5C6E40[index].button_handle);
    }

    for (index = 0; index < 4; index++) {
        if (index == 0 || index == 1) {
            button_create_flags(&(stru_5C6480[index]), 0x2);
        } else {
            intgame_button_create(&(stru_5C6480[index]));
        }
    }

    for (index = 0; index < INTGAME_PRIMARY_BUTTON_COUNT; index++) {
        intgame_button_create(&(intgame_primary_buttons[index]));
    }

    intgame_button_create(&stru_5C6538);
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

    for (index = 0; index < INTGAME_COUNTER_COUNT; index++) {
        iwid = sub_551740(intgame_number_boxes[index].rect.x, intgame_number_boxes[index].rect.y);
        if (iwid == -1) {
            tig_debug_printf("iso_interface_create: ERROR: find iwid match!\n");
            exit(EXIT_SUCCESS); // FIXME: Should be EXIT_FAILURE
        }

        intgame_number_boxes[index].rect.x -= stru_5C6390[iwid].x;
        intgame_number_boxes[index].rect.y -= stru_5C6390[iwid].y;
        intgame_number_boxes[index].window_handle = dword_64C4F8[iwid];
    }

    hotkey_ui_start(dword_64C4F8[1], &(stru_5C6390[1]), dword_64C4F8[1], false);
    intgame_button_create(&stru_5C6F30);
    intgame_button_create(&intgame_mt_button_info);
    intgame_mt_button_disable();

    dword_64C6B8 = 0;
    dword_64C634[0] = 0;
    sub_4F25B0(Tgt_Obj_No_T_Wall | Tgt_Tile);
    intgame_pc_lens_mode = 0;

    font_desc.str = NULL;
    tig_font_measure(&font_desc);
    tig_art_interface_id_create(171, 0, 0, 0, &(font_desc.art_id));
    tig_font_create(&font_desc, &dword_64C670);

    stru_5C6F90.x = stru_5C6F80.x;
    stru_5C6F90.y = stru_5C6F80.y;
    button_create_no_art(&stru_5C6F90, stru_5C6F80.width, stru_5C6F80.height);

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

    intgame_clock_process_callback(NULL);
    intgame_draw_bar(INTGAME_BAR_HEALTH);
    intgame_draw_bar(INTGAME_BAR_FATIGUE);

    // NOTE: Looks meaningless.
    font_desc.str = NULL;
    tig_font_measure(&font_desc);
    font_desc.flags = ~TIG_FONT_SHADOW;
    tig_art_interface_id_create(230, 0, 0, 0, &(font_desc.art_id));

    sub_54B3C0();

    for (index = 0; index < 10; index++) {
        stru_64C540[index].str = MALLOC(200);
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
        window_data.flags = TIG_WINDOW_ALWAYS_ON_TOP;
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
        intgame_button_create_ex(dword_5C6378[index],
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
            FREE(stru_64C540[index].str);
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
bool intgame_button_create_ex(tig_window_handle_t window_handle, TigRect* rect, UiButtonInfo* button_info, unsigned int flags)
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
bool intgame_button_create(UiButtonInfo* button_info)
{
    int index;

    index = sub_551740(button_info->x, button_info->y);
    if (index == -1) {
        return false;
    }

    return intgame_button_create_ex(dword_64C4F8[index], &(stru_5C6390[index]), button_info, TIG_BUTTON_FLAG_0x01);
}

// 0x54AB20
bool button_create_flags(UiButtonInfo* button_info, unsigned int flags)
{
    int index;

    index = sub_551740(button_info->x, button_info->y);
    if (index == -1) {
        return false;
    }

    return intgame_button_create_ex(dword_64C4F8[index], &(stru_5C6390[index]), button_info, flags);
}

// 0x54ABD0
bool button_create_no_art(UiButtonInfo* button_info, int width, int height)
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
void intgame_button_destroy(UiButtonInfo* button_info)
{
    tig_button_destroy(button_info->button_handle);
    button_info->button_handle = TIG_BUTTON_HANDLE_INVALID;
}

// 0x54AD00
void intgame_draw_counter(int counter, int value, int digits)
{
    IntgameIsoWindowTypeInfo* info;
    TigRect rect;
    TigFont font_desc;
    char format[12];
    char str[80];
    int pos;

    if (!dword_64C6B4) {
        return;
    }

    if (counter == INTGAME_COUNTER_MANA && intgame_iso_window_type != 8) {
        return;
    }

    info = &(intgame_number_boxes[counter]);

    tig_font_push(dword_64C670);

    if (counter != INTGAME_COUNTER_POISON) {
        info->rect.height++;
        tig_window_fill(info->window_handle,
            &(info->rect),
            tig_color_make(0, 0, 0));
        info->rect.height--;
    }

    if (value < 0 && (counter == INTGAME_COUNTER_MANA || counter == INTGAME_COUNTER_MONEY)) {
        if (value == -1) {
            for (pos = 0; pos < digits; pos++) {
                str[pos] = '-';
            }
            str[pos] = '\0';
        } else {
            for(pos = 0; pos < digits; pos++) {
                str[pos] = '?';
            }
            str[pos] = '\0';
        }
    } else {
        sprintf(format, "%%0%dd", digits);
        sprintf(str, format, value);
    }

    font_desc.str = str;
    font_desc.width = 0;
    tig_font_measure(&font_desc);

    if (font_desc.width < info->rect.width) {
        rect.x = info->rect.x + (info->rect.width - font_desc.width) / 2;
        rect.y = info->rect.y + (info->rect.height - font_desc.height) / 2;
        rect.width = font_desc.width;
        rect.height = font_desc.height;
    } else {
        font_desc.width = info->rect.width;
        tig_font_measure(&font_desc);

        if (font_desc.height > info->rect.height) {
            tig_font_pop();
            return;
        }

        rect.x = info->rect.x;
        rect.y = info->rect.y + (info->rect.height - font_desc.height) / 2;
        rect.width = font_desc.width;
        rect.height = font_desc.height;
    }

    tig_window_text_write(info->window_handle, str, &rect);
    tig_font_pop();
}

// 0x54AEE0
void intgame_draw_bar(int bar)
{
    switch (bar) {
    case INTGAME_BAR_HEALTH:
        intgame_draw_bar_rect(&intgame_health_bar_frame);
        break;
    case INTGAME_BAR_FATIGUE:
        intgame_draw_bar_rect(&intgame_fatigue_bar_frame);
        break;
    }
}

// 0x54AF10
void intgame_draw_bar_rect(TigRect* rect)
{
    int64_t pc_obj;
    TigRect rects[INTGAME_BAR_COUNT];
    int nums[INTGAME_BAR_COUNT];
    int poison;
    int bar;
    TigArtBlitInfo art_blit_info;
    TigRect blit_rect;
    TigRect tmp_rect;
    TigRect dst_rect;
    int value;
    int fullness;
    int filled_height;
    int empty_height;

    if (!dword_64C6B0) {
        return;
    }

    pc_obj = player_get_local_pc_obj();
    if (pc_obj == OBJ_HANDLE_NULL) {
        return;
    }

    rects[INTGAME_BAR_HEALTH] = intgame_health_bar_frame;
    rects[INTGAME_BAR_FATIGUE] = intgame_fatigue_bar_frame;

    poison = stat_level_get(pc_obj, STAT_POISON_LEVEL);
    nums[INTGAME_BAR_HEALTH] = poison > 0 ? 17 : 18;
    nums[INTGAME_BAR_FATIGUE] = 19;

    for (bar = 0; bar < INTGAME_BAR_COUNT; bar++) {
        tmp_rect = rects[bar];
        if (tig_rect_intersection(&tmp_rect, rect, &blit_rect) == TIG_OK) {
            blit_rect.x -= stru_5C6390[1].x;
            blit_rect.y -= stru_5C6390[1].y;
            tig_art_interface_id_create(184, 0, 0, 0, &(art_blit_info.art_id));

            art_blit_info.src_rect = &blit_rect;
            art_blit_info.dst_rect = &blit_rect;
            art_blit_info.flags = 0;
            tig_window_blit_art(dword_64C4F8[1], &art_blit_info);
        }

        if (bar == INTGAME_BAR_HEALTH) {
            value = object_hp_max(pc_obj);
            if (value != 0) {
                fullness = 100 * object_hp_current(pc_obj) / value;
            } else {
                fullness = 50;
            }
        } else {
            value = critter_fatigue_max(pc_obj);
            if (value != 0) {
                fullness = 100 * critter_fatigue_current(pc_obj) / value;
            } else {
                fullness = 50;
            }
        }

        filled_height = fullness * tmp_rect.height / 100;
        empty_height = tmp_rect.height - filled_height;
        if (empty_height > 0) {
            tmp_rect.x = rects[bar].x;
            tmp_rect.y = rects[bar].y;
            tmp_rect.width = rects[bar].width;
            tmp_rect.height = empty_height;

            if (tig_rect_intersection(&tmp_rect, rect, &blit_rect) == TIG_OK) {
                tmp_rect.x = blit_rect.x - tmp_rect.x;
                tmp_rect.y = blit_rect.y - tmp_rect.y;
                tmp_rect.width = blit_rect.width;
                tmp_rect.height = blit_rect.height;
                tig_art_interface_id_create(20, 0, 0, 0, &(art_blit_info.art_id));

                dst_rect.width = tmp_rect.width;
                dst_rect.height = tmp_rect.height;
                dst_rect.x = blit_rect.x - stru_5C6390[1].x;
                dst_rect.y = blit_rect.y - stru_5C6390[1].y;

                art_blit_info.flags = 0;
                art_blit_info.src_rect = &tmp_rect;
                art_blit_info.dst_rect = &dst_rect;
                tig_window_blit_art(dword_64C4F8[1], &art_blit_info);
            }
        }

        if (filled_height > 0) {
            int v14;
            int v15;
            int v16;

            tmp_rect.x = rects[bar].x;
            tmp_rect.y = rects[bar].y;
            tmp_rect.width = rects[bar].width;

            v14 = 8;
            v15 = filled_height + 8;
            v16 = empty_height - 8;
            if ( v15 > rects[bar].height )
            {
                v14 += rects[bar].height - v15;
                v16 += v15 - rects[bar].height;
                v15 = rects[bar].height;
            }
            tmp_rect.y += v16;
            tmp_rect.height = v15;

            if (tig_rect_intersection(&tmp_rect, rect, &blit_rect) == TIG_OK) {
                tmp_rect.x = blit_rect.x - tmp_rect.x;
                tmp_rect.y = blit_rect.y - v14 - tmp_rect.y + 8;
                tmp_rect.width = blit_rect.width;
                tmp_rect.height = blit_rect.height;
                tig_art_interface_id_create(nums[bar], 0, 0, 0, &(art_blit_info.art_id));

                dst_rect.x = blit_rect.x - stru_5C6390[1].x;
                dst_rect.width = tmp_rect.width;
                dst_rect.y = blit_rect.y - stru_5C6390[1].y;
                dst_rect.height = tmp_rect.height;

                art_blit_info.flags = 0;
                art_blit_info.src_rect = &tmp_rect;
                art_blit_info.dst_rect = &dst_rect;
                tig_window_blit_art(dword_64C4F8[1], &art_blit_info);
            }
        }

        if (bar == INTGAME_BAR_HEALTH) {
            intgame_draw_counter(INTGAME_COUNTER_HEALTH, object_hp_current(pc_obj), 3);
            if (poison > 0) {
                intgame_draw_counter(INTGAME_COUNTER_POISON, poison, 3);
            }
        } else {
            intgame_draw_counter(INTGAME_COUNTER_FATIGUE, critter_fatigue_current(pc_obj), 3);
        }
    }

    compact_ui_draw();
}

// 0x54B3A0
void intgame_draw_bars()
{
    intgame_draw_bar(INTGAME_BAR_HEALTH);
    intgame_draw_bar(INTGAME_BAR_FATIGUE);
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
    pc_obj = player_get_local_pc_obj();

    if (pc_obj != OBJ_HANDLE_NULL) {
        item_obj = item_wield_get(pc_obj, ITEM_INV_LOC_WEAPON);
        if (item_obj != OBJ_HANDLE_NULL) {
            ammo_type = item_weapon_ammo_type(item_obj);
            if (ammo_type != 10000) {
                qty = item_ammo_quantity_get(pc_obj, ammo_type);
                art_num = dword_5C728C[ammo_type];
            } else {
                qty = obj_field_int32_get(item_obj, OBJ_F_ITEM_MANA_STORE);
                if (qty > 0) {
                    art_num = 469;
                }
            }
        }

        if (qty <= 0) {
            qty = item_gold_get(pc_obj);
        }
    }

    intgame_draw_counter(INTGAME_COUNTER_MONEY, qty, 6);
    tig_art_interface_id_create(art_num, 0, 0, 0, &art_id);
    intgame_ammo_icon_refresh(art_id);

    if (qword_64C688 != OBJ_HANDLE_NULL
        && intgame_iso_window_type == 8) {
        if ((obj_field_int32_get(qword_64C688, OBJ_F_ITEM_FLAGS) & OIF_IDENTIFIED) != 0) {
            mana = obj_field_int32_get(qword_64C688, OBJ_F_ITEM_SPELL_MANA_STORE);
            if (mana >= 0) {
                intgame_draw_counter(INTGAME_COUNTER_MANA, mana, 3);
            } else {
                intgame_draw_counter(INTGAME_COUNTER_MANA, -1, 3);
            }
        } else {
            intgame_draw_counter(INTGAME_COUNTER_MANA, -2, 3);
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
bool sub_54B5D0(TigMessage* msg)
{
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
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

        if (msg->data.keyboard.key != DIK_ESCAPE
            && msg->data.keyboard.key != DIK_O) {
            return dword_64C6CC(msg);
        }

        return false;
    }

    if (combat_turn_based_is_active()) {
        if (!player_is_local_pc_obj(combat_turn_based_whos_turn_get())) {
            if (msg->type != TIG_MESSAGE_KEYBOARD
                && msg->type == TIG_MESSAGE_CHAR
                && msg->data.character.ch == ' ') {
                combat_turn_based_toggle();
                return true;
            }
            return false;
        }
    }

    if (msg->type == TIG_MESSAGE_MOUSE) {
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_MOVE || sub_541680()) {
            return false;
        }

        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_IDLE) {
            if (dword_64C674 != -1) {
                mes_file_entry.num = dword_64C674;
                ui_message.type = UI_MSG_TYPE_FEEDBACK;
                if (mes_search(intgame_mes_file, &mes_file_entry)) {
                    ui_message.str = mes_file_entry.str;
                    sub_550750(&ui_message);
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
                    intgame_refresh_cursor();
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
                sub_56D130(player_get_local_pc_obj(), player_get_local_pc_obj());
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_LOGBOOK].button_handle) {
                logbook_ui_open(player_get_local_pc_obj());
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_CHAR].button_handle) {
                sub_552740(player_get_local_pc_obj(), CHAREDIT_MODE_ACTIVE);
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_INVENTORY].button_handle) {
                inven_ui_open(player_get_local_pc_obj(), OBJ_HANDLE_NULL, INVEN_UI_MODE_INVENTORY);
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_MAP].button_handle) {
                wmap_ui_open();
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_FATE].button_handle) {
                fate_ui_open(player_get_local_pc_obj());
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6538.button_handle) {
                if (tig_net_is_active()) {
                    sub_5700C0();
                } else {
                    sleep_ui_open(OBJ_HANDLE_NULL);
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
                    intgame_refresh_cursor();
                    return true;
                }
                if (msg->data.button.button_handle == stru_5C65F8[0].button_handle) {
                    if (!sub_573620()) {
                        sub_552930();
                        return true;
                    }
                    sub_575770();
                    intgame_refresh_cursor();
                    return true;
                }
                break;
            case 1:
                for (index = 0; index < 5; index++) {
                    if (intgame_spell_buttons[5 * dword_64C530 + index].art_num != -1
                        && msg->data.button.button_handle == intgame_spell_buttons[5 * dword_64C530 + index].button_handle) {
                        if (spell_is_known(player_get_local_pc_obj(), 5 * dword_64C530 + index)) {
                            sub_57EFA0(3, 5 * dword_64C530 + index, OBJ_HANDLE_NULL);
                            sub_57BC70(player_get_local_pc_obj(), 5 * dword_64C530 + index);
                            return true;
                        }
                    }
                }
                break;
            case 2:
                for (index = 0; index < 4; index++) {
                    if (msg->data.button.button_handle == stru_5C6C68[index].button_handle) {
                        sub_57EFA0(2, index, OBJ_HANDLE_NULL);
                        sub_579FA0(player_get_local_pc_obj(), index);
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
                    textedit_ui_clear();
                    return true;
                }

                if (msg->data.button.button_handle == stru_5C6CA8[1].button_handle) {
                    textedit_ui_restore();
                    return true;
                }

                if (msg->data.button.button_handle == stru_5C6CA8[2].button_handle) {
                    textedit_ui_commit();
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
            if (msg->data.button.button_handle == stru_5C6480[1].button_handle) {
                if (tig_button_state_get(stru_5C6480[0].button_handle, &button_state) == TIG_OK
                    && button_state == TIG_BUTTON_STATE_PRESSED) {
                    tig_button_state_change(stru_5C6480[0].button_handle, TIG_BUTTON_STATE_RELEASED);
                }
                sub_5506C0(1);
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6480[0].button_handle) {
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
                    if (spell_college_small_icon(index) != -1
                        && msg->data.button.button_handle == intgame_college_buttons[index].button_handle) {
                        intgame_spells_hide_college_spells(dword_64C530);
                        dword_64C530 = index;
                        intgame_spells_show_college_spells(dword_64C530);
                        return true;
                    }
                }
                break;
            case 9:
                if (msg->data.button.button_handle == stru_5C6D08[1].button_handle) {
                    if (dword_64C678 < dword_64C478) {
                        dword_64C678++;
                        sub_553960();
                    }
                    return true;
                }
                if (msg->data.button.button_handle == stru_5C6D08[2].button_handle) {
                    if (dword_64C678 > 0) {
                        dword_64C678--;
                        sub_553960();
                    }
                    return true;
                }
                break;
            case 6:
                for (index = 3; index < 6; index++) {
                    if (msg->data.button.button_handle == stru_5C6CA8[index].button_handle) {
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

                ui_message.type = 6;
                ui_message.str = buffer;
                sub_550750(&ui_message);
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

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_LOGBOOK].button_handle) {
                dword_64C674 = 1004; // "Log Book"
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_CHAR].button_handle) {
                dword_64C674 = 1005; // "Character Editor"
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_INVENTORY].button_handle) {
                dword_64C674 = 1006; // "Inventory"
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_MAP].button_handle) {
                dword_64C674 = 1007; // "Maps"
                return true;
            }

            if (msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_FATE].button_handle) {
                dword_64C674 = 1008; // "Fate Points"
                return true;
            }

            if (msg->data.button.button_handle == stru_5C6538.button_handle) {
                dword_64C674 = (tig_net_is_active()) ? 1010 : 1009;
            }

            switch (intgame_iso_window_type) {
            case 1:
                for (index = 0; index < 16; index++) {
                    if (msg->data.button.button_handle == intgame_college_buttons[index].button_handle) {
                        sub_550860(index);
                        return true;
                    }
                }
                for (index = 0; index < 5; index++) {
                    if (msg->data.button.button_handle == intgame_spell_buttons[5 * dword_64C530 + index].button_handle) {
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
                        art_id = tig_mouse_cursor_get_art_id();
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
                        art_id = tig_mouse_cursor_get_art_id();
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
                intgame_refresh_cursor();
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
                intgame_refresh_cursor();
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
                broadcast_ui_open();
                v2 = true;
            }
        }

        if (sub_541680()) {
            return false;
        }

        switch (msg->data.character.ch) {
        case ' ':
            combat_turn_based_toggle();
            break;
        case 'C':
        case 'c':
            sub_552740(player_get_local_pc_obj(), CHAREDIT_MODE_ACTIVE);
            break;
        case 'F':
        case 'f':
            fate_ui_open(player_get_local_pc_obj());
            break;
        case 'I':
        case 'i':
            inven_ui_open(player_get_local_pc_obj(), OBJ_HANDLE_NULL, INVEN_UI_MODE_INVENTORY);
            break;
        case 'R':
        case 'r':
            sub_54EBF0();
            break;
        case 'S':
        case 's':
            sleep_ui_open(OBJ_HANDLE_NULL);
            break;
        case 'T':
        case 't':
            sub_56D130(player_get_local_pc_obj(), player_get_local_pc_obj());
            break;
        case 'W':
        case 'w':
            wmap_ui_open();
            break;
        default:
            if (!v2) {
                return false;
            }
            break;
        }

        gsound_play_sfx(0, 1);
        return true;
    } // msg->type == TIG_MESSAGE_CHAR

    return false;
}

// 0x54C8E0
bool iso_interface_message_filter(TigMessage* msg)
{
    // NOTE: Strange case - this function is huge but it's binary identical to
    // 0x54B5D0.
    return sub_54B5D0(msg);
}

// 0x54DBF0
void sub_54DBF0(int btn, int window_type)
{
    int state;

    tig_button_state_get(stru_5C6480[btn].button_handle, &state);
    if (state != TIG_BUTTON_STATE_PRESSED) {
        tig_button_state_change(stru_5C6480[btn != 1 ? 1 : 0].button_handle, TIG_BUTTON_STATE_RELEASED);
        intgame_force_fullscreen();
        tig_button_state_change(stru_5C6480[btn].button_handle, TIG_BUTTON_STATE_PRESSED);
        sub_5506C0(window_type);
    } else {
        sub_5506C0(0);
        tig_button_state_change(stru_5C6480[btn].button_handle, TIG_BUTTON_STATE_RELEASED);
        intgame_unforce_fullscreen();
    }
}

// 0x54DC80
bool sub_54DC80(TigMessage* msg)
{
    int index;

    // FIXME: No check for msg type.
    if (msg->data.button.state != TIG_BUTTON_STATE_MOUSE_OUTSIDE) {
        return false;
    }

    sub_54ECD0();

    if (msg->data.button.button_handle == stru_5C6480[1].button_handle
        || msg->data.button.button_handle == stru_5C6480[0].button_handle
        || msg->data.button.button_handle == stru_5C6480[2].button_handle
        || msg->data.button.button_handle == stru_5C6480[3].button_handle
        || msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_LOGBOOK].button_handle
        || msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_CHAR].button_handle
        || msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_INVENTORY].button_handle
        || msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_MAP].button_handle
        || msg->data.button.button_handle == intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_FATE].button_handle
        || msg->data.button.button_handle == stru_5C6538.button_handle) {
        dword_64C674 = -1;
        sub_550720();
        return true;
    }

    switch (intgame_iso_window_type) {
    case 0:
        if (msg->data.button.button_handle == stru_5C65F8[1].button_handle
            || msg->data.button.button_handle == stru_5C65F8[0].button_handle) {
            if (!sub_573620()) {
                intgame_refresh_cursor();
            }
            return true;
        }
        if (msg->data.button.button_handle == stru_5C6F90.button_handle) {
            intgame_message_window_clear();
        }
        break;
    case 1:
    case 2:
    case 8:
        intgame_message_window_clear();
        break;
    }

    for (index = 0; index < 5; index++) {
        if (msg->data.button.button_handle == stru_5C6E40[index].button_handle) {
            intgame_message_window_clear();
            return true;
        }
    }

    for (index = 0; index < 5; index++) {
        if (msg->data.button.button_handle == stru_5C6E90[index].button_handle) {
            intgame_message_window_clear();
            return true;
        }
    }

    return false;
}

// 0x54DE50
void sub_54DE50(TigMessage* msg)
{
    int64_t pc_obj;
    int64_t loc = 0;
    S4F2810 v1;
    TigMouseState mouse_state;
    TigMessage fake_mouse_move;

    pc_obj = player_get_local_pc_obj();

    if (combat_turn_based_is_active()) {
        if (combat_turn_based_whos_turn_get() != pc_obj) {
            return;
        }
    }

    if (msg->type == TIG_MESSAGE_KEYBOARD
        && !textedit_ui_is_focused()
        && !msg->data.keyboard.pressed
        && msg->data.keyboard.key >= DIK_1
        && msg->data.keyboard.key <= DIK_0) {
        sub_57F1D0(msg->data.keyboard.key - DIK_1);
    }

    switch (sub_551A00()) {
    case 0:
        switch (msg->type) {
        case TIG_MESSAGE_MOUSE:
            switch (msg->data.mouse.event) {
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_DOWN:
                if (sub_5517A0(msg)
                    && sub_552050(&(msg->data.mouse), &v1)) {
                    if (v1.is_loc) {
                        if (sub_573620() != OBJ_HANDLE_NULL) {
                            int64_t v2;

                            v2 = sub_573620();
                            if (sub_57DE00()) {
                                sub_57E8B0();
                                sub_573740(v2, false);
                                if (sub_573620() != OBJ_HANDLE_NULL) {
                                    v2 = sub_573620();
                                }
                            }

                            if (!sub_57DE00()) {
                                if (critter_is_active(pc_obj)) {
                                    sub_573840();
                                    intgame_refresh_cursor();
                                    anim_goal_throw_item(pc_obj, v2, v1.loc);
                                } else {
                                    sub_575770();
                                }
                            }
                        } else if (critter_is_active(pc_obj)
                            && !tig_kb_is_key_pressed(DIK_LSHIFT)
                            && !tig_kb_is_key_pressed(DIK_RSHIFT)) {
                            if ((tig_kb_is_key_pressed(DIK_LCONTROL)
                                    || tig_kb_is_key_pressed(DIK_RCONTROL)
                                    || tig_kb_is_key_pressed(DIK_NUMLOCK))
                                && !settings_get_value(&settings, "always run")) {
                                sub_433C80(pc_obj, v1.loc);
                            } else {
                                sub_433640(pc_obj, v1.loc);
                            }

                            if (dword_64C6D8) {
                                sub_436CF0();
                            }
                            dword_64C6D8 = true;
                        }
                    } else if (!dword_64C6D8) {
                        sub_54ED30(&v1);
                    }
                }
                break;
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
                dword_64C6D8 = false;
                break;
            case TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP:
                if (pc_obj != OBJ_HANDLE_NULL) {
                    unsigned int spell_flags;
                    unsigned int critter_flags;

                    spell_flags = obj_field_int32_get(pc_obj, OBJ_F_SPELL_FLAGS);
                    critter_flags = obj_field_int32_get(pc_obj, OBJ_F_CRITTER_FLAGS);
                    if ((spell_flags & OSF_STONED) == 0
                        && (critter_flags & (OCF_PARALYZED | OCF_STUNNED)) == 0
                        && critter_is_active(pc_obj)
                        && !critter_is_prone(pc_obj)) {
                        if (combat_critter_is_combat_mode_active(pc_obj)) {
                            if (anim_is_current_goal_type(pc_obj, AG_ANIM_FIDGET, NULL)
                                || !sub_423300(pc_obj, 0)) {
                                sub_54EBF0();
                            }
                        } else {
                            if (critter_is_concealed(pc_obj)
                                && !sub_423300(pc_obj, NULL)) {
                                critter_set_concealed(pc_obj, false);
                            }
                        }

                        if (sub_424070(pc_obj, 3, 0, 0)) {
                            sub_4B4320(pc_obj);

                            tig_mouse_get_state(&mouse_state);
                            if (location_at(mouse_state.x, mouse_state.y, &loc)
                                && sub_5517A0(msg)) {
                                int64_t pc_loc;
                                tig_art_id_t aid;
                                int rot;

                                pc_loc = obj_field_int64_get(pc_obj, OBJ_F_LOCATION);
                                aid = obj_field_int32_get(pc_obj, OBJ_F_CURRENT_AID);
                                rot = location_rot(pc_loc, loc);
                                if (!sub_423300(pc_obj, 0)) {
                                    anim_goal_rotate(pc_obj, rot);
                                } else if (anim_is_current_goal_type(pc_obj, AG_ANIM_FIDGET, NULL)) {
                                    object_set_current_aid(pc_obj, tig_art_id_rotation_set(aid, rot));
                                }
                            }
                        }
                    }

                    if (sub_573620()) {
                        sub_575770();
                        intgame_refresh_cursor();
                    }
                }
                break;
            case TIG_MESSAGE_MOUSE_IDLE:
                sub_551910(msg);
            }
            break;
        case TIG_MESSAGE_KEYBOARD:
            if (!textedit_ui_is_focused()
                && !msg->data.keyboard.pressed) {
                switch (msg->data.keyboard.key) {
                case DIK_F1:
                case DIK_F2:
                case DIK_F3:
                case DIK_F4:
                case DIK_F5:
                case DIK_F6:
                    intgame_get_location_under_cursor(&loc);
                    sub_4C3BE0(msg->data.keyboard.key - DIK_F1, loc);
                    break;
                case DIK_HOME:
                    sub_54EB60();
                    break;
                }
            }
            break;
        case TIG_MESSAGE_PING:
            if (tig_mouse_get_state(&mouse_state) == TIG_OK) {
                fake_mouse_move.timestamp = msg->timestamp;
                fake_mouse_move.type = TIG_MESSAGE_MOUSE;
                fake_mouse_move.data.mouse.x = mouse_state.x;
                fake_mouse_move.data.mouse.y = mouse_state.y;
                fake_mouse_move.data.mouse.event = TIG_MESSAGE_MOUSE_MOVE;
                fake_mouse_move.data.mouse.z = mouse_state.z;
                sub_553A70(&fake_mouse_move);
            }
            break;
        }
        return;
    case 1:
        switch (msg->type) {
        case TIG_MESSAGE_MOUSE:
            switch (msg->data.mouse.event) {
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
                if (!inven_ui_is_created()) {
                    if (sub_4F2830(&(msg->data.mouse), &v1, intgame_fullscreen)) {
                        sub_57C110(&v1);
                    } else if (sub_4F2D10() == 0x100000) {
                        sub_57BC30();
                    }
                }
                break;
            case TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP:
                sub_57BFF0();
                break;
            case TIG_MESSAGE_MOUSE_IDLE:
                sub_551910(msg);
                break;
            }
            break;
        case TIG_MESSAGE_KEYBOARD:
            if (!textedit_ui_is_focused()) {
                if (!msg->data.keyboard.pressed) {
                    switch (msg->data.keyboard.key) {
                    case DIK_LMENU:
                    case DIK_RMENU:
                        if (!tig_kb_is_key_pressed(DIK_LMENU)
                            && !tig_kb_is_key_pressed(DIK_RMENU)) {
                            sub_57C0E0();
                        }
                        break;
                    case DIK_F1:
                    case DIK_F2:
                    case DIK_F3:
                    case DIK_F4:
                    case DIK_F5:
                    case DIK_F6:
                        intgame_get_location_under_cursor(&loc);
                        sub_4C3BE0(msg->data.keyboard.key - DIK_F1, loc);
                        break;
                    case DIK_HOME:
                        sub_54EB60();
                        break;
                    }
                } else {
                    switch (msg->data.keyboard.key) {
                    case DIK_LMENU:
                    case DIK_RMENU:
                        sub_57C0B0();
                        break;
                    }
                }
            }
            break;
        case TIG_MESSAGE_PING:
            if (tig_mouse_get_state(&mouse_state) == TIG_OK) {
                fake_mouse_move.timestamp = msg->timestamp;
                fake_mouse_move.type = TIG_MESSAGE_MOUSE;
                fake_mouse_move.data.mouse.x = mouse_state.x;
                fake_mouse_move.data.mouse.y = mouse_state.y;
                fake_mouse_move.data.mouse.event = TIG_MESSAGE_MOUSE_MOVE;
                fake_mouse_move.data.mouse.z = mouse_state.z;
                sub_553A70(&fake_mouse_move);
            }
            break;
        }
        return;
    case 2:
        switch (msg->type) {
        case TIG_MESSAGE_MOUSE:
            switch (msg->data.mouse.event) {
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
                if (sub_4F2830(&(msg->data.mouse), &v1, intgame_fullscreen)) {
                    sub_57A1F0(&v1);
                }
                break;
            case TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP:
                sub_57A1A0();
                break;
            }
        }
        return;
    case 3:
        switch (msg->type) {
        case TIG_MESSAGE_MOUSE:
            switch (msg->data.mouse.event) {
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_DOWN:
                if (sub_5517A0(msg)
                    && sub_4F2830(&(msg->data.mouse), &v1, intgame_fullscreen)
                    && v1.is_loc
                    && !sub_573620()
                    && !critter_is_dead(pc_obj)
                    && !tig_kb_is_key_pressed(DIK_LSHIFT)
                    && !tig_kb_is_key_pressed(DIK_RSHIFT)) {
                    if ((tig_kb_is_key_pressed(DIK_LCONTROL)
                            || tig_kb_is_key_pressed(DIK_RCONTROL)
                            || tig_kb_is_key_pressed(DIK_NUMLOCK))
                        && !settings_get_value(&settings, "always run")) {
                        sub_433C80(pc_obj, v1.loc);
                    } else {
                        sub_433640(pc_obj, v1.loc);
                    }

                    if (dword_64C6D8) {
                        sub_436CF0();
                    }
                    dword_64C6D8 = true;
                }
                break;
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
                dword_64C6D8 = false;
                break;
            case TIG_MESSAGE_MOUSE_IDLE:
                sub_551910(msg);
                break;
            }
            break;
        case TIG_MESSAGE_PING:
            if (!dword_64C6CC
                && tig_mouse_get_state(&mouse_state) == TIG_OK) {
                fake_mouse_move.timestamp = msg->timestamp;
                fake_mouse_move.type = TIG_MESSAGE_MOUSE;
                fake_mouse_move.data.mouse.x = mouse_state.x;
                fake_mouse_move.data.mouse.y = mouse_state.y;
                fake_mouse_move.data.mouse.event = TIG_MESSAGE_MOUSE_MOVE;
                fake_mouse_move.data.mouse.z = mouse_state.z;
                sub_553A70(&fake_mouse_move);
            }
            break;
        }
        return;
    case 16:
        switch (msg->type) {
        case TIG_MESSAGE_MOUSE:
            switch (msg->data.mouse.event) {
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
                if (sub_4F2830(&(msg->data.mouse), &v1, intgame_fullscreen)) {
                    sub_571CB0(&v1);
                } else if (sub_4F2D10() == 0x100000) {
                    sub_57BC30();
                }
                break;
            case TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP:
                sub_571C80();
                break;
            case TIG_MESSAGE_MOUSE_IDLE:
                sub_551910(msg);
                break;
            }
            break;
        case TIG_MESSAGE_PING:
            if (tig_mouse_get_state(&mouse_state) == TIG_OK) {
                fake_mouse_move.timestamp = msg->timestamp;
                fake_mouse_move.type = TIG_MESSAGE_MOUSE;
                fake_mouse_move.data.mouse.x = mouse_state.x;
                fake_mouse_move.data.mouse.y = mouse_state.y;
                fake_mouse_move.data.mouse.event = TIG_MESSAGE_MOUSE_MOVE;
                fake_mouse_move.data.mouse.z = mouse_state.z;
                sub_553A70(&fake_mouse_move);
            }
            break;
        }
        return;
    case 18:
        switch (msg->type) {
        case TIG_MESSAGE_MOUSE:
            switch (msg->data.mouse.event) {
            case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
                if (sub_4F2830(&(msg->data.mouse), &v1, intgame_fullscreen)) {
                    follower_ui_execute_order(&v1);
                }
                break;
            case TIG_MESSAGE_MOUSE_RIGHT_BUTTON_UP:
                follower_ui_end_order_mode();
                break;
            case TIG_MESSAGE_MOUSE_IDLE:
                sub_551910(msg);
                break;
            }
            break;
        case TIG_MESSAGE_KEYBOARD:
            if (textedit_ui_is_focused()
                && !msg->data.keyboard.pressed) {
                switch (msg->data.keyboard.key) {
                case DIK_F1:
                case DIK_F2:
                case DIK_F3:
                case DIK_F4:
                case DIK_F5:
                case DIK_F6:
                    intgame_get_location_under_cursor(&loc);
                    sub_4C3BE0(msg->data.keyboard.key - DIK_F1, loc);
                    break;
                case DIK_HOME:
                    sub_54EB60();
                    break;
                }
            }
            break;
        case TIG_MESSAGE_PING:
            if (tig_mouse_get_state(&mouse_state) == TIG_OK) {
                fake_mouse_move.timestamp = msg->timestamp;
                fake_mouse_move.type = TIG_MESSAGE_MOUSE;
                fake_mouse_move.data.mouse.x = mouse_state.x;
                fake_mouse_move.data.mouse.y = mouse_state.y;
                fake_mouse_move.data.mouse.event = TIG_MESSAGE_MOUSE_MOVE;
                fake_mouse_move.data.mouse.z = mouse_state.z;
                sub_553A70(&fake_mouse_move);
            }
            break;
        }
        return;
    }
}

// 0x54EA80
void sub_54EA80(S4F2810* a1)
{
    int64_t pc_obj;
    S4F2680 v1;

    pc_obj = player_get_local_pc_obj();

    if (!combat_turn_based_is_active()
        || combat_turn_based_whos_turn_get() == pc_obj) {
        v1.field_0 = pc_obj;
        v1.field_8 = pc_obj;
        v1.field_10 = a1;

        switch (sub_551A00()) {
        case 1:
            if (sub_4F2680(&v1)) {
                sub_57C110(a1);
            }
            break;
        case 2:
            if (sub_4F2680(&v1)) {
                sub_57A1F0(a1);
            }
            break;
        case 16:
            if (sub_4F2680(&v1)) {
                sub_571CB0(a1);
            }
            break;
        default:
            sub_54ED30(a1);
            break;
        }
    }
}

// 0x54EB50
bool sub_54EB50()
{
    return sub_57DE00();
}

// 0x54EB60
void sub_54EB60()
{
    int64_t obj;
    int64_t loc;
    int64_t x;
    int64_t y;

    obj = player_get_local_pc_obj();
    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
    sub_4B8940(loc, &x, &y);

    if (x != 0 || y != 0) {
        location_origin_set(loc);
        iso_redraw();
    } else {
        if (combat_turn_based_is_active()) {
            obj = combat_turn_based_whos_turn_get();
            if (obj != OBJ_HANDLE_NULL) {
                loc = obj_field_int64_get(obj, OBJ_F_LOCATION);
                location_origin_set(loc);
                iso_redraw();
            }
        }
    }
}

// 0x54EBF0
void sub_54EBF0()
{
    int64_t pc_obj;
    MesFileEntry mes_file_entry;
    UiMessage ui_message;
    int64_t obj;

    pc_obj = player_get_local_pc_obj();
    if (critter_is_dead(pc_obj)) {
        return;
    }

    if (combat_critter_is_combat_mode_active(pc_obj)) {
        if (sub_4B3D90(pc_obj)) {
            combat_critter_deactivate_combat_mode(pc_obj);
        } else {
            mes_file_entry.num = 24;
            mes_get_msg(intgame_mes_file, &mes_file_entry);

            ui_message.type = UI_MSG_TYPE_FEEDBACK;
            ui_message.str = mes_file_entry.str;
            sub_550750(&ui_message);
        }
    } else {
        combat_critter_activate_combat_mode(pc_obj);
    }

    sub_5517F0();

    obj = qword_64C690;
    if (obj == OBJ_HANDLE_NULL) {
        obj = sub_43C570();
    }

    if (obj != OBJ_HANDLE_NULL) {
        sub_43C270(OBJ_HANDLE_NULL);
        sub_43C270(obj);
        if (obj != OBJ_HANDLE_NULL) {
            sub_57CCF0(pc_obj, obj);
        }
    }
}

// 0x54ECD0
void sub_54ECD0()
{
    if (qword_64C690 == OBJ_HANDLE_NULL) {
        return;
    }

    if (sub_43C570() != OBJ_HANDLE_NULL) {
        return;
    }

    sub_43C270(qword_64C690);
    sub_43C270(OBJ_HANDLE_NULL);
    sub_57CCF0(player_get_local_pc_obj(), qword_64C690);
}

// TODO: Lots of jumps, check.
//
// 0x54ED30
void sub_54ED30(S4F2810* a1)
{
    int64_t pc_obj;
    int64_t item_obj = OBJ_HANDLE_NULL;
    int64_t target_loc = 0;
    int target_type;
    unsigned int spell_flags;
    unsigned int critter_flags;
    AnimGoalData goal_data;
    AnimID anim_id;
    int anim;
    bool v26 = false;

    pc_obj = player_get_local_pc_obj();
    if (pc_obj == OBJ_HANDLE_NULL) {
        return;
    }

    if (!critter_is_active(pc_obj)) {
        return;
    }

    target_type = obj_field_int32_get(a1->obj, OBJ_F_TYPE);
    if (!a1->is_loc && a1->obj == pc_obj) {
        return;
    }

    spell_flags = obj_field_int32_get(pc_obj, OBJ_F_SPELL_FLAGS);
    critter_flags = obj_field_int32_get(pc_obj, OBJ_F_CRITTER_FLAGS);

    if ((spell_flags & OSF_STONED) != 0
        && (critter_flags & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
        return;
    }

    if (sub_573620() != OBJ_HANDLE_NULL) {
        if ((spell_flags & OSF_POLYMORPHED) != 0) {
            return;
        }

        item_obj = sub_573620();
        if (sub_57DE00()) {
            sub_57E8B0();
            sub_573740(item_obj, false);
        }

        if (sub_57DE00()) {
            return;
        }

        target_loc = obj_field_int64_get(a1->obj, OBJ_F_LOCATION);
        if (sub_573620() != OBJ_HANDLE_NULL) {
            item_obj = sub_573620();
        }

        sub_573840();
        intgame_refresh_cursor();

        anim = AG_THROW_ITEM;
    } else {
        if (!combat_critter_is_combat_mode_active(pc_obj)) {
            if (sub_44E940(a1->obj, 0, pc_obj)) {
                combat_critter_activate_combat_mode(pc_obj);
            }
        }

        if (combat_critter_is_combat_mode_active(pc_obj)) {
            switch (target_type) {
            case OBJ_TYPE_WALL:
            case OBJ_TYPE_PORTAL:
                if (tig_kb_is_key_pressed(DIK_LMENU) || tig_kb_is_key_pressed(DIK_RMENU)) {
                    if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                        anim = AG_ATTEMPT_ATTACK;
                    } else {
                        anim = AG_ATTACK;
                    }
                } else {
                    if ((spell_flags & OSF_POLYMORPHED) != 0) {
                        return;
                    }

                    anim = AG_USE_OBJECT;
                    v26 = true;
                }
                break;
            case OBJ_TYPE_CONTAINER:
                if ((spell_flags & OSF_POLYMORPHED) != 0) {
                    return;
                }

                if (tig_kb_is_key_pressed(DIK_LMENU) || tig_kb_is_key_pressed(DIK_RMENU)) {
                    if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                        anim = AG_ATTEMPT_ATTACK;
                    } else {
                        anim = AG_ATTACK;
                    }
                } else {
                    anim = AG_USE_CONTAINER;
                    v26 = true;
                }
                break;
            case OBJ_TYPE_SCENERY:
                if (tig_kb_is_key_pressed(DIK_LMENU) || tig_kb_is_key_pressed(DIK_RMENU)) {
                    if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                        anim = AG_ATTEMPT_ATTACK;
                    } else {
                        anim = AG_ATTACK;
                    }
                } else {
                    if ((spell_flags & OSF_POLYMORPHED) != 0) {
                        return;
                    }

                    anim = AG_USE_OBJECT;
                    v26 = true;
                }
                break;
            case OBJ_TYPE_WEAPON:
            case OBJ_TYPE_AMMO:
            case OBJ_TYPE_ARMOR:
            case OBJ_TYPE_GOLD:
            case OBJ_TYPE_FOOD:
            case OBJ_TYPE_SCROLL:
            case OBJ_TYPE_KEY:
            case OBJ_TYPE_KEY_RING:
            case OBJ_TYPE_WRITTEN:
            case OBJ_TYPE_GENERIC:
                if ((spell_flags & OSF_POLYMORPHED) != 0) {
                    return;
                }

                anim = AG_PICKUP_ITEM;
                break;
            case OBJ_TYPE_PC:
            case OBJ_TYPE_NPC:
                if (critter_is_dead(a1->obj)) {
                    if (tig_kb_is_key_pressed(DIK_LMENU) || tig_kb_is_key_pressed(DIK_RMENU)) {
                        anim = AG_USE_CONTAINER;
                        v26 = true;
                    }
                } else {
                    if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                        anim = AG_ATTEMPT_ATTACK;
                    } else {
                        anim = AG_ATTACK;
                    }

                    if (!a1->is_loc
                        && player_is_local_pc_obj(critter_pc_leader_get(a1->obj)
                        && !tig_kb_is_key_pressed(DIK_LMENU))) {
                        return;
                    }
                }
                break;
            case OBJ_TYPE_TRAP:
                anim = AG_MOVE_TO_TILE;
                target_loc = obj_field_int64_get(a1->obj, OBJ_F_LOCATION);
                break;
            default:
                return;
            }

            if (anim == AG_ATTACK || anim == AG_ATTEMPT_ATTACK) {
                if ((spell_flags & OSF_BODY_OF_AIR) != 0) {
                    MesFileEntry mes_file_entry;
                    UiMessage ui_message;

                    mes_file_entry.num = 25; // "You cannot attack in this form."
                    mes_get_msg(intgame_mes_file, &mes_file_entry);
                    ui_message.type = UI_MSG_TYPE_FEEDBACK;
                    ui_message.str = mes_file_entry.str;
                    sub_550750(&ui_message);
                    return;
                }
            }
        } else {
            switch (target_type) {
            case OBJ_TYPE_PORTAL:
                if ((spell_flags & OSF_POLYMORPHED) != 0) {
                    return;
                }

                anim = AG_USE_OBJECT;
                v26 = true;
                break;
            case OBJ_TYPE_CONTAINER:
                if ((spell_flags & OSF_POLYMORPHED) != 0) {
                    return;
                }

                anim = AG_USE_CONTAINER;
                v26 = true;
                break;
            case OBJ_TYPE_WEAPON:
            case OBJ_TYPE_AMMO:
            case OBJ_TYPE_ARMOR:
            case OBJ_TYPE_GOLD:
            case OBJ_TYPE_FOOD:
            case OBJ_TYPE_SCROLL:
            case OBJ_TYPE_KEY:
            case OBJ_TYPE_KEY_RING:
            case OBJ_TYPE_WRITTEN:
            case OBJ_TYPE_GENERIC:
                if ((spell_flags & OSF_POLYMORPHED) != 0) {
                    return;
                }

                if (tig_kb_is_key_pressed(DIK_LMENU) || tig_kb_is_key_pressed(DIK_RMENU)) {
                    sub_4445A0(pc_obj, a1->obj);
                    return;
                }

                anim = AG_PICKUP_ITEM;
                break;
            case OBJ_TYPE_PC:
            case OBJ_TYPE_NPC:
                if (!critter_is_dead(a1->obj)
                    || (obj_field_int32_get(a1->obj, OBJ_F_SPELL_FLAGS) & OSF_SPOKEN_WITH_DEAD) != 0) {
                    // FIXME: Useless.
                    obj_field_int32_get(a1->obj, OBJ_F_CURRENT_AID);

                    if (player_is_pc_obj(a1->obj)) {
                        return;
                    }

                    anim = AG_TALK;
                } else {
                    if ((spell_flags & OSF_POLYMORPHED) != 0) {
                        return;
                    }

                    if (tig_kb_is_key_pressed(DIK_LMENU) || tig_kb_is_key_pressed(DIK_RMENU)) {
                        sub_4445A0(pc_obj, a1->obj);
                        return;
                    }

                    anim = AG_USE_CONTAINER;
                    v26 = true;
                }
                break;
            case OBJ_TYPE_TRAP:
                anim = AG_MOVE_TO_TILE;
                target_loc = obj_field_int64_get(a1->obj, OBJ_F_LOCATION);
                break;
            default:
                return;
            }
        }
    }

    // 0x54F25B
    if (!sub_44D500(&goal_data, pc_obj, anim)) {
        return;
    }

    goal_data.params[AGDATA_TARGET_OBJ].obj = a1->obj;

    if (target_loc != 0) {
        goal_data.params[AGDATA_TARGET_TILE].loc = target_loc;
    }

    if (item_obj != OBJ_HANDLE_NULL) {
        goal_data.params[AGDATA_SCRATCH_OBJ].obj = item_obj;
    }

    if (anim == AG_ATTACK || anim == AG_ATTEMPT_ATTACK) {
        if (tig_kb_is_key_pressed(DIK_COMMA)) {
            goal_data.params[AGDATA_SCRATCH_VAL3].data = 1;
        } else if (tig_kb_is_key_pressed(DIK_PERIOD)) {
            goal_data.params[AGDATA_SCRATCH_VAL3].data = 2;
        } else if (tig_kb_is_key_pressed(DIK_SLASH)) {
            goal_data.params[AGDATA_SCRATCH_VAL3].data = 3;
        } else {
            goal_data.params[AGDATA_SCRATCH_VAL3].data = 0;
        }

        int64_t weapon_obj = item_wield_get(pc_obj, ITEM_INV_LOC_WEAPON);
        if (weapon_obj != OBJ_HANDLE_NULL
            && obj_field_int32_get(weapon_obj, OBJ_F_TYPE) == OBJ_TYPE_WEAPON
            && (obj_field_int32_get(weapon_obj, OBJ_F_WEAPON_FLAGS) & OWF_DEFAULT_THROWS) != 0
            && !item_check_remove(weapon_obj)) {
            int64_t throwable_instance_obj = item_find_first_matching_prototype(pc_obj, weapon_obj);
            if (throwable_instance_obj == OBJ_HANDLE_NULL) {
                throwable_instance_obj = weapon_obj;
            }

            item_remove(throwable_instance_obj);

            goal_data.params[AGDATA_TARGET_TILE].loc = obj_field_int64_get(a1->obj, OBJ_F_LOCATION);
            goal_data.params[AGDATA_SCRATCH_OBJ].obj = throwable_instance_obj;

            anim_goal_throw_item(pc_obj, throwable_instance_obj, obj_field_int64_get(a1->obj, OBJ_F_LOCATION));
            return;
        }
    }

    if (combat_auto_attack_get(pc_obj)) {
        if (sub_44E6F0(pc_obj, &goal_data)
            || !sub_424070(pc_obj, 3, 0, 0)) {
            return;
        }

        unsigned int flags = 0;
        if (tig_net_is_active()) {
            if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                flags |= 0x100;
            } else if (tig_kb_is_key_pressed(DIK_LCONTROL)
                || tig_kb_is_key_pressed(DIK_RCONTROL)
                || ((!tig_kb_get_modifier(DIK_NUMLOCK) || get_always_run(pc_obj))
                    && !tig_kb_is_key_pressed(DIK_LCONTROL)
                    && !tig_kb_is_key_pressed(DIK_RCONTROL))) {
                flags |= 0x40;
            }

            if (v26) {
                flags |= 0x4000;
            }
        }

        if (!sub_44D540(&goal_data, &anim_id, flags)) {
            return;
        }

        if (tig_net_is_active()) {
            return;
        }
    } else if (sub_423300(pc_obj, &anim_id)) {
        // 0x54F68E
        if (combat_turn_based_is_active()) {
            AnimID fidget_anim_id;
            if (anim_is_current_goal_type(pc_obj, AG_ANIM_FIDGET, &fidget_anim_id)
                && anim_id_is_equal(&anim_id, &fidget_anim_id)
                && num_goal_subslots_in_use(&anim_id) < 4) {
                if (is_anim_forever(&anim_id)) {
                    if (sub_424070(pc_obj, 3, false, false)) {
                        if (!sub_44D520(&goal_data, &anim_id)) {
                            return;
                        }
                    }
                } else {
                    if (!sub_44DBE0(anim_id, &goal_data)) {
                        return;
                    }
                }
            }
        } else if (sub_44E6F0(pc_obj, &goal_data)) {
            if (anim == AG_ATTACK || anim == AG_ATTEMPT_ATTACK) {
                if (num_goal_subslots_in_use(&anim_id) < 4) {
                    if (is_anim_forever(&anim_id)) {
                        if (sub_424070(pc_obj, 3, 0, 0)
                            && !sub_44D520(&goal_data, &anim_id)) {
                            return;
                        }
                    } else {
                        if (!sub_44DBE0(anim_id, &goal_data)) {
                            return;
                        }
                    }
                }
            } else {
                if (sub_424070(pc_obj, 3, false, false)
                    && sub_44D520(&goal_data, &anim_id)) {
                    if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                        sub_436C50(anim_id);
                    } else if (tig_kb_is_key_pressed(DIK_LCONTROL) || tig_kb_is_key_pressed(DIK_RCONTROL)) {
                        turn_on_running(anim_id);
                    } else {
                        if (tig_kb_get_modifier(DIK_NUMLOCK)) {
                            if (get_always_run(pc_obj)
                                && !tig_kb_is_key_pressed(DIK_LCONTROL)
                                && !tig_kb_is_key_pressed(DIK_RCONTROL)) {
                                turn_on_running(anim_id);
                            }
                        } else {
                            if (!tig_kb_is_key_pressed(DIK_LCONTROL)
                                && !tig_kb_is_key_pressed(DIK_RCONTROL)) {
                                turn_on_running(anim_id);
                            }
                        }
                    }

                    if (v26) {
                        sub_436ED0(anim_id);
                    }
                }
            }
        } else if (sub_424070(pc_obj, 3, false, false)) {
            if (tig_net_is_active()
                && !tig_kb_is_key_pressed(DIK_LSHIFT)
                && !tig_kb_is_key_pressed(DIK_RSHIFT)
                && !tig_kb_is_key_pressed(DIK_LCONTROL)
                && !tig_kb_is_key_pressed(DIK_RCONTROL)) {
                // NOTE: Some useless checks.
            }

            if (sub_44D520(&goal_data, &anim_id)
                && !tig_net_is_active()) {
                if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                    sub_436C50(anim_id);
                } else if (tig_kb_is_key_pressed(DIK_LCONTROL) || tig_kb_is_key_pressed(DIK_RCONTROL)) {
                    turn_on_running(anim_id);
                } else {
                    if (tig_kb_get_modifier(DIK_NUMLOCK)) {
                        if (get_always_run(pc_obj)
                            && !tig_kb_is_key_pressed(DIK_LCONTROL)
                            && !tig_kb_is_key_pressed(DIK_RCONTROL)) {
                            turn_on_running(anim_id);
                        }
                    } else {
                        if (!tig_kb_is_key_pressed(DIK_LCONTROL)
                            && !tig_kb_is_key_pressed(DIK_RCONTROL)) {
                            turn_on_running(anim_id);
                        }
                    }
                }

                if (v26) {
                    sub_436ED0(anim_id);
                }
            }
        }
    } else {
        if (!sub_44D520(&goal_data, &anim_id)) {
            return;
        }
    }

    // 0x54FB19
    if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
        sub_436C50(anim_id);
    } else {
        if (tig_kb_is_key_pressed(DIK_LCONTROL) || tig_kb_is_key_pressed(DIK_RCONTROL)) {
            turn_on_running(anim_id);
        } else {
            if (tig_kb_get_modifier(DIK_NUMLOCK)) {
                if (get_always_run(pc_obj)
                    && !tig_kb_is_key_pressed(DIK_LCONTROL)
                    && !tig_kb_is_key_pressed(DIK_RCONTROL)) {
                    turn_on_running(anim_id);
                }
            } else {
                if (!tig_kb_is_key_pressed(DIK_LCONTROL)
                    && !tig_kb_is_key_pressed(DIK_RCONTROL)) {
                    turn_on_running(anim_id);
                }
            }
        }
    }

    if (v26) {
        sub_436ED0(anim_id);
    }
}

// 0x54FCF0
void sub_54FCF0(Hotkey* hotkey)
{
    int64_t pc_obj;
    int64_t weapon_obj;
    int64_t v1;
    int64_t v2;

    if ((hotkey->flags & HOTKEY_DRAGGED) != 0) {
        return;
    }

    pc_obj = player_get_local_pc_obj();
    if (pc_obj == OBJ_HANDLE_NULL) {
        return;
    }

    if (critter_is_dead(pc_obj)) {
        return;
    }

    if (critter_is_unconscious(pc_obj)) {
        return;
    }

    if ((obj_field_int32_get(pc_obj, OBJ_F_CRITTER_FLAGS) & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
        return;
    }

    switch (hotkey->type) {
    case HOTKEY_ITEM:
        sub_551A80(0);
        sub_444130(&(hotkey->item_obj));
        if (obj_field_handle_get(hotkey->item_obj.obj, OBJ_F_ITEM_PARENT) == pc_obj) {
            v2 = hotkey->item_obj.obj;
            if (obj_field_int32_get(v2, OBJ_F_TYPE) != OBJ_TYPE_WRITTEN
                || (sub_462C30(pc_obj, v2)
                    && (obj_field_int32_get(v2, OBJ_F_ITEM_FLAGS) & OIF_USE_IS_THROW) == 0)) {
                v1 = item_find_first_matching_prototype(pc_obj, v2);
                if (v1 != OBJ_HANDLE_NULL) {
                    v2 = v1;
                }
            }

            if (sub_462C30(pc_obj, v2)
                || (obj_field_int32_get(v2, OBJ_F_ITEM_FLAGS) & OIF_USE_IS_THROW) != 0) {
                switch (obj_field_int32_get(v2, OBJ_F_TYPE)) {
                case OBJ_TYPE_WEAPON:
                    sub_550000(pc_obj, hotkey, ITEM_INV_LOC_WEAPON);
                    break;
                case OBJ_TYPE_AMMO:
                case OBJ_TYPE_GOLD:
                    break;
                case OBJ_TYPE_ARMOR:
                    sub_550000(pc_obj, hotkey, item_location_get(v2));
                    break;
                default:
                    v1 = sub_573620();
                    if (v1 != v2) {
                        if (v1 != OBJ_HANDLE_NULL) {
                            sub_575770();
                        }
                        inven_ui_destroy();
                        sub_573740(v2, true);
                    }
                    break;
                }
            } else if (tig_kb_is_key_pressed(DIK_LSHIFT) || tig_kb_is_key_pressed(DIK_RSHIFT)) {
                item_use(pc_obj, v2, pc_obj);
            } else {
                item_use(pc_obj, v2, OBJ_HANDLE_NULL);
            }
        }
        break;
    case HOTKEY_SKILL:
        sub_579FA0(pc_obj, hotkey->data);
        break;
    case HOTKEY_SPELL:
        sub_57BC70(pc_obj, hotkey->data);
        break;
    case HOTKEY_ITEM_SPELL:
        sub_444130(&(hotkey->item_obj));

        weapon_obj = item_wield_get(pc_obj, ITEM_INV_LOC_WEAPON);
        if (obj_field_handle_get(hotkey->item_obj.obj, OBJ_F_ITEM_PARENT) == pc_obj) {
            if (weapon_obj == hotkey->item_obj.obj) {
                sub_57C080(hotkey->item_obj.obj, hotkey->data);
            } else if (weapon_obj == OBJ_HANDLE_NULL || sub_464C80(weapon_obj)) {
                if (item_wield_set(hotkey->item_obj.obj, ITEM_INV_LOC_WEAPON)) {
                    sub_57C080(hotkey->item_obj.obj, hotkey->data);
                }
            }
        }
        break;
    }
}

// 0x550000
void sub_550000(int64_t critter_obj, Hotkey* hotkey, int inventory_location)
{
    int64_t item_obj;
    int v1;
    int sound_id;

    item_obj = item_wield_get(critter_obj, inventory_location);
    if (item_obj == hotkey->item_obj.obj) {
        return;
    }

    if (item_obj != OBJ_HANDLE_NULL) {
        v1 = sub_464D20(hotkey->item_obj.obj, inventory_location, critter_obj);
        if (v1 != 0 && v1 != 4) {
            item_error_msg(critter_obj, v1);
            return;
        }

        if (!sub_464C80(item_obj)) {
            return;
        }
    }

    v1 = sub_464D20(hotkey->item_obj.obj, inventory_location, critter_obj);
    if (v1 != 0) {
        item_error_msg(critter_obj, v1);
        if (item_obj != OBJ_HANDLE_NULL) {
            item_wield_set(item_obj, inventory_location);
        }
        return;
    }

    if (!item_wield_set(hotkey->item_obj.obj, inventory_location)) {
        item_error_msg(critter_obj, 0);
        if (item_obj != OBJ_HANDLE_NULL) {
            item_wield_set(item_obj, inventory_location);
        }
        return;
    }

    if (item_obj != OBJ_HANDLE_NULL) {
        sound_id = sub_4F0BF0(item_obj, critter_obj, OBJ_HANDLE_NULL, 1);
    } else {
        sound_id = inventory_location;
    }

    if (sound_id != -1) {
        gsound_play_sfx(sound_id, 1);
    }

    sub_57E5A0(hotkey);
}

// 0x550150
void sub_550150(Hotkey* hotkey)
{
    if ((hotkey->flags & HOTKEY_DRAGGED) != 0) {
        return;
    }

    switch (hotkey->type) {
    case HOTKEY_ITEM:
        sub_57CCF0(player_get_local_pc_obj(), hotkey->item_obj.obj);
        break;
    case HOTKEY_SKILL:
        sub_5508C0(hotkey->data);
        break;
    case HOTKEY_SPELL:
    case HOTKEY_ITEM_SPELL:
        sub_5507E0(hotkey->data);
        break;
    }
}

// 0x5501C0
bool sub_5501C0()
{
    int index;
    tig_button_handle_t college_radio_group[COLLEGE_COUNT];
    int college_radio_group_size = 0;
    int selected_college_index = 0;
    tig_button_handle_t group[3];

    for (index = 0; index < 2; index++) {
        button_create_no_art(&(stru_5C65F8[index]), 382, 41);
    }

    for (index = 0; index < COLLEGE_COUNT; index++) {
        intgame_college_buttons[index].art_num = spell_college_small_icon(index);
        if (intgame_college_buttons[index].art_num != -1) {
            intgame_button_create(&(intgame_college_buttons[index]));
            college_radio_group[college_radio_group_size] = intgame_college_buttons[index].button_handle;

            if (index == dword_64C530) {
                selected_college_index = college_radio_group_size;
            }

            college_radio_group_size++;
        }
    }

    for (index = 0; index < 4; index++) {
        stru_5C6C68[index].art_num = sub_579F50(index);
        if (stru_5C6C68[index].art_num != -1) {
            intgame_button_create(&(stru_5C6C68[index]));
        }
    }

    intgame_spells_init();
    sub_550D20();

    for (index = 0; index < 5; index++) {
        stru_64C4A8[index].x = stru_5C6D60[5].rect.x;
        stru_64C4A8[index].y = stru_5C6D60[5].rect.y + stru_5C6D60[5].rect.y / 5;
        stru_64C4A8[index].art_num = -1;
        stru_64C4A8[index].button_handle = TIG_BUTTON_HANDLE_INVALID;
        button_create_no_art(&(stru_64C4A8[index]), stru_5C6D60[5].rect.width, stru_5C6D60[5].rect.y / 5);
    }

    index = sub_551740(574, 506);
    if (index != -1) {
        sub_5708C0(dword_64C4F8[index], &(stru_5C6390[index]));
    }

    index = sub_551740(286, 524);
    if (index != -1) {
        sub_5701A0(dword_64C4F8[index], &(stru_5C6390[index]));
    }

    for (index = 0; index < 6; index++) {
        intgame_button_create(&(stru_5C6CA8[index]));
    }

    for (index = 3; index < 6; index++) {
        group[index - 3] = stru_5C6CA8[index].button_handle;
    }
    tig_button_radio_group_create(3, group, 0);

    for (index = 0; index < 5; index++) {
        intgame_button_create(&(stru_5C6D08[index]));
    }

    for (index = 1; index < 11; index++) {
        iso_interface_window_disable(index);
    }

    sub_5503F0(0, 100);

    tig_button_radio_group_create(college_radio_group_size,
        college_radio_group,
        selected_college_index);

    intgame_iso_window_type = 0;

    dword_5C6D58 = -1;

    return true;
}

// 0x5503F0
bool sub_5503F0(int a1, int a2)
{
    int v1;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    v1 = sub_551740(stru_5C6548[a1].x, stru_5C6548[a1].y);
    if (v1 == -1) {
        return false;
    }

    tig_art_interface_id_create(stru_5C6548[a1].art_num, 0, 0, 0, &(art_blit_info.art_id));
    tig_art_frame_data(art_blit_info.art_id, &art_frame_data);

    src_rect.width = art_frame_data.width;
    src_rect.height = a2 * art_frame_data.height / 100;
    src_rect.x = 0;
    src_rect.y = art_frame_data.height - src_rect.height;

    dst_rect.width = src_rect.width;
    dst_rect.height = src_rect.height;
    dst_rect.x = stru_5C6548[a1].x - stru_5C6390[v1].x;
    dst_rect.y = stru_5C6548[a1].y - stru_5C6390[v1].y;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;

    return tig_window_blit_art(dword_64C4F8[v1], &art_blit_info);
}

// 0x5504F0
void iso_interface_window_disable(int window_type)
{
    int index;

    switch (window_type) {
    case 0:
        for (index = 0; index < 2; index++) {
            tig_button_hide(stru_5C65F8[index].button_handle);
        }
        break;
    case 1:
        for (index = 0; index < COLLEGE_COUNT; index++) {
            if (spell_college_small_icon(index) != -1) {
                tig_button_hide(intgame_college_buttons[index].button_handle);
            }
        }
        intgame_spells_hide_college_spells(dword_64C530);
        break;
    case 2:
        for (index = 0; index < 4; index++) {
            if (sub_579F50(index) != -1) {
                tig_button_hide(stru_5C6C68[index].button_handle);
            }
        }
        break;
    case 3:
        sub_5709E0();
        break;
    case 4:
        break;
    case 5:
        for (index = 0; index < 5; index++) {
            tig_button_hide(stru_64C4A8[index].button_handle);
        }
        break;
    case 6:
        for (index = 0; index < 6; index++) {
            tig_button_hide(stru_5C6CA8[index].button_handle);
        }
        break;
    case 7:
        break;
    case 8:
        sub_550D60();
        break;
    case 9:
        for (index = 0; index < 5; index++) {
            tig_button_hide(stru_5C6D08[index].button_handle);
        }
        break;
    case 10:
        sub_5703B0();
        break;
    default:
        tig_debug_printf("iso_interface_window_disable: ERROR: window type out of range!\n");
        break;
    }

    dword_64C6E0 = false;
}

// 0x5506C0
void sub_5506C0(int window_type)
{
    broadcast_ui_close();
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
            intgame_message_window_clear();
        }
    }
}

// 0x550750
void sub_550750(UiMessage* ui_message)
{
    sub_552770(ui_message);
    if (dword_64C6D4 != NULL) {
        dword_64C6D4(ui_message);
    }
}

// 0x550770
void sub_550770(int a1, char* str)
{
    UiMessage ui_message;

    (void)a1;

    if (dword_64C6D4 != NULL) {
        ui_message.type = UI_MSG_TYPE_FEEDBACK;
        ui_message.str = str;
        dword_64C6D4(&ui_message);
    } else if (dword_64C6B4) {
        intgame_message_window_write_text_centered(str, &(stru_5C6D60[intgame_iso_window_type].rect));
    }
}

// 0x5507D0
void sub_5507D0(void(*func)(UiMessage* ui_message))
{
    dword_64C6D4 = func;
}

// 0x5507E0
void sub_5507E0(int spl)
{
    UiMessage ui_message;

    if (intgame_iso_window_type != 0) {
        intgame_message_window_write_text_centered(spell_name(spl), &(stru_5C6D60[intgame_iso_window_type].rect));
    } else {
        ui_message.type = UI_MSG_TYPE_SPELL;
        ui_message.field_8 = spl;
        ui_message.field_C = 0;
        ui_message.field_10 = player_get_local_pc_obj();
        sub_550750(&ui_message);
        sub_552770(&ui_message);
    }
}

// 0x550860
void sub_550860(int college)
{
    UiMessage ui_message;

    if (intgame_iso_window_type != 0) {
        intgame_message_window_write_text_centered(spell_college_name(college), &(stru_5C6D60[intgame_iso_window_type].rect));
    } else {
        ui_message.type = UI_MSG_TYPE_COLLEGE;
        ui_message.field_8 = college;
        sub_550750(&ui_message);
        sub_552770(&ui_message);
    }
}

// 0x5508C0
void sub_5508C0(int value)
{
    UiMessage ui_message;

    if (intgame_iso_window_type != 0) {
        intgame_message_window_write_text_centered(sub_57A700(value), &(stru_5C6D60[intgame_iso_window_type].rect));
    } else {
        ui_message.type = UI_MSG_TYPE_SKILL;
        ui_message.field_8 = sub_57A6A0(value);
        ui_message.field_C = 0;
        sub_550750(&ui_message);
    }
}

// 0x550930
void intgame_message_window_clear()
{
    if (!dword_64C6B4) {
        return;
    }

    if (intgame_is_compact_interface()) {
        sub_568D20();
        sub_568F40();
    } else {
        tig_window_fill(stru_5C6D60[intgame_iso_window_type].window_handle,
            &(stru_5C6D60[intgame_iso_window_type].rect),
            tig_color_make(0, 0, 0));
    }
}

// 0x5509C0
void intgame_message_window_write_text_centered(char* str, TigRect* rect)
{
    if (!dword_64C6B4) {
        return;
    }

    if (stru_5C6D60[intgame_iso_window_type].rect.width == 0) {
        return;
    }

    intgame_message_window_clear();
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        rect,
        dword_64C498,
        MSG_TEXT_HALIGN_CENTER | MSG_TEXT_VALIGN_CENTER);
}

// 0x550A10
bool intgame_message_window_write_text(tig_window_handle_t window_handle, char* str, TigRect* rect, tig_font_handle_t font, unsigned int flags)
{
    TigFont font_desc;
    TigRect text_rect;
    int width;
    int rc;

    if (!dword_64C6B4) {
        return false;
    }

    tig_font_push(font);

    font_desc.str = str;
    font_desc.width = 0;
    tig_font_measure(&font_desc);

    text_rect.x = rect->x;
    text_rect.y = rect->y;

    if (intgame_is_compact_interface()) {
        text_rect.x -= 210;
        text_rect.y -= 59;
    }

    width = rect->width;
    if ((flags & MSG_TEXT_SECONDARY) != 0) {
        width /= 2;
        text_rect.x += width;
    }

    if (font_desc.width >= width) {
        font_desc.width = width;
        tig_font_measure(&font_desc);
        if (font_desc.height > rect->height) {
            font_desc.height = rect->height;
        }
    } else {
        if ((flags & MSG_TEXT_HALIGN_CENTER) != 0) {
            text_rect.x += (width - font_desc.width) / 2;
        } else if ((flags & MSG_TEXT_HALIGN_RIGHT) != 0) {
            text_rect.x += width - font_desc.width;
        }
    }

    if ((flags & MSG_TEXT_VALIGN_CENTER) != 0) {
        text_rect.y = rect->y + (rect->height - font_desc.height) / 2;

        if (intgame_is_compact_interface()) {
            text_rect.y -= 59;
        }
    }

    text_rect.width = font_desc.width;
    text_rect.height = font_desc.height;

    if (intgame_is_compact_interface()) {
        if (str[0] == '\0') {
            // FIXME: Leaking font!
            return true;
        }

        window_handle = sub_568D20();
    }

    rc = tig_window_text_write(window_handle, str, &text_rect);
    if (rc != TIG_OK) {
        if ((flags & MSG_TEXT_TRUNCATE) != 0) {
            size_t pos;
            char ch;

            pos = strlen(str);
            while (rc != TIG_OK && pos > 0) {
                ch = str[pos - 1];
                str[pos - 1] = '\0';
                rc = tig_window_text_write(window_handle, str, &text_rect);
                str[pos - 1] = ch;
                pos--;
            }
        }
    }

    tig_font_pop();

    return rc == TIG_OK;
}

// 0x550BD0
bool intgame_spells_init()
{
    int clg;
    int spl;
    int lvl;

    for (clg = 0; clg < COLLEGE_COUNT; clg++) {
        for (lvl = 0; lvl < 5; lvl++) {
            spl = clg * 5 + lvl;
            intgame_spell_buttons[spl].art_num = spell_icon(spl);
            if (intgame_spell_buttons[spl].art_num != -1
                && !intgame_button_create(&(intgame_spell_buttons[spl]))) {
                return false;
            }
        }

        intgame_spells_hide_college_spells(clg);
    }

    return true;
}

// 0x550C60
void intgame_spells_show_college_spells(int clg)
{
    int64_t pc_obj;
    int spl;
    int lvl;

    pc_obj = player_get_local_pc_obj();
    if (pc_obj != OBJ_HANDLE_NULL) {
        for (lvl = 0; lvl < 5; lvl++) {
            spl = clg * 5 + lvl;
            if (!spell_is_known(pc_obj, spl)) {
                break;
            }

            if (intgame_spell_buttons[spl].button_handle != TIG_BUTTON_HANDLE_INVALID) {
                tig_button_show(intgame_spell_buttons[spl].button_handle);
            }
        }
    }
}

// 0x550CD0
void intgame_spells_hide_college_spells(int clg)
{
    int spl;
    int lvl;

    for (lvl = 0; lvl < 5; lvl++) {
        spl = clg * 5 + lvl;
        if (intgame_spell_buttons[spl].button_handle != TIG_BUTTON_HANDLE_INVALID) {
            tig_button_hide(intgame_spell_buttons[spl].button_handle);
        }
    }

    sub_5503F0(intgame_iso_window_type, 100);
}

// 0x550D20
bool sub_550D20()
{
    int index;

    for (index = 0; index < 5; index++) {
        stru_5C6C18[index].art_num = spell_icon(0);
        if (stru_5C6C18[index].art_num != -1) {
            if (!intgame_button_create(&(stru_5C6C18[index]))) {
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
void intgame_pc_lens_do(PcLensMode mode, PcLens* pc_lens)
{
    TigVideoBufferCreateInfo vb_create_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigArtBlitInfo blit_info;

    if (intgame_pc_lens_mode == mode) {
        return;
    }

    switch (intgame_pc_lens_mode) {
    case PC_LENS_MODE_PASSTHROUGH:
        if (tig_video_buffer_destroy(intgame_pc_lens_video_buffer) == TIG_OK) {
            intgame_pc_lens_video_buffer = NULL;
        }
        break;
    case PC_LENS_MODE_BLACKOUT:
        sub_4D81F0();
        break;
    }

    switch (mode) {
    case PC_LENS_MODE_NONE:
        intgame_pc_lens.window_handle = TIG_WINDOW_HANDLE_INVALID;
        sub_558130(NULL);
        iso_redraw();

        if (dword_5C72B0 < 1) {
            gamelib_renderlock_release();
            dword_5C72B0++;
        }
        break;
    case PC_LENS_MODE_PASSTHROUGH:
        sub_551A10(player_get_local_pc_obj());

        intgame_pc_lens.window_handle = pc_lens->window_handle;
        intgame_pc_lens.art_id = pc_lens->art_id;
        intgame_pc_lens.rect = &intgame_pc_lens_rect;
        intgame_pc_lens_rect = *pc_lens->rect;

        vb_create_info.width = stru_64C510.width;
        vb_create_info.height = stru_64C510.height;
        vb_create_info.flags = 0;
        vb_create_info.background_color = 0;

        if (tig_video_buffer_create(&vb_create_info, &intgame_pc_lens_video_buffer) == TIG_OK) {
            intgame_pc_lens_redraw();
        }

        sub_558130(&stru_64C510);

        if (dword_5C72B0 < 1) {
            gamelib_renderlock_release();
            dword_5C72B0++;
        }
        break;
    case PC_LENS_MODE_BLACKOUT:
        sub_4D81F0();

        intgame_pc_lens.window_handle = pc_lens->window_handle;
        intgame_pc_lens.art_id = pc_lens->art_id;
        intgame_pc_lens.rect = &intgame_pc_lens_rect;
        intgame_pc_lens_rect = *pc_lens->rect;

        tig_window_fill(intgame_pc_lens.window_handle,
            &intgame_pc_lens_rect,
            tig_color_make(0, 0, 0));

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = intgame_pc_lens_rect.width;
        src_rect.height = intgame_pc_lens_rect.height;

        dst_rect.x = intgame_pc_lens.rect->x;
        dst_rect.y = intgame_pc_lens.rect->y;
        dst_rect.width = intgame_pc_lens_rect.width;
        dst_rect.height = intgame_pc_lens_rect.height;

        blit_info.flags = 0;
        blit_info.art_id = intgame_pc_lens.art_id;
        blit_info.src_rect = &src_rect;
        blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(intgame_pc_lens.window_handle, &blit_info);

        sub_558130(&stru_64C698);

        gamelib_renderlock_acquire();
        dword_5C72B0--;
        break;
    }

    intgame_pc_lens_mode = mode;
}

// 0x551000
bool intgame_pc_lens_check_pt(int x, int y)
{
    TigWindowData window_data;

    if (intgame_pc_lens.window_handle == TIG_WINDOW_HANDLE_INVALID) {
        return false;
    }

    if (tig_window_data(intgame_pc_lens.window_handle, &window_data) != TIG_OK) {
        return false;
    }

    if (intgame_pc_lens.rect == NULL) {
        return false;
    }

    return x >= window_data.rect.x + intgame_pc_lens.rect->x
        && x < window_data.rect.x + intgame_pc_lens.rect->x + intgame_pc_lens.rect->width
        && y >= window_data.rect.y + intgame_pc_lens.rect->y
        && y < window_data.rect.y + intgame_pc_lens.rect->y + intgame_pc_lens.rect->height;
}

// 0x551080
void intgame_pc_lens_redraw()
{
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    if (intgame_pc_lens_video_buffer != NULL) {
        tig_window_copy(intgame_pc_lens.window_handle,
            intgame_pc_lens.rect,
            dword_64C52C,
            &stru_64C510);

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = intgame_pc_lens.rect->width;
        src_rect.height = intgame_pc_lens.rect->height;

        if (tig_window_copy_to_vbuffer(dword_64C52C, &stru_64C510, intgame_pc_lens_video_buffer, &src_rect) == TIG_OK) {
            dst_rect = src_rect;
            dst_rect.x = intgame_pc_lens.rect->x;
            dst_rect.y = intgame_pc_lens.rect->y;

            blit_info.art_id = intgame_pc_lens.art_id;
            blit_info.flags = 0;
            blit_info.src_rect = &src_rect;
            blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(intgame_pc_lens.window_handle, &blit_info);
        }
    }
}

// 0x551160
void sub_551160()
{
    int64_t pc_obj;
    int64_t obj;

    if (dword_64C6B4) {
        pc_obj = player_get_local_pc_obj();

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
            intgame_draw_counter(INTGAME_COUNTER_FATE,
                stat_level_get(pc_obj, STAT_FATE_POINTS),
                2);
            intgame_refresh_experience_gauges(pc_obj);
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

    pc_obj = player_get_local_pc_obj();

    switch (window_type) {
    case 0:
        for (index = 0; index < 2; index++) {
            tig_button_show(stru_5C65F8[index].button_handle);
        }
        break;
    case 1:
        for (index = 0; index < COLLEGE_COUNT; index++) {
            if (spell_college_small_icon(index) != -1
                && spell_college_is_known(pc_obj, index)) {
                tig_button_show(intgame_college_buttons[index].button_handle);
            }
        }
        tig_button_state_change(intgame_college_buttons[dword_64C530].button_handle, TIG_BUTTON_STATE_PRESSED);
        intgame_spells_show_college_spells(dword_64C530);
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

    if (player_get_local_pc_obj() == OBJ_HANDLE_NULL
        || qword_64C688 == OBJ_HANDLE_NULL) {
        return;
    }

    if ((obj_field_int32_get(qword_64C688, OBJ_F_ITEM_FLAGS) & OIF_IDENTIFIED) != 0
        && obj_field_int32_get(qword_64C688, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY) > 0) {
        for (index = 0; index < 5; index++) {
            spl = mt_item_spell(qword_64C688, index);
            if (spl != -1
                && !magictech_is_tech(spl)
                && stru_5C6C18[index].art_num != -1) {
                stru_5C6C18[index].art_num = spell_icon(spl);
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
bool intgame_get_location_under_cursor(int64_t* loc_ptr)
{
    TigMouseState mouse_state;
    S4F2810 v1;

    if (tig_mouse_get_state(&mouse_state) == TIG_OK
        && sub_5518C0(mouse_state.x, mouse_state.y)
        && sub_4F2CB0(mouse_state.x, mouse_state.y, &v1, Tgt_Tile, intgame_fullscreen)
        && v1.is_loc) {
        *loc_ptr = v1.loc;
        return true;
    }

    *loc_ptr = 0;
    return false;
}

// 0x5518C0
bool sub_5518C0(int x, int y)
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
void sub_551910(TigMessage* msg)
{
    S4F2810 v1;

    if (sub_5517A0(msg)) {
        sub_551F80();

        if (!map_is_clearing_objects()) {
            if (sub_4F2CB0(msg->data.mouse.x, msg->data.mouse.y, &v1, qword_5C7280, intgame_fullscreen)) {
                if (!v1.is_loc) {
                    sub_57CCF0(player_get_local_pc_obj(), v1.obj);
                    sub_43C270(v1.obj);
                }
            } else if (combat_turn_based_is_active()
                && sub_4F2CB0(msg->data.mouse.x, msg->data.mouse.y, &v1, Tgt_Tile, intgame_fullscreen)
                && v1.is_loc
                && sub_551A00() == 0) {
                sub_4B7830(player_get_local_pc_obj(), v1.loc);
            }
        }
    }
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
            location_origin_set(location);
            iso_redraw();
        }
    }
}

// 0x551A80
bool sub_551A80(int a1)
{
    int64_t pc_obj;
    int64_t obj;
    int v19;
    bool v1 = false;
    bool v2 = false;
    bool v17 = false;
    bool v18 = false;

    if (dword_64C6E8) {
        return true;
    }

    while (1) {
        dword_64C6E8 = true;
        pc_obj = player_get_local_pc_obj();
        v19 = dword_64C634[dword_64C6B8];

        if (a1 != 0) {
            dword_64C6B8++;
            v18 = true;
        } else {
            if (dword_64C6B8 > 0) {
                a1 = dword_64C634[--dword_64C6B8];
                v2 = true;
            }
            if (!sub_573620()) {
                intgame_refresh_cursor();
            }
        }

        switch (v19) {
        case 0:
        case 18:
            v1 = true;
            sub_4F25B0(Tgt_Tile | Tgt_Obj_No_T_Wall);
            break;
        case 1:
            v1 = true;
            v17 = true;
            sub_57BFF0();
            sub_4B7AA0(player_get_local_pc_obj());
            break;
        case 2:
            v1 = true;
            v17 = true;
            sub_57A1A0();
            sub_4B7AE0(player_get_local_pc_obj());
            break;
        case 3:
            sub_551A10(pc_obj);
            v1 = 1;
            if (v2) {
                sub_5678D0(player_get_local_pc_obj(), 0);
            } else {
                sub_567A20(player_get_local_pc_obj());
            }
            break;
        case 4:
            if (a1 != 13) {
                inven_ui_destroy();
            }
            v1 = true;
            if (a1 != 13) {
                intgame_unforce_fullscreen();
            }
            break;
        case 5:
            v1 = true;
            wmap_ui_close();
            if (intgame_iso_window_type == 6) {
                sub_5506C0(0);
            }
            scroll_set_scroll_func(NULL);
            intgame_unforce_fullscreen();
            break;
        case 6:
            v1 = true;
            sleep_ui_close();
            break;
        case 7:
            v1 = true;
            logbook_ui_close();
            intgame_unforce_fullscreen();
            break;
        case 8:
            switch (a1) {
            case 1:
            case 2:
            case 13:
            case 16:
                v1 = true;
                break;
            default:
                v1 = false;
                inven_ui_destroy();
                break;
            }

            if (a1 != 13) {
                intgame_unforce_fullscreen();
            }
            break;
        case 9:
            v1 = true;
            charedit_close();
            intgame_unforce_fullscreen();
            break;
        case 10:
            if (a1 == 1
                || a1 == 2) {
                if (a1 != 13) {
                    intgame_unforce_fullscreen();
                }
            } else if (a1 != 13) {
                inven_ui_destroy();
            }
            break;
        case 11:
            if (a1 != 13) {
                inven_ui_destroy();
                intgame_unforce_fullscreen();
            }
            break;
        case 13:
            sub_5506C0(0);
            obj = sub_579760();
            if (tig_net_is_active()) {
                mp_item_flags_unset(obj, OIF_NO_DISPLAY);
            } else {
                unsigned int flags;

                flags = obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS);
                flags &= ~OIF_NO_DISPLAY;
                obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, flags);
            }

            sub_576100(OBJ_HANDLE_NULL);
            break;
        case 14:
            v1 = true;
            schematic_ui_close();
            intgame_unforce_fullscreen();
            break;
        case 15:
            v1 = true;
            written_ui_close();
            intgame_unforce_fullscreen();
            break;
        case 16:
            sub_571C80();
            if (sub_573620()) {
                sub_575770();
                intgame_refresh_cursor();
            }
            break;
        case 19:
        case 20:
            if (a1 != 13) {
                inven_ui_destroy();
            }
            v1 = true;
            intgame_unforce_fullscreen();
            break;
        }

        switch (a1) {
        case 0:
        case 1:
        case 2:
            sub_5517F0();
            break;
        case 3:
            sub_568F20();
            if (!v18) {
                sub_567A60(player_get_local_pc_obj());
            }
            break;
        case 4:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 14:
        case 15:
        case 19:
        case 20:
            intgame_force_fullscreen();
            break;
        case 5:
            intgame_force_fullscreen();
            scroll_set_scroll_func(wmap_ui_scroll);
            break;
        case 13:
            obj = sub_579760();
            if (tig_net_is_active()) {
                mp_item_flags_set(obj, OIF_NO_DISPLAY);
            } else {
                unsigned int flags;

                flags = obj_field_int32_get(obj, OBJ_F_ITEM_FLAGS);
                flags |= OIF_NO_DISPLAY;
                obj_field_int32_set(obj, OBJ_F_ITEM_FLAGS, flags);
            }
            sub_576100(OBJ_HANDLE_NULL);
            break;
        }

        if (v1) {
            if (sub_573620()) {
                sub_575770();
            }
        }

        dword_64C634[dword_64C6B8] = a1;
        intgame_refresh_cursor();

        v1 = false;
        dword_64C6E8 = false;

        if (!v17) {
            return true;
        }

        a1 = 0;
        v18 = false;
        v2 = false;
        v17 = false;
    }
}

// 0x551F20
void intgame_force_fullscreen()
{
    if (intgame_is_compact_interface()) {
        intgame_toggle_interface();
        intgame_fullscreen_forced = true;
    }
}

// 0x551F40
void intgame_unforce_fullscreen()
{
    if (intgame_fullscreen_forced) {
        if (!intgame_is_compact_interface()) {
            intgame_toggle_interface();
            intgame_fullscreen_forced = false;
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
    int64_t pc_obj;

    pc_obj = player_get_local_pc_obj();
    if (pc_obj != OBJ_HANDLE_NULL) {
        if (sub_551A00()) {
            qword_5C7280 = Tgt_Object;
            return;
        }

        if (combat_critter_is_combat_mode_active(pc_obj)) {
            if (tig_kb_is_key_pressed(DIK_LALT) || tig_kb_is_key_pressed(DIK_RALT)) {
                sub_4F25B0(Tgt_Obj_No_Self | Tgt_Obj_No_T_Wall | Tgt_Tile);
                qword_5C7280 = Tgt_Obj_No_Self | Tgt_Obj_No_T_Wall;
            } else {
                sub_4F25B0(Tgt_Obj_No_Self | Tgt_Obj_No_ST_Critter_Dead | Tgt_Obj_No_T_Wall | Tgt_Tile | Tgt_Non_Party_Critters);
                qword_5C7280 = Tgt_Obj_No_Self | Tgt_Obj_No_ST_Critter_Dead | Tgt_Obj_No_T_Wall | Tgt_Non_Party_Critters;
            }
            return;
        }
    }

    sub_4F25B0(Tgt_Obj_No_T_Wall | Tgt_Tile);
    qword_5C7280 = Tgt_Object;
}

// 0x552050
bool sub_552050(TigMouseMessageData* a1, S4F2810* a2)
{
    sub_551F80();
    return sub_4F2830(a1, a2, intgame_fullscreen);
}

// 0x552070
int sub_552070()
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
            anim_ui_event_add(ANIM_UI_EVENT_TYPE_8, window_type);
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
void intgame_text_edit_refresh(const char* str, tig_font_handle_t font)
{
    intgame_text_edit_refresh_color(str,
        font,
        tig_color_make(0, 0, 0),
        0);
}

// 0x5521B0
void intgame_text_edit_refresh_color(const char* str, tig_font_handle_t font, tig_color_t color, bool a4)
{
    tig_window_handle_t window_handle;
    TigRect rect;
    TigFont font_desc;

    window_handle = stru_5C6D60[intgame_iso_window_type].window_handle;
    rect = stru_5C6D60[intgame_iso_window_type].rect;

    tig_window_fill(window_handle, &rect, color);

    if (str != NULL && *str != '\0') {
        tig_font_push(font);
        font_desc.width = 0;
        font_desc.str = str;
        tig_font_measure(&font_desc);

        // NOTE: Many jumps, check.
        if (font_desc.width > rect.width && a4) {
            while (font_desc.width > rect.width && *str != '\0') {
                font_desc.width = 0;
                font_desc.str = str++;
                tig_font_measure(&font_desc);
            }

            if (*str == '\0') {
                tig_font_pop();
                return;
            }

        }

        rect.width = font_desc.width;

        tig_window_text_write(window_handle, str, &rect);
        tig_font_pop();
    } else {
        rect.width = 0;
    }

    tig_font_push(font);
    font_desc.width = 0;
    font_desc.str = "*";
    tig_font_measure(&font_desc);
    rect.x += rect.width + 3;
    rect.width = font_desc.width;
    if (tig_window_text_write(window_handle, "*", &rect) != TIG_OK) {
        tig_debug_printf("intgame_text_edit_refresh_color: ERROR: tig_window_text_write failed!\n");
    }
    tig_font_pop();
}

// 0x5522F0
void intgame_clock_refresh()
{
    char str[32];
    TigRect rect;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigFont font_desc;
    DateTime datetime;
    int v1;
    int cycle;
    int num_cycles;
    int width;
    int dst_x;
    int dst_width;

    if (!dword_64C6B4) {
        return;
    }

    if (dword_64C6D0) {
        sprintf(str, "%d Hours", datetime_current_hour());

        rect.x = stru_5C6390[0].x + 650;
        rect.y = stru_5C6390[0].y + 5;
        rect.width = 50;
        rect.height = 25;

        font_desc.str = str;
        font_desc.width = 0;
        tig_font_measure(&font_desc);

        if (tig_window_fill(dword_64C4F8[0], &rect, tig_color_make(0, 0, 0)) == TIG_OK) {
            tig_window_text_write(dword_64C4F8[0], str, &rect);
        }
        return;
    }

    datetime = sub_45A7C0();
    v1 = (dword_64C47C[1] + dword_64C47C[0]
        + (datetime_seconds_since_reference_date(&datetime) + 73800) % 86400 * (dword_64C47C[1] + dword_64C47C[0]) / 86400
        - stru_5C6F80.width / 2) % (dword_64C47C[1] + dword_64C47C[0]);
    if (dword_5C7308 == v1) {
        return;
    }

    dword_5C7308 = v1;
    tig_window_fill(dword_64C4F8[0], &stru_5C6F80, tig_color_make(0, 0, 0));

    dst_x = stru_5C6F80.x;
    dst_width = stru_5C6F80.width;

    cycle = 0;
    num_cycles = 0;
    while (dst_width > 0 && num_cycles < 3) {
        width = dword_64C47C[cycle];
        if (cycle == 0) {
            // 207: "clk_timestrip.art"
            if (tig_art_interface_id_create(207, 0, 0, 0, &(art_blit_info.art_id)) != TIG_OK) {
                tig_debug_printf("intgame_clock_refresh: ERROR: tig_art_interface_id_create failed!\n");
                return;
            }
        } else {
            int v2;
            int idx;

            v2 = ((datetime_seconds_since_reference_date(&datetime) + 43200) / 84600) % 28;

            for (idx = 0; idx < 8; idx++) {
                if (v2 <= dword_5C730C[idx]) {
                    break;
                }
            }

            // 208 - 215: "clk_moon#.art"
            if (tig_art_interface_id_create(208 + idx, 0, 0, 0, &(art_blit_info.art_id)) != TIG_OK) {
                tig_debug_printf("intgame_clock_refresh: ERROR: tig_art_interface_id_create2 failed!\n");
                return;
            }
        }

        if (v1 < width) {
            src_rect.x = v1;
            src_rect.y = 0;
            src_rect.width = min(width - v1, dst_width);
            src_rect.height = stru_5C6F80.height;

            dst_rect.x = dst_x;
            dst_rect.y = stru_5C6F80.y;
            dst_rect.width = src_rect.width;
            dst_rect.height = stru_5C6F80.height;

            art_blit_info.flags = 0;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(dword_64C4F8[0], &art_blit_info);

            dst_x += src_rect.width;
            dst_width -= src_rect.width;

            v1 = 0;
        } else {
            v1 -= width;
        }

        cycle = 1 - cycle;
        num_cycles++;
    }

    // 216: "timepnts.art"
    if (tig_art_interface_id_create(216, 0, 0, 0, &(art_blit_info.art_id)) != TIG_OK) {
        tig_debug_printf("intgame_clock_refresh: ERROR: tig_art_interface_id_create3 failed!\n");
        return;
    }

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = 100;
    src_rect.height = 100;

    dst_rect.x = 708;
    dst_rect.y = 6;
    dst_rect.width = 100;
    dst_rect.height = 100;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(dword_64C4F8[0], &art_blit_info);
}

// 0x5526F0
bool intgame_clock_process_callback(TimeEvent* timeevent)
{
    DateTime datetime;
    TimeEvent next_timeevent;

    (void)timeevent;

    if (dword_64C6B4) {
        intgame_clock_refresh();
        next_timeevent.type = TIMEEVENT_TYPE_CLOCK;
        sub_45A950(&datetime, 3600000);
        timeevent_clear_one_typed(TIMEEVENT_TYPE_CLOCK);
        sub_45B800(&next_timeevent, &datetime);
    }

    return true;
}

// 0x552740
void sub_552740(int64_t obj, ChareditMode mode)
{
    if (!dword_64C6B4) {
        return;
    }

    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    charedit_open(obj, mode);
}

// 0x552770
void sub_552770(UiMessage* ui_message)
{
    // 0x64C6EC
    static tig_timestamp_t dword_64C6EC;

    int v1;

    if (ui_message->type >= 6 && ui_message->type <= 12) {
        if (tig_timer_elapsed(dword_64C6EC) > 3000
            && intgame_iso_window_type == 0) {
            intgame_message_window_clear();
            sub_5529C0(stru_5C6D60[intgame_iso_window_type].window_handle, ui_message, 1);
        }
    } else {
        tig_timer_now(&dword_64C6EC);

        v1 = dword_64C6C4;
        if (dword_64C6C0 >= 10) {
            dword_64C6C4 = (dword_64C6C4 + 1) % 10;
        } else {
            dword_64C6C4 = dword_64C6C0;
        }

        if (dword_64C6C0 != 0
            && (ui_message->type == 4
                || ui_message->type == 5)) {
            if (strcmpi(ui_message->str, stru_64C540[v1].str) == 0) {
                dword_64C6C4 = v1;
                dword_64C6C8 = v1;
                sub_552960(1);
                return;
            }
        }

        if (dword_64C6C0 < 10) {
            dword_64C6C0 += 1;
        }

        stru_64C540[dword_64C6C4].type = ui_message->type;
        strncpy(stru_64C540[dword_64C6C4].str, ui_message->str, 200);
        stru_64C540[dword_64C6C4].str[200 - 1] = '\0';
        stru_64C540[dword_64C6C4].field_8 = ui_message->field_8;
        stru_64C540[dword_64C6C4].field_C = ui_message->field_C;
        dword_64C6C8 = dword_64C6C4;
        sub_552960(1);
    }
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
                intgame_message_window_clear();
                sub_5529C0(stru_5C6D60[intgame_iso_window_type].window_handle,
                    &(stru_64C540[dword_64C6C8]),
                    play_sound);
            }
        }
    }
}

// 0x5529C0
void sub_5529C0(tig_window_handle_t window_handle, UiMessage* ui_message, bool play_sound)
{
    MesFileEntry mes_file_entry1;
    MesFileEntry mes_file_entry2;
    char str[MAX_STRING];

    if (intgame_is_compact_interface()) {
        window_handle = sub_568D20();
    }

    switch (ui_message->type) {
    case UI_MSG_TYPE_LEVEL:
        intgame_message_window_draw_image(window_handle, dword_5C708C[0]);

        mes_file_entry1.num = 21; // "Level Up"
        mes_get_msg(intgame_mes_file, &mes_file_entry1);
        intgame_message_window_write_text(window_handle,
            mes_file_entry1.str,
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);

        mes_file_entry1.num = 11; // "Congratulations! You are now level %d."
        mes_get_msg(intgame_mes_file, &mes_file_entry1);
        sprintf(str, mes_file_entry1.str, ui_message->field_8);
        intgame_message_window_write_text(window_handle,
            str,
            &stru_5C70D8,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);

        if (ui_message->field_C != -1) {
            mes_file_entry1.num = 11; // "You now have %d character point(s) to spend."
            mes_get_msg(intgame_mes_file, &mes_file_entry1);
            sprintf(str, mes_file_entry1.str, ui_message->field_8);
            intgame_message_window_write_text(window_handle,
                str,
                &stru_5C70E8,
                dword_64C498,
                MSG_TEXT_HALIGN_LEFT);
        } else {
            intgame_message_window_write_text(window_handle,
                ui_message->str,
                &stru_5C7128,
                dword_64C498,
                MSG_TEXT_HALIGN_LEFT);
        }

        if (play_sound) {
            gsound_play_sfx(3005, 1);
        }
        break;
    case UI_MSG_TYPE_POISON:
        intgame_message_window_draw_image(window_handle, dword_5C708C[1]);
        intgame_message_window_write_text(window_handle,
            ui_message->str,
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);

        mes_file_entry1.num = 13; // "You have absorbed %d unit(s) of poison."
        mes_get_msg(intgame_mes_file, &mes_file_entry1);
        sprintf(str, mes_file_entry1.str, ui_message->field_8);
        intgame_message_window_write_text(window_handle,
            str,
            &stru_5C70D8,
            dword_64C49C,
            MSG_TEXT_HALIGN_LEFT);

        if (play_sound) {
            gsound_play_sfx(3006, 1);
        }
        break;
    case UI_MSG_TYPE_CURSE:
        intgame_message_window_draw_image(window_handle, dword_5C708C[2]);
        intgame_message_window_write_text(window_handle,
            ui_message->str,
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);

        curse_copy_description(ui_message->field_8, str);
        intgame_message_window_write_text(window_handle,
            str,
            &stru_5C7138,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);

        if (play_sound) {
            gsound_play_sfx(3003, 1);
        }
        break;
    case UI_MSG_TYPE_BLESS:
        intgame_message_window_draw_image(window_handle, dword_5C708C[3]);
        intgame_message_window_write_text(window_handle,
            ui_message->str,
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);

        bless_copy_description(ui_message->field_8, str);
        intgame_message_window_write_text(window_handle,
            str,
            &stru_5C7138,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);

        if (play_sound) {
            gsound_play_sfx(3002, 1);
        }
        break;
    case UI_MSG_TYPE_EXCLAMATION:
        if (play_sound) {
            gsound_play_sfx(3004, 1);
        }
        // FALLTHROUGH
    case UI_MSG_TYPE_QUESTION:
    case UI_MSG_TYPE_FEEDBACK: {
        size_t pos = 0;
        bool rc;

        intgame_message_window_draw_image(window_handle, dword_5C708C[ui_message->type]);

        while (ui_message->str[pos] != '\0' && ui_message->str[pos] != '\n') {
            pos++;
        }

        if (ui_message->str[pos] == '\n') {
            ui_message->str[pos] = '\0';
            intgame_message_window_write_text(window_handle,
                ui_message->str,
                &stru_5C70C8,
                dword_739F88,
                MSG_TEXT_HALIGN_LEFT);
            rc = intgame_message_window_write_text(window_handle,
                ui_message->str + pos + 1,
                &stru_5C7138,
                dword_64C498,
                MSG_TEXT_HALIGN_LEFT);
            ui_message->str[pos] = '\n';
        } else {
            rc = intgame_message_window_write_text(window_handle, ui_message->str, &stru_5C7108, dword_64C498, 1);
        }

        if (!rc) {
            intgame_message_window_clear();
            if (ui_message->str[pos] == '\n') {
                ui_message->str[pos] = '\0';
                intgame_message_window_write_text(window_handle, ui_message->str,
                    &stru_5C7148,
                    dword_739F88,
                    MSG_TEXT_HALIGN_LEFT | MSG_TEXT_TRUNCATE);
                intgame_message_window_write_text(window_handle,
                    ui_message->str + pos + 1,
                    &stru_5C7168,
                    dword_64C498,
                    MSG_TEXT_HALIGN_LEFT | MSG_TEXT_TRUNCATE);
                ui_message->str[pos] = '\n';
            } else {
                intgame_message_window_write_text(window_handle,
                    ui_message->str,
                    &stru_5C7158,
                    dword_64C498,
                    MSG_TEXT_HALIGN_LEFT | MSG_TEXT_TRUNCATE);
            }
        }
        break;
    }
    case UI_MSG_TYPE_SKILL:
        intgame_message_window_draw_image(window_handle, dword_5C708C[6]);
        intgame_message_window_write_text(window_handle,
            IS_TECH_SKILL(ui_message->field_8)
                ? tech_skill_name(GET_TECH_SKILL(ui_message->field_8))
                : basic_skill_name(GET_BASIC_SKILL(ui_message->field_8)),
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);
        intgame_message_window_write_text(window_handle,
            IS_TECH_SKILL(ui_message->field_8)
                ? tech_skill_description(GET_TECH_SKILL(ui_message->field_8))
                : basic_skill_description(GET_BASIC_SKILL(ui_message->field_8)),
            &stru_5C7138,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);

        if (ui_message->field_C != 0) {
            mes_file_entry1.num = 28 + (IS_TECH_SKILL(ui_message->field_8)
                ? tech_skill_stat(GET_TECH_SKILL(ui_message->field_8))
                : basic_skill_stat(GET_BASIC_SKILL(ui_message->field_8)));
            mes_get_msg(intgame_mes_file, &mes_file_entry1);
            sprintf(str, "%s: %d", mes_file_entry1.str, ui_message->field_C);
            intgame_message_window_write_text(window_handle,
                str,
                &stru_5C70C8,
                dword_64C49C,
                MSG_TEXT_HALIGN_RIGHT);
        }
        break;
    case UI_MSG_TYPE_SPELL: {
        intgame_message_window_draw_image(window_handle, spell_college_large_icon(ui_message->field_8 / 5));
        intgame_message_window_write_text(window_handle,
            spell_name(ui_message->field_8),
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);

        mes_file_entry1.num = 73; // "Bonus to Heal skill"
        mes_get_msg(intgame_mes_file, &mes_file_entry1);
        sprintf(str, "%s: %d", mes_file_entry1.str, spell_min_willpower(ui_message->field_8));
        intgame_message_window_write_text(window_handle,
            str,
            &stru_5C70D8,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);

        mes_file_entry1.num = 59; // "Damage"
        mes_get_msg(intgame_mes_file, &mes_file_entry1);

        int cast_cost;
        int maintain_cost;
        int maintain_period;

        cast_cost = spell_cast_cost(ui_message->field_8, ui_message->field_10);
        maintain_cost = spell_maintain_cost(ui_message->field_8, ui_message->field_10, &maintain_period);
        if (maintain_period == 1) {
            mes_file_entry2.num = 74; // "second"
            mes_get_msg(intgame_mes_file, &mes_file_entry2);
            sprintf(str,
                "%s: %d  (%d / %s)",
                mes_file_entry1.str,
                cast_cost,
                maintain_cost,
                mes_file_entry2.str);
        } else if (maintain_period > 1) {
            mes_file_entry2.num = 75; // "seconds"
            mes_get_msg(intgame_mes_file, &mes_file_entry2);
            sprintf(str,
                "%s: %d  (%d / %d %s)",
                mes_file_entry1.str,
                cast_cost,
                maintain_cost,
                maintain_period,
                mes_file_entry2.str);
        } else {
            sprintf(str, "%s: %d", mes_file_entry1.str, cast_cost);
        }
        intgame_message_window_write_text(window_handle,
            str,
            &stru_5C70D8,
            dword_64C498,
            MSG_TEXT_HALIGN_RIGHT);

        intgame_message_window_write_text(window_handle,
            spell_description(ui_message->field_8),
            &stru_5C7128,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);
        break;
    }
    case UI_MSG_TYPE_COLLEGE:
        intgame_message_window_draw_image(window_handle, spell_college_large_icon(ui_message->field_8));
        intgame_message_window_write_text(window_handle,
            spell_college_name(ui_message->field_8),
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);
        intgame_message_window_write_text(window_handle,
            spell_college_description(ui_message->field_8),
            &stru_5C7138,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);
        break;
    case UI_MSG_TYPE_TECH:
        intgame_message_window_draw_image(window_handle, dword_5C708C[6]);
        intgame_message_window_write_text(window_handle,
            tech_discipline_name_get(ui_message->field_8),
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);
        intgame_message_window_write_text(window_handle,
            tech_discipline_description_get(ui_message->field_8),
            &stru_5C7138,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);
        break;
    case UI_MSG_TYPE_DEGREE:
        intgame_message_window_draw_image(window_handle, dword_5C708C[6]);
        intgame_message_window_write_text(window_handle,
            tech_degree_name_get(ui_message->field_8 % 8),
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);

        mes_file_entry1.num = 17;
        mes_get_msg(intgame_mes_file, &mes_file_entry1);
        sprintf(str,
            "%s: %d",
            mes_file_entry1.str,
            tech_degree_min_intelligence_get(ui_message->field_8 % 8));
        intgame_message_window_write_text(window_handle,
            str,
            &stru_5C70C8,
            dword_64C498,
            MSG_TEXT_HALIGN_RIGHT);

        intgame_message_window_write_text(window_handle,
            tech_degree_description_get(ui_message->field_8 % 8, ui_message->field_8 / 8),
            &stru_5C7138,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);
        break;
    case UI_MSG_TYPE_STAT: {
        size_t pos = 0;

        intgame_message_window_draw_image(window_handle, dword_5C708C[6]);

        while (ui_message->str[pos] != '\0' && ui_message->str[pos] != '\n') {
            pos++;
        }

        if (ui_message->str[pos] == '\n') {
            ui_message->str[pos] = '\0';
            intgame_message_window_write_text(window_handle,
                ui_message->str,
                &stru_5C70C8,
                dword_739F88,
                MSG_TEXT_HALIGN_LEFT);
            intgame_message_window_write_text(window_handle,
                ui_message->str + pos + 1,
                &stru_5C7138,
                dword_64C498,
                MSG_TEXT_HALIGN_LEFT);
            ui_message->str[pos] = '\n';
        } else {
            intgame_message_window_write_text(window_handle,
                ui_message->str,
                &stru_5C7108,
                dword_64C498,
                MSG_TEXT_HALIGN_LEFT);
        }
        break;
    }
    case UI_MSG_TYPE_SCHEMATIC:
        intgame_message_window_draw_image(window_handle, dword_5C708C[6]);
        intgame_message_window_write_text(window_handle,
            sub_56E9D0(ui_message->field_8),
            &stru_5C70C8,
            dword_739F88,
            MSG_TEXT_HALIGN_LEFT);
        intgame_message_window_write_text(window_handle,
            ui_message->str,
            &stru_5C7138,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);
        break;
    }
}

// 0x553320
bool intgame_dialog_begin(bool(*func)(TigMessage* msg))
{
    dword_64C6CC = func;
    tc_show();
    sub_551A80(0);
    sub_551A80(3);

    return true;
}

// 0x553350
void intgame_dialog_end()
{
    dword_64C6CC = NULL;
    tc_hide();
    sub_551A80(0);
}

// 0x553370
void intgame_dialog_clear()
{
    tc_clear();
}

// 0x553380
void intgame_dialog_set_option(int index, const char* str)
{
    tc_set_option(index, str);
}

// 0x5533A0
int intgame_dialog_get_option(TigMessage* msg)
{
    return tc_handle_message(msg);
}

// 0x5533B0
int sub_5533B0()
{
    return intgame_iso_window_type;
}

// 0x5533C0
void sub_5533C0(UiButtonInfo* button, int index, tig_art_id_t art_id, tig_window_handle_t window_handle)
{
    int64_t pc_obj;
    int score;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    if (!dword_64C6B4) {
        return;
    }

    if (art_id == TIG_ART_ID_INVALID) {
        pc_obj = player_get_local_pc_obj();
        if (pc_obj != OBJ_HANDLE_NULL) {
            score = stat_level_get(pc_obj, STAT_INTELLIGENCE) / 4;
        } else {
            score = 5;
        }

        if (tig_art_interface_id_create(188 + index, 0, 0, 0, &art_id) == TIG_OK) {
            tig_button_set_art(button->button_handle, art_id);
            tig_button_hide(button->button_handle);
        }

        if (index < score) {
            if (tig_art_frame_data(art_id, &art_frame_data) == TIG_OK) {
                src_rect.x = 0;
                src_rect.y = 0;
                src_rect.width = art_frame_data.width;
                src_rect.height = art_frame_data.height;

                dst_rect.x = 0;
                dst_rect.y = 0;
                dst_rect.width = art_frame_data.width;
                dst_rect.height = art_frame_data.height;

                art_blit_info.flags = 0;
                art_blit_info.art_id = art_id;
                art_blit_info.src_rect = &src_rect;
                art_blit_info.dst_rect = &dst_rect;

                if (window_handle == dword_64C4F8[0]) {
                    dst_rect.x = button->x;
                    dst_rect.y = button->y;
                } else {
                    dst_rect.x = button->x - stru_5C6EE0[index].x;
                    dst_rect.y = button->y - stru_5C6EE0[index].y;
                }

                tig_window_blit_art(window_handle, &art_blit_info);
            }
        } else {
            if (tig_art_interface_id_create(628 + index, 0, 0, 0, &art_id) == TIG_OK
                && tig_art_frame_data(art_id, &art_frame_data) == TIG_OK) {
                src_rect.x = 0;
                src_rect.y = 0;
                src_rect.width = art_frame_data.width;
                src_rect.height = art_frame_data.height;

                dst_rect.x = button->x - 1;
                dst_rect.y = button->y - 1;
                dst_rect.width = art_frame_data.width;
                dst_rect.height = art_frame_data.height;

                art_blit_info.flags = 0;
                art_blit_info.art_id = art_id;
                art_blit_info.src_rect = &src_rect;
                art_blit_info.dst_rect = &dst_rect;

                tig_window_blit_art(window_handle, &art_blit_info);
            }
        }

        if (window_handle != dword_64C4F8[0]) {
            tig_window_hide(dword_5C6378[index]);
        }
    } else {
        if (window_handle != dword_64C4F8[0]) {
            tig_window_show(dword_5C6378[index]);
        }
        tig_button_set_art(button->button_handle, art_id);
        tig_button_show(button->button_handle);
    }
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
void intgame_spell_maintain_refresh_func(tig_button_handle_t button_handle, UiButtonInfo* info, int num, bool a4, tig_window_handle_t window_handle)
{
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    int current_num;
    bool hidden;
    tig_art_id_t art_id;

    if (tig_button_data(button_handle, &button_data) != TIG_OK) {
        return;
    }

    current_num = tig_art_num_get(button_data.art_id);
    if (a4) {
        if (current_num == num + 188 && tig_button_is_hidden(button_handle, &hidden) == TIG_OK) {
            tig_button_show(button_handle);
            if (tig_art_interface_id_create(num + 188, 0, 0, 0, &art_id) != TIG_OK) {
                tig_debug_printf("intgame_spell_maintain_refresh_func: ERROR: tig_art_interface_id_create failed!\n");
                return;
            }

            tig_button_set_art(button_handle, art_id);
            if (tig_art_interface_id_create(num + 628, 0, 0, 0, &art_id) != TIG_OK) {
                return;
            }

            if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
                return;
            }

            if (tig_art_interface_id_create(185, 0, 0, 0, &art_id) != TIG_OK) {
                return;
            }

            src_rect.x = info->x - 1;
            src_rect.y = info->y - 1;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.height;

            dst_rect.x = src_rect.x;
            dst_rect.y = info->y - 1;
            dst_rect.width = art_frame_data.width;
            dst_rect.height = art_frame_data.height;

            art_blit_info.flags = 0;
            art_blit_info.art_id = art_id;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(window_handle, &art_blit_info);
        }
    } else if (current_num != num + 628) {
        if (current_num == num + 188) {
            if (tig_art_interface_id_create(num + 188, 0, 0, 0, &art_id)) {
                tig_button_set_art(button_handle, art_id);
                tig_button_hide(button_handle);
            }

            if (tig_art_interface_id_create(num + 628, 0, 0, 0, &art_id) != TIG_OK) {
                return;
            }

            if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
                return;
            }

            src_rect.x = info->x - 1;
            src_rect.y = info->y - 1;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.height;

            dst_rect.x = 0;
            dst_rect.y = 0;
            dst_rect.width = art_frame_data.width;
            dst_rect.height = art_frame_data.height;

            art_blit_info.flags = 0;
            art_blit_info.art_id = art_id;
            art_blit_info.src_rect = &dst_rect;
            art_blit_info.dst_rect = &src_rect;
            tig_window_blit_art(window_handle, &art_blit_info);
        } else {
            sub_57C370(num);
        }
    }
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
void intgame_refresh_cursor()
{
    bool have_weapon = false;
    int64_t pc_obj;
    tig_art_id_t art_id;
    int art_num;

    pc_obj = player_get_local_pc_obj();
    if (pc_obj != OBJ_HANDLE_NULL) {
        art_id = obj_field_int32_get(pc_obj, OBJ_F_CURRENT_AID);
        if (tig_art_critter_id_weapon_get(art_id) != TIG_ART_WEAPON_TYPE_NO_WEAPON) {
            have_weapon = true;
        }
    }

    if (!sub_57DE00()) {
        // FIXME: Meaningless.
        tig_mouse_cursor_get_art_id();

        art_num = dword_5C7178[sub_551A00()];
        if (art_num == -1) {
            if (have_weapon) {
                if (tig_kb_is_key_pressed(DIK_COMMA)) {
                    art_num = 818; // "cursor-called-head.art"
                } else if (tig_kb_is_key_pressed(DIK_PERIOD)) {
                    art_num = 819; // "cursor-called-leg.art"
                } else if (tig_kb_is_key_pressed(DIK_SLASH)) {
                    art_num = 820; // "cursor-called-arm.art"
                } else {
                    art_num = 353; // "battlecur.art"
                }
            } else {
                art_num = 0; // "cursor.art"
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
void sub_553A70(TigMessage* msg)
{
    int64_t obj;
    S4F2810 v1;

    if (!sub_5517A0(msg)) {
        return;
    }

    obj = sub_43C570();
    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    if (sub_4F2CB0(msg->data.mouse.x, msg->data.mouse.y, &v1, qword_5C7280, intgame_fullscreen)) {
        if (obj != v1.obj) {
            sub_57CCF0(player_get_local_pc_obj(), v1.obj);
            sub_43C270(v1.obj);
        }
    } else {
        if (qword_64C690 != OBJ_HANDLE_NULL || sub_43C570() == OBJ_HANDLE_NULL) {
            if (intgame_iso_window_type != 3) {
                if (sub_4E5470(qword_64C690)) {
                    sub_43C270(qword_64C690);
                    sub_43C270(OBJ_HANDLE_NULL);
                    sub_57CCF0(player_get_local_pc_obj(), qword_64C690);
                }
                qword_64C690 = OBJ_HANDLE_NULL;
            }
        } else {
            if (intgame_iso_window_type != 3) {
                sub_550770(-1, "");
                sub_568F40();
            }
            sub_43C270(OBJ_HANDLE_NULL);
        }
    }
}

// 0x553BE0
void sub_553BE0(int64_t a1, int64_t a2, char* str)
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
                intgame_examine_portal(a1, a2, str);
                break;
            case OBJ_TYPE_CONTAINER:
                intgame_examine_container(a1, a2, str);
                break;
            case OBJ_TYPE_SCENERY:
                intgame_examine_scenery(a1, a2, str);
                break;
            case OBJ_TYPE_PROJECTILE:
                break;
            case OBJ_TYPE_WEAPON:
            case OBJ_TYPE_AMMO:
            case OBJ_TYPE_ARMOR:
            case OBJ_TYPE_GOLD:
            case OBJ_TYPE_FOOD:
            case OBJ_TYPE_SCROLL:
            case OBJ_TYPE_KEY:
            case OBJ_TYPE_KEY_RING:
            case OBJ_TYPE_WRITTEN:
            case OBJ_TYPE_GENERIC:
                intgame_examine_item(a1, a2, str);
                break;
            case OBJ_TYPE_PC:
            case OBJ_TYPE_NPC:
                intgame_examine_critter(a1, a2, str);
                break;
            default:
                sub_550770(-1, str);
                break;
            }
        }
    }
}

// 0x553D10
bool sub_553D10(int64_t a1, int64_t a2, int* portrait_ptr)
{
    unsigned int critter_flags;

    *portrait_ptr = 432; // levelupicon.art

    switch (obj_field_int32_get(a2, OBJ_F_TYPE)) {
    case OBJ_TYPE_PORTAL:
        *portrait_ptr = tig_art_portal_id_type_get(obj_field_int32_get(a2, OBJ_F_CURRENT_AID)) == TIG_ART_PORTAL_TYPE_WINDOW
            ? 786 // iconwindow.art
            : 436; // door_icon.art
        return false;
    case OBJ_TYPE_CONTAINER:
        switch (sub_49B290(a2)) {
        case 3023:
            *portrait_ptr = 832; // cont_junk.art
            break;
        case 3033:
        case 3034:
            *portrait_ptr = 834; // cont_safe.art
            break;
        case 3055:
            *portrait_ptr = 835; // cont_trash.art
            break;
        case 3056:
            *portrait_ptr = 833; // cont_body.art
            break;
        case 3057:
            *portrait_ptr = 837; // cont_altar_good.art
            break;
        case 3058:
            *portrait_ptr = 838; // cont_altar_neutral.art
            break;
        case 3059:
            *portrait_ptr = 839; // cont_altar_evil.art
            break;
        case 3060:
            *portrait_ptr = 836; // cont_trash.art
            break;
        default:
            *portrait_ptr = 435; // containericon.art
            break;
        }
        return false;
    case OBJ_TYPE_SCENERY:
        *portrait_ptr = 437;
        return false;
    case OBJ_TYPE_WEAPON:
    case OBJ_TYPE_AMMO:
    case OBJ_TYPE_ARMOR:
    case OBJ_TYPE_GOLD:
    case OBJ_TYPE_FOOD:
    case OBJ_TYPE_SCROLL:
    case OBJ_TYPE_KEY:
    case OBJ_TYPE_KEY_RING:
    case OBJ_TYPE_WRITTEN:
    case OBJ_TYPE_GENERIC:
        *portrait_ptr = sub_554C20(a2);
        return false;
    case OBJ_TYPE_PC:
        *portrait_ptr = portrait_get(a2);
        return true;
    case OBJ_TYPE_NPC:
        if (critter_pc_leader_get(a2) == a1) {
            int portrait = portrait_get(a2);
            if (portrait != 0) {
                *portrait_ptr = portrait;
                return true;
            }
        }

        critter_flags = obj_field_int32_get(a2, OBJ_F_CRITTER_FLAGS);
        if ((critter_flags & OCF_UNDEAD) != 0) {
            *portrait_ptr = 384; // undead.art
            return false;
        }

        if ((critter_flags & OCF_MONSTER) != 0) {
            *portrait_ptr = 383; // monstericon.art
            return false;
        }

        if ((critter_flags & OCF_ANIMAL) != 0) {
            *portrait_ptr = 385; // animalicon.art
            return false;
        }

        if ((critter_flags & OCF_MECHANICAL) != 0) {
            *portrait_ptr = 434; // generaltechicon.art
            return false;
        }

        // Generic race-specific icon.
        *portrait_ptr = dword_5C6FA0[stat_level_get(a2, STAT_RACE)];
        return false;
    default:
        return false;
    }
}

// 0x553F70
void intgame_examine_critter(int64_t pc_obj, int64_t critter_obj, char* str)
{
    int obj_type;
    bool is_detecting_alignment;
    int alignment;
    MesFileEntry mes_file_entry;
    char buffer[80];
    int64_t leader_obj;

    obj_type = obj_field_int32_get(critter_obj, OBJ_F_TYPE);
    intgame_message_window_clear();

    leader_obj = critter_pc_leader_get(critter_obj);

    is_detecting_alignment = (obj_field_int32_get(pc_obj, OBJ_F_SPELL_FLAGS) & OSF_DETECTING_ALIGNMENT) != 0;
    if (is_detecting_alignment) {
        alignment = stat_level_get(critter_obj, STAT_ALIGNMENT);
    }

    if (pc_obj != critter_obj
        && combat_critter_is_combat_mode_active(pc_obj)
        && !critter_is_dead(critter_obj)) {
        sub_554830(pc_obj, critter_obj);
    } else {
        if (is_detecting_alignment) {
            int step = (stat_level_max(critter_obj, STAT_ALIGNMENT) - stat_level_min(critter_obj, STAT_ALIGNMENT)) / 6;
            int alignment_type = (alignment - stat_level_min(critter_obj, STAT_ALIGNMENT)) / step;
            if (alignment_type > 5) {
                alignment_type = 5;
            }
            intgame_message_window_draw_image(stru_5C6D60[intgame_iso_window_type].window_handle, intgame_alignment_icons[alignment_type]);
        } else {
            int portrait;

            if (sub_553D10(pc_obj, critter_obj, &portrait)) {
                intgame_draw_portrait(critter_obj, portrait, stru_5C6D60[intgame_iso_window_type].window_handle, 217, 69);
            } else {
                intgame_message_window_draw_image(stru_5C6D60[intgame_iso_window_type].window_handle, portrait);
            }
        }
    }

    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70C8,
        dword_739F88,
        MSG_TEXT_HALIGN_LEFT);

    if (critter_is_dead(critter_obj)) {
        mes_file_entry.num = 16; // "Dead"
        mes_get_msg(intgame_mes_file, &mes_file_entry);
        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            mes_file_entry.str,
            &stru_5C70D8,
            dword_64C49C,
            MSG_TEXT_HALIGN_LEFT);
        return;
    }

    if (obj_type == OBJ_TYPE_NPC) {
        if (critter_is_concealed(pc_obj)) {
            mes_file_entry.num = 37; // "Prowling state"
            mes_get_msg(intgame_mes_file, &mes_file_entry);

            int awareness_dist;
            int see_extra_dist = ai_can_see(critter_obj, pc_obj);
            if (see_extra_dist != 0) {
                awareness_dist = ai_can_hear(critter_obj, pc_obj, LOUDNESS_SILENT);
                if (awareness_dist >= see_extra_dist) {
                    awareness_dist = see_extra_dist;
                }
            } else {
                awareness_dist = 0;
            }

            if (critter_is_sleeping(critter_obj) && awareness_dist == 0) {
                awareness_dist = 1;
            }

            MesFileEntry suffix;
            switch (awareness_dist) {
            case 0:
                suffix.num = 42; // "Aware!!!"
                break;
            case 1:
                suffix.num = 41; // "Perilous!"
                break;
            case 2:
                suffix.num = 40; // "Dangerous"
                break;
            case 3:
                suffix.num = 39; // "Risky"
                break;
            default:
                suffix.num = 38; // "Safe"
                break;
            }

            mes_get_msg(intgame_mes_file, &suffix);
            sprintf(buffer, "%s: %s", mes_file_entry.str, suffix.str);
        } else {
            if (is_detecting_alignment) {
                mes_file_entry.num = 36; // "Alignment"
                mes_get_msg(intgame_mes_file, &mes_file_entry);
                sprintf(buffer, "%s: %d", mes_file_entry.str, alignment / 10);
            } else {
                int reaction_value = reaction_get(critter_obj, pc_obj);
                int reaction_level = reaction_translate(reaction_value);
                const char* reaction_name = reaction_get_name(reaction_level);

                mes_file_entry.num = 1; // "Reaction"
                mes_get_msg(intgame_mes_file, &mes_file_entry);

                sprintf(buffer,
                    "%s: %d (%s)",
                    mes_file_entry.str,
                    reaction_value,
                    reaction_name);
            }
        }

        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            buffer,
            &stru_5C70D8,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);
    }

    mes_file_entry.num = 0; // "Level"
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    sprintf(buffer, "%s: %d", mes_file_entry.str, stat_level_get(critter_obj, STAT_LEVEL));
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70D8,
        dword_64C498,
        MSG_TEXT_HALIGN_RIGHT);

    int cur_hp = object_hp_current(critter_obj);
    int max_hp = object_hp_max(critter_obj);
    int hp_ratio = 100 * cur_hp / max_hp;
    if (stat_level_get(critter_obj, STAT_POISON_LEVEL) > 0) {
        sub_554640(665, 666, &stru_5C70E8, hp_ratio);
    } else {
        sub_554640(463, 464, &stru_5C70E8, hp_ratio);
    }

    if (pc_obj == critter_obj || leader_obj == pc_obj) {
        sprintf(buffer, "%d/%d", cur_hp, max_hp);
    } else {
        sprintf(buffer, "%d%%", hp_ratio);
    }
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70E8,
        dword_64C49C,
        MSG_TEXT_HALIGN_RIGHT);

    int cur_fatigue = critter_fatigue_current(critter_obj);
    int max_fatigue = critter_fatigue_max(critter_obj);
    int fatigue_ratio = 100 * cur_fatigue / max_fatigue;
    sub_554640(465, 466, &stru_5C70F8, fatigue_ratio);

    if (pc_obj == critter_obj || leader_obj == pc_obj) {
        sprintf(buffer, "%d/%d", cur_fatigue, max_fatigue);
    } else {
        sprintf(buffer, "%d%%", fatigue_ratio);
    }
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70F8,
        dword_64C500,
        MSG_TEXT_HALIGN_RIGHT);
}

// 0x554560
void intgame_message_window_draw_image(tig_window_handle_t window_handle, int num)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    if (intgame_is_compact_interface()) {
        window_handle = sub_568D20();
    }

    tig_art_interface_id_create(num, 0, 0, 0, &art_id);
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
void sub_554640(int a1, int a2, TigRect* rect, int value)
{
    tig_window_handle_t window_handle;
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    window_handle = stru_5C6D60[intgame_iso_window_type].window_handle;
    if (intgame_is_compact_interface()) {
        window_handle = sub_568D20();
    }

    if (value < 0) {
        value = 0;
    } else if (value > 100) {
        value = 100;
    }

    if (value != 0) {
        tig_art_interface_id_create(a1, 0, 0, 0, &art_id);
        tig_art_frame_data(art_id, &art_frame_data);

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = value * art_frame_data.width / 100;
        if (src_rect.width < art_frame_data.width) {
            src_rect.width++;
        }
        src_rect.height = art_frame_data.height;

        dst_rect.x = rect->x;
        dst_rect.y = rect->y;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = art_frame_data.height;

        if (intgame_is_compact_interface()) {
            dst_rect.x -= 210;
            dst_rect.y -= 59;
        }

        art_blit_info.flags = 0;
        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;

        tig_window_blit_art(window_handle, &art_blit_info);
    }

    if (value != 100) {
        tig_art_interface_id_create(a2, 0, 0, 0, &art_id);
        tig_art_frame_data(art_id, &art_frame_data);

        src_rect.x = art_frame_data.width - art_frame_data.width * (100 - value) / 100;
        src_rect.y = 0;
        src_rect.height = art_frame_data.height;
        src_rect.width = art_frame_data.width * (100 - value) / 100;

        dst_rect.x = src_rect.x + rect->x;
        dst_rect.y = rect->y;
        dst_rect.width = src_rect.width;
        dst_rect.height = art_frame_data.height;

        if (intgame_is_compact_interface()) {
            dst_rect.x -= 210;
            dst_rect.y -= 59;
        }

        art_blit_info.flags = 0;
        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;

        tig_window_blit_art(window_handle, &art_blit_info);
    }
}

// 0x554830
void sub_554830(int64_t a1, int64_t a2)
{
    tig_window_handle_t window_handle;
    int64_t weapon_obj;
    int skill;
    int effectiveness;
    SkillInvocation skill_invocation;
    int v3;
    TigRect rect;
    char str[20];
    int penalty;
    int slot;
    MesFileEntry mes_file_entry;

    window_handle = stru_5C6D60[intgame_iso_window_type].window_handle;
    if (intgame_is_compact_interface()) {
        window_handle = sub_568D20();
    }

    sub_554B00(window_handle, 582, 207, 57);

    weapon_obj = item_wield_get(a1, ITEM_INV_LOC_WEAPON);
    skill = item_weapon_skill(weapon_obj);
    if (IS_TECH_SKILL(skill)) {
        effectiveness = tech_skill_effectiveness(a1, GET_TECH_SKILL(skill), a2);
    } else {
        effectiveness = basic_skill_effectiveness(a1, GET_BASIC_SKILL(skill), a2);
    }

    skill_invocation_init(&skill_invocation);
    sub_4440E0(a1, &(skill_invocation.source));
    sub_4440E0(a2, &(skill_invocation.target));
    sub_4440E0(weapon_obj, &(skill_invocation.item));
    skill_invocation.skill = skill;

    effectiveness -= skill_invocation_difficulty(&skill_invocation);

    if (weapon_obj != OBJ_HANDLE_NULL
        && skill != SKILL_THROWING) {
        v3 = sub_461620(weapon_obj, a1, a2);
        if (v3 > 0) {
            if (v3 > 20) {
                skill_invocation.flags |= SKILL_INVOCATION_MAGIC_TECH_PENALTY;
            }
            effectiveness -= effectiveness * v3 / 100;
        }
    }

    if (effectiveness < 0 || (skill_invocation.flags & (SKILL_INVOCATION_BLOCKED_SHOT | SKILL_INVOCATION_PENALTY_RANGE)) != 0) {
        effectiveness = 0;
    }

    if ((skill_invocation.flags & SKILL_INVOCATION_PENALTY_MASK) != 0) {
        rect.x = 215;
        rect.y = 66;
        rect.width = 64;
        rect.height = 64;

        sprintf(str, "%d%%", effectiveness);
        intgame_message_window_write_text(window_handle,
            str,
            &rect,
            dword_64C4A0,
            MSG_TEXT_HALIGN_CENTER);

        slot = 0;
        for (penalty = 0; penalty < INTGAME_PENALTY_COUNT; penalty++) {
            if (slot >= INTGAME_PENALTY_SLOTS) {
                break;
            }

            if ((intgame_penalty_flags[penalty] & skill_invocation.flags) != 0) {
                sub_554B00(window_handle,
                    intgame_penalty_icons[penalty],
                    intgame_penalty_slot_x[slot],
                    intgame_penalty_slot_y[slot]);
                slot++;
            }
        }
    } else {
        mes_file_entry.num = 20;
        mes_get_msg(intgame_mes_file, &mes_file_entry);

        rect.x = 215;
        rect.y = 85;
        rect.width = 64;
        rect.height = 64;

        intgame_message_window_write_text(window_handle,
            mes_file_entry.str,
            &rect,
            dword_64C498,
            MSG_TEXT_HALIGN_CENTER);

        sprintf(str, "%d%%", effectiveness);
        rect.y += 18;
        intgame_message_window_write_text(window_handle,
            str,
            &rect,
            dword_64C4A0,
            MSG_TEXT_HALIGN_CENTER);
    }
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
int sub_554C20(int64_t item_obj)
{
    int obj_type;
    int complexity;
    int num;
    tig_art_id_t art_id;
    int armor_coverage;

    obj_type = obj_field_int32_get(item_obj, OBJ_F_TYPE);
    complexity = obj_field_int32_get(item_obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);

    if (item_is_identified(item_obj)
        && (obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_HEXED) != 0) {
        num = 440;

        switch (obj_type) {
        case OBJ_TYPE_WEAPON:
            art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_USE_AID_FRAGMENT);
            switch (tig_art_item_id_subtype_get(art_id)) {
            case TIG_ART_WEAPON_TYPE_BOW:
                num = 792;
                break;
            case TIG_ART_WEAPON_TYPE_DAGGER:
            case TIG_ART_WEAPON_TYPE_SWORD:
            case TIG_ART_WEAPON_TYPE_AXE:
            case TIG_ART_WEAPON_TYPE_MACE:
            case TIG_ART_WEAPON_TYPE_TWO_HANDED_SWORD:
            case TIG_ART_WEAPON_TYPE_STAFF:
                num = 791;
                break;
            }
            break;
        case OBJ_TYPE_ARMOR:
            art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_INV_AID);
            armor_coverage = tig_art_item_id_armor_coverage_get(art_id);
            switch (armor_coverage) {
            case TIG_ART_ARMOR_COVERAGE_TORSO:
                art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_USE_AID_FRAGMENT);
                switch (tig_art_item_id_subtype_get(art_id)) {
                case TIG_ART_ARMOR_TYPE_VILLAGER:
                case TIG_ART_ARMOR_TYPE_CITY_DWELLER:
                    num = 794;
                    break;
                case TIG_ART_ARMOR_TYPE_ROBE:
                    num = 797;
                    break;
                default:
                    num = 793;
                    break;
                }
                break;
            case TIG_ART_ARMOR_COVERAGE_RING:
                num = 796;
                break;
            case TIG_ART_ARMOR_COVERAGE_MEDALLION:
                num = 795;
                break;
            default:
                num = 793;
                break;
            }
            break;
        }
        return num;
    }

    switch (obj_type) {
    case OBJ_TYPE_WEAPON:
        art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_USE_AID_FRAGMENT);
        num = dword_5C6FE4[tig_art_item_id_subtype_get(art_id)];
        if (num > 0) {
            num += 1;
        } else if (num < 0) {
            num += 2;
        }
        break;
    case OBJ_TYPE_AMMO:
        num = dword_5C7020[obj_field_int32_get(item_obj, OBJ_F_AMMO_TYPE)];
        break;
    case OBJ_TYPE_ARMOR:
        art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_INV_AID);
        armor_coverage = tig_art_item_id_armor_coverage_get(art_id);
        switch (armor_coverage) {
        case TIG_ART_ARMOR_COVERAGE_TORSO:
            art_id = obj_field_int32_get(item_obj, OBJ_F_ITEM_USE_AID_FRAGMENT);
            num = dword_5C7030[tig_art_item_id_subtype_get(art_id)];
            break;
        default:
            num = dword_5C7054[armor_coverage];
            break;
        }
        if (num > 0) {
            num += 1;
        } else if (num < 0) {
            num += 2;
        }
        break;
    case OBJ_TYPE_GOLD:
        num = 417;
        break;
    case OBJ_TYPE_FOOD:
        num = 423;
        if (complexity > 0) {
            num = 418;
        } else if (complexity < 0) {
            switch (obj_field_int32_get(item_obj, OBJ_F_ITEM_DISCIPLINE)) {
            case TECH_HERBOLOGY:
                num = 420;
                break;
            case TECH_CHEMISTRY:
                num = 421;
                break;
            case TECH_THERAPEUTICS:
                num = 419;
                break;
            }
        }
        break;
    case OBJ_TYPE_SCROLL:
        num = 425;
        break;
    case OBJ_TYPE_KEY:
        num = 426;
        break;
    case OBJ_TYPE_KEY_RING:
        num = 427;
        break;
    case OBJ_TYPE_WRITTEN:
        num = intgame_written_icons[obj_field_int32_get(item_obj, OBJ_F_WRITTEN_SUBTYPE)];
        break;
    case OBJ_TYPE_GENERIC:
        num = 432;
        if (complexity > 0) {
            num = 433;
        } else if (complexity < 0) {
            num = 434;
        }
        break;
    }

    return num;
}

// 0x554F10
void intgame_examine_item(int64_t pc_obj, int64_t item_obj, char* str)
{
    int obj_type;
    int64_t parent_obj;
    bool is_identified;
    int complexity;
    MesFileEntry mes_file_entry;
    MesFileEntry suffix;
    char buffer[MAX_STRING];
    int quantity_fld;
    int value;

    obj_type = obj_field_int32_get(item_obj, OBJ_F_TYPE);

    intgame_message_window_clear();

    if (item_parent(item_obj, &parent_obj)
        && parent_obj != OBJ_HANDLE_NULL
        && IS_WEAR_INV_LOC(item_inventory_location_get(item_obj))) {
        pc_obj = parent_obj;
    }

    intgame_message_window_draw_image(stru_5C6D60[intgame_iso_window_type].window_handle,
        sub_554C20(item_obj));

    is_identified = item_is_identified(item_obj);
    complexity = obj_field_int32_get(item_obj, OBJ_F_ITEM_MAGIC_TECH_COMPLEXITY);

    if ((obj_field_int32_get(item_obj, OBJ_F_ITEM_FLAGS) & OIF_HEXED) != 0
        && is_identified) {
        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            str,
            &stru_5C70C8,
            dword_64C538,
            MSG_TEXT_HALIGN_LEFT);
    } else {
        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            str,
            &stru_5C70C8,
            complexity > 0 ? dword_64C470 : dword_739F88,
            MSG_TEXT_HALIGN_LEFT);
    }

    if (obj_type == OBJ_TYPE_WEAPON) {
        value = item_weapon_magic_speed(item_obj, pc_obj);

        mes_file_entry.num = 8; // "Speed"
        mes_get_msg(intgame_mes_file, &mes_file_entry);

        sprintf(buffer, "%s: %d", mes_file_entry.str, value);
        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            buffer,
            &stru_5C70C8,
            dword_64C49C,
            MSG_TEXT_HALIGN_RIGHT);
    }

    switch (obj_type) {
    case OBJ_TYPE_WEAPON:
        mes_file_entry.num = obj_field_int32_get(item_obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS);
        mes_file_entry.str = item_effect_get_text(mes_file_entry.num);
        if (mes_file_entry.str != NULL
            && *mes_file_entry.str != '\0'
            && (complexity <= 0 || is_identified)) {
            strcpy(buffer, mes_file_entry.str);
        } else {
            sub_555910(item_obj, buffer);
        }
        break;
    case OBJ_TYPE_AMMO:
    case OBJ_TYPE_GOLD:
        sub_462410(item_obj, &quantity_fld);
        value = obj_field_int32_get(item_obj, quantity_fld);

        mes_file_entry.num = 6; // "Quantity"
        mes_get_msg(intgame_mes_file, &mes_file_entry);

        sprintf(buffer, "%s: %d", mes_file_entry.str, value);
        break;
    case OBJ_TYPE_ARMOR:
        mes_file_entry.num = obj_field_int32_get(item_obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS);
        mes_file_entry.str = item_effect_get_text(mes_file_entry.num);
        if (mes_file_entry.str != NULL
            && *mes_file_entry.str != '\0'
            && (complexity <= 0 || is_identified)) {
            strcpy(buffer, mes_file_entry.str);
        } else {
            sub_555B50(item_obj, buffer);
        }
        break;
    case OBJ_TYPE_FOOD:
    case OBJ_TYPE_SCROLL:
    case OBJ_TYPE_WRITTEN:
    case OBJ_TYPE_GENERIC:
        buffer[0] = '\0';
        if (complexity <= 0 || is_identified) {
            mes_file_entry.num = obj_field_int32_get(item_obj, OBJ_F_ITEM_DESCRIPTION_EFFECTS);
            mes_file_entry.str = item_effect_get_text(mes_file_entry.num);
            if (mes_file_entry.str != NULL) {
                strcpy(buffer, mes_file_entry.str);
            }
        }
        if (obj_type == OBJ_TYPE_GENERIC) {
            unsigned int flags = obj_field_int32_get(item_obj, OBJ_F_GENERIC_FLAGS);
            if ((flags & OGF_IS_LOCKPICK) != 0) {
                mes_file_entry.num = 71; // "Bonus to Pick Locks skill"
            } else if ((flags & OGF_IS_HEALING_ITEM) != 0) {
                mes_file_entry.num = 72; // "Bonus to Heal skill"
            } else {
                mes_file_entry.num = -1;
            }

            if (mes_file_entry.num != -1) {
                mes_get_msg(intgame_mes_file, &mes_file_entry);
                sprintf(buffer,
                    "%s: %+d%%",
                    mes_file_entry.str,
                    obj_field_int32_get(item_obj, OBJ_F_GENERIC_USAGE_BONUS));
            }
        }
        break;
    case OBJ_TYPE_KEY:
        buffer[0] = '\0';
        break;
    case OBJ_TYPE_KEY_RING:
        value = item_get_keys(item_obj, NULL);

        mes_file_entry.num = 7; // "Keys"
        mes_get_msg(intgame_mes_file, &mes_file_entry);

        sprintf(buffer, "%s: %d", mes_file_entry.str, value);
        break;
    }

    if (buffer[0] != '\0' || !intgame_is_compact_interface()) {
        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            buffer,
            &stru_5C70D8,
            dword_64C498,
            MSG_TEXT_HALIGN_LEFT);
    }

    if (is_identified
        && (obj_type == OBJ_TYPE_WEAPON
            || obj_type == OBJ_TYPE_ARMOR
            || obj_type == OBJ_TYPE_SCROLL)) {
        if (complexity > 0) {
            value = item_effective_power(item_obj, pc_obj);

            mes_file_entry.num = 2; // "Magic power available"
            mes_get_msg(intgame_mes_file, &mes_file_entry);

            sprintf(buffer, "%s: %d%%", mes_file_entry.str, 100 * value / complexity);
            intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
                buffer,
                &stru_5C70E8,
                dword_64C498,
                MSG_TEXT_HALIGN_LEFT);
        } else if (complexity < 0) {
            value = item_aptitude_crit_failure_chance(item_obj, pc_obj);

            mes_file_entry.num = 3; // "Aptitude adj to chance of critical failure"
            mes_get_msg(intgame_mes_file, &mes_file_entry);

            sprintf(buffer, "%s: %+d%%", mes_file_entry.str, value);
            intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
                buffer,
                &stru_5C70E8,
                dword_64C498,
                MSG_TEXT_HALIGN_LEFT);
        }
    }

    buffer[0] = '\0';
    value = obj_field_int32_get(item_obj, OBJ_F_ITEM_SPELL_MANA_STORE);

    if (complexity > 0 && value != 0) {
        mes_file_entry.num = 19; // "Uses"
        mes_get_msg(intgame_mes_file, &mes_file_entry);

        if (is_identified) {
            if (value > 0) {
                sprintf(buffer, "%s: +", mes_file_entry.str);
            } else {
                sprintf(buffer, "%s: %d", mes_file_entry.str, value);
            }
        } else {
            sprintf(buffer, "%s: ??", mes_file_entry.str);
        }
    } else {
        switch (obj_type) {
        case OBJ_TYPE_GENERIC:
            if ((obj_field_int32_get(item_obj, OBJ_F_GENERIC_FLAGS) & OGF_IS_HEALING_ITEM) != 0) {
                mes_file_entry.num = 19; // "Uses"
                mes_get_msg(intgame_mes_file, &mes_file_entry);

                sprintf(buffer,
                    "%s: %d",
                    mes_file_entry.str,
                    obj_field_int32_get(item_obj, OBJ_F_GENERIC_USAGE_BONUS));
            }
            break;
        case OBJ_TYPE_WEAPON: {
            int ammo_type = item_weapon_ammo_type(item_obj);
            if (ammo_type != 10000) {
                int consumption = obj_field_int32_get(item_obj, OBJ_F_WEAPON_AMMO_CONSUMPTION);
                if (consumption > 0) {
                    sprintf(buffer,
                        "%s: %d",
                        ammunition_type_get_name(ammo_type),
                        consumption);
                }
            }
            break;
        }
        }
    }

    if (buffer[0] != '\0' || !intgame_is_compact_interface()) {
        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            buffer,
            &stru_5C70E8,
            dword_64C498,
            MSG_TEXT_HALIGN_RIGHT);
    }

    value = item_weight(item_obj, pc_obj);
    mes_file_entry.num = 4; // "Weight"
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    suffix.num = 5; // "stone"
    mes_get_msg(intgame_mes_file, &suffix);
    sprintf(buffer, "%s: %d %s", mes_file_entry.str, value, suffix.str);
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70F8,
        dword_64C49C,
        MSG_TEXT_HALIGN_LEFT);

    if (tig_art_item_id_destroyed_get(obj_field_int32_get(item_obj, OBJ_F_CURRENT_AID)) == 0) {
        if (obj_type == OBJ_TYPE_WEAPON
            || obj_type == OBJ_TYPE_ARMOR
            || (obj_type == OBJ_TYPE_GENERIC
                && (obj_field_int32_get(item_obj, OBJ_F_GENERIC_FLAGS) & 0x20) != 0)) {
            sprintf(buffer,
                "%d/%d",
                object_hp_current(item_obj),
                object_hp_max(item_obj));
            intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
                buffer,
                &stru_5C70F8,
                dword_64C49C,
                MSG_TEXT_HALIGN_RIGHT);
        }
    }
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
            sprintf(tmp, "%d", min);
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
void intgame_examine_scenery(int64_t pc_obj, int64_t scenery_obj, char* str)
{
    int portrait;
    char buffer[2000];

    if (str[0] == '\0') {
        return;
    }

    intgame_message_window_clear();

    if (sub_553D10(pc_obj, scenery_obj, &portrait)) {
        intgame_draw_portrait(scenery_obj, portrait, stru_5C6D60[intgame_iso_window_type].window_handle, 217, 69);
    } else {
        intgame_message_window_draw_image(stru_5C6D60[intgame_iso_window_type].window_handle, portrait);
    }

    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C7118,
        dword_739F88,
        MSG_TEXT_HALIGN_LEFT);

    if ((obj_field_int32_get(scenery_obj, OBJ_F_FLAGS) & OF_INVULNERABLE) == 0) {
        sprintf(buffer, "%d/%d", object_hp_current(scenery_obj), object_hp_max(scenery_obj));
        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            buffer,
            &stru_5C70F8,
            dword_64C49C,
            MSG_TEXT_HALIGN_RIGHT);
    }

    if ((obj_field_int32_get(scenery_obj, OBJ_F_SCENERY_FLAGS) & OSCF_MARKS_TOWNMAP) != 0) {
        wmap_ui_mark_townmap(scenery_obj);
    }
}

// 0x555EC0
void intgame_examine_portal(int64_t pc_obj, int64_t portal_obj, char* str)
{
    int portrait;
    ObjectPortalFlags portal_flags;
    MesFileEntry mes_file_entry;
    char buffer[MAX_STRING];

    intgame_message_window_clear();

    if (sub_553D10(pc_obj, portal_obj, &portrait)) {
        intgame_draw_portrait(portal_obj, portrait, stru_5C6D60[intgame_iso_window_type].window_handle, 217, 69);
    } else {
        intgame_message_window_draw_image(stru_5C6D60[intgame_iso_window_type].window_handle, portrait);
    }

    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70C8,
        dword_739F88,
        MSG_TEXT_HALIGN_LEFT);

    portal_flags = obj_field_int32_get(portal_obj, OBJ_F_PORTAL_FLAGS);
    if ((portal_flags & OPF_JAMMED) != 0) {
        mes_file_entry.num = 26; // "Jammed"
    } else if ((portal_flags & OPF_MAGICALLY_HELD) != 0) {
        mes_file_entry.num = 27; // "Magically held"
    } else if (object_locked_get(portal_obj)) {
        mes_file_entry.num = 9; // "Locked"
    } else {
        mes_file_entry.num = 10; // "Unlocked"
    }
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        mes_file_entry.str,
        &stru_5C70F8,
        dword_64C49C,
        MSG_TEXT_HALIGN_LEFT);

    sprintf(buffer, "%d/%d", object_hp_current(portal_obj), object_hp_max(portal_obj));
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70F8,
        dword_64C49C,
        MSG_TEXT_HALIGN_RIGHT);
}

// 0x556040
void intgame_examine_container(int64_t pc_obj, int64_t container_obj, char* str)
{
    int portrait;
    unsigned int container_flags;
    MesFileEntry mes_file_entry;
    char buffer[MAX_STRING];

    intgame_message_window_clear();

    if (sub_553D10(pc_obj, container_obj, &portrait)) {
        intgame_draw_portrait(container_obj, portrait, stru_5C6D60[intgame_iso_window_type].window_handle, 217, 69);
    } else {
        intgame_message_window_draw_image(stru_5C6D60[intgame_iso_window_type].window_handle, portrait);
    }

    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70C8,
        dword_739F88,
        MSG_TEXT_HALIGN_LEFT);

    container_flags = obj_field_int32_get(container_obj, OBJ_F_CONTAINER_FLAGS);
    if ((container_flags & OCOF_JAMMED)) {
        mes_file_entry.num = 26; // "Jammed"
    } else if ((container_flags & OCOF_MAGICALLY_HELD) != 0) {
        mes_file_entry.num = 27; // "Magically held"
    } else if (object_locked_get(container_obj)) {
        mes_file_entry.num = 9; // "Locked"
    } else {
        mes_file_entry.num = 10; // "Unlocked"
    }
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        mes_file_entry.str,
        &stru_5C70F8,
        dword_64C49C,
        MSG_TEXT_HALIGN_LEFT);

    if ((obj_field_int32_get(container_obj, OBJ_F_FLAGS) & OF_INVULNERABLE) == 0) {
        sprintf(buffer, "%d/%d", object_hp_current(container_obj), object_hp_max(container_obj));
        intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
            buffer,
            &stru_5C70F8,
            dword_64C49C,
            MSG_TEXT_HALIGN_RIGHT);
    }
}

// 0x5561D0
void intgame_draw_portrait(int64_t obj, int portrait, tig_window_handle_t window_handle, int x, int y)
{
    if (intgame_is_compact_interface()) {
        window_handle = sub_568D20();
        x -= 211;
        y -= 59;
    }

    portrait_draw_native(obj, portrait, window_handle, x, y);
}

// 0x556220
void sub_556220(int64_t obj)
{
    MesFileEntry mes_file_entry;
    char str[MAX_STRING];
    int64_t weapon_obj;
    int skill;
    int effectiveness;
    int min_dam;
    int max_dam;

    mes_file_entry.num = 57; // "Total Attack"
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    sprintf(str, "%s: %d", mes_file_entry.str, item_total_attack(obj));

    if (intgame_iso_window_type != 0) {
        sub_550770(-1, str);
        return;
    }

    intgame_message_window_clear();

    intgame_message_window_draw_image(stru_5C6D60[intgame_iso_window_type].window_handle, 675);
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70C8,
        dword_739F88,
        MSG_TEXT_HALIGN_LEFT);

    weapon_obj = item_wield_get(obj, ITEM_INV_LOC_WEAPON);
    skill = item_weapon_skill(weapon_obj);

    mes_file_entry.num = 69; // "Base To Hit"
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    if (IS_TECH_SKILL(skill)) {
        effectiveness = tech_skill_effectiveness(obj, GET_TECH_SKILL(skill), OBJ_HANDLE_NULL);
    } else {
        effectiveness = basic_skill_effectiveness(obj, GET_BASIC_SKILL(skill), OBJ_HANDLE_NULL);
    }

    sprintf(str, "%s: %d%%", mes_file_entry.str, effectiveness);
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70D8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT);

    mes_file_entry.num = 59; // "Fatigue"
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    item_weapon_damage(weapon_obj, obj, 4, skill, 0, &min_dam, &max_dam);
    if (max_dam != 0) {
        sprintf(str, "%s: %d-%d", mes_file_entry.str, min_dam, max_dam);
    } else {
        sprintf(str, "%s: 0", mes_file_entry.str);
    }
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70E8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT);

    mes_file_entry.num = 61; // "Fire Damage"
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    item_weapon_damage(weapon_obj, obj, 3, skill, 0, &min_dam, &max_dam);
    if (max_dam != 0) {
        sprintf(str, "%s: %d-%d", mes_file_entry.str, min_dam, max_dam);
    } else {
        sprintf(str, "%s: 0", mes_file_entry.str);
    }
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70F8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT);

    mes_file_entry.num = 58; // "Damage"
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    item_weapon_damage(weapon_obj, obj, 0, skill, 0, &min_dam, &max_dam);
    if (max_dam != 0) {
        sprintf(str, "%s: %d-%d", mes_file_entry.str, min_dam, max_dam);
    } else {
        sprintf(str, "%s: 0", mes_file_entry.str);
    }
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70D8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT | MSG_TEXT_SECONDARY);

    mes_file_entry.num = 60; // "Electrical Damage"
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    item_weapon_damage(weapon_obj, obj, 2, skill, 0, &min_dam, &max_dam);
    if (max_dam != 0) {
        sprintf(str, "%s: %d-%d", mes_file_entry.str, min_dam, max_dam);
    } else {
        sprintf(str, "%s: 0", mes_file_entry.str);
    }
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70E8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT | MSG_TEXT_SECONDARY);

    mes_file_entry.num = 62; // "Poison"
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    item_weapon_damage(weapon_obj, obj, 2, skill, 0, &min_dam, &max_dam);
    if (max_dam != 0) {
        sprintf(str, "%s: %d-%d", mes_file_entry.str, min_dam, max_dam);
    } else {
        sprintf(str, "%s: 0", mes_file_entry.str);
    }
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        str,
        &stru_5C70F8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT | MSG_TEXT_SECONDARY);
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
    value = item_total_defence(obj);
    sprintf(buffer,
        "%s: %d",
        mes_file_entry.str,
        value);

    if (intgame_iso_window_type != 0) {
        sub_550770(-1, buffer);
        return;
    }

    intgame_message_window_clear();

    intgame_message_window_draw_image(stru_5C6D60[intgame_iso_window_type].window_handle, 674);
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70C8,
        dword_739F88,
        MSG_TEXT_HALIGN_LEFT);

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
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70D8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT);

    // Magic Resistance
    mes_file_entry.num = 65;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = object_get_resistance(obj, RESISTANCE_TYPE_MAGIC, true);
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
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70E8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT);

    // Fire Resistance
    mes_file_entry.num = 67;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = object_get_resistance(obj, RESISTANCE_TYPE_FIRE, true);
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
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70F8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT);

    // Damage Resistance
    mes_file_entry.num = 64;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = object_get_resistance(obj, RESISTANCE_TYPE_NORMAL, true);
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
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70D8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT | MSG_TEXT_SECONDARY);

    // Electrical Resistance
    mes_file_entry.num = 66;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = object_get_resistance(obj, RESISTANCE_TYPE_ELECTRICAL, true);
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
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70E8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT | MSG_TEXT_SECONDARY);

    // Poison Resistance
    mes_file_entry.num = 68;
    mes_get_msg(intgame_mes_file, &mes_file_entry);
    value = object_get_resistance(obj, RESISTANCE_TYPE_POISON, true);
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
    intgame_message_window_write_text(stru_5C6D60[intgame_iso_window_type].window_handle,
        buffer,
        &stru_5C70F8,
        dword_64C498,
        MSG_TEXT_HALIGN_LEFT | MSG_TEXT_SECONDARY);
}

// 0x556A90
void intgame_toggle_primary_button(UiPrimaryButton btn, bool on)
{
    if (on) {
        switch (btn) {
        case UI_PRIMARY_BUTTON_CHAR:
            if (charedit_is_created()) {
                return;
            }
            break;
        case UI_PRIMARY_BUTTON_LOGBOOK:
            if (logbook_ui_is_created()) {
                return;
            }
            break;
        case UI_PRIMARY_BUTTON_TOWNMAP:
        case UI_PRIMARY_BUTTON_WORLDMAP:
            if (wmap_ui_is_created()) {
                return;
            }
            break;
        case UI_PRIMARY_BUTTON_INVENTORY:
            if (inven_ui_is_created()) {
                return;
            }
            break;
        }

        intgame_ui_primary_button_icons[btn] = intgame_ui_primary_button_highlighted_icons[btn];
        intgame_refresh_primary_button(btn);
    } else {
        intgame_ui_primary_button_icons[btn] = intgame_ui_primary_button_normal_icons[btn];
        intgame_refresh_primary_button(btn);
    }
}

// 0x556B70
void intgame_set_map_button(UiPrimaryButton btn)
{
    intgame_map_button = btn;
    intgame_refresh_primary_button(btn);
}

// 0x556B90
void intgame_refresh_primary_button(UiPrimaryButton btn)
{
    tig_button_handle_t button_handle;
    tig_art_id_t art_id;

    switch (btn) {
    case UI_PRIMARY_BUTTON_CHAR:
        button_handle = intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_CHAR].button_handle;
        break;
    case UI_PRIMARY_BUTTON_LOGBOOK:
        button_handle = intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_LOGBOOK].button_handle;
        break;
    case UI_PRIMARY_BUTTON_TOWNMAP:
        if (intgame_map_button != UI_PRIMARY_BUTTON_TOWNMAP) {
            return;
        }

        button_handle = intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_MAP].button_handle;
        break;
    case UI_PRIMARY_BUTTON_WORLDMAP:
        if (intgame_map_button != UI_PRIMARY_BUTTON_WORLDMAP) {
            return;
        }

        button_handle = intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_MAP].button_handle;
        break;
    case UI_PRIMARY_BUTTON_INVENTORY:
        button_handle = intgame_primary_buttons[INTGAME_PRIMARY_BUTTON_INVENTORY].button_handle;
        break;
    }

    tig_art_interface_id_create(intgame_ui_primary_button_icons[btn], 0, 0, 0, &art_id);
    tig_button_set_art(button_handle, art_id);
}

// 0x556C20
void intgame_refresh_experience_gauges(int64_t obj)
{
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    tig_art_id_t art_id;
    int cnt;
    int index;

    if (tig_art_interface_id_create(184, 0, 0, 0, &art_id) == TIG_OK) {
        src_rect.x = 211;
        src_rect.y = 37;
        src_rect.width = 384;
        src_rect.height = 14;

        art_blit_info.flags = 0;
        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &src_rect;
        tig_window_blit_art(dword_64C4F8[1], &art_blit_info);
    }

    cnt = 11 * level_get_experience_points_to_next_level_in_percent(obj) / 10;
    for (index = 0; index < cnt / 100; index++) {
        if (tig_art_interface_id_create(stru_5C71D0[index].art_num, 0, 0, 0, &art_id) == TIG_OK
            && tig_art_frame_data(art_id, &art_frame_data) == TIG_OK) {
            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width;
            src_rect.height = art_frame_data.height;

            dst_rect.x = stru_5C71D0[index].x;
            dst_rect.y = stru_5C71D0[index].y;
            dst_rect.width = src_rect.width;
            dst_rect.height = src_rect.height;

            art_blit_info.flags = 0;
            art_blit_info.art_id = art_id;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(dword_64C4F8[1], &art_blit_info);
        }
    }

    if (cnt % 100 != 0) {
        if (tig_art_interface_id_create(stru_5C7270.art_num, 0, 0, 0, &art_id) == TIG_OK
            && tig_art_frame_data(art_id, &art_frame_data) == TIG_OK) {
            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = art_frame_data.width * (cnt % 100) / 100;
            src_rect.height = art_frame_data.height;

            dst_rect.x = stru_5C7270.x;
            dst_rect.y = stru_5C7270.y;
            dst_rect.width = src_rect.width;
            dst_rect.height = src_rect.height;

            art_blit_info.flags = 0;
            art_blit_info.art_id = art_id;
            art_blit_info.src_rect = &src_rect;
            art_blit_info.dst_rect = &dst_rect;
            tig_window_blit_art(dword_64C4F8[1], &art_blit_info);
        }
    }
}

// 0x556E60
void sub_556E60()
{
    int64_t parent_obj;

    if (qword_64C688 != OBJ_HANDLE_NULL) {
        parent_obj = obj_field_handle_get(qword_64C688, OBJ_F_ITEM_PARENT);
        if (parent_obj == OBJ_HANDLE_NULL || !player_is_local_pc_obj(parent_obj)) {
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
        qword_64C688 = item_wield_get(player_get_local_pc_obj(), ITEM_INV_LOC_WEAPON);
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
        obj = player_get_local_pc_obj();
        item_obj = item_wield_get(obj, ITEM_INV_LOC_WEAPON);
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
        sub_57CCF0(player_get_local_pc_obj(), qword_64C690);
    }
}

// 0x5570D0
void sub_5570D0(int64_t obj, bool a2, int a3)
{
    int index;
    Hotkey* v1;

    for (index = 0; index < 10; index++) {
        v1 = sub_57F240(index);
        if (a2 && v1->item_obj.obj == obj) {
            sub_57EF90(index);
        }

        if (v1->type == 1 && v1->item_obj.obj != OBJ_HANDLE_NULL) {
            v1->count = item_count_items_matching_prototype(player_get_local_pc_obj(), v1->item_obj.obj);
            intgame_hotkey_refresh(index);
        }
    }

    if (a2) {
        sub_57F2C0(obj, a2);
        if (qword_64C688 == obj && intgame_iso_window_type == 8) {
            sub_5506C0(0);
            qword_64C688 = OBJ_HANDLE_NULL;
        }

        if (a3 == 1004) {
            intgame_mt_button_disable();
        }
    } else {
        intgame_mt_button_enable();
    }

    if (sub_57DE00()) {
        intgame_refresh_cursor();
        dword_6839B0 = 0;
        dword_5CB4E4 = -1;
    }
}

// 0x5571C0
void sub_5571C0(int64_t obj)
{
    if (player_is_local_pc_obj(obj)) {
        anim_ui_event_add(ANIM_UI_EVENT_TYPE_UPDATE_HEALTH_BAR, -1);
    }

    if (sub_43C570() == obj || qword_64C690 == obj) {
        sub_57CCF0(player_get_local_pc_obj(), obj);
    }

    follower_ui_update_obj(obj);
}

// 0x557230
bool intgame_big_window_create()
{
    TigWindowData window_data;

    window_data.flags = TIG_WINDOW_MESSAGE_FILTER;
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
void sub_557370(int64_t a1, int64_t a2)
{
    unsigned int spell_flags;
    unsigned int critter_flags;
    int obj_type;

    if (a1 == OBJ_HANDLE_NULL) {
        return;
    }

    if (!critter_is_active(a1)) {
        return;
    }

    if (a2 == a1) {
        return;
    }

    spell_flags = obj_field_int32_get(a1, OBJ_F_SPELL_FLAGS);
    critter_flags = obj_field_int32_get(a1, OBJ_F_CRITTER_FLAGS);

    if ((spell_flags & OSF_STONED) != 0
        || (critter_flags & (OCF_PARALYZED | OCF_STUNNED)) != 0) {
        return;
    }

    obj_type = obj_field_int32_get(a2, OBJ_F_TYPE);

    // NOTE: The code below omit some unused checks from the original code. I'm
    // not sure if it's a bug or not.
    switch (sub_551A00()) {
    case 0:
        if (sub_573620() == OBJ_HANDLE_NULL) {
            switch (obj_type) {
            case OBJ_TYPE_WALL:
            case OBJ_TYPE_PORTAL:
            case OBJ_TYPE_SCENERY:
                if (tig_kb_is_key_pressed(DIK_LALT) || tig_kb_is_key_pressed(DIK_RALT)) {
                    sub_4B78D0(a1, a2);
                } else {
                    if ((spell_flags & OSF_POLYMORPHED) == 0) {
                        sub_4B79A0(a1, a2);
                    }
                }
                break;
            case OBJ_TYPE_CONTAINER:
                if ((spell_flags & OSF_POLYMORPHED) == 0) {
                    if (tig_kb_is_key_pressed(DIK_LALT) || tig_kb_is_key_pressed(DIK_RALT)) {
                        sub_4B78D0(a1, a2);
                    } else {
                        sub_4B79A0(a1, a2);
                    }
                }
                break;
            case OBJ_TYPE_WEAPON:
            case OBJ_TYPE_AMMO:
            case OBJ_TYPE_ARMOR:
            case OBJ_TYPE_GOLD:
            case OBJ_TYPE_FOOD:
            case OBJ_TYPE_SCROLL:
            case OBJ_TYPE_KEY:
            case OBJ_TYPE_KEY_RING:
            case OBJ_TYPE_WRITTEN:
            case OBJ_TYPE_GENERIC:
                if ((spell_flags & OSF_POLYMORPHED) == 0) {
                    sub_4B7A20(a1, a2);
                }
                break;
            case OBJ_TYPE_PC:
            case OBJ_TYPE_NPC:
                if (critter_is_dead(a2)) {
                    if (tig_kb_is_key_pressed(DIK_LALT) || tig_kb_is_key_pressed(DIK_RALT)) {
                        sub_4B79A0(a1, a2);
                    }
                } else {
                    if (!player_is_local_pc_obj(critter_pc_leader_get(a2)) || tig_kb_is_key_pressed(DIK_LALT)) {
                        sub_4B78D0(a1, a2);
                    }
                }
                break;
            case OBJ_TYPE_TRAP:
                sub_4B7830(a1, obj_field_int64_get(a2, OBJ_F_LOCATION));
                break;
            }
        }
        break;
    case 1:
        sub_4B7AA0(a1);
        break;
    case 2:
        sub_4B7AE0(a1);
        break;
    }
}

// 0x557670
void intgame_there_is_nothing_to_loot()
{
    MesFileEntry mes_file_entry;
    UiMessage ui_message;

    mes_file_entry.num = 2000;
    mes_get_msg(intgame_mes_file, &mes_file_entry);

    ui_message.type = UI_MSG_TYPE_FEEDBACK;
    ui_message.str = mes_file_entry.str;
    sub_550750(&ui_message);
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
    UiMessage ui_message;

    mes_file_entry.num = index + 3000;
    if (mes_search(intgame_mes_file, &mes_file_entry)) {
        mes_get_msg(intgame_mes_file, &mes_file_entry);

        ui_message.type = UI_MSG_TYPE_FEEDBACK;
        ui_message.str = mes_file_entry.str;
        sub_550750(&ui_message);
    }
}

// 0x557790
void sub_557790(int64_t obj)
{
    if (obj != OBJ_HANDLE_NULL
        && obj == sub_43C570()) {
        sub_57CCF0(player_get_local_pc_obj(), obj);
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

    window_data.flags = intgame_iso_window_flags | TIG_WINDOW_ALWAYS_ON_BOTTOM | TIG_WINDOW_VIDEO_MEMORY;
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
    GameResizeInfo resize_info;
    int index;

    if (!intgame_fullscreen) {
        return;
    }

    tig_debug_printf("Resizing Iso View...");

    resize_info.window_handle = dword_64C52C;
    intgame_compact_interface = !intgame_compact_interface;

    if (intgame_iso_window != TIG_WINDOW_HANDLE_INVALID) {
        if (tig_window_data(dword_64C52C, &window_data) != TIG_OK) {
            tig_debug_printf("intgame_toggle_interface: ERROR: tig_window_data failed!\n");
            exit(EXIT_FAILURE);
        }

        resize_info.window_rect = window_data.rect;
        resize_info.content_rect = window_data.rect;

        if (!intgame_is_compact_interface()) {
            resize_info.content_rect.x = 0;
            resize_info.content_rect.y = 41;
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

            resize_info.window_rect.height = 400;
            resize_info.content_rect.height = 400;

            gamelib_resize(&resize_info);
            gameuilib_resize(&resize_info);

            for (index = 0; index < 2; index++) {
                tig_window_show(dword_64C4F8[index]);
            }

            compact_ui_destroy();
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
void sub_557AC0(int clg, int lvl, UiButtonInfo* button_info)
{
    if (button_info != NULL) {
        *button_info = intgame_spell_buttons[clg * 5 + lvl];
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
int sub_557C00()
{
    TigMouseState mouse_state;
    TigButtonData button_data;
    int x;
    int y;
    int index;
    int64_t pc_obj;

    if (intgame_iso_window_type != 8) {
        return 5;
    }

    tig_mouse_get_state(&mouse_state);
    x = mouse_state.x - stru_5C6390[1].x;
    y = mouse_state.y - stru_5C6390[1].y;

    pc_obj = player_get_local_pc_obj();
    if (pc_obj == OBJ_HANDLE_NULL) {
        return 5;
    }

    for (index = 0; index < 5; index++) {
        if (tig_button_data(stru_5C6C18[index].button_handle, &button_data) != TIG_OK) {
            return 6;
        }

        if (x >= button_data.x
            && y >= button_data.y
            && x < button_data.x + button_data.width
            && y < button_data.y + button_data.height) {
            if (!sub_45A030(mt_item_spell(qword_64C688, index))) {
                return index;
            } else {
                return 5;
            }
        }
    }

    return 5;
}

// 0x557CF0
int sub_557CF0()
{
    TigMouseState mouse_state;
    TigButtonData button_data;
    int x;
    int y;
    int index;
    int64_t pc_obj;

    if (sub_557AA0() != 1) {
        return 5;
    }

    tig_mouse_get_state(&mouse_state);
    x = mouse_state.x - stru_5C6390[1].x;
    y = mouse_state.y - stru_5C6390[1].y;

    pc_obj = player_get_local_pc_obj();
    if (pc_obj == OBJ_HANDLE_NULL) {
        return 5;
    }

    for (index = 0; index < 5; index++) {
        if (tig_button_data(intgame_spell_buttons[5 * dword_64C530 + index].button_handle, &button_data) != TIG_OK) {
            return 6;
        }

        if (x >= button_data.x
            && y >= button_data.y
            && x < button_data.x + button_data.width
            && y < button_data.y + button_data.height) {
            if (spell_is_known(pc_obj, 5 * dword_64C530 + index)) {
                return index;
            } else {
                return 5;
            }
        }
    }

    return 5;
}
