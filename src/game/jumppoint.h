#ifndef ARCANUM_GAME_JUMPPOINT_H_
#define ARCANUM_GAME_JUMPPOINT_H_

#include "game/context.h"

typedef struct JumpPoint {
    int field_0;
    int field_4;
    int64_t location;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
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
