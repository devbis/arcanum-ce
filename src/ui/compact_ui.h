#ifndef ARCANUM_UI_COMPACT_UI_H_
#define ARCANUM_UI_COMPACT_UI_H_

#include "game/context.h"

bool compact_ui_init(GameInitInfo* init_info);
void compact_ui_exit();
void compact_ui_reset();
bool compact_ui_create();
bool compact_ui_destroy();
void compact_ui_draw();
tig_window_handle_t compact_ui_message_window_acquire();
void compact_ui_message_window_box();
void compact_ui_message_window_hide();
void compact_ui_message_window_release();

#endif /* ARCANUM_UI_COMPACT_UI_H_ */
