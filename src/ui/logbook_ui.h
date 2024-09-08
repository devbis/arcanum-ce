#ifndef ARCANUM_UI_LOGBOOK_UI_H_
#define ARCANUM_UI_LOGBOOK_UI_H_

#include "game/context.h"

bool logbook_ui_init(GameInitInfo* init_info);
void logbook_ui_exit();
void logbook_ui_reset();
void sub_53F020(int64_t obj);
void sub_53F090();
bool logbook_ui_is_created();
void sub_5407F0();

#endif /* ARCANUM_UI_LOGBOOK_UI_H_ */
