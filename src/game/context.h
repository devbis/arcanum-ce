#ifndef ARCANUM_GAME_CONTEXT_H_
#define ARCANUM_GAME_CONTEXT_H_

#include "tig/rect.h"

typedef struct GameContext {
    bool editor;
    int iso_window_handle;
    int field_8;
    int fiedl_C;
};

static_assert(sizeof(GameContext) == 0x10, "wrong size");

typedef struct ResizeContext {
    int iso_window_handle;
    TigRect field_4;
    TigRect field_14;
};

static_assert(sizeof(ResizeContext) == 0x24, "wrong size");

#endif /* ARCANUM_GAME_CONTEXT_H_ */
