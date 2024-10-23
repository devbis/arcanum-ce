#ifndef ARCANUM_GAME_TELEPORT_H_
#define ARCANUM_GAME_TELEPORT_H_

#include "game/context.h"
#include "game/gfade.h"

typedef enum TeleportFlags {
    TELEPORT_MOVIE1 = 0x0001,
    TELEPORT_FADE_OUT = 0x0002,
    TELEPORT_FADE_IN = 0x0004,
    TELEPORT_TIME = 0x0008,
    TELEPORT_SOUND = 0x0010,
    TELEPORT_0x0020 = 0x0020,
    TELEPORT_MOVIE2 = 0x0040,
    TELEPORT_0x0100 = 0x0100,
    TELEPORT_0x80000000 = 0x80000000,
} TeleportFlags;

typedef struct TeleportData {
    /* 0000 */ unsigned int flags;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t obj;
    /* 0010 */ int64_t loc;
    /* 0018 */ int map;
    /* 001C */ int movie1;
    /* 0020 */ unsigned int movie_flags1;
    /* 0024 */ int movie2;
    /* 0028 */ unsigned int movie_flags2;
    /* 002C */ FadeData fade_out;
    /* 0040 */ FadeData fade_in;
    /* 0054 */ int sound_id;
    /* 0058 */ int time;
    /* 005C */ int field_5C;
} TeleportData;

// See 0x4D3380.
static_assert(sizeof(TeleportData) == 0x60, "wrong size");

bool teleport_init(GameInitInfo* init_info);
bool teleport_reset();
void teleport_exit();
void teleport_ping();
bool sub_4D3380(TeleportData* teleport_data);
bool sub_4D3410();
bool sub_4D3420(int64_t obj);

#endif /* ARCANUM_GAME_TELEPORT_H_ */
