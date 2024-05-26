#ifndef ARCANUM_UI_COMPACT_UI_H_
#define ARCANUM_UI_COMPACT_UI_H_

bool compact_ui_init(GameInitInfo* init_info);
void compact_ui_exit();
void compact_ui_reset();
bool compact_ui_create();
bool compact_ui_destroy();
void compact_ui_draw();
tig_window_handle_t sub_568D20();
void sub_568E70();
void sub_568F20();
void sub_568F40();

#endif /* ARCANUM_UI_COMPACT_UI_H_ */
