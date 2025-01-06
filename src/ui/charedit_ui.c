#include "ui/charedit_ui.h"

#include <stdio.h>

#include "game/anim.h"
#include "game/combat.h"
#include "game/critter.h"
#include "game/effect.h"
#include "game/gamelib.h"
#include "game/level.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/object.h"
#include "game/player.h"
#include "game/portrait.h"
#include "game/skill.h"
#include "game/spell.h"
#include "game/stat.h"
#include "game/tech.h"
#include "game/ui.h"
#include "ui/intgame.h"
#include "ui/schematic_ui.h"
#include "ui/scrollbar_ui.h"
#include "ui/skill_ui.h"
#include "ui/spell_ui.h"
#include "ui/tech_ui.h"

typedef enum ChareditStat {
    CHAREDIT_STAT_UNSPENT_POINTS,
    CHAREDIT_STAT_LEVEL,
    CHAREDIT_STAT_XP_TO_NEXT_LEVEL,
    CHAREDIT_STAT_3,
    CHAREDIT_STAT_HP_PTS,
    CHAREDIT_STAT_5,
    CHAREDIT_STAT_FATIGUE_PTS,
    CHAREDIT_STAT_STRENGTH_BASE,
    CHAREDIT_STAT_STRENGTH_CURRENT,
    CHAREDIT_STAT_CONSTITUTION_BASE,
    CHAREDIT_STAT_CONSTITUTION_CURRENT,
    CHAREDIT_STAT_DEXTERITY_BASE,
    CHAREDIT_STAT_DEXTERITY_CURRENT,
    CHAREDIT_STAT_BEAUTY_BASE,
    CHAREDIT_STAT_BEAUTY_CURRENT,
    CHAREDIT_STAT_INTELLIGENCE_BASE,
    CHAREDIT_STAT_INTELLIGENCE_CURRENT,
    CHAREDIT_STAT_WILLPOWER_BASE,
    CHAREDIT_STAT_WILLPOWER_CURRENT,
    CHAREDIT_STAT_PERCEPTION_BASE,
    CHAREDIT_STAT_PERCEPTION_CURRENT,
    CHAREDIT_STAT_CHARISMA_BASE,
    CHAREDIT_STAT_CHARISMA_CURRENT,
    CHAREDIT_STAT_COUNT,
} ChareditStat;

typedef enum ChareditSkillGroup {
    CHAREDIT_SKILL_GROUP_COMBAT,
    CHAREDIT_SKILL_GROUP_THIEVING,
    CHAREDIT_SKILL_GROUP_SOCIAL,
    CHAREDIT_SKILL_GROUP_TECHNOLOGICAL,
    CHAREDIT_SKILL_GROUP_COUNT,
} ChareditSkillGroup;

#define CHAREDIT_SKILLS_PER_GROUP 4

typedef struct S5C8150 {
    const char* str;
    int x;
    int y;
    int value;
} S5C8150;

typedef struct S5C87D0 {
    int x;
    int y;
    tig_button_handle_t button_handle;
    int art_num;
} S5C87D0;

typedef struct S5C8CA8 {
    int x;
    int y;
    int width;
    int height;
    int field_10;
    tig_button_handle_t button_handle;
} S5C8CA8;

static void sub_55A240();
static bool sub_55A5C0(TigMessage* msg);
static void sub_55AE70(int a1);
static void sub_55B150();
static void sub_55B1B0();
static void sub_55B280();
static void sub_55B2A0(int stat);
static int sub_55B410(int stat);
static int sub_55B4D0(int64_t obj, int stat);
static void sub_55B720(int64_t obj, int stat, int value);
static void sub_55B880(tig_window_handle_t window_handle, tig_font_handle_t font, S5C8150* a3, const char** list, int a5, int a6);
static bool charedit_create_skills_win();
static void sub_55BD10(int group);
static void charedit_refresh_skills_win();
static bool charedit_create_tech_win();
static void charedit_draw_tech_degree_icon(int index);
static void charedit_refresh_tech_win();
static bool charedit_create_spells_win();
static void sub_55CA70(int a1, int a2);
static void charedit_refresh_spells_win();
static bool sub_55D060();
static void sub_55D210();
static bool sub_55D3A0(TigMessage* msg);
static bool sub_55D6F0(TigMessage* msg);
static bool charedit_tech_win_message_filter(TigMessage* msg);
static bool sub_55DC60(TigMessage* msg);
static bool sub_55DF90(TigMessage* msg);
static bool sub_55E110();
static void sub_55EBA0();
static void sub_55EC90();
static void sub_55EFB0();
static void sub_55EFE0();
static void sub_55EFF0();
static void sub_55F0D0();
static void sub_55F0E0(int value);
static void sub_55F110(TigRect* rect);

// 0x5C7E70
static struct {
    int x;
    int y;
    int field_8;
} stru_5C7E70[CHAREDIT_STAT_COUNT] = {
    { 337, 94, 3 },
    { 337, 75, 2 },
    { 337, 113, 6 },
    { 0, 0, 3 },
    { -447, 184, 3 },
    { 0, 0, 3 },
    { -447, 254, 3 },
    { 0, 0, 2 },
    { -211, 156, 2 },
    { 0, 0, 2 },
    { -211, 196, 2 },
    { 0, 0, 2 },
    { -211, 236, 2 },
    { 0, 0, 2 },
    { -211, 276, 2 },
    { 0, 0, 2 },
    { -350, 156, 2 },
    { 0, 0, 2 },
    { -350, 196, 2 },
    { 0, 0, 2 },
    { -350, 236, 2 },
    { 0, 0, 2 },
    { -350, 276, 2 },
};

// 0x5C7F88
static S5C87D0 stru_5C7F88[10] = {
    { 465, 144, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_HP_PTS },
    { 465, 214, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_FATIGUE_PTS },
    { 224, 117, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_STRENGTH_BASE },
    { 224, 157, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_CONSTITUTION_BASE },
    { 224, 197, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_DEXTERITY_BASE },
    { 224, 237, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_BEAUTY_BASE },
    { 367, 117, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_INTELLIGENCE_BASE },
    { 367, 157, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_WILLPOWER_BASE },
    { 367, 197, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_PERCEPTION_BASE },
    { 367, 237, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_CHARISMA_BASE },
};

// 0x5C8028
static S5C87D0 stru_5C8028[10] = {
    { 409, 144, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_HP_PTS },
    { 409, 214, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_FATIGUE_PTS },
    { 170, 117, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_STRENGTH_BASE },
    { 170, 157, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_CONSTITUTION_BASE },
    { 170, 197, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_DEXTERITY_BASE },
    { 170, 237, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_BEAUTY_BASE },
    { 313, 117, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_INTELLIGENCE_BASE },
    { 313, 157, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_WILLPOWER_BASE },
    { 313, 197, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_PERCEPTION_BASE },
    { 313, 237, TIG_BUTTON_HANDLE_INVALID, CHAREDIT_STAT_CHARISMA_BASE },
};

// 0x5C80C8
static int dword_5C80C8[] = {
    106,
    107,
    109,
    110,
    111,
    112,
    113,
    114,
    115,
    116,
};

// 0x5C8124
static int dword_5C8124[] = {
    -1,
    -1,
    STAT_STRENGTH,
    STAT_CONSTITUTION,
    STAT_DEXTERITY,
    STAT_BEAUTY,
    STAT_INTELLIGENCE,
    STAT_WILLPOWER,
    STAT_PERCEPTION,
    STAT_CHARISMA,
};

// 0x5C8150
static S5C8150 stru_5C8150[] = {
    { NULL, 212, 94, 0 },
    { NULL, 212, 75, 0 },
    { NULL, 212, 113, 0 },
    { NULL, 212, 56, 0 },
    { NULL, 337, 75, 0 },
    { NULL, 337, 94, 0 },
    { NULL, 337, 113, 0 },
    { NULL, -178, 323, 0 },
    { NULL, -383, 323, 0 },
};

// 0x5C81E0
static S5C8150 stru_5C81E0[] = {
    { NULL, 49, 349, STAT_CARRY_WEIGHT },
    { NULL, 49, 367, STAT_DAMAGE_BONUS },
    { NULL, 49, 385, STAT_AC_ADJUSTMENT },
    { NULL, 49, 403, STAT_SPEED },
    { NULL, 178, 349, STAT_HEAL_RATE },
    { NULL, 178, 367, STAT_POISON_RECOVERY },
    { NULL, 178, 385, STAT_REACTION_MODIFIER },
    { NULL, 178, 403, STAT_MAX_FOLLOWERS },
    { NULL, 322, 342, RESISTANCE_TYPE_NORMAL },
    { NULL, 322, 358, RESISTANCE_TYPE_MAGIC },
    { NULL, 322, 374, RESISTANCE_TYPE_FIRE },
    { NULL, 322, 390, RESISTANCE_TYPE_POISON },
    { NULL, 322, 406, RESISTANCE_TYPE_ELECTRICAL },
};

// 0x5C82B0
static S5C87D0 charedit_skill_group_buttons[CHAREDIT_SKILL_GROUP_COUNT] = {
    { 21, 17, TIG_BUTTON_HANDLE_INVALID, 302 },
    { 75, 17, TIG_BUTTON_HANDLE_INVALID, 303 },
    { 129, 17, TIG_BUTTON_HANDLE_INVALID, 304 },
    { 185, 17, TIG_BUTTON_HANDLE_INVALID, 305 },
};

// 0x5C82F0
static S5C8150 stru_5C82F0[BASIC_SKILL_COUNT] = {
    { NULL, 520, 167, BASIC_SKILL_BOW },
    { NULL, 520, 233, BASIC_SKILL_DODGE },
    { NULL, 520, 299, BASIC_SKILL_MELEE },
    { NULL, 520, 365, BASIC_SKILL_THROWING },
    { NULL, 520, 167, BASIC_SKILL_BACKSTAB },
    { NULL, 520, 233, BASIC_SKILL_PICK_POCKET },
    { NULL, 520, 299, BASIC_SKILL_PROWLING },
    { NULL, 520, 365, BASIC_SKILL_SPOT_TRAP },
    { NULL, 520, 167, BASIC_SKILL_GAMBLING },
    { NULL, 520, 233, BASIC_SKILL_HAGGLE },
    { NULL, 520, 299, BASIC_SKILL_HEAL },
    { NULL, 520, 365, BASIC_SKILL_PERSUATION },
};

// 0x5C83B0
static S5C8150 stru_5C83B0[TECH_SKILL_COUNT] = {
    { NULL, 520, 167, TECH_SKILL_REPAIR },
    { NULL, 520, 233, TECH_SKILL_FIREARMS },
    { NULL, 520, 299, TECH_SKILL_PICK_LOCKS },
    { NULL, 520, 365, TECH_SKILL_DISARM_TRAPS },
};

// 0x5C83F0
static S5C8150 stru_5C83F0[4] = {
    { NULL, 517, 206, -1 },
    { NULL, 517, 261, -1 },
    { NULL, 517, 316, -1 },
    { NULL, 517, 371, -1 },
};

// 0x5C8430
static S5C87D0 charedit_skills_plus_buttons[SKILL_COUNT] = {
    { 192, 84, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_BOW },
    { 192, 150, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_DODGE },
    { 192, 216, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_MELEE },
    { 192, 282, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_THROWING },
    { 192, 84, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_BACKSTAB },
    { 192, 150, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_PICK_POCKET },
    { 192, 216, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_PROWLING },
    { 192, 282, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_SPOT_TRAP },
    { 192, 84, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_GAMBLING },
    { 192, 150, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_HAGGLE },
    { 192, 216, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_HEAL },
    { 192, 282, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_PERSUATION },
    { 192, 84, TIG_BUTTON_HANDLE_INVALID, TECH_SKILL_REPAIR },
    { 192, 150, TIG_BUTTON_HANDLE_INVALID, TECH_SKILL_FIREARMS },
    { 192, 216, TIG_BUTTON_HANDLE_INVALID, TECH_SKILL_PICK_LOCKS },
    { 192, 282, TIG_BUTTON_HANDLE_INVALID, TECH_SKILL_DISARM_TRAPS },
};

// 0x5C8530
static S5C87D0 charedit_skills_minus_buttons[SKILL_COUNT] = {
    { 18, 84, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_BOW },
    { 18, 150, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_DODGE },
    { 18, 216, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_MELEE },
    { 18, 282, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_THROWING },
    { 18, 84, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_BACKSTAB },
    { 18, 150, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_PICK_POCKET },
    { 18, 216, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_PROWLING },
    { 18, 282, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_SPOT_TRAP },
    { 18, 84, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_GAMBLING },
    { 18, 150, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_HAGGLE },
    { 18, 216, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_HEAL },
    { 18, 282, TIG_BUTTON_HANDLE_INVALID, BASIC_SKILL_PERSUATION },
    { 18, 84, TIG_BUTTON_HANDLE_INVALID, TECH_SKILL_REPAIR },
    { 18, 150, TIG_BUTTON_HANDLE_INVALID, TECH_SKILL_FIREARMS },
    { 18, 216, TIG_BUTTON_HANDLE_INVALID, TECH_SKILL_PICK_LOCKS },
    { 18, 282, TIG_BUTTON_HANDLE_INVALID, TECH_SKILL_DISARM_TRAPS },
};

// 0x5C8630
static S5C87D0 charedit_college_buttons[COLLEGE_COUNT] = {
    { 516, 119, TIG_BUTTON_HANDLE_INVALID, COLLEGE_CONVEYANCE },
    { 541, 119, TIG_BUTTON_HANDLE_INVALID, COLLEGE_DIVINATION },
    { 566, 119, TIG_BUTTON_HANDLE_INVALID, COLLEGE_AIR },
    { 591, 119, TIG_BUTTON_HANDLE_INVALID, COLLEGE_EARTH },
    { 616, 119, TIG_BUTTON_HANDLE_INVALID, COLLEGE_FIRE },
    { 641, 119, TIG_BUTTON_HANDLE_INVALID, COLLEGE_WATER },
    { 665, 119, TIG_BUTTON_HANDLE_INVALID, COLLEGE_FORCE },
    { 690, 119, TIG_BUTTON_HANDLE_INVALID, COLLEGE_MENTAL },
    { 528, 144, TIG_BUTTON_HANDLE_INVALID, COLLEGE_META },
    { 553, 144, TIG_BUTTON_HANDLE_INVALID, COLLEGE_MORPH },
    { 578, 144, TIG_BUTTON_HANDLE_INVALID, COLLEGE_NATURE },
    { 603, 144, TIG_BUTTON_HANDLE_INVALID, COLLEGE_NECROMANTIC_BLACK },
    { 627, 144, TIG_BUTTON_HANDLE_INVALID, COLLEGE_NECROMANTIC_WHITE },
    { 652, 144, TIG_BUTTON_HANDLE_INVALID, COLLEGE_PHANTASM },
    { 677, 144, TIG_BUTTON_HANDLE_INVALID, COLLEGE_SUMMONING },
    { 702, 144, TIG_BUTTON_HANDLE_INVALID, COLLEGE_TEMPORAL },
};

// 0x5C8730
static S5C8150 charedit_spell_title_labels[5] = {
    { NULL, 562, 180, -1 },
    { NULL, 562, 228, -1 },
    { NULL, 562, 275, -1 },
    { NULL, 562, 323, -1 },
    { NULL, 562, 370, -1 },
};

// 0x5C8780
static S5C8150 charedit_spell_minimum_level_labels[5] = {
    { NULL, 562, 201, -1 },
    { NULL, 562, 249, -1 },
    { NULL, 562, 296, -1 },
    { NULL, 562, 344, -1 },
    { NULL, 562, 391, -1 },
};

// 0x5C87D0
static S5C87D0 charedit_tech_buttons[TECH_COUNT] = {
    { 8, 7, TIG_BUTTON_HANDLE_INVALID, TECH_HERBOLOGY },
    { 34, 29, TIG_BUTTON_HANDLE_INVALID, TECH_CHEMISTRY },
    { 61, 7, TIG_BUTTON_HANDLE_INVALID, TECH_ELECTRIC },
    { 88, 28, TIG_BUTTON_HANDLE_INVALID, TECH_EXPLOSIVES },
    { 116, 7, TIG_BUTTON_HANDLE_INVALID, TECH_GUN },
    { 142, 29, TIG_BUTTON_HANDLE_INVALID, TECH_MECHANICAL },
    { 171, 7, TIG_BUTTON_HANDLE_INVALID, TECH_SMITHY },
    { 198, 29, TIG_BUTTON_HANDLE_INVALID, TECH_THERAPEUTICS },
};

