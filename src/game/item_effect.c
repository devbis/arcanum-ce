#include "game/item_effect.h"

#include "game/mes.h"

// 0x6018C0
static int dword_6018C0;

// 0x6018C4
static mes_file_handle_t game_item_effect_mes_file;

// 0x6018C8
static mes_file_handle_t item_effect_mes_file;

// 0x4D3F60
bool item_effect_init(GameInitInfo* init_info)
{
    int cnt;
    MesFileEntry mes_file_entry;

    (void)init_info;

    if (!mes_load("mes\\item_effect.mes", &item_effect_mes_file)) {
        return false;
    }

    cnt = mes_entries_count(item_effect_mes_file);
    if (cnt != 0) {
        mes_get_entry(item_effect_mes_file, cnt - 1, &mes_file_entry);
        dword_6018C0 = mes_file_entry.num;
    } else {
        dword_6018C0 = 0;
    }

    game_item_effect_mes_file = MES_FILE_HANDLE_INVALID;

    return true;
}

// 0x4D3FE0
void item_effect_exit()
{
    mes_unload(item_effect_mes_file);
}

// 0x4D3FF0
bool item_effect_mod_load()
{
    int cnt;
    MesFileEntry mes_file_entry;

    if (mes_load("mes\\gameitemeffect.mes", &game_item_effect_mes_file)) {
        cnt = mes_entries_count(game_item_effect_mes_file);
        if (cnt != 0) {
            mes_get_entry(game_item_effect_mes_file, cnt - 1, &mes_file_entry);
            dword_6018C0 = mes_file_entry.num;
        }
    }

    return true;
}

// 0x4D4050
void item_effect_mod_unload()
{
    mes_unload(game_item_effect_mes_file);
    game_item_effect_mes_file = -1;
}

// 0x4D4080
char* item_effect_get_text(int num)
{
    mes_file_handle_t mes_file;
    MesFileEntry mes_file_entry;

    if (num < 0 || num > dword_6018C0) {
        return NULL;
    }

    if (num < 30000) {
        mes_file = item_effect_mes_file;
    } else {
        mes_file = game_item_effect_mes_file;
        if (mes_file == -1) {
            return NULL;
        }
    }

    mes_file_entry.num = num;
    if (!mes_search(mes_file, &mes_file_entry)) {
        return NULL;
    }

    return mes_file_entry.str;
}
