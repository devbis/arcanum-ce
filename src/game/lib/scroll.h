#ifndef ARCANUM_GAME_LIB_SCROLL_H_
#define ARCANUM_GAME_LIB_SCROLL_H_

#include "game/context.h"

typedef enum ScrollDirection {
    SCROLL_DIRECTION_UP,
    SCROLL_DIRECTION_UP_RIGHT,
    SCROLL_DIRECTION_RIGHT,
    SCROLL_DIRECTION_DOWN_RIGHT,
    SCROLL_DIRECTION_DOWN,
    SCROLL_DIRECTION_DOWN_LEFT,
    SCROLL_DIRECTION_LEFT,
    SCROLL_DIRECTION_UP_LEFT,
};

bool scroll_init(GameContext* ctx);
void scroll_exit();
void scroll_reset();
void scroll_resize(ResizeContext* ctx);
bool scroll_update_view(ViewOptions* view_options);
void scroll_start_scrolling_in_direction(int direction);
void scroll_stop_scrolling();
void scroll_set_fps(int fps);
void scroll_set_distance(int distance);

#endif /* ARCANUM_GAME_LIB_SCROLL_H_ */
