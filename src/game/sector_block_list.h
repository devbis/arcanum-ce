#ifndef ARCANUM_GAME_SECTOR_BLOCK_LIST_H_
#define ARCANUM_GAME_SECTOR_BLOCK_LIST_H_

#include <tig/tig.h>

typedef struct SectorBlockList {
    /* 0000 */ bool modified;
    /* 0004 */ uint32_t mask[128];
} SectorBlockList;

// See 0x4F81E0.
static_assert(sizeof(SectorBlockList) == 0x204, "wrong size");

typedef struct SectorBlockListNode {
    /* 0000 */ void* data;
    /* 0004 */ struct SectorBlockListNode* next;
} SectorBlockListNode;

// See 0x4F83B1.
static_assert(sizeof(SectorBlockListNode) == 0x8, "wrong size");

bool sector_block_list_init(SectorBlockList* list);
bool sector_block_list_reset(SectorBlockList* list);
bool sector_block_list_exit(SectorBlockList* list);
bool sector_block_list_load(SectorBlockList* list, TigFile* stream);
bool sector_block_list_save(SectorBlockList* list, TigFile* stream);
bool sector_block_list_load_with_dif(SectorBlockList* list, TigFile* stream);
bool sector_block_list_save_with_dif(SectorBlockList* list, TigFile* stream);
void sub_4F8330();
void sub_4F8340();
SectorBlockListNode* sector_block_list_node_create();
void sector_block_list_node_destroy(SectorBlockListNode* node);

#endif /* ARCANUM_GAME_SECTOR_BLOCK_LIST_H_ */
