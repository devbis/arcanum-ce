#include "game/lib/spell.h"

#include "game/lib/message.h"

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
