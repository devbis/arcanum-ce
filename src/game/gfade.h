#ifndef ARCANUM_GAME_GFADE_H_
#define ARCANUM_GAME_GFADE_H_

#include "game/context.h"
#include "game/timeevent.h"

typedef struct FadeData {
    int field_0;
    int color;
    int steps;
    float duration;
    int field_10;
} FadeData;

static_assert(sizeof(FadeData) == 0x14, "wrong size");

int gfade_init(GameInitInfo* init_info);
void gfade_exit();
void gfade_resize(ResizeInfo* resize_info);
void sub_4BDFA0(FadeData* fade_data);
bool gfade_timeevent_process(TimeEvent* timeevent);

#endif /* ARCANUM_GAME_GFADE_H_ */
