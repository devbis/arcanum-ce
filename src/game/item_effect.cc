#include "game/lib/item_effect.h"

#include "game/lib/message.h"

// 0x6018C0
static int dword_6018C0;

// 0x6018C4
static int game_item_effect_msg_file;

// 0x6018C8
static int item_effect_msg_file;

// 0x4D3F60
bool item_effect_init(GameContext* ctx)
{
    if (!message_load("mes\\item_effect.mes", &item_effect_msg_file)) {
        return false;
    }

    int count = message_count(item_effect_msg_file);
    if (count != 0) {
        MessageListItem msg;
        sub_4D4500(item_effect_msg_file, count - 1, &msg);
        dword_6018C0 = msg.num;
    } else {
        dword_6018C0 = 0;
    }

    game_item_effect_msg_file = -1;

    return true;
}

// 0x4D3FE0
void item_effect_exit()
{
    message_unload(item_effect_msg_file);
}

// 0x4D3FF0
bool item_effect_mod_load()
{
    if (message_load("mes\\gameitemeffect.mes", &game_item_effect_msg_file)) {
        int count = message_count(game_item_effect_msg_file);
        if (count != 0) {
            MessageListItem msg;
            sub_4D4500(game_item_effect_msg_file, count - 1, &msg);
            dword_6018C0 = msg.num;
        }
    }

    return true;
}

// 0x4D4050
void item_effect_mod_unload()
{
    message_unload(game_item_effect_msg_file);
    game_item_effect_msg_file = -1;
}

// 0x4D4080
const char* item_effect_get_text(int num)
{
    if (num < 0 || num > dword_6018C0) {
        return NULL;
    }

    int msg_file;
    if (num < 30000) {
        msg_file = item_effect_msg_file;
    } else {
        msg_file = game_item_effect_msg_file;
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
