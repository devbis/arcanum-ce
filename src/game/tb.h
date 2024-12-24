#ifndef ARCANUM_GAME_TB_H_
#define ARCANUM_GAME_TB_H_

#include "game/context.h"
#include "game/obj.h"

typedef enum TbType {
    TB_TYPE_WHITE,
    TB_TYPE_RED,
    TB_TYPE_GREEN,
    TB_TYPE_BLUE,
    TB_TYPE_COUNT,
} TbType;

bool tb_init(GameInitInfo* init_info);
void tb_reset();
void tb_exit();
void tb_resize(ResizeInfo* resize_info);
bool tb_update_view(ViewOptions* view_options);
void tb_close();
void tb_toggle();
void tb_ping(unsigned int time);
void tb_render(UnknownContext* render_info);
void tb_add(int64_t obj, int type, const char* str);
void sub_4D6160(object_id_t object_id, int a2);
void tb_move(int64_t obj, int64_t loc, int offset_x, int offset_y);
void sub_4D62B0(object_id_t object_id);
void sub_4D6320();

#endif /* ARCANUM_GAME_TB_H_ */
