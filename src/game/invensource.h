#ifndef ARCANUM_GAME_INVENSOURCE_H_
#define ARCANUM_GAME_INVENSOURCE_H_

#include "game/context.h"

#define INVEN_SOURCE_SET_SIZE 300

typedef struct InvenSourceSet {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int field_8;
    /* 000C */ int default_rate;
    /* 0010 */ int default_basic_prototype;
    /* 0014 */ int rate[INVEN_SOURCE_SET_SIZE];
    /* 04C4 */ int basic_prototype[INVEN_SOURCE_SET_SIZE];
    /* 0974 */ int buy_basic_prototype[INVEN_SOURCE_SET_SIZE];
} InvenSourceSet;

static_assert(sizeof(InvenSourceSet) == 0xE24, "wrong size");

bool inven_source_init(GameInitInfo* init_info);
void invensource_exit();
void invensource_get_id_list(int id, InvenSourceSet* set);

#endif /* ARCANUM_GAME_INVENSOURCE_H_ */
