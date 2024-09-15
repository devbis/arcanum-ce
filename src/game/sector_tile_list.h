#ifndef ARCANUM_GAME_SECTOR_TILE_LIST_H_
#define ARCANUM_GAME_SECTOR_TILE_LIST_H_

#include <tig/tig.h>

typedef struct SectorTileList {
    int field_0[4096];
    uint8_t field_4000[512];
    uint8_t field_4200;
} SectorTileList;

static_assert(sizeof(SectorTileList) == 0x4204, "wrong size");

bool sector_tile_list_init(SectorTileList* list, int* a2);
bool sector_tile_list_reset(SectorTileList* list);
bool sector_tile_list_exit(SectorTileList* list);
bool sector_tile_list_load(SectorTileList* list, TigFile* stream);
bool sector_tile_list_save(SectorTileList* list, TigFile* stream);
bool sector_tile_list_load_with_dif(SectorTileList* list, TigFile* stream);
bool sector_tile_list_save_with_dif(SectorTileList* list, TigFile* stream);
void sub_4F7CC0(SectorTileList* list);

#endif /* ARCANUM_GAME_SECTOR_TILE_LIST_H_ */
