#ifndef ARCANUM_GAME_WP_H_
#define ARCANUM_GAME_WP_H_

#include "game/context.h"

bool wp_init(GameInitInfo* init_info);
void wp_exit();
void wp_resize(GameResizeInfo* resize_info);
bool wp_update_view(ViewOptions* view_options);
bool wp_is_enabled();
void wp_toggle();
void wp_render(UnknownContext* render_info);

#endif /* ARCANUM_GAME_WP_H_ */
