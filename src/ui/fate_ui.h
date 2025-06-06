#ifndef ARCANUM_UI_FATE_UI_H_
#define ARCANUM_UI_FATE_UI_H_

#include "game/context.h"

bool fate_ui_init(GameInitInfo* init_info);
void fate_ui_reset();
void fate_ui_exit();
void fate_ui_toggle(int64_t obj);
void fate_ui_close();

#endif /* ARCANUM_UI_FATE_UI_H_ */
