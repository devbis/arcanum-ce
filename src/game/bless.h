#ifndef ARCANUM_GAME_BLESS_H_
#define ARCANUM_GAME_BLESS_H_

#include "game/context.h"
#include "game/obj.h"
#include "game/timeevent.h"

typedef struct BlessInfo {
    /* 0000 */ int id;
    /* 0008 */ DateTime datetime;
} BlessInfo;

static_assert(sizeof(BlessInfo) == 0x10, "wrong size");

bool bless_mod_load();
void bless_mod_unload();
void bless_copy_name(int bless, char* buffer);
void bless_copy_description(int bless, char* buffer);
int sub_4C4200(object_id_t obj, BlessInfo* blessings);
bool bless_is_added_to(object_id_t obj, int bless);
void bless_add(object_id_t obj, int bless);
int bless_get_effect(int bless);
void bless_remove(object_id_t obj, int bless);

#endif /* ARCANUM_GAME_BLESS_H_ */
