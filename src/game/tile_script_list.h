#ifndef ARCANUM_GAME_TILE_SCRIPT_LIST_H_
#define ARCANUM_GAME_TILE_SCRIPT_LIST_H_

#include "game/script.h"

typedef struct TileScriptListNode {
    /* 0000 */ unsigned int flags;
    /* 0004 */ unsigned int id;
    /* 0008 */ Scr scr;
    /* 0014 */ struct TileScriptListNode* next;
} TileScriptListNode;

static_assert(sizeof(TileScriptListNode) == 0x18, "wrong size");

typedef struct TileScriptList {
    /* 0000 */ unsigned int flags;
    /* 0004 */ TileScriptListNode* head;
} TileScriptList;

static_assert(sizeof(TileScriptList) == 0x8, "wrong size");

void sub_4F6310();
TileScriptListNode* tile_script_node_create();
void tile_script_node_destroy(TileScriptListNode* node);
bool sector_tile_script_list_init(TileScriptList* list);
bool sector_tile_script_list_reset(TileScriptList* list);
bool sector_tile_script_list_exit(TileScriptList* list);
bool sub_4F6470(TileScriptList* list, unsigned int id, Scr* scr);
bool sub_4F6520(TileScriptList* list, unsigned int id);
bool sub_4F6570(TileScriptList* list, unsigned int id, Scr* scr);
bool tile_script_list_load(TileScriptList* list, TigFile* stream);
bool tile_script_list_save(TileScriptList* list, TigFile* stream);
bool sub_4F66C0(TileScriptList* list);
bool tile_script_list_load_with_dif(TileScriptList* list, TigFile* stream);
bool tile_script_list_save_with_dif(TileScriptList* list, TigFile* stream);

#endif /* ARCANUM_GAME_TILE_SCRIPT_LIST_H_ */
