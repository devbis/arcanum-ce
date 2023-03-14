#include "game/lib/tech.h"

#include "game/lib/message.h"

// 0x5B5124
int dword_5B5124[8] = {
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
};

// 0x5B5164
int dword_5B5164[8] = {
    0,
    5,
    8,
    11,
    13,
    15,
    17,
    19,
};

// 0x5F84A8
static char* degree_names[DEGREE_COUNT];

// 0x5F84C8
static char* degree_descriptions[TECH_COUNT][DEGREE_COUNT];

// 0x5F85C8
static int tech_msg_file;

// 0x5F85CC
static char* tech_descriptions[TECH_COUNT];

// 0x5F85EC
static char* tech_names[TECH_COUNT];

// 0x4AFCD0
bool tech_init(GameContext* ctx)
{
    MessageListItem message_list_item;

    if (!message_load("mes\\tech.mes", &tech_msg_file)) {
        return false;
    }

    for (int tech = 0; tech < TECH_COUNT; tech++) {
        message_list_item.num = tech;
        sub_4D43A0(tech_msg_file, &message_list_item);
        tech_names[tech] = message_list_item.text;
    }

    for (int degree = 0; degree < DEGREE_COUNT; degree++) {
        message_list_item.num = degree + 8;
        sub_4D43A0(tech_msg_file, &message_list_item);
        degree_names[degree] = message_list_item.text;
    }

    for (int tech = 0; tech < TECH_COUNT; tech++) {
        message_list_item.num = tech + 16;
        sub_4D43A0(tech_msg_file, &message_list_item);
        tech_descriptions[tech] = message_list_item.text;
    }

    // NOTE: Original code is slightly different. It does not use nested loop,
    // instead it simply runs 64 iterations, implying `degree_descriptions`
    // is one-dimensional array. Making it two-dimensional slightly increase
    // code readability.
    for (int tech = 0; tech < TECH_COUNT; tech++) {
        for (int degree = 0; degree < DEGREE_COUNT; degree++) {
            message_list_item.num = tech * DEGREE_COUNT + degree + 24;
            sub_4D43A0(tech_msg_file, &message_list_item);
            degree_descriptions[tech][degree] = message_list_item.text;
        }
    }

    return true;
}

// 0x4AFDD0
void tech_exit()
{
    message_unload(tech_msg_file);
}

// 0x4AFE10
const char* tech_get_name(int tech)
{
    return tech_names[tech];
}

// 0x4AFE20
const char* tech_get_description(int tech)
{
    return tech_descriptions[tech];
}

// 0x4AFE30
const char* degree_get_name(int degree)
{
    return degree_names[degree];
}

// 0x4AFE40
const char* degree_get_description(int degree, int tech)
{
    return degree_descriptions[tech][degree];
}

// 0x4B00A0
int sub_4B00A0(int a1)
{
    return dword_5B5124[a1];
}

// 0x4B0110
int sub_4B0110(int a1)
{
    return dword_5B5164[a1];
}
