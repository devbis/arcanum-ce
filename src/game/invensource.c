#include "game/invensource.h"

#include "game/mes.h"

static bool parse_set_data(mes_file_handle_t invensource_mes_file, mes_file_handle_t invensourcebuy_mes_file);
static void show_error(const char* msg);

// 0x5B64A8
static const char* off_5B64A8 = "Rules\\InvenSource.mes";

// 0x5B64AC
static const char* off_5B64AC = "Rules\\InvenSourceBuy.mes";

// 0x5FC520
static InvenSourceSet* invensource_sets;

// 0x5FC524
static int invensource_num_sets;

// 0x5FC528
static bool invensource_editor;

// 0x5FC52C
static char invensource_error[256];

// 0x5FC62C
static bool dword_5FC62C;

// 0x5FC630
static bool invensource_initialized;

// 0x5FC634
static bool invensource_have_buy;

// 0x4BF390
bool invensource_init(GameInitInfo* init_info)
{
    mes_file_handle_t invensource_mes_file;
    mes_file_handle_t invensourcebuy_mes_file;

    if (!invensource_initialized) {
        invensource_editor = init_info->editor;

        if (!mes_load(off_5B64A8, &invensource_mes_file)) {
            sprintf(invensource_error, "Can't load message file [%s].", off_5B64A8);
            show_error(invensource_error);
            return false;
        }

        if (mes_load(off_5B64AC, &invensourcebuy_mes_file)) {
            invensource_have_buy = true;
        } else {
            invensource_have_buy = false;
            sprintf(invensource_error, "Can't load message file [%s].", off_5B64AC);
            show_error(invensource_error);
        }

        invensource_num_sets = mes_entries_count(invensource_mes_file);
        if (invensource_num_sets < 1) {
            sprintf(invensource_error, "No sets to parse in [%s].", off_5B64A8);
            mes_unload(invensource_mes_file);
            mes_unload(invensourcebuy_mes_file);
            return false;
        }

        invensource_sets = MALLOC(sizeof(*invensource_sets) * invensource_num_sets);
        if (!parse_set_data(invensource_mes_file, invensourcebuy_mes_file)) {
            // FIXME: Leaking `invensource_sets`.
            mes_unload(invensource_mes_file);
            mes_unload(invensourcebuy_mes_file);
            return false;
        }

        mes_unload(invensource_mes_file);
        mes_unload(invensourcebuy_mes_file);
        invensource_have_buy = false;

        invensource_initialized = true;
    }

    return true;
}

// 0x4BF510
void invensource_exit()
{
    if (invensource_initialized) {
        FREE(invensource_sets);
        invensource_initialized = false;
    }
}

// 0x4BF5D0
void invensource_get_id_list(int id, InvenSourceSet* set)
{
    if (invensource_initialized) {
        if (id > 0 && id < invensource_num_sets) {
            *set = invensource_sets[id - 1];
            return;
        }

        tig_debug_printf("Range error in invensource_get_id_list, set: %d\n", id);
    }

    set->field_4 = 0;
    set->field_8 = 0;
}

// 0x4BF640
bool parse_set_data(mes_file_handle_t invensource_mes_file, mes_file_handle_t invensourcebuy_mes_file)
{
    // TODO: Incomplete.
}

// 0x4BF7B0
void sub_4BF7B0()
{
    // TODO: Incomplete.
}

// 0x4BFAA0
void sub_4BFAA0()
{
    // TODO: Incomplete.
}

// 0x4BFCA0
void show_error(const char* msg)
{
    tig_debug_println(msg);
    if (!dword_5FC62C && invensource_editor) {
        MessageBoxA(NULL, msg, "Error", MB_ICONHAND | MB_ICONQUESTION);
    }
}
