#ifndef ARCANUM_GAME_CONTEXT_H_
#define ARCANUM_GAME_CONTEXT_H_

#include "tig/file.h"
#include "tig/rect.h"

typedef enum ViewType {
    VIEW_TYPE_ISOMETRIC,
    VIEW_TYPE_TOP_DOWN,
};

typedef void(GameContextF8)(TigRect* rect);
typedef bool(GameContextFC)();

typedef struct GameContext {
    bool editor;
    int iso_window_handle;
    GameContextF8* field_8;
    GameContextFC* field_C;
};

static_assert(sizeof(GameContext) == 0x10, "wrong size");

typedef struct ResizeContext {
    int iso_window_handle;
    TigRect field_4;
    TigRect field_14;
};

static_assert(sizeof(ResizeContext) == 0x24, "wrong size");

typedef struct LoadContext {
    int version;
    TigFile* stream;
};

static_assert(sizeof(LoadContext) == 0x8, "wrong size");

typedef struct ViewOptions {
    int type;
    int zoom;
};

static_assert(sizeof(ViewOptions) == 0x8, "wrong size");

typedef struct UnknownContext {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    TigRectListNode* rects;
};

static_assert(sizeof(UnknownContext) == 0x14, "wrong size");

#endif /* ARCANUM_GAME_CONTEXT_H_ */
