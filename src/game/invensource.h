#ifndef ARCANUM_GAME_INVENSOURCE_H_
#define ARCANUM_GAME_INVENSOURCE_H_

#include "game/context.h"

typedef struct InvenSourceSet {
    unsigned char dummy[0xE6C];
} InvenSourceSet;

static_assert(sizeof(InvenSourceSet) == 0xE6C, "wrong size");

bool inven_source_init(GameInitInfo* init_info);
void invensource_exit();
void invensource_get_id_list(int id, InvenSourceSet* set);

#endif /* ARCANUM_GAME_INVENSOURCE_H_ */
