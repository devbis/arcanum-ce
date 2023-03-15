#ifndef ARCANUM_GAME_LIB_GFADE_H_
#define ARCANUM_GAME_LIB_GFADE_H_

#include "game/context.h"

typedef struct FadeData {
    int field_0;
    int field_4;
    int field_8;
    float duration;
    int field_10;
};

static_assert(sizeof(FadeData) == 0x14, "wrong size");

int gfade_init(GameContext* ctx);
void gfade_exit();
void gfade_resize(ResizeContext* ctx);
void sub_4BDFA0(FadeData* fade_data);
void sub_4BE050();

#endif /* ARCANUM_GAME_LIB_GFADE_H_ */
