#ifndef ARCANUM_GAME_TELEPORT_H_
#define ARCANUM_GAME_TELEPORT_H_

#include "game/context.h"
#include "game/gfade.h"

#define TELEPORT_FLAG_0x00000002 0x00000002
#define TELEPORT_FLAG_0x00000020 0x00000020
#define TELEPORT_FLAG_0x80000000 0x80000000

typedef struct TeleportData {
    unsigned int flags;
    int field_4;
    long long field_8;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    FadeData fade;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
} TeleportData;

// See 0x4D3380.
static_assert(sizeof(TeleportData) == 0x60, "wrong size");

bool teleport_init(GameInitInfo* init_info);
bool teleport_reset();
void teleport_exit();
void teleport_ping();
bool sub_4D3380(TeleportData* teleport_data);
bool sub_4D3410();

#endif /* ARCANUM_GAME_TELEPORT_H_ */
