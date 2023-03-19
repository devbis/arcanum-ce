#include "game/lib/script_name.h"

#include <stdio.h>

#include "tig/debug.h"
#include "tig/file.h"
#include "tig/list.h"

// 0x601750
static TigList stru_601750;

// 0x601760
static TigList stru_601760;

// 0x601770
static bool dword_601770;

// 0x601774
static bool dword_601774;

// 0x4CEC10
bool script_name_init(GameContext* ctx)
{
    tig_list_init(&stru_601760, MAX_PATH);

    TigFileList file_list;
    tig_file_list_create(&file_list, "scr\\*.scr");

    for (int index = 0; index < file_list.count; index++) {
        int num = atoi(file_list.entries[index].path);
        if (num >= 1000) {
            if (tig_list_has_value(&stru_601760, num)) {
                tig_debug_printf("Error! Multiple script files numbered %.5d\n", num);
            }
            tig_list_set_value(&stru_601760, num, file_list.entries[index].path);
        }
    }

    tig_file_list_destroy(&file_list);

    dword_601770 = true;

    return true;
}

// 0x4CECC0
void script_name_exit()
{
    tig_list_exit(&stru_601760);
    dword_601770 = false;
}

// 0x4CECE0
bool script_name_mod_load()
{
    tig_list_init(&stru_601750, MAX_PATH);

    TigFileList file_list;
    tig_file_list_create(&file_list, "scr\\*.scr");

    for (int index = 0; index < file_list.count; index++) {
        int num = atoi(file_list.entries[index].path);
        if (num >= 1000) {
            if (tig_list_has_value(&stru_601750, num)) {
                tig_debug_printf("Error! Multiple script files numbered %.5d\n", num);
            }
            tig_list_set_value(&stru_601750, num, file_list.entries[index].path);
        }
    }

    tig_file_list_destroy(&file_list);

    dword_601774 = true;

    return true;
}

// 0x4CEDA0
void script_name_mod_unload()
{
    tig_list_exit(&stru_601750);
    dword_601774 = false;
}

// 0x4CEDC0
bool script_name_build_scr_name(int index, char* buffer)
{
    char path[MAX_PATH];

    if (index != 0) {
        if (dword_601770) {
            if (index < 1000) {
                if (tig_list_get_value(&stru_601760, index, path)) {
                    sprintf(buffer, "scr\\%s", path);
                    return true;
                }
            }
        }

        if (dword_601774) {
            if (index >= 1 && index < 1000) {
                if (tig_list_get_value(&stru_601750, index, path)) {
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
