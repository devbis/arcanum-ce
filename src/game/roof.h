#ifndef ARCANUM_GAME_ROOF_H_
#define ARCANUM_GAME_ROOF_H_

#include "game/context.h"

bool roof_init(GameInitInfo* init_info);
void roof_exit();
void roof_resize(ResizeInfo* resize_info);
bool sub_439100(ViewOptions* view_options);
void roof_toggle();
void roof_render(UnknownContext* render_info);
int64_t sub_4395E0(int64_t loc);
bool sub_439890(int x, int y);
void sub_439D30(int64_t loc);
void sub_439EA0(int64_t a1);
void sub_439EE0(int64_t loc);
void sub_439F20(int64_t loc);
bool sub_439FA0(int64_t loc);
bool sub_439FF0(int64_t x, int64_t y, int a3);
bool sub_43A030(int64_t loc, int a2);
void roof_blit_flags_set(unsigned int flags);
unsigned int roof_blit_flags_get();

#endif /* ARCANUM_GAME_ROOF_H_ */
