#ifndef ARCANUM_UI_FOLLOWER_UI_H_
#define ARCANUM_UI_FOLLOWER_UI_H_

#include "game/context.h"
#include "game/target.h"

bool follower_ui_init(GameInitInfo* init_info);
void follower_ui_exit();
void follower_ui_reset();
void follower_ui_resize(GameResizeInfo* resize_info);
bool follower_ui_load(GameLoadInfo* load_info);
bool follower_ui_save(TigFile* stream);
void sub_56B180(S4F2810* a1);
void sub_56B280();
void sub_56B290();
void sub_56B4D0(int64_t obj);
void sub_56B6F0();

#endif /* ARCANUM_UI_FOLLOWER_UI_H_ */
