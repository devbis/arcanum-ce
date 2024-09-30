#ifndef ARCANUM_GAME_ANIMFX_H_
#define ARCANUM_GAME_ANIMFX_H_

#include "game/context.h"

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
} AnimFxListEntryFlags;

typedef struct AnimFxListEntry {
    tig_art_id_t eye_candy_art_id;
    unsigned int flags;
    int sound;
    tig_art_id_t light_art_id;
    int light_color;
    int projectile_speed;
} AnimFxListEntry;

static_assert(sizeof(AnimFxListEntry) == 0x18, "wrong size");

typedef enum AnimFxListFlags {
    ANIMFX_LIST_LOADED = 0x1,
} AnimFxListFlags;

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
} AnimFxList;

static_assert(sizeof(AnimFxList) == 0x2C, "wrong size");

typedef struct AnimFxNode {
    /* 0000 */ int list;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t obj;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
    /* 0020 */ int field_20;
    /* 0024 */ int field_24;
    /* 0028 */ int field_28;
    /* 002C */ int field_2C;
    /* 0030 */ int field_30;
    /* 0034 */ int field_34;
    /* 0038 */ int field_38;
    /* 003C */ int field_3C;
    /* 0040 */ int field_40;
    /* 0044 */ int field_44;
    /* 0048 */ int rotation;
    /* 004C */ int field_4C;
} AnimFxNode;

static_assert(sizeof(AnimFxNode) == 0x50, "wrong size");

extern const char* off_5B7658[];
extern int dword_5B7680[];

bool animfx_init(GameInitInfo* init_info);
void animfx_exit();
void sub_4CCD20(AnimFxList* list, AnimFxNode* node, int64_t obj, int a4, int a5);
void sub_4CCD80(AnimFxNode* node);
void animfx_remove(AnimFxList* list, int64_t obj, int index, int a4);
bool animfx_list_init(AnimFxList* list);
bool animfx_list_load(AnimFxList* list);
int animfx_list_find(AnimFxList* list);
AnimFxList* animfx_list_get(int index);
void animfx_list_exit(AnimFxList* list);
bool animfx_id_get(AnimFxList* list, int index, AnimFxListEntry** entry);

#endif /* ARCANUM_GAME_ANIMFX_H_ */
