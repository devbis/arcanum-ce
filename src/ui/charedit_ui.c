#include "ui/charedit_ui.h"

#include <stdio.h>

#include "game/combat.h"
#include "game/critter.h"
#include "game/level.h"
#include "game/mes.h"
#include "game/skill.h"
#include "game/spell.h"
#include "game/stat.h"
#include "ui/intgame.h"
#include "ui/scrollbar_ui.h"

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
static void sub_55B150();
static int sub_55B4D0(int64_t obj, int param);
static bool sub_55BAB0();
static bool sub_55C110();
static void sub_55C2E0(int a1);
static bool sub_55C890();
static bool sub_55D060();
static bool sub_55E110();
static void sub_55EBA0();
static void sub_55EFB0();
static void sub_55EFE0();
static void sub_55EFF0();
static void sub_55F0D0();
static void sub_55F0E0(int value);
static void sub_55F110(TigRect* rect);

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
    { NULL, 322, 342, 0 },
    { NULL, 322, 358, 4 },
    { NULL, 322, 374, 1 },
    { NULL, 322, 390, 3 },
    { NULL, 322, 406, 2 },
};

// 0x5C82B0
static S5C87D0 stru_5C82B0[4] = {
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
static S5C87D0 stru_5C8430[16] = {
    { 192, 84, TIG_BUTTON_HANDLE_INVALID, 0 },
    { 192, 150, TIG_BUTTON_HANDLE_INVALID, 1 },
    { 192, 216, TIG_BUTTON_HANDLE_INVALID, 2 },
    { 192, 282, TIG_BUTTON_HANDLE_INVALID, 3 },
    { 192, 84, TIG_BUTTON_HANDLE_INVALID, 4 },
    { 192, 150, TIG_BUTTON_HANDLE_INVALID, 5 },
    { 192, 216, TIG_BUTTON_HANDLE_INVALID, 6 },
    { 192, 282, TIG_BUTTON_HANDLE_INVALID, 7 },
    { 192, 84, TIG_BUTTON_HANDLE_INVALID, 8 },
    { 192, 150, TIG_BUTTON_HANDLE_INVALID, 9 },
    { 192, 216, TIG_BUTTON_HANDLE_INVALID, 10 },
    { 192, 282, TIG_BUTTON_HANDLE_INVALID, 11 },
    { 192, 84, TIG_BUTTON_HANDLE_INVALID, 0 },
    { 192, 150, TIG_BUTTON_HANDLE_INVALID, 1 },
    { 192, 216, TIG_BUTTON_HANDLE_INVALID, 2 },
    { 192, 282, TIG_BUTTON_HANDLE_INVALID, 3 },
};

// 0x5C8530
static S5C87D0 stru_5C8530[16] = {
    { 18, 84, TIG_BUTTON_HANDLE_INVALID, 0 },
    { 18, 150, TIG_BUTTON_HANDLE_INVALID, 1 },
    { 18, 216, TIG_BUTTON_HANDLE_INVALID, 2 },
    { 18, 282, TIG_BUTTON_HANDLE_INVALID, 3 },
    { 18, 84, TIG_BUTTON_HANDLE_INVALID, 4 },
    { 18, 150, TIG_BUTTON_HANDLE_INVALID, 5 },
    { 18, 216, TIG_BUTTON_HANDLE_INVALID, 6 },
    { 18, 282, TIG_BUTTON_HANDLE_INVALID, 7 },
    { 18, 84, TIG_BUTTON_HANDLE_INVALID, 8 },
    { 18, 150, TIG_BUTTON_HANDLE_INVALID, 9 },
    { 18, 216, TIG_BUTTON_HANDLE_INVALID, 10 },
    { 18, 282, TIG_BUTTON_HANDLE_INVALID, 11 },
    { 18, 84, TIG_BUTTON_HANDLE_INVALID, 0 },
    { 18, 150, TIG_BUTTON_HANDLE_INVALID, 1 },
    { 18, 216, TIG_BUTTON_HANDLE_INVALID, 2 },
    { 18, 282, TIG_BUTTON_HANDLE_INVALID, 3 },
};

// 0x5C87D0
static S5C87D0 stru_5C87D0[8] = {
    { 8, 7, TIG_BUTTON_HANDLE_INVALID, 0 },
    { 34, 29, TIG_BUTTON_HANDLE_INVALID, 1 },
    { 61, 7, TIG_BUTTON_HANDLE_INVALID, 2 },
    { 88, 28, TIG_BUTTON_HANDLE_INVALID, 3 },
    { 116, 7, TIG_BUTTON_HANDLE_INVALID, 4 },
    { 142, 29, TIG_BUTTON_HANDLE_INVALID, 5 },
    { 171, 7, TIG_BUTTON_HANDLE_INVALID, 6 },
    { 198, 29, TIG_BUTTON_HANDLE_INVALID, 7 },
};

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

// 0x5C8630
static S5C87D0 stru_5C8630[COLLEGE_COUNT] = {
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

// 0x5C8990
static John stru_5C8990 = { 4, 0, 0, 0, 0 };

// 0x5C8CA8
static S5C8CA8 stru_5C8CA8[4] = {
    { 520, 126, 200, 60, 0, TIG_BUTTON_HANDLE_INVALID },
    { 520, 192, 200, 60, 0, TIG_BUTTON_HANDLE_INVALID },
    { 520, 258, 200, 60, 0, TIG_BUTTON_HANDLE_INVALID },
    { 520, 324, 200, 60, 0, TIG_BUTTON_HANDLE_INVALID },
};

// 0x5C8F40
static TigRect stru_5C8F40 = { 209, 60, 17, 255 };

// 0x5C8F70
static int dword_5C8F70[] = {
    8,
    8,
    8,
    8,
    8,
    8,
    8,
};

// 0x5C8F8C
static int dword_5C8F8C[] = {
    96,
    129,
    162,
    195,
    228,
    261,
    294,
};

// 0x5C8FA8
static int dword_5C8FA8[] = {
    653,
    649,
    650,
    651,
    652,
    654,
    655,
    656,
};

// 0x64C7A0
static tig_font_handle_t dword_64C7A0;

// 0x64C7A8
static ScrollbarId stru_64C7A8;

// 0x64C7B0
static tig_window_handle_t dword_64C7B0;

// 0x64C7E8
static tig_button_handle_t dword_64C7E8[15];

// 0x64C828
static tig_font_handle_t dword_64C828;

// 0x64C840
static tig_font_handle_t dword_64C840;

// 0x64C844
static const char* charedit_fatigue_str;

// 0x64C848
static tig_font_handle_t dword_64C848;

// 0x64C9D0
static tig_font_handle_t dword_64C9D0;

// 0x64CA60
static tig_window_handle_t dword_64CA60;

// 0x64CA64
static tig_window_handle_t dword_64CA64;

// 0x64CA68
static tig_font_handle_t dword_64CA68;

// 0x64CA6C
static tig_window_handle_t dword_64CA6C;

// 0x64CA70
static mes_file_handle_t charedit_mes_file;

// 0x64CA74
static const char* dword_64CA74[TRAINING_COUNT];

// 0x64CA84
static const char* charedit_seconds_str;

// 0x64CA88
static const char* charedit_second_str;

// 0x64CA8C
static tig_window_handle_t dword_64CA8C;

// 0x64CA90
static const char* dword_64CA90[200];

// 0x64CDB0
static tig_font_handle_t dword_64CDB0;

// 0x64CDB8
static tig_font_handle_t dword_64CDB8;

// 0x64CDBC
static tig_font_handle_t dword_64CDBC;

// 0x64CDC0
static tig_font_handle_t dword_64CDC0;

// 0x64CDC4
static int dword_64CDC4;

// 0x64CDC8
static int dword_64CDC8;

// 0x64CDCC
static int dword_64CDCC;

// 0x64CDD0
static tig_font_handle_t dword_64CDD0;

// 0x64CDD4
static tig_button_handle_t spell_plus_bid;

// 0x64CFE0
static tig_font_handle_t dword_64CFE0;

// 0x64CFE4
static int dword_64CFE4[200];

// 0x64D3A4
static tig_font_handle_t dword_64D3A4;

// 0x64D3A8
static tig_font_handle_t dword_64D3A8;

// 0x64D3B0
static tig_font_handle_t dword_64D3B0;

// 0x64D3B4
static int dword_64D3B4;

// 0x64D3BC
static tig_font_handle_t dword_64D3BC;

// 0x64D3C0
static const char* charedit_quest_str;

// 0x64D3C4
static const char* dword_64D3C4[23];

// 0x64D420
static tig_font_handle_t dword_64D420;

// 0x64D424
static int dword_64D424;

// 0x64D42C
static tig_font_handle_t dword_64D42C;

// 0x64D430
static tig_button_handle_t spell_minus_bid;

// 0x64DEE4
static bool dword_64DEE4;

// 0x64DF0C
static tig_font_handle_t dword_64DF0C;

// 0x64E010
static int64_t qword_64E010;

// 0x64E018
static bool charedit_created;

// 0x64E01C
static int dword_64E01C;

// 0x64E020
static int dword_64E020;

// 0x64E024
static int dword_64E024;

// 0x64E028
static int dword_64E028;

// 0x64E02C
static bool dword_64E02C;

// 0x64DEE8
static const char* charedit_minimum_level_str;

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

    if (!sub_55C890()) {
        sub_55EBA0();
        tig_window_destroy(dword_64CA60);
        return false;
    }

    if (!sub_55C110()) {
        sub_55EBA0();
        tig_window_destroy(dword_64C7B0);
        tig_window_destroy(dword_64CA60);
        return false;
    }

    if (!sub_55BAB0()) {
        sub_55EBA0();
        tig_window_destroy(dword_64C7B0);
        tig_window_destroy(dword_64CA8C);
        tig_window_destroy(dword_64CA60);
        return false;
    }

    tig_window_hide(dword_64CA6C);
    tig_window_hide(dword_64C7B0);
    tig_window_hide(dword_64CA8C);
    tig_window_hide(dword_64CA60);

    return true;
}

// 0x559770
void charedit_exit()
{
    sub_55EBA0();
    tig_window_destroy(dword_64CA6C);
    tig_window_destroy(dword_64C7B0);
    tig_window_destroy(dword_64CA8C);
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
void sub_5597C0()
{
    // TODO: Incomplete.
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
            object_set_hp_damage(qword_64E010, 0);
            critter_fatigue_damage_set(qword_64E010, 0);
        }
        intgame_big_window_unlock();
        tig_window_hide(dword_64CA6C);
        tig_window_hide(dword_64C7B0);
        tig_window_hide(dword_64CA8C);
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
    sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[3]), &(v2[3]), -1, 1);

    if (obj_field_int32_get(qword_64E010, OBJ_F_TYPE) == OBJ_TYPE_PC) {
        sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[4]), &(v2[4]), -1, 3);
    } else if (!sub_45F730(qword_64E010)) {
        sub_55B880(dword_64CA64, dword_64D3A8, &(stru_5C8150[4]), &(v2[4]), -1, 2);
    }
}