// 0x5C8850
static S5C8150 stru_5C8850[7] = {
    { NULL, 549, 198, -1 },
    { NULL, 549, 231, -1 },
    { NULL, 549, 264, -1 },
    { NULL, 549, 297, -1 },
    { NULL, 549, 330, -1 },
    { NULL, 549, 363, -1 },
    { NULL, 549, 396, -1 },
};

// 0x5C88C0
static S5C8150 stru_5C88C0[7] = {
    { NULL, 549, 213, -1 },
    { NULL, 549, 246, -1 },
    { NULL, 549, 279, -1 },
    { NULL, 549, 312, -1 },
    { NULL, 549, 345, -1 },
    { NULL, 549, 378, -1 },
    { NULL, 549, 411, -1 },
};

// 0x5C8930
static TigRect stru_5C8930 = { 12, 10, 89, 89 };

// 0x5C8940
static S5C8150 stru_5C8940[3] = {
    { NULL, -57, 239, 0 },
    { NULL, -767, 107, 0 },
    { NULL, -767, 406, 0 },
};

// 0x5C8970
static TigRect stru_5C8970 = { 756, 68, 30, 16 };

// 0x5C8980
static TigRect stru_5C8980 = { 756, 367, 30, 16 };

// 0x5C8E40
static S5C8150 stru_5C8E40 = { 0, -620, 125, 0 };

// 0x5C8E50
static S5C8150 stru_5C8E50[15] = {
    { NULL, 526, 170, 0 },
    { NULL, 526, 186, 0 },
    { NULL, 526, 202, 0 },
    { NULL, 526, 218, 0 },
    { NULL, 526, 234, 0 },
    { NULL, 526, 250, 0 },
    { NULL, 526, 266, 0 },
    { NULL, 526, 282, 0 },
    { NULL, 526, 298, 0 },
    { NULL, 526, 314, 0 },
    { NULL, 526, 330, 0 },
    { NULL, 526, 346, 0 },
    { NULL, 526, 362, 0 },
    { NULL, 526, 378, 0 },
    { NULL, 526, 394, 0 },
};

// 0x5C8990
UiMessage stru_5C8990 = { 4, 0, 0, 0, 0 };

static S5C8CA8 stru_5C89A8[32] = {
    { 141, 21, 66, 66, 0x64, TIG_BUTTON_HANDLE_INVALID },
    { 212, 15, 243, 75, 0x65, TIG_BUTTON_HANDLE_INVALID },
    { 15, 138, 85, 85, 0x66, TIG_BUTTON_HANDLE_INVALID },
    { 337, 34, 123, 18, 0x67, TIG_BUTTON_HANDLE_INVALID },
    { 337, 34, 123, 18, 0x68, TIG_BUTTON_HANDLE_INVALID },
    { 337, 72, 123, 18, 0x69, TIG_BUTTON_HANDLE_INVALID },
    { 410, 134, 74, 37, 0x6A, TIG_BUTTON_HANDLE_INVALID },
    { 410, 204, 74, 37, 0x6B, TIG_BUTTON_HANDLE_INVALID },
    { 751, 88, 38, 274, 0x6C, TIG_BUTTON_HANDLE_INVALID },
    { 124, 110, 126, 31, 0x6D, TIG_BUTTON_HANDLE_INVALID },
    { 124, 150, 126, 31, 0x6E, TIG_BUTTON_HANDLE_INVALID },
    { 124, 190, 126, 31, 0x6F, TIG_BUTTON_HANDLE_INVALID },
    { 124, 230, 126, 31, 0x70, TIG_BUTTON_HANDLE_INVALID },
    { 266, 110, 126, 31, 0x71, TIG_BUTTON_HANDLE_INVALID },
    { 266, 150, 126, 31, 0x72, TIG_BUTTON_HANDLE_INVALID },
    { 266, 190, 126, 31, 0x73, TIG_BUTTON_HANDLE_INVALID },
    { 266, 230, 126, 31, 0x74, TIG_BUTTON_HANDLE_INVALID },
    { 51, 282, 253, 18, 0x75, TIG_BUTTON_HANDLE_INVALID },
    { 49, 308, 125, 14, 0x76, TIG_BUTTON_HANDLE_INVALID },
    { 49, 326, 125, 14, 0x77, TIG_BUTTON_HANDLE_INVALID },
    { 49, 344, 125, 14, 0x78, TIG_BUTTON_HANDLE_INVALID },
    { 49, 362, 125, 14, 0x79, TIG_BUTTON_HANDLE_INVALID },
    { 178, 308, 125, 14, 0x7A, TIG_BUTTON_HANDLE_INVALID },
    { 178, 326, 125, 14, 0x7B, TIG_BUTTON_HANDLE_INVALID },
    { 178, 344, 125, 14, 0x7C, TIG_BUTTON_HANDLE_INVALID },
    { 178, 362, 125, 14, 0x7D, TIG_BUTTON_HANDLE_INVALID },
    { 315, 282, 136, 18, 0x7E, TIG_BUTTON_HANDLE_INVALID },
    { 322, 301, 105, 14, 0x7F, TIG_BUTTON_HANDLE_INVALID },
    { 322, 317, 105, 14, 0x80, TIG_BUTTON_HANDLE_INVALID },
    { 322, 333, 105, 14, 0x81, TIG_BUTTON_HANDLE_INVALID },
    { 322, 349, 105, 14, 0x82, TIG_BUTTON_HANDLE_INVALID },
    { 322, 365, 105, 14, 0x83, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C8CA8
static S5C8CA8 charedit_skills_hover_areas[CHAREDIT_SKILLS_PER_GROUP] = {
    { 520, 126, 200, 60, 0, TIG_BUTTON_HANDLE_INVALID },
    { 520, 192, 200, 60, 0, TIG_BUTTON_HANDLE_INVALID },
    { 520, 258, 200, 60, 0, TIG_BUTTON_HANDLE_INVALID },
    { 520, 324, 200, 60, 0, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C8D08
static S5C8CA8 stru_5C8D08[8] = {
    { 512, 129, 219, 27, 0, TIG_BUTTON_HANDLE_INVALID },
    { 535, 162, 177, 27, 0, TIG_BUTTON_HANDLE_INVALID },
    { 535, 194, 177, 27, 0, TIG_BUTTON_HANDLE_INVALID },
    { 535, 226, 177, 27, 0, TIG_BUTTON_HANDLE_INVALID },
    { 535, 258, 177, 27, 0, TIG_BUTTON_HANDLE_INVALID },
    { 535, 290, 177, 27, 0, TIG_BUTTON_HANDLE_INVALID },
    { 535, 322, 177, 27, 0, TIG_BUTTON_HANDLE_INVALID },
    { 535, 354, 177, 27, 0, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C8DC8
static S5C8CA8 stru_5C8DC8[5] = {
    { 521, 142, 196, 35, 0, TIG_BUTTON_HANDLE_INVALID },
    { 521, 190, 196, 35, 0, TIG_BUTTON_HANDLE_INVALID },
    { 521, 237, 196, 35, 0, TIG_BUTTON_HANDLE_INVALID },
    { 521, 285, 196, 35, 0, TIG_BUTTON_HANDLE_INVALID },
    { 521, 332, 196, 35, 0, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C8F40
static TigRect stru_5C8F40 = { 209, 60, 17, 255 };

// 0x5C8F50
static S5C8150 stru_5C8F50 = { NULL, 0, 0, -1 };

// 0x5C8F70
static int charedit_tech_degree_icons_x[TECH_COUNT - 1] = {
    8,
    8,
    8,
    8,
    8,
    8,
    8,
};

// 0x5C8F8C
static int charedit_tech_degree_icons_y[TECH_COUNT - 1] = {
    96,
    129,
    162,
    195,
    228,
    261,
    294,
};

// 0x5C8FA8
static int charedit_tech_degree_icons[TECH_COUNT] = {
    653,
    649,
    650,
    651,
    652,
    654,
    655,
    656,
};

// 0x5C8FC8
static S5C8150 stru_5C8FC8[3] = {
    { 0, 520, 174, -1 },
    { 0, -621, 173, -1 },
    { 0, 707, 176, -1 },
};

// 0x5C8FF8
static int dword_5C8FF8[5] = {
    521,
    521,
    521,
    521,
    521,
};

// 0x5C900C
static int dword_5C900C[11] = {
    186,
    234,
    281,
    329,
    376,
    621,
    200,
    248,
    296,
    344,
    392,
};

// 0x64C7A0
static tig_font_handle_t dword_64C7A0;

// 0x64C7A8
static ScrollbarId stru_64C7A8;

// 0x64C7B0
static tig_window_handle_t charedit_spells_win;

// 0x64C7B4
static tig_button_handle_t dword_64C7B4;

// 0x64C7B8
static int dword_64C7B8[BASIC_SKILL_COUNT];

// 0x64C7E8
static tig_button_handle_t dword_64C7E8[15];

// 0x64C824
static tig_button_handle_t dword_64C824;

// 0x64C828
static tig_font_handle_t dword_64C828;

// 0x64C82C
static int dword_64C82C[TECH_SKILL_COUNT];

// 0x64C83C
static tig_button_handle_t dword_64C83C;

// 0x64C840
static tig_font_handle_t dword_64C840;

// 0x64C844
static const char* charedit_fatigue_str;

// 0x64C848
static tig_font_handle_t dword_64C848;

// 0x64C84C
static int dword_64C84C[8][BASIC_SKILL_COUNT];

// 0x64C9CC
static int dword_64C9CC;

// 0x64C9D0
static tig_font_handle_t dword_64C9D0;

// 0x64C9D4
static int dword_64C9D4[8][TECH_SKILL_COUNT];

// 0x64CA54
static tig_button_handle_t dword_64CA54;

// 0x64CA58
static int dword_64CA58;

// 0x64CA5C
static tig_button_handle_t dword_64CA5C;

// 0x64CA60
static tig_window_handle_t dword_64CA60;

// 0x64CA64
static tig_window_handle_t dword_64CA64;

// 0x64CA68
static tig_font_handle_t dword_64CA68;

// 0x64CA6C
static tig_window_handle_t charedit_skills_win;

// 0x64CA70
static mes_file_handle_t charedit_mes_file;

// 0x64CA74
static const char* dword_64CA74[TRAINING_COUNT];

// 0x64CA84
static const char* charedit_seconds_str;

// 0x64CA88
static const char* charedit_second_str;

// 0x64CA8C
static tig_window_handle_t charedit_tech_win;

// 0x64CA90
static const char* dword_64CA90[200];

// 0x64CDB0
static tig_font_handle_t dword_64CDB0;

// 0x64CDB4
static int dword_64CDB4;

// 0x64CDB8
static tig_font_handle_t dword_64CDB8;

// 0x64CDBC
static tig_font_handle_t dword_64CDBC;

// 0x64CDC0
static tig_font_handle_t dword_64CDC0;

// 0x64CDC4
static tig_button_handle_t charedit_dec_tech_degree_btn;

// 0x64CDC8
static int dword_64CDC8;

// 0x64CDCC
static int dword_64CDCC;

// 0x64CDD0
static tig_font_handle_t dword_64CDD0;

// 0x64CDD4
static tig_button_handle_t spell_plus_bid;

// 0x64CDD8
static tig_button_handle_t dword_64CDD8;

// 0x64CDDC
static int dword_64CDDC[8][COLLEGE_COUNT];

// 0x64CFDC
static tig_button_handle_t dword_64CFDC;

// 0x64CFE0
static tig_font_handle_t dword_64CFE0;

// 0x64CFE4
static int dword_64CFE4[200];

// 0x64D304
static int dword_64D304[23];

// 0x64D360
static int dword_64D360;

// 0x64D364
static int dword_64D364[COLLEGE_COUNT];

// 0x64D3A4
static tig_font_handle_t dword_64D3A4;

// 0x64D3A8
static tig_font_handle_t dword_64D3A8;

// 0x64D3AC
static tig_button_handle_t dword_64D3AC;

// 0x64D3B0
static tig_font_handle_t dword_64D3B0;

// 0x64D3B4
static tig_button_handle_t charedit_inc_tech_degree_btn;

// 0x64D3B8
static tig_button_handle_t dword_64D3B8;

// 0x64D3BC
static tig_font_handle_t dword_64D3BC;

// 0x64D3C0
static const char* charedit_quest_str;

// 0x64D3C4
char* dword_64D3C4[23];

// 0x64D420
static tig_font_handle_t dword_64D420;

// 0x64D424
static int dword_64D424;

// 0x64D428
static tig_button_handle_t dword_64D428;

// 0x64D42C
static tig_font_handle_t dword_64D42C;

// 0x64D430
static tig_button_handle_t spell_minus_bid;

// 0x64D434
static int dword_64D434[8][CHAREDIT_STAT_COUNT];

// 0x64D714
static char byte_64D714[2000];

// 0x64DEE4
static bool dword_64DEE4;

// 0x64DEE8
static const char* charedit_minimum_level_str;

// 0x64DEEC
static int dword_64DEEC[TECH_COUNT];

// 0x64DF0C
static tig_font_handle_t dword_64DF0C;

// 0x64DF10
static int dword_64DF10[8][TECH_COUNT];

// 0x64E010
static int64_t qword_64E010;

// 0x64E018
bool charedit_created;

// 0x64E01C
static int dword_64E01C;

// 0x64E020
static int dword_64E020;

// 0x64E024
static int dword_64E024;

// 0x64E028
static int charedit_selected_tech;

// 0x64E02C
static bool dword_64E02C;

// 0x559690
bool charedit_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!sub_55E110()) {
        return false;
    }

    if (!sub_55D060()) {
        sub_55EBA0();
        return false;
    }

    if (!charedit_create_spells_win()) {
        sub_55EBA0();
        tig_window_destroy(dword_64CA60);
        return false;
    }

    if (!charedit_create_tech_win()) {
        sub_55EBA0();
        tig_window_destroy(charedit_spells_win);
        tig_window_destroy(dword_64CA60);
        return false;
    }

    if (!charedit_create_skills_win()) {
        sub_55EBA0();
        tig_window_destroy(charedit_spells_win);
        tig_window_destroy(charedit_tech_win);
        tig_window_destroy(dword_64CA60);
        return false;
    }

    tig_window_hide(charedit_skills_win);
    tig_window_hide(charedit_spells_win);
    tig_window_hide(charedit_tech_win);
    tig_window_hide(dword_64CA60);

    return true;
}

// 0x559770
void charedit_exit()
{
    sub_55EBA0();
    tig_window_destroy(charedit_skills_win);
    tig_window_destroy(charedit_spells_win);
    tig_window_destroy(charedit_tech_win);
    tig_window_destroy(dword_64CA60);
}

// 0x5597B0
void charedit_reset()
{
    if (charedit_is_created()) {
        charedit_destroy();
    }
}

// 0x5597C0
bool charedit_create(int64_t obj, int a2)
{
    int64_t pc_obj;
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigWindowData window_data;
    TigButtonData button_data;
    int index;
    int portrait;
    S550DA0 v1;
    Packet127 pkt;

    if (charedit_created) {
        charedit_destroy();
        return true;
    }

    pc_obj = player_get_pc_obj();
    if (critter_is_dead(pc_obj) || !sub_551A80(0) || !sub_551A80(9)) {
        return false;
    }

    qword_64E010 = obj;
    dword_64CDCC = a2;

    if (dword_64CDCC == 0) {
        object_hp_damage_set(obj, 0);
        critter_fatigue_damage_set(obj, 0);
    }

    tig_art_interface_id_create(22, 0, 0, 0, &art_id);
    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        qword_64E010 = OBJ_HANDLE_NULL;
        return false;
    }

    if (!intgame_big_window_lock(sub_55A5C0, &dword_64CA64)) {
        qword_64E010 = OBJ_HANDLE_NULL;
        return false;
    }

    tig_window_data(dword_64CA64, &window_data);

    window_data.rect.x = 0;
    window_data.rect.y = 0;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = window_data.rect.width;
    dst_rect.height = window_data.rect.height;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &(window_data.rect);
    art_blit_info.dst_rect = &dst_rect;
    if (tig_window_blit_art(dword_64CA64, &art_blit_info) != TIG_OK) {
        intgame_big_window_unlock();
        qword_64E010 = OBJ_HANDLE_NULL;
        return false;
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = dword_64CA64;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    tig_art_interface_id_create(24, 0, 0, 0, &button_data.art_id);
    for (index = 0; index < 10; index++) {
        if (dword_64CDCC == 2) {
            stru_5C7F88[index].button_handle = TIG_BUTTON_HANDLE_INVALID;
        } else {
            button_data.x = stru_5C7F88[index].x;
            button_data.y = stru_5C7F88[index].y;
            if (tig_button_create(&button_data, &(stru_5C7F88[index].button_handle))) {
                intgame_big_window_unlock();
                qword_64E010 = OBJ_HANDLE_NULL;
                return false;
            }
        }
    }

    tig_art_interface_id_create(23, 0, 0, 0, &button_data.art_id);
    for (index = 0; index < 10; index++) {
        if (dword_64CDCC == 2) {
            stru_5C8028[index].button_handle = TIG_BUTTON_HANDLE_INVALID;
        } else {
            button_data.x = stru_5C8028[index].x;
            button_data.y = stru_5C8028[index].y;
            if (tig_button_create(&button_data, &(stru_5C8028[index].button_handle))) {
                intgame_big_window_unlock();
                qword_64E010 = OBJ_HANDLE_NULL;
                return false;
            }
        }
    }

    sub_55A240();

    if (sub_553D10(pc_obj, qword_64E010, &portrait)) {
        portrait_draw_native(qword_64E010, portrait, dword_64CA64, stru_5C89A8[0].x, stru_5C89A8[0].y);
    } else {
        tig_art_interface_id_create(portrait, 0, 0, 0, &art_id);
        tig_art_frame_data(art_id, &art_frame_data);

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.height;

        dst_rect.x = stru_5C89A8[0].x;
        dst_rect.y = stru_5C89A8[0].y;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = art_frame_data.height;

        art_blit_info.flags = 0;
        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &src_rect;
        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(dword_64CA64, &art_blit_info);
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = dword_64CA64;
    tig_art_interface_id_create(34, 0, 0, 0, &(button_data.art_id));
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    button_data.x = 527;
    button_data.y = 11;
    if (tig_button_create(&button_data, &dword_64C7B4) != TIG_OK) {
        intgame_big_window_unlock();
        qword_64E010 = OBJ_HANDLE_NULL;
        return false;
    }

    button_data.x = 595;
    tig_art_interface_id_create(35, 0, 0, 0, &(button_data.art_id));
    if (tig_button_create(&button_data, &dword_64C83C) != TIG_OK) {
        intgame_big_window_unlock();
        qword_64E010 = OBJ_HANDLE_NULL;
        return false;
    }

    button_data.x = 663;
    tig_art_interface_id_create(36, 0, 0, 0, &(button_data.art_id));
    if (tig_button_create(&button_data, &dword_64D3B8) != TIG_OK) {
        intgame_big_window_unlock();
        qword_64E010 = OBJ_HANDLE_NULL;
        return false;
    }

    button_data.x = 754;
    button_data.y = 21;
    tig_art_interface_id_create(568, 0, 0, 0, &(button_data.art_id));
    if (tig_button_create(&button_data, &dword_64CA54) != TIG_OK) {
        intgame_big_window_unlock();
        qword_64E010 = OBJ_HANDLE_NULL;
        return false;
    }

    tig_window_show(charedit_skills_win);
    tig_window_show(charedit_spells_win);
    tig_window_show(charedit_tech_win);
    tig_window_show(dword_64CA60);
    sub_51E850(charedit_skills_win);

    if (dword_64CDCC == 2) {
        for (index = 0; index < 15; index++) {
            tig_button_hide(dword_64C7E8[index]);
        }
    } else {
        for (index = 0; index < 15; index++) {
            tig_button_show(dword_64C7E8[index]);
        }
    }

    dword_64E01C = 0;
    sub_55BD10(dword_64E020);

    if (qword_64E010 != OBJ_HANDLE_NULL) {
        dword_64D428 = TIG_BUTTON_HANDLE_INVALID;
        dword_64CFDC = TIG_BUTTON_HANDLE_INVALID;
    } else {
        button_data.flags = 1;
        button_data.window_handle = dword_64CA64;
        button_data.mouse_down_snd_id = 3000;
        button_data.mouse_up_snd_id = 3001;
        button_data.mouse_enter_snd_id = -1;
        button_data.mouse_exit_snd_id = -1;

        tig_art_interface_id_create(24, 0, 0, 0, &(button_data.art_id));
        button_data.x = 82;
        button_data.y = 196;
        tig_button_create(&button_data, &dword_64D428);

        button_data.y += 12;
        tig_art_interface_id_create(23, 0, 0, 0, &(button_data.art_id));
        tig_button_create(&button_data, &dword_64CFDC);
    }

    sub_55B150();
    location_origin_set(obj_field_int64_get(qword_64E010, OBJ_F_LOCATION));

    v1.window_handle = dword_64CA64;
    v1.rect = &stru_5C8930;
    tig_art_interface_id_create(198, 0, 0, 0, &v1.art_id);

    if (dword_64CDCC == 1 || dword_64CDCC == 2) {
        sub_550DA0(1, &v1);
    }

    if (dword_64CDCC != 3) {
        if ((tig_net_flags & TIG_NET_CONNECTED) == 0
            || (tig_net_flags & TIG_NET_HOST) != 0
            || multiplayer_is_locked()) {
            for (index = 0; index < 23; index++) {
                dword_64D304[index] = sub_55B4D0(qword_64E010, index);
            }

            for (index = 0; index < TECH_COUNT; index++) {
                dword_64DEEC[index] = tech_get_degree(qword_64E010, index);
            }

            for (index = 0; index < COLLEGE_COUNT; index++) {
                dword_64D364[index] = spell_college_level_get(qword_64E010, index);
            }

            for (index = 0; index < BASIC_SKILL_COUNT; index++) {
                dword_64C7B8[index] = basic_skill_get_base(qword_64E010, index);
            }

            for (index = 0; index < TECH_SKILL_COUNT; index++) {
                dword_64C82C[index] = tech_skill_get_base(qword_64E010, index);
            }
        } else {
            pkt.type = 127;
            pkt.field_4 = 0;
            tig_net_send_app_all(&pkt, sizeof(pkt));
        }
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = dword_64CA64;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;
    button_data.art_id = TIG_ART_ID_INVALID;

    for (index = 0; index < 32; index++) {
        button_data.x = stru_5C89A8[index].x;
        button_data.y = stru_5C89A8[index].y;
        button_data.width = stru_5C89A8[index].width;
        button_data.height = stru_5C89A8[index].height;
        tig_button_create(&button_data, &stru_5C89A8[index].button_handle);
    }

    dword_64C824 = TIG_BUTTON_HANDLE_INVALID;
    dword_64CA5C = TIG_BUTTON_HANDLE_INVALID;

    if (dword_64CDCC == 2
        && critter_leader_get(qword_64E010) == pc_obj
        && sub_45E590(qword_64E010) != qword_64E010) {
        button_data.flags = TIG_BUTTON_FLAG_0x01;
        button_data.window_handle = dword_64CA64;

        tig_art_interface_id_create(827, 0, 0, 0, &(button_data.art_id));
        button_data.x = 126;
        button_data.y = 44;
        tig_button_create(&button_data, &dword_64C824);

        tig_art_interface_id_create(828, 0, 0, 0, &(button_data.art_id));
        button_data.x = 461;
        button_data.y = 44;
        tig_button_create(&button_data, &dword_64CA5C);
    }

    dword_64C9CC = -1;
    dword_64CA58 = -1;
    dword_64CDB4 = -1;
    dword_64D360 = -1;

    if (player_is_pc_obj(obj)) {
        sub_460790(0, 0);
    }

    sub_424070(obj, 4, 0, 1);

    charedit_created = true;

    if (combat_auto_attack_get(pc_obj)) {
        dword_64E02C = true;
        combat_auto_attack_set(0);
    }

    return true;
}

// 0x55A150
void charedit_destroy()
{
    if (charedit_created) {
        charedit_created = false;
        if (dword_64CDCC == 1 || dword_64CDCC == 2) {
            sub_550DA0(0, 0);
        }
        if (dword_64CDCC == 0) {
            object_hp_damage_set(qword_64E010, 0);
            critter_fatigue_damage_set(qword_64E010, 0);
        }
        intgame_big_window_unlock();
        tig_window_hide(charedit_skills_win);
        tig_window_hide(charedit_spells_win);
        tig_window_hide(charedit_tech_win);
        tig_window_hide(dword_64CA60);
        qword_64E010 = OBJ_HANDLE_NULL;
        sub_551160();
        sub_551A80(0);
        sub_55EFB0();
        if (dword_64E02C) {
            combat_auto_attack_set(true);
            dword_64E02C = false;
        }
    }
}

// 0x55A220
bool charedit_is_created()
{
    return charedit_created;
}

// 0x55A230
void sub_55A230()
{
    if (charedit_created) {
        sub_55B150();
    }
}

// 0x55A240
void sub_55A240()
{
    TigFont font_desc;
    char* pch;
    char v1[7][40];
    const char* v2[7];
    int index;

    sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[0]), 0, -1, 3);
    sub_55B880(dword_64CA64, dword_64CDD0, &(stru_5C8150[3]), 0, -1, 1);

    if (obj_field_int32_get(qword_64E010, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[4]), 0, -1, 5);
    } else if (sub_45F730(qword_64E010)) {
        sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[7]), 0, -1, 2);
    } else {
        sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[4]), 0, -1, 2);
        sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[7]), 0, -1, 2);
    }

    sub_55B880(dword_64CA64, dword_64C9D0, stru_5C81E0, 0, -1, 13);

    sub_441B60(qword_64E010, qword_64E010, v1[3]);
    tig_font_push(dword_64CDD0);
    font_desc.str = v1[3];
    pch = &(v1[3][strlen(v1[3])]);
    do {
        *pch-- = '\0';
        font_desc.width = 0;
        sub_535390(&font_desc);
    } while (font_desc.width > 243);
    tig_font_pop();

    sprintf(v1[0],  ": %d  ", sub_55B4D0(qword_64E010, 0));
    sprintf(v1[1],  ": %d  ", sub_55B4D0(qword_64E010, 1));
    sprintf(v1[2],  ": %d  ", sub_55B4D0(qword_64E010, 2));
    sprintf(v1[4],  ": %s", race_get_name(stat_level(qword_64E010, STAT_RACE)));
    sprintf(v1[5],  ": %s", gender_get_name(stat_level(qword_64E010, STAT_GENDER)));
    sprintf(v1[6],  ": %d ", stat_level(qword_64E010, STAT_AGE));

    for (index = 0; index < 7; index++) {
        v2[index] = v1[index];
    }

    sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[0]), &(v2[0]), -1, 3);
    sub_55B880(dword_64CA64, dword_64CDD0, &(stru_5C8150[3]), &(v2[3]), -1, 1);

    if (obj_field_int32_get(qword_64E010, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[4]), &(v2[4]), -1, 3);
    } else if (!sub_45F730(qword_64E010)) {
        sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[4]), &(v2[4]), -1, 2);
    }
}

