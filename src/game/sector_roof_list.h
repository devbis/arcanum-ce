#ifndef ARCANUM_GAME_SECTOR_ROOF_LIST_H_
#define ARCANUM_GAME_SECTOR_ROOF_LIST_H_

#include <tig/tig.h>

typedef struct SectorRoofList {
    int field_0;
    int field_4[256];
} SectorRoofList;

// See 0x4F7F20.
static_assert(sizeof(SectorRoofList) == 0x404, "wrong size");

bool sector_roof_list_init(SectorRoofList* list);
bool sector_roof_list_reset(SectorRoofList* list);
bool sector_roof_list_exit(SectorRoofList* list);
bool sector_roof_list_load(SectorRoofList* list, TigFile* stream);
bool sector_roof_list_save(SectorRoofList* list, TigFile* stream);
bool sub_4F7FC0(SectorRoofList* list);
bool sector_roof_list_load_with_dif(SectorRoofList* list, TigFile* stream);
bool sector_roof_list_save_with_dif(SectorRoofList* list, TigFile* stream);
void sub_4F7FF0(SectorRoofList* list);

#endif /* ARCANUM_GAME_SECTOR_ROOF_LIST_H_ */
