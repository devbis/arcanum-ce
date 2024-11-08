#include "game/sector_tile_list.h"

#include <stdio.h>

#include "game/sector.h"

static bool sector_tile_list_load_internal(SectorTileList* list, TigFile* stream);
static bool sector_tile_list_save_internal(SectorTileList* list, TigFile* stream);
static bool sector_tile_list_load_with_dif_internal(SectorTileList* list, TigFile* stream);
static bool sector_tile_list_save_with_dif_internal(SectorTileList* list, TigFile* stream);

// 0x4F7BA0
bool sector_tile_list_init(SectorTileList* list, tig_art_id_t* art_id_ptr)
{
    int index;

    for (index = 0; index < 4096; index++) {
        list->art_ids[index] = *art_id_ptr;
    }

    memset(list->difmask, 0, sizeof(list->difmask));
    list->dif = false;

    return true;
}

// 0x4F7BE0
bool sector_tile_list_reset(SectorTileList* list)
{
    // FIXME: Why 32?
    memset(list->difmask, 0, 32);
    list->dif = false;

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

    list->dif = false;

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
        sub_4D0E70(list->art_ids[index]);
    }
}

// 0x4F7CE0
bool sector_tile_list_load_internal(SectorTileList* list, TigFile* stream)
{
    if (tig_file_fread(list->art_ids, sizeof(*list->art_ids), 4096, stream) != 4096) {
        return false;
    }

    return true;
}

// 0x4F7D10
bool sector_tile_list_save_internal(SectorTileList* list, TigFile* stream)
{
    if (tig_file_fwrite(list->art_ids, sizeof(*list->art_ids), 4096, stream) != 4096) {
        return false;
    }

    return true;
}

// 0x4F7D40
bool sector_tile_list_load_with_dif_internal(SectorTileList* list, TigFile* stream)
{
    int cnt;
    int index;
    int tile;

    if (tig_file_fread(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    for (index = 0; index < cnt; index++) {
        if (tig_file_fread(&tile, sizeof(tile), 1, stream) != 1) {
            return false;
        }

        if (tig_file_fread(&(list->art_ids[tile]), sizeof(list->art_ids[tile]), 1, stream) != 1) {
            return false;
        }

        list->difmask[tile / 32] |= 1 << (tile & 31);
        list->dif = true;
    }

    return true;
}

// 0x4F7DF0
bool sector_tile_list_save_with_dif_internal(SectorTileList* list, TigFile* stream)
{
    int cnt_pos;
    int cnt;
    int tile;
    int pos;

    cnt_pos = tig_file_ftell(stream);
    if (cnt_pos == -1) {
        return false;
    }

    cnt = 0;
    if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
        return false;
    }

    for (tile = 0; tile < 4096; tile++) {
        if ((list->difmask[tile / 32] & (1 << (tile & 31))) != 0) {
            if (tig_file_fwrite(&tile, sizeof(tile), 1, stream) != 1) {
                return false;
            }

            if (tig_file_fwrite(&(list->art_ids[tile]), sizeof(list->art_ids[tile]), 1, stream) != 1) {
                return false;
            }

            cnt++;
        }
    }

    if (cnt != 0) {
        pos = tig_file_ftell(stream);
        if (pos == -1) {
            return false;
        }

        if (tig_file_fseek(stream, cnt_pos, SEEK_SET) != 0) {
            return false;
        }

        if (tig_file_fwrite(&cnt, sizeof(cnt), 1, stream) != 1) {
            return false;
        }

        if (tig_file_fseek(stream, pos, SEEK_SET) != 0) {
            return false;
        }
    }

    return true;
}