// 0x55A5C0
bool sub_55A5C0(TigMessage* msg)
{
    int index;
    int param;
    int value;
    int cost;
    int unspent_points;
    int stat;

    switch (msg->type) {
    case TIG_MESSAGE_BUTTON:
        switch (msg->data.button.state) {
        case TIG_BUTTON_STATE_MOUSE_INSIDE:
            if (msg->data.button.button_handle == dword_64C7B4) {
                dword_64C9CC = 132;
                return true;
            }

            if (msg->data.button.button_handle == dword_64C83C) {
                dword_64C9CC = 133;
                return true;
            }

            if (msg->data.button.button_handle == dword_64D3B8) {
                dword_64C9CC = 134;
                return true;
            }

            if (msg->data.button.button_handle == dword_64CA54) {
                dword_64C9CC = 140;
                return true;
            }

            for (index = 0; index < 32; index++) {
                if (msg->data.button.button_handle == stru_5C89A8[index].button_handle) {
                    dword_64C9CC = stru_5C89A8[index].field_10;
                    return true;
                }
            }

            for (index = 0; index < 10; index++) {
                if (msg->data.button.button_handle == stru_5C7F88[index].button_handle) {
                    dword_64C9CC = dword_5C80C8[index];
                    return true;
                }

                if (msg->data.button.button_handle == stru_5C8028[index].button_handle) {
                    dword_64C9CC = dword_5C80C8[index];
                    return true;
                }
            }

            return false;
        case TIG_BUTTON_STATE_MOUSE_OUTSIDE:
            if (msg->data.button.button_handle == dword_64C7B4
                || msg->data.button.button_handle == dword_64C83C
                || msg->data.button.button_handle == dword_64D3B8
                || msg->data.button.button_handle == dword_64CA54) {
                dword_64C9CC = -1;
                sub_550720();
                return true;
            }

            for (index = 0; index < 32; index++) {
                if (msg->data.button.button_handle == stru_5C89A8[index].button_handle) {
                    dword_64C9CC = -1;
                    sub_550720();
                    return true;
                }
            }

            for (index = 0; index < 10; index++) {
                if (msg->data.button.button_handle == stru_5C7F88[index].button_handle
                    || msg->data.button.button_handle == stru_5C8028[index].button_handle) {
                    dword_64C9CC = -1;
                    sub_550720();
                    return true;
                }
            }

            return false;
        case TIG_BUTTON_STATE_RELEASED:
            if (msg->data.button.button_handle == dword_64C824) {
                sub_55B0E0(0);
                return true;
            }

            if (msg->data.button.button_handle == dword_64CA5C) {
                sub_55B0E0(1);
                return true;
            }

            return false;
        case TIG_BUTTON_STATE_PRESSED:
            for (index = 0; index < 10; index++) {
                if (msg->data.button.button_handle == stru_5C7F88[index].button_handle) {
                    param = stru_5C7F88[index].art_num;
                    value = sub_55B4D0(qword_64E010, param) + 1;
                    if (param == CHAREDIT_STAT_HP_PTS || param == CHAREDIT_STAT_FATIGUE_PTS) {
                        cost = 1;
                    } else {
                        stat = sub_55B410(param);
                        if (stat_level(qword_64E010, stat) >= stat_get_max_value(qword_64E010, stat)) {
                            stru_5C8990.str = dword_64D3C4[0];
                            sub_550750(&stru_5C8990);
                            return true;
                        }
                        cost = sub_4B0F50(value);
                    }

                    unspent_points = stat_level(qword_64E010, STAT_UNSPENT_POINTS);
                    if (cost > unspent_points) {
                        stru_5C8990.str = dword_64D3C4[1];
                        sub_550750(&stru_5C8990);
                        return true;
                    }

                    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
                        && (tig_net_flags & TIG_NET_HOST) == 0
                        && !multiplayer_is_locked()) {
                        Packet127 pkt;

                        pkt.type = 127;
                        pkt.field_4 = 1;
                        pkt.field_8 = 0;
                        pkt.field_C = param;
                        tig_net_send_app_all(&pkt, sizeof(pkt));

                        return true;
                    }

                    sub_55B720(qword_64E010, param, value);

                    if (sub_55B4D0(qword_64E010, param) < value) {
                        stru_5C8990.str = dword_64D3C4[0];
                        sub_550750(&stru_5C8990);
                        return true;
                    }

                    stat_set_base(qword_64E010, STAT_UNSPENT_POINTS, unspent_points - cost);
                    sub_55B150();
                    sub_550720();
                    return true;
                }

                if (msg->data.button.button_handle == stru_5C8028[index].button_handle) {
                    param = stru_5C8028[index].art_num;
                    value = sub_55B4D0(qword_64E010, param);
                    if (value == dword_64D304[param]) {
                        stru_5C8990.str = dword_64D3C4[2];
                        sub_550750(&stru_5C8990);
                        return true;
                    }

                    if (param == CHAREDIT_STAT_HP_PTS || param == CHAREDIT_STAT_FATIGUE_PTS) {
                        cost = 1;
                    } else {
                        cost = sub_4B0F50(value);
                    }

                    value--;

                    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
                        && (tig_net_flags & TIG_NET_HOST) == 0
                        && !multiplayer_is_locked()) {
                        Packet127 pkt;

                        pkt.type = 127;
                        pkt.field_4 = 2;
                        pkt.field_8 = 0;
                        pkt.field_C = param;
                        tig_net_send_app_all(&pkt, sizeof(pkt));

                        return true;
                    }

                    sub_55B720(qword_64E010, param, value);

                    if (sub_55B4D0(qword_64E010, param) > value) {
                        if (value == 0) {
                            stru_5C8990.str = dword_64D3C4[3];
                            sub_550750(&stru_5C8990);
                            return true;
                        }

                        if (dword_5C8124[index] != -1
                            && !sub_4C6B50(qword_64E010, dword_5C8124[index], value - 1)) {
                            stru_5C8990.str = dword_64D3C4[11];
                            sub_550750(&stru_5C8990);
                            return true;
                        }

                        if (param == CHAREDIT_STAT_INTELLIGENCE_BASE
                            && !sub_4B1B90(qword_64E010, value - 1)) {
                            stru_5C8990.str = dword_64D3C4[12];
                            sub_550750(&stru_5C8990);
                            return true;
                        }

                        if (param == CHAREDIT_STAT_INTELLIGENCE_BASE
                            && !sub_4B02B0(qword_64E010, value - 1)) {
                            stru_5C8990.str = dword_64D3C4[14];
                            sub_550750(&stru_5C8990);
                            return true;
                        }

                        if (param == CHAREDIT_STAT_WILLPOWER_BASE
                            && !sub_4B1C00(qword_64E010, value - 1)) {
                            stru_5C8990.str = dword_64D3C4[22];
                            sub_550750(&stru_5C8990);
                            return true;
                        }

                        // Something else is wrong.
                        return true;
                    }

                    unspent_points = stat_level(qword_64E010, STAT_UNSPENT_POINTS);
                    stat_set_base(qword_64E010, STAT_UNSPENT_POINTS, unspent_points + cost);
                    sub_55B150();
                    sub_550720();
                    return true;
                }
            }

            if (msg->data.button.button_handle == dword_64C7B4) {
                if (dword_64E01C == 3) {
                    sub_55F0D0();
                }
                sub_51E850(charedit_skills_win);
                dword_64E01C = 0;
                sub_55B150();
                return true;
            }

            if (msg->data.button.button_handle == dword_64C83C) {
                if (dword_64E01C == 3) {
                    sub_55F0D0();
                }
                sub_51E850(charedit_tech_win);
                dword_64E01C = 1;
                sub_55B150();
                return true;
            }

            if (msg->data.button.button_handle == dword_64D3B8) {
                if (dword_64E01C == 3) {
                    sub_55F0D0();
                }
                sub_51E850(charedit_spells_win);
                dword_64E01C = 2;
                sub_55B150();
                return true;
            }

            if (msg->data.button.button_handle == dword_64CA54) {
                sub_51E850(dword_64CA60);
                if (dword_64E01C != 3) {
                    sub_55EFE0();
                }
                dword_64E01C = 3;
                sub_55B150();
                return true;
            }

            if (msg->data.button.button_handle == dword_64D428) {
                value = stat_get_base(qword_64E010, STAT_LEVEL) + 1;
                if (stat_set_base(qword_64E010, STAT_LEVEL, value) == value) {
                    unspent_points = stat_get_base(qword_64E010, STAT_UNSPENT_POINTS);
                    stat_set_base(qword_64E010, STAT_UNSPENT_POINTS, unspent_points + 1);
                    sub_55B150();
                }
                return true;
            }

            if (msg->data.button.button_handle == dword_64CFDC) {
                unspent_points = stat_get_base(qword_64E010, STAT_UNSPENT_POINTS);
                if (unspent_points >= 1) {
                    value = stat_get_base(qword_64E010, STAT_LEVEL) - 1;
                    if (stat_set_base(qword_64E010, STAT_LEVEL, value) == value) {
                        stat_set_base(qword_64E010, STAT_UNSPENT_POINTS, unspent_points - 1);
                    }
                    sub_55B150();
                }
                return true;
            }

            return false;
        }
        return false;
    case TIG_MESSAGE_MOUSE:
        switch (msg->data.mouse.event) {
        case TIG_MESSAGE_MOUSE_IDLE:
            sub_55AE70(dword_64C9CC);
            return false;
        case TIG_MESSAGE_MOUSE_LEFT_BUTTON_UP:
            if (dword_64CDCC != 0
                && dword_64CDCC != 3
                && sub_551000(msg->data.mouse.x, msg->data.mouse.y)) {
                charedit_destroy();
                return true;
            }
            return false;
        default:
            return false;
        }
    case TIG_MESSAGE_CHAR:
        if (toupper(msg->data.character.ch) == 'X'
            && (tig_net_flags & TIG_NET_CONNECTED) == 0
            && dword_64CDCC == 1
            && (!sub_4B6D70()
                || sub_4B6D80() == player_get_pc_obj())) {
            gamelib_save("ExportSave", "Export-Save");
            sub_4A6470(player_get_pc_obj());
            gamelib_load("ExportSave");
            sub_403790("ExportSave");
            return true;
        }
        return false;
    default:
        return false;
    }
}

