#include "game/lib/curse.h"

#include "game/lib/message.h"

#define CURSE_FIELD_NAME 0
#define CURSE_FIELD_EFFECT 1
#define CURSE_FIELD_DESCRIPTION 2

static void curse_copy_field(int curse, int field, char* dest);

// 0x5FF40C
static int curse_msg_file;

// 0x4C3C60
bool curse_mod_load()
{
    message_load("mes\\gamecurse.mes", &curse_msg_file);
    return true;
}

// 0x4C3C80
void curse_mod_unload()
{
    message_unload(curse_msg_file);
    curse_msg_file = -1;
}

// 0x4C3CA0
void curse_copy_name(int curse, char* dest)
{
    curse_copy_field(curse, CURSE_FIELD_NAME, dest);
}

// 0x4C3CC0
void curse_copy_field(int curse, int field, char* dest)
{
    dest[0] = '\0';

    if (curse >= 50) {
        MessageListItem message_list_item;
        message_list_item.num = curse * 10 + field;
        if (message_find(curse_msg_file, &message_list_item)) {
            strcpy(dest, message_list_item.text);
        }
    }
}

// 0x4C3D30
void curse_copy_description(int curse, char* dest)
{
    curse_copy_field(curse, CURSE_FIELD_DESCRIPTION, dest);
}

// 0x4C3F70
int curse_get_effect(int curse)
{
    char buffer[80];
    curse_copy_field(curse, CURSE_FIELD_EFFECT, buffer);
    return atoi(buffer);
}
