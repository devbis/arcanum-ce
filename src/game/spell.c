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

typedef struct SpellInfo {
    /* 0000 */ char* name;
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
static int spell_minimum_levels[MAX_SPELL_LEVEL] = {
    1,
    1,
    5,
    10,
    15,
};

// Small circle-shaped spell college icons (24x24).
//
// 0x5B55B0
static int spell_college_small_icons[COLLEGE_COUNT] = {
    /*         COLLEGE_CONVEYANCE */ 11,
    /*         COLLEGE_DIVINATION */ 44,
    /*                COLLEGE_AIR */ 38,
    /*              COLLEGE_EARTH */ 92,
    /*               COLLEGE_FIRE */ 50,
    /*              COLLEGE_WATER */ 86,
    /*              COLLEGE_FORCE */ 56,
    /*             COLLEGE_MENTAL */ 98,
    /*               COLLEGE_META */ 62,
    /*              COLLEGE_MORPH */ 68,
    /*             COLLEGE_NATURE */ 74,
    /*  COLLEGE_NECROMANTIC_BLACK */ 104,
    /*  COLLEGE_NECROMANTIC_WHITE */ 110,
    /*           COLLEGE_PHANTASM */ 80,
    /*          COLLEGE_SUMMONING */ 116,
    /*           COLLEGE_TEMPORAL */ 122,
};

// Large square-shaped spell college icon (64x64).
//
// 0x5B55F0
static int spell_college_large_icons[COLLEGE_COUNT] = {
    /*         COLLEGE_CONVEYANCE */ 446,
    /*         COLLEGE_DIVINATION */ 447,
    /*                COLLEGE_AIR */ 448,
    /*              COLLEGE_EARTH */ 449,
    /*               COLLEGE_FIRE */ 450,
    /*              COLLEGE_WATER */ 451,
    /*              COLLEGE_FORCE */ 452,
    /*             COLLEGE_MENTAL */ 453,
    /*               COLLEGE_META */ 454,
    /*              COLLEGE_MORPH */ 455,
    /*             COLLEGE_NATURE */ 456,
    /*  COLLEGE_NECROMANTIC_BLACK */ 457,
    /*  COLLEGE_NECROMANTIC_WHITE */ 458,
    /*           COLLEGE_PHANTASM */ 459,
    /*          COLLEGE_SUMMONING */ 460,
    /*           COLLEGE_TEMPORAL */ 461,
};

// 0x5B5630
static int spell_icons[SPELL_COUNT] = {
    /*                  SPELL_DISARM */ 12,
    /*       SPELL_UNLOCKING_CANTRIP */ 13,
    /*            SPELL_UNSEEN_FORCE */ 14,
    /*      SPELL_SPATIAL_DISTORTION */ 15,
    /*           SPELL_TELEPORTATION */ 16,
    /*         SPELL_SENSE_ALIGNMENT */ 45,
    /*            SPELL_SEE_CONTENTS */ 46,
    /*               SPELL_READ_AURA */ 47,
    /*            SPELL_SENSE_HIDDEN */ 48,
    /*           SPELL_DIVINE_MAGICK */ 49,
    /*         SPELL_VITALITY_OF_AIR */ 39,
    /*          SPELL_POISON_VAPOURS */ 40,
    /*              SPELL_CALL_WINDS */ 41,
    /*             SPELL_BODY_OF_AIR */ 42,
    /*      SPELL_CALL_AIR_ELEMENTAL */ 43,
    /*       SPELL_STRENGTH_OF_EARTH */ 93,
    /*             SPELL_STONE_THROW */ 94,
    /*           SPELL_WALL_OF_STONE */ 95,
    /*           SPELL_BODY_OF_STONE */ 96,
    /*    SPELL_CALL_EARTH_ELEMENTAL */ 97,
    /*         SPELL_AGILITY_OF_FIRE */ 51,
    /*            SPELL_WALL_OF_FIRE */ 52,
    /*               SPELL_FIREFLASH */ 53,
    /*            SPELL_BODY_OF_FIRE */ 54,
    /*     SPELL_CALL_FIRE_ELEMENTAL */ 55,
    /*         SPELL_PURITY_OF_WATER */ 87,
    /*                SPELL_CALL_FOG */ 88,
    /*           SPELL_SQUALL_OF_ICE */ 89,
    /*           SPELL_BODY_OF_WATER */ 90,
    /*    SPELL_CALL_WATER_ELEMENTAL */ 91,
    /*    SPELL_SHIELD_OF_PROTECTION */ 57,
    /*                    SPELL_JOLT */ 58,
    /*           SPELL_WALL_OF_FORCE */ 59,
    /*       SPELL_BOLT_OF_LIGHTNING */ 60,
    /*            SPELL_DISINTEGRATE */ 61,
    /*                   SPELL_CHARM */ 99,
    /*                    SPELL_STUN */ 100,
    /*              SPELL_DRAIN_WILL */ 101,
    /*               SPELL_NIGHTMARE */ 102,
    /*           SPELL_DOMINATE_WILL */ 103,
    /*           SPELL_RESIST_MAGICK */ 63,
    /*         SPELL_DISPERSE_MAGICK */ 64,
    /*          SPELL_DWEOMER_SHIELD */ 65,
    /*         SPELL_BONDS_OF_MAGICK */ 66,
    /*       SPELL_REFLECTION_SHIELD */ 67,
    /*          SPELL_HARDENED_HANDS */ 69,
    /*                  SPELL_WEAKEN */ 70,
    /*                  SPELL_SHRINK */ 71,
    /*          SPELL_FLESH_TO_STONE */ 72,
    /*               SPELL_POLYMORPH */ 73,
    /*             SPELL_CHARM_BEAST */ 75,
    /*                SPELL_ENTANGLE */ 76,
    /*           SPELL_CONTROL_BEAST */ 77,
    /*           SPELL_SUCCOUR_BEAST */ 78,
    /*              SPELL_REGENERATE */ 79,
    /*                    SPELL_HARM */ 105,
    /*          SPELL_CONJURE_SPIRIT */ 106,
    /*           SPELL_SUMMON_UNDEAD */ 107,
    /*           SPELL_CREATE_UNDEAD */ 108,
    /*             SPELL_QUENCH_LIFE */ 109,
    /*           SPELL_MINOR_HEALING */ 111,
    /*             SPELL_HALT_POISON */ 112,
    /*           SPELL_MAJOR_HEALING */ 113,
    /*               SPELL_SANCTUARY */ 114,
    /*               SPELL_RESURRECT */ 115,
    /*              SPELL_ILLUMINATE */ 81,
    /*                   SPELL_FLASH */ 82,
    /*              SPELL_BLUR_SIGHT */ 83,
    /*        SPELL_PHANTASMAL_FIEND */ 84,
    /*            SPELL_INVISIBILITY */ 85,
    /*       SPELL_PLAGUE_OF_INSECTS */ 117,
    /*         SPELL_ORCISH_CHAMPION */ 118,
    /*           SPELL_GUARDIAN_OGRE */ 119,
    /*                SPELL_HELLGATE */ 120,
    /*                SPELL_FAMILIAR */ 121,
    /*                SPELL_MAGELOCK */ 123,
    /*            SPELL_CONGEAL_TIME */ 124,
    /*                  SPELL_HASTEN */ 125,
    /*                  SPELL_STASIS */ 126,
    /*            SPELL_TEMPUS_FUGIT */ 127,
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

    obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, SPELL_MASTERY_IDX, -1);
}

