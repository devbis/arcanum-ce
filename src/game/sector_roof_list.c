#include "game/sector_roof_list.h"

static bool sector_roof_list_load_internal(SectorRoofList* list, TigFile* stream);
static bool sector_roof_list_save_internal(SectorRoofList* list, TigFile* stream);

// 0x4F7F20
bool sector_roof_list_init(SectorRoofList* list)
{
    memset(list->field_4, -1, sizeof(list->field_4));
    list->field_0 = 1;

    return true;
}

// 0x4F7F40
bool sector_roof_list_reset(SectorRoofList* list)
{
    memset(list->field_4, -1, sizeof(list->field_4));
    list->field_0 = 1;

    return true;
}

// 0x4F7F70
bool sector_roof_list_exit(SectorRoofList* list)
{
    (void)list;

    return true;
}

// 0x4F7F80
bool sector_roof_list_load(SectorRoofList* list, TigFile* stream)
{
    return sector_roof_list_load_internal(list, stream);
}

// 0x4F7FA0
bool sector_roof_list_save(SectorRoofList* list, TigFile* stream)
{
    return sector_roof_list_save_internal(list, stream);
}

// 0x4F7FC0
bool sub_4F7FC0(SectorRoofList* list)
{
    return false;
}

// 0x4F7FD0
bool sector_roof_list_load_with_dif(SectorRoofList* list, TigFile* stream)
{
    (void)list;
    (void)stream;

    return true;
}

// 0x4F7FE0
bool sector_roof_list_save_with_dif(SectorRoofList* list, TigFile* stream)
{
    (void)list;
    (void)stream;

    return true;
}

// 0x4F7FF0
void sub_4F7FF0(SectorRoofList* list)
{
    int index;

    for (index = 0; index < 256; index++) {
        sub_4D0E70(index);
    }
}

// 0x4F8020
bool sector_roof_list_load_internal(SectorRoofList* list, TigFile* stream)
{
    if (tig_file_fread(list->field_0, sizeof(list->field_0), 1, stream) != 1) {
        return false;
    }

    if (list->field_0 == 0) {
        if (tig_file_fread(list->field_4, sizeof(*list->field_4), 256, stream) != 256) {
            return false;
        }
    } else {
        memset(list->field_4, -1, sizeof(list->field_4));
        list->field_0 = 1;
    }

    return true;
}

// 0x4F8090
bool sector_roof_list_save_internal(SectorRoofList* list, TigFile* stream)
{
    if (tig_file_fwrite(list->field_0, sizeof(list->field_0), 1, stream) != 1) {
        return false;
    }

    if (list->field_0 == 0) {
        if (tig_file_write(list->field_4, sizeof(*list->field_4), 256, stream) != 256) {
            return false;
        }
    }

    return true;
}
