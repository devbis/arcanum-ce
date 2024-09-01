#ifndef ARCANUM_GAME_OBJECT_H_
#define ARCANUM_GAME_OBJECT_H_

#include "game/context.h"
#include "game/obj.h"

void object_resize(ResizeInfo* resize_info);
void object_reset();
void object_close();
void object_set_offset(object_id_t obj, int offset_x, int offset_y);
void object_set_current_aid(object_id_t obj, tig_art_id_t aid);
void object_set_light(object_id_t obj, unsigned int flags, tig_art_id_t aid, tig_color_t color);
void object_set_overlay_light(object_id_t obj, int index, unsigned int flags, tig_art_id_t aid, tig_color_t color);
void object_set_blit_scale(object_id_t obj, int value);
int sub_441AE0(object_id_t obj1, object_id_t obj2);
int sub_441B20(object_id_t obj1, object_id_t obj2);

#endif /* ARCANUM_GAME_OBJECT_H_ */