// 0x4B1570
int spell_icon(int spell)
{
    if (spell >= 0 && spell < SPELL_COUNT) {
        return spell_icons[spell];
    }

    return magictech_is_magic(spell) ? 36 : 35;
}

// 0x4B15A0
size_t spell_name_length(int spell)
{
    if (spell == 10000 || spell == -1) {
        return 0;
    } else {
        return strlen(spells[spell].name);
    }
}

// 0x4B1600
char* spell_name(int spell)
{
    // 0x5B5784
    static char empty[1] = "";

    if (spell == 10000 || spell == -1) {
        return empty;
    } else {
        return magictech_spell_name(spell);
    }
}

// 0x4B1620
char* spell_description(int spell)
{
    // 0x5B5788
    static char empty[1] = "";

    if (spell == 10000 || spell == -1) {
        return empty;
    } else {
        return spells[spell].description;
    }
}

// 0x4B1650
int spell_cost(int spell)
{
    (void)spell;

    return 1;
}

// 0x4B1660
int spell_cast_cost(int spell, int64_t obj)
{
    int cost;

    cost = magictech_get_cost(spell);
    if (stat_level_get(obj, STAT_RACE) == RACE_DWARF) {
        cost *= 2;
    }

    if (spell_mastery_get(obj) == COLLEGE_FROM_SPELL(spell)) {
        cost /= 2;
    }

    return cost;
}