// 0x55AE70
void sub_55AE70(int a1)
{
    UiMessage ui_message;
    MesFileEntry mes_file_entry;
    int value;

    if (a1 == -1) {
        return;
    }

    if (a1 >= 109 && a1 <= 116) {
        ui_message.type = UI_MSG_TYPE_STAT;
        ui_message.field_8 = dword_5C8124[a1 - 109];
        value = stat_get_base(qword_64E010, ui_message.field_8);
        if (value < stat_get_max_value(qword_64E010, ui_message.field_8)) {
            ui_message.field_C = sub_4B0F50(value + 1);
        } else {
            ui_message.field_C = 0;
        }
        mes_file_entry.num = a1;
        if (mes_search(charedit_mes_file, &mes_file_entry)) {
            ui_message.str = mes_file_entry.str;
            sub_550750(&ui_message);
        }
    } else if (a1 >= 1000 && a1 < 1999) {
        ui_message.type = UI_MSG_TYPE_SKILL;
        ui_message.field_8 = a1 - 1000;
        if (IS_TECH_SKILL(ui_message.field_8)) {
            ui_message.field_C = sub_4C6B20(sub_4C6580(qword_64E010, GET_TECH_SKILL(ui_message.field_8)) + 4);
        } else {
            ui_message.field_C = sub_4C64E0(sub_4C5E50(qword_64E010, GET_BASIC_SKILL(ui_message.field_8)) + 4);
        }
        sub_550750(&ui_message);
    } else if (a1 >= 2000 && a1 < 2999) {
        ui_message.type = UI_MSG_TYPE_SPELL;
        ui_message.field_8 = a1 - 2000;
        ui_message.field_C = sub_4B1650(a1 - 2000);
        ui_message.field_10 = qword_64E010;
        sub_550750(&ui_message);
    } else if (a1 >= 3000 && a1 < 3999) {
        ui_message.type = UI_MSG_TYPE_COLLEGE;
        ui_message.field_8 = a1 - 3000;
        sub_550750(&ui_message);
    } else if (a1 >= 4000 && a1 < 4999) {
        ui_message.type = UI_MSG_TYPE_TECH;
        ui_message.field_8 = a1 - 4000;
        sub_550750(&ui_message);
    } else if (a1 >= 5000 && a1 < 5999) {
        ui_message.type = UI_MSG_TYPE_DEGREE;
        ui_message.field_8 = a1 - 5000;
        ui_message.field_C = tech_get_cost_for_degree(ui_message.field_8 % DEGREE_COUNT);
        sub_550750(&ui_message);
    } else {
        ui_message.type = UI_MSG_TYPE_FEEDBACK;
        mes_file_entry.num = a1;
        if (mes_search(charedit_mes_file, &mes_file_entry)) {
            ui_message.str = mes_file_entry.str;
            sub_550750(&ui_message);
        }
    }
}

// 0x55B0E0
void sub_55B0E0(bool a1)
{
    int64_t v1;
    int v2;

    if (a1) {
        v1 = sub_45E590(qword_64E010);
    } else {
        v1 = sub_45E4F0(qword_64E010);
    }

    if (v1 != qword_64E010) {
        v2 = dword_64CDCC;
        charedit_destroy();
        charedit_create(v1, v2);
    }
}

// 0x55B150
void sub_55B150()
{
    sub_55B280();
    sub_55B1B0();
    switch (dword_64E01C) {
    case 0:
        charedit_refresh_skills_win();
        break;
    case 1:
        charedit_refresh_tech_win();
        break;
    case 2:
        charedit_refresh_spells_win();
        break;
    default:
        sub_55D210();
        break;
    }
    sub_55EC90();
    sub_55A240();
    sub_551160();
    sub_54AEE0(0);
    sub_54AEE0(1);
}

// 0x55B1B0
void sub_55B1B0()
{
    char v1[13][10];
    const char* v2[13];
    int index;

    for (index = 0; index < 13; index++) {
        v2[index] = v1[index];
    }

    for (index = 0; index < 8; index++) {
        sprintf(v1[index],
            ": %d  ",
            stat_level(qword_64E010, stru_5C81E0[index].value));
    }

    for (index = 8; index < 13; index++) {
        sprintf(v1[index],
            ": %d  ",
            sub_43D6D0(qword_64E010, stru_5C81E0[index].value, true));
    }

    sub_55B880(dword_64CA64, dword_64C9D0, stru_5C81E0, v2, -1, 13);
}

// 0x55B280
void sub_55B280()
{
    int stat;

    for (stat = 0; stat < CHAREDIT_STAT_COUNT; stat++) {
        sub_55B2A0(stat);
    }
}

// 0x55B2A0
void sub_55B2A0(int stat)
{
    tig_font_handle_t font;
    int value;
    int base_value;
    int effective_value;
    char str[160];

    switch (stat) {
    case CHAREDIT_STAT_UNSPENT_POINTS:
    case CHAREDIT_STAT_LEVEL:
    case CHAREDIT_STAT_XP_TO_NEXT_LEVEL:
    case CHAREDIT_STAT_STRENGTH_BASE:
    case CHAREDIT_STAT_CONSTITUTION_BASE:
    case CHAREDIT_STAT_DEXTERITY_BASE:
    case CHAREDIT_STAT_BEAUTY_BASE:
    case CHAREDIT_STAT_INTELLIGENCE_BASE:
    case CHAREDIT_STAT_WILLPOWER_BASE:
    case CHAREDIT_STAT_PERCEPTION_BASE:
    case CHAREDIT_STAT_CHARISMA_BASE:
        return;
    }

    font = dword_64D3A8;
    switch (stat) {
    case CHAREDIT_STAT_HP_PTS:
        value = object_hp_max(qword_64E010);
        break;
    case CHAREDIT_STAT_FATIGUE_PTS:
        value = critter_fatigue_max(qword_64E010);
        break;
    default:
        value = sub_55B4D0(qword_64E010, stat);
        base_value = sub_55B4D0(qword_64E010, stat - 1);
        effective_value = sub_4EA930(qword_64E010, sub_55B410(stat), base_value);
        if (effective_value > base_value) {
            font = dword_64C7A0;
        } else if (effective_value < base_value) {
            font = dword_64D3BC;
        }
    }

    sprintf(str, " %d ", value);
    stru_5C8F50.str = str;
    stru_5C8F50.x = stru_5C7E70[stat].x;
    stru_5C8F50.y = stru_5C7E70[stat].y;
    sub_55B880(dword_64CA64, font, &stru_5C8F50, NULL, -1, 1);
}

// 0x55B410
int sub_55B410(int stat)
{
    switch (stat) {
    case CHAREDIT_STAT_UNSPENT_POINTS:
        return STAT_UNSPENT_POINTS;
    case CHAREDIT_STAT_LEVEL:
        return STAT_LEVEL;
    case CHAREDIT_STAT_3:
    case CHAREDIT_STAT_HP_PTS:
        return -3;
    case CHAREDIT_STAT_5:
    case CHAREDIT_STAT_FATIGUE_PTS:
        return -2;
    case CHAREDIT_STAT_STRENGTH_BASE:
    case CHAREDIT_STAT_STRENGTH_CURRENT:
        return STAT_STRENGTH;
    case CHAREDIT_STAT_CONSTITUTION_BASE:
    case CHAREDIT_STAT_CONSTITUTION_CURRENT:
        return STAT_CONSTITUTION;
    case CHAREDIT_STAT_DEXTERITY_BASE:
    case CHAREDIT_STAT_DEXTERITY_CURRENT:
        return STAT_DEXTERITY;
    case CHAREDIT_STAT_BEAUTY_BASE:
    case CHAREDIT_STAT_BEAUTY_CURRENT:
        return STAT_BEAUTY;
    case CHAREDIT_STAT_INTELLIGENCE_BASE:
    case CHAREDIT_STAT_INTELLIGENCE_CURRENT:
        return STAT_INTELLIGENCE;
    case CHAREDIT_STAT_WILLPOWER_BASE:
    case CHAREDIT_STAT_WILLPOWER_CURRENT:
        return STAT_WILLPOWER;
    case CHAREDIT_STAT_PERCEPTION_BASE:
    case CHAREDIT_STAT_PERCEPTION_CURRENT:
        return STAT_PERCEPTION;
    case CHAREDIT_STAT_CHARISMA_BASE:
    case CHAREDIT_STAT_CHARISMA_CURRENT:
        return STAT_CHARISMA;
    default:
        // FIXME: Ideally should return something else to denote error.
        return 0;
    }
}

// 0x55B4D0
int sub_55B4D0(int64_t obj, int stat)
{
    switch (stat) {
    case CHAREDIT_STAT_UNSPENT_POINTS:
        return stat_get_base(obj, STAT_UNSPENT_POINTS);
    case CHAREDIT_STAT_LEVEL:
        return stat_level(obj, STAT_LEVEL);
    case CHAREDIT_STAT_XP_TO_NEXT_LEVEL:
        return level_get_experience_points_to_next_level(obj);
    case CHAREDIT_STAT_3:
        return object_hp_current(obj);
    case CHAREDIT_STAT_HP_PTS:
        return object_hp_pts_get(obj);
    case CHAREDIT_STAT_5:
        return critter_fatigue_current(obj);
    case CHAREDIT_STAT_FATIGUE_PTS:
        return critter_fatigue_pts_get(obj);
    case CHAREDIT_STAT_STRENGTH_BASE:
        return stat_get_base(obj, STAT_STRENGTH);
    case CHAREDIT_STAT_STRENGTH_CURRENT:
        return stat_level(obj, STAT_STRENGTH);
    case CHAREDIT_STAT_CONSTITUTION_BASE:
        return stat_get_base(obj, STAT_CONSTITUTION);
    case CHAREDIT_STAT_CONSTITUTION_CURRENT:
        return stat_level(obj, STAT_CONSTITUTION);
    case CHAREDIT_STAT_DEXTERITY_BASE:
        return stat_get_base(obj, STAT_DEXTERITY);
    case CHAREDIT_STAT_DEXTERITY_CURRENT:
        return stat_level(obj, STAT_DEXTERITY);
    case CHAREDIT_STAT_BEAUTY_BASE:
        return stat_get_base(obj, STAT_BEAUTY);
    case CHAREDIT_STAT_BEAUTY_CURRENT:
        return stat_level(obj, STAT_BEAUTY);
    case CHAREDIT_STAT_INTELLIGENCE_BASE:
        return stat_get_base(obj, STAT_INTELLIGENCE);
    case CHAREDIT_STAT_INTELLIGENCE_CURRENT:
        return stat_level(obj, STAT_INTELLIGENCE);
    case CHAREDIT_STAT_WILLPOWER_BASE:
        return stat_get_base(obj, STAT_WILLPOWER);
    case CHAREDIT_STAT_WILLPOWER_CURRENT:
        return stat_level(obj, STAT_WILLPOWER);
    case CHAREDIT_STAT_PERCEPTION_BASE:
        return stat_get_base(obj, STAT_PERCEPTION);
    case CHAREDIT_STAT_PERCEPTION_CURRENT:
        return stat_level(obj, STAT_PERCEPTION);
    case CHAREDIT_STAT_CHARISMA_BASE:
        return stat_get_base(obj, STAT_CHARISMA);
    case CHAREDIT_STAT_CHARISMA_CURRENT:
        return stat_level(obj, STAT_CHARISMA);
    default:
        return 0;
    }
}

// 0x55B720
void sub_55B720(int64_t obj, int stat, int value)
{
    switch (stat) {
    case CHAREDIT_STAT_HP_PTS:
        object_hp_pts_set(obj, value);
        break;
    case CHAREDIT_STAT_FATIGUE_PTS:
        critter_fatigue_pts_set(obj, value);
        break;
    case CHAREDIT_STAT_STRENGTH_BASE:
        stat_set_base(obj, STAT_STRENGTH, value);
        break;
    case CHAREDIT_STAT_CONSTITUTION_BASE:
        stat_set_base(obj, STAT_CONSTITUTION, value);
        break;
    case CHAREDIT_STAT_DEXTERITY_BASE:
        stat_set_base(obj, STAT_DEXTERITY, value);
        break;
    case CHAREDIT_STAT_BEAUTY_BASE:
        stat_set_base(obj, STAT_BEAUTY, value);
        break;
    case CHAREDIT_STAT_INTELLIGENCE_BASE:
        stat_set_base(obj, STAT_INTELLIGENCE, value);
        break;
    case CHAREDIT_STAT_WILLPOWER_BASE:
        stat_set_base(obj, STAT_WILLPOWER, value);
        break;
    case CHAREDIT_STAT_PERCEPTION_BASE:
        stat_set_base(obj, STAT_PERCEPTION, value);
        break;
    case CHAREDIT_STAT_CHARISMA_BASE:
        stat_set_base(obj, STAT_CHARISMA, value);
        break;
    }
}

