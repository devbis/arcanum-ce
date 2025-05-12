#include "game/sector_roof_list.h"

#include "game/sector.h"

static bool sector_roof_list_load_internal(SectorRoofList* list, TigFile* stream);
static bool sector_roof_list_save_internal(SectorRoofList* list, TigFile* stream);

// 0x4F7F20
bool sector_roof_list_init(SectorRoofList* list)
{
    int index;

    for (index = 0; index < SECTOR_ROOF_LIST_SIZE; index++) {
        list->art_ids[index] = TIG_ART_ID_INVALID;
    }

    list->field_0 = 1;

    return true;
}

// 0x4F7F40
bool sector_roof_list_reset(SectorRoofList* list)
{
    int index;

    for (index = 0; index < SECTOR_ROOF_LIST_SIZE; index++) {
        list->art_ids[index] = TIG_ART_ID_INVALID;
    }

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
bool sector_roof_list_is_modified(SectorRoofList* list)
{
    (void)list;

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
void sector_roof_list_precache_art(SectorRoofList* list)
{
    int index;

    for (index = 0; index < SECTOR_ROOF_LIST_SIZE; index++) {
        sector_art_cache_register(list->art_ids[index]);
    }
}

// 0x4F8020
bool sector_roof_list_load_internal(SectorRoofList* list, TigFile* stream)
{
    int index;

    if (tig_file_fread(&(list->field_0), sizeof(list->field_0), 1, stream) != 1) {
        return false;
    }

    if (list->field_0 == 0) {
        if (tig_file_fread(list->art_ids, sizeof(*list->art_ids), SECTOR_ROOF_LIST_SIZE, stream) != SECTOR_ROOF_LIST_SIZE) {
            return false;
        }
    } else {
        for (index = 0; index < SECTOR_ROOF_LIST_SIZE; index++) {
            list->art_ids[index] = TIG_ART_ID_INVALID;
        }

        list->field_0 = 1;
    }

    return true;
}

// 0x4F8090
bool sector_roof_list_save_internal(SectorRoofList* list, TigFile* stream)
{
    if (tig_file_fwrite(&(list->field_0), sizeof(list->field_0), 1, stream) != 1) {
        return false;
    }

    if (list->field_0 == 0) {
        if (tig_file_fwrite(list->art_ids, sizeof(*list->art_ids), SECTOR_ROOF_LIST_SIZE, stream) != SECTOR_ROOF_LIST_SIZE) {
            return false;
        }
    }

    return true;
}