// 0x4B16C0
int spell_maintain_cost(int spell, int64_t obj, int* period_ptr)
{
    MagicTechMaintenanceInfo* maintenance;
    int cost;

    maintenance = magictech_get_maintenance(spell);

    cost = maintenance->cost;
    if (stat_level_get(obj, STAT_RACE) == RACE_DWARF) {
        cost *= 2;
    }

    if (period_ptr != NULL) {
        *period_ptr = maintenance->period;
        if (spell_mastery_get(obj) == COLLEGE_FROM_SPELL(spell)) {
            *period_ptr *= 2;
        }
    }

    return cost;
}

// 0x4B1740
int spell_money(int spell)
{
    (void)spell;

    return 100;
}

// 0x4B1750
int spell_min_intelligence(int spell)
{
    return magictech_min_intelligence(spell);
}

// 0x4B1760
int spell_min_willpower(int spell)
{
    return magictech_min_willpower(spell);
}

// 0x4B1770
int spell_min_level(int spell)
{
    return spell_minimum_levels[LEVEL_FROM_SPELL(spell)];
}

// 0x4B1790
bool spell_add(int64_t obj, int spell, bool force)
{
    int college;
    int new_spell_level;
    int spell_level;
    int magic_points;
    int cost;

    if (!multiplayer_is_locked() && player_is_pc_obj(obj)) {
        PlayerBuySpellPacket pkt;

        if (!tig_net_is_host()) {
            pkt.type = 48;
            pkt.player = sub_4A2B10(obj);
            pkt.spell = spell;
            pkt.force = force;
            tig_net_send_app_all(&pkt, sizeof(pkt));
            return true;
        }

        pkt.type = 49;
        pkt.player = sub_4A2B10(obj);
        pkt.spell = spell;
        pkt.force = force;
        tig_net_send_app_all(&pkt, sizeof(pkt));
    }

    college = COLLEGE_FROM_SPELL(spell);
    new_spell_level = LEVEL_FROM_SPELL(spell) + 1;
    spell_level = spell_college_level_get(obj, college);

    if (!force) {
        if (new_spell_level != spell_level + 1) {
            return false;
        }

        if (spell_min_level(spell) > stat_level_get(obj, STAT_LEVEL)) {
            return false;
        }

        if (spell_min_intelligence(spell) > stat_level_get(obj, STAT_INTELLIGENCE)) {
            return false;
        }

        if (spell_min_willpower(spell) > stat_level_get(obj, STAT_WILLPOWER)) {
            return false;
        }

        cost = spell_cost(spell);
    } else  {
        cost = new_spell_level - spell_level;
        if (cost < 0) {
            return true;
        }
    }

    magic_points = stat_base_get(obj, STAT_MAGICK_POINTS);
    magic_points += cost;
    stat_base_set(obj, STAT_MAGICK_POINTS, magic_points);

    spell_college_level_set(obj, college, new_spell_level);

    if (player_is_local_pc_obj(obj)) {
        sub_4601C0();
    }

    return true;
}