// 0x55B880
void sub_55B880(tig_window_handle_t window_handle, tig_font_handle_t font, S5C8150* a3, const char** list, int a5, int a6)
{
    TigWindowData window_data;
    TigArtBlitInfo art_blit_info;
    TigRect rect;
    TigFont font_desc;
    int num;
    int index;
    int x;

    tig_window_data(window_handle, &window_data);

    if (window_handle == charedit_skills_win) {
        num = 29;
    } else if (window_handle == charedit_tech_win) {
        num = 30;
    } else if (window_handle == charedit_spells_win) {
        num = 31;
    } else if (window_handle == dword_64CA60) {
        num = 567;
    } else {
        num = 22;
    }

    tig_art_interface_id_create(num, 0, 0, 0, &(art_blit_info.art_id));
    art_blit_info.flags = 0;

    // TODO: The loop below does not look good, review. For now initialize some
    // vars to silence compiler warnings.
    rect.x = 0;
    font_desc.width = 0;
    font_desc.str = NULL;

    tig_font_push(font);
    for (index = 0; index < a6; index++) {
        if (a5 == -1) {
            font_desc.str = a3[index].str;
            font_desc.width = 0;
            sub_535390(&font_desc);

            x = a3[index].x;
            if (x < 0) {
                x = -x - font_desc.width / 2;
            }
            rect.x = x - window_data.rect.x;
            rect.width = font_desc.width;
            rect.height = font_desc.height;
        }
        rect.y = a3[index].y - window_data.rect.y;

        if (list != NULL) {
            font_desc.str = list[index];
            if (a5 == -1) {
                if (a3[index].str != NULL) {
                    rect.x += font_desc.width;
                }
            } else if (a5 < 0) {
                font_desc.width = 0;
                sub_535390(&font_desc);
                rect.x = -window_data.rect.x - font_desc.width / 2 - a5;
            } else {
                rect.x = a5 - window_data.rect.x;
            }

            font_desc.width = 0;
            sub_535390(&font_desc);

            if (a5 == -1 && a3[index].str == NULL) {
                x = a3[index].x;
                if (x < 0) {
                    x = -x - font_desc.width / 2;
                }
                rect.x = x - window_data.rect.x;
            }

            rect.width = font_desc.width;
            rect.height = font_desc.height;
        }

        art_blit_info.src_rect = &rect;
        art_blit_info.dst_rect = &rect;
        tig_window_blit_art(window_handle, &art_blit_info);
        tig_window_text_write(window_handle, font_desc.str, &rect);
    }
    tig_font_pop();
}

