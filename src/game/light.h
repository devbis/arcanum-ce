#ifndef ARCANUM_GAMELIB_LIGHT_H_
#define ARCANUM_GAMELIB_LIGHT_H_

#include "game/context.h"
#include "game/lib/object.h"

#define LIGHT_HANDLE_INVALID 0
#define SHADOW_HANDLE_INVALID 0

typedef unsigned int light_handle_t;
typedef unsigned int shadow_handle_t;

bool light_init(GameContext* ctx);
void light_exit();
void light_resize(ResizeContext* ctx);
bool light_update_view(ViewOptions* view_options);
void light_build_color(uint8_t red, uint8_t green, uint8_t blue, int* color);
void light_get_color_components(int color, uint8_t* red, uint8_t* green, uint8_t* blue);

#endif /* ARCANUM_GAMELIB_LIGHT_H_ */