// 0x55A5C0
void sub_55A5C0()
{
    // TODO: Incomplete.
}

// 0x55AE70
void sub_55AE70()
{
    // TODO: Incomplete.
}

// 0x55B0E0
void sub_55B0E0()
{
    // TODO: Incomplete.
}

// 0x55B150
void sub_55B150()
{
    sub_55B280();
    sub_55B1B0();
    switch (dword_64E01C) {
    case 0:
        sub_55BF20();
        break;
    case 1:
        sub_55C3A0();
        break;
    case 2:
        spells_print_all();
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
    // TODO: Incomplete.
}

// 0x55B280
void sub_55B280()
{
    // TODO: Incomplete.
}

// 0x55B2A0
void sub_55B2A0()
{
    // TODO: Incomplete.
}

// 0x55B410
void sub_55B410()
{
    // TODO: Incomplete.
}

// 0x55B4D0
int sub_55B4D0(int64_t obj, int param)
{
    switch (param) {
    case 0:
        return stat_get_base(obj, STAT_UNSPENT_POINTS);
    case 1:
        return stat_level(obj, STAT_LEVEL);
    case 2:
        return level_get_experience_points_to_next_level(obj);
    case 3:
        return sub_43D600(obj);
    case 4:
        return object_get_hp_pts(obj);
    case 5:
        return sub_45D700(obj);
    case 6:
        return critter_fatigue_pts_get_(obj);
    case 7:
        return stat_get_base(obj, STAT_STRENGTH);
    case 8:
        return stat_level(obj, STAT_STRENGTH);
    case 9:
        return stat_get_base(obj, STAT_CONSTITUTION);
    case 10:
        return stat_level(obj, STAT_CONSTITUTION);
    case 11:
        return stat_get_base(obj, STAT_DEXTERITY);
    case 12:
        return stat_level(obj, STAT_DEXTERITY);
    case 13:
        return stat_get_base(obj, STAT_BEAUTY);
    case 14:
        return stat_level(obj, STAT_BEAUTY);
    case 15:
        return stat_get_base(obj, STAT_INTELLIGENCE);
    case 16:
        return stat_level(obj, STAT_INTELLIGENCE);
    case 17:
        return stat_get_base(obj, STAT_WILLPOWER);
    case 18:
        return stat_level(obj, STAT_WILLPOWER);
    case 19:
        return stat_get_base(obj, STAT_PERCEPTION);
    case 20:
        return stat_level(obj, STAT_PERCEPTION);
    case 21:
        return stat_get_base(obj, STAT_CHARISMA);
    case 22:
        return stat_level(obj, STAT_CHARISMA);
    default:
        return 0;
    }
}

// 0x55B720
void sub_55B720()
{
    // TODO: Incomplete.
}

// 0x55B880
void sub_55B880()
{
    // TODO: Incomplete.
}

// 0x55BAB0
bool sub_55BAB0()
{
    tig_art_id_t art_id;
    TigWindowData window_data;
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    int index;
    tig_button_handle_t button_handles[4];

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
    if (tig_window_create(&window_data, &dword_64CA6C) != TIG_OK) {
        return false;
    }

    button_data.flags = TIG_BUTTON_FLAG_0x01 | TIG_BUTTON_FLAG_HIDDEN;
    button_data.window_handle = dword_64CA6C;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    tig_art_interface_id_create(624, 0, 0, 0, &(button_data.art_id));
    for (index = 0; index < 16; index++) {
        button_data.x = stru_5C8430[index].x;
        button_data.y = stru_5C8430[index].y;
        if (tig_button_create(&button_data, &(stru_5C8430[index].button_handle)) != TIG_OK) {
            tig_window_destroy(dword_64CA6C);
            return false;
        }
    }

    tig_art_interface_id_create(625, 0, 0, 0, &(button_data.art_id));
    for (index = 0; index < 16; index++) {
        button_data.x = stru_5C8530[index].x;
        button_data.y = stru_5C8530[index].y;
        if (tig_button_create(&button_data, &(stru_5C8530[index].button_handle)) != TIG_OK) {
            tig_window_destroy(dword_64CA6C);
            return false;
        }
    }

    button_data.flags = TIG_BUTTON_FLAG_0x02 | TIG_BUTTON_FLAG_0x04;
    for (index = 0; index < 4; index++) {
        button_data.x = stru_5C82B0[index].x;
        button_data.y = stru_5C82B0[index].y;
        tig_art_interface_id_create(stru_5C82B0[index].art_num, 0, 0, 0, &(button_data.art_id));
        if (tig_button_create(&button_data, &(stru_5C82B0[index].button_handle)) != TIG_OK) {
            tig_window_destroy(dword_64CA6C);
            return false;
        }
    }

    tig_button_radio_group_create(4, button_handles, dword_64E020);

    button_data.flags = TIG_BUTTON_FLAG_0x01;
    button_data.mouse_down_snd_id = -1;
    button_data.mouse_up_snd_id = -1;
    button_data.art_id = -1;

    for (index = 0; index < 4; index++) {
        button_data.x = stru_5C8CA8[index].x - 503;
        button_data.y = stru_5C8CA8[index].y - 63;
        button_data.width = stru_5C8CA8[index].width;
        button_data.height = stru_5C8CA8[index].height;
        tig_button_create(&button_data, &(stru_5C8CA8[index].button_handle));
        // FIXME: No error checking as seen above.
    }

    return true;
}

// 0x55BD10
void sub_55BD10()
{
    // TODO: Incomplete.
}

// 0x55BF20
void sub_55BF20()
{
    // TODO: Incomplete.
}

// 0x55C110
bool sub_55C110()
{
    tig_art_id_t art_id;
    TigWindowData window_data;
    TigButtonData button_data;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    tig_button_handle_t button_handles[8];
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
    window_data.message_filter = sub_55D940;
    if (tig_window_create(&window_data, &dword_64CA8C) != TIG_OK) {
        return false;
    }

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &(window_data.rect);
    art_blit_info.dst_rect = &(window_data.rect);
    if (tig_window_blit_art(dword_64CA8C, &art_blit_info) != TIG_OK) {
        tig_window_destroy(dword_64CA8C);
        return false;
    }

    button_data.flags = TIG_BUTTON_FLAG_0x02 | TIG_BUTTON_FLAG_0x04;
    button_data.window_handle = dword_64CA8C;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    for (index = 0; index < 8; index++) {
        tig_art_interface_id_create(306 + index, 0, 0, 0, &(button_data.art_id));
        button_data.x = stru_5C87D0[index].x;
        button_data.y = stru_5C87D0[index].y;
        if (tig_button_create(&button_data, &(stru_5C87D0[index].button_handle)) != TIG_OK) {
            tig_window_destroy(dword_64CA8C);
            return false;
        }

        button_handles[index] = stru_5C87D0[index].button_handle;
    }

    tig_button_radio_group_create(8, button_handles, dword_64E028);
    dword_64D3B4 = -1;
    dword_64CDC4 = -1;

    return true;
}

// 0x55C2E0
void sub_55C2E0(int a1)
{
    tig_art_id_t art_id;
    TigArtFrameData art_frame_data;
    TigArtBlitInfo art_blit_info;
    TigRect src_rect;
    TigRect dst_rect;

    tig_art_interface_id_create(dword_5C8FA8[dword_64E028], 0, 0, 0, &art_id);
    tig_art_frame_data(art_id, &art_frame_data);

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = art_frame_data.width;
    src_rect.height = art_frame_data.height;

    dst_rect.x = dword_5C8F8C[a1];
    dst_rect.y = dword_5C8F70[a1];
    dst_rect.width = art_frame_data.width;
    dst_rect.height = art_frame_data.height;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &src_rect;
    art_blit_info.dst_rect = &dst_rect;
    tig_window_blit_art(dword_64CA8C, &art_blit_info);
}

// 0x55C3A0
void sub_55C3A0()
{
    // TODO: Incomplete.
}

// 0x55C890
bool sub_55C890()
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
    if (tig_window_create(&window_data, &dword_64C7B0) != TIG_OK) {
        return false;
    }

    window_data.rect.x = 0;
    window_data.rect.y = 0;

    art_blit_info.flags = 0;
    art_blit_info.art_id = art_id;
    art_blit_info.src_rect = &(window_data.rect);
    art_blit_info.dst_rect = &(window_data.rect);
    if (tig_window_blit_art(dword_64C7B0, &art_blit_info) != TIG_OK) {
        tig_window_destroy(dword_64C7B0);
        return false;
    }

    button_data.flags = TIG_BUTTON_FLAG_0x02 | TIG_BUTTON_FLAG_0x04;
    button_data.window_handle = dword_64C7B0;
    button_data.mouse_down_snd_id = 3000;
    button_data.mouse_up_snd_id = 3001;
    button_data.mouse_enter_snd_id = -1;
    button_data.mouse_exit_snd_id = -1;

    for (index = 0; index < COLLEGE_COUNT; index++) {
        art_num = college_get_art_num(index);
        if (art_num != -1) {
            tig_art_interface_id_create(art_num, 0, 0, 0, &(button_data.art_id));
            button_data.x = stru_5C8630[index].x - 503;
            button_data.y = stru_5C8630[index].y - 104;
            if (tig_button_create(&button_data, &(stru_5C8630[index].button_handle)) != TIG_OK) {
                tig_window_destroy(dword_64C7B0);
                return false;
            }

            button_handles[index] = stru_5C8630[index].button_handle;
        }
    }

    tig_button_radio_group_create(COLLEGE_COUNT, button_handles, dword_64E024);

    spell_plus_bid = TIG_BUTTON_HANDLE_INVALID;
    spell_minus_bid = TIG_BUTTON_HANDLE_INVALID;

    return true;
}

