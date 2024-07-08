#include "game/lib/oname.h"

#include "game/lib/message.h"

// 0x60372C
static int o_name_oname_msg_file;

// 0x603730
static int o_name_faction_msg_file;

// 0x603734
static bool o_name_initialized;

// 0x4E72B0
bool o_name_init(GameContext* ctx)
{
    if (!message_load("oemes\\oname.mes", &o_name_oname_msg_file)) {
        return false;
    }

    if (!message_load("oemes\\faction.mes", &o_name_faction_msg_file)) {
        return false;
    }

    o_name_initialized = true;
    return true;
}

// 0x4E72F0
void o_name_exit()
{
    if (o_name_initialized) {
        message_unload(o_name_oname_msg_file);
        message_unload(o_name_faction_msg_file);
        o_name_initialized = false;
    }
}

// 0x4E7320
bool o_name_mod_load()
{
    int msg_file;

    if (message_load("oemes\\gameoname.mes", &msg_file)) {
        sub_4D46F0(o_name_oname_msg_file, msg_file);
        message_unload(msg_file);
    }

    if (message_load("oemes\\gamefaction.mes", &msg_file)) {
        sub_4D46F0(o_name_faction_msg_file, msg_file);
        message_unload(msg_file);
    }

    return true;
}

// 0x4E7390
void o_name_mod_unload()
{
    message_unload(o_name_oname_msg_file);
    if (!message_load("oemes\\oname.mes", &o_name_oname_msg_file)) {
        tig_debug_println("Error:  Can't load oemes\\oname.mes in o_name_mod_unload");
    }

    message_unload(o_name_faction_msg_file);
    if (!message_load("oemes\\faction.mes", &o_name_faction_msg_file)) {
        tig_debug_println("Error:  Can't load oemes\\faction.mes in o_name_mod_unload");
    }
}

// 0x4E73F0
int sub_4E73F0()
{
    return message_count(o_name_oname_msg_file);
}

// 0x4E7400
const char* sub_4E7400(int num)
{
    MessageListItem msg;
    msg.num = num;
    sub_4D43A0(o_name_oname_msg_file, &msg);
    return msg.text;
}
