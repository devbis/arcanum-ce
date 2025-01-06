#include "game/spell.h"

#include "game/magictech.h"
#include "game/mes.h"
#include "game/mp_utils.h"
#include "game/multiplayer.h"
#include "game/object.h"
#include "game/player.h"
#include "game/stat.h"

#define FIRST_COLLEGE_NAME_ID 500
#define FIRST_SPELL_DESCRIPTION_ID 700
#define FIRST_COLLEGE_DESCRIPTION_ID 8000
#define FIRST_COLLEGE_MASTERY_DESCRIPTION_ID 9000

#define FIVE 5

typedef struct SpellInfo {
    /* 0000 */ char* field_0;
    /* 0004 */ char* description;
    /* 0008 */ int field_8;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ int field_40;
    /* 0044 */ int field_44;
    /* 0048 */ int field_48;
    /* 004C */ int field_4C;
    /* 0050 */ int field_50;
    /* 0054 */ int field_54;
    /* 0058 */ int field_58;
    /* 005C */ int field_5C;
    /* 0060 */ int field_60;
    /* 0064 */ int field_64;
    /* 0068 */ int field_68;
    /* 006C */ int field_6C;
    /* 0070 */ int field_70;
    /* 0074 */ int field_74;
    /* 0078 */ int field_78;
    /* 007C */ int field_7C;
    /* 0080 */ int field_80;
    /* 0084 */ int field_84;
    /* 0088 */ int field_88;
    /* 008C */ int field_8C;
    /* 0090 */ int field_90;
    /* 0094 */ int field_94;
    /* 0098 */ int field_98;
    /* 009C */ int field_9C;
    /* 00A0 */ int field_A0;
    /* 00A4 */ int field_A4;
    /* 00A8 */ int field_A8;
    /* 00AC */ int field_AC;
    /* 00B0 */ int field_B0;
    /* 00B4 */ int field_B4;
} SpellInfo;

static_assert(sizeof(SpellInfo) == 0xB8, "wrong size");

// 0x5B5770
static int spell_minimum_levels[FIVE] = {
    1,
    1,
    5,
    10,
    15,
};

// 0x5B55B0
static int college_art_nums[COLLEGE_COUNT] = {
    11,
    44,
    38,
    92,
    50,
    86,
    56,
    98,
    62,
    68,
    74,
    104,
    110,
    80,
    116,
    122,
};

// 0x5B55F0
static int college_icons[COLLEGE_COUNT] = {
    446,
    447,
    448,
    449,
    450,
    451,
    452,
    453,
    454,
    455,
    456,
    457,
    458,
    459,
    460,
    461,
};

// 0x5B5630
static int spell_icons[SPELL_COUNT] = {
    12,
    13,
    14,
    15,
    16,
    45,
    46,
    47,
    48,
    49,
    39,
    40,
    41,
    42,
    43,
    93,
    94,
    95,
    96,
    97,
    51,
    52,
    53,
    54,
    55,
    87,
    88,
    89,
    90,
    91,
    57,
    58,
    59,
    60,
    61,
    99,
    100,
    101,
    102,
    103,
    63,
    64,
    65,
    66,
    67,
    69,
    70,
    71,
    72,
    73,
    75,
    76,
    77,
    78,
    79,
    105,
    106,
    107,
    108,
    109,
    111,
    112,
    113,
    114,
    115,
    81,
    82,
    83,
    84,
    85,
    117,
    118,
    119,
    120,
    121,
    123,
    124,
    125,
    126,
    127,
};

// 0x5F8730
static IsoInvalidateRectFunc* dword_5F8730;

// 0x5F8734
static char* college_mastery_descriptions[COLLEGE_COUNT];

// 0x5F8774
static char* college_names[COLLEGE_COUNT];

// 0x5F87B4
static char* college_descriptions[COLLEGE_COUNT];

// 0x5F87F8
static SpellInfo spells[SPELL_COUNT];