// 0x4B1950
bool spell_is_known(int64_t obj, int spell)
{
    return obj != OBJ_HANDLE_NULL
        && spell >= 0 && spell < SPELL_COUNT
        && LEVEL_FROM_SPELL(spell) + 1 <= spell_college_level_get(obj, COLLEGE_FROM_SPELL(spell));
}

// 0x4B19B0
bool spell_remove(int64_t obj, int spell)
{
    int college;
    int spell_level;
    int cost;
    int magic_points;

    college = COLLEGE_FROM_SPELL(spell);
    spell_level = LEVEL_FROM_SPELL(spell);
    if (spell_college_level_get(obj, college) != spell_level + 1) {
        return false;
    }

    cost = spell_cost(spell);
    magic_points = stat_base_get(obj, STAT_MAGICK_POINTS);
    magic_points -= cost;
    stat_base_set(obj, STAT_MAGICK_POINTS, magic_points);

    spell_college_level_set(obj, college, spell_level);

    return true;
}

// 0x4B1A40
char* spell_college_name(int college)
{
    return college_names[college];
}

// 0x4B1A50
char* spell_college_description(int college)
{
    if (spell_mastery_get(player_get_local_pc_obj()) == college) {
        return college_mastery_descriptions[college];
    } else {
        return college_descriptions[college];
    }
}

// 0x4B1A80
int spell_college_small_icon(int college)
{
    return spell_college_small_icons[college];
}

// 0x4B1A90
int spell_college_large_icon(int college)
{
    if (college >= 0 && college < COLLEGE_COUNT) {
        return spell_college_large_icons[college];
    } else {
        return spell_college_large_icons[0];
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
bool spell_college_is_known(int64_t obj, int college)
{
    if (obj != OBJ_HANDLE_NULL) {
        return spell_college_level_get(obj, college) > 0;
    } else {
        return false;
    }
}

// 0x4B1B30
int spell_college_level_set(int64_t obj, int college, int level)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return 0;
    }

    mp_obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, college, level);

    return level;
}

// 0x4B1B90
bool spell_check_intelligence(int64_t obj, int intelligence)
{
    int college;
    int cnt;
    int level;
    int spell;

    for (college = 0; college < COLLEGE_COUNT; college++) {
        cnt = spell_college_level_get(obj, college);
        for (level = 0; level < cnt; level++) {
            spell = college * 5 + level;
            if (spell_min_intelligence(spell) > intelligence) {
                return false;
            }
        }
    }

    return true;
}

// 0x4B1C00
bool spell_check_willpower(int64_t obj, int willpower)
{
    int college;
    int cnt;
    int level;
    int spell;

    for (college = 0; college < COLLEGE_COUNT; college++) {
        cnt = spell_college_level_get(obj, college);
        for (level = 0; level < cnt; level++) {
            spell = college * 5 + level;
            if (spell_min_willpower(spell) > willpower) {
                return false;
            }
        }
    }

    return true;
}

// 0x4B1C70
bool spell_can_become_master_of_college(int64_t obj, int college)
{
    if (spell_mastery_get(obj) == -1) {
        return false;
    }

    if (spell_college_level_get(obj, college) < 5) {
        return false;
    }

    return true;
}

// 0x4B1CB0
int spell_mastery_get(int64_t obj)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return -1;
    }

    return obj_arrayfield_uint32_get(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, SPELL_MASTERY_IDX);
}

// 0x4B1CF0
void spell_mastery_set(int64_t obj, int college)
{
    if (!obj_type_is_critter(obj_field_int32_get(obj, OBJ_F_TYPE))) {
        return;
    }

    if (!spell_can_become_master_of_college(obj, college)) {
        return;
    }

    obj_arrayfield_uint32_set(obj, OBJ_F_CRITTER_SPELL_TECH_IDX, SPELL_MASTERY_IDX, college);
}
