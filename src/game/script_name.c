#include "game/script_name.h"

#include <stdio.h>

// 0x601750
static TigIdxTable script_name_mod_idxtable;

// 0x601760
static TigIdxTable script_name_idxtable;

// 0x601770
static bool script_name_initialized;

// 0x601774
static bool script_name_mod_loaded;

// 0x4CEC10
bool script_name_init(GameInitInfo* init_info)
{
    TigFileList file_list;
    unsigned int index;
    int num;

    (void)init_info;

    tig_idxtable_init(&script_name_idxtable, TIG_MAX_PATH);
    tig_file_list_create(&file_list, "scr\\*.scr");

    for (index = 0; index < file_list.count; index++) {
        num = atoi(file_list.entries[index].path);
        if (num >= 1000) {
            if (tig_idxtable_contains(&script_name_idxtable, num)) {
                tig_debug_printf("Error! Multiple script files numbered %.5d\n", num);
            }
            tig_idxtable_set(&script_name_idxtable, num, file_list.entries[index].path);
        }
    }

    tig_file_list_destroy(&file_list);

    script_name_initialized = true;

    return true;
}

// 0x4CECC0
void script_name_exit()
{
    tig_idxtable_exit(&script_name_idxtable);
    script_name_initialized = false;
}

// 0x4CECE0
bool script_name_mod_load()
{
    TigFileList file_list;
    unsigned int index;
    int num;

    tig_idxtable_init(&script_name_mod_idxtable, TIG_MAX_PATH);
    tig_file_list_create(&file_list, "scr\\*.scr");

    for (index = 0; index < file_list.count; index++) {
        num = atoi(file_list.entries[index].path);
        if (num >= 1 && num >= 1000) {
            if (tig_idxtable_contains(&script_name_mod_idxtable, num)) {
                tig_debug_printf("Error! Multiple script files numbered %.5d\n", num);
            }
            tig_idxtable_set(&script_name_mod_idxtable, num, file_list.entries[index].path);
        }
    }

    tig_file_list_destroy(&file_list);

    script_name_mod_loaded = true;

    return true;
}

// 0x4CEDA0
void script_name_mod_unload()
{
    tig_idxtable_exit(&script_name_mod_idxtable);
    script_name_mod_loaded = false;
}

// 0x4CEDC0
bool script_name_build_scr_name(int index, char* buffer)
{
    char path[TIG_MAX_PATH];

    if (index != 0) {
        if (script_name_initialized) {
            if (index >= 1000) {
                if (tig_idxtable_get(&script_name_idxtable, index, path)) {
                    sprintf(buffer, "scr\\%s", path);
                    return true;
                }
            }
        }

        if (script_name_mod_loaded) {
            if (index >= 1 && index < 1000) {
                if (tig_idxtable_get(&script_name_mod_idxtable, index, path)) {
                    sprintf(buffer, "scr\\%s", path);
                    return true;
                }
            }
        }
    }

    return false;
}

// 0x4CEE60
bool script_name_build_dlg_name(int index, char* buffer)
{
    if (!script_name_build_scr_name(index, buffer)) {
        return false;
    }

    size_t len = strlen(buffer);
    buffer[0] = 'd';
    buffer[1] = 'l';
    buffer[2] = 'g';
    buffer[len - 3] = 'd';
    buffer[len - 2] = 'l';
    buffer[len - 1] = 'g';

    return true;
}
