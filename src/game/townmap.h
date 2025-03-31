#ifndef ARCANUM_GAME_TOWNMAP_H_
#define ARCANUM_GAME_TOWNMAP_H_

#include "game/context.h"

typedef struct TownMapInfo {
    /* 0000 */ int version;
    /* 0004 */ int field_4;
    /* 0008 */ int map;
    /* 000C */ int field_C;
    /* 0010 */ int field_10;
    /* 0014 */ int width;
    /* 0018 */ int height;
    /* 001C */ int field_1C;
    /* 0020 */ int64_t loc;
    /* 0028 */ float scale;
    /* 002C */ int field_2C;
} TownMapInfo;

static_assert(sizeof(TownMapInfo) == 0x30, "wrong size");

void townmap_reset();
bool townmap_mod_load();
void townmap_mod_unload();
void townmap_flush();
int sub_4BE380(int64_t sector_id);
int townmap_count();
const char* townmap_name(int map);
bool townmap_info(int map, TownMapInfo* tmi);
void sub_4BE670(TownMapInfo* tmi, int64_t loc, int* a3, int* a4);
void sub_4BE780(TownMapInfo* tmi, int x, int y, int64_t* loc_ptr);
bool sub_4BE8F0(int64_t loc);
bool sub_4BEAB0(int map, int a2);
bool sub_4BEAF0(int map, int index, TigVideoBufferBlitInfo* vb_blit_info);
bool sub_4BECC0(int map);

#endif /* ARCANUM_GAME_TOWNMAP_H_ */