// 0x55BAB0
bool charedit_create_skills_win()
{
    tig_art_id_t art_id;
    TigWindowData window_data;
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    int index;
    tig_button_handle_t button_handles[CHAREDIT_SKILL_GROUP_COUNT];

    tig_art_interface_id_create(29, 0, 0, 0, &art_id);
    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect.x = 503;
    window_data.rect.y = 104;
    window_data.rect.width = art_frame_data.width;
    window_data.rect.height = art_frame_data.height;
    window_data.background_color = 0;
    window_data.message_filter = sub_55D3A0;
    if (tig_window_create(&window_data, &charedit_skills_win) != TIG_OK) {
        return false;
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01 | TIG_BUTTON_FLAG_HIDDEN;
    button_data.window_handle = charedit_skills_win;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    tig_art_interface_id_create(624, 0, 0, 0, &(button_data.art_id));
    for (index = 0; index < SKILL_COUNT; index++) {
        button_data.x = charedit_skills_plus_buttons[index].x;
        button_data.y = charedit_skills_plus_buttons[index].y;
        if (tig_button_create(&button_data, &(charedit_skills_plus_buttons[index].button_handle)) != TIG_OK) {
            tig_window_destroy(charedit_skills_win);
            return false;
        }
    }

    tig_art_interface_id_create(625, 0, 0, 0, &(button_data.art_id));
    for (index = 0; index < SKILL_COUNT; index++) {
        button_data.x = charedit_skills_minus_buttons[index].x;
        button_data.y = charedit_skills_minus_buttons[index].y;
        if (tig_button_create(&button_data, &(charedit_skills_minus_buttons[index].button_handle)) != TIG_OK) {
            tig_window_destroy(charedit_skills_win);
            return false;
        }
    }

    button_data.flags = TIG_BUTTON_FLAG_0x02 | TIG_BUTTON_FLAG_0x04;
    for (index = 0; index < CHAREDIT_SKILL_GROUP_COUNT; index++) {
        button_data.x = charedit_skill_group_buttons[index].x;
        button_data.y = charedit_skill_group_buttons[index].y;
        tig_art_interface_id_create(charedit_skill_group_buttons[index].art_num, 0, 0, 0, &(button_data.art_id));
        if (tig_button_create(&button_data, &(charedit_skill_group_buttons[index].button_handle)) != TIG_OK) {
            tig_window_destroy(charedit_skills_win);
            return false;
        }

        button_handles[index] = charedit_skill_group_buttons[index].button_handle;
    }

    tig_button_radio_group_create(CHAREDIT_SKILL_GROUP_COUNT, button_handles, dword_64E020);

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.art_id = TIG_ART_ID_INVALID;

    for (index = 0; index < CHAREDIT_SKILLS_PER_GROUP; index++) {
        button_data.x = charedit_skills_hover_areas[index].x - 503;
        button_data.y = charedit_skills_hover_areas[index].y - 63;
        button_data.width = charedit_skills_hover_areas[index].width;
        button_data.height = charedit_skills_hover_areas[index].height;
        tig_button_create(&button_data, &(charedit_skills_hover_areas[index].button_handle));
        // FIXME: No error checking as seen above.
    }

    return true;
}

// 0x55BD10
void sub_55BD10(int group)
{
    int index;
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect rect;
    int training;
    char v1[4][80];
    const char* v2[4];
    int trainings[4];

    for (index = 0; index < SKILL_COUNT; index++) {
        tig_button_hide(charedit_skills_plus_buttons[index].button_handle);
        tig_button_hide(charedit_skills_minus_buttons[index].button_handle);
    }

    dword_64E020 = group;

    if (dword_64CDCC != 2) {
        for (index = 0; index < 4; index++) {
            tig_button_show(charedit_skills_plus_buttons[4 * dword_64E020 + index].button_handle);
            tig_button_show(charedit_skills_minus_buttons[4 * dword_64E020 + index].button_handle);
        }
    }

    tig_art_interface_id_create(29, 0, 0, 0, &art_id);

    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    rect.x = 0;
    rect.y = 0;
    rect.width = art_frame_data.width;
    rect.height = art_frame_data.height;

    art_blit_info.art_id = art_id;
    art_blit_info.flags = 0;
    art_blit_info.src_rect = &rect;
    art_blit_info.dst_rect = &rect;
    tig_window_blit_art(charedit_skills_win, &art_blit_info);

    sub_55B880(charedit_skills_win, dword_64D3A8, &stru_5C82F0[4 * dword_64E020], 0, -1, 4);

    for (index = 0; index < 4; index++) {
        if (dword_64E020 < 3) {
            training = basic_skill_get_training(qword_64E010,
                stru_5C82F0[4 * dword_64E020 + index].value);
        } else {
            training = tech_skill_get_training(qword_64E010,
                stru_5C82F0[4 * dword_64E020 + index].value);
        }

        v2[index] = dword_64CA74[index];
        trainings[index] = training;
    }

    for (index = 0; index < 4; index++) {
        if (trainings[index] != 0) {
            sprintf(v1[index], " (%s)", v2[index]);
        } else {
            v1[index][0] = '\0';
        }

        v2[index] = v1[index];
    }

    sub_55B880(charedit_skills_win, dword_64D3A8, &stru_5C82F0[4 * dword_64E020], v2, -1, 4);
    charedit_refresh_skills_win();
}

// 0x55BF20
void charedit_refresh_skills_win()
{
    tig_art_id_t skills_win_art_id;
    TigArtFrameData skills_win_art_frame_data;
    tig_art_id_t gauge_art_id;
    TigArtFrameData gauge_art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    int y;
    int skill_level;
    int index;

    tig_art_interface_id_create(29, 0, 0, 0, &skills_win_art_id);
    if (tig_art_frame_data(skills_win_art_id, &skills_win_art_frame_data) != TIG_OK) {
        return;
    }

    art_blit_info.flags = 0;
    art_blit_info.dst_rect = &dst_rect;
    art_blit_info.src_rect = &src_rect;

    if (tig_art_interface_id_create(623, 0, 0, 0, &gauge_art_id) != TIG_OK) {
        return;
    }

    if (tig_art_frame_data(gauge_art_id, &gauge_art_frame_data) != TIG_OK) {
        return;
    }

    index = 0;
    for (y = 87; y < 351; y += 66) {
        if (dword_64E020 < 3) {
            skill_level = basic_skill_level(qword_64E010,
                stru_5C82F0[dword_64E020 * 4 + index].value);
            sub_4C64B0(qword_64E010,
                stru_5C82F0[dword_64E020 * 4 + index].value);
        } else {
            skill_level = tech_skill_level(qword_64E010,
                stru_5C82F0[dword_64E020 * 4 + index].value);
            sub_4C6AF0(qword_64E010,
                stru_5C82F0[dword_64E020 * 4 + index].value);
        }

        src_rect.x = 59;
        src_rect.y = y;
        src_rect.width = gauge_art_frame_data.width;
        src_rect.height = gauge_art_frame_data.height;

        dst_rect.x = 59;
        dst_rect.y = y;
        dst_rect.width = gauge_art_frame_data.width;
        dst_rect.height = gauge_art_frame_data.height;

        art_blit_info.art_id = skills_win_art_id;
        tig_window_blit_art(charedit_skills_win, &art_blit_info);

        if (skill_level != 0) {
            src_rect.x = 0;
            src_rect.y = 0;
            src_rect.width = skill_level * gauge_art_frame_data.width / 20;
            src_rect.height = gauge_art_frame_data.height;

            dst_rect.x = 59;
            dst_rect.y = y;
            dst_rect.width = src_rect.width;
            dst_rect.height = src_rect.height;

            art_blit_info.art_id = gauge_art_id;
            tig_window_blit_art(charedit_skills_win, &art_blit_info);
        }

        index++;
    }
}

// 0x55C110
bool charedit_create_tech_win()
{
    tig_art_id_t art_id;
    TigWindowData window_data;
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    tig_button_handle_t button_handles[TECH_COUNT];
    int index;

    tig_art_interface_id_create(30, 0, 0, 0, &art_id);
    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect.x = 503;
    window_data.rect.y = 104;
    window_data.rect.width = art_frame_data.width;
    window_data.rect.height = art_frame_data.height;
    window_data.background_color = 0;
    window_data.message_filter = charedit_tech_win_message_filter;
    if (tig_window_create(&window_data, &charedit_tech_win) != TIG_OK) {
        return false;
    }

    window_data.rect.x = 0;
    window_data.rect.y = 0;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &(window_data.rect);
    art_blit_info.dst_rect = &(window_data.rect);
    if (tig_window_blit_art(charedit_tech_win, &art_blit_info) != TIG_OK) {
        tig_window_destroy(charedit_tech_win);
        return false;
    }

    button_data.flags = TIG_BUTTON_FLAG_0x02 | TIG_BUTTON_FLAG_0x04;
    button_data.window_handle = charedit_tech_win;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    for (index = 0; index < TECH_COUNT; index++) {
        tig_art_interface_id_create(306 + index, 0, 0, 0, &(button_data.art_id));
        button_data.x = charedit_tech_buttons[index].x;
        button_data.y = charedit_tech_buttons[index].y;
        if (tig_button_create(&button_data, &(charedit_tech_buttons[index].button_handle)) != TIG_OK) {
            tig_window_destroy(charedit_tech_win);
            return false;
        }

        button_handles[index] = charedit_tech_buttons[index].button_handle;
    }

    tig_button_radio_group_create(TECH_COUNT, button_handles, charedit_selected_tech);

    charedit_inc_tech_degree_btn = TIG_BUTTON_HANDLE_INVALID;
    charedit_dec_tech_degree_btn = TIG_BUTTON_HANDLE_INVALID;

    return true;
}

// 0x55C2E0
void charedit_draw_tech_degree_icon(int index)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    tig_art_interface_id_create(charedit_tech_degree_icons[charedit_selected_tech], 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = charedit_tech_degree_icons_x[index];
    dst_rect.y = charedit_tech_degree_icons_y[index];
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(charedit_tech_win, &art_blit_info);
}

// 0x55C3A0
void charedit_refresh_tech_win()
{
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect rect;
    char str[7][80];
    char v1[80];
    char v2[80];
    int index;
    tig_art_id_t art_id;
    int degree;
    int next_degree;

    tig_art_interface_id_create(30, 0, 0, 0, &art_id);
    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    // TODO: What's the purpose of 25px offset?
    rect.x = 0;
    rect.y = 25;
    rect.width = art_frame_data.width;
    rect.height = art_frame_data.height - 25;

    art_blit_info.art_id = art_id;
    art_blit_info.flags = 0;
    art_blit_info.src_rect = &rect;
    art_blit_info.dst_rect = &rect;

    if (charedit_inc_tech_degree_btn != TIG_BUTTON_HANDLE_INVALID) {
        tig_button_destroy(charedit_inc_tech_degree_btn);
    }

    if (charedit_dec_tech_degree_btn != TIG_BUTTON_HANDLE_INVALID) {
        tig_button_destroy(charedit_dec_tech_degree_btn);
    }

    charedit_inc_tech_degree_btn = TIG_BUTTON_HANDLE_INVALID;
    charedit_dec_tech_degree_btn = TIG_BUTTON_HANDLE_INVALID;

    for (index = 0; index < 8; index++) {
        if (stru_5C8D08[index].button_handle != TIG_BUTTON_HANDLE_INVALID) {
            tig_button_destroy(stru_5C8D08[index].button_handle);
            stru_5C8D08[index].button_handle = TIG_BUTTON_HANDLE_INVALID;
        }
    }

    tig_window_blit_art(charedit_tech_win, &art_blit_info);

    for (index = 0; index < 3; index++) {
        stru_5C8FC8[index].str = str[0];
    }

    str[0][0] = (char)('1' + charedit_selected_tech);
    str[0][1] = '\0';
    sub_55B880(charedit_tech_win, dword_64D3A4, &(stru_5C8FC8[0]), 0, -1, 1);

    sprintf(str[0],
        "%s %s",
        tech_get_name(charedit_selected_tech),
        degree_get_name(tech_get_degree(qword_64E010, charedit_selected_tech)));
    sub_55B880(charedit_tech_win, dword_64CFE0, &(stru_5C8FC8[1]), 0, -1, 1);

    sprintf(str[0], "%d", sub_4B00B0(qword_64E010, charedit_selected_tech));
    sub_55B880(charedit_tech_win, dword_64CA68, &(stru_5C8FC8[2]), 0, -1, 1);

    degree = tech_get_degree(qword_64E010, charedit_selected_tech);
    next_degree = degree < 7 ? degree + 1 : degree;

    for (index = 0; index < degree; index++) {
        charedit_draw_tech_degree_icon(index);
    }

    for (index = 1; index - 1 < 7; index++) {
        stru_5C8850[index - 1].str = sub_56EA10(charedit_selected_tech, index);
        sub_56EA30(charedit_selected_tech, index, v1, v2);
        sprintf(str[index - 1], "[%s]+[%s]", v1, v2);
        stru_5C88C0[index - 1].str = str[index - 1];
    }

    if (next_degree > degree) {
        if (degree > 0) {
            sub_55B880(charedit_tech_win, dword_64CFE0, stru_5C8850, 0, -1, degree);
            sub_55B880(charedit_tech_win, dword_64C840, stru_5C88C0, 0, -1, degree);
        }

        sub_55B880(charedit_tech_win, dword_64CDC0, &(stru_5C8850[degree]), 0, -1, 1);
        sub_55B880(charedit_tech_win, dword_64CDB8, &(stru_5C88C0[degree]), 0, -1, 1);

        if (degree + 1 < 7) {
            sub_55B880(charedit_tech_win, dword_64D3B0, &stru_5C8850[degree + 1], 0, -1, 6 - degree);
            sub_55B880(charedit_tech_win, dword_64D42C, &stru_5C88C0[degree + 1], 0, -1, 6 - degree);
        }
    } else {
        sub_55B880(charedit_tech_win, dword_64CFE0, stru_5C8850, 0, -1, 7);
        sub_55B880(charedit_tech_win, dword_64C840, stru_5C88C0, 0, -1, 7);
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = charedit_tech_win;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    if (next_degree > degree
        && dword_64CDCC != 2) {
        tig_art_interface_id_create(647, 0, 0, 0, &(button_data.art_id));
        button_data.x = charedit_tech_degree_icons_x[next_degree - 1];
        button_data.y = charedit_tech_degree_icons_y[next_degree - 1];
        tig_button_create(&button_data, &charedit_inc_tech_degree_btn);
    }

    if (degree > 0
        && dword_64CDCC != 2
        && (dword_64CDCC == 0
            || dword_64CDCC == 3
            || dword_64DEEC[charedit_selected_tech] < degree)) {
        tig_art_interface_id_create(648, 0, 0, 0, &(button_data.art_id));
        button_data.x = charedit_tech_degree_icons_x[degree - 1];
        button_data.y = charedit_tech_degree_icons_y[degree - 1];
        tig_button_create(&button_data, &charedit_dec_tech_degree_btn);
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.art_id = TIG_ART_ID_INVALID;

    for (index = 0; index < 8; index++) {
        button_data.x = stru_5C8D08[index].x - 503;
        button_data.y = stru_5C8D08[index].y - 63;
        button_data.width = stru_5C8D08[index].width;
        button_data.height = stru_5C8D08[index].height;
        tig_button_create(&button_data, &(stru_5C8D08[index].button_handle));
    }
}

// 0x55C890
bool charedit_create_spells_win()
{
    tig_art_id_t art_id;
    TigWindowData window_data;
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    int index;
    int art_num;
    tig_button_handle_t button_handles[COLLEGE_COUNT];

    tig_art_interface_id_create(31, 0, 0, 0, &art_id);
    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect.x = 503;
    window_data.rect.y = 104;
    window_data.rect.width = art_frame_data.width;
    window_data.rect.height = art_frame_data.height;
    window_data.background_color = 0;
    window_data.message_filter = sub_55DC60;
    if (tig_window_create(&window_data, &charedit_spells_win) != TIG_OK) {
        return false;
    }

    window_data.rect.x = 0;
    window_data.rect.y = 0;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &(window_data.rect);
    art_blit_info.dst_rect = &(window_data.rect);
    if (tig_window_blit_art(charedit_spells_win, &art_blit_info) != TIG_OK) {
        tig_window_destroy(charedit_spells_win);
        return false;
    }

    button_data.flags = TIG_BUTTON_FLAG_0x02 | TIG_BUTTON_FLAG_0x04;
    button_data.window_handle = charedit_spells_win;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    for (index = 0; index < COLLEGE_COUNT; index++) {
        art_num = college_get_art_num(index);
        if (art_num != -1) {
            tig_art_interface_id_create(art_num, 0, 0, 0, &(button_data.art_id));
            button_data.x = charedit_college_buttons[index].x - 503;
            button_data.y = charedit_college_buttons[index].y - 104;
            if (tig_button_create(&button_data, &(charedit_college_buttons[index].button_handle)) != TIG_OK) {
                tig_window_destroy(charedit_spells_win);
                return false;
            }

            button_handles[index] = charedit_college_buttons[index].button_handle;
        }
    }

    tig_button_radio_group_create(COLLEGE_COUNT, button_handles, dword_64E024);

    spell_plus_bid = TIG_BUTTON_HANDLE_INVALID;
    spell_minus_bid = TIG_BUTTON_HANDLE_INVALID;

    return true;
}

// 0x55CA70
void sub_55CA70(int a1, int a2)
{
    int art_num;
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtAnimData art_anim_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;
    TigPaletteModifyInfo palette_modify_info;
    TigPalette tmp_palette;

    art_num = spell_get_icon(a1 + 5 * dword_64E024);
    if (art_num != -1) {
        tig_art_interface_id_create(art_num, 0, 0, 0, &art_id);
        tig_art_frame_data(art_id, &art_frame_data);

        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = art_frame_data.width;
        src_rect.height = art_frame_data.height;

        art_blit_info.art_id = art_id;
        art_blit_info.src_rect = &src_rect;

        if (a1 < a2) {
            art_blit_info.flags = 0;
            tmp_palette = NULL;
        } else {
            tig_art_anim_data(art_id, &art_anim_data);

            tmp_palette = tig_palette_create();
            palette_modify_info.flags = TIG_PALETTE_MODIFY_GRAYSCALE;
            palette_modify_info.src_palette = art_anim_data.palette1;
            palette_modify_info.dst_palette = tmp_palette;
            tig_palette_modify(&palette_modify_info);

            art_blit_info.flags = TIG_ART_BLT_PALETTE_OVERRIDE;
            art_blit_info.palette = tmp_palette;
        }

        dst_rect.x = dword_5C8FF8[a1] - 503;
        dst_rect.y = dword_5C900C[a1] - 104;
        dst_rect.width = art_frame_data.width;
        dst_rect.height = art_frame_data.height;

        art_blit_info.dst_rect = &dst_rect;
        tig_window_blit_art(charedit_spells_win, &art_blit_info);

        if (tmp_palette != NULL) {
            tig_palette_destroy(tmp_palette);
        }
    }
}

// 0x55CBC0
void charedit_refresh_spells_win()
{
    tig_art_id_t art_id;
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect rect;
    int index;
    int cnt;
    int v1;
    int spells[5];
    char spell_minimum_levels[5][80];
    int rc;

    tig_art_interface_id_create(31, 0, 0, 0, &art_id);
    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    // TODO: What's the purpose of 25px offset?
    rect.x = 0;
    rect.y = 25;
    rect.width = art_frame_data.width;
    rect.height = art_frame_data.height - 25;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &rect;
    art_blit_info.dst_rect = &rect;

    if (spell_plus_bid != TIG_BUTTON_HANDLE_INVALID) {
        tig_button_destroy(spell_plus_bid);
    }

    if (spell_minus_bid != TIG_BUTTON_HANDLE_INVALID) {
        tig_button_destroy(spell_minus_bid);
    }

    spell_plus_bid = TIG_BUTTON_HANDLE_INVALID;
    spell_minus_bid = TIG_BUTTON_HANDLE_INVALID;

    for (index = 0; index < 5; index++) {
        if (stru_5C8DC8[index].button_handle != TIG_BUTTON_HANDLE_INVALID) {
            tig_button_destroy(stru_5C8DC8[index].button_handle);
        }
    }

    tig_window_blit_art(charedit_spells_win, &art_blit_info);

    cnt = spell_college_level_get(qword_64E010, dword_64E024);

    // TODO: Refactor v1.
    v1 = cnt;
    if (v1 < 5) {
        v1++;
    }

    for (index = 0; index < 5; index++) {
        sub_55CA70(index, cnt);
    }

    for (index = 0; index < 5; index++) {
        spells[index] = index + 5 * dword_64E024;
        charedit_spell_title_labels[index].str = spell_get_name(spells[index]);
    }

    if (v1 > cnt) {
        if (cnt > 0) {
            sub_55B880(charedit_spells_win, dword_64D3A8, &(charedit_spell_title_labels[0]), NULL, -1, cnt);
        }

        sub_55B880(charedit_spells_win,
            dword_64CDCC == 2 ? dword_64C828 : dword_64CDD0,
            &charedit_spell_title_labels[cnt], NULL, -1, 1);

        if (cnt + 1 < 5) {
            sub_55B880(charedit_spells_win, dword_64C828, &(charedit_spell_title_labels[cnt + 1]), NULL, -1, 4 - cnt);
        }
    } else {
        sub_55B880(charedit_spells_win, dword_64D3A8, &(charedit_spell_title_labels[0]), 0, -1, 5);
    }

    for (index = 0; index < 5; index++) {
        sprintf(spell_minimum_levels[index],
            "%s: %d",
            charedit_minimum_level_str,
            spell_min_level(spells[index]));
        charedit_spell_minimum_level_labels[index].str = spell_minimum_levels[index];
    }

    if (v1 > cnt) {
        if (cnt > 0) {
            sub_55B880(charedit_spells_win, dword_64DF0C, &(charedit_spell_minimum_level_labels[0]), 0, -1, cnt);
        }

        sub_55B880(charedit_spells_win,
            dword_64CDCC == 2 ? dword_64CDB0 : dword_64D420,
            &(charedit_spell_minimum_level_labels[cnt]), NULL, -1, 1);

        if (cnt + 1 < 5) {
            sub_55B880(charedit_spells_win, dword_64CDB0, &(charedit_spell_minimum_level_labels[cnt + 1]), NULL, -1, 4 - cnt);
        }
    } else {
        sub_55B880(charedit_spells_win, dword_64DF0C, &(charedit_spell_minimum_level_labels[0]), 0, -1, 5);
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = charedit_spells_win;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    if (v1 > cnt) {
        if (dword_64CDCC != 2) {
            tig_art_interface_id_create(647, 0, 0, 0, &(button_data.art_id));
            button_data.x = dword_5C8FF8[v1 - 1] - 503;
            button_data.y = dword_5C900C[v1 - 1] - 104;

            rc = tig_button_create(&button_data, &spell_plus_bid);
            if (rc != TIG_OK) {
                tig_debug_printf("spells_print_all: ERROR: failed to create spell_plus_bid: %d!\n", rc);
            }
        }
    }

    if (cnt > 0
        && dword_64CDCC != 2
        && (dword_64CDCC == 0
            || dword_64CDCC == 3
            || dword_64D364[dword_64E024] < cnt)) {
        tig_art_interface_id_create(648, 0, 0, 0, &(button_data.art_id));
        button_data.x = dword_5C8FF8[cnt - 1] - 503;
        button_data.y = dword_5C900C[cnt - 1] - 104;

        rc = tig_button_create(&button_data, &spell_minus_bid);
        if (rc != TIG_OK) {
            tig_debug_printf("spells_print_all: ERROR: failed to create spell_minus_bid: %d!\n", rc);
        }
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = charedit_spells_win;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.art_id = TIG_ART_ID_INVALID;

    for (index = 0; index < 5; index++) {
        button_data.x = stru_5C8DC8[index].x - 503;
        button_data.y = stru_5C8DC8[index].y - 63;
        button_data.width = stru_5C8DC8[index].width;
        button_data.height = stru_5C8DC8[index].height;

        rc = tig_button_create(&button_data, &(stru_5C8DC8[index].button_handle));
        if (rc != TIG_OK) {
            tig_debug_printf("spells_print_all: ERROR: failed to create Hover Help Spell Start: %d!\n", rc);
        }
    }
}

// 0x55D060
bool sub_55D060()
{
    tig_art_id_t art_id;
    TigWindowData window_data;
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    int index;

    tig_art_interface_id_create(567, 0, 0, 0, &art_id);
    if (tig_art_frame_data(art_id, &art_frame_data) != TIG_OK) {
        return false;
    }

    window_data.flags = TIG_WINDOW_FLAG_0x02;
    window_data.rect.x = 503;
    window_data.rect.y = 104;
    window_data.rect.width = art_frame_data.width;
    window_data.rect.height = art_frame_data.height;
    window_data.background_color = 0;
    window_data.message_filter = sub_55DF90;
    if (tig_window_create(&window_data, &dword_64CA60) != TIG_OK) {
        return false;
    }

    window_data.rect.x = 0;
    window_data.rect.y = 0;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &(window_data.rect);
    art_blit_info.dst_rect = &(window_data.rect);
    if (tig_window_blit_art(dword_64CA60, &art_blit_info) != TIG_OK) {
        tig_window_destroy(dword_64CA60);
        return false;
    }

    dword_64CDC8 = 0;
    for (index = 0; index < 200; index++) {
        dword_64CFE4[dword_64CDC8] = index;
        dword_64CA90[dword_64CDC8] = level_advancement_scheme_get_name(index);
        if (dword_64CA90[dword_64CDC8] != NULL) {
            dword_64CDC8++;
        }
    }

    dword_64D424 = 0;

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.window_handle = dword_64CA60;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;
    button_data.art_id = TIG_ART_ID_INVALID;

    for (index = 0; index < 15; index++) {
        button_data.x = stru_5C8E50[index].x - 503;
        button_data.y = stru_5C8E50[index].y - 104;
        button_data.width = 160;
        button_data.height = 16;
        tig_button_create(&button_data, &(dword_64C7E8[index]));
    }

    return true;
}

// 0x55D210
void sub_55D210()
{
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect rect;
    int cnt;
    int index;

    tig_art_interface_id_create(567, 0, 0, 0, &(art_blit_info.art_id));
    if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    rect.x = 0;
    rect.y = 0;
    rect.width = art_frame_data.width;
    rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &rect;
    art_blit_info.dst_rect = &rect;
    tig_window_blit_art(dword_64CA60, &art_blit_info);

    if (obj_field_int32_get(qword_64E010, OBJ_F_TYPE) == OBJ_TYPE_PC
        && (obj_field_int32_get(qword_64E010, OBJ_F_PC_FLAGS) & OPCF_USE_ALT_DATA) != 0) {
        if (!sub_4A49E0(qword_64E010, byte_64D714)) {
            byte_64D714[0] = '\0';
        }
        stru_5C8E40.str = byte_64D714;
    } else {
        stru_5C8E40.str = level_advancement_scheme_get_name(level_auto_level_scheme_get(qword_64E010));
    }

    cnt = dword_64CDC8;
    if (cnt >= 15) {
        cnt = 15;
    }

    for (index = 0; index < cnt; index++) {
        stru_5C8E50[index].str = dword_64CA90[dword_64D424 + index];
    }

    sub_55B880(dword_64CA60, dword_64CA68, &stru_5C8E40, NULL, -1, 1);
    sub_55B880(dword_64CA60, dword_64C9D0, &(stru_5C8E50[0]), NULL, -1, cnt);

    sub_5806F0(stru_64C7A8);
}

// 0x55D3A0
bool sub_55D3A0(TigMessage* msg)
{
    int index;

    switch (msg->type) {
    case TIG_MESSAGE_MOUSE:
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_IDLE) {
            sub_55AE70(dword_64CA58);
        }
        break;
    case TIG_MESSAGE_BUTTON:
        switch (msg->data.button.state) {
        case TIG_BUTTON_STATE_MOUSE_INSIDE:
            for (index = 0; index < CHAREDIT_SKILL_GROUP_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skill_group_buttons[index].button_handle) {
                    dword_64CA58 = index + 135;
                    return true;
                }
            }

            for (index = 0; index < CHAREDIT_SKILLS_PER_GROUP; index++) {
                if (msg->data.button.button_handle == charedit_skills_hover_areas[index].button_handle) {
                    dword_64CA58 = 4 * dword_64E020 + 1000 + index;
                    return true;
                }
            }

            for (index = 0; index < BASIC_SKILL_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skills_plus_buttons[index].button_handle) {
                    dword_64CA58 = charedit_skills_plus_buttons[index].art_num + 1000;
                    return true;
                }

                if (msg->data.button.button_handle == charedit_skills_minus_buttons[index].button_handle) {
                    dword_64CA58 = charedit_skills_minus_buttons[index].art_num + 1000;
                    return true;
                }
            }
            break;
        case TIG_BUTTON_STATE_MOUSE_OUTSIDE:
            for (index = 0; index < CHAREDIT_SKILL_GROUP_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skill_group_buttons[index].button_handle) {
                    dword_64CA58 = -1;
                    sub_550720();
                    return true;
                }
            }

            for (index = 0; index < CHAREDIT_SKILLS_PER_GROUP; index++) {
                if (msg->data.button.button_handle == charedit_skills_hover_areas[index].button_handle) {
                    dword_64CA58 = -1;
                    sub_550720();
                    return true;
                }
            }

            for (index = 0; index < BASIC_SKILL_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skills_plus_buttons[index].button_handle) {
                    dword_64CA58 = -1;
                    sub_550720();
                    return true;
                }

                if (msg->data.button.button_handle == charedit_skills_minus_buttons[index].button_handle) {
                    dword_64CA58 = -1;
                    sub_550720();
                    return true;
                }
            }
            break;
        case TIG_BUTTON_STATE_PRESSED:
            for (index = 0; index < BASIC_SKILL_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skills_plus_buttons[index].button_handle) {
                    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
                        && (tig_net_flags & TIG_NET_HOST) == 0
                        && !multiplayer_is_locked()) {
                        Packet127 pkt;

                        pkt.type = 127;
                        pkt.field_4 = 1;
                        pkt.field_8 = 1;
                        pkt.field_C = charedit_skills_plus_buttons[index].art_num;
                        tig_net_send_app_all(&pkt, sizeof(pkt));

                        return true;
                    }

                    skill_ui_inc_skill(qword_64E010, charedit_skills_plus_buttons[index].art_num);
                    return true;
                }

                if (msg->data.button.button_handle == charedit_skills_minus_buttons[index].button_handle) {
                    // FIX: Original code has mess with plus/minus buttons, but
                    // it does not affect outcome as both plus/minus buttons
                    // refer to the same skills.
                    if ((tig_net_flags & TIG_NET_CONNECTED) != 0
                        && (tig_net_flags & TIG_NET_HOST) == 0
                        && !multiplayer_is_locked()) {
                        Packet127 pkt;

                        pkt.type = 127;
                        pkt.field_4 = 2;
                        pkt.field_8 = 1;
                        pkt.field_C = charedit_skills_minus_buttons[index].art_num;
                        tig_net_send_app_all(&pkt, sizeof(pkt));

                        return true;
                    }

                    if (basic_skill_get_base(qword_64E010, charedit_skills_minus_buttons[index].art_num) == dword_64C7B8[charedit_skills_minus_buttons[index].art_num]) {
                        stru_5C8990.str = dword_64D3C4[6];
                        sub_550750(&stru_5C8990);
                    } else {
                        skill_ui_dec_skill(qword_64E010, charedit_skills_minus_buttons[index].art_num);
                    }

                    return true;
                }
            }

            for (index = 0; index < CHAREDIT_SKILL_GROUP_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skill_group_buttons[index].button_handle) {
                    sub_55BD10(index);
                    return true;
                }
            }
            break;
        }
        break;
    }

    return sub_55D6F0(msg);
}

