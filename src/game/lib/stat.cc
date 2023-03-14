#include "game/lib/stat.h"

#include "game/lib/message.h"

// 0x5B5194
int dword_5B5194[STAT_COUNT] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    300,
    -50,
    -9,
    1,
    0,
    1,
    -65,
    1,
    -100,
    0,
    0,
    -1000,
    0,
    0,
    0,
    0,
    0,
    20,
    0,
    0,
};

// 0x5F8644
static int stat_msg_file;

// 0x5F8610
static char* gender_names[GENDER_COUNT];

// 0x5F8618
static char* race_names[RACE_COUNT];

// 0x5F8648
static char* stat_names[STAT_COUNT];

// 0x5F86B8
static char* stat_short_names[STAT_COUNT];

// 0x4B0340
bool stat_init(GameContext* ctx)
{
    MessageListItem message_list_item;

    if (!message_load("mes\\stat.mes", &stat_msg_file)) {
        return false;
    }

    for (int stat = 0; stat < STAT_COUNT; stat++) {
        message_list_item.num = stat;
        sub_4D43A0(stat_msg_file, &message_list_item);
        stat_names[stat] = message_list_item.text;
    }

    for (int stat = 0; stat < STAT_COUNT; stat++) {
        message_list_item.num = stat + 500;
        sub_4D43A0(stat_msg_file, &message_list_item);
        stat_short_names[stat] = message_list_item.text;
    }

    for (int gender = 0; gender < GENDER_COUNT; gender++) {
        message_list_item.num = gender + 28;
        sub_4D43A0(stat_msg_file, &message_list_item);
        gender_names[gender] = message_list_item.text;
    }

    for (int race = 0; race < RACE_COUNT; race++) {
        message_list_item.num = race + 30;
        sub_4D43A0(stat_msg_file, &message_list_item);
        race_names[race] = message_list_item.text;
    }

    return true;
}

// 0x4B0440
void stat_exit()
{
    message_unload(stat_msg_file);
}

// 0x4B0F80
const char* stat_get_name(int stat)
{
    return stat_names[stat];
}

// 0x4B0F90
const char* stat_get_short_name(int stat)
{
    return stat_short_names[stat];
}

// 0x4B0FA0
const char* gender_get_name(int gender)
{
    return gender_names[gender];
}

// 0x4B0FB0
const char* race_get_name(int race)
{
    return race_names[race];
}

// 0x4B0FC0
int sub_4B0FC0(int a1, int a2, int a3)
{
    return dword_5B5194[a3];
}