// 0x55CA70
void sub_55CA70()
{
    // TODO: Incomplete.
}

// 0x55CBC0
void spells_print_all()
{
    // TODO: Incomplete.
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
    // TODO: Incomplete.
}

// 0x55D3A0
void sub_55D3A0()
{
    // TODO: Incomplete.
}

// 0x55D6F0
void sub_55D6F0()
{
    // TODO: Incomplete.
}

// 0x55D940
void sub_55D940()
{
    // TODO: Incomplete.
}

// 0x55DC60
void sub_55DC60()
{
    // TODO: Incomplete.
}

// 0x55DF90
void sub_55DF90()
{
    // TODO: Incomplete.
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
    // TODO: Incomplete.
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
void sub_55F200(int type)
{
    if (charedit_created) {
        switch (type) {
        case 0:
            stru_5C8990.str = dword_64D3C4[15];
            sub_550750(&stru_5C8990);
            break;
        case 1:
            stru_5C8990.str = dword_64D3C4[16];
            sub_550750(&stru_5C8990);
            break;
        case 2:
            stru_5C8990.str = dword_64D3C4[17];
            sub_550750(&stru_5C8990);
            break;
        case 3:
            stru_5C8990.str = dword_64D3C4[18];
            sub_550750(&stru_5C8990);
            break;
        case 4:
            stru_5C8990.str = dword_64D3C4[9];
            sub_550750(&stru_5C8990);
            break;
        case 5:
            stru_5C8990.str = dword_64D3C4[20];
            sub_550750(&stru_5C8990);
            break;
        case 6:
            stru_5C8990.str = dword_64D3C4[19];
            sub_550750(&stru_5C8990);
            break;
        case 7:
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
void sub_55F360()
{
    // TODO: Incomplete.
}

// 0x55F450
void sub_55F450()
{
    // TODO: Incomplete.
}

// 0x55F5F0
void sub_55F5F0()
{
    // TODO: Incomplete.
}
