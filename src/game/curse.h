#ifndef ARCANUM_GAME_CURSE_H_
#define ARCANUM_GAME_CURSE_H_

#include "game/context.h"
#include "game/obj.h"

typedef struct CurseInfo {
    /* 0000 */ int id;
    /* 0008 */ int64_t ts;
} CurseInfo;

static_assert(sizeof(CurseInfo) == 0x10, "wrong size");

bool curse_mod_load();
void curse_mod_unload();
void curse_copy_name(int curse, char* buffer);
void curse_copy_description(int curse, char* buffer);
int sub_4C3D50(object_id_t obj, CurseInfo* curses);
bool curse_is_added_to(object_id_t obj, int curse);
void curse_add(object_id_t obj, int curse);
int curse_get_effect(int curse);

#endif /* ARCANUM_GAME_CURSE_H_ */
