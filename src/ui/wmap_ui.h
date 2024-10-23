#ifndef ARCANUM_UI_WMAP_UI_H_
#define ARCANUM_UI_WMAP_UI_H_

#include "game/context.h"
#include "game/timeevent.h"

bool wmap_ui_init(GameInitInfo* init_info);
void wmap_ui_exit();
void sub_560720();
int sub_560740();
void sub_560750();
void sub_560760();
void sub_560790(int64_t a1, int a2);
void sub_560F40();
int wmap_ui_is_created();
void sub_564000(int a1);
void sub_564AF0(int64_t a1);
bool wmap_ui_bkg_process_callback(TimeEvent* timeevent);
void sub_564F60(long long a1, long long a2);
void sub_566CC0(int64_t* location_ptr);

#endif /* ARCANUM_UI_WMAP_UI_H_ */
