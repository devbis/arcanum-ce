#ifndef ARCANUM_GAME_SECTOR_LIST_LIST_H_
#define ARCANUM_GAME_SECTOR_LIST_LIST_H_

#include <tig/tig.h>

#include "game/sector_block_list.h"

typedef struct SectorLightList {
    SectorBlockListNode* head;
    int field_4;
} SectorLightList;

static_assert(sizeof(SectorLightList) == 0x8, "wrong size");

bool sector_light_list_init(SectorLightList* list);
bool sector_light_list_reset(SectorLightList* list);
bool sector_light_list_exit(SectorLightList* list);
bool sub_4F71C0(SectorLightList* list, int a2);
void sub_4F7200(SectorLightList* list, int a2);
bool sub_4F7250(SectorLightList* list, int a2, int a3, int a4, int a5, int a6);
bool sub_4F7710(SectorLightList* list);

#endif /* ARCANUM_GAME_SECTOR_LIST_LIST_H_ */
