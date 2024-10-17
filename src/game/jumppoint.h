#ifndef ARCANUM_GAME_JUMPPOINT_H_
#define ARCANUM_GAME_JUMPPOINT_H_

#include "game/context.h"

typedef struct JumpPoint {
    /* 0000 */ int field_0;
    /* 0004 */ int field_4;
    /* 0008 */ int64_t location;
    /* 0010 */ int field_10;
    /* 0014 */ int field_14;
    /* 0018 */ int field_18;
    /* 001C */ int field_1C;
} JumpPoint;

// See 0x4E3800.
static_assert(sizeof(JumpPoint) == 0x20, "wrong size");

bool jumppoint_init(GameInitInfo* init_info);
void jumppoint_reset();
void jumppoint_exit();
void jumppoint_resize(ResizeInfo* resize_info);
void sub_4E3050(MapResetInfo* a1);
bool jumppoint_open(const char* a1, const char* a2);
void jumppoint_close();
bool jumppoint_flush();
bool jumppoint_update_view(ViewOptions* view_options);
void sub_4E3320(UnknownContext* a1);
bool jumppoint_find_by_location(int64_t location, JumpPoint* jumppoint);

#endif /* ARCANUM_GAME_JUMPPOINT_H_ */
