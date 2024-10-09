#ifndef ARCANUM_UI_DIALOG_UI_H_
#define ARCANUM_UI_DIALOG_UI_H_

#include "game/context.h"

bool dialog_ui_init(GameInitInfo* init_info);
void dialog_ui_exit();
void dialog_ui_reset();
bool sub_567400(long long obj);
void sub_567460(int64_t a1, int64_t a2, int a3, int a4, int a5);
void sub_5678D0(long long obj);
int sub_567A10();
void sub_567A20(long long obj);
void sub_567A60(long long obj);
void sub_567E00(int index, int a2);
void sub_5681C0(long long a1, long long a2);
void sub_568220(int a1, int a2, int a3, int a4, int a5, int a6);
void sub_568830(int64_t obj);
void sub_568880(long long obj, int a2, int a3, int a4, int a5, int a6, const char* str);

#endif /* ARCANUM_UI_DIALOG_UI_H_ */
