#include "game/lib/bless.h"

#include "game/lib/message.h"

#define BLESS_FIELD_NAME 0
#define BLESS_FIELD_EFFECT 1
#define BLESS_FIELD_DESCRIPTION 2

static void bless_copy_field(int bless, int field, char* dest);

// 0x5FF410
static int bless_msg_file;

// 0x4C4110
bool bless_mod_load()
{
    message_load("mes\\gamebless.mes", &bless_msg_file);
    return true;
}

// 0x4C4130
void bless_mod_unload()
{
    message_unload(bless_msg_file);
    bless_msg_file = -1;
}

// 0x4C4150
void bless_copy_name(int bless, char* dest)
{
    bless_copy_field(bless, BLESS_FIELD_NAME, dest);
}

// 0x4C4170
void bless_copy_field(int bless, int field, char* dest)
{
    dest[0] = '\0';

    if (bless >= 50) {
        MessageListItem message_list_item;
        message_list_item.num = bless * 10 + field;
        if (message_find(bless_msg_file, &message_list_item)) {
            strcpy(dest, message_list_item.text);
        }
    }
}

// 0x4C41E0
void bless_copy_description(int bless, char* dest)
{
    bless_copy_field(bless, BLESS_FIELD_DESCRIPTION, dest);
}

// 0x4C4420
int bless_get_effect(int bless)
{
    char buffer[80];
    bless_copy_field(bless, BLESS_FIELD_EFFECT, buffer);
    return atoi(buffer);
}
