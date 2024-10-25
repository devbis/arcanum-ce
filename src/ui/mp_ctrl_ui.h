#ifndef ARCANUM_UI_MP_CTRL_H_
#define ARCANUM_UI_MP_CTRL_H_

#include "game/context.h"
#include "game/timeevent.h"

bool mp_ctrl_ui_init(GameInitInfo* init_info);
void mp_ctrl_ui_exit();
void sub_569C70(int a1, int a2, const char* a3);
void sub_569DA0();
bool mp_ctrl_ui_process_callback(TimeEvent* timeevent);
bool sub_569F20(tig_window_handle_t window_handle);
void sub_569F30();

#endif /* ARCANUM_UI_MP_CTRL_H_ */
