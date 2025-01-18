#ifndef ARCANUM_GAME_GFADE_H_
#define ARCANUM_GAME_GFADE_H_

#include "game/context.h"
#include "game/timeevent.h"

#define FADE_IN 0x0001

typedef struct FadeData {
    /* 0000 */ unsigned int flags;
    /* 0004 */ tig_color_t color;
    /* 0008 */ int steps;
    /* 000C */ float duration;
    /* 0010 */ int field_10;
} FadeData;

static_assert(sizeof(FadeData) == 0x14, "wrong size");

bool gfade_init(GameInitInfo* init_info);
void gfade_exit();
void gfade_resize(GameResizeInfo* resize_info);
void gfade_run(FadeData* fade_data);
bool gfade_timeevent_process(TimeEvent* timeevent);

#endif /* ARCANUM_GAME_GFADE_H_ */