// 0x4B1440
bool spell_init(GameInitInfo* init_info)
{
    MesFileEntry mes_file_entry;
    int index;

    dword_5F8730 = init_info->invalidate_rect_func;

    for (index = 0; index < COLLEGE_COUNT; index++) {
        mes_file_entry.num = index + FIRST_COLLEGE_NAME_ID;
        magictech_get_msg(&mes_file_entry);
        college_names[index] = mes_file_entry.str;
    }

    for (index = 0; index < SPELL_COUNT; index++) {
        mes_file_entry.num = index + FIRST_SPELL_DESCRIPTION_ID;
        magictech_get_msg(&mes_file_entry);
        spells[index].description = mes_file_entry.str;
    }

    for (index = 0; index < COLLEGE_COUNT; index++) {
        mes_file_entry.num = index + FIRST_COLLEGE_DESCRIPTION_ID;
        magictech_get_msg(&mes_file_entry);
        college_descriptions[index] = mes_file_entry.str;
    }

    for (index = 0; index < COLLEGE_COUNT; index++) {
        mes_file_entry.num = index + FIRST_COLLEGE_MASTERY_DESCRIPTION_ID;
        magictech_get_msg(&mes_file_entry);
        college_mastery_descriptions[index] = mes_file_entry.str;
    }

    return true;
}

// 0x4B1520
void spell_exit()
{
}

// 0x4B1530
void spell_set_defaults(int64_t obj)
{
    int college;

    for (college = 0; college < COLLEGE_COUNT; college++) {
        obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, college, 0);
    }

    obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, COLLEGE_COUNT, -1);
}

// 0x4B1570
int spell_get_icon(int spell)
{
    if (spell >= 0 && spell < SPELL_COUNT) {
        return spell_icons[spell];
    }

    return sub_459F90(spell) ? 36 : 35;
}

// 0x4B15A0
size_t sub_4B15A0(int spell)
{
    if (spell == 10000 || spell == -1) {
        return 0;
    } else {
        return strlen(spells[spell].field_0);
    }
}

// 0x4B1600
char* spell_get_name(int spell)
{
    if (spell == 10000 || spell == -1) {
        return "";
    } else {
        return magictech_spell_name(spell);
    }
}

// 0x4B1620
char* spell_get_description(int spell)
{
    if (spell == 10000 || spell == -1) {
        return "";
    } else {
        return spells[spell].description;
    }
}

// 0x4B1650
int sub_4B1650(int spell)
{
    (void)spell;

    return 1;
}

// 0x4B1660
int sub_4B1660(int spell, int64_t object_id)
{
    int cost;

    cost = magictech_get_cost(spell);
    if (stat_level(object_id, STAT_RACE) == RACE_DWARF) {
        cost *= 2;
    }

    if (sub_4B1CB0(object_id) == spell / 5) {
        cost /= 2;
    }

    return cost;
}

// 0x4B16C0
int sub_4B16C0(int spell, int64_t obj, int* a3)
{
    int* maintain;
    int value;

    maintain = magictech_get_maintain1(spell);

    value = maintain[0];
    if (stat_level(obj, STAT_RACE) == RACE_DWARF) {
        value *= 2;
    }

    if (a3 != NULL) {
        *a3 = maintain[1];
        if (sub_4B1CB0(obj) == spell / 5) {
            *a3 *= 2;
        }
    }

    return value;
}

// 0x4B1740
int sub_4B1740(int spell)
{
    (void)spell;

    return 100;
}

// 0x4B1750
int sub_4B1750(int spell)
{
    return sub_4502E0(spell);
}

// 0x4B1760
int spell_get_iq(int spell)
{
    return magictech_get_iq(spell);
}

// 0x4B1770
int spell_get_minimum_level(int spell)
{
    return spell_minimum_levels[spell % FIVE];
}