// 0x55D6F0
bool sub_55D6F0(TigMessage* msg)
{
    int index;
    Packet127 pkt;

    if (msg->type == TIG_MESSAGE_BUTTON) {
        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_INSIDE) {
            for (index = 0; index < TECH_SKILL_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skills_plus_buttons[BASIC_SKILL_COUNT + index].button_handle) {
                    dword_64CA58 = charedit_skills_plus_buttons[BASIC_SKILL_COUNT + index].art_num + 1012;
                    return true;
                }

                if (msg->data.button.button_handle == charedit_skills_minus_buttons[BASIC_SKILL_COUNT + index].button_handle) {
                    dword_64CA58 = charedit_skills_minus_buttons[BASIC_SKILL_COUNT + index].art_num + 1012;
                    return true;
                }
            }

            return false;
        }

        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_OUTSIDE) {
            // NOTE: Original code is buggy.
            for (index = 0; index < TECH_SKILL_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skills_plus_buttons[BASIC_SKILL_COUNT + index].button_handle) {
                    dword_64CA58 = -1;
                    sub_550720();
                    return true;
                }

                if (msg->data.button.button_handle == charedit_skills_minus_buttons[BASIC_SKILL_COUNT + index].button_handle) {
                    dword_64CA58 = -1;
                    sub_550720();
                    return true;
                }
            }

            return false;
        }

        if (msg->data.button.state == TIG_BUTTON_STATE_PRESSED) {
            for (index = 0; index < TECH_SKILL_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_skills_plus_buttons[BASIC_SKILL_COUNT + index].button_handle) {
                    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                        || (tig_net_flags & TIG_NET_HOST) != 0
                        || multiplayer_is_locked()) {
                        skill_ui_inc_skill(qword_64E010, charedit_skills_plus_buttons[BASIC_SKILL_COUNT + index].art_num + 12);
                    } else {
                        pkt.type = 127;
                        pkt.field_4 = 1;
                        pkt.field_8 = 2;
                        pkt.field_C = charedit_skills_plus_buttons[BASIC_SKILL_COUNT + index].art_num + 12;
                        tig_net_send_app_all(&pkt, sizeof(pkt));
                    }

                    return true;
                }

                if (msg->data.button.button_handle == charedit_skills_minus_buttons[BASIC_SKILL_COUNT + index].button_handle) {
                    if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                        || (tig_net_flags & TIG_NET_HOST) != 0
                        || multiplayer_is_locked()) {
                        if (tech_skill_get_base(qword_64E010, charedit_skills_minus_buttons[BASIC_SKILL_COUNT + index].art_num) == dword_64C82C[charedit_skills_minus_buttons[BASIC_SKILL_COUNT + index].art_num]) {
                            stru_5C8990.str = dword_64D3C4[6];
                            sub_550750(&stru_5C8990);
                        } else {
                            skill_ui_dec_skill(qword_64E010, charedit_skills_minus_buttons[BASIC_SKILL_COUNT + index].art_num + 12);
                        }
                    } else {
                        pkt.type = 127;
                        pkt.field_4 = 2;
                        pkt.field_8 = 2;
                        pkt.field_C = charedit_skills_minus_buttons[BASIC_SKILL_COUNT + index].art_num + 12;
                        tig_net_send_app_all(&pkt, sizeof(pkt));
                    }

                    return true;
                }
            }

            return false;
        }

        return false;
    }

    return false;
}

// 0x55D940
bool charedit_tech_win_message_filter(TigMessage* msg)
{
    int index;
    Packet127 pkt;
    int v1;

    if (msg->type == TIG_MESSAGE_MOUSE) {
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_IDLE) {
            sub_55AE70(dword_64D360);
        }

        return false;
    }

    if (msg->type == TIG_MESSAGE_BUTTON) {
        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_INSIDE) {
            for (index = 0; index < TECH_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_tech_buttons[index].button_handle) {
                    dword_64CDB4 = 4000 + index;
                    return true;
                }
            }

            for (index = 0; index < 8; index++) {
                if (msg->data.button.button_handle == stru_5C8D08[index].button_handle) {
                    if (index == 0) {
                        dword_64CDB4 = 139;
                    } else {
                        dword_64CDB4 = 5000 + 8 * charedit_selected_tech + index;
                    }
                    return true;
                }
            }

            if (msg->data.button.button_handle == charedit_inc_tech_degree_btn) {
                v1 = tech_get_degree(qword_64E010, charedit_selected_tech);
                dword_64CDB4 = 5001 + 8 * charedit_selected_tech + v1;
                return true;
            }

            if (msg->data.button.button_handle == charedit_dec_tech_degree_btn) {
                v1 = tech_get_degree(qword_64E010, charedit_selected_tech);
                dword_64CDB4 = 5000 + 8 * charedit_selected_tech + v1;
                return true;
            }

            return false;
        }

        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_OUTSIDE) {
            for (index = 0; index < TECH_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_tech_buttons[index].button_handle) {
                    dword_64CDB4 = -1;
                    sub_550720();
                    return true;
                }
            }

            for (index = 0; index < 8; index++) {
                if (msg->data.button.button_handle == stru_5C8D08[index].button_handle) {
                    dword_64CDB4 = -1;
                    sub_550720();
                    return true;
                }
            }

            if (msg->data.button.button_handle == charedit_inc_tech_degree_btn) {
                dword_64CDB4 = -1;
                sub_550720();
                return true;
            }

            if (msg->data.button.button_handle == charedit_dec_tech_degree_btn) {
                dword_64CDB4 = -1;
                sub_550720();
                return true;
            }

            return false;
        }

        if (msg->data.button.state == TIG_BUTTON_STATE_PRESSED) {
            for (index = 0; index < TECH_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_tech_buttons[index].button_handle) {
                    if (charedit_selected_tech != index) {
                        charedit_selected_tech = index;
                        charedit_refresh_tech_win();
                    }

                    return true;
                }
            }

            if (msg->data.button.button_handle == charedit_inc_tech_degree_btn) {
                if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                    || (tig_net_flags & TIG_NET_HOST) != 0
                    || multiplayer_is_locked()) {
                    tech_ui_inc_degree(qword_64E010, charedit_selected_tech);
                } else {
                    pkt.type = 127;
                    pkt.field_4 = 1;
                    pkt.field_8 = 3;
                    pkt.field_C = charedit_selected_tech;
                    tig_net_send_app_all(&pkt, sizeof(pkt));
                }

                return true;
            }

            if (msg->data.button.button_handle == charedit_dec_tech_degree_btn) {
                if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                    || (tig_net_flags & TIG_NET_HOST) != 0
                    || multiplayer_is_locked()) {
                    tech_ui_dec_degree(qword_64E010, charedit_selected_tech);
                } else {
                    pkt.type = 127;
                    pkt.field_4 = 2;
                    pkt.field_8 = 3;
                    pkt.field_C = charedit_selected_tech;
                    tig_net_send_app_all(&pkt, sizeof(pkt));
                }
            }
        }

        return false;
    }

    return false;
}

// 0x55DC60
bool sub_55DC60(TigMessage* msg)
{
    int index;
    Packet127 pkt;
    int v1;

    if (msg->type == TIG_MESSAGE_MOUSE) {
        if (msg->data.mouse.event == TIG_MESSAGE_MOUSE_IDLE) {
            sub_55AE70(dword_64D360);
        }

        return false;
    }

    if (msg->type == TIG_MESSAGE_BUTTON) {
        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_INSIDE) {
            for (index = 0; index < COLLEGE_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_college_buttons[index].button_handle) {
                    dword_64D360 = 3000 + index;
                    return true;
                }
            }

            for (index = 0; index < 5; index++) {
                if (msg->data.button.button_handle == stru_5C8DC8[index].button_handle) {
                    dword_64D360 = 2000 + 5 * dword_64E024 + index;
                    return true;
                }
            }

            if (msg->data.button.button_handle == spell_plus_bid) {
                dword_64D360 = spell_college_level_get(qword_64E010, dword_64E024) + 5 * (dword_64E024 + 400);
                return true;
            }

            if (msg->data.button.button_handle == spell_minus_bid) {
                dword_64D360 = spell_college_level_get(qword_64E010, dword_64E024) - 1 + 5 * (dword_64E024 + 400);
                return true;
            }

            return false;
        }

        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_OUTSIDE) {
            for (index = 0; index < COLLEGE_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_college_buttons[index].button_handle) {
                    dword_64D360 = -1;
                    sub_550720();
                    return true;
                }
            }

            for (index = 0; index < 5; index++) {
                if (msg->data.button.button_handle == stru_5C8DC8[index].button_handle) {
                    dword_64D360 = -1;
                    sub_550720();
                    return true;
                }
            }

            if (msg->data.button.button_handle == spell_plus_bid) {
                dword_64D360 = -1;
                sub_550720();
                return true;
            }

            if (msg->data.button.button_handle == spell_minus_bid) {
                dword_64D360 = -1;
                sub_550720();
                return true;
            }

            return false;
        }

        if (msg->data.button.state == TIG_BUTTON_STATE_PRESSED) {
            for (index = 0; index < COLLEGE_COUNT; index++) {
                if (msg->data.button.button_handle == charedit_college_buttons[index].button_handle) {
                    if (dword_64E024 != index) {
                        dword_64E024 = index;
                        charedit_refresh_spells_win();
                    }

                    return true;
                }
            }

            if (msg->data.button.button_handle == spell_plus_bid) {
                if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                    || (tig_net_flags & TIG_NET_HOST) != 0
                    || multiplayer_is_locked()) {
                    v1 = spell_college_level_get(qword_64E010, dword_64E024);
                    sub_57C540(qword_64E010, 5 * dword_64E024 + v1);
                } else {
                    pkt.type = 127;
                    pkt.field_4 = 1;
                    pkt.field_8 = 4;
                    pkt.field_C = dword_64E024;
                    tig_net_send_app_all(&pkt, sizeof(pkt));
                }

                return true;
            }

            if (msg->data.button.button_handle == spell_minus_bid) {
                if ((tig_net_flags & TIG_NET_CONNECTED) == 0
                    || (tig_net_flags & TIG_NET_HOST) != 0
                    || multiplayer_is_locked()) {
                    v1 = spell_college_level_get(qword_64E010, dword_64E024);
                    if (v1 == dword_64D364[dword_64E024]) {
                        stru_5C8990.str = dword_64D3C4[10];
                        sub_550750(&stru_5C8990);
                    } else {
                        spell_ui_remove(qword_64E010, 5 * dword_64E024 + v1 - 1);
                    }
                } else {
                    pkt.type = 127;
                    pkt.field_4 = 2;
                    pkt.field_8 = 4;
                    pkt.field_C = dword_64E024;
                    tig_net_send_app_all(&pkt, sizeof(pkt));
                }

                return true;
            }

            return false;
        }

        return false;
    }

    return false;
}

// 0x55DF90
bool sub_55DF90(TigMessage* msg)
{
    int index;

    if (msg->type == TIG_MESSAGE_BUTTON) {
        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_INSIDE) {
            for (index = 0; index < 15; index++) {
                if (msg->data.button.button_handle == dword_64C7E8[index]) {
                    if (index < dword_64CDC8) {
                        sub_55B880(dword_64CA60, dword_64CA68, &stru_5C8E50[index], NULL, -1, 1);
                    }

                    return true;
                }
            }

            return false;
        }

        if (msg->data.button.state == TIG_BUTTON_STATE_MOUSE_OUTSIDE) {
            for (index = 0; index < 15; index++) {
                if (msg->data.button.button_handle == dword_64C7E8[index]) {
                    if (index < dword_64CDC8) {
                        sub_55B880(dword_64CA60, dword_64C9D0, &stru_5C8E50[index], NULL, -1, 1);
                    }

                    return true;
                }
            }

            return false;
        }

        if (msg->data.button.state == TIG_BUTTON_STATE_PRESSED) {
            for (index = 0; index < 15; index++) {
                if (msg->data.button.button_handle == dword_64C7E8[index]) {
                    if (index < dword_64CDC8) {
                        level_auto_level_scheme_set(qword_64E010, dword_64CFE4[index + dword_64D424]);
                        if ((tig_net_flags & TIG_NET_CONNECTED) != 0) {
                            sub_4A45B0(player_get_pc_obj());
                        }
                        sub_55D210();
                    }

                    return true;
                }
            }

            if (msg->data.button.button_handle == dword_64CDD8) {
                if (dword_64D424 > 0) {
                    dword_64D424--;
                    sub_55D210();
                }

                return true;
            }

            if (msg->data.button.button_handle == dword_64D3AC) {
                if (dword_64D424 < dword_64CDC8 - 15) {
                    dword_64D424++;
                    sub_55D210();
                }

                return true;
            }

            return false;
        }

        return false;
    }

    return false;
}

