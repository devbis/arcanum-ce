#ifndef ARCANUM_GAME_TELEPORT_H_
#define ARCANUM_GAME_TELEPORT_H_

#include "game/context.h"
#include "game/gfade.h"

#define TELEPORT_FLAG_0x00000002 0x00000002
#define TELEPORT_FLAG_0x00000020 0x00000020
#define TELEPORT_FLAG_0x80000000 0x80000000

typedef struct TeleportData {
    /* 0000 */ unsigned int flags;
    /* 0000 */ int field_4;
    /* 0000 */ long long field_8;
    /* 0000 */ long long field_10;
    /* 0000 */ int map;
    /* 001C */ int movie1;
    /* 0020 */ unsigned int movie_flags1;
    /* 0024 */ int movie2;
    /* 0028 */ unsigned int movie_flags2;
    /* 002C */ FadeData fade;
    /* 0040 */ int field_40;
    /* 0044 */ tig_color_t field_44;
    /* 0048 */ int field_48;
    /* 004C */ int field_4C;
    /* 0050 */ int field_50;
    /* 0054 */ int field_54;
    /* 0058 */ int field_58;
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
