#ifndef ARCANUM_UI_ITEM_UI_H_
#define ARCANUM_UI_ITEM_UI_H_

#include "game/context.h"
#include "game/target.h"

bool item_ui_init(GameInitInfo* init_info);
void item_ui_exit();
void item_ui_activate(int64_t owner_obj, int64_t item_obj);
void sub_571C80();
void sub_571CB0(S4F2810* a1);

#endif /* ARCANUM_UI_ITEM_UI_H_ */
