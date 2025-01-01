#ifndef ARCANUM_GAME_TF_H_
#define ARCANUM_GAME_TF_H_

#include "game/context.h"

typedef enum TextFloaterType {
    TF_TYPE_WHITE,
    TF_TYPE_RED,
    TF_TYPE_GREEN,
    TF_TYPE_BLUE,
    TF_TYPE_YELLOW,
    TF_TYPE_COUNT,
} TextFloaterType;

bool tf_init(GameInitInfo* init_info);
void tf_resize(ResizeInfo* resize_info);
void tf_reset();
void tf_exit();
bool tf_update_view(ViewOptions* view_options);
void tf_map_close();
int tf_level_set(int value);
int tf_level_get();
void tf_ping(tig_timestamp_t timestamp);
void tf_render(UnknownContext *render_info);
void tf_add(int64_t obj, int type, const char* str);
void tf_move(int64_t obj, int64_t loc, int offset_x, int offset_y);
void sub_4D5620(int64_t obj);
void tf_remove(int64_t obj);

#endif /* ARCANUM_GAME_TF_H_ */
