#ifndef ARCANUM_GAME_TC_H_
#define ARCANUM_GAME_TC_H_

#include "game/context.h"

bool tc_init(GameInitInfo* init_info);
void tc_exit();
void tc_resize(GameResizeInfo* resize_info);
void tc_draw(UnknownContext* render_info);
void tc_scroll(int dx, int dy);
void tc_show();
void tc_hide();
void tc_clear();
void tc_set_option(int index, const char* str);
int tc_handle_message(TigMessage* msg);
int tc_check_size(const char* str);

#endif /* ARCANUM_GAME_TC_H_ */
