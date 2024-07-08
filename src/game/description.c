#include "game/lib/description.h"

#include "game/lib/message.h"

// 0x6018CC
static int game_key_msg_file;

// 0x6018D0
static int game_description_msg_file;

// 0x6018D4
static int description_msg_file;

// 0x6018D8
static int dword_6018D8;

// 0x4D40D0
bool description_init(GameContext* ctx)
{
    if (!message_load("mes\\description.mes", &description_msg_file)) {
        return false;
    }

    int count = message_count(description_msg_file);
    if (count != 0) {
        MessageListItem msg;
        sub_4D4500(description_msg_file, count - 1, &msg);
        dword_6018D8 = msg.num;
    } else {
        dword_6018D8 = 0;
    }

    game_description_msg_file = -1;

    return true;
}

// 0x4D4150
void description_exit()
{
    message_unload(description_msg_file);
}

// 0x4D4160
bool description_mod_load()
{
    if (message_load("mes\\gamedesc.mes", &game_description_msg_file)) {
        int count = message_count(game_description_msg_file);
        if (count != 0) {
            MessageListItem msg;
            sub_4D4500(game_description_msg_file, count - 1, &msg);
            dword_6018D8 = msg.num;
        }
    }

    message_load("mes\\gamekey.mes", &game_key_msg_file);

    return true;
}

// 0x4D41D0
int description_mod_unload()
{
    message_unload(game_description_msg_file);
    game_description_msg_file = -1;

    message_unload(game_key_msg_file);
    game_key_msg_file = -1;
}

// 0x4D4210
const char* description_get_name(int num)
{
    if (num < 0 || num > dword_6018D8) {
        return NULL;
    }

    int msg_file;
    if (num < 30000) {
        msg_file = description_msg_file;
    } else {
        msg_file = game_description_msg_file;
        if (msg_file == -1) {
            return NULL;
        }
    }

    MessageListItem msg;
    msg.num = num;
    if (!message_find(msg_file, &msg)) {
        return NULL;
    }

    return msg.text;
}

// 0x4D4260
const char* description_get_key_name(int num)
{
    MessageListItem msg;
    msg.num = num;
    if (!message_find(game_key_msg_file, &msg)) {
        return NULL;
    }

    return msg.text;
}
