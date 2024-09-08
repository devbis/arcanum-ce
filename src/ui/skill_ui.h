#ifndef ARCANUM_UI_SKILL_UI_H_
#define ARCANUM_UI_SKILL_UI_H_

#include "game/context.h"

bool skill_ui_init(GameInitInfo* init_info);
void skill_ui_reset();
void skill_ui_exit();
int sub_579F50(int index);
int sub_579F70(int index);
int sub_579F90();
void sub_57A1A0();
int sub_57A6A0(int index);
const char* sub_57A700(int index);
void sub_57AC50(int64_t obj, int skill, int a3);
void sub_57AC90(int64_t obj, int skill, int a3);
void sub_57ACD0(int64_t obj, int skill);
void sub_57AEB0(int64_t obj, int skill);

#endif /* ARCANUM_UI_SKILL_UI_H_ */
