#include "game/sector_tile_list.h"

static bool sector_tile_list_load_internal(SectorTileList* list, TigFile* stream);
static bool sector_tile_list_save_internal(SectorTileList* list, TigFile* stream);
static bool sector_tile_list_load_with_dif_internal(SectorTileList* list, TigFile* stream);
static bool sector_tile_list_save_with_dif_internal(SectorTileList* list, TigFile* stream);

// 0x4F7BA0
bool sector_tile_list_init(SectorTileList* list, int* a2)
{
    int index;

    for (index = 0; index < 4096; index++) {
        list->field_0[index] = *a2;
    }

    memset(list->field_4000, 0, sizeof(list->field_4000));
    list->field_4200 = 0;

    return true;
}

// 0x4F7BE0
bool sector_tile_list_reset(SectorTileList* list)
{
    // FIXME: Why 32?
    memset(list->field_4000, 0, 32);
    list->field_4200 = 0;

    return true;
}

// 0x4F7C20
bool sector_tile_list_exit(SectorTileList* list)
{
    (void)list;

    return true;
}

// 0x4F7C30
bool sector_tile_list_load(SectorTileList* list, TigFile* stream)
{
    return sector_tile_list_load_internal(list, stream);
}

// 0x4F7C50
bool sector_tile_list_save(SectorTileList* list, TigFile* stream)
{
    if (!sector_tile_list_save_internal(list, stream)) {
        return false;
    }

    list->field_4200 = 0;

    return true;
}

// 0x4F7C80
bool sector_tile_list_load_with_dif(SectorTileList* list, TigFile* stream)
{
    return sector_tile_list_load_with_dif_internal(list, stream);
}

// 0x4F7CA0
bool sector_tile_list_save_with_dif(SectorTileList* list, TigFile* stream)
{
    return sector_tile_list_save_with_dif_internal(list, stream);
}

// 0x4F7CC0
void sub_4F7CC0(SectorTileList* list)
{
    int index;

    for (index = 0; index < 4096; index++) {
        sub_4D0E70(list->field_0[index]);
    }
}

// 0x4F7CE0
bool sector_tile_list_load_internal(SectorTileList* list, TigFile* stream)
{
    if (tig_file_fread(list->field_0, sizeof(*list->field_0), 4096, stream) != 4096) {
        return false;
    }

    return true;
}

// 0x4F7D10
bool sector_tile_list_save_internal(SectorTileList* list, TigFile* stream)
{
    if (tig_file_fwrite(list->field_0, sizeof(*list->field_0), 4096, stream) != 4096) {
        return false;
    }

    return true;
}

// 0x4F7D40
bool sector_tile_list_load_with_dif_internal(SectorTileList* list, TigFile* stream)
{
    // TODO: Incomplete.
}

// 0x4F7DF0
bool sector_tile_list_save_with_dif_internal(SectorTileList* list, TigFile* stream)
{
    // TODO: Incomplete.
}
