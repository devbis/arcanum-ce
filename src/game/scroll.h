#ifndef ARCANUM_GAME_SCROLL_H_
#define ARCANUM_GAME_SCROLL_H_

#include "game/context.h"
#include "game/location.h"

typedef enum ScrollDirection {
    SCROLL_DIRECTION_UP,
    SCROLL_DIRECTION_UP_RIGHT,
    SCROLL_DIRECTION_RIGHT,
    SCROLL_DIRECTION_DOWN_RIGHT,
    SCROLL_DIRECTION_DOWN,
    SCROLL_DIRECTION_DOWN_LEFT,
    SCROLL_DIRECTION_LEFT,
    SCROLL_DIRECTION_UP_LEFT,
} ScrollDirection;

typedef void(ScrollFunc)(int direction);

bool scroll_init(GameInitInfo* init_info);
void scroll_exit();
void scroll_reset();
void scroll_resize(GameResizeInfo* resize_info);
void scroll_update_view(ViewOptions* view_options);
void scroll_speed_set(int value);
int scroll_speed_get();
void scroll_start(int direction);
void scroll_stop();
void scroll_set_fps(int fps);
void scroll_set_distance(int distance);
int scroll_get_distance();
void scroll_set_center(int64_t location);
void scroll_set_scroll_func(ScrollFunc* func);

#endif /* ARCANUM_GAME_SCROLL_H_ */
