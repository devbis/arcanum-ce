#ifndef ARCANUM_GAME_LIB_GFADE_H_
#define ARCANUM_GAME_LIB_GFADE_H_

#include "game/context.h"
#include "game/lib/timeevent.h"

typedef struct FadeData {
    int field_0;
    int color;
    int steps;
    float duration;
    int field_10;
};

static_assert(sizeof(FadeData) == 0x14, "wrong size");

int gfade_init(GameContext* ctx);
void gfade_exit();
void gfade_resize(ResizeContext* ctx);
void sub_4BDFA0(FadeData* fade_data);
bool gfade_timeevent_process(TimeEvent* timeevent);

#endif /* ARCANUM_GAME_LIB_GFADE_H_ */
