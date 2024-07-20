#include "game/lib/spell.h"

#include "game/lib/message.h"
#include "game/lib/object.h"
#include "game/lib/stat.h"

#define FIRST_COLLEGE_NAME_ID 500
#define FIRST_SPELL_DESCRIPTION_ID 700
#define FIRST_COLLEGE_DESCRIPTION_ID 8000
#define FIRST_COLLEGE_MASTERY_DESCRIPTION_ID 9000

#define FIVE 5

typedef struct SpellData {
    char* field_0;
    char* description;
};

static_assert(sizeof(SpellData) == 0xB8, "wrong size");

// 0x5B5770
static int dword_5B5770[FIVE] = {
    1,
    1,
    5,
    10,
    15,
};

// 0x5B55B0
int dword_5B55B0[COLLEGE_COUNT] = {
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
int dword_5B55F0[COLLEGE_COUNT] = {
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

// 0x5F8730
static GameContextF8* dword_5F8730;

// 0x5F8734
static char* college_mastery_descriptions[COLLEGE_COUNT];

// 0x5F8774
static char* college_names[COLLEGE_COUNT];

// 0x5F87B4
static char* college_descriptions[COLLEGE_COUNT];

// 0x5F87F8
static SpellData spells[SPELL_COUNT];

// 0x4B1440
bool spell_init(GameContext* ctx)
{
    MessageListItem msg;

    dword_5F8730 = ctx->field_8;

    for (int college = 0; college < COLLEGE_COUNT; college++) {
        msg.num = college;
        sub_44FDC0(&msg);
        college_names[college] = msg.text;
    }

    for (int spell = 0; spell < SPELL_COUNT; spell++) {
        msg.num = spell;
        sub_44FDC0(&msg);
        spells[spell].description = msg.text;
    }

    for (int college = 0; college < COLLEGE_COUNT; college++) {
        msg.num = college + FIRST_COLLEGE_DESCRIPTION_ID;
        sub_44FDC0(&msg);
        college_descriptions[college] = msg.text;
    }

    for (int college = 0; college < COLLEGE_COUNT; college++) {
        msg.num = college + FIRST_COLLEGE_MASTERY_DESCRIPTION_ID;
        sub_44FDC0(&msg);
        college_mastery_descriptions[college] = msg.text;
    }

    return true;
}

// 0x4B1520
void spell_exit()
{
}

// 0x4B1530
void spell_set_defaults(long long object_id)
{
    for (int college = 0; college < COLLEGE_COUNT; college++) {
        sub_4074E0(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, college, 0);
    }

    sub_4074E0(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, COLLEGE_COUNT, -1);
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
const char* spell_get_name(int spell)
{
    if (spell == 10000 || spell == -1) {
        return "";
    } else {
        return sub_44FDE0(spell);
    }
}

// 0x4B1620
const char* spell_get_description(int spell)
{
    if (spell == 10000 || spell == -1) {
        return "";
    } else {
        return spells[spell].description;
    }
}

// 0x4B1650
int sub_4B1650()
{
    return 1;
}

// 0x4B1660
int sub_4B1660(int spell, long long object_id)
{
    int v1 = sub_450340(spell);
    if (sub_4B0490(object_id, STAT_RACE) == RACE_DWARF) {
        v1 *= 2;
    }

    if (sub_4B1CB0(object_id) == spell / 5) {
        v1 /= 2;
    }

    return v1;
}

// 0x4B1740
int sub_4B1740()
{
    return 100;
}

// 0x4B1750
int sub_4B1750()
{
    return sub_4502E0();
}

// 0x4B1760
int sub_4B1760(int a1)
{
    return sub_4502F0(a1);
}

// 0x4B1770
int sub_4B1770(int a1)
{
    return dword_5B5770[a1 % FIVE];
}

// 0x4B1A40
const char* college_get_name(int college)
{
    return college_names[college];
}

// 0x4B1A50
const char* college_get_description(int college)
{
    if (sub_4B1CB0(sub_40DA50()) == college) {
        return college_mastery_descriptions[college];
    } else {
        return college_descriptions[college];
    }
}

// 0x4B1A80
int sub_4B1A80(int college)
{
    return dword_5B55B0[college];
}

// 0x4B1A90
int sub_4B1A90(int college)
{
    if (college >= 0 && college < 16) {
        return dword_5B55F0[college];
    } else {
        return dword_5B55F0[0];
    }
}

// 0x4B1AB0
int sub_4B1AB0(long long object_id, int a2)
{
    if (obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_PC || obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        return sub_407470(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, a2);
    } else {
        return 0;
    }
}

// 0x4B1B00
bool sub_4B1B00(long long object_id, int a2)
{
    if (object_id != 0) {
        return sub_4B1AB0(object_id, a2) > 0;
    } else {
        return false;
    }
}

// 0x4B1B30
int sub_4B1B30(long long object_id, int a2, int a3)
{
    if (obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_PC || obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        object_field_set_with_network(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, a2, a3);
        return a3;
    } else {
        return 0;
    }
}

// 0x4B1C70
bool sub_4B1C70(long long object_id, int a2)
{
    if (sub_4B1CB0(object_id) == -1) {
        return sub_4B1AB0(object_id, a2) >= 5;
    } else {
        return false;
    }
}

// 0x4B1CB0
int sub_4B1CB0(long long object_id)
{
    int type = obj_field_int32_get(object_id, OBJ_F_TYPE);
    if (type == OBJ_TYPE_PC || type == OBJ_TYPE_NPC) {
        // TODO: Figure out constant meaning.
        return sub_407470(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, 16);
    } else {
        return -1;
    }
}

// 0x4B1CF0
void sub_4B1CF0(long long object_id, int a2)
{
    if (obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_PC || obj_field_int32_get(object_id, OBJ_F_TYPE) == OBJ_TYPE_NPC) {
        if (sub_4B1C70(object_id, a2)) {
            // TODO: Figure out constant meaning.
            sub_4074E0(object_id, OBJ_F_CRITTER_SPELL_TECH_IDX, 16, a2);
        }
    }
}
