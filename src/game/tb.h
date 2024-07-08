#ifndef ARCANUM_GAMELIB_TB_H_
#define ARCANUM_GAMELIB_TB_H_

#include "game/context.h"
#include "game/lib/object.h"

bool tb_init(GameContext* ctx);
void tb_reset();
void tb_exit();
void tb_resize(ResizeContext* ctx);
bool tb_update_view(ViewOptions* view_options);
void tb_close();
void tb_ping(unsigned int time);
void sub_4D5F10(UnknownContext* ctx);
void sub_4D5FE0(object_id_t object_id, int font, const char* text);
void sub_4D6160(object_id_t object_id, int a2);
void sub_4D6210(object_id_t object_id, long long location, int offset_x, int offset_y);
void sub_4D62B0(object_id_t object_id);
void sub_4D6320();

#endif /* ARCANUM_GAMELIB_TB_H_ */
