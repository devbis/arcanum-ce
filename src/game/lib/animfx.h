#ifndef ARCANUM_GAMELIB_ANIMFX_H_
#define ARCANUM_GAMELIB_ANIMFX_H_

#include "game/context.h"
#include "tig/art.h"

typedef enum AnimFxListEntryFlags {
    ANIMFX_LIST_ENTRY_FOREGROUND_OVERLAY = 0x01,
    ANIMFX_LIST_ENTRY_BACKGROUND_OVERLAY = 0x02,
    ANIMFX_LIST_ENTRY_UNDERLAY = 0x04,
    ANIMFX_LIST_ENTRY_TRANSLUCENCY = 0x08,
    ANIMFX_LIST_ENTRY_TINTING = 0x10,
    ANIMFX_LIST_ENTRY_ANIMATES = 0x20,
    ANIMFX_LIST_ENTRY_LOOPS = 0x40,
    ANIMFX_LIST_ENTRY_CAN_AUTOSCALE = 0x80,
    ANIMFX_LIST_ENTRY_ANY_OVERLAY = ANIMFX_LIST_ENTRY_FOREGROUND_OVERLAY | ANIMFX_LIST_ENTRY_BACKGROUND_OVERLAY,
};

typedef struct AnimFxListEntry {
    art_id_t eye_candy_art_id;
    unsigned int flags;
    int sound;
    art_id_t light_art_id;
    int light_color;
    int projectile_speed;
};

static_assert(sizeof(AnimFxListEntry) == 0x18, "wrong size");

typedef enum AnimFxListFlags {
    ANIMFX_LIST_LOADED = 0x1,
};

typedef struct AnimFxList {
    const char* path;
    unsigned int flags;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int* field_24;
    AnimFxListEntry* entries;
};

static_assert(sizeof(AnimFxList) == 0x2C, "wrong size");

bool animfx_init(GameContext* ctx);
void animfx_exit();
bool animfx_list_init(AnimFxList* anim_fx_list);
bool animfx_list_load(AnimFxList* list);
int animfx_list_find(AnimFxList* list);
AnimFxList* animfx_list_get(int index);
void animfx_list_exit(AnimFxList* list);
bool animfx_id_get(AnimFxList* list, int index, AnimFxListEntry** entry);

#endif /* ARCANUM_GAMELIB_ANIMFX_H_ */
