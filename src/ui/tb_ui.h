#ifndef ARCANUM_UI_TB_UI_H_
#define ARCANUM_UI_TB_UI_H_

#include "game/context.h"

bool tb_ui_init(GameInitInfo* init_info);
void tb_ui_reset();
void tb_ui_exit();
void sub_57CCF0(long long a1, long long a2);
bool sub_57CD60(long long a1, long long a2, char* buffer);

#endif /* ARCANUM_UI_TB_UI_H_ */