// 0x4B1790
bool sub_4B1790(int64_t obj, int spell, bool force)
{
    int v1;
    int v2;
    int magic_points;
    int cost;

    if (!multiplayer_is_locked() && sub_40DA20(obj)) {
        PlayerBuySpellPacket pkt;

        if ((tig_net_flags & TIG_NET_HOST) == 0) {

            pkt.type = 48;
            pkt.player = sub_4A2B10(obj);
            pkt.spell = spell;
            pkt.field_C = force;
            tig_net_send_app_all(&pkt, sizeof(pkt));
            return true;
        }

        pkt.type = 49;
        pkt.player = sub_4A2B10(obj);
        pkt.spell = spell;
        pkt.field_C = force;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    v1 = spell % 5 + 1;
    v2 = spell_college_level_get(obj, spell / 5);

    if (!force) {
        if (v1 != v2 + 1) {
            return false;
        }

        if (spell_get_minimum_level(spell) > stat_level(obj, STAT_LEVEL)) {
            return false;
        }

        if (sub_4B1750(spell) > stat_level(obj, STAT_INTELLIGENCE)) {
            return false;
        }

        if (spell_get_iq(spell) > stat_level(obj, STAT_WILLPOWER)) {
            return false;
        }

        cost = sub_4B1650(spell);
    } else  {
        cost = v1 - v2;
        if (cost < 0) {
            return true;
        }
    }

    magic_points = stat_get_base(obj, STAT_MAGICK_POINTS);
    magic_points += cost;
    stat_set_base(obj, STAT_MAGICK_POINTS, magic_points);

    sub_4B1B30(obj, spell / 5, v1);

    if (player_is_pc_obj(obj)) {
        sub_4601C0();
    }

    return true;
}

bool sub_4B1950(int64_t obj, int spell)
{
    return obj != OBJ_HANDLE_NULL
        && spell >= 0 && spell < SPELL_COUNT
        && spell % 5 + 1 <= spell_college_level_get(obj, spell / 5);
}

// 0x4B19B0
bool sub_4B19B0(int64_t obj, int spell)
{
    int cost;
    int magic_points;

    if (spell_college_level_get(obj, spell / 5) != spell % 5 + 1) {
        return false;
    }

    cost = sub_4B1650(spell);
    magic_points = stat_get_base(obj, STAT_MAGICK_POINTS);
    magic_points -= cost;
    stat_set_base(obj, STAT_MAGICK_POINTS, magic_points);

    sub_4B1B30(obj, spell / 5, spell % 5 - 1);

    return true;
}

// 0x4B1A40
char* college_get_name(int college)
{
    return college_names[college];
}

// 0x4B1A50
char* college_get_description(int college)
{
    if (sub_4B1CB0(player_get_pc_obj()) == college) {
        return college_mastery_descriptions[college];
    } else {
        return college_descriptions[college];
    }
}

// 0x4B1A80
int college_get_art_num(int college)
{
    return college_art_nums[college];
}

// 0x4B1A90
int college_get_icon(int college)
{
    if (college >= 0 && college < COLLEGE_COUNT) {
        return college_icons[college];
    } else {
        return college_icons[0];
    }
}

// 0x4B1AB0
int spell_college_level_get(int64_t obj, int college)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    return obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, college);
}

// 0x4B1B00
bool sub_4B1B00(int64_t obj, int a2)
{
    if (obj != OBJ_HANDLE_NULL) {
        return spell_college_level_get(obj, a2) > 0;
    } else {
        return false;
    }
}

// 0x4B1B30
int sub_4B1B30(int64_t obj, int a2, int a3)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        mp_obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, a2, a3);
        return a3;
    } else {
        return 0;
    }
}

// TODO: Review.
//
// 0x4B1B90
bool sub_4B1B90(int64_t obj, int intelligence)
{
    int college;
    int v1;
    int v2;

    v1 = 0;
    for (college = 0; college < COLLEGE_COUNT; college++) {
        for (v2 = 0; v2 < spell_college_level_get(obj, college); v2++) {
            if (sub_4B1750(v1 + v2) > intelligence) {
                return false;
            }
        }

        v1 += 5;
    }

    return true;
}

// TODO: Review.
//
// 0x4B1C00
bool sub_4B1C00(int64_t obj, int willpower)
{
    int college;
    int v1;
    int v2;

    v1 = 0;
    for (college = 0; college < COLLEGE_COUNT; college++) {
        for (v2 = 0; v2 < spell_college_level_get(obj, college); v2++) {
            if (spell_get_iq(v1 + v2) > willpower) {
                return false;
            }
        }

        v1 += 5;
    }

    return true;
}

// 0x4B1C70
bool sub_4B1C70(int64_t object_id, int a2)
{
    if (sub_4B1CB0(object_id) == -1) {
        return spell_college_level_get(object_id, a2) >= 5;
    } else {
        return false;
    }
}

// 0x4B1CB0
int sub_4B1CB0(int64_t object_id)
{
    int type;

    type = obj_field_int32_get(object_id, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        // TODO: Figure out constant meaning.
        return obj_arrayfield_uint32_get(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, 16);
    } else {
        return -1;
    }
}

// 0x4B1CF0
void sub_4B1CF0(int64_t obj, int a2)
{
    if (obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_PC
        || obj_field_int32_get(obj, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        if (sub_4B1C70(obj, a2)) {
            // TODO: Figure out constant meaning.
            obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, 16, a2);
        }
    }
}
