#ifndef ARCANUM_GAME_TOWNMAP_H_
#define ARCANUM_GAME_TOWNMAP_H_

#include "game/context.h"

typedef struct TownMapInfo {
    /* 0000 */ int version;
    /* 0004 */ int field_4;
    /* 0008 */ int map;
    /* 000C */ int width;
    /* 0010 */ int height;
    /* 0014 */ int num_hor_tiles;
    /* 0018 */ int num_vert_tiles;
    /* 0020 */ int64_t loc;
    /* 0028 */ float scale;
} TownMapInfo;

static_assert(sizeof(TownMapInfo) == 0x30, "wrong size");

void townmap_reset();
bool townmap_mod_load();
void townmap_mod_unload();
void townmap_flush();
int townmap_get(int64_t sec);
void townmap_set(int64_t sec, int townmap);
int townmap_count();
const char* townmap_name(int map);
bool townmap_info(int map, TownMapInfo* tmi);
void sub_4BE670(TownMapInfo* tmi, int64_t loc, int* a3, int* a4);
void sub_4BE780(TownMapInfo* tmi, int x, int y, int64_t* loc_ptr);
bool townmap_mark_visited(int64_t loc);
bool sub_4BEAB0(int map, int a2);
bool townmap_tile_blit_info(int map, int index, TigVideoBufferBlitInfo* vb_blit_info);
bool townmap_is_waitable(int map);

#endif /* ARCANUM_GAME_TOWNMAP_H_ */
