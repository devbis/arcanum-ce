#include "game/description.h"

#include "game/mes.h"

// 0x6018CC
static mes_file_handle_t gamekey_mes_file;

// 0x6018D0
static mes_file_handle_t gamedesc_mes_file;

// 0x6018D4
static mes_file_handle_t description_mes_file;

// 0x6018D8
static int dword_6018D8;

// 0x4D40D0
bool description_init(GameInitInfo* init_info)
{
    int cnt;
    MesFileEntry mes_file_entry;

    (void)init_info;

    if (!mes_load("mes\\description.mes", &description_mes_file)) {
        return false;
    }

    cnt = mes_entries_count(description_mes_file);
    if (cnt != 0) {
        mes_get_entry(description_mes_file, cnt - 1, &mes_file_entry);
        dword_6018D8 = mes_file_entry.num;
    } else {
        dword_6018D8 = 0;
    }

    gamedesc_mes_file = MES_FILE_HANDLE_INVALID;

    return true;
}

// 0x4D4150
void description_exit()
{
    mes_unload(description_mes_file);
}

// 0x4D4160
bool description_mod_load()
{
    int cnt;
    MesFileEntry mes_file_entry;

    if (mes_load("mes\\gamedesc.mes", &gamedesc_mes_file)) {
        cnt = mes_entries_count(gamedesc_mes_file);
        if (cnt != 0) {
            mes_get_entry(gamedesc_mes_file, cnt - 1, &mes_file_entry);
            dword_6018D8 = mes_file_entry.num;
        }
    }

    mes_load("mes\\gamekey.mes", &gamekey_mes_file);

    return true;
}

// 0x4D41D0
void description_mod_unload()
{
    mes_unload(gamedesc_mes_file);
    gamedesc_mes_file = MES_FILE_HANDLE_INVALID;

    mes_unload(gamekey_mes_file);
    gamekey_mes_file = MES_FILE_HANDLE_INVALID;
}

// 0x4D4210
const char* description_get_name(int num)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    if (num < 0 || num > dword_6018D8) {
        return NULL;
    }

    if (num < 30000) {
        mes_file = description_mes_file;
    } else {
        mes_file = gamedesc_mes_file;
        if (mes_file == MES_FILE_HANDLE_INVALID) {
            return NULL;
        }
    }

    mes_file_entry.num = num;
    if (!mes_search(mes_file, &mes_file_entry)) {
        return NULL;
    }

    return mes_file_entry.str;
}

// 0x4D4260
const char* description_get_key_name(int num)
{
    MesFileEntry mes_file_entry;

    mes_file_entry.num = num;
    if (!mes_search(gamekey_mes_file, &mes_file_entry)) {
        return NULL;
    }

    return mes_file_entry.str;
}
