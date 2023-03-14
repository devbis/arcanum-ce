#include "game/lib/skill.h"

#include "game/lib/message.h"
#include "game/lib/stat.h"

#define FIRST_PRIMARY_SKILL_NAME_ID 0
#define FIRST_SECONDARY_SKILL_NAME_ID (FIRST_PRIMARY_SKILL_NAME_ID + PRIMARY_SKILL_COUNT)
#define FIRST_TRAINING_NAME_ID (FIRST_SECONDARY_SKILL_NAME_ID + SECONDARY_SKILL_COUNT)
#define FIRST_PRIMARY_SKILL_DESC_ID (FIRST_TRAINING_NAME_ID + TRAINING_COUNT)
#define FIRST_SECONDARY_SKILL_DESC_ID (FIRST_PRIMARY_SKILL_DESC_ID + PRIMARY_SKILL_COUNT)

// 0x5B6F04
static int primary_skill_stats[PRIMARY_SKILL_COUNT] = {
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_DEXTERITY,
    STAT_PERCEPTION,
    STAT_PERCEPTION,
    STAT_INTELLIGENCE,
    STAT_WILLPOWER,
    STAT_INTELLIGENCE,
    STAT_CHARISMA,
};

// 0x5B6F34
int secondary_skill_stats[SECONDARY_SKILL_COUNT] = {
    STAT_INTELLIGENCE,
    STAT_PERCEPTION,
    STAT_DEXTERITY,
    STAT_PERCEPTION,
};

// 0x5FF424
static char* primary_skill_descriptions[PRIMARY_SKILL_COUNT];

// 0x5FF454
static char* primary_skill_names[PRIMARY_SKILL_COUNT];

// 0x5FF484
static int skill_msg_file;

// 0x5FF488
static char* secondary_skill_descriptions[SECONDARY_SKILL_COUNT];

// 0x5FF498
static char* training_names[TRAINING_COUNT];

// 0x5FF4E8
static char* secondary_skill_names[SECONDARY_SKILL_COUNT];

// 0x6876A0
static SkillCallbacks skill_callbacks;

// 0x6876C4
static int dword_6876C4;

// 0x4C5BD0
bool skill_init(GameContext* ctx)
{
    MessageListItem message_list_item;

    // TODO: Incomplete.

    if (!message_load("mes\\skill.mes", &skill_msg_file)) {
        return false;
    }

    for (int skill = 0; skill < PRIMARY_SKILL_COUNT; skill++) {
        message_list_item.num = skill + FIRST_PRIMARY_SKILL_NAME_ID;
        sub_4D43A0(skill_msg_file, &message_list_item);
        primary_skill_names[skill] = message_list_item.text;
    }

    for (int skill = 0; skill < SECONDARY_SKILL_COUNT; skill++) {
        message_list_item.num = skill + FIRST_SECONDARY_SKILL_NAME_ID;
        sub_4D43A0(skill_msg_file, &message_list_item);
        secondary_skill_names[skill] = message_list_item.text;
    }

    for (int training = 0; training < TRAINING_COUNT; training++) {
        message_list_item.num = training + FIRST_TRAINING_NAME_ID;
        sub_4D43A0(skill_msg_file, &message_list_item);
        training_names[training] = message_list_item.text;
    }

    for (int skill = 0; skill < PRIMARY_SKILL_COUNT; skill++) {
        message_list_item.num = skill + FIRST_PRIMARY_SKILL_DESC_ID;
        sub_4D43A0(skill_msg_file, &message_list_item);
        primary_skill_descriptions[skill] = message_list_item.text;
    }

    for (int skill = 0; skill < SECONDARY_SKILL_COUNT; skill++) {
        message_list_item.num = skill + FIRST_SECONDARY_SKILL_DESC_ID;
        sub_4D43A0(skill_msg_file, &message_list_item);
        secondary_skill_descriptions[skill] = message_list_item.text;
    }
}

// 0x4C5D50
void skill_set_callbacks(SkillCallbacks* callbacks)
{
    skill_callbacks = *callbacks;
}

// 0x4C5DB0
void skill_exit()
{
    message_unload(skill_msg_file);
}

// 0x4C5DC0
bool skill_load(LoadContext* ctx)
{
    if (tig_file_fread(&dword_6876C4, sizeof(dword_6876C4), 1, ctx->stream) != 1) return false;

    return true;
}

// 0x4C5DE0
bool skill_save(TigFile* stream)
{
    if (tig_file_fwrite(&dword_6876C4, sizeof(dword_6876C4), 1, stream) != 1) return false;

    return true;
}

// 0x4C62B0
const char* primary_skill_get_name(int skill)
{
    return primary_skill_names[skill];
}

// 0x4C62C0
const char* primary_skill_get_description(int skill)
{
    return primary_skill_descriptions[skill];
}

// 0x4C62D0
int sub_4C62D0(int a1, int a2, int a3)
{
    return (a3 + 1) * (a2 + 2);
}

// 0x4C64B0
int sub_4C64B0()
{
    return 1;
}

// 0x4C64C0
int sub_4C64C0()
{
    return 1;
}

// 0x4C64D0
int primary_skill_get_stat(int skill)
{
    return primary_skill_stats[skill];
}

// 0x4C6510
int sub_4C6510()
{
    return 100;
}

// 0x4C69A0
const char* secondary_skill_get_name(int skill)
{
    return secondary_skill_names[skill];
}

// 0x4C69B0
const char* secondary_skill_get_description(int skill)
{
    return secondary_skill_descriptions[skill];
}

// 0x4C69D0
const char* training_get_name(int training)
{
    return training_names[training];
}

// 0x4C69E0
int sub_4C69E0(int a1, int a2, int a3)
{
    return (a3 + 1) * (a2 + 2);
}

// 0x4C6AF0
int sub_4C6AF0()
{
    return 1;
}

// 0x4C6B00
int sub_4C6B00()
{
    return 1;
}

// 0x4C6B10
int secondary_skill_get_stat(int skill)
{
    return secondary_skill_stats[skill];
}
