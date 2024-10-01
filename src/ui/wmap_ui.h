#ifndef ARCANUM_UI_WMAP_UI_H_
#define ARCANUM_UI_WMAP_UI_H_

#include "game/context.h"
#include "game/timeevent.h"

bool wmap_ui_init(GameInitInfo* init_info);
void wmap_ui_exit();
void sub_560F40();
int wmap_ui_is_created();
bool wmap_ui_bkg_process_callback(TimeEvent* timeevent);

#endif /* ARCANUM_UI_WMAP_UI_H_ */