// 0x55E110
bool sub_55E110()
{
    int num;
    int index;
    MesFileEntry mes_file_entry;
    TigFont font_desc;

    if (!mes_load("mes\\charedit.mes", &charedit_mes_file)) {
        return false;
    }

    num = 0;

    for (index = 0; index < 9; index++) {
        mes_file_entry.num = num++;
        mes_get_msg(charedit_mes_file, &mes_file_entry);
        stru_5C8150[index].str = mes_file_entry.str;
    }

    for (index = 0; index < 2; index++) {
        mes_file_entry.num = num++;
        mes_get_msg(charedit_mes_file, &mes_file_entry);
    }

    // Skip "Electrical".
    num++;

    for (index = 0; index < 13; index++) {
        if (index < 8) {
            stru_5C81E0[index].str = stat_get_name(stru_5C81E0[index].value);
        } else {
            mes_file_entry.num = num++;
            mes_get_msg(charedit_mes_file, &mes_file_entry);
            stru_5C81E0[index].str = mes_file_entry.str;
        }
    }

    for (index = 0; index < 23; index++) {
        mes_file_entry.num = num++;
        mes_get_msg(charedit_mes_file, &mes_file_entry);
        dword_64D3C4[index] = mes_file_entry.str;
    }

    mes_file_entry.num = num++;
    mes_get_msg(charedit_mes_file, &mes_file_entry);
    charedit_fatigue_str = mes_file_entry.str;

    mes_file_entry.num = num++;
    mes_get_msg(charedit_mes_file, &mes_file_entry);
    charedit_quest_str = mes_file_entry.str;

    mes_file_entry.num = num++;
    mes_get_msg(charedit_mes_file, &mes_file_entry);
    charedit_second_str = mes_file_entry.str;

    mes_file_entry.num = num++;
    mes_get_msg(charedit_mes_file, &mes_file_entry);
    charedit_seconds_str = mes_file_entry.str;

    mes_file_entry.num = num++;
    mes_get_msg(charedit_mes_file, &mes_file_entry);
    charedit_minimum_level_str = mes_file_entry.str;

    for (index = 0; index < BASIC_SKILL_COUNT; index++) {
        stru_5C82F0[index].str = basic_skill_get_name(index);
    }

    for (index = 0; index < TECH_SKILL_COUNT; index++) {
        stru_5C83B0[index].str = tech_skill_get_name(index);
    }

    for (index = 0; index < TRAINING_COUNT; index++) {
        dword_64CA74[index] = training_get_name(index);
    }

    for (index = 0; index < 4; index++) {
        stru_5C83F0[index].str = stru_5C8150[0].str;
    }

    font_desc.flags = 0;
    tig_art_interface_id_create(26, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_64CDBC);

    font_desc.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_64D3A8);

    font_desc.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 210, 0);
    tig_font_create(&font_desc, &dword_64CDD0);

    font_desc.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(128, 128, 128);
    tig_font_create(&font_desc, &dword_64C828);

    font_desc.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(0, 255, 0);
    tig_font_create(&font_desc, &dword_64C7A0);

    font_desc.flags = 0;
    tig_art_interface_id_create(27, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 0, 0);
    tig_font_create(&font_desc, &dword_64D3BC);

    font_desc.flags = 0;
    tig_art_interface_id_create(28, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_64DF0C);

    font_desc.flags = 0;
    tig_art_interface_id_create(28, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 0);
    tig_font_create(&font_desc, &dword_64D420);

    font_desc.flags = 0;
    tig_art_interface_id_create(28, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(128, 128, 128);
    tig_font_create(&font_desc, &dword_64CDB0);

    font_desc.flags = 0;
    tig_art_interface_id_create(171, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_64C848);

    font_desc.flags = 0;
    tig_art_interface_id_create(300, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_64D3A4);

    font_desc.flags = 0;
    tig_art_interface_id_create(301, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_64CFE0);

    font_desc.flags = 0;
    tig_art_interface_id_create(301, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 0);
    tig_font_create(&font_desc, &dword_64CDC0);

    font_desc.flags = 0;
    tig_art_interface_id_create(301, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(128, 128, 128);
    tig_font_create(&font_desc, &dword_64D3B0);

    font_desc.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(0, 0, 255);
    tig_font_create(&font_desc, &dword_64CA68);

    font_desc.flags = 0;
    tig_art_interface_id_create(229, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_64C9D0);

    font_desc.flags = 0;
    tig_art_interface_id_create(483, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 255);
    tig_font_create(&font_desc, &dword_64C840);

    font_desc.flags = 0;
    tig_art_interface_id_create(483, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(255, 255, 0);
    tig_font_create(&font_desc, &dword_64CDB8);

    font_desc.flags = 0;
    tig_art_interface_id_create(483, 0, 0, 0, &(font_desc.art_id));
    font_desc.str = NULL;
    font_desc.color = tig_color_make(128, 128, 128);
    tig_font_create(&font_desc, &dword_64D42C);

    return true;
}

// 0x55EBA0
void sub_55EBA0()
{
    mes_unload(charedit_mes_file);
    tig_font_destroy(dword_64CDBC);
    tig_font_destroy(dword_64D3A8);
    tig_font_destroy(dword_64CDD0);
    tig_font_destroy(dword_64C828);
    tig_font_destroy(dword_64C7A0);
    tig_font_destroy(dword_64D3BC);
    tig_font_destroy(dword_64DF0C);
    tig_font_destroy(dword_64D420);
    tig_font_destroy(dword_64CDB0);
    tig_font_destroy(dword_64C848);
    tig_font_destroy(dword_64D3A4);
    tig_font_destroy(dword_64CFE0);
    tig_font_destroy(dword_64CDC0);
    tig_font_destroy(dword_64D3B0);
    tig_font_destroy(dword_64CA68);
    tig_font_destroy(dword_64C9D0);
    tig_font_destroy(dword_64C840);
    tig_font_destroy(dword_64CDB8);
    tig_font_destroy(dword_64D42C);
}

// 0x55EC90
void sub_55EC90()
{
    int index;
    char buffer[3][10];
    const char* tmp[3];
    int value;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    for (index = 0; index < 3; index++) {
        tmp[index] = buffer[index];
    }

    value = stat_level(qword_64E010, STAT_ALIGNMENT) / 10;
    sprintf(buffer[0], "%d", value);

    tig_art_interface_id_create(254, value / 10 + 10, 0, 0, &(art_blit_info.art_id));
    if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = 56 - art_frame_data.hot_x;
    dst_rect.y = 181 - art_frame_data.hot_y;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(dword_64CA64, &art_blit_info);

    tig_art_interface_id_create(255, 0, 0, 0, &(art_blit_info.art_id));
    if (tig_art_frame_data(art_blit_info.art_id, &art_frame_data) != TIG_OK) {
        return;
    }

    tig_art_interface_id_create(22, 0, 0, 0, &(art_blit_info.art_id));

    src_rect.x = 775;
    src_rect.y = 87;
    src_rect.width = art_frame_data.width;
    src_rect.height = 278;

    dst_rect.x = 775;
    dst_rect.y = 87;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = 278;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(dword_64CA64, &art_blit_info);

    value = stat_level(qword_64E010, STAT_MAGICK_TECH_APTITUDE);
    if (value < 0) {
        sprintf(buffer[2], "%d", -value);
        strcpy(buffer[1], "0");
    } else {
        sprintf(buffer[1], "%d", value);
        strcpy(buffer[2], "0");
    }

    tig_art_interface_id_create(255, 0, 0, 0, &(art_blit_info.art_id));

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = 775;
    dst_rect.y = 221 - (value * 128) / 100;
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(dword_64CA64, &art_blit_info);

    tig_art_interface_id_create(22, 0, 0, 0, &(art_blit_info.art_id));

    art_blit_info.src_rect = &stru_5C8970;
    art_blit_info.dst_rect = &stru_5C8970;
    art_blit_info.flags = 0;
    tig_window_blit_art(dword_64CA64, &art_blit_info);

    art_blit_info.src_rect = &stru_5C8980;
    art_blit_info.dst_rect = &stru_5C8980;
    tig_window_blit_art(dword_64CA64, &art_blit_info);

    sub_55B880(dword_64CA64, dword_64C848, stru_5C8940, tmp, -1, 3);
}

// 0x55EFB0
void sub_55EFB0()
{
    if (dword_64DEE4) {
        scrollbar_ui_control_destroy(stru_64C7A8);
        dword_64DEE4 = false;
    }
}

// 0x55EFE0
void sub_55EFE0()
{
    sub_55EFF0();
}

// 0x55EFF0
void sub_55EFF0()
{
    ScrollbarUiControlInfo sb;

    if (!dword_64DEE4) {
        sb.flags = 0x39F;
        sb.field_38 = dword_64D424;
        sb.field_3C = sub_55F0E0;
        if (dword_64CDC8 > 15) {
            sb.field_24 = dword_64CDC8 - 15;
        } else {
            sb.field_24 = 0;
        }
        sb.field_4 = stru_5C8F40;
        sb.field_28 = 0;
        sb.rect.width = stru_5C8F40.width + 160;
        sb.rect.x = 23;
        sb.rect.y = 66;
        sb.rect.height = 240;
        sb.field_40 = sub_55F110;
        sb.field_2C = 1;
        scrollbar_ui_control_create(&stru_64C7A8, &sb, dword_64CA60);
        sub_5806F0(stru_64C7A8);
        dword_64DEE4 = true;
    }
}

// 0x55F0D0
void sub_55F0D0()
{
    sub_55EFB0();
}

// 0x55F0E0
void sub_55F0E0(int value)
{
    if (dword_64DEE4) {
        dword_64D424 = value;
        if (dword_64E01C == 3) {
            sub_55D210();
        }
    }
}

// 0x55F110
void sub_55F110(TigRect* rect)
{
    TigArtBlitInfo blit_info;

    if (dword_64DEE4) {
        tig_art_interface_id_create(567, 0, 0, 0, &(blit_info.art_id));
        blit_info.flags = 0;
        blit_info.src_rect = rect;
        blit_info.dst_rect = rect;
        tig_window_blit_art(dword_64CA60, &blit_info);
    }
}

// 0x55F160
void sub_55F160()
{
    if (charedit_created) {
        stru_5C8990.str = dword_64D3C4[1];
        sub_550750(&stru_5C8990);
    }
}

// 0x55F180
void sub_55F180()
{
    if (charedit_created) {
        stru_5C8990.str = dword_64D3C4[13];
        sub_550750(&stru_5C8990);
    }
}

// 0x55F1A0
void sub_55F1A0()
{
    if (charedit_created) {
        stru_5C8990.str = dword_64D3C4[9];
        sub_550750(&stru_5C8990);
    }
}

// 0x55F1E0
void sub_55F1E0()
{
    if (charedit_created) {
        stru_5C8990.str = dword_64D3C4[4];
        sub_550750(&stru_5C8990);
    }
}

// 0x55F200
void sub_55F200(int stat)
{
    if (charedit_created) {
        switch (stat) {
        case STAT_STRENGTH:
            stru_5C8990.str = dword_64D3C4[15];
            sub_550750(&stru_5C8990);
            break;
        case STAT_DEXTERITY:
            stru_5C8990.str = dword_64D3C4[16];
            sub_550750(&stru_5C8990);
            break;
        case STAT_CONSTITUTION:
            stru_5C8990.str = dword_64D3C4[17];
            sub_550750(&stru_5C8990);
            break;
        case STAT_BEAUTY:
            stru_5C8990.str = dword_64D3C4[18];
            sub_550750(&stru_5C8990);
            break;
        case STAT_INTELLIGENCE:
            stru_5C8990.str = dword_64D3C4[9];
            sub_550750(&stru_5C8990);
            break;
        case STAT_PERCEPTION:
            stru_5C8990.str = dword_64D3C4[20];
            sub_550750(&stru_5C8990);
            break;
        case STAT_WILLPOWER:
            stru_5C8990.str = dword_64D3C4[19];
            sub_550750(&stru_5C8990);
            break;
        case STAT_CHARISMA:
            stru_5C8990.str = dword_64D3C4[21];
            sub_550750(&stru_5C8990);
            break;
        default:
            stru_5C8990.str = dword_64D3C4[5];
            sub_550750(&stru_5C8990);
            break;
        }
    }
}

// 0x55F320
void sub_55F320()
{
    if (charedit_created) {
        stru_5C8990.str = dword_64D3C4[7];
        sub_550750(&stru_5C8990);
    }
}

// 0x55F340
void sub_55F340()
{
    if (charedit_created) {
        stru_5C8990.str = dword_64D3C4[8];
        sub_550750(&stru_5C8990);
    }
}

// 0x55F360
void sub_55F360(int player)
{
    int64_t obj;
    int index;

    obj = sub_4A2B60(player);
    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    for (index = 0; index < CHAREDIT_STAT_COUNT; index++) {
        dword_64D434[player][index] = sub_55B4D0(obj, index);
    }

    for (index = 0; index < TECH_COUNT; index++) {
        dword_64DF10[player][index] = tech_get_degree(obj, index);
    }

    for (index = 0; index < COLLEGE_COUNT; index++) {
        dword_64CDDC[player][index] = spell_college_level_get(obj, index);
    }

    for (index = 0; index < BASIC_SKILL_COUNT; index++) {
        dword_64C84C[player][index] = basic_skill_get_base(obj, index);
    }

    for (index = 0; index < TECH_SKILL_COUNT; index++) {
        dword_64C9D4[player][index] = tech_skill_get_base(obj, index);
    }
}

// 0x55F450
void sub_55F450(int player, int type, int param)
{
    int64_t obj;
    int value;
    int cost;
    int unspent_points;

    obj = sub_4A2B60(player);
    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    switch (type) {
    case 0:
        value = sub_55B4D0(obj, param) + 1;
        if (param == CHAREDIT_STAT_HP_PTS || param == CHAREDIT_STAT_FATIGUE_PTS) {
            cost = 1;
        } else {
            int stat;

            stat = sub_55B410(param);
            if (stat_level(obj, stat) > stat_get_max_value(obj, stat)) {
                stru_5C8990.str = dword_64D3C4[0];
                sub_4EDA60(&stru_5C8990, player, 0);
                return;
            }

            cost = sub_4B0F50(value);
        }

        unspent_points = stat_level(obj, STAT_UNSPENT_POINTS);
        if (cost > unspent_points) {
            stru_5C8990.str = dword_64D3C4[1];
            sub_4EDA60(&stru_5C8990, player, 0);
            return;
        }

        sub_55B720(obj, param, value);

        if (sub_55B4D0(obj, param) < value) {
            stru_5C8990.str = dword_64D3C4[0];
            sub_4EDA60(&stru_5C8990, player, 0);
            return;
        }

        stat_set_base(obj, STAT_UNSPENT_POINTS, unspent_points - cost);
        break;
    case 1:
    case 2:
        skill_ui_inc_skill(obj, param);
        break;
    case 3:
        tech_ui_inc_degree(obj, param);
        break;
    case 4:
        sub_57C540(obj, spell_college_level_get(obj, param) + 5 * param);
        break;
    }
}

// 0x55F5F0
void sub_55F5F0(int player, int type, int param)
{
    int64_t obj;
    int value;
    int cost;
    int unspent_points;

    obj = sub_4A2B60(player);
    if (obj == OBJ_HANDLE_NULL) {
        return;
    }

    switch (type) {
    case 0:
        value = sub_55B4D0(obj, param);
        if (value == dword_64D434[player][param]) {
            stru_5C8990.str = dword_64D3C4[2];
            sub_4EDA60(&stru_5C8990, player, 0);
            return;
        }

        if (param == CHAREDIT_STAT_HP_PTS || param == CHAREDIT_STAT_FATIGUE_PTS) {
            cost = 1;
        } else {
            cost = sub_4B0F50(value);
        }

        value--;

        sub_55B720(obj, param, value);
        if (sub_55B4D0(obj, param) > value) {
            if (value == 0) {
                stru_5C8990.str = dword_64D3C4[3];
                sub_4EDA60(&stru_5C8990, player, 0);
                return;
            }

            if (dword_5C8124[param] != -1
                && !sub_4C6B50(obj, dword_5C8124[param], value - 1)) {
                stru_5C8990.str = dword_64D3C4[11];
                sub_4EDA60(&stru_5C8990, player, 0);
                return;
            }

            if (stru_5C8028[param].art_num == CHAREDIT_STAT_INTELLIGENCE_BASE
                && !sub_4B1B90(obj, value - 1)) {
                stru_5C8990.str = dword_64D3C4[12];
                sub_4EDA60(&stru_5C8990, player, 0);
                return;
            }

            if (stru_5C8028[param].art_num == CHAREDIT_STAT_INTELLIGENCE_BASE
                && !sub_4B02B0(obj, value - 1)) {
                stru_5C8990.str = dword_64D3C4[14];
                sub_4EDA60(&stru_5C8990, player, 0);
                return;
            }

            if (stru_5C8028[param].art_num == CHAREDIT_STAT_WILLPOWER_BASE
                && !sub_4B1C00(obj, value - 1)) {
                stru_5C8990.str = dword_64D3C4[22];
                sub_4EDA60(&stru_5C8990, player, 0);
                return;
            }

            // Something else is wrong.
            return;
        }

        unspent_points = stat_level(obj, STAT_UNSPENT_POINTS);
        stat_set_base(obj, STAT_UNSPENT_POINTS, unspent_points + cost);
        break;
    case 1:
        if (tech_skill_get_base(obj, GET_TECH_SKILL(param)) == dword_64C84C[player][param]) {
            stru_5C8990.str = dword_64D3C4[6];
            sub_4EDA60(&stru_5C8990, player, 0);
            return;
        }
        skill_ui_dec_skill(obj, param);
        break;
    case 2:
        if (tech_skill_get_base(obj, GET_TECH_SKILL(param)) == dword_64C9D4[player][param]) {
            stru_5C8990.str = dword_64D3C4[6];
            sub_4EDA60(&stru_5C8990, player, 0);
            return;
        }
        skill_ui_dec_skill(obj, param);
        break;
    case 3:
        tech_ui_dec_degree(obj, param);
        break;
    case 4:
        value = spell_college_level_get(obj, param);
        if (value == dword_64CDDC[player][param]) {
            stru_5C8990.str = dword_64D3C4[10];
            sub_4EDA60(&stru_5C8990, player, 0);
            return;
        }

        spell_ui_remove(obj, value + 5 * param - 1);
        break;
    }
}
