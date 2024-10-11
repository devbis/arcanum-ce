#ifndef ARCANUM_UI_ANIM_UI_H_
#define ARCANUM_UI_ANIM_UI_H_

#include "game/context.h"

bool anim_ui_init(GameInitInfo* init_info);
void anim_ui_exit();
void anim_ui_reset();
bool anim_ui_save(TigFile* stream);
bool anim_ui_load(GameLoadInfo* load_info);
void sub_57D350(int a1, int a2);
void sub_57D370(int a1, int a2, int milliseconds);
void sub_57D3E0(int list, int a2);
void sub_57D620();
void sub_57D640();

#endif /* ARCANUM_UI_ANIM_UI_H_ */
