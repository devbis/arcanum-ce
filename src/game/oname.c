#include "game/oname.h"

#include "game/mes.h"

// 0x60372C
static mes_file_handle_t o_name_oname_mes_file;

// 0x603730
static mes_file_handle_t o_name_faction_mes_file;

// 0x603734
static bool o_name_initialized;

// 0x4E72B0
bool o_name_init(GameInitInfo* init_info)
{
    (void)init_info;

    if (!mes_load("oemes\\oname.mes", &o_name_oname_mes_file)) {
        return false;
    }

    if (!mes_load("oemes\\faction.mes", &o_name_faction_mes_file)) {
        return false;
    }

    o_name_initialized = true;

    return true;
}

// 0x4E72F0
void o_name_exit()
{
    if (o_name_initialized) {
        mes_unload(o_name_oname_mes_file);
        mes_unload(o_name_faction_mes_file);
        o_name_initialized = false;
    }
}

// 0x4E7320
bool o_name_mod_load()
{
    int msg_file;

    if (mes_load("oemes\\gameoname.mes", &msg_file)) {
        mes_merge(o_name_oname_mes_file, msg_file);
        mes_unload(msg_file);
    }

    if (mes_load("oemes\\gamefaction.mes", &msg_file)) {
        mes_merge(o_name_faction_mes_file, msg_file);
        mes_unload(msg_file);
    }

    return true;
}

// 0x4E7390
void o_name_mod_unload()
{
    mes_unload(o_name_oname_mes_file);
    if (!mes_load("oemes\\oname.mes", &o_name_oname_mes_file)) {
        tig_debug_println("Error:  Can't load oemes\\oname.mes in o_name_mod_unload");
    }

    mes_unload(o_name_faction_mes_file);
    if (!mes_load("oemes\\faction.mes", &o_name_faction_mes_file)) {
        tig_debug_println("Error:  Can't load oemes\\faction.mes in o_name_mod_unload");
    }
}

// 0x4E73F0
int o_name_count()
{
    return mes_entries_count(o_name_oname_mes_file);
}

// 0x4E7400
const char* o_name_get(int oname)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = oname;
    mes_get_msg(o_name_oname_mes_file, &mes_file_entry);

    return mes_file_entry.str;
}
