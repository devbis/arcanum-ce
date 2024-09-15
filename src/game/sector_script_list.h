#ifndef ARCANUM_GAME_SECTOR_SCRIPT_LIST_H_
#define ARCANUM_GAME_SECTOR_SCRIPT_LIST_H_

#include <tig/tig.h>

#include "game/script.h"

typedef struct SectorScriptList {
    /* 0000 */ unsigned int flags;
    /* 0004 */ Scr scr;
} SectorScriptList;

static_assert(sizeof(SectorScriptList) == 0x10, "wrong size");

bool sector_script_list_init(SectorScriptList* list);
bool sector_script_list_reset(SectorScriptList* list);
bool sector_script_list_exit(SectorScriptList* list);
bool sub_4F61C0(SectorScriptList* list, Scr* scr);
bool sub_4F61F0(SectorScriptList* list, Scr* scr);
bool sub_4F6220(SectorScriptList* list);
bool sector_script_list_load(SectorScriptList* list, TigFile* stream);
bool sector_script_list_save(SectorScriptList* list, TigFile* stream);
bool sub_4F62A0(SectorScriptList* list);
bool sector_script_list_load_with_dif(SectorScriptList* list, TigFile* stream);
bool sector_script_list_save_with_dif(SectorScriptList* list, TigFile* stream);

#endif /* ARCANUM_GAME_SECTOR_SCRIPT_LIST_H_ */
