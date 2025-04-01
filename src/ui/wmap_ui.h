#ifndef ARCANUM_UI_WMAP_UI_H_
#define ARCANUM_UI_WMAP_UI_H_

#include "game/context.h"
#include "game/timeevent.h"

bool wmap_ui_init(GameInitInfo* init_info);
void wmap_ui_exit();
void wmap_ui_reset();
bool wmap_ui_save(TigFile* stream);
bool wmap_ui_load(GameLoadInfo* load_info);
void sub_560720();
int sub_560740();
void sub_560750();
void wmap_ui_open();
void wmap_ui_select(int64_t obj, int spell);
void wmap_ui_close();
int wmap_ui_is_created();
void wmap_ui_scroll(int direction);
void sub_564000(int a1);
void sub_564AF0(int64_t a1);
bool wmap_ui_bkg_process_callback(TimeEvent* timeevent);
void sub_564F60(int64_t a1, int64_t a2);
void sub_566CC0(int64_t* location_ptr);

#endif /* ARCANUM_UI_WMAP_UI_H_ */
