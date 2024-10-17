#include "game/sector_script_list.h"

// 0x4F6170
bool sector_script_list_init(SectorScriptList* list)
{
    return sector_script_list_reset(list);
}

// 0x4F6180
bool sector_script_list_reset(SectorScriptList* list)
{
    list->flags = 0;
    memset(&(list->scr), 0, sizeof(list->scr));
    list->flags = 0;
    list->scr.num = 0;

    return true;
}

// 0x4F61B0
bool sector_script_list_exit(SectorScriptList* list)
{
    return sector_script_list_reset(list);
}

// 0x4F61C0
bool sub_4F61C0(SectorScriptList* list, Script* scr)
{
    list->scr = *scr;
    list->flags |= 0x1;
    return true;
}

// 0x4F61F0
bool sub_4F61F0(SectorScriptList* list, Script* scr)
{
    *scr = list->scr;

    return scr->num != 0;
}

// 0x4F6220
bool sub_4F6220(SectorScriptList* list)
{
    if (list->scr.num != 0) {
        sector_script_list_reset(list);
        list->flags |= 0x1;
    }

    return true;
}

// 0x4F6250
bool sector_script_list_load(SectorScriptList* list, TigFile* stream)
{
    if (tig_file_fread(&(list->scr), sizeof(list->scr), 1, stream) != 1) {
        return false;
    }

    return true;
}

// 0x4F6270
bool sector_script_list_save(SectorScriptList* list, TigFile* stream)
{
    if (tig_file_fwrite(&(list->scr), sizeof(list->scr), 1, stream) != 1) {
        return false;
    }

    list->flags &= ~0x1;

    return true;
}

// 0x4F62A0
bool sub_4F62A0(SectorScriptList* list)
{
    return (list->flags & 0x1) != 0;
}

// 0x4F62B0
bool sector_script_list_load_with_dif(SectorScriptList* list, TigFile* stream)
{
    if (!sector_script_list_load(list, stream)) {
        return false;
    }

    list->flags |= 0x1;

    return true;
}

// 0x4F62E0
bool sector_script_list_save_with_dif(SectorScriptList* list, TigFile* stream)
{
    if (!sector_script_list_save(list, stream)) {
        return false;
    }

    list->flags |= 0x1;

    return true;
}
